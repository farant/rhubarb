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
