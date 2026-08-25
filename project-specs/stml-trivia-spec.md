# STML Trivia Model — Spec

*2026-08-24. Status: M1 SHIPPED (c7476a72..d3959eab, goldens +
corpus green). M2 amendments added 2026-08-24: capture collapse
pulled forward from M4 as the trivia stress test. Decisions §0.2
made by Fran in conversation.*

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
- **Tag-interior trivia is MODELED, not disclosed away** (§1.6):
  the formatter should someday be able to emit
  `<a\n  attr="123"\n  attr2="abc">` for attribute-heavy tags, and
  that requires per-attribute layout to survive round trips.
- **CRLF: input canonicalization.** One document-wide CRLF→LF
  normalization at parse entry; the fidelity contract is defined
  over the canonicalized bytes; a flag on the parse result reports
  that it happened. No per-kind CRLF rules anywhere.
- **Collapse aesthetics (M2 pull-forward, 2026-08-24).** Pretty
  may collapse a single-child spine to capture form on one line:
  `<tag(> <tag(> <tag(> foo`. The paren GLUES to the name when the
  tag has no attributes (`<tag(>`); after attributes it stays
  space-separated (`<tag attr="v" (>`). Spine segments separate by
  single spaces (each captor's `spatia_post`). Width budget: 72
  columns from the current indent, all-or-nothing — a spine that
  does not fit stays in plain block form. **Content floor (third
  decree, 2026-08-24): tectum = max(72, indent + 40)** — under
  heavy indentation the budget grows with depth so at least 40
  columns of content room always remain; without it, deep wraps
  compound (each wrapped line is one step deeper and two columns
  narrower) into a staircase of one-link overflowing lines. The
  floor derives only from tree depth, so the fixed point is
  unaffected; nothing changes at indent ≤ 16.
- **Vertical collapse (second collapse decree, 2026-08-24, at M2
  close).** Capture form is UNIVERSAL for single-child elements
  with an element child — width decides only LAYOUT, never form.
  Captor links pack GREEDILY onto a line within the budget; a
  line break indents ONE step (per line, not per link); the
  chain's block terminal opens on a fresh line one step deeper
  than the last captor line, in block form with its close tag at
  its own level (option 2: the captor never absorbs the block's
  open tag onto its line):

      <a(> <b(>
        <c>
          <d/>
          <e/>
        </>

  The invariant this buys: close tags exist exactly where block
  form does — multi-child and mixed elements only. Vertical
  compression is the payoff: every converted wrapper level
  deletes a `</>` line.

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

**Capture-tag amendment (2026-08-24, the collapse pull-forward).**
After a capture-form open tag, SAME-LINE whitespace belongs to the
captor's `spatia_post`: a whitespace-only run in full, and the
newline-free leading edge of a content run. It never forms a text
node and never joins the next valor. Precedent: the newline case
already lands in the captor's post via the general split, and §6's
emission order ([tag][post][capti]) is the slot this rule feeds.
Two named consequences: (i) a bare same-line space is no longer
capturable — `<a (> <b/>` captures `<b/>`, killing the §0.1.3 bug
reborn in same-line form (deliberately: capturing a lone space is
the absurd corner); (ii) the rule diverges from a normal open tag
(`<p> salve` keeps its space in the valor) — after `(>` the space
is capture syntax's separator, not content. Refinements from
implementation (T3): CRUDUS captors are excluded (the raw line
carries its own bytes; the case is mute by construction — the
following run always starts at the terminating newline), and a
directly-captured TEXT value must be non-empty, single-line, and
start with a non-whitespace byte (`_valor_capturabilis`) — a
leading whitespace byte would migrate to the captor's post on
re-read and change the value, so such terminals refuse collapse.
Same-line interleaving inside MULTI-paren captures and before
RETRO captors has the same disease and is NOT addressed here —
deferred to M4's counting policy; pretty produces only
single-paren forward spines.

### §1.3 Text runs

- A run that is **entirely whitespace AND contains a newline**
  produces **no node at all** — its bytes distribute by §1.2. (This
  deletes the child-walker tax and fixes capture counting by
  construction.) A same-line whitespace-only run remains a text
  node with its valor — `<sep>   </sep>` keeps its deliberate
  three-space value (scenario I refinement, 2026-08-24).
- **The `internus` carve-out is RESCINDED** (M1 audit,
  2026-08-24). As designed, `internus` excluded `spatia_clausurae`
  so `<a>\n</a>` would read `""` — believed to be the ONE bridge
  change. The goldens measured the true cost: ~6,000 divergent
  element records, because the close-tag indentation of EVERY
  nested element in a pretty-printed document lives in clausurae.
  `internus` is the BYTES reading — it reassembles clausurae too,
  and the bridge is EXACT (zero divergence outside capture
  reshapes). The empty-with-layout-reads-empty semantics Fran
  approved lives where it belongs: `stml_textus_valor`, whose trim
  produces `""` for `<a>\n</a>` naturally.
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

**Form details.** `\` is a KIND FLAG, never part of the tag name —
selectors match `multiline` plain (our own selector engine shares
the CSS syntax family where `\` is the escape character; a marker
in the name would make marked elements selector-unaddressable, the
`.species` registry caution replayed). With attributes the marker
sits between name and attribute list: `<multiline\ clavis="v">`.
Anonymous close `</>` is legal for `\` (it does not change
lexing); `!\` REQUIRES the named close (raw mode scans for
`</tag` to find its end) — asymmetry deliberate and fixtured.
Self-close with the marker is legal and empty. All-whitespace
`\` content elides under §1.3 — the element is genuinely empty.
Dedent's common prefix is **bytes, never columns** — no tab
expansion; a `\t`-indented line and a space-indented line share
no prefix, so nothing is stripped and everything stays in the
valor, deterministically.

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

### §1.6 Tag-interior trivia

The lexer accepts newlines between attributes; the writer today
normalizes them away — a standing fidelity hole this spec closes
by MODELING it (decretum Franis: the formatter should someday emit
multi-line attribute layouts for attribute-heavy tags):

- `StmlAttributum` gains `spatia_ante` — the whitespace before
  this attribute (after the name or the previous attribute).
  NIHIL = canonical single space.
- The node gains one pre-close chorda — whitespace before the
  tag's `>` or `/>`. NIHIL = none (canonical).
- Non-pretty reassembles stored tag layout byte-exact; pretty
  regenerates (single-space today; a multi-line attribute layout
  rule is a RESERVED formatter policy, §11).
- **Named narrow exception**: whitespace around `=` inside an
  attribute (`attr = "v"`) is NORMALIZED to the tight form, both
  modes — modeling it would cost two more chordas per attribute
  for a shape no house document uses. The goldens run measures
  whether that claim holds; if a document disagrees, it gets
  reformatted once, loudly.
- **Capture tags (amended 2026-08-24)**: the whitespace between
  the name/attributes and the paren group is MODELED — M1
  discarded it, a latent fidelity hole: `<tag(>` (glued `(` has
  always lexed; `(` is a name terminator) parsed fine but
  re-emitted as `<tag (>`, invisible because no corpus file glues.
  NIHIL = glued canonical. Fidelity emits the stored bytes; pretty
  regenerates: GLUED when the tag has no attributes, single space
  after attributes (decretum Franis, §0.2). Whitespace between the
  parens and `>` stays normalized-away (named narrow exception,
  like `=`).

## §2 Accessor contract (the triad + the bridge)

Three readings, one per text kind — and **the declared kind
outranks the accessor** (reading marked content through `fluxus`
returns the valor untouched; the document's declaration wins):

- `stml_textus_internus` — bytes: reassembles ante + valor +
  clausurae + post (+ indentatio re-insertion for `\`). **Result
  bytes UNCHANGED — the bridge is EXACT** (amended 2026-08-24: the
  clausurae exclusion and its `<a>\n</a>` carve-out are rescinded,
  see §1.3 — the goldens measured them as ~6k divergences, not
  one). Canon's identity-sensitive sites (keys, citations —
  `lib/canon.c:1444,1585`) see no change. Direct queries on TEXT
  nodes reassemble the node's own ante+valor+post; the share of a
  leading edge that §1.2 assigns to the PRIOR sibling is
  unreachable from the text node — a named narrow divergence,
  absent from the corpus.
- `stml_textus_valor` — NEW: the value itself. For `\` content this
  is already dedented with newlines intact — the least-surprise
  code/poetry reading lives in the MODEL, not an accessor.
- `stml_textus_fluxus` — NEW: the prose reading (parent-level,
  mixed content): joins child text, emitting ONE SPACE wherever a
  newline-bearing boundary or interior newline-bearing run sat.
  Three edge rules: EDGE trivia (ante of the first content, post
  of the last) contributes NO soft space — `<p>\n salve\n</p>`
  flows to `"salve"`, not `" salve "`; CONSECUTIVE soft boundaries
  merge to one space — `salve\n<!-- nota -->\nmunde` flows to
  `"salve munde"`; recursion is textContent-shaped — an element
  child contributes its own fluxus, so `<p>a<br/>b</p>` flows to
  `"ab"`. Deliberate divergence from HTML: same-line runs stay
  literal (`a  b` keeps two spaces) — one rule everywhere, we are
  a data format.
- `stml_textus_normalizatus` — the shipped read-time dedent
  (`_normalizare_spatium_album`): unchanged, and remains the pre
  reading AGAINST unmarked elements (compat). On `\` content it is
  near-identity since the parser already dedented. Expected to go
  quiet as documents adopt markers (§11).

## §3 Parser

**Input canonicalization first**: CRLF→LF over the whole document
before lexing; `StmlResultus` gains a flag reporting it happened.
The fidelity contract (§4, §7) is defined over the canonicalized
bytes — byte-exact for every LF document, deterministic
one-time normalization for CRLF input. No per-kind CRLF rules
exist anywhere downstream.

Whitespace classification happens at tree assembly (where text nodes
are created today): whitespace-only runs route to trivia by §1.2;
content runs split edges by §1.3; tag-interior runs attach per
§1.6. `positus` semantics for elements unchanged (open tag through
close; trivia OUTSIDE extents — the sedes table and 0032's
addressing keep their meaning). Lexer changes are limited to the
`\` tag flag and CRLF canonicalization; `stml_lexemata_colligere`
remains the raw total stream (over canonicalized input).

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
- **M2 layout rule** (replaces TERMINI): ONE decision per element,
  derived from its children — any text child → INLINE (children
  emitted on the tag line, no boundaries introduced); otherwise →
  BLOCK (one child per line, indented). No per-boundary state
  machine, no transparency. Same-line whitespace-only text nodes
  are CONTENT and are kept — TERMINI dropped them, a bug under the
  M1 decree. M2 does NOT exercise flow re-wrap: flow valor is
  emitted verbatim; the re-wrap liberty stays reserved with the
  formator width policy (§11).
- **Blank-line preservation (decreed 2026-08-24, on seeing pretty
  eat every paragraph break in c89.stml).** Authored blank lines
  at BLOCK level are paragraph separators, not layout to
  regenerate: pretty preserves one or two, and collapses three or
  more to two (N → min(N, 2)). Applies between block siblings
  (elements AND comments), after an open tag before the first
  block child, before the closing tag, and between document-level
  children; never inside collapsed spines or inline content;
  leading blanks of the FILE drop. Derivation is trivia-honest
  and fixed-point stable: blank count = newlines in the
  follower's `spatia_ante` (the ownership law sends the prior's
  single line-break newline to its post, every extra newline to
  the follower's ante), minus the one baseline newline in
  first-child and clausurae positions, clamped to [0, 2].
- **Collapse (pulled from M4, 2026-08-24, §0.2)**: pretty emits a
  single-child spine in capture form on one line when the whole
  line fits the 72-column budget from the current indent.
  Eligible: exactly one child; not crudus/multilinea; no comment
  children; the child is an element (spine continues) or a
  single-line flow text node (the terminal — the §6 inner-value
  refusal is RESCINDED for this narrow case; text with interior
  newlines keeps the open form). Capture form is FORM MEMORY:
  non-pretty preserves the author's choice byte-exact; pretty
  re-derives it — the clausura tacita doctrine applied to
  captures. SHIPPED in M2 T3c with the over-budget fallback =
  plain block form; that fallback is superseded by the vertical
  decree below.
- **Vertical collapse (§0.2 second decree — SHIPPED 2026-08-24,
  the session after M2).** A
  single-child element whose child is an ELEMENT takes capture
  form even when the inline tail does not fit. Layout is GREEDY
  LINE PACKING: captor links accumulate on the current line while
  the budget holds (`<a(> <b(>`); a break starts a new line ONE
  indent step deeper (per LINE, not per link — a line of captors
  consumes one step together); the packing recurses until the
  chain ends. An INLINE terminal rides at the end of the final
  captor line; a BLOCK terminal (multi-child element) opens on a
  fresh line one step below the last captor line, in block form
  with its close tag at its own level:

      <a(> <b(>
        <c>
          <d/>
          <e/>
        </>

  Deep chains with inline terminals pack the same way:

      <t1(> <t2(> <t3(>
        <t4(> foo

  A single TEXT child stays inline-only: capture-inline when
  capturable (§1.2 T3 refinements) and within budget, else the
  plain open/close form — bare text on its own line under a
  captor is legal INPUT but never formatter-produced (the 72/73
  boundary fixtures stand unchanged). AUTHORED single-paren ANTE
  captors re-derive their layout through the SAME routine — form
  memory fully re-derived; this unification is what keeps the
  fixed point, since a reparsed stacked document is full of
  captors that must reproduce their own layout. (Pre-M2b pretty
  inlined authored captees unconditionally, width-blind — that
  path merged into the new routine: the collapse hook precedes
  the captio dispatch, so extending the link test to single-paren
  ANTE captors routed them with no branch changes.) Multi-paren
  authored captors keep glued inline emission (M4 corners).
  Stacked captees parse by the existing ownership law: newline →
  the captor's post, indent → the captee's ante — the §0.1.3
  fixture shape, no parser changes. Implementation notes: the
  final captor and its inline terminal are width-measured
  TOGETHER (else the terminal overflows past a link that "fit");
  exotic children (crudus/multilinea/fragmentum/retro/farcimen/
  multi-paren, comment children) refuse conversion and keep
  their authored forms.
- **Pretty's text liberties follow the kind ladder** (§1.4): flow
  text may be RE-WRAPPED at will (newline runs are semantically one
  space — the mixed-content corruption class dies at the root);
  `\` blocks may only be re-indented as a whole (indentatio
  regenerated to nesting depth, interior untouched); raw content
  is never touched.
- **Flow re-wrap (M3, decreed 2026-08-24 — the fluxus-gated
  liberty exercised).** Pretty OWNS flow prose layout:
  ALWAYS-CANONICAL — authored breaks in flow text are not
  preserved (they are semantically one space; `\` is the opt-out
  for authored line structure, and a blank line INSIDE one flow
  valor is prose, not a paragraph break — the §4 blank-line
  decree governs BETWEEN nodes only). Scope v1: elements whose
  children are exactly ONE flow text node with CLEAN EDGES
  (first and last valor byte non-white — dirty edges would be
  eaten by edge-trimming on reparse); mixed content, ws-only
  text, and document-level text stay verbatim-inline (named
  follow-on: mixed-content paragraph filling). Layout is
  width-driven with NO hanging form: the joined text is judged
  by the existing machinery — capture-inline when it fits
  (`_valor_capturabilis` drops its unilinear requirement; the
  spine emits the JOINED reading), else CAPTURE-STACKED FILL
  (fourth decree, 2026-08-25, from Fran eyeballing
  c89-formatted): `<tag (>` with canonical paren placement, text
  greedily filled at indent+1 within the tectum, NO close tag.
  One text run = one node (runs split only at markup, never at
  spaces), rule A owns the leading boundary, so the stacked
  captee parses whole; the writer can never produce the
  text-sibling merging hazard because text-bearing parents lay
  out inline. This COMPLETES the M2b invariant: close tags exist
  exactly where MULTI-child block form does — every single-child
  element, text or element child, is capture form. Sedes: tag
  only (captor semantics). Break candidates are
  single-space runs and newline-bearing runs ONLY — words and
  same-line multi-space runs are unbreakable atoms (breaking one
  would change the fluxus reading; the gate enforces this
  mechanically), and over-budget atoms overflow, accepted. A
  wrapped line's indentation rides inside a newline-bearing run
  in the valor — soft under fluxus, never literal content. Gate
  relaxation (§7.7): tree equivalence compares FLOW text valors
  modulo fluxus-equality (newline-bearing runs ≡ one space);
  `\`/`!` content stays byte-exact. Companion fix: capturable
  text requires the LAST byte non-white too (latent M2b hole —
  a trailing space in a captured text died into post on
  reparse; never bitten, now pinned).
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
- **Collapse policy — MOVED TO M2** (2026-08-24, chosen as the
  trivia stress test; the policy now lives in §4). The inner-value
  refusal is relaxed there for single-line flow text terminals.
  Multi-paren collapse remains parse-supported but never
  formatter-produced. What stays M4: the counting policy for the
  same-line corners the §1.2 capture amendment defers (multi-paren
  interleaving, retro captors).
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
7. **Tree equivalence** (M2): `legere(scribere(x, pretty))` must be
   tree-equal to `x` modulo trivia and form-memory fields
   (`captio_*`, `clausura_anonyma`, tag-interior layout) — pretty
   may move bytes, never meaning. New comparator; also the oracle
   the §8 flow re-wrap fixture was waiting for. M2's planted
   fault: emit one stored trivia chorda in pretty — the fixed
   point and/or this gate must go red.

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
- `<a>\n</a>` — childless-with-clausurae: internus reads `"\n"`
  (exact bridge), writer keeps the open form (`/>` would drop the
  interior bytes), `valor` reads `""`
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
- multi-line tag `<a\n  attr="1"\n  attr2="2">` — reassembles
  byte-exact (§1.6); pre-`>` whitespace
- CRLF document — canonicalized once, flag set, then byte-exact
  over the LF form
- mixed tab/space dedent — no shared prefix, nothing stripped
- `<multiline\ clavis="v">` — marker with attributes
- `</>` on `\` legal; missing named close on `!\` refused
- fluxus: edge-trivia no-space, consecutive-boundary merge,
  `<p>a<br/>b</p>` → "ab"
- glued capture `<tag(>` — byte-exact round trip (the M1 hole)
- `<tag attr="blah" (>` — spaced form after attributes preserved
- spine `<tag(> <tag(> <tag(> foo` — captures ELEMENTS not
  spaces (§1.2 amendment), reassembles byte-exact, and is
  tree-equal (modulo form memory) to the nested block form
- `<tag(> foo` — captured text: valor `"foo"`, captor post `" "`
- width boundary: a spine at exactly 72 columns collapses, at 73
  stays block; both are fixed-point stable
- blank lines: one authored blank between block siblings (and
  before a comment) survives pretty; three or more collapse to
  two; blanks after the open tag and before the close tag
  survive with the same clamp; all fixed-point stable
- vertical (M2b): `<a><b><c/><d/></b></a>` → packed captors +
  block interior (`<a(> <b(>` / block c at +1 — the §0.2
  exemplar); over-budget chain ending in an inline terminal →
  packed captor lines + final spine line; line-packing boundary
  at the budget (a chain that fits on one captor line never
  splits); stacked form fixed-point stable and tree-equal;
  authored `<a (>` + newline captee re-derives its own layout

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
  ladder (`\` lexing, dedent-at-parse, indentatio, refusals),
  tag-interior trivia (§1.6), and CRLF canonicalization; golden
  bridge + byte-fidelity corpus green throughout; arbor gate
  green.
- **M2** — pretty = trivia regeneration; TERMINI deleted;
  inline/block layout rule; `\` block re-indent; capture collapse
  + glued parens (§0.2) + the §1.2 capture-post amendment + §1.6
  pre-paren trivia; fixed point + tree equivalence gates land.
  Flow re-wrap NOT exercised (reserved with the formator width
  policy). SHIPPED 2026-08-24 (71b09525, ec1f20d0, 2048a784) —
  plus, unplanned: chained ANTE captures fixed (never worked)
  and captor-adjacent comments preserved (silently vanished).
- **M2b** — vertical collapse (§4 second decree): stacked
  capture form for over-budget and block-terminated single-child
  chains; authored-captor layout unification. DECREED at M2
  close; SHIPPED 2026-08-24 the following session — writer-only
  (parser untouched), the all-or-nothing fallback deleted, the
  72/73 text fixtures unmoved. Churn: 2 root expectations +
  9 silva arbor assertions (finds in lib/stml.worklog.md).
  Follow-ons same day: blank-line preservation (§4) — decreed on
  first live run over c89.stml, where pretty ate every paragraph
  break; N authored blanks → min(N, 2), derived from the
  follower's spatia_ante over a positional baseline. And the
  depth floor (§0.2 third decree): tectum = max(72, indent + 40),
  killing the deep-wrap staircase (arbor documents reach 36
  columns of indent in mid-size lib files).
- **M3** — consumer migration (§9) + `fluxus` accessor; amalgam +
  lab. ACCESSORS + MANDATED MIGRATION SHIPPED 2026-08-24: valor
  (pure model read, zero kind logic) + fluxus (two-flag state
  machine; every §2 edge rule falls out of pendens/emissum).
  canon's one value-read moved to valor (behavior-neutral, its
  internus identity sites stay on the bridge forever); silva
  arbor deliberately NOT migrated — its `_textus_directus`
  carries a two-part ws-only contract valor does not share (only
  the stale rationale comment fixed). Remaining consumers at
  leisure per §9. No struct changes → no lab re-vendor needed.
  Flow re-wrap SHIPPED 2026-08-25 (§4 decree above): joined
  capture + capture-stacked fill (fourth decree — `<t (>` +
  filled lines, no closer; the M2b invariant exact); Gate B
  modulo fluxus for flow text; `_valor_capturabilis`
  clean-edges-both-ends (latent trailing-space hole closed). COMPANION: arbor now declares
  carried values RAW (`!`) — code bytes are not prose; trivium
  elements always, lexeme elements when single-child after ALL
  involucra attach (mixed stay flow, safe under v1 scope); loud
  refusal if a value contains its own closing sequence. The
  plagula gate holds 78+154 at 0 divergences THROUGH a
  pretty-print. M3 COMPLETE. Named follow-ons: mixed-content
  paragraph filling; §1.6 multi-line attribute layout (the only
  remaining width feature — 33 residual long lines in c89.stml
  pretty are all attr-heavy open tags).
- **M4** — captures: counting policy, including the same-line
  corners the §1.2 amendment defers (multi-paren interleaving,
  retro captors); raw multi-paren refusal (§6). (Stream-order
  emission landed in M1; collapse moved to M2.)

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
- **Multi-line attribute layout as a formatter RULE** (when to
  break attribute-heavy tags across lines): RESERVED — §1.6 builds
  the carrying capacity; the pretty policy comes when the formator
  rule is designed.
- **Whitespace around `=` in attributes**: normalized, not
  modeled (§1.6's named narrow exception).
- **`normalizatus` deprecation path**: RESERVED — it stays as the
  read-time dedent against unmarked elements; revisit once marked
  documents dominate.
- The lab's mini `_stml_generale_legere` is NOT taught trivia,
  captures, or `\` — real parsers arrive over the bridge (0029
  doctrine); named consequence, not an obligation.
