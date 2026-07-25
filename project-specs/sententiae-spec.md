# Sententiae — specification v1

**Date:** 2026-07-25
**Derived from:** `project-specs/sententiae-interview.md` (24 questions)
**Exemplar / first corpus:** `de-imagine.md`
**Status:** design spec. A v2 follows after codebase exploration.

---

## I. What this is

Fran writes documents as **explicitly addressed outlines**: numbered claims
nested by an address written into the text, carrying epistemic grades and
cross-references. *De Imagine* is the first — 273 points across 14 parts.

Sententiae makes such a document a first-class object in the forum app: it
is pasted in, parsed into a structured projection, and thereafter every
individual point can be read, cited, commented on, tagged, and searched by
its authored address.

The citation form is `DI/4.obj.1.1`.

### Why the format is worth building for

This is not a markdown list with numbers. The address is **authored**, not
derived from position. A markdown list encodes `4.obj.1.1` as "fourth item,
sub-branch, first, first" and you recover it by counting; here it is
literally in the text. That single property is what makes stable citation,
durable anchors, and a trivially simple parser all possible at once.

The document argues for its own format at 28.2.1: it exists this way so that
*addressable claims can be checked one at a time by someone with no view
about the nature of their source.* The tooling is downstream of that
argument, not a convenience layered on top.

---

## II. Architecture — the governing decision

**The document's markdown is truth. Everything else is a rebuildable
projection.**

```
        PASTE
          │
          ▼
    ┌───────────────┐
    │  liber (res)  │   verbatim markdown, stored as authored
    │   = TRUTH     │   never written by the app
    └───────┬───────┘
            │  parse (one-way, deterministic)
            ▼
    ┌───────────────────────────────────────────┐
    │  PROJECTION  — discard and rebuild freely │
    │  · point tree (locus → text, grade, part) │
    │  · grade vocabulary                       │
    │  · reference graph + backlinks            │
    │  · siglum table                           │
    │  · search rows                            │
    └───────────────────────────────────────────┘
            ▲
            │  anchor: authored locus + seal of text
    ┌───────┴───────────────────────────────────┐
    │  scholia (comments) · point tags          │
    │  = independent entities, NOT projection   │
    └───────────────────────────────────────────┘
```

Three consequences, each load-bearing:

**1. The parser may improve forever with no data migration.** Ship a v1 that
understands addresses, text, and grades; add reference resolution, then
named-segment semantics, then whatever the format grows into. Every
existing document gains the new structure on reparse, because the source was
never lossy. Same relationship as `tabularium.jsonl` (truth) to the `.db`
(projection).

**2. Anchors bind to authored data, never to parser output.** A comment
anchors to the address Fran wrote plus a seal of the point's text. It never
references a parser-assigned node id. Violating this would silently couple
comment history to parser version and take back consequence 1.

**3. The parse is one-way.** Edits happen to the document text; never to
projected points. The app is never required to be a printer, and
`print(parse(x)) == x` over hand-wrapped text with hanging indents never
becomes a contract we have to honor.

### The format is the API

The markdown convention is what lives in Fran's fingers and in 273 existing
points. It is expensive to change. The parser is implementation and is
cheap to change. **Therefore: prefer parser cleverness over format
changes, always.** The grade-vocabulary rule in §IV.4 is the first
application of this and should be the model for later ones.

---

## III. The format, as parsed

### III.1 Document shape

```
prooemium        ← everything before the first addressed line
## Part I — …    ← heading (markdown, outside the fence)
```                ← fence opens
1. …               ← sententiae live inside fences
1.1. …
```                ← fence closes
## Part II — …
…
```

The fences are load-bearing in the source: they are why hanging indentation
survives markdown rendering on GitHub. The parser treats a fenced block
following a part heading as that part's body.

### III.2 Grammar

```
liber        := prooemium pars*
pars         := titulus_partis? saeptum sententia* saeptum
sententia    := locus SP textus continuatio*
locus        := numerus ( "." segmentum )* "."     ← FIRST segment is numeric
numerus      := [0-9]+
segmentum    := [0-9]+ | [a-z]+
continuatio  := line whose first character is whitespace
```

**The first segment must be numeric.** An earlier draft allowed
`segmentum` in initial position, which silently admits any English word
ending in a period. Measured against the exemplar:

| grammar | column-0 matches |
|---|---|
| first segment `[0-9]+ \| [a-z]+` (wrong) | **275** — two false positives |
| first segment `[0-9]+` (correct) | **273** — exact |

The two false positives are `de-imagine.md:15` (`publication. Grades: …`)
and `:21` (`reports. Such reports are…`) — ordinary prose in the
prooemium. Named segments only ever occur in non-initial position, which
is true of every one of the 21 `obj` addresses.

The whole of the parse rests on one measured invariant:

> **Every address begins at column 0. Every continuation line is indented.
> Nesting is expressed entirely in the address and never in the indentation.**

Verified across all 273 points in the exemplar with zero exceptions. This is
what reduces the core loop to roughly fifteen lines: a column-0 line matching
the address pattern opens a new sententia, an indented line appends to the
current one, a blank line does nothing. There is no list-nesting state
machine and no ambiguity about what two spaces means.

The invariant is not a convenience — it is measured load-bearing. **42
indented lines in the exemplar would match the address grammar if a parser
stripped leading whitespace before matching.** Column-0 discipline is the
only thing separating an address from a sentence that happens to begin with
a word and a period. Any implementation that trims before testing produces
42 phantom points.

### III.3 Segments are number-or-name

`obj` appears 21 times as a middle segment (`4.obj.1.1`). A digits-only
grammar would drop every objection in the document, which is where the
argument actually happens.

**Named segments are opaque in v1.** The parser knows a segment may be a
word; it attaches no meaning to `obj`. A future document using `resp` or
`sc` needs no code change. Because the projection is rebuildable, teaching
the parser meaning later costs nothing — so YAGNI applies here with unusual
force, since the usual penalty for deferring (migration) does not exist.

### III.4 Tags span continuation lines

**This is the trap that must be handled first.** Six of the seven retracted points
in the exemplar open on one line and close on another:

```
29.2. "Unity by art is a deficiency." [retracted: defeated by the
      carved stone — art perfects, and the worked stone is really
      better than the raw one; see 23.1]
```

A line-based tag extractor finds 1 of 7 and reports success. Therefore the
pipeline order is fixed:

> **join continuations → then extract tags → then extract references.**

Any tool that inspects tags before joining is wrong, and wrong silently,
which is the dangerous kind.

---

## IV. The parser

### IV.1 Pipeline

```
raw markdown
  │
  ├─ 1. split prooemium / parts        (first FENCE or "## " ends prooemium)
  ├─ 2. collect grade vocabulary       (from prooemium brackets, §IV.4)
  ├─ 3. parse prooemium fields         (title, siglum, status, siglum table)
  ├─ 4. scan parts and fences
  ├─ 5. per line: address or continuation
  ├─ 6. JOIN continuations             ← before any tag work
  ├─ 7. extract grades + annotations   (§IV.5)
  ├─ 8. extract references             (§VII)
  ├─ 9. compute seals                  (§V)
  └─ 10. validate                      (§IV.6)
```

### IV.2 Library boundary

A pure C89 library — no I/O, no store, arena-allocated, `chorda` in and
out — so it is gated offline in root `probationes/` against fixtures. All
app concerns (rendering, panels, store writes) live above it.

Proposed: `include/sententiae.h` + `lib/sententiae.c`.

Latin naming (avoiding every `latina.h` macro):

| Concept | Name |
|---|---|
| document | `liber` |
| point | `sententia` |
| address | `locus` |
| address segment | `segmentum` |
| grade | `gradus` |
| grade vocabulary | `vocabularium` |
| part | `pars` |
| front matter | `prooemium` |
| seal | `sigillum` |
| cross-reference | `remissio` |
| free annotation | `annotatio` |
| orphaned comment tray | `limbus` |

`limbus` for the orphan tray is not a joke — comments whose point no longer
exists are in exactly that condition.

`Sententia` here is a **library-internal C struct**. It does not collide
with the store, which already has a genus of that name — see §IV.7.

### IV.3 Naming hazard

`casus`, `ordinarius`, `character`, `integer`, `constans`, `unio`,
`enumeratio`, `nomen`, `magnitudo`, `structura`, `externus`, `registrum`
are `latina.h` macros and are forbidden as identifiers. `ordinarius` and
`nomen` have each already cost a debugging session in this repo. The censor
hook will catch it; the point is not to spend the round trip.

### IV.4 Grade vocabulary — positional, not phrasal

Fran's rule: **any bracketed name appearing before the first addressed point
is a grade declaration.** Deduplicate.

Accept `[word]` and `[word: ...]` where *word* is a single lowercase run.
Skip markdown link forms `[x](y)`.

Verified against the exemplar:

- the prooemium yields **exactly the 8 grades**, no false positives
- no markdown links in the prooemium to confuse it
- **every grade used in the body is declared** — zero undeclared
- `[retracted: ...]` declares its own arity: the literal `...` marks it
  payload-carrying

This is superior to parsing the declaration sentence because it survives
rewording, and it requires no format change — the first and best application
of §II's rule.

### IV.5 Grades vs annotations

In the body, a bracketed span matching the vocabulary is a **grade**;
anything else is a **free annotation**. Shape alone separates them in
practice: grades are single lowercase words, while 15.1's
`[Fran's thesis; the strongest result in this document]` is capitalized and
contains spaces.

A bracket that *looks* like a grade attempt — a lone lowercase word not in
the vocabulary, e.g. `[principal]` — is accepted but **flagged**, since a
silent typo becoming a new grade is the failure most worth catching.

A point may carry both a grade and an annotation; 15.1 does.

### IV.6 Strictness — two tiers

**Structural errors block the paste and name the line.** These are the ones
that make the projection *wrong*:

- duplicate address within a document
- unclosed bracket
- an address whose parent does not exist
- malformed address (empty segment, uppercase segment)

**Anomalies are accepted and listed.** These make it merely *incomplete*:

- bracket that looks like an undeclared grade
- unresolvable internal cross-reference
- a line inside a fence that is neither address nor continuation
- no grade vocabulary found in the prooemium

Duplicate addresses block because two points would claim one anchor, and
comments would attach to the wrong claim — a lying anchor, which §VI treats
as the worst outcome in the system.

### IV.7 A genus named `sententia` already exists

Found by the board's dedup guard while filing this work, and verified live
against the store:

```
genus  sententia          (created 2026-07-23, by Fable 5)
campi  textus_sententiae  area     ← title field
       fons               textus
       annus              annus    ← default sort, ascending
```

That is a **quotation entity** — a saying with a source and a year. It is
not a document, and it is not a point in one.

This is a naming collision and must be handled deliberately:

- The document genus is **`liber`**, which is free. (Independently the
  right word: *Liber Sententiarum* is what a book of sentences is called.)
- The ancora genus is **`locus`**, not `sententia` (§VI.3).
- The C struct `Sententia` in the parser library is fine — it never enters
  the store's genus namespace.
- The `liber` field holding the verbatim markdown is `fons`, which is also a
  field name in the existing genus meaning *source/citation*. Different
  genera, different field namespaces, and `fons` for source text is already
  idiomatic in this repo (`gesta/fontes/`, `silva/fontes/`). Survivable, but
  named here so it is not rediscovered as a surprise.

**The collision is also an opportunity.** The two unify cleanly: a
*sententia* is a claim. The existing genus holds standalone ones carrying a
source and a date; a `liber` holds many, addressed and in argued order. The
obvious integration — deferred, but the reason §XII's "sigla as bibliography
entities" is more attractive than it first looked:

- an external reference like `ST I q.21 a.3` at 18.2 could become or link to
  a `sententia` entity
- typed relations (`citat` / `respondet` / `refutat`) would then connect
  standalone sentences to points in a liber and to each other

That relation vocabulary is named in the prior board item this work fulfils
(`01KYAJX10H`, filed from Fran's pipatum 2026-07-24: *"make sententiae a
first class thing that we can parse and then have addressible points"*).
This spec is the document half of that desideratum.

---

## V. Seals and drift

### V.1 What is sealed

Whitespace-normalized text, **grades excluded**:

0. normalize line endings (`\r\n` → `\n`, lone `\r` → `\n`)
1. join continuation lines with a single space
2. strip the leading `locus` and the space after it
3. remove bracketed spans matching the grade vocabulary (including payloads)
4. collapse whitespace runs to a single space
5. trim
6. hash with **SHA-256** — `sigillum_computare`, `include/sigillum.h`

Step 0 exists because line endings are the one realistic paste-buffer
hazard; the exemplar has zero CRLF, but a clipboard round trip can
introduce them. Three lines of byte code, pinned by a fixture whose twin
differs *only* in line endings.

**Never `chorda_friare`, `friatio_fnv1a`, or any 32-bit hash.** The house
draws this distinction deliberately — `friatio` hashes to *disperse* (table
buckets), `sigillum` hashes to *identify* (content addressing). At 32 bits,
273 points carries roughly a 1-in-10⁵ birthday collision chance per
document, and a seal collision means an anchor resolving `INTEGRUM` against
the **wrong claim**. That is the lying anchor §V.2 treats as the worst
outcome in the system, arrived at by arithmetic rather than by an edit.

### V.1b Normalization and hashing are two functions

```c
/* pure, public, separately testable */
chorda   sententiae_normalizare (chorda textus, constans Vocabularium*, Piscina*);
Sigillum sententiae_sigillare   (chorda textus_normalizatus);
```

Splitting them is what makes §XI's "rewrapping a paragraph leaves its seal
unchanged" a *useful* test. Asserted against the digest, a failure says only
that something differed; asserted against the normalized string, it says
exactly what. The digest test then becomes a thin wrapper over a fact
already pinned.

Rewrapping a paragraph, changing indentation, or reflowing to a different
column width does **not** break an anchor. Only changing the words does.

This matters because Fran hand-wraps at ~70 columns, so editing one word
mid-paragraph rewraps everything after it. A byte-exact seal would flag the
whole paragraph for a one-word change and train him to ignore the flags.

Grades are excluded so that promoting `[conjecture]` → `[theorem]` does not
flag every comment on the point. This was a close call: a change of
epistemic status is arguably exactly when you would want to re-read the
discussion. Recorded as reversible — it is a seal-function change plus a
reparse, and the projection is rebuildable.

Free annotations are *not* stripped, so they participate in the seal.

### V.1a No Unicode folding — measured, not assumed

The exemplar was inventoried character by character:

| | |
|---|---|
| Size | 45,144 bytes · 885 lines |
| Distinct non-ASCII characters | **2** |
| U+2014 em-dash | 52 (parenthetical breaks) |
| U+2013 en-dash | 1 — `books I–IV`, a correct range, not a slip |
| Curly quotes | **0** (44 straight `"`) |
| Tabs | **0** |
| Trailing whitespace | **0** |
| Longest line | 71 columns |

So normalization needs **no smart-quote folding, no tab expansion, and no
trailing-whitespace stripping beyond the general collapse**. Whitespace
handling operates on ASCII space and newline; UTF-8 bytes otherwise pass
through untouched.

Deliberately *not* adding Unicode folding, even defensively. If a future
paste ever does introduce curly quotes — drafting somewhere that
auto-substitutes them — the seal will correctly register that as a text
change, because it *is* one. Folding would hide a real edit in order to
prevent a flag that is accurate.

### V.2 Drift classification

At paste time, each existing anchor resolves to exactly one state:

| State | Condition | Behavior |
|---|---|---|
| `INTEGRUM` | address present, seal matches | nothing |
| `MOTUM` | seal matches at a *different* address | flag; report old and new address |
| `MUTATUM` | address present, seal differs | flag; show the text it was written against |
| `ORBUM` | address absent, seal found nowhere | to the `limbus` |

`MOTUM` is detected but **not auto-followed**. The anchor stays where it
was, flagged, with the likely new address named. Auto-following handles pure
reordering invisibly but must guess when text was both moved and edited —
and a wrong guess is a *lying* anchor, which is worse than a broken one
because nothing signals it.

### V.3 The paste gate

Because paste is the only way a document changes, it is the only moment the
cost of a change is knowable. So it is where the gate goes:

```
┌──────────────────────────────────────────────────┐
│  Pasting a new version of De Imagine             │
├──────────────────────────────────────────────────┤
│  Parse: 276 points (was 273), 14 parts      ✓    │
│  Structural errors: none                    ✓    │
│  Anomalies: 1  ▸ undeclared grade [principal]    │
│                                                  │
│  ANCHORS                                         │
│   241 unaffected                                 │
│     3 moved      4.obj.1.1 → 4.obj.2.1  (2 💬)   │
│     1 changed    13.2.1                 (1 💬)   │
│     1 removed    29.4                   (3 💬)   │
│                        └─ 3 comments → limbus    │
│                                                  │
│              [ Cancel ]   [ Accept paste ]       │
└──────────────────────────────────────────────────┘
```

Nothing is written until Accept. This is the house pattern — a gate that
names its cost — placed where the cost is actually computable.

---

## VI. Data model

### VI.1 The document

A **runtime-defined genus** in Res, so it inherits tags, relations,
comments, and Res listing without new machinery. Fields:

| Field | Notes |
|---|---|
| `titulus` | from the `#` heading |
| `siglum` | **canonical key**, uniqueness enforced |
| `slug` | derived alias, e.g. `de-imagine` |
| `status_libri` | from the prooemium (DRAFT etc.) — **not** `status`, see below |
| `fons` | the verbatim markdown — the truth |

**`status` is a reserved key and must not be used as a genus field.** The
reserved set is eight, not two:

```
genus · titulus · corpus · tags · ancorae · signatura · status · notae
```

Reserved keys are skipped by field judgment entirely, and `status` is
additionally projected to the `res.status` lifecycle column. A `status`
field in `campi` would therefore be silently unjudged *and* would collide
with the lifecycle machine. Hence `status_libri`, which gets judged like any
other field.

Two silent traps verified live in the store, both worth knowing before
building the paste path:

- **`status` inside `datum` bypasses the state machine entirely.** Field
  judgment skips it, the merge is blind, and the projection copies it to the
  column. A document could be given an invented lifecycle status with no
  complaint. Setting it at creation also suppresses `status_initialis`.
- **`datum` beats the `corpus` argument.** `addere` writes `corpus` first,
  then merges `datum` keys over it — so a `corpus` key inside `datum` wins
  silently.

**`fons` does not collide** with the `sententia` genus's field of the same
name: field namespaces are strictly per-genus. Confirmed by reading the
lookup path, not assumed.

A useful side effect of putting the body in a genus field rather than the
reserved `corpus` key: `corpus` is embedded in the entity `.md` projection,
so a 45KB `corpus` would rewrite a 45KB markdown file on every touch of the
entity. `fons` does not. (Moot for the forum daemon, which writes neither
projection — but it makes `fons` the right choice on the board too.)

The siglum is canonical because it is authored, short, already in the file,
and already what the cross-document citation apparatus uses. A slug is
derived, so retitling would silently change every URL. `/de-imagine/…`
resolves as a friendly alias.

### VI.2 Points

Points are **projection, not entities**. They are materialized as rows for
search and for the reference graph, and are discarded and rebuilt on every
reparse. They carry no identity of their own — their identity is the
authored `locus`.

### VI.3 Anchors

Comments and app-added point tags both anchor identically:

```json
{"genus": "locus", "scopus": "DI/4.obj.1.1", "sigillum": "<hex>"}
```

The ancora genus is `locus`, not `sententia` — the latter is taken by an
existing genus (§IV.7), and `locus` is in any case the more accurate word:
what is anchored is an address.

This is the existing `ancorae` shape, which already carries `sigillum` and
already reports CAUTIO when unresolved — so drift reporting has a precedent
to follow rather than a mechanism to invent.

**v2 must confirm** whether `genus` accepts a new value without a change to
`gesta`; the documented set is `symbolum | via | nid`.

### VI.4 Grades vs point tags

Two different things that must not be merged:

- **Grade** — authored in the document, belongs to the argument, changes
  only by editing the text. Per-document closed vocabulary; does *not*
  enter the global tag space, so `principle` cannot collide with unrelated
  forum tags and a future document may use the same word differently.
- **Point tag** — added in the app, belongs to the working process
  (`needs-source`, `for-1x12`). Anchored like a comment, and therefore
  subject to the same drift and orphaning.

---

## VII. References

### VII.1 Internal

Three syntaxes in the exemplar: `(see 11.)`, `(see 8.2)`, and bare
`(10.obj.1)`. All resolve to points in the same document and become links.

### VII.2 External

Siglum-prefixed: `ST I q.21 a.3`, and bare `NO` / `MS` mentions. These
render as citations and stay inert **until a document declaring that siglum
exists in the store**, at which point they light up automatically.

`MS` is *The Magnetic Sector*, Fran's own work. Pasting it as a sententiae
document would make every `MS` reference in *De Imagine* live, with no edit
to either document. This is why §VII.3 matters.

### VII.3 The siglum table is data

The prooemium's markdown table maps siglum → corpus. It must be parsed,
not merely rendered, because resolving external references requires knowing
which tokens are sigla at all.

### VII.4 Backlinks

Each point shows what cites it — standing on 13.5 and seeing that
15.obj.1.1 points here. Pure projection over data the parser already
extracts, so it costs no stored data.

This is what makes 273 points navigable rather than merely addressable.
Part XIV is literally six pointers to unfilled answer slots; with backlinks
it becomes a live view instead of a hand-maintained list.

---

## VIII. The reading view

### VIII.1 Verbatim, monospace, with a gutter

What is on screen is what is in the file — same line breaks, same hanging
indents, grades inline as written. The app never shows text that is not in
the document.

```
┌────────────┬──────────────────────────────────────────┬──────────────┐
│ PARTS      │ Part II — On the mask                    │  4.1         │
│            │                                          │  ─────────── │
│ I  Method  │ ·  4. Is a persona a mask over something │  💬 2        │
│ ▸II On the │       else?                              │              │
│    mask    │ ·  4.1. No. The picture requires a real  │  fran        │
│ III On     │ 💬2    interior thing standing behind a  │  This is the │
│    formatn │       presented one, which is a          │  strongest   │
│ IV …       │       Cartesian commitment, not a        │  claim in    │
│            │       finding. [theorem]                 │  Part II.    │
│ ─────────  │ ·  4.1.1. On a hylomorphic account the   │              │
│ QUESTIONS  │       operations are not a screen in     │  ┌─────────┐ │
│  1. What   │       front of the substance; they are   │  │ reply…  │ │
│     disti… │       how the substance is present.      │  └─────────┘ │
│  2. Are    │                                          │              │
│     the p… │ ·  4.obj.1. Objection: the substrate     │  cites this: │
│  …         │       demonstrably supports other        │   15.obj.1.1 │
│            │       characters — this is what          │              │
│ [ 16.3.2 ]│       jailbreaks exploit.                 │              │
│  ↑ jump    │                                          │              │
└────────────┴──────────────────────────────────────────┴──────────────┘
   sidebar              verbatim column                    side panel
```

### VIII.2 Navigation

- **Part sidebar**, listing the 14 parts and the 30 top-level questions.
  Those questions are already the document's real table of contents — they
  are interrogative headings by construction.
- **Jump box** taking a bare address: `16.3.2`, `4.obj.1.1`.

### VIII.3 Threads open in a side panel

The text column never reflows or shifts. Inline expansion was rejected
because it moves the document under you and puts things in the verbatim
column that are not in the file — precisely what §VIII.1 exists to prevent.

### VIII.4 Retracted points

Dimmed, fully readable, with the `[retracted: …]` payload shown as the
correction. This matches the document's own stance at 29: *recorded so the
corrections are addressable, not to display contrition.*

Retracted points must not render identically to live ones, since scanning is
the one situation where the distinction matters most.

### VIII.5 Document header

Parsed prooemium fields — title, siglum, status, grade vocabulary, siglum
table — render as a structured header. The prooemium prose still renders
below it, unchanged.

---

## IX. Generated views and reconciliation

Parts XIII (retractions) and XIV (open questions) are hand-maintained and
fully derivable from the `[retracted]` and `[unresolved]` grades.

The app **generates the views and reconciles them against the source**,
flagging disagreement in both directions:

- a point graded `[unresolved]` with no entry in Part XIV
- a Part XIV entry whose target is no longer `[unresolved]`

The hand-written sections stay in the document. It remains self-contained
and portable — checkable by someone who has never seen the app, which is
28.2.1's whole claim — while the tool catches the drift that hand
maintenance always eventually produces.

Reconciliation is advisory. It never edits the document (§II.3).

---

## X. Search

Points are indexed individually; results group by document. Searching
*misericordia* returns 18.2 itself, with its address, grade, and part,
nested under *De Imagine*.

### X.1 Implement by parsing `fons` at query time — no store change

Three mechanisms exist. The store was inspected to rank them:

| | Cost | Verdict |
|---|---|---|
| Points as **entities** | 273 `creatio` events per paste, 273 res + FTS rows, and **gesta has no delete** — a re-paste can only add | **Never.** Also gives points ULIDs, the identity §VI.2 refuses to bind to |
| A **`puncta` side table** + its own FTS table | one migration, a materializer, and `gesta_replicare` must learn to clear it | The right answer eventually |
| **Parse `fons` in the app at query time** | nothing — the parser exists by then | **v1** |

A few hundred KB of documents parsed on demand is trivial, and it preserves
the rebuildable-projection property perfectly. The `puncta` table becomes
worthwhile when document count makes it so; deferring costs nothing because
there is no data to migrate.

**Document-granularity search is already free.** The store's FTS corpus
builder sweeps every string-valued datum field, so a `fons` field is
whole-document searchable the moment it is written — verified live by
searching for terms that occur only deep inside the body. What v1 adds is
*point* granularity on top of a document hit.

### X.2 There is no stemmer — prefix queries are mandatory

The FTS tokenizer is `unicode61 remove_diacritics 2` with **no stemmer
configured at all** — not merely "English stems only". So `misericordia`
will not match `misericordiam`.

For a corpus this Latin, that is not a footnote: the search UI must append
`*` to query terms the way `_similia_reddere` already does internally, or
inflected searches will fail silently and look like absent content.

### X.3 `gesta_quaerere` cannot express grouping

`LIMIT 50` is hardcoded, results are `{res_id, genus, titulus, status}` with
no snippet, offsets, or rank, and MATCH syntax errors return empty rather
than erroring. Grouping by document needs a new query function, a wider
result struct, a per-document limit, and a new tool or mode flag. None of it
is hard; none is reachable by extending the existing call. Since v1 parses
in the app (§X.1), none of it is needed yet either.

---

## XI. Build order

All four bundles ship as v1 by Fran's decision. They are staged internally
by dependency, not by attractiveness.

| Stage | Content | Gate |
|---|---|---|
| **S0** | parser core: locus, sententia, continuation joining, parts, prooemium fields, grade vocabulary | pure, offline, `de-imagine.md` as fixture |
| **S1** | seal + normalization + drift classification | pure, offline, twin-document fixtures |
| **S2** | store shape: `liber` genus, point materialization, `sententia` ancora | headless |
| **S3** | reading view: verbatim column, gutter, sidebar, jump box, header | hand-run |
| **S4** | comments, side panel, paste damage preview, `limbus` | headless where possible |
| **S5** | references, external resolution, backlinks | pure over parser output |
| **S6** | generated views + reconciliation | pure |
| **S7** | point tags + point-level search | headless |

S0 and S1 are pure functions over text and carry the entire risk of the
feature. They should be gated hard, offline, and completely, before any UI
exists — the villa lesson in reverse: fixtures prove parsing, and this
*is* parsing.

### Test corpus

`de-imagine.md` is a real 273-point document and should be the primary
fixture. Assertions worth pinning, each of which caught something during
the interview:

- 273 points, 14 parts, max depth 4
- all 7 retracted points found — **not 1** (the multi-line bracket trap).
  Note 8 *lines* mention `retracted`; the eighth is the vocabulary
  declaration in the prooemium, not an applied grade
- 21 `obj` addresses parsed
- 15.1 carries both a grade and a free annotation
- exactly 8 grades collected from the prooemium, zero undeclared in the body
- 17 internal references resolve
- rewrapping a paragraph to a different column width leaves its seal
  unchanged

---

## XII. Out of scope for v1

- **Rendered export.** Copy-source ships (nearly free — the store holds the
  markdown verbatim). Static HTML with stable per-point anchors is the
  strongest post-v1 candidate: it is 28.2.1's external-checkability claim
  actually honored rather than merely asserted.
- **Semantic named segments.** `obj` stays opaque. An optional front-matter
  glossary is the natural growth path and follows the §IV.4 pattern.
- **Range and phrase anchors.** Points only.
- **Commentable front matter and part headings.** They have no authored
  address, so anchors would key on text alone and drift easily.
- **Sigla as bibliography entities.** Making `ST`, `NO`, `MS` first-class
  would be genuinely useful and is clearly its own feature.
- **Grades on comments.** A reply marked `[objection]` would extend the
  argumentative structure into the discussion. Attractive; not v1.
- **Document versioning.** Every paste replaces; history lives in the event
  stream. Readable prior versions were considered and deferred.
- **Global tag projection for grades.** Deferrable at zero cost.

---

## XIII. Open questions for v2

1. Does the `ancorae` `genus` field accept a new value (`sententia`) without
   a change to `gesta`? Documented set is `symbolum | via | nid`.
2. Where does the reconciliation report surface — document view, Res entity,
   or a global surface?
3. Should the grade filter rejected in navigation return once generated
   views exist, given they are the same mechanism?
4. Where does the paste UI live, and is there a library view listing all
   sententiae documents?
5. ~~Does seal normalization need to fold em-dashes and smart quotes?~~
   **ANSWERED — no.** The exemplar contains exactly two non-ASCII
   characters (em-dash ×52, en-dash ×1), zero curly quotes, zero tabs, zero
   trailing whitespace. See §V.1a.
6. ~~Size ceiling on a document-body field?~~ **ANSWERED — none in the
   store.** No length check exists in field validation; SQLite's limit is
   1 GB and the MCP transport's is 64 MiB. Proven live: the full 45,144-byte
   document was stored and auto-FTS-indexed with no error. `forum.db`
   already holds a **51,473-byte** record in daily use, so this is not even
   unprecedented. **But see §XIV — the ceiling is in the app, not the
   store.**
7. ~~Can the Scholia tab filter anchored comments?~~ **ANSWERED — no.** No
   filter, sort, collapse, pagination, or virtualization exists anywhere in
   that tab, and `_res_legere` hard-caps at 200 with no offset parameter
   anywhere in the stack. 200 comments on one document would both flood the
   view and hit the cap exactly.
8. Should a point in a `liber` be relatable to a standalone `sententia`
   entity via typed relations (`citat` / `respondet` / `refutat`), and does
   the forum's relation layer already support typed verbs? (§IV.7)
9. Does the existing `sententia` genus want a `liber` + `locus` field pair
   so a quotation can record where in a document it lives — or is that
   backwards, with the liber owning its points and the quotation merely
   citing them?

---

## XIV. The ceiling is in the app, not the store

Two explorations converged on this from opposite directions, which is why it
is stated as a section rather than a caveat.

**The store does not care.** No length validation, 1 GB SQLite limit, 64 MiB
MCP transport, a 51,473-byte record already living in `forum.db`.

**The forum's own client does.**

```c
lib/cliens_tabularii.c:16
#define RESPONSUM_CAPACITAS 262144      /* 256 KB per daemon response */
```

One `liber` measured **47,176 bytes** through `legere` — the whole `datum` is
embedded per row with no truncation and no field filter. So:

> **Five or six documents saturate the forum's response buffer.**

The overflow is not clean: the read loop breaks mid-stream and returns a
truncated buffer, `json_legere` fails, and the UI shows a generic error —
which the app then likely swallows anyway, because `internuntius.vocare`
rejects with an `Error` whose text is on `.message` while the app's error
display reads `.nuntius`. **The failure mode for the sixth document is a
blank tab.**

Two existing behaviors make it worse rather than better:

- `setInterval(anquirere, 2000)` refetches everything for the active view
  every two seconds
- the Tags tab sweeps *all* user-defined genera, which would include `liber`

So the naive implementation pulls every document body, in full, thirty times
a minute, and dies silently at the sixth document.

### Required design amendments

1. **`fons` must never appear in a list read.** Listing shows a summary
   projection — title, siglum, point count, status. The body is fetched only
   when one document is opened.
2. **Exclude `liber` from the tags sweep**, exactly as `codex` and
   `scholium` already are.
3. The 200-item list cap with no offset anywhere applies to every read, so
   any future points-as-rows design is capped at 200 of 273 points — another
   reason §X.1 parses in the app instead.
4. Worth filing separately: `legere` embedding full `datum` per row with no
   field filter is a general context-budget hazard, not a sententiae
   problem. Fifty libri would be a ~2.3 MB response.

### Also true of the app layer

- **`apps/forum/forum.sh` omits `capsula_generare`.** Villa's launcher does
  it and names the bug in a comment. Every edit to `index.html` is invisible
  until the capsula is regenerated by hand. They are currently in sync, so
  the staleness is latent. **Fixing the launcher is step zero of S3.**
- **No runner sweeps `apps/`.** `compile_tests.sh` globs `probatio` and
  `probationes` only. The forum app has zero automated coverage that any
  runner will execute — which is precisely why §XI puts the parser and seal
  in root `probationes/` and only rendering in the hand-run fumus.
- **Everything is blocking on the UI thread.** A C-side parse of a 45KB
  document inside a handler freezes the window for its duration. Fine for a
  fast parser; a reason to parse one document per call rather than a
  library.
- **Polling clobbers live inputs** unless the element carries
  `data-inter="verum"`. The paste box and the jump box both need it.
