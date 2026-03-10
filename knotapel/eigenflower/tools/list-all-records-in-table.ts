#!/usr/bin/env bun
/**
 * Lists all records in a given table with their ID, title, and status.
 * Usage: bun tools/list-all-records-in-table.ts <table_name>
 * Example: bun tools/list-all-records-in-table.ts goals
 * Example: bun tools/list-all-records-in-table.ts potential_techniques
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const tableName = process.argv[2];
if (!tableName) {
  console.error('Usage: bun tools/list-all-records-in-table.ts <table_name>');
  console.error('Available tables:');
  const { readdirSync, statSync } = await import('fs');
  const { join } = await import('path');
  const dbPath = getDbPath();
  for (const t of readdirSync(dbPath).sort()) {
    if (statSync(join(dbPath, t)).isDirectory()) console.error(`  ${t}`);
  }
  process.exit(1);
}

const records = await readAllRecords(getDbPath());
const tableRecords = records
  .filter(r => r.table === tableName)
  .sort((a, b) => getId(a).localeCompare(getId(b)));

if (tableRecords.length === 0) {
  console.log(`No records found in table "${tableName}"`);
  process.exit(0);
}

console.log(`${tableName.toUpperCase()} (${tableRecords.length} records)`);
console.log('─'.repeat(80));

for (const r of tableRecords) {
  const id = getId(r);
  const title = String(r.fields.title || '(no title)');
  const status = r.fields.status ? ` [${r.fields.status}]` : '';
  console.log(`${id.padEnd(12)} ${title}${status}`);
}
