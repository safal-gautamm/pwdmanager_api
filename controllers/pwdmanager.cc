#include "pwdmanager.h"

void CreateController::create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string apikey = getAPIKey();

    std::string name = req->getParameter("name");
    if (name.empty()) {
        // Return 400 Bad Request immediately, no greeting or API key generated
        Json::Value errorJson;
        errorJson["error"] = "Missing required parameter: name";

        auto resp = HttpResponse::newHttpJsonResponse(errorJson);
        resp->setStatusCode(k400BadRequest);

        callback(resp);
        return;  // stop further processing, acts like exit for this request
    }
    Json::Value json;
    json["name"] = name;
    json["apikey"] = apikey;

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

std::string CreateController::getAPIKey()
{
    time_t now = time(nullptr);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << now;

    return oss.str();
}
