#include "FileReader.h"

FileReader::FileReader(const std::string& filePath) {
    mInputStream.open(filePath, std::ios::in | std::ios::binary);
}

FileReader::~FileReader() {
    if (mInputStream.is_open()) {
        mInputStream.close();
    }
}

std::string FileReader::GetString(const uint64_t offset) {
    if (!IsValid()) {
        return {};
    }

    const size_t size = GetSize();
    if (size == 0) {
        return{};
    }

    if (offset >= size) return {};

    const size_t bytesToRead = size - offset;
    std::string content(bytesToRead, '\0');

    if (Seek(offset)) {
        mInputStream.read(content.data(), static_cast<std::streamsize>(bytesToRead));
    }

    return content;
}

size_t FileReader::GetSize() {
    if (!IsValid()) {
        return 0;
    }

    const std::streampos currentPos = mInputStream.tellg();
    mInputStream.seekg(0, std::ios::end);

    const size_t size = mInputStream.tellg();
    mInputStream.seekg(currentPos, std::ios::beg);

    return size;
}

bool FileReader::Seek(const uint64_t absoluteOffset)   {
    if (!IsValid()) {
        return false;
    }

    mInputStream.clear();
    mInputStream.seekg(static_cast<int>(absoluteOffset), std::ios::beg);
    return mInputStream.good();
}
