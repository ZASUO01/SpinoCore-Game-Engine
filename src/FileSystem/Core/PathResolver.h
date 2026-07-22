#pragma once
#include <optional>
#include <string_view>

namespace FileSystem::Core::PathResolver {
    struct VirtualPath {
        std::string_view protocol;
        std::string_view path;
    };

    [[nodiscard]] std::optional<VirtualPath> Parse(std::string_view fullVirtualPath);
    [[nodiscard]] bool IsValidPath(std::string_view path);
}