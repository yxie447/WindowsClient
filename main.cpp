//
// Created by Yaopeng Xie on 2023/11/14.
//

#include "ClipboardManager.h"
#include "NetworkConnection.h"

#include <iostream>
#include <thread>

bool containsSpecialCharacters(const std::string &str);

void removeNullCharacter(std::string &str);

int main() {
    HWND hWindow = ClipboardManager::GetClipboardWindowHandle();

    auto *conn = new NetworkConnection();

    while (true) {
        if (ClipboardManager::HasClipboardChanged()) { // TODO: Also compare with the server contents
            std::string updateTime = ClipboardManager::GetLastUpdateTime();
            std::cout << "Clipboard content was last updated at: " << updateTime << std::endl;

            ClipboardContent content = ClipboardManager::GetClipboardContent(hWindow);
            content.setUpdateTime(updateTime);

            std::string username = "jboerse2";

            nlohmann::json jsonData = {
                    {"username", username},
            };

            switch (content.getDataType()) {
                case ClipboardDataType::Text:
                    std::wcout << L"The clipboard data type: text. Content: " << content.getTextData() << std::endl;

                    jsonData["content"] = ClipboardManager::convertToUtf8(content.getTextData());

                    conn->postData("/api/clipboard/send", jsonData);

                    break;
                case ClipboardDataType::Image:
                    std::wcout << L"The clipboard data type: image" << std::endl;
                    break;
                case ClipboardDataType::File:
                    std::cout << "The clipboard data type: file." << std::endl;
                    break;
                default:
                    std::cout << "Unknown format/empty clipboard." << std::endl;
                    break;
            }
        }

        // Important: The clipboard must only be updated from the server after the local contents have been pushed
        // Note: This may be possible to change by doing a check with the server clipboard contents

        // std::string contents = conn->getData();
        nlohmann::json response = conn->getData("/api/clipboard/receive");

        ClipboardContent content2 = ClipboardManager::GetClipboardContent(hWindow);

        if (content2.getDataType() == ClipboardDataType::Text) {
            std::string converted = ClipboardManager::convertToUtf8(content2.getTextData());

            removeNullCharacter(converted);

            std::string bodyContent = response.at("body").at("content");

            if (converted != bodyContent) {
                std::cout << "Updating clipboard: " << bodyContent << std::endl;
                ClipboardManager::PushStringToClipboard(bodyContent);
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay to prevent excessive CPU usage
    }

    return 0;
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