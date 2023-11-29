//
// Created by jarboer on 2023-11-28.
//

#include "NetworkConnection.h"

const std::string NetworkConnection::IP = "http://10.0.0.48:49162"; // http://0.0.0.0:18080

void NetworkConnection::postData(const std::string &content) {
    // ------ POST METHOD ------
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
