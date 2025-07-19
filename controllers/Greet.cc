#include "Greet.h"

void Greet:: greet(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value json;
    json["greetings"] = "Namaskar";
    json["name"] = "Safal";
    json["message"] = json["greetings"].asString() + " " + json["name"].asString() + " daju" ;

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}