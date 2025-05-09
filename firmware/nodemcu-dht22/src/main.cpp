#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "secrets.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_BROKER;

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor pins and type
#define DHTPIN1 D2
#define DHTPIN2 D3
#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

// MQTT topic
const char* topic = "greenhouse/sensors";

void connectToWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected to WiFi.");
}

void connectToMQTT() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("NodeMCU_DataPublisher")) {
            Serial.println(" connected.");
        } else {
            Serial.print(" failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 2 seconds...");
            delay(2000);
        }
    }
}

void setup() {
    Serial.begin(9600);
    dht1.begin();
    dht2.begin();

    connectToWiFi();

    client.setServer(mqtt_server, 1883);
    connectToMQTT();
}

void loop() {
    if (!client.connected()) {
        connectToMQTT();
    }
    client.loop();

    float h1 = dht1.readHumidity();
    float t1 = dht1.readTemperature() * 1.8 + 32;
    float h2 = dht2.readHumidity();
    float t2 = dht2.readTemperature() * 1.8 + 32;
    Serial.println("h1 sensor value: " + String(h1));
    Serial.println("t1 sensor value: " + String(t1));

    Serial.println("h2 sensor value: " + String(h2));
    Serial.println("t2 sensor value: " + String(t2));


    if (!isnan(h1) && !isnan(t1) && !isnan(h2) && !isnan(t2)) {
        String payload = "{\"sensor1\": {\"temp\": " + String(t1) +
                         ", \"humidity\": " + String(h1) + "}, " +
                         "\"sensor2\": {\"temp\": " + String(t2) +
                         ", \"humidity\": " + String(h2) + "}}";

        client.publish(topic, payload.c_str());
        Serial.println("Published: " + payload);
    } else {
        Serial.println("Failed to read from DHT sensors");
    }

    delay(60000);
}
