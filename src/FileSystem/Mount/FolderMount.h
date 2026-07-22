#pragma once
#include <filesystem>
#include "MountPoint.h"

namespace FileSystem::Mount {
    class FolderMount : public MountPoint {
    public:
        explicit FolderMount(std::filesystem::path rootPath, bool isReadOnly = true);

        [[nodiscard]] bool Initialize() override;
        [[nodiscard]] bool Exists(std::string_view localPath) const override;
        [[nodiscard]] std::optional<std::vector<uint8_t>> Read(std::string_view localPath) const override;
        [[nodiscard]] bool Write(std::string_view localPath, const std::vector<uint8_t> &data) override;
        [[nodiscard]] bool IsReadOnly() const override { return mIsReadOnly; }

    private:
        [[nodiscard]] std::optional<std::filesystem::path> ResolvePhysicalPath(std::string_view localPath) const;

        std::filesystem::path mRootPath;
        bool mIsReadOnly;
        bool mIsMounted;
    };
}
