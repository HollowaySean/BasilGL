#ifndef SRC_DATA_GLUNIFORM_HPP_
#define SRC_DATA_GLUNIFORM_HPP_

#include <string>
#include <variant>

namespace basil {

/** @brief Union type of inputs to setUniform in GLShaderProgram */
using GLUniformType = std::variant<bool, float, int, unsigned int>;

/** @brief Struct which contains value and name of OpenGL uniform */
struct GLUniform {
 public:
    GLUniform(GLUniformType value,
            const std::string& name,
            unsigned int ID)
        : value(value), name(name), uniformID(ID) {}

    GLUniformType value;
    std::string name;

    unsigned int getID() { return uniformID; }

 private:
    unsigned int uniformID;
};

}  // namespace basil

#endif  // SRC_DATA_GLUNIFORM_HPP_
