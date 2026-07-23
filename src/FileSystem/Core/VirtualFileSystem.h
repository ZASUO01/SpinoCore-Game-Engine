#pragma once
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace FileSystem::Mount {
    class MountPoint;
}

namespace FileSystem::Core {
    class VirtualFileSystem {
    public:
        [[nodiscard]] static std::unique_ptr<VirtualFileSystem> Create();

        // Public constructor with private pass key
        class ConstructorKey{ friend class VirtualFileSystem; ConstructorKey(){}};
        explicit VirtualFileSystem(ConstructorKey);
        ~VirtualFileSystem();

        // Prevent copy
        VirtualFileSystem(const VirtualFileSystem&) = delete;
        VirtualFileSystem& operator=(const VirtualFileSystem&) = delete;

        [[nodiscard]] bool Mount(std::string_view protocol, std::unique_ptr<Mount::MountPoint> mountPoint);
        [[nodiscard]] bool Exists(std::string_view fullVirtualPath) const;
        [[nodiscard]] std::optional<std::vector<uint8_t>> Read(std::string_view fullVirtualPath) const;
        [[nodiscard]] bool Write(std::string_view fullVirtualPath, const std::vector<uint8_t>& data);

    private:
        mutable std::mutex mMutex;
        std::unordered_map<std::string, std::vector<std::unique_ptr<Mount::MountPoint>>> mMounts;
    };
}
