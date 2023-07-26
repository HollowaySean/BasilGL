#ifndef SRC_FRAME_FRAMEMETRICS_HPP_
#define SRC_FRAME_FRAMEMETRICS_HPP_

#include <list>
#include <string>
#include <utility>

/** @brief Utility class to record frame rate/time metrics. */
class FrameMetrics {
 public:
    explicit FrameMetrics(int setWindowSize = 1);

    /** @brief Type abbreviation for value-name pair. */
    using TimeReport = std::pair<float, std::string>;

    /** @brief Type for returning list of time stamps with names. */
    using ReportList = std::list<TimeReport>;

    /** @brief Adds new time stamps to calculate metrics.
     *  @param start Start-of-loop timestamp.
     *  @param stop  End-of-loop timestamp.
     *  @param wait  Time spent waiting for loop.
     *  @param doneTimeReports List of names and done times for runnables.
     */
    void updateTimeStamps(
        float start, float stop, float wait,
        ReportList doneTimeReports = ReportList());

    /** @param windowSize Number of frames to average over for metrics. */
    void setSmoothingWindow(int newWindowSize);

    /** @return Average frame time in milliseconds. */
    float getFrameTimeMS();

    /** @return Average frame rate in Hertz. */
    float getFrameRate();

    /** @return Average time in loop spent running runnables.
     *      Equal to getFrameTimeMS if frame rate is uncapped. */
    float getWorkTimeMS();

    /** @return Average frame rate with wait time discarded.
     *      Equal to getFrameRate if frame rate is uncapped. */
    float getUncappedFrameRate();

    /** @return List of runtimes in milliseconds
     *      and names of runnables. */
    ReportList getRunTimesMS();

    /** @brief Container struct for metrics outputs. */
    struct MetricsReport {
     public:
        float frameTimeMS;
        float workTimeMS;
        float frameRate;
        float uncappedFrameRate;
        ReportList runTimesMS;
        int windowSize;
    };

    /** @return Struct containing all metrics. */
    MetricsReport getMetricsReport();

    /** @brief Clears all metrics. */
    void clear();

 private:
    struct FrameTimeStamps {
     public:
        float start;
        float stop;
        float wait;

        ReportList doneTimes;
        ReportList runTimes;

        float work() {
            return stop - start;
        }

        float frame() {
            return wait + work();
        }
    };
    std::list<FrameMetrics::FrameTimeStamps> timestamps;
    int windowSize = 1;
    float sumFrameTime = 0;
    float sumWorkTime = 0;
    ReportList sumRunTimes;
};

#endif  // SRC_FRAME_FRAMEMETRICS_HPP_
