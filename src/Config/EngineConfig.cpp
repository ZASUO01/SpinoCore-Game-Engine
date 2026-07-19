#include "SpinoCore/Config/EngineConfig.h"

EngineConfig::EngineConfig() = default;

void EngineConfig::Set(const ConfigKey &key, const ConfigValue &value, const bool readOnly) {
    if (const auto it = mConfigEntries.find(key.hash); it != mConfigEntries.end() && it->second.isReadOnly) {
        return;
    }

    mConfigEntries[key.hash] = ConfigEntry{value, readOnly};
}