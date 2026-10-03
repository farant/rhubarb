# fabrica — spec 3 (gates judged by what they read)

*Born 2026-10-03 from the slice-3 interview
(`project-specs/fabrica-interview-3.md`, Q1–Q11) plus an inline
codebase pass the same day. Predecessors: `fabrica-spec-v1.md` (§XI
slices), `fabrica-spec-v2.md` (1a; §I gives slice 3's mapping),
`fabrica-spec-1b.md` (model + executor; §IX.2 the verdict-flake
warning), `fabrica-spec-2.md` (read ledger, store, compilator; §XIII
as built). Question that prompted it: slice 2 made objects and
generated files judged by what was actually read - but every GATE is
still judged by the whole tree, and its pass is thrown away at every
commit.*

*Status: v2 (2026-10-03). A1 decided (outside reads digested); A2-A4
carry recommendations. T1 spike done (§XI): ready, with four
corrections folded into T3.*

## 0. What the measurements say (dated 2026-10-03)

- Since 2026-09-26: 184 gate runs, 13,110 s = 3.6 h
  (`build/portae/tempora.tsv`). Most expensive per run: pythonica
  344 s, aedilis 215 s, fabrica 204 s, generata 139 s, radix 79 s;
  toml 34 s (8 runs).
- A live receipt (`build/portae/<nomen>.viva.json`) is keyed to
  `sigillum_arboris()` = HEAD + whole `git diff` + every untracked file
  (`pythonica/silva.py:2526`). Any edit anywhere makes it stale, and
  `commissio` deletes every live receipt after each commit
  (`recepta_viva_delere`). A pass is never reused across commits:
  a doc-only commit after a toml commit owes no toml gate, but a later
  toml-touching commit re-runs it even if toml's inputs are unchanged
  since the last pass.
- Owed gates come from aedilis closures + inventory lenses
  (`portae_debitae`). They are declared, and declarations drift: on
  2026-10-03 `tools/cursor_communis.sh` (sourced by four suites) owed
  no suite gate at all.

## I. Framing

Slice 3 makes one thing true: **a gate's pass is a record keyed to
what the gate read.** If none of it changed, the pass stands - across
edits elsewhere and across commits. If any of it changed, or the
reads could not all be digested, the gate runs.

Decided (interview):
- Gate receipts first; per-test verdicts later, on top, if this holds
  (Q1).
- Only passes are recorded. A failure is never cached and always
  re-runs (Q2).
- First client: the toml gate (Q3).
- Inputs = the read ledger's trace + the shell scripts the runner
  sources, derived by crusta (Q4, Q10).
- The gate is a fabrica action of a new kind, judged by `bin/fabrica`
  (Q5).
- `commissio` and `porta()` both reuse a pass; shadow runs record
  passes too (Q6).
- No expiry by time (Q8). The oracle = plants per input class + a
  sampling audit (Q9).
- Gates not yet migrated keep tree-seal receipts unchanged (Q11).

Out of scope: per-test verdict actions; migrating gates other than
toml (a batch job afterwards, like W0ZBW); a store shared across
worktrees; removing tree-seal receipts.

## II. Verdict actions

### II.1 The kind
A new action kind `iudicium` (`FABRICA_ACTIO_IUDICIUM` beside
generator / formatio / institutio, `include/fabrica.h:43`):

```xml
<actio titulus="porta_toml" genus="iudicium" lectiones="verum">
  <mandatum>
    <verbum! (>python3
    <verbum! (>pythonica/silva.py
    <verbum! (>-iudicium
    <verbum! (>toml
  </mandatum>
  <ingressus genus="fasciculus" via="pythonica/silva.py"/>
  <ingressus genus="instrumentum" via="bin/aedilis"/>
  <ingressus genus="instrumentum" via="bin/compilator"/>
  <praecondicio actio="toml_corpus"/>
  <praecondicio actio="toml_aurum_silvestre"/>
  <exitus via="build/fabrica/verdicta/toml.txt" provenientia="regeneratio"/>
</actio>
```

- `lectiones="verum"` is REQUIRED. A verdict action without a trace
  is refused at declaration parse.
- **One home for the gate command.** The mandatum calls a new
  `silva.py -iudicium <porta>` mode. It runs the gate exactly as
  `porta()` does today (the `PORTAE` command + its signum regex,
  unfiltered, `receptum=False`). On a pass it writes the verdict file;
  otherwise it deletes it and exits non-zero. The signum stays in
  `PORTAE`, with no second copy in STML.
- **Verdict file** `build/fabrica/verdicta/<porta>.txt` holds the porta
  name + the signum match (e.g. `TOML PROBATIONES: 13/13
  praeteritae`). No timings, so it is deterministic.
- **Sweeps skip it.** `iudicare -plenus`, `-omnia`, `-tacta`,
  `sanare installata` / composites and the fabrica oracle never judge
  or run an `iudicium` action unless it is named. Without this, every
  sweep would run test suites.

### II.2 Pass-only by construction
The existing trace store (`lectiones_scribere`, slice 2 T2) is written
only after a successful regeneration. A failing gate exits non-zero,
so it leaves no trace and no verdict file, and the next judgement is
STALUM/IGNOTUM: it runs. No new store is needed.
- Retention: the store keeps the LATEST trace per (titulus, exitus).
  A newer run replaces the older pass, and nothing expires by time
  (Q8). Switching branches back and forth re-runs once per switch
  (§X A3).

### II.3 Judging and running
- `bin/fabrica iudicare porta_toml`:
  - RECENS when the verdict file exists AND every trace entry
    (L A X D E) and every declared ingressus still digests the same;
  - STALUM when anything differs, naming the first differing path;
  - IGNOTUM when there is no trace, or the trace holds a read that
    cannot be digested (§III.5).
- `bin/fabrica sanare porta_toml` runs it (FABRICA_LECTIONES on, for
  the whole process tree) and records the pass.
- The memo audit (`auditus`, `iudicare -audit`, 1-in-N) applies
  unchanged: a RECENS verdict is re-run anyway and must still pass.

## III. Inputs

### III.1 The trace
fabrica sets `FABRICA_LECTIONES` for the gate. Children inherit it, so
every house binary in the tree appends to the same ledger: bin/aedilis
(closures), bin/compilator (header hashes through filum), and the test
binaries (fixtures through filum). bash and python do not append (see
III.2–III.4). Store reads stay exempt (the judge filter ejects the
store root, slice 2 T3).

### III.2 Scripts the runner sources, derived by crusta (Q10)
A new tool `bin/fontationes <script>` (crusta/instrumenta/
fontationes.c, on `crusta_arbor_parsare`, `crusta_imperium_titulus`,
`crusta_verbum_staticum`). It prints the transitive set of repo
scripts reached by `source` / `.` or executed as a command word.
- **Resolution.** Static words resolve directly. `$V/rest` resolves
  when V is assigned once at top level by one of the house idioms:
  - `$(cd "$(dirname "${BASH_SOURCE[0]}")[/rel]" && pwd)`;
  - `$(cd "$W/rel" && pwd)` with W already resolved;
  - `$(cd "$(dirname "$0")" && pwd)`.

  Measured on the toml runner, this covers every source line:
  `$RADIX_DIR/tools/{sera,vexilla,cursor_communis,mensor_suitae}.sh`.
- **Over-approximation is fine.** A source inside an `if` counts.
- **Unresolvable** (`source "$X"`, `"$COMPILATOR" ...`, a computed
  path in source position) is printed as UNRESOLVED with its line. The
  action is IGNOTUM unless that line's target is declared by hand
  (`<ingressus genus="fasciculus">`). Executed variables that resolve
  to binaries (`$COMPILATOR`) are covered by III.3, not by this tool.
- Derived at judge time, every time, and the derived set's digest
  joins the key.

### III.3 Executed binaries
House binaries the gate runs (`bin/aedilis`, `bin/compilator`) are
declared `genus="instrumentum"` (bytes). A relink changes the bytes
(LC_UUID), so it re-runs the gate. That is conservative: never stale,
sometimes wasted.

clang: a new ingressus genus `identitas_clang` whose digest is the
identity bin/compilator already computes (real binary by path, size,
mtime and inode, `tools/compilator.c:168`). It covers the linker
calls the runner makes with plain clang, and system headers.
Machine-specific paths stay out of committed STML.

### III.4 Inputs written by bash become declared actions
Rule: **a traced read under `build/` must be written by the same run
(an S record) or be the exitus of a declared action; otherwise the run
is IGNOTUM** ("unowned build input", with the path named). A verdict
would otherwise trust a file that bash wrote and nothing judged.

toml has two:
- `build/toml_corpus.lst`. The runner writes it from `git ls-files
  '*.toml'`; probatio_toml_corpus reads it. Becomes action
  `toml_corpus` (strategy ignota, realized blind as a precondition,
  ~10 ms), so it is current when the verdict is judged and a new
  tracked `.toml` moves its digest.
- `toml/build/aurum_silvestre.txt`. The runner regenerates it with
  python tomllib when the manifest is newer (mtime). Becomes generator
  action `toml_aurum_silvestre`: inputs = manifest +
  `toml/tomllib_aurum.sh` + python3 (`instrumentum`, as at
  aedificatio.stml:368).

The runner keeps working by hand. When `FABRICA_LECTIONES` is unset it
calls `bin/fabrica sanare toml_corpus toml_aurum_silvestre` (cheap
when RECENS). Under fabrica it does nothing: the preconditions were
already realized, and a nested sanare would meet fabrica's own lock.

### III.5 Reads outside the repo
Q7 answered "allowlist (system roots pinned by the clang identity),
else IGNOTUM". The codebase pass found a conflict, raised in §X A1:
probatio_toml_differentia and toml_corpus_ambulare read the
"silvestria" corpus from `$HOME` (paths in
`toml/probationes/fixa/silvestria.manifestum`, each pinned there by
its sigillum; a mismatch is counted as omitted, not failed). Under Q7
as answered, the toml gate is IGNOTUM on every run. DECIDED
(Fran, 2026-10-03, §X A1 option (a), decretum …JQ1V0) - Q7 amended:
- Allowed and not digested: the SDK and system roots (`/usr/include`,
  `/usr/lib`, `/Library/Developer`, `/Applications/Xcode.app`) - pinned
  by `identitas_clang`.
- Digested like repo files: any other regular file or directory the
  ledger names, inside or outside the repo (the ledger records paths;
  the judge digests them afterwards).
- IGNOTUM: what cannot be digested - devices, sockets, FIFOs, and
  paths that no longer exist with no A record.

### III.6 Environment
E records come only through the recorded wrapper (`lectiones_ambitus`).
The toml closure's raw `getenv` calls (RHUBARB_RADIX, HOME,
ORACULUM_OMNIA, ORACULUM_EXEMPLUM, COMPUTUS_SCRIBERE) move onto it, and
`tools/lectiones_lint.sh` enforces this (§V).

E values at or under the tree root are stored relative, like paths.
Otherwise the same gate in a shadow clone would never match the live
tree (§IV.3).

### III.7 Side channels
`sera` locks, the tee log, and mensor's metrics volume are written and
read every run. If any appears as an L/D entry, the trace changes
every run and nothing is ever reused. The spike (§VII T1) checks this.
Any that appear run with `FABRICA_LECTIONES` unset (`env -u`), named
in the worklog.

## IV. Consumers

### IV.1 porta()
- `porta(nomen)` for a gate that has a verdict action calls `bin/fabrica
  iudicare porta_<nomen>`:
  - RECENS → returns `Porta(sana=True, compendium='... [digestum idem,
    transitus ante N min]')` without running;
  - otherwise → `bin/fabrica sanare porta_<nomen>` and returns its
    verdict.
- `porta(nomen, vis=True)` always runs (and records a pass).
- Filtered calls (`porta(nomen, filtrum)`) are unchanged: tree-seal
  receipt, never a verdict record.

### IV.2 commissio
For every named or owed gate with a verdict action, commissio asks
fabrica first and prints `porta X non iterum cursa: digestum idem
(transitus <commit|ante N min>)` when RECENS. Verdict records are NOT
deleted after a commit; tree-seal receipts still are (Q11).
`_fabricam_exigere`'s `iudicare -plenus -tacta` skips iudicium actions
(II.1).

### IV.3 Shadow runs (Q6)
porta_umbra runs in a photograph clone with its own `build/`. To record
a pass usable in the live tree:
- the clone's fabrica writes its trace into the LIVE `build/fabrica.db`;
- paths and E values are stored relative (III.6).

This is the riskiest consumer and comes last in §VII (§X A4).

## V. The toml pilot

Changes inside toml (each is a step of §VII):
1. `probatio_toml_computus.c` reads its gold with raw `fopen`
   (lines 51, 182, 211) and `probatio_toml_totalitas.c` writes a
   failure file with raw `fopen` (233). Both move to filum.
2. All raw `getenv` in the closure move to `lectiones_ambitus`
   (III.6).
3. `tools/lectiones_lint.sh`: `VIA_PILOTA_RADICES` gains the toml test
   mains, so raw IO in their closures BLOCKS at commit.
4. Actions `toml_corpus` and `toml_aurum_silvestre` (III.4); the runner
   stops writing those files itself.
5. `porta_toml` declared in a new `toml/aedificatio.stml` (beside
   officina/, silva/, tessera/ - T5 confirms fabrica picks it up).
6. v2 (§XI): `materia/fontes/materia_coctor.c:1230` reads the registrum
   grammar raw; it notes the read itself via `lectiones_notare`
   (materia stays filum-free), and the lint accepts that form.

## VI. Oracle (Q9)

Plants, each one real and compiling, run as a new gate
`iudicium-fumus`:

| # | Plant | Expected |
|---|---|---|
| P1 | comment edit in `toml/fontes/toml_lector.c` (traced via compilator) | STALUM, runs |
| P2 | comment edit in `tools/cursor_communis.sh` (derived script) | STALUM, runs |
| P3 | edit `README.md` / a doc | RECENS - the point of the slice |
| P4 | raw `fopen` added to a toml test | lectiones_lint blocks |
| P5 | toml test reads a FIFO | IGNOTUM |
| P6 | a toml test made to fail | no record; next call runs again |
| P7 | runner line `source "$X"` | IGNOTUM naming the line |
| P8 | new tracked `.toml` file | `toml_corpus` changes → STALUM |
| P9 | `ORACULUM_OMNIA=1` in the gate env | STALUM (E record) |
| P10 | relink bin/compilator without source change | STALUM (instrumentum bytes) |

**Audit.** Under `iudicare -audit` (all) or 1-in-N, a RECENS verdict
re-runs. A failure after RECENS is DISCORDIA: loud, recorded in
`cursus`, and the old trace is diffed against the new one so that
entries only in the new trace are named as the probable missed input.

**Agreement over real work.** During the slice, every toml verdict
reuse in a commit also runs under audit (`FABRICA_AUDITUS=1` set for
the slice's commits). Done needs ≥10 reuses audited, 0 discord.

## VII. Order

- **T1 Spike (no code kept).** Run the toml gate by hand with
  `FABRICA_LECTIONES` set. List every ledger entry by kind, and explain
  each: closures, headers, fixtures, side channels, outside reads.
  Pass = every entry explained, every raw-IO gap (V.1–2) found, the
  side channels settled (III.7). One unexplained entry = the design
  isn't ready.
- **T2** `bin/fontationes` (III.2) + its tests (idioms, conditional,
  UNRESOLVED).
- **T3** toml filum/env migration + lint pilot (V.1–3, V.6), plus the
  §XI corrections: compilator records its destination as S;
  mensor_suitae.sh runs bin/mensor without the ledger.
- **T4** `toml_corpus` / `toml_aurum_silvestre` actions + runner rule
  (III.4).
- **T5** iudicium kind in lib/fabrica.c and bin/fabrica:
  - declaration parse, refusal without lectiones;
  - sweep exclusion;
  - verdict file;
  - the build/ ownership rule;
  - outside-read rule (as decided in A1);
  - `identitas_clang`;
  - derived scripts in the key.
- **T6** `silva.py -iudicium`, porta() and commissio integration
  (IV.1–2).
- **T7** `iudicium-fumus` (P1–P10) + audit DISCORDIA with trace diff.
- **T8** Shadow recording (IV.3), if A4 keeps it in.
- **T9** Measure; §XIII As built; worklogs; memory.

## VIII. Done means

- A doc-only edit followed by `porta('toml')` returns a pass without
  running. After a toml-touching commit, a later commit that doesn't
  touch toml's inputs reuses it.
- P1–P10 green in `iudicium-fumus`, which is itself in PORTAE and
  inventoried.
- ≥10 audited reuses during the slice, 0 discord.
- `iudicare -plenus` time unchanged (no verdict runs in sweeps).
- toml gate cost when RECENS: under 2 s (judging + script derivation).

## IX. Review focus (failure modes no step above tests by default)

1. **A verdict file left by an older run.** RECENS needs BOTH the file
   and a matching trace. Only the -iudicium mode writes the file, and
   it deletes it first.
2. **The tree changes during the gate.** The existing write/footprint
   check covers generators. For a 30–300 s gate, a source edited
   mid-run must make the run's trace untrusted (digest at end ≠
   digest read). That check exists per action; confirm it covers
   iudicium actions.
3. **Flaky passes.** Q2 caches passes, so a flaky test that passed is
   reused until its inputs change. The audit samples catch it
   statistically; that is accepted, not solved.
4. **Trace size.** The toml corpus reads every tracked `.toml`, and
   silvestria many more. Judging digests all of them each time: measure
   in T1 against the 2 s target.
5. **The gate reads its own previous outputs** (toml/build/* binaries,
   objects). S-then-L within one run is fine; an L of a previous run's
   build artifact with no S this run is the III.4 rule. compilator
   writes objects as filum_scribere(temporary) + filum_movere
   (tools/compilator.c:420-425): confirm the MOVE is recorded as S on
   the final path, otherwise every linked object is an "unowned build
   input".
6. **python's own reads** (silva.py imports, PORTAE) are covered by the
   declared `pythonica/silva.py` ingressus only. Other imported modules
   would be invisible; T1 lists them.

## X. AUDIENDA — open, for Fran

- **A1. Outside reads (amends Q7) - DECIDED (a), 2026-10-03.** The toml gate reads
  content-pinned files from `$HOME` (silvestria). Options:
  - (a) Recommended: digest any regular file the ledger names, inside
    or outside the repo; IGNOTUM only for what cannot be digested
    (III.5 draft);
  - (b) keep Q7 as answered, and move the silvestria checks out of the
    toml gate into a manual or separate gate;
  - (c) keep Q7 and let toml stay IGNOTUM (the slice proves nothing on
    toml).
- **A2. Names.** Action kind `iudicium`; verdict actions titled
  `porta_<nomen>`; verdict files in `build/fabrica/verdicta/`; tool
  `bin/fontationes`; gate `iudicium-fumus`.
- **A3. Retention (clarifies Q8).** The trace store keeps only the
  latest pass per gate. Keeping several would make branch switches
  free, at the cost of a store-shaped verdict record. Recommended:
  latest only in slice 3.
- **A4. Shadow runs.** IV.3 needs the clone to write the live database
  and relative E values. Recommended: keep it as T8, but the slice is
  done without it if T8 turns out larger than a step.

## XI. T1 spike (2026-10-03) - corrections marked v2

Method: the toml gate (`toml/compile_probationes.sh`, warm store, 13/13
pass, 34 s) run twice by hand with `FABRICA_LECTIONES`; paths normalized
(tree root -> relative, `$HOME` -> `~`), unique entries classified. No
code kept.

**Size.** 5,280 unique entries; 4,192 outside the store (A 833, D 5,
L 3,025, X 329). E: **0** - every environment read in the closure is
raw `getenv`.

**Stable.** Second warm run: 424 differences, ALL under
`build/aedilis/obiecta/` (first run missed in aedilis's store, second
hit). Outside the store the two traces are identical. The store is
already ejected by the judge (slice 2 T3), so the key is stable.

**Judge cost.** The 3,025 L files = 18.1 MB; `shasum -a 256` over all
of them 0.42 s. Plus ~1.2k stat calls (A/X/D). The §VIII target
(< 2 s when RECENS) holds with room.

**Every entry accounted for:**
| Group | Count | What |
|---|---|---|
| L `~/.cargo/registry`, `~/Documents/projects`, `/opt/homebrew` | 1,674 + 120 | silvestria corpus, pinned by `silvestria.manifestum` (A1: digested) |
| L/X `/Applications/Xcode.app` | 162 + 162 | SDK headers hashed by compilator from depfiles (allowlisted, `identitas_clang`) |
| L `toml/probationes/fixa/**` | 884 | toml-test fixtures (filum) |
| L `toml/fontes`, `include`, `lib`, `materia/fontes` + X same | ~93 + ~115 | compilator's header hashes and include lookups |
| L `toml/build/*.o`, `toml/build/probationes/*.o` + X | 58 + 58 | **compilator's destination check** (v2 correction 1) |
| A (outside store) | 833 | include-root misses (aedilis + compilator) - negative dependencies, the shadowing guard |
| D | 5 | compilator's -I roots (`toml/fontes`, `include`, `materia/fontes`, `toml/probationes`) + source dir `lib` |
| L `build/toml_corpus.lst`, `toml/build/aurum_silvestre.txt` | 2 | bash/python-written inputs with no S - confirms III.4 |
| L `*.toml` across the tree (apps/, book_assets/, probationes/, tools/, briar/) | 13 | the house corpus listed in toml_corpus.lst |
| L `aedilis.stml`, `bin/aedilis`, `toml/grammatica/toml.canon` | 3 | aedilis config + its own bytes (store key); a grammar fixture |
| X `~/.rhubarb/mensurae.volumen` | 1 | **mensor side channel** (v2 correction 3) |

**Absent but read (the gaps):**
- `toml/probationes/fixa/computus/basis.tsv` - computus gold, raw `fopen`
  (read at :211, written at :182 under COMPUTUS_SCRIBERE). Its three
  corpus files appear only because OTHER tests read them through filum.
- `toml/grammatica/toml.registrum.stml` - read raw by
  `materia/fontes/materia_coctor.c:1230` (`_plagulam_legere`, the
  "porta rancoris" that probatio_toml_registrum calls). materia keeps
  filum out of client chains on purpose (comment at :1222).
- 15 raw `getenv` sites (RHUBARB_RADIX x10, HOME x2, COMPUTUS_SCRIBERE,
  ORACULUM_OMNIA, ORACULUM_EXEMPLUM) - `lectiones_lint -omnes`
  intersected with the toml closures (58 files) gives exactly these
  plus the two fopen sites above and totalitas:233 (a write on
  failure only).
- Not readable by design: the bash scripts (III.2), bin/compilator's
  bytes (III.3), clang/ld (identitas_clang), git and python (III.4).

**v2 corrections:**
1. **compilator's destination check (III.4, IX.5).** On a hit,
   compilator reads the existing object to avoid rewriting identical
   bytes; that read is logged L with no S, so a warm run has 58
   "unowned build inputs" and III.4 would make EVERY warm run IGNOTUM.
   The read is output-side: compilator records the destination as
   **S** (this run owns the path) whether it rewrites or not. Change in
   `tools/compilator.c`, step T3.
2. **materia_coctor (V).** New V.6: `_plagulam_legere` stays raw (no
   filum in client chains) and calls `lectiones_notare(LECTIO_LEGIT,
   via)` / `LECTIO_ABSENS` itself - lectiones.c has no dependencies.
   `tools/lectiones_lint.sh` must accept a raw read whose function also
   calls `lectiones_notare` (nexus: same function, both callees);
   otherwise the pilot rule blocks it. Step T3.
3. **Side channels (III.7).** Only mensor shows up (one X, stable, on
   `~/.rhubarb/mensurae.volumen`); sera and the tee log are bash, not
   traced. `tools/mensor_suitae.sh` runs bin/mensor with
   `FABRICA_LECTIONES` unset anyway - the volume is output, never an
   input. Step T3.
4. **Executed binaries (III.3).** bin/aedilis reads its own bytes into
   the trace (store key), so it is covered twice; bin/compilator is not
   - the declared `instrumentum` stands.

Verdict: every entry explained, the gaps are a closed list (3 raw reads
+ 15 env reads + 2 bash-written files), the trace is stable. The design
is ready, with corrections 1-4 folded into T3.
