# Vox Bibliothecarum

The voice guide for this project's library documentation. Read this in full before
writing or editing any `docs/<library-name>.md` file. Don't work from a memory of what
this document used to say — it gets revised, and a stale recollection from a previous
session is not a substitute for reading it again.

## What this replaced, and why the history matters

Two earlier attempts got rejected, and the reasons are worth keeping on record so they
don't quietly get reinvented by a future session that hasn't seen this document's
history.

**The streamer bit** — Chicago-blustery, Larry-David-esque, a streamer crashing out
because the superchats keep asking questions other people have already asked a
thousand times. It's genuinely funny in short bursts, and it's the voice the old
`docs/*.md` files (now in `docs/old/`) were written in. It doesn't survive a real
teaching document, because the irritation is aimed at the reader, and the reader here
is a genuine beginner at C89 who needs to trust the material, not brace for it.

**Casey Muratori's delivery, kept — his substance, dropped.** Confident, direct,
teaches through reasoning instead of hedging: all of that is right, and it's why this
voice still doesn't apologize or both-sides things. But Muratori's actual engineering
worldview is minimalist and YAGNI-flavored — "you probably don't need this function,"
premature generality is where bugs live. That's a defensible position when a human
team pays a real coordination cost for every extra feature. It is not this project's
position. This project is explicitly building the comprehensive version of things
because an AI-assisted team doesn't pay a charisma, communication, or budget tax to
carry scope that would sink a human team. A doc written in Muratori's actual voice
would keep telling the reader they don't need half of what's in front of them, and for
a library like `piscina` — which already ships two creation modes, aligned and
unaligned allocation, mark/reset, and a full usage-introspection API — that's simply
the wrong thing to say.

**What survived from both:** the confidence and the refusal to hedge. What got
replaced: who that confidence is pointed at.

## The voice: narrative-historian

Dry wit in the mode of Thomas Costain — a popular historian who tells a technique's
lineage as a story worth hearing on its own terms, unhurried, and doesn't need to sell
the reader because the material is interesting on its own. Not combative (a library
isn't a rebuke to people who built things a lesser way). Not solemn (a library isn't a
cathedral cornerstone either). Not irritated at the reader. The default stance toward
almost everything this project builds is: *this has real precedent, we're not claiming
otherwise, and the precedent is worth knowing.*

Calibration anchor — this passage is the confirmed reference point for the whole
voice:

> Every so often a technique falls out of fashion not because it stopped working, but
> because the generation that needed it retired. Arena allocation is one of those. It
> shows up in Apache's request pools, in Lisp's obstacks, in id Software's per-frame
> allocators — not as some shared inspiration, but because anyone who spends long
> enough fighting malloc's bookkeeping arrives at roughly the same idea independently.
> We didn't invent this. We just declined to forget it.

## Register changes by section — it is not one register throughout

**Teaching prose** (the main body explaining how to use the library): patient,
beginner-respecting. This is where it matters most that the reader is a genuine C89
beginner, whatever else they know.

**API reference and FAQ**: denser, more unapologetic, assumes more of the reader.

Same fact, in both registers, to make the contrast concrete:

*Teaching prose:*

> One thing worth sitting with before you allocate your first real structure: the pool
> doesn't clear memory for you. When `piscina_allocare()` hands you a pointer, whatever
> bytes were sitting in that spot before are still there — old data from a previous use
> of that bucket, or just whatever happened to be lying around. If your code quietly
> assumes fresh memory starts at zero, and a lot of C code does assume that without
> quite realizing it, you'll get bugs that only show up sometimes, depending on what
> used to occupy that address. If you need zeroed memory, ask for it explicitly with
> `memset()`.

*API reference / FAQ:*

> `piscina_allocare()` does not zero-initialize. Callers relying on zeroed memory must
> `memset()` explicitly — true for every allocation function in the family, aligned or
> not, fatal or try-variant.

Historical and etymological asides follow the same split: tight and infrequent in
teaching prose (don't let a digression eat the 50/50 code-to-prose budget), more
concentrated in reference/FAQ commentary, where a lineage aside is often the actual
point of the answer.

## Ground everything in verified precedent

Named precedents — Apache's pools, Lisp's obstacks, id Software's frame allocators,
whatever comes up for a given library — must be checked against real sources before
they're written down. Not recalled because they sound plausible. This is not a
generic nicety; it's specific to this voice. A comedic voice can invent a reference and
lose nothing — the joke doesn't depend on the reference being true. This voice's entire
premise is "grounded in real history, not a dramatic claim," and the first invented or
wrong citation is the moment that premise stops being true. Verify, every time, before
publishing.

## The maximalist-philosophy FAQ pattern

This is the part most likely to get flattened into a slogan if it's ever summarized
carelessly, so the nuance is spelled out in full here rather than compressed.

The reasoning behind why this project builds comprehensively — no coordination tax
when the team is AI — is allowed to appear explicitly in a doc. It belongs in FAQ
answers only. Never a dedicated manifesto section, never woven into teaching prose.

Two recurring shapes to reach for, **only when genuinely true for the specific
library in front of you**:

1. *"Why is this more complicated than it needs to be"* / *"why do you have this extra
   feature"* — the "why didn't you go the YAGNI route" question, answered honestly.
2. Its mirror — *"why didn't you go further here"* / *"why doesn't X exist yet"* — and
   the honest answer is allowed to be as plain as **"it was an oversight."** That is a
   legitimate, complete answer. It does not need to be dressed up as a principled
   decision if it wasn't one. The other legitimate answer is substantive: "this
   genuinely isn't load-bearing enough yet to justify building." Both are fine. What's
   not fine is pretending every gap is deliberate when some of them just haven't come
   up yet.

A mirror-question answer can gesture at a larger vision for the library that isn't
built yet — but only inside an FAQ answer actually asking about it. Never a dedicated
roadmap or future-direction section.

**Never manufacture this tension.** If a library doesn't have a real "why so complex"
or "why not more" story, that FAQ pairing is simply absent from its doc. Organic only,
every time — a library with nothing honest to say here should say nothing.

Worked examples, illustrative of the shape rather than locked-in final copy for any
specific doc:

> **Q: Why does allocation come in four variants —`piscina_allocare`,
> `piscina_allocare_ordinatum`, `piscina_conari_allocare`,
> `piscina_conari_allocare_ordinatum` — instead of one function with a couple of
> optional parameters?**
>
> Because those four cover four genuinely different situations, and threading two
> independent choices — "die on failure or hand me NULL," "any alignment or a specific
> one" — through a single call site means every caller pays for options most of them
> never touch. A parser reading a config file under 8KB wants the fatal, unaligned
> version and nothing else. Code allocating SSE vectors wants the aligned version and
> genuinely can't use the unaligned one. Four small functions that each do one obvious
> thing costs less at every call site than one flexible function would.

> **Q: Why can't you ask a pool how many buckets (*alvei*) it currently has?**
>
> Internally, the pool already walks its bucket list for `piscina_summa_usus()` and
> `piscina_summa_inutilis_allocatus()` — a bucket count would be the same loop with a
> counter instead of a sum. It's missing because nobody's needed it yet, not because of
> a considered decision to leave it out. If you want it, that's a legitimate reason to
> add it.

## Structural format (unchanged across every library doc)

- ~50/50 ratio of code to prose. The default failure mode is too much code — resist it.
- Allman brace style in every snippet.
- Audience: experienced programmer, genuine C89 beginner, intermediate systems
  background.
- Teach Latin vocabulary inline as it appears — the codebase is written in Latin.
- Structural/economy reference point: Kernighan & Pike's *The Practice of Programming*,
  filtered through everything above.
- One markdown file per library, not chaptered, but sized like 2-3 book chapters —
  a real read, not a cheat sheet.
- Shape: teaching prose → API reference (every function) → educational FAQ.
- Location: `docs/<library-name>.md`.
