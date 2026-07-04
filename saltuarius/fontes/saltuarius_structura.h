/* saltuarius_structura.h - Index structurae / TOC (Phase D, D1)
 *
 * Tabula contentorum ex vistis additionum II: INCLUSIONES /
 * DEFINITIONES / REGIONES plagulae PRINCIPALIS (filtrum
 * fons_princeps - latina.h sola tabulam ~L definitionibus
 * mergeret; visus omnium fontium v0.1 nominatus). Semantica
 * diarii SERVATA: redefinitiones bis apparent, #undef nihil
 * delet - diarium est fructus.
 *
 * NB FULMEN SEXTUM: "structura" IPSA macro latina est (= struct)!
 * Typus SaltuariusStructura licet (signum aliud); identificator
 * nudus "structura" NUMQUAM - variabiles hic "index" vocantur
 * (index = tabula contentorum, latine proprie).
 *
 * Exemplar originis: arena propria refecta, ordines ad aperturam
 * RECENTES aedificati (cache nullum - O(vistae) aedificatio,
 * macro_vista O(acta) per vocationem, trivialis ad mensuram TOC).
 */

#ifndef SALTUARIUS_STRUCTURA_H
#define SALTUARIUS_STRUCTURA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "saltuarius_liber.h"

nomen enumeratio {
    SALT_ORDO_TITULUS = 0,   /* caput sectionis - non saltabile */
    SALT_ORDO_INCLUSIO,      /* saltus: IN plagulam inclusam */
    SALT_ORDO_DEFINITIO,     /* saltus: linea plagulae currentis */
    SALT_ORDO_REGIO,         /* saltus: linea plagulae currentis */
    SALT_ORDO_FUNCTIO,       /* M2c D: definitio functionis (c89) */
    SALT_ORDO_PROTOTYPUM,    /* M2c D+: declarator functionis
                              * sine corpore (prototypum; NB
                              * variabiles monstratorum functionum
                              * sine initiatore hic quoque cadunt -
                              * vista genus solum fert) */
    SALT_ORDO_TYPUS,         /* M2c D: typedef (c89) */
    SALT_ORDO_DECLARATIO     /* M2c D: declaratio cetera (c89) */
} SaltuariusOrdoGenus;

nomen structura {
    SaltuariusOrdoGenus genus;
    chorda              titulus;   /* textus ordinis (in arena) */
    b32                 saltabile;
    chorda              via;       /* INCLUSIO: via ut scripta */
    i32                 linea;     /* DEFINITIO/REGIO: 1-basata.
                                    * REGIO: linea CORPORIS
                                    * bracchii (ex corpus_initium
                                    * derivata - vista lineam
                                    * regionis communem fert);
                                    * bracchium vacuum -> linea
                                    * regionis (refugium) */
} SaltuariusOrdo;

nomen structura {
    Piscina*        arena;     /* refecta per aedificationem */
    PiscinaNotatio  nota;
    SaltuariusOrdo* ordines;   /* in arena */
    i32             numerus;
    s32             selectio;
    s32             volumen;   /* primus ordo visibilis */
    b32             apertum;
} SaltuariusStructura;

SaltuariusStructura*
saltuarius_structura_creare (Piscina* persistens);

/* Indicem aedificare ex parsura libri. VERUM si ordines nati
 * (tabula aperienda); FALSUM si nihil narrabile (merus / sine
 * parsura / vacuum). Selectio in primum ordinem saltabilem. */
b32
saltuarius_structura_aedificare (SaltuariusStructura* index,
    constans SaltuariusLiber* liber);

vacuum
saltuarius_structura_claudere (SaltuariusStructura* index);

/* Selectionem movere (clausa; tituli sectionum selectabiles sed
 * non saltabiles - exemplar originis) */
vacuum
saltuarius_structura_movere (SaltuariusStructura* index,
    s32 delta);

/* Volumen aptare ut selectio intra altitudinem visibilem sit
 * (pictor vocat cum altitudine interiore tabulae) */
vacuum
saltuarius_structura_aptare (SaltuariusStructura* index,
    s32 altitudo);

/* Ordo selectus si saltabilis; NIHIL alioquin */
constans SaltuariusOrdo*
saltuarius_structura_saltus (constans SaltuariusStructura* index);

#endif /* SALTUARIUS_STRUCTURA_H */
