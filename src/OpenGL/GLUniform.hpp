#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "GLTexture.hpp"

namespace basil {

/** @brief Variant of data types which can be cast to OpenGL uniforms */
using GLUniformVariant = std::variant<int, unsigned int, float, bool>;

/** @brief Concept restricting uniform data types */
template<class T>
concept GLUniformType = std::is_convertible_v<T, GLUniformVariant>;

/** @brief Template class holding pointer to OpenGL uniform data */
template<GLUniformType T>
class GLUniformSource {
 public:
    /** @brief Initialize GLUniformSource with given data pointer */
    explicit GLUniformSource(T* dataSource) : dataSource(dataSource) {}

    /** @returns Pointer to underlying data */
    T* data() { return dataSource; }

 private:
    T* dataSource = nullptr;
};

/** @brief Variant of underlying OpenGL uniform data types */
using GLUniformSourceGeneric = std::variant<
    GLUniformSource<int>, GLUniformSource<unsigned int>, GLUniformSource<float>>;

/** @brief Concept restricting to underlying OpenGL uniform data types */
template<class T>
concept GLUniformSourceType =
    std::is_convertible_v<GLUniformSource<T>, GLUniformSourceGeneric>;

/** @brief Base interface for OpenGL uniform wrappers */
class GLUniform {
 public:
    /** @returns Name of uniform in shader code */
    std::string getName() const { return name;   }

    /** @returns Length of vector data, or first dimension of matrix data */
    virtual unsigned int getLength() const { return length; }

    /** @returns Second dimension of matrix data */
    virtual unsigned int getWidth()  const { return width;  }

    /** @returns Number of values contained in data */
    virtual unsigned int getCount()  const { return count;  }

    /** @returns Un-typed variant of source wrapper */
    GLUniformSourceGeneric getSource() { return source; }

    /** @returns Void pointer to underlying data source */
    void* getData() {
        return std::visit([](auto s){ return reinterpret_cast<void*>(s.data()); }, this->source);
    }

 protected:
    GLUniform(
        GLUniformSourceGeneric uniformSource,
        const std::string& uniformName,
        unsigned int length = 1,
        unsigned int width = 1,
        unsigned int count = 1)
        : source(uniformSource),
          name(uniformName),
          length(length),
          width(width),
          count(count) {}

    GLUniformSourceGeneric source;
    std::string name;

    unsigned int length;
    unsigned int width;
    unsigned int count;
};

/** @brief Implementation of GLUniform containing a data pointer */
template<GLUniformType T>
class GLUniformPointer : public GLUniform {
 public:
    /** @brief Constructs GLUniform wrapper from pointer
     *  @param pointer  Pointer to uniform values
     *  @param name     Uniform name in shader code
     *  @param length   Length of vector, or first matrix dimension
     *  @param width    Second matrix dimension
     *  @param count    Number of values contained in data */
    GLUniformPointer(
        T* pointer,
        const std::string& name,
        unsigned int length = 1,
        unsigned int width = 1,
        unsigned int count = 1)
        : GLUniform(GLUniformSource<T>(pointer), name, length, width, count) {}

    /** @brief Reassigns pointer in GLUniformPointer
     *  @param pointer  Pointer to uniform values
     *  @param base     Original GLUniform object */
    GLUniformPointer(T* pointer, const GLUniform& base)
        : GLUniformPointer(pointer,
            base.getName(), base.getLength(),
            base.getWidth(), base.getCount()) {}
};

/** @brief Template specialization for boolean GLUniformPointer */
template<>
class GLUniformPointer<bool> : public GLUniform {
 public:
    /** @brief Constructs GLUniform wrapper from pointer
     *  @param pointer  Pointer to uniform values
     *  @param name     Uniform name in shader code
     *  @param length   Length of vector, or first matrix dimension
     *  @param width    Second matrix dimension
     *  @param count    Number of values contained in data */
    GLUniformPointer(
        bool* pointer,
        const std::string& name,
        unsigned int length,
        unsigned int width,
        unsigned int count)
        : GLUniform(GLUniformSource<int>(
            reinterpret_cast<int*>(pointer)),
            name, length, width, count) {}

    /** @brief Reassigns pointer in GLUniformPointer
     *  @param pointer  Pointer to uniform values
     *  @param base     Original GLUniform object */
    GLUniformPointer(bool* pointer, const GLUniform& base)
        : GLUniformPointer(pointer,
            base.getName(), base.getLength(),
            base.getWidth(), base.getCount()) {}
};

/** @brief Implementation of GLUniform containing a scalar value */
template<GLUniformType T>
class GLUniformScalar : public GLUniform {
 public:
    /** @brief Constructs GLUniform wrapper from scalar value
     *  @param uniformValue Value of OpenGL uniform
     *  @param uniformName  Uniform name in shader code */
    GLUniformScalar(T uniformValue, const std::string& uniformName)
            : value(uniformValue),
            GLUniform(GLUniformSource<T>(nullptr), uniformName) {
        this->source = GLUniformSource<T>(&(this->value));
    }

    /** @brief Reassigns value in GLUniformScalar
     *  @param uniformValue Value of OpenGL uniform
     *  @param base         Original GLUniform object */
    GLUniformScalar(T uniformValue, const GLUniform& base)
        : GLUniformScalar(uniformValue, base.getName()) {}

 private:
    T value;
};

/** @brief Template specialization for boolean GLUniformScalar */
template<>
class GLUniformScalar<bool> : public GLUniform {
 public:
    /** @brief Constructs GLUniform wrapper from scalar value
     *  @param uniformValue Value of OpenGL uniform
     *  @param uniformName  Uniform name in shader code */
    GLUniformScalar(bool uniformValue, const std::string& uniformName)
        : value(static_cast<int>(uniformValue)),
          GLUniform(GLUniformSource<int>(nullptr), uniformName) {
        this->source = GLUniformSource<int>(&(this->value));
    }

    /** @brief Reassigns value in GLUniformScalar
     *  @param uniformValue Value of OpenGL uniform
     *  @param base         Original GLUniform object */
    GLUniformScalar(bool uniformValue, const GLUniform& base)
        : GLUniformScalar(uniformValue, base.getName()) {}

 private:
    int value;
};

/** @brief Implementation of GLUniform containing a std::vector */
template<GLUniformType T>
class GLUniformVector : public GLUniform {
 public:
    /** @brief Constructs GLUniform wrapper from pointer
     *  @param vector   Vector of uniform values
     *  @param name     Uniform name in shader code
     *  @param length   Length of vector, or first matrix dimension.
     *                  Defaults to vector size.
     *  @param width    Second matrix dimension
     *  @param count    Number of values contained in data.
     *                  Defaults to vector size / (length x width). */
    GLUniformVector(
            std::vector<T> vector,
            const std::string& name,
            unsigned int length = 0,
            unsigned int width = 1,
            unsigned int count = 0)
            : sourceVector(vector),
              GLUniform(GLUniformSource<T>(nullptr), name, length, width, count) {
        if (this->length == 0) {
            this->length = sourceVector.size();
        }

        this->source = GLUniformSource<T>(this->sourceVector.data());
    }

    /** @brief Reassigns value in GLUniformVector
     *  @param vector   Vector of uniform values
     *  @param base     Original GLUniform object */
    GLUniformVector(
        std::vector<T> vector,
        const GLUniform& base)
        : GLUniformVector(vector,
            base.getName(), base.getLength(), base.getWidth()) {}

    /** @returns Count if provided, otherwise vector size / (length x width) */
    unsigned int getCount() const override {
        if (count != 0) return count;
        return sourceVector.size() / (length * width);
    }

 private:
    std::vector<T> sourceVector;
};

/** @brief Template specialization of boolean GLUniformVector*/
template<>
class GLUniformVector<bool> : public GLUniform {
 public:
    /** @brief Constructs GLUniform wrapper from pointer
     *  @param vector   Vector of uniform values
     *  @param name     Uniform name in shader code
     *  @param length   Length of vector, or first matrix dimension.
     *                  Defaults to vector size.
     *  @param width    Second matrix dimension
     *  @param count    Number of values contained in data.
     *                  Defaults to vector size / (length x width). */
    GLUniformVector(
            std::vector<bool> vector,
            const std::string& name,
            unsigned int length = 0,
            unsigned int width = 1,
            unsigned int count = 0)
            : sourceVector(std::vector<int>(vector.begin(), vector.end())),
              GLUniform(GLUniformSource<int>(nullptr), name, length, width, count) {
        if (this->length == 0) {
            this->length = sourceVector.size();
        }

        this->source = GLUniformSource<int>(this->sourceVector.data());
    }

    /** @brief Reassigns value in GLUniformVector
     *  @param vector   Vector of uniform values
     *  @param base     Original GLUniform object */
    GLUniformVector(
        std::vector<bool> vector,
        const GLUniform& base)
        : GLUniformVector(vector,
            base.getName(), base.getLength(), base.getWidth()) {}

    /** @returns Count if provided, otherwise vector size / (length x width) */
    unsigned int getCount() const override {
        if (count != 0) return count;
        return sourceVector.size() / (length * width);
    }

 private:
    std::vector<int> sourceVector;
};

/** @brief Implementation of GLUniform containing OpenGL texture location */
class GLUniformTexture : public GLUniformScalar<int> {
 public:
    /** @brief Constructs GLUniform wrapper from texture
     *  @param texture  Pointer to IGLTexture wrapper object
     *  @param name     Name of uniform in shader */
    GLUniformTexture(
        std::shared_ptr<IGLTexture> texture,
        const std::string& name)
        : sourceTexture(texture),
          GLUniformScalar<int>(
            texture->getUniformLocation(), name) {}

    /** @brief Reassigns value in GLUniformTexture
     *  @param texture  Pointer to IGLTexture wrapper object
     *  @param base     Original GLUniform object */
    GLUniformTexture(
        std::shared_ptr<IGLTexture> texture,
        const GLUniform& base)
        : GLUniformTexture(texture, base.getName()) {}

    /** @returns Pointer to IGLTexture object */
    std::shared_ptr<IGLTexture> getSource() const { return sourceTexture; }

 private:
    std::shared_ptr<IGLTexture> sourceTexture;
};

}   // namespace basil
