#!/usr/bin/env bun
/**
 * Shows all records that are open/untested/proposed AND have no unresolved dependencies.
 * These are things that can be worked on RIGHT NOW.
 * Usage: bun tools/show-ready-queue-of-actionable-items.ts
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const records = await readAllRecords(getDbPath());
const byId = new Map(records.map(r => [getId(r), r]));

const actionableStatuses = new Set(['open', 'untested', 'proposed']);
const resolvedStatuses = new Set(['achieved', 'answered', 'confirmed', 'refuted', 'validated', 'killed', 'resolved', 'superseded']);

function depsResolved(record: typeof records[0]): boolean {
  const deps = record.fields.depends_on;
  if (!Array.isArray(deps) || deps.length === 0) return true;

  for (const dep of deps) {
    const depRecord = byId.get(String(dep));
    if (!depRecord) return true; // If dep doesn't exist, don't block on it
    const depStatus = String(depRecord.fields.status || '');
    if (!resolvedStatuses.has(depStatus)) return false;
  }
  return true;
}

const ready = records
  .filter(r => {
    const status = String(r.fields.status || '');
    return actionableStatuses.has(status) && depsResolved(r);
  })
  .sort((a, b) => {
    const tc = a.table.localeCompare(b.table);
    if (tc !== 0) return tc;
    return getId(a).localeCompare(getId(b));
  });

console.log(`READY QUEUE — actionable items with no unresolved dependencies (${ready.length} items)`);
console.log('─'.repeat(80));

let currentTable = '';
for (const r of ready) {
  if (r.table !== currentTable) {
    currentTable = r.table;
    console.log(`\n  ${currentTable.toUpperCase()}`);
  }
  const id = getId(r);
  const title = String(r.fields.title || '(no title)');
  const status = `[${r.fields.status}]`;
  console.log(`  ${id.padEnd(12)} ${title} ${status}`);
}
