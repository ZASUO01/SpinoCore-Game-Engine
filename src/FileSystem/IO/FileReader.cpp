#include "FileReader.h"

namespace FileSystem::IO {
    FileReader::FileReader(const std::string& filePath) {
        mInputStream.open(filePath, std::ios::in | std::ios::binary);
    }

    FileReader::~FileReader() {
        if (mInputStream.is_open()) {
            mInputStream.close();
        }
    }

    std::vector<uint8_t> FileReader::GetBinary(const uint64_t offset) {
        if (!IsValid()) {
            return {};
        }

        if (!Seek(offset)) {
            return {};
        }

        const size_t totalSize = GetSize();

        if (totalSize <= offset) {
            return {};
        }

        const size_t bytesToRead = totalSize - offset;

        std::vector<uint8_t> buffer(bytesToRead);

        if (mInputStream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(bytesToRead))) {
            return buffer;
        }

        return {};
    }

    size_t FileReader::GetSize() {
        if (!IsValid()) {
            return 0;
        }

        const std::streampos currentPos = mInputStream.tellg();
        if (currentPos == std::streampos(-1)) {
            return 0;
        }

        mInputStream.seekg(0, std::ios::end);
        const std::streampos endPos = mInputStream.tellg();

        mInputStream.seekg(currentPos, std::ios::beg);

        if (endPos == std::streampos(-1)) {
            return 0;
        }

        return endPos;
    }

    bool FileReader::Seek(const uint64_t absoluteOffset)   {
        if (!IsValid()) {
            return false;
        }

        mInputStream.clear();
        mInputStream.seekg(static_cast<std::streamoff>(absoluteOffset), std::ios::beg);
        return mInputStream.good();
    }
}