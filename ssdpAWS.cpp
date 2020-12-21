#include <Arduino.h>
#include <ssdpAWS.h>

ssdpAWS::ssdpAWS(AsyncWebServer *ws) {
    _ws = ws;
}

void ssdpAWS::begin(const char *nameSSDP, const char *sernum, const char *nameModel, const char *versionModel,
        const char *nameManuf, const char *urlManuf) {
            _nameSSDP = nameSSDP;
            _sernum = sernum;
            _nameModel = nameModel;
            _versionModel = versionModel;
            _nameManuf = nameManuf;
            _urlManuf = urlManuf;

            SSDP.setDeviceType("upnp:rootdevice");
            SSDP.setSchemaURL("description.xml");
            SSDP.setHTTPPort(80);
            SSDP.setName(_nameSSDP);
            SSDP.setSerialNumber(_sernum);
            SSDP.setURL("/");
            SSDP.setModelName(_nameModel);
            SSDP.setModelNumber(_versionModel);
            SSDP.setModelURL("http://" + String(WiFi.status() != WL_CONNECTED ? WiFi.softAPIP() : WiFi.localIP()));
            SSDP.setManufacturer(_nameManuf);
            SSDP.setManufacturerURL(_urlManuf);
            SSDP.begin();
            setHTTPAnswer();
}

void ssdpAWS::setHTTPAnswer() {
    _ws->on("/description.xml", HTTP_GET, [&](AsyncWebServerRequest *request){
      request->send(200, "text/xml", getSSDPSchema());
    });
}

String ssdpAWS::getSSDPSchema() {
    uint32_t chipId;
    #ifdef ESP32
        chipId = ESP.getEfuseMac();
    #else
        chipId = ESP.getChipId();    
    #endif  
	String s = "";
    s +="<?xml version=\"1.0\"?>\n";
    s +="<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n";
    s +="\t<specVersion>\n";
    s +="\t\t<major>1</major>\n";
    s +="\t\t<minor>0</minor>\n";
    s +="\t</specVersion>\n";
    s +="<URLBase>";
    s +="http://"+(WiFi.status() != WL_CONNECTED ? WiFi.softAPIP().toString() : WiFi.localIP().toString());
    s +="</URLBase>";
    s +="<device>\n";
    s +="\t<deviceType>upnp:rootdevice</deviceType>\n";
    s +="\t<friendlyName>";
    s += _nameSSDP;
    s +="</friendlyName>\r\n";
    s +="\t<presentationURL>index.html</presentationURL>\r\n";
    s +="\t<serialNumber>";
    s += String(chipId);
    s +="</serialNumber>\r\n";
    s +="\t<modelName>";
    s += _nameModel;
    s +="</modelName>\r\n";
    s +="\t<modelNumber>";
    s += _versionModel;
    s +="</modelNumber>\r\n";
    s +="\t<modelURL>";
    s += _urlManuf;
    s += "</modelURL>\r\n";
    s +="\t<manufacturer>";
    s += _nameManuf;
    s +="</manufacturer>\r\n";
    s +="\t<manufacturerURL>";
    s += _urlManuf;
    s +="</manufacturerURL>\r\n";
    s +="\t<UDN>38323636-4558-4dda-9188-cda0e6";
    s +=String((uint8_t) ((chipId >> 16) & 0xff));
    s +=String((uint8_t) ((chipId >>  8) & 0xff));
    s +=String((uint8_t)   chipId        & 0xff);
    s +="</UDN>\r\n";
    s +="\t</device>\n";
    s +="</root>\r\n\r\n";
	return s;
}