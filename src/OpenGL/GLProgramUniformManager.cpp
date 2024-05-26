#include "GLProgramUniformManager.hpp"

#include <fmt/format.h>

namespace basil {

void GLProgramUniformManager::setUniform(
        const GLUniform& uniform) {
    cacheUniform(uniform);

    int location = getUniformLocation(uniform.name);
    if (location == -1) return;

    setUniformAt(uniform, location);
}

void GLProgramUniformManager::setTextureSource(
        std::shared_ptr<IGLTexture> texture,
        const GLUniform& uniform) {
    if (textureMap.contains(uniform.name)) {
        textureMap.at(uniform.name) = texture;
    } else {
        textureMap.emplace(uniform.name, texture);
    }
}

void GLProgramUniformManager::applyCachedUniforms() {
    for (auto uniform : uniformCache) {
        setUniform(uniform.second);
    }
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
void GLProgramUniformManager::setUniformVectorOrMatrix<std::vector<float>>(
        std::vector<float> value, unsigned int count, int location,
        unsigned int length, unsigned int width) {
    unsigned int signature = 4*(width - 1) + length;
    switch (signature) {
        case 1:   // width: 1, length 1
            glProgramUniform1fv(
                programID, location, count, value.data());
            break;
        case 2:   // width: 1, length 2
            glProgramUniform2fv(
                programID, location, count, value.data());
            break;
        case 3:   // width: 1, length 3
            glProgramUniform3fv(
                programID, location, count, value.data());
            break;
        case 4:   // width: 1, length 4
            glProgramUniform4fv(
                programID, location, count, value.data());
            break;
        case 6:   // width: 2, length 2
            glProgramUniformMatrix2fv(
                programID, location, count, false, value.data());
            break;
        case 7:   // width: 2, length 3
            glProgramUniformMatrix2x3fv(
                programID, location, count, false, value.data());
            break;
        case 8:   // width: 2, length 4
            glProgramUniformMatrix2x4fv(
                programID, location, count, false, value.data());
            break;
        case 11:  // width: 3, length 3
            glProgramUniformMatrix3fv(
                programID, location, count, false, value.data());
            break;
        case 12:  // width: 3, length 4
            glProgramUniformMatrix3x4fv(
                programID, location, count, false, value.data());
            break;
        case 16:  // width: 4, length 4
            glProgramUniformMatrix4fv(
                programID, location, count, false, value.data());
            break;
        default:
        case 5:   // width: 2, length 1
        case 9:   // width: 3, length 1
        case 10:  // width: 3, length 2
        case 13:  // width: 4, length 1
        case 14:  // width: 4, length 2
        case 15:  // width: 4, length 3
            break;
    }
}

template<>
void GLProgramUniformManager::setUniformVectorOrMatrix<std::vector<int>>(
        std::vector<int> value, unsigned int count, int location,
        unsigned int length, unsigned int width) {
    switch (length) {
        case 1:
            glProgramUniform1iv(
                programID, location, count, value.data());
            break;
        case 2:
            glProgramUniform2iv(
                programID, location, count, value.data());
            break;
        case 3:
            glProgramUniform3iv(
                programID, location, count, value.data());
            break;
        case 4:
            glProgramUniform4iv(
                programID, location, count, value.data());
            break;
        default:
            break;
    }
}

template<>
void GLProgramUniformManager::setUniformVectorOrMatrix
            <std::vector<unsigned int>>(
        std::vector<unsigned int> value, unsigned int count, int location,
        unsigned int length, unsigned int width) {
    switch (length) {
        case 1:
            glProgramUniform1uiv(
                programID, location, count, value.data());
            break;
        case 2:
            glProgramUniform2uiv(
                programID, location, count, value.data());
            break;
        case 3:
            glProgramUniform3uiv(
                programID, location, count, value.data());
            break;
        case 4:
            glProgramUniform4uiv(
                programID, location, count, value.data());
            break;
        default:
            break;
    }
}

template<>
void GLProgramUniformManager::setUniformVectorOrMatrix<std::vector<bool>>(
        std::vector<bool> value, unsigned int count, int location,
        unsigned int length, unsigned int width) {
    std::vector<int> intValue(value.begin(), value.end());
    switch (length) {
        case 1:
            glProgramUniform1iv(
                programID, location, count, intValue.data());
            break;
        case 2:
            glProgramUniform2iv(
                programID, location, count, intValue.data());
            break;
        case 3:
            glProgramUniform3iv(
                programID, location, count, intValue.data());
            break;
        case 4:
            glProgramUniform4iv(
                programID, location, count, intValue.data());
            break;
        default:
            break;
    }
}

void GLProgramUniformManager::setUniformAt(
        const GLUniform& uniform, int location) {
    std::visit([&](auto value) {
        setUniformVectorOrMatrix(value,
            uniform.count, location,
            uniform.length, uniform.width);
    }, uniform.value);
}

void GLProgramUniformManager::cacheUniform(const GLUniform& uniform) {
    if (uniformCache.contains(uniform.name)) {
        uniformCache.at(uniform.name) = uniform;
    } else {
        uniformCache.emplace(uniform.name, uniform);
    }
}

}  // namespace basil
