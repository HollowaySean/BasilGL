#include "Logger.hpp"

namespace basil {

void Logger::log(const std::string& message, LogLevel level) {
    if (level >= logLevel) {
        std::string label = levelLabels[level];
        ostream << "[" << label << "]: " << message << std::endl;
    }

    lastOutputLevel = level;
}

void Logger::lineBreak(LogLevel level) {
    if (level >= logLevel) {
        ostream << std::endl;
    }

    lastOutputLevel = level;
}

}  // namespace basil
