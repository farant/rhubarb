# Mutationes briar

Quid in quaque versione briar mutatum sit, pro scriptoribus plagularum
`.thistle`. Imprimitur per `briar -mutationes`.

Leges chartae:

- Caput supremum `## vN — dies` versionem binarii DAT: `briar -versio`
  eam ex hac charta (in binario infixa) legit - numerus alibi non
  scribitur. Capita numeris stricte descendentibus stant, novissimum
  primum.
- `## inedita` mutationes nondum editas colligit, linea una per
  mutationem, eo tempore quo fiunt. Editio = caput `## inedita` in
  versionem proximam vertere et novum `## inedita` supra ponere.
- Tegit: mores briar ipsius; bibliothecas corporis additas aut remotas;
  mutationes corporis quae plagulas exsistentes frangunt. Mutationes
  corporis minores non hic: linea `corpus:` in `briar -versio`
  commissum exactum nominat.

## inedita

- corpus: glyphae_ductae v3 - litterae Graecae Latinis dissimiles,
  numeri supra et infra scripti, radices, << >> (LXI nova); fons.c
  Graeca Latinis similia in litteras Latinas vertit (antea TOFU).

- corpus: SPATIA arboris (vicus-latera S2a-1) - `Componens.spatium`
  (`componens_ponere_spatium`, `componens_spatium`,
  `componens_invenire_in_spatio`), registra actionum et figurarum cum
  spatio per introitum (`*_miscere_in_spatio`, `*_invenire_in_spatio`),
  `Motus.spatium`; pingere et dispensator intra spatium resolvunt.
  Spatium "" = mores priores. Structurae crevere (rebake).

- corpus: `terminale` nova (applicatio terminalis super aemulator_hospes:
  componens, figura visus, claves per codificator_terminalis, rotula ad
  visum). Nota mandatum.h emendata: color RGBA mandati = pixelum
  tabulae (color_ad_pixelum), non litteralis 0xRRGGBBAA - mores nulli
  mutati.

- corpus: imagines quadrorum (screenshots): `tabula_pixelorum_in_imaginem`
  (tabula_pixelorum.h nunc imago_typus.h includit) et
  `ludus_fenestra_imaginem_scribere` (quadrum ultimum in PNG; etiam sine
  fenestra in tabula nuda); applicationes pictor/scriba/vicus optionem
  `-imago <via>` accipiunt.

- corpus: `aemulator_hospes` nova (hospes emulatoris, aemulator-plan
  B4): nucleus + pseudoterminale + pulsus - effusio infantis in
  nucleum, responsa et initus per caudam (pars responsis reservata),
  magnitudo ad ambos, campana/titulus ad vocantem. Purus.

- corpus: `pseudoterminale` nova (infans in pseudo-terminali,
  aemulator-plan B3): tabula functionum `Pseudoterminale` (legere,
  scribere numquam obstans, amplitudo, finitus, fossa, claudere),
  pons posix (openpty, sessio nova, terminale regens, ambitus, exec
  fractum distinctum ab exitu 127) et pons memoriae (sutura
  probationum).

- corpus: `aemulator` nova (nucleus emulatoris terminalis, aemulator-plan
  A1) - octeti programmatis intrant, schirmum exit: cellulae (UTF-8,
  latitudo angusta/lata/cauda/caput, stilus), cursor cum involutione
  pendente, impressio per runae, CR LF BS HT BEL, mutatio magnitudinis
  (capacitas geometrica), effusio plana ut Ghostty plainString; A2:
  CUP et motus relativi, ED/EL (fundus calami servatur), SGR cum stilis
  internatis (collectio), DECTCEM, 1049, 2026, DECSC/DECRC; B1: regio
  volutionis (DECSTBM) et IND RI NEL SU SD in ea, IL DL ICH DCH ECH,
  sistae tabulationis (HT HTS TBC CHT CBT), LNM; B2: responsa (DA1,
  DA2, DA3, DSR 5, CPR, XTVERSION) per effectum `responsum`, tituli
  OSC 0/2 per effectum `titulus`; `AemulatorConfiguratio` campos
  `titulus` et `versio` accipit (identitas XTVERSION;
  `AEMULATOR_VERSIO`); B4b: DECSTR, `CSI 18 t`, DECRQCRA sub campo
  novo `lectio_schirmi` (ordinarie FALSUM); C: historia (campus
  `historia_octeti`, ordinarie X MB, paginae cum stilis propriis) et
  visus (`aemulator_historia`, `aemulator_visus`,
  `aemulator_visum_movere`, `aemulator_visus_cellula`,
  `aemulator_visus_involuta`, `aemulator_visum_effundere`,
  `aemulator_historiam_effundere`); hospes:
  `aemulator_hospes_visum_movere`, initus acceptus visum ad imum
  reducit; D1: HPR/VPR, REP, SCOSC/SCORC, modi 47/1047/1048, DECALN,
  RIS; D2: modi in tabula (DECRQM), DECOM, IRM, involutio retro
  45/1045, ESC = / ESC >, `aemulator_modi` (AemulatorModi, AemulatorMus,
  AemulatorMusForma) pro hospite; D3: copiae characterum (G0-G3, SO/SI,
  SS2/SS3, DEC Special Graphics). series_terminalis: octeti alti post
  ESC N|O non iam pereunt (FUGA); codificator_terminalis (D6a):
  CodificatorModi mus_forma (X10/UTF-8/urxvt/SGR-pixela; 0 = SGR),
  sagittae_applicationis (DECCKM), lnm, CODIFICATOR_MUS_X10; D6b:
  AemulatorModi.rotula_sagittis (?1007), terminale_titulus,
  TerminaleApplicatio.contextus; terminale modos, murem, rotulam,
  focum, colores honorat; vicus-latera S1a: TerminaleApplicatio.ramus
  (superficies ex ramo); S1b: terminale_montare, terminale_componere
  publica; S1c: ludus_fenestra/ludus_tessera pulsus et pingendum,
  vicus_pulsare (VicusPulsus, VicusFacies.pulsare/vivit_in_fundo,
  VicusTabula.finita), terminale_ambitus, terminale genus vici
  (tabula t1 ordinaria); D7c: AemulatorModi.schirmus_inversus
  (DECSCNM ?5; terminale colores nativos permutat), terminale
  ornamenta (sublineae V, transfixa, superlinea, crassum fictum;
  TerminaleApplicatio.ornamenta_pixelorum; lineae capsarum, quadra,
  braille per glyphae_ductae, contrastus non in graphicis), DECCOLM
  minimum (sub ?40 delet,
  regionem et cursorem restituit, latitudo manet); D7a: DECXCPR, DECDSR, DECID, DECRQSS,
  acervus titulorum, DECSCUSR servatus; codificator legacy Ctrl-[/I/M
  = C0; D6c: fenestra_macos focum/defocum et
  glutinum (Cmd-V) ut eventa; codificator kitty: shift consumptus ->
  textus planus; D4: acervus
  vexillorum clavium kitty
  per schirmum (CSI ? > < = u), AemulatorModi.kitty_vexilla; D5:
  tabula colorum viva (OSC 4/10/11/12, 104/110-112), configuratio
  color_litterae/fundi/cursoris + tabula_colorum, aemulator_color.
  series_terminalis: lexema OSC terminatorem in 'finale' fert. Series
  ceterae
  consumuntur et numerantur. PURUS: nulla I/O, nullum tempus.

- corpus: caput `eventus.h` novum - vocabularium initus commune ex
  fenestra.h divisum (fenestra.h id includit), sine iactura auctum:
  codex physicus W3C (`EventusCodex`), runa logica, actio
  (PRESSA/ITERATA/SOLUTA), latera modificantium, EVENTUS_TEXTUS
  separatus, indicator (id, genus, pressio, exempla), rotula integra
  (dx/dy + genus), depositio, facultates; genera derivata
  TRACTUS_INCIPIT/TRACTUS/TRACTUS_FINIT. FRANGIT: `datum.mus.x/y` et
  `datum.depositio.x/y` nunc s32 (olim i32) - plagula quae ea in i32
  ponit sub -Wsign-conversion castrum explicitum poscit.

- corpus: `eventus_cauda` nova (cauda eventuum fontis: anulus, textus
  per lectionem copiatus, motus coalitus cum exemplis, residuum
  rotulae), `claves_physicae` nova (kVK macOS -> EventusCodex),
  `eventus_conformitas` nova (tabula conformitatis fontium), caput
  `fenestra_tempus.h` novum (horologium fenestrae ex fenestra.h
  divisum; fenestra.h id includit). Purae.

- corpus: plagulae NOTATAE - `EventusNotatum` (eventus + scopus),
  `eventus_notata_scribere_stml` / `_legere_stml`,
  `dispensator_notarium_ponere` (notarius), `destinatio_ad_locale`,
  `manus_ludus_iterare` (iteratio CRUDA aut SEMANTICA cum
  divergentiis).

- corpus (mores): dispensator Tab per clavem LOGICAM - Ctrl+I focum non
  iam movet; retro per MOD_SHIFT (olim bitus 0x1, qui latus Ctrl est).
  `manus_ludus_clavem` modificantes ut fenestra eos fert
  (`MOD_SHIFT | MOD_SHIFT_SINISTER`), non iam 'I' pro Shift.

- corpus: `Eventus.datum.rotula.delta_x/delta_y` (f32) DELETA (spec
  D2 gradus III). FRANGIT: plagula quae eas legit `dx`/`dy` (s32,
  pixela nostra) cum `genus` (GRADATA: gradus = `gradus_rotulae`
  FACULTATUM) legat; plagulae STML veteres delta_* in dx/dy rotundant.
  `ImportatioVisus.gradus_rotulae` novum.

- corpus: `Eventus.datum.clavis.typus` DELETUM -> `s32 producta`
  (character a clave productus sub modificantibus, Unicode plena, 0
  nullus; spec D2 gradus III). FRANGIT: plagula quae `typus` legit
  `producta` legat (comparationes cum literis characterum eaedem);
  plagulae STML veteres attributum `typus` in `producta` legunt.

- corpus (eventus phasis B): fons terminalis - `series_terminalis`
  nova (lexemator DEC/Williams, API trahens), `interpres_terminalis`
  nova (lexemata -> Eventus: legacy, kitty, xterm `CSI 27;m;c~`),
  `rivus_terminalis` nova (pipeline PURA: tradere / mora_ms / moram /
  eventum, lectio coalita, modi declarati, glutinum -> DEPOSITIO),
  `copia_terminalis` nova (OSC 52), `terminalis` (POSIX: modus
  crudus, lectio cum mora, amplitudo) nova, `codificator_terminalis`
  nova (Eventus -> octeti: claves legacy/kitty, mus SGR per Modulum,
  glutinum tutum, focus). `claves_physicae`: `claves_codex_ex_littera`,
  `_ex_kitty`, `claves_littera_ex_codex`. `eventus_cauda`:
  `eventus_caudae_depositionem_impellere`. `manus_ludus_iterare_per`
  (iteratio per traditionem).

- corpus: `eventus.h` - `datum.rotula` x/y/modificantes et
  `EventusFacultates.modificantes_textus` AD FINEM addita (plagulae
  veteres eaedem). `eventus_conformitas_comparare` nunc
  `ConformitasVerdictum` reddit (FRACTA 0, CONFORMIS, EXCUSATA) - olim
  b32; `si (comparare(...))` idem significat, sed EXCUSATA verum est.
  Scaena: `excusationes`, `<terminalis profilum octeti>`.

- corpus: `modulus` nova (modulus strati delineandi) - cellula in
  pixelis nostris, extensio, proportio schirmi (rationalis, reservata);
  columna/linea pavimento, margines proximi (divisio negativa manu,
  C89); `modulus_textum_metiri` per mensorem SCOPI (FONTIS: runa =
  cellula, ut fons_6x8 pingit; RUNARUM: unitates runae). Pura.

- corpus: `tessellatio` nova (Mandata in cellulas terminalis) - textus
  (graphemata integra, latae), rectangula, margines et lineae axiales
  per juncturas (┼ ├ ┬ ...), via pixelorum (imago, polygonum, linea
  obliqua -> quadrans) ut stratum inferius, ordine pictoris; sine
  tessera, sine Cocoa.

- corpus: caput `tabula_pixelorum.h` novum - tabula pixelorum PURA
  (typus, pixela, RGB/RGBA, textus fons_6x8 et mensura) ex fenestra.h
  divisa; fenestra.h id includit (nihil mutatum vocantibus). Rasterizare
  sine fenestra: `delineare_mandata_selecta` (filtrum primitivorum),
  `delineare_mandata_modulus` / `_mensor`.

- corpus: `quadrans` nova (imago in cellulas terminalis) - regio
  imaginis (`Imago`) in cellulas quadrantum (2x2, XVI figurae) aut
  dimidiorum (▀) vertitur: runa + color litterae + color fundi
  (`quadrans_computare`, `quadrans_mensurare`); colores MEDIA (ordinarii)
  aut EXTREMA; `quadrans_aptare` imaginem sine distortione aptat;
  `quadrans_error` errorem reconstructionis metitur. Pura: nulla
  allocatio, mathematica integra.

- corpus: `runae` nova (nucleus Unicode, acervi textus stratum primum;
  `runae_tabulae` tabulae eius generatae ex Unicode 15.1.0) - latitudo
  runae in cellulis terminalis (`runae_latitudo`, regula Ghostty),
  rupturae graphematum UAX #29 (`runae_rumpitur`,
  `runae_graphema_proximum`), latitudo textus et columna
  (`runae_latitudo_textus`, `runae_columnam_quaerere`). Pura: nulla
  allocatio. Sine `#include` derivatur ut quodvis caput domus.

- `manus clavis a` / `manus clavis 7`: littera aut numerus SOLUS (sine
  modificatore) pressio UNA clavis nativa est (keydown + keyup) - olim
  recusatum. Brevitates paginae in `document` ('a', 'n', '1'-'9') nunc
  via nativa probantur (lapide feature-requests/024). Maiuscula sola
  ('A') Shift implicat; textus (chorda) per `scribere` manet.

- `imago_opus`: `imago_creare` (colore uno plena), `imago_excidere`
  (regio SINE scala, ad fontem praecisa), `imago_transcribere` (fons in
  dest ad dx, dy - pixela substituuntur, dx/dy negativi licent),
  `imago_rectangulum` (ambitus crassitudine introrsum) - omnia ad
  margines praecisa (lapide feature-requests/021). `imago_extrahere_et_
  scalare` cum regione EXTRA fontem iam imaginem vacuam reddit (olim I x
  I et lectio extra limites).

- ictus cache celer: extra arborem rhubarb (corpus infixum) clavis ex
  octetis (plagula + bibliothecae) computatur ANTE silvam et fabricam -
  programma iam aedificatum statim currit (textus lapidis IXM
  linearum: 0,75 s -> 0,00 s per cursum). Clavis eadem ac antea (nulla
  aedificatio nova). `BRIAR_VESTIGIUM=1` ictum celerem in stderr nominat.

- stampa corporis infixi = SIGILLUM contentorum (`corpus
  sigillum=...`), non iam `commit=... dies=...`: eadem dum corpus idem
  est, ergo proiecta in `~/.rhubarb/briar/` corporis eiusdem non iam
  orbantur post aedificationem novam (clavis olim omni aedificatione
  mutabatur). Identitas aedificationis: linea `aedificatum:` et
  `-provenientia` (fabrica 1b T6).

- corpus: `stilus_terminalis` nova (modulus 004) - stilus terminalis
  (colores nativus/tabula/RGB litterae, fundi, sublineae; VIII
  ornamenta; sublinea VI generum) <-> SGR: `stilus_codificare`
  (PLENA aut CCLVI; ultra XXIV parametra in series plures divisa),
  `stilus_applicare` (Ghostty sgr.zig; ignota numerata), tabula CCLVI,
  `stilus_quantizare`. tessera per eam emittit - octeti idem.

- corpus: `pictor_applicatio` nova (modulus 013 A4) - compositio
  pictoris communis (volumen, documentum, canones, insulae, registra,
  dispensator) quam principalia fenestrae et terminalis vocant.
  FRANGIT: `pictor_documentum.h` iam non includit `fenestra.h` (solum
  `tabula_pixelorum.h`) - plagula quae fenestram per id accipiebat
  `fenestra.h` ipsa includat.

- corpus (mores): dispensator in MUTARE_MAGNITUDINEM attributa ephemera
  `superficies_latitudo`/`_altitudo` (pixela nostra, scriptor
  "dispensator", ut focus) scribit; ludus_fenestra magnitudinem tabulae
  semel ante eventum aut quadrum primum nuntiat. Applicatio cum canone
  ephemerarum clauso ea declaret (et dominos) - aliter insula tacite
  recusat.

- corpus: `dispositio` nova - dispositio pura in CELLULIS, exemplar
  Clay (apta/crescens/fixa/pars per axem, min/max, spatium,
  intervallum, allineatio, praecisio); radix implicita linea
  superficiei; `dispositio_computare` + `dispositio_fines`; crescens et
  contractio (aequatio Clay; ora quaeque pavimentum orae exactae);
  textus per mensorem (liber primus virtualis, linea una; minimum =
  verbum latissimum).

- corpus (FRANGIT): `PictorCompositio.status_altitudo` (pixela) deletum
  -> `cellula_latitudo/_altitudo` + `status_lineae` (013 B3); pictor
  componit ex `superficies_*` per dispositio in cellulis.

- corpus (mores): fenestra MUTARE_MAGNITUDINEM magnitudinem CONTENTI
  narrat (olim quadri cum titulo); `TabulaPixelorum.capacitas` nova et
  `tabula_pixelorum_ad_fenestram` (scala servata); ludus_fenestra
  tabulam aptat et eventum in pixela nostra rescribit (013 B3b).

- corpus (mores): delineare_mandata primitiva ORIGINE NEGATIVA praecidit
  (olim rectangulum evanescebat, linea et polygonum in aeternum
  pendebant - API delineare i32 insignatum); primitiva in spatio
  positivo octetis eisdem.

- corpus: `historia` nova - cauda actorum cum proiectione (ex
  pictor_documentum extracta, scriba-plan H1): acta in volumine, rami
  post revocationem, checkpoints viva sola, cursor, revocare/reficere,
  verificare; proiectio clientis per `HistoriaProiectio` (memoria
  fixae mensurae + vacare + applicare).

- corpus (FRANGIT): `PictorDocumentum` per `historia` (scriba-plan
  H2): campi `cursor`, `finis`, `numerus_vivorum`, `sigillum` deleti
  (in `doc->historia` vivunt) - per `pictor_documentum_cursor/_finis`
  et `pictor_documentum_numerus_vivorum` (nova) lege. Volumina vetera
  octetis eisdem leguntur et scribuntur.

- corpus: insula-rami T4 - `vicus_applicatio` (nova: compositio
  communis hospitis - genera scriba et pictor, index ordinarius s1/p1,
  dispensator ligatus; `vicus_volumen_aperire`, `vicus_applicatio_
  aedificare`); applicationes `apps/vicus/vicus.sh` (fenestra) et
  `vicus_terminalis.sh`. `manus_ludus`: ictus et tractus bottonem
  sinistrum ferunt (olim nullum - codificator terminalis pressionem
  sine bottone abiciebat), motus nudus nullum.

- corpus (FRANGIT): insula-rami T3b - `vicus_motum_ligare(v, motus)`
  -> `vicus_dispensatorem_ligare(v, d)` (Motum ligat ET destinationem
  hospitis ponit). Actio radicis `vicus.magnitudo` -> `vicus.radix`;
  `vicus.tabula` (nova, ictus in tabulam). Ctrl-A praefixum: n / p /
  1-9, Ctrl-A iterum = tabula prior; canon ephemera vici: `praefixum`,
  `prior`.

- corpus: insula-rami T3a - `Motus.ramus` (novum; nullatum = radix,
  mores applicationum solarum idem): focus dispensatoris et
  `motus_effundere` per ramum activum. `pictor_montare` /
  `scriba_montare` focum ordinarium in ramo scribunt. `vicus_motum_
  ligare` (nova); `VicusFacies.gestum_ponere/_ctx` (nova; facies ante
  describere nullatur).

- corpus: vicus T2b - `vicus_componere` (nova; Componere-formata, ctx =
  Vicus*): radix `vicus` cum actione `vicus.magnitudo`, linea tabularum
  (`PARTES_INDEX`, VIII pixela), arbor activae infra eam translata.
  Superficies ramorum (fenestra minus linea) ab hospite scribuntur in
  aperire et mutatione magnitudinis. `VICUS_CELLULA_LATITUDO/_ALTITUDO`,
  `VICUS_ALTITUDO_TABULARUM` (nova).

- corpus (FRANGIT): vicus T2a - `vicus_genus_addere` functionem
  `describere` (VicusFacies) accipit; `vicus_actiones`, `vicus_figurae`,
  `vicus_imago_fons` (nova). `actio_registrum_vacare/_miscere`,
  `figura_registrum_vacare/_miscere` (nova; collisio = nihil additur).

- corpus: `vicus` nova (insula-rami-plan T1b) - hospes applicationum:
  repositorium unum (radices `<vicus>`), volumen unum, index tabularum
  et activa in plagula `vicus/tabulae`; genera a principali registrata
  (`vicus_genus_addere`: mensura montationis + functio montandi);
  `vicus_aperire`, `vicus_tabulam_addere`, `vicus_activam_ponere`.

- corpus (FRANGIT): MONTATIO (insula-rami-plan T1a) - `pictor_montare`,
  `scriba_montare`, `PictorMontatio`, `ScribaMontatio` (nova);
  `insula_ramum_initiare` (nova). `PictorApplicatio` /
  `ScribaApplicatio`: contextus in `montatio` (olim `actiones_ctx`,
  `figurae_ctx`, `compositio` directe) - `&app.montatio.figurae_ctx`.

- corpus (mores): historia LOCUM REVOCANDI servat (insula-rami-plan
  R5) - revocare/reficere notam `<cursor ad="seq"/>` appendunt;
  aperire ad notam ultimam non obsoletam redit (olim semper ad finem).
  Volumina nova acta plura ferunt (seq posteriores moventur).

- corpus (FRANGIT): scriba in RAMO (insula-rami-plan R4) - canones
  scribae radicem `scriba` (cum `id`) nominant (olim `documentum` /
  `ephemera`). `ScribaActiones.ramus`, `ScribaCompositio.ramus` (nova;
  repo NIHIL = radix repositorii dati).

- corpus (FRANGIT): pictor in RAMO (insula-rami-plan R3) - canones
  pictoris radicem `pictor` (cum `id`) nominant (olim `documentum` /
  `ephemera`): insulae pictoris `<pictor …>` radicem habeant.
  `PictorActiones.ramus`, `PictorCompositio.ramus` (nova; repo NIHIL =
  radix repositorii dati).

- corpus (FRANGIT): SPATIUM NOMINUM (insula-rami-plan R2) -
  `historia_creare/_aperire`, `pictor_documentum_creare/_aperire`,
  `scriba_documentum_creare/_aperire` parametrum `spatium` post volumen
  accipiunt ("" = nomina nuda, octeti veteres). Documenta plura in
  volumine uno: genus, notae, checkpoints et manifestum praefixum
  'spatium/' ferunt.

- corpus: insula RAMOS habet (insula-rami-plan R1) - `InsulaRamus`
  (repositorium + liberum radicis per elementum et id), `insula_ramus`,
  `insula_ramus_radix`, `_nodus`, `_attributum`, `mutare_ramum`,
  `insula_ramus_dominum_ponere`, `_dominos_legere`, `_canonem_ponere`.
  `InsulaDominus` et `InsulaRepositorium` creverunt (rebake). Canon
  radicis ramos canonem proprium habentes non videt.

- corpus (mores): margo paginae (scriba, pictor) in cellulis
  MARGINIS - cellula tota extra paginam, non I pixelum (tessellatio
  margo I pixeli in cellulas paginae rotundabat). `PictorFigurae`
  `cellula_latitudo/_altitudo` (nova; 0 = I pixelum ut olim).

- corpus: `scriba_applicatio` nova (scriba-plan S3) - compositio
  scribae communis (volumen, documentum, canones, insulae, registra,
  dispensator, gestus) quam principalia fenestrae et terminalis vocant
  (`apps/scriba/scriba{,_terminalis}.sh`, AEDIFICARE_SOLUM=1 struit
  solum).

- corpus: `scriba_componentia` et `scriba_figurae` novae - arbor
  scribae (prospectus/pagina/status in cellulis; cursor et ancora in
  puncta, modus in titulo; volutio sine statu cursorem centrans) et
  figurae (mensa, charta cum margine, selectio linearum, cursor colore
  status modi, linea status).

- corpus: `scriba_actiones` nova - actio `pagina.clavis` (vim super
  folium laboris in gestu; status vim in insula ephemerarum; servatio
  statim in modo normali, post quietem in inserendo, Esc claudit;
  frusta insertionis coniuncta). `scriba_effugere`/`scriba_solvere` et
  `scriba_documentum_committere_coniunctum` (nova).

- corpus (mores): destinatio TEXTUM commissum ad FOCUM mittit (olim ad
  radicem); `manus_ludus_scribere` (nova) - clavis + textus ut fons
  verus; `motus_gestum_effundere` scriptorem priorem restituit (olim
  anonymum).

- corpus: `historia_actum_coniunctum` (nova) - actum priori
  coniunctum (nota historiae `<coniunctio/>` ante id); revocare et
  reficere gregem coniunctum gradu uno transeunt. Clientes sine
  coniunctione octetis eisdem.

- corpus: Motus GESTUM applicationis habet (`MotusGestus gestus`:
  status opacus, effusor, quies propria) - `motus_gestum_ponere`,
  `mutare_gestum`, `motus_gestus_quies`, `motus_gestum_effundere`;
  dispensator gestum in quiete sua effundit; `dispensator_finire`
  (nova) pendentia ante exitum effundit - glutina fenestrae et
  terminalis eam vocant. Structura Motus crevit (rebake).

- corpus: `scriba_documentum` nova - folium textus (TabulaCharacterum)
  per historia: actum `<mutatio linea deletae><linea indentatio
  textus/>...</mutatio>` = lineae substitutae (effectus, non claves);
  `scriba_mutatio_computare/_applicare` purae; `_committere` folium
  laboris in caudam; revocare/reficere/verificare.

- corpus (mores): `ludus_fenestra_currere` otiosa EXSPECTAT
  (`fenestra_expectare_eventus`, mora `ludus_fenestra_mora` nova: 0
  ante quadrum primum, deinde quies dispensatoris) - olim perscrutabatur
  sine mora (CPU C%); eventus statim excitat.

- corpus (mores): tessellatio ORDINEM PICTORIS servat - rectangulum,
  textus, linea ANTE imaginem posita sub ea manet (tabula `operta`: per
  cellulam index imaginis ultimae tegentis). Olim stratum pixelorum
  semper sub transitu cellularum iacebat: mensa ante paginam picta
  paginam totam celabat.

- corpus: `COLOR_SUPERFICIES` nova (thema: mensa circa paginam in
  prospectu) et `PARTES_PROSPECTUS` nova (componens: fenestra in
  contentum; titulus STML "prospectus"); pictor prospectum mensa implet
  et paginam margine cingit.

## v4 — 2026-09-29

FRANGIT: `lib/toml.c` vetus remotum - plagulae `toml_capere_*` aut
`toml_error` utentes ad `toml` novum migrent (tabula infra). Nova:
plagulae inter se codicem communicant per `<bibliotheca via="x.thistle"/>`
(lapide feature-requests/015); TOML 1.0 integer (feature-requests/013);
vexilla atrii programmatibus vitreis praemittuntur (feature-requests/023);
adiutores probationis sine `staticus` (documentation-ideas/016);
descriptiones bibliothecarum (documentation-ideas/017,
feature-requests/016); directivae condicionales in regione C.

- FRANGIT: `toml` novum - cliens materiae TOML 1.0 integer
  (`toml/fontes/toml.h`), `lib/toml.c` vetus remotum. Vetera -> nova:
  `toml_legere(t, p)` -> `toml_legere(t, via, p)`; `toml_error(doc)` ->
  `toml_diagnostica_scribere(p, doc, VERUM)` (omnia vitia cum
  `via:linea:columna` et caret); `toml_capere_chorda(doc, k)` ->
  `toml_chorda(doc, k, &c)` (FALSUM si abest - ordinarium tuum
  explicitum scribe); `toml_capere_numerum` -> `toml_integer` (s64);
  `toml_capere_boolean` -> `toml_boolean`; `toml_capere_tabulatum` ->
  `toml_chordae(doc, k, p, &xar)`. Novum: `[tabulae]` servantur (clavis
  punctata `toml_chorda(doc, "llama-server.versio", &c)`),
  enumeratio (`toml_tabulae_*`, `toml_seriei_*`), clavis iterata
  refutatur nominata cum sede priore, fluitantes, tempora, tabulae
  inlineae, series tabularum (lapide feature-requests/013). Sine
  `#include`: `toml.h` ex usu derivatur ut quodvis caput domus.
- corpus: radices clientium materiae (`materia/fontes`, `toml/fontes`)
  in corpore infixo - `-bibliothecae` eas enumerat, `-bibliotheca toml
  -functiones` signaturas dat, `-amalgama` eas complectitur; omne
  proiectum genitum eas vendit (instrumentum capsulae toml trahit).
- `-bibliothecae`: XXV descriptiones novae cum verbis Anglicis
  (`tabula_dispersa` = hash table, `internamentum` = string interning,
  `piscina` = arena, `xar` = vector segmentatus, ...); sine descriptione
  CXIII -> LXXXVIII (lapide documentation-ideas/017). `-dialectus`:
  laqueus `atoi(c.datum)` -> `chorda_ut_s32` / `chorda_ut_s64`, quae
  iam exsistunt et nunc documentantur (feature-requests/016).
- regio `munus="probatio"`: prototypi adiutorum generantur ut in regione
  principali - adiutor sine `staticus` non iam frangit
  `-Wmissing-prototypes`, et ordo definitionum liber est (lapide
  documentation-ideas/016). `-dialectus` regulam `staticus` nominat.
  Cache: fontes briar ipsius clavem intrant - briar mutatus proiectum
  novum aedificat etiam sine commisso novo.
- vexilla atrii ante plagulam (`-vivum`, `-retro`, `-portus <n>`,
  `-radix <via>`) programmati vitreo praemittuntur: `manus incipere
  briar x.thistle` nunc currit (lapide feature-requests/023); olim
  `vexillum ignotum: -vivum`.
- directivae condicionales in regione C: coniunctio `#ifndef X` /
  `#define X` / typus / `#endif` iam aedificatur (olim directivae eius
  bis in caput genitum emittebantur - `#define` primum copiam typi
  celabat: 'unknown type name'). Coniunctio directivarum sola ordine
  fontis manet; `#ifdef` intra corpus functionis in corpore manet
  (parcum VF42V).
- `<bibliotheca via="textus.thistle"/>` (elementum, columna 0): regiones
  C PLANAE plagulae alterius (functiones, typi) in omni regione C
  praesto - `principale`, regiones ceterae, `-probatio` - sine
  `#include`, derivatione ut caput domus (lapide feature-requests/015).
  Transitiva; `via` contra directorium PLAGULAE (non cwd); `interior` /
  `staticus` privata manent; `principale`, probatio, `methodus`,
  fragmenta membri numquam praebentur. Refutationes cum sede: circulus
  (cum remedio), nomen publicum bis, titulus iteratus, plagula absens.
  Cache: octeti membrorum omnium clavem intrant (membro SOLO mutato
  aedificatur). `-amalgama` membra complectitur (statica cuiusque
  renominata; plagula una clang sola compilat); `-partes` lineam
  `bibliotheca:` per membrum dat (membra attacta, nomina publica).

## v3 — 2026-09-28

FRANGIT: `MMMM` et `MMMMXCVI` remota - plagulae eis utentes `IV * M`
et `IV * MXXIV` scribant (lapide `loca.thistle` ter). Cetera: http
sine truncatione tacita, nuntii mortis latina.h plenius, et quae
lapide invenit (bugs/012, 013, 015, 016, 018; documentation-ideas/014).

- `http` (cliens, HTTPS): corpora supra ~IV KB integra redeunt (olim
  truncata cum `successus`: tls_recipere lectionem partialem ut
  finem reddebat) - lapide bugs/015, bugs/016. Corpus brevius quam
  `Content-Length` promisit, aut chunked sine fragmento terminali,
  nunc ERROR est (`HTTP_ERROR_IO`, "Corpus truncatum: Content-Length
  N, recepti M"), numquam successus cum corpore partiali.
- `-h` linea prima = linea prima `-versio` (cum `+inedita(n)`;
  olim `vN` solum) - bugs/013.
- `-dialectus`: series numerorum non integra regulam dicit (`omnes
  0-N; supra selecti tantum`) pro `(344)` qui integram simulabat -
  documentation-ideas/014 (latina.h ipsa nunc integra, infra).
- Nuntius mortis: admonitio latina.h etiam cum verbum latinae ante
  lexema mortis stat (`s32 nomen = 1` moritur in `=`: "'nomen'
  macrum latina.h est") - bugs/018.
- `via_nomen` / `via_directorium` (corpus): separatores terminales ut
  POSIX - `"/foo/bar/"` -> `"bar"` + `"/foo"` (olim `""` +
  `"/foo/bar"`); `"/"` -> nomen `""` consulto - bugs/012.
- **FRANGIT** latina.h (corpus): numeri Romani OMNES `ZEPHYRUM`-
  `MMMCMXCIX` definiti (olim 344 selecti; `CCCXIX` nunc exstat), sectio
  generata et ordinata. `MMMM` et `MMMMXCVI` REMOTA (numerus classicus
  ad 3999 finit): scribe `IV * M` et `IV * MXXIV` - in corpore macro
  `(IV * MXXIV)` parenthesibus. `-dialectus`: `(4000)`, sine regula.

## v2 — 2026-09-26

Regiones C per silvam novam (silva in materiam migrata, phasis V): quae
lapide invenit (bugs/001, 009, 010) sanata.

- `--help` et `--version` synonyma sunt `-h` et `-versio` (ante
  plagulam solum; post eam argumenta programmatis manent); nota
  vexilli ignoti `-h` nominat.
- `-versio` lineam `aedificatum:` addit (tempus, sigillum fontium briar,
  commissum; SORDIDUM si fontes mutationes non commissas ferunt) - duo
  binaria eiusdem versionis nunc discernuntur. Mutationes ineditae in
  charta: linea prima `briar vN+inedita(n) — dies`.
- Regiones C: `va_arg(va, T)`, `offsetof(T, m)`, `va_start`, `va_end`,
  `assert` parsantur (antea `regio C: parsura fracta` - bugs/009);
  typi systematis (`FILE`, `size_t`...) noti. Transitus silvae
  lexicon systematis habet, ut examen.
- `__attribute__((...))` accipitur in omni sede (post prototypum, ante
  declarationem, post `}` structurae) et ad clang transit, qui eam
  tractat (`sentinel`, `format`, `unused`...) - bugs/010.
- Errores syntaxis in regione C: linea et columna VERAE in plagula
  `.thistle` (antea linea prima regionis semper), ordo
  `plagula:linea:columna` sub summario cum excerpto ('hic coepit' ubi
  unitas incipit, 'hic exspectatur' ubi parsura periit); `(1 error)`
  singulare; `-visio` excerptum in `<pre>` servat - bugs/001.
- Nuntii errorum causam dicunt: lexema inventum nominatur (`lexema
  'redde' quod grammatica hic non accipit`); verba `latina.h` ut nomina
  usa admonentur (`'nomen' macrum latina.h est ('typedef'): nomen aliud
  elige`, etiam `casus`, `C`...); `';'`, `'}'`, `')'` fortasse deest
  suggeritur ubi status parsurae id exspectabat. Errores plures in
  unitate una: primus solus (limes notus).

## v1 — 2026-09-24

Versio prima numerata: status omnium quae ad hunc diem exstant (antea
"v0" sine charta). Historia accuratior in git et in
`briar/fontes/briar.worklog.md`.

- Plagulae litteratae C89 (`.thistle`): regiones C, regio probationis,
  fragmenta contexta, `#line` verum ad lineam plagulae; forma
  `#!/usr/bin/env briar` cum argumento reservato uno post plagulam.
- Currere: aedificare si abest, deinde fieri; `-probatio`; `-struere`
  [`-iterum`]. Clavis aedificationis = corpus + vexilla + octeti.
- Corpus domus infixum (`include/`, `lib/`, fontes venditi): `-f
  <radix>` > ascensus ex directorio currenti > corpus infixum.
- `-arbor` (proiectio STML), `-partes` (clausura), `-amalgama`
  (plagula una quam clang SOLA compilat - effugium), `-html` (pagina
  litterata), `-visio` (spectator), `-app` (fasciculus `.app`, `-icon
  <via>`).
- Documentatio corporis: `-bibliothecae` (bibliothecae cum
  descriptione), `-bibliotheca <nomen>` [`-fons` | `-functiones`],
  `-dialectus` (charta dialecti: typi, vexilla, verba latina.h,
  laquei C89).
- `-versio` (versio, stampa corporis, sigilla vexillorum) et
  `-mutationes` (haec charta).
