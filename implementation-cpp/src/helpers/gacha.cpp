#include "helpers/gacha.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>

#include "generated/enums_generated.h"
#include "master_data.h"

// ports helpers/gacha.py; see that file for the derivation of every rate rule. per-rarity rates
// are server-side config (_RATES, only gacha_type=Pickup verified); everything else derives from
// GachaMaster + the Character/Poster master a thing points at. displayed probabilities are ceiled
// to 7 decimals, not rounded.

namespace gacha {

namespace {

constexpr long long kDugong = 140000;    // generic character talent-bloom item
constexpr long long kDaiPiece = 40;      // a Rare4 dupe also pays its own ActorDaiPiece
constexpr long long kPiecePickup = 400;  // Rare4 pickup rides along with this many pieces
constexpr long long kPieceRegular = 100;

// one pool's per-rarity totals plus the flat per-item rate a pickup gets out of its rarity's total
struct Rates {
    std::map<long long, double> normal;
    std::map<long long, double> fixed;
    double pickup;
};

// (gacha_type, card_type) -> rates. ONLY gacha_type=Pickup is verified (gachas 1161 and 2519).
const std::map<std::pair<long long, long long>, Rates>& rates_table() {
    static const std::map<std::pair<long long, long long>, Rates> t = [] {
        std::map<std::pair<long long, long long>, Rates> m;
        m[{enums::GachaTypes::Pickup, enums::GachaCardTypes::Character}] =
            Rates{{{2, 0.82}, {3, 0.15}, {4, 0.03}}, {{2, 0.0}, {3, 0.97}, {4, 0.03}}, 0.0075};
        m[{enums::GachaTypes::Pickup, enums::GachaCardTypes::Poster}] =
            Rates{{{1, 0.80}, {2, 0.16}, {3, 0.04}}, {{1, 0.0}, {2, 0.96}, {3, 0.04}}, 0.01};
        return m;
    }();
    return t;
}

// rarity -> GachaEmissionFlags bit. characters run Rare2/3/4, posters R/SR/SSR.
const std::map<long long, long long>& character_bits() {
    static const std::map<long long, long long> m = {
        {enums::CharacterRarities::Rare2, enums::GachaEmissionFlags::Rare2},
        {enums::CharacterRarities::Rare3, enums::GachaEmissionFlags::Rare3},
        {enums::CharacterRarities::Rare4, enums::GachaEmissionFlags::Rare4},
    };
    return m;
}
const std::map<long long, long long>& poster_bits() {
    static const std::map<long long, long long> m = {
        {enums::PossessionRarities::R, enums::GachaEmissionFlags::Rare2},
        {enums::PossessionRarities::SR, enums::GachaEmissionFlags::Rare3},
        {enums::PossessionRarities::SSR, enums::GachaEmissionFlags::Rare4},
    };
    return m;
}

// rarity -> dugong amount a convertible character dupe pays
const std::map<long long, long long>& character_dupe() {
    static const std::map<long long, long long> m = {
        {enums::CharacterRarities::Rare2, 1},
        {enums::CharacterRarities::Rare3, 10},
        {enums::CharacterRarities::Rare4, 100},
    };
    return m;
}
// rarity -> (frame item id, quantity) a poster dupe pays
const std::map<long long, std::pair<long long, long long>>& poster_dupe() {
    static const std::map<long long, std::pair<long long, long long>> m = {
        {enums::PossessionRarities::R, {130063, 5}},
        {enums::PossessionRarities::SR, {130064, 5}},
        {enums::PossessionRarities::SSR, {130064, 50}},
    };
    return m;
}

// {id_: row} over GachaMaster, built once (python _BY_ID)
const std::unordered_map<long long, const json*>& by_id() {
    static const std::unordered_map<long long, const json*> m = [] {
        std::unordered_map<long long, const json*> r;
        for (const json& g : master_data::table("GachaMaster")) r[g.at("id_").get<long long>()] = &g;
        return r;
    }();
    return m;
}
// {id_: row} over CharacterMaster, built once (python _CHARACTERS)
const std::unordered_map<long long, const json*>& characters() {
    static const std::unordered_map<long long, const json*> m = [] {
        std::unordered_map<long long, const json*> r;
        for (const json& c : master_data::table("CharacterMaster")) r[c.at("id_").get<long long>()] = &c;
        return r;
    }();
    return m;
}
// {id_: row} over PosterMaster, built once (python _POSTERS)
const std::unordered_map<long long, const json*>& posters() {
    static const std::unordered_map<long long, const json*> m = [] {
        std::unordered_map<long long, const json*> r;
        for (const json& p : master_data::table("PosterMaster")) r[p.at("id_").get<long long>()] = &p;
        return r;
    }();
    return m;
}
// {character_master_id: item_master_id} for a given TalentBloom item type, built once
const std::unordered_map<long long, long long>& piece_index(long long talent_bloom_item_type) {
    static const std::unordered_map<long long, long long> dai = [] {
        std::unordered_map<long long, long long> r;
        for (const json& row : master_data::table("CharacterPieceMaster"))
            if (row.at("talent_bloom_item_type").get<long long>() == enums::TalentBloomItemTypes::ActorDaiPiece)
                r[row.at("character_master_id").get<long long>()] = row.at("item_master_id").get<long long>();
        return r;
    }();
    static const std::unordered_map<long long, long long> piece = [] {
        std::unordered_map<long long, long long> r;
        for (const json& row : master_data::table("CharacterPieceMaster"))
            if (row.at("talent_bloom_item_type").get<long long>() == enums::TalentBloomItemTypes::ActorPiece)
                r[row.at("character_master_id").get<long long>()] = row.at("item_master_id").get<long long>();
        return r;
    }();
    return talent_bloom_item_type == enums::TalentBloomItemTypes::ActorDaiPiece ? dai : piece;
}

// displayed probabilities are ceiled at 7dp, not rounded -- e.g. 0.82/21 is 0.0390476190... and the
// client is told 0.0390477. 1e7 == 10 ** 7.
double ceil7(double value) { return std::ceil(value * 1e7) / 1e7; }

// the rate entry for a banner, or null: its own (gacha_type, card_type), else the Pickup fallback
const Rates* rates_for(const json& gacha) {
    long long card = gacha.at("card_type").get<long long>();
    const auto& table = rates_table();
    auto it = table.find({gacha.at("gacha_type").get<long long>(), card});
    if (it != table.end()) return &it->second;
    auto it2 = table.find({enums::GachaTypes::Pickup, card});
    return it2 != table.end() ? &it2->second : nullptr;
}

// the rarity of the Character/Poster a thing points at, or nullopt if unknown
std::optional<long long> rarity_of(const json& gacha, const json& thing) {
    const auto& lookup = gacha.at("card_type").get<long long>() == enums::GachaCardTypes::Character
                             ? characters()
                             : posters();
    auto it = lookup.find(thing.at("thing_id").get<long long>());
    if (it == lookup.end()) return std::nullopt;
    return it->second->at("rarity").get<long long>();
}

bool has_pickup(const json& thing) {
    auto it = thing.find("pickup_order");
    return it != thing.end() && !it->is_null();
}

// one pool's per-item probabilities: pickups take the flat rate, everything else of that rarity
// splits the remainder evenly. rarities at 0 contribute no items at all.
std::vector<json> pool(const json& gacha, const std::map<long long, double>& totals, double pickup_rate) {
    std::map<long long, std::vector<const json*>> by_rarity;
    auto tit = gacha.find("things");
    if (tit != gacha.end() && tit->is_array()) {
        for (const json& thing : *tit) {
            std::optional<long long> rarity = rarity_of(gacha, thing);
            if (!rarity) continue;
            auto rit = totals.find(*rarity);
            if (rit != totals.end() && rit->second > 0.0) by_rarity[*rarity].push_back(&thing);
        }
    }

    std::vector<json> items;
    for (const auto& kv : by_rarity) {
        long long rarity = kv.first;
        std::vector<const json*> pickups, rest;
        for (const json* t : kv.second) (has_pickup(*t) ? pickups : rest).push_back(t);
        double remainder = totals.at(rarity) - pickup_rate * static_cast<double>(pickups.size());
        double share = !rest.empty() ? remainder / static_cast<double>(rest.size()) : 0.0;
        for (const json* t : pickups) {
            json item = json::object();
            item["id_"] = t->at("id_").get<long long>();
            item["probability"] = ceil7(pickup_rate);
            items.push_back(std::move(item));
        }
        for (const json* t : rest) {
            json item = json::object();
            item["id_"] = t->at("id_").get<long long>();
            item["probability"] = ceil7(share);
            items.push_back(std::move(item));
        }
    }
    std::sort(items.begin(), items.end(), [](const json& a, const json& b) {
        return a.at("id_").get<long long>() < b.at("id_").get<long long>();
    });
    return items;
}

long long now_utc() { return static_cast<long long>(std::time(nullptr)); }

// Howard Hinnant's days-from-civil (days relative to 1970-01-01)
long long days_from_civil(long long y, unsigned m, unsigned d) {
    y -= (m <= 2);
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

// epoch seconds of an iso-8601 string; mirrors datetime.fromisoformat (naive -> utc, Z == +00:00).
// false for anything not a valid date(+time), so active_gacha_ids reads it as "skip this banner".
bool iso_epoch_seconds(const std::string& s, long long& out) {
    size_t i = 0, n = s.size();
    auto read = [&](int len, int& v) -> bool {
        v = 0;
        int k = 0;
        for (; k < len && i < n && s[i] >= '0' && s[i] <= '9'; ++k, ++i) v = v * 10 + (s[i] - '0');
        return k == len;
    };
    int year, mon, day;
    if (!read(4, year)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, mon)) return false;
    if (i >= n || s[i] != '-') return false;
    ++i;
    if (!read(2, day)) return false;

    int hh = 0, mm = 0, ss = 0;
    long long tz = 0;
    if (i < n) {
        ++i;  // date/time separator
        if (!read(2, hh)) return false;
        if (i >= n || s[i] != ':') return false;
        ++i;
        if (!read(2, mm)) return false;
        if (i < n && s[i] == ':') {
            ++i;
            if (!read(2, ss)) return false;
        }
        if (i < n && (s[i] == '.' || s[i] == ',')) {
            ++i;
            while (i < n && s[i] >= '0' && s[i] <= '9') ++i;
        }
        if (i < n) {
            char c = s[i];
            if (c == 'Z' || c == 'z') {
                ++i;
            } else if (c == '+' || c == '-') {
                int sign = (c == '-') ? -1 : 1;
                ++i;
                int oh = 0, om = 0, os_ = 0;
                if (!read(2, oh)) return false;
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, om);
                } else {
                    read(2, om);
                }
                if (i < n && s[i] == ':') {
                    ++i;
                    read(2, os_);
                }
                tz = sign * (oh * 3600LL + om * 60LL + os_);
            }
        }
    }
    out = days_from_civil(year, static_cast<unsigned>(mon), static_cast<unsigned>(day)) * 86400LL +
          hh * 3600LL + mm * 60LL + ss - tz;
    return true;
}

// the module default rng (python global `random`); per-thread so concurrent rolls never race
std::mt19937_64& default_engine() {
    static thread_local std::mt19937_64 eng{std::random_device{}()};
    return eng;
}

}  // namespace

std::optional<Lineup> lineup(long long gacha_master_id) {
    const auto& idx = by_id();
    auto it = idx.find(gacha_master_id);
    if (it == idx.end()) return std::nullopt;
    const json& gacha = *it->second;
    const Rates* rates = rates_for(gacha);
    if (rates == nullptr) return std::nullopt;

    auto to_probs = [](const std::map<long long, double>& totals) {
        std::vector<json> v;
        for (const auto& kv : totals) {  // std::map ascending by rarity == sorted(totals.items())
            json p = json::object();
            p["rarity"] = kv.first;
            p["probability"] = kv.second;
            v.push_back(std::move(p));
        }
        return v;
    };

    Lineup out;
    out.normal_probabilities = to_probs(rates->normal);
    out.fixed_probabilities = to_probs(rates->fixed);
    out.normal_items = pool(gacha, rates->normal, rates->pickup);
    out.fixed_items = pool(gacha, rates->fixed, rates->pickup);
    return out;
}

std::pair<long long, long long> emission_flags(long long gacha_master_id) {
    const auto& idx = by_id();
    auto it = idx.find(gacha_master_id);
    if (it == idx.end()) return {0, 0};
    const json& gacha = *it->second;
    const Rates* rates = rates_for(gacha);
    if (rates == nullptr) return {0, 0};
    const auto& bits = gacha.at("card_type").get<long long>() == enums::GachaCardTypes::Character
                           ? character_bits()
                           : poster_bits();
    auto mask = [&bits](const std::map<long long, double>& totals) -> long long {
        long long m = 0;
        for (const auto& kv : bits) {
            auto tit = totals.find(kv.first);
            if (tit != totals.end() && tit->second > 0.0) m += kv.second;
        }
        return m;
    };
    return {mask(rates->normal), mask(rates->fixed)};
}

std::pair<json, json> detail_of(long long gacha_detail_master_id) {
    for (const json& gacha : master_data::table("GachaMaster")) {
        auto dit = gacha.find("gacha_details");
        if (dit == gacha.end() || !dit->is_array()) continue;
        for (const json& detail : *dit)
            if (detail.at("id_").get<long long>() == gacha_detail_master_id) return {gacha, detail};
    }
    return {json(nullptr), json(nullptr)};
}

std::vector<json> roll_prizes(const json& gacha, const json& detail, std::mt19937_64* rng) {
    std::optional<Lineup> picked = lineup(gacha.at("id_").get<long long>());
    if (!picked) return {};
    const std::vector<json>& normal_items = picked->normal_items;
    const std::vector<json>& fixed_items = picked->fixed_items;

    std::unordered_map<long long, const json*> things;
    auto tit = gacha.find("things");
    if (tit != gacha.end() && tit->is_array())
        for (const json& t : *tit) things[t.at("id_").get<long long>()] = &t;

    std::mt19937_64& engine = rng != nullptr ? *rng : default_engine();
    auto draw = [&](const std::vector<json>& items) -> json {
        if (items.empty()) return json(nullptr);  // python raises on an empty pool; never happens live
        std::vector<double> cum;
        cum.reserve(items.size());
        double acc = 0.0;
        for (const json& i : items) {
            acc += i.at("probability").get<double>();
            cum.push_back(acc);
        }
        double total = cum.back();
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        double x = dist(engine) * total;
        // bisect_right(cum, x, 0, hi) with hi = n - 1
        size_t hi = items.size() - 1;
        size_t idx = static_cast<size_t>(std::upper_bound(cum.begin(), cum.begin() + hi, x) - cum.begin());
        return *things.at(items[idx].at("id_").get<long long>());
    };

    auto int_or_zero = [](const json& obj, const char* key) -> long long {
        auto it = obj.find(key);
        return (it == obj.end() || it->is_null()) ? 0 : it->get<long long>();
    };
    long long total = std::max<long long>(0, int_or_zero(detail, "prize_count"));
    long long fixed = std::min(total, std::max<long long>(0, int_or_zero(detail, "fixed_prize_count")));

    std::vector<json> out;
    out.reserve(static_cast<size_t>(total));
    for (long long i = 0; i < total - fixed; ++i) out.push_back(draw(normal_items));
    for (long long i = 0; i < fixed; ++i) out.push_back(draw(fixed_items));
    return out;
}

std::vector<ThingTriple> dupe_conversion(const json& gacha, const json& thing) {
    const long long item = enums::ThingTypes::Item;
    long long thing_id = thing.at("thing_id").get<long long>();
    if (gacha.at("card_type").get<long long>() == enums::GachaCardTypes::Character) {
        const auto& chars = characters();
        auto cit = chars.find(thing_id);
        if (cit == chars.end()) return {};
        long long rarity = cit->second->at("rarity").get<long long>();
        const auto& dupe = character_dupe();
        auto ait = dupe.find(rarity);
        if (ait == dupe.end()) return {};
        std::vector<ThingTriple> out;
        out.emplace_back(item, kDugong, ait->second);
        if (rarity == enums::CharacterRarities::Rare4) {
            std::optional<long long> dai = dai_piece_item(thing_id);
            if (dai) out.emplace_back(item, *dai, kDaiPiece);
        }
        return out;
    }

    const auto& post = posters();
    auto pit = post.find(thing_id);
    if (pit == post.end()) return {};
    long long rarity = pit->second->at("rarity").get<long long>();
    const auto& dupe = poster_dupe();
    auto fit = dupe.find(rarity);
    if (fit == dupe.end()) return {};
    return {ThingTriple(item, fit->second.first, fit->second.second)};
}

std::optional<long long> dai_piece_item(long long character_master_id) {
    const auto& idx = piece_index(enums::TalentBloomItemTypes::ActorDaiPiece);
    auto it = idx.find(character_master_id);
    if (it == idx.end()) return std::nullopt;
    return it->second;
}

long long poster_max_phase(long long poster_master_id) {
    const auto& post = posters();
    auto it = post.find(poster_master_id);
    if (it == post.end()) return 0;
    auto fit = it->second->find("poster_breakthrough_max_phase");
    if (fit == it->second->end() || fit->is_null()) return 0;
    return fit->get<long long>();
}

std::optional<long long> piece_item(long long character_master_id) {
    const auto& idx = piece_index(enums::TalentBloomItemTypes::ActorPiece);
    auto it = idx.find(character_master_id);
    if (it == idx.end()) return std::nullopt;
    return it->second;
}

std::vector<ThingTriple> piece_bonus(const json& gacha, const json& thing) {
    if (gacha.at("card_type").get<long long>() != enums::GachaCardTypes::Character) return {};
    long long thing_id = thing.at("thing_id").get<long long>();
    const auto& chars = characters();
    auto cit = chars.find(thing_id);
    if (cit == chars.end() || cit->second->at("rarity").get<long long>() != enums::CharacterRarities::Rare4)
        return {};
    std::optional<long long> item = piece_item(thing_id);
    if (!item) return {};
    long long amount = has_pickup(thing) ? kPiecePickup : kPieceRegular;
    return {ThingTriple(enums::ThingTypes::Item, *item, amount)};
}

std::vector<long long> active_gacha_ids(std::optional<std::time_t> now) {
    long long at = now ? static_cast<long long>(*now) : now_utc();
    std::vector<const json*> live;
    for (const json& gacha : master_data::table("GachaMaster")) {
        auto sit = gacha.find("start_date");
        auto eit = gacha.find("end_date");
        if (sit == gacha.end() || !sit->is_string() || eit == gacha.end() || !eit->is_string()) continue;
        long long start, end;
        if (!iso_epoch_seconds(sit->get_ref<const std::string&>(), start)) continue;
        if (!iso_epoch_seconds(eit->get_ref<const std::string&>(), end)) continue;
        if (start <= at && at <= end) live.push_back(&gacha);
    }
    std::sort(live.begin(), live.end(), [](const json* a, const json* b) {
        long long ao = a->at("order").get<long long>(), bo = b->at("order").get<long long>();
        if (ao != bo) return ao < bo;
        return a->at("id_").get<long long>() < b->at("id_").get<long long>();
    });
    std::vector<long long> out;
    out.reserve(live.size());
    for (const json* g : live) out.push_back(g->at("id_").get<long long>());
    return out;
}

std::vector<json> roll_limits(long long gacha_master_id, const std::map<long long, long long>& used) {
    const auto& idx = by_id();
    auto it = idx.find(gacha_master_id);
    if (it == idx.end()) return {};
    const json& gacha = *it->second;
    std::vector<json> out;
    auto dit = gacha.find("gacha_details");
    if (dit == gacha.end() || !dit->is_array()) return out;
    for (const json& detail : *dit) {
        std::vector<long long> limits;
        for (const char* key : {"daily_roll_limit", "overall_roll_limit"}) {
            auto f = detail.find(key);
            if (f != detail.end() && !f->is_null()) limits.push_back(f->get<long long>());
        }
        if (limits.empty()) continue;
        long long detail_id = detail.at("id_").get<long long>();
        auto uit = used.find(detail_id);
        long long left = *std::min_element(limits.begin(), limits.end()) - (uit != used.end() ? uit->second : 0);
        json rl = json::object();
        rl["gacha_detail_master_id"] = detail_id;
        rl["roll_left"] = std::max<long long>(0, left);
        out.push_back(std::move(rl));
    }
    return out;
}

}  // namespace gacha
