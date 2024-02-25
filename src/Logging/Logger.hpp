#ifndef SRC_LOGGING_LOGGER_HPP_
#define SRC_LOGGING_LOGGER_HPP_

#include <iostream>
#include <sstream>
#include <map>
#include <string>

namespace basil {

/**
 * @brief Severity level for log message.
*/
enum LogLevel { DEBUG, INFO, WARN, ERROR };

/**
 * @brief Global logger using Singleton pattern.
*/
class Logger {
 public:
    /**
     *  @brief          Logs message to output stream.
     *  @param message  Message to log.
     *  @param level    Severity level, defaults to "DEBUG".
    */
    void log(const std::string& message, LogLevel level = DEBUG);

    /**
     * @brief           Logs line break to output stream
     * @param level     Severity level, defaults to "DEBUG".
    */
    void lineBreak(LogLevel level = DEBUG);

    /** @return Get minimum severity level to output to ostream. */
    LogLevel getLevel() { return logLevel; }

    /** @brief Set minimum severity level to output to ostream. */
    void setLevel(LogLevel level) { logLevel = level; }

    /** @return Instance of Singleton logger. */
    static Logger& get() {
        static Logger instance;
        return instance;
    }

#ifdef TEST_BUILD
    std::string getLastOutput() {
        return lastMessage;
    }

    LogLevel getLastLevel() {
        return lastOutputLevel;
    }

    bool didOutputLastMessage() {
        return didOutput;
    }

    void clearTestInfo() {
        lastOutputLevel = DEBUG;
        lastMessage = "";
        didOutput = false;
    }

 private:
    std::ostringstream stringStream;
    std::ostream& ostream = stringStream;

    LogLevel lastOutputLevel = DEBUG;
    std::string lastMessage = "";
    bool didOutput = false;
#else

 private:
    std::ostream& ostream = std::cout;
#endif

 private:
    #ifdef DEBUG_BUILD
        LogLevel logLevel = DEBUG;
    #else
        LogLevel logLevel = INFO;
    #endif

    std::map<LogLevel, std::string> levelLabels = {
        {LogLevel::DEBUG,  "DEBUG"},
        {LogLevel::INFO,   "INFO"},
        {LogLevel::WARN,   "WARN"},
        {LogLevel::ERROR,  "ERROR"}
    };

    // Inaccessable methods to enforce Singleton
    Logger() {}
    Logger(Logger const&);
    void operator=(Logger const&);
};

}  // namespace basil

#endif  // SRC_LOGGING_LOGGER_HPP_
