#include "mqtt.h"

#ifdef MQTT
WiFiClient net;
PubSubClient client(net);
#endif

void reconnect();
void connect();
void mqtt_callback(char *topic, byte *payload, unsigned int length);

myMqtt::myMqtt()
{
}

/*
Starts MQTT
*/
void myMqtt::begin()
{
#ifdef MQTT
    Serial.println("Connecting to Mqtt...");
    connect();
#endif
}

/*
Loops MQTT
*/
void myMqtt::loop()
{
#ifdef MQTT
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
#endif
}

/*
Send values over MQTT
@param power Actual power (float)
@param energyToday Energy today (float)
@param AC_U AC voltage (float)
@param AC_I AC current (float)
@param AC_F AC frequency (float)
@param DC_U DC voltage (float)
@param DC_I DC current (float)
@param temperature temperature (float)
*/
void myMqtt::sendValues(float power, float energyToday, float AC_U, float AC_I, float AC_F, float DC_U, float DC_I, float temperature)
{
#ifdef MQTT
    char bufferSend[10];

    dtostrf(power, 0, 0, bufferSend);
    client.publish(mqtt_power_topic, bufferSend);
    dtostrf(energyToday, 0, 1, bufferSend);
    client.publish(mqtt_eToday_topic, bufferSend);
    dtostrf(temperature, 0, 1, bufferSend);
    client.publish(mqtt_temp_topic, bufferSend);

    dtostrf(AC_U, 0, 1, bufferSend);
    client.publish(mqtt_acu_topic, bufferSend);
    dtostrf(AC_I, 0, 1, bufferSend);
    client.publish(mqtt_aci_topic, bufferSend);
    dtostrf(AC_F, 0, 2, bufferSend);
    client.publish(mqtt_acf_topic, bufferSend);

    dtostrf(DC_U, 0, 1, bufferSend);
    client.publish(mqtt_dcu_topic, bufferSend);
    dtostrf(DC_I, 0, 1, bufferSend);
    client.publish(mqtt_dci_topic, bufferSend);
#endif
}

/*
Sends status
*/
void myMqtt::sendStatus()
{
#ifdef MQTT
    char bufferSend[10];

    dtostrf(WiFi.RSSI(), 0, 0, bufferSend);
    client.publish(mqtt_status_rssi_topic, bufferSend);
#endif
}

#ifdef otherNode
void myMqtt::sendOtherNode(String str)
{
#ifdef MQTT
    client.publish(mqttOtherNodeHoldingTopic, (char *)str.c_str());
#endif
}
#endif

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
#ifdef MQTT
    String sTopic = String(topic);
    // Workaround to get int from payload
    payload[length] = '\0';
    //uint32_t value = String((char * ) payload).toInt();

    if (sTopic == mqtt_info_ping_topic)
    {
        client.publish(mqtt_info_pong_topic, "pong");
        char bufferSend[10];
        dtostrf(WiFi.RSSI(), 0, 0, bufferSend);
        client.publish(mqtt_status_rssi_topic, bufferSend);
    }
#endif
}

void reconnect()
{
#ifdef MQTT
    while (!client.connected())
    {
        Serial.println("Mqtt reconnect");
        String clientId = "esp8266-";
        clientId += String(WiFi.macAddress());

#ifdef MQTTUSER
        if (client.connect(clientId.c_str(), MQTTUSER, MQTTPASS))
        {
            client.subscribe(mqtt_base_topic);
        }
        else
        {
            delay(2000);
        }
#else
        if (client.connect(clientId.c_str()))
        {
            client.subscribe(mqtt_base_topic);
        }
        else
        {
            delay(2000);
        }
#endif
    }
#endif
}

//Connect to WLAN subroutine
void connect()
{
#ifdef MQTT
    Serial.print("IP number assigned by DHCP is ");
    Serial.println(WiFi.localIP());

    //connecting Mqtt
    Serial.println("Connecting to mqtt");
    client.setServer(mqttBrokerIP, mqttBrokerPort);
    client.setCallback(mqtt_callback);

#ifdef MQTTUSER
    while (!client.connected())
    {
        String clientId = "esp8266-";
        clientId += String(WiFi.macAddress());
        if (client.connect(clientId.c_str(), MQTTUSER, MQTTPASS))
        {
            client.subscribe(mqtt_base_topic);
        }
        else
        {
            delay(2000);
        }
    }
#else
    while (!client.connected())
    {
        String clientId = "esp8266-";
        clientId += String(WiFi.macAddress());
        if (client.connect(clientId.c_str()))
        {
            client.subscribe(mqtt_base_topic);
        }
        else
        {
            delay(2000);
        }
    }
#endif
#endif
}
