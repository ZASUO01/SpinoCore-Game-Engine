#include "FileWriter.h"

namespace FileSystem::IO {
    FileWriter::FileWriter(const std::string& filepath) {
        mOutputStream.open(filepath, std::ios::out | std::ios::trunc | std::ios::binary);
    }

    FileWriter::~FileWriter() {
        if (mOutputStream.is_open()) {
            mOutputStream.flush();
            mOutputStream.close();
        }
    }

    bool FileWriter::IsValid() const {
        return mOutputStream.is_open() && mOutputStream.good();
    }

    bool FileWriter::WriteBinary(const void* data, const size_t sizeBytes, const uint64_t offset) {
        if (!IsValid() || !data || sizeBytes == 0) {
            return false;
        }

        if (offset != IGNORE_OFFSET) {
            if (!Seek(offset)) {
                return false;
            }
        }

        mOutputStream.write(static_cast<const char*>(data), static_cast<std::streamsize>(sizeBytes));
        return mOutputStream.good();
    }

    bool FileWriter::WriteString(const std::string_view text, const uint64_t offset) {
        if (!IsValid() || text.empty()) {
            return false;
        }

        if (offset != IGNORE_OFFSET) {
            if (!Seek(offset)) {
                return false;
            }
        }

        mOutputStream.write(text.data(), static_cast<std::streamsize>(text.size()));
        return mOutputStream.good();
    }

    bool FileWriter::Seek(const uint64_t absoluteOffset)  {
        if (!IsValid()) {
            return false;
        }

        mOutputStream.clear();
        mOutputStream.seekp(static_cast<std::streamoff>(absoluteOffset), std::ios::beg);
        return mOutputStream.good();
    }
}