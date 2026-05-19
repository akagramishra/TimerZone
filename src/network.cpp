#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#include <curl/curl.h>
#include <string>
#include "todo.hpp"
#include "network.hpp"
#include "../Res/includes/json.hpp"
using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string SaveTodoToServer(const char* text) {
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    std::string response;
    std::string url = "https://amdevevelop.pythonanywhere.com/todos";

    // Safe JSON building
    json body;
    body["text"] = text;
    body["completed"] = false;
    std::string data = body.dump();

    struct curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK || response.empty()) return "";

    json parsed = json::parse(response, nullptr, false);
    if (parsed.is_discarded()) return "";
    return parsed["id"].get<std::string>();
}

void LoadTodosFromServer(TODO_TSK& todo) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, "https://amdevevelop.pythonanywhere.com/todos");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (response == "null" || response.empty()) return;

    json data = json::parse(response, nullptr, false);
    if (data.is_discarded()) return;
    todo.items.clear();

    for (auto& [key, val] : data.items()) {
        TODO_ITEM item;
        item.id = key;
        item.text = val["text"].get<std::string>();
        item.completed = val["completed"].get<bool>();
        todo.items.push_back(item);
    }
}

void DeleteTodoFromServer(const std::string& id) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    std::string url = "https://amdevevelop.pythonanywhere.com/todos/" + id;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

    CURLcode res = curl_easy_perform(curl); // only once
    printf("Delete result for %s: %d\n", url.c_str(), res);
    curl_easy_cleanup(curl);
}