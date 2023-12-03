#include "NetworkConnection.h"

std::string NetworkConnection::IP = "";

void NetworkConnection::setIP(std::string serverIP) {
    IP = serverIP;
}

// Make a POST request to the server to send data
// TODO: Wrap in try catch? Here or in call...
// TODO: Return bool
void NetworkConnection::postData(const std::string &url, const nlohmann::json &jsonData) {
    httplib::Headers headers = {
            {"Content-Type", "application/json"}
    };

    auto postRes = client->Post(url, headers, jsonData.dump(), "application/json");

    if (postRes.error() == httplib::Error::Connection) { // If the server has no connection
        std::cerr << "POST: Unable to reach server " << std::endl;

        return;
    }

    std::cout << std::endl << "postRes: " << postRes << std::endl;

    if (postRes) {
        std::cout << std::endl << "POST: " << std::endl;

        std::cout << "Status: " << postRes->status << std::endl;
        std::cout << "Body: " << postRes->body << std::endl;
    }
}

// Make a GET request to the server to get data
// TODO: Wrap in try catch? Here or in call...
nlohmann::json NetworkConnection::getData(const std::string &url) {
    auto getRes = client->Get(url);

    std::string contentType;
    nlohmann::json responseData;

    // If there is a connection error return early
    if (getRes.error() == httplib::Error::Connection) {
        responseData["status"] = 504; // Connection timeout

        return responseData;
    } else if (!getRes) {
        std::cerr << "The get response is bad" << std::endl;

        responseData["status"] = 504; // Connection timeout

        return responseData;
    }

    for (const auto& header : getRes->headers) {
        if (header.first == "Content-Type") {
            contentType = header.second;
            break;
        }
    }

    if (!getRes->body.empty()) {
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
