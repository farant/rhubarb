/* utf8: verificare credo_drafts recensoris - debent hodie FALLERE */
#include "latina.h"
#include "utf8.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
_sectio_proxima_invalida (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* C3 'A': C3 sine continuatione est unus byte invalidus; 'A' est limes */
        constans i8 datum[] = { 0xC3, 'A' };

        CREDO_AEQUALIS_PTR(utf8_proxima_runa(datum, datum + II), datum + I);
    }

    {
        /* E2 '(' A1: proxima non devorat '(' */
        constans i8 datum[] = { 0xE2, 0x28, 0xA1 };

        CREDO_AEQUALIS_PTR(utf8_proxima_runa(datum, datum + III), datum + I);
    }

    {
        /* F0 9F 'A' 'A': limes post F0 9F? decodere progreditur I */
        constans i8 datum[] = { 0xF0, 0x9F, 'A', 'A' };

        CREDO_AEQUALIS_PTR(utf8_proxima_runa(datum, datum + IV), datum + I);
    }

    {
        /* gradus proximae == utf8_numerare_runas in textu fracto */
        constans i8  datum[] = { 0xE2, 0x82, 'A', 0xC3, 'B', 0xF0, 0x9F, 0x98 };
        constans i8* p       = datum;
        constans i8* finis   = datum + VIII;
                 s32 gradus  = ZEPHYRUM;

        dum (p < finis && gradus < XX)
        {
            p = utf8_proxima_runa(p, finis);
            gradus++;
        }
        CREDO_AEQUALIS_S32(gradus, utf8_numerare_runas(datum, VIII));
    }
}

interior vacuum
_sectio_prior_invalida (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* signum euronis + duae continuationes orphanae: E2 82 AC 80 80 */
        constans i8  datum[] = { 0xE2, 0x82, 0xAC, 0x80, 0x80 };
        constans i8* p;

        p = utf8_prior_runa(datum + V, datum);
        CREDO_AEQUALIS_PTR(p, datum + IV);
        CREDO_FALSUM(p == datum + I);   /* numquam intra signum euronis */
    }

    {
        /* 'A' 80: continuatio orphana est runa sua; 'A' non salitur */
        constans i8 datum[] = { 'A', 0x80 };

        CREDO_AEQUALIS_PTR(utf8_prior_runa(datum + II, datum), datum + I);
    }

    {
        /* C3 A9 80: e-acutum validum non salitur */
        constans i8 datum[] = { 0xC3, 0xA9, 0x80 };

        CREDO_AEQUALIS_PTR(utf8_prior_runa(datum + III, datum), datum + II);
    }

    {
        /* 80 x V: quisque byte est limes */
        constans i8 datum[] = { 0x80, 0x80, 0x80, 0x80, 0x80 };

        CREDO_AEQUALIS_PTR(utf8_prior_runa(datum + V, datum), datum + IV);
    }
}

interior vacuum
_sectio_numerare_invalida (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* overlong C0 80: duo bytes invalidi -> II */
        constans i8 datum[] = { 0xC0, 0x80 };

        CREDO_AEQUALIS_S32(utf8_numerare_runas(datum, II), II);
    }

    {
        /* overlong E0 80 80 -> III */
        constans i8 datum[] = { 0xE0, 0x80, 0x80 };

        CREDO_AEQUALIS_S32(utf8_numerare_runas(datum, III), III);
    }

    {
        /* surrogatum ED A0 80 -> III */
        constans i8 datum[] = { 0xED, 0xA0, 0x80 };

        CREDO_AEQUALIS_S32(utf8_numerare_runas(datum, III), III);
    }

    {
        /* supra U+10FFFF: F4 90 80 80 -> IV */
        constans i8 datum[] = { 0xF4, 0x90, 0x80, 0x80 };

        CREDO_AEQUALIS_S32(utf8_numerare_runas(datum, IV), IV);
    }

    {
        /* aequalitas: overlong et truncatum eodem modo numerantur */
        constans i8 overlong[]  = { 0xE0, 0x80, 0x80 };
        constans i8 truncatum[] = { 0xE2, 0x82, 0x41 };

        CREDO_AEQUALIS_S32(utf8_numerare_runas(overlong, III),
                           utf8_numerare_runas(truncatum, III));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "utf8_proxima_runa invalida", _sectio_proxima_invalida, NIHIL, NIHIL, NIHIL, NIHIL },
    { "utf8_prior_runa invalida", _sectio_prior_invalida, NIHIL, NIHIL, NIHIL, NIHIL },
    { "utf8_numerare_runas invalida", _sectio_numerare_invalida, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("utf8_drafts", SECTIONES);
}
