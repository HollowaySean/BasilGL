#ifndef SRC_FRAME_TIMESOURCE_HPP_
#define SRC_FRAME_TIMESOURCE_HPP_

#include <chrono>

namespace basil {

/** @brief      Wrapper template for clock type from std::chrono library.
 *  @tparam T   Clock type, statically checked by std::chrono::is_clock_v
 *  @tparam D   Duration type, subtype of T
 *  @tparam P   Time point type, subtype of T
 */
template <
    typename T,
    typename D = typename T::duration,
    typename P = typename T::time_point
>
class TimeSource {
 public:
    TimeSource() {
        static_assert(std::chrono::is_clock_v<T>,
            "T must be a Clock type from std::chrono");
    }

    /** @returns Current time, as time point type P */
    P getTimestamp() {
        return T::now();
    }

    /** @brief Waits for a given duration */
    void waitForDuration(D duration) {
        std::this_thread::sleep_for(duration);
    }

    /** @brief Waits until a given time point */
    void waitUntilTime(P time) {
        std::this_thread::sleep_until(time);
    }
};

}  // namespace basil

#endif  // SRC_FRAME_TIMESOURCE_HPP_
