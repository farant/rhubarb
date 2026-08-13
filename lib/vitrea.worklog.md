
## 2026-08-13 — errores paginae ad C (canalis 'erratum')

Vitrea nunc custodes errorum in omnem paginam ponit, et quod capiunt
in C mittit. Ordinarie in stderr clamatur; `vitrea_erratorem_ponere`
suturam praebet applicationi quae eos condere velit.

**Applicatio NIHIL mutare debet.** Mensuratum in laboratorio, cuius
laboratorium.c non tactum est:

```
[vitrea] erratum paginae: {"genus":"exceptio","nuntius":"ReferenceError:
  Can't find variable: functio_nusquam_definita","ubi":"undefined:1"}
[vitrea] erratum paginae: {"genus":"promissum","nuntius":"promissum
  orbum","ubi":""}
```

### Cur canalis SEPARATUS et non cauda pontis

Applicationes omnes hodie hunc morem servant:

```c
si (genus == VITREA_NUNTIUS_PONS) internuntius_tractare(...);
alioquin                          vitrea_recargare(vitrea);
```

Quidquid ergo PONS non est RECARGATIONEM parit. Error per caudam
missus paginam in gyrum cadendi-recargandi-cadendi verteret - vitium
peius quam illud quod nuntiare conatur. Genus novum in enumeratione
idem faceret sine quinque applicationibus emendatis.

Canalis alter (`messageHandlers.erratum`) nihil quod exstat tangit:
tractator ObjC per `nuntius.name` dividit, et via erroris caudam
omnino non intrat.

### Quod hoc capit et collector manus non potest

Iniectio `AtDocumentStart`, ergo custodes ANTE scripta paginae
currunt. Mensuratum culpa plantata in laboratorium.js ipso:

```
"ubi":"http://127.0.0.1:17300/assets/laboratorium.js:2"
```

Collector manus post `manus_aperire` iniicitur - onus paginae iam
transiit, et illa exceptio ei invisibilis est in aeternum. Praeterea
hic sedes VERA (plagula:linea) redditur; in codice per eval aestimato
`undefined:1` solum est, quia fons URL caret.

Duo strata ergo COMPLEMENTA sunt, non duplicata: vitrea onus et usum
verum tegit, manus id quod probatio ipsa excitat.

### Notae

- Sutura per FUNCTIONEM ponitur, non per campum in
  VitreaConfiguratio: applicationes figuram campo-post-campum implent
  SINE memset (vide laboratorium.c), ergo campus additus monstratorem
  purgamenti ferret. Additio in fine VitreaFructus tuta est - lectores
  soli.
- `mittere` errorem suum vorat (`catch` vacuum): si canalis abest,
  clamor de clamore gyrum pareret.
- Sub `_nuntium_inserere` cauda plena `console.error` vocat - quod
  nunc per canalem erroris redit. Non est gyrus: console.error viam
  'erratum' adhibet, quae caudam non tangit.

## 2026-08-13 — console.log ad stdout (canalis 'consola')

`console.log` / `warn` / `info` nunc in effusionem processus eunt,
praefixo notata:

```
[vitrea/log] salve ex pagina
[vitrea/log] res: {"a":1,"b":[2,3]} 42 true
[vitrea/monitum] cave
[vitrea/info] nota
```

`console.error` in stderr manet (canalis 'erratum'), ergo `2>/dev/null`
defectus solos tacet. Divisio ordinaria servata: log NUNTIUS est, non
defectus.

### Cur canalis alter, non 'erratum' cum genere novo

Sutura `VitreaErrator` errores accipit. Si logi per eandem viam
irent, applicatio quae errores CONDIT subito omnes lineas consolae
conderet - mutatio morum tacita in codice qui iam scriptus est.
Canalis alter ambo separatos servat.

### Pingere, non String()

`console.log('res', obiectum)` argumenta PLURA fert, et pleraque
chordae non sunt. `String(obiectum)` `[object Object]` reddit - linea
quae ADEST et TACET, pessimum genus actorum. Ergo JSON.stringify cum
reditu duplici: circularia iactant, et `undefined` a stringify
OMITTITUR (non 'undefined' redditur) - utrumque ad String() cadit.

### Linea humana, JSON ad suturam

`_lineam_imprimere` JSON parsat et lineam legibilem scribit; sutura
JSON CRUDUM accipit. Homo `[vitrea/log] salve` legit, applicatio
campos habet. Error paginae quoque hac forma nunc apparet (prius
JSON crudum in stderr).

`fflush` semper: effusio ad FISTULAM (quod fit cum manus
applicationem generat, aut cum acta in plagulam vertuntur) plene
bufferitur, ergo linea non effusa ordine falso apparet aut, si
applicatio cadit, omnino perit.

### Consola PRIOR servata

`involvere` functionem priorem semper vocat post missionem, ergo
inspector Safari quod semper habebat retinet. Additio est, non
substitutio - qui inspectorem aperit nihil amittit.

### Cavendum (nondum mordax)

Pagina in gyro logens effusionem inundat. Sub `manus_incipere`
fistula est, et `processus_pulsare` eam haurit (vide manus.h);
sub terminali nihil obstat. Nullus modus temperandi positus - si
umquam mordet, sedes recta est 'mittere' in JS, non C.
