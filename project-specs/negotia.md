# negotia — projects as places, and a vicus view of them

Status: DRAFT 2026-11-04, brainstormed with Fran (secunda session
134c8417); awaiting Fran's review. Latin names are proposals until
the header text of each phase is approved. Builds on comites
(`comites.md`), routes (`semitae-spec.md`), data categories
(`genera-datorum.md`) and colloquia (`colloquia-plan.md`).

*The question (Fran, 2026-11-04): the library is a garden with
hundreds of threads - utf8, the hash table, M5 - each worked for a day
or two and then left for weeks or months. Fran engages mostly through
conversations, often five at once, and has no view of what happened
in each without combing transcripts. How do we make a thread easy to
pick back up, and the whole garden visible?*

## I. Intent

1. **Resume a thread months later without reconstructing it.** Where
   it stands, why, what is next, and what was learned the hard way.
2. **See what is happening across parallel sessions** without reading
   them: a project map that grows, and an activity log of meaningful
   events.
3. **Notes on anything you look at** (Cmd+T companions), and later:
   ask a follow-up question about a point and have the answer saved
   as a note there.
4. **A more polished, mouse-first widget design language** in vicus -
   calendarium's level or better - starting with these views.

Non-goals (v1): fixed fields per record type; automatic event
extraction from transcripts; a replacement for the ledger, phase-logs
or worklogs; multi-user anything.

Anti-goals: a ticket tracker (records must say what KIND of thing they
are); a second copy of facts that already have a home (the ledger's
rule "one thing, one home" holds); a view that only the keyboard can
drive.

## II. Doctrine

1. **Bones before fields** (Fran 2026-11-04). Entity types and links
   are fixed early; bodies are free text in v1. Fields grow later from
   what people actually write.
2. **Two layers.** STANDING things say what is true now and are
   revised, never closed (vision, features, complications). OPEN items
   have a lifecycle (deferred work, questions). MOMENTS are append-only
   (the activity log). The map shows the first, the log the third.
3. **Stable identity and revision history for everything** - through
   volumen (Fran 2026-11-04). A departure points at the exact revision
   of the vision item it changed; "what did we believe in March" has
   an answer.
4. **A project involves sessions; it does not own them.** This very
   session belongs to both M5 and negotia.
5. **Complication = house term** (Smaragda, `smaragda-v2.md:12`): a
   discovered property of the material that could not be anticipated
   before starting and must be taken into account. The simulations
   record them as discovered-while / consists-in / consequences /
   handled-by (`officina-simulatio-7.md:21`); v1 takes free text, the
   four parts are a later field set.

## III. What exists today (cited)

- **Companion notebook per view location**, Cmd+T (comites P1-P5b);
  per Claude session `terminale/claude/<sessio>`
  (`lib/vicus_applicatio.c:2053`, comites P7). In secunda, not main.
- **Right-pane stack per tab** (`acervus`): `vicus_acervo_aperire`,
  `vicus_acervo_redire` (`include/vicus.h:661-672`); Esc pops.
- **Global cycle Cmd+] / Cmd+[** home -> notifications -> sessions ->
  agenda: `vicus_cyclum_ponere`, `vicus_cyclo_gradi`
  (`include/vicus.h:644-653`, colloquia T4f).
- **Per-view history Ctrl+O / Ctrl+I** (semitae R3).
- **Cmd+H and Cmd+L are free**: fenestra's own menu holds only Quit
  and Edit items (`lib/fenestra_macos.m:690-721`), no Hide; every
  key-down reaches the app (memory: fenestra Cmd keys).
- **Sessions -> tmux panes**: praeco's event log carries session id
  and pane (`include/praeconium.h:42`); colloquia derives per-session
  status (`include/colloquia.h`), and a session in a tmux pane maps to
  its colloquium (`include/colloquia.h:108`).
- **Layout**: `dispositio` (Clay model, in cells; `include/dispositio.h`),
  used by scriba and pictor composers.
- **Ids**: `moneta_ulid` (ledger and code annotations;
  `genera-datorum.md` §VIII-c recommends it for all entities).
- **Volumen**: append-only acts (`volumen_actum_appendere`,
  `volumen_acta_legere`, `include/volumen.h:154,402`) and records
  (`volumen_plagulam_condere`, `:181`).
- **The ledger** (tabularium) already holds parks, questions,
  decisions (incl. `natura:principium`), wishes, plan tasks and
  regions (`mappa`), with links and a "since last read" cursor.

## IV. The bones

```
PLACES          project (negotium)   module   region
  | lies-in (many: cross-module work)
STANDING        vision   feature (planned|partial|shipped)   complication
OPEN            deferred   question   (ledger items, by reference)
MOMENTS         event: committed, decided, paused, gate red, merged...
NOTES           free text, attached to anything
SOURCES         Fran | session (id) | commit | worktree
```

Links (small fixed vocabulary): `lies-in` (item -> place), `forced`
(complication -> decision), `refines` / `departs-from` (decision ->
vision revision), `defers` (deferred -> feature), `about` (note ->
anything), `produced-by` (anything -> source), `involves` (project ->
session), `supersedes`.

**Refines vs departs** (Fran's frame, Newman's distinction): a
refinement keeps the intent and sharpens it; a departure changes the
intent. A project whose departures pile up is telling you something.

## V. The vicus view

- **Project list** - the entry point; mouse-first (click a row to
  open). Opening a project sets the right pane's ring.
- **Project ring: Cmd+H / Cmd+L** move left/right through the open
  project's views. Initial ring (fixed per project in v1, not hand
  arranged): overview -> conversation(s) -> standing (vision, features,
  complications) -> activity log. Cmd+H/L is NEW and separate from:
  Cmd+]/[ (the global cycle, unchanged - Fran 2026-11-04), Esc (pop
  stack), Ctrl+O/I (history within a view).
- **Conversation view** = a terminal attached to the session's tmux
  pane (as the sessions view already does). Its notes are the
  per-session companion, so they follow the session across projects.
- **Every project view has a location**, so Cmd+T notes land on
  "negotia / complications" etc. for free.
- **Design language**: one shared kit before the views - clickable
  rows with hover, toolbar, cards, status chips, page arrows - on
  `dispositio`, mouse path for every key action, keyboard still
  complete.

## VI. Dogfooding target (Fran 2026-11-04)

negotia is the first project. Seeded with:
- this session (`134c8417-...`), also involved in M5;
- the decisions of the brainstorm: Cmd+H/L ring; Cmd+]/[ stays global;
  typed entities with free-text bodies; complications as standing
  facts; refines/departs links; identity + history via volumen;
- its open questions (§IX) as questions.

## VII. Phases (PROPOSED)

First slice (Fran 2026-11-04): project list -> per-project view whose
right-hand ring is this conversation + a feature list that drills
down into a feature's details. Dogfood it on negotia, expand from
there.

| | |
|---|---|
| **N1** | `negotia` library over the vicus volume: projects, features (status + free-text body, revisions as acts), `involves` sessions; ULIDs. CLI so sessions can write (`negotia addere`, `involvere`, ...). Step one: prove a CLI write from another worktree is seen by a running vicus (§X). Tests. |
| **N2** | Seed negotia itself (§VI) through the CLI: the project, this session, the features of this spec. |
| **N3** | Project list pane (app genus `negotia`, organum.stml, routes), mouse-first. The shared widget-kit pieces (§V) are born here as the list needs them, then reused. |
| **N4** | Per-project ring: host support + Cmd+H/L; ring = conversation (by session id; ended -> card + resume) <-> feature list. |
| **N5** | Feature details: click/Enter on a feature -> its details view (body, status, revisions); Esc back. Every view has a location, so Cmd+T notes work throughout. |
| later | Vision and complications as standing views; refines/departs links; activity log (§IX-4); ask-a-question-on-a-point -> answer saved as a note; four-part complications. |

## VIII. Decisions (Fran, 2026-11-04)

1. Name: **negotia**.
2. UI first; bodies are free text in v1.
3. Cmd+H / Cmd+L = project ring (overriding default macOS meaning is
   fine; it is free in vicus anyway).
4. Cmd+] / Cmd+[ stays the universal cycle of screens.
5. Cmd+T companion notebooks are the note-taking mechanism.
6. Stable identity and revision history through volumen.
7. negotia dogfoods itself; this conversation is attached to it.
8. Records live in the **vicus volume** for now (a dedicated negotia
   volume also acceptable), central and shared by every worktree.
9. First slice = project list, per-project ring of conversation +
   feature list, feature details drill-down (§VII).

## VIII-a. Builder's calls (Claude, after the builder check; Fran may overrule)

1. **The vicus volume Fran runs is already central**: the live vicus
   is `bin/vicus -volumen ~/.rhubarb/paginae-proba.volumen` (ps,
   2026-11-04); the default `vicus.volumen` is cwd-relative
   (`lib/vicus_applicatio.c:4260`). negotia uses the volume vicus is
   given; the CLI takes the same path (`-volumen`, default the same
   file). Same volume = companion notes and records sit together. N1
   step one verifies an outside writer while vicus runs (§X); if
   that creaks, fall back to a dedicated `~/.rhubarb/negotia.volumen`.
2. **A revision = one saved edit** of a standing item = one act; its
   id is the act identity (`volumen_actum_identitas`). `departs-from`
   stores that id. No separate "deliberate revision" notion in v1.
3. **Feature status is the one structured field** in v1 (planned |
   partial | shipped), like an open item's lifecycle: it is part of
   the type's bones, not a body field. Everything else is free text.
4. **`involves` is written explicitly in v1**: the session runs
   `negotia involvere <project>` (CLI) when it starts work on a
   project. Automatic linking (by worktree, praeco hook) is later.
5. **An ended session's conversation view** shows the session card
   (title, dates, worktree, last status line) and its companion
   notes, plus a "resume" action that opens a terminal running
   `claude --resume <id>`. Transcript display and summaries are
   later.

## IX. Open questions for Fran

Q1 settled by decision VIII-8 (vicus volume; ledger items linked by
id later, never copied). Q2-Q3 do not block the first slice: v1
projects are plain records with a ULID (resumption links can be added
without changing identity), and v1 records are written when Fran asks
or by the session working on the project at natural moments (task
done, decision made) - revisit after dogfooding.

1. **(settled) Ledger vs negotia volume.** Parks, questions, decisions already
   live in the ledger. Proposal: negotia's volume holds places,
   standing items and notes; ledger items are linked by id, never
   copied; the ledger's regions seed modules. Or should negotia's
   records live in the ledger itself (then N1 extends tabularii and
   decision VIII-6 changes)?
2. **Thread vs project identity.** Is a project a bounded arc that
   closes (M5), a long-lived thread that goes dormant and resumes
   (utf8), or both kinds? If utf8 is picked up months later for a new
   reason: the same project resumed, or a new project linked to the
   old one / to the utf8 module? (Also decides what the project list
   shows: arcs, modules, or both.)
3. **Who writes records unasked.** Do agents file vision items,
   complications and decisions on their own during work (like the
   ledger's scribe contract), or only when Fran asks? Shapes the CLI,
   the noise level and N2's seeding.
4. **Activity log sources** (blocks N6 only). Derived (commits,
   merges, gate results, ledger acts, praeco status) + written by
   agents (decided, paused, found) - and at what granularity?

## X. AUDIENDA

- ~~Whether a running vicus notices records written by another
  process~~ ANSWERED 11-04 (survey for the plan): it does not -
  `insula_versio` is an in-process counter, `data_version` is unused,
  and no open document catches up (`lib/conspectus.worklog.md:140`).
  The only cross-process signal is `volumen_seq_ultimum`
  (`include/volumen.h:421`); negotia polls it each pulse (plan T1,
  T3).
- Whether paginae-proba.volumen is replicated by communio (then
  negotia records would sync too - probably fine, not checked).
- How a volume record's earlier revisions are read back (acts are
  append-only; a per-record revision reader was not checked).
- Whether comites P1-P9 and semitae are merged to main by the time
  N3 starts (they were secunda-only in memory notes; not re-checked).
- The ledger's link verbs (`gerere.verbum`) were not compared with
  §IV's vocabulary.
- Cmd+H freedom was checked in fenestra's menu only, not against any
  global macOS shortcut setting on Fran's machine.
