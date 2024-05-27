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
    unsigned int addUniform(GLUniform uniform);

    /** @brief Updates uniform in model
     *  @param uniform      New GLUniform
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    bool setUniform(
        GLUniform uniform, unsigned int uniformID);

    /** @brief Updates uniform in model
     *  @param value        New value
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    template<GLUniformVectorType T>
    bool setUniformValue(
            T value, unsigned int uniformID) {
        if (uniforms.contains(uniformID)) {
            uniforms.at(uniformID).value = value;
            uniforms.at(uniformID).length = value.size();
            return true;
        }

        return false;
    }

    /** @brief Updates uniform in model
     *  @param value        New value
     *  @param uniformID    ID of uniform to set
     *  @returns            Whether uniform with ID was found */
    template<GLUniformScalarType T>
    bool setUniformValue(
            T value, unsigned int uniformID) {
        if (uniforms.contains(uniformID)) {
            auto setUniform = uniforms.at(uniformID);
            setUniform.setValue(value);
            return true;
        }

        return false;
    }

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
        Builder& withUniform(GLUniform uniform);

        /** @brief Adds texture to model */
        Builder& withTexture(std::shared_ptr<IGLTexture> texture,
            const std::string& name);
    };

 private:
    std::map<std::string, unsigned int> uniformIDs;
    std::map<unsigned int, GLUniform> uniforms;
    std::map<unsigned int, std::shared_ptr<IGLTexture>> textures;

    static inline unsigned int nextID = 0;
};

}   // namespace basil
