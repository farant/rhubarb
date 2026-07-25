# fixa sententiarum — contractus

## Cur EXEMPLAR, non plagula viva

`de_imagine.md` hic **COPIA** est plagulae radicis `de-imagine.md`.
Consulto: `de-imagine.md` documentum VIVUM Franis est et mutabitur
(prooemium ipsum dicit *"Addresses not frozen. Reorder freely until
first publication."*). Si probatio plagulam vivam legeret, quaelibet
editio documenti suitam RUBRAM faceret ob causam quae ad lectorem nihil
pertinet.

Ergo: documentum vivum liberum manet ut mutetur; fixum PINNATUM manet.

**Renovare** (cum consulto volumus fixum ad statum novum documenti
adducere — numquam sponte):

```sh
cp de-imagine.md probationes/fixa/sententiae/de_imagine.md
./compile_tests.sh sententiae      # numeri fixi MUTABUNTUR
```

Numeri in `probatio_sententiae.c` figuntur (CCLXXIII sententiae, XIV
partes, VII retractae, XXI loci cum 'obj', VIII gradus, IV
auctoritates). Renovatio fixi eos frangere DEBET si documentum crevit —
id est porta, non molestia. Numeros novos manu recensere, non caece
substituere.

## Conditio: omnia PINNATA

| plagula | conditio | quid probat |
|---|---|---|
| `de_imagine.md` | PIN | corpus verum, CCLXXIII sententiae, XLV KB |
| `punctum_latum.txt` | PIN | idem punctum, involutio ad ~LXX columnas |
| `punctum_angustum.txt` | PIN | idem punctum, involutio ad ~XLV columnas |
| `punctum_crlf.txt` | PIN | idem punctum, fines linearum CRLF |

Nulla VARIANS hic est: omnis contentus manu scriptus aut semel captus
est, nullus ex machina cuius exitus per cursus fluctuet.

## Gemini involutionis

Tria `punctum_*.txt` sunt idem punctum (4.1 de *De Imagine*) tribus
formis scriptum. Differunt **sola** involutione et finibus linearum.

Hoc figunt:

- normalizatio easdem chordas reddit — ergo involutio ad latitudinem
  aliam ancoram NON frangit, quod est totum consilium §V.1
- sigilla congruunt
- gradus `[theorem]` ex forma normata DETRAHITUR
- locus `4.1.` ex forma normata DETRAHITUR

Cum novum genus normalizationis additur, gemellum novum hic addendum
est quod **solam** eam mutationem fert. Gemellus qui duo simul mutat
causam differentiae non probat.

## Porta absentiae

`_fixum()` in `probatio_sententiae.c` frangit si plagula abest AUT
vacua est. Ratio: `filum_legere_totum` chordam VACUAM pro plagula
absente reddit, et lector chordae vacuae "nihil inventum" laete
nuntiat — ergo via mendosa probationem VIRIDEM sine ulla assertione
pareret. Ne portam illam removeas.
