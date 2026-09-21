#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include "protocol.h"

#define DHTPIN_INTERNAL 4
#define DHTPIN_EXTERNAL 5
#define DHTTYPE DHT22

DHT dhtInternal(DHTPIN_INTERNAL, DHTTYPE);
DHT dhtExternal(DHTPIN_EXTERNAL, DHTTYPE);

const unsigned long LOCAL_SENSOR_INTERVAL_MS = 5000;
unsigned long lastLocalSensorRead = 0;
String gatewayMacStr = "";
uint32_t localSeq = 0;

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len != sizeof(EspNowPayload)) {
        return;
    }

    EspNowPayload payload;
    memcpy(&payload, incomingData, sizeof(EspNowPayload));

    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    StaticJsonDocument<256> doc;
    doc["messageType"] = "telemetry";
    doc["nodeId"] = macStr;
    doc["device"] = payload.deviceType;
    doc["batteryMv"] = payload.batteryMv;
    doc["seq"] = payload.sequenceId;

    JsonArray raw = doc.createNestedArray("raw");
    for (uint8_t i = 0; i < payload.valueCount; i++) {
        raw.add(payload.values[i]);
    }

    serializeJson(doc, Serial);
    Serial.print('\n');
}

void setup() {
    Serial.begin(115200);

    dhtInternal.begin();
    dhtExternal.begin();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    gatewayMacStr = WiFi.macAddress();

    if (esp_now_init() != ESP_OK) {
        Serial.println("{\"type\":\"error\",\"msg\":\"ESP-NOW Init Failed\"}");
        return;
    }

    esp_now_register_recv_cb(onDataRecv);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastLocalSensorRead >= LOCAL_SENSOR_INTERVAL_MS) {
        lastLocalSensorRead = currentMillis;

        float inTemp = dhtInternal.readTemperature();
        float inHumidity = dhtInternal.readHumidity();
        float outTemp = dhtExternal.readTemperature();
        float outHumidity = dhtExternal.readHumidity();

        // get device mac address

        if (!isnan(inTemp) && !isnan(inHumidity)) {
            StaticJsonDocument<256> doc;
            doc["messageType"] = "telemetry";
            doc["nodeId"] = gatewayMacStr;
            doc["device"] = "gateway-node";
            doc["batteryMv"] = 0; // Node interprets this as AC-powered.
            doc["seq"] = localSeq++;

            JsonArray raw = doc.createNestedArray("raw");
            raw.add(inTemp);
            raw.add(inHumidity);
            raw.add(outTemp);
            raw.add(outHumidity);

            serializeJson(doc, Serial);
            Serial.print('\n');
        }
    }
}