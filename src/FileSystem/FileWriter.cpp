#include "FileWriter.h"

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

bool FileWriter::Write(const void* data, const size_t sizeBytes) {
    if (!IsValid()) {
        return false;
    }

    mOutputStream.write(static_cast<const char*>(data), sizeBytes);
    return mOutputStream.good();
}

bool FileWriter::Write(const std::string_view text) {
    if (!IsValid()) {
        return false;
    }

    mOutputStream.write(text.data(), text.size());
    return mOutputStream.good();
}

bool FileWriter::Seek(const uint64_t absoluteOffset)  {
    if (!IsValid()) {
        return false;
    }

    mOutputStream.clear();
    mOutputStream.seekp(absoluteOffset, std::ios::beg);
    return mOutputStream.good();
}

