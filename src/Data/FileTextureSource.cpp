#include "FileTextureSource.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // NOLINT

namespace basil {

FileTextureSource::FileTextureSource(std::filesystem::path filePath) {
    int width, height, channels;
    imageData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    setWidth(width);
    setHeight(height);
    format = TextureFormat::getTextureFormat<unsigned char, 4>();
}

const void* FileTextureSource::data() {
    return imageData;
}

}  // namespace basil
