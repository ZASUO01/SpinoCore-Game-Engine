#pragma once
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace FileSystem::Mount {
    class MountPoint;
}

namespace FileSystem::Core {
    class VirtualFileSystem {
    public:
        VirtualFileSystem() = default;
        ~VirtualFileSystem() = default;

        // prevent copy
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
