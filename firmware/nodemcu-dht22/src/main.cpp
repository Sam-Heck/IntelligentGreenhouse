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

void setup() {
    Serial.begin(115200);
    dht1.begin();
    dht2.begin();

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    // Create each card sprite
    card1.createSprite(CARD_WIDTH - CARD_PADDING * 2, 60);
    card2.createSprite(CARD_WIDTH - CARD_PADDING * 2, 60);
    
    card1.setTextSize(2);
    card1.setTextColor(TFT_WHITE, TFT_DARKGREY);
    card2.setTextSize(2);
    card2.setTextColor(TFT_WHITE, TFT_DARKGREY);
    
    tft.fillScreen(TFT_BLACK);

    // Draw static card backgrounds directly on the screen
    tft.fillRoundRect(CARD_MARGIN, CARD_MARGIN, CARD_WIDTH, CARD_HEIGHT, 8, TFT_DARKGREY);
    tft.fillRoundRect(CARD_MARGIN, CARD_MARGIN * 2 + CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT, 8, TFT_DARKGREY);
    
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
    
        // --- Sensor 1 Card Text Sprite ---
        card1.fillSprite(TFT_DARKGREY);
        card1.setCursor(0, 0);
        card1.setTextColor(TFT_CYAN, TFT_DARKGREY);
        card1.println("Sensor 1");
    
        card1.setTextColor(TFT_WHITE, TFT_DARKGREY);
        card1.setCursor(TEXT_INDENT, 20);
        if (!isnan(t1) && !isnan(h1)) {
            card1.printf("Temp: %.1f F\n", t1);
            card1.setCursor(TEXT_INDENT, 40);
            card1.printf("Humidity: %.1f%%", h1);
        } else {
            card1.println("Error reading data");
        }
    
        // Push only the text sprite (not full card)
        card1.pushSprite(CARD_MARGIN + CARD_PADDING, CARD_MARGIN + CARD_PADDING + 20);
    
        // --- Sensor 2 Card Text Sprite ---
        card2.fillSprite(TFT_DARKGREY);
        card2.setCursor(0, 0);
        card2.setTextColor(TFT_CYAN, TFT_DARKGREY);
        card2.println("Sensor 2");
    
        card2.setTextColor(TFT_WHITE, TFT_DARKGREY);
        card2.setCursor(TEXT_INDENT, 20);
        if (!isnan(t2) && !isnan(h2)) {
            card2.printf("Temp: %.1f F\n", t2);
            card2.setCursor(TEXT_INDENT, 40);
            card2.printf("Humidity: %.1f%%", h2);
        } else {
            card2.println("Error reading data");
        }
    
        card2.pushSprite(CARD_MARGIN + CARD_PADDING, CARD_MARGIN * 2 + CARD_HEIGHT + CARD_PADDING + 20);
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