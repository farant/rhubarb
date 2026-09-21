
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
