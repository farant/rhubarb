# Laboratorium — officina experimentorum interfaciei

**Status**: consilium (nihil aedificatum). 2026-08-12.
**Decreta Franis** (haec conversatio): (1) praeregistratio OBLIGATORIA —
quaestio + criterium ruinae ante opus; (2) messis in armarium — quod
probatum est in bibliothecam componentium migrat.

---

## §0 Quid sit

Applicatio vitrea in qua ideae interfaciei per **experimenta numerata**
(0001, 0002, …) probantur. Quodque experimentum tria fert:

1. **interfaciem ipsam** — componentia telae (web components), viva
2. **propositum ante opus** — quid ignoremus, et quid ruinam constituat
3. **observationes post opus** — quid inventum sit, et cuius generis

Non scrinium ideārum sed LABORATORIUM: quod concludi non potest,
concludi non licet.

### Ratio (the frame)

**Experimentum est MOTUS quo ignotum solvitur.**

Natura hoc iam fert (`/iudicium/ignotum`): "absentia notitiae AGNITA et
nominata: hiatus qui, quia nominatus est, tractari potest". Apparatus
eius IPSE est caput experimenti nostri:

| apparatus ignoti | munus in laboratorio |
|---|---|
| `quaestio` [textus] | quid quaerimus |
| `species` | ignotum_notum, ignotum_ignotum, indeterminatum, controversum… |
| `status` | agnitum → quaesitum → solutum \| relictum \| insolubile |
| `impedit ->` [multiplex] | quod opus verum hoc ignoto obstruitur |
| `solvitur_per -> observatio` | SPINA: ignotum per observationem solvitur |

Species `ignotum_notum` exemplum SUUM iam fert: *"quanta sit mora
huius…"* — id est, quaestio interfaciei.

---

## §1 Vocabularium

### Quod natura IAM FERT (nihil mintandum)

- **`/iudicium/ignotum`** + species VII — caput experimenti
- **`/iudicium/observatio`** — "notitia rei singularis accepta, quae
  iudicium fundare potest: quid videatur, quid **sentiatur**"
- **`/iudicium/signum`** (sub observatio) — "ab observatore deprehensum…
  **quivis peritus idem videre potest**, et instrumentum [metiri]"
- **`/iudicium/symptoma`** (sub observatio) — "a subiecto ipso relatum…
  **ab alio verificari NON POTEST** - solus patiens accessum habet"
- **`/iudicium/investigatio`** — inquisitio ordinata in causas
- **`/fabricatio/probatio`** — CONTRA quam definiendum (vide infra)

**SYMPTOMA REGULA EST, NON EXCEPTIO** *(decretum Franis 2026-08-12)*.
Praedictiones et effectus PLERUMQUE qualitativi erunt. Exemplum verum
Franis:

> ante: "puto utile fore si textus talis sponte in nexus vertatur"
> post: "re vera adiuvit hoc modo… sed pagina visu turbida facta est,
> et nexus satis graves non sentiebantur"

Nullus numerus usquam. Definitio symptomatis id exacte capit: "a
subiecto ipso relatum… ab alio verificari NON POTEST - solus patiens
accessum habet."

**Quid ergo structura emat, si omnia qualitativa sunt?** Tria, et sola
tria — sed haec tria sufficiunt:

1. **Praedictio ANTE scribitur.** Contra memoriam, quae se ipsam post
   factum corrigit.
2. **Praedictum a proventu SEPARATUR.** Discrimen ipsum est notitia:
   quod expectatum non evenit, et quod inexpectatum evenit.
3. **INOPINATUM sedem propriam habet.** In exemplo supra, messis vera
   NON est "adiuvit" (id praevisum erat) sed "turbida facta est" et
   "satis graves non sentiebantur" — quae neutra praevisa sunt.
   Ubi omnia qualitativa sunt, notitia in SURPRISA habitat.

Canon nihilominus postulat ut observatio se declaret (`signum` an
`symptoma`), non quia signa multa erunt, sed quia rarum signum sciri
debet ESSE signum. Et signum, cum venerit, viam suam ferat more censūs:
**numerus qui viam suam non fert, signum non est.**

### Quod mintandum est (omnia LIBERA, probatum)

| nomen | quid sit |
|---|---|
| `experimentum` | cursus unus: ignotum + variationes + observationes |
| `variatio` | alternativa quae comparatur (A, B, C) |
| `laboratorium` | continens experimentorum |
| `armarium` | bibliotheca componentium messorum |
| `refutatio` | criterium ruinae: quid experimentum necet |

**`experimentum` CONTRA `probatio`.** Probatio est "interrogatio
exsecutabilis intentionis: scaena parata, actio exercita, effectus
contra exspectationem" — responsum EXSPECTATUM tenes, et probatio
cadit cum res differt. Experimentum responsum NON tenet; ea res tota
est. Probatio custodit; experimentum discit.

**`armarium`**: vocabulum monasticum — armarium erat scrinium librorum,
id est bibliotheca claustri, et armarius monachus libris praepositus.
Laboratorium ubi laboratur; armarium ubi probata reponuntur.

---

## §2 Documentum experimenti

**Unum documentum per experimentum**, non tria. Ratio: propositum et
sententia si in plagulis diversis sedeant, DIVERGENT — et tota res in
eo est ut divergere non possint. Praeterea porta simplex fit:
experimentum `solutum` sine observationibus mendacium est quod canon
recusare potest.

Exemplar sequens exemplum VERUM Franis induit (nexus sponte facti):

```xml
<experimentum numerus="0004" status="solutum">

  <!-- ANTE OPUS - obligatorium, et post 'agnitum' IMMUTABILE -->
  <ignotum species="ignotum_notum"
    quaestio="Utrum textus talis sponte in nexus versus lectorem
              adiuvet?">
    <exspectatio>Adiuvabit: nexum manu quaerere molestum est, et
      nomina rerum iam in prosa sedent.</exspectatio>
    <refutatio>Si nexus factos non premo, aut si pagina propterea
      turbidior fit sine pretio reddito, idea cadit.</refutatio>
    <impedit ad="&forum;"/>
  </ignotum>

  <!-- DUM OPUS - libera, quantum vis (aut nihil) -->
  <acta>
    <nota quando="2026-08-13">Umbra radicis ictus absorbuit -
      shadow DOM hoc facit.</nota>
  </acta>

  <!-- POST OPUS - ad statum 'solutum' obligatoriae -->
  <observationes>
    <symptoma respondens="exspectationi">Adiuvat re vera: ad rem
      citatam sine quaesitione salire licet.</symptoma>

    <inopinatum>Pagina VISU TURBIDA fit - hoc non praevidi.</inopinatum>
    <inopinatum>Nexus 'satis graves' non sentiuntur: pondus visuale
      ponderi sensus non respondet.</inopinatum>

    <sententia verdictum="mixtum">Munus manet, vestis non. Sponte
      nectere rectum est; QUOMODO id monstretur ignotum novum
      est.</sententia>
  </observationes>

</experimentum>
```

**`<inopinatum>` campus pretiosissimus est.** In exemplo supra,
`<symptoma>` id solum confirmat quod Fran iam putabat; duo
`<inopinatum>` sunt quae experimentum RE VERA docuit. Ubi observationes
qualitativae sunt, notitia in surprisa habitat — ergo surprisae sedes
propria, non nota in margine.

**DUO AXES, NE CONFUNDANTUR.** Error facilis est haec miscere:

| axis | quaerit | valores |
|---|---|---|
| `ignotum/@status` | clausane est rima? | agnitum→quaesitum→**solutum**\|relictum\|insolubile |
| `sententia/@verdictum` | bonane erat idea? | confirmatum \| refutatum \| **mixtum** \| incertum |

Experimentum `solutum` cum verdicto `refutatum` SUCCESSUS est: rem
didicisti, et res mala erat. Separatis axibus numerare licet quot
experimenta DOCUERINT seorsum a quot ideae VALUERINT — quorum prior
numerus laboratorium metitur, posterior iudicium tuum.

`mixtum` casus COMMUNIS erit, non rarus: fere semper aliquid valet et
aliquid constat. `incertum` honestum est et permittitur — cucurristi,
adhuc nescis.

**Cur `impedit` momenti sit**: ligat experimentum ad opus verum quod eo
obstruitur. Experimentum quod nihil impedit fortasse currendum non est.

**Variationes optionales sunt.** Si duas formas comparas, `<variatio>`
adde; si unam solam temptas (ut in exemplo), omitte. Comparatio
instrumentum est, non ritus.

---

## §3 Portae (canon)

Canon `laboratorium.canon`, per registrum dispatchatus, `bin/canon_examen`
iudicante (more census/natura).

1. **PRAEREGISTRATIO** — `<ignotum>` cum `quaestio`, `<exspectatio>`
   ET `<refutatio>` adesse debet antequam status ex `agnitum` moveatur.
   *(decretum Franis)*
2. **SIGILLUM PROPOSITI** — cum status `agnitum` relinquit, `<ignotum>`
   sigillo signatur. Mutatio postea = vulnus. Ita ratiocinatio
   post-facta structuraliter impeditur.
3. **CONCLUSIO VERA** — status `solutum` postulat ≥1 observationem ET
   `<sententia verdictum=…>`.
4. **SIGNUM VIAM FERT** — quodque `<signum>` `via=` habeat, aliter
   symptoma est. *(regula censūs mutuata)*
5. **GENUS DECLARATUM** — observatio nuda non licet: `signum`,
   `symptoma`, aut `inopinatum`.
6. **MESSIS IUSTIFICATA** — componentia armarii experimentum suum
   citent (§5).

**Quod canon NON postulat**: variationes, acta, numeros, taxinomiam
inopinatorum. Periculum verum est ne schema experimenta DEVORET — si
structuram implere carius sit quam ideam temptare, experimenta
cessabunt. Porta minima esto: quaestio, quid expectes, quid te
sententiam mutare faciat — et postea conclusio quae conclusionem habeat.

**REFUTATIO QUALITATIVA.** Praeregistratio numeros NON poscit. Poscit ut
ANTE scribas quid te sententiam mutare faciat, tam certe ut effugere
non possis. "Si nexus factos non premo, aut si pagina turbidior fit
sine pretio reddito" refutatio VERA est: nullus numerus, sed post opus
negari non potest. Popper sine mensura stat.

---

## §3a Vocabularium ex laboratorio, non in laboratorium

`<inopinatum>` liberum textum fert — CONSULTO. Taxinomia defectuum
interfaciei ANTE experimenta scripta divinatio esset; eadem post
centum experimenta MESSIS erit.

Regula: cum idem inopinatum TER apparuerit, nomen meretur et in
`natura/laboratorium.genera` migrat. Exemplum iam adest — "pondus
visuale ponderi sensus non respondet" (§2) species est quae, si
recurrat, `discrepantia-ponderis` fieri debet.

**Ergo laboratorium duo gignit**: componentia (in armarium, §5) et
VOCABULA (in naturam). Alterum fortasse pretiosius.

Hoc legem domus INVERTIT — alibi `bin/natura quaere` ante nominationem
curritur, ne quod adest iterum mintetur. Hic corpus nomen habere NON
potest, quia res nondum observata est. Laboratorium ergo unicus locus
domus est ubi genera ex experientia oriuntur, non ex analysi.

---

## §3b Stirps experimentorum

Exemplum Franis ipsum ostendit: "nexus satis graves non sentiuntur"
responsum non est sed IGNOTUM NOVUM — *quantum ponderis visualis
affordantia secundaria ferre debeat?*

Natura hoc iam ferre potest: `ignotum` relationem `agnitum_per ->
iudicium` habet. Ergo ignotum experimenti 0009 sententiam experimenti
0004 citat:

```xml
<experimentum numerus="0009" status="agnitum">
  <ignotum species="ignotum_notum"
    quaestio="Quantum ponderis visualis affordantia secundaria ferat?">
    <agnitum-per ad="&experimentum-0004;"/>
    …
```

Filius patrem citat (directio naturae); visus laboratorii eam invertat
ut stirpem monstret. Unde laboratorium non index fit sed ARBOR
INQUISITIONIS: quaestio quaestionem parit, et via ab ignoto primo ad
opus praesens legi potest. Quod charta iam pingere scit.

---

## §4 Architectura applicationis

**Una applicatio cum itinerario, non binarium per experimentum.** Ad
comparandum opus est duo simul videre; et XL binaria XL refectiones
sunt.

**Componentia telae** (custom elements + shadow DOM) segregationem
VERAM dant: experimentum 0007 in 0003 stilum suum effundere non
potest, nec laboratorium totum frangere.

### Duo modi serviendi — CARDO USABILITATIS

Mensuratum 2026-08-12: `vitrea.h` iam duos fert —

```c
VITREA_ORIGO_CAPSULA   /* capsula:// - res compacta */
VITREA_ORIGO_URL       /* modus-dev: URL http vivum */
```

Ergo:

- **modus operis** (`ORIGO_URL`): `hospitium` directorium
  `apps/laboratorium/` e DISCO servit; vitrea ad `localhost` spectat.
  Plagulam experimenti muta → recarica → vide. **NULLA refectio.**
- **modus vecturae** (`ORIGO_CAPSULA`): capsula omnia includit, ad
  condendum per silex.

Hoc CARDO est: laboratorium cuius gyrus refectionem poscit laboratorium
non est. Utraque pars iam exstat (vitrea ORIGO_URL + hospitium V1) —
nihil novum aedificandum, solum coniungendum.

### Recarica calida

Documenta `.stml` per JSON-RPC e disco leguntur (exemplar `censum_legere`
chartae: signum `mtime.magnitudo.inode`, `{mutatum:false}` breviter
respondens). Ita index experimentorum et tabella observationum se
renovant dum scribis.

---

## §5 Messis — semita in armarium

*Decretum Franis: quod probatum est in bibliothecam componentium migrat.*

**Nullum componens sine experimento quod id iustificat.** Idea census
(iustificatum contra pertingens) ad interfaciem translata.

```
apps/laboratorium/armarium/
    armarium.stml          <- quid intus sit, et unde quodque venerit
    arbor-nidificata.js
    tabella-ordinabilis.js
```

```xml
<componens nomen="&arbor-nidificata;" plagula="arbor-nidificata.js"
  elementum="rh-arbor">
  <natum-ex ad="&experimentum-0001;"/>
  <glossa lingua="en">Nested tree view. Earned its place in 0001:
    4.1s vs 6.8s time-to-find against a flat list.</glossa>
</componens>
```

Porta VI: `natum-ex` citationem experimenti SOLUTI ferat. Componens sine
experimento in armarium non intrat.

**Communicatio inter applicationes — mensurata, IAM POSSIBILIS.**
`capsula_generare` TOML legit cuius `files` glob-formas admittit, et
vias relative ad directorium ipsius TOML solvit
(`tools/capsula_generare.c:142`). Ergo forum, villa, quaelibet:

```toml
forum_files = ["index.html", "../../laboratorium/armarium/*.js"]
```

**Nihil in instrumentis mutandum est.** Semita messis hodie patet.

---

## §6 Dispositio

```
apps/laboratorium/
    laboratorium.c              <- vitrea + internuntius (exemplar fori)
    laboratorium.sh             <- capsula_generare + aedilis + curre
    assets/
        index.html              <- testa: itinerarium, tabella, index
        laboratorium.toml
    experimenta/
        0001/
            experimentum.stml   <- acta structurata
            0001.js             <- componentia variationum
            metiri.sh           <- via signorum (si mensurabile)
    armarium/
        armarium.stml
        *.js
natura/laboratorium.genera      <- experimentum, variatio, armarium…
canones/laboratorium.canon      <- coctus e generibus
```

Semen per `silex novum -vitrea`; sedes in monorepo (ut forum), quia
instrumentum domus est.

---

## §7 Quaestiones apertae (Fran iudicet)

1. **`refutatio` an `ruina`?** Praevisio quam elegisti `<ruina>` ferebat;
   `refutatio` terminus artis est (Popper). Verbum unum.
2. **Sigillum propositi (porta II) — quam durum?** Vulnus (canon
   recusat) an monitum (canon queritur)? Durum honestum est sed
   experimentum male scriptum in aeternum male scriptum manet.
3. **Numeri: `0001` an ULID?** Numeri ordinem narrant et breves sunt;
   ULID cum tabulario congruit. Suadeo numeros — laboratorium narratio
   est, et narrationes numerantur.
4. **Quid de experimento `relictum`?** Servaturne in indice an
   celatur? (Suadeo servari et monstrari: quod temptatum et desertum
   est notitia est.)
5. ~~Metirine possumus omnino?~~ **RESPONSUM 2026-08-12**: plerumque
   non, et id consulto. Symptoma regula est; signum rarum manet sed
   viam suam ferre debet cum venerit. Vide §1.
6. **Quid de `<variatio>` fiat?** Nunc optionalis (comparatio
   instrumentum, non ritus). Placetne, an experimentum semper duas
   formas comparare debet?
7. **`<exspectatio>` seorsum a `<refutatio>`?** Nunc duo campi sunt:
   quid futurum putes, et quid te sententiam mutare faciat. Duo re
   vera differunt (exspectatio impleri potest dum refutatio simul
   accenditur — quod exemplo tuo EVENIT), sed duo campi duplum
   scribendi sunt. Unumne an duo?
