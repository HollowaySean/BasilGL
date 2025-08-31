#pragma once

#include <map>
#include <memory>

#include <Basil/Packages/Chrono.hpp>

#include "ProcessInstance.hpp"

namespace basil {

struct MetricsRecord {
 public:
    /** @brief Initialize record with all zeros. */
    explicit MetricsRecord(unsigned int frameID = 0)
        : frameID(frameID) {}

    /** @brief Frame number that this record represeents. */
    unsigned int frameID;

    /** @brief Time from start of frame to end of frame. */
    FrameClock::duration frameTime = FrameClock::duration::zero();

    /** @brief Time from start of frame to end of processes. */
    FrameClock::duration workTime = FrameClock::duration::zero();

    /** @brief Map of process times and their durations*/
    std::map<std::shared_ptr<ProcessInstance>,
        FrameClock::duration> processTimes;

    /** @return Current frame rate calculated from the period. */
    double getFrameRate();

    /** @return Frame rate if there were no waiting time. */
    double getUncappedFrameRate();

    MetricsRecord operator+(MetricsRecord addend);
    MetricsRecord operator-(MetricsRecord subtrahend);
    MetricsRecord operator/(int divisor);

    bool isEqual(const MetricsRecord& comparison);
};

}   // namespace basil
