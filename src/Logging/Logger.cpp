#include "Logger.hpp"

void Logger::log(
        const std::string& message, Level level, std::ostream& ostream) {
    if (level >= logLevel) {
        std::string label = levelLabels[level];
        ostream << "[" << label << "]: " << message << std::endl;
    }
}

void Logger::lineBreak(
        Level level, std::ostream& ostream) {
    if (level >= logLevel) {
        ostream << std::endl;
    }
}
