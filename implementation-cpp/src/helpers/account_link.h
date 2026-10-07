#pragma once
#include <optional>
#include <string>
#include <utility>

#include "wire.h"

// Account link / takeover: register password + linkage code, confirmation
// code, and resolve linkage code + password to an account + login token.
// Ports helpers/account_link.py. Results flow as wire::json keyed by field
// name (TakeOverCodeResult / TimedConfirmationCode / TakeOverAccountResult).

namespace account_link {

// the confirmation code is a 6 digit code that stays valid for about ten minutes
constexpr long long CONFIRMATION_TTL_SECONDS = 600;

// Store the takeover password and return (TakeOverCodeResult, row id), or
// (empty result, nullopt) when unauthenticated.
std::pair<wire::json, std::optional<long long>> register_take_over_password(
    std::optional<long long> user_id, const std::string& password);

// Issue (or re-issue) the short-lived confirmation code for support.
wire::json get_confirmation_code(std::optional<long long> user_id);

// Resolve linkage_code + password to the account and a fresh login token;
// a wrong password yields the same empty result as an unknown code.
wire::json get_take_over_account(const std::string& linkage_code,
                                 const std::string& password);

}  // namespace account_link
