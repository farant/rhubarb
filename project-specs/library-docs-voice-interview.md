# Library Docs Voice — Interview

Raw record of the conversation that produced this spec. Started as "let's make a FAQ
for piscina" and turned into a full redesign of the `/library-docs` voice, because the
first two tone experiments taught us more by being wrong than by being right.

## Starting point

Read `include/piscina.h` and `lib/piscina.c` together first (the arena allocator —
`Piscina` opaque handle, `Alveus` chained buckets, mark/reset via `PiscinaNotatio`,
grow-past-double branch for oversized single allocations). Fran's idea: build a FAQ
section for it. That expanded into redoing the whole `/library-docs` command.

Discovered mid-conversation that `.claude/commands/library-docs.md` **is** the original
raw prompt verbatim — it's a slash command that replays that exact text. Replacing that
file is part of this spec's deliverable.

## Original format constraints (from the existing `/library-docs` command)

These are inputs, not decisions made in this interview — carried forward unchanged:

- ~50/50 ratio of code to prose. Explicit warning against being too code-heavy.
- Allman brace style in every snippet.
- Audience: experienced programmer generally, genuine beginner at C89 specifically,
  intermediate systems-programming background.
- Teach Latin vocabulary inline as it comes up (Fran writes the codebase in Latin).
- Reference frame: Kernighan & Pike's *The Practice of Programming*, filtered through
  everything else.
- Not chaptered — one big markdown file — but sized like 2-3 book chapters, not a
  cheat sheet.
- Structure: teaching prose → API reference (function by function) → educational FAQ.
- Location: `docs/<library-name>.md`.

## Tone experiment 1: the streamer bit (rejected)

Original ask: talk like someone from Chicago, blustery, Larry-David-esque, a streamer
about to crash out because the superchats keep asking questions other people have
already asked a million times. No apologies, tsundere, confident, doesn't care much
about the reader, just trying to get through the queue.

Fran's own reaction on seeing a sample: *"i think the voice is kind of funny but i'm
not sure it's actually the best at this point."* Asked for comps.

Note: `docs/piscina.md` (the existing doc, pre-dating this interview) is already
written in a close cousin of this voice — exhausted-explainer, "I've told you this a
thousand times" energy. That's part of what's getting replaced.

## Tone experiment 2: Casey Muratori register (partially rejected)

Proposed confident/direct/no-hedging delivery in the style of Casey Muratori (already
one of the project's stated influences per root `CLAUDE.md`), reasoning through
principles rather than being annoyed at the reader.

Fran's response was the key turn in the whole interview:

> "so here's the funny thing, i agree with the muratori tone and like 90% of that, but
> there are particular issues where i am kind of diametrically opposed so i want to be
> careful that the perspective i don't agree with (because now the economics of
> developing software are different with ai, pre-ai i think the casey perspective wins
> on practicality) doesn't leak too much into the projects own material because i think
> that could be misleading. the perspective i have is kind of 'maximalist' auteur which
> i sometimes describe as: we are writing epic, maximalist auteur software because now
> we can with ai. there is no charisma, communication or budget tax for bringing teams
> along for the ride when the team is ai. ... i am anti-yagni"

So: keep the delivery mechanics (confidence, no hedging, teaching-through-reasoning),
reject the substance (YAGNI, "you probably don't need this function," premature-
generalization-is-evil). Concretely flagged risk: piscina already ships mark/reset,
two creation modes, alignment variants, and a full usage-introspection API — a
Muratori-voiced doc would keep wanting to tell the reader they don't need most of that,
which is exactly the wrong sentence for this project.

Proposed two comps to replace the substance while keeping the delivery: Alan Kay
(combative about *insufficient* ambition rather than excessive scope) and a
cathedral-builder / guild-master register (patient, centuries-long, unapologetic about
investment nobody needs yet).

## Round 1 — AskUserQuestion

**Q: Should the maximalist-auteur philosophy show up as implicit scope, or should the
docs sometimes break the fourth wall and argue for it explicitly (e.g. FAQ: "why does
this have so many functions")?**
A: **Both.** Mostly implicit, but real explicit call-outs where it's genuinely the
answer to a question a reader would ask.

**Q: How should the epic/ambitious framing interact with the actual teaching passages,
given Fran is a genuine C89 beginner despite being an experienced programmer overall?**
A: **Depends on section.** Teaching/tutorial prose stays patient and beginner-
respecting; API reference and FAQ sections can be denser and more unapologetic.

**Q: Register choice — Kay-combative-visionary vs. cathedral-builder-serene vs.
blended? (previews given, same arena-allocation example for comparability)**
A: **None selected.** Free-text instead:

> "i would say to some degree it can actually be more approachable / non confrontational.
> like not everything we are doing is necessarily a dramatic statement or even that new,
> lots of stuff is common in some c circles already. i would lean more to emphasizing
> historical precedents if anything"

This rejected both proposed options — neither "we're more ambitious than you" (Kay) nor
"we're laying foundations for the ages" (cathedral) fit. The correction: ground claims
in real precedent, avoid framing anything as a dramatic first-mover statement.

**Q: How much comedic wit should survive from the streamer bit vs. a more earnest
register?**
A: *"real wit is good but i think it can be pretty dry, like thomas costain kind of?"*
(Thomas Costain — popular narrative historian, dry wit, unhurried, doesn't need to sell
the reader because the material carries itself.)

## Round 2 — AskUserQuestion (register recalibration)

**Q: Three calibrations of a precedent-first, Costain-ish voice — terse precedent-list,
narrative-historian, or light-touch-history? (previews given)**
A: **Narrative-historian**, selected preview:

> "Every so often a technique falls out of fashion not because it stopped working, but
> because the generation that needed it retired. Arena allocation is one of those. It
> shows up in Apache's request pools, in Lisp's obstacks, in id Software's per-frame
> allocators — not as some shared inspiration, but because anyone who spends long
> enough fighting malloc's bookkeeping arrives at roughly the same idea independently.
> We didn't invent this. We just declined to forget it."

**Q: Should the "AI removes the coordination tax" reasoning ever appear explicitly in
the docs, or stay unstated background?**
A: Wants a **more complete exploration**, not a light touch:

> "i think we can touch on the AI coordination stuff but we should probably have a more
> complete exploration of the philosophy because i think some of the details are
> important. i think it would mostly come out in faq type questions of 'why is this
> part more complicated' 'why do you have this extra feature' basically 'why didn't you
> go the yagni route' but it can also touch back to the larger vision for the library
> that is not necessarily in place yet but that it's working towards. i guess there can
> also be faq type questions of 'why didn't you go the more maximalist way here' where
> the answer might be 'it was an oversight' or 'these are things we genuinely feel like
> we can leave out because they are not load bearing in some way'"

This is the load-bearing insight of the whole interview: the FAQ philosophy cuts
**both directions**, honestly. Sometimes the honest answer to "why isn't this bigger"
is "oversight," sometimes it's "genuinely not load-bearing." It is not one-note
maximalist propaganda.

**Q: Should historical/etymological framing (C-technique lineage + Latin word
origins) be woven throughout or concentrated in specific spots?**
A: **Depends on section** — matches the beginner-mix answer pattern (tight/
instructional in teaching prose, concentrated in reference/FAQ commentary).

**Q: Should the settled voice become a durable standalone style-guide file for future
doc-writing sessions?**
A: *"we can actually probably replace the .claude/commands/library-docs.md skill but
documenting it as a voice separately makes sense too"* — both artifacts, not either/or.

## Round 3 — AskUserQuestion

**Q: Should the "larger vision not yet in place" idea get a dedicated section, or
FAQ-only?**
A: **FAQ-only.** No dedicated roadmap/future-direction section.

**Q: Should every library's FAQ force at least one "why so complex" / "why not more
maximalist" pairing, or only when there's a genuinely true answer?**
A: **Organic only.** Never manufacture the tension. Skip it if there's no honest story
for that particular library.

**Q: Should historical precedent claims (Apache pools, Lisp obstacks, id Software,
etc.) be verified via actual research before publishing, given the whole voice depends
on grounded-not-invented lineage?**
A: **Yes, verify before publishing.** Explicitly not "write from confident-sounding
recall."

**Q: Where should the standalone voice-guide file live?**
A: `docs/` folder confirmed, plus an unprompted scope discovery:

> "docs/ but there are already markdown files that are doing the old bit so we might
> move them to docs/old or something and then have the new docs be oin docs."

This surfaced a real migration question that wasn't part of the original ask.

## Round 4 — AskUserQuestion (scope closure)

**Q: Migrate the ~24 existing `docs/*.md` files to `docs/old/` all at once now, or
lazily as each one is actually replaced?**
A: **Move all now.**

**Q: Does this spec cover rewriting every existing doc in the new voice, or just the
voice/process plus `piscina.md` as the first applied example?**
A: **Voice/process + piscina only.** The rest is an acknowledged backlog, not tackled
now.

## Summary of what's settled

- Voice: narrative-historian, Costain-flavored, dry wit, non-confrontational,
  precedent-grounded rather than declaring novelty or vision.
- Register varies by section: patient in teaching prose, denser/more unapologetic in
  API reference and FAQ.
- Maximalist-auteur philosophy surfaces mostly implicitly (via scope/thoroughness),
  with genuine explicit FAQ call-outs — organic only, never manufactured, and honest in
  both directions (why something is bigger than expected, and why something *isn't*
  bigger than expected).
- "Why isn't this more maximalist" answers are allowed to be as plain as "oversight" —
  the philosophy does not require every gap to be defended as intentional.
- The AI-coordination-tax reasoning can appear explicitly, mainly in FAQ form, and can
  gesture at unbuilt future vision — FAQ-only, no dedicated roadmap section.
- Historical/etymological claims must be verified via real research before publishing,
  not written from plausible-sounding recall.
- Deliverables: replace `.claude/commands/library-docs.md`; new standalone voice guide
  in `docs/`; batch-move existing `docs/*.md` files to `docs/old/` now; produce
  `docs/piscina.md` as the first proof-of-concept application. Rewriting the other
  ~23 existing docs is explicitly out of scope for this spec.
