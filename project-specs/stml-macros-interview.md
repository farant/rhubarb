# STML Macros — Interview (raw transcript)

2026-08-25, Fran + Fable. Per mutatio-visio §7: sigil/surface decisions
happen at spec time, interview-first, raw transcript archived (the
G1/G2 recovery lesson). Consolidation target: an stml-macros spec in
project-specs/.

Context read before round 1: stml-visio.md (whole, §9.1 the decreed
pins), mutatio-visio.md (whole), arbor-parsura-spec.md §4 (slots
RESERVED with `&@` syntax).

Decreed before this interview (visio §9.1, not re-litigated):
tree-level substitution with provenance; expansion in strata
(stratum sees only earlier strata, termination by construction);
query fragments subsume conditional inclusion (arms retained);
"macros = fragments if they could take arguments" (Fran).

## Round 1 — scope + consumer

**Q: Where does the v1 scope line fall?** (options: params-first /
full §9.1 trio / resolution-only)

**Fran:** "we can do params first, if you look in arbor-parsura-spec.md
you can see some of the plan for slots with &@ syntax although i am
open to refining that as makes sense"

**Q: Which real consumer drives the walking skeleton?** (options:
arbor / natura frame templates / silex-vitrea templates / canon
census)

**Fran:** Arbor (the recommended option — expansion bodies and
ambiguity arms are a DAG in tree clothes; M2 complete and gated, so
the consumer exists and byte-exact oracles come free).

Consequence noted: v1 = parameterized fragments with the parsura
spec's `&@` slot design as the base syntax (refinable); arbor's
parsura documents are the first consumer; query fragments staged
later.

## Round 2 — call form, nesting, provenance, truth posture

**Q: Call form — visio §6 reserved `<<#f attr="v">>`; parsura §4's
example calls with self-closing `<#lex-zephyrum position="123"/>`.
Which?**

**Fran:** `<<#f args>>` (recommended option) — invocation stays
reference-shaped, `<#id>` stays purely definitional, transclusion
resolution delivered as the same machinery.

**Q: May a macro body call other macros in v1?**

**Fran:** Document-order strata (recommended) — a body may call only
macros defined EARLIER in the document; strata = document order;
forward reference = loud vitium; termination by construction with
zero graph machinery.

**Q: Provenance shape in v1?**

**Fran:** Side table (recommended) — the sedes precedent: node →
macro id + call site, StmlNodus untouched, queries join the table.

**Q: Truth posture — file keeps macro form, expansion pure tree→tree
in memory at load, writer writes the unexpanded tree, original tree
stays queryable?**

**Fran:** Confirmed.

## Round 3 — children args, definitions, resolution scope, engine home

**Q: Call-site children — `<<#f args>>` is a leaf; where do child
arguments go in v1?**

**Fran:** Attribute-args only (recommended). `&@...children;` stays
RESERVED with semantics written; arbor's shared bodies are closed
shapes with attribute holes; no paired-transclusion syntax invented
under no pressure. (Noted alternative for the future: capturing
transclusion — reusing capture machinery for call-site children —
recorded as a reservation candidate, not designed.)

**Q: Definitions in the expanded projection?**

**Fran:** Dropped (recommended) — expanded tree is the CONTENT view;
definitions remain queryable in the unexpanded tree, which the truth
posture keeps.

**Q: Resolution scope — `<<#id>>` resolves same-document; selector
transclusion stays RESERVATUM?**

**Fran:** Confirmed.

**Q: Engine home?**

**Fran:** New TU, lib/stml_macros.c (recommended) — expander depends
only on the public tree API (which also proves that API sufficient);
amalgam/aedilis pick it up as a normal lib.

## Round 4 — naming, sequencing, blessing

**Q: Latin for SLOT?**

**Fran:** loculus (recommended) — loculi are literally
slots/pigeonholes; vitium messages read naturally (loculus non
impletus, loculus ignotus).

**Q: Milestone boundary?**

**Fran:** Engine + arbor slice (recommended) — expander with full
gates PLUS one real arbor slice (emitter authors fragments+calls for
one shared shape, loader expands, byte-gate holds) before the
milestone closes.

**Q: Bless parsura §4 definition-side design as-is?**

**Fran:** Blessed — refinements ride the spec review.

## Interview closed 2026-08-25. Spec follows in
## project-specs/stml-macros-spec.md.
