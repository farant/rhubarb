/* saltuarius_tabularium.c - Implementatio exemplaris indicis */

#include "saltuarius_tabularium.h"

/* Litteram ASCII ad minusculam plicare (sine ctype - locale non
 * negotium nostrum) */
interior s32
_minuscula (s32 c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde c + XXXII;
    }
    redde c;
}

/* Ordo: directoria primum, deinde tituli ordine octetorum
 * (brevior prior in aequalitate praefixi). Negativum si a < b. */
interior s32
_comparare (constans DirectoriumIntroitus* a,
    constans DirectoriumIntroitus* b)
{
    b32 a_dir = (a->genus == INTROITUS_DIRECTORIUM);
    b32 b_dir = (b->genus == INTROITUS_DIRECTORIUM);
    i32 minima;
    i32 k;

    si (a_dir != b_dir)
    {
        redde a_dir ? -I : I;
    }
    minima = (a->titulus.mensura < b->titulus.mensura)
        ? a->titulus.mensura : b->titulus.mensura;
    per (k = ZEPHYRUM; k < minima; k++)
    {
        si (a->titulus.datum[k] != b->titulus.datum[k])
        {
            redde (a->titulus.datum[k] < b->titulus.datum[k])
                ? -I : I;
        }
    }
    si (a->titulus.mensura != b->titulus.mensura)
    {
        redde (a->titulus.mensura < b->titulus.mensura) ? -I : I;
    }
    redde ZEPHYRUM;
}

SaltuariusTabularium*
saltuarius_tabularium_creare (Piscina* piscina)
{
    SaltuariusTabularium* tabularium;

    tabularium = (SaltuariusTabularium*)piscina_allocare_ordinatum(
        piscina, (memoriae_index)magnitudo(SaltuariusTabularium),
        IV);
    si (tabularium == NIHIL)
    {
        redde NIHIL;
    }
    tabularium->introitus = NIHIL;
    tabularium->numerus = ZEPHYRUM;
    tabularium->selectio = -I;
    tabularium->volumen = ZEPHYRUM;
    redde tabularium;
}

vacuum
saltuarius_tabularium_implere (SaltuariusTabularium* tabularium,
    Piscina* piscina, constans DirectoriumIntroitus* fontes,
    i32 numerus)
{
    i32 k;
    i32 tenuti = ZEPHYRUM;

    /* NB: tabula vetus in piscina manet (arena est; index parvus,
     * pretium acceptum - impletio rara, non per-quadrum) */
    tabularium->introitus = NIHIL;
    tabularium->numerus = ZEPHYRUM;
    si (numerus > ZEPHYRUM)
    {
        tabularium->introitus = (DirectoriumIntroitus*)
            piscina_allocare_ordinatum(piscina,
                (memoriae_index)numerus
                    * (memoriae_index)magnitudo(DirectoriumIntroitus),
                IV);
    }
    si (tabularium->introitus != NIHIL)
    {
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            /* filtrum punctatorum (narratio Phase A: celata, sine
             * commutatore) */
            si (fontes[k].titulus.mensura > ZEPHYRUM
                && fontes[k].titulus.datum[ZEPHYRUM] == '.')
            {
                perge;
            }
            /* insertio ordinata (N centena - satis) */
            {
                s32 j = (s32)tenuti - I;

                dum (j >= ZEPHYRUM
                    && _comparare(&tabularium->introitus[j],
                        &fontes[k]) > ZEPHYRUM)
                {
                    tabularium->introitus[j + I] =
                        tabularium->introitus[j];
                    j--;
                }
                tabularium->introitus[j + I] = fontes[k];
                tenuti++;
            }
        }
    }
    tabularium->numerus = tenuti;
    tabularium->selectio = (tenuti > ZEPHYRUM) ? ZEPHYRUM : -I;
    tabularium->volumen = ZEPHYRUM;
}

constans DirectoriumIntroitus*
saltuarius_tabularium_selectum (
    constans SaltuariusTabularium* tabularium)
{
    si (tabularium->selectio < ZEPHYRUM
        || tabularium->selectio >= (s32)tabularium->numerus)
    {
        redde NIHIL;
    }
    redde &tabularium->introitus[tabularium->selectio];
}

vacuum
saltuarius_tabularium_movere (SaltuariusTabularium* tabularium,
    s32 delta)
{
    s32 nova;

    si (tabularium->numerus == ZEPHYRUM)
    {
        redde;
    }
    nova = tabularium->selectio + delta;
    si (nova < ZEPHYRUM)
    {
        nova = ZEPHYRUM;
    }
    si (nova >= (s32)tabularium->numerus)
    {
        nova = (s32)tabularium->numerus - I;
    }
    tabularium->selectio = nova;
}

vacuum
saltuarius_tabularium_primum (SaltuariusTabularium* tabularium)
{
    si (tabularium->numerus > ZEPHYRUM)
    {
        tabularium->selectio = ZEPHYRUM;
    }
}

vacuum
saltuarius_tabularium_ultimum (SaltuariusTabularium* tabularium)
{
    si (tabularium->numerus > ZEPHYRUM)
    {
        tabularium->selectio = (s32)tabularium->numerus - I;
    }
}

b32
saltuarius_tabularium_saltare (SaltuariusTabularium* tabularium,
    s32 runa)
{
    s32 quaesita = _minuscula(runa);
    i32 k;

    si (tabularium->numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = I; k <= tabularium->numerus; k++)
    {
        s32 index = (tabularium->selectio + (s32)k)
            % (s32)tabularium->numerus;
        constans DirectoriumIntroitus* intr =
            &tabularium->introitus[index];

        si (intr->titulus.mensura > ZEPHYRUM
            && _minuscula((s32)intr->titulus.datum[ZEPHYRUM])
                == quaesita)
        {
            tabularium->selectio = index;
            redde VERUM;
        }
    }
    redde FALSUM;
}

vacuum
saltuarius_tabularium_aptare (SaltuariusTabularium* tabularium,
    i32 altitudo_fenestrae)
{
    si (tabularium->selectio < ZEPHYRUM
        || altitudo_fenestrae == ZEPHYRUM)
    {
        tabularium->volumen = ZEPHYRUM;
        redde;
    }
    si (tabularium->selectio < tabularium->volumen)
    {
        tabularium->volumen = tabularium->selectio;
    }
    si (tabularium->selectio
        >= tabularium->volumen + (s32)altitudo_fenestrae)
    {
        tabularium->volumen = tabularium->selectio
            - (s32)altitudo_fenestrae + I;
    }
    si (tabularium->volumen < ZEPHYRUM)
    {
        tabularium->volumen = ZEPHYRUM;
    }
}
