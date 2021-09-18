#include "SDHandler.h"

static RealTimeClock *realTimeClock;
static SensorData *sensorData;
static ConfigValues *configValues;

void SDHandler::init(RealTimeClock *_realTimeClock, SensorData *_sensorData, ConfigValues *_configValues)
{
    realTimeClock = _realTimeClock;
    sensorData = _sensorData;
    configValues = _configValues;

    deviceID = "arev" + String(ESP.getChipId(), 16);
    deviceID.toLowerCase();

    SD.begin(CS_PIN);

    readName();
    readConfig();
}

void SDHandler::writeName(String newName)
{
    if (SD.exists("/config/name.txt"))
    {
        SD.remove("/config/name.txt");
    }
    File nameFile = SD.open("/config/name.txt", FILE_WRITE);
    deviceName = newName;
    nameFile.print(newName);
    nameFile.close();
}

void SDHandler::readName(void)
{
    if (SD.exists("/config/name.txt"))
    {

        File nameFile = SD.open("/config/name.txt");
        String newName = "";

        while (nameFile.available())
        {
            char newChar = nameFile.read();
            newName += newChar;
        }

        deviceName = newName;
        nameFile.close();
    }
    else
    {
        deviceName = deviceID;
    }
}

void SDHandler::readConfig(void)
{
    if (SD.exists("/config/params.txt"))
    {
        File configFile = SD.open("/config/params.txt");
        String jsonInput = "";
        while (configFile.available())
        {
            char newChar = configFile.read();
            jsonInput = jsonInput + newChar;
        }
        configFile.close();

        StaticJsonDocument<200> jsonObject;

        deserializeJson(jsonObject, jsonInput);

        int newSystState = jsonObject["systState"].as<int>();
        int newInitTime = jsonObject["initTime"].as<int>();
        int newFinalTime = jsonObject["finalTime"].as<int>();
        int newInitTemp = jsonObject["initTemp"].as<int>();
        int newFinalTemp = jsonObject["finalTemp"].as<int>();

        configValues->update(newInitTemp, newFinalTemp, newInitTime, newFinalTime, newSystState);
    }
    else
    {
        configValues->update(40, 45, 10, 22, 0);
    }
}

void SDHandler::writeConfig(String jsonInput)
{
    if (SD.exists("/config/params.txt"))
    {
        SD.remove("/config/params.txt");
    }
    File myFile;
    myFile = SD.open("/config/params.txt", FILE_WRITE);
    myFile.print(jsonInput);
    myFile.close();
}

void SDHandler::writeToDay(uint8_t _isResistanceOn)

{
    String currentDate = realTimeClock->currentDate();
    uint32_t currentTime = realTimeClock->timeInSeconds();

    float currentTemp = sensorData->currentTemp;
    uint8_t isResistanceOn = _isResistanceOn;

    String dataToWrite = String(currentTime) + "," + String(currentTemp) + "," + String(isResistanceOn) + ";";

    File myFile;
    String fileToOpen = "/day/" + currentDate + ".csv";

    /* Analizar todos los posibles escenarios respecto a esto */
    if (!SD.exists(fileToOpen))
    {
        String firstRead = "0," + String(currentTemp) + "," + String(isResistanceOn) + ";";
        myFile = SD.open(fileToOpen, FILE_WRITE);
        myFile.print(firstRead);
        myFile.close();
    }

    myFile = SD.open(fileToOpen, FILE_WRITE);
    myFile.print(dataToWrite);
    myFile.close();
}

void SDHandler::writeToMonth(uint8_t _isResistanceOn)
{

    uint32_t currentTime = realTimeClock->timeInSeconds();
    String currentMonth = realTimeClock->currentMonth();
    String dayNumber = realTimeClock->dayNumber();

    uint8_t isResistanceOn = _isResistanceOn;

    String dataToWrite = dayNumber + "," + String(currentTime) + "," + String(isResistanceOn) + ";";

    File myFile;
    String fileToOpen = "/month/" + currentMonth + ".csv";
    myFile = SD.open(fileToOpen, FILE_WRITE);
    myFile.print(dataToWrite);
    myFile.close();
}