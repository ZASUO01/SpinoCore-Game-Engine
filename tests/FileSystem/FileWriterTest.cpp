#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "FileSystem/FileReader.h"
#include "FileSystem/FileWriter.h"

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
        const FileWriter writer(testFileName);
        EXPECT_TRUE(writer.IsValid());
    }

    EXPECT_TRUE(std::filesystem::exists(testFileName));
}

TEST_F(FileWriterTest, ShouldWriteStringView) {
    std::string_view text = "This is a test";

    {
        FileWriter writer(testFileName);
        ASSERT_TRUE(writer.IsValid());

        bool success = writer.Write(text);
        EXPECT_TRUE(success);
    }

    FileReader reader(testFileName);
    auto readText = reader.GetString();

    EXPECT_EQ(readText, text);
}

TEST_F(FileWriterTest, ShouldWriteBinaryData) {
    struct TestBlob {
        uint32_t magic;
        float version;
    };

    TestBlob original{0xC0DEBABE, 2.5f};

    {
        FileWriter writer(testFileName);
        ASSERT_TRUE(writer.IsValid());

        bool success = writer.Write(&original, sizeof(TestBlob));
        EXPECT_TRUE(success);
    }

    FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    TestBlob read{};
    auto in = std::move(reader.GetStream());
    in.read(reinterpret_cast<char*>(&read), sizeof(TestBlob));

    EXPECT_EQ(read.magic, 0xC0DEBABE);
    EXPECT_FLOAT_EQ(read.version, 2.5f);
}

TEST_F(FileWriterTest, SeekPermiteSobrescreverDados) {
    {
        FileWriter writer(testFileName);
        ASSERT_TRUE(writer.IsValid());

        writer.Write("AAAAA");

        bool seekSuccess = writer.Seek(2);
        EXPECT_TRUE(seekSuccess);

        writer.Write("BB");
    }

    FileReader reader(testFileName);
    ASSERT_TRUE(reader.IsValid());

    auto text = reader.GetString();
    EXPECT_EQ(text, "AABBA");
}