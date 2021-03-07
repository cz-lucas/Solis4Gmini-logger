#include <Arduino.h>
#include "main.h"

#define staticIP
#ifdef staticIP
// Set your Static IP address
IPAddress local_IP(192, 168, 111, 230);
// Set your Gateway IP address
IPAddress gateway(192, 168, 111, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 111, 1);
IPAddress secondaryDNS(192, 168, 111, 2); // optional
#endif

inverter Inverter;
myMqtt MQTTClient;
myTicker ticker;
influx db;
timeControl Clock;
pvoutput PVoutput;
LED led;
DS18B20 ds18b20;

// ESP-Dash
AsyncWebServer server(80);
ESPDash dashboard(&server);
Card card_power(&dashboard, GENERIC_CARD, "Power (W)");
Card card_energyToday(&dashboard, GENERIC_CARD, "Energy Today (kWh)");
Card card_temperature(&dashboard, TEMPERATURE_CARD, "Temperature (°C)");

Card card_AC_U(&dashboard, GENERIC_CARD, "AC VOLTAGE (V)");
Card card_AC_I(&dashboard, GENERIC_CARD, "AC CURRENT (A)");
Card card_AC_F(&dashboard, GENERIC_CARD, "AC FREQUENCY (Hz)");

Card card_DC_U(&dashboard, GENERIC_CARD, "DC VOLTAGE (V)");
Card card_DC_I(&dashboard, GENERIC_CARD, "DC CURRENT (I)");

float dc_u = 0.0;
float dc_i = 0.0;

float ac_u = 0.0;
float ac_i = 0.0;
float ac_f = 10.0;

float energyToday = 0.0;
float power = 0.0;
float temperature = 0.0;

uint16_t Year = 2000;
uint8_t Month = 0;
uint8_t Day = 0;
uint8_t Hour = 0;
uint8_t Minute = 0;

void readInverter();
void notFound(AsyncWebServerRequest *request);
String buildResponse(byte type);

void setup()
{
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  led.begin();

  Serial.begin(115200);
  Serial.println("Hello");

  Inverter.begin();

  Serial.print("Connecting to: ");
  Serial.println(SSID);

#ifdef staticIP
  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
#endif

  WiFi.begin(SSID, PASSWORD);
  WiFi.hostname(HOSTNAME);

  uint8_t wifiCnt;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    led.yellowToggle();
    Serial.print(".");
    wifiCnt++;
    if (wifiCnt == 254)
    {
      led.yellowOn();
      delay(120000); // 120s
      ESP.restart(); // Restart ESP
    }
  }

  Serial.println(" ");
  Serial.println("Connected");

  // Start mqtt
  MQTTClient.begin();

  /* Start AsyncWebServer */
  server.onNotFound(notFound);

  server.on("/api/power.json", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", buildResponse(0));
  });
  server.on("/api/all.json", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", buildResponse(1));
  });

  server.begin();
  // Start OTA
  ArduinoOTA.begin();

  db.begin();
  Clock.begin();
  ds18b20.begin();

  // Read inverter
  readInverter();
  MQTTClient.sendValues(power, energyToday, ac_u, ac_i, ac_f, dc_u, dc_i, temperature);

  Clock.getDate(&Year, &Month, &Day);
  Clock.getTime(&Hour, &Minute);
  if (Inverter.isInverterReachable() == true)
  {
    PVoutput.send(Year, Month, Day, Hour, Minute, energyToday, power, dc_u, temperature);
    db.write(power, energyToday, ac_u, ac_i, ac_f, dc_u, dc_i, temperature);
  }

  ticker.begin();
  led.yellowOff();

  delay(10000);
  Serial.println("--------------------------------------------------------");
}

void loop()
{

  ArduinoOTA.handle();
  MQTTClient.loop();

  // Send status-message every 5 minutes
  if (ticker.getMqttStatusFlag() == true)
  {
    MQTTClient.sendStatus();
    ticker.setMqttStatusFlagToFalse();
  }

  // Send data every 45 seconds
  if (ticker.getReadFlag() == true)
  {
    readInverter();
    if (Inverter.isInverterReachable() == true)
    {
      led.disableNightblink();
      led.yellowOn();
      led.blueOn();
      MQTTClient.sendValues(power, energyToday, ac_u, ac_i, ac_f, dc_u, dc_i, temperature);
      Inverter.setIsInverterReachableFlagLast(true);
      delay(150);
    }
    else
    {
      led.blueOff();
      led.enableNightBlink();
    }

    if (Inverter.isInverterReachable() != Inverter.getIsInverterReachableFlagLast())
    {
      db.writeEnergy(energyToday);
      Inverter.setIsInverterReachableFlagLast(false);
    }

    led.yellowOff();
    ticker.setReadFlagToFalse();
  }

  if (ticker.getinfluxDBFlag() == true)
  {
    if (Inverter.isInverterReachable() == true)
    {
      db.write(power, energyToday, ac_u, ac_i, ac_f, dc_u, dc_i, temperature);
    }
    ticker.setInfluxDBFlagToFalse();
  }

  if (ticker.getPVoutputFlag() == true)
  {
    Clock.getDate(&Year, &Month, &Day);
    Clock.getTime(&Hour, &Minute);
    if (Inverter.isInverterReachable() == true)
    {
      PVoutput.send(Year, Month, Day, Hour, Minute, energyToday, power, dc_u, temperature);
    }
    else
    {
      if (Hour == 0)
      {
        energyToday = 0.0; // Reset EenergyToday at 0 o'clock
      }
    }
    ticker.setPVoutputFlagToFalse();
  }

  delay(300);

  digitalWrite(2, HIGH);
  //Clock.print();
}

void readInverter()
{
  Inverter.request();
  Inverter.getAC(&ac_u, &ac_i, &ac_f);
  Inverter.getDC(&dc_u, &dc_i);
  Inverter.getEnergyToday(&energyToday);
  Inverter.getPower(&power);
  Inverter.getTemperature(&temperature);
  Serial.println("------------------------------------------");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.println("W");

  Serial.print("DC U: ");
  Serial.print(dc_u);
  Serial.println("V");

  Serial.print("DC I: ");
  Serial.print(dc_i);
  Serial.println("A");

  Serial.print("AC U: ");
  Serial.print(ac_u);
  Serial.println("V");

  Serial.print("AC I: ");
  Serial.print(ac_i);
  Serial.println("A");

  Serial.print("AC F: ");
  Serial.print(ac_f);
  Serial.println("Hz");

  Serial.print("Energy today: ");
  Serial.print(energyToday);
  Serial.println("kWh");

  card_power.update(power);
  card_energyToday.update(energyToday);
  card_temperature.update(temperature);
  card_AC_U.update(ac_u);
  card_AC_I.update(ac_i);
  card_DC_U.update(dc_u);
  card_DC_I.update(dc_i);
  card_AC_F.update(ac_f);

  dashboard.sendUpdates();
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

String buildResponse(byte type)
{
  String str = "{";

  switch (type)
  {

  case 0: // Only return power
    str += "\"power\": ";
    str += String(power);
    str += ",\"energyToday\": ";
    str += String(energyToday);
    str += ",\"isOnline\": ";
    str += String(Inverter.isInverterReachable());
    break;

  case 1: // Return all data
    str += "\"power\": ";
    str += String(power);
    str += ",\"energyToday\": ";
    str += String(energyToday);
    str += ",\"isOnline\": ";
    str += String(Inverter.isInverterReachable());

    str += ",\"dc_u\": ";
    str += String(dc_u);
    str += ",\"dc_i\": ";
    str += String(dc_i);

    str += ",\"ac_u\": ";
    str += String(ac_u);
    str += ",\"ac_i\": ";
    str += String(ac_i);
    str += ",\"ac_f\": ";
    str += String(ac_f);
    str += ",\"temperature\": ";
    str += String(temperature);
    break;
  }

  str += "}";
  return str;
}