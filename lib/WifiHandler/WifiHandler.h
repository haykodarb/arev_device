    #ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include "SDHandler.h"
#include "RelayHandler.h"

#define AP_PASSWORD "75N2XLR2"

void WifiHandler_init(SDHandler *_sdHandler);
void WifiHandler_loop(void);

#endif