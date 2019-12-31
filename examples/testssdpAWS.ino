#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ssdpAWS.h> //SSDP for ESPAsyncWebServer 

AsyncWebServer server(80);
ssdpAWS mySSDP(&server); 

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const char* PARAM_MESSAGE = "message";

//Define SSDP and model name
const char* SSDP_Name = "mySSDPName";
const char* modelName = "myDevice";
const char* nVersion = "v1.11";
const char* SerialNumber = "000000001";
const char* Manufacturer = "LIGHTWELL";
const char* ManufacturerURL = "https://diy.led-lightwell.eu";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    //******************Start SSDP*********************
    mySSDP.begin(SSDP_Name, SerialNumber, modelName, nVersion, Manufacturer, ManufacturerURL); Serial.println(F("Start SSDP"));

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
}
