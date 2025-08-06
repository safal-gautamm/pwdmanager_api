#include "pwdmanager.h"

const std::string encryptionKey = "MySecretXORKey";

// Simple XOR encrypt/decrypt function (symmetric)
std::string xorCrypt(const std::string &input, const std::string &key)
{
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i] = input[i] ^ key[i % key.size()];
    }
    return output;
}

// Serve home page instructions
void CreateController::home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newFileResponse("../index.html");
    callback(resp);
}

// Generate API key from current time as hex string
std::string CreateController::getAPIKey()
{
    time_t now = time(nullptr);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << now;
    return oss.str();
}

// Load JSON data from file, or create empty root if none
Json::Value CreateController::loadData()
{
    Json::Value root;
    std::ifstream file(dataFile);
    if (file.good())
    {
        file >> root;
    }
    file.close();

    if (!root.isObject())
    {
        root["users"] = Json::Value(Json::arrayValue);
    }
    return root;
}

// Save JSON data to file
void CreateController::saveData(const Json::Value &root)
{
    std::ofstream file(dataFile);
    file << root.toStyledString();
    file.close();
}

// Validate if name + apikey exist in data
bool CreateController::validateApiKey(const Json::Value &root, const std::string &name, const std::string &apikey)
{
    const Json::Value &users = root["users"];
    for (const auto &user : users)
    {
        if (user["name"].asString() == name && user["apikey"].asString() == apikey)
        {
            return true;
        }
    }
    return false;
}

// POST /create?name=...
void CreateController::create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string name = req->getParameter("name");
    if (name.empty())
    {
        Json::Value json;
        json["error"] = "Missing required parameter: name";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    Json::Value root = loadData();
    Json::Value &users = root["users"];

    for (const auto &user : users)
    {
        if (user["name"].asString() == name)
        {
            Json::Value json;
            json["error"] = "User already exists";
            json["status"] = 409;
            auto resp = HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(k409Conflict);
            callback(resp);
            return;
        }
    }

    std::string apikey = getAPIKey();

    Json::Value newUser;
    newUser["name"] = name;
    newUser["apikey"] = apikey;
    newUser["passwords"] = Json::Value(Json::arrayValue);
    users.append(newUser);
    saveData(root);

    Json::Value json;
    json["name"] = name;
    json["apikey"] = apikey;
    json["status"] = 200;

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

// POST /add?apikey=...&site=...&username=...&password=...
void CreateController::add(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string apikey = req->getParameter("apikey");
    std::string site = req->getParameter("site");
    std::string username = req->getParameter("username");
    std::string password = req->getParameter("password");

    if (apikey.empty() || site.empty() || username.empty() || password.empty())
    {
        Json::Value json;
        json["error"] = "Missing required parameters";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    Json::Value root = loadData();
    Json::Value &users = root["users"];

    bool found = false;
    for (auto &user : users)
    {
        if (user["apikey"].asString() == apikey)
        {
            Json::Value &passwords = user["passwords"];

            bool siteExists = false;
            for (auto &entry : passwords)
            {
                if (entry["site"].asString() == site && entry["username"].asString() == username)
                {
                    siteExists = true;
                    break;
                }
            }
            if(siteExists)
            {
                Json::Value json;
                json["error"] = "User already exists";
                json["status"] = 409;
                auto resp = HttpResponse::newHttpJsonResponse(json);
                resp->setStatusCode(k409Conflict);
                callback(resp);
                return;
            }
            if (!siteExists)
            {
                Json::Value newEntry;
                newEntry["site"] = site;
                newEntry["username"] = username;
                newEntry["password"] = xorCrypt(password, encryptionKey);
                passwords.append(newEntry);
            }

            found = true;
            break;
        }
    }

    if (!found)
    {
        Json::Value json;
        json["error"] = "Invalid API key";
        json["status"] = 403;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k403Forbidden);
        callback(resp);
        return;
    }

    saveData(root);

    Json::Value json;
    json["status"] = 200;
    json["message"] = "Password added successfully";
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

// PUT /update?apikey=...&site=...&username=...&password=...
void CreateController::update(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string apikey = req->getParameter("apikey");
    std::string site = req->getParameter("site");
    std::string username = req->getParameter("username");
    std::string password = req->getParameter("password");

    if (apikey.empty() || site.empty() || (username.empty() && password.empty()))
    {
        Json::Value json;
        json["error"] = "Missing required parameters";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    Json::Value root = loadData();
    Json::Value &users = root["users"];

    bool foundUser = false;
    bool siteUpdated = false;

    for (auto &user : users)
    {
        if (user["apikey"].asString() == apikey)
        {
            foundUser = true;
            bool sitefound = false;

            Json::Value &datas = user["passwords"];
            for (auto &data : datas)
            {
                if (data["site"].asString() == site)
                {
                    if (!username.empty())
                        data["username"] = username;
                    if (!password.empty())
                        data["password"] = xorCrypt(password, encryptionKey);
                    sitefound = true;
                    siteUpdated = true;
                    break;
                }
            }

            if (!sitefound)
            {
                Json::Value json;
                json["error"] = "Site does not exist";
                json["status"] = 404;
                auto resp = HttpResponse::newHttpJsonResponse(json);
                resp->setStatusCode(k404NotFound);
                callback(resp);
                return;
            }
            break;
        }
    }

    if (!foundUser)
    {
        Json::Value json;
        json["error"] = "User does not exist";
        json["status"] = 404;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k404NotFound);
        callback(resp);
        return;
    }

    if (siteUpdated)
    {
        saveData(root);
        Json::Value json;
        json["status"] = 200;
        json["message"] = "Site updated successfully";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    }
}

// GET /view?name=...&apikey=...
void CreateController::view(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string name = req->getParameter("name");
    std::string apikey = req->getParameter("apikey");

    if (name.empty() || apikey.empty())
    {
        Json::Value json;
        json["error"] = "Missing required parameters";
        json["status"] = 400;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    Json::Value root = loadData();

    if (!validateApiKey(root, name, apikey))
    {
        Json::Value json;
        json["error"] = "Invalid API key or user";
        json["status"] = 403;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k403Forbidden);
        callback(resp);
        return;
    }

    const Json::Value &users = root["users"];
    for (const auto &user : users)
    {
        if (user["name"].asString() == name)
        {
            Json::Value decryptedPasswords(Json::arrayValue);
            for (const auto &entry : user["passwords"])
            {
                Json::Value decryptedEntry = entry;
                decryptedEntry["password"] = xorCrypt(entry["password"].asString(), encryptionKey);
                decryptedPasswords.append(decryptedEntry);
            }

            Json::Value json;
            json["passwords"] = decryptedPasswords;
            auto resp = HttpResponse::newHttpJsonResponse(json);
            callback(resp);
            return;
        }
    }

    Json::Value json;
    json["error"] = "User not found";
    json["status"] = 404;
    auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k404NotFound);
    callback(resp);
}

// DELETE /delete?name=...&apikey=...
void CreateController::deleteData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string name = req->getParameter("name");
    std::string apikey = req->getParameter("apikey");

    if (name.empty() || apikey.empty())
    {
        Json::Value json;
        json["error"] = "Missing required parameters";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        json["status"] = 400;
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    Json::Value root = loadData();

    if (!validateApiKey(root, name, apikey))
    {
        Json::Value json;
        json["error"] = "Invalid API key or user";
        json["status"] = 403;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k403Forbidden);
        callback(resp);
        return;
    }

    Json::Value &users = root["users"];
    bool deleted = false;
    for (auto &user : users)
    {
        if (user["name"].asString() == name)
        {
            user["passwords"] = Json::Value(Json::arrayValue);
            deleted = true;
            break;
        }
    }

    if (!deleted)
    {
        Json::Value json;
        json["error"] = "User not found";
        json["status"] = 404;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(k404NotFound);
        callback(resp);
        return;
    }

    saveData(root);

    Json::Value json;
    json["status"] = 204;
    json["message"] = "All passwords deleted successfully";
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}
