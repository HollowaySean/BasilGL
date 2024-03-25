#ifndef SRC_DATA_FILETEXTURESOURCE_HPP_
#define SRC_DATA_FILETEXTURESOURCE_HPP_

#include <filesystem>

#include "ITextureSource.hpp"

namespace basil {

class FileTextureSource : public ITextureSource2D {
 public:
    explicit FileTextureSource(std::filesystem::path filePath);
    const void* data() override;

 private:
    const unsigned char* imageData;
};

}  // namespace basil

#endif  // SRC_DATA_FILETEXTURESOURCE_HPP_
