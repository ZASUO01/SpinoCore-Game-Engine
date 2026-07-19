#pragma once
#include <unordered_map>
#include "ConfigEntry.h"

class EngineConfig {
public:
    EngineConfig();
    ~EngineConfig() = default;

    void Set(const ConfigKey& key, const ConfigValue& value, bool readOnly = false);

    template <typename T>
    [[nodiscard]] T Get(const ConfigKey& key, const T& defaultValue) const {
        if (const auto it = mConfigEntries.find(key.hash); it != mConfigEntries.end()) {
            if (const T* value = std::get_if<T>(&it->second.value)) {
                return *value;
            }
        }

        return defaultValue;
    }

    [[nodiscard]] std::string Get(const ConfigKey& key, const char* defaultValue) const {
        return Get<std::string>(key, std::string(defaultValue));
    }

private:
    std::unordered_map<uint32_t, ConfigEntry> mConfigEntries;
};