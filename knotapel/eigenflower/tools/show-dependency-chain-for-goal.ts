#!/usr/bin/env bun
/**
 * Walks the depends_on chain backward from a goal to show what must happen first.
 * Shows the full dependency tree with depth and status.
 * Usage: bun tools/show-dependency-chain-for-goal.ts <GOAL_ID>
 * Example: bun tools/show-dependency-chain-for-goal.ts G0014
 */
import { readAllRecords, getDbPath, getId } from './parse-frontmatter';

const goalId = process.argv[2];
if (!goalId) {
  console.error('Usage: bun tools/show-dependency-chain-for-goal.ts <GOAL_ID>');
  process.exit(1);
}

const records = await readAllRecords(getDbPath());
const byId = new Map(records.map(r => [getId(r), r]));

const target = byId.get(goalId);
if (!target) {
  console.error(`Record ${goalId} not found`);
  process.exit(1);
}

console.log(`DEPENDENCY CHAIN FOR: ${goalId} — ${target.fields.title}`);
console.log(`Status: ${target.fields.status || 'unknown'}`);
console.log('─'.repeat(80));

const visited = new Set<string>();
let maxDepth = 0;

function printChain(id: string, depth: number): void {
  if (visited.has(id)) {
    console.log(`${'  '.repeat(depth)}↻ ${id} (circular — already visited)`);
    return;
  }
  visited.add(id);
  if (depth > maxDepth) maxDepth = depth;

  const record = byId.get(id);
  if (!record) {
    console.log(`${'  '.repeat(depth)}? ${id} (not found)`);
    return;
  }

  const status = record.fields.status ? `[${record.fields.status}]` : '';
  const title = String(record.fields.title || '(no title)');
  const marker = String(record.fields.status) === 'achieved' ? '✓' :
                 String(record.fields.status) === 'in_progress' ? '►' : '○';

  console.log(`${'  '.repeat(depth)}${marker} ${id} — ${title} ${status}`);

  const deps = record.fields.depends_on;
  if (Array.isArray(deps)) {
    for (const dep of deps) {
      printChain(String(dep), depth + 1);
    }
  }
}

printChain(goalId, 0);

console.log('');
console.log(`Chain depth: ${maxDepth}`);
console.log(`Total nodes: ${visited.size}`);

// Find unresolved leaf dependencies
const unresolved: string[] = [];
for (const id of visited) {
  const r = byId.get(id);
  if (r && String(r.fields.status) !== 'achieved' && String(r.fields.status) !== 'in_progress') {
    const deps = r.fields.depends_on;
    if (!Array.isArray(deps) || deps.length === 0) {
      unresolved.push(id);
    }
  }
}

if (unresolved.length > 0) {
  console.log(`\nREADY TO START (leaf nodes, no dependencies):`);
  for (const id of unresolved) {
    const r = byId.get(id);
    console.log(`  ${id} — ${r?.fields.title || '(no title)'}`);
  }
}
