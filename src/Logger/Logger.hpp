#ifndef SRC_LOGGER_LOGGER_HPP_
#define SRC_LOGGER_LOGGER_HPP_

#include <iostream>
#include <string>

// TODO(sholloway): Use preprocessor statement to set this
// TODO(sholloway): Documentation

enum Level { DEBUG, INFO, WARN, ERROR };

class Logger {
 public:
    void Log(std::string message,
        Level level = DEBUG,
        std::ostream& ostream = std::cout);

    Level getLevel() { return logLevel; }
    void setLevel(Level level) { logLevel = level; }

    // Singleton pattern implementation
    static Logger& get() {
        static Logger instance;
        return instance;
    }

 private:
    Level logLevel = INFO;

    Logger() {}
    Logger(Logger const&);
    void operator=(Logger const&);
};

#endif  // SRC_LOGGER_LOGGER_HPP_
