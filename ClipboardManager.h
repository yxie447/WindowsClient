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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/**
 * @brief Manages clipboard operations.
 *
 * This class provides static methods to interact with the system clipboard, including reading and writing content.
 */
class ClipboardManager {
public:
    static std::string lastUpdateTime; ///< Last update time of the clipboard content

    /**
     * @brief Checks if the clipboard content has changed.
     * @return True if the clipboard content has changed, false otherwise.
     */
    static bool HasClipboardChanged();

    /**
     * @brief Gets the current time as a string.
     * @return The current time in a human-readable string format.
     */
    static std::string GetCurrentTimeAsString();

    /**
     * @brief Retrieves the handle to the window that currently owns the clipboard.
     *
     * This method is used to obtain the handle of the window that is currently interacting with the clipboard.
     * It is typically used in conjunction with other clipboard operations.
     *
     * @return The handle to the window owning the clipboard, or NULL if no window owns the clipboard.
     */
    static HWND GetClipboardWindowHandle();

    /**
     * @brief Gets the last update time of the clipboard content.
     *
     * This method retrieves the timestamp of when the clipboard content was last updated.
     *
     * @return A string representing the last update time of the clipboard content.
     */
    static std::string GetLastUpdateTime();

    /**
     * @brief Gets the format of the current data on the clipboard.
     *
     * This method checks the clipboard and identifies the format of the data currently stored on it.
     * Supported formats include text, images, and file handles.
     *
     * @return The clipboard format identifier of the current clipboard data.
     */
    static UINT GetClipboardFormat();

    /**
     * @brief Sets the given text onto the clipboard.
     *
     * This method places a Unicode text string onto the system clipboard. If successful,
     * the text can be pasted from the clipboard into other applications.
     *
     * @param hWindow A handle to the window that currently owns the clipboard.
     * @param s The Unicode text string to be placed onto the clipboard.
     */
    static void SetClipboardText(HWND hWindow, const std::wstring &s);

    /**
     * @brief Retrieves text from the clipboard.
     *
     * This method fetches a Unicode text string from the system clipboard, if available.
     *
     * @param hWindow A handle to the window that currently owns the clipboard.
     * @return A Unicode string containing the text retrieved from the clipboard.
     */
    static std::wstring GetClipboardText(HWND hWindow);

    /**
     * @brief Retrieves the current content of the clipboard.
     *
     * This method fetches the current content of the clipboard and encapsulates it in a ClipboardContent object.
     * The content can include text, images, or files.
     *
     * @param hWindow A handle to the window that currently owns the clipboard.
     * @return A ClipboardContent object containing the current clipboard content.
     */
    static ClipboardContent GetClipboardContent(HWND hWindow);

    /**
    * @brief Pushes a string to the system clipboard.
    * @param str The string to be pushed to the clipboard.
    */
    static void PushStringToClipboard(const std::string& str);


    static std::string convertToUtf8(const std::wstring &unicodeText);

private:
    static DWORD lastClipboardSequenceNumber; ///< Last known clipboard sequence number

    /**
     * @brief Sets the data type of the clipboard content.
     *
     * This private method is used to set the type of data currently stored in the clipboard.
     * It is utilized internally to manage and categorize clipboard data based on its type, such as text, image, or file.
     *
     * @param type The type of the clipboard data, as defined in the ClipboardDataType enum.
     */
    void setDataType(ClipboardDataType type);
};

#endif // CLIPBOARDMANAGER_H
