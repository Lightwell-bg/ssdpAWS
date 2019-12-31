#ifndef ssdpAWS_h
#define ssdpAWS_h
#include <Arduino.h>
#ifdef ESP32
  #include <ESP32SSDP.h>
  #include <WiFi.h>
#else
  #include <ESP8266SSDP.h>
  #include <ESP8266WiFi.h>
#endif
#include <ESPAsyncWebServer.h>

class ssdpAWS {
    public:
        ssdpAWS(AsyncWebServer *ws);
        void begin(const char *nameSSDP, const char *sernum, const char *nameModel, const char *versionModel,
        const char *nameManuf, const char *urlManuf);
        String getSSDPSchema();
        void setHTTPAnswer();
        
    private:
        AsyncWebServer* _ws;
        const char *_nameSSDP;
        const char *_sernum;
        const char *_nameModel;
        const char *_versionModel;
        const char *_nameManuf;
        const char *_urlManuf;
        
};

#endif