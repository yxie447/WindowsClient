//
// Created by jarboer on 2023-11-28.
//

#ifndef WINDOWSCLIENTTEST1_NETWORKCONNECTION_H
#define WINDOWSCLIENTTEST1_NETWORKCONNECTION_H

#include "httplib.h"
#include <windows.h>
#include <nlohmann/json.hpp>

class NetworkConnection {
private:
    static const std::string IP;

    httplib::Client *client;
public:
    NetworkConnection() {
        // Initialize the HTTP client
        client = new httplib::Client(IP);
    }

    void postData(const std::string &content);
};


#endif //WINDOWSCLIENTTEST1_NETWORKCONNECTION_H
