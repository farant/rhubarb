#!/usr/bin/env bun
/**
 * Finds every record in the database that references a given ID.
 * Searches all frontmatter fields and body text.
 * Usage: bun tools/find-all-references-to-id.ts <ID>
 * Example: bun tools/find-all-references-to-id.ts G0001
 * Example: bun tools/find-all-references-to-id.ts P0010
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const targetId = process.argv[2];
if (!targetId) {
  console.error('Usage: bun tools/find-all-references-to-id.ts <ID>');
  process.exit(1);
}

const records = await readAllRecords(getDbPath());

// Find the target record itself
const target = records.find(r => getId(r) === targetId);
if (target) {
  console.log(`TARGET: ${targetId} — ${target.fields.title || '(no title)'} [${target.table}]`);
  console.log('');
}

// Find all records that reference this ID
const references: { record: typeof records[0]; where: string[] }[] = [];

for (const r of records) {
  if (getId(r) === targetId) continue;

  const locations: string[] = [];

  // Check all frontmatter fields
  for (const [key, value] of Object.entries(r.fields)) {
    if (Array.isArray(value)) {
      if (value.some(v => String(v).includes(targetId))) {
        locations.push(`frontmatter.${key}`);
      }
    } else if (String(value).includes(targetId)) {
      locations.push(`frontmatter.${key}`);
    }
  }

  // Check body text
  if (r.body.includes(targetId)) {
    locations.push('body');
  }

  if (locations.length > 0) {
    references.push({ record: r, where: locations });
  }
}

if (references.length === 0) {
  console.log(`No references to ${targetId} found in any other record.`);
} else {
  console.log(`REFERENCED BY (${references.length} records):`);
  console.log('─'.repeat(80));
  for (const ref of references.sort((a, b) => getId(a.record).localeCompare(getId(b.record)))) {
    const id = getId(ref.record);
    const title = String(ref.record.fields.title || '(no title)');
    const where = ref.where.join(', ');
    console.log(`${id.padEnd(12)} ${title}`);
    console.log(`${''.padEnd(12)} found in: ${where}`);
  }
}
