#ifndef RELAY_HANDLER_H
#define RELAY_HANDLER_H

class SDHandler;

#include "Arduino.h"
#include "RealTimeClock.h"
#include "SDHandler.h"

#define RELAY_PIN 2U
#define RELAY_ON HIGH
#define RELAY_OFF LOW

class RelayHandler
{
public:
    uint8_t isResistanceOn;
    void changeRelayState(uint8_t currentHour, float currentTemp);
    void init(ConfigValues *_configValues, SDHandler *_sdHandler, RealTimeClock *_realTimeClock);

private:
    void onStateChange(void);
    void turnOff(void);
    void turnOn(void);
};

#endif