#ifndef PROCESS_VALUES_H
#define PROCESS_VALUES_H

#include "Arduino.h"

#include "DallasTemperature.h"

#define SENSOR_PIN 4U

class SensorData
{
public:
    float currentTemp = 0;
    void init(void);
    void update(void);
};

class ConfigValues
{

public:
    int initTemp;
    int finalTemp;
    int initTime;
    int finalTime;
    int systState;
    bool configChangedFlag = false;

    void update(int newInitTemp, int newFinalTemp, int newInitTime, int newFinalTime, int newSystState);
};

#endif