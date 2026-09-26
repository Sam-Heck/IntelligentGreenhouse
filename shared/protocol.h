// shared/protocol.h
#pragma once

#include <stdint.h>

#define PROTOCOL_VERSION 1
#define SERIAL_BAUD_RATE 115200

enum MessageType : uint8_t {
  MSG_TELEMETRY = 1,
  MSG_HEARTBEAT = 2,
  MSG_COMMAND = 3,
  MSG_ACK = 4
};

enum SensorType : uint8_t {
    TEMP_C = 1,
    HUMIDITY = 2,
    PRESSURE_PA = 3,
    SOIL_MOIST = 4,
    LIGHT_LUX = 5,
    RELAY_STATE = 6,
    SERVO_STATE = 7
};

struct __attribute__((packed)) Reading {
    uint8_t type;
    uint8_t index;
    float value;
};

struct __attribute__((packed)) EspNowPayload {
  uint8_t version;
  uint8_t msgType;
  uint16_t batteryMv;
  uint32_t sequenceId;
  uint8_t readingCount;
  Reading readings[6];
};