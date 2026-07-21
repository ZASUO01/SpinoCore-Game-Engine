#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "FileSystem/IO/FileReader.h"
#include "FileSystem/IO/FileWriter.h"

class FileReaderTest : public testing::Test {
protected:
    const std::string testFileName = "test_file.txt";
    const std::string testText = "This is a test\n made for engine";

    void SetUp() override {
        if (FileSystem::IO::FileWriter writer(testFileName); writer.IsValid()) {
            writer.WriteString(testText);
        }
    }

    void TearDown() override {
        if (std::filesystem::exists(testFileName)) {
            std::filesystem::remove(testFileName);
        }
    }
};

TEST_F(FileReaderTest, ShouldInvalidadeForInexistentFile) {
    FileSystem::IO::FileReader reader("inexistent_file.json");

    EXPECT_FALSE(reader.IsValid());
    EXPECT_EQ(reader.GetSize(), 0);
}

TEST_F(FileReaderTest, ShouldGetCorrectByteSize) {
    FileSystem::IO::FileReader reader(testFileName);

    ASSERT_TRUE(reader.IsValid());
    EXPECT_EQ(reader.GetSize(), testText.size());
}

TEST_F(FileReaderTest, ShouldGetInputStream) {
    FileSystem::IO::FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    std::ifstream& stream = reader.GetStream();
    std::string firstWord;

    stream >> firstWord;

    EXPECT_EQ(firstWord, "This");
}

TEST_F(FileReaderTest, ShouldReadBinaryFile) {
    FileSystem::IO::FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    std::vector<uint8_t> data = reader.GetBinary();

    ASSERT_FALSE(data.empty());

    std::string result(data.begin(), data.end());
    EXPECT_EQ(result, testText);
}

TEST_F(FileReaderTest, ShouldReadBinaryWithOffset) {
    FileSystem::IO::FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    constexpr uint64_t offset = 5;
    std::vector<uint8_t> data = reader.GetBinary(offset);

    ASSERT_FALSE(data.empty());

    std::string result(data.begin(), data.end());
    EXPECT_EQ(result, testText.substr(offset));
}

TEST_F(FileReaderTest, ShouldReturnEmptyOnOutOfBoundsOffset) {
    FileSystem::IO::FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    const uint64_t boundaryOffset = testText.size();
    std::vector<uint8_t> data1 = reader.GetBinary(boundaryOffset);
    EXPECT_TRUE(data1.empty());

    const uint64_t invalidOffset = testText.size() + 10;
    std::vector<uint8_t> data2 = reader.GetBinary(invalidOffset);
    EXPECT_TRUE(data2.empty());
}