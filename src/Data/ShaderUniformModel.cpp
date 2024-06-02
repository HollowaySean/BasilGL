#include "ShaderUniformModel.hpp"

namespace basil {

unsigned int ShaderUniformModel::addUniform(const GLUniform& uniform) {
    const std::string& name = uniform.getName();
    if (uniformIDs.contains(name)) {
        unsigned int ID = uniformIDs.at(name);
        setUniform(uniform, ID);

        return ID;
    }

    uniforms.emplace(nextID, uniform);
    uniformIDs.emplace(name, nextID);

    return nextID++;
}

bool ShaderUniformModel::setUniform(
        const GLUniform& uniform, unsigned int uniformID) {
    if (uniforms.contains(uniformID)) {
        uniforms.at(uniformID) = uniform;
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
        std::shared_ptr<IGLTexture> texture,
        const std::string& name) {
    if (uniformIDs.contains(name)) {
        unsigned int ID = uniformIDs.at(name);
        setTextureSource(texture, ID);

        return ID;
    }

    auto uniform = GLUniformTexture(texture, name);
    uniforms.emplace(nextID, uniform);
    textures.emplace(nextID, texture);
    uniformIDs.emplace(name, nextID);

    return nextID++;
}

bool ShaderUniformModel::setTextureSource(
        std::shared_ptr<IGLTexture> texture,
        unsigned int uniformID) {
    if (uniforms.contains(uniformID)) {
        const std::string& name = uniforms.at(uniformID).getName();
        uniforms.at(uniformID) = GLUniformTexture(texture, name);
        textures.at(uniformID) = texture;

        return true;
    }

    return false;
}

std::optional<std::shared_ptr<IGLTexture>>
ShaderUniformModel::getTextureSource(unsigned int uniformID) const {
    if (textures.contains(uniformID)) {
        return std::optional(textures.at(uniformID));
    }

    return std::nullopt;
}

std::optional<std::shared_ptr<IGLTexture>>
ShaderUniformModel::getTextureSource(const std::string& name) const {
    if (uniformIDs.contains(name)) {
        unsigned int ID = uniformIDs.at(name);
        return std::optional(textures.at(ID));
    }

    return std::nullopt;
}

ShaderUniformModel::Builder&
ShaderUniformModel::Builder::withUniform(GLUniform uniform) {
    this->impl->addUniform(uniform);
    return (*this);
}

ShaderUniformModel::Builder&
ShaderUniformModel::Builder::withTexture(std::shared_ptr<IGLTexture> texture,
        const std::string& name) {
    this->impl->addTexture(texture, name);
    return (*this);
}

}  // namespace basil
