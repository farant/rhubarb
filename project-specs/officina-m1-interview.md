# Officina M1 — interview raw Q&A (2026-07-09)

Post-simulatio-3 decision interview. Every answer below is Fran's DECISUS.
Context: project-specs/officina-simulatio-3.md §VI listed the open questions;
this interview settled all of them plus the ceremony core.

## Round 1 — structure

**Q1. Module model (sim C1): what is a MedullaModulus; when is linking designed?**
A: **Modulus-per-TU, conexio at M2.** M1 lowers each TU independently; a
separate conexio (link/merge) step is designed and built at M2 when machinula
needs to load programs. BUT the internal-linkage naming scheme is DECISUS'd
now so the textual form never churns.

**Q2. Where does the silva export package live in the books?**
A: **M1a Chunk A, silva-side.** First chunk of the officina milestone happens
to be silva work — precedent: pre-M1 (genus export). One INTENTIO covers it;
hospes rules apply.

**Q3. Policy for the ~31 POSIX-facing files that can't fully lower (sim C9)?**
A: **Trap op + honest bar.** Unlowerable expressions emit a trap instruction
carrying diagnostic + source position; aborts only IF EXECUTED. Bar reads
"0 ruinae, N traps, all classified." The trap channel is the future
enforcement layer's runtime seat — checking stays out of demissio.

**Q4. Milestone shape?**
A: **Split M1a / M1b** (mirrors M0a/M0b). M1a = silva exports + medulla core;
bar = byte-identical round-trip on golden fixtures. M1b = demissio; bar =
whole monorepo lowers, 0 ruinae, traps classified.

## Round 2 — ceremony core

**Q5. Type suffixes on ops?**
A: **House convention: .s32/.i32/.f64** (i=insignatus/unsigned,
s=signatus/signed, f=fluitans — inherited from the monorepo's own typedefs).
Accepted preview: `%q = dividere.s32 %a, %b` / `%r = dividere.i32 %a, %b`.

**Q6. Textual sigils?**
A: **Full sigils: % register, $ symbol (functio/datum), @ block label;
immediates bare.** QBE-lineage discipline; strict parser trivial. Accepted
preview: the $summa loop function (see vocabularium-medullae.md exemplum).

**Q7. Bitwise op names?**
A: **et / vel / aut / complementum** — the classical-logic set (vel =
inclusive or, aut = EXCLUSIVE or), plus sinistrorsum/dextrorsum for shifts
(.s right shift = arithmetic, .i = logical).

**Q8. Conversion ops: named family or generic convertere?**
A: **Named family**: amputare (truncate), extendere (widen; sign- vs
zero-extension determined by SOURCE type signedness), fluitare (int→float),
integrare (float→int), dilatare (f32→f64), artare (f64→f32). Two-suffix
form: `extendere.s32.s64`.

## Round 3 — remaining names and homes

**Q9. Trap op name?**
A: **sistere** ("to halt") — matches the infinitive op style; no collision
with 'ruinae' (percursus's crash vocabulary).

**Q10. Internal-linkage naming scheme?**
A: **Source-derived prefix.** File statics: `$<file-stem>.<name>` (e.g.
`$chorda_c.adiutor`); function-scope statics: `$<functio>.<name>` (e.g.
`$legere_proximum.cursor`). Deterministic from source alone — goldens never
churn; readable in dumps and in vindex later.

**Q11. Goldens home + extension?**
A: **officina/probationes/fixa/ pairs, .medulla extension.** Hand-written
.medulla fixtures AND .c fixtures with committed .medulla golden dumps
beside them (nomen.c + nomen.medulla). Silva roundtrip-corpus precedent.

**Q12. Corpus-sweep instrument name (percursus's officina analog)?**
A: **fusor** ("the smelter/caster") — the workshop instrument that pours
everything through the melt.

## Round 4 — bookkeeping + process

**Q13. Where does the sealed op vocabulary live?**
A: **Own reference doc**: officina/vocabularium-medullae.md — mirrors the
genera-c89.md precedent; implementation and tooling cite it.

**Q14. How do the two projects' books relate for M1?**
A: **Officina books born now; silva gets pointer entries.** officina/
phase-log.md + CLAUDE.md created at M1a INTENTIO; milestone narrative lives
there; silva's phase-log gets a short cross-reference entry when Chunk A
lands.

**Q15. Process from here?**
A: **Q&A file + spec, skip v2, then INTENTIO.** Simulatio-3's two agents
already did the codebase grounding a v2 would do. Then M1a INTENTIO into
officina's new phase-log.

## Standing note carried through (from /simulate-work invocation)

Type CHECKING is the future enforcement layer — do not implement now;
demissio assumes well-typed input; representation choices must not foreclose
the checker (the sistere channel is its named future runtime seat).
