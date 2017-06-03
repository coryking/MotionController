//
// Created by Cory King on 5/20/17.
//

#ifndef MOTIONCONTROLLER_INTERVALOMETER_H
#define MOTIONCONTROLLER_INTERVALOMETER_H

#define TIME_FOR_SHUTTER_TRIGGER 200


class IntervalometerSettings {
public:
    IntervalometerSettings(int totalFrames, long startPosition, long endPosition, long intervalMs, long shutterSpeedMs, long shutterTriggerDurationMs)
            : totalFrames(totalFrames), endPosition(endPosition), startPosition(startPosition), intervalMs(intervalMs),
              shutterSpeedMs(shutterSpeedMs), shutterTriggerDurationMs(shutterTriggerDurationMs) {
    }

    IntervalometerSettings() {
        shutterTriggerDurationMs = TIME_FOR_SHUTTER_TRIGGER;
    }

    int getTotalFrames() const {
        return totalFrames;
    }

    long getTotalSteps() const {
        return endPosition - startPosition;;
    }

    long getStartPosition() const {
        return startPosition;
    }

    long getEndPosition() const {
        return endPosition;
    }

    long getStepsPerFrame() {
        return getTotalSteps() / getTotalFrames();;
    }

    long getShutterTriggerDurationMs() const {
        return shutterTriggerDurationMs;
    }

    void setTotalFrames(int totalFrames) {
        IntervalometerSettings::totalFrames = totalFrames;
    }

    void setIntervalMs(long intervalMs) {
        IntervalometerSettings::intervalMs = intervalMs;
    }

    void setShutterSpeedMs(long shutterSpeedMs) {
        IntervalometerSettings::shutterSpeedMs = shutterSpeedMs;
    }

    void setStartPosition(long startPosition) {
        IntervalometerSettings::startPosition = startPosition;
    }

    void setEndPosition(long endPosition) {
        IntervalometerSettings::endPosition = endPosition;
    }

    /***
     * Get the relative number of steps for each frame.  Will be negative if you need to move backwards each frame...
     * @return
     */
    long getRelativeStepsPerFrame() {
        if(endPosition < startPosition) {
            return -getStepsPerFrame();
        } else {
            return getStepsPerFrame();
        }
    }

    long getIntervalMs() const {
        return intervalMs;
    }

    long getShutterSpeedMs() const {
        return shutterSpeedMs;
    }

private:
    int totalFrames;

    long startPosition;
    long endPosition;

    long intervalMs;
    long shutterSpeedMs;
    long shutterTriggerDurationMs;
};


#endif //MOTIONCONTROLLER_INTERVALOMETER_H
