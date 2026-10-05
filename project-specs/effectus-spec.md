# effectus — spec (what a script touches, said once for every language)

*Born 2026-10-05 from the effectus interview
(`project-specs/effectus-interview.md`, Q1–Q11) and the dynamic spike
the same day (§0). Predecessor: `fabrica-spec-3.md` (§III.2
fontationes, §III.4 the build/ ownership rule, §XIV as built). Question
that prompted it: fabrica slice 3 keys a gate's pass on what it read,
but bash's OWN reads (`$(cat f)`, `< f`, `[ -f x ]`, globs) never reach
the read ledger - only the 1-in-N audit catches them (iudicium-fumus
AUD). Fran's framing: build it as a pilot for effect analysis in C
(silva) and other execution languages; vocabulary language-neutral,
bash first.*

*Status: APPROVED (2026-10-05) - §XI A1-A5 decided by Fran (all recommendations); plan next.*

## 0. What the measurements say (dated 2026-10-05)

**The dynamic spike** (scratchpad code, not kept; ledger nota …ZT1).
A dylib loaded with `DYLD_INSERT_LIBRARIES` into Homebrew bash
(`/opt/homebrew/bin/bash` 5.2, not SIP protected) interposes open,
openat, stat, lstat, access, fstatat, faccessat, opendir and execve
through the `__DATA,__interpose` section and appends
`pid prog genus via rc` lines to a log.

- **Seen directly:** every effect bash makes itself - `<` reads, `>`
  writes, `source`, `[ -f x ]` (present AND absent), glob listings
  (OPENDIR), exec of a path. House binaries and Homebrew python are
  seen too.
- **Not seen:** effects inside SIP-protected binaries (`/bin/cat`,
  `/usr/bin/sed`, `/usr/bin/git`): dyld strips `DYLD_*` for them.
  Copying `/bin/cat` out of `/bin` does not help - the arm64e copy is
  killed at launch (rc 137, also after an ad-hoc re-sign).
- **Fix 1 - argv.** The interposed execve logs each argument. `cat
  build/vexilla.sigillum` then becomes a read when interpreted by the
  command table (§III) - the same table the static analyzer uses.
- **Fix 2 - shebang redirect.** A script starting `#!/bin/bash` (233
  of the tracked `.sh` files; 9 `#!/bin/sh`, 2 `#!/usr/bin/env bash`)
  would run under SIP bash and drop the injection for its whole
  subtree. The interposed execve reads the first line and re-execs
  `/opt/homebrew/bin/bash <script> <args>` instead. Measured:
  fabrica → `toml/corpus_indicem.sh` stayed visible.
- **On a real runner** (`toml/compile_probationes.sh registrum`,
  1/1): 74,008 log lines. ~58k are house binaries' include probing
  (aedilis, compilator) - excluded, they have their own read ledger.
  Bash-only, minus system roots, PATH lookups and `.`: **62 distinct
  effects, every one explained**. Both seed lint sites appear (`cat`
  of the 13 closure lists; `cat build/vexilla.sigillum`), plus a `<`
  read of `probatio_toml_registrum.lst`.
- **The oracle is a LOWER bound.** sera.sh's four lock reads never
  appeared: no contention, so the code never ran. Static = everything
  that could run (upper bound); dynamic = what ran (lower bound). A
  dynamic effect the static summary lacks is an analyzer defect; a
  static-only site is unexecuted or a false positive.
- **Noise rules found:** the PATH filter needs the PROCESS's PATH,
  not the caller's (Fran's PATH has `…/Application Support/Herd/bin`,
  with a space); relative paths need each process's cwd; bash stats
  `.` 149 times per run.

**Command census** (approximate: first literal word of each
`imperium` in crusta STML over 292 tracked `.sh` outside `oracula/`;
the real census is T2). 16,439 command sites, ~1,027 distinct first
words (the heuristic also counts fragments like `/build` after a
parameter). File-touching externals by sites (files): grep 500 (94),
cat 154 (54), head 150 (94), wc 137 (53), rm 132 (87), sed 126 (42),
sort 126 (42), find 108 (87), git 86 (30), cp 67 (37), ls 61 (49),
cut 58 (28), tail 51 (23), cmp 45 (27), mktemp 40 (35), diff 33 (19),
perl 30 (7), comm 29 (14), readlink 27 (26), jq 23 (8), plus awk 188
(65), mkdir 295 (174). Pure: echo, printf, dirname, basename, tr
(stdin only), true, `:`.

**Seed examples** (scratch lint, 2026-10-05; nine hits in toml's
15-script chain):
- `tools/cursor_communis.sh` 78, 91, 102: closure lists written by
  the same run (`> "$CLAUSURAE_DIR/$name.lst"`, line 58) - written in
  scope, not inputs.
- `tools/sera.sh` 41, 47, 48, 99: lock bookkeeping, written by
  `sera_capere` in the same scope.
- `tools/vexilla.sh` 54: `build/vexilla.sigillum` is **read, compared,
  then rewritten** (line 56) - a cross-run state file. Flow-insensitive
  "written in scope" calls it produced-by-this-run, which is wrong
  about it as an input (§IX.1).

**What exists to build on:**
- `crusta/fontes/crusta_fontationes.c` (1,910 lines) - the symbolic
  evaluator: process scopes with a fixpoint, the house path idioms
  (`$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)`, dirname,
  basename, readlink -f), `local` scoping, `X=1 cmd` not a definition,
  cwd from the last `cd` in the same catena. 13/14 suite runners
  resolve fully (worklog 2026-10-03).
- crusta's exemplar lint (`crusta/lintrum/*.stml`, `crusta_facies`,
  `materia_exemplaria`): structural match, text by equality only,
  `<tolera>` excuses scoped to the owning grammar node.

## I. Framing

**effectus** is the summary of every place a script reads, writes,
executes, sources, lists or tests a file, or reads an environment
variable - each site with its evaluated path. It is emitted as STML in
a dialect of its own, judged by a canon, so a bash emitter (crusta,
now) and a C emitter (silva, later) answer to the same rules.

Decided (interview):
- Bash summary + exemplar lint over it; fontationes becomes a client
  (Q1).
- The dynamic oracle first (Q2) - done, §0.
- Consumers: the lint (blocking in verdict chains, warning elsewhere)
  and a fabrica ingressus genus (Q3, Q11).
- Flow-insensitive: facts per process scope, no ordering (Q4).
- STML dialect + canon, written here (Q5).
- The external-command table is data, seeded by measuring the house
  (Q6).
- An unresolved read in a verdict chain is IGNOTUM unless excused
  (Q7).
- Every tracked `.sh` is summarized as data; blocking only in verdict
  chains (Q8).
- Excuses are `<tolera>`, scoped by grammar node (Q9).
- The full census is a generated file; the ledger holds only decisions
  (Q10).
- New fabrica genus `effectus`; `fontationes` retired (Q11).

**The value rule.** Exemplar rules match by structure and text
equality - no expression language, ever (Canon decree). Every question
about a VALUE ("is this path under build/?", "does this scope also
write it?", "did it resolve?") is answered by the analyzer as an
ATTRIBUTE. Rules then compare attributes by equality.

**Soundness rule (Fran, 2026-10-05).** A flow-insensitive analysis is
safe only while it OVER-approximates inputs. No fact it computes may
remove anything from the fabrica key; facts may only quiet the lint.
Precision that shrinks the key is slice 2's job (below).

Out of scope: C or any other emitter (vocabulary only); effects inside
external commands beyond what the command table says; `eval`, `xargs`,
`find -exec` bodies (they are `ignotum` sites).

**Slice 2 - flow (named, not planned).** Control-flow / ordering is
deferred (Q4) to a second slice and spec, built ON this summary: it adds
ordering attributes to existing sites (e.g. `scripta_ante="verum |
falsum | ignotum"`: a write on every path before this read) and
reaching definitions (more `$X` resolved where fontationes gives up on
plural definitions). Its job is precision - shrinking the key - never
soundness, which slice 1 already owns. Seeded by two lists slice 1
produces as by-products: the oracle's read-before-write warnings
(§VI.2) and the census's unresolved sites (§VI.1). Its first reading:
what the house's C flow-analysis tools already produce, so bash and C
answer ordering questions in one vocabulary.

## II. The dialect `effectus`

### II.1 Shape

```xml
<effectus lingua="bash" radix="toml/compile_probationes.sh"
          registrum-sigillum="873ce8f4">
  <processus radix="toml/compile_probationes.sh">
    <fontatio via="tools/cursor_communis.sh" resolutio="plena"
              classis="arbor" per="aedificium"
              plagula="toml/compile_probationes.sh" linea="31"
              initium="1104" finis="1150"/>
    <lectio via="toml/build/clausurae/*.lst" forma="globus"
            resolutio="plena" classis="build" scripta_in_ambitu="verum"
            per="mandatum" mandatum="cat"
            plagula="tools/cursor_communis.sh" linea="78" .../>
    <probatio via="build/vexilla.sigillum" operator="-f" .../>
    <enumeratio via="toml/probationes/" forma="globus" exemplar="*.c" .../>
    <scriptura via="toml/build/cursor.sera/pid" .../>
    <exsecutio via="bin/aedilis" classis="instrumentum_domus" .../>
    <ambitus_lectio titulus="FABRICA_LECTIONES" assignatum="falsum" .../>
    <ignotum causa="mandatum ignotum" mandatum="jq" .../>
  </processus>
  <processus radix="toml/corpus_indicem.sh"> ... </processus>
</effectus>
```

One `<processus>` per process scope: the root script plus everything it
sources (they share variables). An EXECUTED script opens a new scope
(fontationes' rule, worklog 2026-10-03).

### II.2 Kinds (elements)

| element | meaning |
|---|---|
| `lectio` | file content read: a `<` redirection, or a command's read argument (table) |
| `scriptura` | file written/created/removed: `>`, `>>`, `rm`, `cp` dest, `mkdir`, `touch` |
| `exsecutio` | a file run as a program (path, or a house binary) |
| `fontatio` | a file read as code into this scope (`source`, `.`) |
| `enumeratio` | a directory listed: glob, `ls`, `find` root |
| `probatio` | existence/type/time tested: `[ -f ]`, `[[ -d ]]`, `-nt`, `test` |
| `ambitus_lectio` | an environment variable read that the scope did not assign |
| `ignotum` | an effect site whose effect is unknown (command not in the table, `eval`, unresolvable cwd) |

Names are language-neutral on purpose: C's `fopen(p, "r")` is a
`lectio`, `getenv` an `ambitus_lectio`, `opendir` an `enumeratio`.

### II.3 Attributes (the computed answers)

| attribute | values | computed as |
|---|---|---|
| `via` | path relative to the tree; absolute outside it | evaluator (§IV) |
| `forma` | `via` · `globus` · `praefixum` | plain path; glob pattern; known prefix of a partial value |
| `resolutio` | `plena` · `partialis` · `nulla` | every part known; prefix known; nothing |
| `classis` | `arbor` · `build` · `instrumentum_domus` · `externa` · `systema` | tree source; under any `build/`; `bin/` house binary; absolute outside the tree; system roots (§VI.2) |
| `scripta_in_ambitu` | `verum` · `falsum` | some `scriptura` in the same `processus` covers this path (equal, or glob/prefix match) |
| `per` | `redirectio` · `aedificium` · `mandatum` | operator; bash builtin/syntax; external command via the table |
| `mandatum` | command title | when `per="mandatum"` |
| `operator` | `-f`, `-d`, `-nt`, `<`, `>>`, … | the literal operator |
| `assignatum` | `verum` · `falsum` | `ambitus_lectio`: the scope also assigns it (then it is not an input) |
| `plagula`, `linea`, `initium`, `finis` | | site position; `initium`/`finis` = byte range of the owning crusta node (excuses, §V.2) |

`scripta_in_ambitu` is flow-INSENSITIVE (Q4): it says "this scope also
writes it", not "it was written before this read". It quiets the lint
(§V.1) and never removes a path from the fabrica key (§I soundness
rule, §VII, §IX.1).

### II.4 Canon
**`effectus.canon` at the tree root** (the dialect is not crusta's), beside `aedificatio.canon` and
`fabrica.canon` (the house's other dialects). Hand-written, every
element and attribute above, `electio` lists closed. A drift guard in
crusta's gate judges every emitted summary against it (T1, T3).

## III. The command table

Data, not code: `crusta/effectus_mandata.stml` (bash-specific - a C
emitter will have a call table of its own), with its own small canon
section in `effectus.canon`.

```xml
<mandatum titulus="cat"     argumenta="lectio"/>
<mandatum titulus="head"    argumenta="lectio" optiones_cum_valore="-n -c"/>
<mandatum titulus="cp"      argumenta="lectio" ultimum="scriptura"/>
<mandatum titulus="grep"    argumenta="lectio" primum="exemplar"
                            optiones_cum_valore="-e -f -m -A -B -C"
                            optio_lectio="-f"/>
<mandatum titulus="mkdir"   argumenta="scriptura"/>
<mandatum titulus="find"    primum="enumeratio" reliqua="ignotum"/>
<mandatum titulus="dirname" purum="verum"/>
<mandatum titulus="git"     ignotum="verum" causa="index et obiecta .git"/>
```

- Roles by POSITION among non-option words: `argumenta` (all), `primum`,
  `ultimum`, `reliqua`; `optiones_cum_valore` lists options that eat
  the next word; `optio_lectio` an option whose value is read.
- `purum="verum"`: no file effect.
- `ignotum="verum"`: known to touch files in ways the table does not
  model; every site is an `ignotum` with the table's `causa` - excused
  in place or declared by hand (`git` reads `.git/`: the toml corpus
  index is already an action for exactly this reason, spec 3 §III.4).
- A command NOT in the table = `ignotum` "mandatum ignotum". The table
  grows from these (the examples loop, desideratum …Q4C7D).
- Seeded (T2) from the precise crusta census: every command title with
  ≥ N sites; the rest stay unknown until a verdict chain meets them.
- Command words that are house functions (`sera_capere`,
  `mensor_suitae_nunc`) are NOT commands: the analyzer knows the
  scope's function definitions and descends into them (fontationes
  already walks the whole scope).

## IV. The analyzer

### IV.1 Home
`crusta/fontes/crusta_effectus.{h,c}`: the fontationes evaluator moved
and generalized. `crusta_fontationes_derivare` becomes a projection of
the summary (its `fontatio` and `exsecutio` sites) with its existing
gate unchanged - the equivalence proof (T4).

```c
/* Summarium effectuum scripti (et omnium quae fontat/exsequitur). */
StmlNodus*
crusta_effectus_derivare (
               Piscina*  piscina,
    constans character*  radix,
    constans character*  scriptum,
    constans character** causa_out);
```

CLI: `bin/effectus <script>` prints the STML; `bin/effectus -census`
writes the census (§VI.1).

### IV.2 Sites
- **Redirections**: `<` `<>` → lectio; `>` `>>` `>|` `&>` → scriptura;
  `<<` / `<<<` → nothing. `/dev/null`, `/dev/stderr`, `&N` → nothing.
- **Builtins / syntax**: `source`/`.` → fontatio; `[ ]`, `[[ ]]`,
  `test` file operators → probatio (`-nt`/`-ot` → probatio on both
  sides); unquoted glob in any word → enumeratio of its directory;
  `cd W` → nothing (cwd only); `read` → nothing (its `<` is the site);
  `mapfile`/`readarray` likewise; `eval` → ignotum.
- **Command words**: a path (has `/`) → exsecutio; a title in the
  table → its roles; a scope function → descend; otherwise ignotum.
- **Process substitution** `<( … )`, `$( … )`: their commands are sites
  of the same scope (bash forks, but variables are inherited and
  writes from a subshell still hit the disk).
- **Environment**: `$X` / `${X…}` where X has no definition in the
  scope → ambitus_lectio (`assignatum="falsum"`); bash specials (`$?`,
  `$#`, `$@`, positional, `$$`, `BASH_SOURCE`, `FUNCNAME`, `LINENO`,
  `PIPESTATUS`, `RANDOM`) → nothing.

### IV.3 Values
fontationes' resolution rules unchanged (header of
`crusta_fontationes.h`), plus:
- **Globs**: a word with an unquoted `*`, `?` or `[` after evaluation
  keeps the pattern: `forma="globus"`, `via` = the pattern.
- **Partial**: a known prefix is kept (`forma="praefixum"`), as
  fontationes already does for build/ products.
- **cwd**: the last `cd W` in the same catena; else the last
  top-level `cd W` of the scope BEFORE the site in source order; else
  the tree root. (fontationes uses only the first and third; reads are
  more sensitive to cwd than sources were. Source order is not flow -
  a `cd` inside an `if` does not count.) A `cd` to an unresolved dir →
  the site's `resolutio="nulla"`, causa "cwd ignotum".

## V. The lint

### V.1 Rules
Exemplar rules in `crusta/lintrum/` match the SUMMARY, not the crusta
tree. Seeds (counts measured in T6, not guessed here):

| rule | matches | gravitas |
|---|---|---|
| `effectus-irresolutum` | `lectio`/`fontatio`/`exsecutio`/`probatio` with `resolutio="nulla"` | §V.3 |
| `effectus-build-sine-domino` | `lectio classis="build" scripta_in_ambitu="falsum"` | §V.3 |
| `effectus-mandatum-ignotum` | `ignotum` | §V.3 |

`effectus-build-sine-domino` is the static twin of spec 3 §III.4 (a
build/ read must be written by the run or be a declared exitus). The
lint cannot see fabrica's exitus list; the fabrica genus (§VII) can,
so the lint warns and the key decides.

### V.2 Excuses
`<tolera codex="effectus:RULE" (>causa` in a bash comment, scoped by the
owning crusta node as today: the summary site carries its node's byte
range (`initium`/`finis`), and the facies filter excuses a site whose
range lies inside an annotated node's range in the same `plagula`. One
excuse above `sera_vetus () {` covers its three reads (Q9). A dead
excuse is a defect, judged when its rule ran (crusta lintrum law).

### V.3 Gravitas
Blocking (`erratum`) only for scripts in a VERDICT CHAIN - reachable
from an `iudicium` action's `effectus` ingressus; `monitum` elsewhere
(Q8). The set comes from fabrica (`bin/fabrica catenae`, AUDIENDA A4).

## VI. Census and oracle

### VI.1 Census
`bin/effectus -census` → `build/effectus/census.tsv`: one row per site
over every tracked `.sh` (plagula, linea, genus, per, mandatum,
resolutio, classis, scripta_in_ambitu, excused-by). Regenerable,
diffable, never committed (Q10). The ledger gets a row only for a
DECISION: an excuse class, a table entry added for a reason, a false
positive class with its negative fixture.

### VI.2 The oracle (house tool)
The spike's interposer, rewritten as house C89 in Latin
(`crusta/instrumenta/interpositio.c`, macOS-only, behind the platform
line - AUDIENDA A2), plus `crusta/effectus_oraculum.sh`:
- runs a script under Homebrew bash with the interposer (argv +
  shebang redirect), the log carrying each process's cwd and PATH at
  start;
- normalizes the log INTO the effectus dialect (same elements, same
  `via` rules; `per="observatum"`); commands seen only by argv are
  interpreted through the same table;
- filters: system roots (`/usr`, `/System`, `/Library`, `/bin`,
  `/sbin`, `/dev`, `/private/var/db`, `/opt/homebrew/{Cellar,lib,opt}`),
  the process's PATH lookups, `.` and cwd ancestors, house binaries'
  own pids (their reads are in the read ledger);
- compares: every observed effect must be COVERED by a static site of
  the same kind (equal path, glob match, or prefix) - an uncovered one
  is printed as an analyzer defect, by name.

Like crusta's `oraculum.sh` (C14): goldens for fixed fixture scripts
(`crusta/probationes/fixa/effectus/`) are GENERATED by the oracle and
committed; the gate compares the static summary to the goldens and
spawns nothing. `-domus <runner>` runs live, by hand.

Oracle-only knowledge: the dynamic log HAS order, so it can name a
read-before-write within one pid (§IX.1) - the cases flow-insensitive
analysis gets wrong. Printed as a warning and kept as a list
(`build/effectus/ante_scripta.tsv`); v1 does not act on it - it is
slice 2's example corpus.

## VII. fabrica genus `effectus`

`<ingressus genus="effectus" via="toml/compile_probationes.sh"/>`
replaces `genus="fontationes"`. Derived at judge time, every time; its
digest joins the key:

| site | digested as |
|---|---|
| fontatio, exsecutio, lectio in `arbor` | file bytes |
| exsecutio `instrumentum_domus` | provenance (as `instrumentum_domus` today) |
| lectio `build` with `scripta_in_ambitu="verum"` | file bytes, STILL in the key (soundness rule, §I): at judge time the file holds what the last run left, so the digest matches unless something outside the run changed it - which is the case that must re-run (`build/vexilla.sigillum`). Cost: one re-run after `build/` is deleted. |
| lectio `build`, otherwise | the declared exitus owner's digest, else IGNOTUM "ingressus build/ sine domino" (§III.4) |
| lectio/fontatio `externa` | file bytes (spec 3 decision A1) |
| `systema` | pinned by `identitas_clang`, not digested |
| enumeratio | sorted names matching the pattern |
| probatio | existence + type (+ mtime for `-nt`) - ABSENCE included |
| ambitus_lectio `assignatum="falsum"` | the variable's value at judge time (AUDIENDA A3) |
| `resolutio="nulla"` or `ignotum`, not excused | IGNOTUM, naming the site |
| excused | nothing (the excuse is the decision) |

When nothing declares `fontationes` any more, the genus and its code
path are removed (T8).

## VIII. Order

- **T1 Dialect.** `effectus.canon` + three hand-written summaries
  (pipeline, the toml runner fragment, one of every element) judged
  clean; a plant (unknown attribute value) red.
- **T2 Census of commands, precisely.** Through crusta (command titles
  per site, functions separated from externals), seed
  `crusta/effectus_mandata.stml`; canon section; the census numbers
  recorded in this spec's §XII.
- **T3 Analyzer.** `crusta_effectus` + `bin/effectus`: every §IV.2
  site class with a fixture case first (born red), attributes, the
  canon drift guard over every house summary.
- **T4 fontationes as a projection.** Its gate unchanged and green;
  the duplicated evaluator code deleted.
- **T5 Oracle.** House interposer + `effectus_oraculum.sh`; fixture
  goldens; `-domus` on toml's chain: zero uncovered, or each one
  named and fixed in T3's code.
- **T6 Lint + census.** The three rules with positive and negative
  fixtures; excuses at sera.sh and vexilla.sh; `bin/effectus -census`;
  counts recorded.
- **T7 fabrica genus `effectus`.** porta_toml switches; iudicium-fumus
  gains: `flag.txt` read by `$(cat flag.txt)` edited → **STALUM
  directly** (today only the audit catches it); `source "$NESCIO"` →
  IGNOTUM (P7 kept); a new `[ -f absent ]` file created → STALUM; a
  read-and-written build/ state file edited between runs → STALUM
  (soundness rule).
- **T8 Retire `fontationes`.** Genus removed; measure judge time; §XII
  As built; worklogs; memory.

## IX. Review focus (failure modes no step above tests by default)

1. **Read-before-write state files.** `build/vexilla.sigillum` is read,
   compared, rewritten: a cross-run state file. `scripta_in_ambitu=
   "verum"` would call it "produced by the run" - wrong as an input.
   Slice 1 stays sound anyway: the attribute only quiets the lint, and
   the key still digests the file (§VII). The oracle lists every
   read-before-write it observes (§VI.2); that list seeds slice 2
   (§I), which may then drop truly produced files from the key.
   Plant for T7: edit `build/vexilla.sigillum` under a RECENS pass ->
   STALUM.
2. **The table is checked only by itself.** The oracle interprets SIP
   commands' argv through the SAME table, so a wrong row (a command
   marked `purum` that reads) is invisible to both. Mitigation: rows
   for commands Homebrew also ships unprotected (`gsed`, `gcat`
   from coreutils) can be checked by running those under the
   interposer - a named limit, not a step.
3. **Over-approximation churn.** `probatio` of absent files, env reads
   (`HOME`, `PATH`) and globs move the key more often than reads do.
   T7 measures how often the toml key changes across a day of commits
   versus slice 3's key.
4. **cwd.** §IV.3's source-order rule is wrong for a `cd` inside a
   loop or function called earlier. The oracle compares paths after
   its own cwd tracking; uncovered effects from cwd mistakes show up
   there.
5. **Excuse rot.** An excuse outlives the read it excused. Dead
   excuses are defects only when their rule ran (crusta law) - the
   census must run the effectus rules over every file, not only
   verdict chains.
6. **Summary cost at judge time.** fontationes derived toml's chain in
   well under the 2 s budget (spec 3 §XIV); a full summary with the
   table and globs is more work. T7 measures; budget unchanged.
7. **Subshell writes.** `( cd x && cmd > f )` writes relative to the
   subshell's cwd - covered by the catena-cd rule only when the `cd`
   is in the same catena.

## X. Done means

- `bin/effectus` summarizes every tracked `.sh`; zero canon violations;
  zero crashes (totality inherited from crusta).
- The oracle on toml's whole chain: every observed bash-level effect
  covered by the static summary.
- iudicium-fumus: a `$(cat flag.txt)` edit is STALUM without an audit.
- The three lint rules with fixtures; toml's chain clean or excused;
  census generated.
- Genus `fontationes` gone; toml judge time still under 2 s.

## XI. AUDIENDA — DECIDED (Fran, 2026-10-05: every recommendation)

- **A1. Names - DECIDED as written.** Dialect `effectus`; elements `lectio` `scriptura`
  `exsecutio` `fontatio` `enumeratio` `probatio` `ambitus_lectio`
  `ignotum` inside `processus`; attributes in §II.3; tool
  `bin/effectus`; `crusta_effectus.{h,c}`; table
  `crusta/effectus_mandata.stml`; canon `effectus.canon` at the root;
  rules `effectus-*`.
- **A2. The interposer's home - DECIDED (a).** (a) Recommended: house C89 in Latin
  under `crusta/instrumenta/`, macOS calls behind a small platform
  header - it is ours and long-lived; (b) `oracula/` (foreign-dialect
  glue, excluded from the Latin lint by name) - quicker, but it is not
  foreign glue.
- **A3. Environment reads in the key - DECIDED (a).** (a) Recommended: digest the
  value of every unassigned variable read, and measure churn in T7
  (PATH/HOME rarely change within a session); (b) digest only a
  named list; (c) record but never key on them (blind spot named).
- **A4. Which scripts block - DECIDED (a).** (a) Recommended: derived - scripts
  reachable from any `iudicium` action's `effectus` ingressus (a
  `bin/fabrica catenae` listing the lint reads); (b) a hand list in the
  lint config.
- **A5. `bin/fontationes` - DECIDED (a).** (a) Recommended: keep the CLI as a thin
  projection while anything calls it, delete with the genus in T8;
  (b) delete in T4.

## XII. As built

(empty until T8)
