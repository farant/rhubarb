/* probatio_plastica_textura.c - texturae genitae et coordinatae
 * (plastica_textura.h)
 *
 * generare: genera IX, argumenta mala, colores tres soli,
 * determinata, digesta fixa (arithmetica integra: eadem in omni
 * machina). In descriptione: texella quadrata densitate XVI in omni
 * latere omnis trianguli, summum texturae sursum, ambitus continuus
 * cum sutura una, colores derivati, textura communicata, textura
 * manu scripta, recusationes cum linea.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago_typus.h"
#include "friatio.h"
#include "statua.h"
#include "plastica.h"
#include "plastica_textura.h"
#include "credo.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define LATUS PLASTICA_TEXTURA_LATUS

hic_manens constans i8 COLORES[IX] = {
    10, 20, 30,   100, 110, 120,   200, 210, 220
};

interior b32
_genera (
        CredoContextus* c,
    constans character* genus,
                   i32  latitudo,
                   i32  altitudo,
                   i32  semen,
                 Imago* exitus)
{
    redde plastica_textura_generare(
        chorda_ex_literis(genus, c->piscina), latitudo, altitudo,
        semen, COLORES, exitus, c->piscina);
}

/* index coloris pixeli inter tres; -1 si alius aut non opacus */
interior s32
_index_coloris (
    constans i8* p,
    constans i8* colores)
{
    s32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (   memcmp(p, colores + k * III, III) == ZEPHYRUM
            && p[III]                            == 255)
        {
            redde k;
        }
    }
    redde -I;
}

interior i32
_digestum (
    constans Imago* imago)
{
    redde friatio_fnv1a_literis((constans character*)imago->pixela,
        imago->latitudo * imago->altitudo * IV);
}

interior PlasticaFructus
_lege (
        CredoContextus* c,
    constans character* literae)
{
    redde plastica_legere(chorda_ex_literis(literae, c->piscina),
        c->piscina);
}

interior Statua*
_statua (
        CredoContextus* c,
    constans character* literae)
{
    PlasticaFructus f = _lege(c, literae);

    si (!f.successus)
    {
        printf("descriptio recusata (linea %u): %.*s\n",
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    redde f.statua;
}

/* recusatur in linea data, nuntio fragmentum continente */
#define RECUSATUR(literae, linea_sperata, fragmentum) \
    fac { \
        PlasticaFructus rf = _lege(c, (literae)); \
        CREDO_FALSUM (rf.successus); \
        CREDO_VERUM (rf.statua == NIHIL); \
        CREDO_AEQUALIS_I32 (rf.linea, (linea_sperata)); \
        si (!chorda_continet(rf.error, \
            chorda_ex_literis((fragmentum), c->piscina))) \
        { \
            printf("nuntius: %.*s\n", (integer)rf.error.mensura, \
                (constans character*)rf.error.datum); \
        } \
        CREDO_VERUM (chorda_continet(rf.error, \
            chorda_ex_literis((fragmentum), c->piscina))); \
    } dum (FALSUM)

/* textura tractus cui index k pertinet; NIHIL si nulla */
interior constans Imago*
_textura_indicis (
    constans Statua* s,
                i32  k)
{
    i32 j;

    per (j = ZEPHYRUM; j < s->partium; j++)
    {
        constans StatuaPars* pars = &s->partes[j];

        si (   k >= pars->index_primus
            && k < pars->index_primus + pars->indicum)
        {
            s32 textura = pars->materia < ZEPHYRUM ? -I
                : s->materiae[pars->materia].textura;

            redde textura < ZEPHYRUM ? NIHIL : &s->texturae[textura];
        }
    }
    redde NIHIL;
}

/* latera triangulorum texturatorum quorum longitudo in texellis
 * NON est XVI per longitudinem in spatio solidi; *omnia = latera
 * inspecta */
interior i32
_latera_extenta (
    constans Statua* s,
                i32* omnia)
{
    i32 falsa = ZEPHYRUM;
    i32 k;
    i32 a;

    *omnia = ZEPHYRUM;
    per (k = ZEPHYRUM; k + II < s->indicum; k += III)
    {
        constans Imago* textura = _textura_indicis(s, k);

        si (!textura)
        {
            perge;
        }
        per (a = ZEPHYRUM; a < III; a++)
        {
                     i32  v        = s->indices[k + a];
                     i32  w        = s->indices[k + (a + I) % III];
            constans f32* p        = s->positiones + v * III;
            constans f32* q        = s->positiones + w * III;
            constans f32* e        = s->coordinatae + v * II;
            constans f32* f        = s->coordinatae + w * II;
                     f64  motus_x  = (f64)(p[ZEPHYRUM] - q[ZEPHYRUM]);
                     f64  motus_y  = (f64)(p[I] - q[I]);
                     f64  motus_z  = (f64)(p[II] - q[II]);
                     f64  motus_s = (f64)(e[ZEPHYRUM] - f[ZEPHYRUM])
                         * (f64)textura->latitudo;
                     f64 motus_t = (f64)(e[I] - f[I])
                         * (f64)textura->altitudo;

            (*omnia)++;
            si (fabs(sqrt(motus_s * motus_s + motus_t * motus_t)
                    - (f64)PLASTICA_TEXELLA_PER_UNITATEM
                    * sqrt(motus_x * motus_x + motus_y * motus_y
                        + motus_z * motus_z)) > 0.01)
            {
                falsa++;
            }
        }
    }
    redde falsa;
}

interior b32
_lateralis (
    constans Statua* s,
                i32  v)
{
    redde fabs((f64)s->normales[v * III + I]) < 0.5;
}

interior b32
_idem_locus (
    constans Statua* s,
                i32  v,
                i32  w)
{
    constans f32* p = s->positiones + v * III;
    constans f32* q = s->positiones + w * III;

    redde fabs((f64)(p[ZEPHYRUM] - q[ZEPHYRUM])) < 1.0e-4
        && fabs((f64)(p[I] - q[I])) < 1.0e-4
        && fabs((f64)(p[II] - q[II])) < 1.0e-4;
}

/* loci in lateribus (normalis non erecta) ubi vertices eiusdem
 * loci coordinatas diversas ferunt: suturae */
interior i32
_suturae (
    constans Statua* s)
{
    i32 suturae = ZEPHYRUM;
    i32 v;
    i32 w;

    per (v = ZEPHYRUM; v < s->verticum; v++)
    {
        b32 primus   = VERUM;
        b32 differt  = FALSUM;

        si (!_lateralis(s, v))
        {
            perge;
        }
        per (w = ZEPHYRUM; w < v && primus; w++)
        {
            si (_lateralis(s, w) && _idem_locus(s, v, w))
            {
                primus = FALSUM;
            }
        }
        per (w = v + I; primus && w < s->verticum; w++)
        {
            si (   _lateralis(s, w) && _idem_locus(s, v, w)
                && (   fabs((f64)(s->coordinatae[v * II]
                            - s->coordinatae[w * II])) > 1.0e-3
                    || fabs((f64)(s->coordinatae[v * II + I]
                            - s->coordinatae[w * II + I])) > 1.0e-3))
            {
                differt = VERUM;
            }
        }
        suturae += (i32)(primus && differt);
    }
    redde suturae;
}

/* extensio coordinatae 'axis' (0 = s, I = t) per vertices quorum
 * normalis x maior quam 0.9 est (facies +x) */
interior f64
_extensio_faciei (
    constans Statua* s,
                i32  axis)
{
    f64 minimum = 1.0e9;
    f64 maximum = -1.0e9;
    i32 v;

    per (v = ZEPHYRUM; v < s->verticum; v++)
    {
        f64 x = (f64)s->coordinatae[v * II + axis];

        si (s->normales[v * III] < 0.9f)
        {
            perge;
        }
        si (x < minimum)
        {
            minimum = x;
        }
        si (x > maximum)
        {
            maximum = x;
        }
    }
    redde maximum - minimum;
}


/* ==================================================
 * Generare
 * ================================================== */

interior vacuum
_sectio_genera (
    CredoContextus* c)
{
    hic_manens constans character* constans NOMINA[] = {
        "cortex", "tabulae", "saxum", "lateres", "stramen", "frondes",
        "muscus", "humus", "granum"
    };
    i32 g;

    (vacuum)c;
    CREDO_AEQUALIS_I32 (PLASTICA_TEXTURA_GENERA, IX);
    CREDO_AEQUALIS_I32 (PLASTICA_TEXELLA_PER_UNITATEM, XVI);
    per (g = ZEPHYRUM; g < PLASTICA_TEXTURA_GENERA; g++)
    {
        CREDO_NECESSE (plastica_textura_genus(g) != NIHIL);
        CREDO_VERUM (strcmp(plastica_textura_genus(g), NOMINA[g])
            == ZEPHYRUM);
    }
    CREDO_VERUM (plastica_textura_genus(PLASTICA_TEXTURA_GENERA)
        == NIHIL);
}

interior vacuum
_sectio_argumenta (
    CredoContextus* c)
{
    Imago imago;
       i8 signum = VII;

    imago.pixela    = &signum;
    imago.latitudo  = 99;
    imago.altitudo  = 98;
    CREDO_FALSUM (_genera(c, "marmor", VIII, VIII, ZEPHYRUM, &imago));
    CREDO_FALSUM (_genera(c, "", VIII, VIII, ZEPHYRUM, &imago));
    CREDO_FALSUM (_genera(c, "saxum", ZEPHYRUM, VIII, ZEPHYRUM,
        &imago));
    CREDO_FALSUM (_genera(c, "saxum", VIII, ZEPHYRUM, ZEPHYRUM,
        &imago));
    CREDO_FALSUM (_genera(c, "saxum", CCLVI + I, VIII, ZEPHYRUM,
        &imago));
    CREDO_FALSUM (_genera(c, "saxum", VIII, CCLVI + I, ZEPHYRUM,
        &imago));
    CREDO_FALSUM (plastica_textura_generare(
        chorda_ex_literis("saxum", c->piscina), VIII, VIII, ZEPHYRUM,
        NIHIL, &imago, c->piscina));
    CREDO_FALSUM (plastica_textura_generare(
        chorda_ex_literis("saxum", c->piscina), VIII, VIII, ZEPHYRUM,
        COLORES, NIHIL, c->piscina));
    CREDO_FALSUM (plastica_textura_generare(
        chorda_ex_literis("saxum", c->piscina), VIII, VIII, ZEPHYRUM,
        COLORES, &imago, NIHIL));
    /* exitus non tactus */
    CREDO_VERUM (imago.pixela == &signum);
    CREDO_AEQUALIS_I32 (imago.latitudo, 99);
    CREDO_AEQUALIS_I32 (imago.altitudo, 98);
    /* fines ipsi licent */
    CREDO_VERUM (_genera(c, "saxum", I, I, ZEPHYRUM, &imago));
    CREDO_AEQUALIS_I32 (imago.latitudo, I);
    CREDO_VERUM (_genera(c, "saxum", CCLVI, XVI, ZEPHYRUM, &imago));
    CREDO_AEQUALIS_I32 (imago.latitudo, CCLVI);
    CREDO_AEQUALIS_I32 (imago.altitudo, XVI);
}

/* omne pixelum unus ex tribus, opacum; saltem duo colores
 * adhibentur */
interior vacuum
_sectio_colores_tres (
    CredoContextus* c)
{
    hic_manens constans i32 MAGNITUDINES[][II] = {
        { XXXII, XXXII }, { XVI, VIII }, { XLV, XIX }, { III, V }
    };
    i32 g;
    i32 m;
    i32 k;

    per (g = ZEPHYRUM; g < PLASTICA_TEXTURA_GENERA; g++)
    {
        per (m = ZEPHYRUM; m < IV; m++)
        {
            Imago imago;
              i32 alieni = ZEPHYRUM;
              i32 numeri[III];

            memset(numeri, ZEPHYRUM, magnitudo(numeri));
            CREDO_NECESSE (_genera(c, plastica_textura_genus(g),
                MAGNITUDINES[m][ZEPHYRUM], MAGNITUDINES[m][I], V,
                &imago));
            CREDO_AEQUALIS_I32 (imago.latitudo,
                MAGNITUDINES[m][ZEPHYRUM]);
            CREDO_AEQUALIS_I32 (imago.altitudo, MAGNITUDINES[m][I]);
            per (k = ZEPHYRUM; k < imago.latitudo * imago.altitudo;
                k++)
            {
                s32 index =
                    _index_coloris(imago.pixela + k * IV, COLORES);

                si (index < ZEPHYRUM)
                {
                    alieni++;
                }
                alioquin
                {
                    numeri[index]++;
                }
            }
            CREDO_AEQUALIS_I32 (alieni, ZEPHYRUM);
            si (m == ZEPHYRUM)
            {
                CREDO_VERUM ((i32)(numeri[ZEPHYRUM] > ZEPHYRUM)
                    + (i32)(numeri[I] > ZEPHYRUM)
                    + (i32)(numeri[II] > ZEPHYRUM) >= II);
            }
        }
    }
}

interior vacuum
_sectio_determinata (
    CredoContextus* c)
{
    i32 g;

    per (g = ZEPHYRUM; g < PLASTICA_TEXTURA_GENERA; g++)
    {
        constans character* genus = plastica_textura_genus(g);
                     Imago  a;
                     Imago  b;
                     Imago  alia;

        CREDO_NECESSE (_genera(c, genus, LATUS, LATUS, III, &a));
        CREDO_NECESSE (_genera(c, genus, LATUS, LATUS, III, &b));
        CREDO_NECESSE (_genera(c, genus, LATUS, LATUS, IV, &alia));
        CREDO_VERUM (a.pixela != b.pixela);
        CREDO_VERUM (memcmp(a.pixela, b.pixela, LATUS * LATUS * IV)
            == ZEPHYRUM);
        CREDO_VERUM (memcmp(a.pixela, alia.pixela, LATUS * LATUS * IV)
            != ZEPHYRUM);
    }
}

/* digesta fixa: generator integer est, ergo eadem ubique. Mutatio
 * generis cuiusvis omnes res quae eo utuntur mutat - consulto fiat */
interior vacuum
_sectio_digesta (
    CredoContextus* c)
{
    hic_manens constans i32 DIGESTA[PLASTICA_TEXTURA_GENERA] = {
        0x25374b55u, 0xccc2b615u, 0x882739d3u, 0x5477e1a1u,
        0xe6410187u, 0x0891b5efu, 0x6f03cd9bu, 0xbbcffeebu,
        0xa0eb62ddu
    };
    i32 g;

    per (g = ZEPHYRUM; g < PLASTICA_TEXTURA_GENERA; g++)
    {
        Imago imago;

        CREDO_NECESSE (_genera(c, plastica_textura_genus(g), LATUS,
            LATUS, III, &imago));
        si (_digestum(&imago) != DIGESTA[g])
        {
            printf("digestum %s: 0x%08xu\n",
                plastica_textura_genus(g),
                (insignatus)_digestum(&imago));
        }
        CREDO_AEQUALIS_I32 (_digestum(&imago), DIGESTA[g]);
    }
}


/* ==================================================
 * In descriptione
 * ================================================== */

interior vacuum
_sectio_capsa (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">"
        "<capsa magnitudo=\"2 1 2\" textura=\"saxum\" semen=\"3\""
        " colores=\"#0a141e #646e78 #c8d2dc\"/>"
        "</res>");
    Imago speratum;
      i32 omnia;
      i32 v;

    CREDO_AEQUALIS_I32 (s->texturarum, I);
    CREDO_NECESSE (s->texturae != NIHIL);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].latitudo, LATUS);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].altitudo, LATUS);
    /* eadem pixela quae generator dat */
    CREDO_NECESSE (_genera(c, "saxum", LATUS, LATUS, III, &speratum));
    CREDO_VERUM (memcmp(s->texturae[ZEPHYRUM].pixela, speratum.pixela,
        LATUS * LATUS * IV) == ZEPHYRUM);
    CREDO_AEQUALIS_S32 (s->materiae[ZEPHYRUM].textura, ZEPHYRUM);
    CREDO_VERUM (s->materiae[ZEPHYRUM].repetit_s);
    CREDO_VERUM (s->materiae[ZEPHYRUM].repetit_t);
    /* textura colorem fert: factor albus */
    CREDO_F64_PROXIMUS ((f64)s->materiae[ZEPHYRUM].color[ZEPHYRUM],
        1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->materiae[ZEPHYRUM].color[I], 1.0,
        1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->materiae[ZEPHYRUM].color[II], 1.0,
        1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->materiae[ZEPHYRUM].color[III], 1.0,
        1.0e-6);
    CREDO_AEQUALIS_I32 (_latera_extenta(s, &omnia), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (omnia, XXXVI);
    /* facies +x: II unitates latae, I alta = XXXII x XVI texella */
    CREDO_F64_PROXIMUS (_extensio_faciei(s, ZEPHYRUM), 1.0, 1.0e-4);
    CREDO_F64_PROXIMUS (_extensio_faciei(s, I), 0.5, 1.0e-4);
    /* summum texturae sursum: vertex altior t minus habet */
    per (v = ZEPHYRUM; v < s->verticum; v++)
    {
        si (s->normales[v * III] > 0.9f)
        {
            CREDO_F64_PROXIMUS ((f64)s->coordinatae[v * II + I],
                -(f64)s->positiones[v * III + I] * 0.5, 1.0e-4);
        }
    }
}

/* texella quadrata in omni solido, etiam reflexo et verso */
interior vacuum
_sectio_densitas (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">"
        "<speculum axis=\"x\">"
        "<grex locus=\"3 0 1\" gyrus=\"30\">"
        "<capsa magnitudo=\"1 2 3\" textura=\"tabulae\"/>"
        "<cylindrus radius=\"0.4\" radius_summus=\"0.1\""
        " altitudo=\"2\" latera=\"7\" textura=\"cortex\"/>"
        "<cylindrus radius=\"0.5\" radius_summus=\"0\""
        " altitudo=\"1\" textura=\"stramen\"/>"
        "<sphaera magnitudo=\"2 1 3\" segmenta=\"8\""
        " asperitas=\"0.4\" semen=\"7\" textura=\"muscus\"/>"
        "<cuneus magnitudo=\"2 1 3\" rotatio=\"10 20 30\""
        " textura=\"stramen\"/>"
        "<extrusio puncta=\"0,0 2,0 2,1 1,1 1,2 0,2\""
        " altitudo=\"0.7\" textura=\"lateres\"/>"
        "<tornus puncta=\"0,0 0.5,0 0.6,0.4 0.4,0.5 0.3,0.2 0,0.2\""
        " latera=\"9\" textura=\"humus\"/>"
        "</grex>"
        "</speculum>"
        "</res>");
    i32 omnia;

    CREDO_AEQUALIS_I32 (_latera_extenta(s, &omnia), ZEPHYRUM);
    CREDO_VERUM (omnia > D);
    CREDO_AEQUALIS_I32 (omnia, s->indicum);
}

/* ambitus continuus: sutura una (duo loci: imus et summus) */
interior vacuum
_sectio_ambitus (
    CredoContextus* c)
{
    Statua* cylindrus = _statua(c,
        "<res titulus=\"a\">"
        "<cylindrus radius=\"0.5\" altitudo=\"1\" latera=\"6\""
        " textura=\"cortex\"/>"
        "</res>");
    Statua* tornus = _statua(c,
        "<res titulus=\"a\">"
        "<tornus puncta=\"0.5,0 0.5,1 0.5,2.5\" latera=\"6\""
        " textura=\"cortex\"/>"
        "</res>");
    Statua* situla = _statua(c,
        "<res titulus=\"a\">"
        "<tornus puncta=\"0.5,0 0.5,1 0.4,1 0.4,0\" latera=\"6\""
        " textura=\"cortex\"/>"
        "</res>");
    Statua* extrusio = _statua(c,
        "<res titulus=\"a\">"
        "<extrusio puncta=\"0,0 1.3,0 1.3,1 0,1\" altitudo=\"1\""
        " textura=\"lateres\"/>"
        "</res>");
    i32 omnia;

    CREDO_AEQUALIS_I32 (_latera_extenta(cylindrus, &omnia), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_suturae(cylindrus), II);
    CREDO_AEQUALIS_I32 (_latera_extenta(tornus, &omnia), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_suturae(tornus), III);
    /* paries exterior et interior: sutura una uterque */
    CREDO_AEQUALIS_I32 (_latera_extenta(situla, &omnia), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_suturae(situla), IV);
    CREDO_AEQUALIS_I32 (_latera_extenta(extrusio, &omnia), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_suturae(extrusio), II);
}

interior vacuum
_sectio_sine_textura (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">"
        "<capsa magnitudo=\"1 1 1\" color=\"#804020\"/>"
        "</res>");
    i32 v;

    CREDO_AEQUALIS_I32 (s->texturarum, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (s->materiae[ZEPHYRUM].textura, -I);
    CREDO_F64_PROXIMUS ((f64)s->materiae[ZEPHYRUM].color[ZEPHYRUM],
        128.0 / 255.0, 1.0e-6);
    per (v = ZEPHYRUM; v < s->verticum * II; v++)
    {
        CREDO_F64_PROXIMUS ((f64)s->coordinatae[v], 0.0, 1.0e-9);
    }
}

interior vacuum
_sectio_colores_derivati (
    CredoContextus* c)
{
    hic_manens constans i8 DERIVATI[IX] = {
        96, 48, 24,   128, 64, 32,   159, 111, 87
    };
    hic_manens constans i8 DATI[IX] = {
        0x11, 0x11, 0x11,   0x22, 0x22, 0x22,   0x33, 0x44, 0x55
    };
    Statua* s = _statua(c,
        "<res titulus=\"a\">"
        "<capsa magnitudo=\"1 1 1\" color=\"#804020\""
        " textura=\"granum\"/>"
        "<capsa magnitudo=\"1 1 1\" color=\"#804020\""
        " textura=\"granum\" colores=\"#111 #222 #345\"/>"
        "<capsa magnitudo=\"1 1 1\" textura=\"granum\""
        " invisibilis=\"verum\"/>"
        "</res>");
    i32 k;

    CREDO_AEQUALIS_I32 (s->texturarum, III);
    per (k = ZEPHYRUM; k < LATUS * LATUS; k++)
    {
        CREDO_VERUM (_index_coloris(
            s->texturae[ZEPHYRUM].pixela + k * IV, DERIVATI)
            >= ZEPHYRUM);
        CREDO_VERUM (_index_coloris(s->texturae[I].pixela + k * IV,
            DATI) >= ZEPHYRUM);
    }
    /* color absens: albus medius */
    CREDO_AEQUALIS_S32 (s->materiae[II].textura, II);
    /* invisibilis cum textura: alpha 0 manet */
    CREDO_F64_PROXIMUS ((f64)s->materiae[II].color[III], 0.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)s->materiae[II].limen_alpha, 0.5,
        1.0e-9);
}

interior vacuum
_sectio_communicatio (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">"
        "<speculum axis=\"z\">"
        "<series numerus=\"5\" gressus=\"1 0 0\">"
        "<capsa magnitudo=\"1 1 1\" color=\"#888\" textura=\"saxum\""
        " locus=\"0 0 2\"/>"
        "</series>"
        "</speculum>"
        "<capsa magnitudo=\"1 1 1\" color=\"#888\" textura=\"saxum\""
        " semen=\"1\"/>"
        "<capsa magnitudo=\"1 1 1\" color=\"#889\" textura=\"saxum\"/>"
        "<capsa magnitudo=\"1 1 1\" color=\"#888\" textura=\"humus\"/>"
        "<sphaera radius=\"1\" color=\"#888\" textura=\"saxum\"/>"
        "</res>");
    i32 k;

    /* X capsae seriei reflexae et sphaera texturam 0 communicant */
    CREDO_AEQUALIS_I32 (s->texturarum, IV);
    CREDO_AEQUALIS_I32 (s->materiarum, XIV);
    per (k = ZEPHYRUM; k < X; k++)
    {
        CREDO_AEQUALIS_S32 (s->materiae[k].textura, ZEPHYRUM);
    }
    CREDO_AEQUALIS_S32 (s->materiae[X].textura, I);
    CREDO_AEQUALIS_S32 (s->materiae[XI].textura, II);
    CREDO_AEQUALIS_S32 (s->materiae[XII].textura, III);
    CREDO_AEQUALIS_S32 (s->materiae[XIII].textura, ZEPHYRUM);
}

interior vacuum
_sectio_manu (
    CredoContextus* c)
{
    hic_manens constans i8 SPERATA[VIII][III] = {
        { 0, 0, 0 }, { 0, 0, 0 }, { 255, 255, 255 }, { 255, 0, 0 },
        { 255, 255, 255 }, { 255, 255, 255 }, { 0, 0, 0 }, { 0, 0, 0 }
    };
    Statua* s = _statua(c,
        "<res titulus=\"a\">\n"
        "  <textura titulus=\"fenestra\" latitudo=\"4\"\n"
        "           colores=\"#000 #fff #f00\">\n"
        "    aabc\n"
        "    bbaa\n"
        "  </textura>\n"
        "  <capsa magnitudo=\"2 1 2\" textura=\"fenestra\"/>\n"
        "  <capsa magnitudo=\"1 1 1\" textura=\"fenestra\"/>\n"
        "</res>");
    constans Imago* textura;
               i32  omnia;
               i32  k;

    CREDO_AEQUALIS_I32 (s->texturarum, I);
    textura = &s->texturae[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (textura->latitudo, IV);
    CREDO_AEQUALIS_I32 (textura->altitudo, II);
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        CREDO_VERUM (memcmp(textura->pixela + k * IV, SPERATA[k], III)
            == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (textura->pixela[k * IV + III], 255);
    }
    CREDO_AEQUALIS_S32 (s->materiae[ZEPHYRUM].textura, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (s->materiae[I].textura, ZEPHYRUM);
    /* densitas eadem: facies II unitatum VIII tegulas lata est */
    CREDO_AEQUALIS_I32 (_latera_extenta(s, &omnia), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (omnia, LXXII);
    /* textura nodum non facit: radix et duae capsae */
    CREDO_AEQUALIS_I32 (s->nodorum, III);
    /* XVI x XVI licet */
    s = _statua(c,
        "<res titulus=\"a\">"
        "<textura titulus=\"t\" latitudo=\"16\" colores=\"#123\">"
        "aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa"
        " aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa"
        " aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa"
        " aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa"
        " aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaa"
        " aaaaaaaaaaaaaaaa"
        "</textura>"
        "<capsa magnitudo=\"1 1 1\" textura=\"t\"/>"
        "</res>");
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].latitudo, XVI);
    CREDO_AEQUALIS_I32 (s->texturae[ZEPHYRUM].altitudo, XVI);
}

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    RECUSATUR ("<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" textura=\"marmor\"/>\n</res>",
        II, "marmor");
    RECUSATUR ("<res titulus=\"a\">\n\n"
        "<capsa magnitudo=\"1 1 1\" textura=\"saxum\""
        " colores=\"#111 #222\"/>\n</res>", III, "colores");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" textura=\"saxum\""
        " colores=\"#111 #222 #333 #444\"/>\n</res>", II, "colores");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" textura=\"saxum\""
        " colores=\"#111 #222 rubrum\"/>\n</res>", II, "colores");
    /* colores sine textura */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" colores=\"#111 #222 #333\"/>\n"
        "</res>", II, "colores");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" textura=\"saxum\""
        " semen=\"-1\"/>\n</res>", II, "semen");
    /* manu scripta: nimis lata, nimis alta */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"17\" colores=\"#111\">"
        "aaaaaaaaaaaaaaaaa</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "latitudo");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a a a a a a a a a a a a a a a a a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "16");
    /* littera extra colores; signum alienum */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"2\" colores=\"#111 #222\">"
        "abac</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "'c'");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"2\" colores=\"#111 #222\">"
        "ab.a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "'.'");
    /* ordo imperfectus; vacua */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"2\" colores=\"#111 #222\">"
        "aba</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "rows");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"2\" colores=\"#111 #222\">"
        " </textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "rows");
    /* titulus: absens, generis geniti, iteratus */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura latitudo=\"1\" colores=\"#111\">a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "titulus");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"saxum\" latitudo=\"1\" colores=\"#111\">"
        "a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "saxum");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a</textura>\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", III, "twice");
    /* colores: absentes, nimis multi */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\">a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "colores");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#1 #2\">"
        "a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "colores");
    /* non liber <res>; usus ante definitionem; liberum alienum;
     * attributum ignotum */
    RECUSATUR ("<res titulus=\"a\">\n<grex>\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</grex>\n</res>", III,
        "<res>");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" textura=\"t\"/>\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a</textura>\n</res>", II, "'t'");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a<capsa magnitudo=\"1 1 1\"/></textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "children");
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\""
        " altitudo=\"1\">a</textura>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</res>", II, "altitudo");
    /* colores cum textura manu scripta */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a</textura>\n"
        "<capsa magnitudo=\"1 1 1\" textura=\"t\""
        " colores=\"#111 #222 #333\"/>\n</res>", III, "colores");
    /* textura sola rem non facit */
    RECUSATUR ("<res titulus=\"a\">\n"
        "<textura titulus=\"t\" latitudo=\"1\" colores=\"#111\">"
        "a</textura>\n</res>", I, "no solids");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "genera", _sectio_genera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "argumenta", _sectio_argumenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colores_tres", _sectio_colores_tres, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "determinata", _sectio_determinata, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "digesta", _sectio_digesta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capsa", _sectio_capsa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "densitas", _sectio_densitas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_textura", _sectio_sine_textura, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "colores_derivati", _sectio_colores_derivati, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "communicatio", _sectio_communicatio, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "manu", _sectio_manu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica_textura", SECTIONES);
}
