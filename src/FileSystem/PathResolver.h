#pragma once
#include <optional>
#include <string_view>

namespace PathResolver {
    struct VirtualPath {
        std::string_view protocol;
        std::string_view path;
    };

    static std::optional<VirtualPath> Parse(std::string_view fullVirtualPath) noexcept {
        if (fullVirtualPath.empty()) {
            return std::nullopt;
        }

        constexpr std::string_view delimiter = "://";
        const size_t delimiterPos = fullVirtualPath.find(delimiter);

        if (delimiterPos == std::string_view::npos || delimiterPos == 0) {
            return std::nullopt;
        }

        const size_t pathStart = delimiterPos + delimiter.length();


        if (pathStart >= fullVirtualPath.length()) {
            return std::nullopt;
        }

        return VirtualPath{
            fullVirtualPath.substr(0, delimiterPos),
            fullVirtualPath.substr(pathStart)
        };
    }
}
