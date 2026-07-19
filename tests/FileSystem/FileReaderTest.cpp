#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "FileSystem/FileReader.h"
#include "FileSystem/FileWriter.h"

class FileReaderTest : public testing::Test {
protected:
    const std::string testFileName = "test_file.txt";
    const std::string testText = "This is a test\n made for engine";

    void SetUp() override {
        if (FileWriter writer(testFileName); writer.IsValid()) {
            writer.Write(testText);
        }
    }

    void TearDown() override {
        if (std::filesystem::exists(testFileName)) {
            std::filesystem::remove(testFileName);
        }
    }
};

TEST_F(FileReaderTest, ShouldInvalidadeForInexistentFile) {
    FileReader reader("inexistent_file.json");

    EXPECT_FALSE(reader.IsValid());
    EXPECT_EQ(reader.GetSize(), 0);
}

TEST_F(FileReaderTest, ShouldLoadStringCorrectly) {
    FileReader reader(testFileName);

    ASSERT_TRUE(reader.IsValid());
    std::string content = reader.GetString();

    EXPECT_EQ(content, testText);
}

TEST_F(FileReaderTest, ShouldGetCorrectByteSize) {
    FileReader reader(testFileName);

    ASSERT_TRUE(reader.IsValid());
    EXPECT_EQ(reader.GetSize(), testText.size());
}

TEST_F(FileReaderTest, ShouldNavigateWithSeek) {
    FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    auto str = reader.GetString(5);
    EXPECT_EQ(str, "is a test\n made for engine");
}

TEST_F(FileReaderTest, ShouldGetInputStream) {
    FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    std::ifstream& stream = reader.GetStream();
    std::string firstWord;

    stream >> firstWord;

    EXPECT_EQ(firstWord, "This");
}