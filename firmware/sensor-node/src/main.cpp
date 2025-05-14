#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "secrets.h"
#include <TFT_eSPI.h>

const char* ssid = WIFI_SSID; 
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_BROKER;

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN1 16
#define DHTPIN2 17
#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite card1 = TFT_eSprite(&tft);
TFT_eSprite card2 = TFT_eSprite(&tft);

const int CARD_WIDTH = 300;
const int CARD_HEIGHT = 90;
const int CARD_MARGIN = 10;
const int CARD_PADDING = 10;
const int TEXT_INDENT = 20;
const int STATUS_BAR_HEIGHT = 20;

String topic;

const unsigned long DISPLAY_INTERVAL = 2000;
const unsigned long PUBLISH_INTERVAL = 60000;

unsigned long lastDisplayUpdate = 0;
unsigned long lastPublish = 0;

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
        String clientId = "ESP32_" + WiFi.macAddress();
        clientId.replace(":", "");
        if (client.connect(clientId.c_str())) {
            Serial.println(" connected.");
        } else {
            Serial.print(" failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 2 seconds...");
            delay(2000);
        }
    }
}

void drawSensorCard(
    TFT_eSprite& sprite,
    const char* label,
    float temp,
    float humidity,
    int x,
    int y
) {
    // Draw rounded background inside sprite
    sprite.fillSprite(TFT_BLACK);  // base fill to erase
    sprite.fillRoundRect(0, 0, CARD_WIDTH, CARD_HEIGHT, 8, TFT_DARKGREY);


    sprite.drawRoundRect(0, 0, CARD_WIDTH, CARD_HEIGHT, 8, TFT_LIGHTGREY);

    // Title
    sprite.setTextSize(2);
    sprite.setTextColor(TFT_CYAN, TFT_DARKGREY);
    sprite.setCursor(CARD_PADDING, CARD_PADDING);
    sprite.println(label);

    // Sensor values
    sprite.setTextColor(TFT_WHITE, TFT_DARKGREY);
    sprite.setCursor(CARD_PADDING + TEXT_INDENT, CARD_PADDING + 24);
    if (!isnan(temp) && !isnan(humidity)) {
        sprite.printf("Temp: %.1f F\n", temp);
        sprite.setCursor(CARD_PADDING + TEXT_INDENT, CARD_PADDING + 44);
        sprite.printf("Humidity: %.1f%%", humidity);
    } else {
        sprite.println("Sensor error");
    }

    sprite.pushSprite(x, y);
}

void drawStatusBar(bool wifiConnected, bool mqttConnected) {
    // Draw background bar
    tft.fillRect(0, 0, tft.width(), STATUS_BAR_HEIGHT, TFT_BLACK);
  
    // Set status color
    if (wifiConnected && mqttConnected) {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(10, 4);
      tft.print("Status: Connected");
    } else {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.setCursor(10, 4);
      tft.print("Status: Connecting...");
    }
  }
  

void setup() {
    Serial.begin(115200);
    dht1.begin();
    dht2.begin();

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    card1.createSprite(CARD_WIDTH, CARD_HEIGHT);
    card2.createSprite(CARD_WIDTH, CARD_HEIGHT);
    card1.setTextSize(2);
    card2.setTextSize(2);

    connectToWiFi();

    // MQTT topic
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    mac.toLowerCase();
    Serial.println("MAC Address: " + mac);

    topic = "greenhouse/" + mac + "/sensors";
    client.setServer(mqtt_server, 1883);
    connectToMQTT();


}

void loop() {
    bool wifiOK = WiFi.status() == WL_CONNECTED;
    bool mqttOK = client.connected();
    drawStatusBar(wifiOK, mqttOK);    

    if (!client.connected()) {
        connectToMQTT();
    }
    client.loop();

    unsigned long now = millis();
    float h1 = NAN, t1 = NAN, h2 = NAN, t2 = NAN;

    bool shouldReadSensors = false;
    bool displayDue = now - lastDisplayUpdate >= DISPLAY_INTERVAL;
    bool publishDue = now - lastPublish >= PUBLISH_INTERVAL;

    if (displayDue || publishDue) {
        h1 = dht1.readHumidity();
        t1 = dht1.readTemperature(true);
        h2 = dht2.readHumidity();
        t2 = dht2.readTemperature(true);
    }

    if (displayDue) {
        lastDisplayUpdate = now;
    
        drawSensorCard(
            card1, "Sensor 1", t1, h1,
            CARD_MARGIN, STATUS_BAR_HEIGHT + CARD_MARGIN
        );
    
        drawSensorCard(
            card2, "Sensor 2", t2, h2,
            CARD_MARGIN, STATUS_BAR_HEIGHT + CARD_MARGIN * 2 + CARD_HEIGHT
        );
    }    
    
    if (publishDue) {
        lastPublish = now;
    
        if (!isnan(h1) && !isnan(t1) && !isnan(h2) && !isnan(t2)) {
            String payload = "{\"1\": {\"temp\": " + String(t1) +
                             ", \"humidity\": " + String(h1) + "}, " +
                             "\"2\": {\"temp\": " + String(t2) +
                             ", \"humidity\": " + String(h2) + "}}";
    
            client.publish(topic.c_str(), payload.c_str());
            Serial.println("Published to topic: " + topic);
            Serial.println("Payload: " + payload);
    
        } else {
            Serial.println("Failed to read from DHT sensors");
        }
    }
}