/**
 * @file ClipboardManager.cpp
 * @brief Implementation file for the ClipboardManager class.
 *
 * Implements the methods defined in ClipboardManager.h. This file contains the functional logic for managing
 * clipboard operations in a Windows environment, such as pushing text to the clipboard, retrieving clipboard contents,
 * and checking for clipboard changes.
 *
 * @author Yaopeng Xie
 * @date 2023/11/14
 */

#include "ClipboardManager.h"
#include <iostream>
#include <iomanip>

/**
 * @brief Constructs a ClipboardContent object.
 *
 * Initializes a new instance of the ClipboardContent class with specified data type, text data, binary data, and update time.
 *
 * @param type The type of the clipboard data (Text, Image, File, etc.).
 * @param textData The text content of the clipboard data.
 * @param binaryData The binary content of the clipboard data.
 * @param updateTime The timestamp when the clipboard data was last updated.
 */
ClipboardContent::ClipboardContent(ClipboardDataType type, const std::wstring& textData,
                                   const std::vector<unsigned char>& binaryData, const std::string& updateTime)
        : dataType(type), textContent(textData), binaryContent(binaryData), updateTime(updateTime) {}

// Implementations for getter and setter functions
ClipboardDataType ClipboardContent::getDataType() const {
    return dataType;
}

std::wstring ClipboardContent::getTextData() const {
    return textContent;
}

const std::vector<unsigned char>& ClipboardContent::getBinaryData() const {
    return binaryContent;
}

void ClipboardContent::setDataType(ClipboardDataType type) {
    dataType = type;
}

void ClipboardContent::setTextData(const std::wstring& text) {
    textContent = text;
}

void ClipboardContent::setBinaryData(const std::vector<unsigned char>& binaryData) {
    this->binaryContent = binaryData;
}

std::string ClipboardContent::getUpdateTime() const {
    return updateTime;
}

void ClipboardContent::setUpdateTime(const std::string& updateTime) {
    this->updateTime = updateTime;
}



// Initialize the static member of ClipboardManager
DWORD ClipboardManager::lastClipboardSequenceNumber = GetClipboardSequenceNumber();

std::string ClipboardManager::lastUpdateTime = "";

// ClipboardManager class methods

/**
 * @brief Checks if the clipboard content has changed.
 *
 * Compares the current clipboard sequence number against the last known sequence number to determine if the clipboard content has changed.
 * Outputs the current and last sequence numbers for debugging purposes.
 *
 * @return True if the clipboard content has changed, false otherwise.
 */
bool ClipboardManager::HasClipboardChanged() {
    DWORD currentSequenceNumber = GetClipboardSequenceNumber();
    std::cout << "Current Seq Num: " << currentSequenceNumber << ", Last Seq Num: " << lastClipboardSequenceNumber << std::endl; // Debug print

    if (currentSequenceNumber != lastClipboardSequenceNumber) {
        lastClipboardSequenceNumber = currentSequenceNumber;
        lastUpdateTime = GetCurrentTimeAsString();
        return true;
    }
    return false;
}

/**
 * @brief Gets the current time as a string.
 *
 * Retrieves the current system time and formats it as a human-readable string.
 *
 * @return A string representing the current system time.
 */
std::string ClipboardManager::GetCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream stream;
    stream << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H:%M:%S");
    return stream.str();
}

/**
 * @brief Gets the window handle to the current clipboard owner.
 *
 * Opens the clipboard and retrieves the handle to the window that currently owns the clipboard.
 * If the clipboard is successfully opened, it is then closed after retrieving the handle.
 *
 * @return The handle to the clipboard owner window if the clipboard is successfully opened, otherwise NULL.
 */
HWND ClipboardManager::GetClipboardWindowHandle() {
    if (OpenClipboard(NULL)) {
        HWND hWindow = GetClipboardOwner();
        CloseClipboard();
        return hWindow;
    }
    return NULL;
}

/**
 * @brief Gets the last update time of the clipboard content.
 *
 * Retrieves the timestamp of when the clipboard content was last updated.
 * This is a static method providing access to the static member lastUpdateTime.
 *
 * @return A string representing the last update time of the clipboard content.
 */
std::string ClipboardManager::GetLastUpdateTime() {
    return lastUpdateTime;
}

/**
 * @brief Gets the format of the current data on the clipboard.
 *
 * This method checks the clipboard and identifies the format of the data currently stored on it.
 * Supported formats include CF_UNICODETEXT for text, CF_DIB for images, and CF_HDROP for file handles.
 *
 * @return The clipboard format identifier of the current clipboard data, or 0 if no known format is available.
 */
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

/**
 * @brief Sets the given text onto the clipboard.
 *
 * This method places a Unicode text string onto the system clipboard.
 * It opens the clipboard, allocates global memory for the text, copies the text, and sets it on the clipboard.
 * If successful, the text can be pasted from the clipboard into other applications.
 *
 * @param hWindow A handle to the window that currently owns the clipboard.
 * @param s The Unicode text string to be placed onto the clipboard.
 */
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

/**
 * @brief Retrieves text from the clipboard.
 *
 * This method fetches a Unicode text string from the system clipboard, if available.
 * It opens the clipboard, gets the text data, and then closes the clipboard.
 *
 * @param hWindow A handle to the window that currently owns the clipboard.
 * @return A Unicode string containing the text retrieved from the clipboard, or an empty string if no text is available.
 */
std::wstring ClipboardManager::GetClipboardText(HWND hWindow) {
    if (OpenClipboard(hWindow)) {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData != NULL) {
            wchar_t *pszText = static_cast<wchar_t*>(GlobalLock(hData));
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

/**
 * @brief Retrieves the content of the clipboard based on its format.
 *
 * This method checks the current format of the clipboard and retrieves its content accordingly.
 * The content can be either text, image, or file based on the identified format.
 *
 * @param hWindow A handle to the window that currently owns the clipboard.
 * @return A ClipboardContent object containing the retrieved clipboard data.
 */
ClipboardContent ClipboardManager::GetClipboardContent(HWND hWindow) {
    UINT format = GetClipboardFormat();

    switch (format) {
        case CF_UNICODETEXT:
        {
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

/**
 * @brief Pushes a string to the system clipboard.
 *
 * This method handles the process of placing a plain text string onto the clipboard.
 * It opens the clipboard, allocates a global memory block for the string, and sets the clipboard data.
 *
 * @param str The ASCII string to be pushed onto the clipboard.
 */
void ClipboardManager::PushStringToClipboard(const std::string& str) {
    if (OpenClipboard(NULL)) {
        EmptyClipboard(); // Clear the clipboard

        // Allocate a global memory object for the text
        HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (str.size() + 1) * sizeof(char));
        if (hglbCopy == NULL) {
            CloseClipboard();
            return;
        }

        // Lock the handle and copy the text to the buffer
        char* lptstrCopy = (char*)GlobalLock(hglbCopy);
        memcpy(lptstrCopy, str.c_str(), str.size() + 1);
        GlobalUnlock(hglbCopy);

        // Place the handle on the clipboard
        SetClipboardData(CF_TEXT, hglbCopy);

        CloseClipboard(); // Close the clipboard
    }
}