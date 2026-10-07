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

## 2026-09-30 — T8 part 4: the commit check (Fran's option B)

`fabrica_actio_tacta` (core) answers "does this path touch this
action": an expanded input (same `_particulas_colligere` as the digest —
the two cannot disagree), an output, or a new/deleted file in a
directory the action lists (directorium/radices/manifesta: "dir/"
particle; plagulae: its dir + suffix). Inputs that cannot be expanded →
touched (the judge will say IGNOTUM). `bin/fabrica iudicare -plenus
-tacta VIA…` judges only touched REGENERATIO outputs (installed binaries
are not committed — the session hook names those); nothing touched →
exit 0, so exit 2 keeps meaning "could not judge" (lock, broken
declarations). `silva.commissio` → `_fabricam_exigere` after the lint,
before gates; `commissio_umbra` runs it once before its snapshots.
Measured: docs-only commit 0 s; lib/qr.c ~57 s (compile_tests snippet);
silva/fontes/silva_nodus.c touches 46 artifacts.
- pythonica tests stub the binary via `silva.FABRICA_BIN` for the whole
  file (every commissio test would otherwise run the real judge).
- My first commissio-level test used gate 'ficta-petita' left over from
  an earlier section (script deleted) — with the check planted away, the
  suite CRASHED instead of reporting one FRACTUM. Made hermetic (absent
  path, no gates, broad except): planted → one clean red. A plant whose
  red is a crash tells you less than you think.
- Live: hand-edited gesta/fori_fontes_generata.sh → refused, STALUM
  "lineae differentes: 12", SANATIO names the fontes_generare command.

## 2026-09-30 — cooked generated files declared (from the inventory)

The inventory 'generata commissa (fabrica)' (ledger 01M3TBHT) named
committed generated files no declaration covered. Declared now (root
aedificatio.stml, section 'GENERATA COCTA', hand-listed inputs, NOT
memorabilis): canones_cocti (natura_canones.sh: 36 canons + semina.census
= 37 outputs), lectores_cocti (canon_coquere.sh: 4 reader files; input
natura/cocta/planta.canon orders it after canones_cocti), glossae_pagina
(natura_glossae.sh -pagina), registrum_<g> x6 (materia/coquere.sh,
2 files each), capsula_assets. 56 artifacts, all RECENS by regeneration
(5.7 s). Plant: line appended to glossae.html + comment appended to
lib/quaestiones_lectio.c -> exactly those two STALUM, the reader's three
siblings RECENS.
- Scratch modes added: canon_coquere.sh (its -probare path: generate,
  substitute paths, FORMAT, then copy - the reader embeds its own paths),
  natura_canones.sh (and it skips its nested call of canon_coquere.sh
  under the judge), natura_glossae.sh -pagina, materia coctor.c (C:
  -scribere under FABRICA_SCRIPTURA writes below that root). Each
  verified byte-identical against the tree before declaring.
- STALE FOUND, nobody had noticed: both cooked readers (canon_coquere
  -probare: RANCIDUS) - the generator formats its output with the house
  formatter, and the formatter evolved; the diff was whitespace only
  (token streams identical, checked). natura/cocta/glossae.html stale
  since 2026-08-07 (187 -> 468 glossed terms) - real content drift. Both
  regenerated. Lesson in the declaration: the readers' inputs include the
  FORMATTER's sources.
- The registries were NOT unguarded: each client suite compares them
  byte-for-byte via materia_registrum_recens (the coctor source says so:
  "porta rancoris eadem quam probationes clientium vocant"). My inventory
  cell was wrong; corrected.
- capsula_libri NOT declared: libri.toml globs book_assets/*.txt, which
  is gitignored; ~3,300 local Gutenberg books today vs 11 in the
  committed capsule. Regeneration in scratch produced a 3.6 GB file
  (deleted). Not reproducible from the tree - a snapshot fixture; what
  to do with it is Fran's call.
- The inventory's candidate search missed natura/cocta/*.canon (marker
  on line 2, after the XML declaration) - it is a lower bound, as its
  description says; rows added.

## 2026-10-01 — plan 1b T1: the two interfaces (behaviour-preserving)

The vocabulary fit today's code with no forcing: the eight input kinds
became `FabricaGenus` structs (sigillare = the old `_X_explicare`
bodies, untouched; three documentation names share the file-bytes
implementation), the two provenances became `FabricaStrategia` structs
(regeneratio carries memoria in front of it, gated by `memorabilis` as
before; relatio wraps `_relationem_iudicare`). `_particulas_colligere`
calls `genus->sigillare`; `fabrica_iudicare` = sigillare once, then
`exitus->strategia->iudicare`. No switch on a kind remains; the two
switches left are on verdict status (CLI counting) and on locus SHAPE
(`_locus_tangit`: plagula / plagulae / arbor), which is geometry.

`-tacta` is now generic: `fabrica_actionem_enumerare` (each type's
`enumerare`, or derived from its particles: file -> PLAGULA, `dir/` ->
PLAGULAE) plus each output type's `locare`. The old special case for
plagulae (new file matching the suffix) is the plagulae type's own
`enumerare` adding a PLAGULAE locus with its suffixes.

Deviations from the plan's Interfaces block: `FabricaStrategia` gained
`genus_ordinarium` (the output type when a declaration names none:
relatio -> binarium, regeneratio -> fasciculus) - the reader and the
test helper both use it, so the default lives in one place. The reader
also refuses an output whose type has no `locare` ("genus ingressus
solum, exitus esse nequit").

Oracle (two judges, one tree): `build/fabrica/vetus` (pre-edit binary)
and the new `bin/fabrica` gave byte-identical `iudicare -plenus -omnia`
(145 lines) and `digestum` for all 64 actions. One apparent difference
on the first comparison was ORDER, not behaviour: the edit changed
lib/fabrica.c, which is in the compile_tests snippet's closure, so the
first judge to run missed its record, regenerated ('regeneratio
congruit') and wrote a record the second judge then hit ('memoria').
Re-running the new judge -> identical. Lesson for any future
two-binary oracle: warm records with BOTH binaries, or compare after a
second pass. First -plenus after the edit: 119 s (record misses).

Against the pre-edit capture (different tree, by design): briar,
briar_spectator, silex STALUM (lib/*.c is in their corpus plagulae);
digests of fabrica, briar, briar_spectator, silex,
fragmentum_compile_tests moved. T3's sanare heals the three binaries.

Plant: plagulae registered with the file-bytes sigillare. Core: the
plagulae block red only (5 assertions; genera and tacta blocks green -
enumerare for plagulae is separate). Digests: the 15 actions with a
plagulae input fail "ingressus absens" exactly as predicted; a 16th,
`fabrica`, changed digest for a reason not predicted - the planted
lib/fabrica.c is in the judge's own closure (bytes changed, not a
failure). Restored; digests identical again. Assertions 166 -> 213.

## 2026-10-01 — plan 1b T2: composites, preconditions, `ignota`

Names (Fran, 2026-10-01): `compositum titulus` with `pars` children
(exactly one of `artificium=`, `actio=`, `compositum=`), and
`praecondicio actio=` as a child of `actio`. Composites are read from
the same subsystem files by a second reader (`fabrica_composita_legere`)
so the action reader's signature did not change; the CLI collects them
across subsystems (duplicate titles refused) and EXPLAINS every one at
load, so a broken composite fails every judge run (exit 2), never
silently.

Worst-of is computed on a FLAT list (`fabrica_compositum_explicare`
flattens nested composites, de-duplicated, first-appearance order);
since worst-of is associative the flat answer equals the nested one -
a single function, no recursion over verdicts. Order: RECENS <
NON IUDICATUM < IGNOTUM < STALUM. The causa always starts with the
status word ("STALUM: bin/briar, ..."; "RECENS: omnes partes recentes
(9)"), at most three names then "+N" in Roman numerals.

`ignota` is a third strategy with `iudicatur` FALSUM. Rules enforced at
load (`fabrica_praecondiciones_probare` + explicare): a precondition
must name an existing action; an `ignota` output may never be another
action's INPUT (only a precondition reaches it) nor a composite part.
The precondition edge is general (order without digest) - T5 also uses
it for tools installers call. `fabrica_ordinare` treats it as an edge.

Live: `bin/fabrica iudicare installata` (0.9 s) -> the three binaries
stale since T1 and `COMPOSITUM installata - STALUM: bin/briar,
bin/briar-spectator, bin/silex`; `-plenus amalgamata` -> 9 RECENS
(41 s). Fumus XV: red against the pre-composite judge (it took `omnia`
for a path: exit 2), green now. Plant: IGNOTUM ranked above STALUM ->
exactly the 4 predicted assertions red, XV green (no IGNOTUM part).
A `-Wfortify-source` overflow in a 32-byte sprintf buffer slipped past
the root test build and was caught only by fabrica_struere's flags -
the two builds' flags differ (noted, not chased). Assertions 213 -> 259.

## 2026-10-01 — plan 1b T3: `sanare`

`fabrica_sanare` (core, pure) walks the dependency order over the
scope (selected outputs' actions + everything above them via
`_pendet`, which includes precondition edges): judge NOW (plenus) ->
all RECENS: nothing; an upstream FRACTUM/OMISSUM: OMISSUM naming it;
else realize `ignota` preconditions once per run, `agere`, EMPTY the
three per-run memos, re-judge -> SANATUM or FRACTUM ("exitus 0 sed non
RECENS"). Actions whose outputs are all `ignota` are only ever run as
preconditions. `-siccum`: AGENDUM for stale-now, FORTASSE for
recent-now-but-downstream-of-an-AGENDUM.

The memo-purge test needed a real stale-memo path to be honest.
Found two: (1) an action that reads its own output (praelatio, as
fontes_generata.h does) memoizes the OLD digest of that file; a
downstream BINARY (relatio) judged afterwards would use the old digest,
match its old relation, and never be rebuilt. (2) The post-condition
re-judge would reuse the pre-run regeneration memo instead of
regenerating. Plant (purge disabled) -> exactly those 4 assertions red.
The test disk grew scripted generators (`ScriptumFictum`, keyed by
mandatum[0]): output = prefix + TODAY's content of a source file, so
dependencies are modelled for real, and the same script drives both
the scratch regeneration and the in-place `agere`.

Tool: `agere` = processus_exsequi with FABRICA_SCRIPTURA unset, 30 min
deadline, log `build/fabrica/acta/TITULUS.log` (command, effusio,
erratum - separate, the process API captures them apart), cauda = last
20 lines. CLI checks the judge itself FIRST (celer) and refuses before
the lock if bin/fabrica is not RECENS; lock shared with -plenus.

Live: entitates_html output planted stale (blank line appended) ->
`sanare lib/entitates_html_tabula.c` restored it byte-for-byte (git
diff empty), 0.17 s total. Whole-tree `-siccum`: exactly briar,
briar_spectator, silex (125 s - records cold after the lib edits);
`sanare`: 3 SANATUM (briar 215 s, spectator 6.9 s, silex 11.0 s; 5 min
total), then `installata` all RECENS and each `-provenientia` ==
`digestum` (spec par. VI oracle). Fumus XVI-XX red against the T2
judge (no verb), green now. 259 -> 299 assertions. Glossary: sanatio.
Every lib/*.c edit re-stales the three corpus binaries; heal with
`bin/fabrica sanare installata` after committing.

## 2026-10-01 — plan 1b T4: footprints

Step 1 measured every declared action's write set: snapshot (path,
mtime ns, size) of the whole tree minus .git plus ~/.bin, run the
mandatum in place, snapshot, diff - 29 377 files, 0.45 s per python
snapshot; all 64 actions exit 0, tree content unchanged (writes are
mtime-only rewrites). Script + raw JSON were scratchpad-only.

Legit footprints (declared on 52 actions): owned
`build/aedilis/<target>/`, `bin/X.dSYM/`, snippet
`build/fabrica/clausurae/<s>/`, `tessera|officina/build/`,
`build/latina_numeri*`, briar's build/ capsules; SHARED (communis):
`build/aedilis/obiecta/`, `silva/build/` (formator, censor, amalgamator
objects - five actions), `build/aedilis/{amalgama_fontes, excludenda,
caput}/` (six amalgam-chain actions), `build/canon/`, `build/natura/`,
`build/aedilis/natura_quaesitor/`, `materia/build/coctor/` (six
registries), the corpus capsule (briar + silex). ~/.bin/X declared as
vestigium for the four installers until T5 splits the copy out.

Defects (NOT declared; T5 fixes): D1 mensor_ui rewrites committed
capsula_mensor.{c,h}; D2 canon_examen/canon_coquere and the four
natura_* each relink their siblings (one script per family); D3
fontes_* and amalgama_* relink bin/aedilis in place (the 1a guard only
covers FABRICA_SCRIPTURA); D4 amalgama_* rewrites the excludenda
action's excludenda_generata.h (nested producer); D5 excludenda_silva
and amalgama_silva rewrite silva/fontes/silva_latina_datum.{c,h} in
place, trap-restored; D6 tabulae_silvae touches silva/amalgama/silva.h
and silva/instrumenta/principalia/hospes.c (committed, undeclared).

Mechanism: `fabrica_vestigia_comparare` (merge walk of two sorted
snapshots; new, deleted, or mtime/size-changed paths outside the
footprint) - an identical rewrite IS a write (mtime). The live plant
(comment in mensor_ui.c) first named a THIRD path, build/fabrica.db-shm:
the executor's own sqlite state. Executor state (fabrica.db{,-wal,-shm},
sera) is now in every action's envelope for the write check but NOT for
waves (it is common to all, so it would have serialized every pair).
Re-planting the SAME comment after a run is a no-op (the binary was
rebuilt from it: RECENS) - plants must differ each time.

Waves (`-siccum`): greedy by dependency level; two actions share a wave
iff no edge, footprints disjoint (prefix-conservative), neither has a
communis area. Live: briar | briar_spectator | silex (corpus capsule is
shared). Tool: tree walk with lstat (mtime field differs macOS/Linux:
one #if in the tool, not filum.h - a filum.h change owes ~29 gates).
Fixture bug found: sanare_radix inserted extra actions with printf %s,
so `\n` stayed literal (XX had passed on that malformed STML; now %b).
Plant (mtime ignored) -> exactly the identical-rewrite assertion red.
299 -> 317.
Commit refused once by examen (pre-commit): the tool's mtime `#if
defined(__APPLE__) ... #else st_mtim` - examen evaluates the UNGUARDED
branch against silva's Darwin lexicon, which has no st_mtim. House
pattern (lib/vigilia.c): guard the LINUX branch (`#ifdef __linux__`),
leave Darwin unguarded. Run ./silva/examen.sh on touched C before
committing.

## 2026-10-01 — plan 1b T5, part A1: edges by places, D1, D2

Found at T5's start: `fabrica_ordinare` linked actions only through
DECLARED paths, so every generated file consumed through a manifest
closure (latina.h by all installers and snippets, silva.c/tessera.c,
registries, capsules, lexicon/tables, capsula_mensor -> mensor_ui) was
invisible to ordering - `sanare` was right by declaration-order luck.
Fix (Fran 2026-10-01): `fabrica_dependentias_computare` derives each
action's dependencies from its inputs' `enumerare` (manifest closures
included; an input that cannot be enumerated - fresh clone - falls back
to its declared path), stored on `actio->dependentiae`; `_pendet` uses
them, so ordering, sanare scope and failure propagation, and waves
agree. Exposed exactly ONE cycle: aedilis -> amalgama_silva ->
fontes_silva -> aedilis (the bootstrap: aedilis is built from the silva
amalgam whose file lists are generated by running aedilis). Broken by
type semantics: `instrumentum` enumerates nothing (a tool is USED, not
consumed - no order edge, no -tacta). Zero cycles over all 64 actions.
Consequence for D3: no precondition edge to aedilis (it would recreate
the cycle); generators use the existing bin/aedilis. The CLI computes
dependencies with the judge's per-run sigilla memo (shared, nothing
hashed twice); celer 1.4 -> 2.2 s wall. Plant (dependencies ignored)
-> the 3 predicted assertions.

D1: mensor_ui_struere.sh no longer regenerates the committed capsula;
the manifest edge orders capsula_mensor first. T4's born-red plant now
heals SANATUM.

D2: canon (2 binaries) and natura (4) are one action each. Provenance
scripts take an optional ACTIO argument: file and C symbol keep the
BINARY's name, the digest is the action's. The first heal exposed a
real bug: the digest excluded only provenientia/<TITULUS>.c, but family
binaries have per-binary files IN their closures - a file containing
the digest fed the digest (only bin/natura, whose stem equals the
title, matched). Fix: the whole build/fabrica/provenientia/ directory
is excluded (exclusa entries ending in '/' match a prefix) -
provenance is envelope output, never input; single-binary digests
unchanged. All 15 RECENS after.

Still open in part A: D3/D4 (nested producers under the executor), D5
(silva_latina_datum.{c,h} are GENERATED by the silva amalgamator - an
undeclared generated output, written by excludenda_silva and
amalgama_silva, also in place under the judge), D6 (tabulae_silvae's
generator SPLICES tables into committed silva.h and hospes.c - written
in place even in scratch mode: the judge writes the tree). silex's cold
corpus path writes ~35 build/ and root files - T6's corpus action.

## 2026-10-01 — plan 1b T5, part A2: D3–D6 (Fran: FABRICA_AGIT, D5a, D6)

D3/D4: `agere` sets FABRICA_AGIT=1 for every action (envelope
convention beside FABRICA_SCRIPTURA). Under it amalgamare.sh (x3) skips
porta_vetustatis (which ran the fontes generator - relinking aedilis -
and the excludenda generator in place) and amalgama_fontes_generare.sh
uses the existing bin/aedilis; scripts run by hand keep their
conveniences. Fumus XXIII pins it (plant: setenv removed -> XXIII only).

D5 was NOT a transient edit: silva_latina_datum.{c,h} are committed
files GENERATED from include/latina.h by the silva amalgamator's
'passus 0' on every run (excludenda_silva, amalgama_silva, and the
judge's scratch runs too). Now their own action `latina_datum` (new
silva/instrumenta/principalia/latina_datum.c, stdio only;
silva/latina_datum_generare.sh, scratch-aware, memorabilis); the
amalgamator only reads them (emission + 6 manifest fields removed from
silva_amalgama.{c,h} and the three amalgamators). .c byte-identical;
.h preamble corrected to name the new generator (it claimed the
amalgamator) - so silva.c changed too (the amalgam carries it), healed
by sanare.

D6: tabulae_silvae's generator SPLICES tables into hand-written
silva.h and hospes.c; in scratch mode it now copies them into the
scratch dir and splices there (before: the judge rewrote the committed
files). Both declared outputs; plant (byte in the spliced region) ->
STALUM 1 line, sanare restored it octet-exact.

Found by the first whole-tree sanare: excludenda_tessera/officina link
<sub>/build/mech_*.o built by amalgamare.sh step 1 (another action) -
stale after the silva_amalgama struct change, so the amalgamator broke.
A blind-linked object store, the Part B pattern: amalgamare.sh gains
`-obiecta` (step 1 only, exit 0) and three `ignota` actions
obiecta_mechanismi_{silva,tessera,officina} are preconditions of their
excludenda and amalgama actions (the 1a `directorium <sub>/build` input
- names only - removed: the ignota rule refused it, rightly). Second
whole-tree sanare: PRAEPARATUM x2, SANATUM x2, all else RECENS; full
judge 149 recent (145 + latina_datum x2 + silva.h + hospes.c).

Noted, not changed: directory-listing loci (radices) make every
generator writing into an include root an order edge of every snippet
- conservative-correct, but one failure cascades OMISSUM widely (seen
once: 18 snippets blamed amalgama_tessera).

## 2026-10-01 — plan 1b T5, part B1: object stores and tool edges

Measured: two blind-linked stores, as expected - root build/*.o (silex,
stml, briar via build/imago.o, briar_spectator; compile_tools.sh too,
not a fabrica action) and briar/build/*.o (briar, spectator). Both get a
real objects-only mode (Fran: `--obiecta`, the runners' double-dash
convention; amalgamare.sh kept its single-dash `-obiecta` from A2):
compile_tests.sh --obiecta = compile_libraries, exit 0/1 (replaces the
`--libs-only` filter that matched nothing and exited 2 - compile_tools
now checks the code); briar/compile_probationes.sh --obiecta = objects
and helpers, exit before the test loop. Declared `ignota` actions
obiecta_radicis, obiecta_briar; preconditions on their linkers; tool
edge: every installer except aedilis has precondition aedilis
(fabrica: no edge - aedilis's installer calls bin/fabrica digestum;
the self-check covers it; capsula_generare is undeclared).

The footprint check sharpened the declarations in three live plants:
obiecta_radicis also writes build/amalgamata.txt,
build/amalgamata_probatio/ (compile_libraries runs the amalgam compile
gate) and build/test_logs/radix.log; obiecta_briar writes
build/test_logs/briar.log AND regenerates the corpus capsule (the briar
suite embeds it). Corpus write set measured directly (touch a lib file,
snapshot around corpus_infixum_regenerare): capsule .c/.h,
corpus.symbola.tsv, corpus.versio, corpus_silicis.toml (root,
gitignored), build/nexus.tsv, build/inclusiones.tsv, silva/build
objects - declared communis on briar, silex and obiecta_briar until
T6 gives the corpus its own action. The blind-link bug class shown
live: a planted briar source edit makes obiecta_briar rebuild the
stale .o BEFORE the link (PRAEPARATUM), where before the installer
linked the old object under a digest naming the new source.

## 2026-10-01 — plan 1b T5, part B2: ~/.bin copies, celer, ~ expansion (T5 done)

The four `~/.bin` installers split (Q42): the link stays the
`_struere.sh` action (relatio); the copy is `institutio_X` (file,
regeneratio, memorabilis, celer) running new tools/instituere.sh
(`rm` then `cp || exit 1`; under FABRICA_SCRIPTURA it copies into the
scratch dir). The `_struere.sh` scripts call instituere.sh only when NOT
under FABRICA_AGIT, so a human running them by hand still installs.
`celer="verum"` (Fran): regeneration cheap enough to run in the QUICK
judge too - the session hook now sees ~/.bin lagging bin/ (quick judge
of installata incl. 4 copies: 2.1 s). `~/` paths expand from HOME in the
tool's read seam (`_domum_expandere`). installata = 19 parts.
Fumus XXIV with HOME in the temp root: absent -> STALUM under celer;
sanare creates ~/.bin and copies; changed bin/x -> STALUM under celer;
unwritable ~/.bin -> FRACTUM by exit code (the masked-cp class of spec
par. 0.6 can no longer exit 0); the real ~/.bin/briar digest checked
unchanged. Red against the T2 judge.

Plant lesson: the first ~-expansion plant (call removed) did NOT build -
the now-unused static function is an error under the build flags - so
fabrica_struere failed silently (output suppressed), the OLD bin/fabrica
stayed, XXIV stayed green and XI went red on the half-built judge.
`clang -fsyntax-only` does not see -Werror=unused-function: a plant is
checked by BUILDING it with the real flags. Valid plant (short-circuit
inside the function): XXIV only.

T5 done check: the T4 measurement re-run under FABRICA_AGIT over all 70
actions (64 - 4 merged + latina_datum + 3 mechanism stores + 2 object
stores + 4 copies), every write checked against the declared footprint
with the core's locus rules (scratchpad vestigia_probare.py): 70/70 exit
0, 0 writes outside a footprint. Warm only - cold paths (the corpus,
silva/build) were measured separately during B1; a cold whole-tree
measurement belongs with T6/T7.

## 2026-10-01 — plan 1b T6: briar decomposed (Fran: option a')

Finding that changed the option: the corpus stamp was FUNCTIONAL, not
informational - corpus.versio is embedded in the capsule; lib/silex.c
shows it as the corpus title and briar uses it as the cache key of
embedded-corpus projects (~/.rhubarb/briar/<titulus>-<clavis>). As
`commit=... dies=...` it changed on EVERY regeneration (orphaning those
project dirs) and could not be judged. Now `sigillum=<SHA-256>` over the
exact files the capsule embeds (paths + per-file digests, LC_ALL=C
order): reproducible, and a better key - it moves exactly with content.
Build identity stays in `briar -versio`'s `aedificatum:` line and
-provenientia. Tester-visible: MUTATIONES.md '## inedita' bullet.

Shadow root (tools/capsula_radicis.sh): capsula_generare resolves globs
from - and writes next to - its TOML, which must sit at the repo root.
Under FABRICA_SCRIPTURA the scratch dir gets symlinks to the needed
top-level inputs, TOML and generated inputs (corpus.versio, symbol
table) are written there, outputs land in scratch/build/: embedded
names identical, real root untouched (verified by mtime). Corpus
regeneration is DETERMINISTIC (two scratch runs byte-identical) and
takes ~10 s (it was 3+ min inside silex_struere.sh only because the
mtime logic and nexus ran cold there). Under FABRICA_SCRIPTURA or
FABRICA_AGIT the capsule scripts always regenerate (fabrica judges);
by hand they keep the mtime skip. Linkers skip capsule regeneration
under FABRICA_AGIT; briar suite --obiecta skips symbol table and facies.

Actions: corpus_silicis (memorabilis; owns corpus.versio, toml, symbol
table; nexus cache + silva/build communis), capsula_{facies,icon,
mutationes}_briar; links take the capsules as inputs (edges) instead of
the corpus's plagulae. Composites `briar` (6 parts) and `silex` (4).

Bug found in T4's snapshot: the tree walk FOLLOWED symlinked
directories (the iterator reports them as directories despite the
comment saying otherwise) - the shadow roots' symlinks made real
briar/build and silva/build writes reappear under scratch paths,
outside every footprint. Fix: recurse only into REAL directories
(`lstat` + S_ISDIR; S_ISLNK is not in silva's POSIX lexicon, examen
flagged it). Also: silex's own link writes silva/build (it compiles silva
objects there) - restored as silex's communis area.

Measured: MUTATIONES.md edit -> exactly capsula_mutationes_briar + link
+ copy (actions 7 s; corpus/facies/icon untouched); a lib/*.c edit ->
corpus once (9.6 s), both links, both copies, clean. BUT the whole
command took 2-3 min: the judge re-verifies everything upstream of
briar by regeneration, including the amalgam chain (silva.c is in
briar's closure), which is not memorabilis. Judge cost now dominates -
follow-up: memorabilis for the amalgam chain once its inputs are proven
complete (1a T4 left the radices' closures undeclared).
The first T6 commit was refused by gate silex-semen: it proved "the
embedded corpus was used" by grepping for the OLD stamp (`corpus
commit=`). Its intent holds; pattern now `corpus sigillum=`. The only
consumer of the stamp's format.

## 2026-10-01 — plan 1b T7: cursus, estimates, closing

`cursus` = migration II of build/fabrica.db (titulus, initium,
duratio_ms, eventus, causa). Core: one helper `_sanationem_notare`
decides by the event alone - SANATUM/FRACTUM/PRAEPARATUM are recorded
(they ran), OMISSUM never, AGENDUM/FORTASSE (siccum) get an estimate
from the last SANATUM/PRAEPARATUM run and `tempus_notum` (unknown is
not 0 ms). `-siccum` prints `~T s` or `tempus ignotum` and a total.
FabricaSanatio became a tagged struct (forward typedef) - the seam
names it before its definition.

Found: the first real run recorded NOTHING - every insert failed with
'NOT NULL constraint failed: cursus.causa', hidden by my own output
filter. A healed action's empty causa is a chorda whose datum is NIHIL;
scrinium_ligare_textum passes it to sqlite3_bind_text, which binds NULL.
Fixed locally (TEXTUS_VACUUS); the library question - should an empty
chorda always bind as '' - is filed for Fran (…BVN7), since lib/scrinium.c
has many consumers. Fumus XXV; plant (never record) -> the count
assertion red, then a crash on the empty log (exit 139), XXV red.

Timing lens: the producers inventory's 'tempus' was blank for most
rows; filled the 18 blank producer rows + 2 new producer rows
(latina_datum_generare.sh, instituere.sh) from T5's warm measurement of
all 70 actions (existing hand-written cells left alone); cursus is the
living source from now on. Closing: spec 1b par. X 'As built'; parks
…AR15 (amalgam chain memorabilis - the judge dominates sanare time) and
…6X0 (cold footprint measurement).
The first T7 commit was refused by the commit-time check (1a T8): a
lib/*.c edit staled `corpus_silicis`, whose output lives in build/ and
is never committed. Since T6, regeneratio outputs include build
artifacts (corpus, capsules), so the check would block every lib commit
until a `sanare`. The check's question is "touched AND committed".
pythonica/silva.py: STALUM/IGNOTUM lines whose paths git does not track
(VIAE_COMMISSAE, `git ls-files`; tests stub it) are named with a sanare
hint and do not block; committed ones block as before. Pythonica test
added (stale build capsule passes with a note). On the way: the
pythonica suite first failed in an UNRELATED oratio oracle test - its
objects were stale after today's regenerations (the oracle refused,
'compile_probationes.sh registrum primum'); rebuilding them fixed it.

## 2026-10-01 - park …AR15 step 1: the judge's runs in cursus

"Are we timing every script now?" - not quite: T7 recorded what sanare
DID, but the judge's own regenerations (the reproducible actions it
reruns to compare) were invisible, and they dominate sanare time. Now
the currere seam returns the run's duration (i32* duratio_ms_out) and
_regenerare records each REAL run (never a memo hit) through the same
cursum_inscribere seam, outcome FABRICA_IUDICIUM. Estimates still read
only SANATUM/PRAEPARATUM rows, so a judge run never poses as a heal.

First measurement (full `-plenus`, warm, after the lib edits): 74.6 s
wall, 32 regenerations, 71.4 s inside them. The amalgam chain
(fontes_X, excludenda_X, amalgama_X for silva/officina/tessera) is
~44 s of it - fontes_silva alone 22.3 s; corpus_silicis 11.9 s;
lectores_cocti 9.7 s; tabulae_silvae 2.5 s. The cold first run was
146 s (fragmentum_compile_tests 66 s). Next: prove the chain's inputs
complete and mark it memorabilis, then measure again.

## 2026-10-01 - park …AR15 step 2: the amalgam chain is memorabilis

The nine chain actions (fontes_X, excludenda_X, amalgama_X for silva,
tessera, officina) now declare complete inputs and are memorabilis,
following the 1a T6 snippet pattern (genus 'manifesta'):
- fontes_X writes one aedilis manifest per root into
  build/fabrica/clausurae/X__amalgama (aedilis now accepts
  `--manifestum` with `--partes`, so the closure comes free with the
  derivation it already runs).
- excludenda_X writes the amalgamator's OWN closure (amalgamator.c +
  silva_amalgama.c + silva_unitates.c) into X__mechanismus; it builds
  and runs that binary, and the mech_*.o / silva/build objects come from
  the same sources.
- amalgama_X reads both. Soundness rule: a manifest directory is
  written by an action whose own key reads it - a new #include is an
  edit to a file already in the closure, so the writer misses and
  rewrites. A reader downstream of the writer is fine; a reader that
  could hit while the writer doesn't run would not be.
- Both generators check a postcondition (one manifest per root/source):
  an empty directory digests to nothing and the memo would always hit.
  Plant: aedilis taking `--partes --manifestum` but skipping the write
  -> "clausurae incompletae: 0 manifesta pro 5 radicibus", refused.

Two snags on the way:
1. Cycle: `radices` turned each aedilis include root's listing into an
   ordering edge onto every producer writing there; silva/amalgama is
   such a root, so fontes_silva waited on amalgama_silva. Radices are a
   shadowing DIGEST; real per-file edges come from the manifests. Now
   `_nihil_enumerare`, like instrumentum (T5). Test V red first (cycle).
   This also drops redundant edges for the 13 snippet actions.
2. aedilis's own manifest (the snippet precedent) would be a second
   cycle (aedilis is built from the silva amalgam) - the chain keeps
   `instrumentum bin/aedilis`: a relink costs one miss, never a wrong hit.

Plants on the real tree (full -plenus judge, cursus IUDICIUM rows):
- A: comment appended to silva/fontes/silva_lexema.h (closure header,
  undeclared by name) -> the silva chain reruns, silva.c STALUM;
  tessera/officina excludenda+amalgama rerun (lexer is in their
  amalgamator), their fontes hit. Correct on every row.
- M: comment appended to silva/instrumenta/silva_amalgama.h (mechanism
  only) -> excludenda+amalgama x3 rerun, no fontes, all RECENS.

Numbers: no-op `sanare -siccum installata` 41.5 s -> 19.7 s; full
`-plenus` steady state 74.6 s -> 22.3 s (of which corpus_silicis 10.7 s,
stale from this edit, a hit once healed). Left: lectores_cocti 3-10 s,
tabulae_silvae 2.3 s. A real heal (`sanare installata`) still spends
~2 min of wall time nowhere in cursus - suspect the per-action footprint
snapshots (park …6X0's ground), unmeasured. Clang stays ambient.

## 2026-10-01 - the unrecorded heal time was the snapshot SORT (…6X0)

Profiled with macOS `sample` (no code change): the ~3 min per rebake
not in cursus was `_vestigium_capere` sorting the whole-tree snapshot
with xar_ordinare, then a selection sort - ~8 s per snapshot, two per
action, preconditions included. Fixed in the library (Fran: option B,
lib/xar.c, see lib/xar.worklog.md). Full rebake after the change: 433 s
wall = 145 s actions + 271 s judge regenerations (xar.c is in nearly
every closure, so nearly every memo missed once) + ~17 s unaccounted,
down from ~200 s. Also measured and dropped: a digest-only key on
aedilis's sources for the amalgam chain would miss on exactly the
edits the binary key misses on (aedilis's closure beyond the chains'
is ~10 house files), so `instrumentum bin/aedilis` stays.

## 2026-10-02 - why the `fabrica` gate took ~270 s: generata relinked aedilis

tempora.tsv showed the `fabrica` gate (tools/fabrica_oraculum.sh) at
272 s. It runs the whole generata gate, then `iudicare -plenus`, then
compares. Measured separately: generata 162 s; -plenus right after it
117 s with 29 judge regenerations - and 16 s when run again at once.
generata was invalidating the memos, through three timestamp leaks that
ended in one relink:
1. tools/aedilis_struere.sh relinked bin/aedilis on EVERY call
   (generata reaches it via amalgama_fontes_generare.sh and the snippet
   generators). macOS links are not byte-reproducible (LC_UUID), and the
   amalgam chain + fragmentum_compile_tests key on `instrumentum
   bin/aedilis` -> ~100 s of misses after every generata, which the
   oracle gate inflicted on itself.
2. tools/amalgama_caput.sh rewrote build/aedilis/caput/silva.h every call.
3. generata stage III restored every snippet with cp on exit, and
   tools/fontes_generare.sh rewrote its snippet unconditionally - so
   tools/aedilis_fontes_generata.sh (which aedilis_struere sources) was
   always newer than the binary.
Fixes: new tools/nexus_recens.sh (binarium_recens: strictly newer than
every input, never under FABRICA_AGIT; nectere_atomice: link to a temp
name + mv, dSYM carried) shared by aedilis_struere.sh and
natura_struere.sh (which had its own copy since this morning); 2 and 3
write/restore only when content differs. Measured: bin/aedilis
byte-identical across a generata run; -plenus after generata 117 s ->
16.8 s; the oracle gate 272 s -> 221 s (that run included one legitimate
relink; steady state ~180 s). Plant: binarium_recens forced stale ->
digest changes on every run. Lesson, three times in one afternoon: a
generator must not rewrite unchanged output - mtime is an input to
everything that still judges by mtime.
Remaining: the oracle runs generata in full (162 s) even when the
`generata` gate already ran in the same commit.

## 2026-10-02 - oracle reuses the generata gate's live receipt (Fran: option a)

After the relink fix the `fabrica` gate still ran the whole generata
(~160 s) although the `generata` gate had just run in the same commit.
A live receipt already stores the gate's full output
(build/portae/generata.viva.json.acta) and is valid only while the tree
signature is unchanged (silva.receptum_vivum). tools/fabrica_oraculum.sh
now uses that output when the receipt is valid and sane - same tree,
deterministic generators, same output - and runs generata otherwise;
FABRICA_ORACULUM_RECENS=1 forces a fresh run. Measured: valid receipt ->
whole gate 61 s (was 221-272 s), 51 compared, consensus; after one
tracked file changed the receipt read "rancidum" and generata ran in
full (174 s), consensus. Retiring generata (Q15) stays open until a
longer run of agreement.

## 2026-10-02 - plan 2 T1: the read-ledger spike (PASS)

Recorder lib/lectiones.c (`lectiones_notare(genus, via)`): env
FABRICA_LECTIONES names the file; one write() per line with O_APPEND;
two forked children x 500 lines -> 1000 whole lines. Hooks: filum reads
(L on success, A on failure), existence/status (X/A), via_existit (X/A
- aedilis's include resolution), iter_directoria and aedilis's two raw
opendir loops (D). The lexicon lacked O_APPEND: added (Darwin 0x0008),
auspex_posix certifies it against the real header.
Bug found by the test: a descriptor cached across "variable unset"
kept writing into a file deleted meanwhile (same path, new inode) -
"off" now closes the descriptor.
Blast radius: filum.c and via.c now need lectiones.o in every link. 12
generated source snippets regenerated; 7 hand lists edited (briar,
crusta, saltuarius, toml runners; canon_struere, diagnostica,
natura_struere).
SPIKE (tools/lectiones_spica.sh) on silva/fontes/silva_token.c,
lib/xar.c, toml/probationes/probatio_toml_api.c: every manifest entry
that exists in the tree was CONTENT-read (L); extras are aedilis.stml
(configuration) and the root itself; system headers (stdio.h,
sys/wait.h...) are named by the manifest but never read - the compiler
identity (Q6) covers them. Shadowing data comes from failed lookups
(A: 511 / 206 / 2058), not listings (aedilis --enumerare records no D).
Gate lesson: my first criterion accepted X (existence) as coverage; a
plant removing content reads stayed GREEN because every resolved header
gets an X. A trace from X alone would miss header edits. Coverage now
means L only; the same plant is red (latina.h, piscina.h, xar.h named).
First commit attempt refused by generata stage V: tools/latina_numeri.sh
links lib/filum.c by PATH - my hand-list search matched bare names only.
Five path-form lists fixed (latina_numeri.sh, compile_library.sh,
compile_sputnik.sh, compile_lector.sh, glr_quaestio.sh).
After the T1 commit, `sanare installata` failed: registrum_md and
registrum_toml (materia/coquere.sh, whose list ends a line with
`filum`) did not link. The commit's fabrica gate had judged them RECENS
by memo: their declared inputs name materia/fontes, coctor.c and the
script, but not the lib/ sources the script compiles - exactly the
hand-proved-input hole T2's traces close (a ledger of coquere's build
would have listed lib/filum.c). Fixed the list; rebake 7 sanata.

## 2026-10-02 - plan 2 T2 (part 1): verifying traces in the judge

`lectiones="verum"` on an action (aedificatio.canon) makes the judge
key it on its last congruent run's ledger. The core picks the ledger
path (build/fabrica/lectiones/<titulus>.tsv) and passes it to
`currere`; after a CONGRUENT regeneration it reads the ledger through
the seam, drops S lines, the judge's scratch dir, the ledger dir and
absolute paths outside the tree (after stripping `sutura->radix`),
dedups (genus, via), and digests each entry's present state: L =
content, A/X = presence, D = sorted names; an E line makes the trace
unverifiable (not stored). Stored under (titulus, input key, artifact
digest) - same key as verificationes (ruling: a trace is only valid
for the declared inputs and output it was recorded with). Judge: trace
congruent -> RECENS "lectiones congruunt", even under celer.
Tests (in-memory disk, born red): first run stores 3 entries (scratch
and S dropped); unchanged -> no run; L content change, A path created,
D name added, declared input changed -> regeneration; failed run
stores nothing; action without lectiones never asks. Plant (comparison
always congruent) -> 4 red. Tool: migration III table `lectiones`
(digests as 32-byte blobs - no hex parser exists), FABRICA_LECTIONES
set absolute per regeneration (old file removed first), radix = cwd.

## 2026-10-02 - plan 2 T2 (part 2): writes, env, lint, family switch, audit, ordering

**S and E events.** filum hooks S (written) after a successful fopen in
"w"/"wb"/"a"/"ab", filum_scriptor_aperire and copy destinations, and at
the attempt for unlink / rename (both paths) / mkdir. The judge drops S
lines from traces (a program's own writes are outputs, not inputs).
`lectiones_ambitus(titulus)` = getenv through the channel: one E line
(no value tab when unset); any E makes the trace unverifiable, so it is
not stored. T1 bug found on the way: the ledger descriptor stayed open
when the env var was unset.

**Raw-IO lint** (tools/lectiones_lint.sh). Source = silva nexus index
(`usus` rows of fopen/freopen/opendir/stat/lstat/access/getenv/open), not
grep. Pilot path = aedilis closure of tools/aedilis.c; a raw call there
OBSTATs unless the line before it carries `/* lectiones: notatur */`
(the call is channelled by hand: via_existit stat, the two aedilis
opendirs, iter_directoria). lib/filum.c and lib/lectiones.c are the
channel itself (exempt). First heuristic ("near a hooked call") was too
lenient - a planted raw stat next to a hooked one passed; per-line marker
fixed it. A plant that didn't compile (FILE undeclared in via.c) was
re-planted as `(vacuum)stat(buffer, &info);` and went red. Today: pilot
18 files, 0 obstantia, 5 notatae; 701 raw calls outside (warnings).

**Family switch.** The 19 snippet generators (fragmentum_* /
fragmenta_silva in root, officina, silva, tessera aedificatio.stml) went
from memorabilis + hand-listed `manifesta`/`radices` inputs to
lectiones="verum"; plagulae/fasciculus/configuratio/manifestum inputs
stay declared. First -plenus regenerated all 19 and stored 40 traces
(~60k entries); second -plenus 0 snippet regenerations, 44 s -> 22 s.
Real plants: tessera_cellula.h comment -> exactly the 4 tessera
snippets; shadow tessera/fontes/chorda.h (an A path appearing) -> the
same 4; new gesta/probationes file (D) -> fragmentum_gesta only.

**Multi-output bug.** fragmenta_silva has 22 exitus; the trace key had
no exitus, so each exitus's store evicted the last and only one
survived. Key now includes exitus (migration IV `ADD COLUMN exitus`).
The test's first red was for the wrong reason (exitus scriptura defaulted
to gen/<exitus>.c, so the fake never wrote it) - fixed, then proved
with a plant restoring the old eviction.

**Memo audit.** `sutura.auditus`: 0 off, I every action (`-audit`), N =
1 in N by clavis.octeti[0] % N; default XX under -plenus. An audited
RECENS (memo or trace) regenerates anyway: congruent -> "auditus:
regeneratio congruit"; differs -> STALUM "AUDITUM DISCORS" + cursus
event FABRICA_AUDITUM_DISCORS (a lying memo is a judge bug, it should
shout). Full `iudicare -audit` on the tree: 56 regenerations, 0 DISCORS.

**Ordering from traces.** Dropping `manifesta` dropped the ordering
edges: every snippet reads generated files (latina.h, silva amalgam,
runae tables). `lectiones_ultimae(titulus)` returns the distinct paths
of the title's latest traces; fabrica_dependentias_computare adds them
as loci (D -> PLAGULAE, else PLAGULA). Trap: `_ordinare_per_locos` in
the tool builds its OWN light sutura, so wiring only the judge's
sutura would have left the edges dead in the real tool while the unit
test passed. Memoria now opens before ordering in iudicare and sanare.
Real plant (comment appended to silva/amalgama/silva.c, sanare
-siccum): amalgama_silva AGENDUM in wave II, all 19 snippets "post
amalgama_silva" in wave VIII.
Cold-start caveat: with no trace yet (fresh db), a converted action has
no trace edges - ordering rests on its declared inputs only. The judge
writes nothing, so only `sanare` on a fresh db could heal a snippet
before its generated input (the next judge would catch it).
Second -plenus after this commit: 24 s, one snippet run = the 1-in-20
audit ("auditus: regeneratio congruit").
Open: snippets show "tempus ignotum" in sanare estimates.

## 2026-10-02 - plan 2 T3: the store and aedilis records

Store itself: see lib/thesaurus.worklog.md. Here, the fabrica side.

**aedilis --thesaurus <dir>.** The extraction memo looks up key =
SHA-256(prefix || file bytes), prefix = SHA-256(aedilis binary (argv[0])
|| aedilis.stml). Why that key is sound: silva's parse does NO file IO
(expander, context, front end read nothing; latina is compiled in as
silva_latina_textus), so a file's extraction is a function of its bytes
and the extractor code. The binary digest is the version (any code
change, incl. linked lib/, invalidates - a hand-bumped constant would
silently serve stale records when forgotten). `.m` files go through
clang -MM (reads headers) and are never stored. Record = binary-safe
text (D n / mensura angulata / bytes; A n / mensura / bytes); a failed
extraction is never stored; an unreadable record = miss.
Equivalence (scratch script, 201 roots): --partes and --differentia
outputs identical without / cold / warm store (0 differences).
--partes 59 s -> 16 s (cold) -> 10 s (warm). (--differentia numbers
were confounded by the shared clang -MM memo - not a store number.)

**Callers.** --thesaurus build/aedilis/obiecta in the four snippet
generators (tools/fontes_generare.sh, silva_fontes_generare.sh,
compile_tests_fontes_generare.sh, gesta/fontes_generare.sh) and both
per-root calls in tools/aedilis_porta.sh. Explicit flag, not an env var:
an env var read through lectiones_ambitus would make every trace
unverifiable.
Snippet regeneration (one miss), before -> cold store -> warm:
fragmentum_compile_tests ~64 s -> 17.4 s -> 11.4 s; fragmenta_silva ~34 s
-> 4.9 s -> 3.0 s. Cold already wins: the store shares header parses
across the ~200 separate aedilis processes (the in-process memo could
not). Full -audit with the warm store: 0 AUDITUM DISCORS.

**Judge filter.** Drops build/aedilis/obiecta/{blobi,actiones,
generationes}/ from traces - NOT the root (the shared .o files there are
real inputs to anything that links them). Test: two store lines dropped,
a .o in the root kept (III -> IV); plant (whole root) red.

**Lint.** Linking thesaurus put lib/iter_directoria.c on aedilis's pilot
path and the lint named its per-entry stat (type/size during a listing)
at once. Marked as covered by the listing's D event, with the known gap
written at the call: a type/size change under the same name does not
change D's digest (nothing on the pilot path depends on entry type).

**purgare.** `bin/fabrica purgare [-verificare]`, under the fabrica lock,
keeps GENERATIONES_SERVANDAE (V, A3). Fumus XXVI (store built by hand
with shasum digests: 6 lists, the oldest alone holds k1) -> 3 deleted;
-verificare deletes a corrupted kept blob. Plant (VI kept) red.

## 2026-10-02 - plan 2 T4 part II: familia (per-file actions from a directory)

Design (Fran, two rounds): a `<familia titulus via praefixum? suffixum?>`
holds ONE STML template (`<#@id basis="@basis" fons="@fons"> actio </#>`,
house macro syntax). fabrica lists `via` through the seam
(sutura->enumerare), synthesizes one call per matching file
(`<<#@id basis="x" fons="via/x.c">>`, stml_transclusionem_creare - public
API, no hand-set fragment flags) and runs stml_expandere; the expanded
actio nodes join the ordinary actio loop unchanged. fabrica's own
bookkeeping on each instance: title `familia:basis` (a title in the
template is refused - fabrica owns it), the file added as ingressus
fasciculus. basis = file name minus suffix (prefix kept: probatio_x).
First proposal was four fixed binding rules inside fabrica; Fran asked
about STML macros, and reusing the house's one sanctioned template
mechanism beat inventing a second private one (file/basis can go
anywhere in the body; refs work in attributes and raw content).
Only the parameters the template DECLARES are passed (a call with an
undeclared argument is ARGUMENTUM_SUPERFLUUM); a body referencing
anything else fails loudly in the expansion. Names with '"' or '&' are
refused (they would break the synthetic call).
API: fabrica_declarationes_legere stays pure and REFUSES familia;
fabrica_declarationes_legere_cum_sutura takes the seam. Tool: the
declaration collector builds a light sutura (legere, enumerare).
Canon: `familia` element added to aedificatio.canon. Canon treats the
template body as quoted material (opaque) - a templated actio without
titulus passes canon; fabrica's reader judges the expanded instances.
Tests (in-memory disk): 3 of 5 names match (adiumentum.c, .h excluded)
-> 3 instances (titles, last word = fons, exitus build/t/basis, fons as
input, lectiones flag carried); new file -> one more; pure reader
refuses; directory absent; title in template; undeclared loculus.
Plants: prefix filter ignored -> red; fons passed undeclared -> red
(the title case dies on expansion instead). Fumus XXVII on a real disk
through bin/fabrica (2 instances RECENS, alia.c not, new file ->
STALUM); plant (tool passes NIHIL sutura) -> rc 2, red.

## 2026-10-02 - plan 2 T5: the toml pilot (thin runner + oracle)

Scope (Fran): thin runner + oracle only; no toml familia - nothing can
judge a test binary honestly today (relatio needs -provenientia,
regeneratio refuses binaries). familia's first real consumer will be
per-test verdict actions, designed later.

Runner (toml/compile_probationes.sh): the hand lists (24 lib, 13
materia, toml/fontes, helpers) and the coarse "newest header anywhere
makes every object stale" block are gone. Per test: closure from
`bin/aedilis --enumerare --thesaurus`; the union is compiled through
bin/compilator; each test file is compiled through it too
(toml/build/probationes/, new - the old runner compiled tests inline at
link time) and linked against its OWN closure. Lock, tee log, mensor
metrics and reporting unchanged. Sources are passed by ABSOLUTE path
from the caller's cwd exactly like the old runner: -g records both, so
relative paths would have broken byte identity with the old objects.
The excubitor post-build check was removed: it judges by mtime, and
compilator deliberately leaves an identical object untouched (older
mtime than its source) - it would have refused up-to-date objects.

Oracle (tools/toml_oraculum.sh): both runners in the SAME tree, one
after the other, each cold (the plan's scratch worktree could never
match bytes: -g embeds the working directory). -arbor REF runs both in
a worktree at REF (the live runner copied in, live bin/aedilis and
bin/compilator linked). Objects only in the new runner fail; objects
only in the old = hand-list slack (iter_directoria.o, via.o: never in a
closure). A silent oracle is a dead oracle: no runner, 0 common objects
or 0 test lines -> exit 2 (first version said "consensus" on e1e01751,
where toml did not exist yet).
FINDING: Apple clang 16 is non-deterministic on toml/fontes/
toml_scalaris.c with the house flags (4 distinct objects in 10 plain
compiles; -fno-vectorize fixes it; only file among the 45). The oracle
re-compiles a discordant object's source 5x with plain clang and names
compiler non-determinism instead of failing. Park ...ACYVJ for Fran.
Results: consensus on HEAD and three tree states (cadca60e toml,
8d7e6fba materia, 26e34570 include/): 44-45 common objects identical,
370 test lines identical. Plants: chorda.c at -O1 -> byte DISCORDIA;
one test skipped -> output DISCORDIA (first plant named a test that
does not exist - a no-op, caught because it stayed green).

Measurements (wall, whole suite; the 13 test executables alone take
27.9 s of every run):
  warm:                         old 31.8 s   new 31.1 s
  cold build, empty store:      old 39.2 s   new 42.0 s
  cold build, warm store:       old 39.2 s   new 31.5 s
  one header comment (7 users): old 38.0 s (all 47 objects) new 31.2 s
First new-runner measurement was SLOWER (warm 38.6 s): the 13 aedilis
calls ran without --thesaurus (6.8 s vs 0.26 s warm). One store for
both tools (FABRICA_THESAURUS) fixed it.

## 2026-10-03 - plan 2 T6: parallel heal

Shape: the core stays pure. New seam member `agere_simul` (NIHIL = the
old serial path, untouched; FABRICA_FILA=1 gives exactly that). Core
(`_sanare_undatim`): waves from fabrica_undas_formare (no dependency,
disjoint envelopes, no shared areas); per wave the existing pre-logic
(`_ante_agere`, extracted unchanged from the old loop), unsafe actions
ALONE with their own snapshot, safe ones (lectiones="verum") as one
batch (`_undam_agere`), results recorded in title order. Tool
(`_agere_simul`): up to FABRICA_FILA children (default
hw.perflevel0.physicalcpu = 4 here) via processus_incipere/pulsare/
metere, each with its own ABSOLUTE FABRICA_LECTIONES set just before
starting; after a failure running children finish and nothing new
starts. _agere split into _mandatum_parare + _actum_complere, shared.

Ruling - TWO write checks for a batch (the plan's "S trace" alone would
have checked LESS than today: bash generators' own `>` writes are never
in the ledger): (1) per member, recorded S writes vs its envelope -
attributed failure; (2) one snapshot around the batch - writes outside
the union of envelopes AND recorded by no member fail every member
("scriptor ignotus"). A recorded write is excluded from (2) - first
version blamed the innocent member too (test III).
Found while testing: each member's own ledger file was flagged by the
wave snapshot - per-action ledgers are now part of every action's own
envelope. Found on the REAL tree (not by any test): since T3 every
in-place snippet heal writes the store (aedilis --thesaurus), which no
snippet declares - all 19 FRACTUM. T3 only exercised judging (scratch).
The store's three subdirectories are now involucrum (fabrica-owned,
atomic) like acta and fabrica.db - the root's .o files stay real
envelope business. (Trailing-slash trap: an ARBOR locus adds '/' itself;
the first fix matched nothing and the new test stayed red - good.)
Policy after a failure in parallel mode: nothing new STARTS, but every
remaining action is still judged - up to date stays silent, a broken
dependency is named (fumus XVIII caught the generic cause winning), only
actions that would have run are OMISSUM "post fracturam".

Evidence: core tests I-IV + IIIb (3 safe + 1 unsafe + dependent; failure
in a batch; S outside envelope; store write; unrecorded write), plants
red (all actions "safe", no wave check, no attribution filter, no store
involucrum). Fumus XXVIII (4 x 2 s -> 2.15 s with FILA=4), XXIX (one of
four fails: three SANATUM, dependent OMISSUM naming it, exit 1), XXX
(unsafe action's interval overlaps none); plants: tool never wires
agere_simul -> 8.2 s red; unsafe treated as safe -> overlap red.
Real tree, heal of the 19 snippet actions (one output each corrupted):
serial 199.5 s, parallel 109.5 s. The actions parallelize (batch bound
by fragmentum_compile_tests ~36 s); what remains is the judge's
regenerations before and after each action, still serial (~70 s) -
parallel judging is the next lever, not in T6. Remaining failures in
those runs were other processes writing in the tree during the heal
(oratio/build/cursor.sera/, tabularium.db-wal) - reported correctly as
"aut manu mutata dum currebat".

## 2026-10-03 - plan 2 T7 step 1: excubitor vs the store (record)

Method (scratch scripts, not committed): per tree state a worktree with
the T5 thin runner (live bin/aedilis + bin/compilator linked, own store).
compilator's misses counted by a `clang` wrapper on PATH that logs only
real `-c` compiles and passes -print-prog-name through - compilator's
compiler identity, hence every key, is unchanged. excubitor asked
BEFORE each compile pass; objects re-touched between scenarios (the old
runner's mtime baseline). Header: toml/fontes/toml_scalaris.h.

Same result on all four states (HEAD, cadca60e, 8d7e6fba, 26e34570):
  A nothing changed:           excubitor 0, compilator 0
  B header touched, same bytes: excubitor 4 (false alarm), compilator 0
  C header bytes changed:      excubitor 4, compilator 4 (agree)
  D same-named header earlier on -I (copy of include/chorda.h into
    toml/fontes): excubitor 0 (MISSES IT), compilator 44-45 (all)
Also seen live after T5: compilator leaves identical objects untouched,
so excubitor called 3 up-to-date objects stale (B class).
First C run reported excubitor 0 - my script edited the header in the
same second as the last object touch, and excubitor compares with -nt
at 1 s resolution (the same-second trap the runners document). Re-run
with the sleep before the edit: 4/4. Real in practice, but not fair to
count as excubitor's logic.

Finding for Step 2: excubitor is NOT only the report-only call in
compile_tests.sh. ~18 runners call it (crusta x3, css, html, md, oratio
x2 and a crusta fixture BLOCK on it with exit 2; gesta, officina x3,
saltuarius, silva, tessera, diagnostica fumus warn), plus the
.claude/hooks/excubitor-custos.sh hook. Those runners still build by
mtime - excubitor is their real guard. Deleting it before they compile
through the store would remove that guard (a missing script degrades to
a CAUTIO line).
Step 2 (Fran 2026-10-03): migrate first, then delete. Inventory
'consumptores excubitoris' (…MEES9X, 21 rows: usus obstat/monet,
migratum) and batch job 'migratio ad compilatorem (excubitor emeritus)'
(…W0ZBW, 20 open rows, recipe = the T5 toml conversion + its oracle).
Deleting excubitor.sh / fabrica.tsv / compile_tests.sh:1225 / the hook
is the job's last row, not T7's. Spec 2 IX 'excubitor deleted' moves
with it.

## 2026-10-03 - T6b (follow-up): parallel judging (prefetch)

Desideratum …83AQ items 1+2 (Fran). The judge stays unchanged: a prefetch
warms the per-run regeneration memo it already consults.
- Collect mode: sutura->praevisio (Xar) set -> _regenerare records the
  action (once per title, lectiones="verum" only) and returns "praevisio"
  WITHOUT running or memoizing. fabrica_regenerationes_praevidere judges
  through a COPY of the sutura with the collector set (memo tables are
  shared pointers - no const cast), sorts the requests by title, runs
  them through the new seam currere_simul (same scratch dir + ledger
  paths the judge would use), then fills sutura->regenerationes and
  writes the IUDICIUM cursus rows exactly as _regenerare would.
- Tool: _currere_simul = T6 pool (FABRICA_FILA), each child an emptied
  scratch dir, absolute FABRICA_SCRIPTURA and FABRICA_LECTIONES set just
  before starting; judging never stops on a failure. iudicare -plenus
  prefetches the lectiones actions it will judge (same tacta/electa
  filter); sanare prefetches each wave's pre-checks and each batch's
  post-checks.
- sanare wave loop now judges ALL members before any acts (an act clears
  the regeneration memo, so an unsafe member acting mid-loop would have
  thrown the prefetch away). Wave members are independent by
  construction, so the order cannot change a verdict.
Same safety class as T6: only lectiones actions are prefetched -
arbitrary generators may share temp paths.
Evidence: core tests (prefetch batches only lectiones, in title order;
the judge then finds them memoized - one serial regeneration for the
non-lectiones action - verdicts as without prefetch; no seam = no-op;
heal with prefetch: two regeneration batches, 6 runs, none serial),
plants red (collect everything; memo not filled). Fumus XXXI: 4
generators sleeping 2 s under the judge -> 2.04 s with FILA=4; plant
(prefetch unwired) 8.07 s red.
Real tree: -plenus with all 19 snippet traces deleted: serial 112.6 s,
parallel 96.2 s, identical verdicts. Heal of the 19 snippets: serial
93.0 s, parallel 65.7 s, 19 healed. The judge is now dominated by
NON-traced generators regenerating serially: corpus_silicis 19 s,
excludenda_silva 17 s, lectores_cocti 4.5, tabulae_silvae 3.3,
canones_cocti 1.6, numeri_latinae 1.2 (~47 s). Converting them to
lectiones="verum" (the T2 move) would let traces skip them and make them
prefetchable - next family candidate.

## 2026-10-03 - plan 2 T8: slice 2 closed

"As built" is spec 2 par. XIII (done-means vs evidence, built beyond the
plan, found on the way, known limits with ledger ids). Ledger note
…NKVA closes the slice; the progress ledger
(.superpowers/sdd/fabrica-plan-2/progress.md) carries every ruling.
Open for Fran: excubitor deletion via batch job …W0ZBW; the store's
lint exemption; promotion (…XZQG); clang non-determinism (…ACYVJ).
Next family candidate: the six non-traced generators that now dominate
-plenus (…VFF5D).

## 2026-10-03 - excubitor migration, middle path step 1: shared runner pieces

Fran chose the middle path: shared pieces, then three representative
runners (css, crusta, root compile_tests.sh), then slice 3's interview.
- tools/cursor_communis.sh (sourced): cursor_instrumenta_parare (aedilis +
  compilator, ONE store for both via FABRICA_THESAURUS),
  cursor_clausuras_derivare <dir>, cursor_fontes_compilare (union through
  bin/compilator; duplicate basenames refused - flat build dir),
  cursor_probationem_struere <test> <bin> (test through the store, linked
  against its own closure). Every compile command is recorded in
  <build>/clausurae/mandata.tsv.
- tools/cursoris_oraculum.sh <sub>/compile_probationes.sh [-ref|-arbor]
  replaces tools/toml_oraculum.sh: subsystem from the runner's path,
  output-capture variable <SUB>_PROBATIONES_EFFUSIO, and the clang
  non-determinism re-check re-runs the RECORDED command (no per-runner
  include flags hardcoded). Checked: toml_scalaris.o's recorded command
  gives 6 distinct objects in 8 plain compiles, chorda.o's gives 1.
- toml runner rebased onto the helper: generic oracle vs HEAD's T5 runner
  = consensus (45 objects identical, 370 lines).
Batch job …W0ZBW rubric v2 points at these.

## 2026-10-03 - excubitor migration step 2: css

css/compile_probationes.sh onto tools/cursor_communis.sh (hand lists,
newest-header rule and its BLOCKING excubitor call removed). The oracle's
first run caught a real gap: probatio_css_adaptare failed to link
(css_ligator_solvere) - aedilis resolves includes through aedilis.stml's
<inclusa> roots, not the runner's -I flags, and css/fontes was not a
root, so css_adaptare.c / css_lexicon.c never entered the closure. The
old runner hid it by linking every css source into every test. Added
`css/fontes` LAST in aedilis.stml (lowest priority: cannot shadow;
names css_*.h unique). Every materia client will need its own line
(toml/fontes was already there). Commit judge: the 43 committed
generated artifacts touched stay RECENS (no snippet changes).
Oracle: consensus, 33 objects identical, 299 test lines; hand-list slack
excerptum, materia_pictor, runae, runae_tabulae, utf8 (never needed).

## 2026-10-03 - excubitor migration step 3: crusta (runner + two launchers)

Runner: same conversion as css; `crusta/fontes` added LAST to
aedilis.stml (same gap as css). Oracle: consensus, 45 objects identical,
479 test lines; slack processus_posix.o.
facies.sh / oraculum.sh were not runners: they linked EVERY object in
crusta/build (built by the runner) with their main source and refused
when excubitor called those objects stale - "run the runner first". After
migration crusta/build holds only the test closures, so they now BUILD
THEMSELVES: new helper cursor_instrumentum_struere <main.c> <bin> (own
closure from aedilis, objects through compilator in
build/instrumenta/<name>/ - their -I lists differ from the runner's -,
relink only when an object changed). Output vs the old launchers:
oraculum -probare identical (IDEM on both goldens); facies on 5 scripts
identical except one finding the change removed (below).
House lint caught my helper: crusta's nt-aequalitas rule flagged
`[ "$bin" -nt "$o" ] || recens=0` (blocks at commit). Rewritten in the
negated form the house uses (`! [ bin -nt o ]` -> stale, so a same-second
tie relinks). Lint over all 8 scripts this migration touched: clean.
Helper: the mandata.tsv reset moved from cursor_instrumenta_parare to
cursor_clausuras_derivare (a tool launch must not wipe a runner's record).

## 2026-10-03 - excubitor migration step 4: root compile_tests.sh

Scope (Fran): compile through the store only. Kept: the generated source
list (compile_tests_fontes_generata.sh) and link-everything-into-every-
test; GUI apps, tools, speculum untouched. Gone: needs_compile /
newest-header mtime rules, the "Libraries up to date" short-circuit, the
report-only excubitor call at exit. compile_libraries now writes one
command line per object (C, .m, vendor sqlite with VENDOR_FLAGS) and runs
them with `xargs -P $FILA -L 1 bin/compilator`; per-test objects (serial
and probatio_una) go through compilator too (COMPILATOR, CLAUSURAE_DIR
exported to xargs children). Paths stay RELATIVE (cwd = root) so -g
embeds the same strings as the old runner. LIBS_COMPILED (mensor metric
"recompiled this run") = inode listing of build/*.o before vs after -
compilator never rewrites an identical object.

Timing, warm, filter piscina: old 7.4 s; new serial 14.1 s (225 hits x
~29 ms); new parallel 8.5 s. Half of each hit is compilator spawning
`clang -print-prog-name=clang` every call (desideratum ...HHD43). First
cold-store run: 225 objects, only 1 came out with different bytes from
the old runner's existing objects.

Oracle root mode (tools/cursoris_oraculum.sh compile_tests.sh [-filtrum
X]): deletes only build/*.o + build/probationes/*.o (never build/ - it
holds logs, locks, other tools' binaries), compares both sets. Three
oracle bugs found on the way to consensus:
1. awk died with "towc: multibyte conversion failure" on non-UTF-8 test
   output (~700 lines unfiltered at the tail) -> LC_ALL=C.
2. root tests print run-specific data: ASLR addresses, ports, pids,
   UUIDs, multipart boundaries, ms timings, callback counts. Regexing
   them is whack-a-mole (three runs, new class each time), so root mode
   compares VERDICT lines (one per test + summary counts) and only
   counts full-output differences as a note (243 of 107946 lines).
3. sub-runner filter unchanged except colour stripping + LC_ALL=C.
Result: consensus, 431 objects identical (toml_scalaris = known clang
nondeterminism), 196 verdict lines. Plant: new runner with sqlite -O1
-> "DISCORDIA octeti: sqlite3.o", rc 1 (also exercised -filtrum).
Library compile failure exits 2 ("nothing ran"), same as the old runner.
Inventory: tools/cursor_communis.sh was owed by NO suite (only vexilla);
added to tegit viae of radix, toml, css, crusta; radix currit binaria
+= tools/compilator.c.

## 2026-10-03 - slice 3 T1 spike: the toml gate's trace

Ran toml/compile_probationes.sh twice with FABRICA_LECTIONES (warm
store, 13/13). Full classification in spec 3 §XI. The non-obvious parts:
- compilator's "don't rewrite identical object" check READS the
  destination through filum -> 58 L entries on toml/build/*.o with no S.
  Harmless today, fatal under spec 3's "unowned build input" rule
  (every warm run IGNOTUM). Fix: record the destination as S.
- Raw IO hides exactly where you'd expect it to hurt: computus's GOLD
  file (basis.tsv) and the registrum grammar read by materia_coctor's
  staleness check. Both would let a gold/grammar edit reuse a stale pass.
  materia can't take filum (client chains), so it notes the read itself.
- The trace is stable across warm runs once the store is ejected: 424
  differences, all store paths (miss then hit).
- Normalizing paths: the ledger writes absolute paths for some tools
  (repo root and $HOME prefixes); classify with awk index(), not sed
  with a literal tab (zsh ate it the first time and the groups were
  wrong).
- 4,192 entries outside the store; 3,025 files / 18.1 MB to re-digest,
  0.42 s - the < 2 s RECENS target is fine.

## 2026-10-03 - slice 3 T2: crusta/fontationes.sh

Script-input derivation for verdict keys landed in crusta (library
crusta_fontationes.{h,c}, tool + launcher, test with 36 assertions,
plants A/B/C red). On the toml runner it finds exactly the spike's
expectation (sera, vexilla, cursor_communis, mensor_suitae,
tomllib_aurum) plus the conditional builder chain; instrumenta
bin/aedilis, bin/compilator, bin/mensor (+ bin/fabrica via builders);
productum toml/build/*. Details and design notes:
crusta/fontes/crusta_fontationes.worklog.md. Name per A2 would have been
bin/fontationes; it is a self-building launcher like facies.sh instead
(no installata entry needed until T5 decides how fabrica calls it).

## 2026-10-03 - slice 3 T3: toml's reads made visible

Spike gaps closed, verified by a re-trace of the toml gate:
- E records 0 -> 5 (RHUBARB_RADIX, HOME, COMPUTUS_SCRIBERE,
  ORACULUM_OMNIA, ORACULUM_EXEMPLUM): 13 getenv -> lectiones_ambitus in 9
  toml test files.
- computus gold (basis.tsv) now L: corpus + gold through filum (read via a
  small fgets-like line walker over the filum buffer; the COMPUTUS_SCRIBERE
  write builds the text and filum_scribere_literis). Gold numbers
  unchanged (13/13).
- totalitas failure-file write through filum (S), const dropped with the
  house union idiom (lib/vitrea_servus.c:123), not a pointer->int cast.
- materia_coctor's raw read notes itself (L / A) and carries the
  existing `/* lectiones: notatur */` marker the lint already honours -
  no lint rule change needed (the spec's "same function" idea was
  unnecessary). That pulls lectiones.c into every chain linking
  materia_coctor: html, md, oratio hand lists gained "lectiones" (the
  diagnostica lesson from T2 again - hand lists are where new deps bite).
  The first T3 commit attempt found a FOURTH: materia/compile_probationes.sh
  globs materia/fontes/*.c against its own lib hand list (14 link
  failures). Audit of every build compiling materia sources by glob/list:
  that was the last one (silva's substrate excludes the coctor by design).
  The frozen computus fixture html_cursor_2026-09-23.sh is NOT edited.
- compilator: identical-destination hit now records S (miss path was
  already S via filum_movere). Re-trace: 58 objects, 0 L-without-S.
- mensor_suitae.sh: every bin/mensor call runs with `FABRICA_LECTIONES=`
  (empty = ledger closed). Re-trace: no mensurae.volumen entry. My first
  sed also rewrote the three `[ -n "$MSU_MENSOR" ]` guards into
  `[ -n FABRICA_LECTIONES= "$MSU_MENSOR" ]` - a syntax error the `||`
  would have swallowed silently; caught in the diff, restored.
- lectiones_lint pilot = tools/aedilis.c + every toml test main (67
  files); plant (raw getenv in probatio_toml_api.c) -> OBSTAT.
Left for T4: build/toml_corpus.lst and aurum_silvestre.txt (bash-written).
Left for T5 (spec 3 IX.7): compilator's own env (FABRICA_CLANG) is not
in the trace; compilator is not in the lint pilot because its
THESAURUS_GENERATIO read varies per run (store bookkeeping).

## 2026-10-03 - rebake after T3 found a step-4 regression

`bin/fabrica sanare installata` failed: obiecta_radicis FRACTUM, "scripsit
extra vestigium: build/clausurae/bibliothecae.vocationes,
build/clausurae/mandata.tsv". Since 35a1589d (root runner on the shared
cursor) `compile_tests.sh --obiecta` writes its compile records there,
and the action's declared footprint did not include them. Fixed by
`<vestigium via="build/clausurae"/>` on obiecta_radicis. Nothing caught it
for two commits because NO gate realizes obiecta_radicis under its
footprint check - only a rebake does (the radix gate runs the runner
outside fabrica). Lesson for runner migrations: after changing what a
fabrica-run command writes, run `bin/fabrica sanare installata` before
committing, not after.

## 2026-10-03 - slice 3 T4: toml's bash-written inputs become actions

New subsystem toml/aedificatio.stml (registered in fabrica.stml):
- `toml_corpus` -> build/toml_corpus.lst via toml/corpus_indicem.sh
  (git ls-files; byte-identical to the runner's old output). Regeneratio,
  not memoria: the git index is not a declarable input.
- `toml_aurum_silvestre` -> toml/build/aurum_silvestre.txt via
  `toml/tomllib_aurum.sh -silvestre` (python3 declared as instrumentum,
  as entitates_html). Found on the way: the header embedded
  `$(date -u +%Y-%m-%d)`, so the artifact changed EVERY DAY - a verdict
  keyed on it would have re-run daily. Silvestre header is now date-free
  (deterministic: scratch regeneration cmp-equal to the tree copy); the
  committed aurum.txt keeps its date (separate path, untouched).
  Regeneration costs ~0.9 s per judgement (counts against the < 2 s RECENS
  target - measure in T9).
Both honour FABRICA_SCRIPTURA. Plants: appending a line to either
artifact -> -plenus STALUM; sanare heals; RECENS again.
Runner: the mtime rule ("manifest newer than gold") and the bash
`git ls-files` are gone; by hand it calls `bin/fabrica sanare <both>`,
under FABRICA_LECTIONES it does nothing (nested sanare would meet
fabrica's own lock), and without bin/fabrica (shadow clone) or on any
sanare failure it runs the two scripts directly. Fallback tested by
moving bin/fabrica aside with both artifacts deleted: corpus and
differentia green. Full toml 13/13, 34.7 s (unchanged).
Inventory: toml `currit binaria` += tools/fabrica.c, `tegit viae` +=
toml/aedificatio.stml.

## 2026-10-03 - slice 3 T5a: the iudicium kind

Design written first (spec 3 §XII, three v3 corrections: preconditions are
ingressus - only strategy-ignota actions are realized as praecondicio;
E records make _lectiones_colligere return FALSUM today; absolute paths
are silently skipped today).
Landed: FABRICA_ACTIO_IUDICIUM (`genus="iudicium"`); parse refusals
(iudicium without lectiones="verum"; verdictum exitus outside an
iudicium; iudicium exitus that is not verdictum); strategy `verdictum`
(judge only - verdict file absent STALUM, no trace IGNOTUM, a differing
trace entry STALUM naming the path, all congruent RECENS; never calls
_regenerare, so praevisio can't collect it either); exclusion from
`sanare` without arguments (lib) and from the no-argument judge sweep +
praevisio (tool); never in a parallel wave (tuta = lectiones AND not
iudicium). Tests: 5 new blocks in probatio_fabrica (463/463); plant
"remove the sanare exclusion" -> 3 red.
Note for T5b: the named sanare of an iudicium today ends FRACTUM
("exitus 0 sed non RECENS") because nothing records its trace yet - the
test asserts only that it ACTED.
Test gotcha: a declaration with no <ingressus> is refused first
("actio sine ingressu") and masked my three refusal assertions.

## 2026-10-03 - slice 3 T5b: recording a pass, and the verdict trace rules

- `sanare` of an iudicium: the tool's sequential `_agere` sets
  FABRICA_LECTIONES to the ABSOLUTE ledger path (runners `cd`), truncated
  first; `fabrica_liber_via()` is the one name both sides use.
  `_post_agere` (iudicium branch): verdict file present -> key from the
  inputs as they are AFTER the run (per-run memos are cleared after
  agere), `_lectiones_transitus_colligere`, write, then re-judge for
  consensus (must be RECENS, else FRACTUM). Not recordable -> SANATUM
  with "transitus non servatus: <why>" AND the old trace is deleted (an
  empty write; the store's DELETE removes it): this run read something
  the old trace does not explain, so the old pass is no witness. Plant
  (skip the deletion) -> red.
- Collector rules: S paths are outputs (dropped), build/ reads need an
  owner from `exitus_noti` (all declared exitus), system roots dropped,
  other absolute paths digested (A1), species ALIA -> IGNOTUM, E kept only
  when equal to the env fabrica gave the gate (plant "keep all E" -> 8
  red: a runner-set RHUBARB_RADIX would key on itself).
- Genera `identitas_clang` (same identity as compilator incl.
  FABRICA_CLANG) and `fontationes` (crusta/fontationes.sh; irresolutum ->
  key IGNOTUM with the line).
- Tool: `_legere` now refuses non-regular files (a FIFO would hang the
  judge); trace store maps 'E' <-> LECTIO_AMBITUS (it mapped unknown
  letters to L).
- Test fake parity: the fake store returned an EMPTY trace as found; the
  real one returns nothing for zero rows. Fixed in the fake.
- Test gotcha: per-run memos (sutura->digesta) keep the input key inside
  one process - the block runs without memos so judgements follow edits.
488/488.

## 2026-10-03 - slice 3 T5c: porta_toml, first real reuse

`python3 -B pythonica/silva.py -iudicium toml` (verdict written only on a
pass, deleted first; -B so importing silva.py never writes __pycache__,
which the whole-tree snapshot would call "outside the footprint");
`porta_toml` in toml/aedificatio.stml (footprint toml/build,
build/test_logs/toml.log, build/portae/tempora.tsv - porta() records its
timing; inputs silva.py, fontationes of the runner, bin/aedilis,
bin/compilator, python3, identitas_clang, the two T4 artifacts).
First `bin/fabrica sanare build/fabrica/verdicta/toml.txt`: SANATUM 33 s,
verdict "toml: TOML PROBATIONES: 13/13". Then `iudicare` RECENS from the
trace in 1.9 s (no test run). Stored trace: L 2807 / X 108 / A 835 / D 5
/ E 4 - exactly T1's accounting (3025 L - 58 owned objects - 162 SDK + 2
newly visible: computus gold, registrum grammar; E = HOME + three absent
switches, RHUBARB_RADIX dropped as runner-set).
Live plants (each reverted): README edit -> RECENS; comment in
toml/fontes/toml_lector.c -> STALUM "lectio transitus mutata:
toml/fontes/toml_lector.c"; comment in tools/cursor_communis.sh ->
IGNOTUM (key changed, via fontationes); ORACULUM_OMNIA=1 in the judge's
env -> STALUM naming it. Sweep without arguments never lists it.
First T5c commit attempt: the pythonica gate went red on a test that
predates T5c - 'tegit fontes: crusta/fontes/*.c -> crusta (aedilis closure
does not see it)'. Since the crusta migration (8a84c35c) crusta/fontes IS
an aedilis root, so crusta is owed through a closure; pythonica had not run
since. The lens assertion now uses html/fontes (not a root; html rows
added to the test's fixture inventory) and a second assertion pins the new
crusta cause ('in clausura').

## 2026-10-03 - slice 3 v4: house binaries keyed by provenance

The pass recorded in T5c went IGNOTUM right after its own commit: one of
the commit's gates relinked bin/aedilis (same sources, new bytes). Fix:
genus `instrumentum_domus` (digest of the `ingressus` line of
`-provenientia`, never the `commissum` line; bytes if no report),
fontationes' instrumentum lines likewise, and the verdict trace drops
paths that are declared inputs (aedilis L-reads its own binary - the
first fix alone still went STALUM "lectio transitus mutata: bin/aedilis").
Measured: record, `rm bin/aedilis && tools/aedilis_struere.sh` (new
sha), judge RECENS; same with bin/compilator. Test: 496/496 incl. a
block proving commissum-line and relink invariance, ingressus-line
sensitivity, bytes fallback.

## 2026-10-03 - slice 3 T6: porta() and commissio ask fabrica first

`porta(nomen)` (no filter, live tree, gate has a declared `porta_<nomen>`
iudicium action, bin/fabrica present) goes through fabrica:
`iudicare -omnia <verdict>` RECENS -> returns the recorded pass (compendium
"... [transitus servatus, ante N]"), nothing run, a tempora row
"<nomen> (transitus)"; otherwise `sanare <verdict>` runs the gate under the
ledger and records it, and porta() rebuilds its Porta from
build/fabrica/acta/porta_<nomen>.log (the -iudicium mode now prints the
gate's full output so failures keep their fractures). `vis=True` deletes
the verdict first (otherwise sanare sees RECENS and runs nothing - my first
version reported that as a failure). sanare exit 2 (lock held) -> the old
path. Filtered calls, shadows and gates without an action -> the old path
(`_porta_cruda`, also what -iudicium calls: no recursion).
Which gates are verdicts: `_portae_verdictorum()` reads the declaration
TEXT of fabrica.stml's subsystems (regex on
`<actio titulus="porta_X" genus="iudicium"`), memoized per process - so
non-verdict gates never pay a bin/fabrica call (and the pythonica tests,
which fake FABRICA_BIN globally, stay hermetic).
commissio prints "porta X: ... - non iterum cursa (digestum idem)".
Real: porta('toml') first 56.9 s (records), second 2.3 s RECENS; vis=True
41.6 s then 1.8 s; filtered = old path. Plant (a failing toml assertion):
sana False, rc 1, 12/13, the fracture listed with file:line, verdict
removed. Hermetic tests: 6 cases (RECENS, STALUM->sanare, broken, lock,
filter, real-declaration lookup); BSD printf exits 1 on an extra argument
with no format directive - the fake gate uses echo.

## 2026-10-03 - slice 3 T6 follow-up: two ledger bugs the first commit exposed

After the T6 commit, `porta('toml')` ran but reported "transitus non
servatus: ingressus build/ sine domino:
toml/build/probationes/probatio_toml_api.o.compilator.47033.o" - three runs
in a row, same pid. Two bugs:
1. **The verdict ledger was never truncated.** `_agere` "truncated" it with
   `filum_scribere(path, chorda_ex_literis(""))` - an empty chorda has datum
   NIHIL and filum_scribere refuses it before opening the file. The ledger
   had grown to 65 MB / 1,010,944 lines over the session; every recorded
   trace was the UNION of all past runs (stricter, but carrying stale
   entries like a dead temp file). Now deleted before the run, as the
   generator paths already did. One run = 84,200 lines.
2. **filum_delere recorded nothing**, though lectiones.h documents S as
   "scripta, deleta, mota" (filum_arborem_delere and filum_movere did).
   compilator's miss path writes a temp object with clang (invisible),
   reads it through filum (L) and deletes it - an L with no S. Fixed in
   lib/filum.c; probatio_lectiones pins it (file created with the ledger
   off, deleted with it on); plant (remove the record) -> red.
Verified: forced a compile miss (comment in a toml test) -> pass recorded,
the temp object shows L + S. Also: the inner raw run inside -iudicium no
longer writes its own tempora row (the outer porta() does) - the T6
commit had logged toml twice (34.9 s inner, 47.9 s outer).

## 2026-10-03 - slice 3 T7a: audit, dependency realization, fontationes seam

- Audit: `sanare -audit` / FABRICA_AUDITUS=N (1 = all, N = one in N by key
  byte). A RECENS iudicium is marked SANANDI_AUDITUM and runs anyway; its
  old trace is kept in sutura->audita first (the failing run deletes the
  verdict, so it could not be looked up afterwards). Pass -> re-recorded,
  causa "auditus: transitus iterum congruit". Fail ->
  FABRICA_AUDITUM_DISCORS with the reads the failing run had that the
  stored trace lacks. Caught on the way: the stale-trace deletion keyed on
  "ratio non-empty", and the new audit note is non-empty on SUCCESS - it
  would have deleted the trace it had just written; now an explicit
  `servatum` flag. Plant (audit never selected) -> 3 red. Real: porta('toml')
  under FABRICA_AUDITUS=1 ran 45 s and agreed.
- porta() and dependencies (spec v5): named `iudicare` doesn't realize the
  verdict's build/ inputs (stale corpus list would still match). Always
  calling `sanare <verdict>` fixed it but cost 13.2 s per reuse (whole
  closure incl. committed-file generators). Now: sanare the declared build/
  inputs, then iudicare, then sanare only if not RECENS - 3.6 s reuse.
- `FABRICA_FONTATIONES` overrides the fontationes tool path, and the call
  passes `-radix <cwd>` (the launcher's own -radix comes first; the tool
  keeps the last) - so temporary roots resolve their own scripts.
- Plant gotcha (again): a python anchor or a sed address that matches
  nothing is a silent no-op plant - the first audit plant "passed" with 0
  red because nothing was replaced. Check the replacement count before
  reading the result.

## 2026-10-03 - slice 3 T7b: iudicium-fumus

tools/iudicium_fumus.sh (in PORTAE, ~26 s): a temporary root with one
generator (gen -> build/gen.h from gen/fons.txt) and one verdict porta_x
whose gate sources lib.sh, compiles src/a.c through the REAL
bin/compilator (a filum reader: a real trace), runs it, and reads
flag.txt with bash (outside the ledger, for the audit). 14 checks: I
recorded + RECENS, P3 README -> RECENS, P1 header -> STALUM naming it, P2
sourced script -> IGNOTUM, P7 `source "$NESCIO"` -> IGNOTUM naming the
fontatio, P8 generator input -> build/gen.h regenerated + pass not reusable
(IGNOTUM: gen.h is a DECLARED input, so the key moves - my first assertion
expected STALUM), P6 failing gate never cached, AUD blind RECENS ->
`sanare -audit` AUDITUM_DISCORS (rc 1) -> restored "auditus congruit".
P4/P5/P9/P10 are named as covered elsewhere (lint gate, probatio_fabrica).
**The gate found a crash the unit tests could not:** `sanare -audit` with a
discord aborted (SIGABRT, stack protector) - the tool counts sanatio
events in `numeri[VI]`, and FABRICA_AUDITUM_DISCORS is index VI (until T7 it
was only ever a cursus event, never a sanatio). Array now sized from the
enum; a discord counts as a failure in the exit code and is named in the
summary line. probatio_fabrica XI passed throughout because it calls the
library, not the tool's summary.
Plants: the first (an early `redde VERUM;` in _fontationes_sigillare) did
not COMPILE under -Werror, so the old bin/fabrica ran and the gate said
"sanum" - a no-op plant; the compiling version (conditional return) -> P2,
P7 red.

## 2026-10-03 - slice 3 T9: measured, closed

Ten rounds of "unrelated edit -> porta('toml') -> FABRICA_AUDITUS=1
porta('toml')": rounds 2-10 reused (3.5-3.9 s) and the audit agreed (42.5-
46.8 s); with the T7a audit, 10 audited reuses, 0 discord. Round 1 was
FRACTUM "scripsit extra vestigium: gesta/annales/entities/..." - I had
filed two ledger entries while its gate ran. The tree snapshot now skips
the records office's places: gesta/annales/ and, at the root,
tabularium.db* / forum.db* (the first fix covered only gesta/annales and
the next live run failed on tabularium.db-wal - list the resident's
files, don't guess). Verified: porta('toml', vis=True) with a ledger note
written mid-run -> SANATUM. `iudicare -plenus` 15.0 s (165 RECENS, verdict
never in the sweep). T8 parked (…2VP7), after the gate-migration
desideratum (…J6HF). Spec 3 §XIV As built.

## 2026-10-05 - sampled audit at commit time

commissio now audits a reused verdict pass one time in N (default X,
`FABRICA_AUDITUS_COMMISSIONIS=N`, 0 = off): `_auditum_commissionis(nomen)`
rolls per CALL and `porta(nomen, auditus=True)` then runs `sanare -audit`
(iudicare skipped); commissio prints "porta X: ... - auditus (I ex N)".
Why not fabrica's own `FABRICA_AUDITUS=N`: that samples by key byte - a
pass whose inputs stay put for many commits would be audited always or
never, and the long-lived pass is the one most worth spot-checking.
`silva._alea` is injectable; 5 hermetic tests (dice below/above 1/N, N=0,
non-verdict gate never, auditus=True -> `sanare -audit` and no iudicare,
via a fake bin/fabrica logging argv). Plant (never audit) -> red.
Session note: a crash left a second copy of this conversation running in
tmux; both ran pythonica against the same build/ (spurious
'recepta sua deleta' and a clone `cp` failure) and shared one scratchpad.
Nothing committed or doubled; one copy kept, clean rerun green.

## 2026-10-05 - aedificatio.canon caught up with slice 3 (park …9XNXY)

Found while reading the house canons for effectus T1: slice 3 added
action kind `iudicium`, ingressus genera `fontationes`,
`instrumentum_domus`, `identitas_clang` and provenance `verdictum`
to the READER (lib/fabrica.c) but never to `aedificatio.canon` -
`toml/aedificatio.stml` carried 6 canon vitia. The inventory said
fabrica-fumus and the fabrica oracle "cover" aedificatio.canon, yet
neither judged any declaration against it: an owed gate that could
not fail. Also pre-existing since plan 2 T4: `familia via genus="via"`
(not a canon value type) - the canon itself failed canon.canon.

Now `tools/fabrica_fumus.sh` XXXII runs `bin/canon_examen` over every
`<sub>/aedificatio.stml` fabrica.stml lists AND over aedificatio.canon
itself. Born red on the 6 real vitia (then on the familia attribute);
plant: an attribute the reader silently ACCEPTS (`nescio="x"` on an
ingressus) turns XXXII alone red - the reader tolerating it is the
point of having the canon. Lesson: when the reader learns a value,
the canon must learn it in the same commit; XXXII now enforces that
(T7 adds genus `effectus` to both).

## 2026-10-05 - genus `effectus` (effectus-plan T7)

`_effectus_sigillare` turns `crusta/effectus.sh -clavis` lines into
particles: octeti (absent files are an "absens:" particle, not a
failure - a read of a not-yet-existing file must not make the key
IGNOTUM), provenientia, probatio (species), nomina (listing filtered by
the glob - unfiltered, an unrelated new file in src/ would move the key;
plant proved it), globus (listing + bytes of each match), directorium,
ambitus (FABRICA_* skipped: fabrica's own protocol, set for the run it
starts - digesting them would differ between record and judge time;
plant proved it), dominus (declared exitus, else IGNOTUM "sine domino"),
ignotum. Seam slot `sutura->effectus`; `FABRICA_EFFECTUS` overrides the
tool like FABRICA_FONTATIONES. Canon value added in the same commit
(fabrica_fumus XXXII enforces).

## 2026-10-06 - plan 5 T1: why a verdict re-runs

- `cursus.causa` only ever held the RUN's own cause; for a successful
  heal that is empty, so 36 of toml's 41 recorded runs say nothing. The
  judge's reason existed one frame earlier (`_ante_agere`'s `causa`) and
  died with it. Now `stalum` (field + column, migration V).
- The judge's own wording was the second problem: on a missed transitus
  key it could not say WHICH declared input changed - the key is one
  seal over all particles. Storing the particles of each recorded pass
  (migration VI) makes the miss nameable; first live result:
  `ingressus mutatus: pythonica/silva.py`.
- Two traps fixed on the way: stack-built FabricaSanatio (field by
  field - a new field is garbage; memset now) and the migration count
  given as a literal at the call site (derived from the array now).
- bin/fabrica is a declared input of the toml verdict (provenance of a
  house binary) - rebuilding fabrica voids every verdict that declares
  it. To measure in T4.

