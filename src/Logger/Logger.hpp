#ifndef SRC_LOGGER_LOGGER_HPP_
#define SRC_LOGGER_LOGGER_HPP_

#include <iostream>
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
    /** @param message  Message to log.
     *  @param level    Severity level, defaults to "DEBUG".
     *  @param ostream  Output stream, defaults to std::cout.
    */
    void Log(std::string message,
        Level level = DEBUG,
        std::ostream& ostream = std::cout);

    /** @return Get minimum severity level to output to ostream. */
    Level getLevel() { return logLevel; }

    /** @brief Set minimum severity level to output to ostream. */
    void setLevel(Level level) { logLevel = level; }

    /** @return Instance of Singleton logger. */
    static Logger& get() {
        static Logger instance;
        return instance;
    }

 private:
    #ifdef DEBUG_BUILD
        Level logLevel = DEBUG;
    #else
        Level logLevel = INFO;
    #endif

    Logger() {}
    Logger(Logger const&);
    void operator=(Logger const&);
};

#endif  // SRC_LOGGER_LOGGER_HPP_
