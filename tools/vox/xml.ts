/**
 * vox/xml.ts — Hand-rolled XML parser for production scripts
 *
 * Parses our specific schema only. No dependencies.
 * Handles: <production>, <cast>, <character>, <script>,
 *          <section>, <line>, <text>, <delivery>, <pause>
 */

import type { Production, Character, Section, Line, Pause, ScriptElement } from "./types.ts";

/** Extract attribute value from a tag string */
function attr(tag: string, name: string): string {
  /* Match name="value" or name='value' */
  const re = new RegExp(`${name}\\s*=\\s*"([^"]*)"`, "i");
  const m = tag.match(re);
  if (m) return m[1];
  const re2 = new RegExp(`${name}\\s*=\\s*'([^']*)'`, "i");
  const m2 = tag.match(re2);
  return m2 ? m2[1] : "";
}

/** Get inner text content between matching open/close tags */
function innerText(xml: string, tagName: string, startFrom: number): { text: string; end: number } | null {
  const openPattern = `<${tagName}`;
  const closePattern = `</${tagName}>`;

  const openIdx = xml.indexOf(openPattern, startFrom);
  if (openIdx < 0) return null;

  /* Find end of opening tag */
  const tagEnd = xml.indexOf(">", openIdx);
  if (tagEnd < 0) return null;

  /* Self-closing? */
  if (xml[tagEnd - 1] === "/") {
    return { text: "", end: tagEnd + 1 };
  }

  const closeIdx = xml.indexOf(closePattern, tagEnd);
  if (closeIdx < 0) return null;

  return {
    text: xml.slice(tagEnd + 1, closeIdx).trim(),
    end: closeIdx + closePattern.length,
  };
}

/** Find all occurrences of a tag (including self-closing) */
function findTags(xml: string, tagName: string, startFrom: number = 0, endAt?: number): string[] {
  const results: string[] = [];
  const region = endAt !== undefined ? xml.slice(0, endAt) : xml;
  let pos = startFrom;

  while (true) {
    const idx = region.indexOf(`<${tagName}`, pos);
    if (idx < 0) break;

    const end = region.indexOf(">", idx);
    if (end < 0) break;

    results.push(region.slice(idx, end + 1));
    pos = end + 1;
  }

  return results;
}

/** Parse a <character .../> tag */
function parseCharacter(tag: string): Character {
  return {
    id: attr(tag, "id"),
    name: attr(tag, "name"),
    voice: attr(tag, "voice"),
    baseline: attr(tag, "baseline"),
  };
}

/** Parse a single <section> block */
function parseSection(xml: string, sectionStart: number): { section: Section; end: number } | null {
  const openIdx = xml.indexOf("<section", sectionStart);
  if (openIdx < 0) return null;

  const closeIdx = xml.indexOf("</section>", openIdx);
  if (closeIdx < 0) return null;

  const tagEnd = xml.indexOf(">", openIdx);
  const sectionTag = xml.slice(openIdx, tagEnd + 1);
  const sectionBody = xml.slice(tagEnd + 1, closeIdx);

  const title = attr(sectionTag, "title");
  const context = attr(sectionTag, "context");

  const elements: ScriptElement[] = [];

  /* Scan through section body for <line> and <pause> elements */
  let pos = 0;
  while (pos < sectionBody.length) {
    const nextLine = sectionBody.indexOf("<line", pos);
    const nextPause = sectionBody.indexOf("<pause", pos);

    /* Determine which comes first */
    let lineFirst = nextLine >= 0;
    let pauseFirst = nextPause >= 0;

    if (lineFirst && pauseFirst) {
      lineFirst = nextLine < nextPause;
      pauseFirst = !lineFirst;
    }

    if (!lineFirst && !pauseFirst) break;

    if (lineFirst && nextLine >= 0) {
      /* Parse <line> */
      const lineTagEnd = sectionBody.indexOf(">", nextLine);
      const lineTag = sectionBody.slice(nextLine, lineTagEnd + 1);
      const lineClose = sectionBody.indexOf("</line>", lineTagEnd);

      if (lineClose < 0) { pos = lineTagEnd + 1; continue; }

      const lineBody = sectionBody.slice(lineTagEnd + 1, lineClose);

      const textResult = innerText(lineBody, "text", 0);
      const deliveryResult = innerText(lineBody, "delivery", 0);

      const line: Line = {
        id: attr(lineTag, "id"),
        type: (attr(lineTag, "type") || "dialogue") as Line["type"],
        character: attr(lineTag, "character"),
        text: textResult ? textResult.text : "",
        delivery: deliveryResult ? deliveryResult.text : "",
      };

      elements.push(line);
      pos = lineClose + "</line>".length;
    } else if (pauseFirst && nextPause >= 0) {
      /* Parse <pause ms="..."/> */
      const pauseTagEnd = sectionBody.indexOf(">", nextPause);
      const pauseTag = sectionBody.slice(nextPause, pauseTagEnd + 1);
      const ms = parseInt(attr(pauseTag, "ms"), 10) || 1000;
      elements.push({ ms } as Pause);
      pos = pauseTagEnd + 1;
    }
  }

  return {
    section: { title, context, elements },
    end: closeIdx + "</section>".length,
  };
}

/** Parse a complete production XML string */
export function parseProductionXML(xml: string): Production {
  /* Extract <production> attributes */
  const prodTag = xml.match(/<production[^>]*>/);
  const title = prodTag ? attr(prodTag[0], "title") : "";
  const episode = prodTag ? attr(prodTag[0], "episode") : "";

  /* Parse <cast> */
  const cast: Character[] = [];
  const characterTags = findTags(xml, "character");
  for (const tag of characterTags) {
    cast.push(parseCharacter(tag));
  }

  /* Parse <section> elements */
  const sections: Section[] = [];
  const scriptStart = xml.indexOf("<script");
  if (scriptStart >= 0) {
    let pos = scriptStart;
    while (true) {
      const result = parseSection(xml, pos);
      if (!result) break;
      sections.push(result.section);
      pos = result.end;
    }
  }

  return { title, episode, cast, sections };
}

/** Build TTS instructions from three layers: baseline + context + delivery */
export function buildInstructions(
  character: Character | undefined,
  section: Section | undefined,
  line: Line
): string {
  const parts: string[] = [];
  if (character?.baseline) parts.push(character.baseline);
  if (section?.context) parts.push(`Scene: ${section.context}`);
  if (line.delivery) parts.push(line.delivery);
  return parts.join(" ");
}

/** Get all lines from a production, flattened */
export function allLines(production: Production): { line: Line; section: Section }[] {
  const result: { line: Line; section: Section }[] = [];
  for (const section of production.sections) {
    for (const el of section.elements) {
      if ("id" in el) {
        result.push({ line: el as Line, section });
      }
    }
  }
  return result;
}

/** Look up a character by id */
export function findCharacter(production: Production, characterId: string): Character | undefined {
  return production.cast.find(c => c.id === characterId);
}
