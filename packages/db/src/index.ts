import Database from 'better-sqlite3';
import { drizzle } from 'drizzle-orm/better-sqlite3'
import * as schema from './schema';
import path from 'path';

const dbPath = process.env.DB_PATH || path.resolve(__dirname, '../greenhouse.db');

const sqlite = new Database(dbPath);

sqlite.pragma('journal_mode = WAL');
sqlite.pragma('busy_timeout = 5000');
sqlite.pragma('synchronous = NORMAL');

export const db = drizzle(sqlite, { schema });
export * from './schema';
export { sqlite };