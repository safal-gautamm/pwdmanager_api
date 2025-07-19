#include "UserController.h"

void UserController::registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto jsonReq = req->getJsonObject();
    Json::Value respJson;

    if (!jsonReq || !(*jsonReq).isMember("username"))
    {
        respJson["status"] = "error";
        respJson["message"] = "Missing 'username'";
    }
    else
    {
        std::string username = (*jsonReq)["username"].asString();
        respJson["status"] = "success";
        respJson["message"] = "Welcome, " + username + "!";
    }

    auto resp = HttpResponse::newHttpJsonResponse(respJson);
    callback(resp);
}
