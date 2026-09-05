/* pictor_commune.h - exemplar / candidatus / differentia in TEXTU
 *
 * Exemplar absens: candidatus ut <via>.novum scribitur, probatio
 * CADIT cum nuntio 'promove' - promotio est mv manu (disciplina
 * specimen.h / stml_aurea). Discrepantia: .novum scriptus, probatio
 * cadit, 'diff -u' ea confert. Exemplar numquam a probatione
 * scribitur.
 */
#ifndef PICTOR_COMMUNE_H
#define PICTOR_COMMUNE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "filum.h"
#include <stdio.h>

interior b32
exemplar_conferre (
               Piscina* piscina,
    constans character* via_exemplar,
                chorda  candidatus)
{
    chorda exemplar;
    chorda via_novum;

    via_novum = chorda_concatenare(
        chorda_ex_literis(via_exemplar, piscina),
        chorda_ex_literis(".novum", piscina), piscina);
    exemplar = filum_legere_totum(via_exemplar, piscina);
    si (exemplar.mensura == ZEPHYRUM)
    {
        filum_scribere(chorda_ut_cstr(via_novum, piscina), candidatus);
        imprimere("EXEMPLAR DEEST: %s - candidatus scriptus ad %s;"
                  " inspice et promove (mv)\n",
                  via_exemplar, chorda_ut_cstr(via_novum, piscina));
        redde FALSUM;
    }
    si (chorda_aequalis(exemplar, candidatus))
    {
        redde VERUM;
    }
    filum_scribere(chorda_ut_cstr(via_novum, piscina), candidatus);
    imprimere("DIFFERENTIA: %s vs %s (diff -u ea)\n", via_exemplar,
              chorda_ut_cstr(via_novum, piscina));
    redde FALSUM;
}

#endif /* PICTOR_COMMUNE_H */
