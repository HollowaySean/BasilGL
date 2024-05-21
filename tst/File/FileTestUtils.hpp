#pragma once

#include <filesystem>

class FileTestUtils {
 public:
    static std::filesystem::path setUpTempDir(std::filesystem::path fileName) {
        std::filesystem::path tmpDir
            = std::filesystem::path(TEST_DIR)
                / "../build/test-tmp/FileTestUtils/";
        auto fullPath = tmpDir / fileName;

        if (!std::filesystem::exists(tmpDir)) {
            std::filesystem::create_directory(tmpDir);
        } else {
            std::filesystem::remove(fullPath);
        }

        return fullPath;
    }
};
