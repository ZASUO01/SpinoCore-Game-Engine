#include "PathResolver.h"

#include "SpinoCore/Logs/EngineLogger.h"

namespace FileSystem::Core::PathResolver {
    std::optional<VirtualPath> Parse(std::string_view fullVirtualPath){
        if (fullVirtualPath.empty()) {
            return std::nullopt;
        }

        constexpr std::string_view delimiter = "://";
        const size_t delimiterPos = fullVirtualPath.find(delimiter);

        if (delimiterPos == std::string_view::npos) {
            EngineLogger::Warn("[PATH RESOLVER] Missing '://' delimiter in path: '{}'", fullVirtualPath);
            return std::nullopt;
        }

        if (delimiterPos == 0) {
            EngineLogger::Warn("[PATH RESOLVER] Protocol cannot be empty: '{}'", fullVirtualPath);
            return std::nullopt;
        }

        const size_t pathStart = delimiterPos + delimiter.length();

        if (pathStart >= fullVirtualPath.length()) {
            EngineLogger::Warn("[PATH RESOLVER] Local path cannot be empty after protocol: '{}'", fullVirtualPath);
            return std::nullopt;
        }

        return VirtualPath{
            fullVirtualPath.substr(0, delimiterPos),
            fullVirtualPath.substr(pathStart)
        };
    }

    bool IsValidPath(const std::string_view path) {
        if (path.empty()) {
            return false;
        }

        if (path.front() == '/') {
            EngineLogger::Warn("[PATH RESOLVER] Paths cannot start with a slash: '{}'", path);
            return false;
        }

        bool lastWasSlash = false;

        for (const char c : path) {
            if (c == '/') {
                if (lastWasSlash) {
                    EngineLogger::Warn("[PATH RESOLVER] Consecutive slashes '//' are forbidden: '{}'", path);
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
                EngineLogger::Warn("[PATH RESOLVER] Invalid character '{}' found in path: '{}'", c, path);
                return false;
            }
        }

        return true;
    }
}
