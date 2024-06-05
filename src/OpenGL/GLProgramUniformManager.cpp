#include "GLProgramUniformManager.hpp"

#include <fmt/format.h>

namespace basil {

void GLProgramUniformManager::setUniform(
        std::shared_ptr<GLUniform> uniform) {
    cacheUniform(uniform);
    setUniformWithoutCache(uniform);
}

void GLProgramUniformManager::applyCachedUniforms() {
    for (auto uniform : uniformCache) {
        setUniformWithoutCache(uniform.second);
    }
}

void GLProgramUniformManager::setUniformWithoutCache(
        std::shared_ptr<GLUniform> uniform) {
    int location = getUniformLocation(uniform->getName());
    if (location == -1) return;

    setUniformAt(uniform, location);
}

int GLProgramUniformManager::getUniformLocation(
        const std::string& name) {
    int location = glGetUniformLocation(

        programID, name.c_str());
    if (location > -1) {
        errorHistory.erase(name);
        return location;
    }

    if (errorHistory.contains(name)) return -1;

    errorHistory.insert(name);
    logger.log(
        fmt::format(LOG_UNIFORM_FAILURE,
        programID, name),
        LogLevel::DEBUG);
    return -1;
}

template<>
void GLProgramUniformManager::setUniformVectorOrMatrix<float>(
        GLUniformSource<float> source,
        unsigned int width, unsigned int length,
        unsigned int count, int location) {
    unsigned int signature = 4*(width - 1) + length;
    float* data = source.data();
    switch (signature) {
        case 1:   // width: 1, length 1
            glProgramUniform1fv(
                programID, location, count, data);
            break;
        case 2:   // width: 1, length 2
            glProgramUniform2fv(
                programID, location, count, data);
            break;
        case 3:   // width: 1, length 3
            glProgramUniform3fv(
                programID, location, count, data);
            break;
        case 4:   // width: 1, length 4
            glProgramUniform4fv(
                programID, location, count, data);
            break;
        case 6:   // width: 2, length 2
            glProgramUniformMatrix2fv(
                programID, location, count, false, data);
            break;
        case 7:   // width: 2, length 3
            glProgramUniformMatrix2x3fv(
                programID, location, count, false, data);
            break;
        case 8:   // width: 2, length 4
            glProgramUniformMatrix2x4fv(
                programID, location, count, false, data);
            break;
        case 11:  // width: 3, length 3
            glProgramUniformMatrix3fv(
                programID, location, count, false, data);
            break;
        case 12:  // width: 3, length 4
            glProgramUniformMatrix3x4fv(
                programID, location, count, false, data);
            break;
        case 16:  // width: 4, length 4
            glProgramUniformMatrix4fv(
                programID, location, count, false, data);
            break;
    }
}

template<>
void GLProgramUniformManager::setUniformVectorOrMatrix<int>(
        GLUniformSource<int> source,
        unsigned int width, unsigned int length,
        unsigned int count, int location) {
    unsigned int signature = 4*(width - 1) + length;
    int* data = source.data();
    switch (signature) {
        case 1:
            glProgramUniform1iv(
                programID, location, count, data);
            break;
        case 2:
            glProgramUniform2iv(
                programID, location, count, data);
            break;
        case 3:
            glProgramUniform3iv(
                programID, location, count, data);
            break;
        case 4:
            glProgramUniform4iv(
                programID, location, count, data);
            break;
    }
}

template<>
void GLProgramUniformManager::setUniformVectorOrMatrix<unsigned int>(
        GLUniformSource<unsigned int> source,
        unsigned int width, unsigned int length,
        unsigned int count, int location) {
    unsigned int signature = 4*(width - 1) + length;
    unsigned int* data = source.data();
    switch (signature) {
        case 1:
            glProgramUniform1uiv(
                programID, location, count, data);
            break;
        case 2:
            glProgramUniform2uiv(
                programID, location, count, data);
            break;
        case 3:
            glProgramUniform3uiv(
                programID, location, count, data);
            break;
        case 4:
            glProgramUniform4uiv(
                programID, location, count, data);
            break;
    }
}

void GLProgramUniformManager::setUniformAt(
        std::shared_ptr<GLUniform> uniform, int location) {
    auto uniformSource = uniform->getSource();
    std::visit([&](auto source) {
            setUniformVectorOrMatrix(source,
                uniform->getWidth(), uniform->getLength(),
                uniform->getCount(), location); },
        uniformSource);
}

void GLProgramUniformManager::cacheUniform(std::shared_ptr<GLUniform> uniform) {
    std::string name = uniform->getName();
    if (uniformCache.contains(name)) {
        uniformCache.at(name) = uniform;
    } else {
        uniformCache.emplace(name, uniform);
    }
}

}  // namespace basil
