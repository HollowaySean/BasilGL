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

// TODO(sholloway): Add support for vectors
// TODO(sholloway): Add support for textures

/** @brief Implementation of IDataModel to maintain
 *  uniforms for GLShaderProgram objects. */
class ShaderUniformModel : public IDataModel {
 public:
    /** @brief Add uniform to model
     *  @param value        Value of uniform
     *  @param uniformName  Name to use in shader
     *  @returns            UID for string-less lookup */
    unsigned int addUniformValue(
        GLUniformType value,
        const std::string& uniformName);

    /** @brief Updates uniform in model
     *  @param uniformID    ID of uniform to set
     *  @param value        New value
     *  @returns            Whether uniform with ID was found. */
    bool setUniformValue(
        unsigned int uniformID,
        GLUniformType value);

    /** @brief Gets value of uniform with identifier, if found */
    std::optional<GLUniform> getUniform(
        const std::string& uniformName);

    /** @brief Gets value of uniform with ID, if found */
    std::optional<GLUniform> getUniform(
        unsigned int uniformID);

    /** @returns Reference to map containing all uniforms in model */
    const std::map<unsigned int, GLUniform>& getUniforms()
        const { return uniforms; }

 private:
    std::map<unsigned int, GLUniform> uniforms;
    static inline unsigned int nextID = 0;
};

}  // namespace basil

#endif  // SRC_DATA_SHADERUNIFORMMODEL_HPP_
