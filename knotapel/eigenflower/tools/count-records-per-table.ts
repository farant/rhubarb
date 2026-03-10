#!/usr/bin/env bun
/**
 * Shows how many records exist in each table of the database.
 * Usage: bun tools/count-records-per-table.ts
 */
import { readAllRecords, getDbPath } from './parse-frontmatter';

const records = await readAllRecords(getDbPath());
const counts: { [table: string]: number } = {};

for (const r of records) {
  counts[r.table] = (counts[r.table] || 0) + 1;
}

const sorted = Object.entries(counts).sort((a, b) => b[1] - a[1]);
let total = 0;

console.log('TABLE                      COUNT');
console.log('─'.repeat(40));
for (const [table, count] of sorted) {
  console.log(`${table.padEnd(27)}${String(count).padStart(5)}`);
  total += count;
}
console.log('─'.repeat(40));
console.log(`${'TOTAL'.padEnd(27)}${String(total).padStart(5)}`);
