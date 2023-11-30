//
// Created by jarboer on 2023-11-28.
//

#include "NetworkConnection.h"

const std::string NetworkConnection::IP = "http://10.0.0.48:48000"; // http://0.0.0.0:48000

// Make a POST request to the server to send data
void NetworkConnection::postData(const std::string &url, const nlohmann::json &jsonData) {
    httplib::Headers headers = {
            {"Content-Type", "application/json"}
    };

    auto postRes = client->Post(url, headers, jsonData.dump(), "application/json");

    if (postRes && postRes->status == 200) {
        std::cout << std::endl << "POST: " << std::endl;

        std::cout << "Status: " << postRes->status << std::endl;
        std::cout << "Body: " << postRes->body << std::endl;
    } else { // If the server has no connection
        std::cout << "POST: Unable to reach server " << std::endl;
    }


}

// Make a GET request to the server to get data
nlohmann::json NetworkConnection::getData(const std::string &url) {
    auto getRes = client->Get(url);

    nlohmann::json responseData;

    if (getRes && getRes->status == 200) {
        // Parse the JSON text into a json object
        nlohmann::json jsonData = nlohmann::json::parse(getRes->body);

        responseData["status"] = getRes->status;
        responseData["body"] = jsonData;
    } else {
        responseData["status"] = 504; // Connection timeout
    }

    return responseData;
}
