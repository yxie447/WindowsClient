/**
 * @file ClipboardManager.h
 * @brief Header file for the ClipboardManager class.
 *
 *
 * @author Yaopeng Xie
 * @date 2023/11/14
 */

#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include "ClipboardContent.h"

#include <windows.h>

// Declaration of ClipboardManager class
class ClipboardManager {
public:
    static std::string lastUpdateTime;

    static bool HasClipboardChanged();
    static std::string GetCurrentTimeAsString();
    static HWND GetClipboardWindowHandle();
    static std::string GetLastUpdateTime();
    static UINT GetClipboardFormat();
    static void SetClipboardText(HWND hWindow, const std::wstring &s);
    static std::wstring GetClipboardText(HWND hWindow);
    static ClipboardContent GetClipboardContent(HWND hWindow);
    static void PushStringToClipboard(const std::string& str);
    static std::string convertToUtf8(const std::wstring &unicodeText);

private:
    static DWORD lastClipboardSequenceNumber;

    void setDataType(ClipboardDataType type);
};

#endif // CLIPBOARDMANAGER_H
