
## 2026-07-17 — K4 chunk C: tenant + deploy + seal

Chunk C decisions (spec-silent or spec-divergent, owned):
- Seed v4 parens NOT necessarium (spec said "titulus/parens req"):
  root branches carry parens "" and the empty-string law
  (K2 health) counts "" as absent — every root branch would sit
  insalubris forever. Divergence documented in the seed comment.
- _ramum_solvere resolves titles among ACTIVI only, res_id for
  any genus-ramus res (archaeology of fusi/abiecti). Consequence:
  a discarded branch is unreachable BY TITLE for writes — the
  title-uniqueness law does double duty as a write guard at the
  tenant layer (kernel still checks status; belt and braces).
- nexus/denexus + ramus REFUSED: the vinculum sugar is three
  writes plus _res_solvere on 'alterum' against TRUNK state —
  the exact E2-B1 trap. Raw membrum-additum on a branch already
  works (kernel G5). Parked as "quaerere conscium + nexus in ramo".
- Branch res/gerere verification uses gesta_res_in_ramo_datum
  emptiness as the res_id-law check — one error text covers both
  "you passed a title" and "that res_id isn't on this branch":
  "res in ramo ignota (in ramo res_id requiritur - resolutio
  tituli truncalis est)".
- One C89 lesson re-learned pre-compile: a guard statement cannot
  precede declarations — the nexus-branch ramus guard had to sit
  AFTER the declaration block.

Live seal evidence: the six-millisecond cascade in the instance
annales (gradus→porta→actio-acta→gradus→processus-perfectus, all
actor machina) is the K4 milestone's own claudere-cum-decreto
executing against its own park. Smoke test: probatio-vivi branch
born/written/compared/discarded — trunk untouched, seq 183
verified 1:1 including the branch rows.

## 2026-07-20 — gesta entitates: per-entity markdown projection (cleanup pass)

Design summary, written at the end of the branch (core + fixes were
already green when this entry was added — see the four commits on
`gesta-entitates` before this one for the actual mechanism history).

**Three-projection model.** The append-only `tabularium.jsonl` is
the one source of truth. Everything else is a derived, rebuildable
view of it:
1. `tabularium.db` — SQLite, queried by the tool surface at runtime.
2. `tabula.md` — one committed file, a diff-friendly summary of
   *open* items only (the "what's still moving" view).
3. `entities/` (new this branch) — one Markdown file per entity tag
   directory (`entities/<tag>/<genus>-<slug>-<id>.md`), full body,
   meant for browsing/grepping a whole entity's life in one file.
   **This is GITIGNORED**, same status as `tabularium.db`: it's a
   rebuildable artifact, not a record. Nothing under
   `gesta/annales/entities/` is ever committed.

**Reconcile design.** Two paths write the projection:
- Incremental, per-touched-entity: every `addere`/`gerere` call that
  changes a res calls `_entitatem_reconciliare(t, res_id, pn)` right
  after the event lands (tabularium.c ~2717, ~2478-2480 for the
  nexus/vinculum sugar which touches three res at once: both
  members plus the vinculum res itself).
- Full sweep: `_entitates_reconciliare_omnes` — called at
  `initialize`, and at `agere`/`ramus` boundaries (branch create/
  merge/etc.), because those can move many res at once in ways an
  incremental per-res reconcile can't cheaply track.

Both paths funnel through the same **delete-then-write** step:
`_entitatem_reconciliare` starts by calling
`_entis_plagulas_omnes_delere` (walks every tag directory, deletes
any file ending in `-<res_id>.md`), *then* writes fresh copies into
whatever tag directories the entity currently has. This means
retag, rename, and removal-of-a-tag all collapse into the same
idempotent operation — the filesystem itself is the memory of the
old tag set; there's no separate "diff old tags vs new tags" logic
anywhere. The full-sweep path does the same thing at directory
granularity (`_directorium_purgare` wipes every file under
`via_entitatum` first).

Content is deterministic by construction: every timestamp that
lands in the rendered Markdown comes from the event/row data
(`creatum` columns, event history) — never from a "generated at"
wall-clock read. So reconciling an entity that hasn't actually
changed produces a byte-identical file. This matters because full
sweeps happen somewhat often (every `initialize`) and we don't want
git-adjacent tooling or anyone diffing snapshots to see churn on
untouched entities. (The directory itself isn't committed, but the
determinism property is still worth preserving — e.g. it's what
makes the XXIIIc full-sweep test in probatio_tabularium.c a
meaningful assertion: "Probatio Entitatum Alpha", retagged before
the sweep, must show up under its *new* tag after
`_entitates_reconciliare_omnes`, proving the sweep isn't just
replaying stale incremental state.)

**Filesystem access is NOT raw POSIX.** All of it goes through
`filum.h`/`via.h`/`iter_directoria.h` (`filum_directorium_creare_si_
necesse`, `filum_scribere`, `filum_delere`, `via_iungere`,
`directorium_iterator_*`). This is deliberate, not just tidiness:
`tabularium.c` is walked by `silva examen` (our own C89 judgment
tool), and raw `mkdir`/`opendir`/`unlink` calls would either fail
its checks or just not be something the tool understands the shape
of. Keeping the OS coupling confined to `lib/filum.c` and
`lib/iter_directoria.c` (the only two files in the tree that touch
POSIX directory/file primitives directly — confirmed via a MAP.txt
correction filed the same day, see desideratum-adjacent note
01KXVF6X1RB3K6HMRT73HDDHF8) keeps `tabularium.c` itself fully
parseable and judgeable. Verdict on the test file as of this
cleanup pass: `ACCIPE`.

**Known limitations / deferred work** (filed as desiderata in the
ledger, not fixed on this branch):
- **Cross-entity Nexus links go stale after a rename/retag, until
  the next full sweep.** The `## Nexus` section of entity A's file
  contains a relative Markdown link built from entity B's *current*
  slug/tag at the time A was last reconciled. If B is later renamed
  or retagged, only B's own file gets rewritten (incremental
  reconcile only touches the entity that was actually the subject
  of the write) — A's link still points at B's old filename until
  something triggers a full sweep. Filed as desideratum
  **01KXVHZP9W** ("reconciliatio inversa" — the real fix is to look
  up B's *inverse* socii on every write to B and reconcile those
  too, which is a real cost/frequency tradeoff, not a free fix).
- **Emptied tag directories are never pruned.** When an entity's
  last file in a tag directory is deleted (retag/remotion of the
  tag), the now-empty directory is left behind — there's no
  `rmdir`-equivalent wrapper in `filum.h` yet (it has
  `filum_directorium_creare_si_necesse` and `_existit`, no
  `_delere` for directories). Cosmetic, not correctness-affecting.
  Filed as desideratum **01KXVF6NSE**
  (`filum_directorium_delere`) — small (~4 lines in filum.c) but
  deliberately not bundled into this branch to keep the projection
  work and the filum API change separable.

**The `remotio` subtlety** (worth restating because it's easy to
misread from the tool name): `gerere actus=remotio` deletes a
single JSON *key* from an entity's `datum` object
(`_sine_clave` in gesta.c ~743, applied in the "remotio" branch of
the plicatura ~912) — it does **not** delete the entity. There is
no full-entity delete anywhere in the tool surface. So in the
entity-projection world, a `remotio` on `clavis: corpus` reconciles
the entity's file (via the same `_entitatem_reconciliare` incremental
path as any other write) and the file keeps existing, keeps its
frontmatter/status/notes/nexus sections — only the corpus text
disappears from the body. Covered now by test XXIIIf in
`probatio_tabularium.c`.

**Gotcha for future maintainers, filed here because it wastes real
time if you hit it cold:** the gesta test suite (and this test
binary specifically) **must be run from the repo ROOT**:
`cd <repo-root> && ./gesta/compile_probationes.sh tabularium`.
`VIA_DB`/`VIA_AN`/`VIA_ENT`/etc. in `probatio_tabularium.c` are
relative paths (`gesta/build/...`), and `cfg.radix = "."` in the
test's `TabulariumConfiguratio` is also root-relative. Running the
same script from inside `gesta/` doesn't just fail the entities
tests — it fails `initialize` itself (can't find/create the DB or
annales files at the expected relative paths), which then cascades
into a wall of unrelated-looking failures for every later section.
If you see a mass failure starting right at `initialize`, check your
CWD before you start suspecting the actual change you made.

## 2026-07-20 — branch-event leak in the entity render (review find)

Reviewing the entitates commits: the `## Status` and `## Notae`
queries read tessellae by res_id with no `branch_id = ''` filter.
Two failure modes, one of them sitting in the suite's own generated
artifact: (1) post-merge, fundere's raw copies coexist with the
branch originals under the same res_id, so every status line and
nota rendered TWICE (probatio_entities/_sine_tag/nota-cogitatio-
ramalis-*.md showed exactly this — "creatum" x2, the in-branch nota
x2); (2) pre-merge, a nota/status written in a live branch on a
trunk res leaked into the trunk archive, violating "trunco
invisibilis usque ad fusionem".

Fix: `AND branch_id = ''` on both queries, plus the SAME K4-era
latent leak in _tab_res's annales query (meta-lines only there, but
identical mechanism). Regression = XXIIIg: asserts the merged
branch-born entity renders its nota and its creatum line exactly
once (the artifact-diff made this test nearly free).

Lesson: any DIRECT tessellae read by res_id is a trunk-purity
hazard — the K4 WHERE lives in _consumptorem_plicare, so fold-fed
surfaces are safe by construction but raw queries must carry their
own filter. Grep habit for future surfaces: `FROM tessellae` +
res_id ⇒ expect branch_id = ''.

Doc fixes same pass: spec no longer claims empty tag folders are
pruned (rmdir deferred, desideratum 01KXVF6NSE); the section
comment now cites project-specs/gesta-entitates-spec.md instead of
the gitignored .superpowers task brief.

## 2026-07-20 — vigilia manifest: true closure via clang -MM (01KXZYFVER)

Both launchers (gesta/tabularium.sh, officina/legatus.sh) now
compute the include closure with ONE `clang -MM` call (~0.3s each,
measured) and use it for BOTH the per-object rebuild checks and the
vigilia manifest — one source, so builder and watchdog cannot
disagree (the K2.2 invariant, now true in letter, not just spirit).

Before, the two launchers erred in OPPOSITE directions:
- tabularium globbed all of include/ (142 manifest entries, 118
  headers) → false staleness CAUTIO on every commit touching ANY
  header, plus a full recompile of all ~20 objects on any header
  change anywhere.
- legatus omitted lib headers entirely — a piscina.h edit never
  warned AND never recompiled (the silent-ABI-stale case) — and its
  link condition was `[ -n "$obj_files" ]`, always true, so it
  relinked on every single launch.

After: tabularium manifest 142→44 entries (22 headers = the true
closure); legatus 35 entries with lib headers finally covered.
Twin-specimen on both launchers: touch vitrea.h (non-dependency) →
0 build actions; touch piscina.h (true dependency) → full cascade +
relink. Fallback: if -MM fails (broken header mid-edit), the old
glob/hand-list superset — loud, never blind.

Gotchas recorded:
- basename collision in legatus (instrumenta/legatus.c AND
  principalia/legatus.c both produce "legatus.o:" dep lines) —
  deps_obiecti unions all matching lines; per-object superset, safe.
- awk joins the -MM backslash continuations (portable; BSD sed
  label syntax is finicky).
- The amalgam correctly does NOT depend on lib headers — specimen B
  rebuilt lib objects and consumers but not amalgama_silva.o. The
  old hand-maintained SILVA_H ABI checks are subsumed by the truth.

## 2026-07-22 — acta (K4.5 frustum 1)

Global event-stream tail as a read tool. Notes:
- Implemented ENTIRELY in the tabularium layer (handler-local SQL
  via gesta_scrinium) — census's rami-activi count is the
  precedent. If a second consumer wants the query, extract to
  gesta_acta_recentia then.
- One SQL, two optional filters via the double-bind idiom:
  `(? = '' OR col = ?)` binding the same value twice — no dynamic
  SQL assembly.
- Truncus only (branch_id = ''); a ramus param is the named
  future.
- Summarium = first textual key from the event datum, order:
  textus, novus, corpus, titulus, actio, verbum, clavis, valor.
  corpus deliberately BEFORE titulus (creatio events would echo
  the res title otherwise) + chorda_aequalis skip when the
  summarium equals the title anyway.
- Cut at first newline or LXXX bytes with "..." marker.
- Live-drive gotcha: the real server requires the initialize
  handshake ("nondum initiatum" -32002) — the probatio's _mitte
  path initializes in setup, a bare printf-drive must send
  initialize first.
