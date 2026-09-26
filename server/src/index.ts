import { SerialPort, ReadlineParser } from 'serialport'
import * as C from './config'

const port = new SerialPort({ path: C.SERIAL_PORT, baudRate: C.SERIAL_BAUD_RATE })

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }))

parser.on('data', (data) => {
    try {
        const raw = JSON.parse(data);
        if (raw.version !== C.PROTOCOL_VERSION) {
            console.warn(`Version mismatch: expected v${C.PROTOCOL_VERSION}, got v${raw.version}`);
            return;
        }

        const payload = raw as C.EspPayload;
        console.log(payload);
    } catch (err) {
        console.error('Failed to parse serial line:', data, err);
    }

    // send to front end via websocket
    // If message is 'telemetry': persist to db asynchronously (error handling) and update device table `last_seen` and `last_battery_mv` fields.
});