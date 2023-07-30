#ifndef SRC_FRAME_FRAMEMETRICS_HPP_
#define SRC_FRAME_FRAMEMETRICS_HPP_

#include <list>
#include <map>
#include <memory>

#include "ITimerSource.hpp"

/** @brief Struct containing process and frame durations. */
struct FrameMetricsRecord {
 public:
    explicit FrameMetricsRecord(int newFrameID = 0) {
        frameID = newFrameID;
    }

    /** @param timerRecord Timestamp record.
     *  @return Calculated frame metric record. */
    static FrameMetricsRecord fromTimestamp(TimerRecord timerRecord);

    /** @brief Integer frame identifier. */
    int frameID;

    /** @brief Time in ms from frame start to frame end. */
    double frameTime = 0;

    /** @brief Time in ms from frame start to processes done. */
    double workTime = 0;

    /** @brief Time in ms from process start to process done.
     *  Indexed by processID.
    */
    std::map<int, double> processTime;

    /** @return Current frame rate in frames per second. */
    double getFrameRate();

    /** @return Estimated frame rate if not frame rate capped. */
    double getUncappedFrameRate();

    FrameMetricsRecord operator+(FrameMetricsRecord other);
    FrameMetricsRecord operator-(FrameMetricsRecord other);
    FrameMetricsRecord operator/(int divisor);
};

/** @brief Utility class to record frame rate/time metrics. */
class FrameMetrics {
 public:
    explicit FrameMetrics(int newBufferSize = 1);

    /** @param setBufferSize Number of frames to average over. */
    void setBufferSize(int setBufferSize);

    /** @return Number of frames to average over. */
    int getBufferSize();

    /** @return Number of frames currently in buffer. */
    int getBufferCount();

    /** @brief Add a new frame timing record to the buffer. */
    void pushTimerRecord(TimerRecord record);

    /** @return Return current averaged metrics. */
    FrameMetricsRecord getCurrentMetrics();

    /** @brief Clear currently buffered frame metrics. */
    void clearBuffer();

#ifndef TEST_BUILD

 private:
#endif
    int bufferSize;

    FrameMetricsRecord currentSum;

    std::list<FrameMetricsRecord> buffer;
};

#endif  // SRC_FRAME_FRAMEMETRICS_HPP_
