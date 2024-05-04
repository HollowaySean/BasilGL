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
ShaderUniformModel::getUniform(const std::string& uniformName) {
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
ShaderUniformModel::getUniform(unsigned int uniformID) {
    if (uniforms.contains(uniformID)) {
        return std::optional(uniforms.at(uniformID));
    }

    return std::nullopt;
}

}  // namespace basil
