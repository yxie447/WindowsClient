#include "ClipboardContent.h"

// Implement the ClipboardContent class methods
ClipboardContent::ClipboardContent(ClipboardDataType type, const std::wstring &textData,
                                   const std::vector<unsigned char> &binaryData, const std::string &updateTime)
        : dataType(type), textContent(textData), binaryContent(binaryData), updateTime(updateTime) {}


ClipboardDataType ClipboardContent::getDataType() const {
    return dataType;
}

void ClipboardContent::setDataType(ClipboardDataType type) {
    dataType = type;
}

std::wstring ClipboardContent::getTextData() const {
    return textContent;
}

void ClipboardContent::setTextData(const std::wstring &text) {
    textContent = text;
}

const std::vector<unsigned char> &ClipboardContent::getBinaryData() const {
    return binaryContent;
}

void ClipboardContent::setBinaryData(const std::vector<unsigned char> &binaryData) {
    this->binaryContent = binaryData;
}

std::string ClipboardContent::getUpdateTime() const {
    return updateTime;
}

void ClipboardContent::setUpdateTime(const std::string &updateTime) {
    this->updateTime = updateTime;
}