//
// Created by jarboer on 2023-11-28.
//

#include "NetworkConnection.h"

const std::string NetworkConnection::IP = "http://10.0.0.48:4800"; // http://0.0.0.0:48000

// Make a POST request to the server to send data
// TODO: Wrap in try catch? Here or in call...
// TODO: Return bool
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
        std::cout << "POST: Unable to reach server " << std::endl; // TODO: Make less generic as it catches any different status
    }
}

// Make a GET request to the server to get data
// TODO: Wrap in try catch? Here or in call...
nlohmann::json NetworkConnection::getData(const std::string &url) {
    auto getRes = client->Get(url);

    std::string contentType;

    for (const auto& header : getRes->headers) {
        if (header.first == "Content-Type") {
            contentType = header.second;
            break;
        }
    }

    nlohmann::json responseData;

    if (getRes && !getRes->body.empty()) {
        responseData["status"] = getRes->status;

        if (!contentType.empty()) {
            responseData["contentType"] = contentType;
        } else {
            std::cerr << "Warning: contentType not specified" << std::endl;
        }

        if (!contentType.empty() && contentType == "application/json") {
            // Parse the JSON text into a json object
            nlohmann::json jsonData = nlohmann::json::parse(getRes->body);
            responseData["body"] = jsonData;
        } else {
            responseData["body"] = getRes->body;
        }
    } else {
        responseData["status"] = 504; // Connection timeout
    }

    return responseData;
}
