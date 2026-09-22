
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

## 2026-07-22 — F0 forum: tabulariumd + genera fori

**Bind-sine-listen trap (tcp domus).** `tcp_servus_creare` facit
socket+bind SOLUM; `tcp_servus_auscultare` (listen) est vocatio
separata. Symptomata sine ea: daemon "auscultat" imprimit, clientes
connexionem RECUSANTUR (backlog nullum), et ansa accipere in
"Servus non auscultans" errorem cadit + `perge` = nete occupata
100% CPU sine servitio. Hospitium eandem sequentiam vocat — lege
consumptorem exsistentem primum (lex caller-init generalisata:
API creare/auscultare bifidum).

**nc stdin-EOF.** macOS nc post EOF stdin cito exit — responsa
numquam visa. Fumus manualis: `{ printf ...; sleep 1; } | nc`.

**fdopen(dup(fd)) bis.** FILE* lectionis et scriptionis uterque
descriptorem PROPRIUM possidet (dup bis); originalis tcp_claudere
clauditur. fclose ordine quovis tutum.

**Signatura post ramos actuum.** In _tab_gerere mutatio cruda
`datum = r.radix` obiectum commune SUBSTITUIT — positio unica tuta
pro clavi signaturae = post ramos omnes, ante serialisationem.
Nexus/denexus supra redeunt sine ea (acceptum: creatio commentarii
eam fert).

**Numerali phantasmata.** CCCLXXXIV/CCCCLXXX in latina.h desunt
(480 = CDLXXX subtractivum) — examen ambo ante compilationem
cepit (identificator ignotus).

## 2026-07-22 — F1 fori: proxy stdio↔TCP

**Fovea suspensionis nuntiationum.** Daemon nuntiationes tacite
omittit — proxy naivus post quamque lineam responsum legens in
"notifications/initialized" penderet in aeternum. Solutio =
classificatio VERA per tabellarius_nuntium_legere (PETITIO/PRAVUM
exspectant, NUNTIATIO/RESPONSUM mitte-et-obliviscere), non
divinatio chordae ("id" in corpore textus apparere potest).

**Generatio lateralis in probationibus.** Start-if-absent intra
proxy daemon VERUM gigneret in probationibus (mundus verus, portus
probationis!) — vexillum -sine-generatione: defectus statim, error
curae portu VERO nominato. Generatio launcher portum configuratum
transfert (-portus %d), non ordinarium caecum.

## 2026-07-22 — F2 forum: instrumentum legere + app prima

**Legere = lectio structurata.** Inventum F2: exitus instrumentorum
omnium = textus humanus — app tabulam reddere non potest sine
analysi textus fragili. legere reddit TABULATUM JSON ut textus
responsi (involucrum MCP idem manet): datum plicatum INSERTUM ut
obiectum (parsare + reponere, non chorda — corpus/signatura/tags
gratis), actor per subquaestionem eventus creationis (tabula res
actorem non fert), respondet_ad per subquaestionem membra a→b cum
verbo per LIKE in dato nexus (cruditas v1: verbum fori unicum,
json_extract sqlite vitatum — commentata in fonte).

**Involucrum duplex clientis.** App parsare debet BIS: involucrum
MCP (result.content[0].text) deinde textum ipsum ut JSON. Quotes
in responso crudo effugiuntur (\") — asserta strstr sine citationibus
scribere.

**Daemon stalus post instrumentum novum.** Fumus primus "instrumentum
ignotum" — daemon currens legere praecedebat. Necare sufficit:
start-if-absent per launcher REAEDIFICAT (staleness-podatum) et
respawnat. Haec est via renovationis daemonis fori: pkill + vocatio
proxima.

## 2026-07-22 — cp super binarium = SIGKILL (macOS, conventio apps/)

`cp novum bin/forum` SUPER binarium exsistens → exec sequens
SIGKILL (exitus 137) dat: macOS arm64 signaturam ad-hoc inodi
mutati invalidat. Primus cp (destinatio absens) semper viget —
fallacia latens usque ad secundam constructionem. Cura: `rm -f`
ANTE cp (inodus recens = signatura recens). In forum.sh fixum;
conventio apps/ omnium futurorum.

## 2026-07-22 — F4: cursor lectionis + ad + salutatio

**Cursor in consumptoribus.** Tabula consumptorum (titulus, seq)
iam in nucleo erat — "claude-lectum" = ordo unus novus, INSERT OR
REPLACE. Scrinium proiectio rescribilis: cursor perditus =
salutatio iterata, non damnum. ab_lecto ordine ASC (ordo
lectionis) — provectio ad maximum REDDITUM, non ad hwm (DESC+LIMIT
media saltaret). Provectio filtra ignorat — in descriptione arg
nominatum ("optime sine filtris").

**Salutatio, non pulsus.** Initialize instructiones dynamicae:
cursor absens -> conditus ad hwm TACITE (primus conventus non
clamat); praesens + retardatio -> "NOVA: N eventa". Sessio quaevis
nova sponte salutatur — vis nuntiationis exacte recta.

**Schemata MCP onerata consenescunt.** Post mutationem
instrumentorum daemonis, schema sessionis currentis vetus manet
usque ad reconexionem — sed parametra extra transeunt
(additionalProperties ordinarius verus in JSON Schema). "ad" per
schema vetus functum est vivo.

## 2026-07-23 — genera G0: the machine layer of user-defined genera

Three finds worth keeping:

1. **The projection hooks the RES fold, not the genera consumer.**
The explorer plan said "dedicated branch in the genera consumer
keyed on datum.genus==definitio" — but a mutatio event on a
definitio entity carries ONLY the changed keys (no genus marker,
no clavis). The genera consumer is event-shaped; only the res fold
has the full folded datum. So `_definitionem_proicere` runs at the
end of `_rei_applicare` after the res-row write, where
status_obiectum is complete. Corpus-contact law paid again: ten
minutes at the seam flipped the design.

2. **Tool-text asserts need escaped quotes.** legere's payload is
JSON-inside-JSON (tool text embedded in the MCP envelope), so the
raw response bytes contain `\"annus\":-52`, not `"annus":-52`.
Asserting with plain-quote needles fails while the data is
perfect. Either assert bare words (the F2 precedent) or write the
needle as `\\\"annus\\\":-52` in the C literal. Cost: one
duct-tape imprimere to see it.

3. **Registry rows are never deleted (v1 crudeness, deliberate).**
A clavis-change mutatio on a definitio entity is judged ("clavis
immutabilis" custody note) but the write proceeds — and the
projection then creates a NEW genera row under the new clavis; the
old row stays. Same for deleting a definitio entity: row survives.
Harmless (rows are additive, system rows are clobber-protected via
the usor=0 guard), but the registry can accumulate orphans if the
judgment is ignored. The app never offers these edits; MCP writers
see the custody note.

Also banked: unum-cardinality judgment reads the nexus's verbum
from the FOLDED res — in a same-batch multi-event write (K3 agere
effectus) the shadow carries genus only, so the judgment silently
skips. Ordinary paths (gerere nexus, addere ad) fold per call, so
the window is theoretical until actions start creating relatio
links.

## 2026-07-23 — genera G1: the live gate found what the unit gate couldn't

The wire test (probatio_fori datum round-trip) and the live MCP
gate both passed on the first run — but the live gate immediately
exposed a consistency bug the unit sections were structurally
blind to: `_nova_numerare` counted WITHOUT the new ab_lecto
exclusion predicates, so the reader was told "et 1 plura restant"
and the very next drain said "(nihil novi)". Worse, the same
counter feeds the NOVA greeting — a data-entry-only day would
greet the next session with mail that doesn't exist.

The law worth keeping: **a counter that PROMISES a filtered read
must use the read's exact predicate.** Unit sections test each
surface in isolation (ab_lecto shows X, greeting says Y); only a
lived sequence — write data, read mail, read again — crosses the
promise and the delivery. This is the second time the lived bar
caught a cross-surface lie the section bar missed (F4's blindness
proof was the first). Budget a live pass after every reader-side
change.

Fix: same LEFT JOIN + double NOT EXISTS in _nova_numerare;
pinned with "entry-only write → NOVA silent, nothing promised."

## 2026-07-23 — genera: the two-layer title (picker gap)

Every res has a MACHINE titulus (addere param → datum.titulus →
res.titulus column → what lists, relation chips, and FTS show).
A user genus ALSO has its campus_tituli FIELD (datum.<name>) —
what the table column and the entry form show. The form save
keeps both in sync; the inline-create picker only wrote the
machine titulus, so picker-born entities showed their name in
chips/headings but blank in the Name column/field. Fix: picker
now writes the target's campus_tituli field too (fumus asserts
parity). Fran's four persons repaired via MCP mutatio.

Note the orphan machinery CORRECTLY stayed silent here — a
missing field is legal absence, not a spec violation. The gap was
only findable by a human looking at their own data: the lived bar
again, third find in one day.

## 2026-08-01 — legere: nexus filter (01KYZG0V0K)

Born from the Libri tab. Chapters are entities linked to their book
by a `liber` nexus, and `legere` could filter only by genus — so the
app read EVERY `capitulum` in the store (200 ceiling) and selected
client-side. That makes the ceiling a GLOBAL budget: 10 books × 40
chapters = 400, the read returns 200 most-recent-first, and the books
entered earliest quietly show short tables of contents. Worse, the
"append after the last chapter" arithmetic is computed from that
truncated list, so new chapters collide with existing `ordo` values.

Added `nexus_verbum` + `nexus_ad`, both optional, combining inside a
single EXISTS so "verbum X to target Y" must match ONE nexus rather
than two different ones. Subject is `pars = 'a'`, target `pars = 'b'`
— same convention as the nexus projection and `respondet_ad`.
`nexus_ad` resolves ULID prefixes like `res` does; anything else
would be a silent no-match.

### Two things worth carrying forward

**json_extract, not LIKE.** The neighbouring `respondet_ad` subquery
matches `n.datum LIKE '%"verbum":"respondet-ad"%'` — fine, because
that verbum is a literal in the source. Here the verbum comes from
the caller, and `%` in a LIKE pattern is a wildcard: a caller passing
`%` would match every nexus and get a confidently wrong answer with
no error. SQLite 3.53 has JSON1 built in (we don't set
SQLITE_OMIT_JSON), so exact comparison was available all along.
Pinned with a test that passes `%` and expects nothing.

**The both-empty guard is load-bearing for the whole store.** Without
`((? = '' AND ? = '') OR EXISTS (...))` the EXISTS degenerates to
"this res has SOME nexus", so every unfiltered `legere` silently
drops link-less entities. I broke it deliberately to calibrate the
new regression test and it took down EIGHT assertions, most of them
pre-existing — pipata, captured ideas, single-entity reads. A
feature-shaped change to a shared WHERE clause is a change to every
caller of that clause; the only way to know is to watch it fail.

### The bug that never fired

`_res_legere` in apps/forum/forum.c builds a fresh argument object
and forwards arguments BY NAME (genus, quantum, sine_campis, res).
The two new arguments would have been dropped there in silence — and
since the app had just stopped filtering client-side, every book
would have listed every chapter, with the daemon correct, the webview
correct, and no error anywhere. Found by reading the handler before
trusting it. Comment left at the site.

Live proof on Fran's data (42 Lunar Men chapters, via the HTTP
bridge): filtered read = 42, all one book; unfiltered `pipatum` read
still returns 30 link-less entities out of 40; a book with no
chapters returns 0 rather than everything.

## 2026-09-21 — the compact view shows links, with the other side's status

**What changed.** `res {breviter:"verum"}` now carries the `nexus:`
section, and in BOTH views each row ends with the other item's genus
and status: `--impeditur-a--> Cache calida (parcum, clausum)`. One
shared function, `_vincula_reddere`, serves the full view and the
compact one (the K4.2 rule: a shared renderer so the two forms cannot
drift). Rows are capped at `VINCULA_OSTENSA_MAXIMA` (XII) and the
remainder is COUNTED — `(et alia N)` — never dropped silently.

**Why.** K4.2 left `socii` out of the compact view on purpose, to keep
it cheap. But the compact view is the one an agent actually uses (the
full view prints the raw datum — the context trap of 01KYCMQMED), so
in practice blockers were never seen by the reader they exist for.
Found 2026-09-21 by opening an item I had linked three ways an hour
earlier and seeing none of them. And a title alone does not answer
the real question — *is my blocker still open?* — without a second
call. Measured on the live store the same day: of nine live blocking
links, in FOUR the blocker had already closed while the dependent sat
open, and nothing said so.

**A genus without a lifecycle prints no comma.** `decretum` and `nota`
have an empty status column; the row reads `(nota)`, not `(nota, )`.
A member that is not a `res` at all (tolerant members carry raw text)
is printed bare, as `_titulus_membri` always did.

**Two plants, because one would have verified half.** (A) dropping the
call from `_breviarium_reddere` reddens `strstr(r, "nexus:")` — the
compact half. (B) blanking the status in the SQL reddens the
`(parcum, parcatum)` assertion — the status half, which the full view
shares. Plant A alone would have left a title-only renderer green.
Both went red naming a different assertion, both restored green
(`silva.planta`, gate `gesta`, 430 assertions).

**Anchoring note for the next plant here.** `si (status.mensura >
ZEPHYRUM)` occurs four times in this file and
`chorda_aedificator_appendere_chorda(aed, status);` six; the SQL
literal in `_socium_describere` is the unique anchor. And a multi-line
anchor replaced by a flat `novus` with a different token count is
refused by `Editio` ("forma perderetur") — give the replacement the
same number of lines.

**Doctrine and schema text updated** (the auto-loaded doctrine string
and the `breviter` parameter description), because that text is how a
fresh session learns what the compact view contains.

NEXT in this arc (decree 01M32TEK3K, park 01KY57P2WD): declare the five
canonical link verbs in the seed, refuse known near-synonyms loudly,
migrate eight links, then the PARATA view.

## 2026-09-21 — five canonical link verbs, declared as data and enforced at the door

Decree 01M32TEK3K (Fran). Five verbs a machine reads, ONE direction
rule — the arrow runs from the dependent item to the one it leans on:
`impeditur-a` (blocked → blocker, hard), `intra` (child → parent),
`natum-de` (new → origin), `sequitur` (later → earlier, soft),
`respondet-ad`. Everything else stays free: a verb becomes canonical
only when a view reads it.

**Why it was needed, measured.** 37 distinct verbs across 103 live
links; blocking was spelled three ways (`impeditur-a`, `pendet-ex`,
`impedit`) in TWO directions. No ready view can be built on that.

**Where the table lives.** In the `nexus` genus as DATA
(`verba_canonica` + `verba_versio`), written by seed v7 as an
`emendatio-generis` in the manner of seed v2 — so it is visible in the
ledger and rides the event log like every other genus fact. The
handler reads the table from the genus datum, never from the C
constant: a world whose seed predates this refuses nothing, which is
the progressive law working.

**The version guard is monotonic on purpose.** "Emit when the stored
table differs from mine" would be simpler and is WRONG here: five
residents are routinely alive at once, plus worktrees with their own
binaries. Two binaries with different seed tables serving one world
would revert the genus back and forth, one event per start. `condita <
VERSIO` cannot flip-flop. To change the table: edit `VERBA_CANONICA`
AND bump `VERBA_CANONICA_VERSIO`.

**A fresh world takes the upgrade path too** (definitio, then
emendatio) rather than carrying the table in `SEMINA_GENERUM`. One
extra event per new world, and in exchange the upgrade path runs in
every test run instead of only on Fran's live store.

**Refusal, not normalisation.** A known near-synonym is refused
loudly, names the canonical verb, and for an inverse spelling says to
swap `res` and `alterum`. Silent rewriting would teach the writer
nothing and would hide direction mistakes — and direction is the part
that matters. Only link CREATION is judged: `denexus` is untouched, so
links written under the old spellings can still be dissolved (the
migration needs exactly that).

**Synonym lists are evidence, not a thesaurus.** Only spellings that
were actually written in the live store. `pertinet-ad` is deliberately
NOT a synonym of `intra`: two of its four live uses mean "is about",
which is a legitimate free verb.

**THE DOCTRINE IS TRUNCATED AT 2048 CHARACTERS.** Measured while
deciding where to document the verbs: the copy this session received
ends mid-word at character 2048 of a 4017-character string. A session
sees 51 % — the whole MORES section (pull → `tractum` at once, finish
→ `clausum`, "tabula mendax peior prosa"), the cold path, `renovare`
and the forum contract are NEVER seen. So the verbs are documented in
the `gerere.verbum` SCHEMA text (loaded with the tool, not truncated)
plus one short early clause in the doctrine, and a test asserts the
schema carries them. Restructuring the doctrine so the habits come
before the grammar (which the schemas already repeat) is its own step
and needs Fran's eye — it is his contract text. Filed as a question.

**Three plants, three different assertions.** (A) judge cannot find
the table → the synonym refusal reddens. (B) `<` → `<=` in the version
guard → the pre-existing tabula idempotence assertion reddens first
(my `verba_versio` count would redden too). (C) `inversa` not read →
`impedit` slips through as a free verb and the inverse refusal
reddens. A weak assertion was caught before it shipped: checking for
`"impeditur-a"` in the annals passed BEFORE the implementation,
because section X's links already write that verb; it now checks
`"titulus":"impeditur-a"`, which only the seed writes.

Gesta 680 · tabularium 449 · tabulariumd 12 · fori 18.

## 2026-09-21 — a refusal TEACHES, and reports every cause at once

Fran, reviewing the verb refusal the same day it shipped: does the
error say what a VALID write looks like? And when there is more than
one rule, show all the errors together — no drip feed where you fix
one thing and are stopped by the next.

Honest answer to the first: only half. The message named the canonical
verb and said "swap the ends", but never showed the call. And the link
path checked its rules in sequence with early returns (branch →
missing args → verb), so a caller could be stopped three times in a
row.

**Two laws, now in `_nexum_praeiudicare`'s header comment:**

1. **NEVER DRIP.** No check returns early. All run, all are reported,
   numbered, under one count: `nexus RECUSATUS (2 causae)`.
2. **A REFUSAL TEACHES.** The response ends with the corrected call,
   filled with the caller's own values (res_ids where they resolve,
   `<alterum>` placeholders where something is missing), then a gloss
   in titles — `A --impeditur-a--> B` — so direction can be checked at
   a glance, then the list of canonical verbs so a verb can be chosen
   without another call.

**The hidden drip.** A check can be BORN from a correction. An inverse
verb swaps the ends, so `alterum` becomes `res` — and `res` must
resolve, whereas `alterum` is tolerated as raw text. A naive "valid
call" would have handed the caller a second failure one step later.
That check now runs inside the same pass. When adding a rule, ask what
the CORRECTED call newly requires.

**Where new link rules go:** in `_nexum_praeiudicare`, never as an
early return in `_tab_gerere`. `_verbum_iudicare` now returns
structure (canonical verb + inverse flag) rather than a finished
sentence, so the collector owns the wording.

**Plants (three, three different assertions):** (A) suppress "alterum
deest" when another cause is already present — the drip behaviour
itself — reddens `(2 causae)`; (B) do not swap the ends in the valid
call → the titled gloss reddens (and the test also asserts the
UNSWAPPED form is absent); (C) skip the resolvability check born from
the swap → `(2 causae)` reddens on the inverse-plus-unknown case.

The pre-existing branch test (`nondum sustentus`) still passes
unchanged — it was already a three-cause input (branch + inverse verb
+ unresolvable `x`) and now gets all three in one response.

Scope note: this covers the LINK path only. `addere`, status changes
and the engine's fascis validation (`gesta_error` is a single string)
still report their first failure. The command-line write path, next,
is to be born under both laws.

Tabularium 466 · gesta 680 · tabulariumd 12 · fori 18.

## 2026-09-21 — link pre-check made public and given `res`; see frigida.worklog.md

`tabularium_nexum_praeiudicare` (public, READ-ONLY) and
`tabularium_claudere` (public) added for the command-line write path.
`_nexum_praeiudicare` now takes the RAW `res` key and resolves it, so
an unknown or ambiguous item is one cause among the others instead of
an early return ahead of them. `_candidatos_appendere` factored out
of `_ambiguitatem_respondere` and shared. Tabularium 469.

## 2026-09-21 — plan tasks: the task genus declares a body, and search leaves tasks out by default

**Decision (Fran).** Plan tasks at commit grain use the existing `opus`
genus rather than a new one: a unit of work with a done state,
whatever created it. Processes and plans become two sources feeding
one future ready view. The engine wakes on MEMBERSHIP of a running
instance, not on genus, so a hand-made task is inert to the motor.

**Seed v8 declares `corpus`, `tags`, `ancorae` on `opus`.** Not a new
permission — undeclared keys already passed silently, and the compact
view, census, FTS and anchor resolver already read those keys on ANY
genus (probed in a throwaway world first: create, link `intra`, start;
zero guard notes). Declaring is schema honesty. Same rule as the
verbs: an attribute is declared when a machine READS it, so measured
`progressus` is deliberately not declared yet. Monotonic version guard
(`attributa_versio`), same reasoning as the verb seed.

**"E2-B2: board attributes are never stamped into opus" was a scoping
fix, not a principle** (`gesta-k3-exploratio-2.md:61`): the v2 fusion
was unscoped and would have blindly stamped action and process
definitions.

**A vacuous assertion, found by breaking it.** Section XVI asserted
`opus` has no `ancorae` as an E2-B2 guard. It could never have failed:
v2 only touches genera WITHOUT an `attributa` key, and `opus` always
had its own. The real guards are the action and process assertions
beside it (they have no attribute list), and they stand. Replaced with
what now needs guarding: v8 rewrites the WHOLE definition, so nothing
original may be lost (`assignatum`, `prioritas`, `effectus`, the
machine's `pendens`/`omissum`).

**Search excludes tasks by default, and says so.** Measured the same
day: every other genus carries ~200–275 words of written knowledge per
item (body + notes) and 100 % have a body; the four existing tasks
carry zero, with nine-character titles ("spec"). A few hundred of those
would bury the knowledge base. So `quaerere` without a `genus` leaves
`opus` out — IN THE SQL (`gesta_quaerere_excluso`), not afterwards,
because the engine caps at 50 rows and a numerous genus would crowd
real hits out before any post-filter ran. NEVER SILENT: the response
ends `(opera exclusa: N congruunt - adde genus: "opus" …)`, including
when tasks are the ONLY matches (a bare "nihil inventum" would lie).
An explicit `genus` beats the exclusion.

**The duplicate guard follows the same split.** A new knowledge item
is not shown tasks as "similar"; a new task is compared only with
other tasks.

**Six plants, six different assertions:** SQL exclusion neutralised ·
count not computed (the silent form) · server default off · duplicate
guard shows tasks · v8 adds nothing · v8 version guard `<=` (caught by
the older tabula idempotence assertion).

Gesta 687 · tabularium 488 · frigida 50.

## 2026-09-21 — `parata`: the ready view, derived from the link graph

**What it answers.** "What can be done NOW?" — computed at read time
from the two canonical verbs (`impeditur-a`, `intra`). Nothing is
stored: a stored derived status would go stale, and this ledger has
seen enough hand-kept indexes lie.

**Who is in the view.** Open items that stand IN THE GRAPH (carry any
canonical link) plus every `opus`. A lone open item is NOT listed. The
ledger is a knowledge base — 100 open wishes — and the queue is what
someone deliberately placed in the graph. A ready list of 100 stale
wishes would be noise, not a queue.

**Five classes, one per open item in scope:**
`AD LABOREM` (a task with no open blocker) · `AD CONSILIUM` (any other
kind with no children and no open blocker — the next step is to EXPAND
it into a sub-plan) · `AD CLAUSURAM` (all children closed, item still
open: a parent needs its OWN exit test, because under rolling-wave
planning the child list is never known to be complete) · `EXSPECTANT
FRANUM` (explicit `assignatum: fran`) · `IMPEDITA` (open blocker
named). A container with open children is in NO class — its children
tell the story.

**Decisions worth remembering.**
- *Blocked-ness is inherited.* A task under a blocked stage is not
  ready even with no blocker of its own; the cause names the parent.
- *"Waiting on Fran" is never guessed from the genus.* A `quaestio` is
  a bug, a question AND a decision; only an explicit assignment counts.
- *An abandoned blocker unblocks LOUDLY* (`IMPEDIENS RELICTUM … -
  recogita`), per decree 01M32TEK3K. A blocker whose genus has no
  lifecycle can never close, so it does not block.
- *Final states are DERIVED from the genus machine* (entered, never
  left), not listed by hand. The board's hand list
  `clausum/relictum/impletum` did not know a task's `perfectum`, so
  finished tasks sat on `tabula.md` as open. Same function now filters
  the board.
- *Every row says WHY*: blockers closed (naming the last), how many
  open items it blocks (leverage — those sort first), its parent.
- *Deterministic order, no clock* — dependents descending, then
  `res_id` — because `tabula.md` must stay content-deterministic.
- *Titles are capped at 96 bytes ON A UTF-8 CHARACTER BOUNDARY.*
  Ledger titles are sentences (some 200+ bytes) and the first live run
  was unreadably wide. A naive byte cut would leave a bare lead byte
  and emit invalid UTF-8 inside a JSON response; pinned with a title
  whose 96th byte falls inside an `é`.

**`tabula.md` opens with `## PARATA`** (empty classes omitted, ten
rows each), so Fran sees the queue and his own list without a tool.

**First live run (2026-09-21):** 0 tasks · 7 to plan — the two
highest-leverage items first, and the FOUR whose blocker had quietly
closed, each naming it · 1 to close (the K4.5 park: its one recorded
child is done — true of the GRAPH; its remaining steps still live in
prose, which is the very thing this arc is replacing) · 3 blocked.

**Tests lied twice, both times by substring.** (1) "T1 is gone from AD
LABOREM" failed on a correct view: T1's title rightly appears in T2's
reason ("blockers closed, last: T1"). Look for the ROW
(`opus/perfectum`), not the bare title. (2) "the container is in no
class" only checked AD CONSILIUM; plant J showed a container with open
children would have been offered for CLOSING and the test stayed
green. Now checks both, by row.

**Ten plants, ten different assertions** (finality · inheritance ·
scope · out-of-graph items · assignment · closure class · abandoned
flag · UTF-8 boundary · board filter · container). The first finality
plant did not COMPILE (`-Wunused-but-set-variable`); `planta` refused
it, correctly — a plant that breaks the build proves nothing. Replaced
with one that compiles.

Not done: `sequitur` does not yet order the list; `quantum` caps per
section only; no `census` counts yet.

Tabularium 555.

## 2026-09-21 — `parata`: an explicit assignment puts an item in the queue, like a link

Found by the first live use, minutes after shipping: I assigned the
doctrine question to Fran (`assignatum: fran`) and `EXSPECTANT FRANUM`
stayed empty — a lone item carries no canonical link, so it was never
loaded. But assigning is as deliberate an act as linking; the "only
what was placed in the graph" rule is about deliberate placement, not
about links as such. Items with `assignatum = 'fran'` are now loaded
too; an unassigned lone item still stays out (pinned both ways).
Plant K reddens the new assertion. Tabularium 559.

## 2026-09-21 — `census`: ready-view counts, and the tag tail is counted instead of printed

Task 01M335DSK5 (arcus VII). Two changes to the dashboard an agent
reads first after a compaction.

**Counts line.** `parata: ad laborem N, ad consilium M, ad clausuram K,
exspectant Franum J, impedita I  (singula: parata {})`. It comes from
`_parata_computare` — the SAME function the `parata` tool uses — not a
second count that could drift. The test reads the five numbers out of
the tool's own section headers and requires the census line to match
them exactly.

**Tags: top twenty, the rest counted.** Measured on the live store the
morning this arc began: 511 distinct tags, 252 of them used ONCE —
about 450 lines per `census` call, paid at exactly the moment context
is dearest. The engine already sorts by count descending, so the head
of the list is the part that orients. The tail is now one line:
`(et alia N tags, quorum M semel usa - census {tags: "omnia"} omnia
dat; quaerere {tag} unum sequitur)`. Never silent, and it names both
ways out. `tags: "omnia"` prints everything as before.

Pinned with 25 singleton tags plus one FREQUENT tag that sorts last
alphabetically (`zzfrequens`): it must survive the cut, which proves
the cut follows count, not name.

Nothing parses the census text (checked: the `census` hits under
`apps/forum` are the unrelated `rhubarb.census` format).

Four plants, four assertions: counts doubled · no trim · `omnia`
ignored · silent trim (remainder not reported). Tabularium 579.

## 2026-09-22 — status and mutation refusals: all causes, legal next statuses, and an explicit override

Task 01M335DZJM (arcus IX). The link path got Fran's two refusal laws
on 2026-09-21; status and mutation now have them too.

**What a bad status did before this.** Measured in a throwaway world:
an illegal transition was WRITTEN. The engine's "record, don't block"
law fired, a guard note landed on the item, and the tool answered
"eventum status scriptum" with no error flag. The writer learned
nothing, the item sat at a status its own machine does not allow, and
`insalubres` read zero. Two writes in a row put a park at `apertum`
without a whisper.

**Now: refused at the door, with the way forward.** `_statum_praeiudicare`
runs BEFORE the handler's general item resolution (so "unknown item"
is one cause among the others, never a first stop) and collects:
item unknown or ambiguous · `novus` missing (with the legal statuses,
so the writer can choose without a second call) · same status (an
empty event) · transition outside the genus machine, naming the
current status and the LEGAL NEXT ones from the machine. A genus with
no machine (`nota`, `decretum`) judges nothing — the progressive law.
The valid call ends the message; with exactly one legal successor it
is filled in, with several it shows `<a | b>`.

**The override.** Fran's question was refuse-vs-warn; he chose refuse,
and I argued the engine's law should stay reachable on purpose:
`vis: "verum"` writes anyway, and the guard note follows as before. A
fact worth recording must not be lost to a schema gap — but the
writer has to say so.

**Mutation.** `datum` must be a JSON object, or `clavis`+`valor`; and
the key `status` is refused on this path (the machine judges status,
mutation would bypass it), pointing at `actus: "status"`.

**Cold path.** `-vis` flag (no value); `-status` runs the new public
read-only `tabularium_statum_praeiudicare`, so the paste-ready command
carries the legal statuses: `-status "<id>" <tractum | clausum>`, plus
a line saying how to force. A machine placeholder like `<a | b>` is
passed through verbatim (the old code would have re-wrapped it as
`<novus>`).

**Not changed:** the engine and batch effects keep "record, don't
block"; only this door refuses. Health (`insalubres`) still does not
flag an item at an unknown status — that is a history problem, not a
write-time one, and belongs to a later task.

**Seven plants, seven assertions:** illegal not refused · same status
passes · `vis` ignored · in/out swapped when reading the machine
(caught by the OLD section VI assertion — the legal list would have
been wrong for every genus) · `status` via mutation passes · `-vis`
not forwarded · cold-path pre-check skipped. One anchor had to be
byte-exact (`tolerans=False`) because the token form crossed into a
comment.

Tabularium 608 · frigida 66.

## 2026-09-22 — compact view: the closing commit, and a parent's ready children

Task 01M34RACAD (arcus XI), from Fran asking what would help in use.

**`effectus` line.** The commit wiring writes `effectus` ("hash: first
line of the message") on every task it closes, but the value lived
only in the raw datum, so the compact view could not answer "what
closed this?". Now one line, `effectus <value>`, in both views, only
when set, capped at 120 bytes on a UTF-8 boundary.

**Ready children on a parent.** An item with children by `intra`
shows `parata sub hac re:` — its children's ready classes from the
SAME derivation as the `parata` tool, scoped to the item, empty
classes omitted, eight rows per class with the remainder counted. So
one `res` call on a project gives the tree AND the queue: the
return-to-parent view. The item's own class is suppressed (a park in
"ready to close" would otherwise list itself among its children). An
item with no children prints nothing; one whose children are all
closed or blocked says `nihil paratum` — silence would not tell
"nothing under it" from "everything blocked".

`_parata_sectionem` gained an omit-empty switch rather than a copy;
the tool keeps printing `(0)` headers so a reader knows a class was
checked, the board and the child section omit them.

**Two things my own tests got wrong, worth remembering.** (1) The
response is JSON text: a newline arrives as the two characters `\n`.
Three assertions looked for a real newline and failed against correct
output. (2) Plant A stayed GREEN and `planta` said so ("porta muta"):
my fault threshold (`> M`) was never reached by the fixtures. A plant
that cannot fire on the fixtures is not a plant. Replaced with one
that bites (`>= ZEPHYRUM`).

Five plants, five assertions. Tabularium 623.

## 2026-09-22 — `quaerere {intra}`: full-text search inside one project

Task 01M335DW3M (arcus VIII). `intra` takes an item (id, prefix or
title) and restricts the search to its SUBTREE — the item itself and
everything under it through `intra` links, to the same depth the
ready view walks. `_rei_in_scopo_est` climbs the parent chain through
live links (first parent wins, as in the ready view).

**The filter runs AFTER the FTS query, which is the opposite choice
from the task exclusion.** The task exclusion had to be in the SQL
because a numerous genus would crowd the 50-row cap before any
post-filter ran. A subtree filter is the other way round: the scope
is small and specific, so post-filtering the 50 hits loses little,
and pushing a recursive parent walk into SQL would be real work for
no measured need. Recorded here so the asymmetry does not look like
an oversight.

**Named in every response.** The result header says `intra '<title>'`;
the empty case says it too, with "omit intra for the whole ledger" —
a bare "nothing found" would read as a verdict on the whole store.
An unknown or ambiguous scope is refused with candidates. Task
exclusion still applies inside a scope, still counted.

**Plants.** Four: parent chain cut at the first hop · filter bypassed
· empty message without the scope · unknown scope silently widened
to the whole ledger. The last needed a by-hand line edit: the guard
is byte-identical to the ready view's, so `planta` rightly refused
the ambiguous anchor. Restored by `cmp` against a backup.

Tabularium 646.

## 2026-09-22 — design questions: `natura: consilium`, AD COLLOQUIUM, and "seen, not pinned"

Task 01M34TE9Y5 (arcus XII), from the planning conversation with
Fran: the ledger had a queue for "what can I build" but none for
"what is ripe to talk through", and no way to see the landscape of
open design questions blocking a project.

**Measured first.** 40 open `quaestio` items, 38 of them bugs. The
real design questions lived in prose: 21 spec files carry an
AUDIENDA section, and the text-stack park listed EIGHT decisions
waiting on Fran in its body, none of them a ledger item. 68 open
parks with no link at all — invisible to the ready view, silently.

**The mark is `natura: "consilium"` on a `quaestio`**, not `species`:
`species` already means "what kind of thing this GENUS is" (nexus,
actio, processus), so a per-item `species` would overload it. The
genus stays one genus — bugs and design questions share a lifecycle
and both can block — the mark only splits the view.

**AD COLLOQUIUM** is a sixth class: open design questions, ordered by
how many open items they block (leverage), each row naming what it
blocks. A design question enters the graph by its mark alone, like an
assignment does — marking is a deliberate act of placement. It sits
in this class whatever its assignment: a design question is a
conversation, not a to-do for one person. Answering one closes it;
the decree is born from it (`natum-de`); the blocked project moves
to AD CONSILIUM on its own with the closure named.

**"parca visa, non fixa: N."** Open parks with no canonical link and
no assignment are now COUNTED at the foot of the unscoped view
(never listed — they are outside the graph by definition). Pinning
a project means placing it: under a parent, blocked by a question,
or assigned. The count is what keeps a pin that never got placed
from vanishing. 68 on the live store as of this commit.

Also: `natura` shows in both item views; the census carries the
sixth count; the board carries the section; a parent's ready-children
section lists its design questions too.

Five plants, five assertions: solitary design questions not loaded ·
mark not read · class never assigned · unplaced count silenced ·
`natura` line silent. Tabularium 670.

## 2026-09-22 — the ready view as a daily report: age, closure dates, a compact form, soft order

Task 01M335E2KY (arcus X, reshaped). Fran asked how hard the "state
of things" report had been to produce. Gathering was one call; the
work was interpretation — and most of that interpretation was reading
evidence the view had but did not print. Now it prints it.

**Age on every row** — `[natus YYYY-MM-DD, tactus YYYY-MM-DD]` in the
tool form only (not the board, not the compact form). "Old park whose
blocker closed months ago" now reads off the row; the judgement stays
with the reader.

**Closure date in the reason.** `impedientia clausa (ultimum: '…',
clausum 2026-08-14)` — the blocker's `mutatum`, which is its closing
event for anything that closed last.

**`parata {breviter}`** — six counts on one line, then three rows per
section, remainder counted, no ages. This is the daily size. The
doctrine now says, INSIDE the 2048 characters the host keeps (measured
at char 713): `ORIENTATIO: parata {breviter} primum, deinde parata
{intra: propositum}`. A test asserts the sentence's offset from the
doctrine's start, so a future edit that pushes it past the cut fails.

**Soft order.** `sequitur` links (a follows b) now sort within a
section: rank = length of the longest chain of OPEN predecessors,
computed to a fixed point, then leverage, then id. Never blocks —
pinned: the follower stays in AD LABOREM and its reason says
`sequitur 'b'`. The order key lives in ONE comparator
(`_parati_ordinem_conferre`); the old inline compound key was three
copies waiting to disagree.

**A latina landmine, again:** I named a parameter `brevis`. That is
`short`. Seven compile errors, none of which said "macro". Renamed.

Seven plants, seven assertions: age silent · closure date silent ·
rank not computed · rank not in the key · compact form not capped ·
compact form with ages · orientation sentence moved past the cut.
Tabularium 689.

## 2026-09-22 — a question never inherits its parent's blockage

Found by the first real use after the arc (bug 01M350V79N): I filed
the map-layer park with four design questions `intra` it and three of
them `impeditur-a` it — and `parata {intra}` listed all four as
IMPEDITA, "parens … impeditur a: <the question itself>". AD COLLOQUIUM
was empty. The text-stack questions had escaped only because they were
never linked `intra` their park, which is the wrong structure and one
the triage will fix — so the bug stood in the way of the right shape.

The inheritance loop (`parens impeditus filium impedit`) had no
exception by genus. The rule now: a QUESTION never inherits. A question
is always talkable; an explicit `impeditur-a` between two questions
still blocks (the loop over the node's OWN blockers is untouched). The
class comment and the doctrine sentence say so. Work (opus, parcum)
inherits as before — a task under a blocked stage is still not ready.

Plant: inheritance applied to questions too → the new assertion fails
with the question in IMPEDITA. Tabularium 689 → 693.

## 2026-09-22 — quaerere {tag} quotes the tag; an invalid query is named

Bugs 01M350VEHA and 01M350VMNF, found together on the first hyphenated
tag I ever filtered by (`regio-tabularii`, the provisional region
marker). `_tab_quaerere` appended the tag RAW to the FTS query. FTS5
bare terms are alphanumerics only, so any tag with a hyphen was a
syntax error — and the kernel swallowed syntax errors as "no rows",
so the answer was "nihil inventum" plus a helpful hint that was wrong.
`messis-2026-07`, the K1 harvest tag on 53 items, has been
unfilterable since July.

Two changes. The tag is now a phrase restricted to the corpus column
(`corpus:"…"`, inner `"` doubled) — tags are folded into that column
joined by spaces, so a phrase is the precise match and a title that
happens to contain the word no longer counts as a tag hit. And when
the kernel returns NIHIL the response says `quaestio FTS invalida:
<sqlite message>` with the advice to quote — never "nihil inventum".

One existing assertion was a lying pin: it searched `nusquam-
invenietur` (a syntax error) and asserted "nihil inventum". It now
asserts the loud refusal, and a VALID absent word asserts "nihil
inventum" beside it.

Plants: tag appended raw → hyphenated tag not found; NIHIL from the
kernel reported as "apparatus fractus" without the cause → the
"invalida" assertion fails.

## 2026-09-22 — genus 'regio': the map's node, a place and not a thing with a life

Task 01M3567242 (mappa I), first stage of park 01M350NBBN after the
three decrees (01M35650Z4 placement by `intra`, 01M35656PV region as a
statusless genus, 01M3565C2A visions as marked wishes).

**Seed v9** adds `regio` to the genus table: title, corpus, tags, no
status machine — like a decree, it never closes. The seeding loop is
per-genus idempotent, so an existing world grows the genus on its next
init without a version bump.

**What needed NO code**: `parata {intra: <regio>}` and `quaerere
{intra: <regio>}`. Nodes enter the ready view through their links, a
region has no status so it is never `apertus` and never gets a class,
and the scope walk is generic. Pinned by tests so the free lunch stays
free.

**Refusals**, both in the pre-checks so every cause arrives at once:
`impeditur-a` with a region on EITHER side ("locus est, non res
vitae"), and `status` on a region (the progressive law lets genera
without a machine write any status silently — right for user-defined
genera, wrong for a house genus whose whole meaning is "never closes").
`_cap_genus_rei` gained a prototype; it was defined after both users.

**The count that would have lied in stage IV**: "parca visa, non fixa"
treated ANY `intra` link as placement in the plan. Placing 62 parks in
regions would have driven it to zero without a single park gaining a
question or a blocker. Now an `intra` whose parent is a region does
not count as fixation — location is not planning.

Plants: seed entry absent → addere refuses the genus; region blockage
allowed → refusal assertion fails; status on a region allowed; region
placement counted as fixation → the counter assertion fails.
