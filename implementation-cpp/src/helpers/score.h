#pragma once
#include "wire.h"

// score-block hash verification for Lives/FinishAndValidate. ports helpers/score.py.
// each block's hash is a running cumulative sum chained from 0 (independently per block-type
// list): a submission whose chain does not reproduce the reported hashes is a tampered score.
//
// payload is a FinishLivePayload wire::json whose *_score_blocks are null or arrays of blocks;
// a block is an object keyed by the entity field names (score, life, combo, hash, ...).

namespace score {

// true if every score-block hash chain in the FinishLive payload is self-consistent
bool verify_score_blocks(const wire::json& payload);

}  // namespace score
