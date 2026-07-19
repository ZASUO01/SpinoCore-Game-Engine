#include "JSONUtils.h"
#include <nlohmann/json.hpp>
#include "SpinoCore/Logs/EngineLogger.h"

namespace {
    using json = nlohmann::json;

    uint32_t CalculateDepth(const json& j) {
        if (!j.is_object() && !j.is_array()) {
            return 0;
        }

        uint32_t maxChildDepth = 0;
        for (const auto& element : j) {
            if (const uint32_t depth = CalculateDepth(element); depth > maxChildDepth) {
                maxChildDepth = depth;
            }
        }
        return maxChildDepth + 1;
    }
}

namespace JSONUtils {
    bool LoadJSON(
        std::istream& inputStream,
        json& outJSON,
        const std::vector<std::string>& requiredFields,
        const unsigned int maxDepth
    ) {
        try {
            outJSON = json::parse(inputStream);
        }catch (json::parse_error& err) {
            EngineLogger::Error("[JSON HELPER] Failed to parse malformed stream: {}", err.what());

            outJSON.clear();
            return false;
        }

        if (maxDepth > 0) {
            if (CalculateDepth(outJSON) > maxDepth) {
                EngineLogger::Error("[JSON HELPER] JSON nested paths exceeds maximum accepted depth");

                outJSON.clear();
                return false;
            }
        }

        std::string missingField;
        if (!std::ranges::all_of(requiredFields.begin(), requiredFields.end(), [&outJSON, &missingField](const auto& field) {
            try {
                if (!outJSON.contains(nlohmann::json::json_pointer(field))) {
                    missingField = field;
                    return false;
                }
            }catch (const nlohmann::json::parse_error& e) {
                EngineLogger::Error("[JSON HELPER]  Failed to parse malformed path {}: {}", field, e.what());
                return false;
            }

            return true;
        })) {
            EngineLogger::Error("[JSON HELPER] Failed to parse required field {}.", missingField);

            outJSON.clear();
            return false;
        }

        return true;
    }
}