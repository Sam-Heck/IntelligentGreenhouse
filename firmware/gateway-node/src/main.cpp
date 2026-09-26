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

    const EspNowPayload *payload = (const EspNowPayload *)incomingData;

    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    if (payload->version != PROTOCOL_VERSION) {
        Serial.printf("Warning: Dropped packet from %s with unknown protocol v%u\n", 
                      macStr, payload->version);
        return;
    }

    JsonDocument doc;
    doc["version"] = payload->version;
    doc["messageType"] = payload->msgType;
    doc["deviceId"] = macStr;
    doc["batteryMv"] = payload->batteryMv;
    doc["sequenceId"] = payload->sequenceId;

    JsonArray readingsArr = doc["readings"].to<JsonArray>();

    uint8_t count = (payload->readingCount > 6) ? 6 : payload->readingCount;
    for (uint8_t i = 0; i < count; i++) {
        JsonObject item = readingsArr.add<JsonObject>();
        item["type"] = payload->readings[i].type;
        item["index"] = payload->readings[i].index;
        item["value"] = payload->readings[i].value;
    }

    serializeJson(doc, Serial);
    Serial.print('\n');
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

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

        if (!isnan(inTemp) && !isnan(inHumidity)) {
            JsonDocument doc;
            doc["version"] = PROTOCOL_VERSION;
            doc["messageType"] = MSG_TELEMETRY;
            doc["deviceId"] = gatewayMacStr;
            doc["batteryMv"] = 0; // Node interprets this as AC-powered.
            doc["sequenceId"] = localSeq++;

            JsonArray readingsArr = doc["readings"].to<JsonArray>();

            // Internal Sensor: index 0
            JsonObject r1 = readingsArr.add<JsonObject>();
            r1["type"] = TEMP_C;
            r1["index"] = 0;
            r1["value"] = inTemp;

            JsonObject r2 = readingsArr.add<JsonObject>();
            r2["type"] = HUMIDITY;
            r2["index"] = 0;
            r2["value"] = inHumidity;

            // External Sensor: index 1
            if (!isnan(outTemp)) {
                JsonObject r3 = readingsArr.add<JsonObject>();
                r3["type"] = TEMP_C;
                r3["index"] = 1;
                r3["value"] = outTemp;
            }

            if (!isnan(outHumidity)) {
                JsonObject r4 = readingsArr.add<JsonObject>();
                r4["type"] = HUMIDITY;
                r4["index"] = 1;
                r4["value"] = outHumidity;
            }

            serializeJson(doc, Serial);
            Serial.print('\n');
        }
    }
}