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

## 2026-09-30 — T7 (part 1): -provenientia; bin/fabrica and bin/manus

Order changed with Fran: T7 before T6 (decretum 01M3RDBCB8). Built:
`include/provenientia.h` + `lib/provenientia.c` (pure `provenientia_textus`
+ `provenientia_respondere`, which prints that same text — one format),
`tools/provenientia_scribere.sh` (writes build/fabrica/provenientia/T.c
only on change and then DELETES its object, so the same-second mtime trap
of T3 cannot keep an old digest), `fabrica_provenientia_via` +
`fabrica_actionem_sigillare` (the one digest function for judge AND
`bin/fabrica digestum`, provenance file excluded). Tools answer the flag
FIRST in principale (fabrica: before its cwd guard — works from /tmp).

- **The "one function" rule broke on day one**: I switched the judge to
  `fabrica_actionem_sigillare` but left `_digestum` on the unexcluded one
  → three different digests (embedded / digestum / judge). The judge
  itself reported the fresh bin/fabrica STALUM on its first run. Fixed;
  fumus XI now asserts `digestum == ingressus reported` (born red by
  planting the old call: VI and XI red).
- **Bootstrap:** the pre-T7 bin/fabrica cannot exclude the provenance
  file, which aedilis now lists in the closure → "ingressus absens".
  fabrica_struere.sh builds TWICE; pass 1 tolerates (PROVENIENTIA_TOLERANS
  → "ignotum"), pass 2 writes the digest computed by the binary just
  built.
- **The danger, live:** while manus's build was failing, `bin/manus
  -provenientia` ran the OLD binary, which treated the flag as a command
  ("nulla sessio viva"). Harmless here — and exactly why only binaries
  declared with provenance are ever asked.
- **The six-week bug, caught:** a comment line appended to lib/manus.c →
  `STALUM bin/manus - ingressus mutati post institutionem`, healing
  command named; rebuild → RECENS. (My first plant silently did not
  apply — sed matched nothing — and "2 recentia" proved nothing; always
  check the plant's diff.)
- Inputs of an installed binary: its aedilis manifest (complete since P1)
  + aedilis.stml + installer + writer script. Not declared: compiler
  identity, include-root listings (a new shadowing header) — noted.

## 2026-09-30 — T7 (part 2a): ten more installed binaries

aedilis, canon_examen, canon_coquere, natura_examen, natura_canones,
natura_glossae, natura, stml, mensor, mensor_ui now answer
`-provenientia` and are declared RELATIO (12 installed binaries RECENS).
Two installer variants: aedilis-generated struere.sh (mensor, mensor_ui:
the manus recipe) and HAND-built (aedilis, canon, natura, stml): new
`tools/provenientia_obiectum.sh T ARTIFICIUM SCOPUS` refreshes the aedilis
manifest (the digest follows the true closure even when the build uses a
hand list), writes the provenance file, ALWAYS compiles it (no mtime
trap) and prints the object path for the link line. stml's closure loop
skips `build/fabrica/provenientia/*` (basename `stml` would collide with
lib/stml.c's object). aedilis's committed snippet regenerated (+provenientia).

- **HOLE FOUND AND FIXED — the main source was never in the digest.**
  An aedilis manifest names the scope file only in its `scopus`
  attribute, never among `obiecta`; `fabrica_manifestum_legere` read
  sections only. Editing `tools/manus_instrumentum.c` itself would NOT
  have made bin/manus stale. Found because bin/canon_examen and
  bin/canon_coquere reported the SAME digest despite different mains.
  Reader now adds `scopus` (gate: count 5 → 6 + scope present, born red).
  All installs went STALUM (new digest definition), rebuilt → RECENS;
  plant on tools/canon_examen.c → canon_examen STALUM, canon_coquere not.
- Plant on lib/canon.c → exactly the binaries whose MANIFESTS list it
  (canon_examen, canon_coquere, natura_examen); natura_canones/glossae/
  natura link canon.o via their hand list but do not use it — the digest
  follows use, not the link line.
- mensor_ui (GUI) answers the flag and returns before any window.

## 2026-09-30 — T7 (part 2b): briar, spectator, silex

All 15 installed binaries now answer `-provenientia` and are RECENS;
`briar -versio` unchanged. briar/spectator compile the provenance object
via `tools/provenientia_obiectum.sh`; briar compiles lib/provenientia.c
inline (its link list is briar/build only), the spectator and silex get
it from the blind build/*.o list (inline too = duplicate symbols — my
first spectator attempt). briar's declaration adds what the installer
generates outside aedilis's sight: MUTATIONES.md, the icon, the four
facies files. Plant on briar/fontes/briar_plagulae.c → exactly the
manifests' prediction (briar, spectator; not silex).

- **silex_struere.sh had been BROKEN since 74642c4f (toml Q12,
  2026-09-28)**: the root runner started putting materia_*.o in build/,
  silex links build/*.o blind AND compiles materia itself into
  silva/build → duplicate symbols. Nobody runs it routinely, so nobody
  saw. Fixed: the blind list skips any object whose basename silex
  already builds. (My T1 blind-link "spot check" used compile_tools to
  avoid touching ~/.bin — and so missed this.)
- **NOT covered: corpus freshness** ("rebake after lib/"). The embedded
  corpus capsule is in the closure, but it is regenerated by the
  installer by mtime from lib/ include/ vendor/ wholesale; a lib/ file
  outside briar's closure changes the corpus without changing the
  digest. Needs a tree input kind or a declared corpus action — question
  for Fran.
- **celer now 6.0 s (budget < 2 s).** Measured: each briar-family binary
  ~0.9 s (49.8 MB corpus capsule hashed at -O0); the rest is my T1
  design hashing inputs PER OUTPUT (tabulae_silvae: 7x, capsulae: 2x),
  even in celer. Fix proposed: a per-run file-digest memo in the core
  (each file hashed once; the corpus shared by three binaries).
