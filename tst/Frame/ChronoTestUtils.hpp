#ifndef TST_FRAME_CHRONOTESTUTILS_HPP_
#define TST_FRAME_CHRONOTESTUTILS_HPP_

#include <chrono>

class TestClock {
 public:
    typedef std::chrono::seconds                duration;
    typedef duration::rep                       rep;
    typedef duration::period                    period;
    typedef std::chrono::time_point<TestClock>  time_point;
    static const bool is_steady =               false;

    static time_point now() noexcept {
        return time_point(duration(nextTimeStamp));
    }

    inline static int nextTimeStamp = 0.;
};

#endif  // TST_FRAME_CHRONOTESTUTILS_HPP_
