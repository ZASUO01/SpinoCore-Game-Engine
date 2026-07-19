#pragma once
#include <cstdint>
#include <string_view>

namespace ConfigConstants {
    inline constexpr std::string_view ENGINE_VERSION = "1.0.0";

    inline constexpr std::string_view APP_NAME = "SpinoCore Engine";
    inline constexpr std::string_view APP_VERSION = "1.0.0";
    inline constexpr std::string_view APP_IDENTIFIER = "SpinoCore Engine";
    inline constexpr std::string_view APP_CREATOR = "ZASUO01";
    inline constexpr std::string_view APP_URL = "";

    inline constexpr uint8_t LOG_LEVEL = 0;
    inline constexpr uint8_t INTERNAL_LOG_LEVEL = 0;
}
