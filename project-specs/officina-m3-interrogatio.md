# officina M3 — interrogatio (raw Q&A, 2026-07-11)

*Gradus 3/4 of the M3 on-ramp. Twelve questions over three rounds,
sourced from officina-m3-fundamenta.md §VI as sharpened/dissolved by
officina-simulatio-5.md. Raw record; the spec (officina-m3-spec.md) is
derived from this. Answers are Fran's; recommendations were marked where
offered and Fran's choice is recorded verbatim.*

## Round 1 — format identity + economics

**Q1 (name ceremony; pins the header magic).** indicium / vestigium /
testimonium?
**A: indicium.** The carried placeholder is now SEALED. Pairs with
vindex: the evidence and the champion who acts on it.

**Q2 (baking unit; sim C7 made per-module possible).** Per-world v0
with per-module as named future, or per-module from the start?
**A: Per-world v0** (recommended). One file for the world vindex just
linked; per-module stays a cheap named future because addressing is
already (functio, index) = link-independent.

**Q3 (producers).** Vindex-only bake-on-load, plus a fusor flag now, or
always-on everywhere?
**A: Vindex-only v0** (recommended). Bake + write + reopen inside
vindex; fusor -indicium = named pull.

**Q4 (type depth for v0 inspect).** Type string + medulla tag / raw
words / full C89 type nodes?
**A: Type string + medulla tag** (recommended). Declared C type as
display string + medulla type tag (s32/i32/f32/f64/ptr) driving raw-word
formatting; both harvested at demission; type graph deferred.

## Round 2 — vindex identity + ceremony details

**Q5 (vindex home).** instrumenta/principalia vs sibling project?
**A: instrumenta** (recommended). Thin principale + wrapper script,
saltuarius pattern; graduates later only if it outgrows this.

**Q6 (v0 UX surface beyond the guaranteed core; multi-select).**
finish / memory pane / flight-recorder pane / function-name breakpoints?
**A: ALL FOUR.** v0 surface = file:line breakpoints, continue,
step-over, step-into, source pane, locals/register pane, PLUS finish
(step-out), memory pane (hex+ascii at address), flight-recorder pane,
function-name breakpoints (+ run-to-cursor as sugar).

**Q7 (pausa sentinel spelling; sim C11's ceremony flag).**
Machinula-private / flag bit / vocabulary citizen?
**A: Machinula-private** (recommended). MACHINULA_OP_PAUSA =
MEDULLA_OP_NUMERUS+1 defined inside officina_machinula.c; medulla.h and
vocabularium-medullae.md completely untouched — the sentinel is an
implementation detail of machinula's own derived (frozen) structure.

**Q8 (macro-aware stepping UX).** Display-only / step-into-macro action
/ stack hidden?
**A: Display-only v0** (recommended). Position line shows the stack
("probatio_piscina.c:37 · per CREDO_AEQUALIS_I32 <- credo.h:148");
stepping is by ROOT line; the format carries the full stack;
step-into-macro = named future action.

## Round 3 — boundaries and seals

**Q9 (in-memory lineae wiring — distinct from indicium files).**
Always-on everywhere / diagnosis tools only / vindex-only?
**A: Always-on everywhere** (recommended). Cursor + interpretare collect
and feed line maps unconditionally; every halt report gains root-site
file:line; the seam the format depends on is exercised constantly.

**Q10 (M3 seal).** Headless bar + working TUI / headless alone / TUI
demonstrates the bar?
**A: Headless bar + working TUI** (recommended). The headless probatio
is the FORMAL bar (mechanically re-verifiable forever); the TUI must
exist and be daily-drivable (saltuarius v0 standard) but polish doesn't
gate.

**Q11 (freeze seal).** Parity + measure-and-record / parity + ≥1.3x
gate / parity only?
**A: Parity + measure & record** (recommended). Probatio_vim
before/after + sweep wall-time recorded in the worklog; no threshold
gates the seal — the freeze is justified by the addressing structure
alone.

**Q12 (command tongue).** Keys + Latin command line / keys + prompts /
command line only?
**A: Keys + Latin command line** (recommended). Single keys for the hot
loop; ':' command line with Latin verbs (:punctum, :memoria, :functio)
for anything with arguments; help row for discoverability.

## Dissolved before asking (by simulatio-5)

- Macro stack depth cap — variable-length stacked entries need no cap.
- Variable lexical extents in v0 — demissio allocates all locals at
  function entry, so "all named frame registers" is the truth of the
  execution model; extents deferred to any future liveness work.

## Left open deliberately (measure-first, not user decisions)

- machinula_gradus() call overhead in the -O0 daily build (sim Q12):
  measured during chunk ④; fallback shapes named in the spec.
- Freeze cost per cursor fork-child (sim C4): measured at chunk ①'s
  seal; lazy per-functio freeze = named fallback.
