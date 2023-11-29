//
// Created by jarboer on 2023-11-28.
//

#include "NetworkConnection.h"

const std::string NetworkConnection::IP = "http://10.0.0.48:49162"; // http://0.0.0.0:18080

// TODO: Take in JSON
void NetworkConnection::postData(const std::string &content) {
    nlohmann::json json_data = {
            {"username", "jboerse2"},
            {"content",  content}
    };

    httplib::Headers headers = {
            {"Content-Type", "application/json"}
    };

    auto postRes = client->Post("/api/clipboard/send", headers, json_data.dump(), "application/json");

    std::cout << std::endl << "POST: " << std::endl;

    std::cout << "Status: " << postRes->status << std::endl;
    std::cout << "Body: " << postRes->body << std::endl;
}

// TODO: Return with JSON data
std::string NetworkConnection::getData() {
    // ------ GET METHOD ------
    auto getRes = client->Get("/api/clipboard/receive");

    std::cout << "GET: " << std::endl << std::endl;

    std::cout << "Response: " << getRes << std::endl << std::endl;

    std::cout << "Status: " << getRes->status << std::endl;
    std::cout << "Body: " << getRes->body << std::endl;

    return ""; // TODO: Return data
}
