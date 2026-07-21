# Library Docs Voice — Spec v2

Builds on `library-docs-voice-spec.md` with codebase research (agent-driven, see
findings below). Voice, structural format, and the maximalist-FAQ pattern are
unchanged from v1 — this pass only firms up the deliverables and surfaces one real
decision point that v1 didn't know to ask about.

## What research confirmed

**Docs inventory (corrected).** v1 said "24 files" from a partial listing. Actual
count is **25 top-level `.md` files, plus `docs/latina/` — a directory containing one
file, `docs/latina/piscina_latina.md`** (26 items total). That file turns out to be
the *entire existing `piscina.md` translated into Latin* — same streamer-bit voice,
same content, different language. It belongs in the same migration as `piscina.md`
itself; there's no reason to treat it differently.

Corrected file list:

```
actor.md  argumenta.md  c89.md  chorda.md  chorda_aedificator.md  credo.md
delineare.md  fenestra.md  filum.md  latina.h.md  macho.md  nuntium.md
piscina.md  reactor.md  sectio.md  selectio.md  stml.md  symbola.md
tabula_dispersa.md  tcp_posix.md  tempus.md  uuid.md  via.md  xar.md  xml.md

latina/piscina_latina.md
```

Worth a passing note, not a blocker: `nuntium.md`, `selectio.md`, `stml.md`, and
`xml.md` currently have owner-only (`600`) permissions while the rest are world-
readable (`644`). A plain `git mv` won't change that; flagging in case it matters for
anything downstream.

**Cross-references: clean.** Whole-repo grep found no README, script, or source file
anywhere that links to a specific `docs/*.md` path. `MAP.txt:55` mentions the `docs/`
folder generically (unaffected by reorganizing its contents). Everything else that
matched was self-referential (this spec and its interview doc) or false positives from
an unrelated sibling repo (`../bun-libraries`, referenced from old `gesta-k4-*` specs).
**Moving everything to `docs/old/` breaks nothing.**

**Command file conventions: match what's already there.** All 9 files in
`.claude/commands/` are plain prose, no YAML frontmatter, kebab-case filename matching
the slash command. No template to conform to beyond that — the v1 draft replacement
for `library-docs.md` already matches this (plain prose, no scaffolding added).

**No existing precedent for a `docs/old/`-style directory — this needs a decision.**
This is the one finding that changes something. The project already has a real,
established pattern for "this is superseded, kept for reference" — it's just not
relocation. `arbor2` and `lapifex` (the frozen legacy parsers) stay exactly where they
were built, in `include/`, `lib/`, `probationes/`, with their status communicated
through:

- An inline label in `MAP.txt:129` (`LAPIFEX (C89 parser gen) ──── frozen / reference`)
  and a note at `MAP.txt:133` that the whole family is superseded.
- A formal tabularium decree — `decretum-codex-congelatus-arbor2-lapifex-praepara-*`,
  titled *"Codex congelatus (arbor2/lapifex/praeparator) manet ut est"* ("frozen code
  stays as is"), duplicated across four tag-index locations.

No directory named `old/`, `legacy/`, `frozen/`, or `deprecated/` exists anywhere in
the repo. The interview's "move all now" answer was given without this precedent on
the table, so it's worth actually deciding rather than defaulting to it — see options
below.

## Decision needed: relocate to `docs/old/`, or freeze in place with a label/decree?

**Option A — proceed as decided (`docs/old/`).** Simple, matches the mental model of
"docs/ only ever contains current material." Introduces a new organizational pattern
the rest of the repo doesn't use.

**Option B — leave the 26 files in place, mark them with the existing pattern
instead.** Add a short note at the top of each old doc (or a single index) plus a
tabularium decree the way `arbor2`/`lapifex` got one, and `docs/` stays a mix of
old-voice and new-voice material until each file is individually replaced. Consistent
with how the project already handles this exact situation elsewhere.

**Option C — hybrid.** Move them to `docs/old/` (the interview's actual answer, and
arguably cleaner specifically for *docs* — unlike frozen source code, nobody needs to
find these by their working path, they're just superseded prose) but still log a
tabularium decree recording the decision, so the "why" survives the same way it does
for arbor2/lapifex. This keeps the clean directory model while not silently breaking
from the project's habit of recording superseding decisions formally.

No default chosen here — flagging for Fran to pick before implementation touches the
filesystem.

## Migration mechanics

Once the option above is picked:

- Single `git mv` per file (not a script) — 26 individual moves, or `git mv docs/*.md
  docs/old/` plus one more for the `latina/` subdirectory. Small enough that a script
  adds no value over doing it directly.
- One commit, separate from any content changes (i.e., not bundled with writing
  `docs/piscina.md`), so the history shows "reorganize old docs" as its own step.

## Everything else from v1 stands unchanged

Voice (narrative-historian, Costain-flavored, section-dependent register), the
maximalist-FAQ pattern (organic only, honest in both directions, FAQ-only for future
vision, no dedicated roadmap section), the historical-verification requirement, the
structural format (50/50, Allman, K&P reference, one file sized like 2-3 chapters), the
`.claude/commands/library-docs.md` replacement text, and the `docs/vox-bibliothecarum.md`
outline are all carried forward from `library-docs-voice-spec.md` without change.

## Ready to implement, pending the one decision above

1. Pick A/B/C for the old-docs question.
2. Execute the migration (or the freeze-in-place labeling, if B).
3. Write `.claude/commands/library-docs.md` replacement.
4. Write `docs/vox-bibliothecarum.md` in full (v1's outline still needs to become
   actual prose).
5. Write `docs/piscina.md` — first applied proof, including real verified historical
   research for whatever precedent it ends up citing.
