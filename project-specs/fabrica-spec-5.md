# fabrica spec 5 - the gate rollout

Interview: `project-specs/fabrica-5-interview.md` (Q1-Q6 DECIDED, every
recommendation (a), Fran 2026-10-06; on Q3: Python stays a thin wrapper
over house/C - fold into C what makes sense, instrument the rest).
Desideratum …J6HF. Builds on fabrica spec 3 (`fabrica-spec-3.md`: kind
`iudicium`, strategy `verdictum`, the toml pilot, §XIV as built) and
effectus slices 1-3.

## 0. Data (interview §0, plus readings for this spec)

- 7.7 h of gate runs in 91 commits (2026-10-02 -> 10-06); pythonica 353
  s/run, fabrica 182, aedilis 173, generata 133, oratio 102, radix 75,
  toml 40.
- toml, the one migrated gate: 60 real runs, 19 reuses (24%).
- Re-run causes are not recorded: `cursus.causa` stores
  `sanatio->causa` (`tools/fabrica.c:2569`), empty for SANATUM; the
  judge's reason exists one step earlier (`lib/fabrica.c:2988`,
  "lectio transitus mutata: <via>").
- Today's toml verdict is stale by `lectio transitus mutata: include`.
  toml's trace (`build/fabrica.db`, table `lectiones`, titulus
  porta_toml) holds 2,804 L, 849 A, 108 X, 4 E and 5 D rows; the D rows
  (directory enumerations) are `include`, `lib`, `materia/fontes`,
  `toml/fontes`, `toml/probationes`. Header lookups are ALREADY precise
  (A in earlier roots, X where found). D comes from aedilis:
  `tools/aedilis.c:1242` (`--nexus-purus`: every header of every
  inclusion root, promise check) and `:1324` (`_corpus_currere`: the
  `.c` sources of a directory).
- `pythonica/silva.py` is an input of every verdict because
  `porta_<G>`'s mandatum is `python3 -B pythonica/silva.py -iudicium G`;
  `iudicium_currere` (`silva.py:4998`) = delete verdict, `_porta_cruda`
  (run `PORTAE[G]`, check the signum regex), write `<G>: <compendium>`.
  15 of 189 commits since 10-02 touched silva.py; 20 added or removed a
  header in include/.

## I. Framing

A migrated gate saves its run whenever its own inputs did not change.
That only pays if the inputs every verdict shares are FINE - otherwise
one commit in five voids all of them. So: make the shared inputs fine
and observable first (§II), then migrate gates in readiness order with
an extended checklist (§III-§IV); pythonica gets its own input source
(§V).

## II. Shared coarseness (one plan task group, before any migration)

1. **Record why a verdict re-runs.** The judge's stale reason travels
   into the sanatio record and `cursus.causa` (SANATUM rows included).
   Census of causes per gate: a query over `cursus`
   (`bin/fabrica` subcommand or a tools/ script).
2. **Directory enumerations.** Opening reading: which aedilis mode
   lists each D directory during the toml gate, and why it runs there.
   Branches: (a) a genuine enumeration (a gate's own test directory: a
   new test must run) stays; (b) a house-wide check run inside a
   subsystem gate (`--nexus-purus` over every root) moves OUT of the
   subsystem gate (to the gate that owns it) or is scoped to the roots
   the closure uses; (c) an enumeration used only for membership becomes
   per-name A/X records. A result outside these branches goes to Fran.
3. **silva.py out of the verdict (fold into C).** fabrica runs the
   declared runner ITSELF: the `porta_<G>` mandatum becomes the runner's
   argv (`./toml/compile_probationes.sh`), the verdict text and the
   pass test (exit 0 + the gate's signum) are produced by bin/fabrica,
   the signum is declared on the action (A2). Consequences: PORTAE
   keeps the gate for non-migrated paths and `porta()` (Python, thin);
   the effectus ingressus argv = the mandatum's words (the
   `<argumenta>` declaration becomes derivable - A3).
4. **Measure** toml's reuse over the commits of this task group (with
   causes from II.1), before migrating the next gate.

## III. Per-gate checklist (spec 3 §V + effectus)

1. Raw IO in the gate's closure -> filum; raw getenv -> lectiones
   (`tools/lectiones_lint.sh`, runner mains into `VIA_PILOTA_RADICES`).
2. Inputs the runner writes with bash -> declared actions.
3. The runner as an effectus chain root: argv declared; in-chain
   lint 0 errata (resolve, or excuse with a reason); key reviewed.
4. `porta_<G>` in the subsystem's aedificatio.stml: mandatum, signum,
   vestigia (what the gate writes), ingressus.
5. Oracle: `./crusta/effectus_oraculum.sh -domus <runner>` non tecta 0
   (a tool step at migration, never a gate - C14).
6. Plants like `iudicium-fumus` (an input class changed -> re-run;
   unrelated edit -> reuse); 3 audited reuses.
7. Census: in-chain and standalone unresolved reported separately
   (first gate adds the split to `effectus -census`).

## IV. Order and mechanics

Readiness order: aedilis (C, traced, runner clean), fabrica, generata
(big bash trees), oratio, then the cheap ones as they fall out; radix
LAST or never (its key moves almost every commit - measured, not
assumed, before deciding); pythonica per §V. One expeditio row per
gate, one commit each with its plants. Shadow passes (park …2VP7) after
the batch.

## V. pythonica

1. **Fold into C first:** whatever probatio_silva.py exercises through
   Python that is house logic gets measured (which silva.py functions
   the tests spend their 353 s in - `cProfile`) and the candidates for C
   named; moving them is separate work per candidate, not this slice's.
2. **Python read ledger:** `sys.addaudithook` in the test process
   records the AUDITED channels - `open` (path, mode: L or S; a failed
   open = A) and `os.listdir`/`os.scandir` (D) - into FABRICA_LECTIONES
   in the line format of `lib/lectiones.c`. NOT audited by CPython:
   the stat family (`os.stat`, `os.path.exists/isfile/isdir`: X/A) and
   environment reads (E) - those go through recording wrappers in the
   harness (A4). Subprocesses are house binaries (traced themselves) or
   scripts (effectus).
3. Then the §III checklist.

## VI. Measuring

Per gate and overall: reuse rate (reuses / (reuses + runs)) before and
after, causes from II.1, seconds saved per commit; the toml rate after
§II is the headline number. Review after the first two gates whether
the remaining order holds.

## VII. Review focus

1. A gate reused although an input changed (stale green) -> plants per
   input class, audits (`FABRICA_AUDITUS`).
2. bin/fabrica running the runner must reproduce `porta()`'s verdict
   (signum, exit code, acta) - compare on every migrated gate.
3. An enumeration removed in II.2 that was genuine -> a plant that adds
   a file in that directory.
4. The audit hook missing a read channel (C extension, subprocess
   Python) -> plant per channel.

## VIII. Done means

toml reuse rate measured with causes before/after §II; aedilis,
fabrica, generata, oratio migrated with plants and audits; pythonica's
read ledger built and pythonica migrated (or a named blocker); per-gate
savings in a table; park and desideratum closed or re-filed.

## IX. AUDIENDA - DECIDED (Fran 2026-10-06: every recommendation (a))

- **A1. Where verdict logic lives.** (a) Recommended: bin/fabrica runs
  the declared runner and writes the verdict itself (C; silva.py leaves
  every key). (b) A tiny `pythonica/iudicium.py` module, declared
  instead of silva.py (smaller, still Python).
- **A2. The pass test.** (a) Recommended: exit code 0 AND a declared
  signum - a literal prefix the runner prints (`TOML PROBATIONES: `)
  followed by its count line, matched in C without regex (house rule:
  no expression languages); PORTAE's regexes stay for porta(). (b)
  Exit code only.
- **A3. One home for a migrated gate's argv.** (a) Recommended: the
  mandatum is the truth; the effectus ingressus derives its argv from
  the mandatum of its own action when the via matches (no
  `<argumenta>` needed there; kept for chain roots run by OTHER
  actions); the pythonica cross-check then compares PORTAE to the
  mandatum. (b) Keep `<argumenta>` everywhere.
- **A4. Python channels the audit hook cannot see.** (a) Recommended:
  `os.environ` reads and the stat family are not audit events - the
  harness installs recording wrappers (a mapping for `os.environ`;
  `os.stat`/`os.path.*` shims) and a plant per channel proves each is
  seen. (b) Declare pythonica's environment and existence checks
  coarsely.

## X. As built

**T1 (2026-10-06): why a verdict re-runs.** `FabricaSanatio.stalum`
(include/fabrica.h) = the judge's reason BEFORE the action ran; it
travels through both heal paths in a `stala[]` array parallel to
`status` (`_ante_agere` writes it, `_post_agere` stamps every record it
writes, serial and wave paths alike) and lands in `cursus.stalum`
(migration V; the insert binds an explicit empty text, never NULL). The
judge's reason itself was vague where it mattered most: a verdict whose
key is not found said "ingressus declarati aut verdictum mutati, aut
numquam servatum". Now a pass also stores its PARTICLES (migration VI,
table `particulae`, sutura `particulas_scribere/legere`): each declared
input's seal, plus `<mandatum>` and `<verdictum>`; on a miss the judge
diffs them (`_transitum_mutatum_nominare`: `ingressus mutatus: a, b +N`),
then re-checks the latest trace's reads (`lectio transitus mutata: X`),
and only then says "never recorded". `bin/fabrica causae [titulus]`
counts runs per (title, event, reason).

Live check on toml: appending a comment to pythonica/silva.py ->
`IGNOTUM ... ingressus mutatus: pythonica/silva.py`; the first new
cursus row read `lectio transitus mutata: aedilis.stml`. The stored
particles show `bin/fabrica` among toml's declared inputs (house binary
keyed by provenance): every rebuild of fabrica voids the toml pass - a
T4 candidate. Plants: reason dropped in `_ante_agere` -> cursus and
wave assertions red (7); naming off -> the two named-input assertions
plus the transitus test red.

Found on the way: three `FabricaSanatio` built on the stack field by
field (`lib/fabrica.c` judge and audit paths) - a new field was garbage
(`NOT NULL constraint failed: cursus.stalum`); now memset. The
migration count was a second literal (`IV`) at the call site - adding a
migration without it would silently skip it; now derived from the array.

**T2 (2026-10-06): directory enumerations.** Reading: all five D rows
of porta_toml came from `bin/compilator` (its head key sealed the `.h`
names of every -I root and of the source directory - shadowing), not
from aedilis (`--enumerare` emits none; `--nexus-purus` / `--corpus`
never run in this gate). Branch (c), membership only: the head key
keeps the roots' paths; the full key adds, per header used, the
existence of the same relative name in every other root (A/X noted by
`filum_existit`); key version II. toml trace: D 5 -> 0. Live: a new
unrelated header in include/ -> RECENS; `toml/probationes/latina.h`
(shadows include/latina.h) -> STALUM naming it. compilator fumus IX/X
added (X/X); plant (probes off) -> V (shadowing) and IX red.

**T3 (2026-10-06): the verdict runner in C (A1-A3).** An `iudicium`
action may declare `signum` (aedificatio.canon; the parser refuses it
elsewhere). Then bin/fabrica runs the mandatum - the runner itself -
deletes the old verdict BEFORE the run (sutura `verdictum_ponere`,
NIHIL = delete) and after exit 0 reads the action's log
(build/fabrica/acta/<titulus>.log), strips ANSI, finds the first
occurrence of the literal prefix, skips spaces, takes the next word
(`_verdictum_ex_actis`): verdict `<porta>: <signum> <word>` written
atomically; signature absent or `FRACT` (or `Fracti:`/`Failed:` with a
non-zero count) in the summary -> FRACTUM. The prefix needs no trailing
space (an attribute's trailing space would not survive formatting).
toml: `porta_toml` mandatum = `./toml/compile_probationes.sh`,
`signum="TOML PROBATIONES:"`; pythonica/silva.py and python3 left its
ingressus; `<argumenta/>` dropped - A3: an effectus ingressus without
`<argumenta>` takes the argv of its own action's mandatum when the
mandatum's first word is that script (`crusta_effectus_argumenta_radicis`);
the pythonica cross-check compares PORTAE to a runner mandatum.
Verdict text after the switch: `toml: TOML PROBATIONES: 13/13`,
byte-identical to silva.py's (§VII.2); RECENS afterwards; an edit of
silva.py leaves the toml verdict RECENS. Tests: probatio_fabrica
"signum" (pass writes the verdict; no signature -> FRACTUM + old
verdict gone; FRACT -> FRACTUM), iudicium-fumus P16 (same, real
bin/fabrica in a temporary root), crusta XXII (argv from the
mandatum). Plants: signature check off -> probatio II and P16 red;
`<verbum! (>lexicon` added to toml's mandatum -> the live cross-check
names it. A first plant (accept any output from the start of the log)
read past a short buffer and passed silently - undefined behaviour
proves nothing; the plant used names the signature itself instead.

**T4 (2026-10-06): measured reuse.** `tools/reusus_retro.sh TITULUS [-n N]
[-addere F] [-specificatio F]` turns a verdict's inputs - the stored
trace (L -> content, X/A -> existence, D -> entries), its effectus key
(octeti, probatio, nomina, globus, arbor; `provenientia bin/X` -> the
binary's closure as bin/aedilis reads it under FABRICA_LECTIONES), its
declared ingressus (build/ products: the producing action's inputs, one
level) - into rules, and replays `git diff --name-status` of the last N
first-parent commits: reused, or the FIRST voiding cause. Approximations
named in the tool (build/ and absolute paths invisible to git;
environment and clang identity skipped; closures as of today; build
scripts of house binaries not modelled - an undercount). The trace from
BEFORE T2/T3 was not saved at T1 as the plan said; it is reconstructed
exactly from §0 (the five D rows) and toml/aedificatio.stml @41c4a788
(pythonica/silva.py declared) - `-addere`.

| porta_toml | last 40 | last 150 |
|---|---:|---:|
| before T2/T3 | 24/40 (60%) | 103/150 (68%) |
| after T2/T3 | 30/40 (75%) | 118/150 (78%) |

Born checked: "before" names `enumeratio include` (14 of 150) and
`silva.py` (3, first cause only). Remaining 32 voids over 150: 23 are
house-binary provenance (bin/mensor 9 - 7 of them `aedilis.stml`, which
every binary's build action declares as `configuratio`; bin/fabrica 8;
bin/aedilis 5; bin/compilator 1), 9 genuine toml inputs (include/runae.h
5, toml_arbor.c, excerptum.h, tomllib_aurum.sh, the runner). Live
causes since T1 agree (bin/compilator, bin/fabrica, aedilis.stml).
Finding: bin/fabrica, corpus_indicem.sh and tomllib_aurum.sh are in the
key only through the runner's MANUAL branch `[ -z "$FABRICA_LECTIONES" ]`
- dead whenever fabrica runs the verdict (it always sets the variable);
effectus treats conditions as "may".

**T5a (2026-10-06): the channel for the root suite.** The lint pilot
(`tools/lectiones_lint.sh`) now covers every `probationes/probatio_*.c`
(benchmark excluded, as the aedilis gate does): 494 files in the
closure. It blocked 57 raw calls - 22 reads, 11 `stat`, 6 `getenv`
(real channel gaps: a verdict could stay green while such an input
changed) and 18 writes. New transitional shims in `lib/lectiones.c`:
`lectiones_fopen` (read modes note L or A, write modes S) and
`lectiones_stat` (X or A); paths under `/dev/` are NOT noted - a device
is no input, and fabrica cannot seal one (IGNOTUM would make the
verdict never reusable: /dev/urandom in moneta.c, uuid.c). Each site is
a one-word change (`fopen(` -> `lectiones_fopen(`, `stat(` ->
`lectiones_stat(`, `getenv(` -> `lectiones_ambitus(`), applied by exact
line from the lint's list, plus `#include "lectiones.h"` where missing
(29 files; 86 insertions, 57 deletions). Four committed source lists
gained `"lectiones"` (officina legatus/sonda, silva examen/identitates;
regenerated by their own generators; the generata gate found them).
Lint: 0 blocking; root suite 215/215; generata sana. Plants: a raw
`fopen` read in probatio_specimen.c -> OBSTAT naming it; noting off in
lectiones_fopen -> probatio_lectiones IIc red.


**T5b (2026-10-06): porta_aedilis.** `porta_aedilis` in the root
aedificatio.stml: mandatum `./tools/aedilis_porta.sh`, signum
`PORTA AEDILIS:` (PORTAE regex now `PORTA AEDILIS: \d+`, so the C
verdict `aedilis: PORTA AEDILIS: 230` and `porta()`'s agree byte for
byte), the runner as effectus chain root (lint 0, key: nomina
probationes/probatio_*.c, its own .err/.diff, provenientia bin/aedilis),
a second effectus ingressus for `probationes/fixa/villa/ssh_stipes.sh`
(the villa test execs it; a child bash's `cat` reads never reach the
ledger - its key names every fixture by content), the five house
binaries tests run through `system()` (bin/generare, manus, natura,
natura_canones, natura_glossae: an exec is not a ledger read), the
daemon's sources (manifesta of fragmentum_gesta_tabulariumd +
tabulariumd.sh) and clang's identity. First heal 104 s; reuse after it
RECENS.

What the first heals found, in order (each a real gap, not a
formality):

1. *Writes outside the vestigium (418).* Root tests write scratch all
   over build/ - declared by name (5 directories, 28 files; Fran: a
   single scratch area for all tests is a desideratum). Three were
   worse and are fixed: `probatio_generare` wrote helper scripts into
   bin/ beside the installed binaries (bin/generare gained
   `--instrumenta DIR`, default bin; the test uses
   build/probatio_generare_instrumenta), and `probatio_vigilia` wrote
   and removed the REAL commit stamp `.vigilia_commissum` that the
   residents read (`vigilia_viam_commissi_ponere`, NIHIL = default; the
   stamp was in fact missing when looked at). Plant: setter a no-op ->
   the "stamp moved re-arms" assertion red.
2. *A test rebuilt the live resident.* `probatio_cliens_tabularii`,
   `villa_agens` and `sententiae_horreum` ran `./gesta/tabulariumd.sh
   -struere` via `system()` - relinking gesta/build/tabulariumd under
   the running resident, with untraced reads (its key: 4 unresolved
   sites -> IGNOTUM). Now they only check the binary exists; the root
   runner already prebuilt it once; under fabrica the action
   `tabulariumd` (strategy ignota, precondition of porta_aedilis)
   builds it. fabrica refuses an ignota output as an ingressus
   ("praecondicio sola licet"), so the verdict keys the daemon's
   SOURCES, which is the better key anyway.
3. *A child-written temp read back.* bin/aedilis captures `git
   rev-parse HEAD` (and `clang -MM`) through a per-pid temp file under
   build/aedilis: an unowned build/ read, and its content (HEAD) would
   have voided every commit. aedilis now notes the child's write (S)
   before reading - the existing rule "written in the run = not an
   input" then applies.
4. *Reads of the action's own scratch.* sqlite databases, daemon port
   files and silex volumes are written by children that note nothing.
   fabrica rule (new): a read under an action's OWN (non-communis)
   vestigium is the action's product, not an input; communis areas
   stay open to others and excuse nothing. probatio_fabrica VIIb (born
   red).
5. *Temporary roots.* Tests make their areas under /tmp by shell and
   one enumerates /tmp itself (D /tmp - any process touching /tmp
   would void the verdict). fabrica (new): /tmp, /private/tmp,
   /var/folders, /private/var/folders are never inputs, like effectus
   class temporaria (spec-2 par. IX). probatio_fabrica VIIc (born red).
6. *The live repository.* `probatio_git` reads `.`'s HEAD, refs,
   packs and fixed old commits by design - keyed honestly, every commit
   voids the verdict. Fran: the aedilis gate still derives, builds and
   clang-diffs it but does not RUN it (REPOSITORIUM_VIVUM_LISTA); radix
   runs it. Desideratum: a committed fixture repository.
7. *Speculum capsule* (build/speculum/hospes): rewritten every radix
   run with tempus, commissum, sordidum; aedilis sees it only by
   existence (clang reads it outside the ledger). Owner declared
   (`speculum_hospes`, ignota); existence-only is a NAMED approximation
   (Fran); deterministic provenance under tests = desideratum.

Stored trace: 1243 L, 539 X, 2728 A, 44 D (the inclusion roots of
`--nexus-purus` - this gate owns that house-wide check, spec par. II.2
branch b - plus the gate's own test-data directories), 4 E (HOME and
three unset switches). No .git, no temporary roots.

Plants (judge only, restored after each): lib/xar.c, the ssh stub's
df.txt, gesta/fontes/tabularium.c, the runner, bin/generare's bytes and
a new probationes/probatio_*.c -> IGNOTUM naming the input; a new
header in include/ -> STALUM `lectio transitus mutata: include`; an
unrelated project-specs edit -> RECENS.

Oracle (`effectus_oraculum.sh -domus`, par. III.5): 1850 observed
effects not covered, all but a handful inside the GENERATED
build/aedilis/probatio_*/struere.sh (bash `-f`/`-nt` probes, clang
reading objects, mkdir) - build products written by bin/aedilis in the
run, whose sources aedilis itself reads and notes. The rest led to
items 2, 7 and the ssh stub above. Not 0: named residue.

Census split (par. III.7): `effectus -census` now counts unresolved
sites `in catena:` (in a declared chain) and `solum:` separately -
today in catena 27 nulla + 7 partialis, solum 673 + 571 (sums equal
the TSV's totals).

Audited reuses (`bin/fabrica sanare -audit`, re-run of a RECENS pass):
(1) after the census edit in crusta/instrumenta/effectus.c (outside the
closure), (2) after this section's docs edits - both "transitus iterum
congruit", verdict byte-identical; (3) after the commit itself (HEAD
moved: no .git in the key), reported with T6.

| porta_aedilis (`reusus_retro.sh`, today's trace) | last 40 | last 150 |
|---|---:|---:|
| reused | 12/40 (30%) | 64/150 (42%) |

Of the 86 voids over 150, 75 are genuine closure inputs (headers of the
math tier, tessera.h, runae.h, fabrica.h ...), 11 house-binary
provenance (`aedilis.stml` through bin/natura_glossae - …57Y). The
ceiling estimate (~45%) holds: aedilis reads nearly all of include/.

**T6/T7 re-plan (2026-10-07).** The fabrica gate runs
`tools/generata_probare.sh` inside itself: 73 of its 78 unresolved key
sites are generata's (amalgama_fontes_generare.sh 21,
amalgama_excludenda_generare.sh 15 - argv parsed by `for arg in "$@";
case` that effectus does not carry loop values through). Fran: T7
first; then, given the oracle's record (34/34 consensus since 10-02,
fabrica judging 98 artifacts generata never saw), generata RETIRES into
fabrica (spec 1a Q15: oracle, then deletion) and the oracle with it.

**T7a (2026-10-07): the swap.** `tools/generata_iudicare.sh` runs
`bin/fabrica iudicare -plenus -omnia`, keeps the COMMITTED artifacts
(git ls-files; build/, bin/, ~/.bin counted only, as
`_fabricam_exigere` does) and names every non-RECENS one; nothing
judged = FRACTA (a gate that judges nothing is dead, not green). 135
committed artifacts in ~25 s (generata: 51 in 2-6 min); generata's 51
are a subset. Plants, side by side on one tree (latina.h number block,
runae tables, silva amalgam, a fragmentum, silva's amalgamator
manifest, the lexicon header, the entities table, crusta's cooked
registry): the new gate named all 8 plus the artifacts that embed them
(silva_latina_datum.c, officina and tessera amalgams) and tessera's
excludenda generator broken by the latina.h plant (IGNOTUM, named);
generata named the same 7 it covers, not the registry, and reported
the excludenda break as "manifestum amalgamatoris tessera" - the new
gate names the right artifact. Debts: the gate is owed when the judge
or the declarations change (runner, tools/fabrica.c closure,
aedificatio/fabrica stml and canons - inventory row swapped); per-input
debts belong to commissio's `iudicare -plenus -tacta VIAE` phase, which
already blocks every commit on touched committed artifacts (reach
checked: a parser source -> silva tables + amalgam; runae.h -> runae
tables + amalgams; a grammar -> its cooked registry; a .genera -> the
natura canons). The oracle no longer reads generata's receipt (it would
now be the new gate's output - an empty comparison, a false
consensus); it runs generata_probare.sh itself until T7b.

**T7b (2026-10-07): the oracle retires.** `fabrica` left PORTAE and the
inventory (row removed with cause); with generata judged by fabrica
there is no second judge to compare. The one property the oracle had
beyond agreement - a generated artifact whose declaration disappears is
named, not silently unjudged - now lives in generata_iudicare.sh and is
wider than before: every COMMITTED file with `GENERATUM` on line 1 must
be among fabrica's judged artifacts, else `GENERATUM sine iudicio`
(gate red). Today 87 such files; 6 unjudged, all in three named
classes excluded with their cause: fixtures (`probationes/fixa/`:
frozen copies carrying their source's header), knotapel's frozen demo
snapshots, the ledger projection gesta/annales/tabula.md. Plant: the
entitates_html declaration deleted -> `GENERATUM sine iudicio:
lib/entitates_html_tabula.c`, FRACTA. The gate costs ~16-25 s; the
retired pair cost generata 133 s + oracle 150-180 s per commit that
owed them.

**T7c (2026-10-07):** tools/generata_probare.sh and
tools/fabrica_oraculum.sh deleted (0d652faa); comments, the tools page
and memory follow; park …GTQHQ (generata 381 s) closed - 16-18 s
measured in three commits.

**T8 (2026-10-07): oratio measured, not migrated** (Fran). The runner
writes `git ls-files` lists of every tracked `*.md` (the prose corpus)
and `*.c`/`*.h` (the identifier lint) and renews the house-wide nexus
index before its tests; seven of its nineteen tests read those. An
honest key is therefore nearly the whole repository: only 3 of the last
150 first-parent commits touched no .c/.h/.m/.md file - a reuse ceiling
of 2%. Where its 96.6 s go (mensor, 96ebd536): probatio_oratio_oraculum
44.0 s, _stml 26.7 s (reads the md corpus), _canon 7.2 s, _vocabula
3.9 s, the other fifteen ~15 s. The oracle test reads its own fixtures,
not the corpus, but through raw `fopen` (oratio's tests are outside the
lectiones lint): carving it out as its own verdict = desideratum
…BPRSBB (channel first, as T5a). The gate stays crude.

**T9 (2026-10-07): where pythonica's time goes.** `pythonica/profilare.py`
(new, rerunnable) runs probatio_silva.py under cProfile and wraps
`subprocess.Popen` to time every child with its calling test line and
nearest silva.py function. One run: 421.8 s, 1076 children whose wall
time sums to 351 s - the suite is a driver of other programs, not
Python work.

| where | s | what |
|---|---:|---|
| `photographia_materializare` (9 calls) | 137.6 | `_clonare_ignorata` 102.5 (46 `cp -c -R` per snapshot - clonefile per FILE), git clone + read-tree ~13 |
| `photographia_delere` (10) | 52.9 | `shutil.rmtree`: 719,611 unlinks |
| bin/aedilis via `_clausurae` (24) | 63.4 | the same all-suite closures in five portae-debitae tests (~12 s each) |
| `portae_debitae.sh` live (1) | 22.8 | the same closures again |
| `exspectare` sleep (15) | 30.1 | polling shadow-gate workers every 2 s |
| `extenta` / `_porta_cruda` / `formare` / `examen` / `differre` | 25.4 / 20.5 / 11.2 / 9.9 / 8.8 | tests of those tools - inherent |
| git (331 children) | 37.9 | `sigillum_arboris` 210 calls 5.1, snapshots, the rest scattered |

Candidates (desiderata, not this slice's work - plan "not in this
plan"): clonefile(2) on whole directories in C (…6RME, ~100 s);
closures once per tree state through a batch aedilis mode (…4WYGT,
~85 s); asynchronous snapshot deletion and pid-based waiting in Python
(…JFQ99, ~65 s). Together ~250 of 422 s. T10/T11 (the ledger and the
verdict) proceed independently: a reused pythonica verdict costs none of
it.

**T10 (2026-10-07): the Python read ledger.** `pythonica/lectiones.py`
writes the same ledger as lib/lectiones.c, same line forms, same file
(`FABRICA_LECTIONES`, re-read on every note), `/dev/` skipped, paths as
given. `instituere()` once (an audit hook cannot be removed - tests set
it in a CHILD process):
- audit hook: `open` -> L or A for reads (existence through the REAL
  stat, since the hook runs before the open), S for writes; `os.listdir`
  and `os.scandir` -> D;
- wrappers for what CPython does not audit: `os.stat` and `os.lstat`
  (X or A) - the os.path family (exists, isfile, isdir, getmtime) looks
  `os.stat` up at call time and is covered; `os.environ` becomes a dict
  subclass that notes E on explicit lookups (get, [], in) and mirrors
  writes into the real environ (putenv).
A whole copy of the environment (`dict(os.environ)`, `.copy()`) notes
NOTHING, as C's environ is never noted: the subclass leaves `__iter__`
and `keys` alone, so CPython copies it by its fast path without
`__getitem__`. Otherwise every `env=dict(os.environ, ...)` in silva.py
would key the verdict on TERM_SESSION_ID and friends. Test (pythonica,
child process): eleven assertions over every channel plus `/dev/` and
the copy. Plants: open ignored -> the three open assertions red; stat
wrapper off -> the two stat ones; environ off -> the two E ones;
`__iter__`/`keys` overridden on the subclass -> the copy assertion red.
Not yet wired into the gate: T11.
