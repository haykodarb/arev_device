#include "SoftwareTimers.h"
#include "WebServer.h"

static os_timer_t longTimer;
static os_timer_t updateSensorsTimer;

volatile static bool longTimerFlag = false;
volatile static bool updateSensorsFlag = false;

static SensorData *sensorData;
static RealTimeClock *realTimeClock;
static SDHandler *sdHandler;
static RelayHandler *relayHandler;
static WebServer *webServer;
static ConfigValues *configValues;

static void setLongTimerFlag(void *pArg)
{
    longTimerFlag = true;
}

static void setUpdateSensorsFlag(void *pArg)
{
    updateSensorsFlag = true;
}

static void longTimerCallback(void)
{
    realTimeClock->update();
    sdHandler->writeToDay(relayHandler->isResistanceOn);
}

void SoftwareTimers_updateSensorsCallback(void)
{

    realTimeClock->update();

    uint8_t currentHour = realTimeClock->currentHour();

    sensorData->update();

    float currentTemp = sensorData->currentTemp;

    relayHandler->changeRelayState(currentHour, currentTemp);

    webServer->broadcast();
}

void SoftwareTimers_init(SensorData *_sensorData, ConfigValues *_configValues, SDHandler *_sdHandler, RelayHandler *_relayHandler, WebServer *_webServer)
{
    sensorData = _sensorData;
    sdHandler = _sdHandler;
    relayHandler = _relayHandler;
    webServer = _webServer;
    configValues = _configValues;

    os_timer_setfn(&longTimer, setLongTimerFlag, NULL);
    os_timer_setfn(&updateSensorsTimer, setUpdateSensorsFlag, NULL);
    os_timer_arm(&updateSensorsTimer, UPDATE_SENSORS_PERIOD, true);
    os_timer_arm(&longTimer, LONG_TIMER_PERIOD, true);

    longTimerFlag = true;
}

void SoftwareTimers_loop(void)
{
    if (configValues->configChangedFlag)
    {
        updateSensorsFlag = true;
        configValues->configChangedFlag = false;
    }
    if (updateSensorsFlag)
    {
        SoftwareTimers_updateSensorsCallback();
        updateSensorsFlag = false;
    }
    if (longTimerFlag)
    {
        longTimerCallback();
        longTimerFlag = false;
    }
}