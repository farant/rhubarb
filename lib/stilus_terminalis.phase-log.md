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
