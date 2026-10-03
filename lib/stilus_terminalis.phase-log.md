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

## T3 — tessera emits through the codec (2026-10-03)

tessera's `_stilum_emittere` is now a lossless conversion plus
`stilus_codificare(aed, NIHIL, &stilus, PLENA|CCLVI)`; its private
quantizer `_cclvi` (the original of `stilus_quantizare`) and colour
writer are gone. The bar held: tessera 15/15, hospes 7/7 faithful, no
golden changed.

**The bar was weaker than the plan assumed.** The planned plant (the
conversion drops one ornament — strikethrough) passed 15/15: tessera's
goldens pin colour bytes but no test had ever pinned an ornament's bytes.
New section IV in `probatio_tessera_colores.c` pins each of the six and
all six with both colours (17 params, tessera's maximum) in both depths.
Run against HEAD's pre-codec `tessera_opus.c` too: green, so the pins
record the OLD behaviour. Plant re-run: 3 named failures.

**Vendoring exposed a tool bug.** The usual rename (`stilus_` →
`tessera_stilus_`) collides with tessera's own `tessera_stilus_nativus` /
`_aequalis`, so the prefix is `tessera_stilus_terminalis_`. The
excludenda harvester reversed names by stripping `tessera_` only and
CONVERGED FALSELY on fictional names (`stilus_terminalis_applicare`);
only the amalgam's -Werror build noticed. Fran chose fixing the tool:
the amalgamator prints its rename table, the harvester reverses through
it, and a listed name that returns is a named fracture. Detail in
tools/amalgamatio.worklog.md.

Also: tessera's test runner lists its lib sources by hand —
`stilus_terminalis` added (link failure until then).

The first commit attempt was stopped by `generata`: tessera's four tools
(effigies, folium, musivum, spectaculum) have generated source lists
from their include closures, now owing `stilus_terminalis`; regenerated
by each snippet's own `# regeneratio:` line, all four built (TUIs built
with `exec` shadowed, musivum with AEDIFICARE_SOLUM). The tessera gate
does not build those tools - generata was the only witness.

## T4 — RELATIO (2026-10-03)

**Module 004 is done; module 005 (tessera reshaped) is complete with it**
(its last row, SGR emit). terminal-planning ae7392c: both `completed_`
with Outcome sections, README rows and every link updated.

**What exists:** `include/stilus_terminalis.h` + `lib/stilus_terminalis.c`
— one style model (three colours of three kinds, eight flags, six
underline kinds), encode (full reset, PLENA or CCLVI, split past 24
params), decode (Ghostty `sgr.zig`, unknowns counted, non-SGR CSIs
ignored), the xterm 256 palette, and the quantizer tessera used to own.
tessera emits through it; the amalgam vendors it.

**Proof:** 117 codec assertions (31 ported Ghostty vectors, the round
trip over 9,216 styles, split and quantizer pins); tessera 15/15 with
bytes unchanged plus new ornament pins (green on the pre-codec code
too); hospes 7/7; saltuarius 13/13 against the new amalgam (the
first-host proof module 005 promised). Plants caught by name in every
task: ':' read as ';', 22 clearing bold only, no split, conversion
dropping strikethrough (after the pins), empty rename table in the
harvester.

**What the work found, in order of weight:**
1. A full style cannot be ONE SGR sequence in a real terminal (26 > 24
   params). Nobody had sent one; the round trip did.
2. tessera's "goldens unchanged" bar never covered ornaments. A bar is
   only as good as what it pins; the plant showed it before anything
   could slip through.
3. The excludenda harvester could converge on names that do not exist
   ("no new names" stood in for "no warnings"). Fixed in the tool, not
   worked around, by Fran's choice.
4. Generated tool source lists (four tessera instruments) owe
   regeneration when a library's closure grows; only `generata`
   witnesses them.

**Compared with the plan:** tasks as planned; T3 grew by the harvester
fix (asked), the ornament pins and the four tool lists. Divergences
from Ghostty are counting-only and documented in T2.

**Deferred:** a minimal-diff encoder (`prior` given emits a full reset
when different; measure against tessera's `fructus` first); tessera
gaining curly/coloured underline (tessera's decision); the emulator
(module 006) is decode's first real consumer.
