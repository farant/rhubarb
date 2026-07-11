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
