/* crusta_computus.h - imago memoriae et temporis parseris crustae
 * (gemellus silva_computus.h; RP par. 11) - mensura semitae
 * MATERIAE per clientem sextum (P10; gemellus html_computus).
 *
 * Una plagula, una mensura: parsura (lexatio + adaptatio + descensus
 * intra) in piscina PROPRIA cum alveo initiali fixo (numeri inter
 * plagulas et commissiones conferibiles), deinde emissio octetorum,
 * scriptura et lectio STML, comparator arborum in piscina altera
 * (numeri parsurae puri manent). Tempus per clock() (CPU), ms.
 *
 * CUR NUNC: instrumenta perficiendi omnia (computus, phases,
 * aequivalentia, sample) semitam silvae metiuntur; phasis V (C89 per
 * materiam, probatio acceptionis) ordines EOSDEM petet - basis eius
 * hic nascitur, super clientem qui iam vivit.
 *
 * Instrumentum: ./crusta/computus.sh <plagula.sh> [-machina] [-iter N].
 * Porta: probatio_crusta_computus (fixa/crusta et plagulae domus, aurum
 * crusta/probationes/fixa/computus/basis.tsv - numeri deterministici
 * pinnati, tempora non; COMPUTUS_SCRIBERE=1 + causa nominata).
 */
#ifndef CRUSTA_COMPUTUS_H
#define CRUSTA_COMPUTUS_H

#include "latina.h"
#include "piscina.h"

#define CRUSTA_COMPUTUS_ALVEUS_INITIUM \
    ((memoriae_index)(MMMMXCVI * CCLVI))

nomen structura {
               i32 octeti_fontis;
               i32 nodi;                  /* radix inclusa */
               i32 lexemata;              /* loci + listae */
               i32 octeti_stml;           /* documentum scriptum */
               b32 successus;             /* octeti et arbor idem */
    memoriae_index usus;                  /* piscina parsurae: dati */
    memoriae_index commissa;              /* usus + otiosa */
    memoriae_index otiosa;
    memoriae_index apex;
    memoriae_index alvei;
    memoriae_index allocationes;
    memoriae_index allocationes_arboris;  /* piscina altera */
            duplex ms_parsandi;
            duplex ms_emittendi;          /* materia_scribere_nodum */
            duplex ms_arbor_scribendi;    /* arbor_scribere_nodum */
            duplex ms_arbor_legendi;      /* materia_arbor_legere */
            duplex ms_comparandi;         /* materia_arbor_aequalis */
} CrustaComputus;

/* VERUM si parsura successit ET octeti idem ET arbor relecta aequalis;
 * numeri semper impleti (usque ad gradum quo perventum est). */
b32
crusta_computus_metiri (
       constans character* fons,
                      i32  mensura,
           CrustaComputus* exitus);

#endif /* CRUSTA_COMPUTUS_H */
