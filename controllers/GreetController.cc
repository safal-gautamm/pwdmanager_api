#include "GreetController.h"

void GreetController::greet(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value json;
    json["greeting"] = "Namaste, Safal dai!";
    json["language"] = "Nepali";

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}
