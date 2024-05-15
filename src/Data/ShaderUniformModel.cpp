#include "ShaderUniformModel.hpp"

namespace basil {

unsigned int ShaderUniformModel::addUniformValue(
        GLUniformType value, const std::string& uniformName) {
    if (uniformIDs.contains(uniformName)) {
        unsigned int ID = uniformIDs.at(uniformName);
        setUniformValue(value, ID);

        return ID;
    }

    uniforms.emplace(nextID, GLUniform { value, uniformName, nextID });
    uniformIDs.emplace(uniformName, nextID);

    return nextID++;
}

bool ShaderUniformModel::setUniformValue(
        GLUniformType value, unsigned int uniformID) {
    if (uniforms.contains(uniformID)) {
        uniforms.at(uniformID).value = value;
        return true;
    }

    return false;
}

std::optional<GLUniform>
ShaderUniformModel::getUniform(const std::string& uniformName) const {
    if (uniformIDs.contains(uniformName)) {
        unsigned int ID = uniformIDs.at(uniformName);
        return std::optional(uniforms.at(ID));
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
    if (textureIDs.contains(name)) {
        unsigned int ID = textureIDs.at(name);
        setTextureSource(texture, ID);

        return ID;
    }

    textures.emplace(nextID, GLTextureUniform { texture, name, nextID });
    textureIDs.emplace(name, nextID);

    return nextID++;
}

bool ShaderUniformModel::setTextureSource(
        std::shared_ptr<IGLTexture> texture, unsigned int ID) {
    if (textures.contains(ID)) {
        textures.at(ID).texture = texture;
        return true;
    }

    return false;
}

std::optional<GLTextureUniform>
ShaderUniformModel::getTexture(const std::string& textureName) const {
    if (textureIDs.contains(textureName)) {
        unsigned int ID = textureIDs.at(textureName);
        return std::optional(textures.at(ID));
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

ShaderUniformModel::Builder&
ShaderUniformModel::Builder::withUniform(
        GLUniformType value, const std::string& name) {
    this->impl->addUniformValue(value, name);
    return (*this);
}

ShaderUniformModel::Builder&
ShaderUniformModel::Builder::withTexture(
        std::shared_ptr<IGLTexture> texture, const std::string& name) {
    this->impl->addTexture(texture, name);
    return (*this);
}

}  // namespace basil
