/* probatio_cantus.c - Probationes lectoris cantilenae (include/cantus.h)
 *
 * Quadra ad XLVIII milia: nota integra = DCCLXVIII pulsus = 240/t
 * secundae, ergo quadra = rotundatio(pulsus x XV milia / t). Ad t120:
 * quarta XXIV milia, octava XII milia, sedecima VI milia. Numeri
 * exacti asseruntur - lector tempus sine errore cumulato numerat.
 */
#include <stdio.h>
#include "latina.h"
#include "credo.h"
#include "cantus.h"

/* textum legit; VERUM asserit; cantilena nulla si FALSUM */
interior b32
_legere (
    CredoContextus* c,
    constans character* litterae,
         Cantilena* cantilena)
{
    CantusVitium vitium;
             b32 bene;

    bene = cantus_legere(chorda_ex_literis(litterae, c->piscina),
                         c->piscina, cantilena, &vitium);
    CREDO_VERUM (bene);
    si (!bene)
    {
        imprimere("  '%s': %d: %s\n", litterae, (integer)vitium.offset,
                  vitium.causa);
    }
    redde bene;
}

/* textum legit; FALSUM, offset et cantilenam nullam asserit */
interior vacuum
_vitium (
    CredoContextus* c,
    constans character* litterae,
                    i32 offset)
{
       Cantilena cantilena;
    CantusVitium vitium;
             b32 bene;

    vitium.offset  = CMXCIX;   /* nullum verum */
    vitium.causa   = NIHIL;
    bene = cantus_legere(chorda_ex_literis(litterae, c->piscina),
                         c->piscina, &cantilena, &vitium);
    CREDO_FALSUM (bene);
    CREDO_AEQUALIS_S32 ((s32)vitium.offset, (s32)offset);
    CREDO_VERUM (vitium.causa != NIHIL);
    CREDO_AEQUALIS_I32 (cantilena.numerus, ZEPHYRUM);
    CREDO_VERUM (cantilena.notae == NIHIL);
    si ((s32)vitium.offset != (s32)offset)
    {
        imprimere("  '%s': offset %d, causa %s\n", litterae,
                  (integer)vitium.offset,
                  vitium.causa != NIHIL ? vitium.causa : "(nulla)");
    }
}


    /* ==================================================
     * NOTAE: litterae, alterationes, octavae
     * ================================================== */

interior vacuum
_sectio_notae (
    CredoContextus* c)
{
    Cantilena k;

    si (_legere(c, "c d e f g a b", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, VII);
        si (k.numerus == VII)
        {
            CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].nota, LX);   /* C4 */
            CREDO_AEQUALIS_I32 (k.notae[I].nota, LXII);
            CREDO_AEQUALIS_I32 (k.notae[II].nota, LXIV);
            CREDO_AEQUALIS_I32 (k.notae[III].nota, LXV);
            CREDO_AEQUALIS_I32 (k.notae[IV].nota, LXVII);
            CREDO_AEQUALIS_I32 (k.notae[V].nota, LXIX);        /* A4 */
            CREDO_AEQUALIS_I32 (k.notae[VI].nota, LXXI);
        }
    }
    /* '#' et '+' acuunt, '-' deprimit; c- = B3 */
    si (_legere(c, "c# c+ d- c-", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, IV);
        si (k.numerus == IV)
        {
            CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].nota, LXI);
            CREDO_AEQUALIS_I32 (k.notae[I].nota, LXI);
            CREDO_AEQUALIS_I32 (k.notae[II].nota, LXI);
            CREDO_AEQUALIS_I32 (k.notae[III].nota, LIX);
        }
    }
    si (_legere(c, "o0 c o8 b o5 c > c < < c", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, V);
        si (k.numerus == V)
        {
            CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].nota, XII);
            CREDO_AEQUALIS_I32 (k.notae[I].nota, CXIX);
            CREDO_AEQUALIS_I32 (k.notae[II].nota, LXXII);
            CREDO_AEQUALIS_I32 (k.notae[III].nota, LXXXIV);
            CREDO_AEQUALIS_I32 (k.notae[IV].nota, LX);
        }
    }
    /* limites MIDI: '>' ultra o8 licet dum nota intra 0..127 */
    si (_legere(c, "o0 < c o8 > g", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, II);
        si (k.numerus == II)
        {
            CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].nota, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (k.notae[I].nota, CXXVII);
        }
    }
    /* litterae magnae aequae */
    si (_legere(c, "C D# O5 E", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, III);
        si (k.numerus == III)
        {
            CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].nota, LX);
            CREDO_AEQUALIS_I32 (k.notae[I].nota, LXIII);
            CREDO_AEQUALIS_I32 (k.notae[II].nota, LXXVI);
        }
    }
}


    /* ==================================================
     * MENSURAE: longitudines, puncta, 'l', pausae
     * ================================================== */

interior vacuum
_sectio_mensurae (
    CredoContextus* c)
{
    Cantilena k;

    /* ordinarium: quarta ad t120 */
    si (_legere(c, "c", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, I);
        si (k.numerus == I)
        {
            CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].initium,
                (s64)ZEPHYRUM);
            CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].longitudo,
                                (s64)(XXIV * M));
        }
        CREDO_AEQUALIS_S64 (k.longitudo, (s64)(XXIV * M));
    }
    si (_legere(c, "c1 c2 c8 c16 c32 c64", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, VI);
        si (k.numerus == VI)
        {
            CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].longitudo,
                                (s64)(XCVI * M));
            CREDO_AEQUALIS_S64 (k.notae[I].longitudo,
                (s64)(XLVIII * M));
            CREDO_AEQUALIS_S64 (k.notae[II].longitudo, (s64)(XII * M));
            CREDO_AEQUALIS_S64 (k.notae[III].longitudo, (s64)(VI * M));
            CREDO_AEQUALIS_S64 (k.notae[IV].longitudo, (s64)(III * M));
            CREDO_AEQUALIS_S64 (k.notae[V].longitudo, (s64)MD);
            CREDO_AEQUALIS_S64 (k.notae[V].initium, (s64)(CLXV * M));
        }
        CREDO_AEQUALIS_S64 (k.longitudo, (s64)(CLXV * M + MD));
    }
    /* puncta: x1.5, x1.75; etiam in sexagesima quarta (XXI pulsus) */
    si (_legere(c, "c4. c4.. c64..", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, III);
        si (k.numerus == III)
        {
            CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].longitudo,
                                (s64)(XXXVI * M));
            CREDO_AEQUALIS_S64 (k.notae[I].longitudo, (s64)(XLII * M));
            CREDO_AEQUALIS_S64 (k.notae[II].longitudo, (s64)MMDCXXV);
            CREDO_AEQUALIS_S64 (k.notae[II].initium,
                (s64)(LXXVIII * M));
        }
    }
    /* 'l' cum et sine puncto; longitudo propria vincit */
    si (_legere(c, "l8 c c4 c l4. c", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, IV);
        si (k.numerus == IV)
        {
            CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].longitudo,
                                (s64)(XII * M));
            CREDO_AEQUALIS_S64 (k.notae[I].longitudo, (s64)(XXIV * M));
            CREDO_AEQUALIS_S64 (k.notae[II].longitudo, (s64)(XII * M));
            CREDO_AEQUALIS_S64 (k.notae[III].longitudo,
                (s64)(XXXVI * M));
        }
    }
    /* pausae: tempus capiunt, notas non faciunt; ultima longitudinem
     * totius auget */
    si (_legere(c, "c r c r2 r8.", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, II);
        si (k.numerus == II)
        {
            CREDO_AEQUALIS_S64 (k.notae[I].initium, (s64)(XLVIII * M));
        }
        CREDO_AEQUALIS_S64 (k.longitudo, (s64)(CXXXVIII * M));
    }
}


    /* ==================================================
     * TEMPUS: 't', segmenta, nullus error cumulatus
     * ================================================== */

interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
    Cantilena  k;
       chorda  textus;
           i8* memoria;
          i32  j;

    si (_legere(c, "t120 c t60 c", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, II);
        si (k.numerus == II)
        {
            CREDO_AEQUALIS_S64 (k.notae[I].initium, (s64)(XXIV * M));
            CREDO_AEQUALIS_S64 (k.notae[I].longitudo,
                (s64)(XLVIII * M));
        }
        CREDO_AEQUALIS_S64 (k.longitudo, (s64)(LXXII * M));
    }
    /* exemplum capitis: E maior sursum ad t180 (sedecima = IV milia) */
    si (_legere(c, "t180 o6 l16 e g# b >e8", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, IV);
        si (k.numerus == IV)
        {
            CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].nota, LXXXVIII);
            CREDO_AEQUALIS_I32 (k.notae[I].nota, XCII);
            CREDO_AEQUALIS_I32 (k.notae[II].nota, XCV);
            CREDO_AEQUALIS_I32 (k.notae[III].nota, C);
            CREDO_AEQUALIS_S64 (k.notae[I].initium, (s64)(IV * M));
            CREDO_AEQUALIS_S64 (k.notae[III].initium, (s64)(XII * M));
            CREDO_AEQUALIS_S64 (k.notae[III].longitudo,
                (s64)(VIII * M));
        }
        CREDO_AEQUALIS_S64 (k.longitudo, (s64)(XX * M));
    }
    /* segmentum novum incipit ad quadrum rotundatum */
    si (_legere(c, "t133 c16 t120 c", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, II);
        si (k.numerus == II)
        {
            CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].longitudo,
                                (s64)(V * M + CDXIV));
            CREDO_AEQUALIS_S64 (k.notae[I].initium, (s64)(V * M
                + CDXIV));
        }
    }
    /* mille sedecimae ad t133: XLVIII pulsus = 5413.53 quadra. Summa
     * rotundatorum (5414 x n) laberetur; positio exacta non labitur */
    memoria = (i8*)piscina_allocare(c->piscina, (memoriae_index)(M
        + VIII));
    per (j = ZEPHYRUM; j < VIII; j++)
    {
        memoria[j] = (i8)"t133 l16"[j];
    }
    per (j = ZEPHYRUM; j < M; j++)
    {
        memoria[VIII + j] = (i8)'c';
    }
    textus = chorda_ex_buffer(memoria, M + VIII);
    {
        CantusVitium vitium;
                 b32 bene = cantus_legere(textus, c->piscina, &k,
                                          &vitium);

        CREDO_VERUM (bene);
        si (bene && k.numerus == M)
        {
            CREDO_AEQUALIS_S64 (k.notae[I].initium, (s64)(V * M
                + CDXIV));
            CREDO_AEQUALIS_S64 (k.notae[II].initium,
                                (s64)(X * M + DCCCXXVII));
            CREDO_AEQUALIS_S64 (k.notae[CMXCIX].initium,
                                (s64)V * M * M + (s64)(CDVIII * M
                                    + CXX));
        }
        CREDO_AEQUALIS_I32 (k.numerus, M);
        CREDO_AEQUALIS_S64 (k.longitudo,
                            (s64)V * M * M + (s64)(CDXIII * M
                                + DXXXIV));
    }
}


    /* ==================================================
     * MANDATA: 'v', '@', offset, vacua
     * ================================================== */

interior vacuum
_sectio_mandata (
    CredoContextus* c)
{
    Cantilena k;

    si (_legere(c, "c v50 c v0 c", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, III);
        si (k.numerus == III)
        {
            CREDO_F32_PROXIMUS (k.notae[ZEPHYRUM].lucrum, 1.0f, 0.0f);
            CREDO_F32_PROXIMUS (k.notae[I].lucrum, 0.5f, 0.0f);
            CREDO_F32_PROXIMUS (k.notae[II].lucrum, 0.0f, 0.0f);
        }
    }
    si (_legere(c, "c @campana d e @radius_2 f", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, IV);
        si (k.numerus == IV)
        {
            CREDO_CHORDA_VACUA (k.notae[ZEPHYRUM].praeparatum);
            CREDO_CHORDA_AEQUALIS_LITERIS (k.notae[I].praeparatum,
                                           "campana");
            CREDO_CHORDA_AEQUALIS_LITERIS (k.notae[II].praeparatum,
                                           "campana");
            CREDO_CHORDA_AEQUALIS_LITERIS (k.notae[III].praeparatum,
                                           "radius_2");
        }
    }
    si (_legere(c, " c  d#8 |e", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, III);
        si (k.numerus == III)
        {
            CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].offset, I);
            CREDO_AEQUALIS_I32 (k.notae[I].offset, IV);
            CREDO_AEQUALIS_I32 (k.notae[II].offset, IX);
        }
    }
    si (_legere(c, "", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, ZEPHYRUM);
        CREDO_AEQUALIS_S64 (k.longitudo, (s64)ZEPHYRUM);
    }
    si (_legere(c, "  | ", &k))
    {
        CREDO_AEQUALIS_I32 (k.numerus, ZEPHYRUM);
        CREDO_AEQUALIS_S64 (k.longitudo, (s64)ZEPHYRUM);
    }
}


    /* ==================================================
     * VITIA: offset ubi lector desiit, nihil semi-lectum
     * ================================================== */

interior vacuum
_sectio_vitia (
    CredoContextus* c)
{
    _vitium(c, "x", ZEPHYRUM);
    _vitium(c, "c d x", IV);
    _vitium(c, "#c", ZEPHYRUM);
    _vitium(c, "c3", I);
    _vitium(c, "c0", I);
    _vitium(c, "c128", I);
    _vitium(c, "r3", I);
    _vitium(c, "c4...", IV);
    _vitium(c, "o9", I);
    _vitium(c, "oc", I);
    _vitium(c, "l", I);
    _vitium(c, "t10", I);
    _vitium(c, "t401", I);
    _vitium(c, "t99999999999", I);
    _vitium(c, "v101", I);
    _vitium(c, "@ c", I);
    _vitium(c, "o8 > g#", V);
    _vitium(c, "o0 < c-", V);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "notae", _sectio_notae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensurae", _sectio_mensurae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mandata", _sectio_mandata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cantus", SECTIONES);
}
