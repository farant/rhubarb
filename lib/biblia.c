/* biblia.c - accessores Bibliae (vide biblia.h); tabulae ipsae in
 * biblia_dr.c (DATA). Olim in fine biblia_dr.c: accessores a
 * tools/genera_biblia emissi, deinde manu correcti (pons qualificatoris
 * gcc), et quaesitio libri manu addita - separati 2026-10-21 ut
 * biblia_dr.c tabula datorum pura sit. */

#include "biblia.h"

chorda
biblia_versus (
    constans Biblia* biblia,
                i32  liber,
                i32  capitulum,
                i32  versus_num)
{
                chorda  vacua;
        constans Liber* lib;
    constans Capitulum* cap;
                   i32  versus_idx;
                   i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = 0;

    si (!biblia || !biblia->capitula)
    {
        redde vacua;
    }

    si (liber >= biblia->libri_numerus)
    {
        redde vacua;
    }

    lib = &biblia->libri[liber];

    si (capitulum < 1 || capitulum > lib->capitula_numerus)
    {
        redde vacua;
    }

    cap = &biblia->capitula[lib->capitula_initium + capitulum - 1];

    /* Quaerere solum in hoc capitulo */
    per (i = 0; i < cap->versus_numerus; i++)
    {
        versus_idx = cap->versus_initium + i;
        si (biblia->versus[versus_idx].versus == versus_num)
        {
            /* pons qualificatoris: textus constans, chorda.datum
             * mutabilis - unio castum qualificatorem vitat (gcc
             * -Wcast-qual reicit quod clang tacet) */
            chorda result;
            unio { constans character* c; i8* m; } pons;
            pons.c = biblia->textus
                + biblia->versus[versus_idx].textus_initium;
            result.datum = pons.m;
            result.mensura =
                biblia->versus[versus_idx].textus_longitudo;
            redde result;
        }
    }

    redde vacua;
}

constans character*
biblia_nomen_libri (
    constans Biblia* biblia,
                i32  liber)
{
    si (!biblia || liber >= biblia->libri_numerus)
    {
        redde NIHIL;
    }
    redde biblia->libri[liber].nomen_anglicum;
}

i32
biblia_capitula_in_libro (
    constans Biblia* biblia,
                i32  liber)
{
    si (!biblia || liber >= biblia->libri_numerus)
    {
        redde 0;
    }
    redde biblia->libri[liber].capitula_numerus;
}

i32
biblia_versus_in_capitulo (
    constans Biblia* biblia,
                i32  liber,
                i32  capitulum)
{
        constans Liber* lib;
    constans Capitulum* cap;

    si (!biblia || !biblia->capitula)
    {
        redde 0;
    }

    si (liber >= biblia->libri_numerus)
    {
        redde 0;
    }

    lib = &biblia->libri[liber];

    si (capitulum < 1 || capitulum > lib->capitula_numerus)
    {
        redde 0;
    }

    cap = &biblia->capitula[lib->capitula_initium + capitulum - 1];
    redde cap->versus_numerus;
}

/* Auxilium: convertere ad litteram minusculam */
hic_manens character
_character_minusculus (
    character c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde (character)(c + ('a' - 'A'));
    }
    redde c;
}

/* Auxilium: comparare sine casu litterarum (prefix match) */
hic_manens b32
_praefixum_aequale (
    constans character* textus,
    constans character* praefixum)
{
    dum (*praefixum)
    {
        si (!*textus)
        {
            redde FALSUM;
        }
        si (_character_minusculus(*textus)
            != _character_minusculus(*praefixum))
        {
            redde FALSUM;
        }
        textus++;
        praefixum++;
    }
    redde VERUM;
}

s32
biblia_invenire_librum (
       constans Biblia* biblia,
    constans character* quaesitum)
{
                   i32  i;
    constans character* p;
             character  numerus_praefixum[IV];
                   i32  praefixum_longitudo;
                   b32  habet_numerum;

    si (!biblia || !quaesitum || !*quaesitum)
    {
        redde -I;
    }

    /* Videre si quaesitum incipit cum numero (e.g., "1 Maccabees") */
    habet_numerum        = FALSUM;
    praefixum_longitudo  = ZEPHYRUM;
    p                    = quaesitum;

    si (*p >= '1' && *p <= '9')
    {
        habet_numerum                             = VERUM;
        numerus_praefixum[praefixum_longitudo++]  = *p++;

        /* Transire spatium post numerum si adest */
        si (*p == ' ')
        {
            p++;
        }
    }

    /* Quaerere in omnibus libris */
    per (i = ZEPHYRUM; i < biblia->libri_numerus; i++)
    {
        constans character* nomen_anglicum;

        nomen_anglicum = biblia->libri[i].nomen_anglicum;
        si (!nomen_anglicum)
        {
            perge;
        }

        si (habet_numerum)
        {
            /* Pro libris numeratis, verificare praefixum numericum */
            si (nomen_anglicum[ZEPHYRUM] != numerus_praefixum[ZEPHYRUM])
            {
                perge;
            }

            /* Transire spatium in nomine libri si adest */
            si (nomen_anglicum[I] == ' ')
            {
                si (_praefixum_aequale(nomen_anglicum + II, p))
                {
                    redde (s32)i;
                }
            }
        }
        alioquin
        {
            /* Comparatio simplex sine numero */
            si (_praefixum_aequale(nomen_anglicum, quaesitum))
            {
                redde (s32)i;
            }
        }
    }

    redde -I;
}
