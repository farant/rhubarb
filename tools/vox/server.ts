/**
 * vox/server.ts — Bun.serve() web server for Vox
 *
 * API routes + static files + WebSocket for real-time progress.
 * Manages per-episode audio output directories and manifests.
 */

import type {
  Production, Manifest, LineStatus, LineVersion,
  Line, Section, TTSProvider, LLMProvider, WSMessage,
} from "./types.ts";
import { parseProductionXML, findCharacter } from "./xml.ts";
import { isLine, isPause } from "./types.ts";

/* ── Types ── */

interface ProjectState {
  name: string;
  production: Production;
  manifest: Manifest;
  xmlPath: string;
  audioDir: string;
  outputDir: string;
  manifestPath: string;
}

interface LineGroup {
  lines: { line: Line; section: Section }[];
  groupId: string;   /* "g{firstId}-{lastId}" for multi-line, or just lineId for singles */
}

/** Walk production elements in order, grouping consecutive same-character lines.
 *  Pauses and character changes flush the current group. */
function buildGroups(production: Production): LineGroup[] {
  const groups: LineGroup[] = [];
  let current: { line: Line; section: Section }[] = [];
  let currentChar: string | null = null;

  function flush() {
    if (current.length === 0) return;
    const groupId = current.length === 1
      ? current[0].line.id
      : `g${current[0].line.id}-${current[current.length - 1].line.id}`;
    groups.push({ lines: [...current], groupId });
    current = [];
    currentChar = null;
  }

  for (const section of production.sections) {
    for (const el of section.elements) {
      if (isLine(el)) {
        if (el.character === currentChar) {
          current.push({ line: el, section });
        } else {
          flush();
          currentChar = el.character;
          current.push({ line: el, section });
        }
      } else if (isPause(el)) {
        flush();
      }
    }
  }
  flush();

  return groups;
}

/** Find the group containing a given lineId */
function findGroupForLine(groups: LineGroup[], lineId: string): LineGroup | undefined {
  return groups.find(g => g.lines.some(e => e.line.id === lineId));
}

/** Build a surrounding script excerpt for TTS context.
 *  Returns a formatted mini-script showing ~N lines before and after the group,
 *  so the TTS model understands the emotional arc and conversational flow. */
function buildScriptContext(production: Production, group: LineGroup, radius: number = 4): string {
  /* Flatten all elements across sections into a single ordered list with character lookup */
  const flat: { el: Line | { ms: number }; section: Section }[] = [];
  for (const section of production.sections) {
    for (const el of section.elements) {
      flat.push({ el: el as Line | { ms: number }, section });
    }
  }

  /* Find the index range of lines belonging to this group */
  const groupLineIds = new Set(group.lines.map(e => e.line.id));
  let firstIdx = -1;
  let lastIdx = -1;
  for (let i = 0; i < flat.length; i++) {
    const el = flat[i].el;
    if ("id" in el && groupLineIds.has(el.id)) {
      if (firstIdx < 0) firstIdx = i;
      lastIdx = i;
    }
  }

  if (firstIdx < 0) return "";

  /* Gather window: radius elements before and after */
  const windowStart = Math.max(0, firstIdx - radius);
  const windowEnd = Math.min(flat.length - 1, lastIdx + radius);

  const lines: string[] = [];
  for (let i = windowStart; i <= windowEnd; i++) {
    const { el } = flat[i];
    if ("id" in el) {
      const line = el as Line;
      const char = findCharacter(production, line.character);
      const name = char?.name || line.character;
      const inGroup = groupLineIds.has(line.id);
      const marker = inGroup ? ">>>" : "   ";
      lines.push(`${marker} ${name.toUpperCase()}: "${line.text}"`);
    } else if ("ms" in el) {
      lines.push(`    [pause ${(el as { ms: number }).ms}ms]`);
    }
  }

  return `Script context (>>> marks the line(s) to perform):\n${lines.join("\n")}`;
}

/** Compute default announce map: first dialogue line per character per section = true.
 *  Narration lines are excluded — only dialogue gets character introductions. */
function computeAnnounceDefaults(production: Production): Record<string, boolean> {
  const defaults: Record<string, boolean> = {};
  for (const section of production.sections) {
    const seen = new Set<string>();
    for (const el of section.elements) {
      if (isLine(el) && el.type === "dialogue") {
        if (!seen.has(el.character)) {
          defaults[el.id] = true;
          seen.add(el.character);
        } else {
          defaults[el.id] = false;
        }
      }
    }
  }
  return defaults;
}

/** Resolve announce state for a line: override > default > false */
function shouldAnnounce(
  lineId: string,
  defaults: Record<string, boolean>,
  overrides: Record<string, boolean>,
): boolean {
  if (lineId in overrides) return overrides[lineId];
  if (lineId in defaults) return defaults[lineId];
  return false;
}

/* ── Server ── */

export function createServer(opts: {
  port: number;
  tts: TTSProvider;
  llm: LLMProvider;
  projectDir: string;
}) {
  const { port, tts, llm, projectDir } = opts;

  /* WebSocket clients */
  const wsClients = new Set<{ send(data: string): void }>();

  function broadcast(msg: WSMessage) {
    const data = JSON.stringify(msg);
    for (const ws of wsClients) {
      try { ws.send(data); } catch { /* ignore dead sockets */ }
    }
  }

  /* ── Project loading ── */

  async function loadProject(name: string): Promise<ProjectState | null> {
    /* Look for <name>-audio/ directory next to episode markdown */
    const audioDir = `${projectDir}/${name}-audio/audio`;
    const outputDir = `${projectDir}/${name}-audio/output`;
    const manifestPath = `${projectDir}/${name}-audio/manifest.json`;
    const xmlPath = `${projectDir}/${name}-audio/production.xml`;

    const xmlFile = Bun.file(xmlPath);
    if (!(await xmlFile.exists())) return null;

    const xmlText = await xmlFile.text();
    const production = parseProductionXML(xmlText);

    /* Load or create manifest */
    let manifest: Manifest;
    const manifestFile = Bun.file(manifestPath);
    if (await manifestFile.exists()) {
      manifest = await manifestFile.json() as Manifest;
    } else {
      manifest = {
        project: name,
        episode: production.episode,
        createdAt: new Date().toISOString(),
        updatedAt: new Date().toISOString(),
        lines: {},
      };
    }

    return { name, production, manifest, xmlPath, audioDir, outputDir, manifestPath };
  }

  async function saveManifest(state: ProjectState) {
    state.manifest.updatedAt = new Date().toISOString();
    await Bun.write(state.manifestPath, JSON.stringify(state.manifest, null, 2));
  }

  /* ── Audio generation ── */

  async function generateGroup(
    state: ProjectState,
    group: LineGroup,
    feedback?: string,
  ): Promise<{ success: boolean; error?: string }> {
    const firstLine = group.lines[0].line;
    const firstSection = group.lines[0].section;
    const character = findCharacter(state.production, firstLine.character);
    const voice = character?.voice || "nova";

    /* Concatenate all lines' text */
    const combinedText = group.lines.map(e => e.line.text).join("\n\n");

    /* Build instructions: baseline + section context + script context + delivery hints */
    const parts: string[] = [];
    if (character?.baseline) parts.push(character.baseline);
    if (firstSection?.context) parts.push(`Scene: ${firstSection.context}`);
    const scriptContext = buildScriptContext(state.production, group);
    if (scriptContext) parts.push(scriptContext);
    for (const entry of group.lines) {
      if (entry.line.delivery) parts.push(entry.line.delivery);
    }
    const instructions = parts.join(" ");

    /* Determine version from first line */
    const existing = state.manifest.lines[firstLine.id];
    const version = existing ? existing.currentVersion + 1 : 1;
    const audioFile = `${group.groupId}-${firstLine.character}.${version > 1 ? `v${version}.` : ""}mp3`;
    const outputPath = `${state.audioDir}/${audioFile}`;

    /* Ensure audio directory exists */
    const { mkdir } = await import("node:fs/promises");
    await mkdir(state.audioDir, { recursive: true });

    /* Broadcast generating for all lines in group */
    for (const entry of group.lines) {
      broadcast({ type: "progress", lineId: entry.line.id, status: "generating" });
    }

    if (group.lines.length > 1) {
      const lineIds = group.lines.map(e => e.line.id).join(", ");
      console.log(`Generating group ${group.groupId} (${firstLine.character}, ${group.lines.length} lines: ${lineIds})...`);
    }

    /* Check if narrator should announce the character name before this group */
    const announceDefaults = computeAnnounceDefaults(state.production);
    const announceOverrides = state.manifest.announceOverrides || {};
    const needsAnnounce = shouldAnnounce(firstLine.id, announceDefaults, announceOverrides);

    let announceBytes: Uint8Array | null = null;
    if (needsAnnounce && character) {
      const narrator = state.production.cast.find(c => c.id === "narrator");
      const narratorVoice = narrator?.voice || "onyx";
      const announcePath = `${state.audioDir}/_announce-${firstLine.id}.mp3`;
      const announceResult = await tts.generate({
        text: `${character.name}.`,
        voice: narratorVoice,
        instructions: narrator?.baseline || "Brief character introduction. Say the name clearly.",
        outputPath: announcePath,
      });
      if (announceResult.success) {
        const announceFile = Bun.file(announcePath);
        announceBytes = new Uint8Array(await announceFile.arrayBuffer());
        /* Clean up temp file */
        const { unlink } = await import("node:fs/promises");
        await unlink(announcePath).catch(() => {});
      }
    }

    const result = await tts.generate({
      text: combinedText,
      voice,
      instructions,
      outputPath,
    });

    if (!result.success) {
      for (const entry of group.lines) {
        broadcast({ type: "progress", lineId: entry.line.id, status: "error", message: result.error });
      }
      return { success: false, error: result.error };
    }

    /* If we have an announce clip, prepend it to the audio file */
    if (announceBytes) {
      const lineFile = Bun.file(outputPath);
      const lineBytes = new Uint8Array(await lineFile.arrayBuffer());
      /* Small silence gap between announce and line (~200ms) */
      const gap = generateSilence(200);
      const combined = new Uint8Array(announceBytes.byteLength + gap.byteLength + lineBytes.byteLength);
      combined.set(announceBytes, 0);
      combined.set(gap, announceBytes.byteLength);
      combined.set(lineBytes, announceBytes.byteLength + gap.byteLength);
      await Bun.write(outputPath, combined);
    }

    /* Update manifest for each line in the group */
    const timestamp = new Date().toISOString();
    for (let i = 0; i < group.lines.length; i++) {
      const entry = group.lines[i];
      const lineId = entry.line.id;
      const isFirst = i === 0;

      const lineVersion: LineVersion = {
        version,
        delivery: entry.line.delivery,
        feedback: feedback || null,
        audioFile: isFirst ? audioFile : "",  /* only first line stores the file */
        timestamp,
      };

      if (!state.manifest.lines[lineId]) {
        state.manifest.lines[lineId] = {
          lineId,
          character: entry.line.character,
          currentVersion: version,
          versions: [lineVersion],
          groupId: group.lines.length > 1 ? group.groupId : undefined,
        };
      } else {
        state.manifest.lines[lineId].currentVersion = version;
        state.manifest.lines[lineId].versions.push(lineVersion);
        state.manifest.lines[lineId].groupId = group.lines.length > 1 ? group.groupId : undefined;
      }
    }

    await saveManifest(state);
    for (const entry of group.lines) {
      broadcast({ type: "progress", lineId: entry.line.id, status: "done" });
    }

    return { success: true };
  }

  /* ── Redo with LLM feedback ── */

  async function redoLine(
    state: ProjectState,
    lineId: string,
    feedback: string,
  ): Promise<{ success: boolean; error?: string; newDelivery?: string; groupId?: string }> {
    /* Find the group this line belongs to */
    const groups = buildGroups(state.production);
    const group = findGroupForLine(groups, lineId);
    if (!group) return { success: false, error: `Line ${lineId} not found` };

    const firstEntry = group.lines[0];
    const character = findCharacter(state.production, firstEntry.line.character);
    const existing = state.manifest.lines[firstEntry.line.id];

    /* Build LLM prompt — include all lines in the group */
    const systemPrompt = `You are a voice direction assistant for audio production.
Given a character's baseline voice description, the current delivery instructions, the line text(s), and user feedback,
generate improved delivery instructions for a text-to-speech engine.
Return ONLY the new delivery instructions, nothing else. Keep it concise (1-2 sentences).`;

    const lineTexts = group.lines.map(e =>
      `Line ${e.line.id}: "${e.line.text}" (delivery: "${e.line.delivery}")`
    ).join("\n");

    const previousAttempts = existing?.versions.map(v =>
      `Version ${v.version}: "${v.delivery}"${v.feedback ? ` (feedback: ${v.feedback})` : ""}`
    ).join("\n") || "None";

    const userMessage = `Character: ${character?.name || firstEntry.line.character}
Baseline: ${character?.baseline || "No baseline"}
${group.lines.length > 1 ? `Group (${group.groupId}) — ${group.lines.length} lines:` : "Line:"}
${lineTexts}
Previous attempts:
${previousAttempts}
User feedback: "${feedback}"

Generate new delivery instructions:`;

    const llmResult = await llm.complete({ systemPrompt, userMessage });
    if (!llmResult.success) {
      return { success: false, error: llmResult.error };
    }

    /* Update delivery for all lines in the group */
    const newDelivery = llmResult.content.trim();
    for (const entry of group.lines) {
      entry.line.delivery = newDelivery;
    }

    /* Save updated XML */
    await saveProductionXML(state);

    /* Generate new audio for the whole group */
    const genResult = await generateGroup(state, group, feedback);
    if (!genResult.success) return genResult;

    /* Broadcast redo for all lines in group */
    for (const entry of group.lines) {
      const currentVersion = state.manifest.lines[entry.line.id]?.currentVersion || 1;
      broadcast({ type: "redo", lineId: entry.line.id, version: currentVersion });
    }

    return { success: true, newDelivery, groupId: group.groupId };
  }

  /* ── Save production XML back ── */

  async function saveProductionXML(state: ProjectState) {
    const p = state.production;
    let xml = `<production title="${escXml(p.title)}" episode="${escXml(p.episode)}">\n`;
    xml += `  <cast>\n`;
    for (const c of p.cast) {
      xml += `    <character id="${escXml(c.id)}" name="${escXml(c.name)}" voice="${escXml(c.voice)}"\n`;
      xml += `      baseline="${escXml(c.baseline)}" />\n`;
    }
    xml += `  </cast>\n`;
    xml += `  <script>\n`;
    for (const section of p.sections) {
      xml += `    <section title="${escXml(section.title)}" context="${escXml(section.context)}">\n`;
      for (const el of section.elements) {
        if ("id" in el) {
          const line = el as Line;
          xml += `      <line id="${escXml(line.id)}" type="${escXml(line.type)}" character="${escXml(line.character)}">\n`;
          xml += `        <text>${escXml(line.text)}</text>\n`;
          xml += `        <delivery>${escXml(line.delivery)}</delivery>\n`;
          xml += `      </line>\n`;
        } else {
          const pause = el as { ms: number };
          xml += `      <pause ms="${pause.ms}"/>\n`;
        }
      }
      xml += `    </section>\n`;
    }
    xml += `  </script>\n`;
    xml += `</production>\n`;
    await Bun.write(state.xmlPath, xml);
  }

  /* ── Silent MP3 frame generator ── */

  /**
   * A single valid MPEG1 Layer3 128kbps 44100Hz stereo silent frame (417 bytes).
   * Each frame represents ~26.12ms of audio. We tile these to produce silence.
   */
  function makeSilentMp3Frame(): Uint8Array {
    /* MPEG1 Layer3 frame: sync=0xFFFB, 128kbps, 44100Hz, stereo
     * Frame size = 144 * 128000 / 44100 + padding = 417 bytes (no padding)
     * Header: FF FB 90 00 = sync + MPEG1 + Layer3 + 128kbps + 44100 + stereo */
    const frame = new Uint8Array(417);
    frame[0] = 0xFF;
    frame[1] = 0xFB;
    frame[2] = 0x90;
    frame[3] = 0x00;
    /* Rest is zeros = silence. Side info and main data being all-zero
     * produces a valid (silent) frame for most decoders. */
    return frame;
  }

  /** Generate N milliseconds of silent MP3 data */
  function generateSilence(ms: number): Uint8Array {
    if (ms <= 0) return new Uint8Array(0);
    const frameDurationMs = 26.12;
    const numFrames = Math.max(1, Math.round(ms / frameDurationMs));
    const frame = makeSilentMp3Frame();
    const result = new Uint8Array(numFrames * frame.byteLength);
    for (let i = 0; i < numFrames; i++) {
      result.set(frame, i * frame.byteLength);
    }
    return result;
  }

  /* ── Stitch audio files (group-aware, with silence for pauses) ── */

  async function stitchAudio(
    state: ProjectState,
    defaultGapMs: number = 300,
  ): Promise<{ success: boolean; error?: string; outputPath?: string }> {
    const { mkdir } = await import("node:fs/promises");
    await mkdir(state.outputDir, { recursive: true });

    broadcast({ type: "stitch", status: "started" });

    /* Walk production elements in order, building flat sequence of
     * audio chunks and pauses (same logic as client-side buildPlaylist) */
    const groups = buildGroups(state.production);
    const seenGroups = new Set<string>();
    const parts: Uint8Array[] = [];
    let lastWasAudio = false;

    for (const section of state.production.sections) {
      for (const el of section.elements) {
        if (isLine(el)) {
          /* Find this line's group */
          const group = findGroupForLine(groups, el.id);
          if (!group) continue;
          if (seenGroups.has(group.groupId)) continue;
          seenGroups.add(group.groupId);

          /* Insert default gap if two audio entries are adjacent (no pause between) */
          if (lastWasAudio && defaultGapMs > 0) {
            parts.push(generateSilence(defaultGapMs));
          }

          const firstLineId = group.lines[0].line.id;
          const lineStatus = state.manifest.lines[firstLineId];
          if (!lineStatus) continue;

          const latestVersion = lineStatus.versions[lineStatus.versions.length - 1];
          if (!latestVersion.audioFile) continue;
          const filePath = `${state.audioDir}/${latestVersion.audioFile}`;
          const file = Bun.file(filePath);
          if (await file.exists()) {
            const bytes = new Uint8Array(await file.arrayBuffer());
            parts.push(bytes);
            lastWasAudio = true;
          }
        } else if (isPause(el)) {
          /* Insert silence for the pause duration */
          if (lastWasAudio) {
            parts.push(generateSilence(el.ms));
            lastWasAudio = false;
          }
        }
      }
    }

    if (parts.length === 0) {
      broadcast({ type: "stitch", status: "error", message: "No audio files found" });
      return { success: false, error: "No audio files found" };
    }

    /* Concatenate all parts */
    const totalSize = parts.reduce((s, a) => s + a.byteLength, 0);
    const combined = new Uint8Array(totalSize);
    let offset = 0;
    for (const part of parts) {
      combined.set(part, offset);
      offset += part.byteLength;
    }

    const outputPath = `${state.outputDir}/full-mix.mp3`;
    await Bun.write(outputPath, combined);

    broadcast({ type: "stitch", status: "done" });
    return { success: true, outputPath };
  }

  /* ── Helpers ── */

  function escXml(s: string): string {
    return s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;");
  }

  function json(data: unknown, status = 200): Response {
    return new Response(JSON.stringify(data), {
      status,
      headers: { "Content-Type": "application/json" },
    });
  }

  function err(message: string, status = 400): Response {
    return json({ error: message }, status);
  }

  /* ── Route handling ── */

  async function handleRequest(req: Request): Promise<Response> {
    const url = new URL(req.url);
    const path = url.pathname;
    const method = req.method;

    /* API routes */
    if (path.startsWith("/api/")) {
      return handleAPI(req, path, method);
    }

    /* Static file: UI */
    if (path === "/" || path === "/index.html") {
      const uiPath = import.meta.dir + "/ui/index.html";
      const file = Bun.file(uiPath);
      return new Response(file, {
        headers: { "Content-Type": "text/html" },
      });
    }

    return new Response("Not Found", { status: 404 });
  }

  async function handleAPI(req: Request, path: string, method: string): Promise<Response> {
    /* GET /api/project/:name */
    const projectMatch = path.match(/^\/api\/project\/([^/]+)$/);
    if (projectMatch && method === "GET") {
      const state = await loadProject(projectMatch[1]);
      if (!state) return err("Project not found", 404);
      const groups = buildGroups(state.production);
      /* Send a simple lineId→groupId map for the UI */
      const groupMap: Record<string, string> = {};
      for (const g of groups) {
        if (g.lines.length > 1) {
          for (const entry of g.lines) {
            groupMap[entry.line.id] = g.groupId;
          }
        }
      }
      /* Compute announce state (defaults merged with overrides) */
      const announceDefaults = computeAnnounceDefaults(state.production);
      const announceOverrides = state.manifest.announceOverrides || {};
      const announceMap: Record<string, boolean> = {};
      for (const lineId of Object.keys(announceDefaults)) {
        announceMap[lineId] = shouldAnnounce(lineId, announceDefaults, announceOverrides);
      }

      return json({
        production: state.production,
        manifest: state.manifest,
        groups: groupMap,
        announce: announceMap,
      });
    }

    /* GET /api/projects — list available projects */
    if (path === "/api/projects" && method === "GET") {
      const { readdir } = await import("node:fs/promises");
      try {
        const entries = await readdir(projectDir);
        const projects: string[] = [];
        for (const entry of entries) {
          if (entry.endsWith("-audio")) {
            const name = entry.replace(/-audio$/, "");
            const xmlFile = Bun.file(`${projectDir}/${entry}/production.xml`);
            if (await xmlFile.exists()) {
              projects.push(name);
            }
          }
        }
        return json({ projects });
      } catch {
        return json({ projects: [] });
      }
    }

    /* POST /api/generate — generate audio for lines (group-aware) */
    if (path === "/api/generate" && method === "POST") {
      const body = await req.json() as { project: string; lineIds?: string[] };
      const state = await loadProject(body.project);
      if (!state) return err("Project not found", 404);

      const groups = buildGroups(state.production);

      /* Determine which groups to generate */
      let toGenerate: LineGroup[];
      if (body.lineIds) {
        /* Find all groups containing any requested line, deduplicate */
        const seen = new Set<string>();
        toGenerate = [];
        for (const lineId of body.lineIds) {
          const group = findGroupForLine(groups, lineId);
          if (group && !seen.has(group.groupId)) {
            seen.add(group.groupId);
            toGenerate.push(group);
          }
        }
      } else {
        toGenerate = groups;
      }

      /* Generate sequentially to avoid rate limits */
      const results: { groupId: string; lineIds: string[]; success: boolean; error?: string }[] = [];
      for (const group of toGenerate) {
        const result = await generateGroup(state, group);
        results.push({
          groupId: group.groupId,
          lineIds: group.lines.map(e => e.line.id),
          ...result,
        });
      }

      return json({ results });
    }

    /* POST /api/redo/:lineId — redo with feedback */
    const redoMatch = path.match(/^\/api\/redo\/([^/]+)$/);
    if (redoMatch && method === "POST") {
      const body = await req.json() as { project: string; feedback: string };
      const state = await loadProject(body.project);
      if (!state) return err("Project not found", 404);

      const result = await redoLine(state, redoMatch[1], body.feedback);
      return json(result);
    }

    /* GET /api/audio/:project/:filename — serve audio file */
    const audioMatch = path.match(/^\/api\/audio\/([^/]+)\/(.+)$/);
    if (audioMatch && method === "GET") {
      const audioPath = `${projectDir}/${audioMatch[1]}-audio/audio/${audioMatch[2]}`;
      const file = Bun.file(audioPath);
      if (!(await file.exists())) return err("Audio file not found", 404);
      return new Response(file, {
        headers: { "Content-Type": "audio/mpeg" },
      });
    }

    /* GET /api/mix/:project — serve full mix (with optional download) */
    const mixMatch = path.match(/^\/api\/mix\/([^/]+)$/);
    if (mixMatch && method === "GET") {
      const mixPath = `${projectDir}/${mixMatch[1]}-audio/output/full-mix.mp3`;
      const file = Bun.file(mixPath);
      if (!(await file.exists())) return err("Mix not found", 404);
      const dl = new URL(req.url).searchParams.get("dl");
      const headers: Record<string, string> = { "Content-Type": "audio/mpeg" };
      if (dl) {
        headers["Content-Disposition"] = `attachment; filename="${mixMatch[1]}-full-mix.mp3"`;
      }
      return new Response(file, { headers });
    }

    /* POST /api/stitch — stitch all audio */
    if (path === "/api/stitch" && method === "POST") {
      const body = await req.json() as { project: string; defaultGapMs?: number };
      const state = await loadProject(body.project);
      if (!state) return err("Project not found", 404);

      const gapMs = typeof body.defaultGapMs === "number" ? body.defaultGapMs : 300;
      const result = await stitchAudio(state, gapMs);
      return json(result);
    }

    /* GET /api/voices — list available TTS voices */
    if (path === "/api/voices" && method === "GET") {
      return json({ voices: tts.availableVoices() });
    }

    /* POST /api/cast/:project — update a character's voice/baseline */
    const castMatch = path.match(/^\/api\/cast\/([^/]+)$/);
    if (castMatch && method === "POST") {
      const body = await req.json() as { characterId: string; voice?: string; baseline?: string };
      const state = await loadProject(castMatch[1]);
      if (!state) return err("Project not found", 404);

      const character = state.production.cast.find(c => c.id === body.characterId);
      if (!character) return err("Character not found", 404);

      if (body.voice !== undefined) character.voice = body.voice;
      if (body.baseline !== undefined) character.baseline = body.baseline;

      await saveProductionXML(state);

      return json({
        success: true,
        character: { id: character.id, name: character.name, voice: character.voice, baseline: character.baseline },
      });
    }

    /* POST /api/announce/:project — toggle announce for a line */
    const announceMatch = path.match(/^\/api\/announce\/([^/]+)$/);
    if (announceMatch && method === "POST") {
      const body = await req.json() as { lineId: string; announce: boolean };
      const state = await loadProject(announceMatch[1]);
      if (!state) return err("Project not found", 404);

      if (!state.manifest.announceOverrides) {
        state.manifest.announceOverrides = {};
      }

      /* Check if this matches the default — if so, remove override */
      const defaults = computeAnnounceDefaults(state.production);
      const defaultVal = defaults[body.lineId] || false;
      if (body.announce === defaultVal) {
        delete state.manifest.announceOverrides[body.lineId];
      } else {
        state.manifest.announceOverrides[body.lineId] = body.announce;
      }

      await saveManifest(state);
      return json({ success: true, lineId: body.lineId, announce: body.announce });
    }

    /* POST /api/convert — markdown→XML via LLM (Phase 3 stub) */
    if (path === "/api/convert" && method === "POST") {
      return err("Not implemented yet — Phase 3", 501);
    }

    return err("Unknown API route", 404);
  }

  /* ── Start server ── */

  const server = Bun.serve({
    port,
    fetch(req, server) {
      /* WebSocket upgrade */
      if (new URL(req.url).pathname === "/ws") {
        if (server.upgrade(req)) return;
        return new Response("WebSocket upgrade failed", { status: 500 });
      }
      return handleRequest(req);
    },
    websocket: {
      open(ws) {
        wsClients.add(ws);
      },
      close(ws) {
        wsClients.delete(ws);
      },
      message() { /* client→server messages not used yet */ },
    },
  });

  console.log(`Vox server running at http://localhost:${server.port}`);
  return server;
}
