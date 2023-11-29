#include "ClipboardManager.h"

// Initialize the static member of ClipboardManager
DWORD ClipboardManager::lastClipboardSequenceNumber = GetClipboardSequenceNumber();

std::string ClipboardManager::lastUpdateTime = "";

// Implement the ClipboardManager class methods
bool ClipboardManager::HasClipboardChanged() {
    DWORD currentSequenceNumber = GetClipboardSequenceNumber();
    std::cout << "Current Seq Num: " << currentSequenceNumber << ", Last Seq Num: " << lastClipboardSequenceNumber
              << std::endl; // Debug print

    if (currentSequenceNumber != lastClipboardSequenceNumber) {
        lastClipboardSequenceNumber = currentSequenceNumber;
        lastUpdateTime = GetCurrentTimeAsString();
        return true;
    }
    return false;
}

std::string ClipboardManager::GetCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream stream;
    stream << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H:%M:%S");
    return stream.str();
}

HWND ClipboardManager::GetClipboardWindowHandle() {
    if (OpenClipboard(NULL)) {
        HWND hWindow = GetClipboardOwner();
        CloseClipboard();
        return hWindow;
    }
    return NULL;
}

std::string ClipboardManager::GetLastUpdateTime() {
    return lastUpdateTime;
}

UINT ClipboardManager::GetClipboardFormat() {
    if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {
        return CF_UNICODETEXT;
    } else if (IsClipboardFormatAvailable(CF_DIB)) {
        return CF_DIB;
    } else if (IsClipboardFormatAvailable(CF_HDROP)) {
        return CF_HDROP;
    } else {
        return 0;
    }
}

void ClipboardManager::SetClipboardText(HWND hWindow, const std::wstring &s) {
    if (OpenClipboard(hWindow)) {
        EmptyClipboard();
        std::wstring ws(s.begin(), s.end());
        HGLOBAL whg = GlobalAlloc(GMEM_MOVEABLE, (ws.size() + 1) * sizeof(WCHAR));
        if (whg) {
            memcpy(GlobalLock(whg), ws.c_str(), (ws.size() + 1) * sizeof(WCHAR));
            GlobalUnlock(whg);
            SetClipboardData(CF_UNICODETEXT, whg);
        }
        CloseClipboard();
    }
}

std::wstring ClipboardManager::GetClipboardText(HWND hWindow) {
    if (OpenClipboard(hWindow)) {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData != NULL) {
            wchar_t *pszText = static_cast<wchar_t *>(GlobalLock(hData));
            if (pszText != NULL) {
                std::wstring text(pszText);
                GlobalUnlock(hData);
                CloseClipboard();
                return text;
            }
        }
        CloseClipboard();
    }
    return L"";
}

ClipboardContent ClipboardManager::GetClipboardContent(HWND hWindow) {
    UINT format = GetClipboardFormat();

    switch (format) {
        case CF_UNICODETEXT: {
            std::wstring textData = GetClipboardText(hWindow);
            return ClipboardContent(ClipboardDataType::Text, textData, std::vector<unsigned char>());
        }
        case CF_DIB:
            // Placeholder for image data
            return ClipboardContent(ClipboardDataType::Image, L"", std::vector<unsigned char>());

        case CF_HDROP:
            // Placeholder for file data
            return ClipboardContent(ClipboardDataType::File, L"", std::vector<unsigned char>());

        default:
            return ClipboardContent(ClipboardDataType::Unknown, L"", std::vector<unsigned char>());
    }
}

void ClipboardManager::PushStringToClipboard(const std::string &str) {
    if (OpenClipboard(NULL)) {
        EmptyClipboard(); // Clear the clipboard

        // Allocate a global memory object for the text
        HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (str.size() + 1) * sizeof(char));
        if (hglbCopy == NULL) {
            CloseClipboard();
            return;
        }

        // Lock the handle and copy the text to the buffer
        char *lptstrCopy = (char *) GlobalLock(hglbCopy);
        memcpy(lptstrCopy, str.c_str(), str.size() + 1);
        GlobalUnlock(hglbCopy);

        // Place the handle on the clipboard
        SetClipboardData(CF_TEXT, hglbCopy);

        CloseClipboard(); // Close the clipboard
    }
}

std::string ClipboardManager::convertToUtf8(const std::wstring &unicodeText) {
    // If you need to convert it to a narrow character string (char array), you can use WideCharToMultiByte
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, unicodeText.c_str(), -1, NULL, 0, NULL, NULL);
    std::string narrowText(bufferSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, unicodeText.c_str(), -1, &narrowText[0], bufferSize, NULL, NULL);

    // 'narrowText' now contains the UTF-8 encoded string
    return narrowText;
}