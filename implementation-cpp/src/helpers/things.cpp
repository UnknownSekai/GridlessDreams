#include "helpers/things.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db.h"
#include "db/user.h"
#include "generated/enums_generated.h"
#include "helpers/music_unlock.h"
#include "master_data.h"

namespace things {
namespace {

namespace T = enums::ThingTypes;

// possession tables -- one owned row per unit: ThingType -> (table, masterId col, fixed extra cols).
// Character/Accessory are handled separately (they need masterdata-derived fields).
struct Possession {
    const char* table;
    const char* master_col;
    json extra;
};

const std::map<long long, Possession>& _possession() {
    static const std::map<long long, Possession> m = [] {
        std::map<long long, Possession> p;
        // posters start at level 1, not the column's 0 -- Character/Accessory hardcode the same
        // in their create_* queries, and Poster is the only possession here with a level column
        json poster = json::object();
        poster["level"] = 1;
        json music = json::object();
        music["isPossession"] = true;
        p.emplace(T::Poster, Possession{"poster", "posterMasterId", poster});
        p.emplace(T::Costume, Possession{"costume", "costumeMasterId", json::object()});
        p.emplace(T::Trophy, Possession{"trophy", "trophyMasterId", json::object()});
        p.emplace(T::Nameplate, Possession{"nameplate", "namePlateMasterId", json::object()});
        p.emplace(T::Music, Possession{"music", "musicMasterId", music});
        p.emplace(T::Decoration, Possession{"decoration", "decorationMasterId", json::object()});
        p.emplace(T::AlbumTheme, Possession{"album_theme", "albumThemeMasterId", json::object()});
        return p;
    }();
    return m;
}

// per-user singleton collections -- masterId appended to a json array: ThingType -> (table, array col)
struct Collection {
    const char* table;
    const char* array_col;
};

const std::map<long long, Collection>& _collection() {
    static const std::map<long long, Collection> m = {
        {T::Stamp, {"stamp", "stampMasterIds"}},
        {T::NameColor, {"name_color", "nameColorMasterIds"}},
        {T::Bomb, {"bomb", "bombMasterIds"}},
        {T::Note, {"note", "noteMasterIds"}},
        {T::NameBaseColor, {"name_base_color", "nameBaseColorMasterIds"}},
        {T::IconFrame, {"icon_frame", "iconFrameMasterIds"}},
        {T::HomeSkin, {"home_skin", "homeSkinMasterIds"}},
    };
    return m;
}

// ThingType -> the per-user entity type its grant writes (so callers can build `present`)
const std::unordered_map<long long, std::string>& _present_type() {
    static const std::unordered_map<long long, std::string> m = {
        {T::Coin, "Currency"},       {T::Jewel, "Currency"},
        {T::Item, "Item"},           {T::Stamina, "User"},
        {T::Character, "Character"}, {T::Poster, "Poster"},
        {T::Accessory, "Accessory"}, {T::Costume, "Costume"},
        {T::Trophy, "Trophy"},       {T::Nameplate, "Nameplate"},
        {T::Music, "Music"},         {T::Decoration, "Decoration"},
        {T::AlbumTheme, "AlbumTheme"}, {T::Stamp, "Stamp"},
        {T::NameColor, "NameColor"}, {T::Bomb, "Bomb"},
        {T::Note, "Note"},           {T::NameBaseColor, "NameBaseColor"},
        {T::IconFrame, "IconFrame"}, {T::HomeSkin, "HomeSkin"},
    };
    return m;
}

std::mt19937_64& _engine() {
    static thread_local std::mt19937_64 eng{std::random_device{}()};
    return eng;
}

long long _random_id() {
    std::uniform_int_distribution<long long> dist(1000000LL, 9999999999LL);
    return dist(_engine());
}

// id_-keyed master lookups, memoized per header name (python's _masters dict).
std::unordered_map<std::string, std::unordered_map<long long, const json*>> _masters;

const std::unordered_map<long long, const json*>& _by_id(const std::string& header_name) {
    auto it = _masters.find(header_name);
    if (it != _masters.end()) return it->second;
    auto& idx = _masters[header_name];
    for (const json& r : master_data::table(header_name)) {
        auto id = r.find("id_");
        if (id != r.end() && id->is_number_integer()) idx[id->get<long long>()] = &r;
    }
    return idx;
}

const json* _lookup(const std::string& header_name, long long id) {
    const auto& idx = _by_id(header_name);
    auto it = idx.find(id);
    return it == idx.end() ? nullptr : it->second;
}

json _received_thing(long long type, long long thing_id, long long quantity) {
    json rt = json::object();
    rt["type"] = type;
    rt["id_"] = thing_id;
    rt["quantity"] = quantity;
    rt["sent_inbox"] = false;
    return rt;
}

void _grant_character(long long user_id, long long character_master_id) {
    // Character.characterBaseId points at an owned CharacterBase (shared across a
    // character's variants); resolve it via CharacterMaster.character_base_master_id.
    const json* master = _lookup("CharacterMaster", character_master_id);
    std::optional<long long> base_master_id =
        master ? std::optional<long long>(master->at("character_base_master_id").get<long long>())
               : std::nullopt;
    long long base_id = 0;
    if (base_master_id) {
        auto existing = db::fetchrow(db::user::get_character_base(user_id, *base_master_id));
        if (existing) {
            base_id = existing->at("id").get<long long>();
        } else {
            base_id = _random_id();
            const json* bm = _lookup("CharacterBaseMaster", *base_master_id);
            std::optional<long long> costume =
                bm ? std::optional<long long>(
                         bm->at("default_costume_master_id").get<long long>())
                   : std::nullopt;
            db::execute(db::user::create_character_base(user_id, base_id, *base_master_id, costume,
                                                        character_master_id));
        }
    }
    db::execute(db::user::create_character(user_id, _random_id(), character_master_id, base_id));
}

void _grant_accessory(long long user_id, long long accessory_master_id) {
    // fixed effects stay derived from AccessoryMaster on demand; only the rolled random
    // effects (one per random_effect_group) are persisted in accessory_effects.
    const json* master = _lookup("AccessoryMaster", accessory_master_id);
    json effects = json::array();
    if (master) {
        auto reg = master->find("random_effect_groups");
        if (reg != master->end() && reg->is_array()) {
            for (const auto& gid : *reg) {
                const json* group = _lookup("RandomEffectGroupMaster", gid.get<long long>());
                if (group) {
                    auto ae = group->find("accessory_effects");
                    if (ae != group->end() && ae->is_array() && !ae->empty()) {
                        std::uniform_int_distribution<std::size_t> pick(0, ae->size() - 1);
                        effects.push_back((*ae)[pick(_engine())].get<long long>());
                    }
                }
            }
        }
    }
    db::execute(db::user::create_accessory(user_id, _random_id(), accessory_master_id, effects));
}

}  // namespace

std::optional<std::string> present_type(long long thing_type) {
    // all valid ThingTypes are mapped, so a missing key mirrors python's T() ValueError -> None.
    const auto& m = _present_type();
    auto it = m.find(thing_type);
    if (it == m.end()) return std::nullopt;
    return it->second;
}

json grant_thing(long long user_id, long long thing_type, long long thing_id, long long quantity) {
    long long t = thing_type;
    if (t == T::Coin) {
        db::execute(db::user::add_currency(user_id, quantity));
    } else if (t == T::Jewel) {
        db::execute(db::user::add_currency(user_id, 0, quantity));
    } else if (t == T::Item) {
        db::user::increment_item_stock(user_id, thing_id, quantity);
    } else if (t == T::Stamina) {
        db::execute(db::user::add_stamina(user_id, quantity));
    } else if (t == T::Character) {
        for (long long i = 0; i < std::max<long long>(1, quantity); ++i)
            _grant_character(user_id, thing_id);
    } else if (t == T::Accessory) {
        for (long long i = 0; i < std::max<long long>(1, quantity); ++i)
            _grant_accessory(user_id, thing_id);
    } else if (_collection().count(t)) {
        const Collection& c = _collection().at(t);
        db::user::grant_collection(c.table, c.array_col, user_id, thing_id);
    } else if (t == T::Music) {
        // a song earned after the Stella/Olivier thresholds is born already unlocked
        auto state = music_unlock::granted_music_state(user_id, thing_id);
        json extra = json::object();
        extra["isPossession"] = true;
        extra["stellaReleased"] = state.first;
        extra["olivierReleaseStatus"] = state.second;
        db::execute(db::user::grant_possession("music", "musicMasterId", user_id, _random_id(),
                                               thing_id, extra));
    } else if (_possession().count(t)) {
        const Possession& p = _possession().at(t);
        for (long long i = 0; i < std::max<long long>(1, quantity); ++i)  // one owned row per unit
            db::execute(db::user::grant_possession(p.table, p.master_col, user_id, _random_id(),
                                                   thing_id, p.extra));
    }
    return _received_thing(t, thing_id, quantity);
}

std::vector<json> grant_things(long long user_id, const std::vector<ThingTriple>& things) {
    std::vector<json> out;
    out.reserve(things.size());
    for (const auto& tr : things)
        out.push_back(grant_thing(user_id, std::get<0>(tr), std::get<1>(tr), std::get<2>(tr)));
    return out;
}

std::vector<json> grant_things_consolidated(long long user_id,
                                            const std::vector<ThingTriple>& things) {
    std::map<std::pair<long long, long long>, long long> totals;
    std::vector<std::pair<long long, long long>> seq;
    for (const auto& tr : things) {
        std::pair<long long, long long> key{std::get<0>(tr), std::get<1>(tr)};
        if (totals.find(key) == totals.end()) seq.push_back(key);
        totals[key] += std::get<2>(tr);
    }

    long long coin = 0, jewel = 0, stamina = 0;
    std::vector<std::pair<std::int64_t, std::int64_t>> items;
    std::vector<std::tuple<long long, long long, long long>> others;
    std::vector<json> received;
    for (const auto& key : seq) {
        long long tt = key.first, tid = key.second;
        long long qty = totals[key];
        long long t = tt;
        if (t == T::Coin) {
            coin += qty;
        } else if (t == T::Jewel) {
            jewel += qty;
        } else if (t == T::Stamina) {
            stamina += qty;
        } else if (t == T::Item) {
            items.emplace_back(tid, qty);
        } else {
            others.emplace_back(tt, tid, qty);
            continue;
        }
        received.push_back(_received_thing(tt, tid, qty));
    }

    if (coin || jewel) db::execute(db::user::add_currency(user_id, coin, jewel));
    if (stamina) db::execute(db::user::add_stamina(user_id, stamina));
    if (!items.empty()) db::user::increment_item_stocks(user_id, items);
    for (const auto& o : others)
        received.push_back(grant_thing(user_id, std::get<0>(o), std::get<1>(o), std::get<2>(o)));
    return received;
}

}  // namespace things
