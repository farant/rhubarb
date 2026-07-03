# Saltuarius Interview — Raw Q&A

Date: 2026-07-03 (immediately after tessera 1.0 closed)
Context: silva M1 closed, tessera 1.0 shipped; saltuarius is the
named next act — the first *application* in the monorepo, the
real-host proof for both amalgams. Five rounds, twenty questions;
every recommendation taken.

---

## Round 1 — Identity

**Q1. When you Enter a .c file in the miller columns, what
happens?** (Ranger's metaphor is "everything is a directory," but a
C file's M1-contents are preprocessor structure, not functions.)

**A: Source view.** Columns navigate the filesystem only. Enter on
a file opens a full source VIEW — that's where the silva magic
lives (layers, origo, regions), with a structure sidebar riding
along inside it. Cleanest split: ranger outside, silva inside.
(Rejected: "keep drilling" — file as directory of preprocessor
structure; "both from day one.")

**Q2. The v0 money shot — the single on-screen moment that makes
saltuarius real?**

**A: The origo chain.** Cursor on any token in expanded source →
key → provenance chain appears: "from macro X (file:line) ←
included via Y ← layer 2." The thing NO other tool shows; silva's
whole thesis on screen. This is the acceptance scenario the build
aims at. (Rejected: layer flip as primary, regions-first, plain
ranger-for-rhubarb.)

**Q3. How much REPL in v0 — is there a command line?** (Original
framing was "kind of like a silva repl or terminal almost.")

**A: Keys only.** v0 is pure modal navigation. A ':' command line
is NAMED-DEFERRED to v0.1 — the pane/view architecture should
leave room for it, but no command parser ships in v0.

**Q4. How does saltuarius know what to parse — include paths,
project root?**

**A: compile_flags.txt.** `saltuarius [radix]` takes a root dir
(default cwd); if radix/compile_flags.txt exists (the clangd
convention — rhubarb already has one), read its -I lines for
include dirs. CLI -I flags override. Zero config for rhubarb,
portable convention elsewhere. (Rejected: CLI-only, own config
file, rhubarb-hardcoded.)

---

## Round 2 — The source view

**Q5. Layer mechanics: whole-buffer flip or per-invocation
unfold?**

**A: Whole-buffer flip.** A key cycles the entire view layer 0 →
1 → 2 → ... The roundtrip guarantee made visible: the file IS
different text at each layer. Per-invocation unfold (the fold
metaphor) is a named v0.1 candidate.

**Q6. The text changes length across a layer flip — what does the
cursor track?**

**A: Token identity.** The cursor follows the TOKEN through
expansion — silva's extents/origo make this possible (the token
knows its ancestry in both directions). Flip layers and you're
still on the same logical thing. Deliberately chosen ALSO because
it exercises the origo API hard — that's what a real host is for.

**Q7. Is the origo chain display navigable — select a link and
JUMP there?**

**A: Yes, jump.** The origo popup is a menu: arrow to a link,
Enter opens that file at that location. Go-to-definition for the
preprocessor — the moment saltuarius becomes a tool rather than a
demo. (Jump history/back-stack noted as cheap follow-on, not
committed for v0.)

**Q8. Syntax coloring in v0?**

**A: Latin-visible.** Full lexeme-genus coloring (comments,
strings, numbers, directives, keywords) PLUS latina macros
(si/dum/redde...) as their own distinct color class — you SEE the
language layer that makes the codebase itself. Cheap: lookup over
lexeme genus + two word-sets (C89's 32 keywords, latina's define
list).

---

## Round 3 — Engineering shape

**Q9. Testing discipline for an app — where's the line?**

**A: Cell-grid goldens.** Library-shaped core (saltuarius/fontes:
view-models + renderers taking a TesseraOpus), thin main.
Probationes drive scripted key sessions through the memoria pons
and assert on the CELL GRID (tessera_cellulam_legere: "row 3 says
parsare.c") — NOT raw escape bytes. Content-level assertions
survive style tweaks; byte-level goldens stay in tessera's suite
where they belong.

**Q10. Parse strategy and memory lifetime across a session?**

**A: Keep all, per-file piscinae.** Parse on first open, cache
forever, one piscina PER PARSED ROOT FILE. At rhubarb scale
unbounded is honest — and per-file arenas mean LRU eviction is a
v0.1 upgrade, not a redesign. (Rejected: LRU day one, index-
everything-up-front.)

**Q11. The structure sidebar (includes/macros/regions) — how alive
in v0?**

**A: Navigable TOC.** Tab focuses the sidebar, arrow through
includes/macro definitions/conditional regions, Enter jumps the
source cursor there (reusing origo-jump machinery). Makes M1's
structure explorable NOW — and its limits are exactly what
generates the M2 "I want functions in this list" pressure.

**Q12. Status line contents?**

**A: Context + fructus toggle.** Path, cursor line:col, stratum
indicator, region badge when inside a conditional arm, transient
messages; a debug key toggles tessera fructus counters
(cells/bytes/ms) into the line. The render-performance evidence
is one keystroke away — that's how the evidence-gated
scroll-region decision eventually gets made.

---

## Round 4 — Hands and failure

**Q13. Key dialect?**

**A: Vim/ranger dialect.** hjkl AND arrows everywhere; gg/G,
ctrl-d/u, Enter=open/activate, Tab=pane focus, q=back out,
L=layer cycle, o=origo. Arrows keep it guest-friendly.

**Q14. Mouse in v0?**

**A: Wheel + click.** Wheel scrolls the focused pane; click moves
cursor/selection and focuses the pane under it.
Click-to-follow-links stays out of v0.

**Q15. Search — where's the v0 line?**

**A: In-file / + type-jump.** Source view gets / — literal
(non-regex) incremental search with n/N. Miller columns get
type-to-jump on filenames. Cross-file search (quaerere across the
parsed forest) is NAMED-DEFERRED — the killer v0.1 feature, wants
the ':' command line.

**Q16. When silva can't give a file the full treatment (caps /
intermissa / pasta fail-loud / non-C text)?**

**A: Plain text + badge.** The file ALWAYS opens — worst case
uncolored plain text with line numbers. Status line badges the
truth ("silva: intermissa" / "textus merus"); layer/origo keys
politely no-op with a message. Fines degrade, never break — the
browsing never dies because parsing did.

---

## Round 5 — Identity, citizenship, finish line

**Q17. Name?**

**A: Saltuarius confirmed.** Top-level saltuarius/ folder, silva
conventions (fontes/, probationes/, instrumenta/principalia/,
CLAUDE.md, phase-log.md). The Roman estate forest-warden — the
keeper who walks the silva.

**Q18. Dependency citizenship?** (Subtlety: the amalgams' vendored
piscinas only export create/destroy, not allocare — app state
needs an allocator from somewhere.)

**A: Monorepo citizen.** Saltuarius uses rhubarb libs normally for
its own state (piscina, chorda, xar, chorda_aedificator...) AND
consumes silva/tessera STRICTLY via their amalgam headers — never
their fontes/. The amalgam boundary is the API; that's where the
real-host pressure lands. Standalone hostability stays proven by
each project's hospes gate. (Rejected: ascetic amalgams+libc-only
host; citizen-plus-standing-ascetic-demo.)

**Q19. Build order — what works first on screen?**

**A: Walk then talk.** Phase A: miller columns browse rhubarb
(plain previews, NO silva yet) — usable file browser lands
immediately, navigation machinery gets real early. Phase B: source
view + lexeme coloring (silva enters). Phase C: layers + origo +
jump (money shot). Phase D: TOC + search + polish. Each phase ends
with something you'd actually run. Mirrors tessera's
pure-core-then-pons risk ordering.

**Q20. The v0 DONE bar?**

**A: Daily-driver bar.** (1) Browse rhubarb comfortably (columns,
previews, type-jump); (2) the money shot: origo chain on a real
macro in silva_parsare.c, jump followed, layer flip with
token-tracked cursor; (3) TOC + / search work; (4) probationes
green (cell-grid goldens through memoria pons); (5) degradation
badge proven on a non-C file. "I actually use it" is the bar
utility tools deserve.

---

## Decisions taken by fiat during spec-writing (Fran can veto)

- Origo links pointing OUTSIDE the parsed universe (e.g. system
  headers): open as plain text if the file exists on disk, else
  no-op with a status message — consistent with the degradation
  posture.
- Layout is fixed-proportion, recomputed on AMPLITUDO (resize)
  events; no user-resizable panes in v0.
- One piscina per parsed ROOT file (the parse pulls its include
  graph into that same arena); opening an included file directly
  later creates its own parse. Duplicate parsing of shared headers
  across roots is ACCEPTED in v0 (memory is cheap at rhubarb
  scale; dedup is a v0.1 concern with eviction).
- Non-regex '/' search operates on the CURRENTLY DISPLAYED layer's
  text (search what you see).

## Round 6 — v2 decisions (2026-07-03, after agent exploration)

**Q21. Parse cache: accept LRU revision of Q10's keep-all, given
the measured 84–242 MB peak arena per root?**
**A: LRU N=8.** Evict = one piscina_destruere; N tunable.

**Q22. Silva additiones as mini-phases between saltuarius
phases?**
**A: Yes, as spec'd.** Two packages in silva's phase-log
(additiones I: lexare + helpers + telemetry; additiones II:
fons_via + strata retention + vistas), full gates each time.

**Q23. Dimmed omitted arms: explicit grey vs OBSCURUM faint?**
**A: Explicit grey** (~0x707070 uniform — flat color reinforces
inertness; OBSCURUM too terminal-dependent).

**Q24. tessera_replere timing?**
**A: At Phase A close** — build with app-side loops first, let
real usage confirm the signature (eskil rule), then one tessera
1.1 pass.

## The standing meta-note for v2

The entire source-view feature set hinges on what silva.h's public
M1 surface actually exposes per token: origo chains, layer
iteration, extents, line/column mapping. Wherever the amalgam API
falls short, that gap IS the deliverable insight — saltuarius
exists to generate exactly this API pressure. v2 agents must map
the real silva.h surface against every interaction in this spec
before any code is planned.
