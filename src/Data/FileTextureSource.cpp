#include "FileTextureSource.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // NOLINT

namespace basil {

FileTextureSource::FileTextureSource(std::filesystem::path filePath) {
    int width, height, channels;
    // TODO(sholloway): Figure out channels
    imageData = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

    setWidth(width);
    setHeight(height);
    format = TextureFormat::getTextureFormat<unsigned char, 4>();
}

const void* FileTextureSource::data() {
    return reinterpret_cast<const void*>(imageData);
}

}  // namespace basil
