#include "ShaderUniformModel.hpp"

namespace basil {

void ShaderUniformModel::setUniformValue(
        GLUniformType value, const std::string& uniformName) {
    uniforms.emplace_back(value, uniformName);
}

std::optional<ShaderUniformModel::GLUniformType>
ShaderUniformModel::getUniformValue(const std::string& uniformName) {
    auto result = std::find_if(
        uniforms.begin(), uniforms.end(),
        [uniformName](const auto& uniform) {
            return uniform.name == uniformName;
        });

    if (result != uniforms.end()) {
        return std::optional(result->value);
    }

    return std::nullopt;
}

}  // namespace basil
