#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#include "RTClib.h"

/* This pins are defined for the newer PCB version */
#define SDA_PIN 0U
#define SCL_PIN 2U

class RealTimeClock
{
public:
    uint8_t currentHour(void);
    void init(void);
    uint32_t timeInSeconds(void);
    String currentDate(void);
    String webSocketTime(void);
    String webServerTime(void);
    String currentMonth(void);
    String dayNumber(void);
    void update(void);
    void adjust(DateTime newDateTime);

private:
};

#endif
