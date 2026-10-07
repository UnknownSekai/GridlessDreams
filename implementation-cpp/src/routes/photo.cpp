#include "routes/photo.h"

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <map>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "generated/enums_generated.h"
#include "helpers/game_state.h"
#include "master_data.h"
#include "pipeline.h"
#include "platform.h"
#include "wire.h"

// ports routes/photo.py. WatchMusicVideo/WatchTheaterStory/GeneratePhoto/Album*Arranging/
// SetCharacterBaseTags/GetAlbumMainPage carry real logic (raw positional msgpack arrays,
// not KEYS models); the rest are stubs. PIL JPEG verification is replaced by a minimal
// SOI/format header check (port plan), so dimension validation is dropped.

namespace routes {
namespace {

using ojson = nlohmann::ordered_json;  // request/wire json
using rjson = nlohmann::json;          // db/state json (camelCase columns)

// photos live under the writable data dir, matching the platform _data convention
std::string photo_root() {
    static const std::string root = platform::get_writable_dir() + "/_data/photos";
    return root;
}

bool ends_with(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// explicit conversions between the request (ordered) and db (unordered) json flavours;
// mixing them in an overloaded call (operator==, push_back) is otherwise ambiguous
ojson as_ojson(const rjson& v) { return v; }
rjson as_rjson(const ojson& v) { return v; }

// GeneratePhotoResult() with its python model defaults: rarity defaults to PhotoRarities.Rare1
// (=1), not default(enum)=0, so an empty wire object would mis-encode slot 3 as 0.
ojson default_generate_photo_result() {
    ojson r;
    r["rarity"] = enums::PhotoRarities::Rare1;
    return r;
}

// python truthiness for a raw decoded value
bool truthy(const ojson& v) {
    if (v.is_null()) return false;
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number_integer()) return v.get<int64_t>() != 0;
    if (v.is_number_unsigned()) return v.get<uint64_t>() != 0;
    if (v.is_number_float()) return v.get<double>() != 0.0;
    if (v.is_string()) return !v.get_ref<const std::string&>().empty();
    if (v.is_array() || v.is_object()) return !v.empty();
    return true;
}

// ---- local filesystem helpers (direct I/O; read + write share one base) ----

void make_dir(const std::string& p) {
#ifdef _WIN32
    _mkdir(p.c_str());
#else
    ::mkdir(p.c_str(), 0755);
#endif
}

void mkdirs(const std::string& path) {
    std::string cur;
    for (size_t i = 0; i < path.size(); ++i) {
        cur += path[i];
        if (path[i] == '/') make_dir(cur);
    }
    make_dir(path);
}

bool is_file(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;
    return (st.st_mode & S_IFREG) != 0;
}

std::string read_all(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) return std::string();
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// mirrors python path.open("xb"): create+write, failing if the file already exists
void write_exclusive(const std::string& path, const std::string& data) {
    auto slash = path.rfind('/');
    if (slash != std::string::npos) mkdirs(path.substr(0, slash));
    if (is_file(path)) throw std::runtime_error("photo file already exists");
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f.is_open()) throw std::runtime_error("cannot open photo file");
    f.write(data.data(), static_cast<std::streamsize>(data.size()));
    if (!f.good()) throw std::runtime_error("photo write failed");
}

std::string new_uuid() {
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> d(0, 255);
    unsigned char b[16];
    for (unsigned char& x : b) x = static_cast<unsigned char>(d(rng));
    b[6] = static_cast<unsigned char>((b[6] & 0x0F) | 0x40);  // version 4
    b[8] = static_cast<unsigned char>((b[8] & 0x3F) | 0x80);  // variant 10xx
    char buf[37];
    std::snprintf(buf, sizeof(buf),
                  "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", b[0], b[1],
                  b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11], b[12], b[13], b[14],
                  b[15]);
    return std::string(buf);
}

std::string replace_all(std::string s, const std::string& from, const std::string& to) {
    if (from.empty()) return s;
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
    return s;
}

// mimics python uuid.UUID(hex): strip urn:/uuid: prefixes, braces and dashes, then
// require exactly 32 hex digits. doubles as the path-traversal gate for photo_image.
bool valid_uuid(std::string s) {
    s = replace_all(s, "urn:", "");
    s = replace_all(s, "uuid:", "");
    size_t start = s.find_first_not_of("{}");
    size_t end = s.find_last_not_of("{}");
    if (start == std::string::npos)
        s.clear();
    else
        s = s.substr(start, end - start + 1);
    s = replace_all(s, "-", "");
    if (s.size() != 32) return false;
    for (char c : s)
        if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

// ---- photo logic ----

// upsert a MusicVideo/TheaterStory watch row; valid is the master-existence check
void watch(httplib::Response& res, const httplib::Request& req, const std::string& entity,
           const std::string& field, int64_t ident, bool valid) {
    try {
        if (ident == 0 || !valid) throw game_state::Rejected();
        game_state::State s = game_state::transaction(req);
        rjson* row = s.one(entity, rjson{{field, ident}});
        if (row)
            s.update(entity, row, rjson{{field, ident}});
        else
            s.insert(entity, rjson{{field, ident}});
        s.commit();
        ojson result;
        result["is_success"] = true;
        pipeline::respond(res, "BooleanResult", result, ojson::array(), s.present());
    } catch (const game_state::Rejected&) {
        pipeline::respond(res, "BooleanResult", ojson::object());
    }
}

// exact server lottery weights were never exposed; preserve the documented minimum tier.
int64_t film_rarity(const ojson& film_j) {
    if (!film_j.is_number_integer() && !film_j.is_number_unsigned())
        throw game_state::Rejected("Unsupported film");
    int64_t film = film_j.get<int64_t>();
    if (!(510001 <= film && film <= 510006)) throw game_state::Rejected("Unsupported film");
    return std::min<int64_t>(film - 510000, 5);
}

// minimal JPEG header check (replaces PIL verify); returns the raw bytes to store
std::string jpeg_bytes(const ojson& data) {
    if (!data.is_array() || data.size() < 4 || data.size() > 12000000)
        throw game_state::Rejected("Invalid photo image");
    std::string out;
    out.reserve(data.size());
    for (const ojson& b : data) {
        if (!b.is_number_integer() && !b.is_number_unsigned())
            throw game_state::Rejected("Invalid photo image");
        int64_t v = b.get<int64_t>();
        if (v < 0 || v > 255) throw game_state::Rejected("Invalid photo image");
        out.push_back(static_cast<char>(v));
    }
    if (static_cast<unsigned char>(out[0]) != 0xFF || static_cast<unsigned char>(out[1]) != 0xD8)
        throw game_state::Rejected("Invalid photo image");
    if (static_cast<unsigned char>(out[2]) != 0xFF) throw game_state::Rejected("Invalid JPEG");
    return out;
}

// raw[...] is a msgpack bin (array of bytes here); decode its nested album layout
ojson unpack_items(const ojson& raw) {
    if (!raw.is_array()) throw game_state::Rejected("Invalid album layout");
    std::string body;
    body.reserve(raw.size());
    for (const ojson& b : raw) {
        if (!b.is_number_integer() && !b.is_number_unsigned())
            throw game_state::Rejected("Invalid album layout");
        int64_t v = b.get<int64_t>();
        if (v < 0 || v > 255) throw game_state::Rejected("Invalid album layout");
        body.push_back(static_cast<char>(v));
    }
    ojson value = wire::read_request(body, nullptr);
    if (!value.is_array() || value.size() != 1 || !value[0].is_array() || value[0].size() > 200)
        throw game_state::Rejected("Invalid album layout");
    return value[0];
}

void album_arranging(const httplib::Request& req, httplib::Response& res) {
    try {
        ojson p = pipeline::read_request(req);
        if (!p.is_array() || p.size() != 4) throw game_state::Rejected("Invalid album");
        const ojson& publishing = p[0];
        const ojson& page = p[1];
        const ojson& raw = p[2];
        const ojson& theme = p[3];
        if (!(page.is_number_integer() || page.is_number_unsigned()))
            throw game_state::Rejected("Invalid album page");
        int64_t pg = page.get<int64_t>();
        if (!((1 <= pg && pg <= 10) || (101 <= pg && pg <= 110)))
            throw game_state::Rejected("Invalid album page");
        bool detailed = ends_with(req.path, "DetailArranging");
        ojson items = unpack_items(raw);

        game_state::State s = game_state::transaction(req);
        std::vector<int64_t> selected;
        for (ojson& item : items) {
            size_t need = detailed ? 12 : 5;
            if (!item.is_array() || item.size() != need)
                throw game_state::Rejected("Invalid album item");
            ojson kind = detailed ? item[1] : ojson(1);
            if (kind == 1) {
                rjson* photo = s.one("Photo", rjson{{"id", item[0]}});
                if (!photo) throw game_state::Rejected("Photo is not owned");
                int64_t pid = photo->at("id").get<int64_t>();
                if (std::find(selected.begin(), selected.end(), pid) != selected.end())
                    throw game_state::Rejected("Duplicate photo");
                selected.push_back(pid);
                size_t offset = detailed ? 2 : 1;
                item[offset] = as_ojson(photo->at("fileName"));
                item[offset + 1] = as_ojson(photo->at("sasToken"));
            } else if (kind == 3) {
                if (!(item[0].is_number() &&
                      game_state::master("album_theme_master", item[0].get<int64_t>())))
                    throw game_state::Rejected("Unknown theme");
            } else if (kind == 2) {
                const ojson* decoration =
                    item[0].is_number() ? game_state::master("decoration_master",
                                                             item[0].get<int64_t>())
                                        : nullptr;
                if (!decoration ||
                    (!decoration->at("is_default").get<bool>() &&
                     !s.one("Decoration", rjson{{"decorationMasterId", item[0]}})))
                    throw game_state::Rejected("Decoration is not owned");
            } else if (kind == 4) {
                const ojson* stamp = item[0].is_number()
                                         ? game_state::master("stamp_master", item[0].get<int64_t>())
                                         : nullptr;
                std::vector<rjson> owned;
                for (rjson& row : s.rows("Stamp")) {
                    const rjson& ids = row.value("stampMasterIds", rjson(nullptr));
                    if (ids.is_array())
                        for (const rjson& ident : ids) owned.push_back(ident);
                }
                rjson target = as_rjson(item[0]);
                bool in_owned = false;
                for (const rjson& o : owned)
                    if (o == target) {
                        in_owned = true;
                        break;
                    }
                if (!stamp || (!stamp->at("is_default").get<bool>() && !in_owned))
                    throw game_state::Rejected("Stamp is not owned");
            } else {
                throw game_state::Rejected("Unknown album item type");
            }
        }
        if (!theme.is_null()) {
            const ojson* tm = theme.is_number()
                                  ? game_state::master("album_theme_master", theme.get<int64_t>())
                                  : nullptr;
            if (!tm) throw game_state::Rejected("Unknown theme");
        }
        if (detailed) {
            std::vector<ojson> tmp(items.begin(), items.end());
            std::stable_sort(tmp.begin(), tmp.end(),
                             [](const ojson& a, const ojson& b) { return a[1] < b[1]; });
            ojson sorted = ojson::array();
            for (ojson& x : tmp) sorted.push_back(std::move(x));
            items = std::move(sorted);
        }
        bool normal = pg < 100;
        int64_t mask = normal ? 0 : (int64_t{1} << (pg - 101));
        for (rjson& photo : s.rows("Photo")) {
            int64_t pid = photo.at("id").get<int64_t>();
            bool is_sel = std::find(selected.begin(), selected.end(), pid) != selected.end();
            if (normal) {
                if (is_sel) {
                    const rjson& ua = photo.at("useAlbumPage");
                    bool in_set = ua.is_null() || ua == 0 || ua == pg;
                    if (!in_set)
                        throw game_state::Rejected("Photo is already in another album page");
                    s.update("Photo", &photo, rjson{{"useAlbumPage", pg}});
                } else if (photo.at("useAlbumPage") == pg) {
                    s.update("Photo", &photo, rjson{{"useAlbumPage", nullptr}});
                }
            } else {
                int64_t deco = photo.at("useDecoPage").get<int64_t>();
                int64_t flags = is_sel ? (deco | mask) : (deco & ~mask);
                if (flags != deco) s.update("Photo", &photo, rjson{{"useDecoPage", flags}});
            }
        }

        ojson wrapped = ojson::array();
        wrapped.push_back(items);
        std::string packed = wire::pack(nullptr, wrapped);
        rjson items_bytes = rjson::array();
        for (unsigned char c : packed) items_bytes.push_back(static_cast<int>(c));

        rjson values;
        values["page"] = pg;
        values["editType"] = detailed ? 2 : 1;
        values["publishing"] = truthy(publishing) && normal;
        values["items"] = items_bytes;
        values["albumThemeMasterId"] = detailed ? rjson(nullptr) : as_rjson(theme);

        rjson* page_row = s.one("AlbumPage", rjson{{"page", pg}});
        if (page_row)
            s.update("AlbumPage", page_row, values);
        else
            s.insert("AlbumPage", values);

        rjson* album = s.one("Album");
        if (!album) {
            s.insert("Album",
                     rjson{{"level", 1}, {"publishPageNumber", 1}, {"currentPresetOrder", 1}});
            album = s.one("Album");
        }
        if (!selected.empty() && album->at("level") == 0)
            s.update("Album", album, rjson{{"level", 1}});
        if (truthy(publishing) && normal) {
            s.update("Album", album, rjson{{"publishPageNumber", pg}});
            for (rjson& other : s.rows("AlbumPage"))
                if (other.at("page") != pg && other.at("publishing").get<bool>())
                    s.update("AlbumPage", &other, rjson{{"publishing", false}});
        }
        game_state::mission_progress(s, 11, selected.empty() ? 0 : 1, true);
        s.commit();
        ojson result;
        result["is_success"] = true;
        pipeline::respond(res, "BooleanResult", result, ojson::array(), s.present());
    } catch (const game_state::Rejected& e) {
        pipeline::respond(res, "BooleanResult", ojson::object(),
                          ojson::array({pipeline::fault("InvalidAlbum", e.what())}));
    }
}

}  // namespace

void register_photo(httplib::Server& svr) {
    // /api/Photo/WatchMusicVideo?mMusicVideoId=
    svr.Post("/api/Photo/WatchMusicVideo", [](const httplib::Request& req, httplib::Response& res) {
        int64_t m_music_video_id =
            req.has_param("mMusicVideoId") ? std::stoll(req.get_param_value("mMusicVideoId")) : 0;
        watch(res, req, "MusicVideo", "musicVideoMasterId", m_music_video_id,
              game_state::master("music_video_master", m_music_video_id) != nullptr);
    });

    // /api/Photo/WatchTheaterStory?mTheaterStoryId=
    svr.Post("/api/Photo/WatchTheaterStory",
             [](const httplib::Request& req, httplib::Response& res) {
                 int64_t m_theater_story_id =
                     req.has_param("mTheaterStoryId")
                         ? std::stoll(req.get_param_value("mTheaterStoryId"))
                         : 0;
                 bool valid = false;
                 for (const ojson& chapter : master_data::table("TheaterChapterMaster")) {
                     auto it = chapter.find("stories");
                     if (it != chapter.end() && it->is_array())
                         for (const ojson& story : *it)
                             if (story.value("id_", static_cast<int64_t>(0)) == m_theater_story_id) {
                                 valid = true;
                                 break;
                             }
                     if (valid) break;
                 }
                 watch(res, req, "TheaterStory", "theaterStoryMasterId", m_theater_story_id, valid);
             });

    // /photo/{variant}/{filename}  (local photo image serving)
    svr.Get(R"(/photo/([^/]+)/([^/]+))", [](const httplib::Request& req, httplib::Response& res) {
        const std::string variant = req.matches[1].str();
        const std::string filename = req.matches[2].str();
        if (!(variant == "original" || variant == "thumbnail") || !ends_with(filename, ".jpg")) {
            res.status = httplib::StatusCode::NotFound_404;
            return;
        }
        if (!valid_uuid(filename.substr(0, filename.size() - 4))) {
            res.status = httplib::StatusCode::NotFound_404;
            return;
        }
        const std::string path = photo_root() + "/" + variant + "/" + filename;
        if (!is_file(path)) {
            res.status = httplib::StatusCode::NotFound_404;
            return;
        }
        res.set_content(read_all(path), "image/jpeg");
    });

    // /api/Photos/GeneratePhoto
    svr.Post("/api/Photos/GeneratePhoto", [](const httplib::Request& req, httplib::Response& res) {
        std::vector<std::string> saved;
        try {
            ojson p = pipeline::read_request(req);
            if (!p.is_array() || p.size() != 5 || !p[0].is_null())
                throw game_state::Rejected("Unsupported photo request");
            int64_t rarity = film_rarity(p[1]);
            std::string original = jpeg_bytes(p[2]);
            std::string thumbnail = jpeg_bytes(p[3]);
            if (!p[4].is_array() || p[4].size() > 30)
                throw game_state::Rejected("Invalid characters");
            std::vector<ojson> chars;
            for (const ojson& c : p[4])
                if (c.is_array() && c.size() == 2) {
                    const ojson& cid = c[0];
                    if (std::find(chars.begin(), chars.end(), cid) == chars.end())
                        chars.push_back(cid);
                }
            bool bad = static_cast<long long>(chars.size()) != static_cast<long long>(p[4].size());
            if (!bad)
                for (const ojson& cid : chars)
                    if (!cid.is_number() ||
                        !game_state::master("character_base_master", cid.get<int64_t>())) {
                        bad = true;
                        break;
                    }
            if (bad) throw game_state::Rejected("Invalid character");
            std::string name = new_uuid() + ".jpg";

            game_state::State s = game_state::transaction(req);
            s.pay({{p[1].get<int64_t>(), 1}});
            std::vector<std::pair<std::string, std::string>> variants = {{"original", original},
                                                                         {"thumbnail", thumbnail}};
            for (const auto& vd : variants) {
                std::string path = photo_root() + "/" + vd.first + "/" + name;
                write_exclusive(path, vd.second);
                saved.push_back(path);
            }
            ojson chars_json = ojson::array();
            for (const ojson& cid : chars) chars_json.push_back(cid);
            long long generated_at = std::chrono::duration_cast<std::chrono::microseconds>(
                                         std::chrono::system_clock::now().time_since_epoch())
                                         .count();
            rjson values;
            values["fileName"] = name;
            values["sasToken"] = "";
            values["photoEffectMasterId"] = nullptr;
            values["lock"] = false;
            values["useAlbumPage"] = nullptr;
            values["level"] = 1;
            values["rarity"] = rarity;
            values["signMasterId"] = nullptr;
            values["generatedAt"] = generated_at;
            values["thumbnailSasToken"] = "";
            values["appearedCharacterBaseMasterIds"] = as_rjson(chars_json);
            values["taggedCharacterBaseMasterIds"] = as_rjson(chars_json);
            values["useDecoPage"] = 0;
            rjson row = s.insert("Photo", values);
            for (int64_t ident : {static_cast<int64_t>(200300), static_cast<int64_t>(10)})
                game_state::mission_progress(s, ident, 1, true);
            for (const ojson& cid : chars)
                game_state::character_progress(s, cid.get<int64_t>(), 9, 1);
            s.commit();
            ojson result;
            result["photo_id"] = row.at("id").get<int64_t>();
            result["file_name"] = name;
            result["sas_token"] = "";
            result["rarity"] = rarity;
            pipeline::respond(res, "GeneratePhotoResult", result, ojson::array(), s.present());
        } catch (const game_state::Rejected& e) {
            for (const std::string& path : saved) std::remove(path.c_str());
            pipeline::respond(res, "GeneratePhotoResult", default_generate_photo_result(),
                              ojson::array({pipeline::fault("InvalidPhoto", e.what())}));
        } catch (...) {
            for (const std::string& path : saved) std::remove(path.c_str());
            throw;
        }
    });

    // /api/Photos/FinishGeneratePhoto
    svr.Post("/api/Photos/FinishGeneratePhoto",
             [](const httplib::Request& req, httplib::Response& res) {
                 {
                     game_state::State s = game_state::transaction(req);
                     s.commit();
                 }
                 ojson result;
                 result["is_success"] = true;
                 pipeline::respond(res, "BooleanResult", result);
             });

    // /api/Photo/AlbumSimpleArranging  &  /api/Photo/AlbumDetailArranging
    svr.Post("/api/Photo/AlbumSimpleArranging", album_arranging);
    svr.Post("/api/Photo/AlbumDetailArranging", album_arranging);

    // /api/Photo/SetCharacterBaseTags
    svr.Post("/api/Photo/SetCharacterBaseTags",
             [](const httplib::Request& req, httplib::Response& res) {
                 try {
                     ojson p = pipeline::read_request(req);
                     if (!p.is_array() || p.size() != 2 || !p[1].is_array() || p[1].size() > 100)
                         throw game_state::Rejected();
                     for (const ojson& c : p[1])
                         if (!c.is_number() ||
                             !game_state::master("character_base_master", c.get<int64_t>()))
                             throw game_state::Rejected();
                     game_state::State s = game_state::transaction(req);
                     rjson* row = s.one("Photo", rjson{{"id", p[0]}});
                     if (!row) throw game_state::Rejected();
                     ojson deduped = ojson::array();
                     for (const ojson& c : p[1]) {
                         bool found = false;
                         for (const ojson& d : deduped)
                             if (d == c) {
                                 found = true;
                                 break;
                             }
                         if (!found) deduped.push_back(c);
                     }
                     s.update("Photo", row,
                              rjson{{"taggedCharacterBaseMasterIds", as_rjson(deduped)}});
                     s.commit();
                     ojson result;
                     result["is_success"] = true;
                     pipeline::respond(res, "BooleanResult", result, ojson::array(), s.present());
                 } catch (const game_state::Rejected&) {
                     pipeline::respond(res, "BooleanResult", ojson::object());
                 }
             });

    // /api/Photo/GetAlbumMainPage?targetUserId=
    svr.Post("/api/Photo/GetAlbumMainPage",
             [](const httplib::Request& req, httplib::Response& res) {
                 game_state::State s = game_state::transaction(req);
                 std::string target =
                     req.has_param("targetUserId") ? req.get_param_value("targetUserId") : "";
                 ojson out = ojson::array();
                 if (!target.empty() && target != std::to_string(s.uid)) {
                     out.push_back(ojson());
                     out.push_back(false);
                 } else {
                     rjson* album = s.one("Album");
                     rjson* page = album ? s.one("AlbumPage",
                                                 rjson{{"page", album->at("publishPageNumber")}})
                                         : nullptr;
                     if (!page) {
                         out.push_back(ojson());
                         out.push_back(false);
                     } else {
                         const rjson& stored = page->at("items");
                         std::vector<std::uint8_t> bytes;
                         if (stored.is_array())
                             for (const rjson& b : stored)
                                 bytes.push_back(static_cast<std::uint8_t>(b.get<int>()));
                         ojson inner = ojson::array();
                         inner.push_back(as_ojson(page->at("id")));
                         inner.push_back(as_ojson(album->at("id")));
                         inner.push_back(as_ojson(album->at("level")));
                         inner.push_back(as_ojson(page->at("page")));
                         inner.push_back(as_ojson(page->at("editType")));
                         inner.push_back(as_ojson(page->at("publishing")));
                         inner.push_back(ojson::binary(bytes));
                         inner.push_back(as_ojson(page->at("albumThemeMasterId")));
                         out.push_back(inner);
                         out.push_back(true);
                     }
                 }
                 s.commit();
                 pipeline::respond(res, nullptr, out);
             });

    // ---- remaining photo endpoints (not yet implemented) ----

    // /api/Photo/AbilityVarietyUp
    svr.Post("/api/Photo/AbilityVarietyUp", [](const httplib::Request& req, httplib::Response& res) {
        (void)pipeline::read_request(req, "AbilityVarietyUpPayload");
        pipeline::respond(res, "BooleanResult", ojson::object());
    });

    // /api/Photo/AlbumReset?isAllReset=
    svr.Post("/api/Photo/AlbumReset", [](const httplib::Request&, httplib::Response& res) {
        pipeline::respond(res, "BooleanResult", ojson::object());
    });

    // /api/Photo/ChangePhotoAbility
    svr.Post("/api/Photo/ChangePhotoAbility",
             [](const httplib::Request& req, httplib::Response& res) {
                 (void)pipeline::read_request(req, "ChangePhotoAbilityPayload");
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });

    // /api/Photo/Preset/Change/{presetOrder}
    svr.Post("/api/Photo/Preset/Change/:presetOrder",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });

    // /api/Photos/GeneratePhotos
    svr.Post("/api/Photos/GeneratePhotos", [](const httplib::Request& req, httplib::Response& res) {
        (void)pipeline::read_request(req, "GeneratePhotosPayload");
        pipeline::respond(res, "GeneratePhotoResult",
                          ojson::array({default_generate_photo_result()}));
    });

    // /api/Photo/IncreaseAcquirablePhotoLimit?toPhase=
    svr.Post("/api/Photo/IncreaseAcquirablePhotoLimit",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });

    // /api/Photos/PhotoLevelUp
    svr.Post("/api/Photos/PhotoLevelUp", [](const httplib::Request& req, httplib::Response& res) {
        (void)pipeline::read_request(req, "LevelUpPhotoPayload");
        pipeline::respond(res, "LevelUpPhotoResult", ojson::object());
    });

    // /api/Photos/RegeneratePhoto?uPhotoId=
    svr.Post("/api/Photos/RegeneratePhoto", [](const httplib::Request& req, httplib::Response& res) {
        (void)pipeline::read_request(req, "GeneratePhotoPayload");
        pipeline::respond(res, "GeneratePhotoResult", default_generate_photo_result());
    });

    // /api/Photos/Sell
    svr.Post("/api/Photos/Sell", [](const httplib::Request& req, httplib::Response& res) {
        (void)pipeline::read_request(req);
        pipeline::respond(res, "BooleanResult", ojson::object());
    });

    // /api/Photo/SetAlbumPublishing
    svr.Post("/api/Photo/SetAlbumPublishing",
             [](const httplib::Request& req, httplib::Response& res) {
                 (void)pipeline::read_request(req, "SetAlbumPublishingPayload");
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });

    // /api/Photo/Preset/SetName/{presetOrder}
    svr.Post("/api/Photo/Preset/SetName/:presetOrder",
             [](const httplib::Request& req, httplib::Response& res) {
                 (void)pipeline::read_request(req);
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });

    // /api/Photos/{photoId}/SwitchLock
    svr.Post("/api/Photos/:photoId/SwitchLock",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", ojson::object());
             });
}

}  // namespace routes
