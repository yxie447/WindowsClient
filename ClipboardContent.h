#ifndef WINDOWSCLIENTTEST1_CLIPBOARDCONTENT_H
#define WINDOWSCLIENTTEST1_CLIPBOARDCONTENT_H

#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

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


#endif //WINDOWSCLIENTTEST1_CLIPBOARDCONTENT_H
