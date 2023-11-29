/**
 * @file ClipboardContent.h
 * @brief Header file for the ClipboardContent class.
 *
 *
 * @author Yaopeng Xie
 * @date 2023/11/14
 */

#ifndef WINDOWSCLIENTTEST1_CLIPBOARDCONTENT_H
#define WINDOWSCLIENTTEST1_CLIPBOARDCONTENT_H

#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

/**
 * @brief Clipboard data types enumeration.
 *
 * Defines the types of data that can be stored in the clipboard.
 */
enum class ClipboardDataType {
    Text,   ///< Represents text data
    Image,  ///< Represents image data
    File,   ///< Represents file data
    Unknown ///< Represents an unknown or unsupported data type
};

/**
 * @brief Encapsulates clipboard data and its type.
 *
 * This class is used to store clipboard data along with its type. It also stores the time when the data was updated.
 */
class ClipboardContent {
public:
    /**
     * @brief Constructs a ClipboardContent object with given parameters.
     * @param type The type of the clipboard data.
     * @param textData The text data, if the type is Text.
     * @param binaryData The binary data (for images or files).
     * @param updateTime The time when the clipboard data was updated.
     */
    ClipboardContent(ClipboardDataType type, const std::wstring& textData,
                     const std::vector<unsigned char>& binaryData, const std::string& updateTime = "");

    // Getters and setters
    ClipboardDataType getDataType() const;
    void setDataType(ClipboardDataType type);
    std::wstring getTextData() const;
    void setTextData(const std::wstring& text);
    const std::vector<unsigned char>& getBinaryData() const;
    void setBinaryData(const std::vector<unsigned char>& binaryData);
    std::string getUpdateTime() const;
    void setUpdateTime(const std::string& updateTime);

private:
    ClipboardDataType dataType; ///< Data type of the clipboard content
    std::wstring textContent; ///< Text data content
    std::vector<unsigned char> binaryContent; ///< Binary data content (images, files)
    std::string updateTime; ///< Time when the clipboard content was updated
};


#endif //WINDOWSCLIENTTEST1_CLIPBOARDCONTENT_H
