import fs from 'fs'
import path from 'path'
import { fileURLToPath } from 'node:url';
import dotenv from 'dotenv'

const protocolPath = path.resolve(import.meta.dirname, "../../shared/protocol.h");
const headerContent = fs.readFileSync(protocolPath, 'utf-8');

function extractDefine(name: string): number {
    const line = headerContent.split('\n').find(l => l.includes(`#define ${name}`))

    if (!line) {
        throw new Error(`Missing '#define ${name}' in protocol.h`);
    }

    const elArray = line.trim().split(' ');
    return parseInt(elArray[2]);
}

export const PROTOCOL_VERSION = extractDefine('PROTOCOL_VERSION');
export const SERIAL_BAUD_RATE = extractDefine('SERIAL_BAUD_RATE');

const envPath = fileURLToPath(new URL('../../.env', import.meta.url))
dotenv.config({ path: envPath })
export const SERIAL_PORT = process.env.SERIAL_PORT

if (!SERIAL_PORT) {
    console.log("SERIAL_PORT environment variable not found. Please set and try again")
    process.exit(1);
}

export enum MessageType {
  MSG_TELEMETRY = 1,
  MSG_HEARTBEAT = 2,
  MSG_COMMAND = 3,
  MSG_ACK = 4
}

enum SensorType {
    TEMP_C = 1,
    HUMIDITY = 2,
    PRESSURE_PA = 3,
    SOIL_MOIST = 4,
    LIGHT_LUX = 5,
    RELAY_STATE = 6,
    SERVO_STATE = 7
};

export interface Reading {
    type: SensorType;
    index: number;
    value: number;
}

export interface EspPayload {
    version: number;
    msgType: MessageType;
    deviceId: string;
    batteryMv: number;
    sequenceId: number;
    readingCount: number;
    readings: Reading[];
}

export interface MetricMeta {
    key: string;
    label: string;
    unit: string;
}

export const SENSOR_METRICS: Record<SensorType, MetricMeta> = {
    [SensorType.TEMP_C]: {key: 'temperature', label: 'Temperature', unit: '°C'},
    [SensorType.HUMIDITY]: {key: 'humidity', label: 'Humidity', unit: '%'},
    [SensorType.PRESSURE_PA]: {key: 'pressure', label: 'Pressure', unit: 'Pa'},
    [SensorType.SOIL_MOIST]: {key: 'soi_moisture', label: 'Soil Moisture', unit: '%'},
    [SensorType.LIGHT_LUX]: {key: 'lux', label: 'Light', unit: 'lux'},
    [SensorType.RELAY_STATE]: {key: 'relay_state', label: 'Relay', unit: 'state'},
    [SensorType.SERVO_STATE]: {key: 'servo_state', label: 'Servo', unit: 'deg'}
}