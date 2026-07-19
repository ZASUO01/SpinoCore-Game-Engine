#pragma once
#include "nlohmann/json_fwd.hpp"

namespace JSONUtils {
    using json = nlohmann::json;

    [[nodiscard]] bool LoadJSON(
        std::istream& inputStream,
        json& outJSON,
        const std::vector<std::string>& requiredFields = {},
        unsigned int maxDepth = 5
    );
}
