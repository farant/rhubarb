/* annales_sedes.h - SEDES ANNALIUM: ubi acta tabularii et fori
 * habitant. Domus tota, non arbor: omnes arbores (main, secunda...)
 * eandem sedem legunt. Ordo: $RHUBARB_ANNALES; aliter
 * ~/.rhubarb/annales si exstat; aliter radix arboris (LEGATUM, ante
 * migrationem: gesta/annales/ + scrinia in radice). Viae codicis
 * (build/nexus.tsv ...) ad arborem pertinent, non huc. */

#ifndef ANNALES_SEDES_H
#define ANNALES_SEDES_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

nomen enumeratio {
    ANNALES_EX_AMBITU = ZEPHYRUM,   /* $RHUBARB_ANNALES */
    ANNALES_EX_DOMO,                /* ~/.rhubarb/annales */
    ANNALES_EX_ARBORE               /* legatum: radix arboris */
} AnnaliumOrigo;

nomen enumeratio {
    ANNALES_TABULARII = ZEPHYRUM,   /* tabularium.jsonl (veritas) */
    ANNALES_SCRINIUM_TABULARII,     /* tabularium.db (proiectio) */
    ANNALES_TABULA,                 /* tabula.md */
    ANNALES_ENTIA,                  /* entities/ */
    ANNALES_FORI,                   /* forum.jsonl */
    ANNALES_SCRINIUM_FORI           /* forum.db */
} AnnaliumPlagula;

nomen structura {
    AnnaliumOrigo origo;
           chorda radix_arboris;   /* pro LEGATO */
           chorda directorium;     /* EX_AMBITU / EX_DOMO */
} AnnaliumSedes;

/* sedem invenire: FALSUM (causa) si $RHUBARB_ANNALES directorium
 * non exstans nominat - numquam ad arborem tacite cadit */
b32
annales_sedem_invenire (
     constans character* radix_arboris,
                Piscina* piscina,
          AnnaliumSedes* sedes,
                 chorda* causa);

/* via plena plagulae in sede (legatum: forma vetus servata) */
chorda
annales_via (
    constans AnnaliumSedes* sedes,
           AnnaliumPlagula  plagula,
                   Piscina* piscina);

/* CUSTOS ante aperturam (nulla genesis tacita, MKMD2/VSY50E):
 * annales absentes -> FALSUM nisi genesis expresse licita;
 * annales praesentes sed scrinium absens -> FALSUM, causa nominat
 * restitutionem (gesta_ex_annalibus_restituere) */
b32
annales_custodire (
    constans AnnaliumSedes* sedes,
           AnnaliumPlagula  annales,
           AnnaliumPlagula  scrinium,
                       b32  genesis_licita,
                   Piscina* piscina,
                    chorda* causa);

#endif /* ANNALES_SEDES_H */
