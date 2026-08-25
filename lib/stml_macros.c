/* stml_macros.c - Expansio fragmentorum parametrizatorum (v1)
 *
 * Ambulatio una sinistra-dextra super documentum: definitiones
 * (<#id>) colliguntur ordine documenti et ex emissione DEMITTUNTUR
 * (arbor expansa = visio CONTENTI); vocationes (<<#id args>>)
 * corporibus clonatis impletisque substituuntur; cetera verbatim
 * clonantur. Clonatio: nodi recentes in piscina vocantis
 * (originalis immutabilis - lectio simulationis mutationum I);
 * chordae internatae per punctatorem communicantur (immutabiles).
 *
 * Spec: project-specs/stml-macros-spec.md. Planum:
 * stml-macros-plan.md (T2: sceletum - documentum sine macris
 * clonatur, tabula vacua).
 */

#include "stml_macros.h"

/* Ambulatio expansionis: superficialiter duplicare, liberos
 * recursive. Ambulatio iterationem liberorum POSSIDET (numquam
 * stml_duplicare profundum pro parentibus) quia opera posteriora
 * fragmenta et vocationes in ea intercipiunt. Cave:
 * stml_duplicare_superficialiter liberos NIHIL relinquit - Xar
 * liberorum hic creatur. */
interior StmlNodus*
_expandere_nodum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
     StmlNodus* novum;
           i32  i;
           i32  num;

    novum = stml_duplicare_superficialiter(nodus, piscina, intern);
    si (novum == NIHIL)
    {
        redde NIHIL;
    }
    si (nodus->liberi != NIHIL)
    {
        novum->liberi = xar_creare(piscina, magnitudo(StmlNodus*));
        si (novum->liberi == NIHIL)
        {
            redde NIHIL;
        }
        num = xar_numerus(nodus->liberi);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* liberum;
            StmlNodus* liberum_novum;

            liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
            si (liberum == NIHIL)
            {
                perge;
            }
            liberum_novum = _expandere_nodum(liberum, piscina,
                                             intern);
            si (liberum_novum == NIHIL)
            {
                redde NIHIL;
            }
            (vacuum)stml_liberum_addere(novum, liberum_novum);
        }
    }
    redde novum;
}

StmlExpansioResultus
stml_expandere (
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlExpansioResultus resultus;

    resultus.successus           = FALSUM;
    resultus.radix_expansa       = NIHIL;
    resultus.tabula_expansionum  = NIHIL;
    resultus.vitium              = STML_EXPANSIO_BENE;
    resultus.linea               = ZEPHYRUM;
    resultus.fragmentum.datum    = NIHIL;
    resultus.fragmentum.mensura  = ZEPHYRUM;
    resultus.loculus.datum       = NIHIL;
    resultus.loculus.mensura     = ZEPHYRUM;

    si (radix == NIHIL || piscina == NIHIL || intern == NIHIL)
    {
        redde resultus;
    }
    resultus.tabula_expansionum =
        xar_creare(piscina, magnitudo(StmlExpansioNota));
    si (resultus.tabula_expansionum == NIHIL)
    {
        redde resultus;
    }
    resultus.radix_expansa = _expandere_nodum(radix, piscina,
                                              intern);
    si (resultus.radix_expansa == NIHIL)
    {
        redde resultus;
    }
    resultus.successus = VERUM;
    redde resultus;
}
