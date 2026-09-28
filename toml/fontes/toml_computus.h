/* toml_computus.h - imago memoriae et temporis clientis toml
 * (gemellus crusta_computus.h; planum toml-arbor Q11b)
 *
 * Una plagula, una mensura: parsura (toml_arbor) in piscina PROPRIA cum
 * alveo initiali fixo (numeri inter plagulas et commissiones
 * conferibiles); deinde in piscina altera emissio octetorum, scriptura
 * et lectio STML, comparator arborum, et COCTIO (toml_coquere: tabulae,
 * regulae, diagnostica) - numeri parsurae puri manent. Tempus per
 * clock() (CPU), ms.
 *
 * Instrumentum: ./toml/computus.sh <plagula.toml> [-machina] [-iter N].
 * Porta: probatio_toml_computus (tria fixa toml-test, aurum
 * toml/probationes/fixa/computus/basis.tsv - numeri deterministici
 * pinnati, tempora non; COMPUTUS_SCRIBERE=1 + causa nominata).
 * pythonica silva.metiri('x.toml') per titulos columnarum legit.
 */
#ifndef TOML_COMPUTUS_H
#define TOML_COMPUTUS_H

#include "latina.h"
#include "piscina.h"

#define TOML_COMPUTUS_ALVEUS_INITIUM \
    ((memoriae_index)(IV * MXXIV * CCLVI))

nomen structura {
               i32 octeti_fontis;
               i32 nodi;                  /* radix inclusa */
               i32 lexemata;              /* loci + listae */
               i32 octeti_stml;           /* documentum scriptum */
               i32 diagnostica;           /* post coctionem */
               b32 successus;             /* octeti et arbor idem */
    memoriae_index usus;                  /* piscina parsurae: dati */
    memoriae_index commissa;              /* usus + otiosa */
    memoriae_index otiosa;
    memoriae_index apex;
    memoriae_index alvei;
    memoriae_index allocationes;
    memoriae_index allocationes_arboris;  /* piscina altera */
               f64 ms_parsandi;
               f64 ms_emittendi;          /* materia_scribere_nodum */
               f64 ms_arbor_scribendi;    /* arbor_scribere_nodum */
               f64 ms_arbor_legendi;      /* materia_arbor_legere */
               f64 ms_comparandi;         /* materia_arbor_aequalis */
               f64 ms_coquendi;           /* toml_coquere */
} TomlComputus;

/* VERUM si parsura successit ET octeti idem ET arbor relecta aequalis
 * (plagula invalida quoque: lex octetorum omnem fontem tenet); numeri
 * semper impleti (usque ad gradum quo perventum est). */
b32
toml_computus_metiri (
       constans character* fons,
                      i32  mensura,
             TomlComputus* exitus);

#endif /* TOML_COMPUTUS_H */
