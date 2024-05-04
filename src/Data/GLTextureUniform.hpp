#ifndef SRC_DATA_GLTEXTUREUNIFORM_HPP_
#define SRC_DATA_GLTEXTUREUNIFORM_HPP_

#include <memory>
#include <string>

namespace basil {

// Forward declaration
class IGLTexture;

/** @brief Struct which contains pointer and name of OpenGL texture */
struct GLTextureUniform {
 public:
    /** @brief Create new struct with data for OpenGL uniform */
    GLTextureUniform(
            std::shared_ptr<IGLTexture> texture,
            const std::string& name,
            unsigned int ID)
        : texture(texture), name(name), uniformID(ID) {}

    /** @brief Texture container object */
    std::shared_ptr<IGLTexture> texture;

    /** @brief Name used in OpenGL shader program */
    std::string name;

    /** @returns UID of uniform, assigned by ShaderUniformModel */
    unsigned int getID() { return uniformID; }

 private:
    const unsigned int uniformID;
};

}  // namespace basil

#endif  // SRC_DATA_GLTEXTUREUNIFORM_HPP_
