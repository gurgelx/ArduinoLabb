#include "RestServer.h"
#include "Light.h"

#define LED D4
#define LED_PIN D1
#define LED_COUNT 12

int ledStatus = HIGH;
RestServer restServer(LED);
Light light(LED_PIN, LED_COUNT);

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  restServer.startWifi();
  // light.runProgram(Program::nightrider);
}

void handleCommand(RestCommand &cmd)
{
  if (cmd.path == "program")
  {
    Serial.println("running program");
    int n = std::atoi(cmd.value.c_str());
    Serial.println(n);
    Program p = static_cast<Program>(n);
    light.runProgram(p);
  }
}

void loop()
{
  long tick = millis();
  light.update(tick);
  RestCommand cmd = restServer.handle();
  if (!cmd.empty)
  {
    handleCommand(cmd);
  }
}
