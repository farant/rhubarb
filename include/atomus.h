/* atomus.h - ordo memoriae inter fila duo: publicare et legere
 *
 * DUAE operationes solae quas anulus SPSC (circulus.h) eget, et quas
 * C89 exprimere non potest (exemplar memoriae nullum; volatile ordinem
 * non servat - Apple Silicon index ante locum visibilem facere potest).
 * Implementatio lib/atomus.c per postulata_atomica.h (clang/gcc
 * __atomic; arm64 stlr / ldapr). Ceterae plagulae domus C89 purae
 * manent: functiones vocant, numquam __atomic ipsum.
 *
 * Typus i32 SOLUS (indices anuli). Sumptus: vocatio una per scriptum
 * aut lectum anuli - non per exemplum soni.
 *
 * USUS (scriptor):  locum implere; atomus_publicare(&caput, caput + I);
 *      (lector):    si (atomus_legere(&caput) != cauda) locum legere;
 */
/* <aedilis corpus="lib/atomus.c"/> */
#ifndef ATOMUS_H
#define ATOMUS_H

#include "latina.h"

/* valorem scribit (store-release): omnia quae hoc filum ANTE scripsit
 * filo quod hunc valorem legit visibilia sunt */
vacuum
atomus_publicare (
    volatilis i32* locus,
              i32  valor);

/* valorem legit (load-acquire): quae scriptor ante publicationem huius
 * valoris scripsit, post hanc lectionem visibilia sunt */
i32
atomus_legere (
    constans volatilis i32* locus);

#endif /* ATOMUS_H */
