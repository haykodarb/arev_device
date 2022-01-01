#ifndef SD_HANDLER_H
#define SD_HANDLER_H

#include "SD.h"
#include "ArduinoJson.h"
#include "ProcessValues.h"
#include "RealTimeClock.h"

#define CS_PIN 15U
#define DEFAULT_POWER 1000U

class SDHandler
{
public:
    void init(RealTimeClock *_realTimeClock, SensorData *_sensorData, ConfigValues *_configValues);

    void writeName(String newName);
    void readName(void);
    String getName(void);

    void writePower(uint16_t newPowerValue);
    void readPower(void);
    uint16_t getPower(void);

    void readConfig(void);
    void writeConfig(String jsonInput);

    String getDeviceID(void);

    void writeToDay(uint8_t _isResistanceOn);
    void writeToMonth(uint8_t _isResistanceOn);

private:
    String _deviceName;
    String _deviceID;
    uint16_t _devicePower;
};

#endif