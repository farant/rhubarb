#!/usr/bin/env bun
/**
 * Web GUI for browsing the eigenflower database.
 * Usage: bun tools/server.ts [port]
 * Default port: 3000
 */
import { readAllRecords, getDbPath, getId, parseFrontmatter } from './parse-frontmatter';
import type { Record } from './parse-frontmatter';

const PORT = Number(process.argv[2]) || 3000;
const DB_PATH = getDbPath();

// Table display config: prefix, label, color
const TABLE_META: { [key: string]: { prefix: string; color: string } } = {
  goals:                { prefix: 'G',    color: '#4CAF50' },
  papers:               { prefix: 'P',    color: '#2196F3' },
  references:           { prefix: 'REF',  color: '#03A9F4' },
  questions:            { prefix: 'Q',    color: '#FF9800' },
  unknowns:             { prefix: 'UNK',  color: '#9C27B0' },
  complications:        { prefix: 'COMP', color: '#f44336' },
  hypotheses:           { prefix: 'HYP',  color: '#E91E63' },
  experiments:          { prefix: 'EXP',  color: '#009688' },
  decisions:            { prefix: 'DEC',  color: '#607D8B' },
  potential_techniques: { prefix: 'PT',   color: '#FF5722' },
  validated_techniques: { prefix: 'VT',   color: '#8BC34A' },
  results:              { prefix: 'RES',  color: '#00BCD4' },
  frames:               { prefix: 'FR',   color: '#795548' },
  modules:              { prefix: 'MOD',  color: '#3F51B5' },
  connections:          { prefix: 'CON',  color: '#CDDC39' },
  milestones:           { prefix: 'MILE', color: '#FFC107' },
  limitations:          { prefix: 'LIM',  color: '#9E9E9E' },
  tasks:                { prefix: 'TASK', color: '#673AB7' },
  workflows:            { prefix: 'WF',   color: '#00796B' },
  kits:                 { prefix: 'KIT',  color: '#26C6DA' },
};

function escapeHtml(s: string): string {
  return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
}

/** Basic markdown to HTML — handles headers, bold, italic, code, lists, paragraphs */
function markdownToHtml(md: string): string {
  const lines = md.split('\n');
  const out: string[] = [];
  let inList = false;
  let inCode = false;
  let inTable = false;
  let tableRowIndex = 0;
  let codeBlock: string[] = [];

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];

    // Fenced code blocks
    if (line.trimStart().startsWith('```')) {
      if (inCode) {
        out.push(`<pre class="md-code-block"><code>${escapeHtml(codeBlock.join('\n'))}</code></pre>`);
        codeBlock = [];
        inCode = false;
      } else {
        if (inList) { out.push('</ul>'); inList = false; }
        inCode = true;
      }
      continue;
    }
    if (inCode) {
      codeBlock.push(line);
      continue;
    }

    // Close list if non-list line
    if (inList && !line.match(/^\s*[-*]\s/)) {
      out.push('</ul>');
      inList = false;
    }

    // Close table if non-table line
    if (inTable && !line.match(/^\s*\|/)) {
      out.push('</tbody></table>');
      inTable = false;
      tableRowIndex = 0;
    }

    // Empty line
    if (line.trim() === '') {
      continue;
    }

    // Headers
    const hMatch = line.match(/^(#{1,6})\s+(.*)/);
    if (hMatch) {
      const level = hMatch[1].length;
      out.push(`<h${level}>${inlineMarkdown(hMatch[2])}</h${level}>`);
      continue;
    }

    // Table rows
    if (line.match(/^\s*\|/)) {
      const cells = line.split('|').slice(1, -1).map(c => c.trim());

      // Separator row (e.g. |---|---|)
      if (cells.every(c => /^[-:]+$/.test(c))) {
        i++;
        continue;
      }

      if (!inTable) {
        // First row = header
        out.push('<table class="md-table"><thead><tr>');
        for (const cell of cells) {
          out.push(`<th>${inlineMarkdown(cell)}</th>`);
        }
        out.push('</tr></thead><tbody>');
        inTable = true;
        tableRowIndex = 0;
      } else {
        out.push('<tr>');
        for (const cell of cells) {
          out.push(`<td>${inlineMarkdown(cell)}</td>`);
        }
        out.push('</tr>');
        tableRowIndex++;
      }
      continue;
    }

    // Unordered list items
    const liMatch = line.match(/^\s*[-*]\s+(.*)/);
    if (liMatch) {
      if (!inList) { out.push('<ul>'); inList = true; }
      out.push(`<li>${inlineMarkdown(liMatch[1])}</li>`);
      continue;
    }

    // Regular paragraph
    out.push(`<p>${inlineMarkdown(line)}</p>`);
  }

  if (inList) out.push('</ul>');
  if (inTable) out.push('</tbody></table>');
  if (inCode) out.push(`<pre class="md-code-block"><code>${escapeHtml(codeBlock.join('\n'))}</code></pre>`);

  return out.join('\n');
}

/** Inline markdown: bold, italic, code, links, then linkify IDs */
function inlineMarkdown(text: string): string {
  let s = escapeHtml(text);
  // Inline code (before bold/italic so backticks aren't mangled)
  s = s.replace(/`([^`]+)`/g, '<code class="md-inline-code">$1</code>');
  // Bold
  s = s.replace(/\*\*(.+?)\*\*/g, '<strong>$1</strong>');
  // Italic
  s = s.replace(/\*(.+?)\*/g, '<em>$1</em>');
  // Linkify record IDs
  s = s.replace(
    /\b(G|P|REF|Q|UNK|COMP|HYP|EXP|DEC|PT|VT|RES|FR|MOD|CON|MILE|LIM|TASK|WF|KIT)\d{4}\b/g,
    (match) => `<a href="/record/${match}" class="id-link">${match}</a>`
  );
  return s;
}

/** Turn ID references in text into clickable links */
function linkifyIds(text: string): string {
  return escapeHtml(text).replace(
    /\b(G|P|REF|Q|UNK|COMP|HYP|EXP|DEC|PT|VT|RES|FR|MOD|CON|MILE|LIM|TASK|WF|KIT)\d{4}\b/g,
    (match) => `<a href="/record/${match}" class="id-link">${match}</a>`
  );
}

function statusBadge(status: string | undefined): string {
  if (!status) return '';
  const colors: { [key: string]: string } = {
    open: '#4CAF50', proposed: '#FF9800', confirmed: '#8BC34A',
    refuted: '#f44336', tested: '#2196F3', complete: '#00BCD4',
    answered: '#009688', high: '#f44336', medium: '#FF9800', low: '#4CAF50',
  };
  const c = colors[String(status).toLowerCase()] || '#9E9E9E';
  return `<span class="badge" style="background:${c}">${escapeHtml(String(status))}</span>`;
}

function renderIndex(records: Record[]): string {
  // Group by table
  const byTable: { [key: string]: Record[] } = {};
  for (const r of records) {
    if (!byTable[r.table]) byTable[r.table] = [];
    byTable[r.table].push(r);
  }

  // Sort tables by count descending
  const tables = Object.keys(byTable).sort((a, b) => byTable[b].length - byTable[a].length);

  let tableCards = '';
  for (const table of tables) {
    const meta = TABLE_META[table] || { prefix: '?', color: '#999' };
    const count = byTable[table].length;
    tableCards += `
      <a href="/table/${table}" class="table-card" style="border-left: 4px solid ${meta.color}">
        <div class="table-prefix" style="color:${meta.color}">${meta.prefix}</div>
        <div class="table-name">${escapeHtml(table)}</div>
        <div class="table-count">${count} record${count !== 1 ? 's' : ''}</div>
      </a>`;
  }

  // Status summary
  const statusCounts: { [key: string]: number } = {};
  for (const r of records) {
    const s = String(r.fields.status || 'no status');
    statusCounts[s] = (statusCounts[s] || 0) + 1;
  }
  let statusSummary = '';
  for (const [s, c] of Object.entries(statusCounts).sort((a, b) => b[1] - a[1])) {
    statusSummary += `<a href="/status/${encodeURIComponent(s)}" class="status-pill">${statusBadge(s)} ${c}</a> `;
  }

  return page('Eigenflower Database', `
    <div class="header">
      <h1>Eigenflower Database</h1>
      <p class="subtitle">${records.length} records across ${tables.length} tables</p>
      <div class="search-bar">
        <input type="text" id="search" placeholder="Search records..." autofocus />
      </div>
    </div>
    <div class="status-row">${statusSummary}</div>
    <div class="table-grid">${tableCards}</div>
    <div id="search-results" class="record-list" style="display:none"></div>
    <script>
      const search = document.getElementById('search');
      const results = document.getElementById('search-results');
      const grid = document.querySelector('.table-grid');
      let debounce;
      search.addEventListener('input', () => {
        clearTimeout(debounce);
        debounce = setTimeout(async () => {
          const q = search.value.trim();
          if (q.length < 2) { results.style.display = 'none'; grid.style.display = ''; return; }
          const res = await fetch('/api/search?q=' + encodeURIComponent(q));
          const data = await res.json();
          grid.style.display = 'none';
          results.style.display = '';
          if (data.length === 0) {
            results.innerHTML = '<p class="empty">No results</p>';
            return;
          }
          results.innerHTML = data.map(r =>
            '<a href="/record/' + r.id + '" class="record-row">' +
            '<span class="row-id">' + r.id + '</span>' +
            '<span class="row-title">' + r.title + '</span>' +
            (r.status ? '<span class="row-status">' + r.status + '</span>' : '') +
            '</a>'
          ).join('');
        }, 200);
      });
    </script>
  `);
}

function renderTable(table: string, records: Record[]): string {
  const sorted = records.sort((a, b) => getId(a).localeCompare(getId(b)));
  const meta = TABLE_META[table] || { prefix: '?', color: '#999' };

  let rows = '';
  for (const r of sorted) {
    const id = getId(r);
    const title = String(r.fields.title || '(no title)');
    const status = r.fields.status ? String(r.fields.status) : '';
    const priority = r.fields.priority ? String(r.fields.priority) : '';
    rows += `
      <a href="/record/${id}" class="record-row">
        <span class="row-id" style="color:${meta.color}">${escapeHtml(id)}</span>
        <span class="row-title">${escapeHtml(title)}</span>
        ${status ? statusBadge(status) : ''}
        ${priority ? `<span class="priority">${escapeHtml(priority)}</span>` : ''}
      </a>`;
  }

  return page(`${table} — Eigenflower`, `
    <div class="header">
      <a href="/" class="back">&larr; All Tables</a>
      <h1 style="color:${meta.color}">${meta.prefix} — ${escapeHtml(table)}</h1>
      <p class="subtitle">${sorted.length} records</p>
    </div>
    <div class="record-list">${rows}</div>
  `);
}

function renderRecord(record: Record, allRecords: Record[]): string {
  const id = getId(record);
  const title = String(record.fields.title || id);
  const meta = TABLE_META[record.table] || { prefix: '?', color: '#999' };

  // Frontmatter fields
  let fields = '';
  for (const [key, value] of Object.entries(record.fields)) {
    if (key === 'id' || key === 'title') continue;
    let rendered: string;
    if (Array.isArray(value)) {
      if (value.length === 0) {
        rendered = '<span class="empty-val">[]</span>';
      } else {
        rendered = value.map(v => linkifyIds(String(v))).join(', ');
      }
    } else {
      rendered = linkifyIds(String(value));
    }
    fields += `<tr><td class="field-key">${escapeHtml(key)}</td><td>${rendered}</td></tr>`;
  }

  // Inbound references
  const inbound = allRecords.filter(r => {
    if (r === record) return false;
    const text = JSON.stringify(r.fields) + r.body;
    return text.includes(id);
  });
  let inboundHtml = '';
  if (inbound.length > 0) {
    inboundHtml = '<h3>Referenced by</h3><div class="ref-list">';
    for (const r of inbound) {
      const rid = getId(r);
      const rtitle = String(r.fields.title || rid);
      const rmeta = TABLE_META[r.table] || { prefix: '?', color: '#999' };
      inboundHtml += `<a href="/record/${rid}" class="ref-chip" style="border-color:${rmeta.color}">${escapeHtml(rid)}: ${escapeHtml(rtitle)}</a>`;
    }
    inboundHtml += '</div>';
  }

  const bodyHtml = markdownToHtml(record.body);

  return page(`${id} — ${title}`, `
    <div class="header">
      <a href="/table/${record.table}" class="back">&larr; ${escapeHtml(record.table)}</a>
      <h1><span style="color:${meta.color}">${escapeHtml(id)}</span> ${escapeHtml(title)}</h1>
    </div>
    <table class="fields-table">${fields}</table>
    ${inboundHtml}
    <div class="body-content">${bodyHtml}</div>
  `);
}

function renderStatus(status: string, records: Record[]): string {
  // Group by table
  const byTable: { [key: string]: Record[] } = {};
  for (const r of records) {
    if (!byTable[r.table]) byTable[r.table] = [];
    byTable[r.table].push(r);
  }

  let sections = '';
  for (const [table, recs] of Object.entries(byTable).sort((a, b) => b[1].length - a[1].length)) {
    const meta = TABLE_META[table] || { prefix: '?', color: '#999' };
    const sorted = recs.sort((a, b) => getId(a).localeCompare(getId(b)));
    let rows = '';
    for (const r of sorted) {
      const id = getId(r);
      const title = String(r.fields.title || '(no title)');
      rows += `
        <a href="/record/${id}" class="record-row">
          <span class="row-id" style="color:${meta.color}">${escapeHtml(id)}</span>
          <span class="row-title">${escapeHtml(title)}</span>
        </a>`;
    }
    sections += `
      <h3 style="margin-top:16px; color:${meta.color}">${meta.prefix} — ${escapeHtml(table)} (${recs.length})</h3>
      <div class="record-list">${rows}</div>`;
  }

  return page(`${status} — Eigenflower`, `
    <div class="header">
      <a href="/" class="back">&larr; All Tables</a>
      <h1>${statusBadge(status)} ${escapeHtml(status)}</h1>
      <p class="subtitle">${records.length} records</p>
    </div>
    ${sections}
  `);
}

function page(title: string, content: string): string {
  return `<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>${escapeHtml(title)}</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
      background: #0d1117; color: #c9d1d9; line-height: 1.6;
      max-width: 960px; margin: 0 auto; padding: 20px;
    }
    a { color: #58a6ff; text-decoration: none; }
    a:hover { text-decoration: underline; }
    .header { margin-bottom: 24px; }
    .back { font-size: 14px; display: inline-block; margin-bottom: 8px; }
    h1 { font-size: 24px; font-weight: 600; margin-bottom: 4px; }
    .subtitle { color: #8b949e; font-size: 14px; }
    .search-bar { margin-top: 16px; }
    .search-bar input {
      width: 100%; padding: 10px 14px; font-size: 16px;
      background: #161b22; border: 1px solid #30363d; border-radius: 6px;
      color: #c9d1d9; outline: none;
    }
    .search-bar input:focus { border-color: #58a6ff; }
    .status-row { margin-bottom: 16px; display: flex; flex-wrap: wrap; gap: 8px; }
    .status-pill { font-size: 13px; }
    .badge {
      display: inline-block; padding: 2px 8px; border-radius: 12px;
      font-size: 12px; color: #fff; font-weight: 500;
    }
    .table-grid {
      display: grid; grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
      gap: 12px;
    }
    .table-card {
      display: block; background: #161b22; border-radius: 6px; padding: 16px;
      border: 1px solid #30363d; transition: border-color 0.15s;
      text-decoration: none; color: inherit;
    }
    .table-card:hover { border-color: #58a6ff; text-decoration: none; }
    .table-prefix { font-size: 20px; font-weight: 700; font-family: monospace; }
    .table-name { font-size: 14px; color: #8b949e; margin-top: 4px; }
    .table-count { font-size: 13px; color: #8b949e; }
    .record-list { display: flex; flex-direction: column; gap: 2px; }
    .record-row {
      display: flex; align-items: center; gap: 12px; padding: 10px 14px;
      background: #161b22; border-radius: 4px; border: 1px solid #30363d;
      text-decoration: none; color: inherit; transition: border-color 0.15s;
    }
    .record-row:hover { border-color: #58a6ff; text-decoration: none; }
    .row-id { font-family: monospace; font-weight: 600; min-width: 80px; }
    .row-title { flex: 1; }
    .row-status { font-size: 12px; color: #8b949e; }
    .priority { font-size: 12px; color: #8b949e; font-style: italic; }
    .empty { color: #8b949e; padding: 20px; text-align: center; }
    .fields-table {
      width: 100%; border-collapse: collapse; margin-bottom: 24px;
      background: #161b22; border-radius: 6px; overflow: hidden;
    }
    .fields-table tr { border-bottom: 1px solid #21262d; }
    .fields-table td { padding: 8px 12px; font-size: 14px; vertical-align: top; }
    .field-key { color: #8b949e; font-family: monospace; width: 140px; }
    .empty-val { color: #484f58; }
    .id-link { font-family: monospace; font-weight: 600; }
    .body-content {
      background: #161b22; border: 1px solid #30363d; border-radius: 6px;
      padding: 16px; margin-top: 16px;
    }
    .body-content { font-size: 14px; line-height: 1.7; }
    .ref-list { display: flex; flex-wrap: wrap; gap: 8px; margin-bottom: 16px; }
    .ref-chip {
      display: inline-block; padding: 4px 10px; border-radius: 4px;
      font-size: 13px; background: #161b22; border: 1px solid;
      text-decoration: none; color: #c9d1d9;
    }
    .ref-chip:hover { text-decoration: none; background: #1c2129; }
    h3 { font-size: 16px; margin-bottom: 8px; color: #8b949e; }
    .body-content h2 { font-size: 18px; color: #e6edf3; margin: 20px 0 8px; }
    .body-content h3 { font-size: 16px; color: #e6edf3; margin: 16px 0 6px; }
    .body-content h4 { font-size: 14px; color: #e6edf3; margin: 12px 0 4px; }
    .body-content p { margin-bottom: 10px; }
    .body-content ul { margin: 0 0 12px 20px; }
    .body-content li { margin-bottom: 4px; }
    .body-content strong { color: #e6edf3; }
    .md-table {
      width: 100%; border-collapse: collapse; margin: 12px 0;
      font-size: 14px;
    }
    .md-table th {
      text-align: left; padding: 8px 12px; border-bottom: 2px solid #30363d;
      color: #e6edf3; font-weight: 600; background: #161b22;
    }
    .md-table td {
      padding: 6px 12px; border-bottom: 1px solid #21262d;
    }
    .md-table tr:hover td { background: #1c2129; }
    .md-code-block {
      background: #0d1117; border: 1px solid #30363d; border-radius: 4px;
      padding: 12px; margin: 12px 0; overflow-x: auto;
      font-family: "SF Mono", "Fira Code", monospace; font-size: 13px;
    }
    .md-inline-code {
      background: #1c2129; padding: 2px 6px; border-radius: 3px;
      font-family: "SF Mono", "Fira Code", monospace; font-size: 13px;
    }
    .status-pill { text-decoration: none; color: inherit; }
    .status-pill:hover { text-decoration: none; opacity: 0.8; }
  </style>
</head>
<body>${content}</body>
</html>`;
}

// --- Server ---

async function loadRecords(): Promise<Record[]> {
  return readAllRecords(DB_PATH);
}

const server = Bun.serve({
  port: PORT,
  async fetch(req) {
    const url = new URL(req.url);
    const path = url.pathname;

    // API: search
    if (path === '/api/search') {
      const q = (url.searchParams.get('q') || '').toLowerCase();
      if (q.length < 2) return Response.json([]);
      const records = await loadRecords();
      const results = records.filter(r => {
        const id = getId(r).toLowerCase();
        const title = String(r.fields.title || '').toLowerCase();
        const body = r.body.toLowerCase();
        return id.includes(q) || title.includes(q) || body.includes(q);
      }).slice(0, 50).map(r => ({
        id: getId(r),
        title: String(r.fields.title || '(no title)'),
        status: String(r.fields.status || ''),
        table: r.table,
      }));
      return Response.json(results);
    }

    // Record detail
    const recordMatch = path.match(/^\/record\/([A-Z]+\d{4})$/);
    if (recordMatch) {
      const records = await loadRecords();
      const record = records.find(r => getId(r) === recordMatch[1]);
      if (!record) return new Response('Not found', { status: 404 });
      return new Response(renderRecord(record, records), { headers: { 'Content-Type': 'text/html' } });
    }

    // Table listing
    const tableMatch = path.match(/^\/table\/([a-z_]+)$/);
    if (tableMatch) {
      const records = await loadRecords();
      const tableRecords = records.filter(r => r.table === tableMatch[1]);
      if (tableRecords.length === 0) return new Response('Empty table', { status: 404 });
      return new Response(renderTable(tableMatch[1], tableRecords), { headers: { 'Content-Type': 'text/html' } });
    }

    // Status filter
    const statusMatch = path.match(/^\/status\/(.+)$/);
    if (statusMatch) {
      const status = decodeURIComponent(statusMatch[1]);
      const records = await loadRecords();
      const matched = records.filter(r => String(r.fields.status || 'no status') === status);
      return new Response(renderStatus(status, matched), { headers: { 'Content-Type': 'text/html' } });
    }

    // Index
    if (path === '/' || path === '') {
      const records = await loadRecords();
      return new Response(renderIndex(records), { headers: { 'Content-Type': 'text/html' } });
    }

    return new Response('Not found', { status: 404 });
  },
});

console.log(`Eigenflower DB browser running at http://localhost:${PORT}`);
