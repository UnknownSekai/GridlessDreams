"""apply the zip64 central-dir fix to the vendored miniz (external/ is gitignored + re-downloaded
by setup-deps, so the fix can't be committed into miniz.c itself). idempotent: safe to re-run.

miniz 3.0.2's mz_zip_reader_read_central_dir sanity-checks each entry's local-header offset with
the raw 32-bit field. python's zipfile (ZIP64_LIMIT = 2**31-1) stores 0xFFFFFFFF there once an
offset passes 2 GiB and puts the real offset in a zip64 extra field, so the check sees 0xFFFFFFFF,
computes 0xFFFFFFFF + size > archive_size and rejects the whole archive (error 9). extraction
already reads the real 64-bit offset via file_stat, so skipping the check for zip64 placeholder
entries is enough. without this, base_*.zip past 2 GiB and every extra_*.zip (>4 GiB) fail to mount.
"""

from __future__ import annotations

import sys
from pathlib import Path

MINIZ_C = Path(__file__).resolve().parent / "external" / "miniz" / "miniz.c"

ORIGINAL = """            if (comp_size != MZ_UINT32_MAX)
            {
                if (((mz_uint64)MZ_READ_LE32(p + MZ_ZIP_CDH_LOCAL_HEADER_OFS) + MZ_ZIP_LOCAL_DIR_HEADER_SIZE + comp_size) > pZip->m_archive_size)
                    return mz_zip_set_error(pZip, MZ_ZIP_INVALID_HEADER_OR_CORRUPTED);
            }"""

PATCHED = """            // zip64 entries store 0xFFFFFFFF here with the real offset in the extra field
            // (file_stat reads it back for extraction); skip the 32-bit range check for them
            if ((comp_size != MZ_UINT32_MAX) && (local_header_ofs != MZ_UINT32_MAX))
            {
                if ((local_header_ofs + MZ_ZIP_LOCAL_DIR_HEADER_SIZE + comp_size) > pZip->m_archive_size)
                    return mz_zip_set_error(pZip, MZ_ZIP_INVALID_HEADER_OR_CORRUPTED);
            }"""


def main() -> int:
    if not MINIZ_C.is_file():
        print(f"[patch_miniz] {MINIZ_C} not found (run setup-deps first)")
        return 1
    src = MINIZ_C.read_text(encoding="utf-8", errors="surrogateescape")
    if PATCHED in src:
        print("[patch_miniz] already patched")
        return 0
    if ORIGINAL not in src:
        print("[patch_miniz] WARNING: expected miniz 3.0.2 central-dir block not found; not patched")
        return 1
    MINIZ_C.write_text(src.replace(ORIGINAL, PATCHED, 1), encoding="utf-8", errors="surrogateescape")
    print("[patch_miniz] applied zip64 central-dir fix")
    return 0


if __name__ == "__main__":
    sys.exit(main())
