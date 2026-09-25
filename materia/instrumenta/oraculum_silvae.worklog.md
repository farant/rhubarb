# oraculum_silvae — worklog

## 2026-09-24 — born (silva-migratio-plan T2)

**What it is.** One source, two binaries. `-DORACULUM_PIGNUS` compiles
against `silva.h` of the amalgam at the pinned commit (`PIGNUS=7a4847b0`
in `materia/oraculum_silvae_struere.sh`) and links ONLY that amalgam's
object; without the define it compiles against live silva's headers and
links `silva/build/*.o` (the shim's exclusion list). Output: one
tab-separated line per file — `octeti circuitus emissio stml errores
semantica`, plus `lectio comparator` under `-legere` (live only).

**The one real difference between the builds** is the vendored substrate:
the amalgam renames it (`SilvaPiscina`, `SilvaChorda`, `silva_piscina_*`);
silva's own functions keep their names. So the alias block covers only
`PiscinaOraculi`, `ChordaOraculi` and two arena calls. Live headers the
tool needs: `silva_contextus.h`, `silva_c89_oraculum.h`,
`silva_scribere.h`, `silva_arbor.h`, `silva_c89_semantica.h`,
`silva_tabulae_c89.h` — from T7 on, these names are part of the facade
this tool proves. (The plan first named `silva_parsare.h`; the real
entry `silva_c89_parsare` lives in `silva_c89_oraculum.h`.)

**Isolation is checked, not assumed.** The build asserts that every
symbol the pinned tool object needs is defined by the amalgam or is on
a fixed libc list. First run it fired on six innocent names the
COMPILER emits (`printf` → `puts`/`putchar`, `__stack_chk_*`,
`__chkstk_darwin`, `__sprintf_chk`) — list extended, second line of
`LIBC` says why. Plant: a call to the unprefixed house
`piscina_vacare` under `ORACULUM_PIGNUS` → "ISOLATIO VIOLATA:
piscina_vacare", exit 1.

**`-legere` compares like with like.** silva's M2 gate only runs the
parsura comparator AFTER bytes diverge (as diagnosis), so whether a
fresh parse and a loaded document compare equal on healthy files was
never measured. The tool therefore loads BOTH documents — the pinned
one and the live one — with the live reader and compares the two loaded
parsurae (FIDELITAS). `lectio` = the pinned document, read by the live
reader and emitted, equals the source. Negative control: one byte of a
pinned document changed → `lectio=vitium:STML parsari non potuit@40`.

**Default context = latina.h lexicon** (`silva_contextus_latinam_addere`),
so the expansion machinery is exercised; `-nudum` = no context, the
shim's mode. Consequence seen on day one: `include/latina.h` hashes no
symbols — with the lexicon loaded, `LATINA_H` is already defined and the
guarded body is an untaken arm. Correct, not a bug.

**Semantics digest is in INDEX order** (registration order = source
order), not sorted as the plan first said — index order is the stronger
check and nothing in the migration should reorder registration. If a
switch ever does, that is a named divergence, not a reason to sort.

**Cost — and the arena bug it exposed.** Corpus = the shim's
(`lib/*.c include/*.h silva/fontes/*.c`, 390 files today). First
measurement: pinned 21 s, live `-legere` **8 min 47 s, 293 s of it
system time**. Cause: `piscina_vacare` RETAINS its blocks; after
`lib/biblia_dr.c` (6 MB source, ~4 GB resident with `-legere`, the
STML document alone 61 MB) every later file paid for the retained
memory — 389 files without biblia took 1 min 47 s. Fix: a fresh arena
per file, destroyed after (comment at `principale`). Now: pinned
`-stml` **10 s**, live `-legere` **29 s**; 390/390 lines identical
between the two builds, 390 `lectio=idem comparator=aequales`. Amalgam
object: compiled once per pin, ~6 s.

**Checks run.** (b) five files (a roundtrip fixture, `lib/chorda.c`,
`include/latina.h`, a syntax error, `lib/piscina.c`): identical lines.
(c) plant in LIVE `silva_scribere_fontem` (emission shortened by one
byte) → all five live lines `circuitus=dispar`, pinned lines unmoved.
A trap on the way: zsh does not word-split `$FILES`, so the first
"identical" diff compared two identical error messages — always count
the lines before trusting a diff.
