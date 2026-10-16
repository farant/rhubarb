# unitates worklog

## 2026-10-15 - born (U3 step 2b, quinta)

Reader of `unitates.stml` (policy) + every `DIR/unitas.stml`
(declarations); answers "what is this file" with a CAUSA per field.
Spec `project-specs/unitates-codicis-spec.md` §VI⅞f; header doc is the
contract.

Design points that are not obvious from the code:

- **Sutura, not disk** (copied from `portae.h`): the lib never opens a
  file. Tests give an in-memory tree; the CLI will give
  `git ls-files -co --exclude-standard` + disk reads. Declarations are
  found in the enumeration, never by walking (a walk needs its own
  "skip build/" list - the very thing the arc removes).
- **Provenance is lazy.** `unitates_legere` reads only the policy and
  the declarations; `quid` reads line 1 of the asked file; `iudicat`
  reads it LAST, after custody/lifecycle/role/language - so asking the
  lint about `vendor/sqlite3.c` (9 MB) never reads it.
- **Inheritance**: custody = declared on the unit, else on the nearest
  ancestor that declares one, else the INNERMOST unit's kind default.
  Lifecycle = declared, else nearest ancestor's, else `viva`. A unit
  nested in `archivum` is archived; a unit with no custody inside one
  declaring `aliena` is foreign even if its own kind defaults to
  domestica.
- **Library beats the directory around it**; backends (`S_macos`)
  alias their base in the stem table (two passes, a backend's base may
  itself be a backend); family children get the family root as parent.
  `probationes/probatio_X.c` -> longest `_`-prefix of X that is a
  library stem, else a directory unit with that title (`pictor`),
  else the innermost directory (root). Library wins a tie with an app
  of the same title (`vicus`).
- **Identity = kind + title** (app vicus and library vicus coexist).
- Values are `chorda`, never enums: custody/lifecycle/role/language/
  usus are open sets declared in the policy; unknown -> refusal with
  the known list. `provenientiae` is the one closed set (`manu
  generatum`), because it is derived from a marker, not declared.

Plants (silva.planta, all red at the intended assertion, restored):
first-match roles, lifecycle inheritance, custody inheritance, backend
suffixes, provenance read in the judge. **The lifecycle plant first
stayed GREEN**: the only archived fixture sat directly in `archivum`,
which declares its own `vita` - inheritance was never exercised. Added
`archivum/materia` (nested, no vita) and `ora` (cura aliena) +
`ora/clay` (none) to the test world. A suite green on day one proves
nothing until a plant turns it red.

Test gotcha: C89 caps a string literal at 509 bytes (`-pedantic
-Werror` -> overlength-strings), concatenation included - the fake
policy file is an array of lines joined at read time.

**Lifetime of the sutura**: `Unitates` keeps the `UnitatesSutura*`
(lazy provenance reads go through it after `unitates_legere`
returns), so the caller's sutura and its datum must outlive the
`Unitates`. The first test put them in a stack frame as `static`;
examen's credo rule refused it (state across sections) - a plain local
would have dangled. Allocate both in the same piscina as the result.

## 2026-10-15 - U4: first consumer (the Latin lint)

The lint had TWO answers, not one: the hook's trigger list
(`lint_latinus.sh` + `silva.py _EXCLUSA_LINTRI`: skip fixa amalgama
archivum knotapel vendor oracula) and the judged scope (nexus index ∩
tracked − `ORATIO_VOCABULA_EXCLUSA` = knotapel vendor archivum
oracula). fixa/ (135 indexed files) and amalgama/ were JUDGED but never
TRIGGERED: a commit adding an unknown word to a fixture passed
unlinted and the next lib/ commit was refused for it. Now one judge
(`iudex lint_latinus`) answers both; Fran: generated code judged,
fixtures not (they are inputs - JSON, PNG, foreign-style C samples).

- `unitates_viae_iudicatae` filters a newline list (corpus_tractatae);
  vocabula AND the oratio pin test call it, so pin and lint cannot drift.
- A judge admitting every provenance never reads the file (2100 paths,
  no I/O beyond the policy + declarations).
- `unitates_arboris` is the shared git+disk sutura, NOT `_posix`:
  aedilis resolves `include/X.h` to `lib/X_{macos,posix}` BEFORE
  `lib/X.c` (variants) - an adapter named `_posix` replaces the core.
- The hook asks `./oratio/vocabula.sh -iudicandae` (self-building
  binary), not `bin/fabrica` (bin/ is not committed; fresh clones).
- oratio's hand-written link list needed unitates, unitates_arboris,
  filum, processus_posix (processus exists only as the posix variant).
- Tolerated copy: 44 lines deleted (29 fixture-only words + 15 already
  stale), 0 added. Plant: `munera` + fixtura -> `NOVA 29`, exit 1.

## 2026-10-15 - U4 step 2: commit formatter + one shell filter

- `tools/iudex_unitatum.sh` replaces vocabula's `-iudicandae` for every
  bash consumer; `-causae` puts "via: non iudicatur - <cause>" on
  stderr. Consumers must keep NAMING skipped files (unci XIX greps the
  GENERATUM reason) - pass only the files the consumer cares about
  (.c/.h for the formatter) so the causes stay signal, not noise.
- Role `amalgama` (segment rule, after fixa/assets): an amalgam is a
  product; formatting would be a generation stage (Fran). A segment
  role beats a per-file language exception: it covers generated .c and
  hand headers with one rule and survives new amalgams.
- formare_viae.sh lost its vendor changed-lines branch (Fran: vendor
  never touched) and its own GENERATUM check (the judge's
  provenientiae="manu" reads line 1; in -index mode partially staged
  files are skipped, so tree = index for every file it formats).

## 2026-10-15 - U4 step 3: pre-commit hook (5 lists -> 3 judges)

The comparison caught two more of MY rule gaps (`.css` no role, shell
tests under probationes/ as fixtures). Pattern so far: every consumer
migration finds one policy gap - run the old-vs-new comparison FIRST,
by reason (`bin/fabrica quid -iudex J -machina` cause column), before
switching. The hook keeps its extension prefilter (cheap, and the
explicit `UNCUS_VIAE` test mode stays unfiltered without
`UNCUS_CRIBRUM=1`).

## 2026-10-15 - U4 step 4: edit hooks; declarations belong to the container

`_unitas_viae`: a path equal to a directory unit's own declaration
returns that unit's PARENT (root keeps its own). Found because the
canon hook would have stopped judging vendor/knotapel/oracula/archivum
`unitas.stml` - files we write about foreign/frozen directories.
Hook helper `.claude/hooks/iudex_custodum.sh`: `git -C dirname
rev-parse` picks the file's tree; `check-ignore` replaces `*/build/*`;
the repository test replaces `*/scratchpad/*`. Binary freshness by
mtime against its five sources only (the closure is wider - stale
risk accepted for an advisory hook; commit paths use the
self-building wrapper).
