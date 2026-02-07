/**
 * vox/types.ts — All interfaces and types for the Vox audio production tool
 */

/* ── Production Script Types ── */

export interface Character {
  id: string;
  name: string;
  voice: string;
  baseline: string;
}

export interface Line {
  id: string;
  type: "narration" | "dialogue" | "sfx";
  character: string;
  text: string;
  delivery: string;
}

export interface Pause {
  ms: number;
}

export type ScriptElement = Line | Pause;

export function isLine(el: ScriptElement): el is Line {
  return "id" in el;
}

export function isPause(el: ScriptElement): el is Pause {
  return "ms" in el && !("id" in el);
}

export interface Section {
  title: string;
  context: string;
  elements: ScriptElement[];
}

export interface Production {
  title: string;
  episode: string;
  cast: Character[];
  sections: Section[];
}

/* ── Manifest (per-line status tracking) ── */

export interface LineVersion {
  version: number;
  delivery: string;
  feedback: string | null;
  audioFile: string;
  timestamp: string;
}

export interface LineStatus {
  lineId: string;
  character: string;
  currentVersion: number;
  versions: LineVersion[];
  groupId?: string;        /* e.g. "g001-002" — shared by all lines in the group */
}

export interface Manifest {
  project: string;
  episode: string;
  createdAt: string;
  updatedAt: string;
  lines: Record<string, LineStatus>;
  announceOverrides?: Record<string, boolean>;  /* lineId → true/false, overrides computed default */
}

/* ── TTS Provider ── */

export interface TTSGenerateRequest {
  text: string;
  voice: string;
  instructions: string;
  outputPath: string;
}

export interface TTSGenerateResult {
  success: boolean;
  bytesWritten: number;
  error?: string;
}

export interface TTSProvider {
  name: string;
  availableVoices(): string[];
  generate(req: TTSGenerateRequest): Promise<TTSGenerateResult>;
}

/* ── LLM Provider ── */

export interface LLMCompleteRequest {
  systemPrompt: string;
  userMessage: string;
  maxTokens?: number;
}

export interface LLMCompleteResult {
  content: string;
  success: boolean;
  error?: string;
}

export interface LLMProvider {
  name: string;
  complete(req: LLMCompleteRequest): Promise<LLMCompleteResult>;
}

/* ── WebSocket Messages ── */

export type WSMessage =
  | { type: "progress"; lineId: string; status: "generating" | "done" | "error"; message?: string }
  | { type: "redo"; lineId: string; version: number }
  | { type: "stitch"; status: "started" | "done" | "error"; message?: string };
