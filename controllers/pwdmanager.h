#pragma once
#include <drogon/HttpController.h>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace drogon;

class CreateController : public drogon::HttpController<CreateController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CreateController::create, "/create", Post);
    METHOD_LIST_END

    std::string getAPIKey();
    void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};

