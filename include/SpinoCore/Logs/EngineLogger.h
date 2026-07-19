#pragma once
#include <atomic>
#include <cstdint>
#include <format>
#include <string>

namespace EngineLogger {
    enum class Level: uint8_t {
        Info = 0,
        Warn = 1,
        Error = 2,
        Fatal = 3,
        None = 4
    };

    namespace detail {
        extern std::atomic<Level> userLoggingLevel;
        extern std::atomic<Level> SDLInternalLoggingLevel;

        void OutputMessage(Level level, std::string&& message);
    }

    inline void SetLoggingLevel(const Level level) {
        detail::userLoggingLevel.store(level, std::memory_order_relaxed);
    }

    inline void SetSDLInternalLoggingLevel(const Level level) {
        detail::SDLInternalLoggingLevel.store(level, std::memory_order_relaxed);
    }

    void Initialize();
    void Shutdown();

    template <typename... Args>
    void Info(std::format_string<Args...> fmt, Args&&... args) {
        if (detail::userLoggingLevel.load(std::memory_order_relaxed) > Level::Info) return;
        detail::OutputMessage(Level::Info, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Warn(std::format_string<Args...> fmt, Args&&... args) {
        if (detail::userLoggingLevel.load(std::memory_order_relaxed) > Level::Warn) return;
        detail::OutputMessage(Level::Warn, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Error(std::format_string<Args...> fmt, Args&&... args) {
        if (detail::userLoggingLevel.load(std::memory_order_relaxed) > Level::Error) return;
        detail::OutputMessage(Level::Error, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Fatal(std::format_string<Args...> fmt, Args&&... args) {
        if (detail::userLoggingLevel.load(std::memory_order_relaxed) > Level::Fatal) return;
        detail::OutputMessage(Level::Fatal, std::format(fmt, std::forward<Args>(args)...));
    }
}