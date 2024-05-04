#ifndef SRC_DATA_SHADERUNIFORMMODEL_HPP_
#define SRC_DATA_SHADERUNIFORMMODEL_HPP_

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "IDataModel.hpp"
#include "GLUniform.hpp"

namespace basil {

class ShaderUniformModel : public IDataModel {
 public:
    // TODO(sholloway): Add support for vectors
    // TODO(sholloway): Add support for textures

    // TODO(sholloway): Update documentation
    /** @brief Add uniform to model, with uniformName as identifier */
    unsigned int addUniformValue(
        GLUniformType value,
        const std::string& uniformName);

    /** @brief Add uniform to model, with uniformName as identifier */
    bool setUniformValue(
        unsigned int uniformID,
        GLUniformType value);

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<GLUniform> getUniform(
        const std::string& uniformName);

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<GLUniform> getUniform(
        unsigned int uniformID);

    /** @returns std::vector containing all uniforms in model */
    const std::map<unsigned int, GLUniform>& getUniforms()
        const { return uniforms; }

 private:
    std::map<unsigned int, GLUniform> uniforms;
    static inline unsigned int nextID = 0;
};

}  // namespace basil

#endif  // SRC_DATA_SHADERUNIFORMMODEL_HPP_
