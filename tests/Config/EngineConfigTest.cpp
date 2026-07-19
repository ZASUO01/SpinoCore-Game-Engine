#include <gtest/gtest.h>
#include "SpinoCore/Config/ConfigEntry.h"
#include "SpinoCore/Config/EngineConfig.h"

TEST(ConfigKey, HashValueIsDeterministic) {
    constexpr ConfigKey keyA("ValueX");
    constexpr ConfigKey keyB("ValueX");
    constexpr ConfigKey keyC("ValueY");

    EXPECT_EQ(keyA.hash, keyB.hash);
    EXPECT_NE(keyA.hash, keyC.hash);
}

TEST(ConfigKey, FNV1AlgorithmReturnsCorrectHash) {
    constexpr ConfigKey keyA("ValueX");

    EXPECT_EQ(keyA.hash, 2790082998u);
}

class EngineConfigFixture : public testing::Test {
protected:
    EngineConfig config;

    void SetUp() override {
        config.Set("Vsync", true);
        config.Set("Volume", 0.8f);
        config.Set("MaxFPS", 60);
        config.Set("RendererContext", "OpenGL");
    }
};

TEST_F(EngineConfigFixture, GetReturnsCorrectValuesOnSuccess) {
    EXPECT_EQ(config.Get("Vsync", false), true);
    EXPECT_EQ(config.Get("Volume", 1.0f), 0.8f);
    EXPECT_EQ(config.Get("MaxFPS", 120), 60);
}

TEST_F(EngineConfigFixture, GetReturnsDefaultValuesOnFailure) {
    EXPECT_EQ(config.Get("invalid-key", 0), 0);
}

TEST_F(EngineConfigFixture, StringOverloadReturnsExpectedValues) {
    EXPECT_EQ(config.Get("RendererContext", "Vulkan"), "OpenGL");
    EXPECT_EQ(config.Get("invalid-renderer", "DirectX"), "DirectX");
}

TEST_F(EngineConfigFixture, GetReturnsDefaultValuesOnTypeMismatch) {
    EXPECT_EQ(config.Get("Volume", "Vulkan"), "Vulkan");
    EXPECT_EQ(config.Get<int>("RendererContext", 0), 0);
    EXPECT_EQ(config.Get<std::string>("MaxFPS", "Fallback"), "Fallback");
}

TEST_F(EngineConfigFixture, ReadonlyFlagsPreventsChange) {
    config.Set("PhysicsGravity", 9.81f, true);

    config.Set("PhysicsGravity", 0.0f);
    EXPECT_FLOAT_EQ(config.Get<float>("PhysicsGravity", 0.0f), 9.81f);

    config.Set("PhysicsGravity", 100.0f, false);
    EXPECT_FLOAT_EQ(config.Get<float>("PhysicsGravity", 0.0f), 9.81f);
}

TEST_F(EngineConfigFixture, ReadOnlyFlagsPermitsFirstSet) {
    config.Set("CoreThreadCount", 8, true);
    EXPECT_EQ(config.Get<int>("CoreThreadCount", 0), 8);
}