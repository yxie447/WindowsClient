//
// Created by jarboer on 2023-11-28.
//

#include "NetworkConnection.h"

const std::string NetworkConnection::IP = "http://10.0.0.48:49162"; // http://0.0.0.0:18080

void NetworkConnection::postData(const std::string &url, const nlohmann::json &jsonData) {
    httplib::Headers headers = {
            {"Content-Type", "application/json"}
    };

    auto postRes = client->Post(url, headers, jsonData.dump(), "application/json");

    std::cout << std::endl << "POST: " << std::endl;

    std::cout << "Status: " << postRes->status << std::endl;
    std::cout << "Body: " << postRes->body << std::endl;
}

nlohmann::json NetworkConnection::getData(const std::string &url) {
    auto getRes = client->Get(url);

    // Parse the JSON text into a nlohmann::json object
    nlohmann::json jsonData = nlohmann::json::parse(getRes->body);

    nlohmann::json responseData = {
            {"status", getRes->status},
            {"body", jsonData}
    };

    return responseData;
}
