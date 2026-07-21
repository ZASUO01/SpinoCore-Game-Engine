#include "ConfigHelper.h"
#include "../FileSystem/IO/FileReader.h"
#include "SpinoCore/Config/EngineConfig.h"
#include "Constants.h"
#include "nlohmann/json.hpp"
#include "Utils/JSONUtils.h"

namespace {
    struct LogsConfig {
        uint8_t logLevel = ConfigConstants::LOG_LEVEL;
        uint8_t internalLogLevel = ConfigConstants::INTERNAL_LOG_LEVEL;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(LogsConfig, logLevel, internalLogLevel);
}

namespace ConfigHelper {
    [[nodiscard]] std::unique_ptr<EngineConfig> GetInitialConfig() {
        using json = nlohmann::json;
        auto config = std::make_unique<EngineConfig>();

        LogsConfig logsConfig;
        
        if (FileSystem::IO::FileReader file(std::string{ConfigConstants::CONFIG_FILEPATH}); file.IsValid()) {
            if (json root; JSONUtils::LoadJSON(file.GetStream(), root) && root.is_object()) {
                if (root.contains("logs") && root["logs"].is_object()) {
                    root["logs"].get_to(logsConfig);
                }
            }
        }

        config->Set("engine.logs.loglevel", logsConfig.logLevel, true);
        config->Set("engine.logs.internalLogLevel", logsConfig.internalLogLevel, true);

        return config;
    }
}
