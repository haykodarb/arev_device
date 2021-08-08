#include "WebServer.h"

static ESP8266WebServer server(80);
static WebSocketsServer webSocket = WebSocketsServer(81);

static ConfigValues *configValues;
static SDHandler *sdHandler;
static SensorData *sensorData;
static RealTimeClock *realTimeClock;
static RelayHandler *relayHandler;
MDNSResponder::hMDNSService hMDNSService = 0; // The handle of the clock service in the MDNS responder

static bool shouldBroadcast = false;

static void handleRoot(void)
{
    server.send(200, "text/plain", "Root page");
}

static void handleDayFile(void)
{
    String fileToOpen = "/day/" + server.arg("file") + ".csv";
    if (SD.exists(fileToOpen))
    {
        File myFile;
        myFile = SD.open(fileToOpen);
        server.streamFile(myFile, "text/plain");
        myFile.close();
    }
    else
    {
        server.send(400, "text/plain", "File does not exist");
    }
}

static void handleMonthFile(void)
{
    String fileToOpen = "/month/" + server.arg("file") + ".csv";
    if (SD.exists(fileToOpen))
    {
        File myFile;
        myFile = SD.open(fileToOpen);
        server.streamFile(myFile, "text/plain");
        myFile.close();
    }
    else
    {
        server.send(400, "text/plain", "File does not exist");
    }
}

static void handleConfigSet(void)
{
    String jsonInput = server.arg("plain");
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonInput);

    int newSystState = doc["systState"].as<int>();
    int newInitTime = doc["initTime"].as<int>();
    int newFinalTime = doc["finalTime"].as<int>();
    int newInitTemp = doc["initTemp"].as<int>();
    int newFinalTemp = doc["finalTemp"].as<int>();

    configValues->update(newInitTemp, newFinalTemp, newInitTime, newFinalTime, newSystState);

    sdHandler->writeConfig(jsonInput);

    server.send(200, "text/plain", "Configuration saved correctly.");
}

static void handleConfigGet(void)
{
    StaticJsonDocument<200> data;
    String jsonOutput;

    data["systState"] = configValues->systState;
    data["initTime"] = configValues->initTime;
    data["finalTime"] = configValues->finalTime;
    data["initTemp"] = configValues->initTemp;
    data["finalTemp"] = configValues->finalTemp;

    serializeJson(data, jsonOutput);

    server.send(200, "application/json", jsonOutput);
}

static void handleTimeAdjust(void)
{
    uint32_t epochTime = server.arg("epoch").toInt();
    DateTime newTime = DateTime(epochTime);
    realTimeClock->adjust(newTime);
    server.send(200, "text/plain", "Tiempo ajustado correctamente");
}

static void handleNameChange(void)
{
    String newName = server.arg("new");
    sdHandler->writeName(newName);

    String deviceID = sdHandler->deviceID;

    MDNS.close();
    MDNS.setHostname(newName);
    MDNS.begin(deviceID);

    server.send(200, "text/plain", "Nombre cambiado correctamente");
}

static void handleReset(void)
{
    ESP.eraseConfig();
    ESP.reset();
}

static void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_CONNECTED)
    {
        shouldBroadcast = true;
    }
}

void WebServer::broadcast(void)
{
    String currentTime = realTimeClock->webSocketTime();
    String currentTemp = String(sensorData->currentTemp);
    String isResistanceOn = String(relayHandler->isResistanceOn);
    String systState = String(configValues->systState);

    String webSocketPacket = currentTime + "," + currentTemp + "," + isResistanceOn + "," + systState;
    webSocket.broadcastTXT(webSocketPacket);
}

void WebServer::init(ConfigValues *_configValues, SDHandler *_sdHandler, SensorData *_sensorData, RealTimeClock *_realTimeClock, RelayHandler *_relayHandler)
{
    configValues = _configValues;
    sdHandler = _sdHandler;
    sensorData = _sensorData;
    realTimeClock = _realTimeClock;
    relayHandler = _relayHandler;

    String instanceName = sdHandler->deviceName;
    String mdnsAddress = sdHandler->deviceID;

    hMDNSService = MDNS.addService(0, "arev", "tcp", 80);
    MDNS.addServiceTxt(hMDNSService, "type", 1);
    MDNS.setHostname(instanceName);
    MDNS.begin(mdnsAddress);

    server.on("/", handleRoot);
    server.on("/files/day", handleDayFile);
    server.on("/files/month", handleMonthFile);
    server.on("/config/set", handleConfigSet);
    server.on("/config/get", handleConfigGet);
    server.on("/time", handleTimeAdjust);
    server.on("/name", handleNameChange);
    server.on("/reset", handleReset);

    server.begin();

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void WebServer::loop(void)
{

    MDNS.update();

    server.handleClient();

    webSocket.loop();

    if (shouldBroadcast)
    {
        this->broadcast();
        shouldBroadcast = false;
    }
}