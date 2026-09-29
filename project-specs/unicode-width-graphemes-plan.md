# Unicode core, first brick: width + graphemes — plan

*2026-09-28. The first layer of the text stack
(`project-specs/text-stack-visio.md`, "Unicode core"), pulled by a
concrete consumer: tessera assumes every rune is one column. Sources:
terminal-planning modules/001 (unicode width), features/004 (wide
characters), 005 (grapheme clusters), research/width-agreement-problem;
ledger desideratum …VRTHANR ("utf8_latitudo", 2026-09-17). Worktree
`../rhubarb-secunda`. Executed INLINE, one task per turn, Fran approving
each; tasks with a terminal step end with Fran's own look. Names marked
(unsealed) are working names; Fran names.*

**DECISA 2026-09-28: Fran approved D1–D8 as proposed** — library
`runae` over `utf8`; Unicode 15.1.0; the UCD files fetched once (Fran's
OK given for the fetch) and checked in; a C89 generator with a
staleness mode; Unicode's widths in the library + cursor-position
containment in tessera, emoji at 2; no `?2027`; clusters in tessera
decided after U5; Lapide samples checked into rhubarb.

## 1. Goal

A standalone C89 library that answers three questions from GENERATED
tables, and is correct by Unicode's own conformance files:

- how many terminal cells a codepoint takes (0, 1, 2)
- whether there is a grapheme boundary between two codepoints (UAX #29)
- how wide a whole grapheme cluster is

Then tessera consumes it (wide cells, later clusters), and a paginating
viewer over Fran's multilingual Lapide translations serves as the
real-text test corpus and the terminal look.

This is deliberately the **first brick of the text stack's Unicode
core**, not a terminal-only helper. The machinery built here (pinned
UCD files → generator → C89 tables → conformance replay → differential
oracle) is the same machinery normalization, case mapping, line
breaking and bidi will need later. Those stay parked until something
pulls them (the PULL rule); only the shape is chosen with them in mind.

**Not ICU.** ICU's hard, rotting part is locale data (CLDR: collation,
formatting); the text-stack vision already puts that in a separate
library, never in Unicode core. What Unicode core rebuilds is the part
with exact conformance files. See §6.

## 2. What exists (read 2026-09-28)

- **`include/utf8.h`** (111 lines) is a codec and boundary walker only:
  `utf8_decodere`, `utf8_codere`, `utf8_longitudo_byte`,
  `utf8_est_continuatio`, `utf8_numerare_runas`, `utf8_proxima_runa`,
  `utf8_prior_runa`. No character properties at all.
- **tessera:** `_octetos_scribere` (`tessera/fontes/tessera_opus.c`)
  puts each decoded rune in cell `cx` and does `cx++`;
  `tessera_opus.h:105` documents it ("latitudo 1 praesumpta").
  spec-v2 §6 defers wcwidth/wide/combining. Cells: `signum` packs up to
  4 UTF-8 bytes in a u32 (no spare bits); `ornamenta` uses 0x01–0x20 of
  32 bits, so the wide/continuation markers can ride there (features/004).
- **Other consumers with the same assumption:** saltuarius counts
  columns with `utf8_numerare_runas` (`saltuarius_liber.c:537`);
  `lib/excerptum` (the diagnostics caret) lands one column short after
  `広` and one column long after `e`+U+0301 (measured in …VRTHANR —
  the errors point in opposite directions, so only a table fixes both).
- **Local oracles (dev-time only, never linked into the library):**
  - ICU4C 74.2 from Homebrew (`/opt/homebrew/opt/icu4c`), **Unicode
    15.1**, CLDR 44.1: grapheme break iterator + every property.
  - Python 3.12.6 `unicodedata`, **Unicode 15.0**: east_asian_width,
    category, combining (no grapheme breaks).
  - OpenTUI's `packages/native/src/tests/unicode-width-map.zon` (3,909
    lines): OpenTUI's width POLICY, not a standard.
  - Ghostty exists only as a tarball vendored inside opentui; building
    it for a differential run means Zig. Not planned.
- **UCD text files: none in the tree.** Fetching them from unicode.org
  is a network request (house rule: ask Fran first).
- **Corpus: `../lapide`** (935 MB, 3,938 HTML files, 35 languages).
  Files per language (by `hreflang`: the unsuffixed files are
  ENGLISH, and Lapide files Latin under `_lt`): en 1,253 · la (`_lt`)
  1,251 · es/fr/it/pt 198 · de/ja/pl 56 · ar/id 55 · ko 16 · bn ceb el
  fa gu he hi hu ig ml nl ro ru rw sv sw ta th tl tr vi yo zh 10 each;
  all 35 have `01_Preliminares`. What it exercises:

  | Scripts | Languages | Stresses |
  |---|---|---|
  | CJK (wide) | ja, zh, ko | width 2, wrapping without spaces |
  | Indic (conjuncts, virama) | hi, bn, gu, ml, ta | clusters, GB9c (InCB, new in 15.1), width 0 marks |
  | Thai | th | combining marks above/below, no spaces between words |
  | Latin + combining tone marks | yo, ig, vi | combining marks on Latin |
  | RTL | ar, he, fa | bidi (terminals mostly don't; out of scope, §6) |
  | Greek, Cyrillic, Latin and Latin-extended | el, ru, la, en, pl, hu, ro, tr… | width 1 baseline, precomposed letters |

  The house HTML parser (materia's html client) can extract the text.

## 3. Decisions to make before U1 (proposed)

**D1: the library's name and boundary.** This is the text stack's own
open question (quaestio …BR7QJ3MCB, "Latin names of the layers",
blocking every first commit), so answering it here answers it there
for the Unicode core. Boundary proposal: a NEW library over `utf8`
(utf8 stays the tiny tier-1 codec every file can include; the new one
carries the tables). Name candidates:
- **`runae`** — the house already calls a codepoint a *runa* (utf8's
  `utf8_proxima_runa`, tessera's `ev.runa`), so `runae_latitudo`,
  `runae_rumpitur`, later `runae_normalizare` read naturally. My
  recommendation.
- `unicodum` — modules/001's placeholder; clear, not classical.
- `abecedarium` / `elementa` — "the ABCs"; classical, but vaguer.

**D2: Unicode version.** Recommend **15.1.0**: it matches the local ICU
oracle exactly, so the whole-codespace comparison has zero expected
differences. A later bump is its own task with a matching oracle. The
alternative (the newest release) makes every newly assigned codepoint
an "expected" difference and weakens the oracle.

**D3: where the UCD files live** (the text stack's other open question,
quaestio …BYTEWE0, "where oracle corpora live"). Proposal: the few
files needed are fetched ONCE (after Fran's OK), with SHA-256 recorded,
and checked in under the library (`<lib>/ucd/15.1.0/`): EastAsianWidth,
DerivedGeneralCategory, emoji-data, GraphemeBreakProperty,
DerivedCoreProperties (for InCB), GraphemeBreakTest. Rationale: no rot,
works offline, the generator's input is reviewable. Sizes are
unmeasured (§6); if too large, check in only the test file and pin the
rest by hash.

**D4: generator language.** Proposal: C89 in `<lib>/instrumenta/`,
like the amalgamator and the lexicon generator, with a staleness gate
(generated `.c` == regeneration). pythonica is the alternative (faster
to write, but a Python dependency in the build graph).

**D5: width policy.** (U2 made this exact by reading Ghostty's code: the
rule in `lib/runae.phase-log.md` U2. Two corrections to the summary
below: spacing marks, Mc, are 1, and "emoji presentation" isn't a
separate rule; those characters are East Asian Wide.) The library
answers Unicode's question (Ghostty's semantics): 0 for controls, Mn/Me, default-ignorables; 2 for East Asian
W/F, emoji presentation, regional indicators; 1 otherwise; Ambiguous =
1. Tessera adds containment (features/004): an explicit cursor
position after every cell whose width is not 1, so a terminal that
disagrees damages one cell, never the rest of the row. Emoji at 2
(modern terminals) — checked in the corpus look.

**D6: `?2027` (grapheme cluster mode).** Emitting it is a no-op where
unsupported; whether that is "assuming" or "querying" is the thesis
owner's call (research note, open). Proposal: not in this plan.

**D7: clusters in tessera.** Width alone fixes CJK. Hindi, Thai and
Yoruba need a cell to hold MORE than one codepoint (base + marks), which
`signum` can't (features/005). Options: a per-opus cluster pool with an
`ornamenta` marker (005's sketch), or base-only rendering with the
marks dropped (wrong for those languages). Proposal: decide after U5,
with the corpus viewer showing exactly what fails.

**D8: the corpus in the repo.** Proposal: an extraction tool writes a
small pinned sample per language (a few KB of paragraphs from one
Lapide file, e.g. `01_Preliminares_<lang>.html`) as plain UTF-8 into
the library's `probationes/corpus/`, about 35 × 4 KB. The viewer can
also read Lapide HTML live. Lapide's text is Fran's own translation;
checking samples into rhubarb is Fran's call.

**Layout (follows the house precedents, set with the decisions):**
`include/runae.h`, `lib/runae.c`, the generated `lib/runae_tabulae.c`
(like `lib/entitates_html_tabula.c`); the generator `tools/runae_generare.c`
with a `tools/runae_generare.sh` wrapper that has a `-probare`
(fresh? 0/1) mode, like `tools/entitates_html_generare.sh`; the pinned
Unicode files in `probationes/fixa/unicode/15.1.0/` (`data/` is
gitignored); the corpus in `probationes/fixa/runae/corpus/<lingua>.txt`;
tests in `probationes/probatio_runae*.c`. NB the entities precedent's
generator is Python (stdlib); D4 keeps this one in C89.

**D7 revisited after U5 (evidence, 2026-09-28).** Fran's look in
Terminal.app and Ghostty: layout is right everywhere, but DROPPING
width-0 runes changes text. Hindi loses its virama, so न्द splits and
हिन्दी reads "hinadī"; é becomes e. Spacing marks (Mc) kept in their own
cells stay aligned, because Ghostty clusters ह+ि into exactly the two
cells we gave it. So a cell must be able to hold a whole cluster.
Proposal **U5b: grapheme cells**, options for Fran:

**DECISUM 2026-09-28: Fran chose (a).**

- **(a) per-opus cluster table (recommended).** A multi-codepoint cluster
  is stored once in a small table owned by the opus (arena bytes +
  open-addressing index), and the cell's `signum` holds its 32-bit ID
  with a `TESSERA_ORNAMENTUM_GRAPHEMA` marker (0x100, not SGR, like
  LATUM). Interning makes equal clusters equal IDs, so the front/back
  diff stays an integer comparison and emission just writes the stored
  bytes. Memory grows only with DISTINCT clusters ever drawn, so
  "steady state allocates nothing" holds once a screen's clusters have
  been seen. A cap (e.g. 64K clusters, 64 bytes each) degrades to base
  + U+FFFD, never breaks. Single-codepoint cells keep today's packed
  signum (ASCII-transparent pin intact).
- (b) fixed inline side array: N extra bytes per cell in both buffers.
  Simple, but ~7 MB at the 512×256 maximum for a cap that still cuts a
  25-byte family emoji.
- (c) refcounted pool with generations (OpenTUI): the most machinery,
  built for mutation patterns tessera doesn't have.

Not `internamentum`: it's a global singleton returning 64-bit pointers
(a signum is 32 bits), and it would pull `tabula_dispersa` and `chorda`
bodies into the amalgam.

Width = `runae_graphema_proximum` (Ghostty's rule; U4 already matches
Ghostty on its tests). Containment also after every multi-codepoint
cluster, not only wide ones (the research note: clusters are where
terminals disagree most). The Hindi/Yoruba/Arabic rows in spectaculum
become the look.

**U5c done 2026-09-28: after a second look, SIMPLEX = the ZWJ rule only (the spacing-mark rule was refuted; see the research note's Terminal.app table).** Original note — **U5b done 2026-09-28; U5c agreed (Fran):** after the look, Terminal.app
sums codepoint widths (spacing marks 0) and doesn't join ZWJ families,
while Ghostty matches runae. U5c = a width POLICY chosen from the
environment (`TERM_PROGRAM=Apple_Terminal` → clusters measured as the sum
of per-codepoint widths; otherwise Ghostty's cluster rule), a second
cluster-width function in runae and a policy field in the opus. Reading
the environment is not a query (research note, mitigation 4).

## 4. Tasks

**U1: corpus.** Extraction tool (materia html → text) + the pinned
samples (D8). A probatio that every sample is valid UTF-8 and non-empty
per language. No Unicode tables yet; this is test DATA for U3–U7.

**U2: UCD + generator + width table.** Fetch the pinned files (after
Fran's OK), the generator, the multi-level table (Ghostty's 3-level
layout: a few array loads per codepoint, amalgam-sized), and
`<lib>_latitudo(runa)` (unsealed). Total over all of `s32` (negatives,
surrogates, > 0x10FFFF defined). Tests: totality sweep, the two
measured excerptum cases (`広` = 2, U+0301 = 0), ASCII unchanged.
Staleness gate. Red first.

**U3: width oracle.** A dev-time tool linking ICU4C: for every
codepoint 0..0x10FFFF, compute the width our policy SHOULD give from
ICU's properties and compare with the table. Target: zero differences
at 15.1. Then OpenTUI's `.zon` map: every difference is recorded as a
named policy row (it's their policy, not a standard). Python's
unicodedata as a third opinion where cheap.

**U4: grapheme breaks (UAX #29, 15.1).** Pairwise break function with
caller-held state (Ghostty's `graphemeBreak` shape), including GB9c
(Indic conjunct break). Oracles: `GraphemeBreakTest.txt` replayed in
full (exact), then ICU's break iterator over the whole U1 corpus (every
boundary position identical, per language). Cluster width: VS16 → 2,
VS15 → 1, flag pairs = 2, ZWJ sequences by their first pictograph.
Planted faults for each.

**U5: tessera wide cells (features/004).** `ornamenta` markers for
wide start and continuation (not SGR bits: masked from emission,
ignored by style equality); drawing rules (a wide rune at the last
column becomes a space; overwriting either half blanks the other);
emission (skip continuation cells, cursor position after each wide
cell). Goldens: `a中b` cells and bytes, both overwrite cases, the last
column, `tessera_replere` over a half. Amalgam + saltuarius. Terminal
look: spectaculum with CJK and emoji rows.

**U6: consumers.** saltuarius column math and `lib/excerptum`'s caret
move from rune counts to widths (the …VRTHANR cases become passing
tests). Then D7 is decided with evidence.

**U7: the corpus viewer.** A tessera instrumentum (unsealed name) that
loads a sample (or Lapide HTML), wraps it to the terminal width by
grapheme widths, paginates (page keys, language switch), and shows the
language, page and width statistics. Wrapping is deliberately
NAIVE, not a library: break at spaces; break anywhere between wide
graphemes; Thai (no spaces) breaks at the width limit. Real line
breaking (UAX #14, `LineBreakTest.txt`) is the next Unicode-core brick
when something pulls it; Thai additionally needs a dictionary (ICU
uses one). Headless goldens through the memoria pons (page 1 of the ja,
hi and yo samples). **Terminal step:** Terminal.app, iTerm2 and Ghostty
across the language table in §2; a findings table per language saying
what is ours and what is the terminal's (Indic shaping, bidi, emoji
width).

**U8: RELATIO.** Phase log, spec-v2 §6 (wcwidth undeferred, clusters
per D7), terminal-planning modules/001 and features/004 (and 005 per
D7) status, the text-stack park's design questions updated with
D1/D3's answers, ledger (…VRTHANR → impletum), merge to main.

## 5. Names to seal (Fran)

The library name (D1) and its function prefix; `<lib>_latitudo`,
`<lib>_rumpitur`, the break-state struct, `<lib>_graphema_proximum`
(modules/001's sketch); the two `ornamenta` markers; the viewer's name;
the corpus directory. Every one is checked against latina.h's reserved
words before use.

## 6. AUDIENDA (not verified)

- **UCD file sizes** (D3) are unmeasured; UnicodeData.txt alone is
  about 2 MB, which is why D3 proposes the derived files instead.
- **How terminals render the hard scripts.** Most terminals don't
  shape Indic conjuncts, and few do bidi; some of what the viewer shows
  will be the terminal's limit, not ours. U7's findings table must say
  which is which.
- **Emoji width in Terminal.app** (D5 assumes 2). The U7 look measures
  it.
- **The ICU oracle checks the table, not the policy:** ICU has no
  "terminal width"; U3 maps ICU's properties through OUR rules. A wrong
  rule would be wrong on both sides. The OpenTUI map and the corpus
  look are the independent checks.
- **Size of the generated table in the amalgam** (modules/001 open
  question); Ghostty's 3-level layout suggests tens of KB.
- **Whether materia's html client extracts Lapide's text cleanly**
  (entities, verse markers, footnotes). U1 finds out.
- **The newest Unicode release number** was not checked; D2 recommends
  15.1 regardless, because of the oracle.
