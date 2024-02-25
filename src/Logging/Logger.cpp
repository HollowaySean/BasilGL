#include "Logger.hpp"

namespace basil {

void Logger::log(const std::string& message, LogLevel level) {
    if (level >= logLevel) {
        std::string label = levelLabels[level];
        ostream << "[" << label << "]: " << message << std::endl;

        #ifdef TEST_BUILD
        didOutput = true;
    } else {
        didOutput = false;
        #endif
    }

    #ifdef TEST_BUILD
    lastOutputLevel = level;
    lastMessage = message;
    #endif
}

void Logger::lineBreak(LogLevel level) {
    if (level >= logLevel) {
        ostream << std::endl;

        #ifdef TEST_BUILD
        didOutput = true;
    } else {
        didOutput = false;
        #endif
    }

    #ifdef TEST_BUILD
    lastOutputLevel = level;
    lastMessage = "\n";
    #endif
}

}  // namespace basil
