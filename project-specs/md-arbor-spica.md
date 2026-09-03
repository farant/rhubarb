# md → html by STML commands — the spike (2026-09-02, night)

Before the markdown parser has a spec, the question was whether the STML
command layer (EXEMPLAR / PER / CATENA / DIRIBITIO / SINE / DESCENSUS /
the bridge) can carry the markdown-tree → html-tree transform, and what
it lacks. Two hand-written documents in `exhibita/md-html-spica-*.stml`
(a heading with emphasis, a reference link, a nested list) were run
through `stml expandere`. Everything below is measured on those runs and
checked against `lib/stml_macros.c`, not inferred.

## Findings

1. **There is no in-place substitution; PER emits a flat list where it
   stands.** `<PER congruentia="$capitula"><h2>&@c;</h2></PER>` produced
   the `<h2>` at the PER's position wrapping the RAW `<capitulum>`
   subtree — children untransformed. Emphasis matched separately and
   was emitted as its own top-level row, not inside the heading.
2. **Nested matches double-emit.** The inner list appeared twice: once
   raw inside the outer list's spliced subtree, once as its own row.
   Floating application visits every subtree root; a transform needs
   one visit per node with results assembled at the node's position.
3. **Therefore the missing verb is a walk** — apply-templates by another
   name: walk the ORIGINAL subtree; at each node pick a rule by pattern
   (DIRIBITIO-shaped cases); emit the rule's body with a bound
   `&@liberi;` = the transformed children; unmatched nodes pass through
   (identity; text as text). It never re-matches its own output, so it
   sits inside the strata law (results are never re-matched; cycles
   impossible) rather than against it. Working name: AMBULATIO.
4. **Value captures interpolate into attributes** (`gradus="&@g;"`
   worked) **but cannot name an element**: `<h&@g;>` is not a tag.
   Heading levels are a six-case DIRIBITIO today, which needs no engine
   change, or a titled-by-value element form — decide at spec time.
5. **The bridge is intra-row.** A CATENA stage fills the next pattern
   from the row's captures and matches WITHIN the row's subtree;
   `radix="fontis"` retains the incoming row's root, `de=` only pipes
   relations. A document-level lookup (reference link → its definition
   elsewhere) is a join across two relations, §8 door 6, and produced
   zero rows here without a fault. Recommendation: the PARSER resolves
   reference links, which is CommonMark's own semantics (resolution is
   parsing), so the transform never needs the join; the door stays shut.
6. **Refusals are loud where it counts:** a collected relation nobody
   consumes is a fault (`EXITUS_NON_CONSUMPTUS`) — the first run
   stopped on it.
7. **The extent-provenance decree is now pulled.** Exemplaria spec §1.3:
   before any transform verb rides matches, positus must survive
   expansion. The markdown transform is that first transform consumer.
8. **The projection shape held.** Blocks carrying `<inlinea>` with
   `<textus>` leaves and inline elements, attribute values as match keys
   (`gradus`, `genus`, `laxa`) — patterns and captures had no trouble
   with it. Fences were not exercised.

## What this puts in the plan

- A transform arc task BEFORE the html oracle: spec + build AMBULATIO in
  `lib/stml_macros.c` with its own birth gate (this spike's documents,
  expected html pinned), plus the §1.3 extent rule.
- Reference links resolved in the parser (`url=` on the link node).
- Heading levels by DIRIBITIO unless the interview prefers the
  titled-by-value form.
- HTML serialization stays `stml vertere` over the produced html tree.
- The html oracle then compares `vertere`'s bytes with CommonMark's
  expected html under a whitespace normalizer.
