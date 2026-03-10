/**
 * Shared module for parsing YAML frontmatter from markdown files.
 * Zero dependencies — just string parsing for our flat frontmatter format.
 *
 * Handles: simple strings, enums, numbers, and lists (both inline [a, b] and multi-line - a\n- b)
 */

export interface Record {
  /** Full path to the file */
  filepath: string;
  /** Parsed frontmatter key-value pairs */
  fields: { [key: string]: string | string[] | number };
  /** The markdown body after frontmatter */
  body: string;
  /** The table/folder name (e.g., "goals", "papers") */
  table: string;
  /** The filename without path */
  filename: string;
}

function parseValue(raw: string): string | number {
  const trimmed = raw.trim();
  // Try number
  if (/^-?\d+(\.\d+)?$/.test(trimmed)) {
    return Number(trimmed);
  }
  // Strip quotes if present
  if ((trimmed.startsWith('"') && trimmed.endsWith('"')) ||
      (trimmed.startsWith("'") && trimmed.endsWith("'"))) {
    return trimmed.slice(1, -1);
  }
  return trimmed;
}

function parseInlineList(raw: string): string[] {
  // Handle [a, b, c] format
  const inner = raw.trim().slice(1, -1).trim();
  if (inner === '') return [];
  return inner.split(',').map(s => {
    const t = s.trim();
    if ((t.startsWith('"') && t.endsWith('"')) ||
        (t.startsWith("'") && t.endsWith("'"))) {
      return t.slice(1, -1);
    }
    return t;
  });
}

export function parseFrontmatter(content: string, filepath: string): Record | null {
  const lines = content.split('\n');

  // Find frontmatter delimiters
  if (lines[0]?.trim() !== '---') return null;

  let endIndex = -1;
  for (let i = 1; i < lines.length; i++) {
    if (lines[i]?.trim() === '---') {
      endIndex = i;
      break;
    }
  }
  if (endIndex === -1) return null;

  const fmLines = lines.slice(1, endIndex);
  const body = lines.slice(endIndex + 1).join('\n').trim();

  const fields: { [key: string]: string | string[] | number } = {};

  let i = 0;
  while (i < fmLines.length) {
    const line = fmLines[i];

    // Skip empty lines
    if (!line || line.trim() === '') {
      i++;
      continue;
    }

    const colonIndex = line.indexOf(':');
    if (colonIndex === -1) {
      i++;
      continue;
    }

    // Check if this is a list continuation (starts with -)
    if (line.trimStart().startsWith('-') && !line.includes(':')) {
      i++;
      continue;
    }

    const key = line.slice(0, colonIndex).trim();
    const valueRaw = line.slice(colonIndex + 1).trim();

    if (valueRaw === '' || valueRaw === '[]') {
      // Could be multi-line list or empty
      // Check if next lines are list items
      const listItems: string[] = [];
      let j = i + 1;
      while (j < fmLines.length) {
        const nextLine = fmLines[j];
        if (nextLine && nextLine.trimStart().startsWith('- ')) {
          const item = nextLine.trimStart().slice(2).trim();
          // Strip quotes
          if ((item.startsWith('"') && item.endsWith('"')) ||
              (item.startsWith("'") && item.endsWith("'"))) {
            listItems.push(item.slice(1, -1));
          } else {
            listItems.push(item);
          }
          j++;
        } else {
          break;
        }
      }
      if (listItems.length > 0) {
        fields[key] = listItems;
        i = j;
      } else {
        fields[key] = valueRaw === '[]' ? [] : '';
        i++;
      }
    } else if (valueRaw.startsWith('[')) {
      // Inline list
      fields[key] = parseInlineList(valueRaw);
      i++;
    } else {
      // Simple value
      const parsed = parseValue(valueRaw);
      fields[key] = parsed;
      i++;
    }
  }

  const parts = filepath.split('/');
  const filename = parts[parts.length - 1] || '';
  const table = parts[parts.length - 2] || '';

  return { filepath, fields, body, table, filename };
}

export async function readAllRecords(dbPath: string): Promise<Record[]> {
  const { readdirSync, readFileSync, statSync } = await import('fs');
  const { join } = await import('path');

  const records: Record[] = [];
  const tables = readdirSync(dbPath);

  for (const table of tables) {
    const tablePath = join(dbPath, table);
    if (!statSync(tablePath).isDirectory()) continue;

    const files = readdirSync(tablePath);
    for (const file of files) {
      if (!file.endsWith('.md') || file === 'schema.md') continue;
      const filepath = join(tablePath, file);
      const content = readFileSync(filepath, 'utf-8');
      const record = parseFrontmatter(content, filepath);
      if (record) {
        records.push(record);
      }
    }
  }

  return records;
}

export function getDbPath(): string {
  // Resolve relative to this script's location
  const scriptDir = import.meta.dir;
  return scriptDir.replace(/\/tools$/, '/db');
}

export function getId(record: Record): string {
  return String(record.fields.id || record.filename.replace('.md', ''));
}

export function getRelated(record: Record): string[] {
  const r = record.fields.related;
  if (Array.isArray(r)) return r as string[];
  return [];
}
