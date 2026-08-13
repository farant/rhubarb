
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
