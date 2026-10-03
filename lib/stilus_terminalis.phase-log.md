# stilus_terminalis — phase log (module 004, the style codec)

Plan: `project-specs/stilus-terminalis-plan.md`. Sketch:
`../terminal-planning/modules/004-style-codec.md`.

## T1 — the core: types, encode, palette, quantizing (2026-10-03)

**Shape (Fran's decisions):** `stilus_terminalis`; `StilusTerminalis`
= fg / bg / underline colour (`StilusColor`: NATIVUS / TABULA index /
RGB 0x00RRGGBB), eight attribute flags (`STILUS_*`, SGR number in each
comment), six underline kinds (`StilusSublinea`). Pure.

- **`stilus_codificare(aed, prior, novus, codificatio)`**, the canonical
  full reset: `ESC [ 0`, `;1 ;2 ;3`, the underline (`;4`, or `;4:n`
  for the other kinds), `;5 ;7 ;8 ;9 ;53`, then `;38 / ;48 / ;58`
  colours (`2;r;g;b`, or `5;n`), then `m`. For tessera's subset this is
  byte-identical to `_stilum_emittere` today, by construction. Palette
  colours always use `5;n`. `prior` exists from day one: v1 emits
  NOTHING when equal, else the full reset (correct; minimal diff
  later, measured).
- **`STILUS_CODIFICATIO_CCLVI`** quantizes RGB on encode; palette
  indices pass through.
- **`stilus_tabulae_color`**: xterm's default 256 (16 xterm ANSI
  values, the 6-level cube, 24 greys 8 + 10k).
- **`stilus_quantizare`**: tessera's `_cclvi` (quadrans Q4) moved
  here verbatim (Fran: one home); tessera's copy goes in T3.

**Tests (red first, 29 → more):** the tessera subset; all attributes in
canonical order; the five non-single underline forms; palette, RGB and
underline colours in both encodings; `prior` equal/unequal; palette
spot checks; quantizing pinned against a FROZEN copy of tessera's
`_cclvi` in the test (it outlives the original after T3) on a 16³ grid.

**A plant found a test gap.** "The cube level rounds UP at a tie"
survived: the grid steps by 17 and never hits a tie value (115, 155,
195, 235 lie halfway between cube levels), and the explicit `0x737373`
quantizes to a grey anyway. Added: the 6³ grid of tie values against
the oracle, and `0x7300FF` → 57 (cube wins, 115 → level 95). Now
caught. Two of my own hand-computed expectations were also wrong
(`0x112233` and `0x0A0B0C` are nearer a grey: 235 and 232); computed
with a Python copy of the algorithm, not by hand, from then on.

**Glossary:** `sublinea`, `superlinea` (coinages), `undulatus`,
`lineolatus` (adjectives); the 16-colour table is `PRIMAE`, not an
acronym.

## T2 — decode; the round trip finds a 24-parameter limit (2026-10-03)

**`stilus_applicare(lexema, st)`**: Ghostty's `Parser.next` (sgr.zig)
translated, each attribute applied to the current style; returns the
number of unknowns skipped. Separators come from `series_terminalis`'s
`separatores` bitset (bit i: `:` after parameter i), exactly Ghostty's
`params_sep`. Covered: reset (empty or 0); 1 2 3 5 6 7 8 9 53 and their
resets 22 (bold AND faint) 23 25 27 28 29 55, as a small table; 4,
`4:0`-`4:5`, 21, 24; 30-37 / 40-47 / 90-97 / 100-107 as palette 0-15;
39 49 59; 38/48/58 with `5;n`, `2;r;g;b`, `2:r:g:b`, `2::r:g:b` /
`2:cs:r:g:b`. A colon after any other parameter makes that whole colon
run ONE unknown. A CSI with a private marker or intermediates is not
SGR and changes nothing: `ESC[>4;2m` (xterm modifyOtherKeys) must never
read as underline + faint.

**Two deliberate divergences from Ghostty (counting only):**
- a truncated colour (`38;5`, `38;2;44;52`) consumes the REST of the
  parameters as one unknown; Ghostty re-reads the leftover `5` as blink
  and `2` as faint;
- a malformed colon run after 58 (`58:4:`) is ONE unknown, where
  Ghostty counts two.
Ghostty's tests for these assert only "no crash", so all 31 ports hold.

**The round trip found a real encoder bug.** `applicare(codificare(s))
== s` over 9,216 generated styles failed for the fullest ones: every
attribute plus a `4:n` underline plus three RGB colours is 26
parameters (a colon sub-parameter counts), and Ghostty's CSI parser,
like ours, caps a CSI at 24 (`MAX_PARAMS`). The whole SGR would be
DISCARDED by a real terminal. The encoder now writes parameters in
GROUPS that are never split (`38;2;r;g;b` is one group) and starts a
second sequence (`m ESC [`, no leading 0) when the next group would
exceed 24. tessera's subset needs at most 17, so its goldens are
unaffected. A vector pins the split; the test helper now applies every
token, as a terminal would.

Also: a test assertion of mine checked "whole style native" where only
the colours were reset (bold was still set from an earlier line). Fixed.

Tests: 117. Plants caught: `:` read as `;` (18 failures); 22 clearing
only bold (re-run after the table refactor); the encoder never
splitting.
