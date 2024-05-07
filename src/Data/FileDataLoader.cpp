#include "FileDataLoader.hpp"

namespace basil {

template<> const std::string_view
FileDataLoader::TypeMap<float>::key = "float";
template<> const std::string_view
FileDataLoader::TypeMap<int>::key = "int";
template<> const std::string_view
FileDataLoader::TypeMap<unsigned int>::key = "unsigned int";
template<> const std::string_view
FileDataLoader::TypeMap<bool>::key = "bool";

template<>
bool FileDataLoader::TypeMap<float>::isCorrectType(json json) {
    return json.is_number_float();
}
template<>
bool FileDataLoader::TypeMap<int>::isCorrectType(json json) {
    return json.is_number_integer();
}
template<>
bool FileDataLoader::TypeMap<unsigned int>::isCorrectType(json json) {
    return json.is_number_unsigned();
}
template<>
bool FileDataLoader::TypeMap<bool>::isCorrectType(json json) {
    return json.is_boolean();
}

std::optional<ShaderUniformModel>
FileDataLoader::modelFromJSON(std::filesystem::path filePath) {
    // Check for existence of file
    if (!std::filesystem::exists(filePath)) {
        logger.log(
            fmt::format(LOG_FILE_MISSING, filePath.c_str()),
            LogLevel::ERROR);
        return std::nullopt;
    }

    // Parse JSON from file
    json data;
    std::ifstream dataFile;
    dataFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        dataFile.open(filePath);
        data = json::parse(dataFile);

        logger.log(
            fmt::format(LOG_READ_SUCCESS),
            LogLevel::DEBUG);
    } catch(std::ifstream::failure& error) {
        logger.log(
            fmt::format(LOG_READ_FAILURE, filePath.c_str()),
            LogLevel::ERROR);
        logger.log(
            strerror(errno),
            LogLevel::ERROR);

        return std::nullopt;
    } catch(json::parse_error& error) {
        logger.log(
            fmt::format(LOG_PARSE_FAILURE, filePath.c_str(), error.byte),
            LogLevel::ERROR);
        logger.log(
            error.what(),
            LogLevel::ERROR);

        return std::nullopt;
    }

    // Check if uniform definition exists
    if (data.empty() || !data.contains("uniforms")) {
        logger.log(
            fmt::format(LOG_UNIFORMS_MISSING, filePath.c_str()),
            LogLevel::WARN);
        return std::nullopt;
    }
    logger.log("Reading shader uniforms from JSON file.", LogLevel::DEBUG);

    // Read data into model
    data = data.at("uniforms");
    auto model = std::make_shared<ShaderUniformModel>();

    model = addUniforms<float>(model, data);
    model = addUniforms<int>(model, data);
    model = addUniforms<unsigned int>(model, data);
    model = addUniforms<bool>(model, data);

    return std::optional(std::move(*model));
}

}  // namespace basil
