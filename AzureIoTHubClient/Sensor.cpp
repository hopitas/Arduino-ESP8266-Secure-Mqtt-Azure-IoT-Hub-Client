#include "Sensor.h"
#include "MqttClient.h"
#include <math.h>

Sensor::Sensor(MqttClient *mqttClient)
{
  _mqttClient = mqttClient;
}

void Sensor::measure()
{
  temperature = 25;
  humidity = 50;
  pressure = 1000;

  Serial.println(temperature);
  Serial.println(pressure);
  Serial.println(humidity);
}

char *Sensor::toJSON()
{
  /*  https://github.com/bblanchon/ArduinoJson/wiki/Memory-model
    Have allowed for a few extra json fields that actually being used at the moment
*/

  StaticJsonBuffer<JSON_OBJECT_SIZE(16)> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();

  root["Utc"] = getISODateTime();
  root["Celsius"] = round(temperature * 10) / 10.0;
  root["Humidity"] = round(humidity);
  root["hPa"] = pressure;
  root["Light"] = light;
  root["Geo"] = geo;
  root["Schema"] = 1;

//instrumentation
//  root["WiFi"] = telemetry->WiFiConnectAttempts;
#ifdef ARDUINO_ARCH_ESP8266
  root["Mem"] = ESP.getFreeHeap();
#endif
  root["Id"] = ++msgId;

  root.printTo(buffer, sizeof(buffer));

  return buffer;
}

char *Sensor::getISODateTime()
{
  sprintf(isoTime, "%4d-%02d-%02dT%02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
  return isoTime;
}
