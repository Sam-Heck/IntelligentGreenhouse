import { SerialPort } from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline'
import path from 'node:path'
import { fileURLToPath } from 'node:url';
import dotenv from 'dotenv'

const envPath = fileURLToPath(new URL('../../.env', import.meta.url))
dotenv.config({ path: path.resolve(__dirname, '../../.env') });
const DEFAULT_BAUD = 115200
const SERIAL_BAUD_RATE = Number(process.env.SERIAL_BAUD_RATE);

SerialPort.list().then(ports => {
    console.log(ports);
});

const port = new SerialPort({ path: '/dev/ttyUSB0', baudRate: SERIAL_BAUD_RATE })

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }))

parser.on('data', console.log);