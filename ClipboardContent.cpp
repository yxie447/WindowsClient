/**
 * @file ClipboardContent.cpp
 * @brief Implementation file for the ClipboardContent class.
 *
 * Implements the methods defined in ClipboardContent.h. This file contains the data storage for the clipboard
 *
 * @author Yaopeng Xie
 * @date 2023/11/14
 */

#include "ClipboardContent.h"

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