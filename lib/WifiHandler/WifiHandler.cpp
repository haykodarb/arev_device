#include "WifiHandler.h"

static WiFiConnect wc;
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

    char charArrayName[15];
    sdHandler->getDeviceID().toCharArray(charArrayName, 15);

    WiFi.persistent(true);

    wc.setDebug(true);

    /*
       AP_NONE = Continue executing code
       AP_LOOP = Trap in a continuous loop - Device is useless
       AP_RESET = Restart the chip
       AP_WAIT  = Trap in a continuous loop with captive portal until we have a working WiFi connection
    */

    if (!wc.autoConnect())
    {
        wc.startConfigurationPortal(AP_RESET, charArrayName, ""); // if not connected show the configuration portal
    }
}