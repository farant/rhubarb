/* materia_coctor.h - Registrum coctum ex declaratione STML coquere
 *
 * Declaratio <registrum> (canon materia/grammatica/registrum.canon)
 * FONS UNUS tabularum coctarum clientis unius: genera ordine appenso,
 * loci cuiusque generis ordine et specie. Hinc textus DUO redduntur:
 * <grammatica>_registrum_coctum.h (enumeratio generum <typus> cum
 * comite <PRAEFIXUM>_GENUS_NUMERUS_GENERUM, externus
 * <PRAEFIXUM>_REGISTRUM) et .c (series LOCORUM plana, genera cum
 * offset et numero, MateriaRegistrumCoctum). Textus octetim
 * determinati, forma domus (lineae <= LXXII), ergo porta rancoris
 * plagulas octetim confert - numquam tempora.
 *
 * Quod NON generatur, consulto:
 *   - enumerationes LOCORUM (forma earum clientis est - analyses
 *     orationis enumerationem communem partiuntur - non substrati;
 *     probatio clientis titulos earum contra tabulam retirat ut
 *     antea);
 *   - pinna numeri locorum in probatione clientis et sigillum
 *     registri in canone eius: portae sunt, manu motae cum causa.
 *
 * Bibliotheca sine plagulis: textus intrat, textus exit, ut
 * probationes clientium eam in processu vocent. Instrumentum
 * materia/instrumenta/coctor.c (materia/coquere.sh) plagulas legit,
 * declarationem contra canonem iudicat, scribit aut confert.
 */
#ifndef MATERIA_COCTOR_H
#define MATERIA_COCTOR_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

nomen structura {
    chorda caput;         /* textus .h generati */
    chorda fons;          /* textus .c generati */
    chorda grammatica;    /* attributa declarationis */
    chorda praefixum;
    chorda typus;
    chorda sedes;         /* directorium generatorum (relativum) */
    chorda via_capitis;   /* sedes/grammatica_registrum_coctum.h */
    chorda via_fontis;    /* sedes/grammatica_registrum_coctum.c */
       i32 numerus_generum;
       i32 numerus_locorum;
       i32 numerus_diagnosticorum;  /* genus + absentia + vacua */
       i32 numerus_inanium;         /* genera 'inanis' */
    chorda causa;         /* recusationis (FALSUM); vacua aliter */
       i32 linea;         /* declarationis, cum causa nodum nominat */
} MateriaCoctio;

/* Declarationem legere, retirare, textus reddere. via_declarationis
 * in commentarium generatum VERBATIM transit - radici relativa detur,
 * eadem ab instrumento et a probatione, aliter rancor fictus.
 * FALSUM = recusatio (causa, linea) aut memoria (causa vacua). */
b32
materia_registrum_coquere (
                Piscina* piscina,
                 chorda  declaratio,
     constans character* via_declarationis,
          MateriaCoctio* coctio);

/* Titulus speciei loci (kebab ex MateriaLocusSpecies, ordine eius);
 * NIHIL extra. */
constans character*
materia_locus_species_titulus (
    s32 species);

/* Species ex titulo; -I si ignotus. */
s32
materia_locus_species_ex_titulo (
    chorda titulus);

/* Aequalitas octetim; FALSUM cum linea prima divergente (I-basata;
 * textus brevior in linea post octetum suum ultimum divergit). */
b32
materia_coctio_aequalis (
    chorda  generatum,
    chorda  in_disco,
       i32* linea_divergens);

/* PORTA RANCORIS, una pro clientibus omnibus: declarationem legere,
 * coquere, plagulas generatas commissas legere, octetim conferre. */
nomen structura {
              b32 recens;    /* plagulae ambae octetim aequales */
           chorda via;       /* divergens aut absens (relativa) */
              i32 linea;     /* prima divergens; ZEPHYRUM = absens */
           chorda causa;     /* recusatio declarationis */
    MateriaCoctio coctio;    /* redditio (viae, numeri) */
} MateriaRancor;

/* radix = directorium repositorii (viae ei relativae: declaratio,
 * sedes). VERUM = comparatio facta (rancor->recens dicit); FALSUM =
 * declaratio absens, recusata (causa) aut memoria. */
b32
materia_registrum_recens (
               Piscina* piscina,
    constans character* radix,
    constans character* via_declarationis,
         MateriaRancor* rancor);

#endif /* MATERIA_COCTOR_H */
