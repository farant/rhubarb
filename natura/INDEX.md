# NATURA — INDEX GENERUM

**GENERATUM** a `tools/natura_visus.sh` — noli manu emendare.
Regenera: `./tools/natura_visus.sh` (idem cursus qui portam custodit).

Exemplaria **33** · genera **166** · res dictionarii **360** · arcus **496**

Cur haec exsistat, cui serviat, quomodo laboremus: `natura/RATIO.md`.
Forma ipsa (elementa, attributa, regulae): `natura/METAMODULUS.md`.
Historia et doctrina: `natura/natura.worklog.md` (LEGE PRIMUM).

## I. Quaestio usitatissima: an genus iam exsistat?

```
grep -i '<terminus>' natura/INDEX.md      # genus aut res
grep -n 'nomen="<genus>"' natura/*.stml   # sedes definitionis
./tools/natura_quaere.sh <terminus>        # QUAERE (definitiones+species)
./tools/natura_quaere.sh -a mod.genus      # APPARATUS: quid rogare debeas
```

## II. Genera per exemplar

Signa apparatus — quid genus applicanti PRAESTET (RATIO §3):
`⚙` machinam statuum fert · `▣N` partes N declaratas fert
(partes = index quo ABSENTIA visibilis fit).

### acta (v2, genera 4)

- **acta** ⚙ ▣5 — Series eventuum ordinata cui SOLUM APPENDITUR, et quae rerum descriptarum VERITAS est: status quilibet (summa, manifes
- **punctum_actorum** — Locus in actis nominatus et allocutione dignus, ita ut interrogatio "qualia erant HIC?" responsum habeat quod homo pet
- **attestatio** ⚙ — Exemplar authenticum ex actis auctoritativis sumptum, quod quid in actis steterit MOMENTO EMISSIONIS testatur, et trad
- **radix_fiduciae** ⚙ — Terminus catenae attestationum, cui fides non ex alia attestatione venit sed ex DECRETO eius qui catenam adhibet: quod

### actus (v1, genera 6)

- **actus_sollemnis** ⚙ — Actus cuius EFFECTUS a forma praescripta et ab agentis capacitate pendet, et qui ideo VALIDUS aut IRRITUS est, non bon
- **ritus** ⊂ actus_sollemnis — Actus sollemnis ad cultum divinum ordinatus: materia sensibilis et forma verborum, per ministrum, ad effectum spiritua
- **sacramentum** ⊂ ritus — Signum efficax gratiae, a Christo institutum: effectum ponit EX OPERE OPERATO - ex actu ipso rite posito, non ex digni
- **sacramentale** ⊂ ritus — Ab Ecclesia institutum, non a Christo: effectum ponit EX OPERE OPERANTIS - ex dispositione agentis et Ecclesiae precib
- **actus_iuridicus** ⊂ actus_sollemnis — Actus sollemnis cuius vim auctoritas civilis praestat: quod lex formam praescribit, lex effectum agnoscit
- **quorum** — Numerus (aut pars) membrorum cuius praesentia aut consensus requiritur ut coetus valide agat: infra eum acta non fiunt

### animal (v0, genera 1)

- **animal** ⊂ corporalia.vivens ⚙ ▣3 — SENSUM habet et motum localem: mundum percipit et se ad eum movet - unde appetitus, unde fuga, unde quaesitio

### architectura_machinae (v2, genera 3)

- **architectura_machinae** — Forma abstracta machinae computatoriae quam programmata alloquuntur: copia instructionum, registra, modellum memoriae
- **versio_architecturae** ⊂ versio.versio — Editio architecturae nominata: quam copiam instructionum chip vere exsequitur et compilator vere alloquitur - "aedific
- **processorium** — Exemplar productum circuli computantis: forma nominata quam officina in assulas innumeras imprimit, quae architecturam

### causa (v0, genera 1)

- **causa** — Id cuius vi aliquid est aut fit: non quod praecedit tantum (id antecedens est) sed quod EFFICIT

### cliens (v1, genera 9)

- **cliens** — Programma quod ministerium alienum consumit, pro utente agens: dimidium petens colloquii
- **cliens_telae** ⊂ cliens — In navigatro vivit: institutione caret - inscriptio URL ipsa distributio est
- **cliens_nativus** ⊂ cliens — In systemate ipso vivit: institutione indiget, plenitudine praemiatur - plagulae, processus, fenestrae, omnia systemat
- **cliens_terminalis** ⊂ cliens — In terminali vivit, textu solo pingit: quo terminale pervenit, is pervenit - trans SSH in machinas remotas sine mutati
- **cliens_api** ⊂ cliens — Machina machinae, sine facie humana: interfaciem programmaticam consumit
- **cliens_mcp** ⊂ cliens — Cliens cuius UTENS agens artificialis est: ministeria per protocollum instrumentorum consumit, agens pro homine per ag
- **cliens_cli** ⊂ cliens — Instrumentum imperii unius: vocatur, agit, exit
- **cliens_mobilis** ⊂ cliens — In telephono vivit: tactus pro mure, interruptiones pro sessione, pila pro fine - vincula eius differentia sunt
- **servus** — Programma quod ministerium aliis praebet: dimidium RESPONDENS colloquii

### commercium (v3, genera 9)

- **moneta** — Genus pecuniae ab auctoritate emissum et receptum: unitas mensurae valoris - sed unitas cuius valor ipse MUTATUR (metr
- **pecunia** — Quantitas in moneta: numerus CUM MONETA - "3,50" nihil est donec moneta adsit (eadem forma qua quantitas unitatem posc
- **possessio** — Relatio inter personam (aut organizationem) et rem: quis quid habeat
- **pretium** — Pecunia pro re petita: relatio TEMPORE VARIANS inter rem et pecuniam - non proprietas rei
- **venditio** — Eventus quo possessio rei transit et pecunia contrarie fluit: mutatio DUPLEX simul - quod ratio duplex (Pacioli) ante 
- **tessera** — Res cuius EXHIBITIO ius constituit, non probat: differentia ab apocha vulgari tota hic est - apocha te solvisse TESTAT
- **tessera_negotiabilis** ⊂ tessera — Accipiens bona fide et pretio dato titulum PURUM accipit, etiam si tradens ipse ius non habuit: EXCEPTIO ad regulam 'n
- **tessera_simplex** ⊂ tessera — Possessio sufficit ad exhibendum, sed nulla titulorum purgatio: qui tesseram furatam exhibet admittitur (custos nescit
- **ratio_rationaria** ⊂ acta.acta — Acta rerum pecuniariarum ordine temporis: veritas est cauda eventuum, status omnis (summa, debitum, lucrum) PLICATURA

### communicatio (v1, genera 3)

- **actor** — Ens logicum cum identitate, statu privato, moribus: cum aliis SOLUM per nuntios communicat
- **nuntius** ⚙ — Communicatio reificata: data in itinere INSCRIPTA - destinatarius ei debetur
- **eventum** — Res gesta NON inscripta: accidit, et quivis observans videre potest - nulli debetur

### corporalia (v3, genera 7)

- **res_corporea** ⚙ — Quod materiam habet, locum occupat, tempore corrumpitur: res quae manibus tangi potest
- **artefactum** ⊂ res_corporea ▣1 — Ab homine facta ad finem: NUMERABILIS, singularis, haecceitatem habens - hic panis, non alius, etiamsi gemellus adsit
- **materia** ⊂ res_corporea — MENSURABILIS non numerabilis: farina, aqua, oleum - "tres farinae" nihil significat
- **instrumentum** ⊂ artefactum ⚙ — Ad faciendum factum, non ad consumendum: usu SERVATUR (immo usus eius finis est), non destruitur
- **res_sacra** ⊂ res_corporea ⚙ — Divino cultui per ACTUM dedicata, non per naturam nec per materiam: idem lignum mensa aut altare esse potest, et diffe
- **damnum** ⚙ — Laesio integritatis rei corporeae ab externa vi illata: quod unum erat divisum, quod integrum fractum, quod purum mixt
- **vivens** ⊂ res_corporea ⚙ — Se ipsum movet, nutrit, generat: principium motus INTRA se habet, ubi artefactum motum ab alio accipit et materia null

### elementa_programmatis (v4, genera 18)

- **status** — Valor custoditus qui in tempore mutari potest: inter eventus manet, ab eventibus mutatur
- **proiectio** ⊂ status — Ex alio statu (VERITATE sua) integre recomputari potest
- **artefactum_aedificationis** ⊂ proiectio — Proiectio fontium per aedificationem facta: fons veritas, artefactum proiectio
- **promptuarium** ⊂ proiectio ⚙ — Proiectio ad celeritatem servata: valor semel computatus ne iterum computetur
- **index** ⊂ proiectio — Proiectio ad quaerendum ordinata: veritas ordinem quem quaerens petit non habet; index eum praebet
- **lectio** — Eventus quo status observatur, ipso immutato: dimidium alterum vitae status - mutatione scribitur, lectione fructum fe
- **mutatio** — Eventus quo status mutatur: quod ACTA recordant
- **functio** ⚙ — Subprogramma nominatum: parametra accipit, computat, fortasse valorem reddit, fortasse mutationes efficit, fortasse le
- **functio_pura** ⊂ functio — Criterium UNUM in geminos applicatum: lectiones solum per parametra (memoria monstrata inclusa - vide strlen), mutatio
- **constans** — Valor cuius INTENTIO est numquam post ortum mutari - contrarium status: ille mutari exspectatur, hic mutari vetatur
- **constans_numerica** ⊂ constans — Quantitas: arithmetica in ea licet
- **constans_textualis** ⊂ constans — Series characterum constans, plerumque homini destinata: nuntia, tituli, formae
- **bibliotheca** ▣4 — Collectio elementorum (functionum, constantium, typorum) ad usum repetendum edita: interfaciem praebet, corpus celat
- **assertio** — Propositio exsecutabilis: affirmatio de statu programmatis in puncto, quae oraculum suum secum fert - programma ipsum 
- **commentarium** — Pars fontis quam lingua legere NON promittit: textus intra artefactum, lectoribus aliis quam compilatori inscriptus
- **identificator** — Nomen cuius essentia REFERENTIA est, non significatio: ansa ad rem
- **programma** — Collectio elementorum cum INITIO PROPRIO: vocatum currit et ex se ipso pergit, ubi bibliotheca vocata solum vivit
- **instrumentum_aedificationis** ⊂ programma — Programma cuius finis artefacta aedificationis ex fontibus derivare: graphum dependentiarum novit (quid ex quo), et in

### fabricatio (v5, genera 8)

- **postulatum** ⚙ — Intentio declarata ANTE opus: quod verum esse debet ut opus perfectum dicatur
- **facultas** — Quod opus praestare potest, ab utente aestimatum: unitas VALORIS
- **probatio** — Interrogatio exsecutabilis intentionis: scaena parata, actio exercita, effectus contra exspectationem collatus
- **vitium** ⚙ — Discrepantia inter intentionem et mores: programma facit quod scriptum est, non quod volitum - vitium in hiatu vivit
- **debitum_technicum** ⚙ — Imperfectio in opere CONSULTO suscepta ut aliquid citius praestetur, cum cognitione pretii posterioris: non defectus s
- **complicatio** ⚙ — Consideratio implementationis quae consilio praevideri NON POTEST sed opere incepto sola apparet, et consilium retract
- **consilium** ⚙ — Ordo operum propositus ad finem SEMEL petendum: quid quo ordine fiet, quid cui committitur, quid ante quid stare debet
- **depravatio** ⚙ — Status rei factus alius quam esse debuit, sive quis animadvertat sive non: non defectus faciendi (id vitium est) nec d

### formae_datorum (v0, genera 1)

- **forma_datorum** — Figura ordinandi data ab OPERATIONIBUS suis definita, non ab impletione: quae rogare licet, quid promittitur, quo pret

### identificatio (v1, genera 1)

- **schema_identificandi** ⚙ ▣3 — Ratio instituta nomina rebus assignandi, ita ut nomen unum rem unam designet intra ambitum suum: structura definita, a

### iudicium (v4, genera 8)

- **gradus_assensus** — Quanto firmitate animus propositioni adhaeret
- **fons** — Id ex quo propositio accepta est: opus, mensura, testis, corpus institutionis
- **observatio** — Notitia rei singularis accepta, quae iudicium fundare potest: quid videatur, quid sentiatur, quid metiatur
- **iudicium** ⚙ — Propositio de re quam aliquis tenet: cum auctore, momento, fundamento, gradu assensus
- **periculum** — Damnum quod accidere potest nec certum est: species iudicii de futuro, sed DUPLEX - probabilitate et gravitate simul c
- **ignotum** ⚙ — Absentia notitiae AGNITA et nominata: hiatus qui, quia nominatus est, tractari potest
- **investigatio** ⚙ — Inquisitio ordinata in causas eventus, iudicia de causis producens
- **secretum** ⚙ — Res cuius utilitas in eo consistit quod pauci eam sciunt: divulgata non corrumpitur nec perit sed OFFICIO SUO EXUITUR,

### lingua_programmandi (v4, genera 5)

- **lingua_programmandi** ⚙ — Systema signorum formale ad computationes exprimendas: grammatica definita, semantica praescripta (plus minusve strict
- **lingua_iussuum** ⊂ lingua_programmandi — Verbum primum = PROCESSUS ALIENUS vocatus, non functio propria: lingua ad imperia oritur (interactiva), scriptilis fit
- **versio_linguae** ⊂ versio.versio — Editio linguae nominata et citabilis: eventus "edi" in rem conversus
- **implementatio_linguae** — Programma quod linguam exsequitur: compilator, interpres, aut mixtum
- **verbum_clavis** — Verbum quod lingua sibi vindicat: grammaticae servit, identificatori negatur

### locus (v0, genera 4)

- **locus** — Pars spatii determinata ubi res corporeae esse possunt: punctum aut area in terra
- **inscriptio** — Nomen loci ad inveniendum conventum: via, numerus, urbs, codex postalis
- **iurisdictio** — Potestas legum ferendarum et exigendarum super loco: municipium, provincia, civitas
- **aedificium** ▣1 — Structura loco fixa quam homines occupant: res corporea quae moveri non potest - immobilitas essentia eius, unde "res 

### mensura (v0, genera 3)

- **magnitudo_mensurabilis** — Id quod mensurari potest: longitudo, massa, tempus, temperatura
- **unitas_mensurae** — Partitio conventa magnitudinis mensurabilis, qua quantitates numerari possunt
- **quantitas** — Numerus CUM UNITATE: mensura rei determinata

### morbus (v1, genera 1)

- **morbus** ⚙ — Dispositio viventis praeter naturam, qua operatio debita impeditur: defectus ab eo quod naturae rei debetur

### norma (v4, genera 4)

- **norma** ⚙ — Documentum publicum quod formam praescribit: quid conformitas significet definit
- **lex** ⚙ — Ordinatio rationis ad bonum commune, ab eo qui curam communitatis habet promulgata (Thomas, ST I-II q
- **classis_normativa** ⚙ — Categoria a norma constituta in quam res assignantur, cui consequentiae (obligationes, prohibitiones, procedurae) ADHA
- **fictio_iuris** — Praeceptum quo lex rem aliter se habentem TAMQUAM ALITER tractari iubet, sciens ita non esse, propter bonum quod alite

### obligatio (v6, genera 10)

- **obligatio** ⚙ — Iuris vinculum quo quis alicui aliquid praestare adstringitur (Institutiones): non res sed VINCULUM inter personas - d
- **contractus** — Conventio duorum (aut plurium) quae obligationes parit: consensus est fundamentum - ideo utrumque ligat, et neuter sol
- **licentia** ⚙ — Concessio ab auctoritate data ut quid liceat quod aliter non liceret: UNILATERALIS (consensu accipientis non indiget),
- **tributum** ⊂ obligatio — Obligatio ad MAGISTRATUM, ex lege orta, non ex consensu: debetur quia lex ita statuit, non quia quis promisit
- **titulus** — Attestatio publica et permanens ab auctoritate data, quod aliquis aliquid consecutus est aut aliquid EST: non permissi
- **iussum** ⚙ — Directivum ab eo qui potestatem habet latum, quod agendum quid, a quo, et intra quod tempus determinat, et quod ACTU L
- **petitio** ⚙ — Rogatio ad alium directa ut aliquid faciat aut praestet, quam ille CONCEDERE AUT NEGARE potest sine ulla violatione: q
- **delegatio** ⚙ — Potestas alterius nomine agendi, ita ut acta delegati delegantem OBLIGENT tamquam ipse egisset: non licentia agendi se
- **tractatus** ⊂ contractus ⚙ — Conventio inter eos qui superiorem non habent: nullum tribunal supra partes est, unde consensus non semel datur sed BI
- **limitatio** — Declaratio unilateralis qua pars, tractatum accipiens, effectum quorundam articulorum in se excludit aut mutat: consen

### operatio (v1, genera 4)

- **ambitus** — Locus ordinatus ubi programmata currunt: machinae, figurationes, data, nomina
- **pensum** ⚙ — Opus certum alicui commissum: quid agendum, a quo, quo statu
- **incidens** ⚙ — Interruptio aut deminutio ministerii in ambitu vero: non genus vitii sed vitium EXPOSITUM - idem defectus vitium sempe
- **processus** ⚙ ▣5 — Programma in exsecutione: instantia formae, statum proprium gerens, dum vivit allocutione digna

### opus_scriptum (v1, genera 6)

- **opus_scriptum** ⚙ — Creatio intellectualis verbis constans, ab expressionibus suis distincta: "Moby-Dick" opus unum est sive Anglice sive 
- **expressio** — Realizatio operis in signis certis: textus hic, non alius
- **editio** — Corpus editum expressionis: quod praelum vere protulit - charta, forma, paginae, index ISBN
- **codex** ⚙ — Exemplar unum corporeum editionis: hoc quod manu tenes, cum maculis, notis marginalibus, historia possessionis propria
- **periodicum** ⊂ opus_scriptum — Opus quod PERFECTIONEM non petit: in fasciculis continuatur dum curatores volunt - machina compositionis parentis ei f
- **fasciculus** — Pars periodici tempore signata: numerus unus, die suo editus

### organizatio (v6, genera 6)

- **organizatio** — Universitas personarum: coetus in finem communem ordinatus qui UT UNUM agit et mutatis membris perdurat - haec perdura
- **persona_iuridica** ⊂ organizatio — Subiectum iurium et obligationum in iure: possidet, contrahit, agit et convenitur SUO nomine, non nomine membrorum
- **officium** ⚙ — Munus stabiliter constitutum cum oneribus, potestatibus et condicionibus SUIS, quod tenenti suo NON inhaeret: idem off
- **corpus_normans** ⊂ organizatio — Finis proprius: normas edere et custodire - descriptiones auctoritativas quibus alii conformantur
- **societas** ⊂ organizatio — Finis proprius: quaestus per opera vendita; possessores habet quibus respondet
- **magistratus** ⊂ organizatio — Auctoritas ex IMPERIO super loco, non ex consensu (ut corpus normans) nec ex quaestu (ut societas): leges fert quibus 

### persona (v6, genera 1)

- **persona** ⚙ — Naturae rationabilis individua substantia (Boethius)

### pharmacon (v0, genera 4)

- **substantia_chemica** — Genus materiae compositione determinatum: quod omnia exemplaria eiusdem substantiae communicant, ubicumque et quandocu
- **medicamentum** — Substantia (aut plures) cum VI et FORMA determinatis ad morbum tractandum praeparata: quod praescriptio nominat et quo
- **coctura** ⚙ — Omnia quae uno cursu fabricationis facta sunt, provenientiam communem et ideo FATUM COMMUNE habentia: quod contaminatu
- **interactio** — Relatio inter res duas (aut plures) quarum praesentia simultanea effectum mutat quem singulae haberent: non proprietas

### plagula_computatralis (v2, genera 9)

- **plagula_computatralis** ⚙ — Sequentia octetorum NOMINATA in systemate plagularum: identitas quae perdurat dum contentum mutatur (ut universitas mu
- **plagula_fontis** ⊂ plagula_computatralis — Textus a personis scriptus, a personis ET translatoribus lectus: forma exsecutionis futurae, non exsecutio ipsa
- **scriptum_exsecutabile** ⊂ plagula_computatralis — Directe exsequenda sine translatione praevia: crena (#!) interpretem suum ipsa nominat - plagula quae scit quomodo cur
- **plagula_notationis** ⊂ plagula_computatralis — Contentum = prosa cum structura interposita: signa structuram ferunt, textus sensum - pro lectore humano per machinam 
- **plagula_figurationis** ⊂ plagula_computatralis — A programmate in initio lecta ut mores suos formet: declarativa, non exsecutiva - dicit quid, numquam quomodo
- **plagula_datorum** ⊂ plagula_computatralis — Contentum pro programmatibus primo: structura stricta, lector humanus hospes non dominus
- **plagula_sqlite** ⊂ plagula_datorum — Basis datorum integra in plagula una: tabulae, indices, transactiones - omnia intra sequentiam octetorum unam, sine se
- **systema_plagularum** ▣3 — Ordinatio quae sequentiis octetorum NOMINA, locum in arbore, et notas circumstantes dat: quod inter medium (discum, re
- **directorium** — Continens nominatum plagularum et directoriorum aliorum: nodus arboris nominum

### planta (v1, genera 4)

- **planta** ⊂ corporalia.vivens ⚙ ▣6 — Vivens quod se ex loco non movet et lucem in cibum vertit (photosynthesis): radicibus haeret, per partes crescit, semi
- **rosaceae** ⊂ planta — Flores quinque-petali plerumque, stipulae ad folia, fructus varii (pomum, drupa, aggregatus) - familia late diversa qu
- **rosa** ⊂ rosaceae — Caules aculeati, folia imparipinnata, fructus hypanthium carnosum (cynorrhodon) - non pomum
- **malus** ⊂ rosaceae — Fructus POMUM (hypanthium carnosum quod semina includit), non drupa nec cynorrhodon

### praescriptum (v3, genera 6)

- **procedura** — Ordo operationum ad finem: introitus declarati, gradus ordinati, exitus exspectatus
- **procedura_automata** ⊂ procedura ⚙ — A MACHINA exsecuta, ideo AMBIGUITATIS TOLERANTIA NULLA: ubi praescriptum "donec aureum fiat" dicere potest quia homo i
- **praescriptum** ⊂ procedura ⚙ ▣3 — Ordo operationum ad finem, ab homine exsequendus: introitus declarati (materiae, instrumenta), gradus ordinati, exitus
- **gradus_praescripti** — Operatio una in ordine: actio, fortasse cum quantitate, fortasse cum mora, fortasse cum conditione finiendi
- **versio_praescripti** ⊂ versio.versio — Editio praescripti nominata et citabilis: quod praescriptum per emendationes transit, identitate manente ("receptum av
- **index_verificationis** ⊂ procedura — CONFIRMAT quod factum sit; non docet quomodo fiat

### schema (v0, genera 3)

- **schema** — Forma datorum declarata: figura quam res eiusdem generis implere debent - campi nominati, genera valorum, necessitates
- **campus** — Locus nominatus in schemate: nomen, genus valoris, necessitas
- **genus_valoris** — Genus valoris quod campus tenere potest: INTENTIO valoris, non repraesentatio machinae - s32 et char* realizationes su

### systema_operativum (v4, genera 4)

- **systema_operativum** ⚙ — Programma primarium quod machinam possidet: inter ferrum et programmata mediat - memoriam distribuit, processus custod
- **systema_unixiforme** ⊂ systema_operativum — Ad exemplar Unix formatum: arbor plagularum una a radice, "omnia plagulae sunt", processus cum fistulis componendi, co
- **distributio_systematis** — Collectio curata: nucleus + instrumenta + ordinationes + gestor fasciculorum, ab uno curatore electa, probata, edita
- **versio_systematis** ⊂ versio.versio — Editio systematis nominata: quam installatio vere currit - "Darwin 23

### tempus (v4, genera 8)

- **momentum** — Punctum in tempore: quando aliquid accidit
- **intervallum** — Spatium temporis inter duo momenta: initium, finis, ideo duratio
- **recurrentia** — Regula quae momenta (aut intervalla) gignit: "omni die hora quarta", "omni die Lunae", "prima die cuiusque mensis"
- **recurrentia_computata** ⊂ recurrentia — Momenta non ex periodo simplici sed ex ALGORITHMO oriuntur: regula ipsa calculum continet, saepe corpora caelestia aut
- **recurrentia_per_usum** ⊂ recurrentia — Non tempore sed USU ACCUMULATO regitur: horis operis, cyclis, spatio confecto
- **horarium** — Recurrentiae in ordinem compositae quibus opus regitur: horae apertionis, vices operariorum, ordo coctionis
- **calendarium** — Systema quo dies nominantur et ordinantur: menses, hebdomadae, anni, dies festi
- **reservatio** ⚙ — Vindicatio capacitatis futurae: intervallum et res (sedes, cella, hora medici) alicui SERVATA ne alteri detur

### vectura (v0, genera 4)

- **iter** ⚙ — Motus rei aut personae a loco ad locum, proposito susceptus: initium, finis, via, tempus
- **via** — Semita per quam iri potest: res publica plerumque, itineribus innumeris communis
- **vehiculum** ⊂ corporalia.instrumentum — Instrumentum cuius usus MOTUS est: se movet et alia fert
- **onus** ⚙ — Res corporeae sub itinere: non genus rerum sed CONDICIO earum - idem dolium onus est dum fertur, merx dum venditur, su

### versio (v0, genera 1)

- **versio** ⚙ — Editio rei nominata et citabilis, quae rem ipsam per mutationem manentem SIGNAT: res una est, versiones multae, et ver

## III. Index alphabeticus (genera)

| genus | exemplar |
|---|---|
| acta | acta |
| actor | communicatio |
| actus_iuridicus | actus |
| actus_sollemnis | actus |
| aedificium | locus |
| ambitus | operatio |
| animal | animal |
| architectura_machinae | architectura_machinae |
| artefactum | corporalia |
| artefactum_aedificationis | elementa_programmatis |
| assertio | elementa_programmatis |
| attestatio | acta |
| bibliotheca | elementa_programmatis |
| calendarium | tempus |
| campus | schema |
| causa | causa |
| classis_normativa | norma |
| cliens | cliens |
| cliens_api | cliens |
| cliens_cli | cliens |
| cliens_mcp | cliens |
| cliens_mobilis | cliens |
| cliens_nativus | cliens |
| cliens_telae | cliens |
| cliens_terminalis | cliens |
| coctura | pharmacon |
| codex | opus_scriptum |
| commentarium | elementa_programmatis |
| complicatio | fabricatio |
| consilium | fabricatio |
| constans | elementa_programmatis |
| constans_numerica | elementa_programmatis |
| constans_textualis | elementa_programmatis |
| contractus | obligatio |
| corpus_normans | organizatio |
| damnum | corporalia |
| debitum_technicum | fabricatio |
| delegatio | obligatio |
| depravatio | fabricatio |
| directorium | plagula_computatralis |
| distributio_systematis | systema_operativum |
| editio | opus_scriptum |
| eventum | communicatio |
| expressio | opus_scriptum |
| facultas | fabricatio |
| fasciculus | opus_scriptum |
| fictio_iuris | norma |
| fons | iudicium |
| forma_datorum | formae_datorum |
| functio | elementa_programmatis |
| functio_pura | elementa_programmatis |
| genus_valoris | schema |
| gradus_assensus | iudicium |
| gradus_praescripti | praescriptum |
| horarium | tempus |
| identificator | elementa_programmatis |
| ignotum | iudicium |
| implementatio_linguae | lingua_programmandi |
| incidens | operatio |
| index | elementa_programmatis |
| index_verificationis | praescriptum |
| inscriptio | locus |
| instrumentum | corporalia |
| instrumentum_aedificationis | elementa_programmatis |
| interactio | pharmacon |
| intervallum | tempus |
| investigatio | iudicium |
| iter | vectura |
| iudicium | iudicium |
| iurisdictio | locus |
| iussum | obligatio |
| lectio | elementa_programmatis |
| lex | norma |
| licentia | obligatio |
| limitatio | obligatio |
| lingua_iussuum | lingua_programmandi |
| lingua_programmandi | lingua_programmandi |
| locus | locus |
| magistratus | organizatio |
| magnitudo_mensurabilis | mensura |
| malus | planta |
| materia | corporalia |
| medicamentum | pharmacon |
| momentum | tempus |
| moneta | commercium |
| morbus | morbus |
| mutatio | elementa_programmatis |
| norma | norma |
| nuntius | communicatio |
| obligatio | obligatio |
| observatio | iudicium |
| officium | organizatio |
| onus | vectura |
| opus_scriptum | opus_scriptum |
| organizatio | organizatio |
| pecunia | commercium |
| pensum | operatio |
| periculum | iudicium |
| periodicum | opus_scriptum |
| persona | persona |
| persona_iuridica | organizatio |
| petitio | obligatio |
| plagula_computatralis | plagula_computatralis |
| plagula_datorum | plagula_computatralis |
| plagula_figurationis | plagula_computatralis |
| plagula_fontis | plagula_computatralis |
| plagula_notationis | plagula_computatralis |
| plagula_sqlite | plagula_computatralis |
| planta | planta |
| possessio | commercium |
| postulatum | fabricatio |
| praescriptum | praescriptum |
| pretium | commercium |
| probatio | fabricatio |
| procedura | praescriptum |
| procedura_automata | praescriptum |
| processorium | architectura_machinae |
| processus | operatio |
| programma | elementa_programmatis |
| proiectio | elementa_programmatis |
| promptuarium | elementa_programmatis |
| punctum_actorum | acta |
| quantitas | mensura |
| quorum | actus |
| radix_fiduciae | acta |
| ratio_rationaria | commercium |
| recurrentia | tempus |
| recurrentia_computata | tempus |
| recurrentia_per_usum | tempus |
| res_corporea | corporalia |
| res_sacra | corporalia |
| reservatio | tempus |
| ritus | actus |
| rosa | planta |
| rosaceae | planta |
| sacramentale | actus |
| sacramentum | actus |
| schema | schema |
| schema_identificandi | identificatio |
| scriptum_exsecutabile | plagula_computatralis |
| secretum | iudicium |
| servus | cliens |
| societas | organizatio |
| status | elementa_programmatis |
| substantia_chemica | pharmacon |
| systema_operativum | systema_operativum |
| systema_plagularum | plagula_computatralis |
| systema_unixiforme | systema_operativum |
| tessera | commercium |
| tessera_negotiabilis | commercium |
| tessera_simplex | commercium |
| titulus | obligatio |
| tractatus | obligatio |
| tributum | obligatio |
| unitas_mensurae | mensura |
| vehiculum | vectura |
| venditio | commercium |
| verbum_clavis | lingua_programmandi |
| versio | versio |
| versio_architecturae | architectura_machinae |
| versio_linguae | lingua_programmandi |
| versio_praescripti | praescriptum |
| versio_systematis | systema_operativum |
| via | vectura |
| vitium | fabricatio |
| vivens | corporalia |

## IV. Dictionarium (species et individua descripta)

- `acta.acta` — acta_auditus, acta_publica, acta_transactionum, acta_versionum, regesta
- `acta.attestatio` — apostilla, attestatio_civilis, instrumentum_publicum
- `acta.punctum_actorum` — clausura_rationis, conditio, statio
- `acta.radix_fiduciae` — commissio_notarii, origo_dominii, radix_systematis
- `actus.actus_iuridicus` — collatio_tituli, confectio_instrumenti, inauguratio, mancipatio, manumissio, promulgatio, sententia_lata, stipulatio, testamentum
- `actus.quorum` — maioritas_replicarum, quorum_deliberativum
- `actus.sacramentale` — benedictio, consecratio_loci, exorcismus
- `actus.sacramentum` — baptismus, confirmatio, eucharistia, matrimonium, ordo, paenitentia, unctio_infirmorum
- `animal.animal` — apis, canis, homo
- `architectura_machinae.architectura_machinae` — :arm, :riscv, :x86
- `architectura_machinae.processorium` — :apple_m1, :intel_8086
- `architectura_machinae.versio_architecturae` — :armv8, :i386, :x86_64
- `causa.causa` — causa_efficiens, causa_finalis, causa_formalis, causa_materialis
- `cliens.cliens_cli` — :curl
- `cliens.servus` — servus_datorum, servus_mcp, servus_nominum, servus_procuratorius, servus_telae
- `commercium.moneta` — :dollarium_americanum, :euro
- `commercium.ratio_rationaria` — ratio_duplex
- `commercium.tessera_negotiabilis` — apocha_horrei, charta_onerum, nummus_chartaceus, obligatio_ad_portitorem, perscriptio, syngrapha
- `commercium.tessera_simplex` — tessera_digitalis, tessera_repositoria, tessera_spectaculi
- `corporalia.artefactum` — cibus
- `corporalia.damnum` — contaminatio, corruptio, fractura
- `corporalia.res_sacra` — reliquiae, res_sacra_immobilis, res_sacra_mobilis
- `elementa_programmatis.artefactum_aedificationis` — exsecutabile_ligatum, obiectum_compilatum
- `elementa_programmatis.assertio` — assertio_probationis, assertio_statica, invarians, postconditio, praeconditio
- `elementa_programmatis.bibliotheca` — :libc, :sqlite, :zlib
- `elementa_programmatis.commentarium` — annotatio_machinae, codex_sepultus, documentarium, explicativum, monitum_futuri, titulus_iuris
- `elementa_programmatis.constans` — :null
- `elementa_programmatis.constans_numerica` — :eof, :pi
- `elementa_programmatis.functio` — :malloc, :memcpy, :printf, :rand, :scanf, :strtok
- `elementa_programmatis.functio_pura` — :abs, :strlen
- `elementa_programmatis.identificator` — identificator_baptismalis, sigillum_contenti
- `elementa_programmatis.index` — index_arboris, index_inversus
- `elementa_programmatis.instrumentum_aedificationis` — :aedilis, :make
- `elementa_programmatis.lectio` — lectio_externa, lectio_memoriae
- `elementa_programmatis.mutatio` — allocatio, mutatio_memoriae, scriptio_externa, terminatio, transactio
- `elementa_programmatis.programma` — applicatio, daemon, instrumentum_lineae
- `elementa_programmatis.status` — :errno, :index_strtok, :semen_rand
- `fabricatio.consilium` — consilium_computatum
- `fabricatio.probatio` — probatio_fumi, probatio_integrationis, probatio_regressionis, probatio_unitatis
- `fabricatio.vitium` — regressio
- `formae_datorum.forma_datorum` — acervus, arbor, cauda, copia, series, tabula_associativa
- `identificatio.schema_identificandi` — cas, doi, e164, gtin, iban, isbn, isbn_10, isbn_13, issn, ndc, numerus_dea, orcid, rxnorm, ulid, uri, uuid
- `iudicium.fons` — corpus_institutionis, fons_mensurae, fons_scriptus, fons_testimonii
- `iudicium.gradus_assensus` — dubitatio, fides, opinio, scientia, suspicio
- `iudicium.ignotum` — amissum, controversum, ignotum_ignotum, ignotum_notum, impertinens, indeterminatum, retentum
- `iudicium.investigatio` — inquisitio_forensis, investigatio_praeventiva, recensio_post_mortem
- `iudicium.iudicium` — aestimatio, diagnosis, praedictio, sententia_iudicis
- `iudicium.observatio` — signum, symptoma, testimonium
- `iudicium.secretum` — clavis_privata, secretum_negotii, sigillum_confessionis, verbum_arcanum
- `lingua_programmandi.implementatio_linguae` — :clang, :cpython, :fil_c, :gcc, :officina
- `lingua_programmandi.lingua_iussuum` — :bash
- `lingua_programmandi.lingua_programmandi` — :abc, :b, :c, :python, :rust
- `lingua_programmandi.verbum_clavis` — :const, :goto, :restrict, :static
- `lingua_programmandi.versio_linguae` — :c11, :c89, :c99
- `mensura.magnitudo_mensurabilis` — capacitas, longitudo, massa, numerositas, temperatura, tempus_intervallum
- `mensura.unitas_mensurae` — :gradus_celsius, :gramma, :kilogramma, :metrum, :minutum, :secundum
- `morbus.morbus` — morbus_degenerativus, morbus_geneticus, morbus_infectiosus, vulnus
- `norma.classis_normativa` — classis_periculi_mercium, gradus_eventus_aviatorii, schedula_substantiarum
- `norma.fictio_iuris` — adoptio, mors_praesumpta, personalitas_iuridica
- `norma.lex` — consuetudo, lex_aeterna, lex_divina_positiva, lex_humana, lex_naturalis
- `norma.norma` — :iso_9899_1990, :iso_9899_1999, :posix_1
- `obligatio.contractus` — emptio_venditio, locatio_conductio, mandatum, societas_contractus
- `obligatio.delegatio` — delegatio_canonica, plena_potestas, procuratio
- `obligatio.iussum` — iussum_emptionis, iussum_ne_resuscitetur, iussum_operis, mandatum_iudicis, praescriptio_medica
- `obligatio.obligatio` — obligatio_ex_contractu, obligatio_ex_delicto, obligatio_ex_lege
- `obligatio.petitio` — petitio_assecurationis, petitio_remota, supplicatio
- `obligatio.titulus` — character_indelebilis, gradus_academicus, titulus_honoris
- `obligatio.tractatus` — :tractatus_gandavensis
- `obligatio.tributum` — decuma, portorium, tributum_capitis, vectigal
- `operatio.ambitus` — ambitus_fabricae, ambitus_probationis, ambitus_producens
- `opus_scriptum.editio` — :moby_dick_londinensis_1851, :moby_dick_novi_eboraci_1851, :systema_naturae_editio_decima
- `opus_scriptum.expressio` — :abusus_linguae_anglice, :abusus_linguae_germanice
- `opus_scriptum.opus_scriptum` — :abusus_linguae, :moby_dick, :species_plantarum, :summa_theologiae, :systema_naturae
- `organizatio.corpus_normans` — :ansi, :iec, :ieee, :iso
- `organizatio.officium` — curator, episcopus, notarius, parochus, praeses_collegii
- `organizatio.organizatio` — :agentia_isbn, :debian_project
- `organizatio.persona_iuridica` — universitas_personarum, universitas_rerum
- `organizatio.societas` — :amd, :apple, :arm_ltd, :att, :bell_labs, :canonical, :intel, :microsoft
- `persona.persona` — :bertrand_meyer, :brian_fox, :carl_linnaeus, :dennis_ritchie, :duns_scotus, :edsger_dijkstra, :graydon_hoare, :guido_van_rossum, :herman_melville, :josef_pieper, :ken_thompson, :linus_torvalds, :luca_pacioli, :thomas_aquinas
- `pharmacon.interactio` — incompatibilitas_mercium, interactio_cum_cibo, interactio_medicamentorum
- `pharmacon.medicamentum` — medicamentum_compositum, medicamentum_genericum, medicamentum_nominatum
- `pharmacon.substantia_chemica` — :amoxicillinum, :warfarinum
- `plagula_computatralis.plagula_datorum` — plagula_csv, plagula_json
- `plagula_computatralis.plagula_figurationis` — plagula_plist, plagula_toml
- `plagula_computatralis.plagula_fontis` — caput_c, fons_c, fons_python, fons_rust
- `plagula_computatralis.plagula_notationis` — plagula_markdown, plagula_stml, plagula_xml
- `plagula_computatralis.plagula_sqlite` — volumen
- `plagula_computatralis.scriptum_exsecutabile` — scriptum_conchae
- `planta.malus` — granny_smith, malus_domestica
- `planta.rosa` — rosa_canina
- `praescriptum.procedura_automata` — depositio, fistula_integrationis
- `schema.genus_valoris` — citatio, dies, electio, numerus, textus, veritas
- `systema_operativum.distributio_systematis` — :arch, :debian, :ubuntu
- `systema_operativum.systema_operativum` — :vms, :windows
- `systema_operativum.systema_unixiforme` — :linux, :macos, :unix_bell_labs
- `systema_operativum.versio_systematis` — :macos_13_ventura, :macos_14_sonoma
- `tempus.calendarium` — :calendarium_gregorianum, :calendarium_liturgicum
- `tempus.recurrentia_computata` — dies_negotiorum, dies_ordinalis_mensis, pascha
- `tempus.recurrentia_per_usum` — per_cyclos, per_horas_operis, per_spatium
- `vectura.vehiculum` — aeroplanum, currus_onerarius, navis, tramen
- `vectura.via` — :canalis_suesianus, :via_appia, via_aeria, via_maritima, via_terrestris

## V. Umbrae — superficta, nondum descripta

Agenda COMPUTATA (non memorata): quod aliquod exemplar citat sed
nemo describit. DUAE agendae sunt, non una — ordo intra utramque
per numerum citationum (quod plura exemplaria poscunt, prius).

### V.a Genera desiderata (2) — opus consilii

A `relatio` citata: relationes in GENERA tendunt, ergo quaestio
ontologica et iudicium poscitur.

- **typus** — 2x, a `elementa_programmatis`
- **constructum_linguae** — 1x, a `elementa_programmatis`

### V.b Individua desiderata (0) — opus dictionarii

A `relato` SOLO citata: res singularis nominata sub genere iam
exsistente. Nullum consilium — describi potest quovis momento.
(EURISTICA, non lex: relatum genus attingere potest.)


## VI. Dubia aperta

- `acta` / **punctum_actorum** — RAMI: haec relatio 'praecedens' seriem simplicem supponit. Acta ramosa (git) puncta plura post idem punctum ferunt, et tunc 'plicatura ad punctum' viam eligere 
- `elementa_programmatis` / **functio_pura** — An proprietas potius quam sub-genus (puritas="verum")? Argumentum sub-generis: fasciculus licentiarum supra. Fran nondum iudicavit - v0 sub-genus audet. 
- `elementa_programmatis` / **constans** — 'const' in via accessus (parametrum monstratoris) rem ALIAM dicit: pactum non scribendi per hoc nomen - promissum functionis, non constantia rei. Genus proprium
- `elementa_programmatis` / **libc** — libc FORMA est quam glibc, musl, msvcrt IMPLEMENTANT - distinctio lingua/implementatio in bibliothecis recurrit. Genus implementatio_bibliothecae nondum natum; 
- `fabricatio` / **vitium** — SANATIO REBUS CORPOREIS NON CONVENIT (inventum experimenti pistrinae): sanatio = mutatio fontis, quae rem emendabilem supponit. Panis adustus non emendatur - ab
- `fabricatio` / **complicatio** — PERICULUM PIEPERIANUM, aperte notandum: "complicatio" in excusationem verti potest sicut "debitum technicum" versum est - scutum quo lapsus tegatur ("non erravi
- `norma` / **posix_1** — Editio "1003.1" continenter renovatur (1988, 2001, 2008, 2024...) - norma UNA vivens an series normarum se substituentium? Idem dubium quod versiones linguarum:
- `operatio` / **processus** — DAEMON BIS SEDET. Bibliotheca daemonem speciem PROGRAMMATIS facit (elementa_programmatis, divisa per modum finiendi: terminans/perpetuum). At quod daemonem daem
- `opus_scriptum` / **periodicum** — Machina "compositio" generis parentis periodico male sedet: status perfecta numquam attingitur. Estne differentia satis, an machina propria opus est (vigens/int
- `pharmacon` / **interactio** — AN INTERACTIO SEMPER MALA SIT. Species omnes hic nocumentum ferunt, sed synergismus therapeuticus (medicamenta duo consulto coniuncta ut alterum alterius effect
- `planta` / **planta** — Proprietates viventium AMBITUS sunt, non puncta: altitudo "2-5 m", dies florendi "Maius-Iunius". Forma nostra valorem unum fert. Res vivens hanc formam poscit; 
- `schema` / **genus_valoris** — Paletta domus etiam "area" (textus amplus) et "annus" (dies grossior) habet: species propriae, an textus cum praesentatione et dies cum granularitate? Praesenta
- `tempus` / **pascha** — GRADUS AMBIGUUS (rota XXIX): una est regula paschalis, ergo individuum videtur; at "pascha" etiam de quovis anno dicitur ("pascha huius anni"), quod praedicatio
- `versio` / **versio** — FURCA (fork) versio non est - continuatio auctoritatis deficit, unde res nova. Sed casus medii sunt: quid de furca quae nomen retinet et auctoritatem novam habe

## VII. Monita — stirps communis sine cognatione (24)

Regula XVIII (ADVISORIA, portam non frangit). Nomen quod nomen
aliud ut stirpem fert, sed nec sub eo stat nec relationem ad id
declarat. **Pleraque LEGITIMA sunt** — homonyma honesta:
`fons_c` plagula est, `fons` propositionis origo. Pretium
regulae non est quod defectus inveniat sed quod nulla talis
coincidentia INVISA transeat.

- 'artefactum_aedificationis' stirpem 'artefactum' fert sed sub ea non stat
- 'codex_sepultus' stirpem 'codex' fert sed sub ea non stat
- 'dies_negotiorum' stirpem 'dies' fert sed sub ea non stat
- 'dies_ordinalis_mensis' stirpem 'dies' fert sed sub ea non stat
- 'fons_c' stirpem 'fons' fert sed sub ea non stat
- 'fons_python' stirpem 'fons' fert sed sub ea non stat
- 'fons_rust' stirpem 'fons' fert sed sub ea non stat
- 'index_strtok' stirpem 'index' fert sed sub ea non stat
- 'index_verificationis' stirpem 'index' fert sed sub ea non stat
- 'instrumentum_aedificationis' stirpem 'instrumentum' fert sed sub ea non stat
- 'instrumentum_lineae' stirpem 'instrumentum' fert sed sub ea non stat
- 'instrumentum_publicum' stirpem 'instrumentum' fert sed sub ea non stat
- 'isbn_10' stirpem 'isbn' fert sed sub ea non stat
- 'isbn_13' stirpem 'isbn' fert sed sub ea non stat
- 'mandatum_iudicis' stirpem 'mandatum' fert sed sub ea non stat
- 'moby_dick_londinensis_1851' stirpem 'moby_dick' fert sed sub ea non stat
- 'moby_dick_novi_eboraci_1851' stirpem 'moby_dick' fert sed sub ea non stat
- 'numerus_dea' stirpem 'numerus' fert sed sub ea non stat
- 'obligatio_ad_portitorem' stirpem 'obligatio' fert sed sub ea non stat
- 'persona_iuridica' stirpem 'persona' fert sed sub ea non stat
- 'schema_identificandi' stirpem 'schema' fert sed sub ea non stat
- 'societas_contractus' stirpem 'societas' fert sed sub ea non stat
- 'systema_naturae_editio_decima' stirpem 'systema_naturae' fert sed sub ea non stat
- 'titulus_iuris' stirpem 'titulus' fert sed sub ea non stat

## VIII. Vocabularium formae (omnes tituli licentes)

**Elementa (27):** natura, fontes, fons, genus, definitio, differentia, proprietates, proprietas, optio, partes, pars, machina_statuum, status, transitus, actiones, actio, relationes, relatio, species, individuum, cultivar, valor, relatum, historia, eventum, nota, dubium

**Attributa:** nomen, modulus, versio, lingua, sub, gradus, etiam, genus, ad, a, per, multiplex, ordinarius, necessaria, externum, inversa, gerens, quando, actio, clavis, verificatus, certitudo, fons, valens_a, valens_ad, nota

Vocabularium CLAUSUM est (METAMODULUS regula VIII): titulus novus
sine emendatione specificationis portam frangit.
