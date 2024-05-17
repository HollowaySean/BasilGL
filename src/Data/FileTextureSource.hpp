#pragma once

#include <filesystem>

#include <Basil/Builder.hpp>
#include <Basil/Logging.hpp>

#include "ITextureSource.hpp"

namespace basil {

/** @brief ITextureSource implementation which loads image files as
 *  source for GLTexture using STB image library.
 */
class FileTextureSource : public ITextureSource2D,
                          public IBuildable<FileTextureSource> {
 public:
    /** @brief Default initializer. */
    FileTextureSource() = default;

    /** @brief Initializes from path to image file. */
    explicit FileTextureSource(std::filesystem::path filePath);

    /** @returns Pointer to const char* byte values, as const void*. */
    const void* data() override;

    class Builder: public IBuilder<FileTextureSource> {
     public:
         /** @brief Create texture source from image file. */
         Builder& fromFilePath(std::filesystem::path filePath);
    };

#ifndef TEST_BUILD

 private:
#endif
    const unsigned char* imageData;

    Logger& logger = Logger::get();
};

}   // namespace basil
