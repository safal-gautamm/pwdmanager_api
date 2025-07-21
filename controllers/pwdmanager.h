#pragma once
#include <drogon/HttpController.h>
#include <json/json.h>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace drogon;

class CreateController : public drogon::HttpController<CreateController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CreateController::home, "/", Get);        // ✅ New home route
    ADD_METHOD_TO(CreateController::create, "/create", Post);
    ADD_METHOD_TO(CreateController::add, "/add", Post);
    ADD_METHOD_TO(CreateController::view, "/view", Get);
    ADD_METHOD_TO(CreateController::deleteSite, "/delete", Delete);
    METHOD_LIST_END

    void home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void add(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void view(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void deleteSite(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

private:
    std::string getAPIKey();

    Json::Value loadData();
    void saveData(const Json::Value &root);

    bool validateApiKey(const Json::Value &root, const std::string &name, const std::string &apikey);

    const std::string dataFile = "../passwords.json";
};
