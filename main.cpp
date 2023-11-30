//
// Created by Yaopeng Xie on 2023/11/14.
//

#include "ClipboardManager.h"
#include "NetworkConnection.h"

#include <iostream>
#include <thread>

bool containsSpecialCharacters(const std::string &str);

void removeNullCharacter(std::string &str);

void updateClipboard(HWND hWindow, NetworkConnection *conn);

void backgroundClipboardUpdater(const std::string &username, const int &deviceID);

int main() {
    std::string username = "admin";
    int deviceID = 1;

//    std::thread thread_obj(backgroundClipboardUpdater, username);

    backgroundClipboardUpdater(username, deviceID);

    // Wait for the thread to finish
    // thread_obj.join();

    return 0;
}

void backgroundClipboardUpdater(const std::string &username, const int &deviceID) {
    HWND hWindow = ClipboardManager::GetClipboardWindowHandle();

    auto *conn = new NetworkConnection();

    nlohmann::json jsonData = {
            {"username", username},
            {"deviceID", deviceID},
    };

    while (true) {
        if (ClipboardManager::HasClipboardChanged()) { // TODO: Also compare with the server contents
            std::string updateTime = ClipboardManager::GetLastUpdateTime();
            std::cout << "Clipboard content was last updated at: " << updateTime << std::endl;

            ClipboardContent content = ClipboardManager::GetClipboardContent(hWindow);
            content.setUpdateTime(updateTime);

            jsonData["lastUpdatedTime"] = ClipboardManager::GetLastUpdateTime();

            switch (content.getDataType()) {
                case ClipboardDataType::Text:
                    std::wcout << L"Clipboard data type: text" << std::endl;
                    std::wcout << L"Content: " << content.getTextData() << std::endl;

                    jsonData["content"] = ClipboardManager::convertToUtf8(content.getTextData());

                    conn->postData("/api/clipboard/send", jsonData);

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

        // Important: The clipboard must only be updated from the server after the local contents have been pushed
        // Note: This may be possible to change by doing a check with the server clipboard contents
        updateClipboard(hWindow, conn);

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay to prevent excessive CPU usage
    }
}

void updateClipboard(HWND hWindow, NetworkConnection *conn) {
    nlohmann::json serverResponse = conn->getData("/api/clipboard/receive");

    // Return if the server has no connection
    if (serverResponse["status"] == 504) {
        std::cout << "GET: Unable to reach server " << std::endl;
        return;
    }

    ClipboardContent localClipboard = ClipboardManager::GetClipboardContent(hWindow);

    if (localClipboard.getDataType() == ClipboardDataType::Text) {
        std::string localClipboardConverted = ClipboardManager::convertToUtf8(localClipboard.getTextData());

        removeNullCharacter(localClipboardConverted);

        std::string serverClipboard = serverResponse.at("body").at("content");

        if (localClipboardConverted != serverClipboard) {
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