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

## 2026-09-30 — T4: the amalgam chain declared

Scratch mode (`FABRICA_SCRIPTURA`, exported ABSOLUTE by bin/fabrica —
`silva/amalgamare.sh` cds into silva/) in the three amalgamators (sibling
of AMALGAMA_COMPARARE: skip porta_vetustatis, generate only, exit),
`amalgama_fontes_generare.sh` (final mv lands in scratch) and
`amalgama_excludenda_generare.sh`. All nine regenerations byte-equal to
the committed files (silva manifest 22 s dominates); `-plenus` over the
nine: 42 s, 9 RECENS.

- **fontes_generata.h reads ITSELF** (lines 98-109: the committed file's
  order is the "praelatio"), so the output is declared as an input too.
  The planner ignores the self-edge.
- **excludenda writes IN PLACE during its mechanism** (the amalgamator
  recompiles against the committed header each round), restoring the
  committed copy by EXIT trap. Scratch mode behaves like -probare: copy
  the result out, trap restores. Known deviation from "the judge never
  writes over a committed file": transient, and kill -9 skips the trap —
  same exposure as generata IV today.
- **excludenda links objects from `<sub>/build/` that amalgamare.sh
  compiles** ("obiecta calefacta", undeclared input in build/). Found
  when an INVALID first plant (a `/*` inside an open comment →
  -Wcomment) broke silva/build objects: the next run, in dependency
  order, hit excludenda before amalgamare and failed "constructio
  amalgamatoris (gyrus 1)". Reproduced by deleting
  silva/build/silva_token.o. Declared now (silva: 9 objects by name;
  tessera/officina: DIRECTORIUM of the build dir, names only). Stale-but-
  present objects remain mtime-judged inside the generator → slice 2.
- **Oracle mapping for T8:** generata IV reported a COMPILE failure as
  "STALUM manifestum amalgamatoris tessera"; fabrica separates IGNOTUM
  (generator broke) from STALUM. The agreement gate must map, not
  string-compare.
- **Bug found by -plenus:** orphans were skipped whenever ANY argument
  was present (flags included). Fixed (only artifact filters skip), and
  fumus VIII now also runs `-plenus` in a temp root (born red on the old
  binary).
- **Agreement (valid plant, a word in silva_token.h's first comment):**
  fabrica STALUM silva/amalgama/silva.c only (1 line) + 8 RECENS;
  generata STALUM VII silva only (2 diff lines = one line changed).
- **Inputs are NOT complete** (the radices' aedilis closures are not
  declared) — fine for regeneration verdicts, NOT for T6 records. Each
  subsystem file says so in its header comment.

## 2026-09-30 — T5: generated sources declared

26 more artifacts (35 total): silva lexicon (2), silva grammar tables
(6 + `silva/c89.canon` — also generated by silva/generare.sh, per the
canon registry), latina.h numerals (the whole file is the artifact; the
hand part is copied byte-for-byte, so latina.h is its own input), runae,
entitates, seven TOML capsulae (.c + .h). Scratch mode added to the five
generators; capsulae through a new wrapper `tools/capsula_regenerare.sh`
(mirrors the TOML dir into scratch — capsula_generare writes beside its
TOML, so no change to the C tool). Every scratch regeneration was
byte-identical to the committed file. `-plenus` over 35: 78 s.

- No `formatio` action needed: route B (c918661e) made these files the
  generator's exact output.
- entitates reads Python's html.entities table — an input OUTSIDE the
  repo, declared as `instrumentum /opt/homebrew/bin/python3` (a Python
  upgrade changes the digest). Machine-specific path: acceptable for
  this house; revisit if the tree is ever built elsewhere.
- The two TOML-less capsulae (book_assets/capsula_libri.c,
  probationes/capsula_assets.c) are NOT declared (plan said IGNOTUM, but
  the dialect has no "unknown provenance" kind and inventing one is
  YAGNI) — they belong to desideratum 01M3R27R5J (generated-files
  inventory).
- Born red (three plants in one -plenus): a comment appended to
  silva_tabulae_sceleti.c → that file only; to entitates_html_tabula.c
  → STALUM + generata VIII agrees; an index.html edit in apps/villa →
  capsula_villa.c STALUM (12 compressed lines), .h RECENS. generata saw
  only the entitates one: (a) and (c) are coverage the house lacked.
- Inputs still incomplete where a generator is compiled from a whole
  directory (tabulae_silvae: silva/fontes names only) — T6 caution.
