#include "WifiHandler.h"

static WiFiManager wifiManager;
static SDHandler *sdHandler;
static RelayHandler *relayHandler;

void WifiHandler_loop(void)
{
    bool isConnected = WiFi.isConnected();
    if (!isConnected)
    {
        uint8_t isResistanceOn = relayHandler->isResistanceOn;
        sdHandler->writeToDay(isResistanceOn);
        sdHandler->writeToMonth(isResistanceOn);
        delay(10000);
        ESP.reset();
    }
}

void WifiHandler_init(SDHandler *_sdHandler)
{
    sdHandler = _sdHandler;

    WiFi.mode(WIFI_STA);
    wifiManager.setConfigPortalTimeout(150);

    char charArrayName[10];
    sdHandler->deviceID.toCharArray(charArrayName, 10);

    if (!wifiManager.autoConnect(charArrayName, AP_PASSWORD))
    {
        ESP.reset();
        delay(3000);
    }
}