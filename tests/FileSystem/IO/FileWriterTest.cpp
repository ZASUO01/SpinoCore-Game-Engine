#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "FileSystem/IO/FileReader.h"
#include "FileSystem/IO/FileWriter.h"

class FileWriterTest : public testing::Test {
protected:
    const std::string testFileName = "test_file.txt";

    void SetUp() override {
        if (std::filesystem::exists(testFileName)) {
            std::filesystem::remove(testFileName);
        }
    }

    void TearDown() override {
        if (std::filesystem::exists(testFileName)) {
            std::filesystem::remove(testFileName);
        }
    }
};


TEST_F(FileWriterTest, ShouldCreateFileAndCloseItAtDestruction) {
    {
        const FileSystem::IO::FileWriter writer(testFileName);
        EXPECT_TRUE(writer.IsValid());
    }

    EXPECT_TRUE(std::filesystem::exists(testFileName));
}

TEST_F(FileWriterTest, ShouldFailToOpenInvalidPath) {
    const FileSystem::IO::FileWriter writer("invalid_directory/invalid_file.txt");
    EXPECT_FALSE(writer.IsValid());
}

TEST_F(FileWriterTest, ShouldWriteSequentialStrings) {
    {
        FileSystem::IO::FileWriter writer(testFileName);
        ASSERT_TRUE(writer.IsValid());

        EXPECT_TRUE(writer.WriteString("Chunk1-"));
        EXPECT_TRUE(writer.WriteString("Chunk2"));
    }

    FileSystem::IO::FileReader reader(testFileName);
    auto data = reader.GetBinary();
    auto readText = std::string(data.begin(), data.end());

    EXPECT_EQ(readText, "Chunk1-Chunk2");
}

TEST_F(FileWriterTest, ShouldWriteBinaryData) {
    struct TestBlob {
        uint32_t magic;
        float version;
    };

    TestBlob original{0xC0DEBABE, 2.5f};

    {
        FileSystem::IO::FileWriter writer(testFileName);
        ASSERT_TRUE(writer.IsValid());

        bool success = writer.WriteBinary(&original, sizeof(TestBlob));
        EXPECT_TRUE(success);
    }

    FileSystem::IO::FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    auto data = reader.GetBinary();
    ASSERT_EQ(data.size(), sizeof(TestBlob));

    auto readBlob = reinterpret_cast<const TestBlob*>(data.data());

    EXPECT_EQ(readBlob->magic, 0xC0DEBABE);
    EXPECT_FLOAT_EQ(readBlob->version, 2.5f);
}

TEST_F(FileWriterTest, ShouldAllowDataOverwrite) {
    {
        FileSystem::IO::FileWriter writer(testFileName);
        ASSERT_TRUE(writer.IsValid());

        writer.WriteString("AAAAA");
        writer.WriteString("BB", 2);
    }

    FileSystem::IO::FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    auto data = reader.GetBinary();
    auto text = std::string(data.begin(), data.end());
    EXPECT_EQ(text, "AABBA");
}