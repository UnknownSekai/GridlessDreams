"""asset-portion definitions.

package_assets merges all asset kinds per platform and splits each into two portions:
base_<platform>.zip + extra_<platform>.zip. The offline server mounts whichever ship, so
base alone is a small build that boots and plays through the tutorial. Archive paths are
"<kind>/<platform>/<rel>" (per-platform kinds) or "<kind>/<rel>" (shared).

base = every common/UI/structural group in full, PLUS the exact dependency closure of the
tutorial for the few giant per-id pools (song/episode audio, costumes, per-song animations).
The closure is read from the game's own Addressables catalog: the "Tutorial" label and the
tutorial live song resolve to the precise bundle set the client loads, so the 9.7 GB of other
songs' audio and 4.1 GB of other costumes stay in extra without breaking the tutorial.
"""

from __future__ import annotations

import re
from pathlib import Path

import _catalog

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

# groups too big to ship whole; base gets only their tutorial dependency closure (+ extras below).
# everything NOT listed here is included in base in full (common/UI/structural, all modest in size).
SPLIT_GROUPS = {
    "2d-assets": {
        "charactercards_assets_charactercard",  # 590 MB, per card
        "posters_assets_posters",               # 336 MB, per poster
    },
    "3d-assets": {
        "skins_assets_skins",                               # 4.1 GB, per costume
        "performanceanimations_assets_performanceanimations",  # 1.2 GB, per song
        "musicvideoanimations_assets_musicvideoanimations",    # 569 MB, per song MV
        "performanceassets_assets_performanceassets",          # 423 MB, per song
    },
    "cri-assets": {
        "cridata_remote_assets_criaddressables",  # 9.7 GB, per-id .acb/.usm audio+movie
    },
}

# seeds for the base closure, resolved against each catalog.
TUTORIAL_LABEL = "Tutorial"           # the game's own label for tutorial-required assets
TUTORIAL_MUSIC_IDS = (9999,)          # the tutorial live song (MusicMaster id 9999, name "TUTORIAL")
TUTORIAL_EPISODE_IDS = (1000000, 1000001)  # prologue scenario episodes

# home/member/story/shop display characters (db/default_account.json HomeDisplayPreference).
# their costume->skin mapping isn't 1:1, and they aren't under the Tutorial label, so keep all
# of these characters' costume skins in base so the home screen always has one to show.
HOME_DISPLAY_CHARS = (101, 201, 301, 401)


def _closure_keys(cat: _catalog.Catalog) -> list:
    keys = [TUTORIAL_LABEL]
    ids = tuple(str(i) for i in (TUTORIAL_MUSIC_IDS + TUTORIAL_EPISODE_IDS))
    tok = re.compile(r"(?:^|[/_])(%s)(?:[/_]|$)" % "|".join(map(re.escape, ids)))
    keys += [k for k in cat.string_keys() if tok.search(k)]
    return keys


def _skin_base_names(skins_dir: Path, closure: set[str]) -> set[str]:
    names = set(closure)
    if skins_dir.is_dir():
        pres = tuple(f"pc_{c}_" for c in HOME_DISPLAY_CHARS) + tuple(f"npc_{c}_" for c in HOME_DISPLAY_CHARS)
        for p in skins_dir.iterdir():
            if p.name.startswith(pres):
                names.add(p.name)
    return names


class Classifier:
    """decides base vs extra for one platform, using that platform's catalogs."""

    def __init__(self, assets_dir: Path, platform: str):
        self.platform = platform
        self.base_bundles: dict[str, set[str]] = {}
        for kind in PLATFORM_KINDS:
            cat_path = assets_dir / kind / platform / "catalog.json"
            if not cat_path.is_file():
                self.base_bundles[kind] = set()
                continue
            cat = _catalog.load(str(cat_path))
            closure = cat.bundles_for_keys(_closure_keys(cat))
            # restrict to this kind's split groups, then widen skins for the home characters
            names = {b for b in closure}
            if kind == "3d-assets":
                names = _skin_base_names(assets_dir / kind / platform / "skins_assets_skins", names)
            self.base_bundles[kind] = names

    def is_base(self, arcname: str) -> bool:
        parts = arcname.split("/")
        kind = parts[0]
        if kind in SHARED_KINDS:
            return True  # Notations + static-assets ship whole
        if kind not in PLATFORM_KINDS:
            return True
        # <kind>/<platform>/<rel...>
        rel = parts[2:]
        if len(rel) <= 1:
            return True  # catalog.json / catalog.hash directly under the platform dir
        group, filename = rel[0], rel[-1]
        split = SPLIT_GROUPS.get(kind, set())
        if group in split:
            return filename in self.base_bundles.get(kind, set())
        return True  # every other group ships whole
