/* probatio_mundus_mappa.c - mappa tabulae gressus (mundus_mappa.h)
 *
 * In loco ficto: mensura imaginis, colores generum (aqua, solidum,
 * gressibile altitudine clarescens, nihil album), cellae non
 * attactae notatae, reticulum, numeri in margine, notae rerum,
 * determinata, argumenta mala.
 */
#include "latina.h"
#include "piscina.h"
#include "imago_typus.h"
#include "stereometria.h"
#include "statua.h"
#include "mundus_solum.h"
#include "mundus_mappa.h"
#include "credo.h"
#include "mundus_locus_fictus.h"
#include <stdio.h>
#include <string.h>

#define PIXELA_CELLAE    MUNDUS_MAPPA_PIXELA_PER_CELLAM
#define MARGO  MUNDUS_MAPPA_MARGO

/* pixelum in medio cellae quae punctum mundi (x, z) continet;
 * locus fictus origo (0, 0) habet nisi aliter dicitur */
interior constans i8*
_pixelum_mundi (
          constans Imago* imago,
    constans MundusSolum* s,
                     f32  x,
                     f32  z)
{
    i32 cella_x = (i32)((x - s->origo_x)
        * (f32)MUNDUS_CELLAE_PER_UNITATEM);
    i32 cella_z = (i32)((z - s->origo_z)
        * (f32)MUNDUS_CELLAE_PER_UNITATEM);

    redde imago->pixela
        + ((memoriae_index)(MARGO + cella_z * PIXELA_CELLAE + II)
            * imago->latitudo + MARGO + cella_x * PIXELA_CELLAE
                + II) * IV;
}

interior b32
_album (
    constans i8* p)
{
    redde p[ZEPHYRUM] == 255 && p[I] == 255 && p[II] == 255;
}

/* pixela non alba in rectangulo */
interior i32
_non_alba (
    constans Imago* imago,
               i32  x0,
               i32  y0,
               i32  x1,
               i32  y1)
{
    i32 numerus = ZEPHYRUM;
    i32 x;
    i32 y;

    per (y = y0; y < y1; y++)
    {
        per (x = x0; x < x1; x++)
        {
            numerus += (i32)!_album(imago->pixela
                + ((memoriae_index)y * imago->latitudo + x) * IV);
        }
    }
    redde numerus;
}

interior vacuum
_sectio_colores (
    CredoContextus* c)
{
         Statua* locus  = loci_ficti_locus(c);
    MundusSolum* s      = mundus_solum_struere(locus, c->piscina);
          Imago  imago;
    constans i8* p;
    constans i8* q;
             i8  signum = VII;

    CREDO_NECESSE (s != NIHIL);
    imago.pixela = &signum;
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, NIHIL, ZEPHYRUM,
        &imago, c->piscina));
    CREDO_AEQUALIS_I32 (imago.latitudo, MARGO
        + s->latitudo * PIXELA_CELLAE);
    CREDO_AEQUALIS_I32 (imago.altitudo, MARGO
        + s->altitudo * PIXELA_CELLAE);
    CREDO_VERUM (imago.pixela != &signum);
    /* aqua (2.1, 2.1): caeruleum vincit */
    p = _pixelum_mundi(&imago, s, 2.1f, 2.1f);
    CREDO_VERUM (p[II] > p[ZEPHYRUM] + L && p[II] > p[I] + L);
    CREDO_AEQUALIS_I32 (p[III], 255);
    /* saxum solidum (5.6, 1.6): obscurum et griseum */
    p = _pixelum_mundi(&imago, s, 5.6f, 1.6f);
    CREDO_VERUM (p[ZEPHYRUM] < LXXX && p[I] < LXXX && p[II] < LXXX);
    CREDO_AEQUALIS_I32 (p[ZEPHYRUM], p[I]);
    /* solum imum (2.1, 10.1): viride */
    p = _pixelum_mundi(&imago, s, 2.1f, 10.1f);
    CREDO_VERUM (p[I] > p[ZEPHYRUM] && p[I] > p[II]);
    /* superius (10.1, 4.1) altitudine IV: clarius quam imum */
    q = _pixelum_mundi(&imago, s, 10.1f, 4.1f);
    CREDO_VERUM (q[I] > q[II]);
    CREDO_VERUM (q[ZEPHYRUM] > p[ZEPHYRUM] + L);
    CREDO_VERUM (q[I] > p[I] + XX);
    /* margo: charta alba inter numeros */
    CREDO_VERUM (_album(imago.pixela));
    /* omnia opaca */
    CREDO_AEQUALIS_I32 (imago.pixela[(memoriae_index)imago.latitudo
        * imago.altitudo * IV - I], 255);
}

/* tabula cum cellis NIHIL: locus parvus in angulo spatii maioris */
interior vacuum
_sectio_nihil (
    CredoContextus* c)
{
    LociFictiQuadratum  q[II];
           MundusSolum* s;
                 Imago  imago;

    q[ZEPHYRUM]  = loci_ficti_quadratum(0.0f, 2.0f, 0.0f, 2.0f, 0.0f,
        0.0f, LOCI_FICTI_GRESSIBILE);
    q[I]         = loci_ficti_quadratum(6.0f, 8.0f, 6.0f, 8.0f, 0.0f,
        0.0f, LOCI_FICTI_GRESSIBILE);
    s = mundus_solum_struere(loci_ficti_statua(c, q, II), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, NIHIL, ZEPHYRUM,
        &imago, c->piscina));
    CREDO_FALSUM (_album(_pixelum_mundi(&imago, s, 1.1f, 1.1f)));
    CREDO_VERUM (_album(_pixelum_mundi(&imago, s, 5.1f, 1.1f)));
    CREDO_FALSUM (_album(_pixelum_mundi(&imago, s, 7.1f, 7.1f)));
}

/* cellae non attactae roseae; attactae virides */
interior vacuum
_sectio_attingibiles (
    CredoContextus* c)
{
    LociFictiQuadratum  q[II];
           MundusSolum* s;
                 Imago  imago;
                 Imago  nuda;
                    i8* visa;
           constans i8* p;
                   i32  attactae;

    /* duae insulae: altera ab initio non attingitur */
    q[ZEPHYRUM]  = loci_ficti_quadratum(0.0f, 4.0f, 0.0f, 4.0f, 0.0f,
        0.0f, LOCI_FICTI_GRESSIBILE);
    q[I]         = loci_ficti_quadratum(6.0f, 8.0f, 0.0f, 4.0f, 0.0f,
        0.0f, LOCI_FICTI_GRESSIBILE);
    s = mundus_solum_struere(loci_ficti_statua(c, q, II), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    visa = (i8*)loci_ficti_memoria(c,
        (memoriae_index)s->latitudo * s->altitudo);
    attactae = mundus_solum_attingere(s, 2.0f, 2.0f, 0.5f, visa,
        c->piscina);
    CREDO_VERUM (attactae > C);
    CREDO_NECESSE (mundus_mappa_pingere(s, visa, NIHIL, ZEPHYRUM,
        &imago, c->piscina));
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, NIHIL, ZEPHYRUM,
        &nuda, c->piscina));
    /* insula attacta: viride */
    p = _pixelum_mundi(&imago, s, 2.1f, 2.1f);
    CREDO_VERUM (p[I] > p[ZEPHYRUM] && p[I] > p[II]);
    /* insula altera: roseum (rubrum vincit); sine tabula viride */
    p = _pixelum_mundi(&imago, s, 7.1f, 2.1f);
    CREDO_VERUM (p[ZEPHYRUM] > p[I] + L && p[ZEPHYRUM] > p[II] + L);
    p = _pixelum_mundi(&nuda, s, 7.1f, 2.1f);
    CREDO_VERUM (p[I] > p[ZEPHYRUM] && p[I] > p[II]);
    /* margo insulae attactae (corpus radii dimidii eo non pervenit)
     * roseus */
    p = _pixelum_mundi(&imago, s, 0.1f, 0.1f);
    CREDO_VERUM (p[ZEPHYRUM] > p[I] + L);
}

/* reticulum: linea omni IV unitate obscurior; numeri in margine */
interior vacuum
_sectio_reticulum (
    CredoContextus* c)
{
    LociFictiQuadratum  q;
           MundusSolum* s;
                 Imago  imago;
           constans i8* in_linea;
           constans i8* iuxta;
           constans i8* magna;

    q = loci_ficti_quadratum(0.0f, 20.0f, 0.0f, 20.0f, 0.0f, 0.0f,
        LOCI_FICTI_GRESSIBILE);
    s = mundus_solum_struere(loci_ficti_statua(c, &q, I), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, NIHIL, ZEPHYRUM,
        &imago, c->piscina));
    /* linea x = IV: columna MARGO + IV * IV * PIXELA_CELLAE */
    in_linea = imago.pixela + ((memoriae_index)(MARGO + XXX)
        * imago.latitudo + MARGO + IV * IV * PIXELA_CELLAE) * IV;
    iuxta    = in_linea + IV * III;
    CREDO_VERUM (in_linea[I] < iuxta[I]);
    /* linea x = XVI crassior (obscurior) quam x = IV */
    magna = imago.pixela + ((memoriae_index)(MARGO + XXX)
        * imago.latitudo + MARGO + XVI * IV * PIXELA_CELLAE) * IV;
    CREDO_VERUM (magna[I] < in_linea[I]);
    /* linea z = IV */
    in_linea = imago.pixela + ((memoriae_index)(MARGO
        + IV * IV * PIXELA_CELLAE)
        * imago.latitudo + MARGO + XXX) * IV;
    iuxta    = in_linea + (memoriae_index)imago.latitudo * IV * III;
    CREDO_VERUM (in_linea[I] < iuxta[I]);
    /* x = II: nulla linea */
    in_linea = imago.pixela + ((memoriae_index)(MARGO + XXX)
        * imago.latitudo + MARGO + II * IV * PIXELA_CELLAE) * IV;
    CREDO_AEQUALIS_I32 (in_linea[I], in_linea[IV * III + I]);
    /* numeri: "8" et "16" in margine supero circa x = VIII et XVI;
     * circa x = IV nullus */
    CREDO_VERUM (_non_alba(&imago, MARGO + VIII * IV * PIXELA_CELLAE
        - VIII,
        ZEPHYRUM, MARGO + VIII * IV * PIXELA_CELLAE + VIII, MARGO - II)
            > V);
    CREDO_VERUM (_non_alba(&imago, MARGO + XVI * IV * PIXELA_CELLAE
        - VIII,
        ZEPHYRUM, MARGO + XVI * IV * PIXELA_CELLAE + VIII, MARGO - II)
            > X);
    CREDO_AEQUALIS_I32 (_non_alba(&imago,
        MARGO + IV * IV * PIXELA_CELLAE - VIII, ZEPHYRUM,
        MARGO + IV * IV * PIXELA_CELLAE + VIII, MARGO - II), ZEPHYRUM);
    /* "16" plus atramenti habet quam "8" (duae litterae) */
    CREDO_VERUM (_non_alba(&imago, MARGO + XVI * IV * PIXELA_CELLAE
        - VIII,
            ZEPHYRUM, MARGO + XVI * IV * PIXELA_CELLAE + VIII, MARGO
                - II)
        > _non_alba(&imago, MARGO + VIII * IV * PIXELA_CELLAE - VIII,
        ZEPHYRUM,
            MARGO + VIII * IV * PIXELA_CELLAE + VIII, MARGO - II));
    /* margo sinister: numerus ad z = VIII */
    CREDO_VERUM (_non_alba(&imago, ZEPHYRUM,
        MARGO + VIII * IV * PIXELA_CELLAE - VI, MARGO - I,
        MARGO + VIII * IV * PIXELA_CELLAE + VI) > V);
}

/* notae: punctum nigrum cum medio albo et numero in tabella */
interior vacuum
_sectio_notae (
    CredoContextus* c)
{
    LociFictiQuadratum  q;
           MundusSolum* s;
       MundusMappaNota  notae[IV];
                 Imago  imago;
                 Imago  nuda;
           constans i8* p;
                   i32  x;
                   i32  y;

    q = loci_ficti_quadratum(0.0f, 20.0f, 0.0f, 20.0f, 0.0f, 0.0f,
        LOCI_FICTI_GRESSIBILE);
    s = mundus_solum_struere(loci_ficti_statua(c, &q, I), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    notae[ZEPHYRUM].x        = 6.0f;
    notae[ZEPHYRUM].z        = 10.0f;
    notae[ZEPHYRUM].numerus  = VII;
    notae[I].x               = 14.5f;
    notae[I].z               = 3.25f;
    notae[I].numerus         = XLII;
    /* extra tabulam: non pingitur, nihil ruit */
    notae[II].x        = -50.0f;
    notae[II].z        = 900.0f;
    notae[II].numerus  = IX;
    /* paulo ante marginem sinistrum: in margine non pingitur */
    notae[III].x        = -1.0f;
    notae[III].z        = 5.0f;
    notae[III].numerus  = VIII;
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, notae, IV, &imago,
        c->piscina));
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, NIHIL, ZEPHYRUM,
        &nuda, c->piscina));
    x = MARGO + (i32)(6.0f * 4.0f) * PIXELA_CELLAE;
    y = MARGO + (i32)(10.0f * 4.0f) * PIXELA_CELLAE;
    /* medium album, iuxta nigrum */
    p = imago.pixela + ((memoriae_index)y * imago.latitudo + x) * IV;
    CREDO_VERUM (_album(p));
    p = imago.pixela + ((memoriae_index)y * imago.latitudo + x + II)
        * IV;
    CREDO_AEQUALIS_I32 (p[ZEPHYRUM], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (p[I], ZEPHYRUM);
    /* tabella numeri: alba cum atramento */
    p = imago.pixela + ((memoriae_index)(y - V) * imago.latitudo
        + x + IV) * IV;
    CREDO_VERUM (_album(p));
    /* nota II numerum longiorem habet: plus pixelorum mutatorum */
    {
        i32 mutata_prima   = ZEPHYRUM;
        i32 mutata_altera  = ZEPHYRUM;
        i32 dx;
        i32 dy;
        i32 x2 = MARGO + (i32)(14.5f * 4.0f) * PIXELA_CELLAE - IV;
        i32 y2 = MARGO + (i32)(3.25f * 4.0f) * PIXELA_CELLAE - VIII;

        /* fenestra XXVIII x XVI circa notam, ab angulo eius */
        x -= IV;
        y -= VIII;
        per (dy = ZEPHYRUM; dy < XVI; dy++)
        {
            per (dx = ZEPHYRUM; dx < XXVIII; dx++)
            {
                mutata_prima += (i32)(memcmp(
                    imago.pixela + ((memoriae_index)(y + dy)
                        * imago.latitudo + x + dx) * IV,
                    nuda.pixela + ((memoriae_index)(y + dy)
                        * nuda.latitudo + x + dx) * IV, IV)
                    != ZEPHYRUM);
                mutata_altera += (i32)(memcmp(
                    imago.pixela + ((memoriae_index)(y2 + dy)
                        * imago.latitudo + x2 + dx) * IV,
                    nuda.pixela + ((memoriae_index)(y2 + dy)
                        * nuda.latitudo + x2 + dx) * IV, IV)
                    != ZEPHYRUM);
            }
        }
        CREDO_VERUM (mutata_prima > XL);
        CREDO_VERUM (mutata_altera > mutata_prima);
    }
    /* margo sinister ad z = V: nota extra tabulam nihil reliquit */
    CREDO_VERUM (memcmp(
        imago.pixela + ((memoriae_index)(MARGO + XX * PIXELA_CELLAE)
            * imago.latitudo) * IV,
        nuda.pixela + ((memoriae_index)(MARGO + XX * PIXELA_CELLAE)
            * nuda.latitudo) * IV, (memoriae_index)XL * IV)
        == ZEPHYRUM);
    /* procul a notis imago eadem */
    CREDO_VERUM (memcmp(
        imago.pixela + ((memoriae_index)(MARGO + CC)
            * imago.latitudo) * IV,
        nuda.pixela + ((memoriae_index)(MARGO + CC) * nuda.latitudo)
            * IV, (memoriae_index)imago.latitudo * IV) == ZEPHYRUM);
}

interior vacuum
_sectio_argumenta (
    CredoContextus* c)
{
            Statua* locus  = loci_ficti_locus(c);
       MundusSolum* s      = mundus_solum_struere(locus, c->piscina);
       MundusSolum  vacua;
   MundusMappaNota  nota;
             Imago  imago;
             Imago  altera;
                i8  signum = VII;

    CREDO_NECESSE (s != NIHIL);
    memset(&nota, ZEPHYRUM, magnitudo(nota));
    memset(&vacua, ZEPHYRUM, magnitudo(vacua));
    imago.pixela    = &signum;
    imago.latitudo  = 99;
    CREDO_FALSUM (mundus_mappa_pingere(NIHIL, NIHIL, NIHIL, ZEPHYRUM,
        &imago, c->piscina));
    CREDO_FALSUM (mundus_mappa_pingere(s, NIHIL, NIHIL, ZEPHYRUM,
        NIHIL, c->piscina));
    CREDO_FALSUM (mundus_mappa_pingere(s, NIHIL, NIHIL, ZEPHYRUM,
        &imago, NIHIL));
    CREDO_FALSUM (mundus_mappa_pingere(s, NIHIL, NIHIL, II, &imago,
        c->piscina));
    CREDO_FALSUM (mundus_mappa_pingere(&vacua, NIHIL, NIHIL, ZEPHYRUM,
        &imago, c->piscina));
    CREDO_VERUM (imago.pixela == &signum);
    CREDO_AEQUALIS_I32 (imago.latitudo, 99);
    /* determinata */
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, &nota, I, &imago,
        c->piscina));
    CREDO_NECESSE (mundus_mappa_pingere(s, NIHIL, &nota, I, &altera,
        c->piscina));
    CREDO_VERUM (imago.pixela != altera.pixela);
    CREDO_VERUM (memcmp(imago.pixela, altera.pixela,
        (memoriae_index)imago.latitudo * imago.altitudo * IV)
        == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "colores", _sectio_colores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nihil", _sectio_nihil, NIHIL, NIHIL, NIHIL, NIHIL },
    { "attingibiles", _sectio_attingibiles, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "reticulum", _sectio_reticulum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "notae", _sectio_notae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "argumenta", _sectio_argumenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_mappa", SECTIONES);
}
