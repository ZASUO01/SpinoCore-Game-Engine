#pragma once
#include <memory>
#include "Utils/DeepPimpl.h"

class Engine {
public:
    static std::unique_ptr<Engine> Create();

    // Constructor pass key patern.
    class ConstructorKey { friend class Engine; ConstructorKey() = default; };
    explicit Engine(ConstructorKey);
    ~Engine();

    // block copy and movement
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    void Run();
private:
    [[nodiscard]] bool Initialize();
    void SetupConfig();
    void SetupLogger() const;

    void Shutdown();

    // internal modules' struct. Pimpl patern
    struct InternalModules;
    DeepPimpl<InternalModules> mInternalModules;
};
