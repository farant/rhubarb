#!/usr/bin/env bun
/**
 * Shows full detail for a single record: all frontmatter fields and body.
 * Usage: bun tools/show-record-detail-by-id.ts <ID>
 * Example: bun tools/show-record-detail-by-id.ts G0001
 * Example: bun tools/show-record-detail-by-id.ts PT0019
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const targetId = process.argv[2];
if (!targetId) {
  console.error('Usage: bun tools/show-record-detail-by-id.ts <ID>');
  process.exit(1);
}

const records = await readAllRecords(getDbPath());
const record = records.find(r => getId(r) === targetId);

if (!record) {
  console.error(`Record ${targetId} not found`);

  // Suggest close matches
  const close = records
    .filter(r => getId(r).startsWith(targetId.slice(0, targetId.length - 1)))
    .map(r => getId(r));
  if (close.length > 0) {
    console.error(`Did you mean: ${close.join(', ')}?`);
  }
  process.exit(1);
}

console.log(`═══ ${getId(record)} ═══`);
console.log(`Table: ${record.table}`);
console.log(`File:  ${record.filename}`);
console.log('─'.repeat(60));
console.log('FRONTMATTER:');

for (const [key, value] of Object.entries(record.fields)) {
  if (Array.isArray(value)) {
    if (value.length === 0) {
      console.log(`  ${key}: []`);
    } else {
      console.log(`  ${key}:`);
      for (const item of value) {
        console.log(`    - ${item}`);
      }
    }
  } else {
    console.log(`  ${key}: ${value}`);
  }
}

console.log('─'.repeat(60));
console.log('BODY:');
console.log(record.body);
