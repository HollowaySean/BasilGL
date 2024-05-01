#ifndef SRC_DATA_DATAMODEL_HPP_
#define SRC_DATA_DATAMODEL_HPP_

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace basil {

class DataModel {
 public:
    /** @brief Union type of inputs to setUniform in GLShaderProgram */
    using GLUniformType = std::variant<bool, float, int, unsigned int>;

    /** @brief Struct which contains value and name of OpenGL uniform */
    struct GLUniform {
     public:
        GLUniform(GLUniformType value, const std::string& name)
            : value(value), name(name) {}

        GLUniformType value;
        std::string name;
    };

    /** @brief Add uniform to model, with uniformName as identifier */
    void setUniformValue(
        GLUniformType value, const std::string& uniformName);

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<GLUniformType> getUniformValue(
        const std::string& uniformName);

    /** @returns std::vector containing all uniforms in model */
    const std::vector<GLUniform>& getUniforms() const { return uniforms; }

 private:
    std::vector<GLUniform> uniforms = std::vector<GLUniform>();
};

}  // namespace basil

#endif  // SRC_DATA_DATAMODEL_HPP_
