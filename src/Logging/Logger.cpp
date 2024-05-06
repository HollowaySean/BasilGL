#include "Logger.hpp"

namespace basil {

void Logger::log(const std::string& message, LogLevel level) {
    if (level >= logLevel) {
        std::string label = levelLabels[level];
        ostream << label << message << std::endl;

        #ifdef TEST_BUILD
        recordTestInfo(level, message, true);
    } else {
        recordTestInfo(level, message, false);
        #endif
    }
}

void Logger::lineBreak(LogLevel level) {
    if (level >= logLevel) {
        ostream << std::endl;

        #ifdef TEST_BUILD
        recordTestInfo(level, "\n", true);
    } else {
        recordTestInfo(level, "\n", false);
        #endif
    }
}

#ifdef TEST_BUILD

std::string Logger::getLastOutput() {
    TestOutput lastOutput = getLastOutputForThread();
    return lastOutput.message;
}

LogLevel Logger::getLastLevel() {
    TestOutput lastOutput = getLastOutputForThread();
    return lastOutput.level;
}

bool Logger::didOutputLastMessage() {
    TestOutput lastOutput = getLastOutputForThread();
    return lastOutput.didOutput;
}

void Logger::clearTestInfo() {
    auto threadID = std::this_thread::get_id();
    if (lastOutputMap.contains(threadID)) {
        lastOutputMap.erase(threadID);
    }
}

Logger::TestOutput Logger::getLastOutputForThread() {
    std::thread::id threadID = std::this_thread::get_id();
    if (lastOutputMap.contains(threadID)) {
        return lastOutputMap[threadID];
    } else {
        return TestOutput();
    }
}

void Logger::recordTestInfo(
        LogLevel level, const std::string& message, bool didOutput) {
    auto threadID = std::this_thread::get_id();
    TestOutput output = TestOutput(level, message, didOutput);
    if (lastOutputMap.contains(threadID)) {
        lastOutputMap.erase(threadID);
    }
    lastOutputMap.emplace(threadID, output);
}

#endif  // TEST_BUILD

}  // namespace basil
