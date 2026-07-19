#include "SpinoCore/Engine.h"

#include "Config/ConfigHelper.h"
#include "Config/Constants.h"
#include "SpinoCore/Config/EngineConfig.h"
#include "SpinoCore/Logs/EngineLogger.h"

// =====================================
// PIMPL INTERNAL MODULES IMPLEMENTATION
// =====================================
struct Engine::InternalModules {
    // Low level submodules
    std::unique_ptr<EngineConfig> engineConfig;
    std::unique_ptr<EngineConfig> appConfig;
};

// =====================================
// ENGINE CORE IMPLEMENTATION
// =====================================
std::unique_ptr<Engine> Engine::Create() {
    auto engine = std::make_unique<Engine>(ConstructorKey{});

    if (!engine->Initialize()) {
        return nullptr;
    }

    return engine;
}

Engine::Engine(ConstructorKey)
:mInternalModules(std::make_unique<InternalModules>())
{}

Engine::~Engine() {
    Shutdown();
}

void Engine::Run() {

}

bool Engine::Initialize() {
    SetupConfig();
    SetupLogger();

    return true;
}

void Engine::SetupConfig() {
    EngineLogger::SetLoggingLevel(static_cast<EngineLogger::Level>(ConfigConstants::LOG_LEVEL));
    EngineLogger::SetSDLInternalLoggingLevel(static_cast<EngineLogger::Level>(ConfigConstants::INTERNAL_LOG_LEVEL));
    EngineLogger::Initialize();

    mInternalModules->engineConfig = ConfigHelper::GetInitialConfig();
    mInternalModules->appConfig = std::make_unique<EngineConfig>();
}

void Engine::SetupLogger() const {
    const auto& engineConfig = mInternalModules->engineConfig;

    auto level = engineConfig->Get("engine.logs.loglevel", 0);
    auto internalLevel = engineConfig->Get("engine.logs.internalLogLevel", 0);

    EngineLogger::SetLoggingLevel(static_cast<EngineLogger::Level>(level));
    EngineLogger::SetSDLInternalLoggingLevel(static_cast<EngineLogger::Level>(internalLevel));
}

void Engine::Shutdown() {
    EngineLogger::Shutdown();
}

