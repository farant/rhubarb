/* saltuarius_tabularium.h - Exemplar indicis directorii (Phase A)
 *
 * PURUM: numquam discum tangit. Impletur ab EXTRA (limes disci in
 * principali vivit; probationes tabulas fictas tradunt - iniectio
 * EST sutura probationum, spec-v2 par 3). Possidet: ordinationem
 * (directoria primum, deinde ordine octetorum), filtrum
 * punctatorum, selectionem, volumen (scroll), saltum ad litteram.
 */

#ifndef SALTUARIUS_TABULARIUM_H
#define SALTUARIUS_TABULARIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "iter_directoria.h"

nomen structura {
    DirectoriumIntroitus* introitus;  /* ordinati, sine punctatis */
                     i32  numerus;
                     s32  selectio;   /* -1 si vacuum */
                     s32  volumen;    /* primus index visibilis */
} SaltuariusTabularium;

/* Creare tabularium vacuum (structura ex piscina data) */
SaltuariusTabularium*
saltuarius_tabularium_creare (
    Piscina* piscina);

/* Implere ex tabula introituum (transcribitur, filtratur,
 * ordinatur; selectio ad 0 aut -1 si vacuum, volumen ad 0).
 * Tabula ex PISCINA data allocatur - arena refectionis limitis
 * disci esse potest (A3): structura tabularii arenam persistentem
 * habitat, tabulae in arena reficienda vivunt. */
vacuum
saltuarius_tabularium_implere (
             SaltuariusTabularium* tabularium,
                          Piscina* piscina,
    constans DirectoriumIntroitus* fontes,
                              i32  numerus);

/* Introitus selectus; NIHIL si vacuum */
constans DirectoriumIntroitus*
saltuarius_tabularium_selectum (
    constans SaltuariusTabularium* tabularium);

/* Selectionem movere per delta (clausa ad [0, numerus-1]) */
vacuum
saltuarius_tabularium_movere (
    SaltuariusTabularium* tabularium,
                     s32  delta);

/* Ad primum / ultimum (gg / G) */
vacuum
saltuarius_tabularium_primum (
    SaltuariusTabularium* tabularium);

vacuum
saltuarius_tabularium_ultimum (
    SaltuariusTabularium* tabularium);

/* Saltus ad litteram (f<littera>): proximus introitus cuius
 * titulus littera incipit (insensibilis maiusculis), a
 * selectio+1, cyclice. VERUM si inventus. */
b32
saltuarius_tabularium_saltare (
    SaltuariusTabularium* tabularium,
                     s32  runa);

/* Volumen aptare ut selectio intra fenestram altitudinis datae
 * visibilis sit */
vacuum
saltuarius_tabularium_aptare (
    SaltuariusTabularium* tabularium,
                     i32  altitudo_fenestrae);

#endif /* SALTUARIUS_TABULARIUM_H */
