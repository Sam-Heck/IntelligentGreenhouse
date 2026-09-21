# Data Transfer Interfaces

## Goal
Define a unified and flexible data transfer shape so that additional remote nodes of various functions (ie: soil moisture, heater relay, window servos) can be added without writing custom firmware for each type. To accomplish this, all telemetry between nodes via ESP-NOW  will use the same fixed-width binary envelope, which will map to a key-value schema and the host gateway. Sensor type and info will be included with a fixed values array containing the data, which can be interpreted based on sensor type.

## 1. Layer 1 -> Layer 2: Remote Node to Gateway Node (ESP-NOW)
- **Payload Size:** 56 bytes

```cpp
enum MessageType : uint8_t {
    MSG_TELEMETRY = 1,
    MSG_HEARTBEAT = 2,
    MSG_COMMAND = 3,
    MSG_ACK = 4
};

struct __attribute__((packed)) EspNowPayload {
    uint8_t version;        // Protocol version check
    uint8_t msgType;        // MessageType enum
    char deviceType[16];    // identifier for parser routing (ie: "soil_probe")
    uint16_t batteryMv;     // battery millivolts
    uint32_t sequenceId;    // detect dropped packets
    uint8_t valueCount;     // elements populated in values[]
    float values[6];        // positional floats mapped by deviceType
}
```

## 2. Layer 2 -> Layer 3: Gateway Node to Host (USB Serial)
**Framing:** newline delimited JSON so Node.js `readline` knows when the serial stream ends. 

```json
{
    "messageType": "telemetry",
    "nodeId": "soil-bed-01",
    "device": "soil-probe",
    "batteryMv": 3820,
    "seq": 104,
    "data": {
        "moisture": 42.5,
        "temperature": 19.4
    }
}
```

## 3. Edge Ingestion and Persistence
### Typescript Contract
```typescript
export type MessageType = 'telemetry' | 'heartbeat' | 'command' | 'ack';

export interface TelemetryPacket {
    messageType: MessageType;
    nodeId: string;
    device: string;
    batteryMv?: number;
    seq: number;
    data: Record<string, number | boolean>;
}
```

### SQLite Schema

```prisma
model Device {
    id              String          @id // Hardware MAC address
    name            String?
    name            String
    devicetype      String
    isActuator      Boolean         @default(false)
    lastSeen        DateTime        @default(now())
    readings        Telemetry[]
}

model Telemetry {
    id          Int             @id @default(autoincrement())
    nodeId      String
    device      Device          @relation(fields: [nodeId], references: [id])
    timestamp   DateTime        @default(now())
    batteryMv   Int?
    metric      String          // ie: "moisture", "temperature"
    value       Float

    @@index([nodeId, timestamp])
    @@index([metric, timestamp])
}

model ActuatorState {
    deviceId        String      @id
    device          Device      @relation(fields: [deviceId], references: [id])
    targetState     Float       // ie: 1.0 (ON) or 90.0 (degrees open)
    actualState     Float       // last confirmed position/state reported by hardware
    isPending       Boolean     @default(false) // true if command sent but not yet confirmed
    updatedAt       DateTime    @updatedAt
}
```