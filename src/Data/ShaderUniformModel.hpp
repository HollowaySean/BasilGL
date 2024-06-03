#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Basil/Packages/Builder.hpp>

#include "OpenGL/GLUniform.hpp"

namespace basil {

/** @brief Data model used to maintain
 *  uniforms for GLShaderProgram objects. */
class ShaderUniformModel : public IBuildable<ShaderUniformModel> {
 public:
    /** @brief Add uniform to model
     *  @param uniform      GLUniform struct
     *  @returns            UID for string-less lookup */
    unsigned int addUniform(std::shared_ptr<GLUniform> uniform);

    /** @brief Updates uniform in model
     *  @param uniform      New GLUniform
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    bool setUniform(
        std::shared_ptr<GLUniform>, unsigned int uniformID);

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<std::shared_ptr<GLUniform>> getUniform(
        const std::string& uniformName) const;

    /** @brief Gets value of uniform with ID, if found */
    std::optional<std::shared_ptr<GLUniform>> getUniform(
        unsigned int uniformID) const;

    /** @returns Reference to map containing all uniforms in model */
    const std::map<unsigned int, std::shared_ptr<GLUniform>>& getUniforms()
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

    /** @brief Gets texture object with name, if found */
    std::optional<std::shared_ptr<IGLTexture>> getTextureSource(
        const std::string& textureName) const;

    /** @brief Gets texture object with ID, if found */
    std::optional<std::shared_ptr<IGLTexture>> getTextureSource(
        unsigned int textureID) const;

    /** @brief Returns map of IDs to textures */
    const std::map<unsigned int, std::shared_ptr<IGLTexture>>& getTextures()
        const { return textures; }

    /** @brief Builder pattern for ShaderUniformModel */
    class Builder : public IBuilder<ShaderUniformModel> {
     public:
        /** @brief Adds uniform to model */
        Builder& withUniform(std::shared_ptr<GLUniform> uniform);

        /** @brief Adds texture to model */
        Builder& withTexture(std::shared_ptr<IGLTexture> texture,
            const std::string& name);
    };

 private:
    std::map<std::string, unsigned int> uniformIDs;
    std::map<unsigned int, std::shared_ptr<GLUniform>> uniforms;
    std::map<unsigned int, std::shared_ptr<IGLTexture>> textures;

    static inline unsigned int nextID = 0;
};

}   // namespace basil
