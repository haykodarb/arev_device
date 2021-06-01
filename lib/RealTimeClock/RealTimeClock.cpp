#include "RealTimeClock.h"

static RTC_DS1307 rtc;
static DateTime now;

void RealTimeClock::init(void)
{
    rtc.begin();
}

void RealTimeClock::adjust(DateTime newDateTime)
{
    rtc.adjust(newDateTime);
}

void RealTimeClock::update(void)
{
    now = rtc.now();
}

uint8_t RealTimeClock::currentHour(void)
{
    return now.hour();
}

uint32_t RealTimeClock::timeInSeconds(void)
{
    uint32_t time = now.hour() * 3600 + now.minute() * 60 + now.second();
    return time;
}

String RealTimeClock::currentDate(void)
{
    char dayFormat[] = "YYYYMMDD";
    String currentDate = now.toString(dayFormat);
    return currentDate;
}

String RealTimeClock::currentMonth(void)
{
    char monthFormat[] = "YYYYMM";
    String currentDate = now.toString(monthFormat);
    return currentDate;
}

String RealTimeClock::dayNumber(void)
{
    uint8_t dayNumber = now.day();
    return String(dayNumber);
}

String RealTimeClock::webSocketTime(void)
{
    char webSocketFormat[] = "hh:mm";
    String webSocketTime = now.toString(webSocketFormat);
    return webSocketTime;
}