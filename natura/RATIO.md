# RATIO — why this exists, what it is for, how we work on it

**Companion documents.** `INDEX.md` says what EXISTS (generated).
`METAMODULUS.md` says what the FORMAT is and how to write one.
`natura.worklog.md` says HOW WE GOT HERE, chronologically. This
file says WHY — and how the work is done. It exists because those
answers had drifted into whichever document was durable, and a
specification is the wrong home for a rationale.

---

## 1. What this is

A hand-crafted library of **kinds of things** — their properties,
states, relations, parts, and the events that happen to them —
written in STML, in Latin, and grown by discussion rather than
generation.

"OOP done right": the taxonomy layer was object orientation's
real appeal, and the implementation layer was the part that
disappointed. This takes the first and leaves the second.

It is **not** a wire format (schema.org), **not** a compliance
target (FHIR), and **not** an upper ontology awaiting adoption.
It is the distinctions themselves, with the reasons attached.

---

## 2. Why moderate realism is load-bearing

Not decoration, and not merely the house's confession. It is what
makes the method work, and the alternatives fail differently.

**Universals are real, and real *in the things* (in re).**
Consequences:

- **You can go and look.** If the universal is in the things,
  the things are evidence — so when an abstraction question
  stalls, you collect specimens and the specimens can settle it.
  This is the **Linnaean escape hatch**, and it is available to
  no one else. Under nominalism more specimens are just more
  particulars (the grouping is a convention, so collection yields
  data to impose a scheme on, never a verdict). Under Platonic
  realism the form is elsewhere and specimens are shadows, so one
  reasons a priori — which produces taxonomies that match
  nothing.
- **You can decline an abstraction without denying the
  resemblance.** Nominalism is *good at finding* cross-domain
  patterns — it looks for regularity in usage rather than essence
  — but has no way to say "genuinely similar, not the same kind."
  So every strong similarity becomes a candidate abstraction and
  the gears spin. Moderate realism can affirm the likeness, refuse
  the genus, write the individual case down, and **move on**. The
  methodological advantage is *permission to stop*.
- **Instituted kinds are real kinds.** ISBN, an office, a
  currency, a licence are conventional and *genuinely there* —
  real membership conditions, real properties, real histories.
  Realism does not restrict us to natural kinds, which is what
  lets the library descend from "identifier" to *ISBN*, where the
  payload actually lives.

**Pieper is the guard.** Describe things as they are, not as
applications want them. A use-case schema is flattery and breaks
at use case N+1. The subtle version: that failure can relocate
from the property layer to the genus layer, where it hides better
— "does this buy us anything" is a use-case test wearing a
taxonomy costume.

---

## 3. What it is for

### Two halves, different mechanisms

**Per naturam material saves a consumer from getting STRUCTURE
wrong.** It is a distillation of other people's expensive
surprises: ISBN identifies a manifestation, not a work;
recurrence is a rule, not a set (expand it into rows and you
truncate the future); price belongs to a time-varying relation,
not to the item; a status is a fold of a history (store only the
status and you cannot answer "when did it change"); a diagnosis
is a judgment, not a fact (model it as fact and you will
"correct" it and destroy the record); negotiability is clean
title to a good-faith purchaser, not bearer-possession.

The argument is the **cost curve**: none of these bite in week
one. They bite in month eight, after there is data, which is
exactly when schema errors stop being cheap.

**Per iudicium material gives a consumer LEVERAGE THEY LACKED.**
A genus is not a label — it carries a state machine, properties,
relations, actions. Applying one imports an **apparatus**. Judge
"this is technical debt" and you inherit a state machine (is it
serviced or repaid?), a property (what is the *interest*?), and a
relation (`emit` — what capability did it *buy*?). **Naming hands
you the next questions**, which is what expertise looks like from
outside: not more facts, but knowing what to ask next.

`./tools/natura_quaere.sh -a <genus>` makes that mechanical.

### Four modes of use

1. **Read it** as a design reference before writing your schema.
   Works today, zero machinery — the value is distinctions, and
   prose carries distinctions.
2. **Generate from it** — tables, structs, forms. Needs the
   loader.
3. **Cite it at runtime** — app volumina reference library
   genera, and two apps citing the same genus are talking about
   the same thing. This is the interop story.
4. **Use it as an instrument of perception** — apply terms to an
   unstructured environment to *see* structure. Researcher,
   founder, CTO, anyone reading a legacy codebase. Its sharpest
   affordance is **detecting what is MISSING**, which unstructured
   observation cannot do, because absence has no perceptual
   signature. A genus with declared parts makes absence visible.
   (This is our own `umbrae` mechanism turned outward.)

### What it does NOT do

It does not tell you what your application needs. The residence
rule holds: the library describes kinds; your individuals live in
your volumina and cite it. It only stops you getting the KINDS
wrong.

### Why this differs from what failed before

Frames (Minsky), scripts (Schank), the expert systems of the
1980s, universal upper ontologies. The diagnosis is available
rather than mysterious: **their frames were task-shaped; ours are
Pieper-shaped.** A "restaurant visit" frame does not compose with
a "hospital admission" frame, because each encodes a use case.
Frames built for what things ARE compose, because reality does.

Measured, not asserted: `periculum` was written for a clinic and
exercised unchanged by freight insurance one round later;
`quantitas` and `recurrentia`, built from a bakery, met correctly
in "500 mg twice daily."

And their real handicap was not philosophical: **they had no
specimen access.** Knowledge engineering by interview is slow and
expensive, so economising on expert contact and spending effort on
formalism was the *rational* response to that cost structure. It
produced the failures we now name. The cost structure has since
inverted, which is what makes this method practical rather than
merely correct.

---

## 4. Cross-pollination

**Each domain elaborates one thing exceptionally well**, because
that thing is unusually visible there. The same pattern recurs
elsewhere poorly solved, and people reimplement e.g. "contract"
from first principles, painfully. A universal taxonomy makes the
best-elaborated version available everywhere.

Already measurable — Roman law's *obligatio* now serves software
licensing and SLAs; scholastic degrees of assent serve diagnoses,
estimates and our own dictionary dates; FRBR *bent our versio
pattern into something better*; Pacioli gave the volumen doctrine
a five-century ancestor; medicine's latent/chronic/relapse is the
same shape as regression; canon law's vacant office fixes org
software.

Four later cases are stronger, because in each the shared kind
was **found before the genus was written**, and the genus exists
only because of it:

- **`interactio`** — drug interaction and hazmat segregation are
  the same kind (co-presence changing effect); the genus was
  built from both at once, so its `inter` relation is open and
  the hazmat specimen relates *classes*, not goods.
- **`classis_normativa`** — controlled-substance schedules and
  the aviation accident/incident gradation. The aviation half
  had sat unbuilt for two rounds waiting for a second domain,
  and pharmacy supplied it unprompted.
- **`acta`** — five implementations in one house (gesta,
  tabularium, volumen, `ratio_rationaria`, and our own STML
  `<historia>`) and the general kind unnamed under all of them.
- **`attestatio`** — and here the prediction ran forward:
  `acta`, built for event logs, turned out to describe the civil
  registry's correction-by-marginal-annotation practice, which
  predates Pacioli by centuries.

It has also run **backwards**: merchant law's negotiability
revealed that a bearer *token* LACKS clean-title doctrine — a
commercial-law distinction producing a security insight.

It has also run **backwards**: merchant law's negotiability
revealed that a bearer *token* LACKS clean-title doctrine — a
commercial-law distinction producing a security insight.

**The heuristic that makes it searchable: a domain elaborates
what it is PUNISHED for getting wrong.** Law elaborated obligation
because failure means injustice; medicine elaborated diagnosis
because failure kills; accounting elaborated the immutable ledger
under adversarial pressure. So the move is not "browse domains
hopefully" — it is *"who is punished for getting X wrong?"* and go
there. This is the escape hatch **aimed** rather than broadcast,
and it paid on its first use (aviation).

---

## 5. How we work

There are **two instruments**, and they ask different questions.

### The coverage loop (Fran's instrument) — what is MISSING?

Pick a foreign scenario → walk its stations → **cite or surface**
at each → harvest candidate genera *with their forcing content*
→ build after blessing.

Eight runs, and the pattern they made:

| run | scenario | what it found |
|---|---|---|
| 1 | bakery | missing NOUNS — whole domains |
| 2 | clinic | missing EPISTEMIC dimension (how sure, from whom) |
| 3 | freight forwarder | missing TEMPORAL dimension (true when) |
| 4 | parish | `officium` — refined an established ruling |
| 5 | PM / devops | a structural ABSENCE: `programma`, which `bibliotheca` had been defining itself against |
| 6 | aviation | richest yet; chosen by the punishment heuristic |
| 7 | pharmacy | `iussum` — the persisting DIRECTIVE, which `actus` and `obligatio` had been standing around without |
| 8 | civil registry | `attestatio` — *a projection you hand out stops being a projection* |
| 9 | PKI | run as a TEST of a one-round-old genus: found 2 real defects **and** 2 predictions |
| 10 | distributed systems | `petitio`, `quorum`, `depravatio` — and a deferred candidate redeemed by its own stated trigger |
| 11 | diplomacy | ratification IS two-phase commit; apparent authority IS the stale credential |
| 12 | apiculture | `stigmergia` — the third mode of coordination; and three domains REFUSED for the echo trap |
| 13 | sheep | `peritia_communis` — knowledge borne by a collective, killed by discontinuity not forgetting |
| 14 | museum | `fiducia`, `interventio` — and *distinguishable* as an anti-forgery ethic inside repair |
| 15 | metrology | `incertitudo`; and Le Grand K forced `radix_fiduciae.refundata` |

**Three modes have emerged, and they ask different questions.**
Runs 1–8 were pure gap-finding. Run 9 introduced a second:
**aim a fresh domain at a one-round-old genus deliberately** —
the cheapest moment a genus is ever testable, and it found two
defects and two predictions at once. Runs 11–12 introduced a
third: **aim a domain at a whole cluster** to see whether the
genera describe the *problem* or merely the *technology*.

Two disciplines were learned by being burned, and both now pay
every run:

- **Collision-check names BEFORE walking.** Run 11 caught
  `reservatio` already being a genus (a booking, in `tempus`)
  before a word was written.
- **Write the trigger when you refuse.** Run 7 refused `petitio`
  naming the condition that would flip it; run 10 supplied it.
  A refusal with its condition recorded is a standing order, not
  a dead end.

And one trap named before it was walked into (run 12): **do not
walk a domain whose vocabulary was imported FROM computing.**
"Ant colony as distributed system" would have returned our own
voice as discovery, undetectable from inside. The test is whether
*practitioners* developed the vocabulary or whether CS applied
its own to them. Apiculture passed; ecosystems and mycorrhizal
networks were refused outright, since that is exactly where the
scribe is most fluent and least checkable.

Run 5 is the one that shows why the loop cannot be replaced by
introspection: **nobody re-reads their own definitio asking
whether the thing they contrasted with exists.**

Two later refinements, both learned by being burned:

- **Collision-check candidate names BEFORE walking.** Checking
  afterwards means arguing with a name you have grown attached
  to. Run 7 burned three names in advance; run 8's check
  *pre-decided two stations* — `titulus` is defined as permanent,
  so a birth certificate could never be one.
- **A harvest table is a plan, not a gate.** Run 7 recorded its
  non-surfacings meticulously and lost a real surfacing between
  the walk and the file. Close each loop by confirming every
  candidate was built or explicitly refused.

### The modelling pass — does the inventory COMPOSE?

Take one subject and describe it with what exists. The loop is
additive by nature; this asks whether genera defended one at a
time actually work together. Its failure modes are different in
kind: not *"no genus for X"* but **awkward fit** — which is how
we learned that a VCS commit sits in a *sequence*, not in time,
a gap `momentum` could not carry.

**Pick a subject the human can verify.** That is the only real
answer to the scribe's limits in §6 — everywhere else the prose
is equally fluent whether or not it is right.

### Rules of the craft

- **Take DIMENSIONS before nouns.** Divisions are cheap and local
  (rename, re-parent, split — the gate names what broke);
  metamodel changes are expensive and global. Settle structure
  while the corpus is small enough to retrofit.
- **Gate at birth.** Every new mechanism gets its check the same
  hour. The gate has caught its own author at least six times —
  including the hour a mechanism was born (`certitudo="fide"`
  where the species is `fides`).
- **Measure before building.** The loader was assumed to be
  corrective; measurement said 324 values, zero violations, so it
  is preventive and drops in priority. Assumptions about rot are
  cheap to test and expensive to act on.
- **Check the things you are NOT thinking about.** The rot was in
  the metadata, not the content: nineteen defects in flags and
  version numbers, zero in the semantic core — because attention
  followed what was being thought about.
- **One instance is an observation; two in unrelated domains is a
  law.** *Ferrum modum imperat* waited for a bakery oven and a
  freight vehicle. Generated concepts need MORE evidence than
  inventoried ones, not less, because they have no external check.
- **Record the non-surfacings.** Merchandise, triage, backlog
  priority and redundancy were each considered and refused, with
  reasons. The refusals later validated the addressability test —
  a criterion that agrees with judgments made on other grounds is
  one you can trust.
- **A concept that generates an instrument is doing work.**
  `complicatio` produced the simulate-work practice; that is
  evidence of realness, not decoration.
- **A genus that can never falsify itself is jargon, not
  vocabulary.** Per-iudicium genera therefore carry
  `asseritur_per → iudicium`: membership is authored, dated, and
  revisable, so reclassification is legitimate rather than an
  admission of failure.
- **Grep on raw text is not a parser.** Measured three times in
  one session, on our own tooling. The parse is never the hard
  part; the SHAPE of what the parser returns is.
- **Park candidates, then WALK them.** Two candidates carried for
  rounds with specimens attached — `actio` and *scheme
  administrator* — each died on contact with a real walk, one
  because checklists confirm STATES not actions, the other
  because a relation already covered it. Reflection kept both
  alive indefinitely; walking killed both in a sentence. A
  candidate that survives only in the abstract has not been
  tested.
- **When a genus of ACTS holds a member whose definitio describes
  an OBJECT, the object wants out.** Found twice from opposite
  sides: `iussum` (a persisting directive hiding behind the act
  of ordering) and `attestatio` (a document filed among acts,
  its own definitio saying "what is written IN IT is presumed
  true"). The tell is grammatical and cheap to check.
- **The gap may be APPARATUS, not a genus** — "thin where the
  domain is thick." `persona` had been revised five times and
  still had no state machine, while the institutions that handle
  persons professionally elaborate civil status above all else.
  Ask of an old genus: what does the domain that specialises in
  this ask about it?

---

## 6. Honest limits

**The scribe's epistemic position.** This library is largely
written by an LLM, and that access is textual and secondhand.
What practitioners know but never write down is systematically
missing. It is strongest where the literature is thick (Roman
law, FRBR, biological nomenclature) and *equally fluent* where it
is thin — so **confidence does not track accuracy**, and the
difference is not visible from the inside.

**Therefore the discipline, not the expertise, catches errors.**
"Compiled" as a property of a language is the popular view and
would have been reproduced; what caught it was the format forcing
"property of WHICH subject?" An expert without the discipline
restates a field's received simplifications with total
confidence.

**Mitigation is honest marking, not verification** — `certitudo`,
`fons`, and `corpus_institutionis` (a source species that is
unverifiable *by nature*, whose honesty consists in calling the
reader to verification rather than to belief).

**And one structural consolation, which is real rather than
comforting: the taxonomy is more robust than the dictionary.** A
wrong date in an `individuum` is local. The genus structure rests
on many specimens agreeing, so individual errors do not propagate
upward. Be generous with structure and careful with dates.
