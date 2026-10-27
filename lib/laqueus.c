/* laqueus.c - Laquei polygonales exacti: diagrammata et invariantes
 * Geometria tota per situs (exacta). Diagramma: omnia paria segmentorum
 * (contigua per situs_transitus_vicinus, cetera per
 * situs_transitus_parametri); transitus per segmentum ordine parametri,
 * percursus laquei = codex Gauss. Uncinus per summam statuum super
 * codicem PD: positivus ora[u_in, o_out, u_out, o_in], negativus
 * ora[u_in, o_in, u_out, o_out] (contra horologium, spectator ad +v); A
 * iungit (X0 X1)(X2 X3), B (X0 X3)(X1 X2). Alexander per calculum Fox
 * praesentationis Wirtinger: positivus x_b = x_o x_a x_o^-1 -> (1 - t,
 * t, -1); negativus x_b = x_o^-1 x_a x_o -> (1 - t^-1, t^-1, -1). Vide
 * lib/laqueus.worklog.md. */
#include "laqueus.h"
#include "chorda_aedificator.h"
#include "anulus.h"
#include "matrix.h"
#include "congruentia.h"
#include "interpolatio.h"
#include <string.h>


/* ==================================================
 * Auxilia topologiae
 * ================================================== */

interior i32
_componens (
    Laqueus l,
        i32 i)
{
    i32 k = ZEPHYRUM;

    dum (k + I < l.componentes && l.initia[k + I] <= i)
    {
        k++;
    }
    redde k;
}

interior i32
_sequens (
    Laqueus l,
        i32 i)
{
    i32 k = _componens(l, i);

    redde (i + I == l.initia[k + I]) ? l.initia[k] : i + I;
}

interior i32
_prior (
    Laqueus l,
        i32 i)
{
    i32 k = _componens(l, i);

    redde (i == l.initia[k]) ? l.initia[k + I] - I : i - I;
}

/* tria puncta collinearia (proiectiones in tria plana coordinatarum
 * omnes collineares) */
interior b32
_collinearia (
    Punctum  a,
    Punctum  b,
    Punctum  c,
    Piscina* piscina)
{
    PunctumPlani pa;
    PunctumPlani pb;
    PunctumPlani pc;

    pa.x = a.x; pa.y = a.y; pb.x = b.x; pb.y = b.y; pc.x = c.x; pc.y =
                                                                    c.y;
    si (situs_orientatio_plana(pa, pb, pc, piscina) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    pa.x = a.y; pa.y = a.z; pb.x = b.y; pb.y = b.z; pc.x = c.y; pc.y =
                                                                    c.z;
    si (situs_orientatio_plana(pa, pb, pc, piscina) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    pa.x = a.x; pa.y = a.z; pb.x = b.x; pb.y = b.z; pc.x = c.x; pc.y =
                                                                    c.z;
    redde situs_orientatio_plana(pa, pb, pc, piscina) == ZEPHYRUM;
}


/* ==================================================
 * Textus
 * ================================================== */

interior b32
_est_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior chorda
_sine_spatiis (
    chorda textus)
{
    i32 initium  = ZEPHYRUM;
    i32 finis    = textus.mensura;

    dum (initium < finis && _est_spatium(textus.datum[initium]))
    {
        initium++;
    }
    dum (finis > initium && _est_spatium(textus.datum[finis - I]))
    {
        finis--;
    }
    redde chorda_sectio(textus, initium, finis);
}

/* "(x, y, z)" sine spatiis extremis */
interior b32
_punctum_legere (
      chorda  textus,
     Piscina* piscina,
     Punctum* exitus)
{
        i32 commata[II];
        i32 numerus = ZEPHYRUM;
        i32 k;
    Punctum p;

    si (   textus.mensura < II || textus.datum[ZEPHYRUM] != '('
        || textus.datum[textus.mensura - I] != ')')
    {
        redde FALSUM;
    }
    per (k = I; k + I < textus.mensura; k++)
    {
        si (textus.datum[k] == ',')
        {
            si (numerus == II)
            {
                redde FALSUM;
            }
            commata[numerus++] = k;
        }
    }
    si (   numerus != II
        || !fractio_ex_chorda(_sine_spatiis(chorda_sectio(textus, I,
            commata[ZEPHYRUM])), piscina, &p.x)
        || !fractio_ex_chorda(_sine_spatiis(chorda_sectio(textus,
            commata[ZEPHYRUM] + I, commata[I])), piscina, &p.y)
        || !fractio_ex_chorda(_sine_spatiis(chorda_sectio(textus,
            commata[I] + I, textus.mensura - I)), piscina, &p.z))
    {
        redde FALSUM;
    }
    *exitus = p;
    redde VERUM;
}

b32
laqueus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Laqueus* exitus)
{
    Punctum* vertices;
        i32* initia;
        i32  capacitas    = ZEPHYRUM;
        i32  numerus      = ZEPHYRUM;
        i32  componentes  = ZEPHYRUM;
        i32  k;
        i32  initium_componentis = ZEPHYRUM;

    si (textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    textus = _sine_spatiis(textus);
    si (   textus.mensura < II || textus.datum[ZEPHYRUM] != '['
        || textus.datum[textus.mensura - I] != ']')
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == '(')
        {
            capacitas++;
        }
    }
    vertices = (Punctum*)piscina_allocare(piscina, (memoriae_index)(
        capacitas + I) * magnitudo(Punctum));
    initia = (i32*)piscina_allocare(piscina, (memoriae_index)(capacitas
        + II) * magnitudo(i32));
    initia[ZEPHYRUM] = ZEPHYRUM;

    k = I;
    dum (VERUM)
    {
        i32 initium;

        dum (k < textus.mensura - I && _est_spatium(textus.datum[k]))
        {
            k++;
        }
        si (textus.datum[k] != '(')
        {
            redde FALSUM;
        }
        initium = k;
        dum (k < textus.mensura - I && textus.datum[k] != ')')
        {
            k++;
        }
        si (   textus.datum[k] != ')'
            || !_punctum_legere(chorda_sectio(textus, initium, k + I),
                piscina, &vertices[numerus]))
        {
            redde FALSUM;
        }
        numerus++;
        k++;
        dum (k < textus.mensura - I && _est_spatium(textus.datum[k]))
        {
            k++;
        }
        si (textus.datum[k] == ',')
        {
            k++;
            perge;
        }
        si (textus.datum[k] == ';' || k == textus.mensura - I)
        {
            si (numerus - initium_componentis < III)
            {
                redde FALSUM;
            }
            componentes++;
            initia[componentes] = numerus;
            initium_componentis = numerus;
            si (k == textus.mensura - I)
            {
                frange;
            }
            k++;
            perge;
        }
        redde FALSUM;
    }
    exitus->vertices     = vertices;
    exitus->numerus      = numerus;
    exitus->initia       = initia;
    exitus->componentes  = componentes;
    redde VERUM;
}

b32
laqueus_ex_punctis (
    constans Punctum* puncta,
        constans i32* initia,
                 i32  componentes,
             Piscina* piscina,
             Laqueus* exitus)
{
    Punctum* vertices;
        i32* initia_nova;
        i32  numerus;
        i32  k;

    si (componentes == ZEPHYRUM || initia[ZEPHYRUM] != ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < componentes; k++)
    {
        si (initia[k + I] < initia[k] + III)
        {
            redde FALSUM;
        }
    }
    numerus      = initia[componentes];
    vertices     = (Punctum*)piscina_allocare(piscina, (memoriae_index)
        numerus * magnitudo(Punctum));
    initia_nova  = (i32*)piscina_allocare(piscina, (memoriae_index)(
        componentes + I) * magnitudo(i32));
    memcpy(vertices, puncta,
        (memoriae_index)numerus * magnitudo(Punctum));
    memcpy(initia_nova, initia, (memoriae_index)(componentes + I)
        * magnitudo(i32));
    exitus->vertices     = vertices;
    exitus->numerus      = numerus;
    exitus->initia       = initia_nova;
    exitus->componentes  = componentes;
    redde VERUM;
}

chorda
laqueus_ad_chordam (
     Laqueus  l,
     Piscina* piscina)
{
    ChordaAedificator* scriba = chorda_aedificator_creare(piscina,
        (memoriae_index)CXXVIII);
                   i32 k;
                   i32 i;

    (vacuum)chorda_aedificator_appendere_character(scriba, '[');
    per (k = ZEPHYRUM; k < l.componentes; k++)
    {
        si (k > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_literis(scriba, "; ");
        }
        per (i = l.initia[k]; i < l.initia[k + I]; i++)
        {
            si (i > l.initia[k])
            {
                (vacuum)chorda_aedificator_appendere_literis(scriba,
                    ", ");
            }
            (vacuum)chorda_aedificator_appendere_character(scriba, '(');
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                fractio_ad_chordam(l.vertices[i].x, piscina));
            (vacuum)chorda_aedificator_appendere_literis(scriba, ", ");
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                fractio_ad_chordam(l.vertices[i].y, piscina));
            (vacuum)chorda_aedificator_appendere_literis(scriba, ", ");
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                fractio_ad_chordam(l.vertices[i].z, piscina));
            (vacuum)chorda_aedificator_appendere_character(scriba, ')');
        }
    }
    (vacuum)chorda_aedificator_appendere_character(scriba, ']');
    redde chorda_aedificator_finire(scriba);
}

i32
laqueus_numerus (
    Laqueus l)
{
    redde l.numerus;
}

i32
laqueus_componentes (
    Laqueus l)
{
    redde l.componentes;
}

Punctum
laqueus_vertex (
    Laqueus l,
        i32 i)
{
    redde l.vertices[i];
}

b32
laqueus_simplex (
     Laqueus  l,
     Piscina* piscina)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        per (j = i + I; j < l.numerus; j++)
        {
            Punctum a = l.vertices[i];
            Punctum b = l.vertices[_sequens(l, i)];
            Punctum c = l.vertices[j];
            Punctum d = l.vertices[_sequens(l, j)];

            si (_sequens(l, i) == j)
            {
                si (situs_segmenta_vicina(a, c, d, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (_sequens(l, j) == i)
            {
                si (situs_segmenta_vicina(c, a, b, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (situs_segmenta(a, b, c, d, piscina)
                         != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Diagramma
 * ================================================== */

b32
laqueus_diagramma (
        Laqueus  l,
        Punctum  v,
        Piscina* piscina,
      Diagramma* exitus)
{
    Transitus* transitus;
          i32  numerus    = ZEPHYRUM;
          i32  capacitas  = l.numerus + I;
          i32  i;
          i32  j;
          i32* per_segmentum;    /* numerus eventuum per segmentum */
          i32* initia_eventuum;
          i32* eventus;          /* codices 2k / 2k+1 per segmentum */
          i32* percursus;
          i32* initia_percursus;
          i32  positus;

    transitus = (Transitus*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(Transitus));
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        per (j = i + I; j < l.numerus; j++)
        {
                   Punctum a = l.vertices[i];
                   Punctum b = l.vertices[_sequens(l, i)];
                   Punctum c = l.vertices[j];
                   Punctum d = l.vertices[_sequens(l, j)];
                       s32 superius;
                       s32 signum;
                   Fractio s;
                   Fractio t;
            SitusContactus r;

            si (_sequens(l, i) == j)
            {
                si (situs_transitus_vicinus(a, c, d, v, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
                perge;
            }
            si (_sequens(l, j) == i)
            {
                si (situs_transitus_vicinus(c, a, b, v, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
                perge;
            }
            r = situs_transitus_parametri(a, b, c, d, v, piscina,
                &superius, &signum, &s, &t);
            si (r == SITUS_DISIUNCTA)
            {
                perge;
            }
            si (r != SITUS_SECANT)
            {
                redde FALSUM;
            }
            si (numerus == capacitas)
            {
                memoriae_index mensura =
                    (memoriae_index)(capacitas * II)
                    * magnitudo(Transitus);
                Transitus* novi = (Transitus*)piscina_allocare(piscina,
                    mensura);

                memcpy(novi, transitus, (memoriae_index)numerus
                    * magnitudo(Transitus));
                transitus = novi;
                capacitas = capacitas * II;
            }
            si (superius == ZEPHYRUM)
            {
                transitus[numerus].supra             = i;
                transitus[numerus].parametrum_supra  = s;
                transitus[numerus].infra             = j;
                transitus[numerus].parametrum_infra  = t;
            }
            alioquin
            {
                transitus[numerus].supra             = j;
                transitus[numerus].parametrum_supra  = t;
                transitus[numerus].infra             = i;
                transitus[numerus].parametrum_infra  = s;
            }
            transitus[numerus].signum = signum;
            numerus++;
        }
    }

    /* eventus per segmentum, ordine parametri; aequales = punctum
     * triplex (non genericum) */
    per_segmentum = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.numerus + I) * magnitudo(i32));
    initia_eventuum = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.numerus + I) * magnitudo(i32));
    eventus = (i32*)piscina_allocare(piscina, (memoriae_index)(II
        * numerus + I) * magnitudo(i32));
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        per_segmentum[i] = ZEPHYRUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        per_segmentum[transitus[j].supra]++;
        per_segmentum[transitus[j].infra]++;
    }
    positus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        initia_eventuum[i]  = positus;
        positus             += per_segmentum[i];
        per_segmentum[i]    = ZEPHYRUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        i32 si_ = transitus[j].supra;
        i32 in_ = transitus[j].infra;

        eventus[initia_eventuum[si_] + per_segmentum[si_]++] = II * j;
        eventus[initia_eventuum[in_] + per_segmentum[in_]++] = II * j
            + I;
    }
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        i32* e = eventus + initia_eventuum[i];
        i32  m = per_segmentum[i];
        i32  a;
        i32  b;

        /* insertio ordine parametri */
        per (a = I; a < m; a++)
        {
            i32 clavis = e[a];

            b = a;
            dum (b > ZEPHYRUM)
            {
                    i32 x = e[b - I];
                Fractio px = (x % II == ZEPHYRUM)
                    ? transitus[x / II].parametrum_supra
                    : transitus[x / II].parametrum_infra;
                Fractio pc = (clavis % II == ZEPHYRUM)
                    ? transitus[clavis / II].parametrum_supra
                    : transitus[clavis / II].parametrum_infra;
                s32 ordo = fractio_compara(px, pc, piscina);

                si (ordo == ZEPHYRUM)
                {
                    redde FALSUM;   /* punctum triplex */
                }
                si (ordo < ZEPHYRUM)
                {
                    frange;
                }
                e[b] = x;
                b--;
            }
            e[b] = clavis;
        }
    }

    /* percursus per componentes, segmenta ordine */
    percursus = (i32*)piscina_allocare(piscina, (memoriae_index)(II
        * numerus + I) * magnitudo(i32));
    initia_percursus = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.componentes + I) * magnitudo(i32));
    positus = ZEPHYRUM;
    per (j = ZEPHYRUM; j < l.componentes; j++)
    {
        initia_percursus[j] = positus;
        per (i = l.initia[j]; i < l.initia[j + I]; i++)
        {
            i32 a;

            per (a = ZEPHYRUM; a < per_segmentum[i]; a++)
            {
                percursus[positus++] = eventus[initia_eventuum[i] + a];
            }
        }
    }
    initia_percursus[l.componentes] = positus;

    exitus->laqueus           = l;
    exitus->directio          = v;
    exitus->transitus         = transitus;
    exitus->numerus           = numerus;
    exitus->percursus         = percursus;
    exitus->initia_percursus  = initia_percursus;
    redde VERUM;
}

b32
laqueus_diagramma_genericum (
        Laqueus  l,
        Piscina* piscina,
      Diagramma* exitus)
{
    s32 directiones[XII][III] = {
        { 0, 0, 1 }, { 1, 2, 3 }, { 2, 3, 5 }, { 3, 5, 7 },
        { 1, -2, 4 }, { 5, -3, 2 }, { 7, 11, 13 }, { -2, 3, 11 },
        { 13, -7, 5 }, { 1, 1, 17 }, { 19, -23, 29 }, { 31, 37, -41 }
    };
    i32 k;

    per (k = ZEPHYRUM; k < XII; k++)
    {
        PiscinaNotatio nota = piscina_notare(piscina);

        si (laqueus_diagramma(l, situs_punctum(directiones[k][ZEPHYRUM],
            directiones[k][I], directiones[k][II]), piscina, exitus))
        {
            redde VERUM;
        }
        piscina_reficere(piscina, nota);
    }
    /* Series fixa exhauriri potest (polygonum simplex XIII verticum cum
     * segmento parallelo cuique directioni: recensio laqueus-I, B).
     * Curva momentorum (1, k, k^2): quaeque condicio non generica v in
     * plano (vertex in segmento proiectus, vicini superpositi), in
     * recta (segmentum parallelum) aut in cono quadrico (punctum
     * triplex: rectae tres rectas obliquas secantes regulum faciunt)
     * ponit; curva planum bis, conum quater ad summum secat - ergo pro
     * laqueo simplici k finitus sufficit. Laqueus non simplex numquam
     * genericus est: ante iter refutatur. Limes = numerus k irritorum
     * possibilium + 1: n (parallela) + 2n (vicini) + 2n^2 (vertex in
     * segmento) + 4 C(n, 3) (puncta triplicia); ultra eum FALSUM
     * (error, non pendere - planta M32 sine limite X minuta currebat
     * donec interfecta). k^2 < 2^63 etiam. */
    si (!laqueus_simplex(l, piscina))
    {
        redde FALSUM;
    }
    {
        s64 n = (s64)l.numerus;
        s64 limes;
        s64 k;

        limes = (n > (s64)M * M)
            ? (s64)MMMXXXVII * M * M
            : I + III * n + II * n * n + II * n * (n - I) * (n - II)
                / III;
        si (limes > (s64)MMMXXXVII * M * M)
        {
            limes = (s64)MMMXXXVII * M * M;
        }
        per (k = I; k <= limes; k++)
        {
            PiscinaNotatio nota = piscina_notare(piscina);

            si (laqueus_diagramma(l, situs_punctum(I, k, k * k),
                piscina,
                exitus))
            {
                redde VERUM;
            }
            piscina_reficere(piscina, nota);
        }
    }
    redde FALSUM;
}

b32
laqueus_diagramma_minimum (
     Laqueus  l,
         i32  radius,
     Piscina* piscina,
   Diagramma* exitus)
{
    s64 r = (s64)radius;
    s64 a;
    s64 b;
    s64 c;
    s64 optima[III];
    i32 paucissimi  = ZEPHYRUM;
    b32 inventum    = FALSUM;

    si (radius == ZEPHYRUM || radius > LAQUEUS_RADIUS_MAXIMUS)
    {
        redde FALSUM;
    }
    per (a = -r; a <= r; a++)
    {
        per (b = -r; b <= r; b++)
        {
            per (c = -r; c <= r; c++)
            {
                PiscinaNotatio nota;
                     Diagramma d;

                /* una ex +-v: componens prima non nulla positiva */
                si (   a < ZEPHYRUM || (a == ZEPHYRUM && (b < ZEPHYRUM
                    || (b == ZEPHYRUM && c <= ZEPHYRUM))))
                {
                    perge;
                }
                nota = piscina_notare(piscina);
                si (   laqueus_diagramma(l, situs_punctum(a, b, c),
                    piscina,
                        &d)
                    && (!inventum || d.numerus < paucissimi))
                {
                    inventum          = VERUM;
                    paucissimi        = d.numerus;
                    optima[ZEPHYRUM]  = a;
                    optima[I]         = b;
                    optima[II]        = c;
                }
                piscina_reficere(piscina, nota);
            }
        }
    }
    redde inventum
        && laqueus_diagramma(l, situs_punctum(optima[ZEPHYRUM],
        optima[I], optima[II]), piscina, exitus);
}

i32
diagramma_numerus (
    Diagramma d)
{
    redde d.numerus;
}

Transitus
diagramma_transitus (
    Diagramma d,
          i32 k)
{
    redde d.transitus[k];
}


/* ==================================================
 * Invariantes
 * ================================================== */

s32
diagramma_scriptura (
    Diagramma d)
{
    s32 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < d.numerus; k++)
    {
        summa += d.transitus[k].signum;
    }
    redde summa;
}

s32
diagramma_numerus_ligationis (
    Diagramma d,
          i32 a,
          i32 b)
{
    s32 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < d.numerus; k++)
    {
        i32 cs = _componens(d.laqueus, d.transitus[k].supra);
        i32 ci = _componens(d.laqueus, d.transitus[k].infra);

        si ((cs == a && ci == b) || (cs == b && ci == a))
        {
            summa += d.transitus[k].signum;
        }
    }
    redde summa / II;
}

/* positio sequens in percursu (eiusdem componentis, circulariter) */
interior i32
_positio_sequens (
    Diagramma d,
          i32 p)
{
    i32 k = ZEPHYRUM;

    dum (d.initia_percursus[k + I] <= p)
    {
        k++;
    }
    redde (p + I == d.initia_percursus[k + I]) ? d.initia_percursus[k]
        : p + I;
}

interior i32
_radix (
    i32* pater,
    i32  x)
{
    dum (pater[x] != x)
    {
        pater[x]  = pater[pater[x]];
        x         = pater[x];
    }
    redde x;
}

/* ora (codex PD in positionibus 0..m-1, 4 per transitum, sicut
 * diagramma_pd sed 0-basatum) diagrammatis */
interior i32*
_ora_diagrammatis (
    Diagramma  d,
      Piscina* piscina)
{
    i32  c = d.numerus;
    i32  m = II * c;
    i32* ora      = (i32*)piscina_allocare(piscina, (memoriae_index)(IV
        * c + I) * magnitudo(i32));
    i32* positio  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I) * magnitudo(i32));
    i32 k;

    per (k = ZEPHYRUM; k < m; k++)
    {
        positio[d.percursus[k]] = k;
    }
    /* ora: ora p = arcus intrans passum ad positionem p */
    per (k = ZEPHYRUM; k < c; k++)
    {
        i32 u_in   = positio[II * k + I];
        i32 u_out  = _positio_sequens(d, u_in);
        i32 o_in   = positio[II * k];
        i32 o_out  = _positio_sequens(d, o_in);

        ora[IV * k]       = u_in;
        ora[IV * k + II]  = u_out;
        si (d.transitus[k].signum > ZEPHYRUM)
        {
            ora[IV * k + I]    = o_out;
            ora[IV * k + III]  = o_in;
        }
        alioquin
        {
            ora[IV * k + I]    = o_in;
            ora[IV * k + III]  = o_out;
        }
    }
    redde ora;
}

/* uncinus Kauffman ex ora (0..m-1) per summam statuum; ansae_liberae =
 * componentes sine transitu */
interior b32
_uncinus_ex_oris (
    constans i32* ora,
             i32  c,
             i32  m,
             i32  ansae_liberae,
             i32  componentes,
         Piscina* piscina,
      Polynomium* exitus)
{
           i32* pater;
           s64* numeri;     /* [exponens + c][ansae] */
           i32  latitudo;
           i32  k;
           i64  status;
    Polynomium  summa = polynomium_nullum();
    Polynomium  dd;
    Polynomium  potentia_d;

    si (c > LAQUEUS_TRANSITUS_MAXIMI)
    {
        redde FALSUM;
    }
    latitudo = c + componentes + I;
    pater    = (i32*)piscina_allocare(piscina, (memoriae_index)(m + I)
        * magnitudo(i32));
    numeri   = (s64*)piscina_allocare(piscina, (memoriae_index)(II * c
        + I) * (memoriae_index)latitudo * magnitudo(s64));
    per (k = ZEPHYRUM; k < (II * c + I) * latitudo; k++)
    {
        numeri[k] = ZEPHYRUM;
    }
    per (status = ZEPHYRUM; status < ((i64)I << c); status++)
    {
        i32 ansae     = ansae_liberae;
        s32 exponens  = ZEPHYRUM;
        i32 e;

        per (e = ZEPHYRUM; e < m; e++)
        {
            pater[e] = e;
        }
        per (k = ZEPHYRUM; k < c; k++)
        {
            constans i32* x = ora + IV * k;

            si ((status >> k) & (i64)I)
            {
                /* B: (X0 X3)(X1 X2) */
                pater[_radix(pater, x[ZEPHYRUM])] = _radix(pater,
                    x[III]);
                pater[_radix(pater, x[I])] = _radix(pater, x[II]);
                exponens--;
            }
            alioquin
            {
                /* A: (X0 X1)(X2 X3) */
                pater[_radix(pater, x[ZEPHYRUM])] = _radix(pater, x[I]);
                pater[_radix(pater, x[II])] = _radix(pater, x[III]);
                exponens++;
            }
        }
        per (e = ZEPHYRUM; e < m; e++)
        {
            si (_radix(pater, e) == e)
            {
                ansae++;
            }
        }
        numeri[(i32)(exponens + (s32)c) * latitudo + ansae]++;
    }

    /* D uncinatum = summa A^e d^(ansae - 1), d = -A^2 - A^-2 */
    dd = polynomium_nullum();
    {
        Polynomium a2   = polynomium_nullum();
        Polynomium a_2  = polynomium_nullum();

        (vacuum)polynomium_monomium(magnus_ex_s64(-I), II, piscina,
            &a2);
        (vacuum)polynomium_monomium(magnus_ex_s64(-I), -II, piscina,
            &a_2);
        dd = polynomium_adde(a2, a_2, piscina);
    }
    per (k = ZEPHYRUM; k < (II * c + I) * latitudo; k++)
    {
               s32 exponens  = (s32)(k / latitudo) - (s32)c;
               i32 ansae     = k % latitudo;
        Polynomium terminus  = polynomium_nullum();

        si (numeri[k] == ZEPHYRUM)
        {
            perge;
        }
        si (   ansae == ZEPHYRUM
            || !polynomium_potentia(dd, ansae - I, piscina, &potentia_d)
            || !polynomium_monomium(magnus_ex_s64(numeri[k]), exponens,
                piscina, &terminus)
            || !polynomium_multiplica(terminus, potentia_d, piscina,
                &terminus))
        {
            redde FALSUM;
        }
        summa = polynomium_adde(summa, terminus, piscina);
    }
    *exitus = summa;
    redde VERUM;
}

b32
diagramma_uncinus (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    i32 ansae_liberae = ZEPHYRUM;
    i32 k;

    si (d.numerus > LAQUEUS_TRANSITUS_MAXIMI)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < d.laqueus.componentes; k++)
    {
        si (d.initia_percursus[k + I] == d.initia_percursus[k])
        {
            ansae_liberae++;
        }
    }
    redde _uncinus_ex_oris(_ora_diagrammatis(d, piscina), d.numerus,
        II * d.numerus, ansae_liberae, d.laqueus.componentes, piscina,
        exitus);
}

/* Jones V(t) = f(A = t^(-1/4)), f = (-A^3)^-w uncinus = (-1)^w A^-3w
 * uncinus */
interior b32
_jones_ex_uncino (
    Polynomium  uncinus,
           s32  w,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium factor;
    Polynomium f;

    si (   !polynomium_monomium(magnus_ex_s64((w % II == ZEPHYRUM) ? I
            : -I), -III * w, piscina, &factor)
        || !polynomium_multiplica(factor, uncinus, piscina, &f))
    {
        redde FALSUM;
    }
    redde polynomium_contrahe(f, -IV, piscina, exitus);
}

b32
diagramma_jones (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium uncinus;

    /* catena componentium numeri paris: V in t^(1/2) Z[t, t^-1] -
     * exponentes dimidii certi, uncinus (2^c status) frustra */
    si (d.laqueus.componentes % II == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde diagramma_uncinus(d, piscina, &uncinus)
        && _jones_ex_uncino(uncinus, diagramma_scriptura(d), piscina,
            exitus);
}

/* Alexander ex arcubus (calculus Fox): linea k transitus k, columnae
 * arcus supra, arcus intrans infra, arcus exiens infra; positivus (1 -
 * t, t, -1), negativus (1 - t^-1, t^-1, -1); minor linea et columna
 * ultimis deletis, forma normalis */
interior b32
_alexander_ex_arcubus (
               i32  c,
      constans i32* supra_arcus,
      constans i32* in_arcus,
      constans i32* ex_arcus,
      constans s32* signa,
           Piscina* piscina,
        Polynomium* exitus)
{
    constans Anulus* p = &ANULUS_POLYNOMIORUM;
                i32  q;
                i32  k;
             Matrix  matrix_plena;
             Matrix  minor;
         Polynomium  delta;
         Polynomium  unum = polynomium_constans(magnus_ex_s64(I),
             piscina);
         Polynomium t    = polynomium_nullum();
         Polynomium t_1  = polynomium_nullum();
         Polynomium minus_unum = polynomium_constans(magnus_ex_s64(-I),
             piscina);

    si (c == ZEPHYRUM)
    {
        *exitus = unum;
        redde VERUM;
    }
    (vacuum)polynomium_monomium(magnus_ex_s64(I), I, piscina, &t);
    (vacuum)polynomium_monomium(magnus_ex_s64(I), -I, piscina, &t_1);
    (vacuum)matrix_nulla(p, c, c, piscina, &matrix_plena);
    per (k = ZEPHYRUM; k < c; k++)
    {
        Polynomium valores[III];
               i32 columnae[III];
               i32 r;

        si (signa[k] > ZEPHYRUM)
        {
            valores[ZEPHYRUM]  = polynomium_subtrahe(unum, t, piscina);
            valores[I]         = t;
        }
        alioquin
        {
            valores[ZEPHYRUM] = polynomium_subtrahe(unum, t_1, piscina);
            valores[I] = t_1;
        }
        valores[II]         = minus_unum;
        columnae[ZEPHYRUM]  = supra_arcus[k];
        columnae[I]         = in_arcus[k];
        columnae[II]        = ex_arcus[k];
        per (r = ZEPHYRUM; r < III; r++)
        {
            Polynomium summa = polynomium_adde(*(constans Polynomium*)
                matrix_elementum(matrix_plena, k, columnae[r]),
                valores[r], piscina);

            matrix_pone(&matrix_plena, k, columnae[r], &summa);
        }
    }
    /* minor: linea et columna ultimae deletae */
    (vacuum)matrix_nulla(p, c - I, c - I, piscina, &minor);
    per (k = ZEPHYRUM; k + I < c; k++)
    {
        per (q = ZEPHYRUM; q + I < c; q++)
        {
            matrix_pone(&minor, k, q, matrix_elementum(matrix_plena, k,
                q));
        }
    }
    si (   !matrix_determinans(minor, piscina, &delta)
        || polynomium_est_nullum(delta))
    {
        redde FALSUM;
    }
    redde polynomium_normale(delta, piscina, exitus);
}

b32
diagramma_alexander (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    i32  c = d.numerus;
    i32  m = II * c;
    i32* supra_arcus;
    i32* in_arcus;
    i32* ex_arcus;
    s32* signa;
    i32  initium  = ZEPHYRUM;
    i32  arcus    = ZEPHYRUM;
    i32  q;
    i32  k;

    si (d.laqueus.componentes != I)
    {
        redde FALSUM;
    }
    supra_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I)
        * magnitudo(i32));
    in_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(i32));
    ex_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(i32));
    signa = (s32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(s32));
    /* incipe post passum infra primum: arcus 0 ibi incipit */
    per (k = ZEPHYRUM; c > ZEPHYRUM && d.percursus[k] % II == ZEPHYRUM;
         k++)
    {
        initium++;
    }
    per (q = I; c > ZEPHYRUM && q <= m; q++)
    {
        i32 codex  = d.percursus[(initium + q) % m];
        i32 k_     = codex / II;

        si (codex % II == ZEPHYRUM)
        {
            supra_arcus[k_] = arcus;
        }
        alioquin
        {
            in_arcus[k_]  = arcus;
            arcus         = (arcus + I) % c;
            ex_arcus[k_]  = arcus;
        }
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        signa[k] = d.transitus[k].signum;
    }
    redde _alexander_ex_arcubus(c, supra_arcus, in_arcus, ex_arcus,
        signa,
        piscina, exitus);
}


/* ==================================================
 * Codex PD
 * ================================================== */

b32
diagramma_pd (
     Diagramma   d,
       Piscina*  piscina,
           i32** pd)
{
    i32* ora = _ora_diagrammatis(d, piscina);
    i32  k;

    si (d.laqueus.componentes != I)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < IV * d.numerus; k++)
    {
        ora[k] = ora[k] + I;
    }
    *pd = ora;
    redde VERUM;
}

/* codex planus? Systema rotationis (sedes 0..3 contra horologium):
 * facies = orbitae sequentis(d) = sedes post alterum finem ossis d;
 * planum iff V - E + F = 2, i.e. F = c + 2. Codex validus sed non
 * planus (nodus virtualis, e.g. [[1,3,2,4],[2,5,3,6],[4,1,5,6]]: facies
 * una) refutatur - invariantes et motus R2 sine facie planitiem
 * praesumunt. ora 0-basata, bis quodque. */
interior b32
_pd_planus (
    constans i32* ora,
             i32  c,
         Piscina* piscina)
{
    i32  n = IV * c;
    i32* alter  = (i32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(i32));
    i32* prima  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(II * c
        + I) * magnitudo(i32));
    b32* visa   = (b32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(b32));
    i32 facies = ZEPHYRUM;
    i32 d;

    per (d = ZEPHYRUM; d < II * c; d++)
    {
        prima[d] = n;
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        visa[d] = FALSUM;
        si (prima[ora[d]] == n)
        {
            prima[ora[d]] = d;
        }
        alioquin
        {
            alter[d]              = prima[ora[d]];
            alter[prima[ora[d]]]  = d;
        }
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        i32 e = d;

        si (visa[d])
        {
            perge;
        }
        facies++;
        dum (!visa[e])
        {
            visa[e]  = VERUM;
            e        = IV * (alter[e] / IV) + (alter[e] % IV + I) % IV;
        }
    }
    redde facies == c + II;
}

/* codex PD nodi (KnotTheory): ora 1..2c bis quaeque; infra X0 -> X2
 * (X2 = sequens X0), supra aut X3 -> X1 (positivus) aut X1 -> X3
 * (negativus). ora 0-basata et signa redduntur; FALSUM si invalidus aut
 * c == 1 (signum ex ordine orarum non determinatur) */
interior b32
_pd_legere (
    constans i32*  pd,
             i32   c,
         Piscina*  piscina,
             i32** ora,
             s32** signa)
{
    i32  m = II * c;
    i32* visa;
    i32  k;

    si (c == I)
    {
        redde FALSUM;
    }
    *ora    = (i32*)piscina_allocare(piscina, (memoriae_index)(IV * c
        + I)
        * magnitudo(i32));
    *signa  = (s32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(s32));
    visa    = (i32*)piscina_allocare(piscina, (memoriae_index)(m + I)
        * magnitudo(i32));
    per (k = ZEPHYRUM; k < m; k++)
    {
        visa[k] = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        si (pd[k] < I || pd[k] > m)
        {
            redde FALSUM;
        }
        (*ora)[k] = pd[k] - I;
        visa[(*ora)[k]]++;
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        si (visa[k] != II)
        {
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x = *ora + IV * k;

        si (x[II] != (x[ZEPHYRUM] + I) % m)
        {
            redde FALSUM;
        }
        si (x[I] == (x[III] + I) % m)
        {
            (*signa)[k] = I;
        }
        alioquin si (x[III] == (x[I] + I) % m)
        {
            (*signa)[k] = -I;
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    /* quodque os semel intrans, semel exiens (infra X0; supra X3
     * positivus, X1 negativus): aliter ora "bis" sed orientatio falsa -
     * [1, 1, 2, 2, 3, 3, 4, 4] = duae ansae disiunctae. Ita numeratio
     * l -> l + 1 per omnia ora currit: componens una. */
    per (k = ZEPHYRUM; k < m; k++)
    {
        visa[k] = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        visa[(*ora)[IV * k]]++;
        visa[(*ora)[IV * k + ((*signa)[k] > ZEPHYRUM ? III : I)]]++;
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        si (visa[k] != I)
        {
            redde FALSUM;
        }
    }
    redde _pd_planus(*ora, c, piscina);
}

b32
laqueus_uncinus_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus)
{
    i32* ora;
    s32* signa;

    si (transitus == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    redde _pd_legere(pd, transitus, piscina, &ora, &signa)
        && _uncinus_ex_oris(ora, transitus, II * transitus, ZEPHYRUM, I,
            piscina, exitus);
}

b32
laqueus_jones_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus)
{
           i32* ora;
           s32* signa;
           s32  w = ZEPHYRUM;
           i32  k;
    Polynomium  uncinus;

    si (transitus == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (!_pd_legere(pd, transitus, piscina, &ora, &signa))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < transitus; k++)
    {
        w = w + signa[k];
    }
    redde _uncinus_ex_oris(ora, transitus, II * transitus, ZEPHYRUM, I,
            piscina, &uncinus)
        && _jones_ex_uncino(uncinus, w, piscina, exitus);
}

b32
laqueus_alexander_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus)
{
    i32  c = transitus;
    i32  m = II * c;
    i32* ora;
    s32* signa;
    i32* pater;
    i32* numerus_arcus;
    i32* supra_arcus;
    i32* in_arcus;
    i32* ex_arcus;
    i32  arcus = ZEPHYRUM;
    i32  k;

    si (c == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (!_pd_legere(pd, c, piscina, &ora, &signa))
    {
        redde FALSUM;
    }
    /* arcus: ora supra (X1, X3) eiusdem arcus; infra arcus finitur */
    pater          = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I) * magnitudo(i32));
    numerus_arcus  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I) * magnitudo(i32));
    supra_arcus    = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I) * magnitudo(i32));
    in_arcus       = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I) * magnitudo(i32));
    ex_arcus       = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I) * magnitudo(i32));
    per (k = ZEPHYRUM; k < m; k++)
    {
        pater[k]          = k;
        numerus_arcus[k]  = m;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        pater[_radix(pater, ora[IV * k + I])] = _radix(pater,
            ora[IV * k + III]);
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        i32 r = _radix(pater, k);

        si (numerus_arcus[r] == m)
        {
            numerus_arcus[r] = arcus;
            arcus++;
        }
    }
    si (arcus != c)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        supra_arcus[k]  = numerus_arcus[_radix(pater, ora[IV * k + I])];
        in_arcus[k]     = numerus_arcus[_radix(pater, ora[IV * k])];
        ex_arcus[k] = numerus_arcus[_radix(pater, ora[IV * k
            + II])];
    }
    redde _alexander_ex_arcubus(c, supra_arcus, in_arcus, ex_arcus,
        signa,
        piscina, exitus);
}

/* ora transituum vivorum (vivus) cum oris per pater coniunctis secundum
 * nodum renumerantur 0..2c'-1 (ora intrans X0 -> exiens X2 = X0 + 1);
 * signa servantur. FALSUM si os non semel intrans et semel exiens aut
 * percursus ora omnia non tangit. c' == 0: ora nova NIHIL. */
interior b32
_pd_renumerare (
    constans i32*  ora,
    constans s32*  signa,
    constans b32*  vivus,
             i32   c,
             i32*  pater,
         Piscina*  piscina,
             i32** ora_nova,
             s32** signa_nova,
             i32*  c_nova)
{
    i32  m             = II * c;
    i32  locus_nullus  = IV * c;
    i32* intrans  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* numerus  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* exiens   = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* novus    = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I)
        * magnitudo(i32));
    i32 vivi    = ZEPHYRUM;
    i32 primus  = c;
    i32 q       = ZEPHYRUM;
    i32 os;
    i32 os_primum;
    i32 k;
    i32 s;

    per (k = ZEPHYRUM; k < m; k++)
    {
        intrans[k]  = locus_nullus;
        exiens[k]   = locus_nullus;
        numerus[k]  = m;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        si (vivus[k])
        {
            si (primus == c)
            {
                primus = k;
            }
            novus[k] = vivi;
            vivi++;
        }
    }
    *c_nova = vivi;
    si (vivi == ZEPHYRUM)
    {
        *ora_nova    = NIHIL;
        *signa_nova  = NIHIL;
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (s = ZEPHYRUM; vivus[k] && s < IV; s++)
        {
            i32 radix = _radix(pater, ora[IV * k + s]);

            si (s == ZEPHYRUM || s == (signa[k] > ZEPHYRUM ? III : I))
            {
                si (intrans[radix] != locus_nullus)
                {
                    redde FALSUM;
                }
                intrans[radix] = IV * k + s;
            }
            alioquin
            {
                si (exiens[radix] != locus_nullus)
                {
                    redde FALSUM;
                }
                exiens[radix] = IV * k + s;
            }
        }
    }
    /* percursus: os intrans ad sedem s, exiens eiusdem fili */
    os_primum  = _radix(pater, ora[IV * primus]);
    os         = os_primum;
    fac
    {
        i32 locus = intrans[os];

        si (locus == locus_nullus || numerus[os] != m || q >= II * vivi)
        {
            redde FALSUM;
        }
        numerus[os] = q;
        q++;
        k = locus / IV;
        s = locus % IV;
        os  = _radix(pater, ora[IV * k + (s == ZEPHYRUM ? II : (signa[k]
            > ZEPHYRUM ? I : III))]);
    }
    dum (os != os_primum);
    si (q != II * vivi)
    {
        redde FALSUM;
    }
    *ora_nova = (i32*)piscina_allocare(piscina, (memoriae_index)(IV
        * vivi + I) * magnitudo(i32));
    *signa_nova = (s32*)piscina_allocare(piscina, (memoriae_index)(vivi
        + I) * magnitudo(s32));
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (s = ZEPHYRUM; vivus[k] && s < IV; s++)
        {
            (*ora_nova)[IV * novus[k] + s] = numerus[_radix(pater,
                ora[IV * k
                + s])];
        }
        si (vivus[k])
        {
            (*signa_nova)[novus[k]] = signa[k];
        }
    }
    redde VERUM;
}

/* motus unus Reidemeister in oris 0-basatis normalibus: R1 primum,
 * deinde R2. R1: os ad sedes vicinas eiusdem transitus (ansa; in codice
 * valido semper vicinas - monogonum). R2: transitus diversi j, k duo
 * ora e, f communicantes, in j sedibus vicinis (fila diversa), eodem
 * filo supra utrumque (paritas sedis ossis e in j et in k eadem). In
 * nodo (componente una) e, f aut faciem bilateram claudunt (R2
 * classicum) aut tangle 1-1 ad j et alterum ad k pendet ex utraque
 * parte - tunc nodus T1 # T2 et ante et post remotionem, codex planus
 * manet. Ergo facies non requiritur (vide lib/laqueus.worklog.md,
 * 2026-10-07). Reddit 1 applicatus, 0 nullus, -1 error. */
interior s32
_pd_motus (
         i32** ora,
         s32** signa,
         i32*  c,
     Piscina*  piscina)
{
    i32  n = *c;
    i32  m = II * n;
    i32* o = *ora;
    i32* alter  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(IV * n
        + I) * magnitudo(i32));
    i32* prima  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* pater  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    b32* vivus  = (b32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(b32));
    i32 d;
    i32 k;

    per (d = ZEPHYRUM; d < m; d++)
    {
        prima[d] = IV * n;
        pater[d] = d;
    }
    per (d = ZEPHYRUM; d < IV * n; d++)
    {
        si (prima[o[d]] == IV * n)
        {
            prima[o[d]] = d;
        }
        alioquin
        {
            alter[d]            = prima[o[d]];
            alter[prima[o[d]]]  = d;
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        vivus[k] = VERUM;
    }
    /* R1: ansa ad sedes s, s + 1 */
    per (d = ZEPHYRUM; d < IV * n; d++)
    {
        k = d / IV;
        si (alter[d] / IV == k && (d % IV + I) % IV == alter[d] % IV)
        {
            vivus[k] = FALSUM;
            pater[_radix(pater, o[IV * k + (d % IV + II) % IV])] =
                _radix(
                pater, o[IV * k + (d % IV + III) % IV]);
            redde _pd_renumerare(o, *signa, vivus, n, pater, piscina,
                ora,
                signa, c) ? I : -I;
        }
    }
    /* R2: os e = sedes a in j, a1 in k; os f = sedes b = a + 1 in j,
     * b1 in k */
    per (d = ZEPHYRUM; d < IV * n; d++)
    {
        i32 j   = d / IV;
        i32 a   = d % IV;
        i32 a1  = alter[d] % IV;
        i32 b   = (a + I) % IV;
        i32 b1  = alter[IV * j + b] % IV;

        k = alter[d] / IV;
        si (k == j || alter[IV * j + b] / IV != k)
        {
            perge;
        }
        si (a % II != a1 % II)
        {
            /* fibula: filum supra in uno, infra in altero */
            perge;
        }
        vivus[j] = FALSUM;
        vivus[k] = FALSUM;
        /* filum ossis e (sedes oppositae) et filum ossis f iunguntur */
        pater[_radix(pater, o[IV * j + (a + II) % IV])] = _radix(pater,
            o[IV * k + (a1 + II) % IV]);
        pater[_radix(pater, o[IV * j + (b + II) % IV])] = _radix(pater,
            o[IV * k + (b1 + II) % IV]);
        redde _pd_renumerare(o, *signa, vivus, n, pater, piscina, ora,
            signa, c) ? I : -I;
    }
    redde ZEPHYRUM;
}

b32
laqueus_pd_simplificare (
    constans i32*  pd,
             i32   transitus,
         Piscina*  piscina,
             i32** pd_exitus,
             i32*  transitus_exitus)
{
    i32* ora;
    s32* signa;
    i32  c = transitus;
    i32  k;

    si (c == I)
    {
        /* transitus unus: ora 1, 2 bis, X2 = X0 + 1 - ansa semper */
        si (   pd[ZEPHYRUM] < I || pd[ZEPHYRUM] > II
            || pd[II]  != III - pd[ZEPHYRUM] || pd[I] < I || pd[I] > II
            || pd[III] != III - pd[I])
        {
            redde FALSUM;
        }
        c = ZEPHYRUM;
    }
    si (c > ZEPHYRUM && !_pd_legere(pd, c, piscina, &ora, &signa))
    {
        redde FALSUM;
    }
    dum (c > I)
    {
        s32 effectus = _pd_motus(&ora, &signa, &c, piscina);

        si (effectus < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (effectus == ZEPHYRUM)
        {
            frange;
        }
    }
    si (c <= I)
    {
        /* diagramma unius transitus semper ansam habet */
        *pd_exitus         = NIHIL;
        *transitus_exitus  = ZEPHYRUM;
        redde VERUM;
    }
    *pd_exitus = (i32*)piscina_allocare(piscina, (memoriae_index)(IV * c
        + I) * magnitudo(i32));
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        (*pd_exitus)[k] = ora[k] + I;
    }
    *transitus_exitus = c;
    redde VERUM;
}

b32
diagramma_determinans (
     Diagramma  d,
       Piscina* piscina,
        Magnus* exitus)
{
    Polynomium delta;
       Fractio valor;

    si (   !diagramma_alexander(d, piscina, &delta)
        || !polynomium_valor(delta, fractio_ex_s64(-I), piscina,
        &valor))
    {
        redde FALSUM;
    }
    /* Delta in Z[t, t^-1]: valor ad -1 integer */
    *exitus = magnus_absolutum(fractio_numerator(valor), piscina);
    redde VERUM;
}


/* ==================================================
 * Motus trianguli
 * ================================================== */

interior vacuum
_laqueus_cum_vertice (
     Laqueus  l,
         i32  post,        /* insere post verticem 'post' */
     Punctum  c,
     Piscina* piscina,
     Laqueus* exitus)
{
    Punctum* vertices = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)(l.numerus + I) * magnitudo(Punctum));
        i32* initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
            l.componentes + I) * magnitudo(i32));
        i32 i;
        i32 k = _componens(l, post);

    per (i = ZEPHYRUM; i <= post; i++)
    {
        vertices[i] = l.vertices[i];
    }
    vertices[post + I] = c;
    per (i = post + I; i < l.numerus; i++)
    {
        vertices[i + I] = l.vertices[i];
    }
    per (i = ZEPHYRUM; i <= l.componentes; i++)
    {
        initia[i] = l.initia[i] + ((i > k) ? I : ZEPHYRUM);
    }
    exitus->vertices     = vertices;
    exitus->numerus      = l.numerus + I;
    exitus->initia       = initia;
    exitus->componentes  = l.componentes;
}

b32
laqueus_motus_addere (
     Laqueus  l,
         i32  i,
     Punctum  c,
     Piscina* piscina,
     Laqueus* exitus)
{
        i32 n;
        i32 j;
    Punctum a;
    Punctum b;

    si (i >= l.numerus)
    {
        redde FALSUM;
    }
    n = _sequens(l, i);
    a = l.vertices[i];
    b = l.vertices[n];
    si (_collinearia(a, b, c, piscina))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < l.numerus; j++)
    {
        Punctum p = l.vertices[j];
        Punctum q = l.vertices[_sequens(l, j)];

        si (j == i)
        {
            perge;
        }
        si (j == _prior(l, i))
        {
            si (situs_triangulum_vicinum(a, b, c, p, piscina)
                != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
        alioquin si (j == n)
        {
            si (situs_triangulum_vicinum(b, c, a, q, piscina)
                != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
        alioquin si (situs_triangulum_segmentum(a, b, c, p, q, piscina)
                     != SITUS_DISIUNCTA)
        {
            redde FALSUM;
        }
    }
    _laqueus_cum_vertice(l, i, c, piscina, exitus);
    redde VERUM;
}

b32
laqueus_motus_removere (
     Laqueus  l,
         i32  i,
     Piscina* piscina,
     Laqueus* exitus)
{
        i32  k;
        i32  p;
        i32  n;
        i32  pp;
        i32  j;
    Punctum  a;
    Punctum  b;
    Punctum  c;
    Punctum* vertices;
        i32* initia;

    si (i >= l.numerus)
    {
        redde FALSUM;
    }
    k = _componens(l, i);
    si (l.initia[k + I] - l.initia[k] < IV)
    {
        redde FALSUM;
    }
    p   = _prior(l, i);
    n   = _sequens(l, i);
    pp  = _prior(l, p);
    a   = l.vertices[p];
    b   = l.vertices[i];
    c   = l.vertices[n];
    si (_collinearia(a, b, c, piscina))
    {
        /* vertex in segmento [a, c]: nihil verritur; aliter reflexio */
        si (situs_segmenta(a, c, b, b, piscina) == SITUS_DISIUNCTA)
        {
            redde FALSUM;
        }
    }
    alioquin
    {
        per (j = ZEPHYRUM; j < l.numerus; j++)
        {
            Punctum x = l.vertices[j];
            Punctum y = l.vertices[_sequens(l, j)];

            si (j == p || j == i)
            {
                perge;
            }
            si (j == pp)
            {
                si (situs_triangulum_vicinum(a, b, c, x, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (j == n)
            {
                si (situs_triangulum_vicinum(c, a, b, y, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (situs_triangulum_segmentum(a, b, c, x, y,
                         piscina) != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
    }
    vertices = (Punctum*)piscina_allocare(piscina, (memoriae_index)
        l.numerus * magnitudo(Punctum));
    initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.componentes + I) * magnitudo(i32));
    per (j = ZEPHYRUM; j < l.numerus; j++)
    {
        si (j < i)
        {
            vertices[j] = l.vertices[j];
        }
        alioquin si (j > i)
        {
            vertices[j - I] = l.vertices[j];
        }
    }
    per (j = ZEPHYRUM; j <= l.componentes; j++)
    {
        initia[j] = l.initia[j] - ((j > k) ? I : ZEPHYRUM);
    }
    exitus->vertices     = vertices;
    exitus->numerus      = l.numerus - I;
    exitus->initia       = initia;
    exitus->componentes  = l.componentes;
    redde VERUM;
}

b32
laqueus_simplificare (
     Laqueus  l,
     Piscina* piscina,
     Laqueus* exitus)
{
    PiscinaNotatio  nota;
           Punctum* vertices;
               i32* initia;
           Laqueus  cur;
               b32  simplex;
               b32  mutatum = VERUM;
               i32  k;

    nota     = piscina_notare(piscina);
    simplex  = laqueus_simplex(l, piscina);
    piscina_reficere(piscina, nota);
    si (!simplex)
    {
        redde FALSUM;
    }
    /* tabula laboris propria: motus removere solum ut oraculum legitimi
     * vocatur et statim reficitur */
    vertices = (Punctum*)piscina_allocare(piscina, (memoriae_index)
        l.numerus * magnitudo(Punctum));
    initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.componentes + I) * magnitudo(i32));
    memcpy(vertices, l.vertices, (memoriae_index)l.numerus
        * magnitudo(Punctum));
    memcpy(initia, l.initia, (memoriae_index)(l.componentes + I)
        * magnitudo(i32));
    cur.vertices     = vertices;
    cur.numerus      = l.numerus;
    cur.initia       = initia;
    cur.componentes  = l.componentes;
    dum (mutatum)
    {
        mutatum = FALSUM;
        per (k = ZEPHYRUM; k < cur.numerus; k++)
        {
            Laqueus ignotus;
                b32 legitimus;
                i32 j;
                i32 m;

            nota       = piscina_notare(piscina);
            legitimus  = laqueus_motus_removere(cur, k, piscina,
                &ignotus);
            piscina_reficere(piscina, nota);
            si (!legitimus)
            {
                perge;
            }
            /* vertex k e tabula laboris removetur */
            per (j = k; j + I < cur.numerus; j++)
            {
                vertices[j] = vertices[j + I];
            }
            per (m = ZEPHYRUM; m <= cur.componentes; m++)
            {
                si (initia[m] > k)
                {
                    initia[m]--;
                }
            }
            cur.numerus--;
            mutatum = VERUM;
            frange;
        }
    }
    *exitus = cur;
    redde VERUM;
}

b32
laqueus_speculum (
     Laqueus  l,
     Piscina* piscina,
     Laqueus* exitus)
{
    Punctum* puncta = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)
        l.numerus * magnitudo(Punctum));
        i32 k;

    per (k = ZEPHYRUM; k < l.numerus; k++)
    {
        puncta[k]    = l.vertices[k];
        puncta[k].z  = fractio_nega(l.vertices[k].z, piscina);
    }
    redde laqueus_ex_punctis(puncta, l.initia, l.componentes, piscina,
        exitus);
}


/* ==================================================
 * Modulus vertex: uncinus per contractionem retis
 *
 * Index per os (dimensio 2): spin +1 = ansa per os a dart a (primus
 * occursus in codice) ad dart b. Gyrationes t (quartae, a -> b) ex
 * arbore dualis: facies 0 (orbita dart 0) +4, ceterae -4, quisque
 * angulus -1. Tensor transitus = A [(X0 X1)(X2 X3)] + A^-1 [(X0 X3)(X1
 * X2)], par quodque 0 nisi alter finis intrat et alter exit, deinde
 * omega^-1 (arma p -> p+1) aut omega^+1; factor ossis omega^(spin t) ad
 * dart b. Os cuius ambo darts in eodem transitu (ansa R1) index non
 * est: spin eius in tensore summatur. Contractio = d <D>, d = omega^4
 * + omega^-4 = -A^2 - A^-2 cum A = i omega^2. Vide
 * project-specs/contractio-plan.md (The vertex model, Why it is
 * correct).
 * ================================================== */

/* terminus tensoris transitus: elementum += A^potentia_a omega^exponens */
nomen structura {
    i32 elementum;
    s32 potentia_a;
    s32 exponens;
} TerminusVerticis;

structura LaqueusModulusVertex {
                 i32  transitus;
                 s32  scriptura;
                 s32  imus;               /* fenestra uncinus, classis 3w */
                 s32  summus;
                 s32* gyrationes;         /* per os, a -> b */
      ContractioRete* rete;
      ContractioOrdo* ordo;
  ContractioOfficina* officina;
                 i32** valores;           /* per transitum */
                 i32* elementa;           /* per transitum */
                 i32* initia_terminorum;  /* transitus + 1 */
    TerminusVerticis* termini;
                 s32  exponens_maximus;   /* E >= IV */
                 i32* potentiae;          /* omega^j, j in [-E, E] */
                 i32  primus;             /* primus currens, 0 nullus */
                 i32  radix_i;            /* i^2 = -1 modulo primum */
                 i32  limes;              /* bita B (T5a) */
               Magnus limes_exactus;      /* B = min Pi gradus */
                 i32  aestimationes;      /* contractiones factae */
                 i32  mutationes_primi;   /* primi novi in aestimare */
};

/* residuum non negativum s64 modulo IV */
interior s64
_modulo_quattuor (
    s64 x)
{
    s64 r = x % (s64)IV;

    redde r < ZEPHYRUM ? r + (s64)IV : r;
}

/* gyrationes t per os (a -> b) ex aequationibus facierum per arborem
 * dualem (BFS a facie 0, folia primum); *faciei_exitus = facies per
 * positionem (0..c+1, orbita sequentis). ora 0-basata, codex planus.
 * FALSUM si aequatio non integra aut residuum manet (codex non planus:
 * non accidit post _pd_legere). */
interior b32
_gyrationes (
    constans i32*  ora,
             i32   c,
    constans i32*  finis_a,
    constans i32*  finis_b,
         Piscina*  piscina,
             s32*  gyrationes,
             i32** faciei_exitus)
{
    i32  n = IV * c;
    i32  m = II * c;
    i32* faciei = (i32*)piscina_allocare(piscina, (memoriae_index)n
        * magnitudo(i32));
    i32* alter  = (i32*)piscina_allocare(piscina, (memoriae_index)n
        * magnitudo(i32));
    s64* residua;
    s64* basis  = (s64*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(s64));
    i32* initia;
    i32* ossa_faciei;
    i32* parens;
    i32* ordo;
    i32  facies = ZEPHYRUM;
    i32  caput;
    i32  cauda;
    i32  d;
    i32  l;
    i32  f;

    *faciei_exitus = faciei;
    per (l = ZEPHYRUM; l < m; l++)
    {
        alter[finis_a[l]] = finis_b[l];
        alter[finis_b[l]] = finis_a[l];
        basis[l]          = (s64)((finis_b[l] % IV + IV - finis_a[l]
            % IV
            + II) % IV);
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        faciei[d] = n;
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        i32 e = d;

        si (faciei[d] != n)
        {
            perge;
        }
        dum (faciei[e] == n)
        {
            faciei[e] = facies;
            e = IV * (alter[e] / IV) + (alter[e] % IV + I)
                % IV;
        }
        facies++;
    }
    si (facies != c + II)
    {
        redde FALSUM;
    }

    /* R_f = scopus + longitudo - summa s basis; scopus +4 externa, -4
     * ceterae; quartae partes integrae */
    residua = (s64*)piscina_allocare(piscina, (memoriae_index)facies
        * magnitudo(s64));
    per (f = ZEPHYRUM; f < facies; f++)
    {
        residua[f] = f == ZEPHYRUM ? (s64)IV : -(s64)IV;
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        i32 os = ora[d];

        residua[faciei[d]] += (s64)I - (finis_a[os] == d ? basis[os]
            : -basis[os]);
    }
    per (f = ZEPHYRUM; f < facies; f++)
    {
        si (_modulo_quattuor(residua[f]) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        residua[f] /= (s64)IV;
    }

    /* graphum duale: ossa per faciem (quodque os in duabus faciebus) */
    initia       = (i32*)piscina_allocare(piscina,
        (memoriae_index)(facies + I) * magnitudo(i32));
    ossa_faciei  = (i32*)piscina_allocare(piscina, (memoriae_index)(II
        * m) * magnitudo(i32));
    parens       = (i32*)piscina_allocare(piscina,
        (memoriae_index)facies * magnitudo(i32));
    ordo         = (i32*)piscina_allocare(piscina,
        (memoriae_index)facies * magnitudo(i32));
    per (f = ZEPHYRUM; f <= facies; f++)
    {
        initia[f] = ZEPHYRUM;
    }
    per (l = ZEPHYRUM; l < m; l++)
    {
        initia[faciei[finis_a[l]] + I]++;
        initia[faciei[finis_b[l]] + I]++;
    }
    per (f = ZEPHYRUM; f < facies; f++)
    {
        initia[f + I]  += initia[f];
        parens[f]      = m;
    }
    {
        i32* positio = (i32*)piscina_allocare(piscina,
            (memoriae_index)facies * magnitudo(i32));

        per (f = ZEPHYRUM; f < facies; f++)
        {
            positio[f] = initia[f];
        }
        per (l = ZEPHYRUM; l < m; l++)
        {
            ossa_faciei[positio[faciei[finis_a[l]]]++] = l;
            ossa_faciei[positio[faciei[finis_b[l]]]++] = l;
        }
    }

    /* BFS a facie externa; parens[f] = os arboris (m: nullum) */
    ordo[ZEPHYRUM]    = ZEPHYRUM;
    parens[ZEPHYRUM]  = II * m;
    caput             = ZEPHYRUM;
    cauda             = I;
    dum (caput < cauda)
    {
        i32 g = ordo[caput++];
        i32 j;

        per (j = initia[g]; j < initia[g + I]; j++)
        {
            i32 os  = ossa_faciei[j];
            i32 fa  = faciei[finis_a[os]];
            i32 h   = fa == g ? faciei[finis_b[os]] : fa;

            si (parens[h] == m)
            {
                parens[h]      = os;
                ordo[cauda++]  = h;
            }
        }
    }
    si (cauda != facies)
    {
        redde FALSUM;
    }

    /* m_l: folia primum; ossa extra arborem m = 0 */
    per (l = ZEPHYRUM; l < m; l++)
    {
        gyrationes[l] = (s32)basis[l];
    }
    dum (cauda > I)
    {
        i32 g     = ordo[--cauda];
        i32 os    = parens[g];
        i32 fa    = faciei[finis_a[os]];
        i32 fb    = faciei[finis_b[os]];
        s64 gyri  = fa == g ? residua[g] : -residua[g];

        residua[fa]     -= gyri;
        residua[fb]     += gyri;
        gyrationes[os]  = (s32)(basis[os] + (s64)IV * gyri);
    }
    per (f = ZEPHYRUM; f < facies; f++)
    {
        si (residua[f] != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* factorem in productum: acervus u64 (< 2^32 ante multiplicationem,
 * factor < 2^31, ergo < 2^63) in Magnus solum cum crescit effunditur */
interior vacuum
_productum_addere (
        i64* acervus,
     Magnus* productum,
        i32  factor,
    Piscina* piscina)
{
    *acervus *= (i64)factor;
    si (*acervus >= ((i64)I << XXXII))
    {
        *productum = magnus_multiplica(*productum,
            magnus_ex_s64((s64)*acervus), piscina);
        *acervus = I;
    }
}

/* limes coefficientium uncinus (contractio-plan T5a): Sigma |c_k| <=
 * tau(G) <= Pi_{v != r} gradus(v) super graphum Tait utriusque
 * coloris; *limes_exitus = bita minimi, *exactus = minimum. Anguli
 * (k, j) = regio inter arma j et j+1 = faciei[4k + (j+1) % 4]; colores
 * per unionem-inventionem angulorum OPPOSITORUM (0~2, 1~3), non per
 * regulam paritatis (quae in diagrammatibus non alternantibus fallit).
 * Gradus sine colore: per transitum, si anguli oppositi regiones
 * diversae sunt, utraque +1 (ansae Tait excluduntur). FALSUM nisi
 * exacte duae classes et anguli vicini in classibus diversis. */
interior b32
_limes_arborum (
     constans i32* faciei,
              i32  c,
          Piscina* piscina,
           Magnus* exactus,
              i32* limes_exitus)
{
    i32  F       = c + II;
    i32* pater   = (i32*)piscina_allocare(piscina, (memoriae_index)F
        * magnitudo(i32));
    i32* gradus  = (i32*)piscina_allocare(piscina, (memoriae_index)F
        * magnitudo(i32));
       i32 radices[II];
       i32 numerus_radicum = ZEPHYRUM;
    Magnus producta[II];
       i32 q;
       i32 k;
       i32 j;
       i32 f;

    per (f = ZEPHYRUM; f < F; f++)
    {
        pater[f]   = f;
        gradus[f]  = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (j = ZEPHYRUM; j < II; j++)
        {
            i32 a = faciei[IV * k + (j + I) % IV];
            i32 b = faciei[IV * k + (j + III) % IV];

            pater[_radix(pater, a)] = _radix(pater, b);
            si (a != b)
            {
                gradus[a]++;
                gradus[b]++;
            }
        }
    }
    per (f = ZEPHYRUM; f < F; f++)
    {
        si (_radix(pater, f) == f)
        {
            si (numerus_radicum == II)
            {
                redde FALSUM;
            }
            radices[numerus_radicum++] = f;
        }
    }
    si (numerus_radicum != II)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (j = ZEPHYRUM; j < IV; j++)
        {
            si (_radix(pater, faciei[IV * k + (j + I) % IV])
                == _radix(pater, faciei[IV * k + (j + II) % IV]))
            {
                redde FALSUM;
            }
        }
    }

    /* per colorem: radix arboris = vertex gradus maximi exclusus */
    per (q = ZEPHYRUM; q < II; q++)
    {
        i32 maximus = F;
        i64 acervus = I;

        producta[q] = magnus_ex_s64((s64)I);
        per (f = ZEPHYRUM; f < F; f++)
        {
            si (   _radix(pater, f) == radices[q]
                && (maximus == F || gradus[f] > gradus[maximus]))
            {
                maximus = f;
            }
        }
        per (f = ZEPHYRUM; f < F; f++)
        {
            si (_radix(pater, f) == radices[q] && f != maximus)
            {
                _productum_addere(&acervus, &producta[q],
                    gradus[f] > ZEPHYRUM ? gradus[f] : I, piscina);
            }
        }
        producta[q] = magnus_multiplica(producta[q],
            magnus_ex_s64((s64)acervus), piscina);
    }
    *exactus = magnus_compara(producta[ZEPHYRUM], producta[I])
        <= ZEPHYRUM
        ? producta[ZEPHYRUM] : producta[I];
    *limes_exitus = magnus_bitorum(*exactus);
    redde VERUM;
}

/* termini tensoris transitus k: omnes spins ossium distinctorum in
 * transitu (ansa R1: ambo arma idem os, summatur), elementum ex
 * spinibus ossium externorum (ordine armorum; spin -1 = bitum 1,
 * ultimus celerrimus). Redde numerum terminorum scriptorum. */
interior i32
_terminos_transitus (
        constans i32* ora,
                 i32  k,
        constans i32* finis_a,
        constans i32* finis_b,
        constans s32* gyrationes,
    TerminusVerticis* termini,
                 i32* externa,
                 i32* numerus_externorum)
{
    constans i32 paria[II][IV] = {
        { ZEPHYRUM, I, II, III },    /* A: (X0 X1)(X2 X3) */
        { ZEPHYRUM, III, I, II }     /* B: (X0 X3)(X1 X2) */
    };
    i32 distincta[IV];
    i32 numerus_distinctorum = ZEPHYRUM;
    i32 situs_ossis[IV];             /* arma j -> index in distinctis */
    i32 positio_externa[IV];         /* distinctum -> positio, aut IV */
    i32 scripti = ZEPHYRUM;
    i32 masca;
    i32 j;
    i32 u;

    *numerus_externorum = ZEPHYRUM;
    per (j = ZEPHYRUM; j < IV; j++)
    {
        i32 os = ora[IV * k + j];

        per (u = ZEPHYRUM; u < numerus_distinctorum; u++)
        {
            si (distincta[u] == os)
            {
                frange;
            }
        }
        si (u == numerus_distinctorum)
        {
            distincta[numerus_distinctorum++] = os;
            si (finis_a[os] / IV == finis_b[os] / IV)
            {
                positio_externa[u] = IV;
            }
            alioquin
            {
                positio_externa[u]                = *numerus_externorum;
                externa[(*numerus_externorum)++]  = os;
            }
        }
        situs_ossis[j] = u;
    }

    per (masca = ZEPHYRUM; masca < ((i32)I << numerus_distinctorum);
         masca++)
    {
        b32 intrat[IV];
        s32 exponens_ossium  = ZEPHYRUM;
        i32 elementum        = ZEPHYRUM;
        i32 s;

        per (u = ZEPHYRUM; u < numerus_distinctorum; u++)
        {
            i32 os     = distincta[u];
            b32 minus  = ((masca >> u) & I) != ZEPHYRUM;

            si (finis_b[os] / IV == k)
            {
                exponens_ossium += minus ? -gyrationes[os]
                    : gyrationes[os];
            }
            si (positio_externa[u] != IV && minus)
            {
                elementum |= (i32)I << (*numerus_externorum - I
                    - positio_externa[u]);
            }
        }
        per (j = ZEPHYRUM; j < IV; j++)
        {
            i32 os     = ora[IV * k + j];
            b32 minus  = ((masca >> situs_ossis[j]) & I) != ZEPHYRUM;

            /* ad dart b os intrat sse spin +1; ad dart a sse spin -1 */
            intrat[j] = finis_b[os] == IV * k + j ? !minus : minus;
        }
        per (s = ZEPHYRUM; s < II; s++)
        {
            s32 exponens  = exponens_ossium;
            b32 validum   = VERUM;
            i32 q;

            per (q = ZEPHYRUM; q < IV; q += II)
            {
                i32 p0 = paria[s][q];
                i32 p1 = paria[s][q + I];
                i32 in;
                i32 ex;

                si (intrat[p0] == intrat[p1])
                {
                    validum = FALSUM;
                    frange;
                }
                in = intrat[p0] ? p0 : p1;
                ex = intrat[p0] ? p1 : p0;
                exponens += (ex + IV - in) % IV == I ? -(s32)I : (s32)I;
            }
            si (validum)
            {
                termini[scripti].elementum   = elementum;
                termini[scripti].potentia_a  = s == ZEPHYRUM ? (s32)I
                    : -(s32)I;
                termini[scripti].exponens    = exponens;
                scripti++;
            }
        }
    }
    redde scripti;
}

LaqueusModulusVertex*
laqueus_modulus_vertex_creare (
    constans i32* pd,
             i32  transitus,
             i64  octeti_maximi,
         Piscina* piscina)
{
    LaqueusModulusVertex* mv;
      ContractioOptiones  optiones = contractio_optiones_ordinariae();
                     i32* ora;
                     s32* signa;
                     i32* finis_a;
                     i32* finis_b;
                     i32* faciei;
                     i32* index_ossis;
                     i32* pater;
                     i32  c = transitus;
                     i32  m;
                     i32  ansae_a = ZEPHYRUM;
                     i32  ansae_b = ZEPHYRUM;
                     s32  classis;
                     s32  e_max = IV;
                     i32  k;
                     i32  l;

    si (   pd == NIHIL || piscina == NIHIL || c < II
        || !_pd_legere(pd, c, piscina, &ora, &signa))
    {
        redde NIHIL;
    }
    m      = II * c;
    mv     = (LaqueusModulusVertex*)piscina_allocare(piscina,
        magnitudo(LaqueusModulusVertex));
    finis_a = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    finis_b = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    per (l = ZEPHYRUM; l < m; l++)
    {
        finis_a[l] = IV * c;
    }
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        si (finis_a[ora[k]] == IV * c)
        {
            finis_a[ora[k]] = k;
        }
        alioquin
        {
            finis_b[ora[k]] = k;
        }
    }
    mv->transitus         = c;
    mv->primus            = ZEPHYRUM;
    mv->radix_i           = ZEPHYRUM;
    mv->aestimationes     = ZEPHYRUM;
    mv->mutationes_primi  = ZEPHYRUM;
    mv->gyrationes  = (s32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(s32));
    si (   !_gyrationes(ora, c, finis_a, finis_b, piscina,
        mv->gyrationes,
               &faciei)
        || !_limes_arborum(faciei, c, piscina, &mv->limes_exactus,
               &mv->limes))
    {
        redde NIHIL;
    }

    /* rete: index per os non R1, tensor per transitum */
    mv->rete     = contractio_rete_creare(piscina);
    index_ossis  = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    per (l = ZEPHYRUM; l < m; l++)
    {
        si (   finis_a[l] / IV != finis_b[l] / IV
            && !contractio_index_addere(mv->rete, II, &index_ossis[l]))
        {
            redde NIHIL;
        }
    }
    mv->elementa           = (i32*)piscina_allocare(piscina,
        (memoriae_index)c * magnitudo(i32));
    mv->valores            = (i32**)piscina_allocare(piscina,
        (memoriae_index)c * magnitudo(i32*));
    mv->initia_terminorum  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(c + I) * magnitudo(i32));
    mv->termini            =
        (TerminusVerticis*)piscina_allocare(piscina,
        (memoriae_index)c * XXXII * magnitudo(TerminusVerticis));
    mv->initia_terminorum[ZEPHYRUM] = ZEPHYRUM;
    per (k = ZEPHYRUM; k < c; k++)
    {
        i32 externa[IV];
        i32 indices[IV];
        i32 numerus_externorum;
        i32 numerus;
        i32 id;
        i32 j;

        numerus = _terminos_transitus(ora, k, finis_a, finis_b,
            mv->gyrationes, mv->termini + mv->initia_terminorum[k],
            externa, &numerus_externorum);
        mv->initia_terminorum[k + I] = mv->initia_terminorum[k]
            + numerus;
        per (j = ZEPHYRUM; j < numerus_externorum; j++)
        {
            indices[j] = index_ossis[externa[j]];
        }
        si (!contractio_tensor_addere(mv->rete, indices,
            numerus_externorum, &id))
        {
            redde NIHIL;
        }
        mv->elementa[k] = (i32)I << numerus_externorum;
        mv->valores[k]  = (i32*)piscina_allocare(piscina,
            (memoriae_index)mv->elementa[k] * magnitudo(i32));
        per (j = mv->initia_terminorum[k]; j < mv->initia_terminorum[k
            + I]; j++)
        {
            s32 e = mv->termini[j].exponens;

            si (e < ZEPHYRUM)
            {
                e = -e;
            }
            si (e > e_max)
            {
                e_max = e;
            }
        }
    }
    mv->exponens_maximus = e_max;
    mv->potentiae        = (i32*)piscina_allocare(piscina,
        (memoriae_index)(II * e_max + I) * magnitudo(i32));
    /* ordo optimus inter candidatos (contractio-plan T5b): idem
     * uncinus, tempus per punctum saepe multo minus in tricis */
    optiones.methodus = CONTRACTIO_OPTIMUS;
    optiones.octeti_maximi = octeti_maximi;
    mv->ordo = contractio_ordinare(mv->rete, &optiones, piscina);
    si (mv->ordo == NIHIL)
    {
        redde NIHIL;
    }
    mv->officina = contractio_officina_creare(mv->ordo, octeti_maximi,
        piscina);
    si (mv->officina == NIHIL)
    {
        redde NIHIL;
    }

    /* fenestra Kauffman [-c - 2|s_B| + 2, c + 2|s_A| - 2], ad classem
     * 3w modulo 4 rotundata */
    pater = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    per (l = ZEPHYRUM; l < m; l++)
    {
        pater[l] = l;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x = ora + IV * k;

        pater[_radix(pater, x[ZEPHYRUM])]  = _radix(pater, x[I]);
        pater[_radix(pater, x[II])]        = _radix(pater, x[III]);
    }
    per (l = ZEPHYRUM; l < m; l++)
    {
        si (_radix(pater, l) == l)
        {
            ansae_a++;
        }
        pater[l] = l;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x = ora + IV * k;

        pater[_radix(pater, x[ZEPHYRUM])]  = _radix(pater, x[III]);
        pater[_radix(pater, x[I])]         = _radix(pater, x[II]);
    }
    per (l = ZEPHYRUM; l < m; l++)
    {
        si (_radix(pater, l) == l)
        {
            ansae_b++;
        }
    }
    mv->scriptura = ZEPHYRUM;
    per (k = ZEPHYRUM; k < c; k++)
    {
        mv->scriptura += signa[k];
    }
    classis = (s32)_modulo_quattuor((s64)III * (s64)mv->scriptura);
    mv->imus = -(s32)c - II * (s32)ansae_b + II;
    mv->imus += (s32)_modulo_quattuor((s64)classis - (s64)mv->imus);
    mv->summus = (s32)c + II * (s32)ansae_a - II;
    mv->summus -= (s32)_modulo_quattuor((s64)mv->summus - (s64)classis);
    redde mv;
}

s32
laqueus_modulus_vertex_aestimare (
    LaqueusModulusVertex* mv,
                    i32  omega,
                    i32  p,
                    i32* punctum,
                    i32* valor)
{
    i32 a;
    i32 a_inversa;
    i32 w;
    i32 w_inversa;
    i32 d;
    i32 d_inversa;
    i32 z;
    s32 e_max;
    s32 j;
    i32 k;

    si (   mv == NIHIL || punctum == NIHIL || valor == NIHIL || p < V
        || p % IV != I)
    {
        redde INTERPOLATIO_ERRATUM;
    }
    si (p != mv->primus)
    {
        /* primus PROBATUS ante quaestionem g: modulus compositus 3 | p
         * nullum g habet (scansio ~p), alii falso accipiuntur */
        i32 g = II;

        si (!congruentia_est_primus(p))
        {
            redde INTERPOLATIO_ERRATUM;
        }
        /* i = g^((p-1)/4), g non-residuum quadraticum minimum (p
         * primus: dimidium residuorum, g parvum invenitur; limes g < p
         * custos tantum - sine eo modulus compositus, si probatio
         * umquam praeteritur, in aeternum quaereret) */
        dum (   g < p
             && congruentia_potentia(g, (i64)((p - I) / II), p) != p
                 - I)
        {
            g++;
        }
        si (g == p)
        {
            redde INTERPOLATIO_ERRATUM;
        }
        mv->radix_i  = congruentia_potentia(g, (i64)((p - I) / IV), p);
        mv->primus   = p;
        mv->mutationes_primi++;
    }
    w = omega % p;
    si (w == ZEPHYRUM || !congruentia_inversa(w, p, &w_inversa))
    {
        redde INTERPOLATIO_INUTILE;
    }

    /* potentiae omega^j, j in [-E, E] */
    e_max                 = mv->exponens_maximus;
    mv->potentiae[e_max]  = I;
    per (j = I; j <= e_max; j++)
    {
        mv->potentiae[e_max + j] = congruentia_multiplica(
            mv->potentiae[e_max + j - I], w, p);
        mv->potentiae[e_max - j] = congruentia_multiplica(
            mv->potentiae[e_max - j + I], w_inversa, p);
    }
    d = congruentia_adde(mv->potentiae[e_max + IV],
        mv->potentiae[e_max - IV], p);
    si (d == ZEPHYRUM)
    {
        redde INTERPOLATIO_INUTILE;
    }
    a = congruentia_multiplica(mv->radix_i, mv->potentiae[e_max + II],
        p);
    a_inversa = congruentia_multiplica(p - mv->radix_i,
        mv->potentiae[e_max - II], p);

    per (k = ZEPHYRUM; k < mv->transitus; k++)
    {
        per (j = ZEPHYRUM; j < (s32)mv->elementa[k]; j++)
        {
            mv->valores[k][j] = ZEPHYRUM;
        }
        per (j = (s32)mv->initia_terminorum[k];
             j < (s32)mv->initia_terminorum[k + I]; j++)
        {
            constans TerminusVerticis* t = &mv->termini[j];

            mv->valores[k][t->elementum] = congruentia_adde(
                mv->valores[k][t->elementum], congruentia_multiplica(
                t->potentia_a > ZEPHYRUM ? a : a_inversa,
                mv->potentiae[e_max + t->exponens], p), p);
        }
    }
    mv->aestimationes++;
    si (!contractio_exsequi_modulo(mv->rete, mv->ordo, mv->officina,
        (constans i32* constans*)mv->valores, p, &z))
    {
        redde INTERPOLATIO_ERRATUM;
    }
    (vacuum)congruentia_inversa(d, p, &d_inversa);
    *punctum  = a;
    *valor    = congruentia_multiplica(z, d_inversa, p);
    redde INTERPOLATIO_UTILE;
}

constans s32*
laqueus_modulus_vertex_gyrationes (
    constans LaqueusModulusVertex* mv)
{
    redde mv == NIHIL ? NIHIL : mv->gyrationes;
}

ContractioSumptus
laqueus_modulus_vertex_sumptus (
    constans LaqueusModulusVertex* mv)
{
    redde contractio_sumptus(mv == NIHIL ? NIHIL : mv->ordo);
}

constans ContractioOrdo*
laqueus_modulus_vertex_ordo (
    constans LaqueusModulusVertex* mv)
{
    redde mv == NIHIL ? NIHIL : mv->ordo;
}

i32
laqueus_modulus_vertex_limes (
    constans LaqueusModulusVertex* mv)
{
    redde mv == NIHIL ? ZEPHYRUM : mv->limes;
}

b32
laqueus_modulus_vertex_limes_exactus (
    constans LaqueusModulusVertex* mv,
                           Magnus* exitus)
{
    si (mv == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = mv->limes_exactus;
    redde VERUM;
}

vacuum
laqueus_modulus_vertex_vocationes (
    constans LaqueusModulusVertex* mv,
                              i32* aestimationes,
                              i32* primi)
{
    si (aestimationes != NIHIL)
    {
        *aestimationes = mv == NIHIL ? ZEPHYRUM : mv->aestimationes;
    }
    si (primi != NIHIL)
    {
        *primi = mv == NIHIL ? ZEPHYRUM : mv->mutationes_primi;
    }
}

/* aestimator interpolationis: omega = index + 2 */
interior s32
_aestimator_verticis (
     vacuum* contextus,
        i32  index,
        i32  p,
        i32* punctum,
        i32* valor)
{
    redde laqueus_modulus_vertex_aestimare(
        (LaqueusModulusVertex*)contextus, index + II, p, punctum,
        valor);
}

/* uncinus exactus: puncta in classe 3w (passus 4), limes = bita
 * minimi Pi gradus super graphos Tait (T5a: Sigma |c_k| <= arbores
 * extensae <= Pi gradus; numquam > 2c), primi 1 modulo 4 */
interior b32
_uncinus_per_rete (
    LaqueusModulusVertex* mv,
                 Piscina* piscina,
              Polynomium* exitus)
{
    InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();

    q.gradus_imus     = mv->imus;
    q.gradus_summus   = mv->summus;
    q.passus          = IV;
    q.limes_bitorum   = mv->limes;
    q.residuum_primi  = I;
    q.modulus_primi   = IV;
    q.puncta_addita   = II;
    q.conatus_maximi  = (i32)((mv->summus - mv->imus) / IV) + I + II
        + LXIV;
    redde interpolatio_polynomium(_aestimator_verticis, mv, &q, piscina,
        exitus);
}

b32
laqueus_modulus_vertex_uncinus (
    LaqueusModulusVertex* mv,
                 Piscina* piscina,
              Polynomium* exitus)
{
    si (mv == NIHIL || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    redde _uncinus_per_rete(mv, piscina, exitus);
}

interior vacuum
_sumptus_nullus (
    ContractioSumptus* sumptus)
{
    si (sumptus != NIHIL)
    {
        *sumptus = contractio_sumptus(NIHIL);
    }
}

b32
laqueus_uncinus_ex_pd_rete (
         constans i32* pd,
                  i32  transitus,
                  i64  octeti_maximi,
              Piscina* piscina,
           Polynomium* exitus,
    ContractioSumptus* sumptus)
{
    LaqueusModulusVertex* mv;

    si (exitus == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (transitus == ZEPHYRUM)
    {
        _sumptus_nullus(sumptus);
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    mv = laqueus_modulus_vertex_creare(pd, transitus, octeti_maximi,
        piscina);
    si (mv == NIHIL || !_uncinus_per_rete(mv, piscina, exitus))
    {
        redde FALSUM;
    }
    si (sumptus != NIHIL)
    {
        *sumptus = contractio_sumptus(mv->ordo);
    }
    redde VERUM;
}

b32
laqueus_jones_ex_pd_rete (
         constans i32* pd,
                  i32  transitus,
                  i64  octeti_maximi,
              Piscina* piscina,
           Polynomium* exitus,
    ContractioSumptus* sumptus)
{
    LaqueusModulusVertex* mv;
              Polynomium  uncinus;

    si (exitus == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (transitus == ZEPHYRUM)
    {
        _sumptus_nullus(sumptus);
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    mv = laqueus_modulus_vertex_creare(pd, transitus, octeti_maximi,
        piscina);
    si (   mv == NIHIL || !_uncinus_per_rete(mv, piscina, &uncinus)
        || !_jones_ex_uncino(uncinus, mv->scriptura, piscina, exitus))
    {
        redde FALSUM;
    }
    si (sumptus != NIHIL)
    {
        *sumptus = contractio_sumptus(mv->ordo);
    }
    redde VERUM;
}
