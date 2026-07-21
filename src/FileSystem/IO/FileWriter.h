#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <limits>

namespace FileSystem::IO {
    class FileWriter {
    public:
        explicit FileWriter(const std::string& filepath);
        ~FileWriter();

        FileWriter(const FileWriter&) = delete;
        FileWriter& operator=(const FileWriter&) = delete;

        FileWriter(FileWriter&&) noexcept = default;
        FileWriter& operator=(FileWriter&&) noexcept = default;

        [[nodiscard]] bool IsValid() const;

        static constexpr uint64_t IGNORE_OFFSET = std::numeric_limits<uint64_t>::max();

        bool WriteBinary(const void* data, size_t sizeBytes, uint64_t offset = IGNORE_OFFSET);
        bool WriteString(std::string_view text, uint64_t offset = IGNORE_OFFSET);

    private:
        bool Seek(uint64_t absoluteOffset);

        std::ofstream mOutputStream;
    };
}
