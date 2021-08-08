#include "RealTimeClock.h"
#include "SoftwareTimers.h"
#include "WifiHandler.h"

SensorData sensorData;
ConfigValues configValues;
RealTimeClock realTimeClock;
RelayHandler relayHandler;
SDHandler sdHandler;
WebServer webServer;

void setup(void)
{

  Serial.begin(115200);

  relayHandler.init(&configValues, &sdHandler, &realTimeClock);

  WifiHandler_init(&sdHandler);

  realTimeClock.init();

  sensorData.init();

  sdHandler.init(&realTimeClock, &sensorData, &configValues);

  webServer.init(&configValues, &sdHandler, &sensorData, &realTimeClock, &relayHandler);

  SoftwareTimers_init(&sensorData, &configValues, &sdHandler, &relayHandler, &webServer);
}

void loop(void)
{
  webServer.loop();
  SoftwareTimers_loop();
  WifiHandler_loop();
}