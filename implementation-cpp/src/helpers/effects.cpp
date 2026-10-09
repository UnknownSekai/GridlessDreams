#include "effects.h"

#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>

#include "master_data.h"

namespace effects {
namespace {

// array field or empty if absent/null (python `x or []`)
const json& opt_array(const json& obj, const char* key) {
    static const json kEmpty = json::array();
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null() || !it->is_array()) return kEmpty;
    return *it;
}

long long jint(const json& obj, const char* key, long long def) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->get<long long>();
}

double jdouble(const json& obj, const char* key, double def) {
    auto it = obj.find(key);
    if (it == obj.end() || it->is_null()) return def;
    return it->get<double>();
}

// lazy master indexes keyed by id_ (built once; master_data::load() runs at startup)
const json* em_of(long long id) {
    static const std::unordered_map<long long, const json*> idx = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& e : master_data::table("EffectMaster")) m[jint(e, "id_", 0)] = &e;
        return m;
    }();
    auto it = idx.find(id);
    return it == idx.end() ? nullptr : it->second;
}

const json* aem_of(long long id) {
    static const std::unordered_map<long long, const json*> idx = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& e : master_data::table("AccessoryEffectMaster")) m[jint(e, "id_", 0)] = &e;
        return m;
    }();
    auto it = idx.find(id);
    return it == idx.end() ? nullptr : it->second;
}

const json* am_of(long long id) {
    static const std::unordered_map<long long, const json*> idx = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& a : master_data::table("AccessoryMaster")) m[jint(a, "id_", 0)] = &a;
        return m;
    }();
    auto it = idx.find(id);
    return it == idx.end() ? nullptr : it->second;
}

const json* lsm_of(long long id) {
    static const std::unordered_map<long long, const json*> idx = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& l : master_data::table("LeaderSenseMaster")) m[jint(l, "id_", 0)] = &l;
        return m;
    }();
    auto it = idx.find(id);
    return it == idx.end() ? nullptr : it->second;
}

// pick the detail at the highest level <= source level (fallback: first detail)
double detail_value(const json& em, long long level) {
    const json& details = opt_array(em, "details");
    const json* best = nullptr;
    long long best_level = 0;
    for (const json& d : details) {
        long long dl = jint(d, "level", 0);
        if (dl <= level && (best == nullptr || dl > best_level)) {
            best = &d;
            best_level = dl;
        }
    }
    if (best == nullptr && !details.empty()) best = &details[0];
    return best != nullptr ? jdouble(*best, "value", 0.0) : 0.0;
}

json set_to_array(const std::set<long long>& s) {
    json a = json::array();
    for (long long v : s) a.push_back(v);
    return a;
}

bool in_set(const json& arr, const json& val) {
    for (const json& e : arr)
        if (e == val) return true;
    return false;
}

// EffectConditions -> the party-composition set it checks. NeighborPosition(7) and
// CharacterBaseGroup(8) need per-position/group data and are left un-gated (allowed).
const char* cond_key(long long cond) {
    switch (cond) {
        case 1: return "char_bases";
        case 2: return "companies";
        case 3: return "attributes";
        case 4: return "sense_types";
        case 5: return "characters";
        case 6: return "posters";
        default: return nullptr;
    }
}

long long counter_get(const json& counts, long long key) {
    auto it = counts.find(std::to_string(key));
    return it == counts.end() ? 0 : it->get<long long>();
}

long long counter_max(const json& counts) {
    long long mx = 0;
    bool first = true;
    for (auto it = counts.begin(); it != counts.end(); ++it) {
        long long v = it->get<long long>();
        if (first || v > mx) {
            mx = v;
            first = false;
        }
    }
    return mx;
}

bool conditions_met(const json& em, const std::optional<json>& comp) {
    if (!comp.has_value()) return true;
    const json& c = *comp;
    for (const json& cond : opt_array(em, "conditions")) {
        const char* key = cond_key(jint(cond, "condition", enums::EffectConditions::CharacterBase));
        if (key == nullptr) continue;  // NeighborPosition / CharacterBaseGroup -> allow
        auto vit = cond.find("value");
        json cval = (vit != cond.end()) ? *vit : json(nullptr);
        if (!in_set(c.at(key), cval)) return false;
    }
    for (const json& tr : opt_array(em, "triggers")) {
        long long t = jint(tr, "trigger", enums::TriggerType::OverLife);
        auto vit = tr.find("value");
        json valj = (vit != tr.end()) ? *vit : json(nullptr);
        long long vali = valj.is_number() ? valj.get<long long>() : 0;
        if (t == enums::TriggerType::AllMemberBelongingCompany) {
            if (counter_get(c.at("company_counts"), vali) != jint(c, "member_count", 0)) return false;
        } else if (t == enums::TriggerType::MaxMemberBelongingCompanyCount) {
            if (counter_max(c.at("company_counts")) < vali) return false;
        } else if (t == enums::TriggerType::MaxMemberBelongingAttributeCount) {
            if (counter_max(c.at("attribute_counts")) < vali) return false;
        } else if (t == enums::TriggerType::Company) {
            if (!in_set(c.at("companies"), valj)) return false;
        } else if (t == enums::TriggerType::Attribute) {
            if (!in_set(c.at("attributes"), valj)) return false;
        } else if (t == enums::TriggerType::CharacterBase) {
            if (!in_set(c.at("char_bases"), valj)) return false;
        } else if (t == enums::TriggerType::SenseType) {
            if (!in_set(c.at("sense_types"), json(vali))) return false;
        }
    }
    return true;
}

const std::vector<const json*>& poster_abilities(long long poster_master_id) {
    static const std::unordered_map<long long, std::vector<const json*>> idx = [] {
        std::unordered_map<long long, std::vector<const json*>> m;
        for (const json& pa : master_data::table("PosterAbilityMaster"))
            m[jint(pa, "poster_master_id", 0)].push_back(&pa);
        return m;
    }();
    static const std::vector<const json*> kEmpty;
    auto it = idx.find(poster_master_id);
    return it == idx.end() ? kEmpty : it->second;
}

const json* bloom_group(long long id) {
    static const std::unordered_map<long long, const json*> idx = [] {
        std::unordered_map<long long, const json*> m;
        for (const json& g : master_data::table("CharacterBloomBonusGroupMaster")) m[jint(g, "id_", 0)] = &g;
        return m;
    }();
    auto it = idx.find(id);
    return it == idx.end() ? nullptr : it->second;
}

const std::vector<const json*>& circle_by_company(long long company) {
    static const std::unordered_map<long long, std::vector<const json*>> idx = [] {
        std::unordered_map<long long, std::vector<const json*>> m;
        for (const json& cd : master_data::table("CircleSupportCompanyLevelDetailMaster"))
            m[jint(cd, "company", 0)].push_back(&cd);
        return m;
    }();
    static const std::vector<const json*> kEmpty;
    auto it = idx.find(company);
    return it == idx.end() ? kEmpty : it->second;
}

// EffectConditions (1..5) -> the actor attribute that must equal the condition value
const char* actor_cond_key(long long cond) {
    switch (cond) {
        case 1: return "character_base_master_id";
        case 2: return "company";
        case 3: return "attribute";
        case 4: return "sense_type";
        case 5: return "character_master_id";
        default: return nullptr;
    }
}

// a LeaderSense detail applies when it has no conditions, or when any one condition has all of its
// listed categories on the member (a condition listing no category always matches)
bool leader_condition_met(const json& detail, const std::set<long long>& member_cats) {
    const json& conds = opt_array(detail, "conditions");
    if (conds.empty()) return true;
    for (const json& cond : conds) {
        bool all_in = true;
        for (int i = 1; i <= 5; ++i) {
            auto it = cond.find("category_master_id" + std::to_string(i));
            if (it == cond.end() || it->is_null()) continue;
            if (member_cats.find(it->get<long long>()) == member_cats.end()) {
                all_in = false;
                break;
            }
        }
        if (all_in) return true;
    }
    return false;
}

std::optional<long long> light_of(long long effect_type, long long sense_type) {
    if (effect_type == enums::EffectTypes::AddSenseLightSelf) {
        if (sense_type >= enums::SenseLightTypes::Variable && sense_type <= enums::SenseLightTypes::Special)
            return sense_type;
        return enums::SenseLightTypes::Variable;
    }
    if (enums::EffectTypes::AddSenseLightVariable <= effect_type &&
        effect_type <= enums::EffectTypes::AddSenseLightSpecial) {
        return effect_type - enums::EffectTypes::AddSenseLightVariable;
    }
    return std::nullopt;
}

std::optional<long long> decrease_light_type(long long t) {
    switch (t) {
        case enums::EffectTypes::DecreaseRequireSupportLight: return enums::SenseLightTypes::Support;
        case enums::EffectTypes::DecreaseRequireControlLight: return enums::SenseLightTypes::Control;
        case enums::EffectTypes::DecreaseRequireAmplificationLight: return enums::SenseLightTypes::Amplification;
        case enums::EffectTypes::DecreaseRequireSpecialLight: return enums::SenseLightTypes::Special;
        default: return std::nullopt;
    }
}

constexpr long long kBaseMaxPrincipal = 1000;  // base (= TutorialMaxPrincipal), raised by PrincipalGaugeLimitUp
constexpr long long kStatusPercentLimit = 20000;
constexpr long long kPerformancePercentLimit = 20000;

// source priority order for applying caps and rounding
constexpr long long SOURCE_PRIORITY[] = {
    enums::EffectSourceTypes::Album,     enums::EffectSourceTypes::Poster,
    enums::EffectSourceTypes::Accessory, enums::EffectSourceTypes::BloomBonus,
    enums::EffectSourceTypes::Other,     enums::EffectSourceTypes::LeaderSense,
};

std::tuple<long long, long long, long long> fixed_sums(const std::vector<AppliedEffect>& effects,
                                                       long long t0, long long t1, long long t2) {
    return {static_cast<long long>(sum_by_type(effects, t0, enums::CalculationTypes::FixedAddition)),
            static_cast<long long>(sum_by_type(effects, t1, enums::CalculationTypes::FixedAddition)),
            static_cast<long long>(sum_by_type(effects, t2, enums::CalculationTypes::FixedAddition))};
}

}  // namespace

json party_composition(const std::vector<json>& members) {
    std::set<long long> char_bases, companies, attributes, characters, sense_types, posters;
    json company_counts = json::object();
    json attribute_counts = json::object();
    for (const json& m : members) {
        long long company = jint(m, "company", 0);
        long long attribute = jint(m, "attribute", 0);
        char_bases.insert(jint(m, "character_base_master_id", 0));
        companies.insert(company);
        attributes.insert(attribute);
        characters.insert(jint(m, "character_master_id", 0));
        sense_types.insert(jint(m, "sense_type", 0));
        long long poster_id = jint(m, "poster_id", 0);
        if (poster_id) posters.insert(poster_id);
        std::string ck = std::to_string(company);
        company_counts[ck] = (company_counts.contains(ck) ? company_counts[ck].get<long long>() : 0) + 1;
        std::string ak = std::to_string(attribute);
        attribute_counts[ak] = (attribute_counts.contains(ak) ? attribute_counts[ak].get<long long>() : 0) + 1;
    }
    json out = json::object();
    out["char_bases"] = set_to_array(char_bases);
    out["companies"] = set_to_array(companies);
    out["attributes"] = set_to_array(attributes);
    out["characters"] = set_to_array(characters);
    out["sense_types"] = set_to_array(sense_types);
    out["posters"] = set_to_array(posters);
    out["company_counts"] = company_counts;
    out["attribute_counts"] = attribute_counts;
    out["member_count"] = static_cast<long long>(members.size());
    return out;
}

std::vector<AppliedEffect> collect_slot_effects(const std::optional<json>& sense_master,
                                                const std::optional<json>& star_act_master,
                                                long long sense_level, const std::optional<json>& accessory,
                                                const std::optional<json>& poster,
                                                const std::optional<json>& comp, long long bonus_flags,
                                                bool is_leader) {
    std::vector<AppliedEffect> raw;
    if (sense_master.has_value()) {
        for (const json& eo : opt_array(*sense_master, "pre_effects")) {
            const json* em = em_of(jint(eo, "effect_master_id", 0));
            if (em != nullptr) raw.push_back({*em, sense_level, enums::EffectSourceTypes::Other});
        }
        for (const json& br : opt_array(*sense_master, "branches")) {
            for (const json& eo : opt_array(br, "branch_effects")) {
                const json* em = em_of(jint(eo, "effect_master_id", 0));
                if (em != nullptr) raw.push_back({*em, sense_level, enums::EffectSourceTypes::Other});
            }
        }
    }
    if (star_act_master.has_value()) {
        for (const json& eo : opt_array(*star_act_master, "pre_effects")) {
            const json* em = em_of(jint(eo, "effect_master_id", 0));
            if (em != nullptr) raw.push_back({*em, sense_level, enums::EffectSourceTypes::Other});
        }
    }
    if (accessory.has_value()) {
        const json* am = am_of(jint(*accessory, "accessoryMasterId", 0));
        if (am != nullptr) {
            for (const json& faeid : opt_array(*am, "fixed_accessory_effects")) {
                const json* ae = aem_of(faeid.get<long long>());
                const json* em = ae != nullptr ? em_of(jint(*ae, "effect_master_id", 0)) : nullptr;
                if (em != nullptr)
                    raw.push_back({*em, jint(*accessory, "level", 0), enums::EffectSourceTypes::Accessory});
            }
        }
        for (const json& aeid : opt_array(*accessory, "accessoryEffects")) {
            const json* ae = aem_of(aeid.get<long long>());
            const json* em = ae != nullptr ? em_of(jint(*ae, "effect_master_id", 0)) : nullptr;
            if (em != nullptr)
                raw.push_back({*em, jint(*accessory, "level", 0), enums::EffectSourceTypes::Accessory});
        }
    }
    if (poster.has_value()) {
        // poster abilities unlocked at the poster's level and enabled by the slot's
        // BonusAbilityEnableFlags (bit frame_number-1). bonus_flags==0 -> all enabled.
        long long p_lvl = jint(*poster, "level", 0) + jint(*poster, "breakthroughPhase", 0);
        for (const json* pa : poster_abilities(jint(*poster, "posterMasterId", 0))) {
            if (jint(*pa, "type", enums::PosterEffectTypes::Leader) == enums::PosterEffectTypes::Leader &&
                !is_leader)
                continue;
            if (jint(*poster, "level", 0) < jint(*pa, "release_level_at", 0)) continue;
            if (bonus_flags && !((bonus_flags >> (jint(*pa, "frame_number", 0) - 1)) & 1LL)) continue;
            for (const json& br : opt_array(*pa, "branches")) {
                for (const json& eo : opt_array(br, "branch_effects")) {
                    const json* em = em_of(jint(eo, "effect_master_id", 0));
                    if (em != nullptr) raw.push_back({*em, p_lvl, enums::EffectSourceTypes::Poster});
                }
            }
        }
    }
    std::vector<AppliedEffect> out;
    for (const AppliedEffect& e : raw)
        if (conditions_met(e.master, comp)) out.push_back(e);
    return out;
}

std::vector<AppliedEffect> filter_target_effects(const std::vector<AppliedEffect>& effects,
                                                 const json& actor_dict) {
    std::vector<AppliedEffect> out;
    for (const AppliedEffect& e : effects) {
        bool ok = true;
        for (const json& cond : opt_array(e.master, "conditions")) {
            const char* key = actor_cond_key(jint(cond, "condition", enums::EffectConditions::CharacterBase));
            if (key == nullptr) continue;
            auto ait = actor_dict.find(key);
            json actor_val = (ait != actor_dict.end()) ? *ait : json(nullptr);
            auto vit = cond.find("value");
            json cval = (vit != cond.end()) ? *vit : json(nullptr);
            if (actor_val != cval) {
                ok = false;
                break;
            }
        }
        if (ok) out.push_back(e);
    }
    return out;
}

std::pair<std::vector<AppliedEffect>, std::vector<AppliedEffect>> range_split(
    const std::vector<AppliedEffect>& effects) {
    std::vector<AppliedEffect> own, every;
    for (const AppliedEffect& e : effects) {
        if (jint(e.master, "range", enums::EffectTargetRanges::None_) != enums::EffectTargetRanges::All)
            own.push_back(e);
        else
            every.push_back(e);
    }
    return {own, every};
}

std::vector<AppliedEffect> album_effects(long long album_level, const std::optional<json>& comp) {
    std::vector<AppliedEffect> out;
    for (const json& ae : master_data::table("AlbumEffectMaster")) {
        if (jint(ae, "level", 0) <= album_level) {
            const json* em = em_of(jint(ae, "effect_master_id", 0));
            if (em != nullptr) out.push_back({*em, album_level, enums::EffectSourceTypes::Album});
        }
    }
    std::vector<AppliedEffect> res;
    for (const AppliedEffect& e : out)
        if (conditions_met(e.master, comp)) res.push_back(e);
    return res;
}

std::vector<AppliedEffect> bloom_effects(long long bloom_group_master_id, long long bloom_stage,
                                         const std::optional<json>& comp) {
    const json* g = bloom_group(bloom_group_master_id);
    if (g == nullptr) return {};
    std::vector<AppliedEffect> out;
    for (const json& bb : opt_array(*g, "bloom_bonuses")) {
        if (jint(bb, "phase", 0) <= bloom_stage) {
            const json* em = em_of(jint(bb, "effect_master_id", 0));
            if (em != nullptr) out.push_back({*em, bloom_stage, enums::EffectSourceTypes::BloomBonus});
        }
    }
    std::vector<AppliedEffect> res;
    for (const AppliedEffect& e : out)
        if (conditions_met(e.master, comp)) res.push_back(e);
    return res;
}

std::vector<AppliedEffect> circle_effects(long long company,
                                          const std::map<long long, long long>& circle_levels,
                                          const std::optional<json>& comp) {
    auto lit = circle_levels.find(company);
    long long level = (lit != circle_levels.end()) ? lit->second : 0;
    std::vector<AppliedEffect> out;
    for (const json* cd : circle_by_company(company)) {
        if (jint(*cd, "level", 0) <= level) {
            const json* em = em_of(jint(*cd, "effect_master_id", 0));
            // no dedicated EffectSourceTypes for circle support; capped as Other
            if (em != nullptr) out.push_back({*em, jint(*cd, "level", 0), enums::EffectSourceTypes::Other});
        }
    }
    std::vector<AppliedEffect> res;
    for (const AppliedEffect& e : out)
        if (conditions_met(e.master, comp)) res.push_back(e);
    return res;
}

std::vector<AppliedEffect> leader_sense_effects(const std::optional<json>& leader_cm,
                                                const std::optional<json>& member_cm) {
    if (!leader_cm.has_value() || jint(*leader_cm, "leader_sense_master_id", 0) == 0 ||
        !member_cm.has_value())
        return {};
    const json* lsm = lsm_of(jint(*leader_cm, "leader_sense_master_id", 0));
    if (lsm == nullptr || opt_array(*lsm, "details").empty()) return {};
    std::set<long long> member_cats;
    for (const json& cat : opt_array(*member_cm, "categories"))
        member_cats.insert(jint(cat, "category_master_id", 0));
    std::vector<AppliedEffect> out;
    for (const json& detail : opt_array(*lsm, "details")) {
        const json* em = em_of(jint(detail, "effect_master_id", 0));
        if (em != nullptr && leader_condition_met(detail, member_cats))
            out.push_back({*em, 1, enums::EffectSourceTypes::LeaderSense});
    }
    return out;
}

double sum_by_type_and_source(const std::vector<AppliedEffect>& effects, long long effect_type,
                              std::optional<long long> calc_type, std::optional<long long> source_type) {
    double total = 0.0;
    for (const AppliedEffect& e : effects) {
        if (jint(e.master, "type", enums::EffectTypes::BaseVocalUp) != effect_type) continue;
        if (calc_type.has_value() &&
            jint(e.master, "calculation_type", enums::CalculationTypes::PercentageAddition) != *calc_type)
            continue;
        if (source_type.has_value() && e.source != *source_type) continue;
        total += detail_value(e.master, e.level);
    }
    return total;
}

double sum_by_type(const std::vector<AppliedEffect>& effects, long long effect_type,
                   std::optional<long long> calc_type) {
    return sum_by_type_and_source(effects, effect_type, calc_type);
}

std::tuple<long long, long long, long long> base_stat_bonus(const std::vector<AppliedEffect>& effects) {
    return fixed_sums(effects, enums::EffectTypes::BaseVocalUp, enums::EffectTypes::BaseExpressionUp,
                      enums::EffectTypes::BaseConcentrationUp);
}

std::tuple<long long, long long, long long> component_flat_bonus(const std::vector<AppliedEffect>& effects) {
    return fixed_sums(effects, enums::EffectTypes::VocalUp, enums::EffectTypes::ExpressionUp,
                      enums::EffectTypes::ConcentrationUp);
}

long long max_principal(const std::vector<AppliedEffect>& effects) {
    double flat = 0.0;
    double pct = 0.0;
    for (const AppliedEffect& e : effects) {
        if (jint(e.master, "type", enums::EffectTypes::BaseVocalUp) != enums::EffectTypes::PrincipalGaugeLimitUp)
            continue;
        if (jint(e.master, "fire_timing_type", enums::FireTimingTypes::StarAct) !=
            enums::FireTimingTypes::Passive)
            continue;
        double v = detail_value(e.master, e.level);
        long long calc = jint(e.master, "calculation_type", enums::CalculationTypes::PercentageAddition);
        if (calc == enums::CalculationTypes::FixedAddition)
            flat += v;
        else if (calc == enums::CalculationTypes::PercentageAddition)  // /10000
            pct += v;
    }
    return static_cast<long long>(kBaseMaxPrincipal * (1 + pct / 10000) + flat);
}

double base_correction(const std::vector<AppliedEffect>& effects) {
    return sum_by_type(effects, enums::EffectTypes::BaseCorrection, enums::CalculationTypes::PercentageAddition) /
           100.0;
}

std::map<long long, std::tuple<long long, long long, long long>> component_percent_bonuses_by_source(
    const std::vector<AppliedEffect>& effects) {
    double v_cap = kStatusPercentLimit +
                   sum_by_type(effects, enums::EffectTypes::VocalLimitUp, enums::CalculationTypes::PercentageAddition);
    double e_cap =
        kStatusPercentLimit +
        sum_by_type(effects, enums::EffectTypes::ExpressionLimitUp, enums::CalculationTypes::PercentageAddition);
    double c_cap =
        kStatusPercentLimit +
        sum_by_type(effects, enums::EffectTypes::ConcentrationLimitUp, enums::CalculationTypes::PercentageAddition);

    std::map<long long, std::tuple<long long, long long, long long>> res;
    for (long long src : SOURCE_PRIORITY) {
        double v_raw = sum_by_type_and_source(effects, enums::EffectTypes::VocalUp,
                                              enums::CalculationTypes::PercentageAddition, src);
        double e_raw = sum_by_type_and_source(effects, enums::EffectTypes::ExpressionUp,
                                              enums::CalculationTypes::PercentageAddition, src);
        double c_raw = sum_by_type_and_source(effects, enums::EffectTypes::ConcentrationUp,
                                              enums::CalculationTypes::PercentageAddition, src);

        double v_val = std::min(v_raw, v_cap);
        double e_val = std::min(e_raw, e_cap);
        double c_val = std::min(c_raw, c_cap);

        v_cap -= v_val;
        e_cap -= e_val;
        c_cap -= c_val;

        if (v_val != 0.0 || e_val != 0.0 || c_val != 0.0)
            res[src] = {static_cast<long long>(v_val), static_cast<long long>(e_val),
                        static_cast<long long>(c_val)};
    }
    return res;
}

std::map<long long, long long> performance_percent_bonuses_by_source(
    const std::vector<AppliedEffect>& effects) {
    double cap =
        kPerformancePercentLimit +
        sum_by_type(effects, enums::EffectTypes::PerformanceLimitUp, enums::CalculationTypes::PercentageAddition);
    std::map<long long, long long> res;
    for (long long src : SOURCE_PRIORITY) {
        double raw = sum_by_type_and_source(effects, enums::EffectTypes::PerformanceUp,
                                            enums::CalculationTypes::PercentageAddition, src);
        double val = std::min(raw, cap);
        cap -= val;
        if (val != 0.0) res[src] = static_cast<long long>(val);
    }
    return res;
}

double final_performance_multiplier(const std::vector<AppliedEffect>& effects) {
    double mult = 1.0;
    for (const AppliedEffect& e : effects) {
        if (jint(e.master, "calculation_type", enums::CalculationTypes::PercentageAddition) ==
                enums::CalculationTypes::Multiplication &&
            jint(e.master, "type", enums::EffectTypes::BaseVocalUp) ==
                enums::EffectTypes::FinalPerformanceUpCancelSense) {
            double val = detail_value(e.master, e.level);
            if (val > 0) mult *= (val / 100.0);
        }
    }
    return mult;
}

std::vector<long long> added_lights(const std::vector<AppliedEffect>& effects, long long sense_type) {
    std::vector<long long> lights;
    for (const AppliedEffect& e : effects) {
        if (jint(e.master, "fire_timing_type", enums::FireTimingTypes::StarAct) ==
            enums::FireTimingTypes::StartLive)
            continue;
        std::optional<long long> light =
            light_of(jint(e.master, "type", enums::EffectTypes::BaseVocalUp), sense_type);
        if (light.has_value()) {
            long long count = std::max(0LL, static_cast<long long>(detail_value(e.master, e.level)));
            for (long long k = 0; k < count; ++k) lights.push_back(*light);
        }
    }
    return lights;
}

std::vector<long long> start_lights(const std::vector<AppliedEffect>& effects, long long sense_type) {
    std::vector<long long> lights;
    for (const AppliedEffect& e : effects) {
        if (jint(e.master, "fire_timing_type", enums::FireTimingTypes::StarAct) !=
            enums::FireTimingTypes::StartLive)
            continue;
        std::optional<long long> light =
            light_of(jint(e.master, "type", enums::EffectTypes::BaseVocalUp), sense_type);
        if (light.has_value()) {
            long long count = std::max(0LL, static_cast<long long>(detail_value(e.master, e.level)));
            for (long long k = 0; k < count; ++k) lights.push_back(*light);
        }
    }
    return lights;
}

std::map<long long, long long> decrease_require_lights(const std::vector<AppliedEffect>& effects) {
    std::map<long long, long long> reductions;
    for (const AppliedEffect& e : effects) {
        std::optional<long long> light_type =
            decrease_light_type(jint(e.master, "type", enums::EffectTypes::BaseVocalUp));
        if (light_type.has_value())
            reductions[*light_type] += static_cast<long long>(detail_value(e.master, e.level));
    }
    return reductions;
}

std::vector<json> start_effects(const std::vector<AppliedEffect>& effects, long long actor_id,
                                long long start_order) {
    std::vector<json> result;
    long long order = start_order;
    for (const AppliedEffect& e : effects) {
        if (jint(e.master, "fire_timing_type", enums::FireTimingTypes::StarAct) !=
            enums::FireTimingTypes::StartLive)
            continue;
        order += 1;
        double value = detail_value(e.master, e.level);
        // range Self -> this actor; All -> every actor (target_actor_id=None)
        json target = (jint(e.master, "range", enums::EffectTargetRanges::None_) == enums::EffectTargetRanges::Self)
                          ? json(actor_id)
                          : json(nullptr);
        json tgt = json::object();
        tgt["target_actor_id"] = target;
        tgt["value"] = value;
        json eff = json::object();
        eff["order"] = order;
        eff["master_id"] = jint(e.master, "id_", 0);
        eff["effect_types"] = jint(e.master, "type", enums::EffectTypes::BaseVocalUp);
        eff["targets"] = json::array({tgt});
        eff["duration"] = jint(e.master, "duration_second", 0);
        result.push_back(std::move(eff));
    }
    return result;
}

}  // namespace effects
