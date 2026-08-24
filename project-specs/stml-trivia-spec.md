# STML Trivia Model — Spec

*2026-08-24. Status: APPROVED direction, pre-implementation.
Decisions §0.2 made by Fran in conversation; everything else is
proposed design for review.*

## §0 Motivation

### §0.1 The bug family (category, not member)

Fidelity currently lives *inside* text nodes: the parser keeps text
verbatim so that non-pretty emission reproduces bytes, which means
every whitespace run between elements is a full sibling **node** and
every newline around a value rides **in** the value. Four independent
subsystems have each been patched to dodge this:

1. **Raw-capture accumulating newline** (fixed 2026-08-06, "SED SEMEL
   TANTUM", `lib/stml.c` writer) — the line-delimiter newline was
   ambiguous between value and layout; each write→read cycle grew a
   blank line.
2. **TERMINI** (fixed 2026-08-19, the long comment in the pretty
   writer) — pretty emission cumulated newlines around text children
   and had no fixed point until whitespace-only text was made
   transparent to boundary logic.
3. **Capture counting** (deferred, the trigger for this spec) —
   `_processare_captiones` skips comments but **counts text nodes**,
   so `<a (>` followed by a newline captures the whitespace instead
   of the element. Pretty layout around captures changes the tree.
4. **Clausura tacita** — line-counting reaches through text nodes.

Beyond the bugs, every child-walker in the house (canon cardinality,
components, arbor reader, lab) carries skip-whitespace boilerplate.
Four patches and a standing tax = a category. The fix is the model.

### §0.2 Decisions (Fran, 2026-08-24)

- **Trim rule: newline-bearing edges only.** An edge (maximal
  leading/trailing whitespace of a text run) becomes trivia iff it
  contains at least one newline. Same-line-only spacing stays in the
  value: `<p>salve <b>munde</b></p>` keeps `"salve "`;
  `<a>\n  prima\n</a>` has valor `"prima"`. Rule of thumb: crossing a
  line boundary = layout; same-line = content.
- **One arc, compatibility accessors.** Single project; the verbatim
  accessor keeps byte-identical semantics throughout so consumers
  migrate at leisure. No shipped intermediate stage.
- **Flow is the DEFAULT text kind** (the HTML instinct): unmarked
  text has soft newlines — newline-bearing whitespace runs read as
  one space. Newline-preserving content is the exception and must
  DECLARE itself (§1.4); supporting preservation everywhere is bad
  for formatting ergonomics and confusing.
- **The `\` multiline marker, GLUED to the tag name** (`<versus\>`):
  scans fast, jumps out even with attributes present — one could
  argue capture-style space separation (`<tag \>`), but the kind of
  a tag's content is name-adjacent information.
- **One legal sigil order, `!\`** (`<code!\>`); `\!` is a loud parse
  error — `!` changes lexing so it binds first, and fidelity should
  never have to remember which order an author typed.
- **v1 `\` is text-only**; inline child elements inside multiline
  content are deferred (§11) — and they are the long-term
  differentiator from `!`, which can never have children.

### §0.3 The model in one sentence

Value is meaning; whitespace is layout carried *beside* the value
(trivia); comments are content with structure (they stay nodes) —
the same doctrine silva already proved at byte-exact corpus scale,
transcribed to STML with silva's settled ownership answers.

## §1 The model

### §1.1 Node fields

`StmlNodus` gains three trivia fields (whitespace bytes only, raw,
never entity-bearing; `chorda*` interned or piscina-copied —
implementation's choice, interning likely pays since indentation
repeats):

```
spatia_ante        whitespace before this node's first byte
                   (before '<' of the open tag; for text nodes,
                   the trimmed leading edge)
spatia_post        whitespace after this node's last byte, same
                   line up to and INCLUDING the first newline
                   (silva's tie-break, transcribed)
spatia_clausurae   ELEMENTS: interior whitespace between the last
                   child's post and this node's closing tag.
                   DOCUMENTUM: file tail after the root close.
```

NIHIL = empty everywhere; a document with no layout has no trivia.

### §1.2 Ownership law (single owner, transcribed from silva)

A whitespace run between two nodes splits at the first newline,
**inclusive**: everything through that newline is the prior node's
`spatia_post`; the remainder is the next node's `spatia_ante`
(silva_lexema.h: "trailing = trivia eiusdem lineae usque ad primam
NOVA_LINEA inclusive; leading = reliqua"). Whitespace between an
open tag and the first child = first child's `ante`. Between the
last child and the close tag: the last child's `post` takes its
same-line-through-newline share; the remainder is the parent's
`spatia_clausurae`. Every whitespace byte has exactly one owner;
the reassembly in §4 is the proof obligation.

### §1.3 Text runs

- A run that is **entirely whitespace AND contains a newline**
  produces **no node at all** — its bytes distribute by §1.2. (This
  deletes the child-walker tax and fixes capture counting by
  construction.) A same-line whitespace-only run remains a text
  node with its valor — `<sep>   </sep>` keeps its deliberate
  three-space value (scenario I refinement, 2026-08-24).
- **Named `internus` carve-out**: an element whose entire content
  was newline-bearing whitespace (`<a>\n</a>`) becomes genuinely
  empty — `internus` returns `""` where it returned `"\n"`. This is
  the ONE bridge behavior change; the golden audit (§7.1) must
  enumerate every affected fixture, and empty-with-layout reading
  as empty is the least-surprise verdict.
- A run with content: an edge (maximal whitespace prefix/suffix)
  **leaves the valor iff it contains a newline** (§0.2). Bytes that
  leave the valor then distribute by §1.2 exactly like any other
  whitespace — i.e. a trailing edge `"\n  "` splits into this
  node's `post` (`"\n"`) and the next node's `ante` (`"  "`);
  §1.3 decides only WHETHER bytes exit the value, §1.2 alone
  decides who owns them. Newline-free edges stay in the valor
  (content). `valor` keeps interior bytes verbatim,
  entity-resolved exactly as today. Text node `positus` extents
  continue to cover the FULL original run including its edges
  (source-honest; a valor sub-extent is RESERVED, not built).

### §1.4 The kind ladder (what newlines MEAN, declared in the tag)

| form | kind | newlines | dedent | entities/tags | pretty writer may |
|---|---|---|---|---|---|
| `<tag>` | flow (default) | soft (≡ one space) | n/a | live | re-wrap freely |
| `<tag\>` | multiline | content | at parse | live | re-indent block only |
| `<tag!>` | raw | content | no | dead | touch nothing |
| `<tag!\>` | raw multiline | content | at parse | dead | re-indent block only |

The marker is what makes the formatter SOUND: without a declared
kind, pretty mode can never safely re-flow any text (it cannot know
whether a newline is meaning) — the root of the mixed-content
corruption family. With it, the document carries the contract and
every tool reads the same declaration.

**`\` semantics.** Edges diverge deliberately from the general
§1.3 rule at the LEADING end: the leading clip is **the first
newline ALONE** — line indentation stays in the run so every line
participates in the dedent computation. (The general
maximal-prefix rule would eat line 1's indentation into trivia and
silently destroy relative structure when line 1 is indented deeper
than its siblings.) Content on the tag line itself
(`<multiline\>abc`) is legal but EXCLUDED from the dedent
computation (the PEP-257 docstring rule — it has no indentation by
construction and would pin the prefix to zero). The TRAILING clip
is the general rule: the maximal newline-bearing suffix exits
entirely, so `123</>`, `123\n</>`, and `123\n  </>` all yield the
same valor — close-tag placement never matters for content
(decretum Franis, 2026-08-24).

Dedent runs AT PARSE over the lines after the first newline: the
common indentation of non-empty lines is stripped from the valor
and stored as the node's `indentatio` trivia (one chorda);
relative indentation deeper than the common prefix stays in the
valor. Reassembly re-inserts the prefix on non-empty lines —
deterministic and byte-exact because of the normative-form refusal
below. Because the prefix is STORED, the pretty writer re-indents
a `\` block to its current nesting depth without touching interior
structure — moved elements reformat correctly, which `<pre>` never
got right. `StmlNodus` gains `indentatio` (NIHIL for unmarked/raw
nodes) and a kind flag for `\`.

**Normative-form refusal.** Inside `\` content, an interior line
that is whitespace-only but NOT empty is a named vitium (the one
class that would make prefix reassembly ambiguous). Truly empty
lines are fine; trailing whitespace after content on a line is fine
(those bytes live in the valor). Loud beats lossy.

**Composition refusals (v1).** `\` with child elements = vitium
(reserved, §11). `\` with capture parens = vitium. `\!` order =
vitium. Each refusal names itself.

### §1.5 Comments, raw, attributes, sigils

- **Comments stay nodes** (content with structure — this is where
  STML deliberately differs from silva, whose comments are trivia).
  They carry `ante`/`post` like any node.
- **Raw content** (`<tag!>`): interior fully verbatim in valor — no
  trimming, no trivia inside raw. The raw-line capture delimiter
  newline becomes the capture node's `post`, which retires the
  once-only-newline hack (§4).
- **Attribute values**: untouched, raw both ways, as today.
- **Sigils, fragments, transclusion, augmentation**: no semantic
  change; the nodes gain trivia fields like all others.

## §2 Accessor contract (the triad + the bridge)

Three readings, one per text kind — and **the declared kind
outranks the accessor** (reading marked content through `fluxus`
returns the valor untouched; the document's declaration wins):

- `stml_textus_internus` — bytes: reassembles ante + valor + post
  (+ indentatio re-insertion for `\`). **Result bytes UNCHANGED**
  except the §1.3 carve-out. A probatio captures today's outputs
  over a fixture corpus as goldens BEFORE surgery and asserts
  equality after (§7). Canon's identity-sensitive sites (keys,
  citations — `lib/canon.c:1444,1585`) see no change.
- `stml_textus_valor` — NEW: the value itself. For `\` content this
  is already dedented with newlines intact — the least-surprise
  code/poetry reading lives in the MODEL, not an accessor.
- `stml_textus_fluxus` — NEW: the prose reading (parent-level,
  mixed content): joins child text, emitting ONE SPACE wherever a
  newline-bearing boundary or interior newline-bearing run sat.
  Deliberate divergence from HTML: same-line runs stay literal
  (`a  b` keeps two spaces) — one rule everywhere, we are a data
  format.
- `stml_textus_normalizatus` — the shipped read-time dedent
  (`_normalizare_spatium_album`): unchanged, and remains the pre
  reading AGAINST unmarked elements (compat). On `\` content it is
  near-identity since the parser already dedented. Expected to go
  quiet as documents adopt markers (§11).

## §3 Parser

Whitespace classification happens at tree assembly (where text nodes
are created today): whitespace-only runs route to trivia by §1.2;
content runs split edges by §1.3. `positus` semantics for elements
unchanged (open tag through close; trivia OUTSIDE extents — the
sedes table and 0032's addressing keep their meaning). No lexer
changes: `stml_lexemata_colligere` remains the raw total stream.

## §4 Writer

- **Non-pretty = reassembly**: ante + form + children (recursively)
  + `spatia_clausurae` + close + post, verbatim. This IS the
  fidelity oracle: `scribere(legere(x), non-pretty) == x` for the
  whole corpus, now including capture forms with interleaved
  newlines (fixtures §8).
- **Pretty = trivia regeneration**: stored trivia ignored, layout
  generated from structure. The TERMINI machinery and its
  whitespace-transparency special cases are **DELETED** (the
  fourth-fix-is-deletion law); comment placement derives from tree
  order. Clausura tacita policy unchanged.
- **Pretty's text liberties follow the kind ladder** (§1.4): flow
  text may be RE-WRAPPED at will (newline runs are semantically one
  space — the mixed-content corruption class dies at the root);
  `\` blocks may only be re-indented as a whole (indentatio
  regenerated to nesting depth, interior untouched); raw content
  is never touched.
- The raw-capture "SED SEMEL TANTUM" delimiter hack is DELETED —
  the delimiter is the capture node's `post` now, explicit.

## §5 Fixed point

`pretty(legere(pretty(x))) == pretty(x)` becomes a standing gate
(it exists informally today; it gets a probatio). Non-pretty is
fidelity; pretty is legibility with a fixed point — doctrine
unchanged, now structurally enforced.

## §6 Captures (M4 — lands on top)

- **Counting**: trivially clean — newline-bearing whitespace is no
  longer a node, so `_processare_captiones` counts only real nodes.
  Comments remain skipped-but-preserved (today's rule, unchanged).
- **Trivia distribution and capture ORDER**: trivia distributes on
  the PRE-capture sibling stream; capture-form emission replays
  stream order (open tag, then the captor's post, then captured
  children each with their own trivia). Without this sentence,
  "post of a capture node" is ambiguous since its children
  serialize after its open tag (scenario F, 2026-08-24).
- **Collapse policy** (pretty writer may emit `<tag (>` for a node
  it judges collapsible): all children structural (any significant
  text blocks — the inner-value refusal), no comment children, not
  crudus/multiline, and **single child only** in v1 — the
  wrapper-spine case (`<functio (><corpus (><sententia …/>`),
  which is where arbor documents win. Multi-paren collapse is
  parse-supported but not formatter-produced until it earns
  legibility.
- **Raw multi-capture is SUPERSEDED by `<tag!\>`** (§1.4): the
  multi-line-code use case the lexer deferred ("multi-linea +
  dedentatio = futura", `lib/stml.c:737`) lands as declared syntax
  instead of capture magic. Multi-paren on raw-line capture
  (`<tag! ((>`) becomes a LOUD refusal — the silent
  noted-but-ignored middle state is the only wrong answer.

## §7 Gates and oracles

1. **Golden bridge**: pre-surgery snapshot of `stml_textus_internus`
   outputs over the fixture corpus; post-surgery equality. (Generate
   the goldens FIRST — they are the old behavior's testimony.)
2. **Byte-fidelity corpus**: existing round-trip probationes + new
   fixtures (§8), non-pretty, byte-exact.
3. **`probatio_silva_arbor_plagula`** — 154 latinized files,
   parse→STML→load→emit→memcmp vs disk. The arbor documents are
   pretty-printed STML full of whitespace runs; this gate catches a
   single drifted byte across the whole change, for free.
4. **Fixed point** (§5).
5. **`probatio_stml_sedes`** — writer/parser extent parity must hold
   unchanged (extents exclude trivia on both sides).
6. **Planted faults at birth**: mis-own one newline (post vs ante),
   drop one trailing edge, double one delimiter — each must turn a
   gate red before the fix is trusted (silent gate ≡ dead gate).

## §8 Fixtures (new, minimum set)

- authored capture + newline between captor and captee (the §0.1.3
  case), ANTE/RETRO/FARCIMEN each
- multi-paren capture `((`/`(((` with interleaved whitespace
- comment between captor and captees
- raw-line capture with and without trailing sibling text starting
  with `\n` (the SED SEMEL TANTUM case, now as fixture)
- mixed content: `salve <b>munde</b>` (same-line space KEPT in
  valor), multi-line text with indented edges (edges → trivia)
- whitespace-only runs at every position: before first child, between
  siblings, before close, document tail
- `</>` anonymous close and clausura tacita interaction
- a text run whose edge is spaces-only (no newline) — NOT trimmed
- `<sep>   </sep>` — same-line whitespace-only VALUE, preserved
- `<a>\n</a>` — the internus carve-out case, enumerated by goldens
- `<versus\>` block: dedent + relative indentation survives +
  byte-exact reassembly; interior EMPTY lines
- trailing equivalence: `123</>` vs `123\n</>` vs `123\n  </>` —
  same valor, each reassembles its own bytes
- line 1 indented DEEPER than line 2 — relative structure survives
  (the leading-clip divergence earns its keep)
- content on the tag line (`<multiline\>abc`) — excluded from
  dedent computation, rides untrimmed
- `<code!\>` with `<`, `&`, `*` — raw lexing + dedent
- `\` refusals: whitespace-only interior line, child element,
  capture parens, `\!` order — each vitium NAMED
- flow re-wrap: pretty output with different wrap points parses to
  an IDENTICAL tree (fluxus-stable)

## §9 Consumer migration (M3)

Discovery: `awk -F'\t' -v h='include/stml.h' '$2==h{print $1}'
build/inclusiones.tsv` (31 direct includers at spec time). Order:

1. **lib/stml.c itself** + probationes (M1/M2).
2. **canon** — expected NO behavior change (accessor bridge);
   delete any whitespace-child filtering that becomes dead.
3. **silva arbor** — reader/writer walk elements and use the
   accessors; the 154-file gate is the verdict. Then **amalgam
   regen** (silva.h hand-mirror gains the trivia fields —
   remember the CADENDA/hospes obligations) and **lab re-vendor**.
4. **everything else** at leisure via the bridge; each consumer may
   adopt `stml_textus_valor` when touched, never forced.

## §10 Milestones

- **M1** — model + parser + non-pretty writer, INCLUDING the kind
  ladder (`\` lexing, dedent-at-parse, indentatio, refusals);
  golden bridge + byte-fidelity corpus green throughout; arbor
  gate green.
- **M2** — pretty = trivia regeneration; TERMINI deleted; flow
  re-wrap liberty + `\` block re-indent; fixed point gate lands.
- **M3** — consumer migration (§9) + `fluxus` accessor; amalgam +
  lab.
- **M4** — captures: counting (free), stream-order emission,
  collapse policy, raw multi-paren refusal (§6).

## §11 Non-goals / reserved

- No entity-handling changes; no attribute-whitespace changes.
- No schema-driven elision (separate future, canon-versioned, one
  named default per absence — the fons-princeps scar applies).
- Valor sub-extents on text nodes: RESERVED.
- Multi-paren collapse in the formatter: RESERVED until legibility
  earns it.
- **Inline child elements inside `\` content**: RESERVED for v2 —
  the long-term differentiator from raw (poetry with `<em>`,
  dedent across interleaved tags). Refused loudly in v1.
- **Per-element kind declaration in canon** ("element X must be
  `\`"): RESERVED — the document declares today; the schema may
  someday constrain.
- **`normalizatus` deprecation path**: RESERVED — it stays as the
  read-time dedent against unmarked elements; revisit once marked
  documents dominate.
- The lab's mini `_stml_generale_legere` is NOT taught trivia,
  captures, or `\` — real parsers arrive over the bridge (0029
  doctrine); named consequence, not an obligation.
