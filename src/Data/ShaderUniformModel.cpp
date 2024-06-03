#include "ShaderUniformModel.hpp"

namespace basil {

unsigned int ShaderUniformModel::addUniform(std::shared_ptr<GLUniform> uniform) {
    const std::string& name = uniform->getName();
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
        std::shared_ptr<GLUniform> uniform, unsigned int uniformID) {
    if (uniforms.contains(uniformID)) {
        uniforms.at(uniformID) = uniform;
        return true;
    }

    return false;
}

// TODO(sholloway): Update value?

std::optional<std::shared_ptr<GLUniform>>
ShaderUniformModel::getUniform(const std::string& uniformName) const {
    if (uniformIDs.contains(uniformName)) {
        unsigned int ID = uniformIDs.at(uniformName);
        return std::optional(uniforms.at(ID));
    }

    return std::nullopt;
}

std::optional<std::shared_ptr<GLUniform>>
ShaderUniformModel::getUniform(unsigned int uniformID) const {
    if (uniforms.contains(uniformID)) {
        return std::optional(uniforms.at(uniformID));
    }

    return std::nullopt;
}

ShaderUniformModel::Builder&
ShaderUniformModel::Builder::withUniform(std::shared_ptr<GLUniform> uniform) {
    this->impl->addUniform(uniform);
    return (*this);
}

}  // namespace basil
