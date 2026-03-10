#!/usr/bin/env bun
/**
 * Shows all records that have a given status value.
 * Usage: bun tools/show-records-with-status.ts <status>
 * Example: bun tools/show-records-with-status.ts open
 * Example: bun tools/show-records-with-status.ts untested
 * Example: bun tools/show-records-with-status.ts proposed
 *
 * Optionally filter by table:
 * Example: bun tools/show-records-with-status.ts open goals
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const targetStatus = process.argv[2];
const filterTable = process.argv[3];

if (!targetStatus) {
  console.error('Usage: bun tools/show-records-with-status.ts <status> [table]');
  process.exit(1);
}

const records = await readAllRecords(getDbPath());
const matching = records
  .filter(r => String(r.fields.status) === targetStatus)
  .filter(r => !filterTable || r.table === filterTable)
  .sort((a, b) => {
    const tableCompare = a.table.localeCompare(b.table);
    if (tableCompare !== 0) return tableCompare;
    return getId(a).localeCompare(getId(b));
  });

if (matching.length === 0) {
  console.log(`No records with status "${targetStatus}"${filterTable ? ` in ${filterTable}` : ''}`);
  process.exit(0);
}

console.log(`STATUS: "${targetStatus}" (${matching.length} records)${filterTable ? ` in ${filterTable}` : ''}`);
console.log('─'.repeat(80));

let currentTable = '';
for (const r of matching) {
  if (r.table !== currentTable) {
    currentTable = r.table;
    console.log(`\n  ${currentTable.toUpperCase()}`);
  }
  const id = getId(r);
  const title = String(r.fields.title || '(no title)');
  console.log(`  ${id.padEnd(12)} ${title}`);
}
