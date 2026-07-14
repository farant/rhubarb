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
