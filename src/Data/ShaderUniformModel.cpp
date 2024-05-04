#include "ShaderUniformModel.hpp"

namespace basil {

unsigned int ShaderUniformModel::addUniformValue(
        GLUniformType value, const std::string& uniformName) {
    uniforms.emplace(nextID, GLUniform { value, uniformName, nextID });

    return nextID++;
}

bool ShaderUniformModel::setUniformValue(
        unsigned int uniformID, GLUniformType value) {
    if (uniforms.contains(uniformID)) {
        uniforms.at(uniformID).value = value;
        return true;
    }

    return false;
}

std::optional<GLUniform>
ShaderUniformModel::getUniform(const std::string& uniformName) const {
    auto result = std::find_if(
        uniforms.begin(), uniforms.end(),
        [uniformName](const auto& uniform) {
            return uniform.second.name == uniformName;
        });

    if (result != uniforms.end()) {
        return std::optional(result->second);
    }

    return std::nullopt;
}

std::optional<GLUniform>
ShaderUniformModel::getUniform(unsigned int uniformID) const {
    if (uniforms.contains(uniformID)) {
        return std::optional(uniforms.at(uniformID));
    }

    return std::nullopt;
}

unsigned int ShaderUniformModel::addTexture(
        std::shared_ptr<IGLTexture> texture, const std::string& name) {
    textures.emplace(nextID, GLTextureUniform { texture, name, nextID });

    return nextID++;
}

std::optional<GLTextureUniform>
ShaderUniformModel::getTexture(const std::string& textureName) const {
    auto result = std::find_if(
        textures.begin(), textures.end(),
        [textureName](const auto& texture) {
            return texture.second.name == textureName;
        });

    if (result != textures.end()) {
        return std::optional(result->second);
    }

    return std::nullopt;
}

std::optional<GLTextureUniform>
ShaderUniformModel::getTexture(unsigned int textureID) const {
    if (textures.contains(textureID)) {
        return std::optional(textures.at(textureID));
    }

    return std::nullopt;
}


}  // namespace basil
