#include "PathResolver.h"

namespace FileSystem::Core::PathResolver {
    std::optional<VirtualPath> Parse(std::string_view fullVirtualPath){
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

    bool IsValidPath(const std::string_view path) {
        if (path.empty()) {
            if (path.empty()) return false;
        }

        if (path.front() == '/') {
            return false;
        }

        bool lastWasSlash = false;

        for (const char c : path) {
            if (c == '/') {
                if (lastWasSlash) {
                    return false;
                }
                lastWasSlash = true;
                continue;
            }

            lastWasSlash = false;

            const bool isAlphaNumeric = (c >= 'a' && c <= 'z') ||
                                        (c >= 'A' && c <= 'Z') ||
                                        (c >= '0' && c <= '9');

            if (const bool isValidSymbol = c == '_' || c == '-' || c == '.'; !isAlphaNumeric && !isValidSymbol) {
                return false;
            }
        }

        return true;
    }
}
