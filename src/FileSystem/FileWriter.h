#pragma once
#include <cstdint>
#include <string>
#include <fstream>

class FileWriter {
    public:
        explicit FileWriter(const std::string& filepath);
        ~FileWriter();

        FileWriter(const FileWriter&) = delete;
        FileWriter& operator=(const FileWriter&) = delete;

        FileWriter(FileWriter&&) noexcept = default;
        FileWriter& operator=(FileWriter&&) noexcept = default;

        [[nodiscard]] bool IsValid() const;

        bool Write(const void* data, size_t sizeBytes);
        bool Write(std::string_view text);

        bool Seek(uint64_t absoluteOffset);
private:
    std::ofstream mOutputStream;
};