/**
 * vox/cli.ts — Entry point for the Vox audio production tool
 *
 * Usage:
 *   bun tools/vox/cli.ts serve [--port 3333] [--project episodes]
 *   bun tools/vox/cli.ts generate <project-name> [--lines 001,002,003]
 *   bun tools/vox/cli.ts voices
 *   bun tools/vox/cli.ts stitch <project-name>
 */

import { OpenAITTSProvider } from "./tts/openai.ts";
import { OpenAILLMProvider } from "./llm/openai.ts";
import { createServer } from "./server.ts";
import { parseProductionXML, allLines, findCharacter, buildInstructions } from "./xml.ts";

function getApiKey(): string {
  const key = process.env.OPENAI_API_KEY;
  if (!key) {
    console.error("Error: OPENAI_API_KEY environment variable not set.");
    process.exit(1);
  }
  return key;
}

function usage(): never {
  console.log(`Vox — Multi-Voice Audio Production Tool

Usage:
  bun tools/vox/cli.ts serve [options]     Start web UI server
  bun tools/vox/cli.ts generate <project>  Generate audio for all lines
  bun tools/vox/cli.ts voices              List available TTS voices
  bun tools/vox/cli.ts stitch <project>    Concatenate all audio to full-mix.mp3

Options:
  --port <n>       Server port (default: 3333)
  --project <dir>  Episodes directory (default: episodes/)
  --lines <ids>    Comma-separated line IDs to generate (default: all)
`);
  process.exit(0);
}

async function main() {
  const args = Bun.argv.slice(2);
  if (args.length === 0) usage();

  const command = args[0];

  /* Parse flags */
  let port = parseInt(process.env.VOX_PORT || "3333", 10);
  let projectDir = "episodes";
  let lineIds: string[] | undefined;

  for (let i = 1; i < args.length; i++) {
    if (args[i] === "--port" && args[i + 1]) {
      port = parseInt(args[++i], 10);
    } else if (args[i] === "--project" && args[i + 1]) {
      projectDir = args[++i];
    } else if (args[i] === "--lines" && args[i + 1]) {
      lineIds = args[++i].split(",");
    }
  }

  switch (command) {
    case "serve": {
      const apiKey = getApiKey();
      const tts = new OpenAITTSProvider(apiKey);
      const llm = new OpenAILLMProvider(apiKey);
      createServer({ port, tts, llm, projectDir });
      break;
    }

    case "generate": {
      const projectName = args[1];
      if (!projectName || projectName.startsWith("--")) {
        console.error("Error: project name required. Usage: bun tools/vox/cli.ts generate <project-name>");
        process.exit(1);
      }

      const apiKey = getApiKey();
      const tts = new OpenAITTSProvider(apiKey);

      const xmlPath = `${projectDir}/${projectName}-audio/production.xml`;
      const file = Bun.file(xmlPath);
      if (!(await file.exists())) {
        console.error(`Error: ${xmlPath} not found`);
        process.exit(1);
      }

      const xml = await file.text();
      const production = parseProductionXML(xml);
      const entries = allLines(production);
      const toGenerate = lineIds
        ? entries.filter(e => lineIds!.includes(e.line.id))
        : entries;

      console.log(`Generating ${toGenerate.length} lines for ${projectName}...`);

      const audioDir = `${projectDir}/${projectName}-audio/audio`;
      const { mkdir } = await import("node:fs/promises");
      await mkdir(audioDir, { recursive: true });

      for (const entry of toGenerate) {
        const character = findCharacter(production, entry.line.character);
        const instructions = buildInstructions(character, entry.section, entry.line);
        const voice = character?.voice || "nova";
        const outputPath = `${audioDir}/${entry.line.id}-${entry.line.character}.mp3`;

        console.log(`  [${entry.line.id}] ${entry.line.character}: "${entry.line.text.slice(0, 50)}..."`);
        const result = await tts.generate({ text: entry.line.text, voice, instructions, outputPath });
        if (result.success) {
          console.log(`    OK (${(result.bytesWritten / 1024).toFixed(0)} KB)`);
        } else {
          console.error(`    ERROR: ${result.error}`);
        }
      }

      console.log("Done.");
      break;
    }

    case "voices": {
      const apiKey = getApiKey();
      const tts = new OpenAITTSProvider(apiKey);
      console.log("Available voices:");
      for (const v of tts.availableVoices()) {
        console.log(`  ${v}`);
      }
      break;
    }

    case "stitch": {
      const projectName = args[1];
      if (!projectName || projectName.startsWith("--")) {
        console.error("Error: project name required.");
        process.exit(1);
      }

      const xmlPath = `${projectDir}/${projectName}-audio/production.xml`;
      const file = Bun.file(xmlPath);
      if (!(await file.exists())) {
        console.error(`Error: ${xmlPath} not found`);
        process.exit(1);
      }

      const xml = await file.text();
      const production = parseProductionXML(xml);
      const entries = allLines(production);
      const audioDir = `${projectDir}/${projectName}-audio/audio`;
      const outputDir = `${projectDir}/${projectName}-audio/output`;

      const { mkdir } = await import("node:fs/promises");
      await mkdir(outputDir, { recursive: true });

      const parts: Uint8Array[] = [];
      for (const entry of entries) {
        /* Try versioned filenames, fall back to base */
        const basePath = `${audioDir}/${entry.line.id}-${entry.line.character}.mp3`;
        const f = Bun.file(basePath);
        if (await f.exists()) {
          parts.push(new Uint8Array(await f.arrayBuffer()));
        }
      }

      if (parts.length === 0) {
        console.error("No audio files found to stitch.");
        process.exit(1);
      }

      const totalSize = parts.reduce((s, a) => s + a.byteLength, 0);
      const combined = new Uint8Array(totalSize);
      let offset = 0;
      for (const part of parts) {
        combined.set(part, offset);
        offset += part.byteLength;
      }

      const outputPath = `${outputDir}/full-mix.mp3`;
      await Bun.write(outputPath, combined);
      console.log(`Stitched ${parts.length} files → ${outputPath} (${(totalSize / 1024).toFixed(0)} KB)`);
      break;
    }

    default:
      console.error(`Unknown command: ${command}`);
      usage();
  }
}

main();
