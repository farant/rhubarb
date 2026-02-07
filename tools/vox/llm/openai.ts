/**
 * vox/llm/openai.ts — OpenAI GPT LLM provider
 *
 * Used for: redo feedback → new delivery instructions, markdown→XML conversion
 */

import type { LLMProvider, LLMCompleteRequest, LLMCompleteResult } from "../types.ts";

const API_URL = "https://api.openai.com/v1/chat/completions";

export class OpenAILLMProvider implements LLMProvider {
  name = "openai-gpt";
  private apiKey: string;
  private model: string;

  constructor(apiKey: string, model: string = "gpt-4o-mini") {
    this.apiKey = apiKey;
    this.model = model;
  }

  async complete(req: LLMCompleteRequest): Promise<LLMCompleteResult> {
    try {
      const res = await fetch(API_URL, {
        method: "POST",
        headers: {
          Authorization: `Bearer ${this.apiKey}`,
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          model: this.model,
          messages: [
            { role: "system", content: req.systemPrompt },
            { role: "user", content: req.userMessage },
          ],
          max_tokens: req.maxTokens || 1024,
          temperature: 0.7,
        }),
      });

      if (!res.ok) {
        const errText = await res.text();
        return {
          content: "",
          success: false,
          error: `OpenAI API error (${res.status}): ${errText}`,
        };
      }

      const data = await res.json() as {
        choices: { message: { content: string } }[];
      };

      return {
        content: data.choices[0]?.message?.content || "",
        success: true,
      };
    } catch (err) {
      return {
        content: "",
        success: false,
        error: err instanceof Error ? err.message : String(err),
      };
    }
  }
}
