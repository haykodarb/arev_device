#ifndef SD_HANDLER_H
#define SD_HANDLER_H

#include "SD.h"
#include "ArduinoJson.h"
#include "ProcessValues.h"
#include "RealTimeClock.h"

#define CS_PIN 15U

class SDHandler
{
public:
    String deviceName;
    String deviceID;
    void init(RealTimeClock *_realTimeClock, SensorData *_sensorData, ConfigValues *_configValues);
    void writeName(String newName);
    void readName(void);
    void readConfig(void);
    void writeConfig(String jsonInput);
    void writeToDay(uint8_t _isResistanceOn);
    void writeToMonth(uint8_t _isResistanceOn);
};

#endif