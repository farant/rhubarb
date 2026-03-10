#!/usr/bin/env bun
/**
 * Searches all records for a text string in title or body (case-insensitive).
 * Usage: bun tools/search-records-by-text-in-title-or-body.ts <search_term>
 * Example: bun tools/search-records-by-text-in-title-or-body.ts "tensor logic"
 * Example: bun tools/search-records-by-text-in-title-or-body.ts syllogism
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const searchTerm = process.argv[2];
if (!searchTerm) {
  console.error('Usage: bun tools/search-records-by-text-in-title-or-body.ts <search_term>');
  process.exit(1);
}

const records = await readAllRecords(getDbPath());
const termLower = searchTerm.toLowerCase();

const matches = records
  .filter(r => {
    const title = String(r.fields.title || '').toLowerCase();
    const bodyLower = r.body.toLowerCase();
    return title.includes(termLower) || bodyLower.includes(termLower);
  })
  .sort((a, b) => {
    // Title matches first
    const aTitle = String(a.fields.title || '').toLowerCase().includes(termLower) ? 0 : 1;
    const bTitle = String(b.fields.title || '').toLowerCase().includes(termLower) ? 0 : 1;
    if (aTitle !== bTitle) return aTitle - bTitle;
    return a.table.localeCompare(b.table) || getId(a).localeCompare(getId(b));
  });

console.log(`SEARCH: "${searchTerm}" (${matches.length} matches)`);
console.log('─'.repeat(80));

let currentTable = '';
for (const r of matches) {
  if (r.table !== currentTable) {
    currentTable = r.table;
    console.log(`\n  ${currentTable.toUpperCase()}`);
  }
  const id = getId(r);
  const title = String(r.fields.title || '(no title)');
  const inTitle = String(r.fields.title || '').toLowerCase().includes(termLower) ? ' ← title match' : '';
  console.log(`  ${id.padEnd(12)} ${title}${inTitle}`);
}
