#include "SpinoCore/Logs/EngineLogger.h"
#include <chrono>
#include <iostream>
#include <SDL3/SDL.h>

namespace EngineLogger {
    constexpr std::string_view COLOR_RESET = "\033[0m";
    constexpr std::string_view COLOR_INFO = "\033[32m";
    constexpr std::string_view COLOR_WARN = "\033[33m";
    constexpr std::string_view COLOR_ERROR = "\033[31m";
    constexpr std::string_view COLOR_FATAL = "\033[41;37m";

    namespace detail {
        std::atomic<Level> userLoggingLevel{Level::Info};
        std::atomic<Level> SDLInternalLoggingLevel{Level::Info};

        const std::chrono::time_zone* timeZone{nullptr};

        void OutputMessage(const Level level, std::string&& message) {
            SDL_LogPriority sdlPriority{};

            switch (level) {
                case Level::Info: sdlPriority = SDL_LOG_PRIORITY_INFO; break;
                case Level::Warn: sdlPriority = SDL_LOG_PRIORITY_WARN; break;
                case Level::Error: sdlPriority = SDL_LOG_PRIORITY_ERROR; break;
                case Level::Fatal: sdlPriority = SDL_LOG_PRIORITY_CRITICAL; break;
                default: break;
            }

            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, sdlPriority, "%s", message.c_str());
        }
    }

    static void SDLCALL CustomLogOutput([[maybe_unused]] void* userdata, const int category, const SDL_LogPriority priority, const char* message) {
        if (category != SDL_LOG_CATEGORY_APPLICATION) {
            const Level currentSDLLevel = detail::SDLInternalLoggingLevel.load(std::memory_order_relaxed);

            if (currentSDLLevel == Level::None) return;
            auto incomingLevel = Level::Info;

            if (priority >= SDL_LOG_PRIORITY_CRITICAL) incomingLevel = Level::Fatal;
            else if (priority >= SDL_LOG_PRIORITY_ERROR) incomingLevel = Level::Error;
            else if (priority >= SDL_LOG_PRIORITY_WARN) incomingLevel = Level::Warn;

            if (currentSDLLevel > incomingLevel) return;
        }

        std::string_view color;
        std::string_view prefix;

        switch (priority) {
            case SDL_LOG_PRIORITY_INFO: color = COLOR_INFO; prefix = "[INFO]"; break;
            case SDL_LOG_PRIORITY_WARN: color = COLOR_WARN; prefix = "[WARN]"; break;
            case SDL_LOG_PRIORITY_ERROR: color = COLOR_ERROR; prefix = "[ERROR]"; break;
            case SDL_LOG_PRIORITY_CRITICAL: color = COLOR_FATAL; prefix = "[FATAL]"; break;
            default: color = COLOR_RESET; prefix = "[TRACE]"; break;
        }

        const auto now = std::chrono::system_clock::now();
        if (detail::timeZone) {
            std::chrono::zoned_time localTime{detail::timeZone, now};
            std::cerr << std::format("{}{} {:%H:%M:%S} - {}{}\n", color, prefix, localTime, message, COLOR_RESET);
        }else {
            std::cerr << std::format("{}{} {:%H:%M:%S} - {}{}\n", color, prefix, now, message, COLOR_RESET);
        }
    }

    void Initialize() {
        detail::timeZone = std::chrono::current_zone();

        SDL_SetLogOutputFunction(CustomLogOutput, nullptr);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
        Info("[ENGINE LOGGER] Logger initialized");
    }

    void Shutdown() {
        Info("[ENGINE LOGGER] Logger finished.");
        SDL_SetLogOutputFunction(nullptr, nullptr);
    }
}