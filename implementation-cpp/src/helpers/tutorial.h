#pragma once
#include <array>

#include "generated/enums_generated.h"

// The tutorial progression. A client may only advance through these steps, never
// backtrack -- Player_UpdateTutorial rejects a target that precedes the current step.
// Ports helpers/tutorial.py.

namespace tutorial {

// expected order of tutorial steps
constexpr std::array<long long, 8> TUTORIAL_ORDER = {
    enums::TutorialStatus::Start,
    enums::TutorialStatus::TutorialDownLoad,
    enums::TutorialStatus::MainScenario,
    enums::TutorialStatus::TheaterMovie,
    enums::TutorialStatus::Home,
    enums::TutorialStatus::InGame,
    enums::TutorialStatus::MiniTalk,
    enums::TutorialStatus::Finish,
};

// true if target is at or ahead of current in the tutorial order
bool can_advance(long long current, long long target);

}  // namespace tutorial
