//
// Created by yxie447 on 2023/11/27.
//

#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <windows.h>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>

// Declare the enum class ClipboardDataType
enum class ClipboardDataType {
    Text,
    Image,
    File,
    Unknown
};

// Declaration of ClipboardContent class
class ClipboardContent {
public:
    ClipboardContent(ClipboardDataType type, const std::wstring& textData,
                     const std::vector<unsigned char>& binaryData, const std::string& updateTime = "");

    ClipboardDataType getDataType() const;
    void setDataType(ClipboardDataType type);
    std::wstring getTextData() const;
    void setTextData(const std::wstring& text);
    const std::vector<unsigned char>& getBinaryData() const;
    void setBinaryData(const std::vector<unsigned char>& binaryData);
    std::string getUpdateTime() const;
    void setUpdateTime(const std::string& updateTime);

private:
    ClipboardDataType dataType;
    std::wstring textContent; // For text data
    std::vector<unsigned char> binaryContent; // For binary data (images, files)
    std::string updateTime;
};

// Declaration of ClipboardManager class
class ClipboardManager {
public:
    static bool HasClipboardChanged();
    static std::string GetCurrentTimeAsString();
    static HWND GetClipboardWindowHandle();
    static UINT GetClipboardFormat();
    static void SetClipboardText(HWND hWindow, const std::wstring &s);
    static std::wstring GetClipboardText(HWND hWindow);
    static ClipboardContent GetClipboardContent(HWND hWindow);
    static std::string GetLastUpdateTime();
    static std::string lastUpdateTime;

private:
    static DWORD lastClipboardSequenceNumber;

    void setDataType(ClipboardDataType type);
};

#endif // CLIPBOARDMANAGER_H
