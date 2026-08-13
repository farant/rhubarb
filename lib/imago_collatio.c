/*
 * imago_collatio.c - Collatio imaginum
 *
 * Nihil legit, nihil scribit, nihil de platforma scit. Sola
 * computatio: pixela intrant, numeri exeunt.
 *
 * LENIMENTUM: algorithmus Kotsarenko & Ramos ("Measuring perceived
 * color difference"), idem quo 'pixelmatch' utitur. Ratio eius:
 * pixelum leniatum in ora iacet, ergo (a) claritate a vicinis suis
 * in UTRAMQUE partem differt - alium clariorem, alium obscuriorem
 * habet - et (b) vicinus ille extremus in campo plano iacet, id est
 * multa cognata paria habet. Pixelum quod haec duo praestat ora est,
 * non mutatio.
 */

#include "imago_collatio.h"

/* Pondera claritatis (Y ex YIQ) */
#define CLARITAS_R 0.29889531
#define CLARITAS_G 0.58662247
#define CLARITAS_B 0.11448223

/* Quantum fundi in pictura supersit (cetera in album fugit) */
#define PALLOR 0.1

/* ============================================================
 * Auxilia interna
 * ============================================================ */

interior constans i8*
_pixelum (constans Imago* im, i32 x, i32 y)
{
    redde im->pixela + (y * im->latitudo + x) * (i32)IV;
}

/*
 * _claritas - Claritas pixeli (Y), alpha in album miscente
 */
interior duplex
_claritas (constans i8* p)
{
    duplex a;
    duplex r, g, b;

    a = (duplex)p[III] / 255.0;
    r = 255.0 + ((duplex)p[0] - 255.0) * a;
    g = 255.0 + ((duplex)p[I] - 255.0) * a;
    b = 255.0 + ((duplex)p[II] - 255.0) * a;

    redde r * CLARITAS_R + g * CLARITAS_G + b * CLARITAS_B;
}

/*
 * _delta - Maxima differentia absoluta inter canales
 */
interior s32
_delta (constans i8* a, constans i8* b)
{
    s32 maximum;
    s32 i;

    maximum = 0;
    per (i = 0; i < IV; i++)
    {
        s32 d;

        d = (s32)a[i] - (s32)b[i];
        si (d < 0)
        {
            d = -d;
        }
        si (d > maximum)
        {
            maximum = d;
        }
    }
    redde maximum;
}

/*
 * _multa_cognata - Habetne pixelum III aut plura vicina IDENTICA?
 *
 * Ora imaginis pro cognato uno numeratur (pixelum in ora minus
 * vicinos habet, ergo aliter iniuste iudicaretur).
 */
interior b32
_multa_cognata (constans Imago* im, s32 x, s32 y)
{
    s32 x0, y0, x2, y2;
    s32 nx, ny;
    s32 cognata;
    constans i8* centrum;

    x0 = x - I;
    y0 = y - I;
    x2 = x + I;
    y2 = y + I;
    si (x0 < 0)                        { x0 = 0; }
    si (y0 < 0)                        { y0 = 0; }
    si (x2 > (s32)im->latitudo - I)    { x2 = (s32)im->latitudo - I; }
    si (y2 > (s32)im->altitudo - I)    { y2 = (s32)im->altitudo - I; }

    cognata = (x == x0 || x == x2 || y == y0 || y == y2) ? I : 0;
    centrum = _pixelum(im, (i32)x, (i32)y);

    per (ny = y0; ny <= y2; ny++)
    {
        per (nx = x0; nx <= x2; nx++)
        {
            constans i8* alius;

            si (nx == x && ny == y)
            {
                perge;
            }

            alius = _pixelum(im, (i32)nx, (i32)ny);
            si (alius[0]   == centrum[0]   &&
                alius[I]   == centrum[I]   &&
                alius[II]  == centrum[II]  &&
                alius[III] == centrum[III])
            {
                cognata++;
                si (cognata > II)
                {
                    redde VERUM;
                }
            }
        }
    }

    redde FALSUM;
}

/*
 * _leniatum - Estne pixelum ora leniata, non mutatio vera?
 *
 * @im:    imago in qua quaeritur
 * @alter: imago altera (cognatio in AMBABUS probanda est - aliter
 *         mutatio vera quae casu ori similis est spernetur)
 */
interior b32
_leniatum (constans Imago* im, constans Imago* alter, s32 x, s32 y)
{
    s32 x0, y0, x2, y2;
    s32 nx, ny;
    s32 zephyra;
    s32 min_x, min_y, max_x, max_y;
    duplex minimum, maximum;
    duplex claritas_centri;

    x0 = x - I;
    y0 = y - I;
    x2 = x + I;
    y2 = y + I;
    si (x0 < 0)                        { x0 = 0; }
    si (y0 < 0)                        { y0 = 0; }
    si (x2 > (s32)im->latitudo - I)    { x2 = (s32)im->latitudo - I; }
    si (y2 > (s32)im->altitudo - I)    { y2 = (s32)im->altitudo - I; }

    zephyra = (x == x0 || x == x2 || y == y0 || y == y2) ? I : 0;
    minimum = 0.0;
    maximum = 0.0;
    min_x = x;
    min_y = y;
    max_x = x;
    max_y = y;
    claritas_centri = _claritas(_pixelum(im, (i32)x, (i32)y));

    per (ny = y0; ny <= y2; ny++)
    {
        per (nx = x0; nx <= x2; nx++)
        {
            duplex differentia;

            si (nx == x && ny == y)
            {
                perge;
            }

            differentia = claritas_centri
                        - _claritas(_pixelum(im, (i32)nx, (i32)ny));

            si (differentia == 0.0)
            {
                zephyra++;
                si (zephyra > II)
                {
                    /* Campus planus: ora non est */
                    redde FALSUM;
                }
            }
            alioquin si (differentia < minimum)
            {
                minimum = differentia;
                min_x = nx;
                min_y = ny;
            }
            alioquin si (differentia > maximum)
            {
                maximum = differentia;
                max_x = nx;
                max_y = ny;
            }
        }
    }

    /* Sine vicino clariore ET obscuriore ora esse non potest */
    si (minimum == 0.0 || maximum == 0.0)
    {
        redde FALSUM;
    }

    redde (_multa_cognata(im, min_x, min_y) &&
           _multa_cognata(alter, min_x, min_y))
       || (_multa_cognata(im, max_x, max_y) &&
           _multa_cognata(alter, max_x, max_y));
}

/*
 * _canum_pingere - Fundum pallidum ex pixelo primo
 */
interior vacuum
_canum_pingere (i8* destinatio, constans i8* fons)
{
    duplex valor;
    i8     canities;

    valor = 255.0 + (_claritas(fons) - 255.0) * PALLOR;
    si (valor < 0.0)   { valor = 0.0; }
    si (valor > 255.0) { valor = 255.0; }

    canities = (i8)valor;
    destinatio[0]   = canities;
    destinatio[I]   = canities;
    destinatio[II]  = canities;
    destinatio[III] = (i8)CCLV;
}

/* ============================================================
 * Functiones Publicae
 * ============================================================ */

CollatioRegula
collatio_regula_solita (vacuum)
{
    CollatioRegula regula;

    regula.tolerantia        = ZEPHYRUM;
    regula.sperne_lenimentum = VERUM;
    regula.pixela_ferenda    = ZEPHYRUM;
    regula.picturam_facere   = FALSUM;

    redde regula;
}

CollatioFructus
imago_conferre (constans Imago* primus,
                constans Imago* secundus,
                CollatioRegula  regula,
                Piscina*        piscina)
{
    CollatioFructus fructus;
    i32 x, y;
    b32 arca_inventa;
    i32 min_x, min_y, max_x, max_y;

    /* Structura campatim impletur - nulla memoria purgata */
    fructus.sententia       = COLLATIO_ARGUMENTA_INVALIDA;
    fructus.pixela_tota     = ZEPHYRUM;
    fructus.pixela_diversa  = ZEPHYRUM;
    fructus.pixela_leniata  = ZEPHYRUM;
    fructus.delta_maximum   = ZEPHYRUM;
    fructus.arca_x          = ZEPHYRUM;
    fructus.arca_y          = ZEPHYRUM;
    fructus.arca_latitudo   = ZEPHYRUM;
    fructus.arca_altitudo   = ZEPHYRUM;
    fructus.pictura_praesto = FALSUM;
    fructus.pictura.pixela  = NIHIL;
    fructus.pictura.latitudo = ZEPHYRUM;
    fructus.pictura.altitudo = ZEPHYRUM;

    si (primus == NIHIL || secundus == NIHIL ||
        primus->pixela == NIHIL || secundus->pixela == NIHIL)
    {
        redde fructus;
    }

    si (primus->latitudo == ZEPHYRUM || primus->altitudo == ZEPHYRUM)
    {
        redde fructus;
    }

    si (primus->latitudo != secundus->latitudo ||
        primus->altitudo != secundus->altitudo)
    {
        fructus.sententia = COLLATIO_MENSURAE_DISCREPANT;
        redde fructus;
    }

    fructus.pixela_tota = primus->latitudo * primus->altitudo;

    si (regula.picturam_facere && piscina != NIHIL)
    {
        i8* tela;

        tela = (i8*)piscina_allocare(
                   piscina,
                   (memoriae_index)(fructus.pixela_tota * (i32)IV));
        si (tela != NIHIL)
        {
            fructus.pictura.pixela   = tela;
            fructus.pictura.latitudo = primus->latitudo;
            fructus.pictura.altitudo = primus->altitudo;
            fructus.pictura_praesto  = VERUM;
        }
    }

    arca_inventa = FALSUM;
    min_x = ZEPHYRUM;
    min_y = ZEPHYRUM;
    max_x = ZEPHYRUM;
    max_y = ZEPHYRUM;

    per (y = ZEPHYRUM; y < primus->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < primus->latitudo; x++)
        {
            constans i8* pa;
            constans i8* pb;
            s32 delta;
            b32 diversum;
            b32 leniatum;

            pa    = _pixelum(primus, x, y);
            pb    = _pixelum(secundus, x, y);
            delta = _delta(pa, pb);

            si (delta > (s32)fructus.delta_maximum)
            {
                fructus.delta_maximum = (i32)delta;
            }

            diversum = (delta > (s32)regula.tolerantia) ? VERUM : FALSUM;
            leniatum = FALSUM;

            si (diversum && regula.sperne_lenimentum)
            {
                si (_leniatum(primus, secundus, (s32)x, (s32)y) ||
                    _leniatum(secundus, primus, (s32)x, (s32)y))
                {
                    leniatum = VERUM;
                    diversum = FALSUM;
                    fructus.pixela_leniata++;
                }
            }

            si (diversum)
            {
                fructus.pixela_diversa++;

                si (!arca_inventa)
                {
                    min_x = x;
                    max_x = x;
                    min_y = y;
                    max_y = y;
                    arca_inventa = VERUM;
                }
                alioquin
                {
                    si (x < min_x) { min_x = x; }
                    si (x > max_x) { max_x = x; }
                    si (y < min_y) { min_y = y; }
                    si (y > max_y) { max_y = y; }
                }
            }

            si (fructus.pictura_praesto)
            {
                i8* pd;

                pd = fructus.pictura.pixela
                   + (y * fructus.pictura.latitudo + x) * (i32)IV;

                si (diversum)
                {
                    pd[0]   = (i8)CCLV;
                    pd[I]   = (i8)ZEPHYRUM;
                    pd[II]  = (i8)ZEPHYRUM;
                    pd[III] = (i8)CCLV;
                }
                alioquin si (leniatum)
                {
                    pd[0]   = (i8)CCLV;
                    pd[I]   = (i8)CCLV;
                    pd[II]  = (i8)ZEPHYRUM;
                    pd[III] = (i8)CCLV;
                }
                alioquin
                {
                    _canum_pingere(pd, pa);
                }
            }
        }
    }

    si (arca_inventa)
    {
        fructus.arca_x        = min_x;
        fructus.arca_y        = min_y;
        fructus.arca_latitudo = max_x - min_x + (i32)I;
        fructus.arca_altitudo = max_y - min_y + (i32)I;
    }

    /* Limes sententiam solam movet: pixela_diversa, arca, delta
     * INTACTA manent. Mensura numquam mentitur quia iudicium
     * mitius est - aliter limes altior probationem caecam faceret
     * pro sana. */
    fructus.sententia = (fructus.pixela_diversa > regula.pixela_ferenda)
                      ? COLLATIO_DISCREPANT
                      : COLLATIO_CONGRUUNT;

    redde fructus;
}
