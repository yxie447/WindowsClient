//
// Created by Yaopeng Xie on 2023/11/14.
//

#include "ClipboardManager.h"
#include "NetworkConnection.h"

#include <iostream>
#include <thread>

int main() {
    HWND hWindow = ClipboardManager::GetClipboardWindowHandle();

    auto *conn = new NetworkConnection();

    while (true) {
        if (ClipboardManager::HasClipboardChanged()) {
            std::string updateTime = ClipboardManager::GetLastUpdateTime();
            std::cout << "Clipboard content was last updated at: " << updateTime << std::endl;

            ClipboardContent content = ClipboardManager::GetClipboardContent(hWindow);
            content.setUpdateTime(updateTime);

            std::string convertedContent;

            switch (content.getDataType()) {
                case ClipboardDataType::Text:
                    std::wcout << L"The clipboard data type: text. Content: " << content.getTextData() << std::endl;

                    convertedContent = ClipboardManager::convertToUtf8(content.getTextData());
                    conn->postData(convertedContent);

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
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay to prevent excessive CPU usage

    }

    return 0;
}