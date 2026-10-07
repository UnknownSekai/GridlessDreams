#include "environment.h"

#include "config.h"

namespace environment {

wire::json environment() {
    wire::json result = wire::json::object();
    result["application_version"] = config::get_str("server_version");
    result["asset_version"] = config::get_str("asset_version");
    result["api_endpoint"] = config::get_str("api_endpoint");
    result["maintenance_api_endpoint"] = nullptr;
    result["news_api_endpoint"] = nullptr;
    result["is_maintenance"] = config::get_bool("maintenance");
    result["master_data_url"] = config::get_str("master_data_url");
    result["static_content_url"] = config::get_str("static_content_url");
    result["asset_url"] = config::get_str("asset_url");
    result["is_app_review"] = false;
    result["photo_content_url"] = config::get_str("photo_content_url");
    result["multi_real_time_server_url"] = config::get_str("multi_real_time_server_url");
    result["external_payment_url"] = config::get_str("external_payment_url");
    return result;
}

}  // namespace environment
