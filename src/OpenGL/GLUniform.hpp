#pragma once

#include <string>
#include <variant>
#include <vector>

namespace basil {

/** @brief Supported scalar types for OpenGL uniforms */
using GLUniformScalar = std::variant<bool, float, int, uint>;

/** @brief Supported vector types for OpenGL uniforms */
using GLUniformVector = std::variant<
        std::vector<bool>, std::vector<float>,
        std::vector<int>,  std::vector<uint>
    >;

/** @brief Union of supported types for OpenGL uniforms */
using GLUniformType = std::variant<
        bool, float, int, uint,
        std::vector<bool>, std::vector<float>,
        std::vector<int>,  std::vector<uint>
    >;

/** @brief Struct which contains value and name of OpenGL uniform */
struct GLUniform {
 public:
    /** @brief Create new struct with data for OpenGL uniform */
    GLUniform(
        GLUniformType value, const std::string& name, unsigned int ID)
            : value(value), name(name), uniformID(ID) {}

    /** @brief Data type which can be used in OpenGL uniform */
    GLUniformType value;

    /** @brief Name used in OpenGL shader program */
    std::string name;

    /** @returns UID of uniform, assigned by ShaderUniformModel */
    unsigned int getID() { return uniformID; }

 private:
    const unsigned int uniformID;
};

}   // namespace basil
