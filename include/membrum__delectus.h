/* membrum__delectus.h - DELECTUS (dropdown): botton qui valorem
 * currentem ostendit et, ictus, menu optionum SUB SE aperit
 * (project-specs/statuae-spec.md, decretum 9). Super elenchus (menu
 * ipsum: mensurae, arbor, figura, claves) - hoc membrum bottonem,
 * statum apertionis et eventa addit. Genus membrum__ = componens UI
 * compositioni destinatum.
 *
 * Hospes plures delectus habere potest (statuae: cyclus, modus);
 * unus tantum simul apertus est.
 *
 * Status in ramo HOSPITIS - campi manifesti eius, scriptore actione
 * hospitis:
 *   delectus         id delectus aperti ("cyclus"); absens = omnes
 *                    clausi. aspectus="popover": dimissio quoque
 *                    claudit
 *   delectus_electa  optio illuminata; absens = nulla
 *
 * Optiones hospitis sunt (Elenchus: textus, habilis), in memoria
 * eius; delectus eas non servat. Electio hospiti REDDITUR
 * (DELECTUS_ELECTUM + index): hospes valorem suum scribit - delectus
 * nihil de significatione optionum scit.
 *
 * Dum apertus, eventa omnia accipit: motus illuminat; ictus aut
 * Reditus in optione habili claudit et eligit; j / k / sagittae
 * movent; Esc et ictus extra claudunt; cetera deglutiuntur.
 */
#ifndef MEMBRUM__DELECTUS_H
#define MEMBRUM__DELECTUS_H

/* <aedilis corpus="lib/membrum__delectus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "mandatum.h"
#include "elenchus.h"

/* exitus eventus */
nomen enumeratio {
    DELECTUS_NIHIL = ZEPHYRUM,  /* nullus delectus apertus (aut
                                 * eventus non muris nec clavis):
                                 * eventus hospitis est */
    DELECTUS_DEGLUTITUM,        /* consumptum, nihil electum */
    DELECTUS_ELECTUM,           /* optio electa (*electum), clausus */
    DELECTUS_CLAUSUM            /* Esc aut ictus extra: clausus */
} DelectusExitus;

/* latitudo bottonis pro textu (FONS_PARVUS): textus + spatium +
 * sagitta */
i32
delectus_bottonis_latitudo (
    chorda textus);

/* botton: nodus PARTES_BOTTONE cum id dato et actione hospitis */
Componens*
delectus_bottonem_componere (
     constans character* id,
                  Fines  fines,
     constans character* actio,
                Piscina* piscina,
    InternamentumChorda* intern);

/* botton pictus: textus et sagitta deorsum; apertus = depressus; non
 * habilis = obscurus (statua sine cyclis) */
vacuum
delectus_bottonem_pingere (
    Mandata* md,
      Fines  fines,
     chorda  textus,
        b32  apertus,
        b32  habilis);

/* delectum 'id' aperire (alium apertum claudit); electa = optio
 * initio illuminata (valor currens), -1 = nulla */
b32
delectus_aperire (
           InsulaRamus* ramus,
    constans character* scriptor,
    constans character* id,
                   s32  electa,
               Piscina* piscina);

b32
delectus_claudere (
           InsulaRamus* ramus,
    constans character* scriptor,
               Piscina* piscina);

/* estne delectus 'id' apertus? id NIHIL: estne ullus? */
b32
delectus_apertus (
    constans InsulaRamus* ramus,
      constans character* id);

/* optio illuminata ex statu, intra [0, numerus); -1 = nulla */
s32
delectus_electa (
    constans InsulaRamus* ramus,
                     i32  numerus);

/* menu delectus aperti sub bottone (sinistro margine alineatum;
 * supra bottonem si infra aream non capit). optiones->electa ex
 * statu impletur a vocatore (delectus_electa). Filius ULTIMUS
 * radicis hospitis addendus. NIHIL si optiones vacuae */
Componens*
delectus_componere (
      constans Elenchus* optiones,
                  Fines  botton,
                  Fines  area,
     constans character* actio,
                Piscina* piscina,
    InternamentumChorda* intern);

/* eventus dum delectus apertus; nodus = componens tactus.
 * DELECTUS_NIHIL si nullus apertus (status non mutatur) */
DelectusExitus
delectus_eventum_agere (
           InsulaRamus* ramus,
    constans character* scriptor,
     constans Elenchus* optiones,
    constans Componens* nodus,
      constans Eventus* ev,
                   s32* electum,
               Piscina* piscina);

#endif /* MEMBRUM__DELECTUS_H */
