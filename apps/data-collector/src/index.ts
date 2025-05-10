import { PrismaClient } from "@prisma/client";
import dotenv from "dotenv";
import mqtt from "mqtt";

dotenv.config();
const prisma = new PrismaClient();

const client = mqtt.connect(
    `mqtt://${process.env.MQTT_HOST}:${process.env.MQTT_PORT}`
);

interface SensorPayload {
    [sensorId: string]: {
        temp: number;
        humidity: number;
    };
}

client.on("connect", () => {
    console.log("Connected to MQTT");
    client.subscribe("greenhouse/+/sensors");
});

client.on("message", async (topic: string, messageBuffer: Buffer) => {
    try {
        const [, greenhouseIdRaw] = topic.split("/");
        const greenhouseId = greenhouseIdRaw.trim().toLocaleLowerCase();
        const data = JSON.parse(messageBuffer.toString()) as SensorPayload;

        for (const [sensorId, { temp, humidity }] of Object.entries(data)) {
            await prisma.greenhouse.upsert({
                where: { id: greenhouseId },
                update: {},
                create: { id: greenhouseId },
            });

            await prisma.sensorReading.create({
                data: {
                    greenhouseId,
                    sensorId: parseInt(sensorId, 10),
                    temperatureF: temp,
                    humidity,
                },
            });

            console.log(
                `Saved sensor ${sensorId} from greenhouse ${greenhouseId}: ${temp}°F, ${humidity}%`
            );
        }
    } catch (err) {
        console.error("Error processing message:", err);
    }
});
