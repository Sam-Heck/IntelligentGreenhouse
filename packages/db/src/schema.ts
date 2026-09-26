import { sqliteTable, text, integer, real, index } from 'drizzle-orm/sqlite-core'
import { relations } from 'drizzle-orm'

export const devices = sqliteTable("devices", {
    id: text('id').primaryKey(),
    name: text('name'),
    isActuator: integer('is_actuator', { mode: 'boolean' }).notNull().default(false),
    lastSeen: integer('last_seen', { mode: 'timestamp' }).notNull().$defaultFn(() => new Date()),
    lastBatteryMv: integer('last_battery_mv'),
});

export const sensorChannels = sqliteTable('sensor_channels', {
    id: integer('id').primaryKey({ autoIncrement: true }),
    deviceId: text('device_id').notNull().references(() => devices.id, { onDelete: 'cascade' }),
    metric: text('metric').notNull(),
    channel_index: integer('channel_index').notNull().default(0),
    customName: text('custom_name'),
}, (table) => ([
    index('idx_channel_unique').on(table.deviceId, table.metric, table.channel_index),
]));

export const telemetry = sqliteTable('telemetry', {
    id: integer('id').primaryKey({ autoIncrement: true }),
    deviceId: text('device_id').notNull().references(() => devices.id, { onDelete: 'cascade' }),
    metric: text('metric').notNull(),
    channelIndex: integer('channel_index').notNull().default(0),
    value: real('value').notNull(),
    batteryMv: integer('battery_mv'),
    timestamp: integer('timestamp', { mode: 'timestamp' }).notNull().$defaultFn(() => new Date()),
}, (table) => ([
    index('idx_telemetry_node_metric_time').on(table.deviceId, table.metric, table.timestamp),
    index('idx_telemetry_timestamp').on(table.timestamp),
]));

export const actuatorStates = sqliteTable('actuator_states', {
    id: integer('id').primaryKey({ autoIncrement: true }),
    deviceId: text('device_id').notNull().references(() => devices.id, { onDelete: 'cascade'}),
    targetValue: real('target_value').notNull(),
    actualValue: real('actual_value'),
    lastCommand: integer('last_command', { mode: 'timestamp' }).notNull().$defaultFn(() => new Date()),
    updatedAt: integer('updated_at', { mode: 'timestamp' }).notNull().$defaultFn(() => new Date()),
});

export const devicesRelations = relations(devices, ({ many, one }) => ({
    telemetry: many(telemetry),
    actuatorStates: one(actuatorStates, {
        fields: [devices.id],
        references: [actuatorStates.deviceId],
    }),
}));

export const sensorChannelsRelations = relations(sensorChannels, ({ one }) => ({
    device: one(devices, {
        fields: [sensorChannels.deviceId],
        references: [devices.id],
    }),
}));

export const telemetryRelations = relations(telemetry, ({ one }) => ({
    device: one(devices, {
        fields: [telemetry.deviceId],
        references: [devices.id],
    }),
}));

export const actuatorStatesRelations = relations(actuatorStates, ({ one }) => ({
    device: one(devices, {
        fields: [actuatorStates.deviceId],
        references: [devices.id],
    }),
}));

export type Device = typeof devices.$inferSelect;
export type NewDevice = typeof devices.$inferInsert;
export type SensorChannel = typeof sensorChannels.$inferSelect;
export type NewSensorChannel = typeof sensorChannels.$inferInsert;
export type Telemetry = typeof telemetry.$inferSelect;
export type NewTelemetry = typeof telemetry.$inferInsert;