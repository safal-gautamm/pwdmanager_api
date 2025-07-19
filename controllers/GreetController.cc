#include "GreetController.h"

void GreetController::greet(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    // Get the "name" parameter, default to "Safal dai" if not provided
    std::string name = req->getParameter("name");
    if (name.empty())
        name = "Safal dai";

    Json::Value json;
    json["greeting"] = "Namaste, " + name + "!";
    json["language"] = "Nepali";

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}
