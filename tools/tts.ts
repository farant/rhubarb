/**
 * tts.ts — Text-to-Speech via OpenAI API
 *
 * Usage: bun tools/tts.ts <input.md> [output.mp3]
 *
 * Reads a text file, chunks it to fit OpenAI's 4096 char limit,
 * calls the TTS API for each chunk, concatenates the mp3 output.
 *
 * Environment: OPENAI_API_KEY must be set.
 * Voice: onyx (default), override with --voice <name>
 * Model: tts-1 (default), use --hd for tts-1-hd
 */

const CHUNK_LIMIT = 4096;
const API_URL = "https://api.openai.com/v1/audio/speech";

function parseArgs(): {
  input: string;
  output: string;
  voice: string;
  model: string;
} {
  const args = Bun.argv.slice(2);
  let voice = "onyx";
  let model = "tts-1";
  const positional: string[] = [];

  for (let i = 0; i < args.length; i++) {
    if (args[i] === "--voice" && args[i + 1]) {
      voice = args[++i];
    } else if (args[i] === "--hd") {
      model = "tts-1-hd";
    } else {
      positional.push(args[i]);
    }
  }

  if (positional.length < 1) {
    console.error("Usage: bun tools/tts.ts <input.md> [output.mp3]");
    console.error("  --voice <name>   Voice (default: onyx)");
    console.error("  --hd             Use tts-1-hd model");
    process.exit(1);
  }

  const input = positional[0];
  const output =
    positional[1] || input.replace(/\.[^.]+$/, "") + ".mp3";

  return { input, output, voice, model };
}

/** Split text into chunks at sentence/paragraph boundaries */
function chunkText(text: string): string[] {
  const chunks: string[] = [];
  let remaining = text;

  while (remaining.length > 0) {
    if (remaining.length <= CHUNK_LIMIT) {
      chunks.push(remaining);
      break;
    }

    /* Try to split at paragraph boundary */
    let splitAt = remaining.lastIndexOf("\n\n", CHUNK_LIMIT);

    /* Fall back to sentence boundary */
    if (splitAt < CHUNK_LIMIT / 2) {
      splitAt = remaining.lastIndexOf(". ", CHUNK_LIMIT);
      if (splitAt > 0) splitAt += 1; /* keep the period */
    }

    /* Fall back to newline */
    if (splitAt < CHUNK_LIMIT / 2) {
      splitAt = remaining.lastIndexOf("\n", CHUNK_LIMIT);
    }

    /* Last resort: hard cut at limit */
    if (splitAt < CHUNK_LIMIT / 2) {
      splitAt = CHUNK_LIMIT;
    }

    chunks.push(remaining.slice(0, splitAt).trim());
    remaining = remaining.slice(splitAt).trim();
  }

  return chunks.filter((c) => c.length > 0);
}

async function ttsChunk(
  text: string,
  voice: string,
  model: string,
  apiKey: string
): Promise<ArrayBuffer> {
  const res = await fetch(API_URL, {
    method: "POST",
    headers: {
      Authorization: `Bearer ${apiKey}`,
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      model,
      input: text,
      voice,
      response_format: "mp3",
    }),
  });

  if (!res.ok) {
    const err = await res.text();
    throw new Error(`OpenAI API error (${res.status}): ${err}`);
  }

  return res.arrayBuffer();
}

async function main() {
  const { input, output, voice, model } = parseArgs();

  const apiKey = process.env.OPENAI_API_KEY;
  if (!apiKey) {
    console.error("Error: OPENAI_API_KEY environment variable not set.");
    process.exit(1);
  }

  /* Read input file */
  const file = Bun.file(input);
  if (!(await file.exists())) {
    console.error(`Error: file not found: ${input}`);
    process.exit(1);
  }
  const text = await file.text();

  console.log(`Input:  ${input} (${text.length} chars)`);
  console.log(`Output: ${output}`);
  console.log(`Voice:  ${voice}  Model: ${model}`);

  /* Chunk */
  const chunks = chunkText(text);
  console.log(`Chunks: ${chunks.length}`);

  /* Generate audio for each chunk */
  const audioParts: ArrayBuffer[] = [];
  for (let i = 0; i < chunks.length; i++) {
    console.log(
      `  [${i + 1}/${chunks.length}] ${chunks[i].length} chars...`
    );
    const audio = await ttsChunk(chunks[i], voice, model, apiKey);
    audioParts.push(audio);
  }

  /* Concatenate mp3 chunks */
  const totalSize = audioParts.reduce((s, a) => s + a.byteLength, 0);
  const combined = new Uint8Array(totalSize);
  let offset = 0;
  for (const part of audioParts) {
    combined.set(new Uint8Array(part), offset);
    offset += part.byteLength;
  }

  await Bun.write(output, combined);
  console.log(`Done! ${(totalSize / 1024).toFixed(0)} KB written to ${output}`);
}

main();
