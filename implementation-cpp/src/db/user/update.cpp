#include "db/user.h"

#include <cstddef>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// ports db/user/update.py. most functions are plain query builders returning an
// ExecutableQuery/SelectQuery with the Postgres $n placeholders kept verbatim. the data-
// modifying-CTE functions (DELETE/UPDATE/INSERT inside WITH) have no single-statement SQLite
// form, so they run through db::composite_* and therefore EXECUTE here and return the result
// (string tag / rows) instead of a query object -- call these directly, not via db::execute.
namespace db {
namespace user {
namespace {

json opt_json(const std::optional<long long>& v) {
    return v ? json(*v) : json(nullptr);
}

std::string join_csv(const std::vector<std::string>& parts) {
    std::string out;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i) out += ", ";
        out += parts[i];
    }
    return out;
}

ExecutableQuery eq(std::string sql, std::vector<json> args) {
    ExecutableQuery q;
    q.sql = std::move(sql);
    q.args = std::move(args);
    return q;
}

SelectQuery sq(const char* model, std::string sql, std::vector<json> args) {
    SelectQuery q;
    q.sql = std::move(sql);
    q.args = std::move(args);
    q.model = model;
    return q;
}

std::optional<json> first_or_none(std::vector<json> rows) {
    if (rows.empty()) return std::nullopt;
    return std::optional<json>(std::move(rows.front()));
}

}  // namespace

ExecutableQuery update_party_slots(long long user_id, const std::vector<wire::json>& slots) {
    std::vector<std::string> rows;
    std::vector<json> args{json(user_id)};
    for (const wire::json& s : slots) {
        std::size_t n = args.size();
        rows.push_back("($" + std::to_string(n + 1) + ", $" + std::to_string(n + 2) + ", $" +
                       std::to_string(n + 3) + ", $" + std::to_string(n + 4) + ", $" +
                       std::to_string(n + 5) + ")");
        args.push_back(json(s.at("id")));
        args.push_back(json(s.at("characterId")));
        args.push_back(json(s.at("posterId")));
        args.push_back(json(s.at("accessoryId")));
        args.push_back(json(s.at("bonusAbilityEnableFlags")));
    }
    // data-modifying CTE stays one statement, but SQLite has no (VALUES ...) AS v(cols) derived
    // table -- the column names move onto a WITH alias feeding the UPDATE ... FROM
    std::string sql =
        "WITH v(id, character_id, poster_id, accessory_id, flags) AS (VALUES " + join_csv(rows) +
        ") UPDATE \"party_slot\" AS s SET \"characterId\" = v.character_id, "
        "\"posterId\" = v.poster_id, \"accessoryId\" = v.accessory_id, "
        "\"bonusAbilityEnableFlags\" = v.flags FROM v "
        "WHERE s.\"userId\" = $1 AND s.\"id\" = v.id";
    return eq(std::move(sql), std::move(args));
}

ExecutableQuery update_party_slot_positions(
    long long user_id, const std::vector<std::pair<long long, long long>>& positions) {
    std::vector<std::string> rows;
    std::vector<json> args{json(user_id)};
    for (const std::pair<long long, long long>& p : positions) {
        std::size_t n = args.size();
        rows.push_back("($" + std::to_string(n + 1) + ", $" + std::to_string(n + 2) + ")");
        args.push_back(json(p.first));
        args.push_back(json(p.second));
    }
    std::string sql = "WITH v(id, position) AS (VALUES " + join_csv(rows) +
                      ") UPDATE \"party_slot\" AS s SET \"position\" = v.position FROM v "
                      "WHERE s.\"userId\" = $1 AND s.\"id\" = v.id";
    return eq(std::move(sql), std::move(args));
}

ExecutableQuery update_party_leader(long long user_id, long long party_id,
                                    long long position) {
    return ExecutableQuery(
        R"(UPDATE "party" SET "leaderPosition" = $3 WHERE "userId" = $1 AND "id" = $2)", user_id,
        party_id, position);
}

ExecutableQuery update_party_name(long long user_id, long long party_id,
                                  const std::string& name) {
    return ExecutableQuery(R"(UPDATE "party" SET "name" = $3 WHERE "userId" = $1 AND "id" = $2)",
                           user_id, party_id, name);
}

ExecutableQuery update_user_profile_edit(long long user_id, const wire::json& fields) {
    static const char* const cols[] = {
        "name",          "introduction",      "mainUCharacterId",      "mNameplateId",
        "mNameColorId",  "mTrophyId1",        "mTrophyId2",            "mTrophyId3",
        "isPublicPlayerRate", "displayAwakeningStatus", "mainCharacterMasterId",
        "nameBaseColorMasterId", "iconFrameMasterId", "homeSkinMasterId"};
    std::vector<std::string> set_parts;
    std::vector<json> args{json(user_id)};
    for (std::size_t i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i) {
        set_parts.push_back(std::string("\"") + cols[i] + "\" = $" + std::to_string(i + 2));
        args.push_back(fields.contains(cols[i]) ? json(fields.at(cols[i])) : json(nullptr));
    }
    std::string sql =
        "UPDATE \"user_profile\" SET " + join_csv(set_parts) + " WHERE \"userId\" = $1";
    return eq(std::move(sql), std::move(args));
}

ExecutableQuery update_home_display_preference(long long user_id, const wire::json& fields) {
    static const char* const cols[] = {
        "homeCharacterBaseMasterId",   "memberCharacterBaseMasterId",
        "storyCharacterBaseMasterId",  "shopCharacterBaseMasterId",
        "homeCostumeMasterId",         "memberCostumeMasterId",
        "storyCostumeMasterId",        "shopCostumeMasterId",
        "illustCharacterMasterId",     "displayAwakeningStatus",
        "homeCharacterDisplayType",    "loginBonusCharacterBaseMasterId",
        "loginBonusCostumeMasterId"};
    std::vector<std::string> set_parts;
    std::vector<json> args{json(user_id)};
    for (std::size_t i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i) {
        set_parts.push_back(std::string("\"") + cols[i] + "\" = $" + std::to_string(i + 2));
        args.push_back(fields.contains(cols[i]) ? json(fields.at(cols[i])) : json(nullptr));
    }
    std::string sql =
        "UPDATE \"home_display_preference\" SET " + join_csv(set_parts) + " WHERE \"userId\" = $1";
    return eq(std::move(sql), std::move(args));
}

std::vector<std::string> set_home_bgm(long long user_id, long long master_id,
                                      long long selection_type,
                                      std::optional<long long> detail_master_id) {
    // home_b_g_m is a per-user singleton with no id column, so the old row is dropped first
    ExecutableQuery del =
        eq(R"(DELETE FROM "home_b_g_m" WHERE "userId" = $1)", {json(user_id)});
    ExecutableQuery ins =
        eq(R"(INSERT INTO "home_b_g_m" ("userId", "homeBGMMasterId", "selectionType", "homeBGMDetailMasterId") VALUES ($1, $2, $3, $4))",
           {json(user_id), json(master_id), json(selection_type), opt_json(detail_master_id)});
    return composite_exec({del, ins});
}

ExecutableQuery update_character_base_costume(long long user_id,
                                              long long character_base_master_id,
                                              long long costume_master_id) {
    return ExecutableQuery(
        R"(UPDATE "character_base" SET "costumeMasterId" = $3 WHERE "userId" = $1 AND "characterBaseMasterId" = $2)",
        user_id, character_base_master_id, costume_master_id);
}

ExecutableQuery update_character_base_portal(long long user_id,
                                             long long character_base_master_id,
                                             long long portal_character_id,
                                             bool display_awakening) {
    return ExecutableQuery(
        R"(UPDATE "character_base" SET "portalCharacterId" = $3, "portalDisplayAwakeningStatus" = $4 WHERE "userId" = $1 AND "characterBaseMasterId" = $2)",
        user_id, character_base_master_id, portal_character_id, display_awakening);
}

ExecutableQuery update_accessory_level(long long user_id, long long accessory_id,
                                       long long level) {
    return ExecutableQuery(
        R"(UPDATE "accessory" SET "level" = $3 WHERE "userId" = $1 AND "id" = $2)", user_id,
        accessory_id, level);
}

ExecutableQuery update_poster_released_episode(long long user_id, long long poster_id,
                                               long long released_episode) {
    // cap enforced in SQL so a stale request can't roll it back
    return ExecutableQuery(
        R"(UPDATE "poster" SET "releasedEpisode" = $3 WHERE "userId" = $1 AND "id" = $2 AND "releasedEpisode" < $3)",
        user_id, poster_id, released_episode);
}

ExecutableQuery set_stamp_favorites(long long user_id, long long stamp_id,
                                    const wire::json& favorite_ids) {
    return ExecutableQuery(
        R"(UPDATE "stamp" SET "favoriteStampMasterIds" = $3 WHERE "userId" = $1 AND "id" = $2)",
        user_id, stamp_id, favorite_ids);
}

std::string set_favorite_costumes(long long user_id, long long character_base_master_id,
                                  const wire::json& favorite_ids, long long new_id) {
    std::vector<json> args{json(user_id), json(character_base_master_id), json(favorite_ids),
                           json(new_id)};
    ExecutableQuery upd =
        eq(R"(UPDATE "favorite_costume" SET "favoriteCostumeMasterIds" = $3 WHERE "userId" = $1 AND "characterBaseMasterId" = $2)",
           args);
    ExecutableQuery ins =
        eq(R"(INSERT INTO "favorite_costume" ("userId", "id", "characterBaseMasterId", "favoriteCostumeMasterIds") SELECT $1, $4, $2, $3)",
           args);
    return composite_update_or_insert(upd, ins);
}

ExecutableQuery add_watch_record(long long user_id, const std::string& table,
                                 const std::string& column, long long new_id,
                                 long long master_id) {
    // plain idempotent insert (valid SQLite as-is); no unique constraint on (userId, masterId)
    std::string sql = "INSERT INTO \"" + table + "\" (\"userId\", \"id\", \"" + column +
                      "\") SELECT $1, $2, $3 WHERE NOT EXISTS (SELECT 1 FROM \"" + table +
                      "\" WHERE \"userId\" = $1 AND \"" + column + "\" = $3)";
    return eq(std::move(sql), {json(user_id), json(new_id), json(master_id)});
}

ExecutableQuery update_sp_rate_point(long long user_id, long long sp_rate_id,
                                     long long point) {
    return ExecutableQuery(
        R"(UPDATE "sp_rate" SET "point" = $3 WHERE "userId" = $1 AND "id" = $2)", user_id,
        sp_rate_id, point);
}

ExecutableQuery breakthrough_poster(long long user_id, long long poster_id,
                                    long long max_phase) {
    return ExecutableQuery(
        R"(UPDATE "poster" SET "breakthroughPhase" = "breakthroughPhase" + 1 WHERE "userId" = $1 AND "id" = $2 AND "breakthroughPhase" < $3)",
        user_id, poster_id, max_phase);
}

std::string add_gacha_rolls(long long user_id, long long gacha_master_id, long long delta,
                            long long new_id) {
    std::vector<json> args{json(user_id), json(gacha_master_id), json(delta), json(new_id)};
    ExecutableQuery upd =
        eq(R"(UPDATE "gacha" SET "rollCount" = "rollCount" + $3 WHERE "userId" = $1 AND "gachaMasterId" = $2)",
           args);
    ExecutableQuery ins =
        eq(R"(INSERT INTO "gacha" ("userId", "id", "gachaMasterId", "rollCount") SELECT $1, $4, $2, $3)",
           args);
    return composite_update_or_insert(upd, ins);
}

ExecutableQuery add_gacha_historys(long long user_id, long long card_type,
                                   const std::vector<long long>& master_ids,
                                   long long created_at) {
    std::vector<std::string> rows;
    std::vector<json> args{json(user_id), json(card_type), json(created_at)};
    for (long long master_id : master_ids) {
        args.push_back(json(master_id));
        rows.push_back("($1, $2, $" + std::to_string(args.size()) + ", $3)");
    }
    std::string sql =
        "INSERT INTO \"gacha_history\" (\"userId\", \"cardType\", \"masterId\", \"createdAt\") "
        "VALUES " +
        join_csv(rows);
    return eq(std::move(sql), std::move(args));
}

std::vector<std::string> set_gacha_selected_things(long long user_id,
                                                   long long gacha_master_id,
                                                   const std::vector<long long>& thing_ids) {
    ExecutableQuery del =
        eq(R"(DELETE FROM "gacha_selected_thing" WHERE "userId" = $1 AND "gachaMasterId" = $2)",
           {json(user_id), json(gacha_master_id)});
    ExecutableQuery ins =
        eq(R"(INSERT INTO "gacha_selected_thing" ("userId", "gachaMasterId", "gachaThingIds") VALUES ($1, $2, $3))",
           {json(user_id), json(gacha_master_id), json(thing_ids)});
    return composite_exec({del, ins});
}

ExecutableQuery update_multi_party(long long user_id, long long party_id) {
    return ExecutableQuery(
        R"(UPDATE "user_preference" SET "multiPartyId" = $2 WHERE "userId" = $1)", user_id,
        party_id);
}

ExecutableQuery update_character_level(long long user_id, long long character_id,
                                       long long level, long long current_experience) {
    return ExecutableQuery(
        R"(UPDATE "character" SET "level" = $3, "currentExperience" = $4 WHERE "userId" = $1 AND "id" = $2)",
        user_id, character_id, level, current_experience);
}

ExecutableQuery update_character_awakening(long long user_id, long long character_id,
                                           long long phase) {
    return ExecutableQuery(
        R"(UPDATE "character" SET "awakeningPhase" = $3 WHERE "userId" = $1 AND "id" = $2)",
        user_id, character_id, phase);
}

ExecutableQuery update_character_talent_stage(long long user_id, long long character_id,
                                              long long stage) {
    return ExecutableQuery(
        R"(UPDATE "character" SET "talentStage" = $3 WHERE "userId" = $1 AND "id" = $2)", user_id,
        character_id, stage);
}

ExecutableQuery update_character_sense_level(long long user_id, long long character_id,
                                             long long level, bool secondary) {
    const char* column = secondary ? "secondarySenseLevel" : "senseLevel";
    std::string sql = std::string("UPDATE \"character\" SET \"") + column +
                      "\" = $3 WHERE \"userId\" = $1 AND \"id\" = $2";
    return eq(std::move(sql), {json(user_id), json(character_id), json(level)});
}

ExecutableQuery update_birth_date(long long user_id, std::optional<long long> birth_date) {
    return ExecutableQuery(
        R"(UPDATE "user_preference" SET "birthDate" = $2 WHERE "userId" = $1)", user_id,
        opt_json(birth_date));
}

SelectQuery adjust_user_stamina_atomic(long long user_id, long long delta,
                                       long long max_stamina, long long interval_micros,
                                       long long now_micros, bool auto_max_clamp) {
    // CTEs p/eff/calc are SELECT-only; only the final UPDATE ... FROM is modifying, which SQLite
    // supports. ::casts stripped; GREATEST/LEAST -> max/min; CEIL(..)::int -> CAST(ceil(..*1.0)..)
    // (the *1.0 forces float division); alias needs AS; RETURNING u2.* -> RETURNING *.
    return SelectQuery(
        "UserModel",
        R"SQL(
        WITH p AS (
            SELECT $1 AS uid, $2 AS delta, $3 AS maxst,
                   $4 AS interval, $5 AS now, $6 AS clamp
        ),
        eff AS (
            SELECT u.*, p.delta, p.maxst, p.interval, p.now, p.clamp,
                CASE
                    WHEN u."currentStamina" >= p.maxst THEN u."currentStamina"
                    WHEN p.interval <= 0 OR p.now >= u."maxStaminaRestoredAt" THEN p.maxst
                    ELSE max(
                        0,
                        p.maxst - CAST(ceil((u."maxStaminaRestoredAt" - p.now) * 1.0 / p.interval) AS INTEGER)
                    )
                END AS effective
            FROM "user" u JOIN p ON u."userId" = p.uid
        ),
        calc AS (
            SELECT e.*,
                CASE WHEN e.delta > 0 AND e.clamp
                     THEN min(e.maxst, e.effective + e.delta)
                     ELSE e.effective + e.delta
                END AS new_st
            FROM eff e
        )
        UPDATE "user" AS u2 SET
            "currentStamina" = c.new_st,
            "maxStaminaRestoredAt" = CASE
                WHEN c.new_st >= c.maxst THEN c.now
                WHEN c.interval > 0 THEN c.now + (c.maxst - c.new_st) * c.interval
                ELSE c.now
            END
        FROM calc c
        WHERE u2."userId" = c."userId"
          AND c.new_st >= 0
          AND (c.clamp OR c.delta <= 0 OR c.new_st <= c.maxst)
        RETURNING *
        )SQL",
        user_id, delta, max_stamina, interval_micros, now_micros, auto_max_clamp);
}

std::vector<std::string> replace_market_things(
    long long user_id,
    const std::vector<std::tuple<long long, long long, std::optional<long long>>>&
        frames) {
    if (frames.empty()) {
        ExecutableQuery del =
            eq(R"(DELETE FROM "market_thing" WHERE "userId" = $1)", {json(user_id)});
        return composite_exec({del});
    }
    std::vector<std::string> rows;
    std::vector<json> args{json(user_id)};
    for (const std::tuple<long long, long long, std::optional<long long>>& f : frames) {
        args.push_back(json(std::get<0>(f)));
        args.push_back(json(std::get<1>(f)));
        args.push_back(opt_json(std::get<2>(f)));
        std::size_t n = args.size();
        rows.push_back("($1, $" + std::to_string(n - 2) + ", $" + std::to_string(n - 1) + ", $" +
                       std::to_string(n) + ")");
    }
    ExecutableQuery del =
        eq(R"(DELETE FROM "market_thing" WHERE "userId" = $1)", {json(user_id)});
    std::string ins_sql =
        "INSERT INTO \"market_thing\" (\"userId\", \"frameNumber\", "
        "\"marketFrameThingMasterId\", \"discountPercent\") VALUES " +
        join_csv(rows);
    ExecutableQuery ins = eq(std::move(ins_sql), std::move(args));
    return composite_exec({del, ins});
}

SelectQuery purchase_market_thing(long long user_id, long long frame_number) {
    return SelectQuery(
        "MarketThingModel",
        R"(UPDATE "market_thing" SET "hasPurchased" = TRUE WHERE "userId" = $1 AND "frameNumber" = $2 AND "hasPurchased" = FALSE RETURNING *)",
        user_id, frame_number);
}

std::optional<json> roll_over_market(long long user_id, long long now, long long new_id,
                                     long long reset_at) {
    std::vector<json> args{json(user_id), json(now), json(new_id), json(reset_at)};
    SelectQuery upd = sq(
        "MarketModel",
        R"(UPDATE "market" SET "lastRefreshedAt" = $2, "refreshTimes" = 0 WHERE "userId" = $1 AND "lastRefreshedAt" < $4 RETURNING *)",
        args);
    // the upd-guard (NOT EXISTS SELECT 1 FROM upd) is replaced by composite ordering: ins runs
    // only when upd returned no row
    SelectQuery ins = sq(
        "MarketModel",
        R"(INSERT INTO "market" ("userId", "id", "lastRefreshedAt", "refreshTimes") SELECT $1, $3, $2, 0 WHERE NOT EXISTS (SELECT 1 FROM "market" WHERE "userId" = $1) RETURNING *)",
        args);
    return first_or_none(composite_upsert_returning(upd, ins));
}

std::optional<json> consume_market_refresh(long long user_id, long long now,
                                           long long new_id, long long reset_at,
                                           long long max_refreshes) {
    std::string counted = R"(CASE WHEN "lastRefreshedAt" < $4 THEN 1 ELSE "refreshTimes" + 1 END)";
    std::vector<json> args{json(user_id), json(now), json(new_id), json(reset_at),
                           json(max_refreshes)};
    SelectQuery upd = sq("MarketModel",
                         "UPDATE \"market\" SET \"refreshTimes\" = " + counted +
                             ", \"lastRefreshedAt\" = $2 WHERE \"userId\" = $1 AND (" + counted +
                             ") <= $5 RETURNING *",
                         args);
    SelectQuery ins = sq(
        "MarketModel",
        R"(INSERT INTO "market" ("userId", "id", "lastRefreshedAt", "refreshTimes") SELECT $1, $3, $2, 1 WHERE NOT EXISTS (SELECT 1 FROM "market" WHERE "userId" = $1) AND 1 <= $5 RETURNING *)",
        args);
    return first_or_none(composite_upsert_returning(upd, ins));
}

std::optional<json> consume_exchange_limit(long long user_id,
                                           long long exchange_shop_thing_id,
                                           long long quantity, long long limit,
                                           long long replace_type,
                                           std::optional<long long> until, long long now,
                                           long long new_id) {
    std::string expired =
        R"(CASE WHEN "until" IS NOT NULL AND "until" <= $7 THEN $3 ELSE "exchangedCount" + $3 END)";
    std::vector<json> args{json(user_id),     json(exchange_shop_thing_id),
                           json(quantity),    json(limit),
                           json(replace_type), opt_json(until),
                           json(now),          json(new_id)};
    SelectQuery upd = sq("ExchangeLimitModel",
                         "UPDATE \"exchange_limit\" SET \"exchangedCount\" = " + expired +
                             ", \"until\" = $6, \"replaceType\" = $5 WHERE \"userId\" = $1 AND "
                             "\"exchangeShopThingId\" = $2 AND (" +
                             expired + ") <= $4 RETURNING *",
                         args);
    SelectQuery ins = sq(
        "ExchangeLimitModel",
        R"(INSERT INTO "exchange_limit" ("userId", "id", "exchangeShopThingId", "replaceType", "exchangedCount", "until") SELECT $1, $8, $2, $5, $3, $6 WHERE NOT EXISTS (SELECT 1 FROM "exchange_limit" WHERE "userId" = $1 AND "exchangeShopThingId" = $2) AND $3 <= $4 RETURNING *)",
        args);
    return first_or_none(composite_upsert_returning(upd, ins));
}

std::optional<json> consume_permanent_market_limit(long long user_id, long long master_id,
                                                   long long quantity, long long limit) {
    std::vector<json> args{json(user_id), json(master_id), json(quantity), json(limit)};
    SelectQuery upd = sq(
        "PermanentMarketThingModel",
        R"(UPDATE "permanent_market_thing" SET "purchaseCount" = "purchaseCount" + $3 WHERE "userId" = $1 AND "permanentMarketThingMasterId" = $2 AND "purchaseCount" + $3 <= $4 RETURNING *)",
        args);
    SelectQuery ins = sq(
        "PermanentMarketThingModel",
        R"(INSERT INTO "permanent_market_thing" ("userId", "permanentMarketThingMasterId", "purchaseCount") SELECT $1, $2, $3 WHERE NOT EXISTS (SELECT 1 FROM "permanent_market_thing" WHERE "userId" = $1 AND "permanentMarketThingMasterId" = $2) AND $3 <= $4 RETURNING *)",
        args);
    return first_or_none(composite_upsert_returning(upd, ins));
}

SelectQuery release_music_olivier(long long user_id, long long music_master_id,
                                  long long purchasable, long long released) {
    return SelectQuery(
        "MusicModel",
        R"(UPDATE "music" SET "olivierReleaseStatus" = $4 WHERE "userId" = $1 AND "musicMasterId" = $2 AND "olivierReleaseStatus" = $3 RETURNING *)",
        user_id, music_master_id, purchasable, released);
}

std::string record_jewel_shop_purchase(long long user_id,
                                        long long jewel_shop_item_master_id,
                                        long long new_id,
                                        std::optional<long long> re_purchase_date) {
    std::vector<json> args{json(user_id), json(jewel_shop_item_master_id), json(new_id),
                           opt_json(re_purchase_date)};
    ExecutableQuery upd =
        eq(R"(UPDATE "jewel_shop" SET "purchaseCount" = "purchaseCount" + 1, "totalPurchaseCount" = "totalPurchaseCount" + 1, "rePurchaseDate" = $4 WHERE "userId" = $1 AND "jewelShopItemMasterId" = $2)",
           args);
    ExecutableQuery ins =
        eq(R"(INSERT INTO "jewel_shop" ("userId", "id", "jewelShopItemMasterId", "purchaseCount", "totalPurchaseCount", "rePurchaseDate") SELECT $1, $3, $2, 1, 1, $4)",
           args);
    return composite_update_or_insert(upd, ins);
}

std::string touch_viewed_shop(long long user_id, long long category,
                              std::optional<long long> exchange_shop_master_id, long long now,
                              long long new_id) {
    // exchangeShopMasterId is nullable; IS NOT DISTINCT FROM -> SQLite null-safe IS
    std::vector<json> args{json(user_id), json(category), opt_json(exchange_shop_master_id),
                           json(now), json(new_id)};
    ExecutableQuery upd =
        eq(R"(UPDATE "viewed_shop" SET "lastViewedAt" = $4 WHERE "userId" = $1 AND "viewedShopCategory" = $2 AND "exchangeShopMasterId" IS $3)",
           args);
    ExecutableQuery ins =
        eq(R"(INSERT INTO "viewed_shop" ("userId", "id", "exchangeShopMasterId", "lastViewedAt", "viewedShopCategory") SELECT $1, $5, $3, $4, $2)",
           args);
    return composite_update_or_insert(upd, ins);
}

}  // namespace user
}  // namespace db
