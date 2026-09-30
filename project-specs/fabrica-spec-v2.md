# fabrica — spec v2 (codebase pass: AUDIENDA measured, slice 1a planned)

*2026-09-29. v1 (`fabrica-spec-v1.md`) is the design from the
interview (`fabrica-interview.md`, Q1–Q32); v2 is the codebase pass
Fran asked for before any plan. Method (Fran): **inventory and lens**
— ledger inventory 'producentes (fabrica)' 01M3QVKRK4PZG7CX3TNDXVVAFX,
68 rows × 9 lenses, filled from reading and measurement, then the
eight AUDIENDA of v1 measured one by one on the live tree (main at
85834590). Every number below was measured today unless it names
another date. v1's model (§II) and decisions (§XII) STAND; v2 amends
where the codebase disagreed and turns slice 1a into an ordered task
list. Nothing here is built.*

## 0. What the codebase pass overturned or added

Five things v1 did not know. Four of them are staleness machinery
the inventory's first pass missed — which is itself the finding: the
house has more bespoke freshness logic than anyone can list from
memory.

1. **The root `fabrica.stml` is already taken.** It is silex's
   workshop marker (`<fabrica titulus="rhubarb"/>`, dialect `fabrica`,
   `fabrica.canon`, 6372dde4, 2026-09-07). `silex_fabricam_invenire`
   (lib/silex.c) searches for it **ascending from the cwd** — so a
   per-subsystem `silva/fabrica.stml` (v1 §III) would make silex treat
   `silva/` as a workshop root. v1's file name is not usable → Q33.
   (The canon's own comment anticipates growth: "optiones fabricae hic
   NON vivunt donec consumptor eas postulet" — the build layer is that
   consumer.)
2. **`excubitor.sh` + `fabrica.tsv` is an existing JUDGE.** "Verificat
   et explicat solum — scripta constructionis executores manent"
   (excubitor.sh:17) — slice 1a's exact contract, built 2026-07 on
   mtimes over `build/inclusiones.tsv` + a hand manifest `fabrica.tsv`
   (53 `binarium`, 9 `generatum`, 1 `obiectum`, 3 `praefixum` rows).
   Wired into `compile_tests.sh:1199`, `css/compile_probationes.sh:191`
   and a PostToolUse hook (`.claude/hooks/excubitor-custos.sh`, warns
   when a header with ≥2 direct includers changes). Full run today:
   **188 s, 1,333 objects inspected, STALA 171, 460 orphan objects,
   43 ambiguous.** Its six `silva/grammatica/silva_tabulae_*` rows
   name files that do not exist (they live in `silva/fontes/`) — dead
   rules, silently skipped. → the 8th staleness notion; Q35.
3. **aedilis manifests can be silently incomplete.** An unresolved
   `#include "x.h"` is filed under `<systemata>` (phase-A decree:
   "inclusio inresoluta = systema, numquam recusatio"). For roots
   whose headers live outside `aedilis.stml`'s `<inclusa>`: **briar 15
   unresolved quote-includes** (every `briar_*.h`, `compendium.h`) →
   manifest 27 objects vs 79 non-test objects `briar_struere.sh`
   actually links; **spectator 9; silex 2** (`silva_token.h`,
   `silva_lexema.h`). manus, mensor: 0. A digest over such a manifest
   would call briar current after a `briar/fontes` edit — the silent
   stale build this layer exists to prevent. → prerequisite P1.
4. **More committed generated files, no guard:**
   - `silva/fontes/silva_tabulae_{c89,sceleti,imparilis}.{c,h}` from
     the grammars (`silva/generare.sh`, 2.5 s, regeneration == committed
     today, deterministic) — no `generata` stage.
   - **seven capsulae from TOML** (`apps/{forum,mensor,villa}/assets/`,
     `probationes/{hospes,tabella,vitrea}_assets/`, `tools/silex_assets/`)
     via `bin/capsula_generare` (≤0.03 s each), plus two committed
     capsulae with NO toml and an unknown generator
     (`book_assets/capsula_libri.c`, `probationes/capsula_assets.c`).
     All seven regenerated today: six byte-equal, **`capsula_mensor.c`
     differs — 2,112 lines, whitespace only** (committed copy was
     formatted, generator emits raw; equal with whitespace removed).
     Consequence: `tools/mensor_ui_struere.sh:15` regenerates it in
     place on every build → **building mensor_ui dirties the tree**.
   - `lib/entitates_html_tabula.c` — still unguarded.
   - Correction to the inventory: `lib/runae_tabulae.c` IS guarded —
     `generata` stage **VI** (d391af9f, arrived with the secunda
     merge). The amalgam comparison built today is stage **VII** now,
     not VI.
5. **Stale manifests** (corrected at T3: bin/fabrica finds EIGHT, the
   six others from deleted lapifex/eventus_inspector test roots):
   `build/aedilis/probatio_toml/` and
   `build/aedilis/probatio_arbor_quaestio/` name deleted files
   (`lib/toml.c`, `lib/arbor_quaestio.c`) — the orphan class
   "declaration whose inputs vanished", distinct from "output nobody
   produces".

## I. AUDIENDA answered

**1. Does the aedilis manifest hold every input an install needs?**
Partly. A manifest (`build/aedilis/<t>/manifestum.stml`, 205 today)
holds: `<obiecta>` (source + header, `origo` derivatum 5,988 /
oraculum 98 / corpus 101 / annotatio 16), `<capita>` with per-header
`<inclusio>` edges, `<systemata>`, `<vendores>` (sqlite with its own
flags), `<vexilla-annotata>`, and per-object link flags from
`aedilis.stml` `<nexus>` rules. It does NOT hold:
- the global compile flags and include roots — they live in
  `aedilis.stml:2-41` and are baked into each generated `struere.sh`
  (`VEXILLA=`, `INCLUSA=`); → input "aedilis.stml" (whole file, coarse
  and safe);
- the compiler — → input "tool" (`clang --version` text);
- who PRODUCES a generated input: `build/capsula_corpus_silicis.c`,
  `build/speculum/*/capsula_speculi_*.c`, `apps/mensor/assets/capsula_mensor.c`
  appear as plain paths. → the planner links an input path to the
  action whose declared output it is;
- briar's embedded corpus (corpus_infixum reads lib/ include/ vendor/
  canones wholesale) — not an aedilis concept; declared explicitly
  (tier 3) until the corpus is itself a declared output;
- completeness when a quoted header is outside `<inclusa>` (§0.3).
The manifest FILE is not byte-stable (`generatum=` timestamp,
`commissum=`); digests are taken over the files it lists, never over
the manifest. Also: generated `struere.sh` treats EVERY header in
`CAPITA` as a dependency of EVERY object (struere.sh:18-23), though
the manifest carries per-header edges — per-object closures are
derivable when slice 2 wants them.

**2. Determinism of regenerable families.** All measured families
deterministic: amalgams (octet-equal ×8, v1), `fontes_generata.h` /
`excludenda_generata.h`, 37 snippets, lexicon, latina numerals, runae,
entitates, silva grammar tables, capsulae (two runs `cmp`-equal).
Four families are **generate → FORMAT**: lexicon .c, runae_tabulae.c,
entitates_html_tabula.c, capsula_mensor.c — raw ≠ committed, formatted
== committed (verified for the first three; the fourth is equal modulo
whitespace). Today two gates hide this by comparing "spatiis
neglectis" (generata II, VI); `tools/entitates_html_generare.sh:66`
compares raw vs formatted and therefore ALWAYS rewrites. → model
formatting as its own action kind (`formatio`), whose inputs include
the formator binary; comparison is then exact bytes, never "modulo
whitespace".

**3. Is `celer` < 2 s reachable?** Yes, with margin, no cache needed.
- Union of every input across all 205 manifests: 436 files, 49.9 MB
  (39.5 MB without vendor/).
- SHA-256 of all of it with the house `sigillum.h`: **0.44 s at -O2,
  0.82 s at -O0** (house flags carry no `-O`; aedilis builds are -O0).
  openssl (SHA-NI) 0.03 s, shasum 0.19 s for comparison.
- `-provenientia`-style self-report: `briar -versio` ~0 ms; 50
  binaries in `bin/`, ~12 with installers.
A stat cache (git-index style: mtime+size+inode → digest) stays
unbuilt until a measurement says celer is slow (PULL).

**4. Writing a digest into a binary without self-dependency.** briar's
pattern, measured (`tools/briar_struere.sh:41-56`): hash a HAND LIST
of briar's own sources (`FONTES_BRIAR`), truncate to 8 hex, write
`build/briar_aedificatio.c` (`briar_aedificatio_fontes[]`, plus time
and commit, `SORDIDUM` if dirty), compile and link it with the binary.
No cycle, because the generated .c is not in the hashed list. The
pattern is right; the LIST is the bug: `lib/` is absent, which is
exactly why `-versio` never catches the "rebake after lib/" case.
v2's rule: the digest is of the action's FULL declared input set
(manifest closure + aedilis.stml + tool + the installer script),
64 hex, no timestamp inside the digest (time may still be printed).

**5. Vendor sqlite.** Already solved by aedilis: `aedilis.stml:59-69`
compiles `vendor/sqlite3.c` "laxum" with its own flags (-O2, FTS5,
THREADSAFE=0 …), never the house warning set; the object already sits
in `build/aedilis/obiecta/vendor__sqlite3.o` (also gesta/build). Cold
compile 10.0 s, once. `bin/fabrica` built by aedilis gets it free.

**6. Can crusta see the query idiom?** The parse side is ready:
`crusta/arbor.sh` projects `FONTES="$(bin/fabrica quaere 'x')"` as
`<assignatio>` → `<pars-substitutio>` → words `bin/fabrica` `quaere`
and a `<pars-simplex>` holding `'x'`; `for f in $FONTES` gives
`<iteratio>` with `<pars-parametrum>` FONTES. crusta already has a
lint layer (`crusta/lintrum/*.stml`, e.g. `vexilla-domus`). The FLOW
side does not exist: "every read path comes from such a variable"
needs binding analysis (assignment, `for`, `read`) — a small C pass
over the crusta tree, not a canon selector. Slice 4, as planned.

**7. Name collisions.** `fabrica.stml`/`fabrica.canon` (silex marker,
§0.1), `fabrica.tsv` (excubitor, §0.2), `natura/fabricatio.genera`
(natura's genus table, 1,180 lines), briar's `briar_fabrica*` and
`briar/fabrica.sh`, `silex_fabricam_invenire`, `md_fabrica_incipere`
and generic `fabricare` verbs. A library prefix `fabrica_` is free in
lib/ and include/ (no `fabrica_*` symbol outside briar's
`briar_fabrica_*`); the FILE names are not.

**8. portae_debitae and receipts.** `PORTAE` (pythonica/silva.py:1513,
36 gates) maps a name to (command, "it ran" regex). A run writes a
receipt `build/portae/<nomen>[.<filtrum>].viva.json` or, from a
snapshot, `<nomen>.<t>.json` (`porta`, :1730; `porta_umbra`, :2505).
A receipt is keyed to `sigillum_arboris()` (:2309) = HEAD + whole
`git diff` + every untracked file — **any edit anywhere invalidates
every receipt**. `portae_debitae` (:1989) maps paths → gates by aedilis
closures of each suite + inventory lenses. Slice 3's mapping: a gate
becomes an action whose inputs are its suite's closure; a receipt
becomes a record keyed to THAT digest — editing a doc no longer
invalidates the silva gate.

## II. Model amendments

v1 §II stands. Additions:

- **Input kinds, slice 1:** `fasciculus` (bytes), `manifestum` (the
  files an aedilis manifest lists, ordered), `configuratio`
  (`aedilis.stml`, whole), `instrumentum` (a tool: its binary digest,
  or `clang --version` text), `directorium` (a directory's sorted name
  list — for today's globs, e.g. `silva_fontes_generare.sh` over
  `silva/fontes/*.c`).
- **Action kinds, slice 1a (judge only reads them):** `generator`
  (fixed argv, writes into a scratch directory it is handed),
  `formatio` (formator over a generator's output), `installatio`
  (binary + `-provenientia`). `compilatio`/`nexus` wait for slice 2.
- **Input set digest:** SHA-256 over, for each input in path order,
  `via NUL sigillum(bytes)`; 64 hex everywhere.
- **Refusal (Q9 applied):** a manifest with an unresolved quoted
  include is `IGNOTUM: manifestum incompletum (<header> in <file>:<line>)`,
  never "current".

## III. Self-report — `-provenientia` (concrete)

A tiny library, because ~12 binaries need the same five lines:

```c
/* include/provenientia.h */
/* Si argv '-provenientia' fert, relationem scribit et VERUM reddit
 * (vocans tum exit 0). Chordae ab actione installationis generantur
 * (build/fabrica/provenientia/<t>.c) - numquam manu. */
b32
provenientia_respondere (
                 s32  argc,
          character** argv);

externus constans character provenientia_artificium[];
externus constans character provenientia_ingressus[];  /* LXIV hex */
externus constans character provenientia_commissum[];  /* + " SORDIDUM" */
```

Output (v1 §VI, unchanged): `provenientia 1` / `artificium bin/manus` /
`ingressus <64 hex>` / `commissum <hash>[ SORDIDUM]`. The installer
obtains the digest from `bin/fabrica digestum <actio>` and writes the
.c; the judge recomputes it the same way. One function computes it
for both — script and judge cannot disagree (the Q29 principle).

## IV. Slice 1a — ordered tasks

Each task: one commit, its own test cycle, a plant that proves the
check can go red, Fran approves before the next. Oracles are the
existing checks; agreement is asserted, not assumed.

| # | task | deliverable | oracle / plant |
|---|---|---|---|
| P1 | **aedilis: unresolved quoted includes reported** | manifest gains `<inresolutae>` (quoted includes aedilis could not resolve; angle includes stay `<systemata>`) — silva has carried the form since b5e9a8f (`est_angulata` on `SilvaInclusio`/`SilvaInclusioVista`, desideratum 01KY118F, fulfilled 2026-07-27); aedilis only has to consult it; then `<inclusa>` roots added for briar/fontes, md/fontes, officina/instrumenta, silva's headers as measured, until briar/spectator/silex have 0 | porta aedilis; generata IV (amalgam manifests unchanged or deliberately regenerated); plant: remove a root → `<inresolutae>` names the header |
| P2 | **capsula_mensor raw + excubitor's dead rows** | commit `capsula_mensor.c` as the generator emits it (Q34); delete the six `silva_tabulae_*` rows from `fabrica.tsv` (Q35); stale aedilis manifests are left for the judge to report, never deleted by it | `git status` clean after `tools/mensor_ui_struere.sh`; `./excubitor.sh -tacitus silva/` unchanged verdict |
| T1 | **lib/fabrica core (pure)** | `include/fabrica.h`, `lib/fabrica.c`: input kinds, input-set digest, declaration model, judge verdicts (RECENS / STALUM / IGNOTUM / NON_IUDICATUM), plan order — file access through a seam (the aedilis extractor pattern) so tests run on fixtures | `probationes/probatio_fabrica.c` + `probationes/fixa/fabrica/`; plant: one byte in a fixture input → STALUM naming it |
| T2 | **declaration dialect + canon** | file name per Q33; canon registered in `canones.registrum`; reader via the stml library; element names Fran's | `canon_examen` on every declaration; plant: unknown kind → refused with line |
| T3 | **bin/fabrica + bootstrap** | `tools/fabrica.c`, `tools/fabrica_struere.sh` (aedilis-built, sqlite object reused); `iudicare [-plenus] [artificium…]`, `digestum <actio>`, and its own `-provenientia` (the judge judges itself first) | exit contract 0/1/2 by test; plant: stale `bin/fabrica` → it says so |
| T4 | **amalgam chain declared** | silva/tessera/officina: `fontes_generata.h`, `excludenda_generata.h` → amalgam `.c`; generators honour a scratch output dir | generata IV + VII agree on a clean tree AND on a plant (header comment edit → same three artifacts STALUM) |
| T5 | **generated sources declared** | lexicon, numerals, runae, entitates, silva grammar tables, seven TOML capsulae (the two without a toml: IGNOTUM until their generator is found); `formatio` actions for the four generate→format families (exact compare) | generata II, V, VI agree; new coverage (grammar tables, capsulae, entitates) born red by a plant |
| T6 | **records + the snippet win** | `build/fabrica.db` (sqlite): a successful `-plenus` verification is recorded with its input digests; next `-plenus` skips regeneration when digests match. The 37 snippets' inputs = aedilis closure of each root + aedilis.stml + bin/aedilis + globbed directories | generata III agrees; measured: second `-plenus` with nothing changed ≪ 300 s (today's stage III); plant: touch a closure file's bytes → that snippet regenerated, others not |
| T7 | **installed binaries** | `include/provenientia.h`, `lib/provenientia.c`; installers of manus, mensor, mensor_ui, stml, aedilis, natura (4), canon (2) write the digest; briar, spectator, silex after P1 (their closures then complete) — briar keeps `-versio`'s `fontes` line until Q23 convergence | plant: edit a lib/ file in bin/manus's closure → `bin/fabrica iudicare` names bin/manus STALUM (the six-week bug); briar-fumus XX unaffected |
| T8 | **surfaces + oracle gate** | session-start hook runs `celer` (<2 s, measured in the task); `silva.commissio` runs `-plenus` for artifacts the diff touches; a `fabrica` gate asserting agreement with generata I–VII (Q15: oracle, then delete) | the gate born red: a plant where generata and fabrica disagree |

**Explicitly NOT in 1a:** building anything (1b), compile/link kinds
and the shared object store (2), gates as actions (3), the query idiom
and crusta lint (4), comment opt-ins (deferred, Q22), excubitor's
retirement (after 2, Q35), the other ~38 binaries in `bin/` (tests
and compile_tools output — reported once as a COUNT under IGNOTUM,
not line by line, Q36).

**Orphans in 1a** (v1 Q24, narrowed to what 1a can see): aedilis
manifest directories whose `scopus` no longer exists (2 today), and
committed files matching a generator's output pattern that no
declaration names. Full `build/` orphan sizing needs slice 2's
declarations (excubitor's 460 orphan objects are the preview).

## V. Numbers slice 1a is measured against

| today | source |
|---|---|
| `generata` 381 s; stage III ≈ 300 s (silva_fontes_generare 238 s + compile_tests_fontes_generare 59 s) | ledger park …GTQHQ note |
| excubitor full run 188 s, STALA 171 | §0.2 |
| digest of every manifest input 0.44 s (-O2) / 0.82 s (-O0) | §I.3 |
| aedilis closure: briar 0.50 s, spectator 1.11 s, silex 1.51 s | measured |
| `bin/manus` stale six weeks, found by accident | 2026-09-29 |

## VI. Decisions (Fran, 2026-09-29, after reading v2)

- **Q33 → (a)**: the root `fabrica.stml` grows — dialect `fabrica` v2
  lists the subsystems; silex keeps reading only `titulus`. Subsystem
  declaration files take ANOTHER name (never `fabrica.stml`, which
  silex finds by ascent); the name is fixed in T2.
- **Q34 → raw**: `capsula_mensor.c` committed as the generator emits
  it (P2); capsulae are not formatted. **Widened in P2 (route B,
  Fran):** the pre-commit hook formatted every committed C file, so
  "raw" was impossible; now any file whose first line carries
  `GENERATUM` is committed as its generator's exact output, and the
  lexicon, runae and entitates tables are raw too — generate→format
  (§I.2) is retired for them, and their gates compare exactly.
- **Q35 → as recommended**: excubitor stays until slice 2, then oracle
  then delete (with `fabrica.tsv`); its six dead `silva_tabulae_*`
  rows are deleted now (folded into P2).
- **Q36 → as recommended**: 1a judges the ~12 installer-built
  binaries line by line; the rest of `bin/` is one IGNOTUM count.
- **Q37 → as recommended**: P1 before T1.

The questions as they were put:


**Q33. Declaration file name.** `fabrica.stml` is silex's workshop
marker and silex finds it by ascending from the cwd, so no
subdirectory may carry that name. Options: (a) the root
`fabrica.stml` grows children listing subsystems (dialect `fabrica`
v2 — silex reads only `titulus`), and subsystem files take another
name (`aedificatio.stml`, `actiones.stml`, …); (b) a different name
for both root and subsystems, the silex marker untouched.
Recommendation: (a) — the canon's comment already reserves the root
file for build options; Fran names the subsystem file.

**Q34. capsula_mensor.c committed form.** Commit the raw generator
output (as the other six are), or route it through `formatio` like
the lexicon? Recommendation: raw — capsulae are byte tables no one
reads, and six of seven are already raw.

**Q35. excubitor.** It is 1a's ancestor (judge-only, mtime, 188 s)
and it covers what 1a does NOT: `build/` objects. Recommendation: keep
it unchanged until slice 2 declares objects, then oracle-then-delete
it with `fabrica.tsv`; delete its six dead `silva_tabulae_*` rows now
(one-line cleanup, Fran's call).

**Q36. Installed-binary scope in 1a.** The ~12 with installers,
per-line; the other ~38 binaries in `bin/` (test and compile_tools
outputs) as a single IGNOTUM count. Recommendation: yes.

**Q37. P1 ordering.** P1 changes the manifest format (205 manifests
regenerate on next use; committed snippets unaffected — they list
`.c` files only; generata IV compares amalgam manifests and must stay
green). Do P1 before T1 (recommended: T7's briar/silex/spectator
depend on it), or park it and let T7 report those three as IGNOTUM?

Also still waiting (from before this spec): the three likely-dead dev
tools (`compile_genera_biblia.sh`, `glr_debug.sh`, `glr_quaestio.sh`).
