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
- `silva.porta_umbra(nomen)` — porta in umbra (processus separatus),
  receptum JSON in build/portae/ sigillo arboris ligatum;
  `exspectare(via)`, `portae_pendentes()`, `commissio(portae=[via.json])`
  receptum accipit nisi arbor mutata (rancidum); `receptum_delere(via)`
  post consumptionem
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
