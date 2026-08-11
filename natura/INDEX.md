# NATURA — INDEX GENERUM

**GENERATUM** a `tools/natura_visus.sh` — noli manu emendare.
Regenera: `./tools/natura_visus.sh` (idem cursus qui portam custodit).

Exemplaria **35** · genera **460** · res dictionarii **114** · necessitudines **85** · arcus **528**

Cur haec exsistat, cui serviat, quomodo laboremus: `natura/RATIO.md`.
Forma ipsa (elementa, attributa, regulae): `natura/METAMODULUS.md`.
Historia et doctrina: `natura/natura.worklog.md` (LEGE PRIMUM).

## I. Quaestio usitatissima: an genus iam exsistat?

```
grep -i '<terminus>' natura/INDEX.md      # genus aut res
grep -n 'nomen="<genus>"' natura/*.genera   # sedes definitionis
bin/natura quaere <terminus>               # QUAERE (+ umbrae, necessitudines, sententia collisionis)
bin/natura apparatus mod.genus             # APPARATUS: quid rogare debeas
```

## II. Genera per exemplar

Signa apparatus — quid genus applicanti PRAESTET (RATIO §3):
`⚙` machinam statuum fert · `▣N` partes N declaratas fert
(partes = index quo ABSENTIA visibilis fit).

### acta (v4, genera 18)

- **acta** ⚙ ▣5 — Series eventuum ordinata cui SOLUM APPENDITUR, et quae rerum descriptarum VERITAS est: status quilibet (summa, manifes
- **punctum_actorum** — Locus in actis nominatus et allocutione dignus, ita ut interrogatio "qualia erant HIC?" responsum habeat quod homo pet
- **attestatio** ⚙ — Exemplar authenticum ex actis auctoritativis sumptum, quod quid in actis steterit MOMENTO EMISSIONIS testatur, et trad
- **radix_fiduciae** ⚙ — Terminus catenae attestationum, cui fides non ex alia attestatione venit sed ex DECRETO eius qui catenam adhibet: quod
- **acta_publica** ⊂ acta — Res gestae communitatis ordine temporis propositae: acta diurna populi Romani (in foro albata proponebantur), acta sen
- **regesta** ⊂ acta — Liber cancellariae in quo acta EMISSA (litterae, privilegia, bullae) ordine describuntur antequam mittantur: exemplar 
- **acta_transactionum** ⊂ acta — Cauda mutationum ANTE applicationem scripta (write-ahead): basis datorum ipsa proiectio est, et post casum ex actis re
- **acta_versionum** ⊂ acta — Acta mutationum plagularum, quorum plicatura ARBOR est: manifestum viarum ad contenta
- **acta_auditus** ⊂ acta — Acta quorum finis est ut QUIS QUID QUANDO fecerit postea constet, praesertim contra ipsum qui fecit
- **conditio** ⊂ punctum_actorum — Punctum in actis versionum, cum titulo ab homine dato: quod "commit" vulgo vocatur
- **clausura_rationis** ⊂ punctum_actorum — Punctum quo periodus rationaria clauditur et saldum constat: post id nihil in periodum appenditur, emendatio in period
- **statio** ⊂ punctum_actorum — Punctum a machina positum ut plicatura pretiosa semel fiat et postea ab eo, non ab initio, incipiat: quod "checkpoint"
- **attestatio_civilis** ⊂ attestatio — Ex actis status civilis sumpta: nativitatis, matrimonii, mortis
- **instrumentum_publicum** ⊂ attestatio — A notario (aut persona publica) confectum, cui fides publica inest: quod in eo scriptum est verum praesumitur donec fa
- **apostilla** ⊂ attestatio — Attestatio quae subscriptionem aut sigillum attestationis ALTERIUS authenticam esse testatur, ut extra fines valeat (c
- **radix_systematis** ⊂ radix_fiduciae — Auctoritas certificatoria cuius attestatio cum systemate aut navigatro missa est: fides praeinstallata
- **commissio_notarii** ⊂ radix_fiduciae — Auctoritas ipsius notarii, a magistratu collata: quae instrumenta eius publica facit et ipsa ab alio notario non pende
- **origo_dominii** ⊂ radix_fiduciae — Primum in catena titulorum praedii, ultra quod investigatio non procedit: concessio principis, adiudicatio, aut posses

### actus (v2, genera 27)

- **actus_sollemnis** ⚙ — Actus cuius EFFECTUS a forma praescripta et ab agentis capacitate pendet, et qui ideo VALIDUS aut IRRITUS est, non bon
- **ritus** ⊂ actus_sollemnis — Actus sollemnis ad cultum divinum ordinatus: materia sensibilis et forma verborum, per ministrum, ad effectum spiritua
- **sacramentum** ⊂ ritus — Signum efficax gratiae, a Christo institutum: effectum ponit EX OPERE OPERATO - ex actu ipso rite posito, non ex digni
- **sacramentale** ⊂ ritus — Ab Ecclesia institutum, non a Christo: effectum ponit EX OPERE OPERANTIS - ex dispositione agentis et Ecclesiae precib
- **actus_iuridicus** ⊂ actus_sollemnis — Actus sollemnis cuius vim auctoritas civilis praestat: quod lex formam praescribit, lex effectum agnoscit
- **quorum** — Numerus (aut pars) membrorum cuius praesentia aut consensus requiritur ut coetus valide agat: infra eum acta non fiunt
- **baptismus** ⊂ sacramentum — Ablutio aquae cum verbis trinitariis: ianua sacramentorum
- **confirmatio** ⊂ sacramentum — 
- **eucharistia** ⊂ sacramentum — 
- **paenitentia** ⊂ sacramentum — 
- **unctio_infirmorum** ⊂ sacramentum — 
- **ordo** ⊂ sacramentum — 
- **matrimonium** ⊂ sacramentum — 
- **benedictio** ⊂ sacramentale — 
- **consecratio_loci** ⊂ sacramentale — 
- **exorcismus** ⊂ sacramentale — 
- **stipulatio** ⊂ actus_iuridicus — Contractus verbis: interrogatio et responsus congruentes, utroque praesente ("Spondesne? Spondeo")
- **mancipatio** ⊂ actus_iuridicus — Traditio sollemnis rerum mancipi: libripens cum libra aenea, testes quinque cives, verba praescripta, aes percussum
- **manumissio** ⊂ actus_iuridicus — Actus quo servus liber fit: vindicta apud magistratum, censu, aut testamento - formae tres, effectus unus
- **testamentum** ⊂ actus_iuridicus — Dispositio bonorum in mortem, forma sollemni facta: subscriptio, testes, interdum manus propria tota
- **confectio_instrumenti** ⊂ actus_iuridicus — Actus quo notarius (aut persona publica) instrumentum conficit: verba partium in formam publicam redigit, identitates 
- **promulgatio** ⊂ actus_iuridicus — Actus quo lex vim accipit: publicatio forma praescripta
- **inauguratio** ⊂ actus_iuridicus — Iusiurandum aut investitura qua quis officium suscipit: ante actum candidatus, post actum tenens
- **collatio_tituli** ⊂ actus_iuridicus — Actus quo auctoritas titulum confert: gradus academicus, dignitas, honor
- **sententia_lata** ⊂ actus_iuridicus — Actus quo iudex iudicium suum PRONUNTIAT et vim ei dat: ante pronuntiationem opinio, post pronuntiationem ius
- **quorum_deliberativum** ⊂ quorum — Quot adesse debeant ut concilium (senatus, curia, capitulum) deliberare et decernere possit
- **maioritas_replicarum** ⊂ quorum — Quot ex machinis coetus consentire debeant ut scriptio commissa habeatur: plerumque dimidium plus uno

### animal (v1, genera 4)

- **animal** ⊂ corporalia.vivens ⚙ ▣3 — SENSUM habet et motum localem: mundum percipit et se ad eum movet - unde appetitus, unde fuga, unde quaesitio
- **apis** ⊂ animal — Insectum sociale mel faciens: colonia ut unum agens (examen), communicatio per saltationem, pollinatio ut effectus sec
- **canis** ⊂ animal — Canis familiaris: lupus ab homine per millennia mutatus - species cuius forma ipsa OPUS HOMINIS est, sicut malus domes
- **homo** ⊂ animal — Animal rationale: corpus animale cum anima quae intellegit et eligit

### architectura_machinae (v3, genera 3)

- **architectura_machinae** — Forma abstracta machinae computatoriae quam programmata alloquuntur: copia instructionum, registra, modellum memoriae
- **versio_architecturae** ⊂ versio.versio — Editio architecturae nominata: quam copiam instructionum chip vere exsequitur et compilator vere alloquitur - "aedific
- **processorium** — Exemplar productum circuli computantis: forma nominata quam officina in assulas innumeras imprimit, quae architecturam

### causa (v0, genera 5)

- **causa** — Id cuius vi aliquid est aut fit: non quod praecedit tantum (id antecedens est) sed quod EFFICIT
- **causa_materialis** ⊂ causa — Id ex quo aliquid fit et in quo manet: aes statuae, farina panis
- **causa_formalis** ⊂ causa — Forma quae rem hoc ipsum facit quod est: figura statuae, ratio circuli
- **causa_efficiens** ⊂ causa — Unde principium motus: artifex statuae, actus qui officium constituit
- **causa_finalis** ⊂ causa — Id cuius gratia aliquid fit: sanitas ambulationis, bonum commune legis

### cliens (v2, genera 14)

- **cliens** — Programma quod ministerium alienum consumit, pro utente agens: dimidium petens colloquii
- **cliens_telae** ⊂ cliens — In navigatro vivit: institutione caret - inscriptio URL ipsa distributio est
- **cliens_nativus** ⊂ cliens — In systemate ipso vivit: institutione indiget, plenitudine praemiatur - plagulae, processus, fenestrae, omnia systemat
- **cliens_terminalis** ⊂ cliens — In terminali vivit, textu solo pingit: quo terminale pervenit, is pervenit - trans SSH in machinas remotas sine mutati
- **cliens_api** ⊂ cliens — Machina machinae, sine facie humana: interfaciem programmaticam consumit
- **cliens_mcp** ⊂ cliens — Cliens cuius UTENS agens artificialis est: ministeria per protocollum instrumentorum consumit, agens pro homine per ag
- **cliens_cli** ⊂ cliens — Instrumentum imperii unius: vocatur, agit, exit
- **cliens_mobilis** ⊂ cliens — In telephono vivit: tactus pro mure, interruptiones pro sessione, pila pro fine - vincula eius differentia sunt
- **servus** — Programma quod ministerium aliis praebet: dimidium RESPONDENS colloquii
- **servus_telae** ⊂ servus — Protocollo HTTP respondet: petitio sine memoria priorum venit, unde status omnis aut in petitione ipsa aut alibi vivit
- **servus_datorum** ⊂ servus — Non documenta sed INTERROGATIONES accipit: petens quid velit dicit, servus quomodo id inveniat eligit
- **servus_nominum** ⊂ servus — Nomina in inscriptiones vertit: servus cuius responsa SERVANTUR ab omnibus per quos transeunt, unde mutatio non statim
- **servus_procuratorius** ⊂ servus — Petitionem accipit et alteri servo transmittit: SERVUS PETENTI, CLIENS PRAEBENTI - idem programma utrumque simul
- **servus_mcp** ⊂ servus — Instrumenta agenti artificiali praebet: petens homo non est nec programma more solito, sed agens qui instrumenta LEGIT

### commercium (v4, genera 19)

- **moneta** — Genus pecuniae ab auctoritate emissum et receptum: unitas mensurae valoris - sed unitas cuius valor ipse MUTATUR (metr
- **pecunia** — Quantitas in moneta: numerus CUM MONETA - "3,50" nihil est donec moneta adsit (eadem forma qua quantitas unitatem posc
- **possessio** — Relatio inter personam (aut organizationem) et rem: quis quid habeat
- **pretium** — Pecunia pro re petita: relatio TEMPORE VARIANS inter rem et pecuniam - non proprietas rei
- **venditio** — Eventus quo possessio rei transit et pecunia contrarie fluit: mutatio DUPLEX simul - quod ratio duplex (Pacioli) ante 
- **tessera** — Res cuius EXHIBITIO ius constituit, non probat: differentia ab apocha vulgari tota hic est - apocha te solvisse TESTAT
- **tessera_negotiabilis** ⊂ tessera — Accipiens bona fide et pretio dato titulum PURUM accipit, etiam si tradens ipse ius non habuit: EXCEPTIO ad regulam 'n
- **tessera_simplex** ⊂ tessera — Possessio sufficit ad exhibendum, sed nulla titulorum purgatio: qui tesseram furatam exhibet admittitur (custos nescit
- **ratio_rationaria** ⊂ acta.acta — Acta rerum pecuniariarum ordine temporis: veritas est cauda eventuum, status omnis (summa, debitum, lucrum) PLICATURA
- **charta_onerum** ⊂ tessera_negotiabilis — A vectore data, merces acceptas testans et dominium earum ferens: dum navis navigat, merces venduntur charta tradita -
- **apocha_horrei** ⊂ tessera_negotiabilis — Idem officium, re QUIESCENTE: merces in horreo positas testatur et dominium earum fert
- **syngrapha** ⊂ tessera_negotiabilis — Promissum scriptum solvendi: debitum ipsum in chartam versum, ideo vendibile
- **perscriptio** ⊂ tessera_negotiabilis — Iussum argentario datum ut ex ratione solvat: pecunia quae non movetur sed IUBETUR moveri - charta pro pecunia, quia f
- **nummus_chartaceus** ⊂ tessera_negotiabilis — Tessera ad portitorem quam nemo tesseram vocat: "promitto me portitori ad petitionem soluturum" in chartis Britannicis
- **obligatio_ad_portitorem** ⊂ tessera_negotiabilis — Debitum publicum sine nomine, cum foliis usurariis resecandis: species fere exstincta, cuius mors CAUSAM generis illus
- **tessera_spectaculi** ⊂ tessera_simplex — Ius admissionis ad locum et tempus certum: exhibetur, consumitur, exstinguitur
- **tessera_repositoria** ⊂ tessera_simplex — Ius recipiendi rem certam depositam (vestiarium, pignus): tessera rem INDIVIDUAM designat, non genus - unde numerus in
- **tessera_digitalis** ⊂ tessera_simplex — Tessera ad portitorem sine corpore: qui eam tenet auctoritatem exercet, nullo nomine quaesito
- **ratio_duplex** ⊂ ratio_rationaria — Omnis eventus BIS scribitur (debitum et creditum): summa omnium debitorum summae creditorum semper aequalis - INVARIAN

### communicatio (v3, genera 8)

- **actor** — Ens logicum cum identitate, statu privato, moribus: cum aliis SOLUM per nuntios communicat
- **nuntius** ⚙ — Communicatio reificata: data in itinere INSCRIPTA - destinatarius ei debetur
- **eventum** — Res gesta NON inscripta: accidit, et quivis observans videre potest - nulli debetur
- **stigmergia** — Coordinatio per vestigium quod agens in medio communi relinquit: signum nulli inscriptum, quod manet, a quolibet trans
- **vestigium_chemicum** ⊂ stigmergia — Substantia ab agente relicta quae per diffusionem legitur: semita formicarum, feromonum reginae, signum territorii
- **semita_trita** ⊂ stigmergia — Via quam transeuntes calcando faciunt et calcando servant: quod aedificatores "semitas desiderii" vocant
- **tabula_communis** ⊂ stigmergia — Superficies quam multi mutant et legunt sine ordine praestituto: tabula parietis, commentarius communis, vicipaedia
- **vestigia_operis** ⊂ stigmergia — Arbor plagularum quam instrumenta legunt ut quid agendum sit statuant: directorium aedificationis, arbor laborans

### corporalia (v4, genera 14)

- **res_corporea** ⚙ — Quod materiam habet, locum occupat, tempore corrumpitur: res quae manibus tangi potest
- **artefactum** ⊂ res_corporea ▣1 — Ab homine facta ad finem: NUMERABILIS, singularis, haecceitatem habens - hic panis, non alius, etiamsi gemellus adsit
- **materia** ⊂ res_corporea — MENSURABILIS non numerabilis: farina, aqua, oleum - "tres farinae" nihil significat
- **instrumentum** ⊂ artefactum ⚙ — Ad faciendum factum, non ad consumendum: usu SERVATUR (immo usus eius finis est), non destruitur
- **res_sacra** ⊂ res_corporea ⚙ — Divino cultui per ACTUM dedicata, non per naturam nec per materiam: idem lignum mensa aut altare esse potest, et diffe
- **damnum** ⚙ — Laesio integritatis rei corporeae ab externa vi illata: quod unum erat divisum, quod integrum fractum, quod purum mixt
- **vivens** ⊂ res_corporea ⚙ — Se ipsum movet, nutrit, generat: principium motus INTRA se habet, ubi artefactum motum ab alio accipit et materia null
- **cibus** ⊂ artefactum — Artefactum consumendo destinatum: usus eum DESTRUIT - nota quae cibum ab omnibus fere artefactis distinguit (instrumen
- **res_sacra_immobilis** ⊂ res_sacra — Ecclesia, altare, coemeterium: loco fixa et ideo iurisdictioni loci obnoxia simul ac dedicationi
- **res_sacra_mobilis** ⊂ res_sacra — Vasa sacra, vestes, libri liturgici: quae moveri possunt et ideo alienari, unde regulae strictiores
- **reliquiae** ⊂ res_sacra — Corporis sancti partes aut res ei contactae: species cuius venditio omnino prohibita est (simonia), non restricta - un
- **fractura** ⊂ damnum — Divisio quod unum esse debet
- **corruptio** ⊂ damnum — Mutatio substantiae ipsius: putredo, rubigo, umor
- **contaminatio** ⊂ damnum — Admixtio alieni: quod purum esse debet mixtum est

### documentatio (v2, genera 9)

- **dictionarium** ⊂ opus_scriptum.opus_scriptum — Ad INSPECTIONEM destinatum: quid res sint docet, singulatim, ordine quaerendi non legendi
- **manuale** ⊂ opus_scriptum.opus_scriptum — Ad ACTIONEM destinatum: quid faciendum sit docet
- **exemplum** ⊂ opus_scriptum.opus_scriptum — Specimen INTEGRUM ad imitandum: non describit sed monstrat
- **legenda** ⊂ opus_scriptum.opus_scriptum — Clavis notationis: quid signa in opere alio significent docet
- **defectus_documenti** — Quod textus lectori debuit nec lectio reddit: discrepantia inter debitum documenti et quod lector inde construere pote
- **deceptio** ⊂ defectus_documenti — Textus falsum DOCET: lectio recta lectorem in errorem ducit
- **silentium** ⊂ defectus_documenti — Textus tacet quod scit: scientia exstat et narratio eam retinet
- **ambiguitas** ⊂ defectus_documenti — Textus plures sensus admittit nec fundamentum eligendi praebet: lector inter construenda sine causa eligere cogitur
- **ambulatio_lectoris** ⊂ fabricatio.probatio — Probatio documenti per lectorem RESTRICTUM: lector ad fontem sub probatione solum adstringitur, ut fons ipse mensuretu

### elementa_programmatis (v5, genera 45)

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
- **obiectum_compilatum** ⊂ artefactum_aedificationis — Plagula obiecti ex fonte uno: gradus intermedius, ligando destinatus
- **exsecutabile_ligatum** ⊂ artefactum_aedificationis — Artefactum ultimum: obiecta ligata quae currere possunt
- **index_inversus** ⊂ index — A verbo ad loca: quaestioni plenitextus servit - textus per documenta currit, index per verba
- **index_arboris** ⊂ index — Claves ordinatae in arbore: quaestioni per clavem et per intervallum servit - forma basium datorum
- **lectio_memoriae** ⊂ lectio — Status processu gestus legitur: variabilis universalis, hic_manens, aut memoria per monstratorem designata
- **lectio_externa** ⊂ lectio — Status extra processum legitur: plagula, terminale, rete, horologium
- **mutatio_memoriae** ⊂ mutatio — Status processu gestus mutatur: variabilis universalis, hic_manens, aut memoria vocantis per monstratorem
- **scriptio_externa** ⊂ mutatio — Status extra processum mutatur: plagula, terminale, rete
- **allocatio** ⊂ mutatio — Status allocatoris mutatur: memoria petita aut reddita
- **terminatio** ⊂ mutatio — Processus ipse finitur - mutatio quae gerentem omnium statuum ephemerorum tollit; ultima ideo mutationum
- **transactio** ⊂ mutatio — Mutatio composita atomica: tota fit aut nulla - partes eius numquam separatim visibiles
- **praeconditio** ⊂ assertio — Quod vocans debet: verum ante introitum, aut culpa vocantis est
- **postconditio** ⊂ assertio — Quod functio promittit: verum ante reditum, aut culpa functionis est
- **invarians** ⊂ assertio — Quod semper tenet: verum inter omnes mutationes rei - forma rei ipsa, ut propositio custodita
- **assertio_statica** ⊂ assertio — In compilatione probata: propositio quam compilator iudicat, exsecutione non indiget
- **assertio_probationis** ⊂ assertio — In probationibus vivens: affirmatio de effectu exspectato post actionem
- **explicativum** ⊂ commentarium — CUR, non quid - homini legenti id quod codex ipse dicere non potest: consilium, vinculum, causa electionis
- **documentarium** ⊂ commentarium — Promissum interfaciei descriptum: pars contractus, iuxta declarationem habitans
- **monitum_futuri** ⊂ commentarium — Intentio de opere futuro in artefacto praesenti servata (TODO, FIXME): debitum nominatum ubi debetur
- **titulus_iuris** ⊂ commentarium — Ius et licentia: homini legali inscriptum, saepe a nemine lectum, a machinis collectum
- **codex_sepultus** ⊂ commentarium — Codex exclusus commento: mendacium categoriae - codex vestem commentarii gerens, nec vivus nec mortuus
- **annotatio_machinae** ⊂ commentarium — Machinae inscripta per canalem neglectum: propositiones quas instrumenta legunt, lingua ipsa numquam
- **identificator_baptismalis** ⊂ identificator — Identitas DATA: nomen rei impositum (nomen proprium, ULID, UUID)
- **sigillum_contenti** ⊂ identificator — Identitas DERIVATA: ex ipso contento computata
- **instrumentum_lineae** ⊂ programma — Vocatur, agit, exit: compositioni per fistulas aptum (vide cliens
- **daemon** ⊂ programma — Perpetuum, sine terminali, petitionibus serviens: non finem quaerit sed statum servat
- **applicatio** ⊂ programma — Homini praesens, fenestris aut terminali: dum homo adest currit

### fabricatio (v8, genera 18)

- **postulatum** ⚙ — Intentio declarata ANTE opus: quod verum esse debet ut opus perfectum dicatur
- **facultas** — Quod opus praestare potest, ab utente aestimatum: unitas VALORIS
- **probatio** — Interrogatio exsecutabilis intentionis: scaena parata, actio exercita, effectus contra exspectationem collatus
- **vitium** ⚙ — Discrepantia inter intentionem et mores: programma facit quod scriptum est, non quod volitum - vitium in hiatu vivit
- **debitum_technicum** ⚙ — Imperfectio in opere CONSULTO suscepta ut aliquid citius praestetur, cum cognitione pretii posterioris: non defectus s
- **complicatio** ⚙ — Consideratio implementationis quae consilio praevideri NON POTEST sed opere incepto sola apparet, et consilium retract
- **consilium** ⚙ — Ordo operum propositus ad finem SEMEL petendum: quid quo ordine fiet, quid cui committitur, quid ante quid stare debet
- **depravatio** ⚙ — Status rei factus alius quam esse debuit, sive quis animadvertat sive non: non defectus faciendi (id vitium est) nec d
- **interventio** ⚙ — Actus in rem iam exsistentem susceptus ut servetur, sanetur, aut melior fiat: non factio (nihil novum oritur) nec damn
- **probatio_unitatis** ⊂ probatio — Elementum SOLUM probat, in insula: unum genus, unus mos, apparatus minimus
- **probatio_integrationis** ⊂ probatio — Elementa UNA probat: suturae ipsae interrogantur - nam vitia in suturis habitant, ubi assumptiones duorum elementorum 
- **probatio_regressionis** ⊂ probatio — Nata ex vitio: umbra vitii sanati custos posita, ne redeat
- **probatio_fumi** ⊂ probatio — "Vivitne omnino?" - levissima latissima: nihil profundum, omnia tacta
- **regressio** ⊂ vitium — Vitium REDIENS: sanatum olim, reversum - quod semel fractum et refectum, iterum fractum
- **consilium_computatum** ⊂ consilium — Consilium a machina ex statuum comparatione derivatum: quid scribendum, quid creandum, quid obstat - propositum, numqu
- **conservatio** ⊂ interventio — Interventio in opus artis aut monumentum ut corruptioni resistat aut lacunae expleantur
- **restauratio_aedificii** ⊂ interventio — Interventio in aedificium historicum: fulcire, reficere, partes amissas supplere
- **curatio_medica** ⊂ interventio — Interventio in corpus viventis ut morbo resistat aut functio restituatur

### formae_datorum (v0, genera 7)

- **forma_datorum** — Figura ordinandi data ab OPERATIONIBUS suis definita, non ab impletione: quae rogare licet, quid promittitur, quo pret
- **series** ⊂ forma_datorum — Ordo per indicem: locus numeratus, accessus per numerum, finis crescens
- **cauda** ⊂ forma_datorum — Primus intrat, primus exit: disciplina ordinis adventus - iustitia temporis
- **acervus** ⊂ forma_datorum — Novissimus primus: disciplina reditus - quod ultimum incepisti, primum finis
- **tabula_associativa** ⊂ forma_datorum — Clavis ad valorem, ordo nullus promissus: quaestio "quid sub hac clave?" pretio constanti
- **copia** ⊂ forma_datorum — Membra sine duplicatis: quaestio unica "inestne?"
- **arbor** ⊂ forma_datorum — Hierarchia: parens et filii, radix una, via unica ad quodque

### identificatio (v2, genera 17)

- **schema_identificandi** ⚙ ▣3 — Ratio instituta nomina rebus assignandi, ita ut nomen unum rem unam designet intra ambitum suum: structura definita, a
- **uuid** ⊂ schema_identificandi — Identificator 128 bitorum sine auctoritate centrali: spatium tam amplum ut collisio fortuita neglegi possit
- **ulid** ⊂ schema_identificandi — Identificator 128 bitorum cuius pars prior TEMPUS fert, ideo lexicographice ordinabilis: unde in indicibus ordinatis m
- **isbn** ⊂ schema_identificandi — Ratio libros identificandi ab industria libraria instituta: praefixum coetus, editor, titulus, nota custos
- **isbn_10** ⊂ isbn — Forma antiquior, decem characterum: nota custos modulo undecim, unde character 'X' pro valore decem - unicus in his ra
- **isbn_13** ⊂ isbn — Forma hodierna, tredecim digitorum: praefixo 978 aut 979 aucta, ut in spatium EAN codicum mercium conveniret - nota cu
- **issn** ⊂ schema_identificandi — Ratio SERIES identificandi (ephemerides, acta): octo digiti, nota custos modulo undecim cum 'X'
- **doi** ⊂ schema_identificandi — Identificator documentorum PERSISTENS et RESOLVABILIS: praefixum editoris (10
- **orcid** ⊂ schema_identificandi — Identificator PERSONARUM in re litteraria: sedecim digiti, nota custos ISO 7064 modulo 11-2 cum 'X'
- **iban** ⊂ schema_identificandi — Ratio rationum argentariarum trans fines: codex civitatis, duo digiti custodes, codex argentariae et numerus rationis 
- **e164** ⊂ schema_identificandi — Ratio numerorum telephonicorum mundi: codex civitatis et numerus nationalis, quindecim digitis non excedens
- **gtin** ⊂ schema_identificandi — Ratio mercium in commercio (EAN, UPC): praefixum societatis, numerus mercis, nota custos modulo decem ponderibus alter
- **uri** ⊂ schema_identificandi — Ratio generalissima: nomen cum schemate quod interpretationem regit (http, mailto, urn, et nostrum 'massa')
- **cas** ⊂ schema_identificandi — Numerus quo Chemical Abstracts Service substantias chemicas registrat: nihil de structura dicens, sed unicus et perpet
- **ndc** ⊂ schema_identificandi — Codex nationalis medicamentorum (Civitatum Foederatarum): tres partes - fabricator, productum (substantia, vis, forma)
- **rxnorm** ⊂ schema_identificandi — Vocabularium normatum quo medicamenta clinica nominantur et gradus inter se nectuntur: ingrediens, medicamentum clinic
- **numerus_dea** ⊂ schema_identificandi — Numerus quo auctoritas Americana eos registrat quibus substantias regulatas praescribere aut tractare licet: litterae 

### iudicium (v8, genera 54)

- **gradus_assensus** — Quanto firmitate animus propositioni adhaeret
- **fons** — Id ex quo propositio accepta est: opus, mensura, testis, corpus institutionis
- **observatio** — Notitia rei singularis accepta, quae iudicium fundare potest: quid videatur, quid sentiatur, quid metiatur
- **praeceptum** — Regula artis breviter dicta quae agentem in casibus non plene descriptis dirigit: peritia in sententiam contracta
- **iudicium** ⚙ — Propositio de re quam aliquis tenet: cum auctore, momento, fundamento, gradu assensus
- **annotatio** — Propositio quam instrumentum probat, extra programma currens: sine assensu, sine revisione per persuasionem - secundum
- **periculum** — Damnum quod accidere potest nec certum est: species iudicii de futuro, sed DUPLEX - probabilitate et gravitate simul c
- **ignotum** ⚙ — Absentia notitiae AGNITA et nominata: hiatus qui, quia nominatus est, tractari potest
- **investigatio** ⚙ — Inquisitio ordinata in causas eventus, iudicia de causis producens
- **secretum** ⚙ — Res cuius utilitas in eo consistit quod pauci eam sciunt: divulgata non corrumpitur nec perit sed OFFICIO SUO EXUITUR,
- **peritia_communis** ⚙ — Scientia aut ars quae in universitate residet nec in ullo membro tota est: participando traditur, non doctrina sola, e
- **attributio** ⊂ iudicium — Iudicium de eo QUIS FECERIT, quod duabus mensuris simul utitur: quanto assensu teneatur (certitudo, ut omne iudicium) 
- **fundamentum** — Id quo propositio nititur: non unde accepta sit (id fons est) nec quanto assensu teneatur (id gradus assensus) sed QUA
- **scientia** ⊂ gradus_assensus — Certa adhaesio cum causa: quod per demonstrationem aut per definitionem tenetur, sine formidine oppositi
- **opinio** ⊂ gradus_assensus — Adhaesio CUM FORMIDINE OPPOSITI: tenetur, sed contrarium fieri posse conceditur
- **fides** ⊂ gradus_assensus — Adhaesio propter AUCTORITATEM alterius, non propter rem visam: quod ab alio acceptum tenetur
- **suspicio** ⊂ gradus_assensus — Adhaesio levis, ad alteram partem inclinans sine firmitate: quod verisimile videtur
- **dubitatio** ⊂ gradus_assensus — Suspensio adhaesionis: neutram partem tenere
- **fons_scriptus** ⊂ fons — Opus citabile: liber, norma, charta
- **fons_mensurae** ⊂ fons — Instrumentum quod legit: thermometrum, horologium, machina
- **fons_testimonii** ⊂ fons — Persona quae rettulit: aegrota dolorem suum nuntians, testis rem visam
- **corpus_institutionis** ⊂ fons — Memoria machinae discentis: quod ex textibus innumeris hausit sine singulorum memoria
- **symptoma** ⊂ observatio — A subiecto ipso relatum: dolor, nausea, vertigo
- **signum** ⊂ observatio — Ab observatore deprehensum: febris, tumor, pallor
- **testimonium** ⊂ observatio — Ab alio relatum de re quam ipse vidit: testis, socius, acta priora
- **diagnosis** ⊂ iudicium — Iudicium de morbo aegroti: quid habeat
- **aestimatio** ⊂ iudicium — Iudicium de valore rei: quanti sit
- **praedictio** ⊂ iudicium — Iudicium de futuro: quid eventurum sit
- **sententia_iudicis** ⊂ iudicium — Iudicium auctoritate publica latum quod OBLIGATIONEM parit: non solum tenetur sed VINCIT
- **ignotum_notum** ⊂ ignotum — Quaestionem tenes, responsum non: "quanta sit mora huius ministerii ignoramus"
- **ignotum_ignotum** ⊂ ignotum — Quod nescis te nescire: quaestio ipsa nondum formata
- **indeterminatum** ⊂ ignotum — Nondum est quod sciatur: pretium anni venturi, exitus pugnae navalis crastinae
- **amissum** ⊂ ignotum — Scitum olim, nunc perditum: tabulae deletae, testis mortuus, acta rotata
- **retentum** ⊂ ignotum — Aliquis scit et non dicit: ignorantia non in mundo sed in te, ex voluntate alterius
- **impertinens** ⊂ ignotum — Quaestio ad rem non pertinet: "quod nomen virgineum huius societatis?" MOTUS: schema EMENDA - hiatus enim non in notit
- **controversum** ⊂ ignotum — Responsa PLURA et pugnantia: non nullum sed NIMIS
- **investigatio_praeventiva** ⊂ investigatio — Ad praeventionem sola ordinata, culpa expresse exclusa: forma aviatoria, et in medicina conventus mortalitatis et morb
- **inquisitio_forensis** ⊂ investigatio — Ad culpam et damna determinanda: finis legitimus et necessarius, sed ALIUS
- **recensio_post_mortem** ⊂ investigatio — Investigatio praeventiva in arte programmandi post incidens: forma aviatoria mutuata, saepe sine separatione quae eam 
- **clavis_privata** ⊂ secretum — Dimidium secretum paris cryptographici: quod signat aut resignat, et cuius comes publicus libere diffunditur
- **verbum_arcanum** ⊂ secretum — Series characterum qua persona se probat: secretum ab homine memorandum, unde debilitas eius
- **secretum_negotii** ⊂ secretum — Notitia commercialis cuius valor ex ignorantia competitorum venit: ratio, index clientium, processus
- **sigillum_confessionis** ⊂ secretum — Secretum sacramentale quod confessarius nullo modo, nulla causa, nulli prodere potest
- **notitia_pascui** ⊂ peritia_communis — Scientia gregis de finibus pascui sui in monte non saepto: quo eundum, ubi manendum, quando descendendum
- **ars_officinae** ⊂ peritia_communis — Modus operandi quem officina tenet: quid quo ordine, quid caveatur, quid non scribatur quia omnes sciunt
- **memoria_operis** ⊂ peritia_communis — Scientia coetus de opere suo quae in fontibus non est: cur hoc ita factum sit, quid temptatum et reiectum, ubi pericul
- **autopsia** ⊂ fundamentum — Ipse vidi: cognitio ex sensu proprio, nullo medio
- **mensuratio** ⊂ fundamentum — Instrumento mensuratum: fundamentum quod NUMERUM et incertitudinem simul fert
- **documentum_scriptum** ⊂ fundamentum — Scriptura rem attestans: acta, instrumentum, tabula, epistula
- **auctoritas_testis** ⊂ fundamentum — Alius mihi dixit: fundamentum quod in fidem alterius resolvitur
- **illatio** ⊂ fundamentum — Ex aliis notis conclusum: nemo id vidit nec mensuravit, sed ex his sequitur
- **comparatio_stili** ⊂ fundamentum — Ex similitudine formae iudicatum: haec manus illam refert
- **coniectura_periti** ⊂ fundamentum — Peritus ita censet, ratione non plene explicata: aestimatio ex usu longo nata
- **praesumptio** ⊂ fundamentum — Assumptum donec contrarium probetur: fundamentum quod se fundamentum non esse fatetur

### lingua_programmandi (v5, genera 5)

- **lingua_programmandi** ⚙ — Systema signorum formale ad computationes exprimendas: grammatica definita, semantica praescripta (plus minusve strict
- **lingua_iussuum** ⊂ lingua_programmandi — Verbum primum = PROCESSUS ALIENUS vocatus, non functio propria: lingua ad imperia oritur (interactiva), scriptilis fit
- **versio_linguae** ⊂ versio.versio — Editio linguae nominata et citabilis: eventus "edi" in rem conversus
- **implementatio_linguae** — Programma quod linguam exsequitur: compilator, interpres, aut mixtum
- **verbum_clavis** — Verbum quod lingua sibi vindicat: grammaticae servit, identificatori negatur

### locus (v1, genera 4)

- **locus** — Pars spatii determinata ubi res corporeae esse possunt: punctum aut area in terra
- **inscriptio** — Nomen loci ad inveniendum conventum: via, numerus, urbs, codex postalis
- **iurisdictio** — Potestas legum ferendarum et exigendarum super loco: municipium, provincia, civitas
- **aedificium** ▣1 — Structura loco fixa quam homines occupant: res corporea quae moveri non potest - immobilitas essentia eius, unde "res 

### mensura (v2, genera 11)

- **magnitudo_mensurabilis** — Id quod mensurari potest: longitudo, massa, tempus, temperatura
- **unitas_mensurae** — Partitio conventa magnitudinis mensurabilis, qua quantitates numerari possunt
- **quantitas** — Numerus CUM UNITATE: mensura rei determinata
- **incertitudo** — Mensura eius quantum de valore vero ignoremus: intervallum circa valorem mensuratum intra quod valor verus, probabilit
- **instrumentum_mensurae** ⊂ corporalia.instrumentum ⚙ — Instrumentum quod rem suam NON MUTAT sed characterizat: fornax panem coquit, statera panem relinquit qualis erat
- **longitudo** ⊂ magnitudo_mensurabilis — 
- **massa** ⊂ magnitudo_mensurabilis — 
- **tempus_intervallum** ⊂ magnitudo_mensurabilis — 
- **temperatura** ⊂ magnitudo_mensurabilis — 
- **capacitas** ⊂ magnitudo_mensurabilis — 
- **numerositas** ⊂ magnitudo_mensurabilis — 

### morbus (v2, genera 5)

- **morbus** ⚙ — Dispositio viventis praeter naturam, qua operatio debita impeditur: defectus ab eo quod naturae rei debetur
- **morbus_infectiosus** ⊂ morbus — Ab organismo alieno illatus (bacterio, viro, fungo): species sola quae AGENTEM habet - vivens aliud quod in vivente vi
- **morbus_geneticus** ⊂ morbus — In ipsa constitutione generata: praesens ab initio, etiam dum latet
- **morbus_degenerativus** ⊂ morbus — Ex usu et tempore: partes deficiunt quia diu operatae sunt
- **vulnus** ⊂ morbus — Ab externa vi illatum: ictus, sectio, ustio

### necessitudines (v1, genera 0)


### norma (v5, genera 15)

- **norma** ⚙ — Documentum publicum quod formam praescribit: quid conformitas significet definit
- **lex** ⚙ — Ordinatio rationis ad bonum commune, ab eo qui curam communitatis habet promulgata (Thomas, ST I-II q
- **classis_normativa** ⚙ — Categoria a norma constituta in quam res assignantur, cui consequentiae (obligationes, prohibitiones, procedurae) ADHA
- **fictio_iuris** — Praeceptum quo lex rem aliter se habentem TAMQUAM ALITER tractari iubet, sciens ita non esse, propter bonum quod alite
- **lex_aeterna** ⊂ lex — Ratio divinae sapientiae universum gubernans: fons omnium legum, ipsa a nullo lata
- **lex_naturalis** ⊂ lex — Participatio legis aeternae in creatura rationali: quod homo ratione sua cognoscit esse faciendum aut vitandum, nullo 
- **lex_humana** ⊂ lex — Ex lege naturali per rationem derivata et ab auctoritate humana lata: civilis aut canonica
- **lex_divina_positiva** ⊂ lex — A Deo revelata, ratione sola non cognoscibilis: quod humana ratio ad finem supernaturalem non attingit
- **consuetudo** ⊂ lex — Usus diuturnus communitatis cum animo se obligandi, a legislatore tolerata: lex NON SCRIPTA quae tamen obligat
- **schedula_substantiarum** ⊂ classis_normativa — Classes I-V quibus substantiae regulatae assignantur secundum abusus periculum et usum medicum acceptum: quaeque oblig
- **gradus_eventus_aviatorii** ⊂ classis_normativa — Gradus quibus eventus aviatorii distinguuntur - accidens, incidens grave, incidens - quorum quisque obligationem nunti
- **classis_periculi_mercium** ⊂ classis_normativa — Classes quibus merces periculosae in vectura assignantur (explosiva, gasa, inflammabilia, corrosiva
- **adoptio** ⊂ fictio_iuris — Fictio qua adoptans parens fit ad omnes fere effectus, sciente lege eum non genuisse
- **mors_praesumpta** ⊂ fictio_iuris — Fictio qua absens per tempus statutum mortuus habetur, ut hereditas, matrimonium, tutela solvi possint
- **personalitas_iuridica** ⊂ fictio_iuris — Fictio qua coetus (aut massa bonorum) persona habetur: possidet, contrahit, agit, convenitur suo nomine

### obligatio (v9, genera 41)

- **obligatio** ⚙ — Iuris vinculum quo quis alicui aliquid praestare adstringitur (Institutiones): non res sed VINCULUM inter personas - d
- **contractus** — Conventio duorum (aut plurium) quae obligationes parit: consensus est fundamentum - ideo utrumque ligat, et neuter sol
- **licentia** ⚙ — Concessio ab auctoritate data ut quid liceat quod aliter non liceret: UNILATERALIS (consensu accipientis non indiget),
- **tributum** ⊂ obligatio — Obligatio ad MAGISTRATUM, ex lege orta, non ex consensu: debetur quia lex ita statuit, non quia quis promisit
- **titulus** — Attestatio publica et permanens ab auctoritate data, quod aliquis aliquid consecutus est aut aliquid EST: non permissi
- **iussum** ⚙ — Directivum ab eo qui potestatem habet latum, quod agendum quid, a quo, et intra quod tempus determinat, et quod ACTU L
- **fiducia** ⚙ — Tentio rei suo nomine et titulo, sed ALIENO COMMODO et sub officiis: fiduciarius dominus apparet et dominus non est, q
- **petitio** ⚙ — Rogatio ad alium directa ut aliquid faciat aut praestet, quam ille CONCEDERE AUT NEGARE potest sine ulla violatione: q
- **delegatio** ⚙ — Potestas alterius nomine agendi, ita ut acta delegati delegantem OBLIGENT tamquam ipse egisset: non licentia agendi se
- **tractatus** ⊂ contractus ⚙ — Conventio inter eos qui superiorem non habent: nullum tribunal supra partes est, unde consensus non semel datur sed BI
- **limitatio** — Declaratio unilateralis qua pars, tractatum accipiens, effectum quorundam articulorum in se excludit aut mutat: consen
- **portio_communis** — Pars determinata rei communis finitae qua alicui uti licet, ceteris ex eodem fonte utentibus: ius mensuratum, non meru
- **obligatio_ex_contractu** ⊂ obligatio — Ex consensu orta: quod promisisti praestandum est
- **obligatio_ex_delicto** ⊂ obligatio — Ex damno illato orta: qui nocuit reparare tenetur - obligatio quam nemo VOLUIT, tamen vera
- **obligatio_ex_lege** ⊂ obligatio — Ex lege ipsa orta: tributa, alimenta - nec pactum nec damnum, sed praeceptum publicum
- **emptio_venditio** ⊂ contractus — Res pro pretio: contractus omnium frequentissimus
- **locatio_conductio** ⊂ contractus — Usus rei aut opera pro mercede: sub hac specie ius Romanum et habitationem et LABOREM ponit - conductio operarum, unde
- **societas_contractus** ⊂ contractus — Plures in commune negotium: unde organizatio ipsa nascitur - contractus qui personam novam (societatem) parit
- **mandatum** ⊂ contractus — Alius pro me agit: fundamentum repraesentationis - procurator, agens
- **commodatum** ⊂ contractus — Res certa alicui GRATIS utenda datur, eadem reddenda: non pretium sed beneficium, et res ipsa redit, non similis
- **portorium** ⊂ tributum — Tributum ex TRANSITU limitis: quod merces limitem transeuntes debent
- **vectigal** ⊂ tributum — Reditus publicus ex re publica: ex agris, salinis, viis
- **decuma** ⊂ tributum — Pars decima fructuum: tributum ratione PARTIS, non summae - forma antiquissima, quae quantitatem numeratam non poscit 
- **tributum_capitis** ⊂ tributum — Ex capite, non ex re: debetur quia es, non quia habes
- **gradus_academicus** ⊂ titulus — Ab universitate collatus post studia probata: baccalaureatus, licentia (nomen antiquum!), doctoratus
- **character_indelebilis** ⊂ titulus — Signum spirituale quod sacramenta tria imprimunt (baptismus, confirmatio, ordo): ontologicum, unicum, irreiterabile
- **titulus_honoris** ⊂ titulus — Ob merita collatus, non ob probationem: honores, dignitates
- **praescriptio_medica** ⊂ iussum — Iussum medici quo pharmacopola medicamentum aegro certo dispensare iubetur: substantia, vis, quantitas, modus sumendi,
- **mandatum_iudicis** ⊂ iussum — Iussum a iudice latum quo quis aliquid facere aut omittere iubetur: vis publica post illud stat
- **iussum_emptionis** ⊂ iussum — Iussum quo organizatio a venditore merces poscit, pretio et condicionibus praestitutis: quod "purchase order" vocatur
- **iussum_operis** ⊂ iussum — Iussum internum quo opus certum alicui committitur, saepe cum loco, tempore, materiis: quod "work order" vocatur
- **iussum_ne_resuscitetur** ⊂ iussum — Iussum quo medicus, aegro consentiente aut prius declarante, resuscitationem cardiopulmonalem OMITTI iubet
- **petitio_assecurationis** ⊂ petitio — Rogatio ad assecuratorem ut damnum ex polliciatione praestet: quod "claim" vocatur
- **petitio_remota** ⊂ petitio — Rogatio ad programma per rete missa ut operationem agat aut datum reddat
- **supplicatio** ⊂ petitio — Rogatio ad auctoritatem directa quae de re sua statuere potest: petitio venia, gratiae, licentiae
- **plena_potestas** ⊂ delegatio — Potestas legato data ut tractatum nomine principis concludat: exhibenda alteri parti ante negotiationem, et ambitu suo
- **procuratio** ⊂ delegatio — Potestas negotia alterius gerendi, instrumento publico plerumque data: emere, vendere, agere in iudicio
- **delegatio_canonica** ⊂ delegatio — Facultas ab ordinario data ut alius potestatem quae ipsi competit exerceat: absolvendi, assistendi matrimonio, confirm
- **portio_pascui** ⊂ portio_communis — Numerus pecorum quem tenens praedii in communi pascere licet: "stinta" Anglica, curia maneriali custodita
- **portio_piscandi** ⊂ portio_communis — Pondus piscium quod navi certae per annum capere licet: quod "quota transferibilis" vocatur
- **portio_computandi** ⊂ portio_communis — Copia petitionum aut operationum quam cliens intra tempus certum facere licet: quod "limes celeritatis" vocatur

### operatio (v2, genera 7)

- **ambitus** — Locus ordinatus ubi programmata currunt: machinae, figurationes, data, nomina
- **pensum** ⚙ — Opus certum alicui commissum: quid agendum, a quo, quo statu
- **incidens** ⚙ — Interruptio aut deminutio ministerii in ambitu vero: non genus vitii sed vitium EXPOSITUM - idem defectus vitium sempe
- **processus** ⚙ ▣5 — Programma in exsecutione: instantia formae, statum proprium gerens, dum vivit allocutione digna
- **ambitus_fabricae** ⊂ ambitus — Ubi faber ipse operatur: fractura nihil nocet, ideo libertas maxima
- **ambitus_probationis** ⊂ ambitus — Producentem imitatur ut errores ante producentem appareant: pretium eius in FIDELITATE stat - quo minus similis, eo mi
- **ambitus_producens** ⊂ ambitus — Ubi homines veri re vera utuntur: error hic damnum verum est, non nuntius

### opus_scriptum (v2, genera 6)

- **opus_scriptum** ⚙ — Creatio intellectualis verbis constans, ab expressionibus suis distincta: "Moby-Dick" opus unum est sive Anglice sive 
- **expressio** — Realizatio operis in signis certis: textus hic, non alius
- **editio** — Corpus editum expressionis: quod praelum vere protulit - charta, forma, paginae, index ISBN
- **codex** ⚙ — Exemplar unum corporeum editionis: hoc quod manu tenes, cum maculis, notis marginalibus, historia possessionis propria
- **periodicum** ⊂ opus_scriptum — Opus quod PERFECTIONEM non petit: in fasciculis continuatur dum curatores volunt - machina compositionis parentis ei f
- **fasciculus** — Pars periodici tempore signata: numerus unus, die suo editus

### organizatio (v9, genera 17)

- **universitas** — Coetus in finem communem ordinatus qui UT UNUM agit et mutatis membris PERDURAT: haec perduratio eum a mero acervo dis
- **organizatio** ⊂ universitas — Universitas PERSONARUM: quod membra rationalia sunt omnia sequentia parit - fines eligi possunt et mutari, officia con
- **colonia** ⊂ universitas — Universitas VIVENTIUM quorum membra non eligunt: ordo ad finem non consilio sed natura constat, unde nec officia const
- **persona_iuridica** ⊂ organizatio — Subiectum iurium et obligationum in iure: possidet, contrahit, agit et convenitur SUO nomine, non nomine membrorum
- **officium** ⚙ — Munus stabiliter constitutum cum oneribus, potestatibus et condicionibus SUIS, quod tenenti suo NON inhaeret: idem off
- **corpus_normans** ⊂ organizatio — Finis proprius: normas edere et custodire - descriptiones auctoritativas quibus alii conformantur
- **societas** ⊂ organizatio — Finis proprius: quaestus per opera vendita; possessores habet quibus respondet
- **magistratus** ⊂ organizatio — Auctoritas ex IMPERIO super loco, non ex consensu (ut corpus normans) nec ex quaestu (ut societas): leges fert quibus 
- **colonia_apium** ⊂ colonia — Universitas apum cum regina una, operariis, fucis: quam apiarius CURAT et numerat, non apes singulas
- **grex** ⊂ colonia — Universitas animalium quam homo ad usum suum tenet: oves, boves, equi - numerata, curata, ex qua singula tolluntur et 
- **universitas_personarum** ⊂ persona_iuridica — Persona iuridica ex MEMBRIS constans: collegium, societas, communitas
- **universitas_rerum** ⊂ persona_iuridica — Persona iuridica ex BONIS AD FINEM DESTINATIS constans, SINE membris: fundatio, pia causa
- **parochus** ⊂ officium — Officium curae animarum in paroecia: pastor proprius, cum oneribus praedicandi, sacramenta ministrandi, tabulas custod
- **episcopus** ⊂ officium — Officium regiminis dioecesis, ordine episcopali praesuppositum
- **notarius** ⊂ officium — Officium publicum fidem instrumentis praestans
- **curator** ⊂ officium — Officium bona aliena ad finem alienum administrandi: fiducia, non dominium
- **praeses_collegii** ⊂ officium — Officium collegio praesidendi: sedes una inter pares cum potestate ordinandi, non imperandi

### persona (v7, genera 1)

- **persona** ⚙ — Naturae rationabilis individua substantia (Boethius)

### pharmacon (v2, genera 10)

- **substantia_chemica** — Genus materiae compositione determinatum: quod omnia exemplaria eiusdem substantiae communicant, ubicumque et quandocu
- **medicamentum** — Substantia (aut plures) cum VI et FORMA determinatis ad morbum tractandum praeparata: quod praescriptio nominat et quo
- **coctura** ⚙ — Omnia quae uno cursu fabricationis facta sunt, provenientiam communem et ideo FATUM COMMUNE habentia: quod contaminatu
- **interactio** — Relatio inter res duas (aut plures) quarum praesentia simultanea effectum mutat quem singulae haberent: non proprietas
- **medicamentum_genericum** ⊂ medicamentum — Post privilegium inventionis elapsum ab aliis fabricatum, aequivalentia asserta, nomine substantiae venditum
- **medicamentum_nominatum** ⊂ medicamentum — Nomine mercatorio venditum, plerumque ab eo qui invenit - idem medicamentum, nomen aliud, pretium aliud
- **medicamentum_compositum** ⊂ medicamentum — In officina pharmacopolii ad singularem aegrum praeparatum, quia forma mercatoria non convenit (dosis infantilis, alle
- **interactio_medicamentorum** ⊂ interactio — Inter medicamenta duo: alterum alterius concentrationem aut effectum mutat, plerumque per enzymata hepatis aut per via
- **interactio_cum_cibo** ⊂ interactio — Inter medicamentum et cibum: absorptio mutata, aut substantia in cibo eandem viam occupans
- **incompatibilitas_mercium** ⊂ interactio — Inter merces periculosas quae simul vehi aut condi non debent: oxidantia cum inflammabilibus, acida cum basibus

### plagula_computatralis (v3, genera 22)

- **plagula_computatralis** ⚙ — Sequentia octetorum NOMINATA in systemate plagularum: identitas quae perdurat dum contentum mutatur (ut universitas mu
- **plagula_fontis** ⊂ plagula_computatralis — Textus a personis scriptus, a personis ET translatoribus lectus: forma exsecutionis futurae, non exsecutio ipsa
- **scriptum_exsecutabile** ⊂ plagula_computatralis — Directe exsequenda sine translatione praevia: crena (#!) interpretem suum ipsa nominat - plagula quae scit quomodo cur
- **plagula_notationis** ⊂ plagula_computatralis — Contentum = prosa cum structura interposita: signa structuram ferunt, textus sensum - pro lectore humano per machinam 
- **plagula_figurationis** ⊂ plagula_computatralis — A programmate in initio lecta ut mores suos formet: declarativa, non exsecutiva - dicit quid, numquam quomodo
- **plagula_datorum** ⊂ plagula_computatralis — Contentum pro programmatibus primo: structura stricta, lector humanus hospes non dominus
- **plagula_sqlite** ⊂ plagula_datorum — Basis datorum integra in plagula una: tabulae, indices, transactiones - omnia intra sequentiam octetorum unam, sine se
- **systema_plagularum** ▣3 — Ordinatio quae sequentiis octetorum NOMINA, locum in arbore, et notas circumstantes dat: quod inter medium (discum, re
- **directorium** — Continens nominatum plagularum et directoriorum aliorum: nodus arboris nominum
- **fons_c** ⊂ plagula_fontis — Corpus translationis C: definitiones functionum et datorum
- **caput_c** ⊂ plagula_fontis — Caput C: declarationes communicandae - interfacies, non corpus
- **fons_python** ⊂ plagula_fontis — 
- **fons_rust** ⊂ plagula_fontis — 
- **scriptum_conchae** ⊂ scriptum_exsecutabile — Scriptum linguae iussuum: imperia quae manu dari possent, conservata
- **plagula_xml** ⊂ plagula_notationis — 
- **plagula_stml** ⊂ plagula_notationis — Varians XML domus nostrae: notatio annotationum, quaestionum silvae, et horum ipsorum exemplarium
- **plagula_markdown** ⊂ plagula_notationis — 
- **plagula_toml** ⊂ plagula_figurationis — 
- **plagula_plist** ⊂ plagula_figurationis — 
- **plagula_json** ⊂ plagula_datorum — 
- **plagula_csv** ⊂ plagula_datorum — 
- **volumen** ⊂ plagula_sqlite — Plagula sqlite schemate domus nostrae: acta (veritas, solum-appende), massae (contentum sigillo addressatum), plagulae

### planta (v2, genera 7)

- **planta** ⊂ corporalia.vivens ⚙ ▣6 — Vivens quod se ex loco non movet et lucem in cibum vertit (photosynthesis): radicibus haeret, per partes crescit, semi
- **rosaceae** ⊂ planta — Flores quinque-petali plerumque, stipulae ad folia, fructus varii (pomum, drupa, aggregatus) - familia late diversa qu
- **rosa** ⊂ rosaceae — Caules aculeati, folia imparipinnata, fructus hypanthium carnosum (cynorrhodon) - non pomum
- **malus** ⊂ rosaceae — Fructus POMUM (hypanthium carnosum quod semina includit), non drupa nec cynorrhodon
- **rosa_canina** ⊂ rosa — Rosa silvestris Europae: flores simplices roseo-albi, cynorrhoda rubra vitamine C dives, aculei recurvi
- **malus_domestica** ⊂ malus — Malus culta, ex hybridatione longa orta (M
- **granny_smith** ⊂ malus_domestica — Cultivar Australianus: fructus viridis, acidus, durus, conservationi aptus

### praescriptum (v4, genera 8)

- **procedura** — Ordo operationum ad finem: introitus declarati, gradus ordinati, exitus exspectatus
- **procedura_automata** ⊂ procedura ⚙ — A MACHINA exsecuta, ideo AMBIGUITATIS TOLERANTIA NULLA: ubi praescriptum "donec aureum fiat" dicere potest quia homo i
- **praescriptum** ⊂ procedura ⚙ ▣3 — Ordo operationum ad finem, ab homine exsequendus: introitus declarati (materiae, instrumenta), gradus ordinati, exitus
- **gradus_praescripti** — Operatio una in ordine: actio, fortasse cum quantitate, fortasse cum mora, fortasse cum conditione finiendi
- **versio_praescripti** ⊂ versio.versio — Editio praescripti nominata et citabilis: quod praescriptum per emendationes transit, identitate manente ("receptum av
- **index_verificationis** ⊂ procedura — CONFIRMAT quod factum sit; non docet quomodo fiat
- **fistula_integrationis** ⊂ procedura_automata — Procedura quae fontes mutatos aedificat et probat: porta quae opus ante coniunctionem iudicat
- **depositio** ⊂ procedura_automata — Procedura quae artefactum in ambitum ponit: actus qui mutat quid re vera currat - unde in ambitu producente gravissima

### schema (v1, genera 9)

- **schema** — Forma datorum declarata: figura quam res eiusdem generis implere debent - campi nominati, genera valorum, necessitates
- **campus** — Locus nominatus in schemate: nomen, genus valoris, necessitas
- **genus_valoris** — Genus valoris quod campus tenere potest: INTENTIO valoris, non repraesentatio machinae - s32 et char* realizationes su
- **textus** ⊂ genus_valoris — Series characterum libera: nomen, titulus, prosa
- **numerus** ⊂ genus_valoris — Quantitas: comparari, ordinari, computari potest
- **dies** ⊂ genus_valoris — Punctum in tempore civili: ordinandum, intervallis mensurandum
- **veritas** ⊂ genus_valoris — Duorum alterum: est aut non est
- **electio** ⊂ genus_valoris — Valor e copia clausa optionum: quod optiones declaratae sunt, forma valores novos non timet
- **citatio** ⊂ genus_valoris — Valor qui RES ALIA est: campus rem citat, non describit (in paletta domus "relatio" dictum)

### systema_operativum (v5, genera 4)

- **systema_operativum** ⚙ — Programma primarium quod machinam possidet: inter ferrum et programmata mediat - memoriam distribuit, processus custod
- **systema_unixiforme** ⊂ systema_operativum — Ad exemplar Unix formatum: arbor plagularum una a radice, "omnia plagulae sunt", processus cum fistulis componendi, co
- **distributio_systematis** — Collectio curata: nucleus + instrumenta + ordinationes + gestor fasciculorum, ab uno curatore electa, probata, edita
- **versio_systematis** ⊂ versio.versio — Editio systematis nominata: quam installatio vere currit - "Darwin 23

### tempus (v5, genera 14)

- **momentum** — Punctum in tempore: quando aliquid accidit
- **intervallum** — Spatium temporis inter duo momenta: initium, finis, ideo duratio
- **recurrentia** — Regula quae momenta (aut intervalla) gignit: "omni die hora quarta", "omni die Lunae", "prima die cuiusque mensis"
- **recurrentia_computata** ⊂ recurrentia — Momenta non ex periodo simplici sed ex ALGORITHMO oriuntur: regula ipsa calculum continet, saepe corpora caelestia aut
- **recurrentia_per_usum** ⊂ recurrentia — Non tempore sed USU ACCUMULATO regitur: horis operis, cyclis, spatio confecto
- **horarium** — Recurrentiae in ordinem compositae quibus opus regitur: horae apertionis, vices operariorum, ordo coctionis
- **calendarium** — Systema quo dies nominantur et ordinantur: menses, hebdomadae, anni, dies festi
- **reservatio** ⚙ — Vindicatio capacitatis futurae: intervallum et res (sedes, cella, hora medici) alicui SERVATA ne alteri detur
- **pascha** ⊂ recurrentia_computata — Dominica post plenilunium quod aequinoctium vernum sequitur: computus lunisolaris, unde festa mobilia tota pendent (Qu
- **dies_negotiorum** ⊂ recurrentia_computata — "Post dies negotiorum triginta": dies festis et diebus Solis exclusis - regula quae ALIAM recurrentiam (calendarium fe
- **dies_ordinalis_mensis** ⊂ recurrentia_computata — "Tertia feria secunda cuiusque mensis": ordo intra mensem, non intervallum fixum - unde intervalla inaequalia (28 aut 
- **per_horas_operis** ⊂ recurrentia_per_usum — Horis quibus res re vera operata est
- **per_cyclos** ⊂ recurrentia_per_usum — Numero vicium quibus res a quiete ad opus et retro transiit: ubi damnum ex TRANSITU oritur, non ex duratione (metallum
- **per_spatium** ⊂ recurrentia_per_usum — Distantia confecta: vehiculis propria

### vectura (v1, genera 11)

- **iter** ⚙ — Motus rei aut personae a loco ad locum, proposito susceptus: initium, finis, via, tempus
- **via** — Semita per quam iri potest: res publica plerumque, itineribus innumeris communis
- **vehiculum** ⊂ corporalia.instrumentum — Instrumentum cuius usus MOTUS est: se movet et alia fert
- **onus** ⚙ — Res corporeae sub itinere: non genus rerum sed CONDICIO earum - idem dolium onus est dum fertur, merx dum venditur, su
- **via_terrestris** ⊂ via — Per terram: strata, semita, via ferrata
- **via_maritima** ⊂ via — Per mare: nullo opere facta plerumque, sed vera - cursus quos naves servant propter ventos, fluctus, praedones
- **via_aeria** ⊂ via — Per aerem, corridoribus a magistratibus definitis: via quae LEGE sola exsistit - nec aedificata nec naturalis, sed non
- **navis** ⊂ vehiculum — Vehiculum aquae: capacitatis maximae, velocitatis minimae - unde onera gravia et vilia mari eunt, levia et cara aere
- **currus_onerarius** ⊂ vehiculum — Vehiculum viae terrestris: solum quod ab ostio ad ostium pervenit - unde ultimum iteris membrum fere semper eius
- **tramen** ⊂ vehiculum — Vehiculum viae ferratae: viam PROPRIAM postulat, et ideo quo via ferrata non it, non it - vinculum durissimum omnium v
- **aeroplanum** ⊂ vehiculum — Vehiculum aeris: velocissimum, capacitatis minimae pro pretio - electio cum tempus pretium superat

### versio (v1, genera 1)

- **versio** ⚙ — Editio rei nominata et citabilis, quae rem ipsam per mutationem manentem SIGNAT: res una est, versiones multae, et ver

## III. Index alphabeticus (genera)

| genus | exemplar |
|---|---|
| acervus | formae_datorum |
| acta | acta |
| acta_auditus | acta |
| acta_publica | acta |
| acta_transactionum | acta |
| acta_versionum | acta |
| actor | communicatio |
| actus_iuridicus | actus |
| actus_sollemnis | actus |
| adoptio | norma |
| aedificium | locus |
| aeroplanum | vectura |
| aestimatio | iudicium |
| allocatio | elementa_programmatis |
| ambiguitas | documentatio |
| ambitus | operatio |
| ambitus_fabricae | operatio |
| ambitus_probationis | operatio |
| ambitus_producens | operatio |
| ambulatio_lectoris | documentatio |
| amissum | iudicium |
| animal | animal |
| annotatio | iudicium |
| annotatio_machinae | elementa_programmatis |
| apis | animal |
| apocha_horrei | commercium |
| apostilla | acta |
| applicatio | elementa_programmatis |
| arbor | formae_datorum |
| architectura_machinae | architectura_machinae |
| ars_officinae | iudicium |
| artefactum | corporalia |
| artefactum_aedificationis | elementa_programmatis |
| assertio | elementa_programmatis |
| assertio_probationis | elementa_programmatis |
| assertio_statica | elementa_programmatis |
| attestatio | acta |
| attestatio_civilis | acta |
| attributio | iudicium |
| auctoritas_testis | iudicium |
| autopsia | iudicium |
| baptismus | actus |
| benedictio | actus |
| bibliotheca | elementa_programmatis |
| calendarium | tempus |
| campus | schema |
| canis | animal |
| capacitas | mensura |
| caput_c | plagula_computatralis |
| cas | identificatio |
| cauda | formae_datorum |
| causa | causa |
| causa_efficiens | causa |
| causa_finalis | causa |
| causa_formalis | causa |
| causa_materialis | causa |
| character_indelebilis | obligatio |
| charta_onerum | commercium |
| cibus | corporalia |
| citatio | schema |
| classis_normativa | norma |
| classis_periculi_mercium | norma |
| clausura_rationis | acta |
| clavis_privata | iudicium |
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
| codex_sepultus | elementa_programmatis |
| collatio_tituli | actus |
| colonia | organizatio |
| colonia_apium | organizatio |
| commentarium | elementa_programmatis |
| commissio_notarii | acta |
| commodatum | obligatio |
| comparatio_stili | iudicium |
| complicatio | fabricatio |
| conditio | acta |
| confectio_instrumenti | actus |
| confirmatio | actus |
| coniectura_periti | iudicium |
| consecratio_loci | actus |
| conservatio | fabricatio |
| consilium | fabricatio |
| consilium_computatum | fabricatio |
| constans | elementa_programmatis |
| constans_numerica | elementa_programmatis |
| constans_textualis | elementa_programmatis |
| consuetudo | norma |
| contaminatio | corporalia |
| contractus | obligatio |
| controversum | iudicium |
| copia | formae_datorum |
| corpus_institutionis | iudicium |
| corpus_normans | organizatio |
| corruptio | corporalia |
| curatio_medica | fabricatio |
| curator | organizatio |
| currus_onerarius | vectura |
| daemon | elementa_programmatis |
| damnum | corporalia |
| debitum_technicum | fabricatio |
| deceptio | documentatio |
| decuma | obligatio |
| defectus_documenti | documentatio |
| delegatio | obligatio |
| delegatio_canonica | obligatio |
| depositio | praescriptum |
| depravatio | fabricatio |
| diagnosis | iudicium |
| dictionarium | documentatio |
| dies | schema |
| dies_negotiorum | tempus |
| dies_ordinalis_mensis | tempus |
| directorium | plagula_computatralis |
| distributio_systematis | systema_operativum |
| documentarium | elementa_programmatis |
| documentum_scriptum | iudicium |
| doi | identificatio |
| dubitatio | iudicium |
| e164 | identificatio |
| editio | opus_scriptum |
| electio | schema |
| emptio_venditio | obligatio |
| episcopus | organizatio |
| eucharistia | actus |
| eventum | communicatio |
| exemplum | documentatio |
| exorcismus | actus |
| explicativum | elementa_programmatis |
| expressio | opus_scriptum |
| exsecutabile_ligatum | elementa_programmatis |
| facultas | fabricatio |
| fasciculus | opus_scriptum |
| fictio_iuris | norma |
| fides | iudicium |
| fiducia | obligatio |
| fistula_integrationis | praescriptum |
| fons | iudicium |
| fons_c | plagula_computatralis |
| fons_mensurae | iudicium |
| fons_python | plagula_computatralis |
| fons_rust | plagula_computatralis |
| fons_scriptus | iudicium |
| fons_testimonii | iudicium |
| forma_datorum | formae_datorum |
| fractura | corporalia |
| functio | elementa_programmatis |
| functio_pura | elementa_programmatis |
| fundamentum | iudicium |
| genus_valoris | schema |
| gradus_academicus | obligatio |
| gradus_assensus | iudicium |
| gradus_eventus_aviatorii | norma |
| gradus_praescripti | praescriptum |
| granny_smith | planta |
| grex | organizatio |
| gtin | identificatio |
| homo | animal |
| horarium | tempus |
| iban | identificatio |
| identificator | elementa_programmatis |
| identificator_baptismalis | elementa_programmatis |
| ignotum | iudicium |
| ignotum_ignotum | iudicium |
| ignotum_notum | iudicium |
| illatio | iudicium |
| impertinens | iudicium |
| implementatio_linguae | lingua_programmandi |
| inauguratio | actus |
| incertitudo | mensura |
| incidens | operatio |
| incompatibilitas_mercium | pharmacon |
| indeterminatum | iudicium |
| index | elementa_programmatis |
| index_arboris | elementa_programmatis |
| index_inversus | elementa_programmatis |
| index_verificationis | praescriptum |
| inquisitio_forensis | iudicium |
| inscriptio | locus |
| instrumentum | corporalia |
| instrumentum_aedificationis | elementa_programmatis |
| instrumentum_lineae | elementa_programmatis |
| instrumentum_mensurae | mensura |
| instrumentum_publicum | acta |
| interactio | pharmacon |
| interactio_cum_cibo | pharmacon |
| interactio_medicamentorum | pharmacon |
| intervallum | tempus |
| interventio | fabricatio |
| invarians | elementa_programmatis |
| investigatio | iudicium |
| investigatio_praeventiva | iudicium |
| isbn | identificatio |
| isbn_10 | identificatio |
| isbn_13 | identificatio |
| issn | identificatio |
| iter | vectura |
| iudicium | iudicium |
| iurisdictio | locus |
| iussum | obligatio |
| iussum_emptionis | obligatio |
| iussum_ne_resuscitetur | obligatio |
| iussum_operis | obligatio |
| lectio | elementa_programmatis |
| lectio_externa | elementa_programmatis |
| lectio_memoriae | elementa_programmatis |
| legenda | documentatio |
| lex | norma |
| lex_aeterna | norma |
| lex_divina_positiva | norma |
| lex_humana | norma |
| lex_naturalis | norma |
| licentia | obligatio |
| limitatio | obligatio |
| lingua_iussuum | lingua_programmandi |
| lingua_programmandi | lingua_programmandi |
| locatio_conductio | obligatio |
| locus | locus |
| longitudo | mensura |
| magistratus | organizatio |
| magnitudo_mensurabilis | mensura |
| maioritas_replicarum | actus |
| malus | planta |
| malus_domestica | planta |
| mancipatio | actus |
| mandatum | obligatio |
| mandatum_iudicis | obligatio |
| manuale | documentatio |
| manumissio | actus |
| massa | mensura |
| materia | corporalia |
| matrimonium | actus |
| medicamentum | pharmacon |
| medicamentum_compositum | pharmacon |
| medicamentum_genericum | pharmacon |
| medicamentum_nominatum | pharmacon |
| memoria_operis | iudicium |
| mensuratio | iudicium |
| momentum | tempus |
| moneta | commercium |
| monitum_futuri | elementa_programmatis |
| morbus | morbus |
| morbus_degenerativus | morbus |
| morbus_geneticus | morbus |
| morbus_infectiosus | morbus |
| mors_praesumpta | norma |
| mutatio | elementa_programmatis |
| mutatio_memoriae | elementa_programmatis |
| navis | vectura |
| ndc | identificatio |
| norma | norma |
| notarius | organizatio |
| notitia_pascui | iudicium |
| numerositas | mensura |
| numerus | schema |
| numerus_dea | identificatio |
| nummus_chartaceus | commercium |
| nuntius | communicatio |
| obiectum_compilatum | elementa_programmatis |
| obligatio | obligatio |
| obligatio_ad_portitorem | commercium |
| obligatio_ex_contractu | obligatio |
| obligatio_ex_delicto | obligatio |
| obligatio_ex_lege | obligatio |
| observatio | iudicium |
| officium | organizatio |
| onus | vectura |
| opinio | iudicium |
| opus_scriptum | opus_scriptum |
| orcid | identificatio |
| ordo | actus |
| organizatio | organizatio |
| origo_dominii | acta |
| paenitentia | actus |
| parochus | organizatio |
| pascha | tempus |
| pecunia | commercium |
| pensum | operatio |
| per_cyclos | tempus |
| per_horas_operis | tempus |
| per_spatium | tempus |
| periculum | iudicium |
| periodicum | opus_scriptum |
| peritia_communis | iudicium |
| perscriptio | commercium |
| persona | persona |
| persona_iuridica | organizatio |
| personalitas_iuridica | norma |
| petitio | obligatio |
| petitio_assecurationis | obligatio |
| petitio_remota | obligatio |
| plagula_computatralis | plagula_computatralis |
| plagula_csv | plagula_computatralis |
| plagula_datorum | plagula_computatralis |
| plagula_figurationis | plagula_computatralis |
| plagula_fontis | plagula_computatralis |
| plagula_json | plagula_computatralis |
| plagula_markdown | plagula_computatralis |
| plagula_notationis | plagula_computatralis |
| plagula_plist | plagula_computatralis |
| plagula_sqlite | plagula_computatralis |
| plagula_stml | plagula_computatralis |
| plagula_toml | plagula_computatralis |
| plagula_xml | plagula_computatralis |
| planta | planta |
| plena_potestas | obligatio |
| portio_communis | obligatio |
| portio_computandi | obligatio |
| portio_pascui | obligatio |
| portio_piscandi | obligatio |
| portorium | obligatio |
| possessio | commercium |
| postconditio | elementa_programmatis |
| postulatum | fabricatio |
| praeceptum | iudicium |
| praeconditio | elementa_programmatis |
| praedictio | iudicium |
| praescriptio_medica | obligatio |
| praescriptum | praescriptum |
| praeses_collegii | organizatio |
| praesumptio | iudicium |
| pretium | commercium |
| probatio | fabricatio |
| probatio_fumi | fabricatio |
| probatio_integrationis | fabricatio |
| probatio_regressionis | fabricatio |
| probatio_unitatis | fabricatio |
| procedura | praescriptum |
| procedura_automata | praescriptum |
| processorium | architectura_machinae |
| processus | operatio |
| procuratio | obligatio |
| programma | elementa_programmatis |
| proiectio | elementa_programmatis |
| promptuarium | elementa_programmatis |
| promulgatio | actus |
| punctum_actorum | acta |
| quantitas | mensura |
| quorum | actus |
| quorum_deliberativum | actus |
| radix_fiduciae | acta |
| radix_systematis | acta |
| ratio_duplex | commercium |
| ratio_rationaria | commercium |
| recensio_post_mortem | iudicium |
| recurrentia | tempus |
| recurrentia_computata | tempus |
| recurrentia_per_usum | tempus |
| regesta | acta |
| regressio | fabricatio |
| reliquiae | corporalia |
| res_corporea | corporalia |
| res_sacra | corporalia |
| res_sacra_immobilis | corporalia |
| res_sacra_mobilis | corporalia |
| reservatio | tempus |
| restauratio_aedificii | fabricatio |
| retentum | iudicium |
| ritus | actus |
| rosa | planta |
| rosa_canina | planta |
| rosaceae | planta |
| rxnorm | identificatio |
| sacramentale | actus |
| sacramentum | actus |
| schedula_substantiarum | norma |
| schema | schema |
| schema_identificandi | identificatio |
| scientia | iudicium |
| scriptio_externa | elementa_programmatis |
| scriptum_conchae | plagula_computatralis |
| scriptum_exsecutabile | plagula_computatralis |
| secretum | iudicium |
| secretum_negotii | iudicium |
| semita_trita | communicatio |
| sententia_iudicis | iudicium |
| sententia_lata | actus |
| series | formae_datorum |
| servus | cliens |
| servus_datorum | cliens |
| servus_mcp | cliens |
| servus_nominum | cliens |
| servus_procuratorius | cliens |
| servus_telae | cliens |
| sigillum_confessionis | iudicium |
| sigillum_contenti | elementa_programmatis |
| signum | iudicium |
| silentium | documentatio |
| societas | organizatio |
| societas_contractus | obligatio |
| statio | acta |
| status | elementa_programmatis |
| stigmergia | communicatio |
| stipulatio | actus |
| substantia_chemica | pharmacon |
| supplicatio | obligatio |
| suspicio | iudicium |
| symptoma | iudicium |
| syngrapha | commercium |
| systema_operativum | systema_operativum |
| systema_plagularum | plagula_computatralis |
| systema_unixiforme | systema_operativum |
| tabula_associativa | formae_datorum |
| tabula_communis | communicatio |
| temperatura | mensura |
| tempus_intervallum | mensura |
| terminatio | elementa_programmatis |
| tessera | commercium |
| tessera_digitalis | commercium |
| tessera_negotiabilis | commercium |
| tessera_repositoria | commercium |
| tessera_simplex | commercium |
| tessera_spectaculi | commercium |
| testamentum | actus |
| testimonium | iudicium |
| textus | schema |
| titulus | obligatio |
| titulus_honoris | obligatio |
| titulus_iuris | elementa_programmatis |
| tractatus | obligatio |
| tramen | vectura |
| transactio | elementa_programmatis |
| tributum | obligatio |
| tributum_capitis | obligatio |
| ulid | identificatio |
| unctio_infirmorum | actus |
| unitas_mensurae | mensura |
| universitas | organizatio |
| universitas_personarum | organizatio |
| universitas_rerum | organizatio |
| uri | identificatio |
| uuid | identificatio |
| vectigal | obligatio |
| vehiculum | vectura |
| venditio | commercium |
| verbum_arcanum | iudicium |
| verbum_clavis | lingua_programmandi |
| veritas | schema |
| versio | versio |
| versio_architecturae | architectura_machinae |
| versio_linguae | lingua_programmandi |
| versio_praescripti | praescriptum |
| versio_systematis | systema_operativum |
| vestigia_operis | communicatio |
| vestigium_chemicum | communicatio |
| via | vectura |
| via_aeria | vectura |
| via_maritima | vectura |
| via_terrestris | vectura |
| vitium | fabricatio |
| vivens | corporalia |
| volumen | plagula_computatralis |
| vulnus | morbus |

## IV. Dictionarium (individua descripta; species inter genera §II)

- `animal.canis` — :laika
- `architectura_machinae.architectura_machinae` — :arm, :riscv, :x86
- `architectura_machinae.processorium` — :apple_m1, :intel_8086
- `architectura_machinae.versio_architecturae` — :armv8, :i386, :x86_16, :x86_64
- `cliens.cliens_cli` — :curl
- `commercium.moneta` — :dollarium_americanum, :euro
- `elementa_programmatis.bibliotheca` — :libc, :sqlite, :zlib
- `elementa_programmatis.constans` — :null
- `elementa_programmatis.constans_numerica` — :eof, :pi
- `elementa_programmatis.functio` — :malloc, :memcpy, :printf, :rand, :scanf, :strtok
- `elementa_programmatis.functio_pura` — :abs, :strlen
- `elementa_programmatis.instrumentum_aedificationis` — :aedilis, :make
- `elementa_programmatis.status` — :errno, :index_strtok, :semen_rand
- `lingua_programmandi.implementatio_linguae` — :clang, :cpython, :fil_c, :gcc, :officina
- `lingua_programmandi.lingua_iussuum` — :bash
- `lingua_programmandi.lingua_programmandi` — :abc, :b, :c, :python, :rust
- `lingua_programmandi.verbum_clavis` — :const, :goto, :restrict, :static
- `lingua_programmandi.versio_linguae` — :c11, :c89, :c99
- `mensura.unitas_mensurae` — :gradus_celsius, :gramma, :kilogramma, :metrum, :minutum, :secundum
- `norma.norma` — :iso_9899_1990, :iso_9899_1999, :posix_1
- `obligatio.tractatus` — :tractatus_gandavensis
- `opus_scriptum.editio` — :moby_dick_londinensis_1851, :moby_dick_novi_eboraci_1851, :systema_naturae_editio_decima
- `opus_scriptum.expressio` — :abusus_linguae_anglice, :abusus_linguae_germanice
- `opus_scriptum.opus_scriptum` — :abusus_linguae, :moby_dick, :species_plantarum, :summa_theologiae, :systema_naturae
- `organizatio.corpus_normans` — :ansi, :iec, :ieee, :iso
- `organizatio.organizatio` — :agentia_isbn, :debian_project
- `organizatio.societas` — :amd, :apple, :arm_ltd, :att, :bell_labs, :canonical, :intel, :microsoft
- `persona.persona` — :bertrand_meyer, :brian_fox, :carl_linnaeus, :dennis_ritchie, :duns_scotus, :edsger_dijkstra, :graydon_hoare, :guido_van_rossum, :herman_melville, :josef_pieper, :ken_thompson, :linus_torvalds, :luca_pacioli, :thomas_aquinas
- `pharmacon.substantia_chemica` — :amoxicillinum, :warfarinum
- `systema_operativum.distributio_systematis` — :arch, :debian, :ubuntu
- `systema_operativum.systema_operativum` — :vms, :windows
- `systema_operativum.systema_unixiforme` — :linux, :macos, :unix_bell_labs
- `systema_operativum.versio_systematis` — :macos_13_ventura, :macos_14_sonoma
- `tempus.calendarium` — :calendarium_gregorianum, :calendarium_liturgicum
- `vectura.via` — :canalis_suesianus, :via_appia

## V. Necessitudines — relationes declaratae (85)

Identitas relationis SEMEL declaratur; sedes usus (relatio,
terminus) titulo, converso, aut scriptura ligant. Familiae =
necessitudines altae (columna sub vacua).

| necessitudo | conversum | sub | a | ad | scripturae | exemplar |
|---|---|---|---|---|---|---|
| accipit | - | translatio_nuntii | * | * | - | necessitudines |
| adhibet | - | dependentia | * | * | - | necessitudines |
| administrat | administrata_a | custodia | * | * | - | necessitudines |
| afficit | - | effectio | * | * | - | necessitudines |
| asseritur_per | - | fundatio | * | * | - | necessitudines |
| auctor | - | origo | * | * | - | necessitudines |
| causalitas | - | - | * | * | - | necessitudines |
| causat | - | causalitas | * | * | - | necessitudines |
| condita_a | - | origo | * | * | conditum_a | necessitudines |
| conformitas | - | - | * | * | - | necessitudines |
| constituta_per | - | origo | * | * | constituitur_per | necessitudines |
| continet | continetur_in | pars_totum | * | * | - | necessitudines |
| cum_incertitudine | de_quantitate | repraesentatio | * | * | - | mensura |
| currit_ut | - | exsecutio | * | * | - | necessitudines |
| custodia | - | - | * | * | - | necessitudines |
| custodit | custoditur_ab | custodia | * | * | custodita_a custodita_per | necessitudines |
| de | - | fundatio | * | * | - | necessitudines |
| dependentia | - | - | * | * | - | necessitudines |
| edita_a | - | evulgatio | * | * | - | necessitudines |
| effectio | - | - | * | * | - | necessitudines |
| efficit | efficitur_a | effectio | * | * | - | necessitudines |
| emissa_a | - | evulgatio | * | * | - | necessitudines |
| evulgatio | - | origo | * | * | - | necessitudines |
| ex_fonte | - | fundatio | * | * | ex_fontibus | necessitudines |
| ex_vitio | - | causalitas | * | * | - | necessitudines |
| exercet | exercetur_a | exsecutio | * | * | - | necessitudines |
| exsecutio | - | - | * | * | - | necessitudines |
| exsequitur | - | exsecutio | * | * | - | necessitudines |
| fabricatum_in | coquitur | origo | * | * | - | necessitudines |
| fert | fertur_per | dependentia | * | * | - | necessitudines |
| fundat | fundatur_in | fundatio | * | * | - | necessitudines |
| fundatio | - | - | * | * | - | necessitudines |
| gradus_eius | gradus_de | pars_totum | * | * | - | praescriptum |
| habet_expressionem | expressio_de | successio | * | * | - | opus_scriptum |
| habet_fasciculum | fasciculus_de | successio | * | * | - | opus_scriptum |
| habet_versionem | - | successio | * | * | - | necessitudines |
| haurit | hauritur_a | exsecutio | * | * | - | elementa_programmatis |
| impedit | impeditur_a | effectio | * | * | - | necessitudines |
| implementat | implementata_ab | effectio | * | * | - | lingua_programmandi |
| implet | impletur_a | conformitas | * | * | - | necessitudines |
| in_iurisdictione | - | situs | * | * | - | necessitudines |
| ingreditur | ex_materia | pars_totum | * | * | - | corporalia |
| interrogat | interrogatur_a | fundatio | * | * | - | fabricatio |
| intra | - | situs | * | * | - | necessitudines |
| lata_a | - | evulgatio | * | * | latum_per latus_a | necessitudines |
| legit | legitur_a | dependentia | * | * | - | elementa_programmatis |
| membrum | membrum_de | pars_totum | * | * | - | necessitudines |
| mensuratur_per | mensurat | repraesentatio | * | * | - | necessitudines |
| minatur | - | causalitas | * | * | - | necessitudines |
| minuitur_per | - | causalitas | * | * | - | necessitudines |
| mittit | missus_a | translatio_nuntii | * | * | - | necessitudines |
| mutat | mutatur_a | effectio | * | * | - | elementa_programmatis |
| natum_ex | - | causalitas | * | * | - | necessitudines |
| normata_per | - | conformitas | * | * | - | necessitudines |
| obiectum_actus | - | participatio | * | * | - | necessitudines |
| origo | ortum_dat | - | * | * | - | necessitudines |
| orta_ex | - | origo | * | * | ortum_ex | necessitudines |
| parit | - | origo | * | * | - | necessitudines |
| pars_de | - | pars_totum | * | * | - | necessitudines |
| pars_totum | - | - | * | * | - | necessitudines |
| participatio | - | - | * | * | - | necessitudines |
| pendet_ex | - | dependentia | * | * | dependet_ex | necessitudines |
| pertinet_ad | - | fundatio | * | * | - | necessitudines |
| pollinat | pollinatur_a | effectio | * | * | - | necessitudines |
| possessio_eius | - | custodia | * | * | - | necessitudines |
| postulat | - | dependentia | * | * | - | necessitudines |
| praebet | praebetur_a | dependentia | * | * | - | necessitudines |
| producit | per_instrumentum | origo | * | * | - | elementa_programmatis |
| realizata_a | - | effectio | * | * | realizatur_per | necessitudines |
| repraesentatio | - | - | * | * | - | necessitudines |
| retractat | retractatur_a | effectio | * | * | - | fabricatio |
| scripta_in | - | dependentia | * | * | - | necessitudines |
| secundum_normam | - | conformitas | * | * | secundum_legem | necessitudines |
| sequitur | praecedens | successio | * | * | - | necessitudines |
| situm_in | - | situs | * | * | sita_in situs_in | necessitudines |
| situs | - | - | * | * | - | necessitudines |
| solvitur_per | - | exsecutio | * | * | - | necessitudines |
| sub_iurisdictione | - | custodia | * | * | - | necessitudines |
| substituit | - | successio | * | * | - | necessitudines |
| successio | - | - | * | * | - | necessitudines |
| tendit_ad | - | causalitas | * | * | - | necessitudines |
| tenetur_a | - | custodia | * | * | - | necessitudines |
| translatio_nuntii | - | - | * | * | - | necessitudines |
| versio_de | - | successio | * | * | - | necessitudines |
| violat | - | conformitas | * | * | - | necessitudines |

## VI. Umbrae — superficta, nondum descripta

Agenda COMPUTATA (non memorata): quod aliquod exemplar citat sed
nemo describit. DUAE agendae sunt, non una — ordo intra utramque
per numerum citationum (quod plura exemplaria poscunt, prius).

### VI.a Genera desiderata (2) — opus consilii

A `relatio` citata: relationes in GENERA tendunt, ergo quaestio
ontologica et iudicium poscitur.

- **typus** — 2x, a `elementa_programmatis`
- **constructum_linguae** — 1x, a `elementa_programmatis`

### VI.b Individua desiderata (0) — opus dictionarii

A `relato` SOLO citata: res singularis nominata sub genere iam
exsistente. Nullum consilium — describi potest quovis momento.
(EURISTICA, non lex: relatum genus attingere potest.)


## VII. Dubia aperta

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
- `planta` / **planta** — VALOR duo dicit, et quod dicat nondum discernimus. 'rosa' habitum ad 'frutex' figens CONSTRINGIT: rosa quae arbor est rosa non est, ergo valor ad definitionem g
- `schema` / **genus_valoris** — Paletta domus etiam "area" (textus amplus) et "annus" (dies grossior) habet: species propriae, an textus cum praesentatione et dies cum granularitate? Praesenta
- `tempus` / **pascha** — GRADUS AMBIGUUS (rota XXIX): una est regula paschalis, ergo individuum videtur; at "pascha" etiam de quovis anno dicitur ("pascha huius anni"), quod praedicatio
- `versio` / **versio** — FURCA (fork) versio non est - continuatio auctoritatis deficit, unde res nova. Sed casus medii sunt: quid de furca quae nomen retinet et auctoritatem novam habe

## VIII. Monita — stirps communis sine cognatione (23)

Regula XVIII (ADVISORIA, portam non frangit). Nomen quod nomen
aliud ut stirpem fert, sed nec sub eo stat nec relationem ad id
declarat. **Pleraque LEGITIMA sunt** — homonyma honesta:
`fons_c` plagula est, `fons` propositionis origo. Pretium
regulae non est quod defectus inveniat sed quod nulla talis
coincidentia INVISA transeat.

- 'annotatio_machinae' stirpem 'annotatio' fert sed sub ea non stat
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

## IX. Vocabularium formae

Vocabularium CLAUSUM in natura/natura.canon SOLO vivit (fons
unicus post migrationem 2026-08-06); natura_examen id per
lib/canon.c iudicat (VULNUS regula 8). Catalogus legibilis:
METAMODULUS §3/§4 (e canone generatus).
