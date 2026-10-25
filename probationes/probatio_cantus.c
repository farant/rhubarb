/* probatio_cantus.c - Probationes lectoris cantilenae (include/cantus.h)
 *
 * Quadra ad XLVIII milia: nota integra = DCCLXVIII pulsus = 240/t
 * secundae, ergo quadra = rotundatio(pulsus x XV milia / t). Ad t120:
 * quarta XXIV milia, octava XII milia, sedecima VI milia. Numeri
 * exacti asseruntur - lector tempus sine errore cumulato numerat.
 */
#include <stdio.h>
#include <string.h>
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


    /* ==================================================
     * IUSSA - cantilena -> iussa mixturae
     * ================================================== */

#define MEDIUM 0.70710677f
#define QUADRA_REDDENDA (XXXII * M)

hic_manens f32 exitus[II * QUADRA_REDDENDA];
hic_manens f32 exspectata[QUADRA_REDDENDA];
hic_manens f32 tabula[QUADRA_REDDENDA];

interior vacuum
_parare_tabulas (
    CredoContextus* c)
{
    i32 q;

    (vacuum)c;
    per (q = ZEPHYRUM; q < QUADRA_REDDENDA; q++)
    {
        exitus[II * q]      = 0.0f;
        exitus[II * q + I]  = 0.0f;
        exspectata[q]       = 0.0f;
        tabula[q]           = 0.0f;
    }
}

/* pulsus brevis sine varietate: 0.01 + 0.05 + 0.04 s = IV milia DCCC
 * quadra, ergo octavae ad t120 (XII milia) non se tegunt */
interior SonitusParametra
_colorem_parare (
    vacuum)
{
    SonitusParametra p = *sonitus_praeparatum("punctum");

    p.impetus        = 0.01f;
    p.mora           = 0.05f;
    p.ictus          = 0.0f;
    p.remissio       = 0.04f;
    p.saltus         = 1.0f;
    p.saltus_tempus  = 0.0f;
    p.varietas       = 0.0f;
    redde p;
}

interior vacuum
_formam_parare (
                  CantusForma* forma,
    constans SonitusParametra* color)
{
    forma->color      = color;
    forma->resolutor  = NIHIL;
    forma->contextus  = NIHIL;
    forma->tenere     = FALSUM;
    forma->grex       = MIXTURA_GREX_INTERFACIES;
    forma->lucrum     = 1.0f;
    forma->prioritas  = I;
    forma->semen      = XL;
    forma->momentum   = ZEPHYRUM;
}

/* resolutor probationis: 'meum' -> color contextus */
interior constans SonitusParametra*
_resolutor_meus (
    chorda  titulus,
    vacuum* contextus)
{
    si (   titulus.mensura == IV && titulus.datum[ZEPHYRUM] == (i8)'m'
        && titulus.datum[III] == (i8)'m')
    {
        redde (constans SonitusParametra*)contextus;
    }
    redde NIHIL;
}

interior vacuum
_sectio_iussa (
    CredoContextus* c)
{
         Cantilena  k;
       CantusForma  forma;
      CantusVitium  vitium;
     MixturaIussum  iussa[II];
  SonitusParametra  color  = _colorem_parare();
     MixturaRector* r      = mixtura_rectorem_creare(c->piscina, IV);

    si (!_legere(c, "v50 c d8", &k) || r == NIHIL)
    {
        redde;
    }
    _formam_parare(&forma, &color);
    forma.lucrum     = 0.5f;
    forma.prioritas  = II;
    forma.momentum   = (s64)M;
    CREDO_VERUM (cantus_iussa_parare(&k, &forma, r, iussa, &vitium));
    CREDO_AEQUALIS_S32 ((s32)iussa[ZEPHYRUM].genus,
                        (s32)MIXTURA_IUSSUM_SONITUM);
    CREDO_AEQUALIS_S64 (iussa[ZEPHYRUM].momentum, (s64)M);
    CREDO_AEQUALIS_S64 (iussa[I].momentum, (s64)(XXV * M));
    CREDO_F32_PROXIMUS (iussa[ZEPHYRUM].lucrum, 0.25f, 0.0f);
    CREDO_F32_PROXIMUS (iussa[I].lucrum, 0.25f, 0.0f);
    CREDO_F32_PROXIMUS (iussa[ZEPHYRUM].positio, 0.0f, 0.0f);
    CREDO_AEQUALIS_S32 ((s32)iussa[I].grex,
        (s32)MIXTURA_GREX_INTERFACIES);
    CREDO_AEQUALIS_I64 (iussa[ZEPHYRUM].semen, (i64)XL);
    CREDO_AEQUALIS_I64 (iussa[I].semen, (i64)XLI);
    CREDO_F32_PROXIMUS (iussa[ZEPHYRUM].parametra.frequentia,
                        sonus_notae_frequentia(60.0f), 0.0f);
    CREDO_F32_PROXIMUS (iussa[I].parametra.frequentia,
                        sonus_notae_frequentia(62.0f), 0.0f);
    /* cetera coloris intacta; tenere FALSUM: mora quoque */
    CREDO_F32_PROXIMUS (iussa[I].parametra.mora, 0.05f, 0.0f);
    CREDO_F32_PROXIMUS (iussa[I].parametra.remissio, 0.04f, 0.0f);
    CREDO_F32_PROXIMUS (iussa[I].parametra.officium, color.officium,
                        0.0f);
    CREDO_FALSUM (mixtura_ansa_nulla(iussa[ZEPHYRUM].ansa));
    CREDO_FALSUM (mixtura_ansa_nulla(iussa[I].ansa));
    CREDO_VERUM (iussa[ZEPHYRUM].ansa.sedes != iussa[I].ansa.sedes);

    /* tenere VERUM: mora = longitudo - impetus (0.5 - 0.01, 0.25 - 0.01) */
    forma.tenere = VERUM;
    CREDO_VERUM (cantus_iussa_parare(&k, &forma, r, iussa, &vitium));
    CREDO_F32_PROXIMUS (iussa[ZEPHYRUM].parametra.mora, 0.49f, 1e-6f);
    CREDO_F32_PROXIMUS (iussa[I].parametra.mora, 0.24f, 1e-6f);
    CREDO_F32_PROXIMUS (iussa[I].parametra.impetus, 0.01f, 0.0f);
    /* nota brevior impetu: mora 0, non negativa */
    color.impetus = 0.05f;
    si (_legere(c, "c64", &k))
    {
        CREDO_VERUM (cantus_iussa_parare(&k, &forma, r, iussa,
            &vitium));
        CREDO_F32_PROXIMUS (iussa[ZEPHYRUM].parametra.mora, 0.0f, 0.0f);
    }
}

interior vacuum
_sectio_praeparata (
    CredoContextus* c)
{
            Cantilena k;
          CantusForma forma;
         CantusVitium vitium;
        MixturaIussum iussa[III];
     SonitusParametra color  = _colorem_parare();
     SonitusParametra meum   = _colorem_parare();
         MixturaRector* r    = mixtura_rectorem_creare(c->piscina, II);
     constans SonitusParametra* punctum =
         sonitus_praeparatum("punctum");
           MixturaAnsa a;
           MixturaAnsa b;

    si (r == NIHIL || punctum == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    _formam_parare(&forma, &color);
    /* resolutor ordinarius = sonitus_praeparatum per chordam */
    CREDO_VERUM (cantus_praeparatum_sonitus(
                     chorda_ex_literis("radius", c->piscina), NIHIL)
                 == sonitus_praeparatum("radius"));
    CREDO_VERUM (cantus_praeparatum_sonitus(
                     chorda_ex_literis("radiu", c->piscina), NIHIL)
                 == NIHIL);
    CREDO_VERUM (cantus_praeparatum_sonitus(chorda_ex_literis(
        "radius_radius_radius_radius_radius_radius_radius_radius_radius_r",
        c->piscina), NIHIL) == NIHIL);
    si (_legere(c, "c @punctum d", &k))
    {
        CREDO_VERUM (cantus_iussa_parare(&k, &forma, r, iussa,
            &vitium));
        CREDO_F32_PROXIMUS (iussa[ZEPHYRUM].parametra.mora, 0.05f,
            0.0f);
        CREDO_F32_PROXIMUS (iussa[I].parametra.mora, punctum->mora,
            0.0f);
        CREDO_F32_PROXIMUS (iussa[I].parametra.saltus, punctum->saltus,
                            0.0f);
        CREDO_F32_PROXIMUS (iussa[I].parametra.frequentia,
                            sonus_notae_frequentia(62.0f), 0.0f);
    }
    /* resolutor vocantis cum contextu */
    meum.remissio    = 0.123f;
    forma.resolutor  = _resolutor_meus;
    forma.contextus  = &meum;
    si (_legere(c, "@meum c", &k))
    {
        CREDO_VERUM (cantus_iussa_parare(&k, &forma, r, iussa,
            &vitium));
        CREDO_F32_PROXIMUS (iussa[ZEPHYRUM].parametra.remissio, 0.123f,
                            0.0f);
    }
    /* ignotum: FALSUM ad notam eius, NULLA sedes capta (rector recens
     * duas sedes generatione I adhuc dat) */
    r                = mixtura_rectorem_creare(c->piscina, II);
    forma.resolutor  = NIHIL;
    forma.contextus  = NIHIL;
    si (r != NIHIL && _legere(c, "c @punctum d @ignotum e", &k))
    {
        vitium.offset = CMXCIX;
        CREDO_FALSUM (cantus_iussa_parare(&k, &forma, r, iussa,
            &vitium));
        CREDO_AEQUALIS_I32 (vitium.offset, XXII);
        CREDO_VERUM (vitium.causa != NIHIL);
        a = mixtura_rector_capere(r, I);
        b = mixtura_rector_capere(r, I);
        CREDO_AEQUALIS_I32 (a.generatio, I);
        CREDO_AEQUALIS_I32 (b.generatio, I);
    }
    /* color nullus et nota sine '@': vitium ad notam */
    forma.color = NIHIL;
    si (_legere(c, "@punctum c r c", &k) && _legere(c, "c", &k))
    {
        vitium.offset = CMXCIX;
        CREDO_FALSUM (cantus_iussa_parare(&k, &forma, r, iussa,
            &vitium));
        CREDO_AEQUALIS_I32 (vitium.offset, ZEPHYRUM);
    }
}

/* voces plenae prioritatis altioris: notae tacent, non vitium */
interior vacuum
_sectio_sedes (
    CredoContextus* c)
{
         Cantilena  k;
       CantusForma  forma;
      CantusVitium  vitium;
     MixturaIussum  iussa[II];
  SonitusParametra  color  = _colorem_parare();
     MixturaRector* r      = mixtura_rectorem_creare(c->piscina, I);

    si (r == NIHIL || !_legere(c, "c d", &k))
    {
        redde;
    }
    (vacuum)mixtura_rector_capere(r, IX);
    _formam_parare(&forma, &color);
    CREDO_VERUM (cantus_iussa_parare(&k, &forma, r, iussa, &vitium));
    CREDO_VERUM (mixtura_ansa_nulla(iussa[ZEPHYRUM].ansa));
    CREDO_VERUM (mixtura_ansa_nulla(iussa[I].ansa));
}

/* per mixturam: quaeque nota ad quadrum suum exactum, octetim =
 * sonitus_reddere(color ad altitudinem notae, semen + index) x
 * positio media */
interior vacuum
_sectio_reddere (
    CredoContextus* c)
{
         Cantilena  k;
       CantusForma  forma;
      CantusVitium  vitium;
     MixturaIussum  iussa[II];
      MixturaForma  forma_mixturae;
  SonitusParametra  color = _colorem_parare();
           Mixtura* m;
     MixturaRector* r;
               i32  j;
               i32  q;
               b32  pares = VERUM;

    forma_mixturae.voces = IV;
    forma_mixturae.iussa = VIII;
    forma_mixturae.nuntii = VIII;
    m = mixtura_creare(c->piscina, &forma_mixturae);
    r = mixtura_rectorem_creare(c->piscina, IV);
    si (m == NIHIL || r == NIHIL || !_legere(c, "t120 c8 r8 e8", &k))
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    _formam_parare(&forma, &color);
    forma.momentum = (s64)C;
    CREDO_VERUM (cantus_iussa_parare(&k, &forma, r, iussa, &vitium));
    per (j = ZEPHYRUM; j < k.numerus; j++)
    {
        SonitusParametra p = color;
                     i32 n;
                     i32 initium = (i32)(k.notae[j].initium + (s64)C);

        p.frequentia = sonus_notae_frequentia((f32)k.notae[j].nota);
        n = sonitus_reddere(&p, (i64)XL + (i64)j, tabula,
                            QUADRA_REDDENDA - initium);
        per (q = ZEPHYRUM; q < n; q++)
        {
            exspectata[initium + q] = tabula[q];
        }
        (vacuum)mixtura_iubere(m, &iussa[j]);
    }
    mixtura_reddere(m, exitus, QUADRA_REDDENDA);
    per (q = ZEPHYRUM; q < QUADRA_REDDENDA; q++)
    {
        f32 e = exspectata[q] * MEDIUM;

        si (exitus[II * q] != e || exitus[II * q + I] != e)
        {
            si (pares)
            {
                imprimere("  quadrum %d: %f != %f\n", (integer)q,
                          (duplex)exitus[II * q], (duplex)e);
            }
            pares = FALSUM;
        }
    }
    CREDO_VERUM (pares);
    /* non silentium: utraque nota sonat */
    CREDO_VERUM (exspectata[C + D] != 0.0f);
    CREDO_VERUM (exspectata[XXIV * M + C + D] != 0.0f);
    CREDO_VERUM (exspectata[C - I] == 0.0f);
}


    /* ==================================================
     * PULSUS: tempus musicum (CMLX per quartam) et mappa temporis
     * ================================================== */

/* quadrum notae cuiusque = cantus_quadrum_pulsus(mappa, pulsus eius);
 * finis totius idem */
interior b32
_mappa_congruit (
    constans Cantilena* k)
{
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus; i++)
    {
        si (   cantus_quadrum_pulsus(k->tempora, k->numerus_temporum,
                   k->notae[i].pulsus) != k->notae[i].initium
            || cantus_quadrum_pulsus(k->tempora, k->numerus_temporum,
                   k->notae[i].pulsus + k->notae[i].pulsus_longitudo)
               != k->notae[i].initium + k->notae[i].longitudo)
        {
            imprimere("  nota %d: pulsus %ld -> %ld, initium %ld\n",
                (integer)i, (long)k->notae[i].pulsus,
                (long)cantus_quadrum_pulsus(k->tempora,
                    k->numerus_temporum, k->notae[i].pulsus),
                (long)k->notae[i].initium);
            redde FALSUM;
        }
    }
    redde cantus_quadrum_pulsus(k->tempora, k->numerus_temporum,
        k->pulsus) == k->longitudo;
}

interior vacuum
_sectio_pulsus (
    CredoContextus* c)
{
             Cantilena  k;
          CantusTempus  mappa[III];
                   s64  q;
                   s64  p;
                   i32  j;
    constans character* melodiae[] = {
        "l4 c d8 e", "t180 o6 l16 e g# b >e8",
            "t120 c t60 d4. e8 t200 f",
        "t133 l16 c c c c c c c c c c c c c c c c c c c",
        "t97 l8. c d e t41 f8.. g64.. a", NIHIL };

    CREDO_GRADUS ("CMLX per quartam");
    CREDO_AEQUALIS_I32 (CANTUS_PULSUS_QUARTAE, CMLX);
    CREDO_AEQUALIS_I32 (CANTUS_PULSUS_INTEGRAE, IV * CMLX);

    CREDO_GRADUS ("notae in pulsibus");
    CREDO_NECESSE (_legere(c, "l4 c d8 e", &k));
    CREDO_NECESSE (k.numerus == III);
    CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (k.notae[I].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_S64 (k.notae[II].pulsus, (s64)(M + CDXL));
    CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].pulsus_longitudo, (s64)CMLX);
    CREDO_AEQUALIS_S64 (k.notae[I].pulsus_longitudo, (s64)CDLXXX);
    CREDO_AEQUALIS_S64 (k.notae[II].pulsus_longitudo, (s64)CMLX);
    CREDO_AEQUALIS_S64 (k.pulsus, (s64)(MM + CD));
    /* pausae pulsus movent, notas non */
    CREDO_NECESSE (_legere(c, "r8 c r2", &k));
    CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].pulsus, (s64)CDLXXX);
    CREDO_AEQUALIS_S64 (k.pulsus, (s64)(CDLXXX + CMLX + MCMXX));
    /* puncta: sexagesima quarta duplo puncto = LX + XXX + XV */
    CREDO_NECESSE (_legere(c, "c64..", &k));
    CREDO_AEQUALIS_S64 (k.notae[ZEPHYRUM].pulsus_longitudo, (s64)CV);

    /* orthographia: littera ut scripta (c# = C sharp, d- = D flat) */
    CREDO_NECESSE (_legere(c, "c# d- e", &k));
    CREDO_NECESSE (k.numerus == III);
    CREDO_VERUM (k.notae[ZEPHYRUM].littera == 'c');
    CREDO_VERUM (k.notae[I].littera == 'd');
    CREDO_AEQUALIS_I32 (k.notae[ZEPHYRUM].nota, k.notae[I].nota);
    CREDO_VERUM (k.notae[II].littera == 'e');

    CREDO_GRADUS ("mappa temporis");
    CREDO_NECESSE (_legere(c, "c d", &k));
    CREDO_NECESSE (k.numerus_temporum == I);
    CREDO_AEQUALIS_S64 (k.tempora[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (k.tempora[ZEPHYRUM].tempus, CXX);
    CREDO_AEQUALIS_I32 (k.tempora[ZEPHYRUM].millesimae, CXX * M);
    CREDO_AEQUALIS_S64 (k.tempora[ZEPHYRUM].quadrum, ZEPHYRUM);
    CREDO_NECESSE (_legere(c, "t120 c t60 d", &k));
    CREDO_NECESSE (k.numerus_temporum == II);
    CREDO_AEQUALIS_S64 (k.tempora[I].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_I32 (k.tempora[I].tempus, LX);
    CREDO_AEQUALIS_I32 (k.tempora[I].millesimae, LX * M);
    CREDO_AEQUALIS_I32 (k.tempora[ZEPHYRUM].millesimae, CXX * M);
    /* quarta ad t120 = 0.5 s = XXIV milia quadra */
    CREDO_AEQUALIS_S64 (k.tempora[I].quadrum, (s64)(XXIV * M));
    /* 't' initio: segmentum primum, non duo */
    CREDO_NECESSE (_legere(c, "t90 c", &k));
    CREDO_AEQUALIS_I32 (k.numerus_temporum, I);
    CREDO_AEQUALIS_I32 (k.tempora[ZEPHYRUM].tempus, XC);

    CREDO_GRADUS ("quadrum pulsus = initium notae (melodiae variae)");
    per (j = ZEPHYRUM; melodiae[j] != NIHIL; j++)
    {
        CREDO_NECESSE (_legere(c, melodiae[j], &k));
        CREDO_VERUM (_mappa_congruit(&k));
    }

    CREDO_GRADUS ("computare: mappa manu scripta");
    mappa[ZEPHYRUM].pulsus      = ZEPHYRUM;
    mappa[ZEPHYRUM].tempus      = CXX;
    mappa[ZEPHYRUM].millesimae  = CXX * M;
    mappa[I].pulsus             = (s64)CMLX * IV;
    mappa[I].tempus             = LX;
    mappa[I].millesimae         = LX * M;
    mappa[II].pulsus            = (s64)CMLX * VI;
    mappa[II].tempus            = CCXL;
    mappa[II].millesimae        = CCXL * M;
    cantus_tempora_computare(mappa, III);
    CREDO_AEQUALIS_S64 (mappa[ZEPHYRUM].quadrum, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (mappa[I].quadrum, (s64)(XCVI * M));
    CREDO_AEQUALIS_S64 (mappa[II].quadrum, (s64)(XCVI * M + XCVI * M));
    /* quarta post II mutationem (t240) = XII milia */
    CREDO_AEQUALIS_S64 (cantus_quadrum_pulsus(mappa, III,
        (s64)CMLX * VII), (s64)(CXCII * M + XII * M));
    /* mappa vacua: CXX */
    CREDO_AEQUALIS_S64 (cantus_quadrum_pulsus(NIHIL, ZEPHYRUM,
        (s64)CMLX), (s64)(XXIV * M));

    /* millesimae: quarta ad 90.5 = 0.662983 s = 31823.2 quadra;
     * computatio millesimas legit, non tempus */
    {
        CantusTempus fracta[II];

        fracta[ZEPHYRUM].pulsus      = ZEPHYRUM;
        fracta[ZEPHYRUM].tempus      = XC;
        fracta[ZEPHYRUM].millesimae  = XC * M + D;
        fracta[I].pulsus             = (s64)CMLX;
        fracta[I].tempus             = XC;
        fracta[I].millesimae         = XC * M;
        cantus_tempora_computare(fracta, II);
        CREDO_AEQUALIS_S64 (fracta[I].quadrum, (s64)XXXI * M
                            + DCCCXXIII);
        CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(fracta, II,
            fracta[I].quadrum), (s64)CMLX);
        /* inversa INTRA segmentum 90.5 (non in termino) */
        CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(fracta, II,
            cantus_quadrum_pulsus(fracta, II, (s64)CDLXXX)),
            (s64)CDLXXX);
        /* longum: IV M D quartae ad 90.5 (fere L minuta) in segmento
         * uno */
        CREDO_AEQUALIS_S64 (cantus_quadrum_pulsus(fracta, I,
            (s64)CMLX * (M * IV + D)), (s64)M * M * CXLIII
            + (s64)M * CCIV + (s64)CDXX);
    }

    CREDO_GRADUS ("pulsus quadri: inversa, limites segmentorum");
    CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(mappa, III, ZEPHYRUM),
                        ZEPHYRUM);
    CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(mappa, III,
        (s64)(XCVI * M)), (s64)CMLX * IV);
    CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(mappa, III,
        (s64)(XCVI * M) - I), (s64)CMLX * IV - I);
    CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(mappa, III,
        (s64)(CXCII * M + XII * M)), (s64)CMLX * VII);
    /* aequalitas rotundationis: ad t240 pulsus = 12.5 quadra, pulsus I
     * -> quadrum XIII; quadrum XII ergo pulsum 0 segmenti dat, non I */
    CREDO_AEQUALIS_S64 (cantus_quadrum_pulsus(mappa, III,
        (s64)CMLX * VI + I), (s64)(CXCII * M + XIII));
    CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(mappa, III,
        (s64)(CXCII * M + XII)), (s64)CMLX * VI);
    /* reditus: pulsus quadri(quadrum pulsus(p)) == p; quadrum pulsus(
     * pulsus quadri(q)) <= q < quadrum pulsus(p + 1) */
    per (p = ZEPHYRUM; p < (s64)CMLX * VIII; p += CCCLXXXIII)
    {
        q = cantus_quadrum_pulsus(mappa, III, p);
        CREDO_AEQUALIS_S64 (cantus_pulsus_quadri(mappa, III, q), p);
        CREDO_VERUM (cantus_quadrum_pulsus(mappa, III,
            cantus_pulsus_quadri(mappa, III, q + VII)) <= q + VII);
        CREDO_VERUM (cantus_quadrum_pulsus(mappa, III,
            cantus_pulsus_quadri(mappa, III, q + VII) + I) > q + VII);
    }
}


    /* ==================================================
     * LINEAE: '|' servatae (pulsus totus, offset), sonum non mutant
     * ================================================== */

interior vacuum
_sectio_lineae (
    CredoContextus* c)
{
    Cantilena k;
    Cantilena sine;

    CREDO_NECESSE (_legere(c, "c d | e f | g", &k));
    CREDO_NECESSE (k.numerus_linearum == II);
    CREDO_AEQUALIS_S64 (k.lineae[ZEPHYRUM].pulsus, (s64)MCMXX);
    CREDO_AEQUALIS_I32 (k.lineae[ZEPHYRUM].offset, IV);
    CREDO_AEQUALIS_S64 (k.lineae[I].pulsus, (s64)MMMDCCCXL);
    CREDO_AEQUALIS_I32 (k.lineae[I].offset, X);
    /* sonum non mutant: eadem quadra sine lineis */
    CREDO_NECESSE (_legere(c, "c d e f g", &sine));
    CREDO_AEQUALIS_S64 (k.notae[IV].initium, sine.notae[IV].initium);
    CREDO_AEQUALIS_S64 (k.longitudo, sine.longitudo);
    CREDO_AEQUALIS_I32 (sine.numerus_linearum, ZEPHYRUM);
    CREDO_VERUM (sine.lineae == NIHIL);

    /* pulsus TOTUS trans mutationem temporis */
    CREDO_NECESSE (_legere(c, "t60 c | t120 d |", &k));
    CREDO_NECESSE (k.numerus_linearum == II);
    CREDO_AEQUALIS_S64 (k.lineae[ZEPHYRUM].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_I32 (k.lineae[ZEPHYRUM].offset, VI);
    CREDO_AEQUALIS_S64 (k.lineae[I].pulsus, (s64)MCMXX);
    CREDO_AEQUALIS_I32 (k.lineae[I].offset, XV);

    /* initio, iteratae, post pausam punctatam */
    CREDO_NECESSE (_legere(c, "|r4. || c8", &k));
    CREDO_NECESSE (k.numerus_linearum == III);
    CREDO_AEQUALIS_S64 (k.lineae[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (k.lineae[ZEPHYRUM].offset, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (k.lineae[I].pulsus, (s64)MCDXL);
    CREDO_AEQUALIS_I32 (k.lineae[I].offset, V);
    CREDO_AEQUALIS_S64 (k.lineae[II].pulsus, (s64)MCDXL);
    CREDO_AEQUALIS_I32 (k.lineae[II].offset, VI);

    /* vitium post lineam: nihil semi-lectum */
    _vitium(c, "c | x", IV);
}

/* status post praefixum: ordinaria (o4 l4 v100 sine '@'), mandata
 * singula, ad lineam, ad finem; '<' '>' extra 0..8 licent; praefixum
 * intra signum ('l|8') vitium; offset ultra finem FALSUM */
interior b32
_status_est (
         CredoContextus* c,
     constans character* textus,
                    i32  offset,
                    s32  octava,
                    i32  ordinaria,
                    i32  lucrum,
     constans character* praeparatum,
                    s64  pulsus)
{
    CantusStatus s;
    CantusVitium v;
             i32 lp = (i32)strlen(praeparatum);

    memset(&s, ZEPHYRUM, magnitudo(s));
    si (   cantus_status_ad(chorda_ex_literis(textus, c->piscina),
                            offset, &s, &v)
        && s.octava              == octava && s.ordinaria == ordinaria
        && s.lucrum              == lucrum && s.pulsus == pulsus
        && s.praeparatum.mensura == lp
        && (lp == ZEPHYRUM
            || memcmp(s.praeparatum.datum, praeparatum, (size_t)lp)
                   == ZEPHYRUM))
    {
        redde VERUM;
    }
    imprimere("  '%s' @%u: o%d l%u v%u '%.*s' pulsus %lld\n", textus,
              offset, (integer)s.octava, s.ordinaria, s.lucrum,
              (integer)s.praeparatum.mensura,
              s.praeparatum.datum ? (constans character*)
                                    s.praeparatum.datum : "",
              (long long)s.pulsus);
    redde FALSUM;
}

interior vacuum
_sectio_status (
    CredoContextus* c)
{
    hic_manens constans character T[] =
        "o5 l8 v50 @punctum c d | >e4 <f";
    CantusStatus s;
    CantusVitium v;

    CREDO_VERUM (_status_est(c, T, ZEPHYRUM, IV, CMLX, C, "",
                             ZEPHYRUM));
    CREDO_VERUM (_status_est(c, T, III, V, CMLX, C, "", ZEPHYRUM));
    CREDO_VERUM (_status_est(c, T, VI, V, CDLXXX, C, "", ZEPHYRUM));
    CREDO_VERUM (_status_est(c, T, X, V, CDLXXX, L, "", ZEPHYRUM));
    /* ad '|' (offset XXIII): praeparatum vigens, pulsus duarum
     * octavarum */
    CREDO_VERUM (_status_est(c, T, XXIII, V, CDLXXX, L, "punctum",
                             CMLX));
    /* finis: '>' deinde '<' - octava V; 'e4' longitudinem ordinariam
     * non mutat */
    CREDO_VERUM (_status_est(c, T, (i32)strlen(T), V, CDLXXX, L,
                             "punctum", CMLX + CMLX + CDLXXX));
    CREDO_VERUM (_status_est(c, "l4. c", III, IV, MCDXL, C, "",
                             ZEPHYRUM));
    /* lucrum ex f32: 0.59f * C = 58.99... rotundatur (truncatio LVIII
     * daret; inter 0..100 solum LIII et LIX ita cadunt) */
    CREDO_VERUM (_status_est(c, "v59 c", IV, IV, CMLX, LIX, "",
                             ZEPHYRUM));
    CREDO_VERUM (_status_est(c, "o8 >", IV, IX, CMLX, C, "",
                             ZEPHYRUM));
    CREDO_VERUM (_status_est(c, "o0 <", IV, -I, CMLX, C, "",
                             ZEPHYRUM));
    CREDO_VERUM (_status_est(c, "", ZEPHYRUM, IV, CMLX, C, "",
                             ZEPHYRUM));
    /* praefixum intra signum: 'l' sine longitudine */
    CREDO_FALSUM (cantus_status_ad(chorda_ex_literis("l8 c",
                                                     c->piscina),
                                   I, &s, &v));
    CREDO_FALSUM (cantus_status_ad(chorda_ex_literis("o9 c",
                                                     c->piscina),
                                   II, &s, &v));
    CREDO_AEQUALIS_I32 (v.offset, I);
    /* ultra finem */
    CREDO_FALSUM (cantus_status_ad(chorda_ex_literis("c4", c->piscina),
                                   III, &s, &v));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "notae", _sectio_notae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pulsus", _sectio_pulsus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensurae", _sectio_mensurae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mandata", _sectio_mandata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lineae", _sectio_lineae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iussa", _sectio_iussa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "praeparata", _sectio_praeparata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sedes", _sectio_sedes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "status", _sectio_status, NIHIL, NIHIL, NIHIL, NIHIL },
    { "reddere", _sectio_reddere, _parare_tabulas, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cantus", SECTIONES);
}
