#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class GreetController : public drogon::HttpController<GreetController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(GreetController::greet, "/greet", Get);
    METHOD_LIST_END

    void greet(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};
