# pragma once
# include <drogon/drogon.h>

using namespace drogon;

class Greet: public HttpController<Greet>
{
    public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(Greet::greet, "/greet", Get);

    METHOD_LIST_END

    void greet(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    
};