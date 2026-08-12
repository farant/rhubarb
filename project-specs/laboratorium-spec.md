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

**Cur signum/symptoma cardo sit.** Interfacies duo genera proprietatum
habet: quae metiri licet (mora, numerus ictuum, error), et quae solum
sentiuntur ("hoc male se habet"). Fran experimentator ET patiens idem
erit — modus classicus se ipsum fallendi. Medicina saecula in hac
distinctione posuit; nos eam mutuamur. Canon POSTULAT ut quaeque
observatio se ipsam declaret: `signum` an `symptoma`. Ita "melius
sensi" numquam tacite in argumentum transit.

Et quia signum in censu IAM viam suam fert (`via=`), signum laboratorii
idem faciat: **numerus qui viam suam non fert, signum non est.**

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

```xml
<experimentum numerus="0001" status="agnitum">

  <!-- ANTE OPUS - obligatorium, et post 'agnitum' IMMUTABILE -->
  <ignotum species="ignotum_notum"
    quaestio="Utrum arbor nidificata censum celerius navigabilem
              faciat quam index planus?">
    <refutatio>Si neutra variatio indicem planum in tempore-ad-
      inveniendum superat, idea moritur.</refutatio>
    <impedit ad="&charta;"/>
  </ignotum>

  <!-- variationes comparatae -->
  <variatio nomen="&0001-planus;" titulus="index planus (regula)"/>
  <variatio nomen="&0001-arbor;"  titulus="arbor nidificata"/>

  <!-- DUM OPUS - libera, quantum vis -->
  <acta>
    <nota quando="2026-08-13">Umbra radicis eventus ictus
      absorbuit - shadow DOM hoc facit.</nota>
  </acta>

  <!-- POST OPUS - ad statum 'solutum' obligatoriae -->
  <observationes>
    <signum de="&0001-arbor;" contentum="tempus-ad-inveniendum 4.1s"
      via="laboratorium/0001/metiri.sh arbor"/>
    <signum de="&0001-planus;" contentum="tempus-ad-inveniendum 6.8s"
      via="laboratorium/0001/metiri.sh planus"/>
    <symptoma de="&0001-arbor;">Plicare gravius sentitur quam
      numeri dicunt - mora animationis, non quaesitionis.</symptoma>
    <sententia>Arbor vincit, sed animatio corrigenda ante messem.</sententia>
  </observationes>

</experimentum>
```

**Cur `impedit` momenti sit**: ligat experimentum ad opus verum quod eo
obstruitur. Experimentum quod nihil impedit fortasse currendum non est.

---

## §3 Portae (canon)

Canon `laboratorium.canon`, per registrum dispatchatus, `bin/canon_examen`
iudicante (more census/natura).

1. **PRAEREGISTRATIO** — `<ignotum>` cum `quaestio` ET `<refutatio>`
   adesse debet antequam status ex `agnitum` moveatur. *(decretum Franis)*
2. **SIGILLUM PROPOSITI** — cum status `agnitum` relinquit, `<ignotum>`
   sigillo signatur. Mutatio postea = vulnus. Ita ratiocinatio
   post-facta structuraliter impeditur.
3. **CONCLUSIO VERA** — status `solutum` postulat ≥1 observationem quae
   variationem citet.
4. **SIGNUM VIAM FERT** — quodque `<signum>` `via=` habeat, aliter
   symptoma est. *(regula censūs mutuata)*
5. **GENUS DECLARATUM** — observatio nuda non licet: aut `signum` aut
   `symptoma`.
6. **MESSIS IUSTIFICATA** — componentia armarii experimentum suum
   citent (§5).

**Quod canon NON postulat**: variationes plures, acta, sententiam,
mensuras numericas. Periculum verum est ne schema experimenta
DEVORET — si structuram implere carius sit quam ideam temptare,
experimenta cessabunt. Porta minima esto: quaestio, refutatio, et
conclusio quae conclusionem habeat.

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
5. **Metirine possumus omnino?** Signum verum interfaciei (tempus-ad-
   inveniendum) HOMINEM cum horologio poscit. Estne id quod facere vis,
   an sunt experimenta quorum observationes symptomata sola erunt?
   (Legitimum utrumque — sed canon id scire debet.)
