#include <gtest/gtest.h>
#include "FileSystem/Core/PathResolver.h"
#include "SpinoCore/Logs/EngineLogger.h"

using namespace FileSystem::Core;

class PathResolverTest : public testing::Test {
protected:
    static void SetUpTestCase() {
        EngineLogger::SetLoggingLevel(EngineLogger::Level::None);
    }

    static void TearDownTestCase() {
        EngineLogger::SetLoggingLevel(EngineLogger::Level::Info);
    }
};

TEST_F(PathResolverTest, ShouldParseValidVirtualPath) {
    const auto parsed = PathResolver::Parse("proto://folder/TEST_F.txt");

    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(parsed->protocol, "proto");
    EXPECT_EQ(parsed->path, "folder/TEST_F.txt");
}

TEST_F(PathResolverTest, ShouldFailMalformedVirtualPath) {
    EXPECT_FALSE(PathResolver::Parse("").has_value());
    EXPECT_FALSE(PathResolver::Parse("folder/test.txt").has_value());
    EXPECT_FALSE(PathResolver::Parse("://folder/test.txt").has_value());
    EXPECT_FALSE(PathResolver::Parse("proto://").has_value());
}

TEST_F(PathResolverTest, ShouldValidadePathsWithAllowedCharacters) {
    EXPECT_TRUE(PathResolver::IsValidPath("folder/test.txt"));
    EXPECT_TRUE(PathResolver::IsValidPath("folder/folder_1/file-1.txt"));
    EXPECT_TRUE(PathResolver::IsValidPath("file_v2.json"));
    EXPECT_TRUE(PathResolver::IsValidPath("folder.with.dots/file.txt"));
}

TEST_F(PathResolverTest, ShouldInvalidateMalformedPaths) {
    EXPECT_FALSE(PathResolver::IsValidPath(""));
    EXPECT_FALSE(PathResolver::IsValidPath("/folder/test.png"));
    EXPECT_FALSE(PathResolver::IsValidPath("folder//test.png"));
    EXPECT_FALSE(PathResolver::IsValidPath("folder/folder_1///test.obj"));
}

TEST_F(PathResolverTest, ShouldInvalidatePathWithForbiddenCharacters) {
    EXPECT_FALSE(PathResolver::IsValidPath("folder/test space.png"));
    EXPECT_FALSE(PathResolver::IsValidPath("folder\\backslash.png"));
    EXPECT_FALSE(PathResolver::IsValidPath("folder/test@file.obj"));
    EXPECT_FALSE(PathResolver::IsValidPath("folder/file#1.txt"));
    EXPECT_FALSE(PathResolver::IsValidPath("folder/file!.json"));
}
