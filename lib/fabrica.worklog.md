# lib/fabrica.worklog.md

## 2026-09-29 — T1: the pure core (plan 1a)

`include/fabrica.h` + `lib/fabrica.c`: input kinds, input-set digest,
aedilis manifest reading, verdicts, dependency order. Disk and processes
only through `FabricaSutura` (the `AedilisExtractor` pattern); the gate
`probationes/probatio_fabrica.c` runs on an in-memory seam (55 checks).

Decisions made while building:
- **Scratch contract.** `currere` must hand the generator an EMPTY
  scratch dir. Without that a mute generator (exit 0, writes nothing)
  would be compared against the previous run's leftover output and
  judged RECENS — Review Focus 4. The fake seam in the gate empties it,
  and the "mute" case runs right after a writing run to prove it.
- **Digest = path + content.** Per input, in path order: `via NUL
  sha256(bytes)`. The gate's first "contents swapped between two paths"
  assert did NOT pin the path: with path bytes planted out, swapping
  still reorders the content hashes, so it stayed green. The rename
  assert (same content, different path) is the one that went red under
  the plant. Lesson for later gates: ask what an assert would still
  accept if the property were gone.
- **Manifest file never digested whole** (`generatum=` timestamp,
  `commissum=` change on every run); its listed files are. `systemata`
  excluded. A manifest carrying `inresolutae` (P1, 982fec44) makes the
  digest FALSUM with the header named — never "current".
- **Directory inputs** digest their sorted name list, keyed `via/` so a
  directory never collides with a file of the same path. A new header
  in an include root must change every closure digest over that root.
- **Blind-link rule held:** `nm -u build/fabrica.o` = house lib symbols +
  libc only; `compile_tools.sh` (links every `build/*.o`) still links.
- Unsigned trap caught by examen: `index < 0` on an `i32` (always
  false) for `chorda_invenire_index`'s `-1` — now an `s32`.
- Lint renames: INSTALLATIO → INSTITUTIO (house says "institutum:
  ~/.bin/stml"), `FabricaGenusProvenientiae` → `FabricaProvenientia`.
  The plan/spec prose still says "installatio" for the kind.

## 2026-09-30 — T2: the declaration dialect

Names as approved by Fran (spec v2 §VI, plan T2 Step 1): root
`fabrica.stml` gains `subsystema via=` children (dialect `fabrica` v2);
each subsystem's `aedificatio.stml` holds `actio titulus genus` →
`mandatum`/`verbum`, `ingressus genus via`, `exitus via provenientia
scriptura`. Readers: `fabrica_declarationes_legere`,
`fabrica_subsystemata_legere`; `sedes` = "via:linea" from
`StmlNodus.linea` (1-based).

- The placeholder I showed Fran put `<verbum!(>…` and `</mandatum>` on
  one line — a raw capture eats to end of line, so the command is one
  argument per line (as `aedilis.stml` writes its flags).
- **Canon vs reader split.** The canon judges shape (kinds as `electio`,
  `minimum="1"` on ingressus/exitus); the reader refuses the same things
  with the line named AND the one rule a canon cannot say: duplicate
  `titulus` in a file (scratch dirs `build/fabrica/scriptura/TITULUS`
  would collide). `decl_titulus_duplex.stml` passes the canon by design.
- Plant: `minimum="1"` dropped from `exitus` → `decl_sine_exitu` passes
  the canon (VITIA 0), reader still refuses; restored → VITIA 1.
- `subsystemata` is a new word → glossary entry `subsystema` (neuter,
  -atis) in oratio/glossarium.stml.
- The root `fabrica.stml` itself is unchanged until T4 has subsystems
  to list; silex reads only its existence (lib/silex.c:277).

## 2026-09-30 — T3: bin/fabrica, bootstrap, fumus

`tools/fabrica.c` (CLI + real seam), `tools/fabrica_struere.sh`
(aedilis → generated struere.sh → rm+cp; 15 objects), root
`fabrica.stml` gains `<subsystema via="."/>` and a root
`aedificatio.stml` declares the judge itself. `bin/fabrica iudicare`
on the live tree: 0.02 s; first line `IGNOTUM bin/fabrica - sine
provenientia` until T7.

- **Only declared binaries are asked `-provenientia`.** A binary that
  ignores unknown flags could act (bin/manus sends keys). T7 must only
  declare RELATIO for binaries whose installers write provenance.
- **Exit contract:** NON IUDICATUM (celer) does not fail the run — it
  is a mode chosen, not a defect; STALUM/IGNOTUM → 1; nothing judged or
  broken declarations → 2 (a broken declaration is never skipped).
- **Titles unique ACROSS subsystems** too (scratch dirs are per title):
  checked in the tool, since the lib reader sees one file at a time.
- **Orphans: 8, not the 2 spec v2 §0.5 named** — I had only grepped for
  the two deleted files I knew (toml, arbor_quaestio); six lapifex/
  eventus_inspector manifests also point at deleted test roots.
- **Code came before the gate** (plan order was test first). Made up
  for by proving stages red with plants: A (self-first ordering removed)
  → only IX red; B (cross-subsystem duplicate check removed) → only V
  red. Stage VI alone could NOT prove ordering (one artifact) — IX added.
- **The stale-build bug, live:** restoring plant B's source by `cp`
  landed in the same second as the plant's object compile; the
  aedilis-generated struere.sh (`fons -nt obj`) skipped recompiling and
  bin/fabrica kept the plant — fumus stayed red over byte-identical
  source. `touch` + rebuild fixed it. Ledger: ictus on park 01KZYN4VPZ.
  This is exactly slice 2's target (objects by digest).
