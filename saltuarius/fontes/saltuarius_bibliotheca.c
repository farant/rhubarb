/* saltuarius_bibliotheca.c - Implementatio cache librorum */

#include "saltuarius_bibliotheca.h"

vacuum
saltuarius_bibliotheca_parare (
    SaltuariusBibliotheca* bibliotheca)
{
    i32 k;

    per (k = ZEPHYRUM; k < SALT_LIBRI_MAXIMI; k++)
    {
        bibliotheca->libri[k] = NIHIL;
    }
    bibliotheca->numerus = ZEPHYRUM;
}

interior vacuum
_ad_frontem (
    SaltuariusBibliotheca* bibliotheca,
                      i32  index)
{
    SaltuariusLiber* liber = bibliotheca->libri[index];
                i32  k;

    per (k = index; k > ZEPHYRUM; k--)
    {
        bibliotheca->libri[k] = bibliotheca->libri[k - I];
    }
    bibliotheca->libri[ZEPHYRUM] = liber;
}

SaltuariusLiber*
saltuarius_bibliotheca_quaerere (
    SaltuariusBibliotheca* bibliotheca,
                   chorda  via)
{
    i32 k;

    per (k = ZEPHYRUM; k < bibliotheca->numerus; k++)
    {
        si (chorda_aequalis(bibliotheca->libri[k]->via, via))
        {
            _ad_frontem(bibliotheca, k);
            redde bibliotheca->libri[ZEPHYRUM];
        }
    }
    redde NIHIL;
}

vacuum
saltuarius_bibliotheca_condere (
    SaltuariusBibliotheca* bibliotheca,
          SaltuariusLiber* liber)
{
    si (bibliotheca->numerus >= SALT_LIBRI_MAXIMI)
    {
        saltuarius_liber_destruere(
            bibliotheca->libri[SALT_LIBRI_MAXIMI - I]);
        bibliotheca->numerus = SALT_LIBRI_MAXIMI - I;
    }
    {
        i32 k;

        per (k = bibliotheca->numerus; k > ZEPHYRUM; k--)
        {
            bibliotheca->libri[k] = bibliotheca->libri[k - I];
        }
    }
    bibliotheca->libri[ZEPHYRUM] = liber;
    bibliotheca->numerus++;
}

vacuum
saltuarius_bibliotheca_vacare (
    SaltuariusBibliotheca* bibliotheca)
{
    i32 k;

    per (k = ZEPHYRUM; k < bibliotheca->numerus; k++)
    {
        saltuarius_liber_destruere(bibliotheca->libri[k]);
        bibliotheca->libri[k] = NIHIL;
    }
    bibliotheca->numerus = ZEPHYRUM;
}
