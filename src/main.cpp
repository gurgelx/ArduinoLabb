#include "RestServer.h"

#define LED D4
int ledStatus = HIGH;
RestServer restServer(LED);

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  restServer.startWifi();
}

void loop()
{
  restServer.handle();
}