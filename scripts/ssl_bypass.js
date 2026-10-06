// ssl_bypass.js — find SetUrl icall wrapper via ADRP+ADD scan
// no il2cpp exports needed (works on iOS with everything stripped)
// fallback to il2cpp_resolve_icall on android emulators

const TAG = "[GridlessDreams]";
function log(msg) { console.log(`${TAG} ${msg}`); }

const isIOS = Process.platform === "darwin";
const moduleName = isIOS ? "UnityFramework" : "libil2cpp.so";
const mod = Process.findModuleByName(moduleName);
const base = mod.base;
log(`${moduleName} @ ${base} (${(mod.size / 1048576).toFixed(1)} MB)`);

function readStr(p) {
    if (p.isNull()) return null;
    try {
        const len = p.add(0x10).readS32();
        if (len <= 0 || len > 4096) return null;
        return p.add(0x14).readUtf16String(len);
    } catch (_) { return null; }
}

function rewriteUrl(p) {
    const len = p.add(0x10).readS32();
    const chars = p.add(0x14);
    for (let i = 4; i < len - 1; i++)
        chars.add(i * 2).writeU16(chars.add((i + 1) * 2).readU16());
    chars.add((len - 1) * 2).writeU16(0);
    p.add(0x10).writeS32(len - 1);
}

let count = 0;
function installHook(addr, label) {
    Interceptor.attach(addr, {
        onEnter(args) {
            try {
                const url = readStr(args[1]);
                if (url && url.startsWith("https://")) {
                    count++;
                    rewriteUrl(args[1]);
                    log(`[#${count}] ${url.substring(0, 80)} -> http`);
                }
            } catch (_) {}
        }
    });
    log(`hooked ${label} @ ${addr}`);
}

// STRATEGY 1: ADRP+ADD scan for icall string reference
// icall name strings are plain C strings (not obfuscated by BeeByte)
const ICALL = "UnityEngine.Networking.UnityWebRequest::SetUrl(System.String)";
const icallHex = [];
for (let i = 0; i < ICALL.length; i++) icallHex.push(ICALL.charCodeAt(i).toString(16).padStart(2, '0'));
icallHex.push('00');
const icallPattern = icallHex.join(' ');

log("scanning for icall string...");
let strAddr = null;
for (const prot of ["r--", "r-x"]) {
    if (strAddr) break;
    for (const r of mod.enumerateRanges(prot)) {
        try {
            const hits = Memory.scanSync(r.base, r.size, icallPattern);
            if (hits.length > 0) { strAddr = hits[0].address; break; }
        } catch (_) {}
    }
}

let hooked = false;

if (strAddr) {
    const strPage = strAddr.and(ptr("0xFFFFFFFFFFFFF000"));
    const strPageOff = strAddr.and(ptr("0xFFF")).toInt32();
    log(`icall string @ ${strAddr} (page+0x${strPageOff.toString(16)})`);

    // ADD Xn, Xn, #pageOff → top 20 bits are fixed
    const addExpected = (0x91000000 | (strPageOff << 10)) >>> 0;
    const addMask = 0xFFFFFC00;

    log("scanning code for ADRP+ADD...");
    const codeRanges = mod.enumerateRanges("r-x");

    for (const r of codeRanges) {
        if (hooked) break;
        for (let off = 4; off < r.size; off += 4) {
            try {
                const addInstr = r.base.add(off).readU32();
                if ((addInstr & addMask) !== addExpected) continue;

                const rn = (addInstr >> 5) & 0x1f;
                const adrpAddr = r.base.add(off - 4);
                const adrpInstr = adrpAddr.readU32();

                // ADRP: bit31=1, bits28-24=10000 → (instr>>24)&0x9f == 0x90
                if (((adrpInstr >> 24) & 0x9f) !== 0x90) continue;
                if ((adrpInstr & 0x1f) !== rn) continue;

                // decode ADRP page delta
                const immlo = (adrpInstr >> 29) & 0x3;
                const immhi = (adrpInstr >> 5) & 0x7ffff;
                let imm = (immhi << 2) | immlo;
                if (imm & 0x100000) imm -= 0x200000;

                const pcPage = adrpAddr.and(ptr("0xFFFFFFFFFFFFF000"));
                const targetPage = pcPage.add(imm << 12);

                if (!targetPage.equals(strPage)) continue;

                log(`  ADRP+ADD ref @ ${adrpAddr}`);

                // walk back to find function prologue:
                // STP pre-index to SP: (instr & 0xFFC003E0) == 0xA98003E0
                let funcStart = null;
                for (let back = 4; back <= 200; back += 4) {
                    const prev = adrpAddr.sub(back).readU32();
                    // STP Xn, Xm, [SP, #-imm]! (pre-index store pair to stack)
                    if ((prev & 0xFFC003E0) === 0xA98003E0) {
                        funcStart = adrpAddr.sub(back);
                        break;
                    }
                    // SUB SP, SP, #imm
                    if ((prev & 0xFF8003FF) === 0xD10003FF) {
                        funcStart = adrpAddr.sub(back);
                        break;
                    }
                    // hit a RET or BRK — stop (we're in a different function)
                    if (prev === 0xd65f03c0 || (prev & 0xFFE0001F) === 0xd4200000) break;
                }

                if (funcStart) {
                    log(`  function prologue @ ${funcStart} (ADRP at +${adrpAddr.sub(funcStart).toInt32()})`);
                    try {
                        installHook(funcStart, "SetUrl_wrapper");
                        hooked = true;
                        break;
                    } catch (e) { log(`  hook failed: ${e}`); }
                } else {
                    log(`  no prologue found (resolve stub), skipping`);
                }
            } catch (_) {}
        }
    }
}

// STRATEGY 2: il2cpp_resolve_icall fallback (android emulator etc)
if (!hooked) {
    log("trying il2cpp_resolve_icall fallback...");
    const resolve_icall = Module.findExportByName(moduleName, "il2cpp_resolve_icall");
    if (resolve_icall) {
        const resolve = new NativeFunction(resolve_icall, "pointer", ["pointer"]);
        const fn = resolve(Memory.allocUtf8String(
            "UnityEngine.Networking.UnityWebRequest::SetUrl(System.String)"));
        if (!fn.isNull()) {
            installHook(fn, "native_SetUrl");
            hooked = true;
        }
    }
}

log(hooked ? "ready — rewriting https -> http" : "ERROR: no hooks installed");
