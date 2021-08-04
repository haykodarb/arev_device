#include "RealTimeClock.h"
#include "SoftwareTimers.h"
#include "WifiHandler.h"

#define SDA_PIN 0U
#define SCL_PIN 2U

SensorData sensorData;
ConfigValues configValues;
RealTimeClock realTimeClock;
RelayHandler relayHandler;
SDHandler sdHandler;
WebServer webServer;

void setup(void)
{
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.begin(115200);

  WifiHandler_init(&sdHandler);

  realTimeClock.init();

  sensorData.init();

  relayHandler.init(&configValues, &sdHandler, &realTimeClock);

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