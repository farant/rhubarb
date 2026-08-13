# Probatio vitreae — bibliotheca assertionum integrantium

**Status**: consilium (nihil aedificatum). 2026-08-12.
**Fundamentum**: `imperium` (SHIPPED) - vide `lib/imperium.worklog.md`.

---

## §0 Quid probandum sit — et quid NON

Tres viae probandi interfaciem in hac domo iam sunt, et quaeque suum
defectum habet:

| via | quid probat | defectus |
|---|---|---|
| suturae (missor/aestimator) | logicam C | nihil de RENDITIONE |
| `-fumus` | applicationem veram | asserta in C COMPILATA - quaestionem novam sine refectione rogare non licet |
| osascript + DOM falsum | logicam paginae | **QUATER viridis et MENDAX** (probatio_fori.js) - JS meus contra DOM meum |

`imperium` quartam aperit: applicatio VERA, DOM VERUS, per HTTP. Haec
bibliotheca eam ERGONOMICAM facit.

**Doctrina cardinalis: probatio quae ad simulacrum spectat probatio
non est.** Ideo haec bibliotheca ad imperium SOLUM ligetur - nulla via
sit qua DOM falsus supponi possit. Defectus historicus (quater!) hinc
ortus est.

---

## §1 CARDO: mora, non assertio

Asserta pars facilis sunt: `credo` iam CREDO_* et compendium fert;
"textum huius selectoris lege" macro unius horae est.

**Quod suitas interfaciei semper necat ASYNCHRONIA est.** Ictum
immittis; DOM se renovat... quando? Omnis probatio interfaciei fragilis
in historia mundi vitium TEMPORIS est. Et quisquis `sleep` scribit
suitam in alteram partem mentiri docet: transit quia CC millisecunda
hodie in hac machina satis fuerunt.

**Ergo munus primum bibliothecae: moram FACILEM facere, somnum
DIFFICILEM.**

```c
vitrea_expecta(sess, "#experimenta .tessera", MORA_BREVIS);
CREDO_VITREA_TEXTUS(sess, "#titulus", "Laboratorium 0001");
```

`expecta` DOM VERUM per pontem interrogat donec condicio impleatur aut
terminus transeat, et FRACTA nuntiat *quid exspectaverit et quid
viderit* - non solum "falsum".

Si somnus nudus commode scribi non potest, nemo eo utetur. Id consilium
est, non commoditas.

## §2 Forma proposita

```c
ProbatioVitreae* sess = probatio_vitreae_aperire(piscina, "127.0.0.1", portus);

/* actio */
vitrea_premere(sess, "#nav-experimenta");
vitrea_scribere(sess, "#quaestio", "utrum nexus adiuvent?");

/* mora - numquam somnus */
vitrea_expecta(sess, ".tessera", MORA_BREVIS);
vitrea_expecta_textum(sess, "#status", "perfectum", MORA_LONGA);

/* assertio */
CREDO_VITREA_EXISTIT(sess, ".experimentum");
CREDO_VITREA_TEXTUS(sess, "#titulus", "0001");
CREDO_VITREA_NUMERUS(sess, ".tessera", III);   /* quot congruunt */

/* imago (armario postea, §3 consilii laboratorii) */
vitrea_imaginem(sess, "/tmp/0001.png");
```

Quaeque functio sub tegumento: JS componit → POST /imperium → tesseram
petit donec perficiatur → valorem reddit.

## §3 Quaestiones apertae

1. **Nomen.** `probatio_vitreae`? `agitator`? `manus`? (Currendum
   `bin/natura quaere` ANTE - lex quinquies mordax.)
2. **Terminus ordinarius**: quot millisecunda? Suadeo II s cum
   nuntio claro; longiores explicite dicendae.
3. **Quomodo `expecta` interroget**: iterum-atque-iterum POST (simplex,
   sed strepitus) an unum JS quod intus `MutationObserver` ponit et
   pontem semel vocat (mundius, sed status in pagina)? Suadeo primum
   donec strepitus doleat.
4. **Applicationem ipsam gubernare**: an probatio applicationem
   `-vivum` incipiat et occidat (`processus_incipere/metere`), an
   currenti se applicet? Suadeo utrumque, prius ordinarium.

## §4 Porta ad nativitatem

Culpa PLANTATA ante fidem (lex domus): probatio quae `expecta` ferat
RUBRA fieri debet cum elementum numquam apparet, et nuntius quid
exspectatum sit NOMINET. Sine hoc bibliotheca somnum meliorem non est.
