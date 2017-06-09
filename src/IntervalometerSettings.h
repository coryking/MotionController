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
              shutterSpeedMs(shutterSpeedMs), shutterTriggerDurationMs(shutterTriggerDurationMs), startTime(startTime), _isStartTimeSet(true) {
    }

    IntervalometerSettings() : totalFrames(5), endPosition(1), startPosition(0), intervalMs(3000),
                               shutterSpeedMs(1000), shutterTriggerDurationMs(TIME_FOR_SHUTTER_TRIGGER) {
    }

    int getTotalFrames() const {
        return totalFrames;
    }

    long ICACHE_FLASH_ATTR  getTotalSteps() const {
        return endPosition - startPosition;;
    }

    long ICACHE_FLASH_ATTR  getStartPosition() const {
        return startPosition;
    }

    long ICACHE_FLASH_ATTR  getEndPosition() const {
        return endPosition;
    }

    long ICACHE_FLASH_ATTR  getStepsPerFrame() {
        return getTotalSteps() / getTotalFrames();;
    }

    long ICACHE_FLASH_ATTR  getShutterTriggerDurationMs() const {
        return shutterTriggerDurationMs;
    }

    void ICACHE_FLASH_ATTR  setTotalFrames(int totalFrames) {
        IntervalometerSettings::totalFrames = totalFrames;
    }

    void ICACHE_FLASH_ATTR  setIntervalMs(long intervalMs) {
        IntervalometerSettings::intervalMs = intervalMs;
    }

    void ICACHE_FLASH_ATTR  setShutterSpeedMs(long shutterSpeedMs) {
        IntervalometerSettings::shutterSpeedMs = shutterSpeedMs;
    }

    void ICACHE_FLASH_ATTR  setStartPosition(long startPosition) {
        IntervalometerSettings::startPosition = startPosition;
    }

    void ICACHE_FLASH_ATTR  setEndPosition(long endPosition) {
        IntervalometerSettings::endPosition = endPosition;
    }

    /***
     * Get the relative number of steps for each frame.  Will be negative if you need to move backwards each frame...
     * @return
     */
    long ICACHE_FLASH_ATTR  getRelativeStepsPerFrame() {
        if(endPosition < startPosition) {
            return -getStepsPerFrame();
        } else {
            return getStepsPerFrame();
        }
    }

    long ICACHE_FLASH_ATTR  getIntervalMs() const {
        return intervalMs;
    }

    long ICACHE_FLASH_ATTR  getShutterSpeedMs() const {
        return shutterSpeedMs;
    }

    const RtcDateTime &getStartTime() const {
        return startTime;
    }

    void ICACHE_FLASH_ATTR  setStartTime(const RtcDateTime &startTime) {
        IntervalometerSettings::startTime = startTime;
        _isStartTimeSet=true;
    }

    bool isStartTimeSet() {
        return _isStartTimeSet;
    }

private:
    int totalFrames;

    long startPosition;
    long endPosition;

    long intervalMs;
    long shutterSpeedMs;
    long shutterTriggerDurationMs;

    RtcDateTime startTime;
    bool _isStartTimeSet = false;
};


#endif //MOTIONCONTROLLER_INTERVALOMETER_H
