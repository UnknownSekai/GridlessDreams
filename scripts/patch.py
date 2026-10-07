"""
Simplified patcher for GridlessDreams.

Redirects the game to the local server, on BOTH Android (APK/XAPK) and iOS (IPA):
  1. ApplicationConfig._apiEndPoint (the first-hop API host) -> http://127.0.0.1:<port>.
     This value is NOT a global-metadata.dat string literal -- it is a serialized
     MonoBehaviour string field inside Data/sharedassets0.assets (4-byte LE length +
     UTF-8 + 4-byte align), so it is rewritten with UnityPy (which fixes up the asset
     file's object offsets when the string length changes). Same asset on both platforms.
     Every OTHER endpoint (CDN / realtime / ...) comes back from the local server's
     POST /api/Environment response, so nothing else needs static patching.
  2. SSL/verify + reachability + SetUrl static binary patches on the il2cpp binary (see below).

APK: patches sharedassets0.assets, injects .so, rebuilds, signs.
IPA: patches sharedassets0.assets, injects .dylib, repacks zip (sign separately).

Requires: pip install UnityPy lief requests

Usage:
    python patch.py input.apk  -o output.apk
    python patch.py input.xapk -o output.apk
    python patch.py input.ipa  -o output.ipa
"""

import argparse
import json
import os
import shutil
import struct
import sys
import zipfile
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_DIR = SCRIPT_DIR.parent
TEMP_DIR = PROJECT_DIR / ".temp"
TOOLS_DIR = TEMP_DIR / "tools"

LOCALHOST_PORT = 39046
# Modded-app package id (matches INSTALL.md) + display name.
PACKAGE_NAME = "com.utsk.gridlessdreams"
APP_NAME = "GridlessDreams"

# The first-hop API host (ApplicationConfig._apiEndPoint) -- the ONE value that redirects the
# client to the local server. Every other endpoint (CDN / realtime / ...) is handed back by the
# local server's POST /api/Environment response, so nothing else needs static patching. The host
# lives in Data/sharedassets0.assets (a serialized MonoBehaviour string), NOT in global-metadata.dat,
# so it is rewritten by patch_assets_endpoint() -- there is no global-metadata.dat patching here.
API_ENDPOINT_OLD = "https://lb-api.wds-stellarium.com"
API_ENDPOINT_NEW = f"http://127.0.0.1:{LOCALHOST_PORT}"

# APK TOOLS
APKTOOL_URL = "https://github.com/iBotPeaches/Apktool/releases/download/v2.12.0/apktool_2.12.0.jar"
APKEDITOR_URL = "https://github.com/REAndroid/APKEditor/releases/download/V1.4.5/APKEditor-1.4.5.jar"
UBER_SIGNER_URL = "https://github.com/patrickfav/uber-apk-signer/releases/download/v1.3.0/uber-apk-signer-1.3.0.jar"

KEYSTORE_PATH = Path(__file__).resolve().parent.parent / "Dreams.keystore"
KS_ALIAS = "Dreams"

# Keystore password (never commit it). Resolution order:
#   1. DREAMS_KS_PASS env var, if set (optional override)
#   2. otherwise secrets.json at the project root (gitignored), key "keystore_password"
SECRETS_PATH = PROJECT_DIR / "secrets.json"
KS_PASS = os.environ.get("DREAMS_KS_PASS") or ""
if not KS_PASS and SECRETS_PATH.exists():
    try:
        KS_PASS = json.loads(SECRETS_PATH.read_text(encoding="utf-8")).get(
            "keystore_password", ""
        )
    except Exception as _e:
        print(f"[patch] WARNING: could not read {SECRETS_PATH.name}: {_e}")

ASSETS_DIR = PROJECT_DIR / "assets"
DATA_DIR = PROJECT_DIR / "data"


# ========== UTILS ==========


def log(msg: str):
    print(f"[patch] {msg}", flush=True)


def download(url: str, dest: Path):
    if dest.exists():
        return
    import requests

    log(f"Downloading {dest.name}...")
    dest.parent.mkdir(parents=True, exist_ok=True)
    resp = requests.get(url, stream=True)
    resp.raise_for_status()
    tmp = dest.with_suffix(".tmp")
    with open(tmp, "wb") as f:
        for chunk in resp.iter_content(8192):
            f.write(chunk)
    tmp.rename(dest)


def run(cmd: str):
    log(f"$ {cmd}")
    rc = os.system(cmd)
    if rc != 0:
        raise RuntimeError(f"Command failed with exit code {rc}")


def bundle_data(dest_dir: Path):
    """Copy masterdata + abinfo into the app. Assets are NOT bundled (too large)."""
    dest_dir.mkdir(parents=True, exist_ok=True)

    for name in [
        "masterdata.json",
        "userData.json",
        "abinfo_ios.json",
        "abinfo_android.json",
    ]:
        src = DATA_DIR / name
        if src.exists():
            shutil.copy(src, dest_dir / name)
            log(f"Bundled {name}")
        else:
            log(f"WARNING: {name} not found at {src}")

    log(f"NOTE: Assets must be copied separately to the device.")
    log(
        f"  Android: copy assets/ folder to /sdcard/Android/data/<pkg>/files/gridlessdreams/assets/"
    )
    log(f"  iOS: copy assets/ folder to <App>/Documents/gridlessdreams/assets/")


# ========== API ENDPOINT PATCH (Data/sharedassets0.assets) ==========
# ApplicationConfig._apiEndPoint is a serialized MonoBehaviour string in sharedassets0.assets,
# stored as <4-byte LE length><UTF-8 bytes><pad to 4-byte align>. The same asset ships on both
# Android (assets/bin/Data/) and iOS (<App|UnityFramework.framework>/Data/). UnityPy rewrites the
# object and fixes up the asset file's object offset table so a different-length string stays valid.


def _rewrite_unity_string(raw: bytes, old: bytes, new: bytes) -> bytes:
    """Replace one Unity-serialized string field (length prefix + utf8 + align) in raw object bytes."""
    idx = raw.find(old)
    if idx < 0:
        raise ValueError(f"string {old!r} not found in object data")
    if raw.find(old, idx + 1) >= 0:
        raise ValueError(f"string {old!r} occurs more than once; cannot locate it safely")
    len_off = idx - 4
    if len_off < 0:
        raise ValueError("string has no room for its 4-byte length prefix")
    declared = struct.unpack_from("<I", raw, len_off)[0]
    if declared != len(old):
        raise ValueError(f"length-prefix mismatch: field says {declared}, string is {len(old)} bytes")
    old_total = 4 + ((len(old) + 3) & ~3)  # prefix + aligned old payload
    new_field = struct.pack("<I", len(new)) + new + b"\x00" * (((len(new) + 3) & ~3) - len(new))
    return raw[:len_off] + new_field + raw[len_off + old_total :]


SPLIT_CHUNK = 1024 * 1024


def _patch_single_assets(assets_path: Path, old_b: bytes, new_b: bytes):
    if old_b not in assets_path.read_bytes():
        log(
            f"WARNING: {old_b.decode()} not in {assets_path.name} -- API endpoint NOT patched "
            "(already patched, or a different game version)"
        )
        return False
    try:
        import UnityPy
    except ImportError as exc:
        raise SystemExit("UnityPy is required for the API-endpoint patch: pip install UnityPy") from exc

    env = UnityPy.load(str(assets_path))
    targets = [o for o in env.objects if old_b in o.get_raw_data()]
    if len(targets) != 1:
        raise RuntimeError(
            f"expected exactly one object containing {old_b.decode()}, found {len(targets)} in {assets_path.name}"
        )
    obj = targets[0]
    obj.set_raw_data(_rewrite_unity_string(obj.get_raw_data(), old_b, new_b))

    tmp = assets_path.parent / f".{assets_path.name}.patch_tmp"
    shutil.rmtree(tmp, ignore_errors=True)
    tmp.mkdir(parents=True)
    try:
        env.save(out_path=str(tmp))
        saved = tmp / assets_path.name
        if not saved.is_file():
            found = list(tmp.rglob(assets_path.name))
            if not found:
                raise RuntimeError(f"UnityPy did not emit {assets_path.name}")
            saved = found[0]
        shutil.move(str(saved), str(assets_path))
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

    blob = assets_path.read_bytes()
    if new_b not in blob or old_b in blob:
        raise RuntimeError("verification failed: API endpoint not rewritten cleanly")
    return True


def patch_assets_endpoint(
    data_dir: Path, old_url: str = API_ENDPOINT_OLD, new_url: str = API_ENDPOINT_NEW
):
    """Rewrite ApplicationConfig._apiEndPoint in sharedassets0.assets (single file, or Unity's
    .split0../.splitN chunked layout which is reassembled, patched, then re-split in place)."""
    old_b, new_b = old_url.encode("utf-8"), new_url.encode("utf-8")
    if old_b == new_b:
        log("API endpoint already equals the target -- nothing to patch")
        return

    single = data_dir / "sharedassets0.assets"
    if single.is_file():
        if _patch_single_assets(single, old_b, new_b):
            log(f"Patched API endpoint: {old_url} -> {new_url} ({single.name})")
        return

    # Numeric (not lexical) order: ...split2 before ...split10; the host string lives in the LAST chunk.
    splits = sorted(
        data_dir.glob("sharedassets0.assets.split*"),
        key=lambda p: int(p.name.rsplit(".split", 1)[1]),
    )
    if not splits:
        log(f"WARNING: sharedassets0.assets not found under {data_dir} -- API endpoint NOT patched")
        return

    joined = data_dir / "sharedassets0.assets"
    with open(joined, "wb") as out:
        for s in splits:
            out.write(s.read_bytes())
    log(f"Reassembled {len(splits)} sharedassets0.assets chunks ({joined.stat().st_size} B)")

    patched = _patch_single_assets(joined, old_b, new_b)
    if not patched:
        joined.unlink(missing_ok=True)
        return

    data = joined.read_bytes()
    n = max(1, -(-len(data) // SPLIT_CHUNK))
    for i in range(n):
        splits_i = data_dir / f"sharedassets0.assets.split{i}"
        splits_i.write_bytes(data[i * SPLIT_CHUNK : (i + 1) * SPLIT_CHUNK])
    for stale in data_dir.glob("sharedassets0.assets.split*"):
        if int(stale.name.rsplit(".split", 1)[1]) >= n:
            stale.unlink()
    joined.unlink(missing_ok=True)
    log(f"Patched API endpoint: {old_url} -> {new_url} (re-split into {n} chunks)")


# ========== SSL BYPASS (iOS static binary patch) ==========
# Unity uses UnityTLS (mbedTLS inside UnityFramework) which ignores NSC.
# We patch unitytls_tlsctx_get_peer_verify_result to always return 0 (success).
# Strategy:
#   1. Find "SKIP VERIFY flag set" string (unique to this function's log output)
#   2. Find a pointer to it in the binary's data section
#   3. Find ADRP+LDR/ADD that loads that pointer — locates us inside the function
#   4. Walk back to ARM64 prologue (STP X29, X30, [SP,...])
#   5. Overwrite first 8 bytes with MOV W0,#0 + RET


def _find_ssl_verify_fn(binary: bytes) -> int | None:
    needle = b"verify_result: SKIP VERIFY flag set\x00"
    str_off = binary.find(needle)
    if str_off == -1:
        return None

    # pointer to string stored as 8-byte LE value
    ptr_bytes = struct.pack("<Q", str_off)
    ptr_off = binary.find(ptr_bytes)
    if ptr_off == -1:
        # Mach-O: try with typical arm64 load address added
        for base in (0x100000000, 0):
            ptr_bytes = struct.pack("<Q", base + str_off)
            ptr_off = binary.find(ptr_bytes)
            if ptr_off != -1:
                break
        else:
            return None

    ptr_page = ptr_off & ~0xFFF
    ptr_poff = ptr_off & 0xFFF

    for i in range(0, len(binary) - 8, 4):
        instr = struct.unpack_from("<I", binary, i)[0]
        if (instr & 0x9F000000) != 0x90000000:
            continue
        immlo = (instr >> 29) & 0x3
        immhi = (instr >> 5) & 0x7FFFF
        imm21 = (immhi << 2) | immlo
        if imm21 & (1 << 20):
            imm21 -= 1 << 21
        page = i & ~0xFFF
        tp = (page + (imm21 << 12)) & 0xFFFFFFFFFFFFFFFF
        if tp != ptr_page:
            continue

        ni = struct.unpack_from("<I", binary, i + 4)[0]
        match = False
        if (ni & 0xFFC00000) == 0x91000000 and ((ni >> 10) & 0xFFF) == ptr_poff:
            match = True
        if (ni & 0xFFC00000) == 0xF9400000 and (((ni >> 10) & 0xFFF) << 3) == ptr_poff:
            match = True
        if not match:
            continue

        # walk back to prologue: STP X29, X30, [SP, #imm]
        for back in range(i, max(0, i - 4096), -4):
            pi = struct.unpack_from("<I", binary, back)[0]
            if (pi & 0xFF807FFF) == 0xA9007BFD:
                return back

    return None


def patch_ssl_bypass_ios(binary: bytearray) -> bool:
    off = _find_ssl_verify_fn(bytes(binary))
    if off is None:
        log("WARNING: could not find unitytls verify fn — SSL bypass NOT applied")
        return False
    # MOV W0, #0  = 52 80 00 00
    # RET         = C0 03 5F D6
    binary[off : off + 4] = b"\x00\x00\x80\x52"
    binary[off + 4 : off + 8] = b"\xc0\x03\x5f\xd6"
    log(f"SSL bypass patched at binary offset {hex(off)}")
    return True


# ========== IOS BINARY PATCHING (URL rewrite hooks) ==========
# Finds SetUrl icall wrapper via ADRP+ADD scan for the icall name string
# (plain C string, NOT obfuscated by BeeByte).
# Creates prologue branch -> trampoline in code cave -> data slot in __DATA.
# The dylib reads hooks.json at runtime and writes the hook function address to the data slot.


# ARM64 ENCODING


def _encode_adrp(rd: int, pc: int, target_page: int) -> int:
    page_diff = (target_page >> 12) - (pc >> 12)
    return (
        0x90000000
        | ((page_diff & 0x3) << 29)
        | (((page_diff >> 2) & 0x7FFFF) << 5)
        | rd
    )


def _encode_ldr_x_uimm(rt: int, rn: int, offset: int) -> int:
    return 0xF9400000 | (((offset >> 3) & 0xFFF) << 10) | (rn << 5) | rt


def _encode_br(rn: int) -> int:
    return 0xD61F0000 | (rn << 5)


def _encode_cbz(rt: int, pc: int, target: int) -> int:
    imm19 = ((target - pc) >> 2) & 0x7FFFF
    return 0xB4000000 | (imm19 << 5) | rt


def _encode_b(pc: int, target: int) -> int:
    return 0x14000000 | (((target - pc) >> 2) & 0x3FFFFFF)


def _encode_bl(pc: int, target: int) -> int:
    return 0x94000000 | (((target - pc) >> 2) & 0x3FFFFFF)


def _relocate_insn(insn: int, orig_pc: int, new_pc: int) -> int:
    op = (insn >> 26) & 0x3F
    if op == 0x05 or op == 0x25:
        imm26 = insn & 0x3FFFFFF
        if imm26 & 0x2000000:
            imm26 |= ~0x3FFFFFF
        target = orig_pc + imm26 * 4
        return _encode_b(new_pc, target) if op == 0x05 else _encode_bl(new_pc, target)
    if (insn & 0x9F000000) == 0x90000000:  # ADRP
        rd = insn & 0x1F
        immhi = (insn >> 5) & 0x7FFFF
        immlo = (insn >> 29) & 0x3
        imm = (immhi << 2) | immlo
        if imm & 0x100000:
            imm -= 0x200000
        target_page = ((orig_pc >> 12) + imm) << 12
        return _encode_adrp(rd, new_pc, target_page)
    return insn


def _build_prologue_branch(method_va: int, cave_va: int) -> bytes:
    return struct.pack("<I", _encode_b(method_va, cave_va))


def _build_trampoline(
    original_insn: bytes, method_va: int, cave_va: int, data_slot_va: int, reg: int = 16
) -> bytes:
    slot_page = data_slot_va & ~0xFFF
    slot_off = data_slot_va & 0xFFF
    adrp = _encode_adrp(reg, cave_va, slot_page)
    ldr = _encode_ldr_x_uimm(reg, reg, slot_off)
    cbz = _encode_cbz(reg, cave_va + 8, cave_va + 16)
    br = _encode_br(reg)
    orig = struct.unpack_from("<I", original_insn, 0)[0]
    relocated = _relocate_insn(orig, method_va, cave_va + 16)
    b_back = _encode_b(cave_va + 20, method_va + 4)
    return struct.pack("<IIIIII", adrp, ldr, cbz, br, relocated, b_back)


# MACH-O BINARY


class _Segment:
    __slots__ = ("name", "file_offset", "file_size", "vaddr", "vsize", "flags")

    def __init__(
        self,
        name: str,
        file_offset: int,
        file_size: int,
        vaddr: int,
        vsize: int,
        flags: int,
    ):
        self.name = name
        self.file_offset = file_offset
        self.file_size = file_size
        self.vaddr = vaddr
        self.vsize = vsize
        self.flags = flags

    @property
    def writable(self) -> bool:
        return bool(self.flags & 2)

    @property
    def executable(self) -> bool:
        return bool(self.flags & 1)


class _MachOBinary:
    _MH_MAGIC_64 = 0xFEEDFACF
    _FAT_MAGIC = 0xCAFEBABE
    _LC_SEGMENT_64 = 0x19

    def __init__(self, path: Path):
        self.data = bytearray(path.read_bytes())
        self.segments: list[_Segment] = []
        self._parse()

    def _parse(self):
        if struct.unpack_from(">I", self.data, 0)[0] == self._FAT_MAGIC:
            self._parse_fat()
        elif struct.unpack_from("<I", self.data, 0)[0] == self._MH_MAGIC_64:
            self._parse_macho64(0)
        else:
            raise ValueError(f"Not a Mach-O binary: {self.data[:4].hex()}")

    def _parse_fat(self):
        nfat = struct.unpack_from(">I", self.data, 4)[0]
        for i in range(nfat):
            off = 8 + i * 20
            cputype = struct.unpack_from(">I", self.data, off)[0]
            offset = struct.unpack_from(">I", self.data, off + 8)[0]
            if cputype == 0x0100000C:  # ARM64
                self._parse_macho64(offset)
                return
        raise ValueError("No arm64 slice in fat binary")

    def _parse_macho64(self, base: int):
        magic = struct.unpack_from("<I", self.data, base)[0]
        assert magic == self._MH_MAGIC_64
        ncmds = struct.unpack_from("<I", self.data, base + 16)[0]
        off = base + 32
        for _ in range(ncmds):
            cmd, cmdsize = struct.unpack_from("<II", self.data, off)
            if cmd == self._LC_SEGMENT_64:
                segname = (
                    self.data[off + 8 : off + 24].split(b"\x00")[0].decode("ascii")
                )
                vmaddr, vmsize, fileoff, filesize = struct.unpack_from(
                    "<QQQQ", self.data, off + 24
                )
                _, initprot = struct.unpack_from("<ii", self.data, off + 56)
                flags = 0
                if initprot & 1:
                    flags |= 4  # read
                if initprot & 2:
                    flags |= 2  # write
                if initprot & 4:
                    flags |= 1  # execute
                foff = (base + fileoff) if base else fileoff
                self.segments.append(
                    _Segment(segname, foff, filesize, vmaddr, vmsize, flags)
                )
            off += cmdsize

    def va_to_file(self, va: int) -> int:
        for s in self.segments:
            if s.vaddr <= va < s.vaddr + s.vsize:
                return va - s.vaddr + s.file_offset
        raise ValueError(f"VA 0x{va:x} not in any segment")

    def file_to_va(self, off: int) -> int:
        for s in self.segments:
            if s.file_offset <= off < s.file_offset + s.file_size:
                return off - s.file_offset + s.vaddr
        raise ValueError(f"File offset 0x{off:x} not in any segment")

    def read_u64(self, off: int) -> int:
        return struct.unpack_from("<Q", self.data, off)[0]

    def find_code_cave(
        self,
        near_va: int,
        size: int,
        max_distance: int = 0x8000000,
        exclude: set[int] | None = None,
    ) -> int:
        text = next(
            (s for s in self.segments if s.executable and s.file_size > 0x100000), None
        )
        if not text:
            return -1
        target_file = self.va_to_file(near_va)
        if exclude is None:
            exclude = set()
        NOP = b"\x1f\x20\x03\xd5"
        ZERO = b"\x00\x00\x00\x00"
        for dist in range(0, max_distance, 4):
            for direction in (1, -1):
                c = target_file + dist * direction
                if c < text.file_offset or c + size > text.file_offset + text.file_size:
                    continue
                if c % 4 != 0 or c in exclude:
                    continue
                ok = True
                for j in range(0, size + 4, 4):
                    pos = c + j
                    if pos + 4 > text.file_offset + text.file_size:
                        if j >= size:
                            break
                        ok = False
                        break
                    if self.data[pos : pos + 4] not in (NOP, ZERO):
                        ok = False
                        break
                if ok:
                    return c
        return -1

    def find_data_slot_region(self, n_slots: int) -> int:
        slot_size = n_slots * 8
        candidates = []
        for s in self.segments:
            if s.writable and s.file_size > 0:
                priority = (
                    0
                    if s.name == "__DATA"
                    else (1 if "DATA" in s.name and "CONST" not in s.name else 2)
                )
                candidates.append((priority, s))
        candidates.sort(key=lambda x: x[0])
        for _, s in candidates:
            end = s.file_offset + s.file_size
            for off in range(end - slot_size, s.file_offset, -8):
                if self.data[off : off + slot_size] == b"\x00" * slot_size:
                    return off
        raise RuntimeError(
            f"No {slot_size}-byte zero region in any writable DATA segment"
        )

    def save(self, path: Path):
        path.write_bytes(self.data)


# ELF64 BINARY (Android libil2cpp.so)


class _ELFBinary:
    _EI_MAGIC = b"\x7fELF"

    def __init__(self, path: Path):
        self.data = bytearray(path.read_bytes())
        self.segments: list[_Segment] = []
        self._parse()

    def _parse(self):
        assert self.data[:4] == self._EI_MAGIC, "Not an ELF binary"
        assert self.data[4] == 2, "Not ELF64"
        assert self.data[5] == 1, "Not little-endian"

        (e_phoff,) = struct.unpack_from("<Q", self.data, 32)
        (e_phentsize,) = struct.unpack_from("<H", self.data, 54)
        (e_phnum,) = struct.unpack_from("<H", self.data, 56)

        for i in range(e_phnum):
            off = e_phoff + i * e_phentsize
            (p_type,) = struct.unpack_from("<I", self.data, off)
            if p_type != 1:  # PT_LOAD
                continue
            (p_flags,) = struct.unpack_from("<I", self.data, off + 4)
            (p_offset,) = struct.unpack_from("<Q", self.data, off + 8)
            (p_vaddr,) = struct.unpack_from("<Q", self.data, off + 16)
            (p_filesz,) = struct.unpack_from("<Q", self.data, off + 32)
            (p_memsz,) = struct.unpack_from("<Q", self.data, off + 40)

            flags = 0
            if p_flags & 1:
                flags |= 1  # PF_X -> execute
            if p_flags & 2:
                flags |= 2  # PF_W -> write

            self.segments.append(
                _Segment(f"LOAD_{i}", p_offset, p_filesz, p_vaddr, p_memsz, flags)
            )

    def va_to_file(self, va: int) -> int:
        for s in self.segments:
            if s.vaddr <= va < s.vaddr + s.vsize:
                return va - s.vaddr + s.file_offset
        raise ValueError(f"VA 0x{va:x} not in any segment")

    def file_to_va(self, off: int) -> int:
        for s in self.segments:
            if s.file_offset <= off < s.file_offset + s.file_size:
                return off - s.file_offset + s.vaddr
        raise ValueError(f"File offset 0x{off:x} not in any segment")

    def find_code_cave(
        self,
        near_va: int,
        size: int,
        max_distance: int = 0x8000000,
        exclude: set[int] | None = None,
    ) -> int:
        text = next(
            (s for s in self.segments if s.executable and s.file_size > 0x100000), None
        )
        if not text:
            return -1
        target_file = self.va_to_file(near_va)
        if exclude is None:
            exclude = set()
        ZERO = b"\x00\x00\x00\x00"
        for dist in range(0, max_distance, 4):
            for direction in (1, -1):
                c = target_file + dist * direction
                if c < text.file_offset or c + size > text.file_offset + text.file_size:
                    continue
                if c % 4 != 0 or c in exclude:
                    continue
                ok = True
                for j in range(0, size + 4, 4):
                    pos = c + j
                    if pos + 4 > text.file_offset + text.file_size:
                        if j >= size:
                            break
                        ok = False
                        break
                    if self.data[pos : pos + 4] != ZERO:
                        ok = False
                        break
                if ok:
                    return c
        return -1

    def find_data_slot_region(self, n_slots: int) -> int:
        slot_size = n_slots * 8
        for s in self.segments:
            if s.writable and s.file_size > 0:
                end = s.file_offset + s.file_size
                for off in range(end - slot_size, s.file_offset, -8):
                    if self.data[off : off + slot_size] == b"\x00" * slot_size:
                        return off
        raise RuntimeError(f"No {slot_size}-byte zero region in any writable segment")

    def save(self, path: Path):
        path.write_bytes(self.data)


# ADRP+ADD SCAN FOR ICALL WRAPPERS

_ICALL_NAME = b"UnityEngine.Networking.UnityWebRequest::SetUrl(System.String)\x00"
_REACHABILITY_ICALL = b"UnityEngine.Application::get_internetReachability()\x00"


def _find_icall_via_adrp(
    binary: _MachOBinary | _ELFBinary, needle: bytes, label: str
) -> int | None:
    data = binary.data

    str_off = data.find(needle)
    if str_off < 0:
        log(f"{label} icall string not found in binary")
        return None

    str_va = binary.file_to_va(str_off)
    str_page = str_va & ~0xFFF
    str_page_off = str_va & 0xFFF
    log(
        f"{label} icall string at file:0x{str_off:x} VA:0x{str_va:x} (page+0x{str_page_off:x})"
    )

    add_expected = (0x91000000 | (str_page_off << 10)) & 0xFFFFFFFF
    add_mask = 0xFFFFFC00

    for seg in binary.segments:
        if not seg.executable:
            continue
        for off in range(seg.file_offset + 4, seg.file_offset + seg.file_size, 4):
            add_instr = struct.unpack_from("<I", data, off)[0]
            if (add_instr & add_mask) != add_expected:
                continue

            rn = (add_instr >> 5) & 0x1F
            adrp_off = off - 4
            adrp_instr = struct.unpack_from("<I", data, adrp_off)[0]

            if ((adrp_instr >> 24) & 0x9F) != 0x90:
                continue
            if (adrp_instr & 0x1F) != rn:
                continue

            immlo = (adrp_instr >> 29) & 0x3
            immhi = (adrp_instr >> 5) & 0x7FFFF
            imm = (immhi << 2) | immlo
            if imm & 0x100000:
                imm -= 0x200000

            adrp_va = binary.file_to_va(adrp_off)
            pc_page = adrp_va & ~0xFFF
            target_page = (pc_page + (imm << 12)) & 0xFFFFFFFFFFFFFFFF

            if target_page != str_page:
                continue

            log(f"  ADRP+ADD ref at file:0x{adrp_off:x} VA:0x{adrp_va:x}")

            # walk back to function prologue
            for back in range(4, 304, 4):
                prev_off = adrp_off - back
                if prev_off < seg.file_offset:
                    break
                prev = struct.unpack_from("<I", data, prev_off)[0]
                # STP Xn, Xm, [SP, #-imm]! (pre-index STP to SP)
                if (prev & 0xFFC003E0) == 0xA98003E0:
                    func_va = binary.file_to_va(prev_off)
                    log(f"  function prologue at file:0x{prev_off:x} VA:0x{func_va:x}")
                    return func_va
                # STR Xn, [SP, #-imm]! (pre-index STR to SP, stack alloc)
                if (
                    (prev & 0xFFE00C00) == 0xF8000C00
                    and ((prev >> 5) & 0x1F) == 31
                    and (prev & (1 << 20))
                ):
                    func_va = binary.file_to_va(prev_off)
                    log(f"  function prologue at file:0x{prev_off:x} VA:0x{func_va:x}")
                    return func_va
                # SUB SP, SP, #imm
                if (prev & 0xFF8003FF) == 0xD10003FF:
                    func_va = binary.file_to_va(prev_off)
                    log(f"  function prologue at file:0x{prev_off:x} VA:0x{func_va:x}")
                    return func_va
                # RET or BRK — different function
                if prev == 0xD65F03C0 or (prev & 0xFFE0001F) == 0xD4200000:
                    break

            log("  no prologue found, skipping")

    return None


def _patch_hook_trampoline(
    binary: _MachOBinary | _ELFBinary,
    binary_path: Path,
    method_va: int,
    name: str,
    exclude_caves: set[int] | None = None,
    exclude_slots: set[int] | None = None,
) -> dict | None:
    method_file = binary.va_to_file(method_va)
    original_insn = bytes(binary.data[method_file : method_file + 4])

    cave_file = binary.find_code_cave(method_va, 24, exclude=exclude_caves)
    if cave_file < 0:
        log(f"No code cave found near VA 0x{method_va:x} for {name}")
        return None

    # temporarily mark excluded slots so find_data_slot_region skips them
    restore: list[tuple[int, bytes]] = []
    if exclude_slots:
        for s in exclude_slots:
            orig = bytes(binary.data[s : s + 8])
            binary.data[s : s + 8] = b"\xff" * 8
            restore.append((s, orig))
    data_slot_file = binary.find_data_slot_region(1)
    for s, orig in restore:
        binary.data[s : s + 8] = orig
    cave_va = binary.file_to_va(cave_file)
    slot_va = binary.file_to_va(data_slot_file)

    log(f"Code cave at file:0x{cave_file:x} VA:0x{cave_va:x}")
    log(f"Data slot at file:0x{data_slot_file:x} VA:0x{slot_va:x}")

    prologue = _build_prologue_branch(method_va, cave_va)
    trampoline = _build_trampoline(original_insn, method_va, cave_va, slot_va)

    binary.data[method_file : method_file + 4] = prologue
    binary.data[cave_file : cave_file + 24] = trampoline

    log(f"Patched {name} wrapper at VA 0x{method_va:x}")
    return {
        "name": name,
        "method_rva": f"0x{method_va:x}",
        "data_slot_rva": f"0x{slot_va:x}",
        "trampoline_rva": f"0x{cave_va:x}",
        "orig_rva": f"0x{cave_va + 16:x}",
    }


# MAIN HOOK PATCHING


def _patch_reachability_inline(binary: _MachOBinary | _ELFBinary) -> bool:
    """Directly overwrite get_internetReachability wrapper with MOV W0, #2; RET.
    No trampoline/runtime hook needed — this is a static binary patch."""
    log("Scanning for Reachability icall wrapper via ADRP+ADD...")
    func_va = _find_icall_via_adrp(binary, _REACHABILITY_ICALL, "Reachability")
    if func_va is None:
        return False
    func_off = binary.va_to_file(func_va)
    # ARM64: MOV W0, #2  = 0x52800040
    #        RET          = 0xD65F03C0
    binary.data[func_off : func_off + 8] = struct.pack("<II", 0x52800040, 0xD65F03C0)
    log(f"Patched Reachability inline at VA 0x{func_va:x} (MOV W0, #2; RET)")
    return True


def _patch_all_hooks(
    binary: _MachOBinary | _ELFBinary, binary_path: Path
) -> dict | None:
    hooks_info: list[dict] = []
    used_caves: set[int] = set()
    used_slots: set[int] = set()

    # SetUrl: trampoline (needs runtime rewrite logic)
    log("Scanning for SetUrl icall wrapper via ADRP+ADD...")
    func_va = _find_icall_via_adrp(binary, _ICALL_NAME, "SetUrl")
    if func_va is not None:
        h = _patch_hook_trampoline(
            binary, binary_path, func_va, "SetUrl", used_caves, used_slots
        )
        if h:
            used_caves.add(binary.va_to_file(int(h["trampoline_rva"], 16)))
            used_slots.add(binary.va_to_file(int(h["data_slot_rva"], 16)))
            hooks_info.append(h)

    # Reachability: inline patch (just return 2, no runtime hook)
    _patch_reachability_inline(binary)

    if not hooks_info:
        return None

    binary.save(binary_path)
    return {"hooks": hooks_info}


def patch_ios_binary_hooks(binary_path: Path) -> dict | None:
    binary = _MachOBinary(binary_path)
    return _patch_all_hooks(binary, binary_path)


def patch_android_binary_hooks(binary_path: Path) -> dict | None:
    binary = _ELFBinary(binary_path)
    return _patch_all_hooks(binary, binary_path)


# ========== ANDROID ==========


def patch_android(input_path: Path, output_path: Path, inject_so: Path | None):
    TEMP_DIR.mkdir(parents=True, exist_ok=True)
    apk_dir = TEMP_DIR / "apk"
    shutil.rmtree(apk_dir, ignore_errors=True)
    apk_dir.mkdir()

    apktool_jar = TOOLS_DIR / "apktool.jar"
    signer_jar = TOOLS_DIR / "uber-apk-signer.jar"
    download(APKTOOL_URL, apktool_jar)
    download(UBER_SIGNER_URL, signer_jar)

    apk_path = apk_dir / "base.apk"
    if input_path.suffix in (".xapk", ".zip"):
        editor_jar = TOOLS_DIR / "apkeditor.jar"
        download(APKEDITOR_URL, editor_jar)
        split_dir = TEMP_DIR / "split"
        shutil.rmtree(split_dir, ignore_errors=True)
        with zipfile.ZipFile(input_path, "r") as zf:
            zf.extractall(split_dir)
        run(f'java -jar "{editor_jar}" m -i "{split_dir}" -o "{apk_path}"')
    else:
        shutil.copy(input_path, apk_path)

    sources = apk_dir / "sources"
    run(f'java -jar "{apktool_jar}" d -f -o "{sources}" --no-src "{apk_path}"')

    # FIND DATA DIR + BINARY
    data_dir = sources / "assets" / "bin" / "Data"
    il2cpp_path = sources / "lib" / "arm64-v8a" / "libil2cpp.so"

    if not data_dir.is_dir():
        raise FileNotFoundError(f"Unity Data dir not found at {data_dir}")
    if not il2cpp_path.exists():
        raise FileNotFoundError(f"libil2cpp.so not found at {il2cpp_path}")

    # RENAME PACKAGE + APP
    log("Renaming package and app...")
    manifest_path = sources / "AndroidManifest.xml"
    manifest = manifest_path.read_text(encoding="utf-8")
    # find original package name
    import re

    orig_pkg = re.search(r'package="([^"]+)"', manifest)
    if orig_pkg:
        orig_pkg = orig_pkg.group(1)
        manifest = manifest.replace(orig_pkg, PACKAGE_NAME)
        manifest_path.write_text(manifest, encoding="utf-8")
        log(f"Package: {orig_pkg} -> {PACKAGE_NAME}")

    # update apktool.yml
    apktool_yml = sources / "apktool.yml"
    if apktool_yml.exists():
        yml = apktool_yml.read_text(encoding="utf-8")
        yml = re.sub(
            r"renameManifestPackage:.*", f"renameManifestPackage: {PACKAGE_NAME}", yml
        )
        if "renameManifestPackage" not in yml:
            yml += f"\nrenameManifestPackage: {PACKAGE_NAME}\n"
        apktool_yml.write_text(yml, encoding="utf-8")

    # update app name in strings.xml
    strings_path = sources / "res" / "values" / "strings.xml"
    if strings_path.exists():
        strings = strings_path.read_text(encoding="utf-8")
        strings = re.sub(
            r'(<string name="app_name">)[^<]*(</string>)',
            rf"\g<1>{APP_NAME}\g<2>",
            strings,
        )
        strings_path.write_text(strings, encoding="utf-8")

    # REDIRECT THE FIRST-HOP API HOST (ApplicationConfig._apiEndPoint in Data/sharedassets0.assets).
    log("Patching API endpoint...")
    patch_assets_endpoint(data_dir)

    # INJECT .so
    if inject_so and inject_so.exists():
        log(f"Injecting {inject_so.name}...")
        lib_dir = sources / "lib" / "arm64-v8a"
        shutil.copy(inject_so, lib_dir / inject_so.name)

        import lief

        libmain = lib_dir / "libmain.so"
        binary = lief.ELF.parse(str(libmain))
        binary.add_library(inject_so.name)
        binary.write(str(libmain))
        log(f"Injected {inject_so.name} as DT_NEEDED in libmain.so")

    # URL REWRITE HOOKS (static binary patches on libil2cpp.so)
    log("Applying URL rewrite hooks to libil2cpp.so...")
    hooks_data = patch_android_binary_hooks(il2cpp_path)
    if hooks_data:
        hooks_dir = sources / "assets" / "gridlessdreams"
        hooks_dir.mkdir(parents=True, exist_ok=True)
        hooks_json_path = hooks_dir / "hooks.json"
        with open(hooks_json_path, "w") as f:
            json.dump(hooks_data, f)
        log(f"Wrote hooks config ({len(hooks_data['hooks'])} hooks)")
    else:
        log("WARNING: URL rewrite hooks could not be applied")

    # ALLOW CLEARTEXT HTTP TO LOCALHOST (Android 9+ blocks cleartext by default)
    nsc_dir = sources / "res" / "xml"
    nsc_dir.mkdir(parents=True, exist_ok=True)
    (nsc_dir / "network_security_config.xml").write_text(
        '<?xml version="1.0" encoding="utf-8"?>\n'
        "<network-security-config>\n"
        '    <base-config cleartextTrafficPermitted="true">\n'
        "        <trust-anchors>\n"
        '            <certificates src="system"/>\n'
        "        </trust-anchors>\n"
        "    </base-config>\n"
        "</network-security-config>\n"
    )
    manifest_path = sources / "AndroidManifest.xml"
    manifest = manifest_path.read_text(encoding="utf-8")
    if "networkSecurityConfig" not in manifest:
        manifest = manifest.replace(
            "<application ",
            '<application android:networkSecurityConfig="@xml/network_security_config" ',
        )
    if 'android:debuggable="true"' not in manifest:
        if 'android:debuggable="false"' in manifest:
            manifest = manifest.replace(
                'android:debuggable="false"', 'android:debuggable="true"'
            )
        elif "android:debuggable" not in manifest:
            manifest = manifest.replace(
                "<application ",
                '<application android:debuggable="true" ',
            )
    manifest_path.write_text(manifest, encoding="utf-8")
    log("Added network security config + debuggable flag")

    # BUNDLE DATA + ASSETS
    bundle_dest = sources / "assets" / "gridlessdreams"
    bundle_data(bundle_dest)

    # BUILD + SIGN
    log("Building APK...")
    unsigned = TEMP_DIR / "unsigned.apk"
    run(f'java -jar "{apktool_jar}" b "{sources}" -o "{unsigned}"')

    log("Signing APK...")
    if not KEYSTORE_PATH.exists():
        raise FileNotFoundError(f"Keystore not found at {KEYSTORE_PATH}")

    sign_dir = TEMP_DIR / "signed"
    shutil.rmtree(sign_dir, ignore_errors=True)
    run(
        f'java -jar "{signer_jar}" --apks "{unsigned}" -o "{sign_dir}" '
        f'-ks "{KEYSTORE_PATH}" -ksAlias {KS_ALIAS} -ksPass {KS_PASS} --ksKeyPass {KS_PASS}'
    )

    signed_apks = list(sign_dir.glob("*.apk"))
    if not signed_apks:
        raise RuntimeError("Signing failed - no output APK")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy(signed_apks[0], output_path)
    log(f"Output: {output_path}")


# ========== IOS ==========


def patch_ios(input_path: Path, output_path: Path, inject_dylib: Path | None):
    TEMP_DIR.mkdir(parents=True, exist_ok=True)
    ipa_dir = TEMP_DIR / "ipa"
    shutil.rmtree(ipa_dir, ignore_errors=True)

    log("Extracting IPA...")
    with zipfile.ZipFile(input_path, "r") as zf:
        zf.extractall(ipa_dir)

    # FIND APP BUNDLE
    payload = ipa_dir / "Payload"
    apps = list(payload.glob("*.app"))
    if not apps:
        raise FileNotFoundError("No .app found in IPA")
    app_path = apps[0]

    # FIND DATA DIR + BINARY (Unity may place Data/ + the binary under the framework or the .app root)
    fw_path = app_path / "Frameworks" / "UnityFramework.framework"
    data_dir = fw_path / "Data"
    binary_path = fw_path / "UnityFramework"

    if not data_dir.is_dir():
        data_dir = app_path / "Data"
    if not binary_path.exists():
        binary_path = app_path / app_path.stem

    if not data_dir.is_dir():
        raise FileNotFoundError("Unity Data dir not found")
    if not binary_path.exists():
        raise FileNotFoundError("Binary not found")

    # PATCH INFO.PLIST
    import plistlib

    plist_path = app_path / "Info.plist"
    with open(plist_path, "rb") as f:
        plist = plistlib.load(f)
    plist["UIFileSharingEnabled"] = True
    plist["LSSupportsOpeningDocumentsInPlace"] = True
    plist["NSAppTransportSecurity"] = {"NSAllowsArbitraryLoads": True}
    plist["CFBundleIdentifier"] = PACKAGE_NAME
    plist["CFBundleName"] = APP_NAME
    plist["CFBundleDisplayName"] = APP_NAME
    with open(plist_path, "wb") as f:
        plistlib.dump(plist, f)
    log(f"Patched Info.plist (package={PACKAGE_NAME}, name={APP_NAME})")

    # REDIRECT THE FIRST-HOP API HOST (ApplicationConfig._apiEndPoint in Data/sharedassets0.assets).
    log("Patching API endpoint...")
    patch_assets_endpoint(data_dir)

    # INJECT DYLIB
    if inject_dylib and inject_dylib.exists():
        log(f"Injecting {inject_dylib.name}...")
        frameworks = app_path / "Frameworks"
        frameworks.mkdir(exist_ok=True)
        shutil.copy(inject_dylib, frameworks / inject_dylib.name)

        import lief

        binary = lief.MachO.parse(str(binary_path))
        for fat_bin in binary:
            fat_bin.add(
                lief.MachO.DylibCommand.weak_lib(
                    f"@executable_path/Frameworks/{inject_dylib.name}"
                )
            )
            fat_bin.write(str(binary_path))
            break
        log(f"Injected {inject_dylib.name} as LC_LOAD_WEAK_DYLIB")

    # URL REWRITE HOOKS (static binary patches)
    # Must happen AFTER lief injection (lief may change binary layout)
    log("Applying URL rewrite hooks to binary...")
    hooks_data = patch_ios_binary_hooks(binary_path)
    if hooks_data:
        hooks_dir = app_path / "gridlessdreams"
        hooks_dir.mkdir(parents=True, exist_ok=True)
        hooks_json_path = hooks_dir / "hooks.json"
        with open(hooks_json_path, "w") as f:
            json.dump(hooks_data, f)
        log(f"Wrote hooks config ({len(hooks_data['hooks'])} hooks)")
    else:
        log("WARNING: URL rewrite hooks could not be applied")

    # BUNDLE DATA + ASSETS
    bundle_dest = app_path / "gridlessdreams"
    bundle_data(bundle_dest)

    # REPACK IPA
    log("Repacking IPA...")
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(output_path, "w", zipfile.ZIP_DEFLATED) as zf:
        for root, dirs, files in os.walk(ipa_dir):
            for file in files:
                full = Path(root) / file
                arcname = full.relative_to(ipa_dir)
                zf.write(full, arcname)
    log(f"Output: {output_path}")
    log("NOTE: IPA needs to be signed with a valid certificate before installing")


# ========== MAIN ==========


def main():
    parser = argparse.ArgumentParser(description="Patch a Unity game for GridlessDreams")
    parser.add_argument("input", help="Input APK/XAPK/IPA file")
    parser.add_argument("-o", "--output", required=True, help="Output file path")
    parser.add_argument(
        "--inject", help="Path to .so or .dylib to inject", default=None
    )
    args = parser.parse_args()

    input_path = Path(args.input)
    output_path = Path(args.output)

    if not input_path.exists():
        print(f"Input file not found: {input_path}")
        sys.exit(1)

    inject = Path(args.inject) if args.inject else None
    ext = input_path.suffix.lower()

    if ext in (".apk", ".xapk"):
        patch_android(input_path, output_path, inject)
    elif ext == ".ipa":
        patch_ios(input_path, output_path, inject)
    else:
        print(f"Unsupported file type: {ext}")
        sys.exit(1)

    # CLEANUP
    shutil.rmtree(TEMP_DIR, ignore_errors=True)
    log("Done!")


if __name__ == "__main__":
    main()
