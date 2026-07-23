#pragma once
#include <filesystem>
#include "MountPoint.h"

namespace FileSystem::Mount {
    class FolderMount : public MountPoint {
    public:
        [[nodiscard]] static std::unique_ptr<FolderMount> Create(std::filesystem::path rootPath, bool isReadOnly = true);

        // Public constructor with private pass key
        class ConstructorKey { friend class FolderMount; ConstructorKey() = default; };
        FolderMount(ConstructorKey, std::filesystem::path rootPath, bool isReadOnly);
        ~FolderMount() override = default;

        // prevent copy
        FolderMount(const FolderMount&) = default;
        FolderMount(FolderMount&&) = default;

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
