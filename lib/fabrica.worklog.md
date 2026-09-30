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

## 2026-09-30 — digest memo + `plagulae` input (Fran's decisions after T7)

**Memo.** `FabricaSutura.sigilla` (TabulaDispersa via → Sigillum*, per
run): each file read and hashed once per run. Particula now carries the
content digest, not the content. celer 6.0 s → 1.39 s (before the corpus
inputs). Plant (lookup disabled): the no-reread and per-run asserts went
red, digest equality stayed green — as predicted.
- **Bus error on the way:** adding a field to FabricaSutura left the
  tool's stack-allocated sutura with a garbage `sigilla` (my replace
  missed the formatter-aligned lines). New `fabrica_suturam_parare()`
  NIHILs every field; the tool and the gate call it first, so future
  fields default safe.

**`plagulae` input kind** (Fran chose (a), a tree input; built as
depth-0 "files of a directory" with `suffixa=".c .h"` because the
corpus globs are depth 0 — lib/ has 14 nested files the corpus does not
embed). Subdirectories skipped; each file's path is in the digest, so a
new or deleted matching file changes it. Plant (suffix filter always
true) → only the "non-matching file contributes nothing" assert red.
Declared on briar, spectator, silex to mirror corpus_infixum.sh's
globs + named files.
- **Coverage proven:** a comment appended to lib/qr.c — in NO binary's
  closure — made exactly briar, briar-spectator, silex STALUM. The
  "rebake after lib/" ritual is now a named verdict with a command.
- **Timing:** celer steady 1.66 s (warm cache, 15 installed + 35
  generated declared). Cold cache right after heavy builds measured
  ~4.7 s — note for T8's session-start hook.
- zsh non-splitting bit my timing loop once more (`$sel` one word →
  nothing matched → "0.00 s"): a too-good number is a question.

## 2026-09-30 — T6: records (`memorabilis`), and the snippet win

**Win measured (clean, sequential):** `bin/fabrica iudicare -plenus` on
an unchanged tree: 166 s with no records → **46.7 s** with records (38
snippets `memoria`; the rest — amalgams, grammar tables, capsulae,
numerals — still regenerate: they are NOT memorabilis, their inputs are
hand lists). Quick mode now reports the 38 snippets RECENS by record
instead of NON IUDICATUM; 2.64 s (budget 2 s — see below).

**Mechanism.**
- `actio memorabilis="verum"` (canon `genus="veritas"`; absent =
  falsum; anything else refused with via:linea). Records consulted AND
  written only for memorabilis actions; written only after a
  regeneration that matched (never after relatio or a record hit).
- Record key = sha256(input digest ‖ argv words NUL-separated): a new
  root added to a declaration's mandatum does not change the inputs
  until the generator runs (manifests not yet written) — without argv
  in the key the old record would still match. Plant (argv loop
  zeroed) → exactly that assertion red.
- Row matches only if titulus, key AND artifact digest match (Review
  Focus 1: hand-edited artifact → STALUM, never `memoria`).
- sqlite lives in tools/fabrica.c only (scrinium), build/fabrica.db,
  table verificationes(titulus, clavis, artificium, tempus). Deviation
  from plan: no `duratio_ms` (the seam has no duration; YAGNI), no
  separate tools/fabrica_memoria.c (one file, no annotation machinery).

**Snippet inputs (Step 3 = branch b).** `bin/aedilis --enumerare`
returned before writing any manifest. Added `--manifestum VIA` (only
with --enumerare; explicit path — the default build/aedilis/<basename>/
would collide with installed binaries' manifests). Generators
(fontes_generare, silva_fontes_generare, compile_tests_fontes_generare,
gesta/fontes_generare) write one manifest per root into
build/fabrica/clausurae/<snippet>/ (emptied first) and honour
FABRICA_SCRIPTURA. New input kinds: `manifesta` (dir of manifests,
each explicated, names digested) and `radices` (listings of every
include root named in aedilis.stml — 26 dirs; hand-listing them in 17
declarations was the alternative). 17 memorabilis actions cover all 38
snippets (silva = ONE action, 22 outputs, Fran's choice).
- silva_fontes_generare.sh derives each distinct root ONCE (promptuarium
  per run): 238 s → 35.6 s, output byte-identical. The 22 snippets had
  each re-derived the same 25 silva/fontes/*.c roots.
- P1's warning found a real gap: `officina/amalgama` was not an include
  root (vindex.sh passes it as -I) → vindex's closure had
  `inresoluta "officina.h"`. Added to aedilis.stml (no basename
  collisions); side effect: every installed binary STALUM (configuratio
  input) → reinstalled.

**Run-once memos (per judge run), all on the seam, all NIHIL-safe:**
`regenerationes` (generator once per action, failures too — T1 ran it
once per OUTPUT: silva would have run 22×), `digesta` (action input
digest once — celer was 9.4 s: 22 × 0.38 s re-explicating silva's 54
manifests), directory listings in `sigilla` under "via/" (26 include
roots × 17 actions). `-plenus` 78 → 64 s before records existed.

**Bugs on the way (the instructive ones):**
- **Records never hit** in the first clean two-pass run (0 `memoria`).
  Particle trace (FABRICA_VESTIGIUM, temporary printf) showed the input
  set identical in celer, standalone and -plenus — yet stored keys
  differed per run. Diff of two standalone traces: `bin/aedilis` bytes
  changed. Cause: tools/amalgama_fontes_generare.sh (a T4 generator)
  runs `aedilis_struere.sh` UNCONDITIONALLY → every judge run relinked
  bin/aedilis (the judge writing installed state), and the link is NOT
  reproducible (149 bytes: LC_UUID + code signature; `strings` equal).
  Fixes: under FABRICA_SCRIPTURA the generator only requires bin/aedilis;
  snippet actions name aedilis's SOURCES (build/aedilis/aedilis/
  manifestum.stml) instead of the binary — the binary's fidelity to its
  sources is judged by its own relatio action. A bin/ hash snapshot
  around a -plenus is now empty (checked).
- **Two judges ran concurrently** (my zsh glob error did not abort the
  command list; I relaunched) and emptied each other's clausurae and
  scratch dirs → 3 keys per action after "2 runs". A lock belongs in
  the 1b envelope; noted for T8/1b.

**Plant (Step 6):** comment appended to gesta/instrumenta/fori_principale.c
(in exactly one manifest) → `RECENS gesta/fori_fontes_generata.sh -
regeneratio congruit`, all 37 others `memoria`; restored → fori
`memoria` again under the original key.

**Open for T8:** celer 2.64 s > 2 s. Dominated by compile_tests' closure
(652 files, 46 MB: sqlite3.c, biblia_dr.c, capsula_libri.c) hashed at
-O0 (sample: `_bloccum_comprimere` 307/700). Honest fix: sigillum at -O2
(needs a per-object compile rule in aedilis). A stat-keyed digest cache
would violate "never mtime".

## 2026-09-30 — T8 part 1: sigillum at -O2 via aedilis `compilatio`

celer 2.65 s → 1.37 s warm (1.93 s first run) — under the 2 s budget
for the session hook. Mechanism lives in aedilis (tools/aedilis.worklog.md).
Every aedilis-built tool linking sigillum benefits (fabrica, mensor,
mensor_ui, forum, pictor); briar/spectator/silex link the root runner's
build/*.o blind and are unaffected. aedilis.stml changed → all installed
binaries reinstalled. Spike detour worth keeping: my first "all -O2"
link was slower than "sigillum only" — BSD sed has no `\|` in basic
regex, so the substitution silently did nothing and the binary was the
-O0 one. A surprising measurement is a question about the measurement.

## 2026-09-30 — T8 part 2: lock, the Q36 line, the oracle gate

- **Lock.** `-plenus` takes `build/fabrica/sera` via `filum_seram_capere`
  (flock; the kernel releases it when the process dies — no stale lock
  file logic). Held → exit 2, "iudex plenus alius currit". Celer takes
  no lock (writes nothing). Fumus XII holds the lock from python
  `fcntl.flock` (same kernel lock) and checks both sides.
- **Q36 line.** "IGNOTUM: N binaria in bin/ sine declaratione (numerus
  solus)" — planned in T7 Step 6, never built. Informational like
  orphans: it does NOT set exit 1, else celer would always exit 1 (36
  test/compile_tools binaries today). Fumus XIII.
- **Oracle gate** `tools/fabrica_oraculum.sh`, registered as `fabrica`
  (and the smoke gate as `fabrica-fumus`; both in PORTAE and in the
  ledger inventory 'suitae probationum'). Runs generata, then
  `-plenus -omnia` (sequentially — never both at once), maps both to
  per-artifact recens / non; STALUM and IGNOTUM are both "non", which
  dissolves the known generata-IV-calls-breakage-STALUM mismatch.
  Silence counts: an artifact generata judges but fabrica does not =
  DISCORDIA ("declaratio abest?"). Clean tree: 49 compared, 0
  disagreements, 24 fabrica-only (excludenda, silva tables, capsulae),
  ~3.5 min.
- **Plant.** entitates declaration removed + a comment word in the
  committed lib/runae_tabulae.c → exactly one DISCORDIA (entitates,
  fabrica TACET); runae agreed (both "non"). My FIRST plant appended
  `/* … */` inside the header comment → the file stopped compiling →
  generata IV (which compiles) said STALUM for two amalgamator
  manifests while fabrica (which doesn't) said RECENS: two
  "disagreements" that were the plant's fault. Second time this
  session a plant failed to compile; habit recorded (syntax-check the
  plant before any gate). On a tree that does not compile, the oracle
  can disagree on stage IV — that is honest: the two gates measure
  different things there, and radix is red anyway.
