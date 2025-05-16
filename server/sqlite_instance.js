import sqlite3 from 'sqlite3';
import { open } from 'sqlite';

export const db_promise = open({
  filename: './TableTennis.db',
  driver: sqlite3.Database
});
