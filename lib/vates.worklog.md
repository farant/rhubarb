# vates worklog

## 2026-10-08 - part A: neutral model + Anthropic request body (vates-plan-2 T3)

Spec: project-specs/vates-spec.md (incl. §I.4 "config speaks the
provider's language, code none"); header approved by Fran (7b5ca52b).

**Render rules** (`vates_anthropic_corpus`, pure, deterministic):
- body key order fixed: model, max_tokens, thinking (only when showing
  summaries: adaptive + display summarized), output_config (only with
  effort), cache_control (tail breakpoint), system, tools, messages,
  then extras TAGGED "anthropic" (last; other providers' extras skipped).
- a block whose `crudum` came from provider "anthropic" is emitted
  VERBATIM (unknown fields survive); with `signum_thesauri` a shallow
  copy + cache_control is emitted for text/tool_use/tool_result. The
  crudum itself is never mutated. OPACUM never gets cache_control.
- a block whose crudum is from another provider: rebuilt from neutral
  fields if text/tool_use/tool_result, OMITTED if OPACUM.
- a message left with no blocks is omitted (an empty content array is a
  400 at the API).
- json_scribere emits raw UTF-8 (no \u escapes): the golden with
  macrons, a cross and a non-BMP letter matched byte for byte.

**Plants** (4/4 red): signum ignored, foreign OPACUM emitted, empty
message kept, extras emitted regardless of provider.

**Rulings**: `_responsum_vacuum` and `_signa_numerare` moved out of part A
(unused until T4/T5 -> -Werror unused-function); `_blocos_anthropic` ->
`_contentum_anthropic` (lint: "blocos" unknown).

## 2026-10-08 - part B: reading Anthropic responses (vates-plan-2 T4)

`_legere(status, corpus, piscina, &novitas)` (pure; `vates_anthropic_legere`
is the public face that ignores novitas):
- EVERY content block keeps `crudum` + `provisor = "anthropic"`; genus:
  text -> TEXTUS (textus filled), tool_use -> PETITUM (id, name, input),
  everything else -> OPACUM. Unknown types (not thinking /
  redacted_thinking) are NOVITAS "blocus ignotus: <type>" - kept, never
  dropped.
- known keys: top level {id,type,role,model,content,stop_reason,
  stop_sequence,stop_details,usage}; text {type,text,citations};
  tool_use {type,id,name,input}. Any other key = NOVITAS (the block stays
  usable; crudum preserves the key for re-emission). Only the FIRST
  novelty per response is reported (one press per response).
- stop_reason map end_turn/stop_sequence/tool_use/max_tokens/refusal/
  pause_turn; anything else -> ALIA + NOVITAS. refusal category from
  stop_details.category.
- usage: cache_creation.ephemeral_{5m,1h} when present, else all of
  cache_creation_input_tokens counts as 5m.
- status != 200 -> STATUS (error.type / error.message, or the first
  CCLVI bytes of a non-JSON body); 200 + unparseable -> PARSE.

Round trips proven in the tests: a thinking block with its signature +
tool_use come back byte-exact in the next request; a text block with
`citations` keeps them (via crudum). Plants 4/4 red: unknown block
dropped, cache_creation ignored, error.message in error_genus, crudum
ignored for TEXTUS. `_responsum_vacuum` re-inserted here (moved out of
part A in T3).
