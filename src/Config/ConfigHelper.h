#pragma once
#include <memory>

class EngineConfig;

namespace ConfigHelper {
    [[nodiscard]] std::unique_ptr<EngineConfig> GetInitialConfig();
}