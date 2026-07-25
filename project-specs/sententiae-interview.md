# Sententiae — interview record

**Date:** 2026-07-25
**Feature:** structured projection of explicitly-addressed outline documents
in the forum app
**Exemplar:** `de-imagine.md` (repo root) — 273 points, 14 parts
**Format:** raw Q&A, 20 questions over 5 rounds, plus the design conversation
that preceded them.

This file is the record. The spec derives from it. Where an answer settled
something the spec now states flatly, the reasoning lives here.

---

## Part 0 — decided before the interview began

These came out of ordinary conversation, not the question tool, and they
frame everything after.

### 0.1 The document is the res; the point tree is a projection

Fran: *"that's a great idea to have the document be the res and then have the
structured version be a projection. that seems like it gives us a lot of
flexibility to even improve the projection over time"*

The document's markdown is stored verbatim as truth. The parsed point tree
is derived and rebuildable. Consequence: the parser can improve forever with
no data migration, because the source was never lossy. Same architecture as
`tabularium.jsonl` (truth) vs the `.db` (rebuildable projection), which also
implies wanting a reparse-everything command eventually.

### 0.2 Anchors point at authored data, never at parser output

The address `4.obj.1.1` is written in the file in Fran's hand. The node the
parser builds around it is derived. Comments anchor to the **authored
address plus a seal of the point's text** — never to a parser-assigned id.

Without this, a parser improvement silently couples comment history to
parser version, which would take back the flexibility of 0.1. This is the
condition that makes 0.1 real rather than nominal.

### 0.3 The parse is one-way

Edits happen to the document text. Never to projected points.

The creep to resist: reading a point in the app, spotting a typo, fixing it
in place. The moment the UI writes back into a point, the parser must also
become a printer, and `print(parse(x)) == x` over hand-wrapped text with
hanging indents is dramatically harder than parsing. Keeping the parse
one-way is what keeps the whole feature cheap.

### 0.4 The format is the API

Steenberg's rule applied: the markdown convention is what's in Fran's
fingers and in 273 existing points, so it is expensive to change. The
parser is implementation and can be replaced freely. Therefore prefer
parser cleverness over format changes wherever there's a choice.

---

## Findings from analyzing the exemplar

Measured, not assumed — these drove several questions.

| Finding | Value |
|---|---|
| Addressed points | 273 |
| Addresses at column 0 | **all of them**, zero exceptions |
| Nesting | entirely in the address, never in indentation |
| Continuation lines | indented past the address, aligned under the text |
| Max depth | 4 segments (`4.obj.1.1`) |
| Parts | 14, each one fenced block |
| Non-numeric segments | `obj` only, 21 occurrences |
| Grades declared | 8 |
| Grades used but undeclared | **0** |
| Internal cross-refs | 17 |

### Three parsing traps found by measurement

**1. Tags span continuation lines.** A line-based tag extractor found 2
retracted points; there are 7, of which six open on one line and close on
another (an eighth `retracted` line exists, but it is the vocabulary
declaration in the prooemium, not an applied grade):

```
29.2. "Unity by art is a deficiency." [retracted: defeated by the
      carved stone — art perfects, and the worked stone is really
      better than the raw one; see 23.1]
```

Therefore: **join continuations first, then extract tags.** This bug was
demonstrated live before it was written down — the first grep undercounted
by 75% and did so silently, which is the dangerous part.

**2. Address segments are number-or-name.** A digits-only grammar drops all
21 `obj` points — i.e. every objection, which is where the argument lives.

**3. Not every bracket is a grade.** 15.1 carries `[conjecture]` *and*
`[Fran's thesis; the strongest result in this document]`. Grades are single
lowercase words; the annotation is capitalized with spaces. Shape separates
them.

---

## Round 1

### Q1. Drift — what happens when a re-parse finds a seal mismatch?

Context: the document's own front matter says *"Addresses not frozen.
Reorder freely until first publication."* So drift is expected, not
exceptional.

**Chosen: flag in place, Fran judges.** The comment stays at that address,
renders with a CAUTIO badge, and shows the text it was originally written
against. Nothing moves automatically; nothing is lost.

Rejected:
- *Detach to a tray* — honest, but throws away the location hint
- *Follow the text automatically* — handles pure reordering invisibly, but
  silently guesses when text was moved AND edited, and a wrong guess is a
  lying anchor (worse than a broken one)
- *Freeze on first comment* — commenting would become an act that partially
  publishes the document; the tool would start constraining the writing

Matches how `ancorae` already behave in the tabularium.

### Q2. Rendering — how should a document look?

**Chosen: verbatim monospace + gutter.** Render the fenced text nearly
exactly as authored; comment targets and counts live in a left gutter.
What you see in the app is what's in the file.

Rejected:
- *Structured tree* (reflowed, chips, collapsible) — readable at any width
  and scannable by grade, but the app stops looking like the file
- *Hybrid with a toggle* — best of both, ~2× rendering work and a second
  thing to keep gated

Consequence accepted: fixed width, long points may need horizontal scroll.
This choice makes several later ones (threads in a side panel, navigation
by sidebar) follow almost forcibly, since anything that reflows or moves
the column costs the thing that was just bought.

### Q3. Grades — global tag space or per-document vocabulary?

**Chosen: per-document vocabulary.** The document declares its own closed
set; the parser validates against it and flags unknown brackets. Grades do
not touch the global Tags tab, so a future document may use the same words
differently and `principle` cannot collide with unrelated forum tags.

Noted at the time: because the projection is rebuildable, adding a global
tag projection later is a parser change with no migration — so this defers
the namespace question at genuinely zero cost.

### Q4. Generated views — Parts XIII and XIV are derivable. Generate them?

**Chosen: generate AND reconcile.** The app shows live views derived from
the grades, and checks them against the hand-written sections, flagging
disagreement in both directions — an `[unresolved]` point missing from Part
XIV, or a Part XIV entry whose target is no longer unresolved.

Rejected:
- *Generate, leave source alone* — the two can silently disagree, which is
  the exact failure hand-maintenance already has
- *Generate, delete the source sections* — the `.md` stops being complete on
  its own, cutting against the document being checkable outside the app
- *Generate nothing* — leaves the most obviously mechanical work manual

The document stays self-contained and portable; the tool catches drift.

---

## Round 2

### Q5. Ingest — where does the markdown live?

**Chosen: paste into the app; the store is truth.** No filesystem coupling
at all. The res holds the text; re-pasting is how a document changes.

Rejected:
- *Import from path, store snapshots it* (the recommendation) — Fran chose
  the simpler coupling instead
- *File on disk is truth, app watches* — a moved or deleted file would break
  every anchor into it
- *Import plus watch* — drift could then appear unannounced

Consequence: documents need not exist as files at all, and **paste is the
single moment where everything is re-evaluated.** That makes the paste-time
gate (Q9) load-bearing.

### Q6. Home in the app

**Chosen: a genus in Res, with a dedicated reading tab.** The document is a
runtime-defined genus, inheriting tags, relations, comments, and Res listing;
the reading tab exists because 273 points need a gutter and navigation the
generic entity view will not have.

Rejected: genus-only (cramped), extending Articuli (article assumptions —
one prose body, one comment stream — fight the point-level model), a fully
standalone tab (opts out of everything genera already does).

### Q7. Comments — what kind of thing are they?

**Chosen: ordinary forum entities carrying an anchor.** A comment is a normal
scholion with an `ancora` (address + seal). It appears in the document gutter
and the Scholia tab, threads via `respondet_ad`, is searchable and taggable.
No second comment system.

Rejected: hiding them from global views by default, document-local
annotations, and grades-on-comments (the last is interesting — a reply
marked `[objection]` would extend the argumentative structure into the
discussion — but was not chosen for v1).

### Q8. Anchor granularity

**Chosen: whole points only.** One anchor = one address = one point.

Rejected:
- *Points, parts, and document* — parts have no authored address, so their
  anchors would key on heading text, which is weaker
- *Ranges* — natural for objections, but a range breaks if any point inside
  it moves, so it drifts more than its endpoints suggest
- *Phrases within a point* — true marginalia and by far the most fragile;
  character offsets break on any rewrap, so nearly every re-import would
  flag something

---

## Round 3

### Q9. Re-paste semantics

**Chosen: preview the damage, then confirm.** The paste shows what the new
parse found and exactly which anchors would break — "this point moved, 2
comments affected" — before anything is written. Commit or cancel.

Rejected: replace-then-flag (you learn what broke after it broke), keeping
every paste as a version (strongest fidelity, most machinery, and raises
which version is "the" document), preview + versions (most careful, most to
build).

House pattern: a gate that names the cost, placed at the one moment the cost
is knowable.

### Q10. How does the parser learn the grade vocabulary?

**Fran's answer, in his own words:** *"i think that we can parse the existing
prose line, and i would say not relying fully on phrasing but any [name]
before the points start in the kind of 'front matter' are probably safe to
assume are tags being defined (so de-duplicated probably just to be sure)"*

So the rule is **positional, not phrasing-based**: any bracketed name in the
region before the first addressed point is a grade declaration. Deduplicate.

Verified against the exemplar immediately:

- front-matter region (lines 1–28) yields **exactly the 8 grades**, no false
  positives
- **no markdown links** `[x](y)` in front matter to confuse it
- **every grade used in the body is declared** — zero undeclared
- `[retracted: ...]` declares its own arity: the literal `...` marks it as
  payload-carrying

This is better than parsing the phrasing because it survives Fran rewording
the declaration sentence, and it requires no format change.

Guard added: require the bracket content to be a single lowercase word
optionally followed by `: ...`, and skip `[x](y)` link forms. That shape
naturally excludes free annotations like `[Fran's thesis; …]`.

### Q11. Parser strictness

**Chosen: two-tier — structural errors block, anomalies flag.** Things that
would corrupt addressing (duplicate addresses, unclosed brackets) refuse the
paste and name the line. Everything else (unknown grade, unresolvable
cross-ref, stray line) is accepted and listed as a warning.

Rejected: accept-everything (a duplicate address means two points claim one
anchor and comments attach to the wrong one), refuse-on-anything (blocks
over things Fran doesn't care about), accept-but-block-on-anchor-damage.

The principle: block only what makes the projection *wrong*, not what makes
it *incomplete*.

### Q12. Cross-references

**Chosen: internal live now, external resolve when present.** Internal refs
become links immediately. External siglum refs render as citations and stay
inert until a document declaring that siglum exists in the store, at which
point they light up automatically.

Concretely: `MS` is *The Magnetic Sector*, Fran's own work. Pasting it as a
sententiae document would make every `MS` reference in *De Imagine* live,
with no edit to either document.

Rejected: internal-only, and making the siglum table into first-class
bibliography entities (genuinely useful, clearly beyond v1).

---

## Round 4

### Q13. What exactly gets sealed?

**Chosen: whitespace-normalized text, grades excluded.** Join continuation
lines, collapse whitespace runs, strip the address and grade tags, then
hash. Rewrapping a paragraph, fixing indentation, or reflowing to a
different column width will not break an anchor — only changing the words
will.

Rejected:
- *Grades included* — a promotion from `[conjecture]` to `[theorem]` would
  flag every comment on the point. Defensible (a change of epistemic status
  is exactly when you'd want to re-read the discussion) but rejected as too
  noisy for v1
- *Raw bytes* — maximum sensitivity, and with ~70-column hand-wrapping,
  editing one word rewraps the rest of the paragraph anyway
- *Also excluding cross-refs* — most forgiving; risks treating a substantive
  addition as cosmetic

### Q14. Navigation

**Chosen: part sidebar + jump-to-address box.** The sidebar lists the 14
parts and the 30 top-level questions — which are already the document's real
table of contents, since they are interrogative headings. The jump box takes
a bare address (`16.3.2`, `4.obj.1.1`) and scrolls to it.

Rejected: jump box alone (assumes you always know where you're going),
adding a grade filter (deferred), full collapsible outline of all 273 (the
outline becomes a thing you must navigate).

### Q15. Where does a comment thread open?

**Chosen: right side panel.** The text column never reflows or shifts.
Reading position is preserved and the point and its discussion are visible
together.

Rejected: inline expansion (pushes text down, and puts things in the
verbatim column that aren't in the file — precisely what Q2 was chosen to
avoid), popover (awkward for writing), bottom drawer (the point can scroll
out of view).

Note how Q2 constrains this: verbatim rendering makes the side panel nearly
forced.

### Q16. Document identity

**Chosen: siglum canonical, slug as alias.** `DI/4.obj.1.1` is canonical;
the app enforces siglum uniqueness across documents. `/de-imagine/4.obj.1.1`
resolves as a friendly alias.

Reasoning: the siglum is authored, short, already in the file, and already
what the cross-document citation apparatus uses. A slug is derived, so
retitling would silently change every URL.

---

## Round 5

### Q17. Is `obj` semantic or opaque?

**Chosen: opaque — a segment is a number or a name.** The parser knows only
that segments can be words; `obj` renders like any other component. A future
document using `resp` or `sc` needs no code change.

Rejected: an optional front-matter glossary (same pattern as the grade
vocabulary; a natural growth path), hardcoding `obj = objection`, and a
built-in scholastic set (`obj`/`resp`/`sc`).

Justified explicitly by 0.1: since the projection is rebuildable, teaching
the parser meaning later costs nothing. YAGNI applies with unusual force
here because the usual penalty for deferring — migration — does not exist.

### Q18. Can Fran add his own tags to a point?

**Chosen: yes — app-added tags, distinct from grades.** A point can be
`[conjecture]` because Fran wrote that, and also tagged `needs-source`
because he added it in the tool. The grade belongs to the document; the tag
belongs to the working process.

Important consequence: **app tags are per-point state that is not in the
document**, so they anchor exactly like comments (address + seal) and share
the drift and orphan behavior.

Rejected: document-only (cleanest, but nothing to note things with except
comments), a fixed mark set (star/flag/todo), feeding the global Tags tab.

### Q19. Search granularity

**Chosen: points indexed individually, results grouped by document.**
Searching *misericordia* returns 18.2 itself — with its address, grade, and
part — nested under *De Imagine*. You land on the claim.

Rejected: flat point results (a common word floods the list), documents only
(throws away most of the value of having parsed it), a toggle.

This is the biggest store-side change in the feature: points must be
materialized as searchable rows.

### Q20. The non-fenced prose

**Chosen: parse into real fields, keep the prose.** Extract title, siglum,
status, grade vocabulary, and the siglum table into structured fields shown
as a document header; the prose still renders below, unchanged.

The siglum table especially: resolving `ST` or `MS` references requires
knowing they are sigla at all, so Q12 depends on this.

Rejected: render-as-markdown-only, render-everything-verbatim (the siglum
table is a real markdown table and would look worse than on GitHub),
making front matter commentable (needs a second, weaker anchor kind).

---

## Round 6 — scope and edges

### Q21. What happens to comments on a DELETED point?

**Chosen: orphan tray per document.** The comment leaves the reading view
and lands in a tray carrying the address it was written against and the
sealed text of the point that is gone.

Rejected: keeping the dead point visible struck through (the app would show
text that isn't in the file — the one thing verbatim rendering exists to
avoid), blocking the paste until resolved (friction at the worst moment),
treating deletion as undifferentiated drift.

### Q22. How does a retracted point read?

**Chosen: dimmed, with the retraction reason shown.** The claim recedes
visually but stays fully readable, with its `[retracted: …]` payload
displayed as the correction.

Matches the document's own stance at 29: *recorded so the corrections are
addressable, not to display contrition.*

Rejected: no special treatment (retracted and live claims look identical
while scanning, the one place it matters), collapsed by default (you lose
seeing a correction in passing), linking to what superseded it — this last
is attractive and mostly derivable, since the payloads name targets
(`[retracted: … corrected at 13.2]`), and is the obvious first enhancement.

### Q23. v1 scope

**Chosen: all four bundles ship in v1.**

1. Comments, anchors, and the damage preview
2. Reference resolution and backlinks
3. Generated views and reconciliation
4. App-added point tags and point-level search

Noted: this is a large v1. It will be staged internally even though it ships
as one thing, and the staging should follow dependency order rather than
feature attractiveness.

### Q24. Export

**Chosen: copy source now, rendered export later.** Getting the verbatim
markdown back is nearly free since the store holds it, so ship that and
defer anything rendered.

Rejected for now: static HTML with stable per-point anchors (this is the
external-checkability claim of 28.2.1 actually honored rather than merely
asserted, and is the strongest candidate for the first post-v1 addition),
nothing at all, HTML including discussion.

---

## Open questions carried into the spec

1. Does the reconciliation report live in the document view, the Res entity,
   or somewhere global?
2. Should the grade filter from Q14's rejected option come back once the
   generated views exist, given they are the same mechanism?
3. Where exactly does the paste UI live — new-document flow, and is there a
   library/list view of all sententiae documents?
4. Does the seal normalization need to handle em-dashes, smart quotes, and
   other characters that differ between an editor and a paste buffer?
