#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "SDHandler.h"
#include "RelayHandler.h"
#include "WiFiConnect.h"

#define AP_PASSWORD "WIFIPASS00"

void WifiHandler_init(SDHandler *_sdHandler);
void WifiHandler_loop(void);

#endif