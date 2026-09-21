// shared/protocol.h
#pragma once
#include <Arduino.h>

enum MessageType : uint8_t {
  MSG_TELEMETRY = 1,
  MSG_HEARTBEAT = 2,
  MSG_COMMAND = 3,
  MSG_ACK = 4
};

struct __attribute__((packed)) EspNowPayload {
  uint8_t version;
  uint8_t msgType;
  char deviceType[16];
  uint16_t batteryMv;
  uint32_t sequenceId;
  uint8_t valueCount;
  float values[6];
};