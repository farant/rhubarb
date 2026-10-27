# canalis lectionum - every read through filum; gate coverage from declarations (spec)

*Born 2026-10-14 in quinta, from a review of the framework after the
overnight runner conversions (html, saltuarius, materia). Question that
prompted it: Fran asked what the conversions showed about fabrica's
organization; drafting the fix for hand-written gate coverage found
that converted suites can KEEP A STALE PASS. Ledger: question
…QT3PR64 (the bug), decree …PX2QAC (Fran's decisions below), open wish
…C8CCNCC ("lint statice canalem cogit" - this slice fulfils it).
Predecessors: `fabrica-spec-2.md` (the read ledger, §X.1 names this
exact risk), `portae-declaratae-spec.md` (§VI deferred "runtime inputs
through recorded reads instead of tegit" and "PORTAE derived from
annotations" - both are here), `cursores-gradus-spec.md` (the
conversions).*

*Status: DRAFT - headers for Fran's approval before any code.*

**Decisions (Fran 2026-10-14, decree …PX2QAC):**
1. A raw IO call outside the channel is a new **examen code**,
   a house violation that blocks, opted out only by `<tolera>` with a
   reason; it applies to ALL house C, not only test closures; `getenv`
   is outside the first cut.
2. The six converted suites are migrated **on purpose**, not when
   touched.
3. Gaps found in `filum` are **implemented** in the library, not worked
   around.
4. fabrica keys by **observed** reads (made complete by the rule);
   portae derives **statically** (a fresh clone has no records); an
   audit compares the two. Static literals do NOT enter member keys.
5. Direction: this shape (name table + replacement + `<tolera>`, as
   codes 85-87 already do) is how the platform layer gets enforced
   later.
6. A converted suite's gate is named in its declaration (`porta=`);
   the per-suite runner files stay as thin human entry points (Fran
   2026-10-14, accepting both recommendations).

## 0. Census (2026-10-14, quinta at ed85fe10)

**The bug, reproduced.** A line appended to
`html/probationes/fixa/oraculum/recusata.tsv`, then
`bin/fabrica probare probationes_html oraculum` printed
`servatae 1/1: ingressus immutati post transitum ultimum` - the member
did not run. Restored with cp + touch. Cause: the test reads with raw
`fopen` (`html/probationes/probatio_html_oraculum.c:74`); the read
ledger records only what passes through filum / via_existit /
iter_directoria / lectiones_ambitus (`include/lectiones.h:1-12`); a
member's static key is closure + house tools + aedilis.stml + its
source (`lib/fabrica_probationes_c.c:150-232`). The html gate makes the
same `probare` call (`pythonica/silva.py` `_porta_per_gradum`), so a
broken fixture commits green. Warm numbers with nothing changed stay
honest; the hole opens only when an input the trace cannot see
changes.

**Why only these suites.** `tools/lectiones_lint.sh` blocks raw IO only
on its "pilot path" (`VIA_PILOTA_RADICES`: `tools/aedilis.c`, toml's
tests, the root tests) and only warns elsewhere; rubric v3 (batch job
…W0ZBW) never asked for it. Recorded reads per suite in
`build/fabrica.db` (`lectiones`, members only): html 1 L + 13 X, css
1 L + 9 X, md 1 L + 13 X, saltuarius 13 X, materia 14 X - against
toml 20108 L, silva 1676 L, radix 743 L.

**Raw file IO** (silva nexus `usus` rows of fopen freopen open opendir
stat lstat access; `getenv` excluded):

| scope | call sites | files |
|---|---|---|
| whole tree | 482 | 223 |
| closures of the six suites' tests (`bin/aedilis --enumerare`) | 130 | 62 |
| - of which test files | 95 | 55 |
| - the channel itself (filum.c 23, lectiones.c 3, iter_directoria.c 3, via.c 1) | 30 | 4 |
| - lib/credo.c (credo links without filum by design, credo spec 2 §VI.2) | 3 | 1 |
| - sources (silva_canon_coquere.c, materia_coctor.c) | 2 | 2 |

By function in the six closures: fopen 94, opendir 19, stat 12, open 3,
lstat 2. filum already offers whole-file read, a line reader, writers,
size/status, existence and directories (`include/filum.h`); gaps are
expected at the edges (binary streaming, partial reads, seek).

**Hand coverage.** Each converted runner's `# <porta tegit=...>` lists
globs by hand (`toml/compile_probationes.sh:8` etc.). Only toml's covers
its own `aedificatio.stml`: `./tools/portae_debitae.sh
html/aedificatio.stml` owes generata and fabrica-fumus but NOT html -
the same for css, md, silva, saltuarius, materia. Changing an
`<ambitus>` or `<debitum_scripturae>` commits without running the suite.

**Registration.** Converted runners are 51-54 lines that differ only in
the suite name (`diff` html vs css: the annotation, the env guard
variable, one comment). `PORTAE_GRADUUM` (`pythonica/silva.py:1974`)
maps 8 gates to composites by hand; their `PORTAE` signature regex is
reached only on the fallback path, which for a converted suite calls
the same `probare` and cannot succeed where it failed.

## I. INTERFACE A - the examen code (for Fran's approval)

`EXAMEN_CODEX_CANALIS_PRAETERITUS` (next free number, 96): a call to a
system file-IO function outside the channel.

- **Table:** `fopen freopen open opendir stat lstat access` - each row
  names its replacement (`fopen` -> `filum_legere_totum` /
  `filum_lector_aperire` / `filum_scriptor_aperire`; `opendir` ->
  `iter_directoria`; `stat` -> `filum_status` / `filum_existit`;
  `access` -> `via_existit`), printed in the message like 86/87.
- **Detection:** the 85-87 mechanism. The table is DATA: rows of the
  existing tag in `silva/fontes/systema_posix.h` comments,
  `<gradus titulus="fopen" genus="canalis" pro="filum_lector_aperire"/>`,
  read by `_portabilitas_colligere` into `sem->gradus_tabula`
  (`silva/fontes/silva_c89_semantica.c:6330-6360`); the walk over the
  main source's final token stream, reduced to source tokens, already
  sees identifiers in function bodies (`:7040-7115`). New: the
  `canalis` genus judges CALLS only - identifier followed by `(` and
  not after `struct` (`struct stat st;` is a type, not a bypass).
  Decree …PX2QAC (5): the platform layer's table is these rows.
- **Severity:** VIOLATIO domus (exemplar 91: clang accepts, the house
  does not); suppressible per line by
  `<tolera codex="CANALIS_PRAETERITUS" (>causa>`.
- **The channel's own calls** (filum.c, lectiones.c, iter_directoria.c,
  via.c, thesaurus.c) and credo.c carry `<tolera>` with their reason;
  `/* lectiones: notatur */` markers become `<tolera>`.
- **Ratchet (Fran 2026-10-14, option a):** the pre-commit hook judges
  staged `.c`/`.m` (`tools/unci-git/pre-commit:14`) and only VIOLATIO
  severity gives REICE (`silva/instrumenta/principalia/examen.c:589`).
  Today's violators are listed ONCE in
  `silva/probationes/fixa/examinis/canalis_debita.tsv` (`via<TAB>causa`,
  like `tools/radices_manu.tsv`): for a listed file code 96 is reported
  at DOMESTICUM (counted, no REICE); unlisted = VIOLATIO. Two-way check
  (gate examen-corpus): an unlisted file with a raw call = red; a
  listed file without one = red ("linea delenda"). The list only
  shrinks. NOT the corpus exclusions file - that pins whole-file REICE
  with a per-line read reason (12 entries); ~220 blanket entries would
  be the silently-ageing list its header forbids. knotapel/ is outside
  examen entirely (Fran 2026-10-03).
- **House chores:** add the code by hand to `silva/amalgama/silva.h`,
  `./tools/amalgama_excludenda_generare.sh silva`,
  `./silva/amalgamare.sh` (else commissio refuses: credo II T7).
- `tools/lectiones_lint.sh` retires once the code is green; its census
  mode (`-omnes`) is answered by examen over the corpus.

## II. Migration of the six suites

Order: html (the reproduced case), css, md, saltuarius, materia, silva
(largest). Per suite, one commit:
1. every raw call in its test files and the two sources -> filum;
2. **proof:** the fixture plant (edit a fixture the suite reads, member
   must RUN) red before, green after; `tools/gradus_oraculum.sh <sub>`
   agrees; `bin/fabrica probare probationes_<sub>` green;
3. members' recorded reads in `build/fabrica.db` grow from ~1 L to the
   fixtures they actually read (the evidence the trace now sees them).

A missing filum capability is added to `lib/filum.c` with its own test
in the same commit, recorded in `lib/filum.worklog.md`.

## III. INTERFACE B - coverage from the declaration (for Fran's approval)

A composite names its gate:

```xml
<actio titulus="probationes_html" genus="iudicium" porta="html">
  <probationes_c exemplar="html/probationes/probatio_*.c"/>
```

- `aedificatio.canon` admits `porta` on an `iudicium` action; two
  composites claiming one gate = refusal.
- `lib/portae.c` gains a sutura callback `composita`: for each gate
  composite, its member sources, its declaration file, and its declared
  inputs / write debts / house binaries. Member sources go through the
  existing closure and literal scans (`include/portae.h:134-150`).
  `tools/fabrica_portae.c` implements it with fabrica's own declaration
  reader and step expansion (same binary).
- A gate owns its declaration file - the census gap closes by
  construction.
- `tegit` is deleted from converted runners after the shadow comparison
  (V).
- A converted gate needs no runner annotation: the declaration is the
  registration (decision 6; amends portae-declaratae's "every gate has
  an annotated runner").

## IV. The audit

`bin/fabrica portae audit [-machina]`: for every gate composite, each
member's last recorded reads (tracked paths only) against the gate's
derived coverage; a path read but not covered is named
(`INTECTUM_LECTUM\t<porta>\t<membrum>\t<via>`). No traces = said so,
never silent green. Runs as a stage of gate `portae-declaratae` in the
live tree. With I-II in place the trace is complete, so the audit
checks the static derivation against the truth.

## V. Registration and the entry point

- `bin/fabrica portae declaratae -machina` emits
  `COMPOSITUM\t<porta>\t<compositum>\t<cursor>`; pythonica builds the
  converted entries from it; `PORTAE_GRADUUM` and the per-suite
  assertions in `pythonica/probatio_silva.py` are deleted (one generic
  assertion remains: every declared gate composite resolves).
- The dead fallback for converted gates is removed.
- `tools/ianua_probationum.sh` holds the log tee, the runner lock (gate
  sera VIII), the fabrica bootstrap and the `probare` call; a converted
  runner is ~4 lines. The per-suite files stay as human entry points.
- radix keeps its hand `PORTAE_RESIDUA` (prae/post).

## VI. Rubric v4 (batch job …W0ZBW)

Point X: the suite's tests and the sources in their closures read and
write only through filum (examen code clean or tolerated with reason);
the fixture plant re-runs a member. Converting = declaration with
`porta=`, the `fabrica.stml` line, the 4-line runner.

## As built so far

- **T1** (4e80dce5): code 96 + `canalis_debita.tsv` (222 files, 478
  sites); examen, percursus, legatus apply it; colloquium follows the
  rule (D2 script via filum, getenv bridge; interpreter gap parked
  …AV0N8).
- **T2 html**: 15 `fopen` in 11 test files -> `lectiones_fopen` (same
  shape - the suggested replacement since T1); 11 debt lines deleted
  (211 left; html's two tools stay until touched). Recorded content
  reads of html members 1 -> 237. Proof: md fixture edit ran the
  member (before: kept); oracle `gradus_oraculum.sh html -ref
  1713547e^` 14/14. **Found and fixed on the way:** golden files with a
  `<debitum_scripturae>` were dropped from the trace as the member's
  own work - a golden edit never re-ran its test (all six computus
  goldens + the html oracle refusals). `lib/fabrica.c` `_aurum_debiti`
  (Fran 2026-10-14); see lib/fabrica.worklog.md.

## VII. Order (plan to follow)

T1 the examen code (`genus="canalis"` rows, call check, code 96) + the
debt list and its two-way check + plant; T2-T7 the six migrations (II), html
first; T8 `porta=` + `composita` + shadow comparison (owed gates over
the last N commits, tegit vs derived; pairs owed only by tegit = 0 or
explained) + `tegit` removed; T9 the audit; T10 registration + shared
entry point; T11 rubric v4, docs page regenerated, lectiones_lint
retired.

## VIII. Done means

- the html fixture plant: red (member kept) at T0, green (member runs)
  after its migration; the same plant per suite;
- `examen` refuses a new raw `fopen` in any house file by name, with
  the replacement; `<tolera>` without a reason refused (born red by a
  plant);
- every converted gate is owed by an edit to its own
  `aedificatio.stml`; `tegit` gone from converted runners with the
  shadow comparison clean;
- `portae audit` names a planted uncovered read; quiet otherwise;
- registering a converted suite touches its declaration, one
  `fabrica.stml` line and a 4-line runner - nothing in pythonica;
- every filum gap met is a library function with a test.

## IX. Not in this slice

`getenv` (170 sites; members get an exact environment and declared
values are in the key - a later code or a warning); platform-layer
enforcement beyond file IO (the same table shape, decree …PX2QAC (5));
migrating the ~350 other sites outright (the ratchet does it as files
are touched); class B runners (briar, officina, tessera, ludus_tessera,
gesta); deriving `PORTAE` for unconverted runners.

## AUDIENDA (not verified when this was written)

Answered 2026-10-14 (after the draft): the walk sees function bodies
(§I Detection); `<tolera>` matches per site - same source, firing line
or the line before (`_tolera_absorbere_sedem`,
`silva_c89_semantica.c:1347`); the corpus exclusions are whole-file
REICE pins, so the ratchet gets its own list (§I Ratchet).

*2026-10-26:* the window grew by one case - the line right after the
END of the tolera's comment also matches, so a multi-line comment
directly above a statement covers it. Needed where 96 and 97 fire on
the same call (write-mode fopen in filum/credo): one comment, two
`<tolera>` elements (decision …24P17, test in probatio_silva_examen).

Still open:
- how examen finds `canalis_debita.tsv` (it judges one file; the
  repository root it already receives for headers is the candidate);
- whether a test's raw `fopen` in generated sources changes the counts
  (amalgama, fixa and build are outside the edit hook);
- whether filum's line reader and writers are fast enough for the
  largest fixtures (silva corpus tests) - measure before/after in T7;
- the 130-site count joins nexus rows to `--enumerare` closures; a test
  reading through a library outside its closure (dlopen, a child
  process) is not counted.
