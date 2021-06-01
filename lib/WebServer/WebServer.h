#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "ESP8266WebServer.h"

#include <WebSocketsServer.h>

#include <ESP8266mDNS.h>

#include "ArduinoJson.h"
#include "ProcessValues.h"
#include "SDHandler.h"
#include "RealTimeClock.h"
#include "RelayHandler.h"

class WebServer
{
public:
    void init(ConfigValues *_configValues, SDHandler *_sdHandler, SensorData *_sensorData, RealTimeClock *_realTimeClock, RelayHandler *_relayHandler);
    void loop(void);
    void broadcast(void);

private:
    ;
};

#endif