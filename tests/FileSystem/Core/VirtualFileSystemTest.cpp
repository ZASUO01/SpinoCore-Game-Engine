#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "FileSystem/Core/VirtualFileSystem.h"
#include "FileSystem/Mount/MountPoint.h"

class MockMountPoint : public FileSystem::Mount::MountPoint {
public:
    bool fakeExists = false;
    std::optional<std::vector<uint8_t>> fakeRead = std::nullopt;
    bool fakeWrite = false;
    bool fakeReadOnly = true;

    mutable std::string lastQueriedPath;
    mutable int existsCallCount = 0;
    mutable int readCallCount = 0;
    int writeCallCount = 0;

    bool Initialize() override { return true; }

    bool Exists(const std::string_view localPath) const override {
        lastQueriedPath = localPath;
        existsCallCount++;
        return fakeExists;
    }

    std::optional<std::vector<uint8_t>> Read(const std::string_view localPath) const override {
        lastQueriedPath = localPath;
        readCallCount++;
        return fakeRead;
    }

    bool Write(const std::string_view localPath, const std::vector<uint8_t>& data) override {
        lastQueriedPath = localPath;
        writeCallCount++;
        return fakeWrite;
    }

    bool IsReadOnly() const override {
        return fakeReadOnly;
    }
};

class VirtualFileSystemTest : public testing::Test {
protected:
    std::unique_ptr<FileSystem::Core::VirtualFileSystem> vfs;

    void SetUp() override {
        vfs = FileSystem::Core::VirtualFileSystem::Create();
    }
};

TEST_F(VirtualFileSystemTest, ShouldCreateSuccessfully) {
    EXPECT_NE(vfs, nullptr);
}

TEST_F(VirtualFileSystemTest, ShouldFailToMountWithEmptyProtocolOrNullPtr) {
    auto mock = std::make_unique<MockMountPoint>();

    EXPECT_FALSE(vfs->Mount("", std::move(mock)));
    EXPECT_FALSE(vfs->Mount("test", nullptr));
}

TEST_F(VirtualFileSystemTest, ShouldReturnFalseForUnmountedProtocol) {
    EXPECT_FALSE(vfs->Exists("test://folder/image.png"));
    EXPECT_FALSE(vfs->Read("test://folder/image.png").has_value());
    EXPECT_FALSE(vfs->Write("test://folder/file.txt", {'D', 'A', 'T', 'A'}));
}

TEST_F(VirtualFileSystemTest, ShouldReadToCorrectProtocol) {
    auto mockGame = std::make_unique<MockMountPoint>();
    mockGame->fakeExists = true;
    mockGame->fakeRead = std::vector<uint8_t>{'T', 'E', 'S', 'T'};

    const MockMountPoint* rawMock = mockGame.get();

    ASSERT_TRUE(vfs->Mount("proto", std::move(mockGame)));

    const auto data = vfs->Read("proto://file.json");

    ASSERT_TRUE(data.has_value());
    EXPECT_EQ(data->size(), 4);
    EXPECT_EQ(rawMock->lastQueriedPath, "file.json");
    EXPECT_EQ(rawMock->readCallCount, 1);
}

TEST_F(VirtualFileSystemTest, ShouldRespectMountOrder) {
    auto base = std::make_unique<MockMountPoint>();
    base->fakeExists = true;
    base->fakeRead = std::vector<uint8_t>{'B', 'A', 'S', 'E'};
    const MockMountPoint* rawBase = base.get();

    auto patch = std::make_unique<MockMountPoint>();
    patch->fakeExists = true;
    patch->fakeRead = std::vector<uint8_t>{'P', 'A', 'T', 'C', 'H'};
    const MockMountPoint* rawPatch = patch.get();

    ASSERT_TRUE(vfs->Mount("proto", std::move(base)));
    ASSERT_TRUE(vfs->Mount("proto", std::move(patch)));

    auto data = vfs->Read("proto://folder/file.txt");

    ASSERT_TRUE(data.has_value());

    const std::string result(data->begin(), data->end());
    EXPECT_EQ(result, "PATCH");

    EXPECT_EQ(rawPatch->readCallCount, 1);
    EXPECT_EQ(rawBase->readCallCount, 0);
}

TEST_F(VirtualFileSystemTest, ShouldSearchSequentiallyInMounts) {
    auto base = std::make_unique<MockMountPoint>();
    base->fakeExists = true;
    base->fakeRead = std::vector<uint8_t>{'B', 'A', 'S', 'E'};
    const MockMountPoint* rawBase = base.get();

    auto patch = std::make_unique<MockMountPoint>();
    patch->fakeExists = false;
    const MockMountPoint* rawPatch = patch.get();

    ASSERT_TRUE(vfs->Mount("proto", std::move(base)));
    ASSERT_TRUE(vfs->Mount("proto", std::move(patch)));

    const auto data = vfs->Read("proto://folder/file.txt");

    ASSERT_TRUE(data.has_value());

    EXPECT_EQ(rawPatch->existsCallCount, 1);
    EXPECT_EQ(rawPatch->readCallCount, 0);
    EXPECT_EQ(rawBase->readCallCount, 1);
}

TEST_F(VirtualFileSystemTest, ShouldWriteToFirstWritableMount) {
    auto base = std::make_unique<MockMountPoint>();
    base->fakeReadOnly = true;
    const MockMountPoint* rawBase = base.get();

    auto patch1 = std::make_unique<MockMountPoint>();
    patch1->fakeReadOnly = false;
    patch1->fakeWrite = true;
    const MockMountPoint* rawPatch1 = patch1.get();

    auto patch2 = std::make_unique<MockMountPoint>();
    patch2->fakeReadOnly = true;
    const MockMountPoint* rawPatch2 = patch2.get();

    ASSERT_TRUE(vfs->Mount("proto", std::move(base)));
    ASSERT_TRUE(vfs->Mount("proto", std::move(patch1)));
    ASSERT_TRUE(vfs->Mount("proto", std::move(patch2)));

    const bool success = vfs->Write("proto://folder/file.txt", {'D', 'A', 'T', 'A'});

    EXPECT_TRUE(success);

    EXPECT_EQ(rawBase->writeCallCount, 0);
    EXPECT_EQ(rawPatch1->writeCallCount, 1);
    EXPECT_EQ(rawPatch1->lastQueriedPath, "folder/file.txt");
    EXPECT_EQ(rawPatch2->writeCallCount, 0);
}