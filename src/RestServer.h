#ifndef RestServer_h
#define RestServer_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "RestCommand.cpp"

class RestServer
{
public:
    RestServer(uint ledPin);
    void startWifi();
    RestCommand listen();

private:
    String readLine(WiFiClient client);
    void handleCommand(String command);
    void sendOK(WiFiClient client);
    void wifiConnected(WiFiManager *cbWM);
    RestCommand parse(String command); 

    int m_port = 3000;
    uint m_ledPin;
    WiFiServer m_server;
};

#endif