export interface SensorDefinition {
    metrics: readonly string[];
    units?: readonly string[];
}

export const SENSOR_REGISTRY: Record<string, SensorDefinition> = {
    gateway_sensors: {
        metrics: ['inTemp', 'inHumidity', 'outTemp', 'outHumidity'],
        units: ['°C', '%', '°C', '%']
    },
    solar_probe: {
        metrics: ['lux', 'ppfdEstimated'],
        units: ['lux', 'µmol/m²/s']
    },
    soil_probe: {
        metrics: ['moisture', 'tempSoil'],
        units: ['raw_capacitive', '°C']
    }
    // to add more as various remote sensors are added
    // still need to add actuator sensors for heater, fan, and window servo.
} as const