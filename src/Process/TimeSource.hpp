#pragma once

#include <chrono>
#include <thread>

namespace basil {

/** @brief              Wrapper template for clock type from std::chrono library.
 *  @tparam T           Clock type, statically checked by std::chrono::is_clock_v
 *  @tparam duration    Duration type, subtype of T
 *  @tparam rep         Representative type of duration
 *  @tparam period      Period of duration
 *  @tparam time_point  Time point type, subtype of T
 */
template <
    typename T,
    typename duration   = typename T::duration,
    typename rep        = typename duration::rep,
    typename period     = typename duration::period,
    typename time_point = typename T::time_point
>
class TimeSource {
 public:
    // Must follow chrono's strict typing
    static_assert(std::chrono::is_clock_v<T>,
            "T must be a Clock type from std::chrono");

    /** @returns Current time, as time point type P */
    static time_point getTimestamp() {
        return T::now();
    }

    /** @brief Waits for a given duration */
    static void waitForDuration(duration sleepDuration) {
        std::this_thread::sleep_for(sleepDuration);
    }

    /** @brief Waits until a given time point */
    static void waitUntilTime(time_point wakeTime) {
        std::this_thread::sleep_until(wakeTime);
    }

    /** @brief Calculates period for a given frequency */
    static duration frequencyToPeriod(double frequency) {
        if (frequency == 0.) return duration::zero();

        int nanoSecondsPerPeriod = static_cast<int>(1'000'000'000. / frequency);
        auto timeInNanoseconds = std::chrono::nanoseconds(nanoSecondsPerPeriod);

        return duration(timeInNanoseconds);
    }

    /** @brief Calculates frequency for a given period */
    static double periodToFrequency(duration timeSpan) {
        if (timeSpan == duration::zero()) return 0.;

        auto timeInNanoseconds = std::chrono::nanoseconds(timeSpan);
        double frequencyInHertz = 1'000'000'000. / timeInNanoseconds.count();

        return frequencyInHertz;
    }

 private:
    TimeSource() = delete;
};

}   // namespace basil
