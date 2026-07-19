#include "SpinoCore/Engine.h"

std::unique_ptr<Engine> Engine::Create() {
    auto engine = std::make_unique<Engine>(ConstructorKey{});

    if (!engine->Initialize()) {
        return nullptr;
    }

    return engine;
}

Engine::Engine(ConstructorKey) {}

Engine::~Engine() {
    Shutdown();
}

void Engine::Run() {

}

bool Engine::Initialize() {
    return true;
}

void Engine::Shutdown() {

}

