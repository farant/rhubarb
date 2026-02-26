import { mkdir, readdir, readFile, writeFile } from "node:fs/promises";
import { join, dirname, relative } from "node:path";

const ROOT_DIR = ".";
const ATLAS_DIR = "atlas";
const DEST_DIR = "atlas/web";

/**
 * Basic Markdown to HTML converter with table support and auto-linking.
 */
function mdToHtml(md: string, currentRelPath: string, allFiles: string[]): string {
  const isGlossary = currentRelPath.endsWith("glossary.md");

  // 1. Pre-process: Tables, Lists, Glossary entries
  const lines = md.split("\n");
  const processedLines: string[] = [];
  let i = 0;

  while (i < lines.length) {
    const line = lines[i].trim();

    // Detect glossary definition block: consecutive **Label**: content lines
    if (isGlossary && line.match(/^\*\*\w[\w\s]*\*\*:/)) {
      let tableHtml = '<table class="glossary-entry"><tbody>';
      while (i < lines.length) {
        const cur = lines[i].trim();
        const m = cur.match(/^\*\*([\w][\w\s]*)\*\*:\s*(.*)/);
        if (m) {
          tableHtml += `<tr><td class="glossary-label">${m[1]}</td><td>${m[2]}</td></tr>`;
          i++;
        } else {
          break;
        }
      }
      tableHtml += "</tbody></table>";
      processedLines.push(tableHtml);
      continue;
    }

    // Detect table start
    if (line.startsWith("|") && i + 1 < lines.length && lines[i+1].trim().match(/^\|[:\-\s|]+\|$/)) {
      const tableLines: string[] = [];
      while (i < lines.length && lines[i].trim().startsWith("|")) {
        tableLines.push(lines[i].trim());
        i++;
      }
      if (tableLines.length >= 2) {
        const headerRow = tableLines[0];
        const bodyRows = tableLines.slice(2);
        let tableHtml = "<table><thead><tr>";
        const headers = headerRow.split("|").map(s => s.trim()).filter((s, idx, arr) => idx !== 0 && idx !== arr.length - 1);
        headers.forEach(h => { tableHtml += `<th>${h}</th>`; });
        tableHtml += "</tr></thead><tbody>";
        bodyRows.forEach(row => {
          tableHtml += "<tr>";
          const cells = row.split("|").map(s => s.trim()).filter((s, idx, arr) => idx !== 0 && idx !== arr.length - 1);
          cells.forEach(c => { tableHtml += `<td>${c}</td>`; });
          tableHtml += "</tr>";
        });
        tableHtml += "</tbody></table>";
        processedLines.push(tableHtml);
        continue;
      }
    }
    
    // Detect Unordered List start
    if (line.match(/^[\-\*]\s/)) {
      let listHtml = "<ul>\n";
      while (i < lines.length) {
        const curLine = lines[i].trim();
        // If it starts with a bullet, it's a new item
        if (curLine.match(/^[\-\*]\s/)) {
          // If we already started an item, close it
          if (listHtml.endsWith("</li>\n") === false && listHtml !== "<ul>\n") {
            listHtml += "</li>\n";
          }
          const liContent = curLine.replace(/^[\-\*]\s+/, "");
          listHtml += `<li>${liContent}`;
          i++;
        } 
        // If it's empty, the list is over
        else if (curLine === "") {
          break;
        }
        // If it's indented or just text immediately following a bullet, it's a continuation of the previous item
        else {
           listHtml += ` ${curLine}`;
           i++;
        }
      }
      if (listHtml !== "<ul>\n" && !listHtml.endsWith("</li>\n")) {
         listHtml += "</li>\n";
      }
      listHtml += "</ul>";
      processedLines.push(listHtml);
      continue;
    }

    processedLines.push(lines[i]);
    i++;
  }

  let html = processedLines.join("\n");

  // 2. Smart Auto-linking for .md and .c files
  html = html.replace(/(?<![\[\(])\b([\w\.\-\/]+\.(md|c))\b(?![\]\)])/g, (match) => {
    let targetFile = allFiles.find(f => f === match || f.endsWith("/" + match));
    if (targetFile) {
      const currentDir = dirname(currentRelPath);
      // For C files, we append .html to their name, for MD we replace .md with .html
      const targetHtml = targetFile.endsWith(".c") ? targetFile + ".html" : targetFile.replace(/\.md$/, ".html");
      let relLink = relative(currentDir, targetHtml);
      if (!relLink.startsWith(".") && !relLink.startsWith("/")) relLink = "./" + relLink;
      return `<a href="${relLink}">${match}</a>`;
    }
    return match;
  });

  // 3. Standard Markdown replacements
  html = html
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/&lt;table(.*?)&gt;/g, "<table$1>").replace(/&lt;\/table&gt;/g, "</table>")
    .replace(/&lt;thead&gt;/g, "<thead>").replace(/&lt;\/thead&gt;/g, "</thead>")
    .replace(/&lt;tbody&gt;/g, "<tbody>").replace(/&lt;\/tbody&gt;/g, "</tbody>")
    .replace(/&lt;tr&gt;/g, "<tr>").replace(/&lt;\/tr&gt;/g, "</tr>")
    .replace(/&lt;th&gt;/g, "<th>").replace(/&lt;\/th&gt;/g, "</th>")
    .replace(/&lt;td(.*?)&gt;/g, "<td$1>").replace(/&lt;\/td&gt;/g, "</td>")
    .replace(/&lt;ul&gt;/g, "<ul>").replace(/&lt;\/ul&gt;/g, "</ul>")
    .replace(/&lt;li&gt;/g, "<li>").replace(/&lt;\/li&gt;/g, "</li>")
    .replace(/&lt;a (.*?)&gt;/g, "<a $1>").replace(/&lt;\/a&gt;/g, "</a>")
    .replace(/^# (.*$)/gim, "<h1>$1</h1>")
    .replace(/^## (.*$)/gim, "<h2>$1</h2>")
    .replace(/^### (.*$)/gim, "<h3>$1</h3>")
    .replace(/^#### (.*$)/gim, "<h4>$1</h4>")
    .replace(/\*\*\*(.*?)\*\*\*/g, "<strong><em>$1</em></strong>")
    .replace(/\*\*(.*?)\*\*/g, "<strong>$1</strong>")
    .replace(/\*(.*?)\*/g, "<em>$1</em>")
    .replace(/(?<![a-zA-Z0-9])__(.*?)__(?![a-zA-Z0-9])/g, "<strong>$1</strong>")
    .replace(/(?<![a-zA-Z0-9])_(.*?)_(?![a-zA-Z0-9])/g, "<em>$1</em>")
    .replace(/`(.*?)`/g, "<code>$1</code>")
    .replace(/```(\w*)\n([\s\S]*?)```/g, (match, lang, code) => {
      const cls = lang ? ` class="language-${lang}"` : "";
      return `<pre><code${cls}>${code}</code></pre>`;
    })
    .replace(/```([\s\S]*?)```/g, "<pre><code>$1</code></pre>") // fallback for no lang
    .replace(/\[([^\]]+)\]\(([^)]+)\.(md|c)\)/g, (match, text, path, ext) => {
       const htmlPath = ext === 'c' ? `${path}.c.html` : `${path}.html`;
       return `<a href="${htmlPath}">${text}</a>`;
    })
    .replace(/\[([^\]]+)\]\(([^)]+)\)/g, '<a href="$2">$1</a>')
    .split("\n\n")
    .map((p) => {
      const trimmed = p.trim();
      if (trimmed.startsWith("<h") || trimmed.startsWith("<table") || trimmed.startsWith("<ul") || trimmed.startsWith("<li") || trimmed.startsWith("<pre") || trimmed === "") {
        return p;
      }
      return `<p>${trimmed.replace(/\n/g, " ")}</p>`;
    })
    .join("\n")
    .replace(/<\/ul><\/p>/g, "</ul>");

  // 4. Link code elements that reference known .md or .c files
  html = html.replace(/<code>([\w\.\-\/]+\.(md|c))<\/code>/g, (fullMatch, filePath) => {
    const candidates = [filePath];
    if (filePath.startsWith("atlas/")) candidates.push(filePath.slice(6));
    let targetFile: string | undefined;
    for (const c of candidates) {
      targetFile = allFiles.find(f => f === c || f.endsWith("/" + c));
      if (targetFile) break;
    }
    if (targetFile) {
      const currentDir = dirname(currentRelPath);
      const targetHtml = targetFile.endsWith(".c") ? targetFile + ".html" : targetFile.replace(/\.md$/, ".html");
      let relLink = relative(currentDir, targetHtml);
      if (!relLink.startsWith(".") && !relLink.startsWith("/")) relLink = "./" + relLink;
      return `<a href="${relLink}"><code>${filePath}</code></a>`;
    }
    return fullMatch;
  });

  // 5. Link D{N} and D{N}-{M} demo references to inventory entries
  const demoLinkFn = (num: string, text: string): string => {
    const padded = num.length === 1 ? "0" + num : num;
    const demoFile = "inventory/entries/demo_" + padded + ".md";
    if (!allFiles.includes(demoFile)) return text;
    const currentDir = dirname(currentRelPath);
    const targetHtml = "inventory/entries/demo_" + padded + ".html";
    let relLink = relative(currentDir, targetHtml);
    if (!relLink.startsWith(".") && !relLink.startsWith("/")) relLink = "./" + relLink;
    return '<a href="' + relLink + '">' + text + '</a>';
  };
  const demoRegex = /\bD(\d+)(?:([-\u2013])(D?)(\d+))?\b/g;
  const linkParts = html.split(/(<a\s[^>]*>.*?<\/a>)/g);
  html = linkParts.map(part => {
    if (part.startsWith("<a ")) return part;
    return part.replace(demoRegex, (_, n1, sep, dPre, n2) => {
      if (n2 && sep) {
        return demoLinkFn(n1, "D" + n1) + sep + demoLinkFn(n2, (dPre || "") + n2);
      }
      return demoLinkFn(n1, "D" + n1);
    });
  }).join("");

  return html;
}

function wrapHtml(title: string, body: string, depth: number, isIndex = false): string {
  const rootRel = depth === 0 ? "./" : "../".repeat(depth);
  const backLink = isIndex ? "" : `<nav><a href="${rootRel}index.html">← Back to Index</a></nav><hr>`;
  return `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>${title}</title>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Nunito:wght@400;600;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/github.min.css">
    <link rel="stylesheet" href="${rootRel}style.css">
</head>
<body>
    ${backLink}
    ${body}
    <script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/languages/c.min.js"></script>
    <script>hljs.highlightAll();</script>
</body>
</html>`;
}

const CSS_CONTENT = `
body {
    font-family: "Avenir Next", "Avenir", "Nunito", -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    line-height: 1.6;
    color: #1a1a1a;
    max-width: 900px;
    margin: 40px auto;
    padding: 0 20px;
    background: #ffffff;
}
pre {
    background: #f6f8fa;
    padding: 15px;
    border-radius: 5px;
    overflow-x: auto;
    border: 1px solid #eaecef;
}
code {
    font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, monospace;
    background: #f6f8fa;
    padding: 2px 4px;
    border-radius: 3px;
    font-size: 0.9em;
}
pre code {
    background: none;
    padding: 0;
}
a {
    color: #0066cc;
    text-decoration: none;
}
a:hover {
    text-decoration: underline;
}
h1, h2, h3 {
    border-bottom: 1px solid #eaecef;
    padding-bottom: 0.3em;
    margin-top: 1.5em;
    font-weight: 600;
}
hr {
    border: 0;
    border-top: 1px solid #eaecef;
    margin: 2rem 0;
}
nav {
    margin-bottom: 20px;
}
ul {
    padding-left: 20px;
}
li {
    margin-bottom: 5px;
}
table {
    border-collapse: collapse;
    width: 100%;
    margin: 20px 0;
    font-size: 0.95em;
}
th, td {
    border: 1px solid #dfe2e5;
    padding: 8px 12px;
    text-align: left;
}
th {
    background-color: #f6f8fa;
    font-weight: 600;
}
tr:nth-child(even) {
    background-color: #fdfdfd;
}
tr:hover {
    background-color: #f6f8fa;
}
.index-section {
    margin-top: 2em;
}
.index-section h2 {
    background: #f6f8fa;
    padding: 5px 10px;
    border-radius: 4px;
    border-bottom: none;
}
.glossary-entry {
    margin: 0 0 1.5em 0;
}
.glossary-entry td {
    vertical-align: top;
}
.glossary-label {
    font-weight: 600;
    white-space: nowrap;
    width: 1%;
    padding-right: 16px;
}
`;

async function getAllFiles(dir: string, exts: string[], allFiles: string[] = []): Promise<string[]> {
  const entries = await readdir(dir, { withFileTypes: true });
  for (const entry of entries) {
    const res = join(dir, entry.name);
    if (entry.isDirectory()) {
      if (entry.name === "node_modules" || res === DEST_DIR || entry.name.startsWith(".")) continue;
      await getAllFiles(res, exts, allFiles);
    } else {
      if (exts.some(ext => entry.name.endsWith(ext))) {
        allFiles.push(res);
      }
    }
  }
  return allFiles;
}

async function main() {
  console.log(`🚀 Generating organized web folder in ${DEST_DIR}...`);
  
  await mkdir(DEST_DIR, { recursive: true });
  await writeFile(join(DEST_DIR, "style.css"), CSS_CONTENT);
  console.log("- Created: style.css");

  // Get all MD files from atlas only
  const mdFilesAbsolute = await getAllFiles(ATLAS_DIR, [".md"]);
  // Get all C files from root
  const cFilesAbsolute = await getAllFiles(ROOT_DIR, [".c"]);

  const allRelPaths: string[] = [];
  const sections: Record<string, { path: string, title: string }[]> = {};

  // Process Markdown files
  for (const mdFile of mdFilesAbsolute) {
    const relPath = relative(ATLAS_DIR, mdFile);
    allRelPaths.push(relPath);
  }

  // Process C files
  for (const cFile of cFilesAbsolute) {
    const rootRelPath = relative(ROOT_DIR, cFile);
    allRelPaths.push(rootRelPath); 

    const htmlRelPath = rootRelPath + ".html";
    const destPath = join(DEST_DIR, htmlRelPath);
    const depth = htmlRelPath.split("/").length - 1;

    const content = await readFile(cFile, "utf-8");
    const escapedContent = content.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
    const htmlBody = `<h1>${rootRelPath}</h1><pre><code class="language-c">${escapedContent}</code></pre>`;
    const finalHtml = wrapHtml(rootRelPath, htmlBody, depth);

    await mkdir(dirname(destPath), { recursive: true });
    await writeFile(destPath, finalHtml);
    console.log("- Converted C:", rootRelPath);
  }

  for (const mdFile of mdFilesAbsolute) {
    const relPath = relative(ATLAS_DIR, mdFile);
    const htmlRelPath = relPath.replace(/\.md$/, ".html");
    const destPath = join(DEST_DIR, htmlRelPath);
    const depth = htmlRelPath.split("/").length - 1;

    const parts = relPath.split("/");
    const sectionName = parts.length > 1 ? parts[0] : "General";
    if (!sections[sectionName]) sections[sectionName] = [];
    sections[sectionName].push({ path: htmlRelPath, title: relPath });

    const content = await readFile(mdFile, "utf-8");
    const htmlBody = mdToHtml(content, relPath, allRelPaths);
    const finalHtml = wrapHtml(relPath, htmlBody, depth);

    await mkdir(dirname(destPath), { recursive: true });
    await writeFile(destPath, finalHtml);
    console.log("- Converted MD:", relPath);
  }

  // Generate Grouped Index
  let indexBody = `<h1>Atlas Project Index</h1>`;
  const sortedSectionNames = Object.keys(sections).sort((a, b) => {
    if (a === "General") return -1;
    if (b === "General") return 1;
    return a.localeCompare(b);
  });

  for (const section of sortedSectionNames) {
    indexBody += `<div class="index-section"><h2>${section}</h2><ul>`;
    const links = sections[section].sort((a, b) => 
      a.title.localeCompare(b.title, undefined, { numeric: true, sensitivity: 'base' })
    );
    for (const link of links) {
      indexBody += `<li><a href="${link.path}">${link.title}</a></li>`;
    }
    indexBody += `</ul></div>`;
  }

  const indexHtml = wrapHtml("Atlas Project Index", indexBody, 0, true);
  await writeFile(join(DEST_DIR, "index.html"), indexHtml);

  console.log(`\n✅ Done! Organized, styled, auto-linked, and C files generated. Open ${DEST_DIR}/index.html to view.`);
}

main().catch(console.error);
