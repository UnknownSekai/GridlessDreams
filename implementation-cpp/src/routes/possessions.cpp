#include "routes/possessions.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

#include "helpers/game_state.h"
#include "pipeline.h"
#include "wire.h"

// ports routes/possessions.py. favorite stamps/costumes set + sort via gs.transaction.

namespace routes {
namespace {

using rjson = nlohmann::json;          // db rows (camelCase columns)
using ojson = nlohmann::ordered_json;  // master rows, payloads + wire results

// list(column or []) for an int array column
std::vector<long long> int_list(const rjson& value) {
    std::vector<long long> out;
    if (value.is_array())
        for (const rjson& v : value) out.push_back(v.get<long long>());
    return out;
}

bool contains(const std::vector<long long>& xs, long long x) {
    return std::find(xs.begin(), xs.end(), x) != xs.end();
}

void toggle_favorite_stamp(const httplib::Request& req, httplib::Response& res, long long ident,
                           bool add) {
    try {
        game_state::State s = game_state::transaction(req);
        rjson* row = s.one("Stamp");
        const ojson* m = game_state::master("stamp_master", ident);
        if (row == nullptr || m == nullptr ||
            (!m->at("is_default").get<bool>() && !contains(int_list(row->at("stampMasterIds")), ident)))
            throw game_state::Rejected();
        std::vector<long long> favorites = int_list(row->at("favoriteStampMasterIds"));
        if (add) {
            if (!contains(favorites, ident)) {
                favorites.push_back(ident);
                game_state::mission_progress(s, 21, 1, true);
            }
        } else {
            std::vector<long long> kept;
            for (long long v : favorites)
                if (v != ident) kept.push_back(v);
            favorites = std::move(kept);
        }
        s.update("Stamp", row, rjson{{"favoriteStampMasterIds", favorites}});
        s.commit();
        ojson result;
        result["is_success"] = true;
        pipeline::respond(res, "BooleanResult", result, ojson::array(), s.present());
    } catch (const game_state::Rejected&) {
        pipeline::respond(res, "BooleanResult", ojson::object());
    }
}

}  // namespace

void register_possessions(httplib::Server& svr) {
    // /api/Possessions/AddFavoriteStamp/{mStampId}
    svr.Post("/api/Possessions/AddFavoriteStamp/:mStampId",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long m_stamp_id = std::stoll(req.path_params.at("mStampId"));
                 toggle_favorite_stamp(req, res, m_stamp_id, true);
             });

    // /api/Possessions/RemoveFavoriteStamp/{mStampId}
    svr.Post("/api/Possessions/RemoveFavoriteStamp/:mStampId",
             [](const httplib::Request& req, httplib::Response& res) {
                 long long m_stamp_id = std::stoll(req.path_params.at("mStampId"));
                 toggle_favorite_stamp(req, res, m_stamp_id, false);
             });

    // /api/Possessions/SetFavorite
    svr.Post("/api/Possessions/SetFavorite", [](const httplib::Request& req, httplib::Response& res) {
        try {
            ojson p = wire::read_request(req.body, "CostumeFavoritePayload");
            if (p.is_null()) throw game_state::Rejected();
            long long base = p.value("character_base_master_id", static_cast<long long>(0));
            long long ident = p.value("costume_master_id", static_cast<long long>(0));
            bool set_favorite = p.value("set_favorite", false);
            game_state::State s = game_state::transaction(req);
            game_state::costume_owned(s, base, ident);
            rjson* row = s.one("FavoriteCostume", rjson{{"characterBaseMasterId", base}});
            std::vector<long long> favorites =
                row != nullptr ? int_list(row->at("favoriteCostumeMasterIds")) : std::vector<long long>{};
            if (set_favorite && !contains(favorites, ident)) favorites.push_back(ident);
            if (!set_favorite) {
                std::vector<long long> kept;
                for (long long v : favorites)
                    if (v != ident) kept.push_back(v);
                favorites = std::move(kept);
            }
            if (row != nullptr) {
                s.update("FavoriteCostume", row, rjson{{"favoriteCostumeMasterIds", favorites}});
            } else {
                s.insert("FavoriteCostume", rjson{{"characterBaseMasterId", base},
                                                  {"favoriteCostumeMasterIds", favorites}});
            }
            s.commit();
            ojson result;
            result["is_success"] = true;
            pipeline::respond(res, "BooleanResult", result, ojson::array(), s.present());
        } catch (const game_state::Rejected&) {
            pipeline::respond(res, "BooleanResult", ojson::object());
        }
    });

    // /api/Possessions/SortFavoriteStamps
    svr.Post("/api/Possessions/SortFavoriteStamps",
             [](const httplib::Request& req, httplib::Response& res) {
                 try {
                     ojson p = wire::read_request(req.body, "FavoriteStampOrderPayload");
                     if (p.is_null()) throw game_state::Rejected();
                     std::vector<long long> order;
                     if (p.contains("stamp_master_ids") && p["stamp_master_ids"].is_array())
                         for (const ojson& v : p["stamp_master_ids"])
                             order.push_back(v.get<long long>());
                     game_state::State s = game_state::transaction(req);
                     rjson* row = s.one("Stamp");
                     if (row == nullptr) throw game_state::Rejected();
                     // a reorder keeps exactly the current favorites, only their sequence changes
                     std::unordered_set<long long> current;
                     for (long long v : int_list(row->at("favoriteStampMasterIds"))) current.insert(v);
                     std::vector<long long> ordered;
                     for (long long i : order)
                         if (current.count(i)) ordered.push_back(i);
                     s.update("Stamp", row, rjson{{"favoriteStampMasterIds", ordered}});
                     s.commit();
                     ojson result;
                     result["is_success"] = true;
                     pipeline::respond(res, "BooleanResult", result, ojson::array(), s.present());
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", ojson::object());
                 }
             });
}

}  // namespace routes
