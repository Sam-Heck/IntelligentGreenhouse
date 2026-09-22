import { defineConfig } from 'drizzle-kit'
import path from 'path';

export default defineConfig({
    schema: './src/schema.ts',
    out: './drizzle',
    dialect: 'sqlite',
    dbCredentials: {
        url: process.env.DB_PATH || path.resolve(__dirname, 'greenhouse.db'),
    },
});