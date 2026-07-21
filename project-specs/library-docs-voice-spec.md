# Library Docs Voice — Spec v1

Synthesized from `library-docs-voice-interview.md`. Covers the documentation voice and
process, not a rewrite of every existing doc — `docs/piscina.md` is the proof of
concept, the other ~23 existing docs are an acknowledged backlog.

## Goal

Replace the streamer-bit voice currently baked into `.claude/commands/library-docs.md`
(and already present in `docs/piscina.md`) with a register that:

1. Keeps the confident, no-hedging, teach-through-reasoning delivery the Muratori
   experiment got right.
2. Drops the substance that came bundled with it — YAGNI, "you probably don't need
   this function," premature-generalization-is-evil — because this project is
   deliberately anti-YAGNI (maximalist scope is affordable now that an AI-assisted
   team doesn't pay a charisma/communication/budget tax to bring collaborators along).
3. Doesn't oversell that maximalism as a dramatic first-mover statement. Most of what
   this project builds has real precedent in existing C practice — the honest framing
   is "grounded in a lineage," not "we did something new."

## Voice

**Register: narrative-historian.** Dry wit in the mode of Thomas Costain — a popular
historian who tells technique and lineage as a story worth hearing on its own terms,
unhurried, doesn't need to sell the reader because the material carries itself. Not
combative (rejected: Alan-Kay contempt-for-insufficient-ambition), not solemn
(rejected: cathedral-builder gravity), not irritated at the reader (rejected: original
streamer bit).

Confirmed reference sample (from the interview, arena allocation as worked example):

> Every so often a technique falls out of fashion not because it stopped working, but
> because the generation that needed it retired. Arena allocation is one of those. It
> shows up in Apache's request pools, in Lisp's obstacks, in id Software's per-frame
> allocators — not as some shared inspiration, but because anyone who spends long
> enough fighting malloc's bookkeeping arrives at roughly the same idea independently.
> We didn't invent this. We just declined to forget it.

**Register varies by section, not uniformly:**

- Teaching/tutorial prose (the main body explaining how to use the library): patient,
  beginner-respecting. Fran is a genuine C89 beginner despite being an experienced
  programmer generally — this is where that matters most.
- API reference and FAQ: denser, more unapologetic, assumes more of the reader.

**Historical/etymological framing is section-dependent too:** tight and infrequent in
teaching prose (don't let digressions eat the ~50/50 code-to-prose budget), more
concentrated in reference/FAQ commentary where a lineage aside is often the actual
point of the answer.

**Hard requirement: verify historical claims before publishing them.** Named
precedents (Apache pools, Lisp obstacks, id Software's frame allocators, whatever comes
up for a given library) must be checked against real sources, not written from
plausible-sounding recall. The entire premise of this voice — grounded in real
precedent rather than a dramatic claim — collapses the moment a cited lineage turns out
to be invented or wrong. This means an actual research step (web search or equivalent)
happens before a historical claim goes into a doc, every time.

## The maximalist-philosophy FAQ pattern

This is the load-bearing design decision from the interview, and it must be applied
honestly, not as a slogan:

- The "AI removes the coordination tax, build the comprehensive version" reasoning is
  allowed to appear explicitly in a doc, but it belongs in FAQ answers — never a
  dedicated manifesto section, never woven into the teaching prose.
- Recurring FAQ shapes to reach for **when genuinely true for that library**: "why is
  this part more complicated than it needs to be," "why do you have this extra
  feature" (i.e. "why didn't you go the YAGNI route"), and its mirror, "why didn't you
  go further here" or "why doesn't X exist yet."
- The mirror question's honest answer is allowed to be as plain as "it was an
  oversight," or as substantive as "this genuinely isn't load-bearing enough to
  justify building yet." Both are legitimate answers. The point is honesty about a
  real decision, not proving the library is maximalist everywhere.
- A FAQ answer can gesture at a larger vision for the library that isn't built yet —
  but only inside an FAQ answer that's actually asking about it, never as a dedicated
  roadmap/future-direction section.
- **Never manufacture this tension.** If a library doesn't have a genuine "why so
  complex" or "why not more" story, that FAQ pairing is simply absent from its doc.
  Organic only.

## Structural format (carried over unchanged from the original ask)

- ~50/50 ratio of code to prose — resist the code-heavy default.
- Allman brace style in every code snippet.
- Audience: experienced programmer, C89 beginner, intermediate systems background.
- Teach Latin vocabulary inline as it appears (the codebase is written in Latin).
- Reference frame for structure/economy: Kernighan & Pike's *The Practice of
  Programming*, filtered through everything above.
- One markdown file per library, not chaptered, but sized like 2-3 book chapters.
- Shape: teaching prose → API reference (function by function) → educational FAQ.
- Location: `docs/<library-name>.md`.

## Deliverables

### 1. Replace `.claude/commands/library-docs.md`

Current file is the raw prompt verbatim (see interview doc). Proposed replacement below
— stays operationally tight, defers full rationale to the standalone voice guide so the
command doesn't balloon:

```markdown
I want documentation for a library you'll name in your next message, written to
docs/<library-name>.md. Read the full voice and process rules in
docs/vox-bibliothecarum.md before writing anything — don't work from memory of a past
session, that file is the source of truth and it changes.

Quick summary of what's non-negotiable:

- ~50/50 code-to-prose. The default failure mode is too much code — resist it.
- Allman braces in every snippet.
- I'm an experienced programmer but a genuine beginner at C89 — teach accordingly in
  the tutorial prose. The API reference and FAQ can assume more.
- Teach Latin vocabulary inline as it comes up, since the codebase is written in Latin.
- Structural reference point: Kernighan & Pike's "The Practice of Programming."
- One markdown file, not chaptered, but sized like 2-3 book chapters for the whole
  library.
- Shape: teaching prose -> API reference (every function) -> educational FAQ.
- Voice: narrative-historian, dry wit, precedent-grounded, not a dramatic claim of
  novelty. Full detail in docs/vox-bibliothecarum.md.
- Verify any historical claim (named projects, dates, lineage) before it goes in the
  doc. Don't write a precedent from recall and hope it's right.
- The FAQ can honestly discuss "why is this more complex than it needs to be" or "why
  didn't you build more here" -- but only when there's a real, true answer for this
  specific library. Never manufacture the tension.

Tell me the library and I'll start.
```

### 2. New standalone voice guide: `docs/vox-bibliothecarum.md`

Durable reference for the voice and philosophy, meant to survive context resets on its
own (same role RELAX.md/SABAW play for the project's introspective side, but scoped to
documentation craft). Outline for v2 to flesh out in full:

- Why the streamer bit and the Muratori-substance both got rejected, and why — worth
  keeping so a future instance doesn't reintroduce either by default.
- The narrative-historian register, with the confirmed sample passage as calibration
  anchor.
- Section-dependent register rule (teaching vs. reference/FAQ).
- The maximalist-philosophy FAQ pattern, spelled out with the "oversight is a
  legitimate answer" nuance intact — this is the part most likely to get flattened
  into a slogan if summarized carelessly.
- The historical-verification requirement and why it's non-negotiable for this voice
  specifically (unlike, say, a purely comedic voice, where an invented reference would
  cost nothing).
- Worked example(s) beyond the single arena-allocation passage already validated.

### 3. Migrate existing `docs/*.md` to `docs/old/`

All at once, now, regardless of whether a new-voice replacement exists yet. Working
list from current `docs/` (24 files, to be confirmed exactly in v2 — need to check
whether `docs/latina/` is a directory that should move too, and whether any of these
are referenced by path elsewhere in the repo, e.g. from README.md or other docs):

```
actor.md  argumenta.md  c89.md  chorda.md  chorda_aedificator.md  credo.md
delineare.md  fenestra.md  filum.md  latina.h.md  macho.md  nuntium.md
piscina.md  reactor.md  sectio.md  selectio.md  stml.md  symbola.md
tabula_dispersa.md  tcp_posix.md  tempus.md  uuid.md  via.md  xar.md  xml.md
```

### 4. Write `docs/piscina.md` in the new voice

First applied proof of the whole system. Happens after this spec is reviewed, using
`lib/piscina.c` / `include/piscina.h` (already read this session) as source material,
plus real historical research for whatever precedent the arena-allocation section
actually cites.

## Non-goals

- Rewriting the other ~23 existing docs. Explicit backlog, not this spec.
- A dedicated roadmap/future-direction section anywhere. Vision talk is FAQ-only.
- Forcing every future library doc to contain a "why so complex" FAQ entry. Organic
  only, every time.

## Open questions for v2 (codebase research)

- Exact, verified file list for the `docs/old/` migration (confirm `docs/latina/` is
  or isn't a directory, confirm nothing outside `docs/` links to these paths by
  relative reference).
- Whether `.claude/commands/*.md` has any convention beyond plain prose (frontmatter,
  naming, etc.) that the replacement should match — quick scan of siblings.
- Whether the repo already has an "old/legacy" naming precedent elsewhere (e.g. the
  frozen arbor2/lapifex parsers) worth mirroring for consistency in how `docs/old/` is
  described/introduced.
- Concrete migration mechanics: `git mv` list vs. a small script, and whether the move
  should be its own commit separate from anything else.
