#pragma once

#include <iostream>
#include <map>
#include <string>

#ifdef TEST_BUILD
#include <sstream>
#include <thread>
#include <unordered_map>
#endif  // TEST_BUILD

#include "Definitions.hpp"

namespace basil {

/**
 * @brief Severity level for log message.
*/
enum class LogLevel {
    DEBUG, INFO, WARN, ERROR
};

// TODO(sholloway): Make static instead of Singleton

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
    void log(const std::string& message, LogLevel level = LogLevel::DEBUG);

    /**
     * @brief           Logs line break to output stream
     * @param level     Severity level, defaults to "DEBUG".
    */
    void lineBreak(LogLevel level = LogLevel::DEBUG);

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

    struct TestOutput {
        TestOutput() : level(), message(""), didOutput(false) {}

        TestOutput(LogLevel level, std::string message, bool didOutput) :
            level(level), message(message), didOutput(didOutput) {}

        LogLevel level;
        std::string message;
        bool didOutput;
    };

    std::unordered_map<std::thread::id, TestOutput> lastOutputMap;

    std::string getLastOutput();
    LogLevel getLastLevel();
    bool didOutputLastMessage();
    void clearTestInfo();

 private:
    TestOutput getLastOutputForThread();
    void recordTestInfo(
        LogLevel level, const std::string& message, bool didOutput);

    std::ostringstream stringStream;
    std::ostream& ostream = stringStream;
#else

 private:
    std::ostream& ostream = std::cout;
#endif  // TEST_BUILD

 private:
    LogLevel logLevel = BASIL_DEFAULT_LOGGING_LEVEL;

    std::map<LogLevel, std::string> levelLabels = {
        {LogLevel::DEBUG,  "[DEBUG]: "},
        {LogLevel::INFO,   "[INFO]:  "},
        {LogLevel::WARN,   "[WARN]:  "},
        {LogLevel::ERROR,  "[ERROR]: "}
    };

    // Inaccessable methods to enforce Singleton
    Logger() {}
    Logger(Logger const&);
    void operator=(Logger const&);
};

}   // namespace basil
