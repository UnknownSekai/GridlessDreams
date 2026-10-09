#pragma once
#include <map>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "generated/enums_generated.h"
#include "wire.h"

// effect engine: collect a party slot's effects from masterdata and apply them.
// ports helpers/effects.py. effects flow as AppliedEffect{master, level, source};
// master is a masterdata row as wire::json keyed by field name, source is an
// EffectSourceTypes. party/actor composition is a wire::json dict; enums are
// long long (enums::<Enum>::<Member>). mirrors the Python 1:1.

namespace effects {

using json = wire::json;

// one collected effect: its master, the level used to pick the detail value, and
// its origin
struct AppliedEffect {
    json master;
    long long level;
    long long source = enums::EffectSourceTypes::Other;
};

// the sets and counts ValidateTargetCondition matches against. members: dicts with
// character_master_id/character_base_master_id/company/attribute/sense_type/poster_id.
json party_composition(const std::vector<json>& members);

// [AppliedEffect] for one slot from its resolvable sources, keeping only effects
// whose target conditions are met by the party composition comp
std::vector<AppliedEffect> collect_slot_effects(
    const std::optional<json>& sense_master,
    const std::optional<json>& star_act_master,
    long long sense_level,
    const std::optional<json>& accessory,
    const std::optional<json>& poster = std::nullopt,
    const std::optional<json>& comp = std::nullopt,
    long long bonus_flags = 0,
    bool is_leader = false);

// filter effects whose target conditions match the individual actor
std::vector<AppliedEffect> filter_target_effects(
    const std::vector<AppliedEffect>& effects, const json& actor_dict);

// (self_or_none effects, all-range effects)
std::pair<std::vector<AppliedEffect>, std::vector<AppliedEffect>> range_split(
    const std::vector<AppliedEffect>& effects);

// party-wide effects unlocked by the user's album level
std::vector<AppliedEffect> album_effects(
    long long album_level, const std::optional<json>& comp = std::nullopt);

// character talent-bloom effects whose phase <= the character's bloom stage
std::vector<AppliedEffect> bloom_effects(
    long long bloom_group_master_id, long long bloom_stage,
    const std::optional<json>& comp = std::nullopt);

// circle-support effects for an actor of company up to the user's circle level
std::vector<AppliedEffect> circle_effects(
    long long company, const std::map<long long, long long>& circle_levels,
    const std::optional<json>& comp = std::nullopt);

// effects granted to member_cm by leader_cm's LeaderSenseMaster
std::vector<AppliedEffect> leader_sense_effects(
    const std::optional<json>& leader_cm, const std::optional<json>& member_cm);

// GetEffectValue: sum of an effect type's detail values, optionally filtered by
// calculation_type and source_type
double sum_by_type_and_source(
    const std::vector<AppliedEffect>& effects, long long effect_type,
    std::optional<long long> calc_type = std::nullopt,
    std::optional<long long> source_type = std::nullopt);

double sum_by_type(const std::vector<AppliedEffect>& effects, long long effect_type,
                   std::optional<long long> calc_type = std::nullopt);

// per-component flat Base<Component>Up sums -> (vocal, expression, concentration)
std::tuple<long long, long long, long long> base_stat_bonus(
    const std::vector<AppliedEffect>& effects);

// per-component flat <Component>Up sums -> (vocal, expression, concentration)
std::tuple<long long, long long, long long> component_flat_bonus(
    const std::vector<AppliedEffect>& effects);

// LiveUnit.MaxPrincipal: base 1000 raised by passive PrincipalGaugeLimitUp effects
long long max_principal(const std::vector<AppliedEffect>& effects);

// BaseCorrection percent points on the character base
double base_correction(const std::vector<AppliedEffect>& effects);

// per-source component percentage bonuses in basis units, capped in source priority
// order -> {source_type: (vocal_bps, expression_bps, concentration_bps)}
std::map<long long, std::tuple<long long, long long, long long>>
component_percent_bonuses_by_source(const std::vector<AppliedEffect>& effects);

// per-source performance percentage bonuses in basis units, capped in source
// priority order -> {source_type: perf_bps}
std::map<long long, long long> performance_percent_bonuses_by_source(
    const std::vector<AppliedEffect>& effects);

// multiplication factor from CalculationTypes.Multiplication
double final_performance_multiplier(const std::vector<AppliedEffect>& effects);

// extra lights (SenseLightTypes) granted to a sense by passive AddSenseLight* effects
std::vector<long long> added_lights(const std::vector<AppliedEffect>& effects,
                                    long long sense_type);

// lights granted at live start by StartLive AddSenseLight* effects
std::vector<long long> start_lights(const std::vector<AppliedEffect>& effects,
                                    long long sense_type = 0);

// light count reductions for star act from DecreaseRequire*Light effects
std::map<long long, long long> decrease_require_lights(
    const std::vector<AppliedEffect>& effects);

// StartLive effects as LiveUnit.StartEffects Effect entities (wire::json)
std::vector<json> start_effects(const std::vector<AppliedEffect>& effects,
                                long long actor_id, long long start_order = 0);

}  // namespace effects
