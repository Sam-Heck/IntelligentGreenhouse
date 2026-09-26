CREATE TABLE `actuator_states` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`device_id` text NOT NULL,
	`target_value` real NOT NULL,
	`actual_value` real,
	`last_command` integer NOT NULL,
	`updated_at` integer NOT NULL,
	FOREIGN KEY (`device_id`) REFERENCES `devices`(`id`) ON UPDATE no action ON DELETE cascade
);
--> statement-breakpoint
CREATE TABLE `devices` (
	`id` text PRIMARY KEY NOT NULL,
	`name` text,
	`is_actuator` integer DEFAULT false NOT NULL,
	`last_seen` integer NOT NULL,
	`last_battery_mv` integer
);
--> statement-breakpoint
CREATE TABLE `sensor_channels` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`device_id` text NOT NULL,
	`metric` text NOT NULL,
	`channel_index` integer DEFAULT 0 NOT NULL,
	`custom_name` text,
	FOREIGN KEY (`device_id`) REFERENCES `devices`(`id`) ON UPDATE no action ON DELETE cascade
);
--> statement-breakpoint
CREATE INDEX `idx_channel_unique` ON `sensor_channels` (`device_id`,`metric`,`channel_index`);--> statement-breakpoint
CREATE TABLE `telemetry` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`device_id` text NOT NULL,
	`metric` text NOT NULL,
	`channel_index` integer DEFAULT 0 NOT NULL,
	`value` real NOT NULL,
	`battery_mv` integer,
	`timestamp` integer NOT NULL,
	FOREIGN KEY (`device_id`) REFERENCES `devices`(`id`) ON UPDATE no action ON DELETE cascade
);
--> statement-breakpoint
CREATE INDEX `idx_telemetry_node_metric_time` ON `telemetry` (`device_id`,`metric`,`timestamp`);--> statement-breakpoint
CREATE INDEX `idx_telemetry_timestamp` ON `telemetry` (`timestamp`);