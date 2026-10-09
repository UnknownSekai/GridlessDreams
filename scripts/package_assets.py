"""package the downloaded game assets into mountable portion zips for the offline build.

Run ``server-of-dreams``'s ``scripts.download_all_assets`` first to populate ``_data/assets``,
then:

    python scripts/package_assets.py --dry-run            # classify + show sizes, write nothing
    python scripts/package_assets.py                      # build base_/extra_ zips for both platforms
    python scripts/package_assets.py --platform android   # one platform
    python scripts/package_assets.py --assets-dir "C:/.../server-of-dreams/_data/assets"

See _portions for the base/extra split. Entries are stored (bundles are already compressed) at
"<kind>/<platform>/<rel>" (per-platform) or "<kind>/<rel>" (shared) -- what the server resolves
/production/<kind>/<platform>/<version>/<filepath> against.
"""

from __future__ import annotations

import argparse
import sys
import zipfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import _portions  # noqa: E402

REPO_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_ASSETS = REPO_ROOT / "implementation-python" / "_data" / "assets"
DEFAULT_OUT = REPO_ROOT / "dist" / "assets"


class Entry:
    __slots__ = ("src", "arcname", "size")

    def __init__(self, src: Path, arcname: str, size: int):
        self.src = src
        self.arcname = arcname
        self.size = size


def _iter_kind(assets_dir: Path, kind: str, platform: str) -> list[Entry]:
    """files for `kind` in `platform`'s pool, as (source, archive path) entries"""
    if kind in _portions.PLATFORM_KINDS:
        root = assets_dir / kind / platform
        prefix = f"{kind}/{platform}"
    else:  # shared across platforms
        root = assets_dir / kind
        prefix = kind
    if not root.is_dir():
        return []
    out: list[Entry] = []
    for p in root.rglob("*"):
        if p.is_file():
            rel = p.relative_to(root).as_posix()
            out.append(Entry(p, f"{prefix}/{rel}", p.stat().st_size))
    return out


def _plan(assets_dir: Path, platform: str) -> dict[str, list[Entry]]:
    """{'base': [...], 'extra': [...]} for one platform's merged pool"""
    buckets: dict[str, list[Entry]] = {"base": [], "extra": []}
    for kind in _portions.KINDS:
        for e in _iter_kind(assets_dir, kind, platform):
            buckets["base" if _portions.is_base(e.arcname) else "extra"].append(e)
    return buckets


def _write_zip(entries: list[Entry], out_path: Path) -> None:
    out_path.parent.mkdir(parents=True, exist_ok=True)
    total = sum(e.size for e in entries)
    done = 0
    tmp = out_path.with_suffix(out_path.suffix + ".part")
    with zipfile.ZipFile(tmp, "w", zipfile.ZIP_STORED, allowZip64=True) as zf:
        for i, e in enumerate(entries):
            zf.write(e.src, e.arcname)
            done += e.size
            if i % 200 == 0 or done == total:
                pct = f" ({done * 100 // total}%)" if total else ""
                print(f"\r  {out_path.name}: {i + 1}/{len(entries)} files, {done / 1e9:.2f} GB{pct}",
                      end="", flush=True)
    print()
    tmp.replace(out_path)  # atomic: a killed run never leaves a half zip looking complete


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--assets-dir", type=Path, default=DEFAULT_ASSETS,
                    help="_data/assets populated by download_all_assets")
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT, help="output dir for portion zips")
    ap.add_argument("--platform", choices=_portions.PLATFORMS, help="limit to one platform")
    ap.add_argument("--dry-run", action="store_true", help="classify + show sizes, write nothing")
    args = ap.parse_args()

    if not args.assets_dir.is_dir():
        print(f"assets dir not found: {args.assets_dir}\nrun server-of-dreams scripts.download_all_assets first")
        sys.exit(1)

    platforms = [args.platform] if args.platform else list(_portions.PLATFORMS)
    for platform in platforms:
        buckets = _plan(args.assets_dir, platform)
        print(f"\n[{platform}]")
        for portion in _portions.PORTIONS:
            entries = buckets[portion]
            size = sum(e.size for e in entries)
            print(f"  {portion:4}: {len(entries):>7} files, {size / 1e9:7.2f} GB -> {portion}_{platform}.zip")
        if args.dry_run:
            continue
        for portion in _portions.PORTIONS:
            if buckets[portion]:
                _write_zip(buckets[portion], args.out / f"{portion}_{platform}.zip")

    if args.dry_run:
        print("\n(dry run -- nothing written)")
    else:
        print(f"\ndone -> {args.out}")


if __name__ == "__main__":
    main()
