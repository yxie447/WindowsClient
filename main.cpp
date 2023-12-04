#include "ClipboardManager.h"
#include "NetworkConnection.h"

#include <iostream>
#include <thread>
#include <shellapi.h>
#include <regex>

bool containsSpecialCharacters(const std::string &str);

void removeNullCharacter(std::string &str);

void updateClipboard(HWND hWindow, const std::string &serverClipboard);

void backgroundClipboardUpdater(const std::string &username, const std::string &deviceID);

std::string getClipboardFromSever(NetworkConnection *conn, const std::string &username);

int main() {
    std::string serverIPAddress;
    std::string apiServerIPAddress;

    bool flag = true;

    auto ipPattern = std::regex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");
    auto hostNamePattern = std::regex("^[A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?(?:\\.[A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?)*\\.local$");

    while (flag) {
        std::cout << "Please input the server IP (ipv4) address:" << std::endl;
        std::cin >> serverIPAddress;

        if (std::regex_match(serverIPAddress, ipPattern) || std::regex_match(serverIPAddress,hostNamePattern)) {
            flag = false;
        } else {
            std::cout << "Sorry that isn't a valid IP (ipv4) address." << std::endl;
        }
    }

    apiServerIPAddress = "http://" + serverIPAddress + ":4800";
    serverIPAddress = "http://" + serverIPAddress + ":8080";

    NetworkConnection::setIP(apiServerIPAddress);


    std::string username = "Benz"; // admin
    std::string deviceID = "1";

    ShellExecute(nullptr, "open", serverIPAddress.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    std::thread thread_obj(backgroundClipboardUpdater, username, deviceID);

    // TODO: Add Qt code start here

//    backgroundClipboardUpdater(username, deviceID);

    // Wait for the thread to finish
    thread_obj.join();

    return 0;
}

void backgroundClipboardUpdater(const std::string &username, const std::string &deviceID) {
    HWND hWindow = ClipboardManager::GetClipboardWindowHandle();

    auto *conn = new NetworkConnection();

    nlohmann::json jsonData = {
            {"username", username},
            {"deviceID", deviceID},
    };

    while (true) {
        std::string serverClipboardText = getClipboardFromSever(conn, username);

        bool clipboardChanged = false;

        // Send the local clipboard to the server if it changed
        if (ClipboardManager::HasClipboardChanged()) { // TODO: Also compare with the server contents
            std::string updateTime = ClipboardManager::GetLastUpdateTime();
            std::cout << std::endl << "Clipboard content was last updated at: " << updateTime << std::endl;

            ClipboardContent content = ClipboardManager::GetClipboardContent(hWindow);
            content.setUpdateTime(updateTime);

            jsonData["lastUpdatedTime"] = ClipboardManager::GetLastUpdateTime();

            std::string localClipboardConverted;

            switch (content.getDataType()) {
                case ClipboardDataType::Text:
                    std::wcout << L"Clipboard data type: text" << std::endl;
                    std::wcout << L"Content: " << content.getTextData() << std::endl;

                    localClipboardConverted = ClipboardManager::convertToUtf8(content.getTextData());
                    removeNullCharacter(localClipboardConverted);

                    if (!localClipboardConverted.empty() && localClipboardConverted != serverClipboardText) {
                        jsonData["content"] = localClipboardConverted;

                        conn->postData("/api/clipboard/send", jsonData);

                        clipboardChanged = true;
                    } else {
                        std::wcout << L"The contents are the same as the server " << std::endl;
                    }

                    break;
                case ClipboardDataType::Image:
                    std::wcout << L"Clipboard data type: image" << std::endl;
                    break;
                case ClipboardDataType::File:
                    std::cout << "Clipboard data type: file." << std::endl;
                    break;
                default:
                    std::cout << "Unknown format/empty clipboard." << std::endl;
                    break;
            }
        }

        if (!clipboardChanged) {
            updateClipboard(hWindow, serverClipboardText);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay to prevent excessive CPU usage
    }
}

std::string getClipboardFromSever(NetworkConnection *conn, const std::string &username) {
    nlohmann::json serverResponse = conn->getData(
            "/api/clipboard/receive?username=" + username); // TODO: Need to escape?

    // Return if the server has no connection
    if (serverResponse["status"] != 200) {
        if (serverResponse["status"] == 504) {
            std::cerr << std::endl << "GET: Unable to reach server " << std::endl;
        } else {
            std::cout << "Status: " << serverResponse["status"] << std::endl;
            std::cout << "Body: " << serverResponse["body"] << std::endl;
        }

        return "";
    }

    if (serverResponse.at("body").contains("content")) {
        std::string serverClipboard = serverResponse.at("body").at("content");

        return serverClipboard;
    } else {
        std::cerr << "Unable to update local clipboard as the response is missing \"content\"" << std::endl;

        return "";
    }
}

// Updates the local clipboard
void updateClipboard(HWND hWindow, const std::string &serverClipboard) {
    ClipboardContent localClipboard = ClipboardManager::GetClipboardContent(hWindow);

    if (localClipboard.getDataType() == ClipboardDataType::Text) {
        std::string localClipboardConverted = ClipboardManager::convertToUtf8(localClipboard.getTextData());

        removeNullCharacter(localClipboardConverted);

        if (!serverClipboard.empty() && localClipboardConverted != serverClipboard) {
            std::cout << "Updating local clipboard: " << serverClipboard << std::endl;
            ClipboardManager::PushStringToClipboard(serverClipboard);
        }
    }
}

bool containsSpecialCharacters(const std::string &str) {
    for (char ch: str) {
        // Check if the character is not printable or is a control character
        if (!std::isprint(static_cast<unsigned char>(ch)) || std::iscntrl(static_cast<unsigned char>(ch))) {
            return true; // String contains a special character
        }
    }
    return false; // String does not contain special characters
}

void removeNullCharacter(std::string &str) {
    size_t pos = str.find('\u0000');
    while (pos != std::string::npos) {
        str.erase(pos, 1); // Erase 1 character at the found position
        pos = str.find('\u0000', pos); // Find the next occurrence
    }
}