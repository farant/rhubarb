# nexus — worklog (nexus_percursus.c + nexus.c)

## 2026-07-10 — natus (instrumenta prima, tool 4 of 5)

Two parts per the sealed shape (sim-1 C2 + DECISUS #3):
**nexus_percursus** (percursus's two-pass skeleton + dump body:
every non-systema symbol seat via silva_c89_symbolum_per_indicem,
every use-site via a FOLIUM_IDENTIFICATOR walk over
silva_c89_symbolum_nodi; sem2-after-recanonicare discipline since
the binding table is canonical-relative) → **build/nexus.tsv**
(DISPOSABLE, never committed, header says so) → **nexus** CLI
(pure TSV read, no silva: group-by-file, seats first, +N reliquae
caps, -omnia for locals, -similis via similitudo, fortasse on
not-found with the emitte query-shrink). Wrapper does staleness
CAUTIO in shell (find -newer; warns, still answers — DECISUS).

**Numbers (first corpus contact):** 769 plagulae, 640,032 rows,
133s sweep, 45MB tsv; CLI ~0.15s per query. M1a simplification
paid: implicit callees bind on the callee FOLIUM node itself —
no VOCATIO arm needed.

**Bar (10 spot-checks, all correct):** fasti_differentia (the
membrum blast-radius case: 15 uses/2 files instantly);
similitudo_optima (nexus indexed ITSELF — 3 consumers incl.
nexus.c, hours old); malloc (M2's builtin-audit grep: 134 uses/29
files); chorda_aequalis_literis 332 semantic uses vs grep's 414
raw lines (delta = comments/docs — semantic precision, correct
direction); MedullaLineae typedef seats incl. both amalgam copies;
locals with genus+profundity under -omnia; -similis; fortasse
repo-wide.

**Design finds:**
- Headers appeared under TWO via spellings (bare praebere basename
  inside TUs vs full walk path) → bare-basename rows skipped (the
  direct .h walk is canonical); "./" prefix stripped. Dedup
  176,696 → 3,441 (real cross-TU dupes only).
- Systema-symbol USES kept deliberately ("who calls malloc" was a
  real repeated M2 query); systema seats naturally come from the
  direct systema_c89.h walk, not the synthetic TU copies.

**Named deferrals:**
- **Typedef USES are absent** — type-name uses are typus-nominatus
  nodes; semantica records bindings only at identifier-expressions
  and callees (M1a scope). The pull: a type-binding export
  silva-side when saltuarius type-jump (its first named consumer)
  asks. Seats work today.
- roundtrip-fixture copies (probationes/fixa/roundtrip/piscina.h)
  appear as seats — honest but noisy; via column disambiguates;
  filter-by-default only if it bites.
- CLI loads the whole 45MB per query (~0.15s) — fine until it
  isn't; acta is the real answer.

## 2026-07-11 — demand signal from M3 chunk ① (recorded at the reach)

During the congelatio rewrite the load-bearing query was "every
site touching FIELD `bloccus` of Tabulatum/AnulusFigura" — a
member-access pattern, not a symbol. nexus can't answer it (member
accesses aren't in the M1a binding table; the seat would be the
struct field, unexported like typedef uses). grep `->bloccus`
approximated it fine THIS time (only one struct family had the
field), but with a common member name it would drown. This is
syntax-pattern-query territory: quaestio selectors (membrum-access
with $field capture), not a nexus column. Same family as Fran's
lint-rule question (2026-07-11 conversation) — first named consumer
pull for quaestio QB/QC as a lint/query harness. Counting per the
rule of two: this is occurrence ONE.

## 2026-07-14 — RUNG 5: incremental sweep (canonical order + stream-merge)

Full design/soundness argument in silva/phase-log.md (INTENTIO written
hot-context the day before, implemented next session — the
capture-then-implement pattern worked; zero design reversals). Notes
that live here are the FINDS, not the plan:

**Per-file attribution was already true for rows.** fundere's
"viae sine '/' omissae" rule (nexus_ordines.h) meant every via's rows
came only from that file's own standalone judgment — verified 796
runs / 796 distinct vias before any edit. Canonical ordering made it
official; the row filter in the receptor is a safety net (0 alieni
in production). Inclusion edges were NOT attributed (1150 runs / 723
ex, first-TU-wins interleave) — re-pinned to per-ex attribution.

**Named limit — conditional includes vanish from the graph.** Under
per-ex attribution an edge exists only if the ex file's STANDALONE
preprocessing takes it. stb_image's `#ifdef STB_IMAGE_IMPLEMENTATION`
arm includes (7 edges) + one raqiya.h guard edge vanished at re-pin —
audited: 8/8 point at EXTERNAL headers (stdio.h...), which never seed
the staleness closure, so the loss is harmless today. The pattern that
WOULD bite: a repo header conditionally included inside another
header's foreign-TU expansion. The monorepo doesn't do this (amalgams
are generated whole). If it ever appears, the closure under-fires —
watch for it.

**Root-level .c files are invisible to nexus rows** (pre-existing,
discovered via the bar-test specimen at repo root): their via has no
'/', so fundere's basename-omission heuristic swallows their rows
(edges survive — _viam_solvere output keeps them distinct). Full and
incremental agree (parity holds); it's a fundere contract edge, not a
rung 5 bug. Convention already avoids root-level sources; if one ever
becomes real, fundere needs a smarter praebere-copy discriminator.

**Tombstones `# vacua <via>`.** Files judged-but-empty (invalid
roundtrip fixtures: no rows, no edges) looked "new" every run —
eternal re-judgment + table rewrite + stamp bump (= pointless legatus
tier-2 reloads). Fix: judgment writes a `# vacua` comment line for
empty files (both modes); the group scanner treats it as a
zero-line group. All tsv consumers already skip '#' lines (nexus
CLI:168, legatus rows:793, legatus incl:4570). Size-praetermissae
(capsula/biblia) deliberately get NO tombstone — the same st_size
rule that excludes them from nova keeps them re-discoverable when
-omnia lifts the cap.

**Toolchain-force.** nexus.sh passes -plenus whenever ANY celer
object or the sweep binary was rebuilt — a mixed-parser table (some
rows judged by old silva, some by new) can never be born. systema
mtime >= stamp likewise forces plenus inside the binary (systema is
the lexicon channel, invisible to the include graph — M4b find).

**Numbers.** Quiet run (nulla mutatio, tables untouched): 0.3s.
One .c edit: 0.5s. Header edit w/ 3-file closure: 0.7s. Rename:
deleta+nova in one pass. Full sweep unchanged ~50s. Parity octetim
incremental-vs-plenus on both tables at every bar, same tree.

## 2026-09-07 — Header offering order is a law, not alphabetical luck (01M1TD1FMFT3)

Symptom: after a full renovation the table lacked six definition
sites in silva_formator.c (pythonica gate red at `usus: sedes
definitionis`), and the ledger already held the cold table at 0.96M
rows against the resident's 1.10M. Attribution across the tree:
silva_c89_semantica.c 301 definitions / 218 sites, silva_arbor.c
91 / 65, silva_expandere.c 71 / 53 — a class, not six symbols, and
the six had nothing in common (`formator_extenta` and the healthy
`formator_lint` beside it have identical signatures).

Reproduction: a scratch probe (session scratchpad, not committed)
that parses ONE file exactly as the sweep does — system lexicon,
every header offered, oracle, recanonicalization — and dumps every
symbol with the reason a sedes row would be skipped. silva_formator.c
under the sweep's context: successus=1, errores=19; `_praecedentia`
present only as an IMPLICIT symbol at depth 4 (its call site),
`formator_extenta` only as the header prototype. With include/
offered first: errores=0, both definitions emitted at their lines,
implicit symbols 14 → 0, symbols 1467 → 1768. One variable, one run.

Root cause: `_caput_praebere` offered every .h in the tree by
basename, first-wins, in strcmp order — the "include/ ante
probationes/" guarantee was an accident of the letters i < p. The
briar -amalgama fixture (78518952, 2026-09-05) added
`briar/probationes/fixa/amalgama/fabrica/include/latina.h`, a
deliberately TRUNCATED latina.h (no commutatio / casus / frange /
perge / ordinarius / magnitudo / unio / NIHIL / VERUM / FALSUM, no
numeral above II); 'b' < 'i', so every TU including "latina.h" got
the fixture. Any function with a switch, a NIHIL or a numeral above
II parsed with tolerated errors and never registered its definition;
its callers resolved to implicit symbols. The incremental sweep
propagated the damage faithfully on the next run (a new header
basename seeds the closure of every includer — designed behaviour),
so the table degraded the moment the merge landed and kept degrading
with each renovation (oratio's identifier unknowns 2,990 → 2,958 →
2,732 on the same code). Instruments that feed latina through the
LEXICON channel (praeparator cum_latina, examen, formator …) were
immune to this instance: the lexicon defines LATINA_H and the real
inclusion becomes a no-op. The sweep offers latina.h only through
praebenda.

Fix — one law, two producers: `nexus_ordines_capitis_gradus(via)`
(0 = include/, 2 = any path with a whole `fixa` component, 1 = the
rest) and `nexus_ordines_capita_comparare` (rank, then strcmp), in
the module the sweep and the resident already share. The sweep
collects the headers and offers them in that order; the praeparator
now collects candidates during its walk, sorts with the same law,
then offers (it offered inline in readdir order before —
unsorted and nondeterministic). Gate `probatio_officina_capita`
(27 assertions: the rank table incl. `prefixa/`, `fixatio/`, a `./`
prefix, NULL; comparator order; and on the REAL tree the first
latina.h and postulata_posix.h in comparator order must be
include/'s). Planted fault (fixa ranked 1): red on six assertions,
restored green. Full sweep after: 1,100,681 rows, 88 / 88 function
definitions in silva_formator.c, the resident agrees after renovare.

Found on the way: officina/compile_probationes.sh tested
`src -nt obj` — bash 3.2 compares whole seconds, so a source restored
in the same second its object was compiled never rebuilt, and the
restored gate stayed red against the PLANTED object. Same defect
silva's runner fixed on 2026-08-27; ten conditions inverted to
`! obj -nt src`.

Remaining exposure, filed not fixed: twelve silva instruments walk
"." for headers with their own copy of the same loop (examen,
renominare, censor, selecta, quaestio, origo, identitates, emitte,
infidelis, percursus, census_ambigui, silva_iudicium) — readdir
order, first-wins. Immune today only because latina reaches them
through the lexicon; a fixture twin of any OTHER house header would
bite them. The class wants a shared walker, not twelve edits, and
silva is frozen.
