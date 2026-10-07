#include "routes/posters.h"

#include "pipeline.h"
#include "wire.h"

// ports routes/posters.py. every handler is a stub returning BooleanResult().

namespace routes {

using wire::json;

void register_posters(httplib::Server& svr) {
    svr.Post("/api/Posters/:uPosterId/Breakthrough/:phaseTo",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    svr.Post("/api/Posters/ChangePosterAlternativeImage",
             [](const httplib::Request& req, httplib::Response& res) {
                 pipeline::read_request(req, "PosterAlternativeImagePayload");
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    svr.Post("/api/Posters/:uPosterId/LevelUp/:levelTo",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });

    svr.Post("/api/Posters/SetFavorite", [](const httplib::Request& req, httplib::Response& res) {
        pipeline::read_request(req, "PosterFavoritePayload");
        pipeline::respond(res, "BooleanResult", json::object());
    });

    svr.Post("/api/Posters/:uPosterId/UpdateReleasedPosterStory/:posterEpisodeType",
             [](const httplib::Request&, httplib::Response& res) {
                 pipeline::respond(res, "BooleanResult", json::object());
             });
}

}  // namespace routes
