#ifndef TST_PROCESS_CHRONOTESTUTILS_HPP_
#define TST_PROCESS_CHRONOTESTUTILS_HPP_

#include <chrono>
#include <unordered_map>
#include <thread>

class TestClock {
 public:
    typedef std::chrono::nanoseconds            duration;
    typedef duration::rep                       rep;
    typedef duration::period                    period;
    typedef std::chrono::time_point<TestClock>  time_point;
    static const bool is_steady =               true;

    static time_point now() noexcept {
        return time_point(duration(getNextTimeStamp()));
    }

    static void setNextTimeStamp(unsigned int timeStamp) {
        auto threadID = std::this_thread::get_id();
        if (nextTimeStamp.contains(threadID)) {
            nextTimeStamp.erase(threadID);
        }
        nextTimeStamp.emplace(threadID, timeStamp);
    }

    static void setNextTimeStampToNow() {
        unsigned int now = std::chrono::steady_clock::now()
            .time_since_epoch().count();
        setNextTimeStamp(now);
    }

    static unsigned int getNextTimeStamp() {
        auto threadID = std::this_thread::get_id();
        if (nextTimeStamp.contains(threadID)) {
            return nextTimeStamp[threadID];
        } else {
            return 0.;
        }
    }

 private:
    // Thread-safe test utilities
    inline static std::unordered_map<
        std::thread::id, unsigned int> nextTimeStamp;
};

#endif  // TST_PROCESS_CHRONOTESTUTILS_HPP_
