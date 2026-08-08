# Documentatio Module Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build `natura/documentatio.genera` (nine genera: four reader-task
work genres, the defect trichotomy, the reader-walk) with one widening in
fabricatio, prove it by re-modeling the peregrinus scenario as a census.

**Architecture:** Pure DATA work — `.genera` edits, a `.census` fixture,
test additions. `lib/canon.c` and `lib/natura.c` untouched (glossae
precedent). The projection chain (`natura_canones` → `canon_coquere`)
carries the new kinds into canons automatically; the registry already
routes `.census` to the monolith.

**Tech Stack:** STML `.genera` corpus, `bin/natura_examen` (hook-fired
gate), `bin/canon_examen`, `./tools/natura_canones.sh`,
`./tools/canon_coquere.sh`, `./tools/natura_visus.sh`,
`./tools/natura_glossae.sh`, credo tests in
`probationes/probatio_natura_canones.c`.

## Global Constraints

- Spec: `project-specs/documentatio-spec.md`; rulings decretum
  01KZFPWPXH. Nine genera exactly; ONE existing-module change
  (`probatio.exercet` → `ad="*"`); `opus_scriptum` untouched.
- Everything in Latin; latina.h macro words are FORBIDDEN identifiers;
  genus names collision-checked 2026-08-08 (bare `lectio` TAKEN;
  `exemplar` format-reserved).
- Every new genus carries la (definitio-or-differentia) + `<glossa
  lingua="en">` + `<glossa lingua="fr">` AT BIRTH; gate =
  `./tools/natura_glossae.sh -porta`, target SUMMA 187/187 ×3
  (today 178/178).
- Pre-fix-oracle law: every must-fail probe verified failing on
  pre-change artifacts before the change lands.
- Tests: `./compile_tests.sh natura` / `canon` / `planta_lectio`
  (exit 2 = NOTHING RAN — a typo'd filter, never health).
- Regeneration staleness gates fire from natura-custos on `.genera`
  saves; every task that edits `.genera` must run
  `./tools/natura_canones.sh` then `./tools/canon_coquere.sh` before
  committing. `./tools/natura_canones.sh -probare` exit 0 = whole
  chain fresh (exit 3 = readers stale).
- NEVER stage `FAQ.md` or `gesta/annales/*` — add files by explicit
  path only. Commits end with
  `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`.
- Cross-model syntax: `<genus nomen="X" sub="parent"
  modulus="module">`; sub-genera carry `<differentia>`, not
  `<definitio>` (animal precedent, natura/animal.genera:29).
- Attribute-vs-child in documents: single relations project as
  attributes, multiplex as `<rel ad="#..."/>` children; kebab-case
  tags (`ambulatio_lectoris` → `<ambulatio-lectoris>`). When unsure,
  `bin/natura_canones -inspicere <genus>` is the authority.

---

### Task 1: Widen `probatio.exercet` to the open form

**Files:**
- Modify: `natura/fabricatio.genera:155-156` (the exercet relation)
- Test: `probationes/probatio_natura_canones.c` (new section XVIII)

**Interfaces:**
- Consumes: `_documentum_iudicare(canon, doc_chars, vitium, &generis,
  &omnia, piscina, intern)` — existing test helper (see section XVII,
  line ~2047, for the exact call shape); `monolithus` already loaded
  in the test scope.
- Produces: an open `exercet` closure that Task 2's
  `ambulatio_lectoris` narrows; test section XVIII that Task 3 extends.

- [ ] **Step 1: Write the failing probe** — open a new section XVIII
  after section XVII (before the closing brace of its scope block),
  same scope pattern as XVII:

```c
        /* ----------------------------------------------------
         * XVIII. documentatio (spec documentatio-spec.md):
         * exercet apertum (L15, praecedens coquitur/L2)
         * ---------------------------------------------------- */
        imprimere("\n--- XVIII. documentatio ---\n");

        /* probatio opus exercere potest (ante: clausura functio
         * solae - ORACULUM: rubet ante aperturam, viret post) */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <opus-scriptum nomen=\"#pagina\"/>\n"
            "  <probatio nomen=\"#ambulatio-cruda\">\n"
            "    <exercet ad=\"#pagina\"/>\n"
            "  </probatio>\n"
            "</individua>\n",
            CANON_CITATIO_IRRITA, &generis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (omnia, (i32)ZEPHYRUM);
```

- [ ] **Step 2: Run to verify it fails for the right reason**

Run: `./compile_tests.sh natura_canones`
Expected: FAIL — the `CREDO_AEQUALIS_I32(omnia, 0)` reddens because
citing an opus under the current `exercet ad="functio"` closure raises
a citation vitium. If it fails any other way (or passes), STOP — the
oracle is wrong.

- [ ] **Step 3: Widen the relation** in `natura/fabricatio.genera`
  (anchor the Edit at the full relation element):

```xml
    <relatio nomen="exercet" ad="*" multiplex="verum"
      nota="apertum 2026-08-08 (L15; praecedens coquitur/L2):
            genus iam generale erat, clausura sola non. Species
            restringunt ubi custodia valet - documentatio:
            ambulatio_lectoris ad opus_scriptum"/>
```

(replacing `<relatio nomen="exercet" ad="functio"` /
`modulus="elementa_programmatis" multiplex="verum"/>`)

- [ ] **Step 4: Regenerate and verify green**

Run: `./tools/natura_canones.sh && ./tools/canon_coquere.sh`
Then: `./compile_tests.sh natura_canones`
Expected: PASS (probe green; no other section moves — body counts are
deliberately not pinned in this suite).

- [ ] **Step 5: Commit**

```bash
git add natura/fabricatio.genera natura/cocta \
  probationes/probatio_natura_canones.c include/planta_lectio.h \
  lib/planta_lectio.c
git commit -m "fabricatio: probatio.exercet apertum (L15, praecedens coquitur)

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 2: The documentatio module — nine genera with glossae

**Files:**
- Create: `natura/documentatio.genera`
- Modify: `natura/INDEX.md` (regenerated), `natura/cocta/*` (regenerated,
  incl. new `documentatio.canon` — canon count 34 → 35)

**Interfaces:**
- Consumes: Task 1's open `exercet`.
- Produces: kind tags `<dictionarium>` `<manuale>` `<exemplum>`
  `<legenda>` `<defectus-documenti>` `<deceptio>` `<silentium>`
  `<ambiguitas>` `<ambulatio-lectoris>` in the regenerated monolith;
  relations `in_opere` (single → attribute `in-opere=`), `lector`
  (single → attribute), `invenit` (multiplex → child), narrowed
  `exercet` (multiplex → child). Task 3's census uses exactly these.

- [ ] **Step 1: Write the module file** — full content:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!--
  natura/documentatio.genera
  Exemplar semanticum: DOCUMENTATIO (versio 1)

  Modulus RELATIONIS inter textum et opus lectoris. Natus ex
  circuitu generationis I (2026-08-08): scenarium glossarum
  peregrino restricto mensuratum, lacunae XV in
  project-specs/documentatio-lacunae.md, furcae quinque decisae
  (decretum 01KZFPWPXH), spec project-specs/documentatio-spec.md.

  SUTURA: possidet genus operis secundum opus lectoris (quattuor
  species operis scripti), trichotomiam defectuum documenti, et
  ambulationem lectoris (species probationis). CITAT
  opus_scriptum (identitas; pars_de iam compositionem fert),
  iudicium (status epistemicus lectoris, annotatio), fabricatio
  (quadratum intentionis), schema (grammatica quam documentum
  docet).

  FURCA I (decretum): genus operis INTENTIO auctoris est (finis),
  aptitudo VERDICTUM ambulationis - opus idem dictionarium
  intentum et manuale mensuratum-defectum esse potest sine
  contradictione. Genus numquam successum affirmat.
-->
<natura modulus="documentatio" versio="1" lingua="latina">

<fontes>
  <fons clavis="corpus" genus="corpus_institutionis"
    modulus="iudicium" verificatus="falsum">
    Corpus institutionis scribae: FRBR, ars lexicographica,
    paedagogia technica. NON VERIFICATUM - sed divisiones huius
    moduli non inde sumptae: e scenario mensurato (ledger
    lacunarum) natae, quod in repositorio ipso iacet.
  </fons>
</fontes>

<!-- ============= QUATTUOR GENERA OPERIS ==============
     Species operis scripti, divisae secundum QUOD OPUS
     LECTORI PRAESTARE DESTINATUR. Quattuor mensuratae solae;
     divisio aperta manet donec scenarium novum plura metiatur.
     Opera hybrida per etiam= (sodalitas secundaria). -->

<genus nomen="dictionarium" sub="opus_scriptum"
  modulus="opus_scriptum">

  <differentia>
    Ad INSPECTIONEM destinatum: quid res sint docet, singulatim,
    ordine quaerendi non legendi. Lector rem petit, invenit,
    discedit - opus aditur, non perlegitur.
  </differentia>

  <glossa lingua="en">A dictionary (reference work): a work
    intended for consultation - it teaches what things ARE, entry
    by entry, ordered for seeking rather than reading. The reader
    comes for one thing, finds it, and leaves; the work is
    visited, not read through.</glossa>
  <glossa lingua="fr">Un dictionnaire (ouvrage de référence) :
    une œuvre destinée à la consultation - elle enseigne ce que
    SONT les choses, entrée par entrée, ordonnée pour chercher et
    non pour lire. Le lecteur vient pour une chose, la trouve et
    s'en va ; l'œuvre se consulte, elle ne se lit pas d'un
    trait.</glossa>

  <nota>
    DIVISIO INTENTA (furca I, decretum 01KZFPWPXH): hoc genus et
    tria sequentia intentionem auctoris ferunt, non successum -
    pagina glossarum dictionarium bonum ET manuale defectum uno
    tempore fuit; aptitudo per ambulationem lectoris mensuratur.
  </nota>
</genus>

<genus nomen="manuale" sub="opus_scriptum"
  modulus="opus_scriptum">

  <differentia>
    Ad ACTIONEM destinatum: quid faciendum sit docet. Lector opus
    aliquod perficere vult et gradus a textu accipit - dictionarium
    quid sit dicit, manuale quomodo fiat.
  </differentia>

  <glossa lingua="en">A manual: a work intended for action - it
    teaches what is to be DONE. The reader wants to accomplish some
    task and receives the steps from the text; a dictionary says
    what a thing is, a manual says how it is done.</glossa>
  <glossa lingua="fr">Un manuel : une œuvre destinée à l'action -
    elle enseigne ce qu'il faut FAIRE. Le lecteur veut accomplir
    une tâche et reçoit du texte les étapes ; le dictionnaire dit
    ce qu'est une chose, le manuel dit comment elle se
    fait.</glossa>

  <nota>
    Casus mensuratus: peregrinus manuale postulabat et paginam
    dictionarii accepit - descriptio integra, instrumentum nullum
    (lacuna L9).
  </nota>
</genus>

<genus nomen="exemplum" sub="opus_scriptum"
  modulus="opus_scriptum">

  <differentia>
    Specimen INTEGRUM ad imitandum: non describit sed monstrat.
    Lector exemplum transcribit et mutat; integritas differentiam
    fert - fragmentum demonstrat, exemplum incipere facit.
  </differentia>

  <glossa lingua="en">A worked example: a complete specimen meant
    for imitation - it shows rather than describes. The reader
    copies it and adapts it; completeness is the differentia - a
    fragment demonstrates, a worked example gets you
    started.</glossa>
  <glossa lingua="fr">Un exemple complet : un spécimen intégral
    destiné à l'imitation - il montre au lieu de décrire. Le
    lecteur le transcrit et l'adapte ; l'intégralité fait la
    différence - un fragment démontre, un exemple complet fait
    démarrer.</glossa>

  <nota>
    natura/exempla/hortus.census exemplum est - corpus tandem
    verbum habet pro eo quod iam continebat (lacuna L10).
    NB: 'exemplar' formato reservatum; hoc genus 'exemplum'.
  </nota>
</genus>

<genus nomen="legenda" sub="opus_scriptum"
  modulus="opus_scriptum">

  <differentia>
    Clavis notationis: quid signa in opere alio significent docet.
    Sine legenda notatio muta est - lector signum videt nec scit
    utrum ornamentum an sententia sit.
  </differentia>

  <glossa lingua="en">A legend: the key to a notation - it teaches
    what the signs in another work mean. Without a legend a
    notation is mute: the reader sees a mark and cannot tell
    ornament from statement.</glossa>
  <glossa lingua="fr">Une légende : la clef d'une notation - elle
    enseigne ce que signifient les signes d'une autre œuvre. Sans
    légende, une notation est muette : le lecteur voit un signe
    sans savoir s'il est ornement ou énoncé.</glossa>

  <nota>
    Legenda opus parvum intra opus maius esse solet - compositio
    per opus_scriptum.pars_de, iam exsistentem (furca V: partes
    opera sunt, genus partis nullum).
  </nota>
</genus>

<!-- ============= DEFECTUS DOCUMENTI ============== -->

<genus nomen="defectus_documenti">

  <definitio>
    Quod textus lectori debuit nec lectio reddit: discrepantia
    inter debitum documenti et quod lector inde construere potest.
    Vitio parallelum SINE hereditate - vitium intentionem contra
    mores fert; documentum mores non habet, non agit sed legitur:
    alterum membrum hic LECTIO est.
  </definitio>

  <glossa lingua="en">A document defect: what the text owed the
    reader and a reading does not yield - the discrepancy between
    what the document owed and what a reader can construe from it.
    Parallel to vitium without inheriting from it: a vitium sets
    intention against behavior, and a document has no behavior -
    it does not act, it is read.</glossa>
  <glossa lingua="fr">Un défaut de document : ce que le texte
    devait au lecteur et qu'une lecture ne rend pas - l'écart
    entre ce que le document devait et ce que le lecteur peut en
    tirer. Parallèle au vitium sans en hériter : le vitium oppose
    l'intention au comportement, et un document n'a pas de
    comportement - il n'agit pas, il se lit.</glossa>

  <nota>
    Trichotomia vi ordinata: deceptio (victimam habet), silentium,
    ambiguitas. Defectus contra OPUS lectoris definitur, non
    contra normam solam; inventio ordinaria per ambulationem
    lectoris.
  </nota>

  <proprietates>
    <proprietas nomen="contentum" genus="textus"
      nota="quid praecise defuit aut fefellit, verbis"/>
  </proprietates>

  <relationes>
    <relatio nomen="in_opere" ad="opus_scriptum"
      modulus="opus_scriptum"
      nota="ubi inhaeret - defectus semper alicuius operis est"/>
  </relationes>
</genus>

<genus nomen="deceptio" sub="defectus_documenti">

  <differentia>
    Textus falsum DOCET: lectio recta lectorem in errorem ducit.
    Victimam habet, unde vis maxima - malum non quod deest sed
    quod mendaciter adest.
  </differentia>

  <glossa lingua="en">Active misleading: the text teaches a
    falsehood - a correct reading leads the reader into error. It
    has a victim, hence its rank first in force: the trouble is
    not what is missing but what is falsely present.</glossa>
  <glossa lingua="fr">L'égarement actif : le texte enseigne une
    fausseté - une lecture correcte conduit le lecteur à l'erreur.
    Il a une victime, d'où son rang premier en force : le mal
    n'est pas ce qui manque mais ce qui est faussement
    présent.</glossa>

  <nota>
    Casus mensuratus: truncatio '|...' valorem 'mortua' celavit -
    pagina docuit mortem plantae dici non posse, dum
    'status-vita="mortua"' semper transibat. A depravatione
    distinguitur: illa datum pravum, haec textus qui falsum DOCET.
  </nota>
</genus>

<genus nomen="silentium" sub="defectus_documenti">

  <differentia>
    Textus tacet quod scit: scientia exstat et narratio eam
    retinet. Ab ignoto iudicii distinguitur - illud lacuna
    LECTORIS nominata, hoc lacuna NARRATIONIS.
  </differentia>

  <glossa lingua="en">Silence: the text withholds what it knows -
    the knowledge exists and the telling omits it. Distinct from
    iudicium's ignotum, which is the reader's named gap; this is
    the page's side of the same absence.</glossa>
  <glossa lingua="fr">Le silence : le texte tait ce qu'il sait -
    le savoir existe et le récit l'omet. Distinct de l'ignotum du
    iudicium, qui est la lacune nommée du lecteur ; ici c'est le
    versant de la page de la même absence.</glossa>

  <nota>
    Casus mensuratus: elementum radicis documenti numquam in
    pagina nominatur - peregrinus radicem divinare debuit.
  </nota>
</genus>

<genus nomen="ambiguitas" sub="defectus_documenti">

  <differentia>
    Textus plures sensus admittit nec fundamentum eligendi
    praebet: lector inter construenda sine causa eligere cogitur.
  </differentia>

  <glossa lingua="en">Ambiguity: the text admits several readings
    and gives the reader no ground to choose among them.</glossa>
  <glossa lingua="fr">L'ambiguïté : le texte admet plusieurs
    lectures et ne donne au lecteur aucun fondement pour choisir
    entre elles.</glossa>

  <nota>
    Casus mensuratus: signum '...' tria diversa in eadem pagina
    significabat (truncatio electionis, textus liber, vacuum).
  </nota>
</genus>

<!-- ============= AMBULATIO LECTORIS ============== -->

<genus nomen="ambulatio_lectoris" sub="probatio"
  modulus="fabricatio">

  <differentia>
    Probatio documenti per lectorem RESTRICTUM: lector ad fontem
    sub probatione solum adstringitur, ut fons ipse mensuretur.
    Oraculum non assertio sed successus lectoris in opere dato.
  </differentia>

  <glossa lingua="en">A reader-walk: a test of a document by a
    restricted reader - the reader is confined to the source under
    test, so that the source itself is what gets measured. The
    oracle is not an assertion but the reader's success at a
    stated task.</glossa>
  <glossa lingua="fr">Une marche de lecteur : un test d'un
    document par un lecteur restreint - le lecteur est confiné à
    la source testée, afin que ce soit la source elle-même qui
    soit mesurée. L'oracle n'est pas une assertion mais la
    réussite du lecteur à une tâche donnée.</glossa>

  <nota>
    Propositiones ambulationis vehicula exsistentia vehunt:
    annotatio (lector programma), iudicium (lector persona),
    investigatio quae parit postulata (index emendationum).
    Proprietas hereditaria contentum opus temptatum fert ('quid
    interrogetur' - hic: quid lector facere conatus sit).
  </nota>

  <relationes>
    <relatio nomen="lector" ad="*"
      nota="persona aut programma - eadem apertio consulta ac
            scriba (parcum 01KZFM3GW0: genus ministri machinalis
            dilatum donec census verus tag poscat)"/>
    <relatio nomen="invenit" ad="defectus_documenti"
      multiplex="verum"
      nota="defectus quos ambulatio in aperto posuit"/>
    <relatio nomen="exercet" ad="opus_scriptum"
      modulus="opus_scriptum" multiplex="verum"
      nota="restrictio relationis hereditariae (praecedens
            versio_de): lector codicem physice legit, ambulatio
            OPUS ut expressum probat"/>
  </relationes>
</genus>

</natura>
```

- [ ] **Step 2: Let the hook judge, then verify the gate directly**

The natura-custos hook fires on save. Then run cold:
`bin/natura_examen -plagula natura/documentatio.genera` — expected
exit 0, and corpus-wide vulnera unchanged (only the 2 standing
inversa). Exit 2 = nothing ran = defect.

- [ ] **Step 3: Regenerate the chain**

Run: `./tools/natura_canones.sh && ./tools/canon_coquere.sh`
Expected: canon count reports 35 (was 34); a new
`natura/cocta/documentatio.canon` appears; `planta_lectio`
regenerates untouched-or-trivially (no planta kinds changed).
Then: `./tools/natura_canones.sh -probare` → exit 0.

- [ ] **Step 4: Glossae gate + INDEX**

Run: `./tools/natura_glossae.sh -porta` — expected exit 0, SUMMA
**187/187** in la/en/fr (documentatio row 9/9 9/9 9/9).
Run: `./tools/natura_visus.sh` — regenerates INDEX.md (now 187
genera) and the HTML; expected exit 0.
Run: `./tools/natura_glossae.sh -pagina && ./tools/natura_glossae.sh -probare`
to refresh + byte-verify `natura/cocta/glossae.html`.

- [ ] **Step 5: Run the suites**

Run: `./compile_tests.sh natura` (matches probatio_natura,
probatio_natura_canones, probatio_natura_glossae) and
`./compile_tests.sh planta_lectio`. Expected: all PASS.

- [ ] **Step 6: Commit**

```bash
git add natura/documentatio.genera natura/INDEX.md natura/cocta \
  include/planta_lectio.h lib/planta_lectio.c
git commit -m "documentatio: novem genera - quattuor genera operis, trichotomia defectuum, ambulatio lectoris

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 3: The scenario judges the module — census + probes

**Files:**
- Create: `natura/exempla/documentatio.census`
- Modify: `probationes/probatio_natura_canones.c` (extend section XVIII)
- Modify: `natura/natura.worklog.md` (append entry)

**Interfaces:**
- Consumes: Task 2's kind tags and relation projections; registry route
  `.census → natura/cocta/individua.canon` (canones.registrum);
  disk-judging pattern from suite section XI (~line 1459:
  `filum_legere_totum("natura/cocta/semina.census", piscina)` then the
  same judgment helper).
- Produces: the module's standing proof fixture.

- [ ] **Step 0: Verify projection shapes** before writing the census:
  `bin/natura_canones -inspicere ambulatio_lectoris` and
  `-inspicere defectus_documenti` — confirm `lector=`/`in-opere=`
  project as attributes (single) and `invenit`/`exercet` as cited
  children (multiplex). If any differs, adjust the census below to
  match the fold — the fold is the authority.

- [ ] **Step 1: Write the census** — the peregrinus scenario in the
  new vocabulary:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!--
  natura/exempla/documentatio.census
  Scenarium 2026-08-07/08 in vocabulario novo: pagina glossarum,
  peregrinus restrictus, defectus tres, index emendationum.
  Probatio moduli documentatio - eadem res quae ledger lacunarum
  peperit, nunc DICIBILIS. Iudicatur per monolithum (registrum:
  .census -> natura/cocta/individua.canon).
-->
<individua>

  <programma nomen="#peregrinus"/>

  <dictionarium nomen="#pagina-glossarum"/>

  <exemplum nomen="#hortus-census"/>

  <ambulatio-lectoris nomen="#ambulatio-peregrini"
    lector="#peregrinus"
    contentum="censum validum ex pagina sola scribere">
    <exercet ad="#pagina-glossarum"/>
    <invenit ad="#deceptio-truncationis"/>
    <invenit ad="#silentium-radicis"/>
    <invenit ad="#ambiguitas-punctorum"/>
  </ambulatio-lectoris>

  <deceptio nomen="#deceptio-truncationis"
    in-opere="#pagina-glossarum"
    contentum="truncatio electionum valorem mortua celavit -
               mors plantae indicibilis docta"/>

  <silentium nomen="#silentium-radicis"
    in-opere="#pagina-glossarum"
    contentum="elementum radicis documenti numquam nominatur"/>

  <ambiguitas nomen="#ambiguitas-punctorum"
    in-opere="#pagina-glossarum"
    contentum="signum trium punctorum tria diversa significat"/>

  <postulatum nomen="#postulatum-legendae" ordo="1"
    contentum="legenda signorum addenda"/>

</individua>
```

The committed fixture must judge GREEN — red probes live only in the
C suite, never in the fixture. (`#pagina-glossarum` carries no scriba
child: the glossators wrote the glossae corpus, not this page's
authorship claim, and the census states only what the scenario
measured.)

- [ ] **Step 2: Judge it from disk**

Run: `bin/canon_examen natura/exempla/documentatio.census`
Expected: exit 0, zero vitia (registry routes by extension). Exit 2 =
nothing judged = defect. If red, fix the census (or the module, if
the census exposed a real wound — record which in the worklog).

- [ ] **Step 3: Extend suite section XVIII** — disk-green + four red
  probes (each verified red for the right reason; the pre-fix oracle
  here is the assertion itself, since the module already exists —
  verify each red probe RED by running, and if any comes back green,
  STOP and diagnose before proceeding):

```c
        /* census scenarii ex disco viret (fixtura stans) */
        fons = filum_legere_totum(
                   "natura/exempla/documentatio.census", piscina);
        /* iudica eodem modo ac sectio XI (semina.census) */

        /* invenit defectibus clausum: dictionarium non-defectus */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <dictionarium nomen=\"#pag\"/>\n"
            "  <ambulatio-lectoris nomen=\"#amb\">\n"
            "    <invenit ad=\"#pag\"/>\n"
            "  </ambulatio-lectoris>\n"
            "</individua>\n",
            CANON_CITATIO_IRRITA, &generis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (generis, (i32)I);

        /* exercet in ambulatione RESTRICTUM: functio rubet
         * (parens apertus, species angustat) */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <functio nomen=\"#f\"/>\n"
            "  <ambulatio-lectoris nomen=\"#amb\">\n"
            "    <exercet ad=\"#f\"/>\n"
            "  </ambulatio-lectoris>\n"
            "</individua>\n",
            CANON_CITATIO_IRRITA, &generis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (generis, (i32)I);

        /* in-opere ad ignotum rubet */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <silentium nomen=\"#s\" in-opere=\"#nusquam\"/>\n"
            "</individua>\n",
            CANON_CITATIO_IRRITA, &generis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (generis, (i32)I);

        /* dentes doctrinae PERSEVERANT: auctor in dictionario
         * (hereditate ab opere scripto) personis clausus manet */
        CREDO_VERUM (_documentum_iudicare(monolithus,
            "<individua>\n"
            "  <programma nomen=\"#robotum\"/>\n"
            "  <dictionarium nomen=\"#male\">\n"
            "    <auctor ad=\"#robotum\"/>\n"
            "  </dictionarium>\n"
            "</individua>\n",
            CANON_CITATIO_IRRITA, &generis, &omnia,
            piscina, intern));
        CREDO_AEQUALIS_I32 (generis, (i32)I);
```

(For the disk-green block, copy section XI's exact judgment call —
same helper, `fons` instead of a literal; assert `omnia == 0`.)

- [ ] **Step 4: Run the suites — full sweep**

Run: `./compile_tests.sh natura && ./compile_tests.sh canon &&
./compile_tests.sh planta_lectio`
Expected: all PASS. Then `./tools/natura_canones.sh -probare` → exit 0.

- [ ] **Step 5: Worklog entry** — append to `natura/natura.worklog.md`
  (English fine): the module's birth, the five rulings one line each,
  what the census forced (anything found in Step 2), the L2-precedent
  reuse, and the deferred list (minister machinalis parked; genre
  species beyond four; pars_de multiplex).

- [ ] **Step 6: Commit**

```bash
git add natura/exempla/documentatio.census \
  probationes/probatio_natura_canones.c natura/natura.worklog.md
git commit -m "documentatio: scenarium ipsum modulum iudicat - census viret, probae rubrae quattuor

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

## Self-review notes (done at write time)

- Spec coverage: seam + 9 genera (Task 2), exercet widening (Task 1),
  census + negative probes (Task 3), glossae 187/187 (Task 2 step 4),
  chain green (every task), deferrals recorded in worklog (Task 3
  step 5). opus_scriptum untouched — no task touches it. ✓
- The Task 3 fixture is green-only; red probes live only in the C
  suite. ✓
- Names/types: kind tags kebab-cased consistently; `in_opere` →
  `in-opere=`; helper `_documentum_iudicare` signature copied from
  section XVII verbatim. ✓
- Known risk, accepted: attribute-vs-child projection guesses are
  gated by Task 3 Step 0 (`-inspicere` is the authority).
