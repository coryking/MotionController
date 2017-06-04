//
// Created by Cory King on 5/20/17.
//

#ifndef MOTIONCONTROLLER_INTERVALOMETER_H
#define MOTIONCONTROLLER_INTERVALOMETER_H

#include <RtcDateTime.h>
#include "rtc.h"

#define TIME_FOR_SHUTTER_TRIGGER 200


class IntervalometerSettings {
public:
    IntervalometerSettings(int totalFrames, long startPosition, long endPosition, long intervalMs, long shutterSpeedMs, long shutterTriggerDurationMs, RtcDateTime startTime, bool isAlarmEnabled)
            : totalFrames(totalFrames), endPosition(endPosition), startPosition(startPosition), intervalMs(intervalMs),
              shutterSpeedMs(shutterSpeedMs), shutterTriggerDurationMs(shutterTriggerDurationMs), startTime(startTime), _isAlarmEnabled(isAlarmEnabled) {
    }

    IntervalometerSettings() : totalFrames(5), endPosition(1), startPosition(0), intervalMs(3000),
                               shutterSpeedMs(1000), shutterTriggerDurationMs(TIME_FOR_SHUTTER_TRIGGER), _isAlarmEnabled(false){
        this->startTime = globalRtc.GetDateTime();
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

    const RtcDateTime &getStartTime() const {
        return startTime;
    }

    void setStartTime(const RtcDateTime &startTime) {
        IntervalometerSettings::startTime = startTime;
    }

    bool isAlarmEnabled() const {
        return _isAlarmEnabled;
    }

    void setAlarmEnabled(bool _isAlarmEnabled) {
        IntervalometerSettings::_isAlarmEnabled = _isAlarmEnabled;
    }

private:
    int totalFrames;

    long startPosition;
    long endPosition;

    long intervalMs;
    long shutterSpeedMs;
    long shutterTriggerDurationMs;

    bool _isAlarmEnabled = false;
    RtcDateTime startTime;
};


#endif //MOTIONCONTROLLER_INTERVALOMETER_H
