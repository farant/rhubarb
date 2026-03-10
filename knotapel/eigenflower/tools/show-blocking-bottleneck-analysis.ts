#!/usr/bin/env bun
/**
 * Finds records that appear most often in other records' depends_on, blocks,
 * or impacts fields. These are the bottlenecks — resolving them unblocks the most.
 * Usage: bun tools/show-blocking-bottleneck-analysis.ts
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const records = await readAllRecords(getDbPath());
const blockCount = new Map<string, { count: number; blockedBy: string[] }>();

// Count how many times each ID appears in dependency-related fields
const depFields = ['depends_on', 'blocks', 'impacts', 'blocked_by'];

for (const r of records) {
  for (const field of depFields) {
    const value = r.fields[field];
    if (Array.isArray(value)) {
      for (const dep of value) {
        const depId = String(dep);
        if (!blockCount.has(depId)) {
          blockCount.set(depId, { count: 0, blockedBy: [] });
        }
        const entry = blockCount.get(depId)!;
        entry.count++;
        entry.blockedBy.push(getId(r));
      }
    }
  }
}

// Sort by count descending
const sorted = [...blockCount.entries()]
  .sort((a, b) => b[1].count - a[1].count)
  .filter(([_, v]) => v.count > 0);

if (sorted.length === 0) {
  console.log('No dependency relationships found. Run cross-reference pass (TASK0002) first.');
  process.exit(0);
}

// Lookup records for titles
const byId = new Map(records.map(r => [getId(r), r]));

console.log('BOTTLENECK ANALYSIS — records that block the most other records');
console.log('─'.repeat(80));

for (const [id, info] of sorted.slice(0, 30)) {
  const record = byId.get(id);
  const title = record ? String(record.fields.title || '(no title)') : '(record not found)';
  const status = record?.fields.status ? `[${record.fields.status}]` : '';
  console.log(`\n${id} — ${title} ${status}`);
  console.log(`  Blocks ${info.count} record(s): ${info.blockedBy.join(', ')}`);
}
