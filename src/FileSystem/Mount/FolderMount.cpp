#include "FolderMount.h"
#include "FileSystem/IO/FileReader.h"
#include "FileSystem/IO/FileWriter.h"
#include "SpinoCore/Logs/EngineLogger.h"

namespace FileSystem::Mount {
    FolderMount::FolderMount(std::filesystem::path rootPath, const bool isReadOnly)
    :mRootPath(std::move(rootPath)), mIsReadOnly(isReadOnly), mIsMounted(false)
    {}

    bool FolderMount::Initialize() {
        if (!std::filesystem::exists(mRootPath)) {
            EngineLogger::Error("[FOLDER MOUNT] Failed to mount. The path {} doesn't exists.", mRootPath.string());
            return false;
        }

        if (!std::filesystem::is_directory(mRootPath)) {
            EngineLogger::Error("[FOLDER MOUNT] Failed to mount. The path {} is not a directory.", mRootPath.string());
            return false;
        }

        mRootPath = std::filesystem::weakly_canonical(mRootPath);
        mIsMounted = true;

        EngineLogger::Info("[FOLDER MOUNT] Successfully mounted at: {}", mRootPath.string());
        return true;
    }

    bool FolderMount::Exists(const std::string_view localPath) const {
        if (!mIsMounted) {
            return false;
        }

        const auto targetPath = ResolvePhysicalPath(localPath);

        if (!targetPath) {
            return false;
        }

        return std::filesystem::exists(*targetPath) && std::filesystem::is_regular_file(*targetPath);
    }

    std::optional<std::vector<uint8_t> > FolderMount::Read(const std::string_view localPath) const {
        if (!mIsMounted) {
            EngineLogger::Error("[FOLDER MOUNT] Attempted to read '{}' before mount was initialized.", localPath);
            return std::nullopt;
        }

        auto targetPath = ResolvePhysicalPath(localPath);
        if (!targetPath || !std::filesystem::exists(*targetPath)) {
            return std::nullopt;
        }

        IO::FileReader reader(targetPath->string());
        if (!reader.IsValid()) {
            EngineLogger::Error("[FOLDER MOUNT] Denied read access to existing file: {}", targetPath->string());
            return std::nullopt;
        }

        std::vector<uint8_t> data = reader.GetBinary();

        if (data.empty()) {
            EngineLogger::Warn("[FOLDER MOUNT] Read 0 bytes from file. {}", targetPath->string());
            return std::nullopt;
        }

        return data;
    }

    bool FolderMount::Write(std::string_view localPath, const std::vector<uint8_t> &data) {
        if (!mIsMounted) {
            EngineLogger::Error("[FOLDER MOUNT] Attempted to write '{}' before mount was initialized.", localPath);
            return false;
        }

        if (mIsReadOnly) {
            EngineLogger::Error("[FOLDER MOUNT] Attempted to write '{}' in a read-only mount.", localPath);
            return false;
        }

        auto targetPath = ResolvePhysicalPath(localPath);
        if (!targetPath) {
            return false;
        }

        IO::FileWriter writer(targetPath->string());
        if (!writer.IsValid()) {
            EngineLogger::Error("[FOLDER MOUNT] Failed to access file {} for writing", targetPath->string());
            return false;
        }

        return writer.WriteBinary(data.data(), data.size());
    }

    std::optional<std::filesystem::path> FolderMount::ResolvePhysicalPath(const std::string_view localPath) const {
        if (localPath.empty()) {
            return std::nullopt;
        }

        if (localPath.find("..") != std::string_view::npos) {
            EngineLogger::Warn("[FOLDER MOUNT] Path traversal is not allowed '{}'", localPath);
            return std::nullopt;
        }

        const std::filesystem::path relativeTarget(localPath);

        if (relativeTarget.is_absolute()) {
            EngineLogger::Warn("[FOLDER MOUNT] Absolute paths are not allowed: '{}'", localPath);
            return std::nullopt;
        }

        const std::filesystem::path fullPath = mRootPath / relativeTarget;

        return std::filesystem::weakly_canonical(fullPath);
    }
}
