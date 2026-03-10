#!/usr/bin/env bun
/**
 * Finds records that no other record references — potential forgotten items.
 * Searches all frontmatter fields and body text for each record's ID.
 * Usage: bun tools/find-orphan-records-with-no-inbound-references.ts
 * Optional: bun tools/find-orphan-records-with-no-inbound-references.ts <table>
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const filterTable = process.argv[2];
const records = await readAllRecords(getDbPath());

// Build a set of all IDs
const allIds = new Map<string, typeof records[0]>();
for (const r of records) {
  allIds.set(getId(r), r);
}

// For each ID, check if any OTHER record mentions it
const orphans: typeof records[0][] = [];

for (const [id, record] of allIds) {
  if (filterTable && record.table !== filterTable) continue;

  let referenced = false;
  for (const other of records) {
    if (getId(other) === id) continue;

    // Check frontmatter
    for (const value of Object.values(other.fields)) {
      if (Array.isArray(value)) {
        if (value.some(v => String(v) === id)) { referenced = true; break; }
      } else if (String(value) === id) { referenced = true; break; }
    }
    if (referenced) break;

    // Check body
    if (other.body.includes(id)) { referenced = true; break; }
  }

  if (!referenced) {
    orphans.push(record);
  }
}

// Sort by table then ID
orphans.sort((a, b) => {
  const tc = a.table.localeCompare(b.table);
  if (tc !== 0) return tc;
  return getId(a).localeCompare(getId(b));
});

console.log(`ORPHAN RECORDS (${orphans.length} records with no inbound references)${filterTable ? ` in ${filterTable}` : ''}`);
console.log('─'.repeat(80));

let currentTable = '';
for (const r of orphans) {
  if (r.table !== currentTable) {
    currentTable = r.table;
    console.log(`\n  ${currentTable.toUpperCase()}`);
  }
  const id = getId(r);
  const title = String(r.fields.title || '(no title)');
  const status = r.fields.status ? ` [${r.fields.status}]` : '';
  console.log(`  ${id.padEnd(12)} ${title}${status}`);
}
