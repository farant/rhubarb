# officina — phase log

Append-only. INTENTIO opens a phase, RELATIO closes a chunk/milestone;
full-log audit at milestone boundaries. Chronology is the re-entry reader's
map — never insert, only append.

## NATIVITAS (2026-07-09)

Officina's books are born (interview Q14: milestone narrative lives here;
silva's phase-log gets pointer entries when silva code changes). Prior
history lives silva-side and in project-specs: M0 semantica CLOSED
2026-07-08 (M0a+M0b, silva/phase-log.md) — demissio's input contract
{typus naturalis, typus conversus} FROZEN, haruspex 4,801/0, auspex
3,753/0, coverage 100.00%. Design arc for M1: officina-simulatio-3.md
(pretend-build, complications C1-C9) → officina-m1-interview.md (15
DECISUS) → officina-m1-spec.md (the plan) → vocabularium-medullae.md
(the SEALED vocabulary).

## M1a — INTENTIO (2026-07-09): silva exports + medulla core

**Bar: byte-identical textual round-trip on golden fixtures
(dump(parse(t)) == t); suite green; officina amalgam VERIFICATUM.**

Sealed decisions this milestone leans on (spec §I): modulus-per-TU
(conexio at M2; internal-linkage naming pre-sealed: `$<file-stem>.<name>` /
`$<functio>.<name>`); vocabulary per vocabularium-medullae.md (house
suffixes .s/.i/.f, sigils %/$/@, et/vel/aut/complementum, named conversion
family, sistere, arca); dump defines canonical form, strict parser.

**Chunk A — silva export package** (silva-side; hospes call per row; silva
phase-log pointer entry on landing):
1. Nexus symbolorum: semantica records use-site→symbol during analysis
   (typationes parallel-table pattern); public row proposal
   `silva_c89_symbolum_nodi(sem, nodus)`, canonical-aware.
2. `silva_c89_constans_aestimare(sem, nodus, &valor)` — expose the
   existing aestimator.
3. `silva_c89_chorda_decodere(piscina, nodus)` — escape-decoded string
   bytes (wraps _fugam_decodere; do NOT reimplement).
4. Declare `silva_oraculum_responsa_vacare` in silva.h (one line; it
   already exists with external linkage).
Amalgamare after; suite + hospes green.

**Chunk B — medulla structs + builder**: modulus/functio/blocci/
instructiones with TAGGED operands (registrum | immediatum | symbolum |
datum-ref; per-functio operand pool for vocare arity — sim C2); origo on
every instruction (in-memory always — sim C3); arca frame-slot model;
MedullaDatum {imago, relocationes} (sim C8); builder enforces terminator
discipline. Unit suite (probatio_officina_medulla.c).

**Chunk C — textual form + amalgam birth**: dump + strict parser
(vocabularium §VII); hand-written canonical .medulla fixtures in
probationes/fixa/; round-trip harness = THE BAR; amalgama/officina.c +
hand-written officina.h born (shared silva_amalgama mechanism), hospes +
VERIFICATUM gates.

Then: M1a RELATIO + audit → M1b INTENTIO (demissio; bar: corpus lowers,
0 ruinae, sistere classified, fusor green, every dump round-trips).

Known risks carried from simulatio-3: C2 operand shape is the most-copied
struct (get it right in B before C pins goldens); C4 binding-table is the
largest export (semantica change, not a header line); goldens pin AFTER
the vocabulary — never before (it is already sealed, so safe).

### M1a CHUNK A — RELATIO (2026-07-09): exporta demissionis SHIPPED

All four export-package items landed silva-side (detail:
silva/fontes/silva_c89_semantica.worklog.md 2026-07-09):

1. **Nexus symbolorum** — `silva_c89_symbolum_nodi` + `nexus_numerus`.
   Bindings recorded at BOTH resolution sites (identifier expression +
   vocatio callee — implicit synthesized callees bind too:
   `symbolum_nodi(callee)->est_implicitum` works). Canonical-aware and
   canonical-RELATIVE like typatio.
2. **`silva_c89_constans_aestimare`** — existed in fontes with the exact
   proposed signature; the gap was the silva.h row only (cheaper than
   the sim priced).
3. **`silva_c89_chorda_decodere`** — decoded bytes into a caller piscina;
   fragments concatenated, embedded NUL safe, NO trailing NUL
   (acies.numerus = octeti+1 carries it; demissio zero-fills from the
   type). FINDING: the L-string park is PRE-PARSER (`L"ab"` does not
   parse) — decodere's L guard is defensive armor, not a live path.
4. **`silva_oraculum_responsa_vacare`** — declared in silva.h (existed
   in silva_commissio.h all along).

Gates: semantica suite 308/308 (+22 asserts: nexus identity through the
index, implicit-callee binding, decode with \x/\0/fragments,
invalid-escape FALSUM); silva 29/29; amalgam VERIFICATUM (hospes 28/28,
all five rows called with content asserts); percursus corpus sweep
UNCHANGED on every baseline (0 errores, coverage 100.00%, 399
diagnostica / 31 plagulis, 305 indecisa) — nexus recording is invisible
to every existing gate.

**NEXT: Chunk B — medulla structs + builder** (officina/fontes/
officina_medulla.{h,c}: modulus/functio/blocci/instructiones, tagged
operands + per-functio operand pool (sim C2 — the most-copied struct,
settle before goldens pin), origo on every instruction, arca slots,
MedullaDatum {imago, relocationes}, terminator discipline enforced by
the builder; probatio_officina_medulla.c; officina
compile_probationes.sh born).
