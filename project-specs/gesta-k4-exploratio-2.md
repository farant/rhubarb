# K4 EXPLORATIO 2 — suturae nostrae (gesta/tabularium)
# Relatio exploratoris E2 verbatim (compressa), 2026-07-16. Fons pro spec-v2.

## 0. LEX ARCHITECTONICA (quod totum audit revelat)
Isolatio branch-legendi = UNA sutura. OMNIS lectio praeter scripturam
transit per plicaturas (res/genera/membra/res_fts/sordidae), non per
tessellas crudas. Ergo si _consumptorem_plicare rows-branch excludit
(§3), quaeque proiectio fit trunk-pura GRATIS. Motor (decisio 7),
census, FTS, salus, nexus, actiones — omnes trunk-only per
constructionem, sine tactu. Branch READ path = codex NOVUS
(_res_in_ramo_capere), non emendatio lectionum stantium.
AGERE/PROCESSUS = TRUNK v1 (spec §IV enumerat SOLUM addere/gerere/res;
decisio 7 motor-trunk). Ergo _portam_probare, gesta_agere,
_provehere_ad_fixum, _provectionem_excitare INTACTA. Superficies
branch-iudicii = solum crudae scripturae (creatio/mutatio/status/nota/
membrum-*) + res-lectiones. Parvum.

## 1. INVENTARIUM UNI-TEMPORIS (SELECT/INSERT tessellarum)
Tessellae.branch_id EXISTIT (migratio I, gesta.c:28) sed nemo valorem
non-vacuum scribit; nemo per eum filtrat. Tabula I:
- _consumptorem_plicare:1196-1198 SELECT seq>? — **(c) ADDE branch_id=''**.
  Sutura CENTRALIS. Omnia infra ex hac pendent.
- _eventum_inserere:1554-1557 INSERT (branch_id omissus, DEFAULT '')
  — **(b) branch-aware: ADDE columnam + liga ex paratum**. Trunk liga
  '' expresse (G9). Vide B6.
- _annalem_appendere:1310-1311 hardcode branch_id "" — **(b) param +
  scribe valorem verum**. Vide §5.
- gesta_ex_annalibus_restituere:2628-2632 INSERT — **IAM branch_id
  (col VIII, liga 2662-2664)**. PARATUM K1. Nihil mutandum. (a).
- gesta_annales_verificare:2472-2474 SELECT 8 columnas (branch_id
  ABSENS); compare 2541-2562 — **(a) manet, sed vide B4** (1:1
  debilitatum ad 8/9).
- gesta_seq_ultima:5101-5102 MAX(seq) globalis — **(a) recte
  trunk-agnosticum**: punctum = fork globalis trans branches. Nullus
  vocans internus; publicus pro gesta_ramum_creare.
- _tab_res annales:1946-1949 SELECT ...WHERE res_id — miscet trunk+
  branch eventus rei — **(c) ADDE branch_id='' pro visu trunci**
  (aut branch-aware si res+ramus). Minor.
- Proiectiones (res/genera/membra/res_fts/sordidae/consumptores):
  _hwm_capere:212, _genus_datum_capere:237, _res_capere:268,
  gesta_quaerere:2216, census_generum:2265, census_tagorum:2336,
  nexus_rei:2705, socii_rei:2747, insalubres:3134, actiones_rei:3962,
  excitare:4779 — **OMNES (a) trunk-purae AUTOMATICE** post §3 filtrum.

## 2. DECOMPOSITIO PLICATURAE (_rei_applicare:746-1093)
Solae contactus tabulae res: _res_capere:762 (lectio) + INSERT OR
REPLACE:1074-1092 (scriptura). Media 785-1072 = pura manipulatio JSON
+ genus-lectiones (815 trunk-globalis). Scissio (stylus K3 praeparare/
validare/inserere):
- **_status_transformare** (PURA, JSON in/out): extrahe 785-1072 —
  catena si/alioquin (creatio/mutatio/remotio/status/nota/membrum-*/
  gradus-*/opus-genitum/actio-acta) INCLUSA plicatura membrorum K2
  (916-980) et cursus/opera/actae K3 (981-1046). Signatura:
  `_status_transformare(GestaMundus* m, GestaStatusFold* f,
    chorda genus_ev, chorda datum, chorda actor, chorda creatum,
    Piscina* p)` ubi GestaStatusFold {JsonValor* status_obiectum;
    chorda genus_columna; chorda creatum_columna; b32 exsistit;
    b32 mutatum;}. Non 100% pura — legit genera (815-835) pro
    status_initialis; genera TRUNK-GLOBALIA immutabilia intra passum,
    licitum (decisio 8). "State-pure": nulla res-tabula lecta/scripta.
- **_rei_applicare** (involucrum trunk avidum): 762 _res_capere ->
  imple GestaStatusFold -> _status_transformare -> 1062-1092 proiectio
  + INSERT. Nomen/mores idem (G9 byte-idem).
- **_res_in_ramo_capere** (NOVA, branch pigra): _catena_rami walk ->
  SELECT catena (§0 oraculi: (branch_id='' AND seq<=p1) OR (branch_id
  =a AND seq<=p2) OR branch_id=ramus) ORDER BY seq -> ansa
  _status_transformare in memoria -> reddit GestaResOrdo (idem typus
  ac _res_capere -> commutationes §4 drop-in).
MEMBRA INDEX (_membris_applicare:1102) TRUNK-ONLY — branch membra
vivunt in statu-plicato (state.membra), non in indice. VERIFICATUM:
_membrum_validare comites/cardinalitas legit ex ordo.datum:562,
json "membra":568 — TABULA membra NON tacta. Ergo si
_res_in_ramo_capere ordo.datum branch-plicatum reddit, _membrum_
validare 562-595 branch-recte OPERATUR SINE MUTATIONE. Soli
consumptores indicis (nexus_rei/socii_rei/excitare) trunk — recte.

## 3. SEMANTICA HWM (branch-seqs saltandi)
_consumptorem_plicare:1204 liga hwm; 1229 ultima=seq; 1232 si ultima>
hwm scribe. hwm = LIMEN (seq>), NON numerator contiguus. Adde
`AND branch_id=''`:1196 — ansa videt SOLUM rows trunk, ultima = seq
trunci ULTIMI. Exemplum: trunk 1,2,3 / branch 4,5 / trunk 6, hwm=3 ->
videt 6 solum -> ultima=6 -> hwm=6. Proximum seq>6 -> nihil. RECTE:
saltat 4,5 sine applicatione, numquam revisit (< 6). NULLA statio,
NULLA refoldatio. Casus limes: cauda tota-branch (trunk 1,2,3/branch
4,5, hwm=3) -> videt nihil -> ultima=hwm -> `ultima>hwm` FALSUM ->
hwm MANET 3 (recte); proximum trunk seq 6 re-scanit seq>3 filtratque
4,5 (vile). Ergo INDEX: **migratio IV `CREATE INDEX
idx_tessellae_ramus ON tessellae(branch_id, seq)`** — servit AMBAS
semitas: trunk-scan (praefixum branch_id='' + seq>hwm) ET catena-
branch (praefixum branch_id=X). Non necessarius correctioni
(seq-range scan + filtrum corpore vilis), sed cauda-branch-longa
sine indice O(cauda) per plicaturam. gesta_seq_ultima callers:
punctum globalis — recte, nullus tactus.

## 4. PUNCTA COMMUTATIONIS LECTIONIS-VALIDATIONIS
Regula: **res-ordo lectio (identitas/status rei) -> BRANCH in via
scripturae; genus-definitio lectio (_genus_datum_capere/_species_est)
-> SEMPER TRUNK.** _res_capere_cum_obumbra:512/525/598 (in _membrum_
validare), :1458 (_eventum_validare transitio), :1655 (_fascis_scribere
dup creationis) — OMNES via scripturae -> **commuta ad _res_in_ramo_
capere_cum_obumbra(m, res_id, ramus, obumbrae, p)** (= _res_capere si
ramus vacuus, G9). _genus_datum_capere:1445/1467 (_eventum_validare),
_species_est:521/535/540 (_membrum_validare) — genera -> TRUNK, NON
commuta (branch eventus contra trunk genera validant, decisio 8).
NON via scripturae (manent trunk): _res_capere:1131 (_membris_applicare
solutum — consumptor plicaturae, trunk rows solum), :2110 (fts),
:2851 (salus — superficies, branch optio non postulata), :3499
(_portam_probare — AGERE trunk v1), :3955 (actiones_rei), :4259/4351
(motor). Ergo mechanismus: filum `constans character* ramus` per
_eventum_validare/_membrum_validare/dup-check; NIHIL/""=truncus.

## 5. ANNALES
- _annalem_appendere:1286-1336: hardcode 1310-1311 `_ch("")`.
  MUTATIO: adde param `constans character* branch_id`, scribe
  `_ch(branch_id)`. Vocans _eventum_inserere:1578 tradit ex paratum.
  Trunk paratum.branch_id="" -> linea byte-idem (G9).
- gesta_ex_annalibus_restituere: **IAM branch_id (col VIII in INSERT
  2631, liga 2662-2664)** ex linea annalium. K1 providentia — nihil
  mutandum, round-trip GRATIS. Cautio: linea vetus sine campo
  branch_id -> json_objectum_capere NIHIL -> _chorda_tuta("") ->
  "". Tutum.
- gesta_annales_verificare: branch-AGNOSTICUM (decisio 11); legit 8
  campos, branch_id IGNORAT. TENET pro lineis branch (8 campi
  congruunt). Vide B4 (1:1 debilitatum).

## 6. FORMA TRANSACTIONIS FUSIONIS (nullum BEGIN nidificatum)
scrinium BEGIN crudum (k3-expl §2). Conflictus-probatio (lectiones)
EXTRA txn. Sequela gesta_ramum_fundere(m, ramus, vis, p, out):
1. catena=_catena_rami; conflictus=gesta_confligentia (lectiones). Si
   conflictus && !vis -> imple out.conflictus; redde VERUM (recusatum,
   non fractum). NULLA scriptura.
2. rows = `SELECT seq,id,res_id,genus_eventus,datum,actor,origo,creatum
   FROM tessellae WHERE branch_id=? ORDER BY seq` (colligere).
3. aedifica copy-paratums: id NOVUS (ULID cusum — id UNIQUE, fons-id
   reuse violat), res_id/genus_eventus/datum/actor SERVATA, origo ->
   "merge:<titulus>", branch_id="" (in truncum). seq novus per
   AUTOINCREMENT.
4. + paratum status ramus->fusus (res_id=ramus, genus="status",
   datum={"novus":"fusus"}, branch_id="") — **INTRA txn copiae**
   (atomicitas: si copia committit sed status separatim cadit, ramus
   merged-sed-activus lacer; K1 lex = solum annales-superset toleratur;
   branch-metadata trunk-est decisio 6).
5. UNA txn: scrinium_incipere -> ansa _eventum_inserere per paratum
   (INSERT + _annalem_appendere(branch='') + sordidae, ordine seq,
   annales ANTE COMMIT) -> scrinium_committere. **NON _fascis_scribere
   — RAW COPIES sine re-validatione** (decisio 5; vide B3).
6. gesta_plicare semel (plicaturae trunci resumunt — copiae folded ut
   trunk normalis: K2 membra, K3 cursus).
7. cribatio sondae: per eventum COPIATUM genus="status" ->
   _provectionem_excitare(m, res_id). VERIFICATUM callable: interior
   (4762), fundere frater in gesta.c. Reuse ut est.
Reuse _eventum_inserere = fructus decompositionis K3-A ITERUM. Una
BEGIN, unus COMMIT, N annales ante commit, plicare semel, cribatio
post. Nulla txn nidificata. origo "merge:<titulus>" — titulus = status
ramus rei (non res_id). Vide B5 (nested).

## 7. FORMA API (param vs semitae parallelae)
Vocantes gesta_scribere: _tab_addere:1257, _tab_gerere (regio
1337-1569), _seminare:2421/2459/2475, + effectus/motor interni +
probationes. OMNES trunk, numquam ramum tradent. Param in signatura ->
churn mechanicus in ~duodecim locis trunk-solis (praecedens: NOTA
actoris K3 phase-log:672-677 — differtus ad stratum instrumenti ob
churn). RECOMMENDATIO: **SEMITAE PARALLELAE pro paucis superficiebus
tenentis, param per KERNEL internum.**
- Publica NOVA (tenens §IV addere/gerere/res solum): gesta_scribere_
  ramo(m, eventum, ramus, res_id_out); gesta_res_datum_ramo/
  gesta_res_status_ramo(m, res_id, ramus, p). + lifecycle (ramus
  naturale): gesta_ramum_creare/ramos_enumerare/confligentia/ramum_
  fundere/ramum_abicere.
- Publica STANTIA (gesta_scribere/fascis_scribere/res_datum) INTACTA
  — corpora delegant ramus="".
- Kernel internum (_fascis_scribere/_eventum_validare/_membrum_
  validare/dup-check/_annalem_appendere/_eventum_inserere) filum
  ramus ACCIPIT, NIHIL/""=truncus (G9 byte-idem). Semitae _ramo
  ramum verum tradunt.
Refutatio generis-in-ramo (decisio 8): in ansa _fascis_scribere post
praeparare, si ramus!="" && p.est_generis -> _fractum. Porta
mechanica munda (chunk A).

## 8. TERMINI FRUSTORUM (barra inertiae G9 quaque)
**A — decompositio + branch kernel** (barra: mores no-ramus byte-idem):
_status_transformare/_rei_applicare scissio (refactor mores-nullus,
ut K3-A _scribere_crudum); migratio IV idx + `AND branch_id=''` in
plicare (iners — omnes rows ''); filum ramus per kernel; branch_id in
INSERT+annales lig ''; _catena_rami/_res_in_ramo_capere(_cum_obumbra);
porta generis-in-ramo. BARRA: nullum semen ramus, nullus vocator _ramo
-> 498+ gesta + tabularium asserta byte-idem; filtrum no-op in acervo
omni-trunk.
**B — lifecycle + fusio + cribatio** (iners sine rami): ramum_creare
(punctum=seq_ultima, res genus ramus ON TRUNK), ramos_enumerare,
confligentia (ConflictInfo), ramum_fundere (§6), ramum_abicere
(status->abiectus); semitae publicae _ramo ligatae. BARRA: sine
genere "ramus" seminato + nullo vocante, NIHIL excitat (ut K3-B "motor
INERS sine instantiis"); goldens branch NOVA solum (isolatio fork,
catenae nestae, fusio felix/conflictus, abicere, restitutio round-
trip).
**C — tenens** (census/quaerere/tabula minus RAMI byte-idem; addere/
gerere/res sine ramus byte-idem): semen v4 (+genus ramus, SEMINA_
NUMERUS IX->X, INFRA SEMINA_BOARD_NUMERUS — nulla fusio attributa,
disciplina E2-B2); instrumentum "ramus" {actus} + param ramus optionis
in addere/gerere/res; tabula RAMI; doctrina v5; deploy; sigillum per
ritum currentem.

## SPEC V1 BREAKS
B1 (MUST-FIX, silens): resolutio-tituli tenentis legit TRUNCUM.
    _res_solvere:663 + _tituli_numerus:639 tabulam res (proiectio
    trunci) quaerunt. Sub `gerere/res ...ramus=X`, titulus->res_id
    resolvitur contra TRUNCUM — res in branch creata (nondum in
    trunco) irresolubilis; res trunk-post-punctum resolveretur (sed
    branch caeca — violatio fork honesti). v1: (a) branch scripturae/
    lectiones res_id POSTULANT (non titulum) + nota doctrinae; aut
    (b) _res_solvere variantem branch-consciam (resolvit contra
    plicaturam branch). RECOMMENDO (a) v1, flag (b). SILENS: nullum
    test branch-titulum exercet; resolvens trunk "operatur" (false).

B2 (MUST-FIX, silens): _membrum_validare OMNES tres ordo-fontes
    branch. Container:512 + membrum:525/598 via _res_capere_cum_
    obumbra; comes-numerus:562-595 legit container ordo.datum. Si
    UNUS solus commutatur (container branch, membrum trunk vel
    contra), membrum branch-creatum = "inexistens" spurium AUT
    membrum trunk-post-punctum false-visibile. AMBO + datum
    cardinalitatis ex EADEM catena branch. VERIFICA 512, 525, 598 ET
    ordo.datum:562 ex _res_in_ramo_capere. SILENS: nullum test branch.

B3 (MUST-FIX, silens): fusio NON re-validet (raw copies). Si fundere
    _fascis_scribere reusat (vocat _eventum_validare), eventus
    copiati membrum/status RE-IUDICANTUR contra trunk -> notae
    custodiae NOVAE (aut duplicant notas branch, quae ipsae copiantur).
    Decisio 5 = copiae + cribatio, NULLA re-validatio. Fusio semitam
    inserere-solam (_eventum_inserere ansa), NON _fascis_scribere.
    SILENS: fusio-felix sine conflictu adhuc transiret sed annales
    notas machinae phantasmaticas ferrent.

B4 (DECIDE): verificare branch_id non probat. gesta_annales_
    verificare:2472-2562 legit/comparat 8 campos, branch_id NON.
    Decisio 11 "intactum, branch-agnosticum" — intentum, sed 1:1
    debilitatum ad 8/9 columnas. restitutio branch_id deterministice
    ex annalibus scribit -> divergentia impossibilis in praxi, sed
    branch_id corruptus (db manu-editus) indetectus. RECOMMENDO
    columnam IX + clausulam comparationis (vilis, 1:1 plenum
    restituit). DECIDE: decisio 11 verbatim (relinque) vs stringe.
    Inclino stringere.

B5 (DECIDE): fusio branch-nestae in truncum. fundere copiat WHERE
    branch_id=ramus solum. Branch nesta (filia ex branch-parente)
    cuius PARENS non fusus: copiare filiae rows solas in truncum
    landat eventus referentes statum branch-parentis (creatum in
    parente, non trunco) -> res_ids inexistentes in trunco. v1: (a)
    recusa fusionem branch-nestae parente-non-fuso (mechanicum) +
    report, aut (b) funde catenam totam. RECOMMENDO (a) v1, parca (b).
    Spec §III "nested per oraculo" pro LECTIONE; fusio-nested non
    pinnata.

B6 (MECHANISM): INSERT tessellae + _eventum_inserere branch_id
    postulant; oblivio trunk-DEFAULT frangit byte-identitatem.
    _eventum_inserere:1554-1557 omittit branch_id (DEFAULT ''). Cum
    columnam addimus, trunk MUST ligat "" EXPRESSE. paratum.branch_id
    default "" (memset -> NIHIL -> tracta ut ""). VERIFICA _ch("")
    ligat '' non SQL NULL (columna NOT NULL DEFAULT '' + '' explicitum
    tutum; NIHIL char* = decipula NOT NULL quam commentarium
    restitutionis:108-117 IAM monet). Fila branch_id ut chorda per
    paratum, liga per _chorda_tuta. Non silens, sed lapsus NULL-vs-''
    reproducit decipulam quam :108 monet.

## ADDENDUM (reconciliatio E1/E3)
- Motor trunk-only (decisio 7) NON est constrictio addita — CADIT ex
  §0 (folds skip branch -> res/membra proiectiones trunk-purae ->
  _provectionem_excitare:4779 sondat membra-indicem trunk-solum ->
  branch numquam excitat). Elegans: zero codex pro "engine trunk-only".
- Cribatio-fusionis (§6.7) = drain post-commit K3 exacte (k3-expl §2/
  addendum): status copiatus -> excitare -> _provehere_ad_fixum ->
  fascis micro + plicare. B3-noise (obumbrae stale) non mordet: copiae
  iam-iudicatae, cribatio solum STATUS-eventus sondat (non nota/
  creatio), ut drain K3.
- Dup-check-in-ramo (1655) low-risk: creatio ULID cudit (collisio
  nulla); genera-per-titulum in-ramo REFUTATA (decisio 8/porta chunk
  A). Nota, non periculum.
