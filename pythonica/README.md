# pythonica — facies Pythonica instrumentorum silvae (dev-time)

Scripta editionis quae Claude scribit (heredoc python3) codicem NOMINE
tangant, non textu: `silva.py` involucra subprocessus super CLIs domus
(formator `-extenta`/`-scribere`/`-delta`, examen, differre
`-verdictum`) cum effusu `-machina`. Stdlib sola; NIHIL navatum ab hac
plica pendet — veritas in C manet.

    import sys; sys.path.insert(0, 'pythonica'); import silva

- `silva.extenta(via)` — extenta functionum (nomen → lineae)
- `silva.Editio(via)` — `.replace` (ancorae spatiis tolerantes),
  `.substituere(nomen, textus[, genus='typus'])`, `.inserere_post/ante`,
  `.applicare()` (omnia aut nihil, diff redditus)
- `.applicare()` = punctum scripturae unicum = sedes portarum: custos
  lectionis rancidae, forma (plagula tota), examen, differre contra
  textum ante; `Fructus` (diff, examen, formata, differentia, `.sana`,
  `.unitates()`); `strictum=True` REICE revertit
- `silva.verdictum(via)` — examen + `-delta` uno vocamine
- `silva.differre(a, b)` — paria unitatum + `cosmetica_solum`
- `.replace_inter(initium, finis, novus[, inclusae=True])` — spatium
  INTER ancoram primam et alteram (primam post initium) substituere,
  ancorae SERVATAE; `inclusae=True` = ancorae quoque abeunt (bloci cum
  commentis); Textus quoque. Refusiones ancorarum LINEAS sedium
  nominant ('lineae [12, 340]') ut ancora longior sine grep eligatur
- `silva.metiri(via, n=7)` — computus min-of-n: parsare/lexare ms,
  phases, allocationes, usus (A/B optimizationum)
- `.membrum_addere(typus, textus, post=None)` — membrum structurae/
  unionis/enumerationis nomine typi (extenta genus structura/unio/
  enumeratio; `corpus(via, 'S', genus='typus')`)
- `silva.Refactio()` — transactio trans plagulas: `.editio(via)` una per
  viam, `.prototypum_synchronizare(via_c, via_h, nomen)` (caput
  definitionis -> prototypum; `si_absens='finis'` ante `#endif`),
  `.applicare()` unum: custos in omnibus, scriptura omnium, forma,
  iudicium post omnes scriptas; `strictum` omnes restituit
- `silva.legati(instrumentum, **args)` — pons ad legatum (servus MCP
  silvae, processus proprius pigre natus ~1.5s): `vocantes(t)`,
  `vocata(t)`, `inclusiones(via)`, `caput(via)`, `symbolum(t)`,
  `diagnostica(via)` — idem apparatus quem Claude per MCP videt
- `silva.selecta(via, selector, intra=None)` — sedes structurales byte-
  exactae; `.replace_selecta(selector, novus, intra=)` ancora sine
  textu; `silva.differre_git(via, ref)` verdictum contra historiam;
  `silva.origo(via, linea)`, `silva.arbor(via)` textus crudi
- ancorae `.replace` ordinariae = SERIES LEXEMATUM (spatia ubique
  indifferentia, litterae exactae); `tolerans='spatia'` cursus, `False`
  octeti
- `silva.porta(nomen, filtrum=None)` — portam currere (tabula PORTAE:
  radix/silva/css/.../pythonica/fumi/amalgamata): cucurrit (signum) +
  sana; `silva.commissio(nuntius, viae, portae=[...])` — portae omnes
  sanae aut nihil commissum; viae vetitae refutatae; renominationes
  per `git mv` acceptae (viae NOVAE dantur); `verificare=False`
- `silva.planta(via, vetus, novus, porta)` — ritus culpae plantatae:
  rubra (non aedificatio fracta) -> reversio (finally) -> viridis
- `silva.porta_umbra(nomen[, photographica=True])` — porta in umbra
  (processus separatus) contra PHOTOGRAPHIAM status operis: arbor git
  status operis capta (index temporarius, arbor viva intacta), clone
  localis in ~/.rhubarb/umbrae (obiecta hardlinked, .git verum, HEAD =
  basis, read-tree arboris, bin/build/... clonata copy-on-write), porta
  ibi currit - EDITIO PERGIT dum currit. Receptum sigillum = arbor;
  `commissio(portae=[via.json])` plagulas SUAS contra blobs arboris
  confert (HEAD = basis; quod committitur = quod probatum est, cetera
  libera); `photographica=False` = mos vetus (sigillum arboris vivae,
  rancidum si quid mutatum). `exspectare(via)`, `portae_pendentes()`,
  `receptum_delere(via)` (clone quoque sublatus). Primitiva:
  `photographia_capere()` -> Photographia(arbor, basis, via),
  `photographia_continet(arbor, via)`, `photographia_materializare(ph,
  nomen)`, `photographia_delere(ph)` (tempora plagularum tractarum e arbore viva speculata — read-tree 'nunc' stampat, clonefile tempora servat: custodes binariorum in clone ut vivi iudicant). Custodes: nomina (portae, filtra,
  photographiae, imagines) segmentum viae UNUM esse debent (refusio
  ante ullam scripturam); `umbrae_purgare()` clones orphanos (sine
  recepto nec signo pendenti) delet, `portae_pendentes()` eos enumerat
- `silva.Textus(via)` — editio textus PLANI (sh, md, html, toml):
  ancorae exactae numero asserto (`tolerans='spatia'`), omnia aut
  nihil, custos lectionis rancidae, sine formatore/examine
- ancorae `.replace` Editionis: commentum = lexema unum, spatia INTRA
  collapsa (tabulae, lineae refractae indifferentes)
- `silva.commissio_umbra(nuntius, viae, ['radix', 'silva'])` — portae
  umbrae SERIATIM (tempora fida), commissio contra recepta omnia,
  recepta deleta; `siccum=True` = portae solae; reddit (hash, [(porta,
  compendium, totum_s)])
- **porta rubra nominat quid ruberit**: `Porta.fracturae` =
  [Fractura(nomen, relatio)] (relatio = lineae diagnosticae effusus
  probationis IPSIUS: FRACTA/Speratus/Receptus/Totalis/Conditio);
  `silva.fracturae(acta, porta)` lectio pura (formae radix / suita /
  generica); `relatio_fracturarum(fr)`; receptum umbrae eas fert,
  `receptum_relatio(via)` post cursum legit; errores commissionis et
  commissio_umbra eas ferunt pro cauda actorum; planta compendium
  rubrum 'fractae: X (Conditio: ...)' reddit et imprimit
- `silva.probatio_currere(nomen, aedificare=, secunda=, mora=)` —
  probatio UNA sicut cursor eius (e radice, RHUBARB_RADIX, binarium
  suitae: radix /tmp/<nomen>, ceterae <suita>/build/<nomen>); nomen aut
  via binarii; `secunda>0` = profilum per `sample` post moram ->
  `Cursus.profilum` folia [(numerus, functio, bibliotheca)], effusus
  build/sample/<nomen>.probatio.txt; `cursus_textus(c)` compendium
  (assertiones/fractae/conditio + fracturae + profilum);
  `probatio_suita(nomen)`
- **profilum**: `Cursus.via_profili` (effusus sample crudus);
  `silva.profilum_inclusivum(via)` = tempus inclusivum per functionem
  RECURSIONE COLLAPSA (summa ingenua ambulatores recursivos inflat);
  `profilum_viae(via, functio)` = semitae a main ad functionem, ordine
  ponderis, recursio 'fn×k' compressa; `cursus_textus` utrumque imprimit
- **imagines** (oraculum identitatis octetorum): `silva.imago_capere(nomen,
  imperium, plagulae)` effusum imperii per plagulam ('{}' = via) in
  build/imagines/<nomen>/ servat (+ rc, manifestum HEAD);
  `imago_conferre(nomen[, imperium, plagulae])` -> Collatio (eaedem /
  diversae / absentes / novae, `.sana`, str); `imago_differentia(nomen,
  via)` diff ante/post. Methodus probationis domus facta primitivum
- `Refactio.applicare()` reddit `FructusOmnes` (lista Fructus cum
  `.sana` omnium et str) — eadem assertio ac Fructus unius
- `silva.mensurae(praefixum, quantum)` — sessiones voluminis mensoris
  ('' radix, 'silva.' silva; tituli praefixo exuti; plenae solae);
  `compendium_mensurae(sessio)` = totum/cursus/compilatio/praevolatus
  + tardissimae
- `silva.usus(symbolum)` — sedes + usus per plagulam (nexus)
- `silva.renominare(vetus, novum, viae, scribere=False)` — planum/applicatio

Porta natalis: `./pythonica/probare.sh` (probatio_silva.py).

## 2026-09-03 — additiones post B1 (recensio Franis)

- **`Editio` parsationem mortuam nominat**: nomine absente, textus praesens
  pauciora extenta quam originalis reddens = parsatio mortua -> SilvaError
  'parsatio textus praesentis MORTUA (n pro m): <clang error: linea prima>'
  (aliter violatio examinis) - numquam iam 'non inventum' de functione
  quaque posteriore. `Editio.commentum(nomen, novus)` = commentarium ducens
  substituere (substituere corpus solum tangit).
  `Editio.inserere_ante_vocationem(functio, novus)` + `probatio_addere(via,
  novus, ante='credo_imprimere_compendium', **applicare)` = casus
  probationis ante compendium.
- **`syntaxis(via[, textus])`** = clang -fsyntax-only vexillis domus
  (None sana | linea 'error:'); **`planta` praevolatus**: planta non
  compilans ante ritum refutatur (nihil cursum, plagula intacta).
- **`expandere(via | textus)`** = stml expandere -> Expansio(successus,
  vitium titulus, linea, fragmentum, loculus, textus) - sondae C pro
  numero vitii non iam.
- `commissio_umbra` fracta: nuntius `receptum_delere(via)` nominat (receptum
  et clone servata sunt consulto - `umbrae_purgare` orphanos solos tollit).

## 2026-09-03 — lex formae ancorarum (post B3)

- **Lex formae `Editio.replace` tolerantis**: ancora = series lexematum
  (ut olim), sed scriptura novi iam formam plagulae respicit. Novus
  PLANUS (sine linea nova) lexematibus PARIBUS = lexemata sola in situ
  mutantur, spatia et lineae plagulae manent (etiam trans lineas); novus
  planus lexematibus imparibus super extentum plurium linearum = REFUSIO
  nominata ('forma perderetur' + lineae) — novus cum lineis novis
  scribatur; novus vacuus delet; novus cum lineis novis = forma auctoris
  verbatim. Causa: quindecim sedes in md_arbor.c (A8c, B3) et quinque
  in probatio_md_inlinea.c per ancoras tolerantes trans lineas cum novo
  plano contortae — formator (sine regula 'sententia in linea sua')
  lineam iunctam per columnas fregit et fructum ACCEPIT. Formator ter
  innocens probatus (idem plagulae; mutatio bene formata in versionem
  priorem; sonda minima). Reparatio per `differre` = cosmetica omnibus
  unitatibus.
- **Ancora absens sedem PROXIMAM nominat**: series lexematum longissime
  congruens (a linea N), lexema divergens exspectatum/inventum cum linea
  et textu eius; ancora exacta absens seriem lexematum inventam nominat
  ('tolerans=True'); lexema primum absens nominatur. Grep post
  refusionem non iam.
- **Commenta reflexa**: margines ' * ' linearum sequentium in lexemate
  commenti indifferentes (ancora commenti toti sine marginibus scribi
  potest); **`tolerans='verba'`** = verba prosae trans margines
  commenti, scriptura verbatim (sententia in commento capitis mutanda).
- **`silva.oraculum(exemplum=None)`** → Oraculum(praeterita, totalis,
  pinna, sectiones {titulus: (praeterita, totalis)}, fracturae
  [FracturaOraculi numerus/plagula/sectio/causa/sperata/nostra] OMNES,
  exempla [ExemplumOraculi md/sperata/nostra cruda] pro numero (plagula
  utraque), ignoscentiae, acta) — ambitus ORACULUM_OMNIA/EXEMPLUM et
  grep non iam (I s).
- **Refactio pro scriptis plurium plagularum** (monitum, non additio):
  `applicare` per plagulam in scripto uno statum dimidium relinquit cum
  ancora quinta fallit; `silva.Refactio()` ancoras omnes ante scripturam
  ullam resolvit — ea utere, non Editio seriatim.
- **Reparatio formae**: sedes contorta et sedes recta eandem seriem
  lexematum habent ('} m_ad = ...' bis) — ancora tolerans ambigua est;
  octeti exacti (`tolerans=False`) sedem contortam solam nominant.

## 2026-09-04 — Prosa (C1): markdown per ancoras structurales

- **`silva.Prosa(via)`** = Textus + extenta per `./md/extenta.sh <x.md>
  '<selector>'` (selectio super proiectionem md; TSV octetim). OCTETI:
  plagula ut bytes, textus utf-8 pro lectione. `.selecta(selector)` →
  [ProsaExtentum(tag, initium, finis, linea, columna, linea_finis,
  versio)]; sugar `.capitulum(titulus[, gradus])` (linea capituli, textus
  sine marcis), `.sectio(titulus[, gradus])` (capitulum cum corpore usque
  ad capitulum gradus <=, vacuae caudales exclusae), `.elementum(n,
  intra=)`, `.saeptum(lingua=, n=, intra=)`; `.corpus(x)`,
  `.substituere(x, novus)`, `.inserere_post/ante(x, novus)`, `.replace`
  (exacta), `.appendere`; `.applicare()` = custos lectionis rancidae,
  scriptura semel, IUDEX (parsura + extentum 'documentum' == plagula
  tota), ancorae RELATAE (ProsaFructus.ancorae). Extenta post editionem
  RANCESCUNT (versio) — reselige. Refusiones: capitulum absens nominat
  capitula praesentia; n extra numerum; selector malus.
- `silva.prosa_extenta(via, selector)` = involucrum crudum extenta.sh.

## 2026-09-04 — census + citata (C2): instrumentum corporis

- **`./md/census.sh [-machina | -citata [via.md] | -scribere]`** — numeri ex
  ARBORE PARSATA super `git ls-files '*.md'` (MCXXIX plagulae, XVII MB, I
  s): nodi per genus, capitula per gradum, listae/laxae/officia, saepta per
  formam et linguam (lexema derivatum), nexus per formam, lexemata/derivata/
  lineae. `-scribere` = `md/CENSUS.md` (mensura COMMISSA, regenerabilis,
  numquam asserta — corpus mutat); `-citata` = verbatim (code spans) omnia
  'via linea contentum'; porta `probatio_md_census`.
- **`silva.citata(via=None, praefixa=None)`** → Citata(numeri, absentia,
  mota, nuda): citationes in code spans contra arborem (viae: adest / nudum
  = nomen sine directorio, sedes nominatae / motum = via absens, plagula eodem
  nomine alibi / absens = nulla; relativae ad plagulam md quoque) et contra
  `build/nexus.tsv` (symbola minuscula cum '(' scripta: adest / absens);
  `silva.citata_textus(c)` = compendium + absentia per directorium + prima.
  Corpus 2026-09-04: viae MMDCCXXXVI (absunt CCCL, motae CCCLXX), symbola
  MMDCXXXI (absunt CXXXII), 0.6 s. Strepitus notus: nomina PHP/JS in
  worklogs, exempla ('x.worklog.md'), functiones staticae olim deletae.
- **Prosa, additiones post usum primum (2026-09-04)**: `capitulum` /
  `sectio` clavem unam accipiunt ex `titulus` (aequalis), `incipit=`
  (praefixum: `sectio(incipit='9.')`), `continet=` (pars); ambiguitas
  congruentia nominat, absentia praesentia. `paragraphum_addere(sectio,
  textus, ubi='finis'|'initium')` = paragraphus cum separatione recta
  (linea vacua una; ad initium post lineam capituli, setext post
  subductionem: `ProsaExtentum.caput_finis`) - non iam '\n' manu praefixum.
