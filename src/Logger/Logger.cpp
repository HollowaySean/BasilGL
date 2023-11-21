#include "Logger.hpp"

void Logger::Log(std::string message, Level level, std::ostream& ostream) {
    if (level >= logLevel) {
        ostream << message << std::endl;
    }
}
