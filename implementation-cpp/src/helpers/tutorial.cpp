#include "helpers/tutorial.h"

#include <algorithm>
#include <iterator>

namespace tutorial {

namespace {
long long order_index(long long status) {
    return static_cast<long long>(std::distance(
        TUTORIAL_ORDER.begin(),
        std::find(TUTORIAL_ORDER.begin(), TUTORIAL_ORDER.end(), status)));
}
}  // namespace

bool can_advance(long long current, long long target) {
    return order_index(target) >= order_index(current);
}

}  // namespace tutorial
