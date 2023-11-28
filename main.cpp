//
// Created by yxie447 on 2023/11/27.
//

#include "ClipboardManager.h"
#include <iostream>
#include <thread>

int main() {
    HWND hWindow = ClipboardManager::GetClipboardWindowHandle();
    std::string myText = "Hello, Clipboard!";
    ClipboardManager::PushStringToClipboard(myText);

    /*while (true) {
        if (ClipboardManager::HasClipboardChanged()) {
            std::string updateTime = ClipboardManager::GetLastUpdateTime();
            std::cout << "Clipboard content was last updated at: " << updateTime << std::endl;

            ClipboardContent content = ClipboardManager::GetClipboardContent(hWindow);
            content.setUpdateTime(updateTime);

            switch (content.getDataType()) {
                case ClipboardDataType::Text:
                    std::wcout << L"The clipboard contains text: " << content.getTextData() << std::endl;
                    break;
                case ClipboardDataType::Image:
                    std::wcout << L"The clipboard contains an image." << std::endl;
                    break;
                case ClipboardDataType::File:
                    std::cout << "The clipboard contains a file." << std::endl;
                    break;
                default:
                    std::cout << "Unknown format/empty clipboard." << std::endl;
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay to prevent excessive CPU usage

    }*/

    return 0;
}