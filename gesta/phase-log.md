# gesta — phase log

Subsystem born 2026-07-15. Kernel = event-sourcing/entity engine over
scrinium/SQLite; first tenant = tabularium (issue/doc store + MCP).
IMPLEMENTATION SOURCE: project-specs/gesta-k1-spec-v2.md (self-
contained; lineage tabularium-visio → tabularium-gradus →
gesta-k1-interrogatio → spec v1 → 3 explorer reports → v2). Never
implement from a compaction summary — re-enter via spec-v2 + this
tail.

## K1 CHUNK A — GESTA CORE: INTENTIO (2026-07-15)

Scope (spec §XV-A): schema migration, write path (transaction trio +
annales line), genera+res folds w/ bootstrap order, refold +
refold-from-annales, goldens 1-9/13-14.

IMPLEMENTATION DECISIONS FRESH AT THIS CHUNK (not in spec-v2 —
settled at build start):

1. **Eager plicare**: gesta_scribere advances all folds immediately
   after commit. Always-materialized ⇒ write-time validation reads
   current state trivially. HWM machinery stays REAL (refold replays
   batches; consumptores advance exactly-once in the fold
   transaction). Volumes make eager free; the MCP resident inherits
   always-fresh reads.
2. **Validation-before-write, violation = second event**: status
   events checked against genus machina BEFORE insert; illegal →
   primary event STILL written + a violation nota (actor=machina,
   origo="gesta-custodia") appended after it. Duplicate creatio on an
   existing res_id = REFUSED (identity, not validation). Malformed
   datum JSON = REFUSED (syntax is not record-don't-block's domain).
3. **State = ONE JSON object including titulus/status keys** —
   the fold is the TS reducer VERBATIM over one object
   (smaragda.ts:723-774); res columns (titulus/status) are
   PROJECTIONS of reserved keys after folding. genus lives in the
   column only (immutable-by-column; a 'genus' key in mutatio is
   just a state key).
4. **Canonicalization at the door**: event datum parsed
   (json_legere) and re-serialized compact (json_scribere) before
   storage — tessellae.datum and annales lines are byte-canonical;
   refolds byte-stable.
5. **Annales line appended BEFORE COMMIT** (fflush+fclose per
   write): crash window leaves annales a SUPERSET (one extra tail
   line) — detectable by gesta_annales_verificare, healable (the
   line is valid; re-append to db). The opposite order would lose
   durable history silently.
6. **emendatio-generis = whole-definition replace.** TS's
   upsert-by-(ex,ad)-pair (smaragda.ts:1819-1822) is an artifact of
   TS's sub-event granularity (genus_transition_defined); our events
   carry complete definitions — whole-value replace is the
   consistent philosophy (same as mutatio arrays). Golden 6 pins
   whole-replace, TS ref documented.
7. **Genus events' res_id = the genus titulus** (readable, stable,
   practically collision-free vs ULIDs; documented in gesta.h).
8. **remotio = object rebuild without the key** (json lib has no
   pair delete; arena-friendly).
9. sordidae marked at write NOW, drained chunk B; nexus/denexus
   events WRITTEN now, folded chunk B — the unknown-kind no-op law
   makes both safe today.
10. scrinium grows the transaction trio
    (incipere/committere/revolvere) — thin over exsequi
    BEGIN/COMMIT/ROLLBACK, the one ergonomic gap the infra explorer
    named.

Bars for this chunk: goldens 1-9, 13-14 green under house flags;
refold == standing folds; annales verificare catches torn AND extra
tail lines; suite runs via gesta/compile_probationes.sh (tessera
template, vendor sqlite3.o relaxed own-object).

## K1 CHUNK A — RELATIO (2026-07-15, same night as the interview)

NAVIS in one pass: 76/76 asserts first-fix (one bug total — the
BIND-NULL trap, gesta.worklog.md: json_ad_chorda NIHIL-datum chorda
binds SQL NULL through scrinium; _chorda_tuta guards the seam;
found by golden 13b restituere, exactly what the golden existed
for). Compile was CLEAN under the full house flag set first try;
examen verdict ACCIPE with zero diagnostics all tiers.

Shipped: gesta/fontes/gesta.{h,c} (aperire/scribere/plicare/
replicare/annales_verificare/ex_annalibus_restituere + reads);
scrinium transaction trio (incipere/committere/revolvere);
gesta/compile_probationes.sh (tessera template + relaxed vendor
sqlite3.o own-object); probatio_gesta.c = goldens 1-9, 13-14 with
'TS: file:NNN' oracle refs throughout.

All chunk-A INTENTIO decisions HELD in contact with the code — no
reversals. Verified live in the goldens: 1-based seq (fresh
consumer at hwm=0 skips nothing), raw-injected events invisible
until plicare then folded (HWM honest), idempotent batch replay,
refold == standing tables, restituere-from-annales == db WITH the
honest divergence case (a raw-injected row has no annales line —
verificare names "acta plura quam lineae"; the restored world
correctly lacks the raw event), torn-tail detection ("lacera"),
duplicate-creatio refusal at the door + fold-reset semantics for
raw duplicates (TS :726 literal), violation notae for illegal
transitions AND bad genus definitions (record-don't-block live).

Suites: gesta 1/1 (76 asserts), scrinium root regression green,
excubitor tail silent. NEXT: chunk B (nexus fold, FTS+sordidae
drain, census, anchor hashes at write).

## K1 CHUNK B — FOLDS INFRA: INTENTIO (2026-07-15)

Scope (spec §XV-B): nexus fold, FTS + sordidae drain, census,
goldens 10-12. Anchors CLARIFIED OUT of the kernel: "kernel stores,
server resolves" means ancorae ride res datum as ordinary attributes
and the WRITER (MCP layer, chunk C) computes sigillum hashes —
nothing kernel-side to build; sigillum links at chunk C.

Fresh decisions:
1. **FTS corpus column = state.corpus + "clavis: valor" pairs for
   other top-level string attrs + tags joined** (TS 'extra' move,
   smaragda.ts:919-924, ADAPTED: TS excluded arrays; our tags array
   is exactly what the tenant searches by — golden 11 'by tag' rides
   FTS, no separate tag machinery).
2. **No archived-deletion from FTS** (TS :911-914 deletes archived).
   DELIBERATE DIVERGENCE: closed/parked items STAY searchable — the
   tenant's whole point is querying history ("what was parked and
   never pulled"). Ledger entry.
3. **Refold-FTS story**: replicare clears res_fts + sordidae too,
   then repopulates sordidae from all res ids — the next quaerere
   drains and rebuilds the index lazily (libraries.ts rebuild-at-
   init analogue, :481-484, done the dirty-queue way).
4. Delete-then-insert FTS upsert (BM25 honesty, libraries.ts:867)
   via `DELETE FROM res_fts WHERE res_id = ?` — our res_id UNINDEXED
   column makes TS's _meta/fts_rowid table unnecessary (full-scan
   delete fine at tenant volume).
5. quaerere drains sordidae first (lazy, TS search-time pattern
   :1024); empty/whitespace query = empty result (:1117); MATCH
   syntax errors = empty result honestly (quoting = MCP layer's
   business); prefix idiom = caller appends '*'.
6. Consumer order: genera -> res -> nexus.

## K1 CHUNK B — RELATIO (2026-07-15)

NAVIS: 107/107 PRIMO CURSU — zero iterations, no bugs, examen
ACCIPE. Shipped: nexus consumer (third, after genera→res; duplicate
triples IGNORED, denexus selective — golden X live), FTS drain
(delete-then-insert per sordida in one transaction; corpus column =
corpus attr + "clavis: valor" string-attr pairs + tags joined per
INTENTIO B-1), gesta_quaerere (drains first; empty/whitespace =
nihil; genus/status filters; bm25 order; LIMIT 50; MATCH errors =
empty honestly), census generum×statuum + census tagorum,
replicare extended (clears nexus/res_fts/sordidae, repopulates
sordidae from res → next quaerere rebuilds the index lazily —
verified live in golden XVIII).

Goldens X-XII + FTS suite: prose/tag/prefix ('parsur*' finds
'Parsura' — the Latin idiom bar), status-flip reindex through the
dirty queue, notae searchable in their own column, ULID
structure+ordering (never literal values). Divergence held: closed
items STAY indexed (deliberate, ledger §XIII amended by INTENTIO
B-2 — the tenant queries history).

NEXT: chunk C — surfaces (MCP resident via legatus copy-map,
nexus.tsv anchor resolver + similitudo, doctrine w/ scribe
contract, .mcp.json + launcher, nota_frigida cold path, tenant
seed).

## K1 CHUNK C — SURFACES: INTENTIO (2026-07-15)

Scope (spec §XV-C): MCP resident, anchor resolver, doctrine,
registration, cold path, seed. Fresh decisions:

1. **Engine/main split (legatus precedent)**: gesta/fontes/
   tabularium.{h,c} = engine (tractare-per-message, testable —
   probatio drives it with prescripted JSON lines + tmpfile
   capture); gesta/instrumenta/tabularium_principale.c = thin main.
   Engine rides the fontes glob into the suite automatically.
2. **Seed AT INITIALIZE, idempotent**: the server seeds missing
   genera (×5) + the tag-vocabulary nota (res_id
   "vocabularium-tagorum", deterministic) on every initialize —
   "seed = just events" through gesta_scribere; zero manual steps;
   actor=machina origo=seminatio.
3. **Anchor index LAZY**: nexus.tsv (48MB, ~107k sedes) parsed on
   first anchor-bearing request into tabula_dispersa titulus→
   "via:linea" (first-sedes-wins, praebere mirror) + flat tituli
   list for similitudo (hover-vicinitas lesson: flat copy). Config
   carries via_nexus so the PROBATIO points at a small fixture tsv.
4. **Args all strings v1**: tags = comma-separated; ancorae = JSON
   array TEXT (Claude writes JSON trivially); gerere carries both
   simple fields (textus/novus/verbum/alterum/clavis/valor) and a
   raw 'datum' JSON escape hatch for mutatio.
5. **Hash checks v1 = via-anchors only** (file bytes through
   sigillum); symbolum-anchor CONTENT hashing needs corpus
   extraction (legati territory, resident-to-resident impossible) —
   NAMED PARK; symbolum anchors get existence+location+similitudo.
6. **actor defaults to "claude"** on MCP writes (optional arg
   overrides); origo="mcp". Cold path writes actor=fran,
   origo="frigida".
7. Cold path = nota_frigida.c COMPILED (infra-explorer correction
   honored); frigida.sh builds+runs; tabularium.sh = legatus.sh
   template (deps loop duplicated — 6th newest_header copy, AEDILIS
   ledger grows).

## K1 CHUNK C — RELATIO (2026-07-15)

NAVIS. Suites 2/2 (140 asserts: gesta 107 + tabularium 33). TWO
fixture bugs only, both MINE: similitudo test typos were
SUBSTITUTIONS ('secure'→'secare') — the documented subsequence law
(deletion-typos only, same lesson as the legatus goldens) — fixed
to deletion typos, commented at both sites. Engine code compiled
clean under house flags essentially first-try (one stray label +
imaginary Roman-numeral macros in nota_frigida caught pre-compile).

Shipped: gesta/fontes/tabularium.{h,c} (engine, ~1100 lines —
legatus copy-map honored: lineam framing, gating, dispatch ladder,
flexible-args instrumentum builder [legatus's 2-arg builder
extended to arrays], textum_respondere without vigilia [index = db,
fresh by construction]); TABULARII_DOCTRINA carries the SCRIBE
CONTRACT verbatim into agent context; idempotent seed at initialize
(5 genera + vocabularium-tagorum); anchor index lazy-loaded from
config-supplied nexus.tsv (fixture-testable; first-sedes-wins;
similitudo over flat tituli copy); via-anchors hash-check through
sigillum, symbolum-anchor content-hash = NAMED PARK;
tabularium_principale.c + nota_frigida.c (COMPILED cold path);
tabularium.sh launcher (stderr-only, -radix appended) + frigida.sh;
.mcp.json "tabularii" registered; .gitignore db lines; fabrica.tsv
explicit unit rows ×2 binaries (directory rule REJECTED for the
mixed build dir — sibling binaries + test dbs would arm it
permanently; excubitor now silent on the pool).

LIVE: the real store BORN through the actual launcher pipeline —
initialize seeded 5 genera + vocabulary (seq 1-6), census answered
through real MCP framing; frigida.sh wrote event 7 (a nota on
vocabularium-tagorum, actor fran origo frigida) WITH NO RESIDENT —
the cold-path bar met; gesta/annales/tabularium.jsonl = 7 lines,
committed history begins.

Probatio drives the engine through tractare with prescripted JSON
lines + tmpfile capture (the engine/main split paying off):
pre-init refusal (-32002), doctrine landing, double-init refusal,
5 tools listed, addere w/ tags+anchors, res w/ resolved anchor
(first-sedes-wins vs fixture) + CAUTIO + simillima, gerere
status/nota/nexus-by-titulus, quaerere prose/prefix/tag/miss,
res-miss simillima, census, notification silence, ping-pre-init,
-32700 on garbage.

NEXT: chunk D — harvest (open items from the prose ledgers) +
LEDGER BAR + live shakedown w/ registered resident + K1 seal.
NOTE for the seal: /mcp reconnect (or session restart) to pick up
the tabularii registration.

## K1 CHUNK D — MESSIS + SIGILLUM: RELATIO. **K1 SIGILLATUM** (2026-07-15)

MESSIS: 32 res per contractum scribae SPONTE filatae (sine portis -
ipsa messis = barra scribae autonomi): 5 DECRETA (censor-declinata,
nullum-CLI, clausa-in-FTS, mcp_servus-copia, genera-ut-eventus),
17 PARCA (tabula tractuum officinae integra + silva QB-QD +
gesta K2-K5 + saltuarius...), 6 QUAESTIONES (phantasma
specimen_usus, tcp fluctuans, exclusiones-deriva, phase-log-407KB,
nomina-insigillata, fundamentum-1x11), 4 DESIDERATA (sonda,
tokenizator Latinus, MEMORY-migratio, batch-symbolum-debile).
Tag messis-2026-07 in omnibus (origo argumentum in addere deest -
tag sufficit, emendatio minima futura); ancorae via-generis ×3;
nexus unus (sigillum-ancorae impeditur-a LEGATUSD).

**BARRA ACTORUM (spec §XV-D) TENET**: "quid legato debetur?" =
quaerere 'legat*' UNO ictu - omnia quae liber manu-compilatus
2026-07-14 habuit (field-refs/LEGATUSD/filtra/macro-corpora/hover/
batch/sonda/mtimes) PLUS nata postea (sigillum-ancorae cum nexu,
vigilia-tabularii, phantasma). Quaestio dominat scansionem
quattuor documentorum.

**BARRAE SIGILLI in tabulario VERO**: annales == acta
(verificatum); replicatio == tabulae stantes (seq 40). Omne
eventum computatum: 40 = 5 definitio-generis + 33 creatio +
1 nota fran (frigida) + 1 nexus. Census: 33 res, 25 tags,
hwm == seq.

**K1 SIGILLATUM**: quattuor chunks, UNA NOCTE + mane (interrogatio
22 Q&A → spec-v2 per exploratores → A 76/76 → B 107/107 primo
cursu → C 140 asserts + residens vivus + via frigida → D messis +
barrae). Summae: probationes 173 asserts 2/2 suitis; vitia TOTIUS
MILESTONE = 1 verum (BIND-NULL) + 2 fixturae meae (lex
subsequentiae similitudinis); examen ACCIPE ambabus unitatibus;
capture-then-implement QUARTUM tenuit (interrogatio calida →
implementatio eadem nocte, exploratores 3 correctiones ante codicem
- cold path, oracle-attribution, seq-1-basata).

Parca K1 aperta (in tabulario ipso nunc - quaere 'gesta'): K2-K5,
vigilia binarii, piscina scratch, sigillum symboli, tokenizator.
Sequens gradus maior per tabulam: quodlibet Fran trahit.

## K1.1 — DOCTRINA v2 + inventum launcher (2026-07-15)

Doctrina v2 VIVA (mores additi: quaere-ante-filationem, status-
flecte-cum-trahis, orientatio post-compactionem = census+quaerere,
divisio actorum quattuor domorum, via frigida nominata) - navis
ipsa per binarium, sessiones futurae hereditant. INVENTUM excubitoris
(organicum, minuta post editionem): conditio nexus launcher obiecta
recompilata non videbat (main_src + capita sola) - binarium vetus
post editionem fontis, classis ABI-inventi. Sanatio ambobus scriptis
(obj_recentius ambulatio). Excubitor iterum se probavit.

## K1.1 — TABULA.MD PROIECTIO: INTENTIO (2026-07-15)

Pulled by rule-of-two (Fran's projection question + reading question,
one hour apart). Tenant-side renderer in tabularium.c (board FORMAT
= tenant vocabulary; kernel stays generic): _tabulam_scribere after
each successful write + at initialize; path via config (via_tabulae;
NIHIL = off — kernel probatio unaffected; real = gesta/annales/
tabula.md COMMITTED next to the jsonl). CONTENT-DETERMINISTIC header
(seq + last-event creatum, NO wall clock — file changes only when
the store changes, git diffs stay honest). Board = OPEN things:
quaestiones (apertum/laborans), parca (parcatum/tractum), desiderata
(apertum), decreta (all — stateless), nexus links; clausum/relictum/
impletum OMITTED (history = queries, board = now). Bars: file
exists + planted titles present + closed items absent after flip +
regen through real initialize pipe.

## K1.1 — TABULA.MD: RELATIO (2026-07-15)

NAVIS: 147/147 (goldens novi: praesentia apertorum, absentia
clausorum post flip, nexus redditus). Impulsus unum vitium ante
compilationem cepit (campus via_tabulae in configuratione sed non
in structura). VIVA: gesta/annales/tabula.md = 38 ordines aperti,
caput deterministicum (seq+creatum, sine horologio), sectiones
QUAESTIONES/PARCA/DESIDERATA/DECRETA/NEXUS, tags in lineis.
Desideratum quod tabulam petivit -> impletum IN TABULARIO ipso
(circulus perfectus: tabula completionem suam ostendendo desiit
eam monstrare). NB residens conexus (binarium vetus) scripturas
recte facit sed tabulam non regenerat donec /mcp reconnect -
regeneratio interim per fistulam initialize.

## K1.1 — TITULI DUPLICATI SANATI: RELATIO (2026-07-15)

Quaestio prima tabularii VITAM PLENAM vixit: filata (debrief) ->
laborans (tractio) -> nota sanationis -> clausum -> e tabula
evanuit. Sanatio: addere CAUTIO cum titulo exsistente (numeratio
ante-post); _res_solvere ambiguum_out + _ambiguitatem_respondere
(candidati cum res_id/genere/statu, exemplar legati); gerere +
res ambiguitatem recusant nominando; nexus-alterum tolerans
(ligamen ad textum crudum non obstat); nota_frigida LIMIT 2
recusatio; doctrina renovata (tituli duplicati iam NON taciti).
Vitium meum in flumine: buffer LXIV pro cautione ~85 octetorum -
manu captum ante compilationem. Goldens 4; suitae 155/155.

## ADDENDUM ANTE COMPACTIONEM (2026-07-15, mane)

STATE: K1 SEALED + K1.1 (tabula.md, doctrina v2, tituli duplicati)
omnia NAVIS et viva. Suitae 155/155; examen ACCIPE; excubitor
tacitus. Acta vera: seq 49 (semina + messis 32 + ergonomia +
vita quaestionis primae completa).

RE-ENTRY POST COMPACTIONEM:
1. Doctrina in contextum cadit sponte (conexio tabularii) - mores
   ibi; census + quaerere in area laboris = orientatio.
2. gesta/annales/tabula.md = tabula legibilis (etiam tibi, lector).
3. Aedificatio: hic phase-log (INTENTIO/RELATIO omnium chunks) +
   project-specs/gesta-k1-spec-v2.md (fons implementationis).
4. NIHIL in volatu - proximum per tabulam trahitur (K2-K5 parca in
   tabulario; ergonomia tag pro poliendis; nihil praescriptum).

RESIDENS CONEXUS = binarium ANTE tabulam/duplicata - /mcp reconnect
bracchia nova capit. Incommissum: hic phase-log SOLUS (Fran cetera
in itinere commisit - sigillum unum restat).

# ===================================================================
# K2 - RELATIONES PROPRIAE (nexus generibus, salus, opera)
# ===================================================================

## INTENTIO K2 (2026-07-15)

Parcum tractum: 01KXJ2RVM4Y0FWF28MNCKSWX2B. Ritus compressus (per
interrogationem, Q10): interrogatio 10 quaestionum ->
project-specs/gesta-k2-interrogatio.md (crudum) ->
project-specs/gesta-k2-spec.md (FONS IMPLEMENTATIONIS - ex illo
aedifica, non ex summariis). Exploratores omissi: oraculum lectum
directo (Relationships/Health/Tasks + reducer 759-770 + genusReducer
1802 + _validateAttributeType 1790).

DECRETA X (omnia in interrogatione):
1. NEXUS SUBSUMPTUS - machinamentum unum vinculorum; genus seminatum
   species:"nexus" (partes a/b unicus, verbum attributum, machina
   vigens->solutum); eventus veteres nexus/denexus = TUMULI in
   plicatura; vinculum vivum unum manu re-expressum (frustum C).
2. emendatio-generis MANET integra-substitutio; fusio additiva =
   comitas clientis, non semantica actorum.
3. recordare-non-obstare EXTENDITUR ad membra (custodiae notae §VI
   spec - octo probationes; eventus SEMPER cadit).
4. salus SINE genere erratorum - custodiae notae iam flumen
   erratorum nostrum; aestimatio pura status-contra-definitionem.
5. opera = VOCABULARIUM, non API - probatio aurea G12 genus opus
   definit et vitam ambulat per eventus exsistentes solos.
6. typi attributorum: textus/numerus/veritas/TABULATUM (tags iam
   tabulatum vivum; filetree = res palatii, K4-K5).
7. MCP QUIETUM - nulla instrumenta nova; gerere nexus/denexus
   grammatica eadem, saccharum super machinamentum novum; salus
   passive in res/census.
8. nomina: nexus EST verbum (nullum "relatio"); partes,
   cardinalitas (unicus/aliquot/quotlibet), membrum-additum/
   -remotum, salus, opus.
9. semen v2 genera VIVA emendat (attributa: titulus necessarium,
   corpus, tags, ancorae) - salus contra tabularium verum die primo.
10. tabula membra = PLICATURA consumptoris (non scriptio in linea ut
    TS) - architectura K1 exacte-semel; migratio 2 tabulam nexus
    delet.

FRUSTA: A machinamentum (migratio 2, reducer, plicatura membrorum,
tumuli, custodiae, quaesita) / B salus / C tenens (semen v2,
saccharum, tabula, deploy + sigillum). Goldens G1-G15 cum locis
oraculi in spec §IX; divergentiae D1-D11 §X; vectes sigilli §XII.

## RELATIO K2 FRUSTUM A (2026-07-15) - machinamentum membrorum

NAVIS. gesta.{h,c} + probatio_gesta.c; suitae 220/220 (gesta 107
->172, tabularium 48 intactae); examen ACCIPE ter; compilatio
prima-vice munda.

EMENDATIO CONSILII (in aedificatione capta, spec par III emendata):
migratio 2 ADDITIVA facta - tabulam nexus delere in frusto A
tabularium.c frangeret (sectio NEXUS tabulae eam legit usque ad
frustum C); suitae inter frusta virides manent. Divisio: migratio
2 = membra creata (A), migratio 3 = nexus deleta + consumptor
retiratus (C, cum omni codice veteri). Finis idem.

FACTA: migratio 2; reductor membrum-additum/-remotum (paritas TS
759-769 EXACTA: additum sine dedup, remotum omnes tollit clavis
manet); consumptor membra QUARTUS (post res - solutum genus rei
plicatum consulit); tumuli nexus/denexus veterum; exclusio FTS
specierum nexus (paritas TS 538); custodia I-VIII
(_membrum_validare, querela prima redditur, litterae staticae);
auxilia _species_nexus_est/_partem_invenire/_chordae_pares;
quaesita gesta_nexus_rei/gesta_socii_rei (Xar* redduntur - forma
domus, non signaturae spec par VII crudae).

AUREAE G1-G6, G13 (sectiones XIX-XXIV, +65 asserta). Inventum
scribendi auream: eventus violans CADIT in indicem (recordare-non-
obstare!) - fixura sectionis XXII membrum id_b (non id_q) adhibet
ne quaesita reversa XXIV polluat. Lex: in probationibus custodiae
memento etiam eventus malos plicari.

PROXIMUM: frustum B (salus) - validator typorum, salutem_aestimare,
insalubres_enumerare, aureae G7-G12 (opus-gratis inclusa).
