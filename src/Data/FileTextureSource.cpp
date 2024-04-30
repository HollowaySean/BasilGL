#include "FileTextureSource.hpp"

#include <memory>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // NOLINT

namespace basil {

FileTextureSource::FileTextureSource(std::filesystem::path filePath) {
    if (!std::filesystem::exists(filePath)) {
        logger.log("File not found at path: "
            + filePath.string(), LogLevel::ERROR);
        return;
    }

    int width, height, channels;
    imageData = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

    setWidth(width);
    setHeight(height);

    format = TextureFormat::getTextureFormat<unsigned char, 4>();
}

const void* FileTextureSource::data() {
    return reinterpret_cast<const void*>(imageData);
}

FileTextureSource::Builder&
FileTextureSource::Builder::fromFilePath(std::filesystem::path filePath) {
    impl = std::make_unique<FileTextureSource>(filePath);
    return (*this);
}

}  // namespace basil
