#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "GLTexture.hpp"

// TODO(sholloway): Documentation
namespace basil {

using GLUniformVariant = std::variant<int, unsigned int, float, bool>;

template<class T>
concept GLUniformType = std::is_convertible_v<T, GLUniformVariant>;

template<GLUniformType T>
class GLUniformSource {
 public:
    explicit GLUniformSource(T* dataSource) : dataSource(dataSource) {}

    T* data() { return dataSource; }

 private:
    T* dataSource = nullptr;
};

using GLUniformSourceGeneric = std::variant<
    GLUniformSource<int>, GLUniformSource<unsigned int>, GLUniformSource<float>>;

template<class T>
concept GLUniformSourceType =
    std::is_convertible_v<GLUniformSource<T>, GLUniformSourceGeneric>;

class GLUniform {
 public:
    std::string getName() const        { return name;   }

    virtual unsigned int getLength() const  { return length; }
    virtual unsigned int getWidth()  const  { return width;  }
    virtual unsigned int getCount()  const  { return count;  }

    GLUniformSourceGeneric getSource() { return source; }

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

template<GLUniformType T>
class GLUniformPointer : public GLUniform {
 public:
    GLUniformPointer(
        T* pointer,
        const std::string& name,
        unsigned int length = 1,
        unsigned int width = 1,
        unsigned int count = 1)
        : GLUniform(GLUniformSource<T>(pointer), name, length, width, count) {}

    GLUniformPointer(T* pointer, const GLUniform& base)
        : GLUniformPointer(pointer,
            base.getName(), base.getLength(),
            base.getWidth(), base.getCount()) {}
};

template<>
class GLUniformPointer<bool> : public GLUniform {
 public:
    GLUniformPointer(
        bool* pointer,
        const std::string& name,
        unsigned int length,
        unsigned int width,
        unsigned int count)
        : GLUniform(GLUniformSource<int>(
            reinterpret_cast<int*>(pointer)),
            name, length, width, count) {}

    GLUniformPointer(bool* pointer, const GLUniform& base)
        : GLUniformPointer(pointer,
            base.getName(), base.getLength(),
            base.getWidth(), base.getCount()) {}
};

template<GLUniformType T>
class GLUniformScalar : public GLUniform {
 public:
    GLUniformScalar(T uniformValue, const std::string& uniformName)
            : value(uniformValue),
            GLUniform(GLUniformSource<T>(nullptr), uniformName) {
        this->source = GLUniformSource<T>(&(this->value));
    }

    GLUniformScalar(T uniformValue, const GLUniform& base)
        : GLUniformScalar(uniformValue, base.getName()) {}

 private:
    T value;
};

template<>
class GLUniformScalar<bool> : public GLUniform {
 public:
    GLUniformScalar(bool uniformValue, const std::string& uniformName)
        : value(static_cast<int>(uniformValue)),
          GLUniform(GLUniformSource<int>(nullptr), uniformName) {
        this->source = GLUniformSource<int>(&(this->value));
    }

    GLUniformScalar(bool uniformValue, const GLUniform& base)
        : GLUniformScalar(uniformValue, base.getName()) {}

 private:
    int value;
};

template<GLUniformType T>
class GLUniformVector : public GLUniform {
 public:
    GLUniformVector(
            std::vector<T> vector,
            const std::string& name,
            unsigned int length = 0,
            unsigned int width = 1,
            unsigned int count = 0)
            : sourceVector(vector),
              GLUniform(GLUniformSource<T>(nullptr),
                name, length, width, count) {
        if (this->length == 0) {
            this->length = sourceVector.size();
        }

        this->source = GLUniformSource<T>(this->sourceVector.data());
    }

    GLUniformVector(
        std::vector<T> vector,
        const GLUniform& base)
        : GLUniformVector(vector,
            base.getName(), base.getLength(), base.getWidth()) {}

    unsigned int getCount() const override {
        if (count != 0) return count;
        return sourceVector.size() / (length * width);
    }

 private:
    std::vector<T> sourceVector;
};

template<>
class GLUniformVector<bool> : public GLUniform {
 public:
    GLUniformVector(
            std::vector<bool> vector,
            const std::string& name,
            unsigned int length = 0,
            unsigned int width = 1,
            unsigned int count = 0)
            : sourceVector(std::vector<int>(vector.begin(), vector.end())),
              GLUniform(GLUniformSource<int>(nullptr),
                name, length, width, count) {
        if (this->length == 0) {
            this->length = sourceVector.size();
        }

        this->source = GLUniformSource<int>(this->sourceVector.data());
    }

    GLUniformVector(
        std::vector<bool> vector,
        const GLUniform& base)
        : GLUniformVector(vector,
            base.getName(), base.getLength(), base.getWidth()) {}

    unsigned int getCount() const override {
        if (count != 0) return count;
        return sourceVector.size() / (length * width);
    }

 private:
    std::vector<int> sourceVector;
};

class GLUniformTexture : public GLUniformScalar<int> {
 public:
    GLUniformTexture(
        std::shared_ptr<IGLTexture> texture,
        const std::string& name)
        : sourceTexture(texture),
          GLUniformScalar<int>(
            texture->getUniformLocation(), name) {}

    GLUniformTexture(
        std::shared_ptr<IGLTexture> texture,
        const GLUniform& base)
        : GLUniformTexture(texture, base.getName()) {}

    std::shared_ptr<IGLTexture> getSource() const { return sourceTexture; }

 private:
    std::shared_ptr<IGLTexture> sourceTexture;
};

}   // namespace basil
