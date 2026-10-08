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
