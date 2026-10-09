"""asset-portion definitions (modelled on gridlesssekai6-source).

package_assets merges all asset kinds per platform and splits each pool by is_base into two
portions: base_<platform>.zip + extra_<platform>.zip. The offline server mounts whichever ship,
so base alone is a small bootable build. is_base is a heuristic -- tune BASE_GLOBS. Archive
paths are "<kind>/<platform>/<rel>" (per-platform kinds) or "<kind>/<rel>" (shared).
"""

from __future__ import annotations

import fnmatch

PORTIONS = ("base", "extra")
PLATFORMS = ("android", "ios")

# per-platform (<kind>/<platform>/...) vs shared across platforms (<kind>/...)
PLATFORM_KINDS = ("2d-assets", "3d-assets", "cri-assets")
SHARED_KINDS = (
    "Notations",
    # "scenes",  # not packaged: the server regenerates scene bins from _data/episodes
    "static-assets",
)
KINDS = PLATFORM_KINDS + SHARED_KINDS

# base (matched here) = bootable + one playable live; everything else is extra. fnmatch "*"
# spans "/". The cri song globs assume on-disk names keep the catalog cue naming
# (music_<id>.acb etc.) -- recheck once cri-assets is downloaded.
BASE_GLOBS = (
    "*/catalog.json",
    "*catalog*.hash",
    "*.hash",
    "2d-assets/*",
    "Notations/*",
    # "scenes/*",                 # dropped: scenes are regenerated from _data/episodes, not served from the portions
    "static-assets/*",            # event/gacha banner textures (~372 MB, served via static_content_url)
    "cri-assets/*music_*",        # song audio (not "musicvideo", which has no underscore)
    "cri-assets/*musicpreview_*",
    "cri-assets/*inst_*",
)


def is_base(archive_path: str) -> bool:
    return any(fnmatch.fnmatch(archive_path, g) for g in BASE_GLOBS)
