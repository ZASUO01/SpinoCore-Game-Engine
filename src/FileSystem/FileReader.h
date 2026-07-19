#pragma once
#include <cstdint>
#include <fstream>
#include <string>

class FileReader {
public:
    explicit FileReader(const std::string& filePath);
    ~FileReader();

    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

    FileReader(FileReader&&) noexcept = default;
    FileReader& operator=(FileReader&&) noexcept = default;

    [[nodiscard]] bool IsValid() const { return mInputStream.is_open() && mInputStream.good();}

    std::ifstream& GetStream() { return mInputStream; }
    std::string GetString(uint64_t offset = 0);
    size_t GetSize();
private:
    bool Seek(uint64_t absoluteOffset);

    std::ifstream mInputStream;
};
