#include "RelayHandler.h"

static ConfigValues *configValues;
static SDHandler *sdHandler;
static RealTimeClock *realTimeClock;

void RelayHandler::init(ConfigValues *_configValues, SDHandler *_sdHandler, RealTimeClock *_realTimeClock)
{
    configValues = _configValues;
    sdHandler = _sdHandler;
    realTimeClock = _realTimeClock;

    digitalWrite(RELAY_PIN, RELAY_OFF);

    pinMode(RELAY_PIN, OUTPUT);

    isResistanceOn = 0;
}

void RelayHandler::turnOn(void)
{
    uint8_t newResistanceState = 1;
    if (newResistanceState != isResistanceOn)
    {
        onStateChange();
        isResistanceOn = 1;
        digitalWrite(RELAY_PIN, RELAY_ON);
    }
}

void RelayHandler::turnOff(void)
{
    uint8_t newResistanceState = 0;
    if (newResistanceState != isResistanceOn)
    {
        onStateChange();
        isResistanceOn = 0;
        digitalWrite(RELAY_PIN, RELAY_OFF);
    }
}

void RelayHandler::changeRelayState(uint8_t currentHour, float currentTemp)
{
    uint8_t systState = configValues->systState;
    uint8_t initTime = configValues->initTime;
    uint8_t finalTime = configValues->finalTime;
    uint8_t initTemp = configValues->initTemp;
    uint8_t finalTemp = configValues->finalTemp;

    if (systState == 1 && currentHour >= initTime && currentHour < finalTime)
    {
        if (currentTemp < initTemp)
        {
            turnOn();
        }
        if (currentTemp > finalTemp)
        {
            turnOff();
        }
    }
    else
    {
        turnOff();
    }
}

void RelayHandler::onStateChange(void)
{
    realTimeClock->update();
    sdHandler->writeToDay(isResistanceOn);
    sdHandler->writeToMonth(isResistanceOn);
}