#ifndef SOFTWARE_TIMERS_H
#define SOFTWARE_TIMERS_H

#include <Arduino.h>
#include <os_type.h>
#include "SDHandler.h"
#include "RelayHandler.h"
#include "WebServer.h"

#define LONG_TIMER_PERIOD 300000U
#define UPDATE_SENSORS_PERIOD 2000U

void SoftwareTimers_init(SensorData *_sensorData, ConfigValues *_configValues, SDHandler *_sdHandler, RelayHandler *_relayHandler, WebServer *_webServer);

void SoftwareTimers_loop(void);

void SoftwareTimers_updateSensorsCallback(void);

#endif