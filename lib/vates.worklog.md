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

## 2026-10-08 - part C: providers, vates_mittere, ledger, fictus (vates-plan-2 T5)

- **Provider table** compiled in: {"anthropic", vates_anthropic_aperire},
  {"fictus", ...}. `vates_aperire` by name; unknown -> NIHIL + stderr
  listing the known names (seen in the test: "provisor ignotus 'openai'
  - noti: anthropic fictus").
- **The key**: copied ONCE into the Vates (NUL-terminated for the header).
  http_petitio_caput_addere necessarily copies it into the call's piscina
  too (http's API). Proven absent from the ledger, herbarium index,
  every specimen file and the error text (probatio_clavis_non_effunditur:
  a distinctive key through a scripted vectura, every written file read
  back). Plant (key as a ledger field) red.
- **Retry**: RETE always; TEMPUS on the first attempt only (a 10-minute
  call is expensive to repeat); HTTP 408/409/429/>=500. Wait =
  retry-after seconds if present else mora_iterandi_ms, ALWAYS capped by
  mora_iterandi_maxima_ms (a hostile retry-after 3600 with a 5 ms cap:
  the whole test runs in 0.3 s; a cap 1000x too large is red).
- **Usage** on the returned response = sum over attempts; each attempt is
  its own ledger line (write + fsync); price -1 when the model is not in
  the caller's table. Worked example 1000/2000/300/0/50 tokens at
  $4/$0.20/$5/$8/$20 per MTok = 6900 micro-dollars (asserted).
- **Breakpoints**: > IV -> LIMES before anything is sent (fictus saw 0
  requests).
- **fictus** = the Anthropic backend over a scripted vectura: replies are
  rendered as real Anthropic JSON and parsed by the real parser; the sent
  body equals `vates_anthropic_corpus` byte for byte.
- **Novelty -> herbarium**: an unknown block on HTTP 200 is pressed with
  causa "blocus ignotus: server_tool_use" (test; plant red).
- Masking for the Anthropic kind key is a copy of herbarium's (the
  approved header does not export it). `piscina == NIHIL` -> NIHIL, the
  one exception to "never NIHIL".
- Lint renames: hr->resultus_http, ho->optiones_herbarii,
  in_cifris->in_numeris; glossary entry herbarium gained the form
  "herbarii".

## 2026-10-08 - committed specimens + the sweep (vates-plan-2 T6)

`probationes/fixa/vates/herbarium/specimina/` holds eight hand-made seeds
in herbarium's exact format (semen_<kind>-1.json): 529 overloaded, 429
rate_limit (with retry-after), 400 invalid_request, 401 authentication,
500 api_error, 200 refusal (category cyber), 200 max_tokens, 200 with an
unknown block (server_tool_use). `probatio_specimina_commissa` replays
EVERY committed specimen through `herbarium_reddens` +
`vates_anthropic_legere` and demands a NAMED outcome: non-200 ->
VATES_ERROR_STATUS with a non-empty error_genus; 200 -> success with a
causa_finis other than ALIA. Presence is asserted (> 0), not a fixed
count - the set is meant to grow.

**Promotion (how the set grows):** live capture lands in the caller's
herbarium directory (optchat: ../optchat-acta/herbarium, tools/vates_fumus:
build/vates_fumus_herbarium). Read the specimen (error bodies can quote
the request back), then `cp` it here unchanged. If the sweep turns red,
the API sent something vates cannot name yet: teach the parser, then the
suite is green again. Plant: a seed with stop_reason "novum_finis" -> red
at :733 (ALIA = unnamed), removed.

## 2026-10-08 - first live run (vates-plan-2 T7, tools/vates_fumus)

claude-sonnet-5-5, 10:27Z, system prompt 3851 bytes (1839 tokens):

| call | what | in | cache read | cache write | out | udollaria | ms |
|---|---|---|---|---|---|---|---|
| A | first call, system marked | 18 | 0 | 1839 | 6 | 4693 | 1975 |
| B | identical prefix | 18 | **1839** | 0 | 6 | **463** | 1437 |
| C | thinking, effort high | 34 | 0 | 0 | 138 | 1448 | 3695 |
| D | C's thinking block re-sent + new turn | 196 | 0 | 0 | 58 | 972 | 3227 |

Total 7576 udollaria ($0.0076). Cache read ~10x cheaper than the write
call. D proves OPACUM re-emission: the API accepted the thinking
signature from a re-serialized JsonValor (vates-spec AUDIENDA "OPACUM
re-serialization": RESOLVED - harmless). Ledger: 4 lines = 4 attempts.
Key-leak grep (Fran): 0 in stdout, ledger, herbarium (4 files).

**herbarium caught the API's real shape on the first live call:** every
200 carries top-level `container` and `diagnostics` (unknown to
`_claves_summae`, novelty "campus ignotus: container" - `diagnostics`
hidden because only the first novelty is reported) and usage extras
`output_tokens_details`, `service_tier`, `inference_geo` (usage keys are
not novelty-checked). -> T7b.
