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

## RELATIO K2 FRUSTUM B (2026-07-15) - salus

NAVIS. gesta.{h,c} + probatio_gesta.c; suitae 304/304 (gesta 172
->256, +84; tabularium 48 intactae); examen ACCIPE; aureae G7-G12
PRIMO CURSU virides.

FACTA: _typus_validus (textus/numerus[integer aut fluitans]/
veritas/tabulatum; ignotus TRANSIT - lex progressiva TS 1796);
gesta_salutem_aestimare (querelae quattuor typorum: attributum-
necessarium-absens cum LEGE CHORDAE VACUAE TS 4378 [""=absens, et
typum non iudicat 4391], typus-attributi-pravus [cautio non
erratum], status-ignotus [genera sine machina praetereunt],
cardinalitas-violata [species nexus solum, limen + tectum]);
gesta_insalubres_enumerare (filtrum generis, sanae exclusae,
exemplar collige-primum); genus rei ignotum = sanum (TS iacit,
nos praeterimus). Notae custodiae NON numerantur (D4).

VITIUM UNUM in flumine: parametrum 'nomen' scripsi (chorda nomen)
- macro latina.h typedef! Impulsus examinis id ANTE compilationem
cepit (syntaxis fracta + quattuor monstrator-integer-mixti ex
functione non parsata). Ipsum verbum de quo CLAUDE.md monet.
Numeratio firmationum latinarum ~13 nunc. Sanatio: 'intextum'.

AUREA XXX (opus-gratis) = probatio decreti Q5: genus opus
definitum, vita pendens->susceptum->perfectum ambulata, transitus
adprobationis rectus pendens->perfectum - NULLA linea machinae
nova. Genera-ut-eventus opera vere gratis dant.

PROXIMUM: frustum C (tenens + CUTOVER) - migratio 3, deletio
plicaturae veteris, aureae XIV rescriptae, semen v2, saccharum,
tabula, superficies res/census, deploy + re-expressio + vectes
sigilli.

## RELATIO K2 FRUSTUM C + SIGILLUM K2 (2026-07-15)

**K2 SIGILLATUM.** Suitae 316/316 (gesta 253 + tabularium 63);
examen ACCIPE quinquies; vectes sigilli VI/VI.

CUTOVER: migratio III (tabula nexus deleta, consumptor retiratus);
_nexui_applicare + ramus consumptoris deleti; aurea XIV rescripta
(tumuli ubique - eventus veteres in actis cadunt, nihil tangunt).
Cautela clang -Wstring-concatenation: elementum ultimum tabulae
migrationum concatenationem trans lineas suspicatur - fragmenta
media-enuntiatione secare.

TENENS: semen v2 (genus nexus VI-um seminatum; emendatio
integra-substitutio ex definitione currenti + ATTRIBUTA_V2 pro
quinque generibus - idempotens per clavem 'attributa'); saccharum
gerere nexus = creatio + membrum a + membrum b (grammatica eadem,
responsum sagittale), denexus = vinculum per membra+verbum inventum
-> status solutum; tabula NEXUS ex membris (sagitta bi-partibus,
index crudus aliter); res superficies: socii (sagitta directionalis
ex parte) + salus querelae; census: insalubres numerus.

DEPLOY VIVUS (per fistulam launcheri - residens conexus VETUS,
scripturae MCP vitatae post migrationem; testimonium alterum parci
'Vigilia binarii'): migratio + semen v2 (seq 63->69), re-expressio
vinculi impeditur-a ut res 01KXK5T8A629EME3639TZ6BH51 (69->72).

VECTES: (I) suitae+examen supra; (II) annales==acta TENET;
(III) replicatio==stans, tabulae AEQUALES octetim; (IV) sagitta
in tabula per machinamentum novum; (V) insalubres 0 in copia viva
- omnes res schemata v2 transeunt; (VI) instrumenta+doctrina
intacta (diff vacuum in regionibus).

INVENTUM (vectis II falso fractus primo cursu): copia scrinii WAL
sine plagula -wal = acta trunca dum annales pleni - superset
phantasma. Lex: copia verificationis = db+wal+shm aut checkpoint
ante. In gesta.worklog.md.

K2 parcum CLAUSUM (vita plena secunda). Parca nata K2 in tabulario
(praedefinitum, instrumenta MCP propria, multi-partium redditio,
salus per sordidas). POST COMPACTIONEM: /mcp reconnect residentis
(binarium novum), deinde tractio proxima ex tabula.

## RELATIO K2.1 - VIGILIA BINARII (2026-07-16)

NAVIS eodem die quo consilium (Fran: "hash of itself... write a
warning when the binary it started with is out of date"). Suitae
325/325 (+9); examen ACCIPE (tabularium.c modo -posix).

FORMA: launcher (non binarium ipsum!) sigillum computat - modo
aedificavit, octetos habet; -signum/-binarium tradita. Residens:
serverInfo.version = signum breve (erat "0" fixum); census
'binarium <hex16>'; per tools/call stat unum (vile) -> mtime aut
magnitudo mota -> sigillum disci confirmat -> obsoletum GLUTINOSUM
-> CAUTIO VIGILIAE in omni responso (infundibulum
_textum_respondere - praeteriri legendo non potest). Contentum
idem (tactum/reaedificatum par) = status tacite renovatus.

SUTURA POSIX: stat/sys-stat.h = macOS ex decreto, commentario
notata; examen modo -posix iudicat; st_mtimespec.tv_sec (nomen
verum macOS) pro macrone st_mtime quem lexicon nescit. Magnitudo
+ mtime ambo speculata - granularitas secundorum mtime probationem
deterministicam per magnitudinem sinit.

Scala staleness nunc tota tecta: fontes->obiecta->binarium
(excubitor/fabrica) + binarium->processus (vigilia). Parcum
Vigilia CLAUSUM (vita plena tertia), desideratum Signum versionis
IMPLETUM (primum!). Vivum: b6aba9346d3826aa. Reconnect unum
residentem vigilantem capiet - staleness posthac SE IPSA nuntiat.

## RELATIO K2.2 - LIB/VIGILIA (2026-07-16)

NAVIS: formae duae manu-volutae in bibliothecam unam fusae ANTE
tertiam (Fran: "bundle the two... portable and not hand rolled per
mcp server"). lib/vigilia.{c,h} + probatio_vigilia (unitas
propria, quam neutra forma vetus habuit); tabularium et legatus
AMBO adoptaverunt.

FORMA: vigilia DISCI (binarium sub processu mutatum - sigillum
confirmat) + vigilia FONTIUM (fons manifesti recentior binario) +
signum identitatis; glutinosa cum promotione FONTES->BINARIUM.
INVARIANS MANIFESTI: launcher clausuram scribit quam ipse
compilat - vigil et aedificator dissentire non possunt; BFS
fabricae in legato deleta (~200 lineae). MTEMPORA NANOSECUNDIS
(st_mtimespec): fenestra eiusdem-secundi clausa - probationes
deterministicae sine mora/utime (lex ns in probatione ipsa
probata).

ADOPTIO: tabularium - machina inlinea sublata, sys/stat e
tabularium.c migravit (examen DEFAULT iterum!); superficies
eaedem + vigilia fontium NOVA. legatus - construere/percurrere/
congruit deleti, semantica epochae LSP servata (textus pinnati
"LEGATUS IPSE STALUS" intacti - vox servi, mechanismus
bibliothecae), transcripta viridia sine re-benedictione.

Suitae: vigilia unitas + gesta 328 (75 tab) + legatus 282 -
virides. Examen ACCIPE (vigilia.c modo -posix; probatio legati
REICE PRAE-EXSISTENS [utime, 7 ante -> 6 post]). Vivum: legati
5892cea0806b2da0, tabularii 1782fb64faa0755f; manifesta 139/24
lineae. Reconnect ambo capiet - posthac fontes editi CAUTIONEM
ipsi pariunt ante reaedificationem.

## RELATIO ERGONOMIA (2026-07-16) - census ordinatus + acervus verdictorum

Desiderata duo impleta uno halitu: (1) gesta_census_tagorum nunc
ordine numeri (descendens, tag lexicographice in paritate; Xar
segmentatus -> acies plana + qsort + rescriptio; aurea proprietatis
in XVI - numeri numquam crescunt trans indicem). (2) legati
diagnostica vias PLURES commatibus accipit (idioma tags tabularii):
acervus verdictorum capitibus "== via ==" per plagulam, via prava
= linea erroris non recusatio totius; via una = effectus pristinus
ad octetum (transcripta intacta). Suitae gesta 256 + legatus 288
virides; examen ACCIPE. Inter opera: vigilia bis CAUTIONEM in
responsis residentis tulit (binarium sub eo reaedificatum) -
organum heri natum iam quotidianum.

# ===================================================================
# K3 - ACTIONES + PROCESSUS
# ===================================================================

## INTENTIO K3 (2026-07-16) - interrogatio + ritus plenus incohatus

Parcum tractum + RETITULATUM: "Gesta K3: actiones + processus"
(reducers machinulae in parcum proprium moti - statio propria cum
ritu officina-gradus). Interrogatio XIV quaestionum (4 rotae, omnia
commendata tenuerunt) post excursionem VIVAM per smaragda TS
currentem (MCP claude_ai_smaragda): anatomia actionis
Book/checkout, verruca checkout_v2/v3/v4 (nulla redefinitio in
situ - validatio integrae-substitutionis nostrae!), processus
Publication currens (ordines 3 concurrentes, porta convergentiae),
591 opera perfecta.

DECRETA MAIORA: porta obstat / effectus recordant (phases duae);
executio = eventus actio-facta in flumine rei actionis (nullae
tabulae laterales - TS input/action_taken subsumptae); verba
clausa 1:1 in eventus; substitutio Latina + $novus.N (melioratio
oraculi - referentiae creatorum); species:"actio" opes/argumenta/
effectus (praevisus adprobatus = receptum princeps
claudere-cum-decreto); rollback + actio-recusata; gradus tres v1
(opus/actio/porta; fetch+ramus parcata); provectio avida
anguste-icta (membra K2 = index!); semen v3 = opus + receptum
princeps + RITUS-SIGNACULI (ritus milestonis ipse ut processus -
K4 ritum ut instantiam primam K3 curret); instrumentum unum agere;
recepta sunt codex (definitio numquam per MCP).

PRIMITIVUM NOVUM NUCLEI: fascis atomicus (eventus N, transactio
una, lineae annalium N ante COMMIT, plicaturae semel post).

RITUS PLENUS: spec v1 scripta (gesta-k3-spec.md - SCOPUS
exploratorum, NON fons aedificationis) + interrogatio cruda;
EXPLORATORES TRES in cursu (fundo): E1 oraculum (TS Processes
4655-5431 non lectum), E2 suturae nostrae (fascis atomicus,
re-entrantia provectionis, membra), E3 fossio systematis VIVI
(genera processuum 16, formae usus, quid computruerit). Spec-v2 =
fons implementationis post relationes.

## ADDENDUM K3 - RITUS PERFECTUS (2026-07-16)

Exploratores III relati (verbatim: gesta-k3-exploratio-{1,2,3}.md) +
rota interrogationis secunda (iunctio porta-sola oraculi; recusatio
actio-gradus = obstructum-retenta, se-sanans). SPEC-V2 SCRIPTA =
gesta-k3-spec-v2.md - FONS IMPLEMENTATIONIS (16 decisiones, ledger
divergentiarum XIII, aureae XXIV, frusta A/B/C, vectes VI).

Inventa portantia: 'post' + opes/ligamina processuum IN ORACULO NON
SUNT (D8 - aedificamus consulto, receptum princeps ea postulat);
fascis atomicus = ansa exhauriendi post-commissum (nulla recursio -
scrinium BEGIN nidificatum non fert); decipulae B1/B2 (species-porta
membrorum, fusio-v2 sine fine) in frusta A/C consutae; instantia
definitionem PHOTOGRAPHAT ad ortum (systema vivum statūs absurdos
monstravit); opera 97.6% processu-nata - membra-axis rectus.

POST COMMISSIONEM FRANI: frustum A (fascis + actiones).

## RELATIO K3 FRUSTUM A (2026-07-16) - fascis atomicus + actiones

FRUSTUM A PERFECTUM PRIMO CURSU: 399/399 gesta (143 asserta nova,
zephyrum iterationes) + 75/75 tabularium; examen ACCIPE tribus
plagulis; barra G9 TENET (256 asserta K1/K2 intacta viridia -
_scribere_crudum decompositum, mores byte-idem).

QUOD AEDIFICATUM (fons: gesta-k3-spec-v2.md par III/IV; E2 par 1):
- Decompositio: _eventum_praeparare (syntaxis + res_id; cusio ULID
  ad vocantem EXIIT) / _eventum_validare (lectiones purae + obumbrae)
  / _eventum_inserere (INSERT + annales + sordidae, sine BEGIN).
- gesta_fascis_scribere: eventus N, transactio una, notae custodiae
  INTERPOSITAE (scriptura secunda K1 mortua), creatum UNUM per
  fascem (strftime semel - fenestra inter-enuntiata clausa), lineae
  annalium ordine ante COMMIT. Error mechanicus = fascis totus
  recusatus, nihil scriptum.
- Obumbrae B3: res in fasce creatae validationi visibiles
  (exsistentia + genus; status = status_initialis generis) -
  receptum princeps eas TER exercet sine querela falsa.
- gesta_agere: porta obstat (recusationes VI probatae), substitutio
  $arg (signum solum typum servat!)/$res/$novus (ordine crescens -
  antrorsum cadit naturaliter)/$nunc, verba clausa VII, ids eventuum
  prae-cusi (actio-facta eos nominat), actio-recusata in flumine
  (porta ET mechanica). Errores apparatus (actio ignota, genus non
  actio) = FALSUM sine eventu.
- gesta_actiones_rei: affordantiae per genus + status_necessarius.
- _species_est generalizata (E2 par 4); _membrum_validare et
  validatio statûs obumbras accipiunt.

NOTA CONSILII (frustum C tangit): signatura gesta_agere spec-v2
actorem NON habet - effectus et facta/recusata actor="machina",
origo="actio:<titulus>". Subsumptio tabulae input TS (Q3: "eventus
nostri actorem iam ferunt") initiatorem vult; parametrum actoris
addere cum instrumento MCP frusti C (quod actorem novit) proposito.
Quaestio in tabulario.

INSTRUMENTA: impulsus examen-custos statum transitionalem recte
nominavit (situs vocationis _membrum_validare ante emendationem -
cascata argumentorum discrepantium, sponte sanata proximo Edit);
compile_probationes + examen sine asperitate; corpus/census/res ad
orientationem post-compactionem - nihil grepatum quod non oportuit.

PROXIMUM: frustum B (processus + provectio; CUM emendatione E2-B1
membrum-validare species-porta). Suitae virides inter frusta -
binaria tabularium/nota_frigida stala manent usque ad frustum C
(vigilia residentis id nuntiabit).

## RELATIO K3 FRUSTUM B (2026-07-16) - processus + provectio

FRUSTUM B PERFECTUM PRIMO CURSU ITERUM: 498/498 gesta (99 asserta
nova) + 75/75 tabularium; examen ACCIPE; motor INERS sine instantiis
(asserta frusti A/K1/K2 intacta - barra frusti tenuit). Aureae
G12-G19 omnes.

QUOD AEDIFICATUM (fons: spec-v2 par V; E1 par 3 ambulatio, E2 par 2
ansa):
- Reductor: eventus instantiae (gradus-incohatus/-perfectus/
  -defectus, porta-perfecta, opus-genitum, actio-acta) in statum
  plicantur (claves cursus/opera/actae - paritas oraculi
  processInstanceReducer; superficies passivae frusti C eas gratis
  ostendent). processus-perfectus/-defectus signa mera; eventus
  status ordinarius plicat.
- E2-B1 CONSUTA: _membrum_validare speciem processus agnoscit -
  custodia levis (membrum exsistat), partes non iudicantur; purgatio
  solutum nexus-sola manet (vincula instantiae = historia).
- gesta_processum_incipere: porta (grammatica actionum eadem, D8) ->
  photographia (D9 - gradus/ordines/opes/argumenta in datum) ->
  fascis creatio+ligamina -> provectio prima. Recusatio =
  processus-recusatus in flumine generis (speculum actionum - spec
  tacebat, grammaticam paravimus).
- _provehere_ad_fixum: ansa plana (nulla recursio), iteratio =
  computare ex plicaturis -> fascis micro -> plicatura; ambulatio
  per cursus = algorithmus oraculi (terminalis->proximum, opus
  activum = cursus stat, porta omnes-ex tacita obstructa, actio
  duabus stratis substituta). Terminatio: omnes perfecti ->
  processus-perfectus + status; defectus sine activo -> cascata.
- Excitatio: sonda una idx_membra_membrum in eventibus STATUS solis,
  ex gesta_scribere + gesta_fascis_scribere publico + effectibus
  gesta_agere.

DECIPULAE IN CONSILIO CAPTAE (ante codicem - nulla in fuga):
1. RECURSIO EXCITATIONIS: actio intra exhaustum effectus status
   scribit -> excitatio intra exhaustum -> eadem actio ITERUM (ansa
   infinita). Custos m->provectio_activa: excitatio interior tacet,
   ansa exterior fixum recomputat.
2. ANSA RECUSATIONIS: actio recusata pendens manet -> iteratio
   proxima eam iterum temptaret in aeternum. Memoria 'attemptae'
   per exhaustum unum - retentio vera = excitatio PROXIMA (decisio
   16 servata).
3. FORAMEN G16: sonda membra-basata ligamina opum non videbat
   (opera sola membra erant) - actio recusata numquam sanaretur.
   ADDITUM POSSESSUM: instantiatio ligamina per membra nectit
   (pars "ops:<titulus>") - excitatio se-sanans indicem eundem
   equitat (D7); obumbrae B3 frusti A validationem eorum gratis
   texerunt (instantia in eodem fasce creata).

GestaActioFructus crevit: mechanica (porta-retenta contra
defectum - decisio 16 discriminationem postulat) + facta_id
(prae-cusum - actio-acta eventum facta nominat).

PROXIMUM: frustum C (tenens: semen v3 CUM emendatione E2-B2, agere
instrumentum, linea actiones:, doctrina v3, deploy vivum,
ritus-signaculi cum parco K4, vectes sigilli VI).

### ADDENDUM INSTRUMENTA FRUSTI B (debrief)
Adhibita: census/res/gerere (orientatio + scriba), impulsus
examen-custos (tacuit - nihil captum quia nihil peccatum),
compile_probationes (ansa verificationis sola), examen.sh (vectes).
Legati: NUMQUAM tractum hoc frusto - chirurgia uni-plagulae, plagula
tota in contextu post lectionem primam; valor subsellii = navigatio
trans plagulas et quaesita frigida, non chirurgia profunda (inventum
phasi-dependens, non defectus). Vigilia: CAUTIO in omni responso
residentis post editiones gesta.c - lex adhaesiva K2.1 recte, linea
una, numquam obstitit. Asperitates novae: NULLAE. Desiderata nova:
NULLA (sessio rara).

## RELATIO SIGILLI K3 (2026-07-16) - FRUSTUM C + SIGILLUM VIVUM

K3 SIGILLATUM. Frustum C primo cursu TERTIUM: 498/498 gesta +
114/114 tabularium (aureae G20-G23, XXXIX asserta nova); examen
ACCIPE plagulis V. Vectes sigilli VI OMNES:
  I.   suitae virides + examen ACCIPE (612 asserta trans frusta)
  II.  copia VIVA WAL-plena: annales==acta TENET (seq 123),
       replicatio==stantes (res 63 octetim; instrumentum
       verificatio_sigilli.c - candidatum instrumenti permanentis)
  III. receptum princeps VIVUM: parcum K3 verum clausum, decretum
       filatum ET vinculatum (natum-de), fascis unus atomicus -
       nota et status creatum IDEM ferunt (lex $nunc in productione
       visibilis)
  IV.  ritus-signaculi CURRENS instantia 01KXP19AC2GP2G6AFVR6A8PECA,
       propositum = parcum K4-K5; interrogatio generata, portae
       tacite obstant - TABULARIUM MILESTONEM PROXIMUM SUUM IPSUM
       ORCHESTRAT
  V.   sigillum K3 ipsum per agere factum (vectis tabularii -
       decretum 01KXP18B7Z... manus ipsius tabularii)
  VI.  doctrina v3 in reconnexu; vigilia binarium recens confirmat
       (5f28c827e78ee010, CAUTIO nulla)

FRUSTUM C QUOD AEDIFICATUM: parametrum actoris in gesta_agere/
incipere (quaestio 'quis egit?' CLAUSA - initiator in eventibus,
machina in gradibus motoris); semen v3 (opus + claudere-cum-decreto
+ ritus-signaculi; SEMINA_BOARD_NUMERUS = emendatio E2-B2 consuta;
divergentia: cur_sigilli NECESSARIUM - optionale = defectus
mechanicus certus in gradu ultimo); instrumentum agere (resolutio
ligaminum per titulum, ambiguitas candidatos nominat); linea
actiones: in res; sectiones OPERA + PROCESSUS in tabula (cursus
graduum redditus); doctrina v3 (agere + recepta-sunt-codex + vita
operis; pinna CONTRACTUS SCRIBAE intacta).

DEPLOY VIVUS: semen v3 in reconnexu (seq 106->109); verbum natum-de
+ tag k3 sancita in vocabulario; parca VII ex spec par XII filata
(fetch+ramus, post-saccharum, cancellatio cascadens, piscina sub
agere, columna species, superficies definitionis MCP, susceptum).

MILESTONE UNO DIE: ritus (interrogatio XIV + exploratores III +
spec-v2) heri; frusta tria hodie, QUODQUE primo cursu viridium -
zephyrum insecta in fuga trans totum K3. Decipulae omnes ante
codicem captae (exploratio + consilium). Systema quod aedificavimus
sigillum suum scripsit et ritum successoris sui incohavit.

PROXIMUM: K4 per ritum vivum (opus 'interrogatio' pendens in
tabula = ritus K4 incipit ibi); machinula reducers = parcum
separatum; parca K3 vii in tabula.

## INTENTIO K4 - RAMI (2026-07-16, ritus per processum ipsum ductus)

RITUS PLENUS PERFECTUS INTRA RITUM VIVUM: instantia ritus-signaculi
01KXP19AC2 opus quodque genuit dum perficimus - interrogatio
(susceptum ante rotam I, perfectum post IV) -> motor spec genuit ->
spec perfecta -> exploratores geniti -> relati -> perfecti. Cursus
consilii CLAUSUS; porta-spec transiit; FRUSTUM 'frusta' incohatum
manet donec frusta A/B/C aedificata (tunc porta-sigilli -> sigillare
SPONTE flagship in parcum K4 aget - recusata+retenta si parcum
nondum paratum: se-sanatio ipsa demonstratio est).

INTERROGATIO XIV + CHARTA III (gesta-k4-interrogatio.md):
truncus avidus / rami pigri; replay-on-merge; parametricum ubique;
ramus genus in trunco (sentinella '' manet); custodia in statu rami;
genera/motor/indices trunco-soli; confligentia res-eadem + vis;
annales interpositi (forma parata ab K1); NAVE-DISCARD-PRIMO
(inversio E3); verificare INTENSUM (branch_id in comparationem);
fusio nidificata recusata nisi parens fusus.

EXPLORATORES III (gesta-k4-exploratio-{1,2,3}.md VERBATIM):
E1 - lectio nidificata oraculi RIMOSA (tectum seq deest parentibus
mediis - CORRECTIO NOSTRA D1); fusio non-atomica/attributionem
destruens/processus non provehens (nostra = emendationes D2-D4);
familiae indicum avidae VI + unci fusionis IV = quod recusamus (D7).
E2 - sutura UNA (branch_id='' in plicatore 1196) = puritas trunci
GRATIS; decompositio _rei_applicare -> _statum_transformare purum;
hwm limen non numerus = saltus recti; must-fix III (B1 resolutio
tituli trunco-ligata -> res_id in ramo requiritur; B2 lectiones
membri III una mutandae; B3 fusio numquam per _fascis_scribere -
notae phantasmatis). E3 - RAMI ORACULI DEMO-WARE (2 umquam, 0
abiecti, 0 confligentiae detectae, workspaces vicerunt, git pro
speculatione ipsorum; currentBranch deletus post insectum
concurrentiae git 53b0372 - decisio 2 VINDICATA).

SPEC-V2 = FONS IMPLEMENTATIONIS: project-specs/gesta-k4-spec-v2.md
(decisiones XVII, ledger divergentiarum IX, aureae XVIII, frusta
A/B/C, ratio essendi contra provocationem E3: speculatio
compactionem superans + substratum sync). Parcum K4 TRACTUM.
Parcum K4-K5 fissum: K4 retitulatum in situ (ligamen ritūs tenet),
K5 palatium filatum 01KXP9XNE4.

POST COMMISSIONEM FRANI: frustum A (decompositio + lectio rami).

## RELATIO K4 FRUSTUM A - DECOMPOSITIO + LECTIO RAMORUM (2026-07-16)

AEDIFICATUM (spec-v2 par IX frustum A + una translatio possessa):
- Migratio IV: idx_tessellae_ramus ON tessellae(branch_id, seq).
- DECOMPOSITIO PLICATURAE: _rei_applicare fissum in
  _statum_transformare PURUM (obiectum status intra/extra, corpus
  reductoris VERBATIM motum - K1 verba, K2 membra, K3 cursus/opera/
  actae) + involucrum trunci (lectio -> transformatio -> scriptura,
  octetim idem). Fructus TRI-STATUS (IGNOTUM/INANE/MUTATUM) -
  eventus ignotus numquam scribit, inanis ordinem renovat solum si
  res exsistit; bivalens barram G9 fregisset.
- LECTIO RAMORUM: _catena_rami (tectum PER NEXUM - correctio
  E1-B2/D1; custodia altitudinis XVI contra gyros; stricta in dato
  corrupto) + _res_in_ramo_capere (quaesitum catenae unum, plicatura
  in memoria per transformatorem eundem) + par publicum
  gesta_res_in_ramo_datum/_status (NIHIL/"" = delegatio trunco).
- TRANSLATIO POSSESSA: puritas trunci (AND branch_id='' in
  plicatore avido) ex frusto B huc tracta - fixturae crudae aliter
  a plicaturis avidis vorarentur. E2 par 4 exacte tenuit: hwm limen
  non numerus.
- DECISIO NOVA: parametrum rami nuclei = RES_ID rami (columna
  branch_id res_ids fert; tituli inter solos activos unici -
  resolutio tituli = tenens, frustum C).

PROBATIONES: mundus m6 (VIA_DB6/AN6), sectiones XLIX (G2: catena,
delegatio, puritas, errores) + L (G3: tectum per nexum, nidificatio
honesta, gyrus). 498 -> 562 asserta gesta; 114 tabularium intacta.
Fixturae ramorum CRUDAE per _ramo_insere (via scripturae = frustum
B; annales verificare in m6 non vocatur).

INSECTUM UNUM IN FUGA: decipula _chorda_tuta TERTIO contextu -
nexus trunci chorda "" dato NIHIL -> ligare = SQL NULL ->
branch_id = NULL nihil congruit; segmentum trunci vacuum. 561/562:
asserta negativa omnia trivialiter transibant; assertum POSITIVUM
unum (nota trunci per ramum visa) id cepit. Lex probationum: sectio
tota-negativa nihil de via positiva probat.

INSTRUMENTA: adhibita - legati diagnostica (ACCIPE ante compilationem
utramque), corpus/nexus non necessaria (regiones notae ex spec),
excubitor stala honesta post editionem gesta.h. Asperitas UNA:
impulsus diagnosticorum post editionem gesta.h falso-positiva in
probatio_gesta.c ostendit (index residentis medio-editionis;
"numerus argumentorum discrepat" in vocationibus gesta_agere
intactis) - compilator arbiter, suitae virides; nulla actio.
Desideratum: nullum novum.

PROXIMUM: frustum B (scriptura + vita ramorum) - filatio rami per
viam scripturae, permutationes validationis (copia E2-B2 tres una),
annales branch_id, recusatio generis, creare/enumerare/abicere
(FLAGSHIP)/confligentia/fundere + verrere sondae, verificare
intensum. Aureae G1, G4-G8, G10-G16.

## RELATIO K4 FRUSTUM B - SCRIPTURA ET VITA RAMORUM (2026-07-16)

AEDIFICATUM (spec-v2 par IX frustum B):
- FILATIO RAMI per viam scripturae: _fascis_scribere/_eventum_
  validare/_eventum_inserere/_annalem_appendere parametrum ramus
  ferunt (NIHIL/'' = truncus, sedes vocantium trunci VII intactae);
  branch_id EXPRESSE ligatur (E2-B6); scripturae ramorum sine
  sordidis sine excitatione (indices/motor caeci per
  constructionem); porta rami in fasce (exsistat, generis ramus,
  ACTIVUS); genera non ramificantur (recusatio mechanica, D5).
- PERMUTATIO VALIDATIONIS UNA SEDE: _res_validationis_capere
  (trunco = tabula + obumbrae; ramo = plicatura catenae + obumbrae)
  - lectiones dup-check/machinae/membrorum TRES (E2-B2) per eandem
  eunt, dimidia permutatio structuraliter impossibilis.
- VITA: gesta_in_ramo_scribere; gesta_ramum_creare (titulus inter
  activos unicus quaesito uno E1-B9, punctum ante creationem);
  gesta_ramos_enumerare; gesta_ramum_abicere FLAGSHIP (eventus
  UNUS, lectio archaeologica manet); gesta_confligentia (res eadem
  utrimque post punctum, status ambo plicati); gesta_ramum_fundere
  (replay-on-merge: materializatio -> transactio UNA copiae crudae
  attributione ET creato servatis + status fusus [D2/D3] ->
  plicatura -> verrere sondae super status copiatos [D4]; numquam
  per _fascis_scribere [E2-B3]; nidificata recusata nisi parens
  fusus [dec 17]; recusationes more agere: VERUM + causa).
- verificare INTENSUM: branch_id columna VIII in comparationem
  (dec 16); restitutio iam circumferebat (K1) - intacta.
- DECISIONES POSSESSAE: actor in signaturis vitae (lectio K3 -
  attributio cum ope nascitur, non retrofit); creatum copiarum
  SERVATUM (eventus quando accidit; tempus fusionis per seq +
  origo); sordidae per copiam (FTS trunci res fusas videat).

PROBATIONES: mundus m7 (via vera ubique - annales plenae),
sectiones LI-LVIII: G1 insulatio + G7 recusationes; G4 custodia in
statu RAMI + insulatio fratrum; G5 membra solum-ramalia sine nota
phantasma + G6 creatio contra catenam (prae duplicat, post licet -
furca honesta); G10 abicere; G11 confligentia; G12+G13 fusio
(recusatio/vis/attributio/origo/FTS/verificare/nidificata de imo
sursum E2E); G14 verrere sondae (opus fusum processum provehit);
G8 replicatio proprietas + G15 branch_id medax captus + G16
restitutio octetim. Asserta 562 -> 680; tabularium 114 intacta.
INSECTA IN FUGA: ZEPHYRUM (impulsus diagnosticorum auxilia ficta
duo ante compilationem cepit).

INSTRUMENTA: adhibita - legati diagnostica (ACCIPE bis ante
compilationem; impulsus post-editionem sedes vocantium VII
enumeravit - filatio parametri sine grep), excubitor. Asperitates:
nullae novae. Desideratum: nullum.

PROXIMUM: frustum C (tenens + deploy) - semen v4 genus ramus,
instrumentum MCP ramus {creare|enumerare|comparare|fundere|
abicere}, parametrum ramus in addere/gerere/res + lex E2-B1
(res_id in ramo requiritur), census/tabula RAMI, doctrina v5,
deploy vivus, G17-G18, SIGILLUM PER RITUM CURRENTEM (opus frusta
perfectum -> porta-sigilli -> sigillare sponte).

## RELATIO K4 FRUSTUM C + SIGILLUM K4 (2026-07-17)

AEDIFICATUM (tenens): semen v4 genus ramus (SEMINA_NUMERUS X;
DIVERGENTIA POSSESSA: parens NON necessarium - radix fert "" et
lex chordae vacuae salutis eam absentem numeraret); instrumentum
MCP ramus {creare|enumerare|comparare|fundere|abicere} cum
resolutione per titulum inter ACTIVOS (_ramum_solvere; res_id pro
archaeologia fusorum/abiectorum); parametrum ramus in addere/
gerere/res cum LEGE E2-B1 verbatim ("in ramo res_id requiritur -
resolutio tituli truncalis est"); nexus/denexus in ramo RECUSATI
(saccharum multi-scripturae + resolutio truncalis = fovea; parcum
filatum); census linea "rami activi N"; tabula sectio RAMI (activi
soli - lineae apertae quas sessio sequens videre debet); doctrina
v5 (grammatica ramorum + "truncus = veritas, rami = hypothetica;
ABICERE GRATIS - fingere audacter").

PROBATIONES: G17 (vita completa per MCP: creatio/duplicatus
recusatus/addere-in-ramo/FTS caecum/lex res_id/gerere-in-ramo/
nexus-nondum/comparare-absens/fusio/FTS-post-fusionem/abicere/
scriptura-in-abiectum-per-titulum-inattingibilis) + G18 (census
rami-activi, enumerare tri-status, tabula RAMI, semen v4).
Asserta tabularium 114 -> 144; gesta 680 intacta. Primo cursu
viridia - insecta in fuga frusti C: ZEPHYRUM (structura una
si-catenae in gerere ante compilationem reordinata: custodia ante
declarationes C89).

DEPLOY VIVUS: binarium 153ec1932cdd6f5c (praestructum, reconnexus
Frani); semen v4 in reconnexu (seq 161->165); census "rami activi
0" vivus. PROBATIO FUMI FLAGSHIP: ramus probatio-vivi natus ->
nota "cogitatio hypothetica" in ramo creata (trunco invisibilis,
FTS nihil invenit, comparare "truncus: (absens)") -> ABIECTUS.
Cogitatio nata et abiecta sine vestigio in trunco - casus usus
princeps K4 vivus.

SIGILLUM PER RITUM VIVUM (2026-07-17T00:51:14): opus frusta
perfectum (effectus mutatione scriptum) -> CASCADE MILLISECUNDORUM
SEX in annalibus instantiae: gradus-perfectus frusta (.121) ->
porta-perfecta porta-sigilli (.123) -> actio-acta sigillare (.124:
claudere-cum-decreto in parcum K4 - decretum natum, nexus
natum-de, nota + status clausum, actor MACHINA OMNIA) ->
gradus-perfectus sigillare (.126) -> processus-perfectus (.127).
PARCUM K4 CLAUSUM MANU MACHINAE - primo ictu, sine recusatione
(parcum tractum erat ex initio aedificationis). QUIRK ACCEPTUM:
textus decreti cur_sigilli in instantiatione ligatum fert ("rami +
palatium (aut quod interrogatio K4 decernet)") - nomen parci
ante-fissionem; provenientia honesta, photographia D9 recte
functa.

BARRAE SIGILLI: I. suitae 680+144 virides (III mundi novi m6/m7 +
sectiones MCP XX/XXI). II. verificatio_sigilli in copia
WAL-plena: annales == acta TENET (seq 183 - ordines ramorum vivos
inclusos, comparatio INTENSA branch_id 1:1), replicatio == stantes
TENET (res 77 octetim; plicaturae puritate WHERE ramos recte
praetermittunt). III. deploy vivus + fumus flagship. IV. parca
§X spec-v2 filata VII (confligentia per-res, copia plicaturae,
FTS ramalis, processus speculativi, sessio/actor, SYNC super
branch_id, quaerere-conscium+nexus-in-ramo).

K4 MILESTONE: frusta tria diebus duobus, insectum UNUM in fuga
toto milestone (decipula _chorda_tuta frusti A - tertius
contextus eius). Ledger divergentiarum D1-D9 totus exsecutus:
quattuor insecta oraculi demonstrabiliter NON habita (G3 tectum
per-nexum, actor servatus per fusionem, sonda D4, generis
recusatio D5). Ritus per processum vivum ductus AB INITIO AD
SIGILLUM - interrogatio suscepta intra instantiam, sigillum ab
actione instantiae ipsius scriptum. Machina quam aedificavimus
suum proprium aedificatum sigillavit.

PROXIMUM: K5 palatium (parcum 01KXP9XNE4 parcatum; fontes
docs/{memory-palace,palace-2,palace-v2-rendering,npc-spec}.md in
../bun-libraries) - ritus novus incipiet ibi; aut quod Fran
trahat.

## K4.1 ERGONOMIA: praefixum ULID (2026-07-17)

Small pull from the instruments debrief: resolving a res by
remembered ULID prefix ("01KXQ82Z9Z...") gave 'res ignota' - the
post-compact recall pattern is exactly a prefix. _res_solvere gains
a LAST-RESORT step (order preserved: exact id -> exact title ->
unambiguous ULID prefix; existing semantics untouched): 6..25
Crockford chars, LIKE unique resolves, plural = 'praefixum
ambiguum' with candidates (the K1.1 title-ambiguity responder grew
a prefix mode). In ramo the E2-B1 full-id law stands - prefix is a
trunk courtesy. Alphabet has no LIKE metacharacters, so the bind
is safe by construction. Tool descriptions + doctrine line +
miss-text updated. One structural edit slip (anchored an Edit
below the return-type line - the push cascade caught it in
seconds). 680 + 153 (144+9) green; examen -posix ACCIPE.
Residents need relink+reconnect to serve the feature.

## K2.3 POLITICA TACENDI (2026-07-17, Fran's design)

The vigilia noise problem solved by ACKNOWLEDGMENT, not policy:
a tacere tool (both residents) suppresses the CAUTIO for N
responses - the call itself is the receipt, so silence only ever
covers what was demonstrably read. Four re-arms, each covering a
distinct failure mode: numerus exhaustus (absolute bound), quies
>= 300 s of requests (compaction/walk-away - the reader may be
someone else now), commissio git (semantic boundary; post-commit
hook touches .vigilia_commissum, residents stat it - hook writes,
resident reads, same doctrine as manifests), causa/status NOVUS
(old acknowledgment never covers new news). Policy lives in
lib/vigilia (vigilia_tacere + vigilia_cautio_dicenda - the ONE
emission face; injected clock = deterministic tests, no sleeps);
both residents switched their funnels; legatus budget is COMMON
across MCP + LSP surfaces. Option D shipped alongside: the LSP
pseudo-diagnostic now labels itself "VIGILIA LEGATI (status
residentis, non plagulae)" - truth in costume. Default when nobody
calls tacere = today's sticky behavior: forgetting is noise, never
danger. Suites: vigilia +14, gesta 680+160, officina 12/12 (three
outdated expectations updated: old message text x2, tool count
VI->VII). examen ACCIPE all three files. Residents need
relink+reconnect.

## K4.2 ERGONOMIA INTENTIO: similia + breviter + origo (2026-07-18)

Pull from park 01KXTY8SCR (items 1-2) + desideratum 01KXJ3F6TH
(origo). All tenant-surface (tabularium.c): no kernel change, no
migration, no fold change.

1. SIMILIA-ON-ADDERE: after a successful trunk creatio, run FTS
   with terms built from the new titulus (alnum word runs >= III
   chars, max VIII, each prefixed-'*', joined OR - the Latin
   prefix idiom; bm25 ranks, IDF downweights common words) and
   append up to III neighbors "res_id genus/status titulus",
   excluding the new res itself (lazy drain indexes it before the
   read). Zero neighbors = nothing appended. FTS syntax error =
   empty result (documented honesty) = nothing appended. Branch
   addere unchanged (trunk FTS is blind to rami by construction).
   Complements the existing duplicate-title CAUTIO: that guards
   exact collision, similia guards the near-miss.

2. RES BREVITER: 'breviter' param on res ("verum"/"true"/"ita",
   the vis idiom). Compact render = header (titulus/genus/status/
   res_id/creatum/mutatum) + corpus as clean text + tags + notae
   ultimae III (from the state notae array the reducer already
   folds - newest first, count shown "III ex N") + actiones
   (affordances). OMITTED: raw datum JSON, ancorae (file IO +
   hashing), socii, salus, annales. In-ramo reading ignores
   breviter (already macra). Header + actiones factored into
   shared helpers so full and brief paths cannot drift.

3. ORIGO ARG: optional 'origo' on addere + gerere, default "mcp"
   unchanged. Threads all gerere paths: main event, in-ramo,
   nexus sugar (creation + both membra via _membrum_scribere,
   which gains an origo param), denexus solutum. Retires the
   messis-tag-as-provenance workaround for future backfills.

Schemas VII->VIII (addere), XI->XII (gerere), II->III (res);
doctrine lines updated. Tests: origo via annales JSONL strstr
(cheapest full-path check) + default; similia positive (shared
title words) + negative (alien title); breviter presence/absence
asserts. Seal: suites green + examen -posix ACCIPE + relink note.

## K4.2 ERGONOMIA RELATIO: similia + breviter + origo (2026-07-18)

All three shipped as speced, one INTENTIO deviation of note: none.
Suites 2/2 (gesta 680 intact, tabularium +~20 asserts, section
XXII); examen -posix ACCIPE zero diagnostics; resident + cold
path relinked.

FIND OF THE DAY (not in the intentio): frigida.sh's link list was
missing vigilia.o - a LATENT break since K2.2 gave tabularium.o
the vigilia dependency. The cached nota_frigida binary hid it;
today's tabularium.c edit forced the relink and the cold path
failed on undefined _vigilia_* symbols. One-word fix (vigilia in
the object loop). Lesson rhymes with the K1.1 launcher-relink bug:
mtime-gated builds hide dead link lists until the trigger file
changes - excubitor HAD been flagging nota_frigida STALUM for
days; a standing STALUM on a binary nobody spawns is exactly
where this class hides.

Mechanics that held: notae live in the folded state (reducer
appends {textus, actor, creatum}) so breviter needed zero new
queries; similia rides gesta_quaerere's lazy drain (new res
indexed before the read, self-excluded by res_id); origo was
pure plumbing (kernel field existed since K1 - the tenant just
never exposed it).

Residents: build/tabularium relinked - Fran reconnects via /mcp
to serve origo/similia/breviter; the running resident announces
its own staleness meanwhile (K2.1 working as designed).

Store: desideratum origo -> impletum; park 01KXTY8SCR closed via
claudere-cum-decreto (the flagship action closing its own
ergonomics park); items 3-5 re-parcata as residua park.

## ANNOTATIONES CHUNK D — genus ancorae "nid" (2026-07-21)

The board learns the code's identity layer (park 01KY3D7EJP;
arc narrative in silva/phase-log.md): ancora genus "nid" resolved
lazily against build/identitates.tsv (nexus.tsv-twin index),
"citata ex codice" reverse links in res output (build/
citationes.tsv), citation health in census. INTENTIO/RELATIO in
the silva log.

## K4.5 FRUSTUM 1 - FLUMEN ACTORUM (acta): INTENTIO (2026-07-22)

Parcum 01KY57P2WD tractum. Instrumentum lectionis "acta" = cauda
fluminis eventuum globalis (superficies recensionis contractus
scribae: "quid hodie scriptum est" una quaestione). Stratum gesta:
gesta_acta_recentia(mundus, piscina, quantum, genus_filtrum?,
actor_filtrum?) -> Xar de GestaActum {seq, creatum, actor, origo,
genus_eventus, res_id, res_titulus, summarium} - SELECT tessellae
seq DESC LIMIT, trunco solo (branch_id vacuum; param ramus =
futurum nominatum), summarium per genus eventus ex dato JSON
(nota=textus initium, status=novus, creatio=titulus, mutatio=
clavis, actio-facta=actio), titulus per plicaturam res. Stratum
tabularium: instrumentum "acta" {quantum? XXV, genus?, actor?} ->
lineae legibiles recentissima primum. Doctrina += acta. Probatio
in suite gestarum. Barrae: suite gestarum tota + probatio nova +
cursus vivus contra scrinium verum.

## K4.5 FRUSTUM 1 - FLUMEN ACTORUM (acta): RELATIO (2026-07-22)

Navatum: instrumentum "acta" {quantum? XXV tectum CC, genus?,
actor?} - cauda fluminis eventuum trans res omnes, recentissima
primum, forma "seq tempus actor genus titulus - summarium".
Sedes: stratum tabularii solum (SQL localis per gesta_scrinium -
praecedens census/rami; extractio ad gesta si consumptor alter).
SQL una, filtra per idioma ligaminis duplicis (? = '' OR ...).
Summarium per ordinem clavium (corpus ANTE titulum - echo tituli
in creationibus vitatum + saltus aequalitatis). Truncus solus;
param ramus futurum nominatum. Portae: suite gestarum 680 + 224
(V asserta nova acta), fumus vivus contra scrinium verum (bis:
sine filtris + actor=claude; ansa initializationis in fumo
detecta - "nondum initiatum" sine ea). Doctrina += acta (sessiones
futurae eam hereditent). Frustum 1 ex V parci 01KY57P2WD.

## FORUM F0 - TABULARIUMD: INTENTIO (2026-07-22)

Fundamentum fori (spec project-specs/forum-spec-v2.md; quaestio
01KY5AA1YH): daemon TCP super machinam exsistentem, mundus
scriptorum novus, genera fori seminata. Decisiones Franis: limes
pipati = CODICILLI (non bytes), portus 8753 (conditio Romae a.C.,
praefixo 8), proiectio entities/ NON in v1.

Frusta:
1. tabularium.c: (a) initialize IDEMPOTENS - porta unica-vice fit
   "si nondum, aperi" et responsum idem semper redditur (mutatio
   machinae UNICA spec-v2); (b) semina v5 = articulus (conditum->
   publicatum->retractum) + commentarium + pipatum, attributa
   INLINE (fusio ATTRIBUTA_V2 ad SEMINA_BOARD_NUMERUS terminatur),
   SEMINA_NUMERUS X->XIII; (c) custos pipati: _codicillos_numerare
   (bytes non-continuationis UTF-8) > CCXL -> recusatio nominata;
   (d) signatura arg in addere (post ancoras) et gerere (POST
   ramos actuum - mutatio cruda datum substituit, ergo ante
   serialisationem solam), clavis dati liberi - mutatio schematis
   nulla; ARG_ADDERE VIII->IX, ARG_GERERE XII->XIII. Nexus/denexus
   sine signatura (acceptum: responsum eam in creatione
   commentarii fert).
2. instrumenta/tabulariumd_principale.c NOVUM: argumenta -radix/
   -portus/-scrinium/-annales + trias vigiliae; ordinarii = mundus
   fori, proiectiones NIHIL. Se-initiatio per tractare (linea
   synthetica -> tmpfile -> "result" verificatum). Servus TCP
   127.0.0.1 OBSTRUENS (opt.non_blocans=FALSUM - accepti eum
   hereditant, tcp_posix:715), SO_NOSIGPIPE iam in acceptis.
   Ligatio fallita = alter vivit -> exitus 0 quietus. "portus N"
   in stdout (semen fumi; portus 0 = auto). Ansa: piscina
   connexionis effimera + accipere + fdopen(dup(fd)) r/w + ansa
   per-lineam (pn recens per nuntium, exemplar currere) ad EOF ->
   claudere omnia -> proxima.
3. gesta/tabulariumd.sh: exemplar tabularium.sh (clausura -MM,
   trias vigiliae, manifestum proprium), vexillum -struere (aedifica
   et exi - semen fumi), snippet tabulariumd_fontes_generata.sh per
   tools/fontes_generare.sh.
4. Probationes: probatio_tabularium.c += asserta in-processu
   (initialize bis = idempotens; articulus creatio status conditum
   + machina; custos CCXL tri-casus incl. multibyte 240 chars/480
   bytes ACCEPTUM; signatura in addere et in mutatione).
   probatio_tabulariumd.c NOVA = fumus socketi: system() daemon in
   mundo effimero portu 0, plagula portus lecta (anquisitio), cliens
   tcp_connectere + fdopen + lineae, initialize + addere pipatum
   signatum "Fable 5" + recusatio >CCXL trans filum, kill +
   purgatio. gesta/fontes_generare.sh re-cursum (tcp in clausuram
   suitae).
Barrae: suite gestarum tota viridis + fumus socketi viridis.

## FORUM F0 - TABULARIUMD: RELATIO (2026-07-22)

Navatum totum ex intentione, portae virides:
- Machina: initialize IDEMPOTENS (porta unica-vice -> "si nondum";
  probatio vetus "iam initiatum" inversa); semina v5 = articulus/
  commentarium/pipatum attributis inline, SEMINA_NUMERUS XIII;
  custos CCXL per _codicillos_numerare (bytes non-continuationis -
  codicilli, decisio Franis); signatura in addere et gerere (POST
  ramos actuum - mutatio cruda datum substituit), ARG IX/XIII.
- Daemon: instrumenta/tabulariumd_principale.c - accipere obstruens
  sequentiale, fdopen(dup) FILE* bini, ansa per-lineam exsistens,
  se-initiatio per tractare (tmpfile "result" verificatum), portus
  8753 ordinarius / 0 = auto ("portus N" stdout = semen fumi),
  ligatio fallita = exitus 0 quietus. Launcher tabulariumd.sh
  (exemplar tabularium.sh + -struere) + snippet aedilis proprius.
- FOVEA CAPTA: tcp_servus_creare = bind SOLUM, auscultare vocatio
  separata - sine ea accipere in errorem cadit et ansa nete
  occupata spinnat (vide worklog). 
- Probationes: probatio_tabularium 237/237 (XIII nova: machina
  articuli, custos CCXL tri-casus incl. 240 multibyte = 480 bytes
  ACCEPTUM, signatura in creatione + mutatione via annales, filum
  respondet-ad, idempotentia); probatio_tabulariumd NOVA 12/12
  (fumus socketi: daemon verus mundo effimero portu auto, cliens
  tcp domus per-petitionem, initialize bis connexionibus separatis,
  pipatum signatum "Fable 5", recusatio CCXLI trans filum, acta).
  Suite gestarum 3/3. Mundus fori NATUS: forum.db (ignoratum) +
  gesta/annales/forum.jsonl (XIX eventa seminationis, COMMISSUM).
- Instrumenta (debrief): fontes_generare (aedilis) clausuram
  daemonis/suitae uno ictu derivavit (tcp_posix sponte); examen
  numeralia phantasmata (CCCLXXXIV) ante compilationem cepit;
  excubitor residentem stalum post editum nominavit (renexus
  factus). Asperitas: bifidum creare/auscultare tcp API sine
  custode - servus ligans qui accipere non potest obiectum validum
  est; nc stdin-EOF responsa vorat (worklog).
Proximum: F1 - proxy fori (~40 lineae) + introitus .mcp.json;
porta = pipatum primum per MCP a "Fable 5" subscriptum.

## FORUM F1 - PROXY FORI: INTENTIO (2026-07-22)

Introitus MCP secundus "fori" = proxy stdio<->TCP tenuis (spec-v2
par IV). instrumenta/fori_principale.c: ansa lineae stdin ->
classificatio per tabellarius_nuntium_legere (PETITIO/PRAVUM =
responsum exspectatur; NUNTIATIO/RESPONSUM = mitte-et-obliviscere -
fovea suspensionis notificationum SOLUTA analysi, non divinatione)
-> connexio TCP per-petitionem (contractus v1) -> linea missa ->
si exspectatur: frusta recepta VERBATIM in stdout fluunt usque ad
novam lineam -> claudere. Start-if-absent HIC habitat: recusatio
connexionis -> ./gesta/tabulariumd.sh genitum semel (vexillum
staticum) + anquisitio 25x0.2s; defectus finalis + PETITIO ->
tabellarius_errorem (nuntius curae: "tabulariumd absens").
Argumenta: -portus (8753 ordinarius), -mcp acceptum-omissum.
Launcher gesta/fori.sh (exemplar tabulariumd.sh sine vigilia -
proxy machinam non fert; cd radix ante exec) + snippet aedilis.
Probatio_fori.c: daemon effimero portu auto + proxy per system()
canali (printf | fori -portus N > exitus) - asserta: initialize
respondet, NUNTIATIO non suspendit (nuntiatio + petitio una serie
-> responsum unum), pipatum trans proxy, daemon-absens error
curae. .mcp.json introitus fori (NON commissum - plagula Franis).
Portae: suite gestarum viridis + fumus proxy + PORTA VIVA = pipatum
primum per MCP subscriptum "Fable 5" (post reconexionem Franis).

## FORUM F1 - PROXY FORI: RELATIO (2026-07-22)

Navatum ex intentione: instrumenta/fori_principale.c (~200 lineae)
+ launcher gesta/fori.sh (-struere, sine vigilia) + snippet
aedilis + introitus .mcp.json "fori" (plagula Franis, non
commissa). Ansa: linea stdin -> tabellarius_nuntium_legere
(classificatio vera - fovea suspensionis nuntiationum soluta
analysi) -> connexio TCP per-petitionem -> frusta responsi
VERBATIM in stdout usque ad novam lineam -> claudere.
Start-if-absent in proxy: generatio launcher semel (vexillum
staticum) portu configurato translato, anquisitio 25x0.2s;
-sine-generatione pro probationibus (nullus daemon lateralis);
defectus -> error curae portu VERO nominato (emendatio post fumum:
nuntius primo 8753 duro codice mentiebatur).
Portae: probatio_fori 12/12 NOVA (canalis verus printf|fori contra
daemonem verum: petitio+nuntiatio+petitio = responsa DUO exacte,
pipatum signatum trans proxy in annales, daemon necatus = error
curae portu nominato), suite gestarum 4/4, fumus manualis (proxy
vivo, notificatio media, daemon absens). Instrumenta: fontes_
generare clausuram proxy 10 fontium derivavit; examen tacuit.
PORTA VIVA restat: reconexio Franis (/mcp fori) -> pipatum primum
per MCP subscriptum "Fable 5".

## FORUM F2 - SCELETUM APP: INTENTIO (2026-07-22)

Prima app in apps/ (conventiones nascuntur) + instrumentum lectionis
structuratae in machina (inventum recognitionis: exitus instrumentorum
exsistentium = textus humanus, app datum structuratum eget).

Frusta:
1. Machina: instrumentum "legere" {genus?, status?, quantum? (L,
   tectum CC)} -> textus = TABULATUM JSON (recentissima primum per
   creatum DESC): {res_id, genus, titulus, status, actor (subquaestio
   eventus creationis), creatum, mutatum, datum (obiectum plicatum
   INSERTUM - corpus/signatura/tags gratis), respondet_ad (subquaestio
   membra a->b, verbum per LIKE in dato nexus - cruditas v1
   commentata)}. SQL localis tractatoris (praecedens acta/census).
   ARG_LEGERE III, registratio, ramus dispatch. Asserta in-processu.
   Ambo mundi instrumentum vident (ut acta).
2. apps/forum/forum.c: exemplar tabellae (ansa app-possessa, capsula,
   internuntius) + cliens daemonis per-petitionem (tcp + mittere
   omnia + recipere ad novam lineam - exemplar probatio_tabulariumd;
   initialize NON necessarium - daemon se praeinitiat) +
   start-if-absent (ut fori) + tractatores IS "pipata_legere"
   (daemon legere genus=pipatum -> tabulatum ad IS) et
   "pipatum_mittere" {corpus} (titulus = praefixum corporis IS-latere;
   actor fran, sine signatura - decisio colloquii) + vexillum -fumus
   (pipata leguntur ANTE fenestram, numerus impressus, exitus 0 =
   porta scriptabilis sine oculis).
3. apps/forum/assets/index.html: tabula pipatorum recentissima primum,
   responsa indentata (respondet_ad), arca compositionis cum numeratore
   CCXL, button renovare (pulsus vivus = F4). aesthetica tabellae.
4. assets/forum.toml -> capsula_forum.{h,c} COMMISSA (mos tabellae);
   aedilis.stml inclusa += apps/forum, apps/forum/assets;
   apps/forum/forum.sh = involucrum tenue (aedilis + struere + exec).
Portae: suite gestarum (asserta legere) + constructio aedilis viridis
+ bin fumi (-fumus contra daemonem verum) + Fran fenestram aperit et
tabulam videt (acceptatio vera F2).

## FORUM F2 - SCELETUM APP: RELATIO (2026-07-22)

Navatum ex intentione, portae virides:
- Machina: instrumentum "legere" {genus?, status?, quantum? L/CC} ->
  textus = tabulatum JSON recentissima primum (datum plicatum
  insertum obiectum, actor ex subquaestione creationis, respondet_ad
  ex membris LIKE-verbo - cruditas v1 commentata). ARG III,
  registratio, dispatch. Asserta 6 nova (243 totum) - filtrum
  generis, signatura in dato, respondet_ad resolutum ad articulum.
- apps/forum/ NATUM (conventiones primae): forum.c (exemplar
  tabellae + cliens daemonis per-petitionem + start-if-absent +
  tractatores pipata_legere/pipatum_mittere + titulus = praefixum
  XL codicillorum + vexillum -fumus), assets/index.html (tabula
  filata cum responsis indentatis, arca compositionis numeratore
  CCXL codicillorum IS-latere, renovare manuale - pulsus vivus F4),
  forum.toml -> capsula_forum.{h,c} COMMISSA, aedilis.stml inclusa
  += apps/forum + assets, forum.sh involucrum (aedilis + struere +
  cp bin/forum + exec).
- Constructio aedilis PRIMO ICTU viridis (19 obiecta, clausura tota
  derivata incl. tcp_posix + vitrea_macos.m + capsula_forum.c +
  compagines Cocoa/WebKit - nihil manu scriptum).
- Fumus: daemon stalus captus ("instrumentum ignotum" - legere
  praecedebat), pkill + start-if-absent launcher reaedificavit ->
  "[forum] fumus: 1 pipata lecta" (pipatum primum trans catenam
  totam). Suite gestarum 4/4.
- RESTAT ACCEPTATIO VERA: Fran fenestram aperit (./apps/forum/
  forum.sh), tabulam videt, pipat ex arca.
Proximum F3: articuli + fila (visus articulorum, editor, commentaria
respondet-ad, tumuli).

## FORUM F3 - ARTICULI + FILA: INTENTIO (2026-07-22)

Phasis app pura - mutationes machinae NULLAE (semantica probata:
mutatio = mersio superficialis clavium gesta.c:894-910 -> servare
uno eventu, signatura/tags supersunt; remotio = deletio clavis
:912-925 -> tumulus = corpus absens; machina articuli seminata F0;
legere genera omnia servit).

Frusta (apps/forum sola):
1. forum.c tractatores generales: res_legere {genus} (pipata_legere
   generalisatum), mittere {genus, corpus, ad?} (addere titulo
   XL-praefixo + nexus respondet-ad si ad - pipata responsa ET
   commentaria uno tractatore; custos CCXL daemonis pipata tegit),
   articulum_servare {res_id?, titulus, corpus} (absens = addere
   conditum; praesens = mutatio datum-mersione), status_ponere
   {res_id, novus}, mutare {res_id, corpus} (editio commentarii),
   delere {res_id} (remotio corporis = tumulus). Fumus + articuli.
2. index.html: tres visus (tabula | articuli | articulus-unus cum
   editore). Nav vertens. Articuli: index cum sigillis statuum
   (conditum/publicatum/retractum - omnes in app, Fran auctor est),
   click -> visus plenus: titulus, paragraphi (linea vacua = <p>),
   butones legales machinae (conditum->publicare, publicatum->
   retractare), editor (titulus + textarea, servare), filum
   commentariorum profunditate libera cum arcis responsi in linea,
   editio/deletio commentarii, tumuli ([remotum] si filii, celatum
   si sine filiis), arca commentarii summi. Pipata: responsa in
   linea (genus pipatum, numerator CCXL). Anquisitio per visum
   activum cum differentia.
Portae: fumus (-fumus pipata + articuli numerat) + constructio
aedilis + oculi Franis (articulus scriptus, publicatus, commentarium
meum per MCP in fenestra vivente, responsum ex app).

## FORUM F3 - ARTICULI + FILA: RELATIO (2026-07-22)

Navatum ex intentione (phasis app pura - machina intacta):
- forum.c: tractatores sex generales (res_legere/mittere/articulum_
  servare/status_ponere/mutare/delere) super adiutorem unum
  _instrumentum_vocare (petitio json-aedificata, involucrum MCP
  extractum); mittere = addere + nexus respondet-ad uno tractatore
  (pipata responsa ET commentaria); servare = addere conditum aut
  mutatio datum-mersione; tumulus = remotio corporis. Fumus + numerat
  articulos.
- index.html: visus quattuor (pipata | articuli | articulus | editor),
  nav vertens, index articulorum sigillis statuum, visus articuli
  (paragraphi linea-vacua, butones machinae legales soli: conditum->
  publicare, publicatum->retractare, recensere), filum commentariorum
  profunditate libera (arcae responsi in linea, editio, deletio,
  tumuli "[remotum]" si filii / celati si sine), responsa pipatorum
  in linea numeratore CCXL, anquisitio per visum activum cum
  differentia + custodia focus (arca inter tabulam aperta = redditio
  omissa ne perdatur).
- Portae: constructio aedilis viridis, fumus "5 pipata, 0 articuli",
  syntaxis IS per JavaScriptCore (osascript new Function) VALIDA.
- RESTAT ACCEPTATIO OCULIS: Fran articulum scribit/publicat,
  commentarium meum per MCP, responsum ex app - fere BARRA VIVA F4.
Proximum F4: speculum indutum + fumus se-nuntians + barra vivens
(pulsus C-lateris probabiliter non necessarius - anquisitio IS
sufficit scala personali).

## FORUM F4 - ERGONOMIA MCP + SPECULUM + FUMUS: INTENTIO (2026-07-22)

Ex usu vero diei (tria pipata Franis non visa - caecitas probata):
1. Machina (tabularium.c): (a) arg "ad" in addere - nexus
   respondet-ad sponte post creationem (responsa uno vocamine, non
   duobus; in ramo nondum); (b) titulus optionalis si corpus datur
   (praefixum XL codicillorum ad lineam novam derivatum - finis
   tituli-fabricandi); (c) acta {ab_lecto:"verum"} = eventa non
   lecta VETUSTISSIMA PRIMUM (ordo lectionis) + cursor
   "claude-lectum" in tabula consumptorum exsistente (INSERT OR
   REPLACE; provectus ad maximum redditum; "et N plura restant");
   (d) SALUTATIO in initialize: cursor absens -> conditus ad hwm
   (silentium primi conventus); praesens + retardatio > 0 ->
   instructiones += "NOVA: N eventa..." - nuntiatio mitis, non
   pulsus: salutatio. Ambo mundi gratis (tabula quoque). Asserta
   nova in probatione.
2. Speculum indutum (forum.c): annotatio aedilis obiecti +
   externus capsula_speculi_forum + speculum_creare + tangere in
   effusione eventuum (exemplar hospes); forum.sh: aedilis ->
   speculum_generare -> struere.
3. Fumus se-nuntians (-fumus-plenus): fenestra vera, IS in onere
   pipatum mittit, C responsum per clientem daemonis addit (via
   MCP simulata) filo nexum, anquisitio IS filium videt ->
   fumus_perfectus per pontem -> C imprimit et exit 0; tempus
   excedens = exitus 1.
Portae: suite gestarum + fumus plenus + salutatio viva post
reconexionem. Deinde quaestio clauditur - V1 fori integra.

## FORUM F4: RELATIO + V1 SIGILLUM (2026-07-22)

Navatum totum ex intentione, primo cursu:
- Machina: acta {ab_lecto} (cursor claude-lectum in tabula
  consumptorum exsistente - nihil novi in nucleo; ASC = ordo
  lectionis, provectio ad maximum redditum, "et N plura restant"),
  salutatio NOVA in initialize (absens->conditus ad hwm tacite),
  addere {ad} = nexus sponte, titulus ex corpore derivatus.
  Asserta 251 (8 nova), suite 4/4.
- App: speculum indutum (53 plagulae, Cmd+Shift+D), fumus plenus
  se-nuntians FUMUS PLENUS primo cursu (IS pipat -> pons -> daemon
  -> annales -> C respondet per {ad} -> anquisitio IS filium videt
  -> pons -> exitus 0: gyrus totus sine oculis).
- Probationes vivae per MCP: responsum uno vocamine cum titulo
  derivato per schema vetus sessionis (parametra extra transeunt).
- CAECITAS PROBATA TER: quinque res Franis non visae inter
  lectiones explicitas hodie - cursor eas necessarias probavit
  ante navigationem.

BARRA V1 TOTA TRANSITA (spec v1 par V): fumus se-nuntians +
barra viva + suites virides + launcheri staleness-tuti + recusationes
clarae nominatae. FORUM V1 INTEGRUM: dies unus a colloquio ad
mundum vivum inhabitatum (F0 daemon, F1 proxy, F2 sceletum+legere,
F3 articuli+fila, F4 ergonomia+speculum+fumus). Futura nominata:
online pro Amanda + MCP oauth mobilis (pipatum Franis 01KY5H8XKD),
canales-ut-tags, entitates referibiles, exportatio statica,
markdownum, historia revisionum UI.

## GENERA G0 - MACHINA: INTENTIO (2026-07-23)

Genera per app definita (FileMaker-similis; spec-v2 = consilium,
parcum 01KY656BG1). G0 = machina sola, gesta.c + tabularium.c.
Semina recognita CONTRA FONTEM VIVUM (non ex relatione
exploratorum sola); inventum unum novum: mutatio entis definitionis
claves mutatas SOLAS fert - proiectio registri ergo in plicatura
RES (post plicaturam, datum plenum) NON in consumptore generum
(eventum-formatum, clavis absens).

1. gesta.c:
   (a) migratio V: ALTER TABLE genera ADD COLUMN usor (0=systema,
       1=usoris); numerus IV->V.
   (b) proiectio: in _rei_applicare post scripturam ordinis res -
       si genus_columna=="definitio" -> INSERT OR REPLACE genera
       (titulus=datum.clavis, datum=datum plicatum, usor=1).
       CUSTOS: ordinem systematis (usor=0 exsistentem) numquam
       superscribit - collisio clavis notatur, non clobberat.
   (c) _eventum_validare rami novi (iudicat-non-obstat, nota
       custodiae composita per aedificatorem):
       - creatio generis usoris: claves dati contra campi (typus
         textus/area/dies=chorda, annus/numerus=integer,
         relatio=in dato vetitum, clavis ignota notata; claves
         systematis genus/titulus/corpus/tags/ancorae/signatura/
         status/notae praetermissae);
       - creatio definitionis: clavis+campi forma, collisio clavis;
       - mutatio: entis usoris = idem iudicium camporum; entis
         definitionis = emendatio destructiva (campus remotus/
         retypatus, clavis mutata) notata;
       - membrum-additum pars a: cardinalitas unum - nexus stans
         alter eodem verbo ab eadem re -> nota (verbum ex re nexus
         plicata; in fasce eodem umbra datum non fert -> iudicium
         praetermittitur, v1 acceptum).
2. tabularium.c:
   (a) semen definitio (meta-genus, attributa inline, XIII->XIV);
   (b) addere arg datum (chorda JSON escapata, exemplar gerere;
       claves genus/titulus protectae; ARG_ADDERE X->XI);
   (c) ab_lecto: eventa rerum generum usoris (g.usor=1) et nexuum
       quorum membrum-a res usoris est EXCLUSA - epistulae =
       colloquium; definitio creata = epistula manet;
   (d) legere: tabulatum "nexus" per ordinem (verbum ex dato nexus,
       ad, ad_titulus, ad_status) - LIKE non extenditur.
3. Probationes (sectio XVII post F0): definitio->registrum (ens
   generis novi accipitur), gyrus violationis typi (scriptura
   procedit + nota custodiae in annalibus), unum bis-vinctum,
   emendatio additiva/destructiva, ab_lecto discriminat, legere
   datum+nexus refert, collisio clavis quaestionem non clobberat.
   Cursor: ./gesta/compile_probationes.sh tabularium (NON radix).
Porta G0: probationes gestarum virides omnes.

## GENERA G0: RELATIO (2026-07-23)

Navatum totum ex intentione, deviatio nulla; suite gestarum tota
viridis (fori 13, gesta 680, tabularium 293 [42 nova sectio XVII],
tabulariumd 12).
- gesta.c: migratio V (columna usor), _definitionem_proicere in
  plicatura RES (post scripturam - datum plenum; consumptor
  generum eventum-formatus clavem mutationis non videret, inventum
  reconnaissance), custos clobber (usor=0 numquam superscriptum),
  _campos_iudicare (compositae querelae per aedificatorem),
  _definitionem_validare (forma + collisio), _emendationem_
  definitionis_validare (clavis immutabilis, campi additiva sola),
  _unum_validare (LIKE exemplar legendi, nexus fascis eiusdem
  praetermissus - fenestra theoretica). Rami validationis: creatio
  (definitio-forma aut campi), mutatio (novus ramus), membrum-
  additum (post custodiam membrorum).
- tabularium.c: semen definitio (XIV), addere arg datum (chorda
  JSON escapata, genus/titulus protecta, X->XI), ab_lecto NOT
  EXISTS duplex (entia usoris + nexus datorum exclusa; definitio
  creata = epistula manet), legere tabulatum nexus per ordinem
  (verbum/ad/ad_titulus/ad_status, tituli resoluti).
- Gotcha probationum: textus instrumenti = JSON intra JSON -
  asserta cum virgulis escapatis (\\\") aut verba nuda (worklog).
Proximum G1: probatio_fori datum-gyrus + porta viva MCP (post
reconstructionem daemonis + pkill + reconexionem).

## GENERA G1 - FILUM + PORTA VIVA: RELATIO (2026-07-23)

1. probatio_fori: series altera trans proxy - definitio Sententia
   + ens "Alea iacta est" (datum chorda escapata intra JSON intra
   imperium); responsa II, valores PLANI in annales (datum
   obiectum, non chorda) - asserta directa. 18/18.
2. PORTA VIVA TRANSITA: daemon reconstructus (pkill + launcher),
   genus "sententia" in mundo fori VERO per mcp__fori__addere
   definitum (schema vetus sessionis - parametra extra transeunt,
   ut analysis fili praedixit), entia II typata (annus -49 et 62),
   legere embed vivum, ab_lecto entia excludit vivum.
3. INVENTUM PORTAE VIVAE: _nova_numerare sine praedicato
   exclusionis - "et 1 plura restant" promissum, fossa sequens
   "(nihil novi)" negavit. Idem numerator salutationem NOVA alit:
   dies solum-datorum salutasset epistulas inexsistentes. SANATUM
   eodem praedicato ac lectio (LEFT JOIN res + NOT EXISTS duplex);
   probationes pinctae (ens novum solum -> NOVA tacet, nihil
   promittitur). Lex: NUMERATOR PROMISSI = PRAEDICATUM LECTIONIS -
   quotiens lectio filtratur, quisque numerator qui eam promittit
   eodem filtro utatur.
Suite tota: fori 18 / gesta 680 / tabularium 297 / tabulariumd 12.
Reconexio fori non necessaria functioni (parametra extra
transeunt) - solum pro schemate datum viso in tools/list.
Proximum G2: app (tractator transmittens generalis + tabula Res +
creator + visus tabularis + formae + fumus).

## GENERA G2 - APP: INTENTIO (2026-07-23)

Spec-v2 par VI-VII; C minimus, IS maximus.
1. forum.c: tractator UNUS generalis "transmittere"
   {instrumentum, argumenta} - allowlist addere|gerere|legere|
   quaerere, actor="fran" iniectus, per _instrumentum_vocare.
   Post hoc genera/campi novi C numquam reaperiunt.
2. index.html:
   (a) machinae visuum: nav-activa generalizata (binaria ->
       per-tabulam), tabula tertia "Res"; sub-status in variabilibus
       (res_genus/res_visus) non explosione visuum;
   (b) index generum (definitiones + numeri entium + creator);
   (c) creator: titulus monstrans -> clavis derivata viva (semel
       monstrata), ordines camporum (titulus/typus/ad/cardinalitas),
       campus tituli, ordo ordinarius;
   (d) tabula autoderivata: columnae ex definitione, ordinatio per
       columnam IN redditione (sine refetch - custos diff aliter
       supprimeret), annus signatus numerice, ordo ordinarius ex
       definitione;
   (e) visus entis: campi omnes + porticus inversa (quis huc
       monstrat - legere generum monstrantium, filtrum IS-latere);
   (f) forma entis: visus proprius fossae exemptus (exemplar
       editoris - arca_activa TEXTAREA sola est), latitudines per
       typum, annus negativus + titulus aerae, selector relationis
       quaere-dum-scribis + creatio inserta, denexus+nexus in
       remutatione unum;
   (g) arca FTS (quaerere + filtrum generis);
   (h) lapides sepulcrales destinationum mortuarum ('[deletum]');
   (i) chorographia fumi extensa: definire -> entia (annus -753)
       -> tabula ordinata -> creatio inserta + nexus -> porticus
       inversa -> fumus_perfectus (tectum LXXV gradus, ansa
       forum.c:49 si opus).
3. GOTCHA: capsula_generare post editiones index.html (struere
   capsulam praegeneratam compilat); rm ante cp iam in forum.sh.
Porta G2: fumus.sh FUMUS PLENUS + fenestra viva Franis.

## GENERA G2 - APP: RELATIO (2026-07-23)

FUMUS PLENUS PRIMO CURSU. Navatum ex intentione:
- forum.c: tractator UNUS novus "transmittere" (allowlist IV,
  actor fran iniectus, res_id extractum) - tota superficies
  generum C-latere = ~60 lineae; _chorda_est localis addita
  (examen vocationem implicitam cepit - tabularii auxilium erat,
  non fori); FUMUS_GRESSUS_MAXIMI LXXV -> CL (gyri generum).
- index.html (~570 -> ~1230 lineae): tabula tertia Res + sub-visus
  (index generum cum numeris entium / creator / tabula / ens /
  forma); creator cum clave derivata viva + ordinibus camporum +
  campo tituli + ordine ordinario; tabula autoderivata (columnae ex
  definitione, ordinatio per caput columnae SINE refetch, annus
  signatus "N a.C.n."); visus entis cum porticu inversa (genera
  monstrantia + saltus trans genera); forma poll-exempta (exemplar
  editoris) cum selectore quaere-dum-scribis + creatione inserta +
  diff denexus/nexus in editione + remotio camporum vacuatorum;
  arca FTS per quaerere (ids -> filtrum clientis); arca_activa ad
  INPUT extensa (dataset.inter opt-in).
- Chorographia fumi: post filium pipati, definire II genera ->
  entia typata (annus -753) -> creatio inserta Romuli + nexus ->
  legere embed titulo resoluto -> TABULA IN DOM reddita et asserta
  (ordines II, Roma prima ascendente, "753 a.C.n.", columna
  relationis) -> fumus_perfectus. Assertum DOM = porta UI vera
  sine oculis.
Capsula in assets/ regenerata (bin/capsula_generare - via vera
apps/forum/assets/capsula_forum.*, non apps/forum/).
Proximum G3: barra vivida - Fran libri + linea temporis per UI,
usus trans sessiones, ens unum a Claudio via MCP.

## GENERA G2.1 - VERITAS + ELECTIO: INTENTIO (2026-07-23)

Ex usu vero primo (Fran genera creavit): typus veritas (boolean -
decisionem colloquii "tags sufficiunt" barra vivida falsificavit)
+ typus electio (index valorum FileMaker; optiones in campo
definitionis). Palette machinae data-acta: tabularium.c ZERO.
1. gesta.c _campos_iudicare: veritas = json boolean; electio =
   chorda in campo.optiones (extra -> nota).
2. index.html: creator arca optionum (commatibus, visibilis cum
   electio); forma checkbox/select; tabula "✓"/"—" et ordinatio
   electionis PER INDICEM OPTIONUM (ordo definitionis = ordo
   verus, non alphabeticus); fumus extensus.
3. Probationes: campi novi in definitione libri XVII + violationes
   (chorda pro veritate, valor extra optiones).

## GENERA G2.1: RELATIO (2026-07-23)

Navatum; FUMUS PLENUS iterum primo cursu. gesta.c: rami veritas
(json boolean) + electio (valor in optionibus campi - iudicium
optiones ex ipso campo legit, non ex genere) in _campos_iudicare;
tabularium.c INTACTUM (palette data-acta - typus novus = editio
gesta.c + IS solae). App: creator arca optionum, forma checkbox
(non-signatum = false expressum - checkbox absentiam exprimere
nequit) + select cum optione vacua, tabula "✓"/"—" et ordinatio
electionis PER INDICEM OPTIONUM (desideratus < possessus < lectus,
non alphabetice), fumus V columnae assertae. Probationes 299.
Nota culturalis: decisio colloquii "booleani = tags" die uno usus
veri falsificata - barra vivida > praedictio; electio = status-
sine-machina (gradus lecturae sine vita formali).

## GENERA G2.2 - EMENDATIO PLENA GENERUM: INTENTIO+RELATIO (2026-07-23)

Decisio Franis (usus verus, exemplar mentis eius): definitio
IUDICAT, non constringit - emendatio libera, discrepantiae molles
signatae pro reparatione humana. "Additiva-sola v1" mortua die
uno (secunda falsificatio colloquii per barram vividam).
- gesta.c: _campum_iudicare extractum (valor unus, commune
  scripturae ac saluti); _emendationem_definitionis_validare
  RELAXATUM (clavis generis sola immutabilis - notae "emendatio
  destructiva" mortuae: evolutio legitima non violatio); SALUS
  sectio II-bis campi (claves status contra definitionem currentem
  -> census insalubres + res [cautio] = superficies orphanorum).
  Compatibilitas familiis typorum GRATUITA (textus/area/dies =
  chorda; annus/numerus = integer) - retypatio intra familiam
  orphanos non creat.
- App: "genus recensere" in tabula -> creator praeimpletus
  (creator_aperire; clavis fixa in editione); renominatio =
  clavis derivata mutata -> genus_emendare (mutatio definitionis
  + remotio clavium optionalium sublatarum [mersio superficialis
  non delet!] + migratio per ens: mutatio nova + remotio vetus -
  eventus honesti); signa "⚠ extra specem" in ordinibus tabulae
  et capite entis (speculum IS iudicii machinae).
- Fumus: renominatio sors->fatum EODEM motore quo editor + entia
  migrata asserta. FUMUS PLENUS primo cursu. Probationes 303
  (insalubres 1 ante / 3 post remotionem campi - numerus exactus
  ratiocinatus et confirmatus). Suite 18/680/303/12.

## SCHOLIA - EDITOR ADNOTATIONUM (docco domus): INTENTIO (2026-07-23)

Ex editore vetere Franis (fons amissus): documentum quodlibet
adglutinatur, lineae selectae, adnotationes in margine stilo docco
(traditio scholiastarum - nomen exactum). Tabula quarta fori.
MACHINA INTACTA: codex = res cum textu in corpore (FTS gratis,
textus CONGELATUS post adglutinationem - deriva adnotationum);
scholium = addere unum {corpus, ad: codex, datum: {linea_a,
linea_b}} - titulus ex corpore derivatus. Genera per definitiones
se-serentes (primo usu creantur - mundus quilibet). Decisa: clic +
shift-clic pro selectione; scholia = DATA quieta (ab_lecto ea non
videt - studium Franis, non epistulae); nomina scholia/codex/
scholium. UI: sub-visus index (codices + numeri scholiorum) /
novus (adglutinatio, fossae exemptus) / codex (LECTOR: corpus
latum [body.latus ~80em], lineae numeratae dextra, margo
scholiorum sinistra positione absoluta ad lineas ancoratas +
cumulus anti-collisionis, selecta illuminata, arca compositionis
in margine). Fumus: codex III linearum + scholium + asserta DOM.

## SCHOLIA: RELATIO (2026-07-23)

FUMUS PLENUS PRIMO CURSU. Navatum ex intentione integro: machina
INTACTA (codex/scholium genera per definitiones se-serentes primo
usu; scholium = addere unum cum ad + datum ancorae); lector docco
cum margine absoluto + cumulo anti-collisionis + illuminatione
mutua (carta <-> lineae); selectio clic/shift-clic (clic iterum =
dissolutio); compositio et adglutinatio fossae exemptae (sel_a
custos); scholia quieta (usor=1 - ab_lecto ea non videt, decisio);
corpus congelatus post creationem (deriva ancorarum). Chorographia
fumi: genera se-serentia -> codex -> scholium ancoratum -> DOM
(lineae III, carta I, linea secunda adnotata). Sub-visus scholia
in ultima serie inclusi. Tabula quarta fori nata.
