# DOCUMENTATIO — de scientia bibliothecarum et instrumentorum

*Born 2026-10-02 in a conversation in `../rhubarb-tertia`. Question that
prompted it (Fran): "one thing i think it would be interesting to
experiment with is per-library documentation and sort of 'issue
tracking'." This file holds the brainstorm while the mental model
settles; it is a working record, not a spec.*

*Discipline note: the PULL rule applies. Nothing here graduates into a
lens, a tool or a format until a reader (Fran or an agent) needs it in
an actual moment of work.*

## I. The reframing (Fran, 2026-10-02)

> "at a certain point we need to just start treating the docs as a
> first class project and have you build stuff then me give you
> feedback and continue to make changes to iterate/improve it just
> like we would anything else in the codebase. i think part of the
> trap has been to treat it as kind of a finite job like 'document
> this library' then ending up with a markdown file and having the
> scope essentially be 'markdown file for each library' vs a more
> dynamic open-ended type thing"

So documentation is a **system under development**, with its own
iterations, feedback and tooling. It is not a backlog of
documents to produce. Its unit of progress is "the system answers one
more kind of question well", not "one more library has a .md".

## II. Where things stand (measured 2026-10-02, tertia at a7a7d6f2)

| Thing | Count | How measured |
|---|---|---|
| libraries (`lib/*.c`) | 192 | `ls lib/*.c \| wc -l` |
| worklogs (`lib/*.worklog.md`) | 100 | `ls` |
| phase-logs (`lib/*.phase-log.md`) | 4 | `ls` |
| teaching docs in `docs/` (vox-bibliothecarum) | 1 (`piscina.md`) | `ls docs` |
| ledger inventories | 7, all task-scoped (migrations, fabrica, test suites) | `tabularii quaerere {genus: inventarium}` |
| ledger regions naming a single library | ~4 (tessera, manus, bignum, RNG) | `tabularii mappa {forma: nomina}` |
| code citations of ledger items | 1 | `tabularii census` |

No inventory has the libraries themselves as rows, and there is no
place to ask "what is wrong with library X".

## III. The sensemaking frame (Pirolli & Card, PARC, 2005)

From the analyst study: foraging loop (external sources → shoebox →
evidence file) feeding the sensemaking loop (schema → hypotheses →
presentation). The costly step was the middle one, evidence to schema.

| Stage | Rhubarb today |
|---|---|
| shoebox | transcripts, scratchpad, the code |
| evidence file | **worklogs**, ledger notes (strong) |
| schema | MEMORY.md index, ledger regions, CLAUDE.md "don't forget" lines (**thin**) |
| hypotheses | ledger decisions, principles |
| presentation | vox-bibliothecarum docs, phase-log RELATIOs (rare) |

"Documentation" has been covering three stages at once: evidence,
schema and presentation. Each has a different author, moment of writing
and reader. Related PARC idea: **information scent** (foraging theory,
1999). The discovery failure ("an agent writes a hash table because
`tabula_dispersa.h` gave off no scent") is a scent problem.

## IV. Mesofacts and the inventory (Fran's bet)

Fran: inventory + lenses are "a finite investment in documenting
mesofact type things that pays compounding interest", with periodic
re-updating as a control-plane function. (Mesofacts: Samuel Arbesman,
*The Half-Life of Facts*, 2012. Facts that change on a middle timescale.)

What the inventory brings that prose cannot:
- **provenance per cell** (actor, time, `manu` vs `derivatum`), so rot
  is visible rather than silent; derived cells re-derive
- **the footer counts unknowns per lens**, so documentation debt becomes
  a number (that is the control-plane part)
- it is a **schema-layer** object, the layer that is thin today

Possible consequence: a "library card" is a row of a `bibliothecae`
inventory read horizontally, plus a ledger query for that library's
open items. That needs no new file format and no second home.

## V. User stories

### V.1 Libraries (provide capabilities to programs)

Fran's questions when looking at a library, with the decision each one
serves and the shape of its answer:

| I want to know… | so that… | Shape |
|---|---|---|
| capabilities | I can decide whether to use it or build something | list (can / can't) |
| missing capabilities | I know what to build next or what to work around | list; these are **open desiderata** attached to the library |
| vision | I can judge whether a change fits where it is going | short prose |
| related libraries | I avoid duplication and find the sibling | graph edges, hand-judged (alternative-to, successor-of, sibling-of), NOT the same as depends-on |
| comparison with existing libraries (cJSON, stb, talloc…) | I understand why it exists and learn the domain | table of axes + precedent |

Observations:
- every answer has a **different shape**, which is why a single
  per-library document kept feeling wrong
- **missing capabilities is where docs and issue tracking meet**: they
  may be one thing
- comparison is the most expensive to write and the most stable once
  written (it is the vox-bibliothecarum "precedent" job)

### V.2 Jobs / scripts (do things)

Evidence from the same session: before running three `*_struere.sh` in
tertia, the one question that mattered was whether they write outside
the tree (`~/.bin`). Memory is full of script traps of this kind
(`portae_debitae.sh` seeding the journal in a worktree, `struere`
overwriting installed binaries, `sanare` installing from secunda).

| I want to know… | so that… |
|---|---|
| what it reads and writes; is it safe **here** (worktree, mid-commit, in parallel) | I don't damage shared state |
| how long it takes | I can decide whether to background it |
| what proves it worked (its oracle) | I don't trust a silent success |
| when to run it; what it replaces | I pick the right tool |

Libraries are mostly about **capability**. Scripts are mostly about
**side effects**.

### V.3 Agents (Claude, from the session)

- agents rarely *pull* docs; useful knowledge gets **pushed** at the
  right moment (on first opening `lib/X.c`; when about to write a new
  utility; at debrief after touching X)
- a doc that restates the API is near-worthless to an agent (the header
  and a caller via nexus are faster and cannot drift)
- what code cannot give: discovery, status (alive / superseded /
  predecessor), why and why-not, traps

## VI. Candidate lenses for a `bibliothecae` inventory (unconfirmed)

| Lens | Kind | Source |
|---|---|---|
| propositum (what it is for, one sentence; this is the scent) | textus | manu |
| status (alive / superseded / predecessor / experiment) | textus | manu |
| superata-a (what replaced it) | textus | manu |
| laquei (known traps) | textus | manu |
| tegitur (covered by a test suite) | ita-non | derivatum |
| worklog / phase-log / doc exists | ita-non | derivatum |

## VII. Open questions

- One "documentation system" for both libraries and scripts, or two?
- Who is the first reader: Fran browsing, or an agent orienting? (Both,
  but which one shapes v1?)
- Where do the hand-judged relations (alternative-to, successor-of)
  live: ledger links between library entities, or an inventory lens?
- Teaching docs (Fran understanding the code) vs orientation (an agent
  finding its way): one artefact or two?
- Push channels: hooks, a mod, legati, or all three?

## VIII. Next experiment (proposed, not agreed)

Answer the five library questions informally for two libraries, one
Fran knows well and one he has never read. Wherever an answer is hard to
write or needs digging, that is where the system should help.

## IX. Reading budget (Fran, 2026-10-02)

> "it's easy to generate text … we just generate a much greater volume
> of text than i will ever read. i think we want to be thorough but
> ideally it's more of a finite and polished/curated thing … a document
> almost like a book where i can just start reading start to finish …
> once you have the sort of naive scope of 'hey lets add some
> information for each function' you are automatically blowing past
> your reading budget"

Measured: `docs/piscina.md` = 3,701 words (~15 min); `lib/piscina.c` =
1,255 words, so the doc is ~3x the code. × 192 libraries ≈ 710k words ≈
47 h of reading. The naive scope cannot be read.

Working position:
- **process open-ended, artifact finite**: editions, not accretion;
  adding means cutting
- **reading budget first** (a hard constraint, like a frame budget)
- **curation = choosing what to leave out**; the header is the
  per-function reference already
- **tiered length**: some libraries get a chapter, some a page, some a
  paragraph inside another chapter
- **volume goes to the schema layer** (inventory, ledger), which the book
  points to rather than contains

Open: a book per library, or **one book of the whole stack**
(libraries as chapters by tier, told as one story, K&P-shaped)?
**Answered (Fran, 2026-10-02): "a book about the whole rhubarb repo
essentially."**

## X. The spine is the vision (Fran, 2026-10-02)

> "the most important thing about the audience is that it needs to be
> built around the overall vision of the project and you want to see
> how pieces support that vision, explain the vision, how certain
> decisions have been based on the vision, where pieces have
> outstanding work or where there are unfinished/unstarted projects
> that will move the vision forward"

So the book is organised by **purpose, not by module**: every piece is
explained by what it is for. Each part answers four questions:
1. what part of the vision is this
2. which pieces serve it (and how)
3. which decisions it forced (and why)
4. what remains: outstanding work, unstarted projects

Raw material that already exists (measured 2026-10-02):
- **11 house-wide principia** at the root of the ledger map
  (`./gesta/frigida.sh -mappa-plena`), from CLAUDE.md: no entropy in
  programs, dependencies as admitted entropy, arcane lore, unit tests +
  composable libraries, documentation after 30 libraries, the human
  understands ALL the code, long horizon, Latin, API first, Gordian
  knots cut by standalone modules, moderate realism + prior art
- **7 visio records** in the ledger
- **9 `*-visio.md`** in project-specs (stml, arbor-stml, brighton,
  text-stack, tabularium, silva-instrumenta, mutatio, officina-acta,
  briar-plan-8)
- the region tree (unstarted regions = "what remains" candidates:
  compiler, debugger, javascript parser, family cloud…)

Honesty rule for a vision-led book: every piece is marked as built,
partial or not started. A book organised by aspiration easily describes
plans as if they were facts.

## XI. First extraction: the review is a FILTER (2026-10-02)

Pilot on one file: `docs/visiones/brighton.html` (73 claims,
1,326 words, from a 2,709-word source). Fran marked 19 `[+]` (one typed `[+j`), none
`[-]` or `[~]`:

> "i don't think it's thumbs down on any it's more about me doing a
> filter on the inventory. so other ones are not wrong but i think we
> can filter it down to the list i added + to. and in general i think
> filtering tasks like this can be a good use of my judgement in
> certain cases"

Lessons:
- the placet / non placet / iuxta modum model was wrong for this job;
  the real verb is **select** (keep for the distillate vs leave in the
  inventory), not judge
- division of labour: Claude extracts broadly and faithfully (cheap,
  thorough); Fran's judgement is the **filter** (scarce, decisive).
  This is the curation step from IX, done by the person whose vision it is
- two layers per source: the full extraction (inventory, kept) and the
  selection (the page body), full extraction folded below it
- 19/73 ≈ 1 in 4 kept. If that holds, the vision corpus distils roughly
  4:1 at this step

## XII. Vision tiers (2026-10-02)

Fran: vision at the wrong granularity is a real failure; tiers "should
not be nominal, each tier should have real shared traits". Prior art
(from recall, unverified): Aristotle's ordering of ends (NE I.1-2),
Rasmussen's abstraction hierarchy (up = why, down = how), Jaques' time
span of discretion, Brand's pace layering, Cockburn's goal levels (the
coffee-break test). Business vision/mission/strategy is the nominal
counter-example.

**Adopted to start (Fran): the first four tiers**

| Tier | Test | Horizon |
|---|---|---|
| Telos | answers "why?" for everything; never revised, only understood better | lifetimes |
| Principle | holds across ALL projects; revised rarely, only by Fran | decades |
| Project vision | one thing that should exist; has a finished shape | years |
| Load-bearing decision | within a project; expensive to reverse | months-years |

(Dropped for now: Furniture, cheap to change.) Idea not yet adopted:
store the traits as lenses and DERIVE the tier, so no claim is filed by
how grand it sounds. Observation: the Brighton selection mixed tiers
(B1 near telos, B4 load-bearing, B9.2 near furniture).

**Principle split (Fran, same day)** into two tiers, now on the
taxonomy page:
- **Principium** = load-bearing observation. Fran: "a distilled
  observation (that's where the generalization part is important) … also
  a kind of evaluation that this particular observation is load bearing
  or on the critical path … a generalization and an assignment of an
  evaluation". Tests: names several independent instances (distilled);
  names what rests on it (load-bearing). True/false by evidence.
- **Regula** = commitment/standard: kept or broken, enforceable by a
  tool, justified by a principium, revised only by Fran.
Order: telos > principium > regula > project vision > load-bearing
decision.

**Principium sharpened (Fran, same day):** not "load-bearing" but a
**prime condition**: "other things are a function of it". It "doesn't
include the solution. the intervention lives separately … principium just
says that it is one of the sort of 'elite set' of things that you want to
consider". Tested mainly by INTERVENTION, not new evidence: "you create
it and realize that it doesn't fix your relationship to files it just
sort of reveals a deeper problem". So principia are superseded by deeper
ones (keep the lineage), and the tiers gain a side: condition (telos,
principium) vs intervention (regula, project vision, decision).
Precedents: Rittel & Webber (wicked problems), Schön (back-talk). The 11 ledger "principia" are mostly fused (observation +
commitment in one sentence).

**Occasio added (Fran, same day)**, prompted by text-stack X2.1/X2.2.
Condition side, distinct from principium by: situational (a window),
points at a specific intervention, and "if ignored, a gain is missed, not
a failure". Enabling conditions first tagged principium (X3, X4, X9,
X14) and tabularium T4, T5.3, T5.5, T14 retagged occasio.

**Indicium added (Fran, same day)**: "clue … more like a trend or
something that is suggestive" (text-stack X7, X7.1; also X8, tabularium
T5.4). Suggestive, directional, dated; raw material that can be distilled
into a principium. Prior art: Ginzburg's evidential paradigm, Peirce's
abduction.

## XIII. Division of labour (2026-10-02)

Fran: "lean on what we are both good at … avoid situations where neither
of us are able to do things that we are good at." Lives as a page,
`docs/officia.html`: Fran curates it, Claude keeps it updated. Seed:
Fran: filtering a list; enumerating things in the same mental category.
Claude: building inventory and lenses; classifying items into a defined
taxonomy.

## XIV. The right depth for a library: one paragraph, not the API (2026-10-03)

After all 197 headers were described (22 batches over two days), Fran:
"so far i think this is a helpful level of documentation whereas
documenting the whole api of each library would probably have been
overkill".

What the level is: one paragraph per library saying what it is FOR,
what is unusual about it, the incident or decision behind it, and who
uses it; plus groups with an introduction each; plus design patterns
with known uses. What it is not: a function-by-function reference.

Why that division holds:
- The API already has a home, and a good one: the headers, which in
  this house carry the "why it exists" paragraphs, and the tools that
  read them on demand (legati caput / corpus / symbolum, silva/nexus.sh).
  A second copy in the book would drift from the first the day it was
  written.
- The paragraph answers what the header cannot: where the library sits
  among 197, which generation it belongs to, what replaced or extends
  it, and which pattern it is an instance of. That is the reader's real
  question (sections VIII and IX: the reader needs the vision, not the
  functions).
- It fits the reading budget (section VII): 197 paragraphs is an
  afternoon; 197 API references is never.
- It is cheap to keep true: a paragraph changes when a library's
  purpose changes, not when a function is added.

Corollary for later editions: deepen by exception, as json.html was
(capabilities, peers, audit, timeline), only where a library is
load-bearing enough that its own page earns its keep.

## AUDIENDA (not verified)

- The Pirolli & Card citation (title, 2005, intelligence-community
  funding), the 1999 foraging paper and the 1993 "Cost Structure of
  Sensemaking" are from recall, not checked against the papers.
- Arbesman's book title and year are from recall.
- The "~4 regions naming a library" count is a read of the region tree,
  not a query.
