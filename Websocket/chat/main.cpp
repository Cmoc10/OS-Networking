#include "crow.h"
#include <unordered_set>
#include <mutex>
#include <cstring>

#define IPADDRESS 9095

int main() {
    crow::SimpleApp app;

    std::mutex mtx;;
    std::unordered_map<crow::websocket::connection*, std::string> userMap;

    CROW_ROUTE(app, "/ws")
        .websocket()
        .onopen([&](crow::websocket::connection& conn){
            CROW_LOG_INFO << "New websocket connection";
            std::lock_guard<std::mutex> _(mtx);
            // Assign a default username based on client's IP address
            char ipStr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &conn.get_ip_address(), ipStr, INET_ADDRSTRLEN);
            userMap[&conn] = std::string("User_") + ipStr; 
        })
        .onclose([&](crow::websocket::connection& conn, const std::string& /*reason*/){
            CROW_LOG_INFO << "Websocket connection closed";
            std::lock_guard<std::mutex> _(mtx);
            userMap.erase(&conn);
        })
        .onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary){
            std::lock_guard<std::mutex> _(mtx);
            for(auto& user : userMap) {
                if (is_binary)
                    user.first->send_binary(data);
                else
                    // broadcast username along with the message
                    user.first->send_text(user.second + ": " + data);
            }
        });

    CROW_ROUTE(app, "/")
    ([]{
        char name[256];
        gethostname(name, 256);
        crow::mustache::context x;
        x["servername"] = name;
        auto page = crow::mustache::load("ws.html");
        return page.render(x);
    });

    // Add a route for htmx.html
    CROW_ROUTE(app, "/htmx")
    ([](){
        auto page = crow::mustache::load("htmx.html");
        return page.render();
    });

    // Add a simple route to return an HTML string
    CROW_ROUTE(app, "/send-message")
    ([](){
        return "<html><body><h1>hello from server</h1></body></html>";
    });

    app.port(IPADDRESS)
        .multithreaded()
        .run();
}