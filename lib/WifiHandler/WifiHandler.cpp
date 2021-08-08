#include "WifiHandler.h"

static WiFiManager wifiManager;
static SDHandler *sdHandler;
static RelayHandler *relayHandler;

static ESP8266WebServer server(80);

static char apName[10];
static bool connect = false;
static String clientSSID = "";
static String clientPass = "";

static int8_t connectWifi()
{

    if ((WiFi.status() == WL_CONNECTED) && (WiFi.SSID() == clientSSID))
    {
        return WL_CONNECTED;
    }

    if (clientSSID != "")
    {
        ETS_UART_INTR_DISABLE();
        wifi_station_disconnect();
        ETS_UART_INTR_ENABLE();

        WiFi.begin(clientSSID.c_str(), clientPass.c_str(), 0, NULL, true);
    }
    else
    {
        // Using saved credentials
        if (WiFi.SSID() != "")
        {
            ETS_UART_INTR_DISABLE();
            wifi_station_disconnect();
            ETS_UART_INTR_ENABLE();

            WiFi.begin();
        }
    }

    int8_t connectionResult = WiFi.waitForConnectResult();

    return connectionResult;
}

static void handleWifiSave()
{
    Serial.println("PROCESS: Reaching WiFi save");

    clientSSID = server.arg("s").c_str();
    clientPass = server.arg("p").c_str();

    server.send(200, "text/plain", "Parameters saved");

    connect = true; //signal ready to connect/reset
}

static void handleRoot()
{
    Serial.println("Reaching root");

    server.send(200, "text/plain", "Hola en Root");
}

static void handleNotFound()
{
    Serial.println("404 NOT FOUND");

    server.send(404, "text/plain", "NOT FOUND");
}

static void handleCheckConnection()
{
    bool isConnected = WiFi.isConnected();

    if (isConnected)
    {
        server.send(200, "text/plain", "CONNECTED");
    }
    else
    {
        server.send(200, "text/plain", "NOT CONNECTED");
    }
}

static void webServerPortal()
{
    // uint32_t configPortalStart = millis();

    WiFi.softAP("ESP8266", AP_PASSWORD); //password option

    Serial.println("PROCESS: Running on port " + WiFi.softAPIP().toString());
    delay(500); // Without delay I've seen the IP address blank

    server.on("/", handleRoot);
    server.on("/wifisave", handleWifiSave);
    server.on("/connected", handleCheckConnection);
    server.onNotFound(handleNotFound);
    server.begin();
}

static bool startConfigPortal(void)
{

    if (!WiFi.isConnected())
    {
        WiFi.persistent(false);
        WiFi.disconnect();
        WiFi.persistent(true);
    }

    Serial.println("PROCESS: Starting web server portal");

    webServerPortal();

    while (true)
    {
        server.handleClient();
        /*
        if (configPortalHasTimeout())
            break;
        */
        if (connect)
        {
            delay(1000);
            connect = false;

            if (clientSSID == "")
            {
                Serial.println("ERROR: No SSID, skipping connection");
            }
            else
            {
                Serial.println("PROCESS: Connecting to new WiFi...");

                if (connectWifi() != WL_CONNECTED)
                {
                    delay(2000);
                    Serial.println("ERROR: Could not connect with saved parameters, retrying...");
                }
                else
                {
                    Serial.println("SUCCESS: Connected to WiFi at " + clientSSID);
                    delay(1000);
                    WiFi.mode(WIFI_STA);
                    break;
                }
            }
        }
        yield();
    }

    return WiFi.status() == WL_CONNECTED;
}

static bool autoConnect(void)
{

    WiFi.mode(WIFI_STA);

    Serial.println("PROCESS: Auto Connect starts");

    if (connectWifi() == WL_CONNECTED)
    {
        Serial.println("PROCESS: WiFi is already connected");
        return true;
    }
    else
    {
        Serial.println("PROCESS: Starting config portal");

        return startConfigPortal();
    }
}

void WifiHandler_loop(void)
{
    bool isConnected = WiFi.isConnected();

    if (!isConnected)
    {
        uint8_t isResistanceOn = relayHandler->isResistanceOn;
        sdHandler->writeToDay(isResistanceOn);
        sdHandler->writeToMonth(isResistanceOn);
        delay(5000);
        ESP.reset();
    }
}

void WifiHandler_init(SDHandler *_sdHandler)
{
    sdHandler = _sdHandler;

    sdHandler->deviceID.toCharArray(apName, 10);

    Serial.println("PROCESS: WIFI HANDLER INIT");

    delay(5000);

    if (!autoConnect())
    {
        ESP.reset();
        delay(3000);
    }
    else
    {
        server.close();
    }
}