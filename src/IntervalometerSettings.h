//
// Created by Cory King on 5/20/17.
//

#ifndef MOTIONCONTROLLER_INTERVALOMETER_H
#define MOTIONCONTROLLER_INTERVALOMETER_H


class IntervalometerSettings {
public:
    IntervalometerSettings(int totalFrames, long startPosition, long endPosition, long intervalMs, long shutterSpeedMs, long shutterTriggerDurationMs)
            : totalFrames(totalFrames), endPosition(endPosition), startPosition(startPosition), intervalMs(intervalMs),
              shutterSpeedMs(shutterSpeedMs), shutterTriggerDurationMs(shutterTriggerDurationMs) {
        _totalSteps = endPosition - startPosition;
        _stepsPerFrame = getTotalSteps() / getTotalFrames();

    }

    int getTotalFrames() const {
        return totalFrames;
    }

    long getTotalSteps() const {
        return _totalSteps;
    }

    long getStartPosition() const {
        return startPosition;
    }

    long getEndPosition() const {
        return endPosition;
    }

    long getStepsPerFrame() {
        return _stepsPerFrame;
    }

    long getShutterTriggerDurationMs() const {
        return shutterTriggerDurationMs;
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

    long _totalSteps;
    long _stepsPerFrame;

    long intervalMs;
    long shutterSpeedMs;
    long shutterTriggerDurationMs;
};


#endif //MOTIONCONTROLLER_INTERVALOMETER_H
