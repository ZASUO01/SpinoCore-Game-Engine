#include "VirtualFileSystem.h"
#include "PathResolver.h"
#include "FileSystem/Mount/MountPoint.h"

namespace FileSystem::Core {
    bool VirtualFileSystem::Mount(const std::string_view protocol, std::unique_ptr<Mount::MountPoint> mountPoint) {
        if (protocol.empty() || !mountPoint) {
            return false;
        }

        std::lock_guard lock(mMutex);
        mMounts[std::string(protocol)].push_back(std::move(mountPoint));
        return true;
    }

    bool VirtualFileSystem::Exists(const std::string_view fullVirtualPath) const {
        const auto parsed = PathResolver::Parse(fullVirtualPath);
        if (!parsed || !PathResolver::IsValidPath(parsed->path)) {
            return false;
        }

        std::lock_guard lock(mMutex);

        const auto it = mMounts.find(std::string(parsed->protocol));
        if (it == mMounts.end()) {
            return false;
        }

        for (auto mountIt = it->second.rbegin(); mountIt != it->second.rend(); ++mountIt) {
            if ((*mountIt)->Exists(parsed->path)) {
                return true;
            }
        }

        return false;
    }

    std::optional<std::vector<uint8_t>> VirtualFileSystem::Read(const std::string_view fullVirtualPath) const {
        const auto parsed = PathResolver::Parse(fullVirtualPath);
        if (!parsed || !PathResolver::IsValidPath(parsed->path)) {
            return std::nullopt;
        }

        std::lock_guard lock(mMutex);

        const auto it = mMounts.find(std::string(parsed->protocol));
        if (it == mMounts.end()) {
            return std::nullopt;
        }

        for (auto mountIt = it->second.rbegin(); mountIt != it->second.rend(); ++mountIt) {
            if ((*mountIt)->Exists(parsed->path)) {
                return (*mountIt)->Read(parsed->path);
            }
        }

        return std::nullopt;
    }

    bool VirtualFileSystem::Write(const std::string_view fullVirtualPath, const std::vector<uint8_t>& data) {
        const auto parsed = PathResolver::Parse(fullVirtualPath);
        if (!parsed || !PathResolver::IsValidPath(parsed->path)) {
            return false;
        }

        std::lock_guard lock(mMutex);

        const auto it = mMounts.find(std::string(parsed->protocol));
        if (it == mMounts.end()) {
            return false;
        }

        for (auto mountIt = it->second.rbegin(); mountIt != it->second.rend(); ++mountIt) {
            if (!(*mountIt)->IsReadOnly()) {
                return (*mountIt)->Write(parsed->path, data);
            }
        }

        return false;
    }
}
