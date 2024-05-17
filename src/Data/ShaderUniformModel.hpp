#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>

#include <Basil/Packages/Builder.hpp>

#include "OpenGL/GLUniform.hpp"
#include "OpenGL/GLTextureUniform.hpp"

namespace basil {

/** @brief Data model used to maintain
 *  uniforms for GLShaderProgram objects. */
class ShaderUniformModel : public IBuildable<ShaderUniformModel> {
 public:
    /** @brief Add uniform to model
     *  @param value        Value of uniform
     *  @param uniformName  Name to use in shader
     *  @returns            UID for string-less lookup */
    unsigned int addUniformValue(
        GLUniformType value,
        const std::string& uniformName);

    /** @brief Updates uniform in model
     *  @param value        New value
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    bool setUniformValue(
        GLUniformType value,
        unsigned int uniformID);

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<GLUniform> getUniform(
        const std::string& uniformName) const;

    /** @brief Gets value of uniform with ID, if found */
    std::optional<GLUniform> getUniform(
        unsigned int uniformID) const;

    /** @returns Reference to map containing all uniforms in model */
    const std::map<unsigned int, GLUniform>& getUniforms()
        const { return uniforms; }

    /** @brief Add texture object to model
     *  @param texture      IGLTexture pointer
     *  @param name         Name to use in shader
     *  @returns            UID for string-less lookup */
    unsigned int addTexture(std::shared_ptr<IGLTexture> texture,
        const std::string& name);

    /** @brief Updates texture in model
     *  @param texture      IGLTexture pointer
     *  @param textureID    ID of texture to set
     *  @returns            Whether texture with ID was found */
    bool setTextureSource(std::shared_ptr<IGLTexture> texture,
        unsigned int textureID);

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
    std::map<std::string, unsigned int> uniformIDs;
    std::map<unsigned int, GLUniform> uniforms;

    std::map<std::string, unsigned int> textureIDs;
    std::map<unsigned int, GLTextureUniform> textures;
    static inline unsigned int nextID = 0;
};

}   // namespace basil
