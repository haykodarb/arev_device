#include "ProcessValues.h"

static OneWire oneWire(SENSOR_PIN);
static DallasTemperature sensors(&oneWire);

void SensorData::update(void)
{
    sensors.requestTemperatures();
    float newTemp = sensors.getTempCByIndex(0);

    Serial.println(newTemp);

    if (newTemp != DEVICE_DISCONNECTED_C && newTemp != 0.00)
    {
        currentTemp = newTemp;
    }
    /*

        Debería agregar un flag por si el dispositivo tira error de manera repetida.
    
    */
}

void SensorData::init(void)
{
    sensors.begin();
}

void ConfigValues::update(int newInitTemp, int newFinalTemp, int newInitTime, int newFinalTime, int newSystState)
{
    initTemp = newInitTemp;
    finalTemp = newFinalTemp;
    initTime = newInitTime;
    finalTime = newFinalTime;
    systState = newSystState;
    configChangedFlag = true;
}
