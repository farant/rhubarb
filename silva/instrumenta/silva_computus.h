/* silva_computus.h - imago memoriae et temporis parseris (RP §11)
 *
 * Una plagula, una mensura: lexatio sola in piscina propria, deinde
 * parsura plena (clausura inclusionum intra) in piscina altera
 * propria. Piscina PROPRIA cum alveo initiali fixo
 * (SILVA_COMPUTUS_ALVEUS_INITIUM), ergo numeri inter plagulas et
 * inter commissiones conferibiles sunt - piscina vocatoris numeros
 * suos ferret.
 *
 * Numeri quos RP (project-specs/rhubarb-performance-sententiae.md)
 * nominat: dati (usus), commissa (usus + otiosa), otiosa, alvei,
 * lexemata, allocationes (Xar per lexema quattuor - RP 3.2; numerus
 * allocationum eos superat, ergo ipse est mensura). Tempus per
 * clock() (CPU), ms.
 *
 * Instrumentum: ./silva/computus.sh <fons.c> [-nudum] [-machina].
 * Porta: probatio_silva_computus (corpus fixum, aurum
 * fixa/computus/basis.tsv - numeri deterministici pinnati, tempora
 * non).
 */
#ifndef SILVA_COMPUTUS_H
#define SILVA_COMPUTUS_H

#include "latina.h"
#include "piscina.h"

#define SILVA_COMPUTUS_ALVEUS_INITIUM ((memoriae_index)(MMMMXCVI * CCLVI))

nomen structura {
    constans character* via;
    constans character* textus;
                   i32  mensura;
} SilvaComputusClausura;

nomen structura {
               i32 octeti_fontis;
               i32 lexemata;              /* fluxus expansus (parsura) */
               i32 lexemata_cruda;        /* silva_lexare sola */
               i32 errores;               /* nodi ERROR */
               b32 successus;
    memoriae_index usus;                  /* dati (handed out) */
    memoriae_index commissa;              /* usus + otiosa (committed) */
    memoriae_index otiosa;                /* inutilis (idle) */
    memoriae_index apex;                  /* apex usus */
    memoriae_index alvei;                 /* numerus alveorum */
    memoriae_index allocationes;          /* numerus allocationum */
    memoriae_index usus_lexandi;          /* piscina lexationis solius */
    memoriae_index allocationes_lexandi;
            duplex ms_lexandi;
            duplex ms_parsandi;           /* parsura plena, lexatione inclusa */
} SilvaComputus;

/* clausura: capita praebenda (NIHIL / 0 = nudum, sine expansione).
 * VERUM si parsura successit; numeri semper impleti. */
b32
silva_computus_metiri (
                constans character* via,
                constans character* fons,
                               i32  mensura,
    constans SilvaComputusClausura* clausura,
                               i32  numerus_clausurae,
                     SilvaComputus* exitus);

#endif /* SILVA_COMPUTUS_H */
