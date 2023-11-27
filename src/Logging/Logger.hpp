#ifndef SRC_LOGGING_LOGGER_HPP_
#define SRC_LOGGING_LOGGER_HPP_

#include <iostream>
#include <sstream>
#include <map>
#include <string>

/**
 * @brief Severity level for log message.
*/
enum Level { DEBUG, INFO, WARN, ERROR };

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
    void log(const std::string& message, Level level = DEBUG);

    /**
     * @brief           Logs line break to output stream
     * @param level     Severity level, defaults to "DEBUG".
    */
    void lineBreak(Level level = DEBUG);

    /** @return Get minimum severity level to output to ostream. */
    Level getLevel() { return logLevel; }

    /** @brief Set minimum severity level to output to ostream. */
    void setLevel(Level level) { logLevel = level; }

    /** @return Instance of Singleton logger. */
    static Logger& get() {
        static Logger instance;
        return instance;
    }

#ifdef TEST_BUILD
    std::string getLastOutput() {
        std::string output = stringStream.str();
        clearTestInfo();
        return output;
    }

    Level getLastLevel() {
        return lastOutputLevel;
    }

    void clearTestInfo() {
        stringStream.str("");
        stringStream.clear();
    }

 private:
    std::ostringstream stringStream;
    std::ostream& ostream = stringStream;
#else

 private:
    std::ostream& ostream = std::cout;
#endif

 private:
    #ifdef DEBUG_BUILD
        Level logLevel = DEBUG;
    #else
        Level logLevel = INFO;
    #endif

    Level lastOutputLevel;

    std::map<Level, std::string> levelLabels = {
        {Level::DEBUG,  "DEBUG"},
        {Level::INFO,   "INFO"},
        {Level::WARN,   "WARN"},
        {Level::ERROR,  "ERROR"}
    };

    Logger() {}
    Logger(Logger const&);
    void operator=(Logger const&);
};

#endif  // SRC_LOGGING_LOGGER_HPP_
