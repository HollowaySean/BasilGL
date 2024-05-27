#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "GLTexture.hpp"

namespace basil {

/** @brief Union of scalar types for OpenGL uniforms */
using GLUniformScalar = std::variant<bool, float, int, uint>;

/** @brief Union of scalar types for OpenGL uniforms */
using GLUniformPointer = std::variant<bool*, float*, int*, uint*>;

/** @brief Union of scalar types for OpenGL uniforms */
using GLUniformDirectPointer = std::variant<float*, int*, uint*>;

/** @brief Union of vector types for OpenGL uniforms */
using GLUniformVector = std::variant<
        std::vector<bool>,
        std::vector<float>,
        std::vector<int>,
        std::vector<uint>
    >;

// TODO(sholloway): Clean up documentation, there are
// both repeats and incorrect information
// TODO(sholloway): Clean up these concepts

/** @brief Union of vector types for OpenGL uniforms */
using GLUniformDirectVector = std::variant<
        std::vector<float>,
        std::vector<int>,
        std::vector<uint>
    >;

/** @brief Type which can be cast to GLUniformScalar */
template<typename T>
concept GLUniformScalarType =
    std::is_convertible_v<std::vector<T>, GLUniformVector>;

/** @brief Type which can be cast to GLUniformScalar */
template<typename T>
concept GLUniformDirectScalarType =
    std::is_convertible_v<std::vector<T>, GLUniformVector>
    && !std::is_base_of_v<bool, T>;

/** @brief Type which can be cast to GLUniformPointer */
template<typename T>
concept GLUniformPointerType =
    std::is_convertible_v<T, GLUniformPointer>
    && !std::is_base_of_v<bool*, T>;

/** @brief Type which can be cast to GLUniformPointer */
template<typename T>
concept GLUniformDirectPointerType =
    std::is_convertible_v<T, GLUniformPointer>;

/** @brief Type which can be cast to GLUniformVector */
template<typename T>
concept GLUniformVectorType =
    std::is_convertible_v<T, GLUniformVector>;

/** @brief Type which can be cast to GLUniformVector */
template<typename T>
concept GLUniformDirectVectorType =
    std::is_convertible_v<T, GLUniformVector>
    && !std::is_base_of_v<std::vector<bool>, T>;

/** @brief Type which can be cast to GLUniformScalar or GLUniformVector */
template<typename T>
concept GLUniformType =
    GLUniformScalarType<T> || GLUniformVectorType<T>;

/** @brief Struct to hold details of OpenGL uniform value */
struct GLUniform {
    /** @brief Create uniform vector or matrix
     *  @param  vectorValue Source of data to set
     *  @param  name        Name of uniform in shader
     *  @param  count       Number of values to set
     *  @param  length      Length of vector, or height of matrix
     *  @param  width       Width of matrix
     *  @tparam Numeric type of value
     */
    template<GLUniformDirectVectorType T>
    GLUniform(T vectorValue,
            const std::string& name,
            unsigned int count = 1,
            unsigned int length = 0,
            unsigned int width = 1) :
                value(vectorValue),
                name(name),
                count(count),
                length(length),
                width(width) {
        if (length == 0) {
            length = vectorValue.size();
        }
        data = std::visit([](auto& v){
            return GLUniformDirectPointer(v.data()); }, value);
    }

    GLUniform(std::vector<bool> vectorValue,
        const std::string& name,
        unsigned int count = 1,
        unsigned int length = 0,
        unsigned int width = 1) :
            name(name),
            count(count),
            length(length),
            width(width) {
        if (length == 0) {
            length = vectorValue.size();
        }
        value = std::vector<int>(vectorValue.begin(), vectorValue.end());
        data = std::visit([](auto& v){
            return GLUniformDirectPointer(v.data()); }, value);
    }

    /** @brief Create uniform vector or matrix from data pointer
     *  @param  dataPointer Source of data to set
     *  @param  name        Name of uniform in shader
     *  @param  count       Number of values to set
     *  @param  length      Length of vector, or height of matrix
     *  @param  width       Width of matrix
     *  @tparam Numeric type of value
     */
    template<GLUniformScalarType T>
    GLUniform(T* dataPointer,
            const std::string& name,
            unsigned int count = 1,
            unsigned int length = 1,
            unsigned int width = 1) :
                name(name),
                count(count),
                length(length),
                width(width),
                data(dataPointer) { }

    /** @brief Create uniform scalar
     *  @param  scalarValue Source of data to set
     *  @param  name        Name of uniform in shader
     *  @tparam Numeric type of value
     */
    template<GLUniformDirectScalarType T>
    GLUniform(T scalarValue, const std::string& name)
        : GLUniform(
            std::vector<T>({scalarValue}), name, 1, 1, 1) {}

    GLUniform(bool scalarValue, const std::string& name)
        : GLUniform(
            std::vector<int>({ scalarValue }), name, 1, 1, 1) {}

    /** @brief Create uniform from texture
     *  @param  texture     Pointer to IGLTexture object
     *  @param  name        Name of texture uniform in shader
     */
    GLUniform(std::shared_ptr<IGLTexture> texture,
        const std::string& name)
        : GLUniform(texture->getUniformLocation(), name) {}

    template<GLUniformDirectScalarType T>
    void setValue(T newValue) {
        this->value = std::vector<T>({newValue});
        this->length = 1;
        this->data = std::visit([](auto& v){
            return GLUniformDirectPointer(v.data()); }, value);
    }
    void setValue(bool newValue) {
        setValue(static_cast<int>(newValue));
    }

    /** @brief Stored uniform value */
    GLUniformDirectVector value;

    // TODO(sholloway): Make private
    GLUniformDirectPointer data = static_cast<int*>(nullptr);

    /** @brief Name used in OpenGL shader program */
    std::string name;

    /** @brief Length of vector, or height of matrix */
    unsigned int length;

    /** @brief Width of matrix */
    unsigned int width;

    /** @brief Number of values to set */
    unsigned int count;
};

}   // namespace basil
