#ifndef SRC_DATA_SHADERUNIFORMMODEL_HPP_
#define SRC_DATA_SHADERUNIFORMMODEL_HPP_

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <Basil/Builder.hpp>

#include "IDataModel.hpp"
#include "GLUniform.hpp"
#include "GLTextureUniform.hpp"

namespace basil {

/** @brief Implementation of IDataModel to maintain
 *  uniforms for GLShaderProgram objects. */
class ShaderUniformModel : public IDataModel,
                           public IBuildable<ShaderUniformModel> {
 public:
    /** @brief Add uniform to model
     *  @param value        Value of uniform
     *  @param uniformName  Name to use in shader
     *  @returns            UID for string-less lookup */
    unsigned int addUniformValue(
        GLUniformType value,
        const std::string& uniformName);

    /** @brief Updates uniform in model
     *  @param uniformID    ID of uniform to set
     *  @param value        New value
     *  @returns            Whether uniform with ID was found. */
    bool setUniformValue(
        unsigned int uniformID,
        GLUniformType value);

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<GLUniform> getUniform(
        const std::string& uniformName) const;

    /** @brief Gets value of uniform with ID, if found */
    std::optional<GLUniform> getUniform(
        unsigned int uniformID) const;

    /** @returns Reference to map containing all uniforms in model */
    const std::map<unsigned int, GLUniform>& getUniforms()
        const { return uniforms; }

    /** @brief Add texture object to model */
    unsigned int addTexture(std::shared_ptr<IGLTexture> texture,
        const std::string& name);

    /** @brief Gets value of texture with identifier, if found */
    std::optional<GLTextureUniform> getTexture(
        const std::string& textureName) const;

    /** @brief Gets value of texture with ID, if found */
    std::optional<GLTextureUniform> getTexture(
        unsigned int textureID) const;

    /** @returns Reference to map containing all textures in model */
    const std::map<unsigned int, GLTextureUniform>& getTextures()
        const { return textures; }

    /** @brief Builder pattern for ShaderUniformModel */
    class Builder : public IBuilder<ShaderUniformModel> {
     public:
        /** @brief Adds uniform to model */
        Builder& withUniform(
            GLUniformType value, const std::string& name);

        /** @brief Adds texture to model */
        Builder& withTexture(
            std::shared_ptr<IGLTexture> texture, const std::string& name);
    };

 private:
    std::map<unsigned int, GLUniform> uniforms;
    std::map<unsigned int, GLTextureUniform> textures;
    static inline unsigned int nextID = 0;
};

}  // namespace basil

#endif  // SRC_DATA_SHADERUNIFORMMODEL_HPP_
