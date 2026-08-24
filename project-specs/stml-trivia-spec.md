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

- A run that is **entirely whitespace** produces **no node at all**
  — its bytes distribute by §1.2. (This is what deletes the
  child-walker tax and fixes capture counting by construction.)
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

### §1.4 Comments, raw, attributes, sigils

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

## §2 Accessor contract (the compatibility bridge)

- `stml_textus_internus` — **result bytes UNCHANGED**: reassembles
  ante + valor + post. A probatio captures today's outputs over a
  fixture corpus as goldens BEFORE surgery and asserts equality
  after (§7). Canon's identity-sensitive sites (keys, citations —
  `lib/canon.c:1444,1585`) therefore see no change.
- `stml_textus_valor` — NEW: the trimmed value (the least-surprise
  accessor consumers migrate toward).
- `stml_textus_normalizatus` — semantics unchanged (and cheaper).

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
- The raw-capture "SED SEMEL TANTUM" delimiter hack is DELETED —
  the delimiter is the capture node's `post` now, explicit.

## §5 Fixed point

`pretty(legere(pretty(x))) == pretty(x)` becomes a standing gate
(it exists informally today; it gets a probatio). Non-pretty is
fidelity; pretty is legibility with a fixed point — doctrine
unchanged, now structurally enforced.

## §6 Captures (M4 — lands on top)

- **Counting**: trivially clean — whitespace is no longer a node, so
  `_processare_captiones` counts only real nodes. Comments remain
  skipped-but-preserved (today's rule, unchanged).
- **Collapse policy** (pretty writer may emit `<tag (>` for a node
  it judges collapsible): all children structural (any significant
  text blocks — the inner-value refusal), no comment children, not
  crudus, and **single child only** in v1 — the wrapper-spine case
  (`<functio (><corpus (><sententia …/>`), which is where arbor
  documents win. Multi-paren collapse is parse-supported but not
  formatter-produced until it earns legibility.
- **Raw multi-capture**: complete it — `<tag! ((>` captures N raw
  lines (the lexer already notes the count and captures one; the
  silent middle state is the only wrong answer).

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

- **M1** — model + parser + non-pretty writer; golden bridge +
  byte-fidelity corpus green throughout; arbor gate green.
- **M2** — pretty = trivia regeneration; TERMINI deleted; fixed
  point gate lands.
- **M3** — consumer migration (§9); amalgam + lab.
- **M4** — captures: counting (free), collapse policy, raw
  multi-capture completion (§6).

## §11 Non-goals / reserved

- No entity-handling changes; no attribute-whitespace changes.
- No schema-driven elision (separate future, canon-versioned, one
  named default per absence — the fons-princeps scar applies).
- Valor sub-extents on text nodes: RESERVED.
- Multi-paren collapse in the formatter: RESERVED until legibility
  earns it.
- The lab's mini `_stml_generale_legere` is NOT taught trivia or
  captures — real parsers arrive over the bridge (0029 doctrine);
  named consequence, not an obligation.
