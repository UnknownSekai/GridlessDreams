"""minimal Unity Addressables ContentCatalogData reader.

just enough of the catalog format to answer "which bundle files does this address or
label pull", so package_assets can put exactly the tutorial's dependency closure into the
base portion. the heavy pools (song/episode audio, costumes, per-song animations) are keyed
per id, so the closure is the only way to split them without shipping everything.
"""
from __future__ import annotations

import base64
import json
import struct
from dataclasses import dataclass
from typing import Iterable


def _read_key(buf: bytes, off: int):
    t = buf[off]
    off += 1
    if t == 0:  # ascii
        n = struct.unpack_from("<i", buf, off)[0]
        return buf[off + 4:off + 4 + n].decode("ascii", "replace")
    if t == 1:  # utf-16
        n = struct.unpack_from("<i", buf, off)[0]
        return buf[off + 4:off + 4 + n].decode("utf-16-le", "replace")
    if t == 2:
        return struct.unpack_from("<H", buf, off)[0]
    if t == 3:
        return struct.unpack_from("<I", buf, off)[0]
    if t == 4:
        return struct.unpack_from("<i", buf, off)[0]
    return ("?", t)


@dataclass
class _Entry:
    internal_id: str
    dep_key: int


class Catalog:
    def __init__(self, d: dict):
        iids = d["m_InternalIds"]
        bucket_raw = base64.b64decode(d["m_BucketDataString"])
        key_raw = base64.b64decode(d["m_KeyDataString"])
        entry_raw = base64.b64decode(d["m_EntryDataString"])

        n = struct.unpack_from("<i", bucket_raw, 0)[0]
        off = 4
        self.buckets: list[tuple[int, list[int]]] = []
        for _ in range(n):
            key_off, ecount = struct.unpack_from("<ii", bucket_raw, off)
            off += 8
            ents = list(struct.unpack_from("<%di" % ecount, bucket_raw, off))
            off += 4 * ecount
            self.buckets.append((key_off, ents))

        self.keys = [_read_key(key_raw, ko) for ko, _ in self.buckets]
        self.key_to_bucket: dict = {}
        for i, k in enumerate(self.keys):
            self.key_to_bucket.setdefault(k, i)

        ecount = struct.unpack_from("<i", entry_raw, 0)[0]
        self.entries: list[_Entry] = []
        for i in range(ecount):
            iid, _prov, dep, _deph, _dat, _pk, _rt = struct.unpack_from("<7i", entry_raw, 4 + i * 28)
            self.entries.append(_Entry(iids[iid], dep))

    @staticmethod
    def bundle_name(internal_id: str) -> str:
        s = internal_id.split("#", 1)[1] if "#" in internal_id else internal_id
        return s.rsplit("/", 1)[-1]

    def _deps(self, entry_idx: int) -> list[int]:
        b = self.entries[entry_idx].dep_key
        if 0 <= b < len(self.buckets):
            return self.buckets[b][1]
        return []

    def bundles_for_keys(self, keys: Iterable) -> set[str]:
        """bundle filenames pulled by the given addresses/labels (each entry + its deps)"""
        out: set[str] = set()
        for k in keys:
            bi = self.key_to_bucket.get(k)
            if bi is None:
                continue
            for e in self.buckets[bi][1]:
                for idx in [e] + self._deps(e):
                    iid = self.entries[idx].internal_id
                    if iid.endswith(".bundle"):
                        out.add(self.bundle_name(iid))
        return out

    def string_keys(self) -> list[str]:
        return [k for k in self.keys if isinstance(k, str)]


def load(path: str) -> Catalog:
    with open(path, encoding="utf-8") as f:
        return Catalog(json.load(f))
