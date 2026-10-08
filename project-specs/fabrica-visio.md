# Fabrica — Visio (the judge as a tool: model, primitives, chassis, volumes)

Consolidated 2026-10-07 from a design conversation (Fran + Claude)
held right after fabrica slice 5 closed (`project-specs/fabrica-spec-5.md`
§XI) and the pythonica speed work (S1-S3). The question that prompted
it: fabrica began as "a makefile replacement" - if a new project used
it for its build, is there a clean model of what it is and how to use
it, what are the rough edges, and should our bash runners eventually be
written in a vocabulary of our own? Status: **RESERVATUM** - written
down ahead of implementation; the interview (§VII) decides what is
built. Every lesson in §II comes from something that actually happened
in this codebase; the citations are the evidence.

*Discipline note.* PULL rule: a primitive, step kind or plugin
graduates when a consumer needs it. This document names the shape so
that what we add one at a time lands in one system, not many.

## I. The model

Make asks: is the target older than its prerequisites? fabrica asks:

> **Do I have evidence that this artifact equals f(today's inputs)?**

Each action declares an artifact, a recipe, and a strategy for
producing that evidence. There are four, plus one honest non-answer:

| evidence | how | today |
|---|---|---|
| recompute and compare | run the recipe in a scratch area, compare bytes | `provenientia="regeneratio"` |
| remembered check | "compared once with exactly these inputs" (key of inputs + recipe) | `memorabilis="verum"` |
| self-describing artifact | the artifact carries the digest of its own inputs | `provenientia="relatio"` (`-provenientia`) |
| recorded trace | what the run actually read, sealed, replayed by the judge | `provenientia="verdictum"` (`genus="iudicium"`) |
| (none) | run it first; never judged | `provenientia="ignota"` (precondition only) |

Operations follow from the question: **judge** (`iudicare`, never
builds), **heal** (`sanare`, in dependency order), **explain**
(`causae`: why did it re-run). Make's mtime model is the degenerate
case - one strategy, and an unreliable one.

Two postures to keep: **unknown is never RECENS** (IGNOTUM with a named
reason), and **refusals explain themselves** (the named particle, the
read without an owner, the write outside the area).

## II. What the codebase taught - primitives

Each primitive is argued for by incidents, not by taste.

**1. Places with owners; a scratch area handed to every step.**
Root tests wrote 28+ scratch files across build/, two wrote helper
scripts beside installed binaries, one deleted the residents' real
commit stamp `.vigilia_commissum` (spec 5 §X T5b items 1-2). Daemons
and sqlite write without noting anything (§X T5b item 4). Rules that
came out of it: reads inside an action's own area are its product;
temporary roots are never inputs; every build/ read needs an owner.
Primitive: the tool gives each step its own scratch place (path or
volume, §V) and treats any write outside declared places as a refusal.
Desideratum …SFQW is the house-level symptom.

**2. A hermetic environment by default.**
`dict(os.environ, ...)` would have keyed verdicts on
`TERM_SESSION_ID` (spec 5 §X T10); `HOME` and `~/.rhubarb` leaked into
traces (§X T11). Primitive: a step sees declared environment variables
plus a minimal base, nothing else. E keys become small and stable, and
"what does this depend on" has an honest answer.

**3. Input kinds on two axes: what it is × how it is keyed.**
Real cases: content (most); existence (the speculum capsule, §X T5b
item 7); names matching a pattern (new test files); resolution along a
search path (include shadowing, §X T2); a C closure (aedilis
manifests); a tool by provenance, not bytes (relinking changes
LC_UUID); toolchain identity (clang, python3); **repository state**
(HEAD, index - `probatio_git` and the pythonica snapshots, §X T11).
Today's kinds mix the axes (`fasciculus`, `instrumentum`,
`instrumentum_domus`, `manifestum`, `manifesta`, `radices`,
`identitas_clang`, `effectus`). Repository state deserves to be an
explicit kind: a gate that depends on HEAD is then visibly a
never-reusable gate, not discovered after the fact.

**4. Steps, not runners.**
radix runs 237 tests as one unit; one changed test voids all of them
(§XI). Per-step keys - compile, link, run-with-signal,
generate-and-compare, install, foreach over a declared set - are where
reuse actually lives. A runner becomes a group of steps.

**5. Snapshot builds by default.**
"Freeze the tree while gates run" (MEMORY), the formatter rewriting
sources and staling installed binaries mid-commit (T5b recommit), and
the shadow-pass park (…2VP7) are one problem: builds read a moving
tree. clonefile made a snapshot ~3 s (S3: 13.4 -> 2.6 s per snapshot).
Primitive: judge and heal against a frozen snapshot, record results
for that tree state. Shadow passes stop being a special feature.

**6. Content-addressed caching only; mtime nowhere.**
Same-second mtime equality (oratio runner comment), `git checkout`
re-stamping latina.h and staling all of oratio's objects (T7a),
`struere.sh` and installers judging by `-nt`, needless relinks. The
compilator store is the model and should be the only cache.

**7. Capabilities on steps.**
`GUI_LISTA`, `RETICULARIS_LISTA` and `REPOSITORIUM_VIVUM_LISTA`
(`tools/aedilis_porta.sh`) are three ad-hoc versions of one idea:
this step needs a display / the network / the live repository.
Declared capabilities (cf. the `facultates` desideratum) tell the tool
what it may cache, run headless, or run in a snapshot.

**8. Bootstrap order for the judges themselves.**
"iudex ipse non recens" (bin/fabrica stale judging itself, T5a
recommit), aedilis built from amalgams that aedilis judges. Primitive:
the judges (fabrica, compilator, aedilis) are a declared first stage
that must be fresh before anything else is judged.

**9. One graph - gates and debts included.**
Gates live in `PORTAE` (Python), debts in a hand-kept inventory of
`tegit viae` globs, artifacts in `aedificatio.stml`; the inventory and
the declarations disagreed (generata owed nothing for its own inputs,
§X T7a) until `iudicare -plenus -tacta` - a reverse-dependency query
over the declared graph - was recognized as the real answer. If gates
are verdict actions in the graph, "which gates does this change owe"
is a query, and `PORTAE` and the lenses become derived views.

**10. Determinism as a checked property.**
The speculum capsule embeds time and HEAD (…X77ZBE); relinked binaries
differ in LC_UUID; `regeneratio` means nothing for a non-reproducible
generator. Primitive: "regenerate twice and compare" per generator,
and declared volatile fields where volatility is intended (provenance
stamps).

**11. Measurement and trust built in.**
`causae` (spec 5 T1), per-action timings (`cursus`), audit sampling of
RECENS passes (`-audit`), retro reuse (`tools/reusus_retro.sh`), the
profiler (`pythonica/profilare.py`) - each was added late and paid for
itself at once. In the tool they are features, not add-ons.

## III. Steps instead of scripts - a recipe vocabulary

"Backporting bash" (effectus slices 1-3, spec 5) reverse-engineers
intent that a declarative recipe would state. Our runners mostly do:
compile a C file (through compilator) and link; run a test binary and
check its signal line; regenerate a file and compare; copy/install;
iterate over a declared file set; pick a platform variant. As step
kinds in an STML dialect (canon-checked), each knows its reads and
writes by construction: no trace, no effectus, no "sine domino", and
caching and parallelism per step (as compilator already does per
object). Sketch, not syntax:

```
<actio titulus="probationes_radicis" genus="iudicium">
  <pro_omni exemplar="probationes/probatio_*.c" nomen="p">
    <clausura fons="{p}"/>
    <nectere fons="{p}" exitus="build/probationes/{p.basis}"/>
    <currere binarium="build/probationes/{p.basis}" signum="Fracti:     0"/>
  </pro_omni>
</actio>
```

Principles:
- **Declarative only.** No conditionals beyond declared variants;
  loops only over declared sets. The canon rule ("no expression
  language, ever") holds; the failure mode is a second shell.
- **The escape hatch stays, priced.** A raw `<mandatum>` step is
  judged by trace or effectus, or IGNOTUM - and the share of a
  project's graph running through escape hatches is a reported number
  to drive down, like the effectus census.
- **Granularity is the value.** A step-level key lets one changed test
  leave the other 236 reused.
- **Migration is measurable.** The effectus census already says which
  scripts are mostly these step kinds; "backport" becomes "translate,
  then delete the script" (generata retired the same way, T7c).

## IV. The chassis - growing the vocabulary empirically

Fran's framing: discover the vocabulary of inputs, steps, queries and
commands empirically, and give every addition a chassis that requires
the right properties; an addition that does not fit is itself a design
signal.

The seed exists: `FabricaGenus` (`include/fabrica.h:415`) is a vtable -
`titulus`, `sigillare` (seal into named particles), `enumerare` (places
read, for `-tacta`), `locare` (places written), a determinism flag.
Every input kind added so far is an entry. The work is to make the
chassis explicit and enforcing, and to give step kinds, evidence
strategies, queries and commands the same treatment.

Contract (each row from an incident):

| property | why |
|---|---|
| seals into **named particles**, not one opaque hash | "ingressus mutatus: X" became possible only with named particles (T1) |
| **declares its reads** statically, or marks itself *dynamic* (traced) | debts, `-tacta`, reverse dependencies (T7a gap) |
| **declares its writes** | the photograph found 418 undeclared writes (T5b) |
| **declares determinism**, checked by running twice | LC_UUID, the speculum stamp |
| **can refuse**: IGNOTUM with a reason, never default RECENS | every real T5b gap surfaced as a refusal |
| **ships its own plant**: its input class changed -> judgment flips and names it; unrelated change -> RECENS | `iudicium-fumus` made mandatory; the chassis runs every plugin's fixture like a trait conformance test |
| **cost class** | `celer`, scheduling |

The signal: a candidate that fails a property fails a *named*
property - "cannot declare reads" = a dynamic kind, "not
deterministic" = volatile, "cannot name particles" = coarse. A census
of plugins by properties met tells us where the vocabulary is weak,
the same move as the effectus census of unresolved sites. Queries
(`causae`, "what does this path affect", "which gates are owed") and
commands (`iudicare`, `sanare` over a selection) are plugins over the
same graph.

House tools enter as **libraries, not processes** (Fran): aedilis,
compilator, effectus, lectiones linked in. Today they cross process
boundaries (aedilis spawned 24 times in one pythonica run before S2;
`effectus.sh -clavis` per judgment).

## V. Volumes - a layered virtual filesystem

`volumen` (`include/volumen.h`) is most of a layered store already: one
SQLite file holding content-addressed blobs (`massae`), an append-only
event log (`acta`), a projection path -> hash (`plagulae`), and a fold
to any sequence number (`volumen_plicam_ad`) - time travel, which is
what layers are. Snapshot = remember a sequence number; copy-on-write =
shared blobs; layer = a later event range or a volume stacked on a
base; bulk delete = unlink one file (against the 719,611 unlinks per
pythonica run measured in T9).

Uses:
1. **Step scratch areas** (§II.1): a layer per step; disposal is
   instant; nothing leaks into build/.
2. **Hermetic tests**: a filum backend routes a test's reads and writes
   into a volume - no `/tmp` collisions, no live-repo or `~/.rhubarb`
   leakage, parallel-safe.
3. **The read ledger for free**: for filum-routed code the VFS is the
   trace - no separate notes, no channel gaps.
4. **Fixtures as volumes**: a test's input world as one committed
   `.volumen` file - a clean answer to …F654M / …M651F.

Limits and cautions:
- **External programs see the real filesystem.** clang, git, bash,
  python3 cannot be pointed at a volume without a FUSE-style mount;
  macFUSE is a kernel extension - not for this house. Steps that spawn
  them use real directories (clonefile makes them cheap); the bridge is
  a fast "materialize this layer into a directory" step.
- **Semantics drift** is the main risk: a mock that differs from the
  real filesystem (rename atomicity, directories, exec bit, symlinks)
  lets tests pass in the mock and fail for real. The house answer is
  the oracle: one filum conformance suite run against both backends,
  and deliberately small VFS semantics.
- **Runtime selection over a build flag**: a backend vtable (a sutura,
  as `FabricaSutura`) lets one test binary run against both backends -
  the oracle again - while production keeps the real backend; a build
  flag may still compile the virtual backend out.

Staged path: (1) filum backend vtable, real filesystem default; (2)
volumen-backed backend + conformance suite on both; (3) one library's
tests on volume fixtures (pilot); (4) volume scratch areas for steps.

## VI. One system

Steps declare their reads and writes (chassis), work in owned
disposable places (volume layers), see a hermetic world (environment +
VFS), are judged by named evidence (§I), and external tools are the
priced exception. Much of it the house grew independently -
`FabricaGenus`, volumen's log and folds, the compilator store, the
effectus census; the design work is mostly recognizing them as one.

## VII. Interview agenda

1. Core vs plugin: what is the chassis, and which input kinds are core.
2. The two-axis input model (what × how keyed); repository state as a
   kind.
3. Step vocabulary: the first kinds, foreach and variants, the escape
   hatch and its reported share.
4. Snapshot builds as the default; what that changes for commissio.
5. Scratch and environment hermeticity; volumes vs directories.
6. Capabilities.
7. Gates and debts in one graph; retiring `PORTAE` and the inventory
   lenses over time.
8. Bootstrap stage for the judges.
9. Pilot: one subsystem `compile_probationes.sh` in the dialect, judged
   per step.

## AUDIENDA (not verified)

- clonefile(2) on directories with symlinks, hard links or extended
  attributes: untested here (S3 tested files, nested dirs, refusals).
- volumen performance for large binaries and for many concurrent
  writers (SQLite's single writer) - not measured.
- Whether every current input kind can declare its reads statically:
  `effectus` and `iudicium` are dynamic today by construction; the
  others are believed static, not audited.
- The step dialect sketch is illustrative; no grammar or canon exists.
- FUSE alternatives on macOS (NFS loopback, File Provider) not
  explored; assumed out of scope.
- Counts and timings are as measured on 2026-10-06/07 in one machine's
  runs; run-to-run noise was up to ±40 s on the pythonica suite.
