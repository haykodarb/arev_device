#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#include "RTClib.h"

class RealTimeClock
{
public:
    uint8_t currentHour(void);
    void init(void);
    uint32_t timeInSeconds(void);
    String currentDate(void);
    String webSocketTime(void);
    String currentMonth(void);
    String dayNumber(void);
    void update(void);
    void adjust(DateTime newDateTime);

private:
};

#endif
