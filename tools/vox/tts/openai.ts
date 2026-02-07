/**
 * vox/tts/openai.ts — OpenAI gpt-4o-mini-tts provider
 *
 * Uses the gpt-4o-mini-tts model with `instructions` parameter.
 * ~25x cheaper than tts-1 ($0.60/1M tokens vs $15/1M chars).
 */

import type { TTSProvider, TTSGenerateRequest, TTSGenerateResult } from "../types.ts";

const API_URL = "https://api.openai.com/v1/audio/speech";

const VOICES = [
  "alloy", "ash", "ballad", "cedar", "coral", "echo",
  "fable", "marin", "nova", "onyx", "sage", "shimmer", "verse",
];

export class OpenAITTSProvider implements TTSProvider {
  name = "openai-gpt-4o-mini-tts";
  private apiKey: string;

  constructor(apiKey: string) {
    this.apiKey = apiKey;
  }

  availableVoices(): string[] {
    return [...VOICES];
  }

  async generate(req: TTSGenerateRequest): Promise<TTSGenerateResult> {
    try {
      const res = await fetch(API_URL, {
        method: "POST",
        headers: {
          Authorization: `Bearer ${this.apiKey}`,
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          model: "gpt-4o-mini-tts",
          input: req.text,
          voice: req.voice,
          instructions: req.instructions,
          response_format: "mp3",
        }),
      });

      if (!res.ok) {
        const errText = await res.text();
        return {
          success: false,
          bytesWritten: 0,
          error: `OpenAI API error (${res.status}): ${errText}`,
        };
      }

      const audioBuffer = await res.arrayBuffer();
      await Bun.write(req.outputPath, new Uint8Array(audioBuffer));

      return {
        success: true,
        bytesWritten: audioBuffer.byteLength,
      };
    } catch (err) {
      return {
        success: false,
        bytesWritten: 0,
        error: err instanceof Error ? err.message : String(err),
      };
    }
  }
}
