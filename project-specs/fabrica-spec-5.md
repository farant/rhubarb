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

