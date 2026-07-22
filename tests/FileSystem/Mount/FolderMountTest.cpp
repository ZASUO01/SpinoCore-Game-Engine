#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "FileSystem/IO/FileReader.h"
#include "FileSystem/Mount/FolderMount.h"
#include "FileSystem/IO/FileWriter.h"
#include "SpinoCore/Logs/EngineLogger.h"

class FolderMountTest : public testing::Test {
protected:
    const std::filesystem::path testDir = "test_dir";
    const std::string testFileName = "test.txt";
    const std::string testFileContent = "This is a test file.";

    static void SetUpTestCase() {
        EngineLogger::SetLoggingLevel(EngineLogger::Level::None);
    }

    static void TearDownTestCase() {
        EngineLogger::SetLoggingLevel(EngineLogger::Level::Info);
    }

    void SetUp() override {
        if (std::filesystem::exists(testDir)) {
            std::filesystem::remove_all(testDir);
        }

        std::filesystem::create_directories(testDir);

        const std::filesystem::path filePath = testDir / testFileName;
        FileSystem::IO::FileWriter writer(filePath.string());
        writer.WriteString(testFileContent);
    }

    void TearDown() override {
        if (std::filesystem::exists(testDir)) {
            std::filesystem::remove_all(testDir);
        }
    }
};

TEST_F(FolderMountTest, ShouldInitializeValidDirectory) {
    FileSystem::Mount::FolderMount mount(testDir);
    EXPECT_TRUE(mount.Initialize());
}

TEST_F(FolderMountTest, ShouldFailInitIfOnInvalidDirectory) {
    FileSystem::Mount::FolderMount mount("invalid_directory");
    EXPECT_FALSE(mount.Initialize());
}

TEST_F(FolderMountTest, ShouldFailIfPathIsAFile) {
    const std::filesystem::path filePath = testDir / testFileName;
    FileSystem::Mount::FolderMount mount(filePath);

    EXPECT_FALSE(mount.Initialize());
}

TEST_F(FolderMountTest, ShouldCheckIfFileExists) {
    FileSystem::Mount::FolderMount mount(testDir);
    ASSERT_TRUE(mount.Initialize());

    EXPECT_TRUE(mount.Exists(testFileName));
}

TEST_F(FolderMountTest, ShouldReadFile) {
    FileSystem::Mount::FolderMount mount(testDir);
    ASSERT_TRUE(mount.Initialize());

    auto data = mount.Read(testFileName);
    ASSERT_TRUE(data.has_value());

    const std::string content(data->begin(), data->end());
    EXPECT_EQ(content, testFileContent);
}

TEST_F(FolderMountTest, ShouldPreventPathTraversal) {
    FileSystem::Mount::FolderMount mount(testDir);
    ASSERT_TRUE(mount.Initialize());

    const std::string invalidPath = "../" + testDir.string() + "/" + testFileName;

    EXPECT_FALSE(mount.Exists(invalidPath));
    EXPECT_FALSE(mount.Read(invalidPath).has_value());
}

TEST_F(FolderMountTest, ShouldPreventAbsolutePaths) {
    FileSystem::Mount::FolderMount mount(testDir);
    ASSERT_TRUE(mount.Initialize());

    const std::filesystem::path absolutePath = std::filesystem::absolute(testDir / testFileName);

    EXPECT_FALSE(mount.Exists(absolutePath.string()));
    EXPECT_FALSE(mount.Read(absolutePath.string()).has_value());
}

TEST_F(FolderMountTest, ShouldCheckReadOnly) {
    const FileSystem::Mount::FolderMount readOnlyMount(testDir, true);
    EXPECT_TRUE(readOnlyMount.IsReadOnly());

    const FileSystem::Mount::FolderMount writeMount(testDir, false);
    EXPECT_FALSE(writeMount.IsReadOnly());
}

TEST_F(FolderMountTest, ShouldWriteFile) {
    std::filesystem::create_directories(testDir / "test");

    FileSystem::Mount::FolderMount mount(testDir, false);
    ASSERT_TRUE(mount.Initialize());

    const bool success = mount.Write("test/test.txt", std::vector<uint8_t>{'T', 'E', 'S', 'T'});

    EXPECT_TRUE(success);

    FileSystem::IO::FileReader reader(testDir / "test" /"test.txt");
    EXPECT_TRUE(reader.IsValid());

    auto data = reader.GetBinary();
    auto text = std::string(data.begin(), data.end());

    EXPECT_EQ(text, "TEST");
}

TEST_F(FolderMountTest, ShouldFailToWriteOnInvalidDirectory) {
    FileSystem::Mount::FolderMount mount(testDir, false);
    ASSERT_TRUE(mount.Initialize());

    const bool success = mount.Write("invalid/test.txt", std::vector<uint8_t>{'T', 'E', 'S', 'T'});

    EXPECT_FALSE(success);
    EXPECT_FALSE(std::filesystem::exists(testDir / "invalid" / "test.txt"));
}

TEST_F(FolderMountTest, ShouldFailToWriteOnReadOnlyMount) {
    FileSystem::Mount::FolderMount mount(testDir, true);
    ASSERT_TRUE(mount.Initialize());

    const bool success = mount.Write(testFileName, std::vector<uint8_t>{'T', 'E', 'S', 'T'});

    EXPECT_FALSE(success);
}