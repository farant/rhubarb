# Fixa thistle — inventarium

| plagula | forma | partes | nota |
|---|---|---|---|
| `thistle/salve.thistle` | programma planum | prosa, regio c, prosa, regio c(probatio) | briar-spec §2 gemellus planus |
| `thistle/salve_vitreum.thistle` | app atrii | prosa, elementum fenestra, prosa, regio html, prosa, regio c(methodus), prosa, regio c(probatio) | briar-spec §2 verbatim; saeptum ```html in prosa = lex saepti |
| `thistle/adversa/*.thistle` | casus limitis | vide probatio_briar_arbor.c | plan 1, Task 4 |
| `thistle/punctum.thistle` | partitio (typedef + #define + globale + adiutor + principale + probatio) | vide probatio_briar_fabrica.c | plan 2, Task 3 |
| `thistle/adversa/c_fractum.thistle` | regio C fracta (`@@@` linea XI) | linea erroris silvae -> .thistle | plan 2, Task 1 |
| `thistle/adversa/{duo_principalia,nihil,fenestra_et_principale,methodus_absens,methodus_prava,html_duplex,fenestra_prava}.thistle` | recusationes fabricae cum linea | vide probatio_briar_fabrica.c | plan 2, Task 5 |
| `thistle/adversa/probatio_rubra.thistle` | PLANTA portae fumi: probatio quae cadere DEBET (`CREDO_FALSUM(VERUM)`) | tools/briar_fumus.sh gradus V | plan 3, Task 4 |
| `thistle/derivatum.thistle` | NULLUM #include: typus (Piscina), functiones (chorda_ex_literis, piscina_*), macro (CREDO_AEQUALIS_I32) derivata | probatio_briar_silva.c + probatio_briar_fabrica.c (aurum fabrica/derivatum) | inclusiones derivatae, 2026-09-05 |
| `amalgama/fabrica/` | FABRICA FICTA (include/latina.h DECURTATUM, postulata_posix.h, alpha.h/.c, beta.h/.c, corpus.symbola.tsv MANU): statica `_est_spatium` in utraque bibliotheca, typedef `Cella` + variabile `_cella` + macro `ALPHA_MAXIMUM` localia, beta alpham includit | probatio_briar_amalgama.c (aurum amalgama/gamma.c) | -amalgama, 2026-09-05 |
| `amalgama/gamma.thistle` | sine #include (beta.h derivatum), principale solum: `beta_duplicare("ab cd")` = 8 | probatio_briar_amalgama.c; manu: clang linea II auri -> 8 | -amalgama, 2026-09-05 |
| `thistle/adversa/amalgama_venditorium.thistle` | `#include "volumen.h"` -> vendor/sqlite3.c in clausura: fabrica sana, -amalgama RECUSAT | probatio_briar_amalgama.c | -amalgama, 2026-09-05 |

Snapshots manu scripti 2026-09-04 (plan 1). Mutatio = mutatio pinnarum
in probationibus arbor/stml/nexus/computus - causa nominanda.

## Aurum computi

`computus/basis.tsv` — columnae XI (octeti, lexemata, nodi, octeti_stml,
usus, commissa, otiosa, apex, alvei, allocationes, allocationes_arboris)
super `salve.thistle` et `salve_vitreum.thistle`; natum 2026-09-04
(causa: nativitas portae). Regeneratio `COMPUTUS_SCRIBERE=1` cum causa
nominata in commissione.

## Aurum fabricae

`fabrica/<t>/…` — plagulae genitae (fontes/, include/, probationes/,
assets/, ordines plani) pro `salve`, `punctum`, `salve_vitreum`,
octetim comparatae (`CREDO_CHORDA_AEQUALIS`); natae 2026-09-05 (causa:
nativitas portae). Ordines VITREI non aurum: listae fontium e corpore
computatae mutarentur cum omni bibliotheca — assertiones structurales.
Regeneratio `BRIAR_FABRICA_SCRIBERE=1 ./briar/compile_probationes.sh
fabrica` cum causa nominata in commissione; inspectio manu ante.

## Aurum amalgamae

`amalgama/gamma.c` — plagula una super fabricam FICTAM
`amalgama/fabrica/` (non corpus verum: amalgama corporis veri cum omni
mutatione lib/ mutaretur — corpus verum STRUCTURALITER asseritur),
octetim comparata; nata 2026-09-05 (causa: nativitas portae), manu
compilata linea II sua (`clang … gamma.c -o gamma`), imprimit `8`.
Regeneratio `BRIAR_AMALGAMA_SCRIBERE=1 ./briar/compile_probationes.sh
amalgama` cum causa nominata in commissione; inspectio manu ante.
