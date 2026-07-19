#include <fstream>
#include <gtest/gtest.h>
#include "FileSystem/FileReader.h"
#include "FileSystem/FileWriter.h"
#include "nlohmann/json.hpp"
#include "SpinoCore/Logs/EngineLogger.h"
#include "Utils/JSONUtils.h"

using json = nlohmann::json;

class JSONUtilsFixture : public testing::Test {
protected:
    const std::string validFileName{"Valid.json"};
    const std::string invalidFileName{"Invalid.json"};

    static void SetUpTestSuite() {
        EngineLogger::SetLoggingLevel(EngineLogger::Level::None);
    }

    static void TearDownTestSuite() {
        EngineLogger::SetLoggingLevel(EngineLogger::Level::Info);
    }

    void SetUp() override {
        FileWriter validFile(validFileName);
        validFile.Write(R"({
            "name": "SpinoCore-Engine",
            "FPS": 60,
            "assets": {
                "textures": [
                    { "name": "Player", "path": "player.png" }
                ]
            }
        })");

        FileWriter invalidFile(invalidFileName);
        invalidFile.Write(R"({ "name": "Missing closure" )");
    }

    void TearDown() override {
        std::remove(validFileName.c_str());
        std::remove(invalidFileName.c_str());
    }
};


TEST_F(JSONUtilsFixture, ShouldLoadValidJson) {
    FileReader reader(validFileName);
    EXPECT_TRUE(reader.IsValid());

    json result;
    const bool success = JSONUtils::LoadJSON(reader.GetStream(), result);

    EXPECT_TRUE(success);
    EXPECT_EQ(result["name"], "SpinoCore-Engine");
}

TEST_F(JSONUtilsFixture, ShouldFailOnMalformedJsonSyntax) {
    FileReader reader(invalidFileName);
    EXPECT_TRUE(reader.IsValid());

    json result;
    const bool success = JSONUtils::LoadJSON(reader.GetStream(), result);

    EXPECT_FALSE(success);
    EXPECT_TRUE(result.empty());
}

TEST_F(JSONUtilsFixture, ShouldPassWithValidRequiredFields) {
    FileReader reader(validFileName);
    EXPECT_TRUE(reader.IsValid());

    json result;
    const std::vector<std::string> requirements = { "/name", "/assets/textures" };

    const bool success = JSONUtils::LoadJSON(reader.GetStream(), result, requirements);
    EXPECT_TRUE(success);
}

TEST_F(JSONUtilsFixture, ShouldFailWhenRequiredFieldIsMissing) {
    FileReader reader(validFileName);
    EXPECT_TRUE(reader.IsValid());

    json result;
    const std::vector<std::string> requirements = { "/name", "/assets/audio" };

    const bool success = JSONUtils::LoadJSON(reader.GetStream(), result, requirements);
    EXPECT_FALSE(success);
    EXPECT_TRUE(result.empty());
}

TEST_F(JSONUtilsFixture, ShouldPassWhenDepthIsWithinLimits) {
    FileReader reader(validFileName);
    EXPECT_TRUE(reader.IsValid());

    json result;

    const bool success = JSONUtils::LoadJSON(reader.GetStream(), result, {}, 4);
    EXPECT_TRUE(success);
}

TEST_F(JSONUtilsFixture, ShouldFailWhenExceedsMaxDepth) {
    FileReader reader(validFileName);
    EXPECT_TRUE(reader.IsValid());

    json result;

    const bool success = JSONUtils::LoadJSON(reader.GetStream(), result, {}, 3);
    EXPECT_FALSE(success);
    EXPECT_TRUE(result.empty());
}