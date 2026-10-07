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

b32
diagramma_uncinus (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
           i32  c = d.numerus;
           i32  m = II * c;
           i32* ora;        /* codex PD: 4 ora per transitum */
           i32* positio;    /* positio passus 2k / 2k+1 in percursu */
           i32* pater;
           s64* numeri;     /* [exponens + c][ansae] */
           i32  ansae_liberae = ZEPHYRUM;
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
    per (k = ZEPHYRUM; k < d.laqueus.componentes; k++)
    {
        si (d.initia_percursus[k + I] == d.initia_percursus[k])
        {
            ansae_liberae++;
        }
    }
    latitudo = c + d.laqueus.componentes + I;
    ora      = (i32*)piscina_allocare(piscina, (memoriae_index)(IV * c
        + I) * magnitudo(i32));
    positio  = (i32*)piscina_allocare(piscina, (memoriae_index)(m + I)
        * magnitudo(i32));
    pater    = (i32*)piscina_allocare(piscina, (memoriae_index)(m + I)
        * magnitudo(i32));
    numeri   = (s64*)piscina_allocare(piscina, (memoriae_index)(II * c
        + I) * (memoriae_index)latitudo * magnitudo(s64));
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
            i32* x = ora + IV * k;

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
diagramma_jones (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium uncinus;
    Polynomium factor;
    Polynomium f;
           s32 w = diagramma_scriptura(d);

    /* catena componentium numeri paris: V in t^(1/2) Z[t, t^-1] -
     * exponentes dimidii certi, uncinus (2^c status) frustra */
    si (d.laqueus.componentes % II == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* f = (-A^3)^-w D uncinatum = (-1)^w A^-3w D uncinatum */
    si (   !diagramma_uncinus(d, piscina, &uncinus)
        || !polynomium_monomium(magnus_ex_s64((w % II == ZEPHYRUM) ? I
            : -I), -III * w, piscina, &factor)
        || !polynomium_multiplica(factor, uncinus, piscina, &f))
    {
        redde FALSUM;
    }
    redde polynomium_contrahe(f, -IV, piscina, exitus);
}

b32
diagramma_alexander (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    constans Anulus* p = &ANULUS_POLYNOMIORUM;
                i32  c = d.numerus;
                i32  m = II * c;
                i32* supra_arcus;
                i32* in_arcus;
                i32* ex_arcus;
                i32  initium  = ZEPHYRUM;
                i32  arcus    = ZEPHYRUM;
                i32  q;
                i32  k;
             Matrix  matrix_plena;
             Matrix  minor;
         Polynomium  delta;
         Polynomium  unum;
         Polynomium  t    = polynomium_nullum();
         Polynomium  t_1  = polynomium_nullum();
         Polynomium  minus_unum = polynomium_constans(magnus_ex_s64(-I),
             piscina);

    unum = polynomium_constans(magnus_ex_s64(I), piscina);
    si (d.laqueus.componentes != I)
    {
        redde FALSUM;
    }
    si (c == ZEPHYRUM)
    {
        *exitus = unum;
        redde VERUM;
    }
    (vacuum)polynomium_monomium(magnus_ex_s64(I), I, piscina, &t);
    (vacuum)polynomium_monomium(magnus_ex_s64(I), -I, piscina, &t_1);
    supra_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)c
        * magnitudo(i32));
    in_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)c
        * magnitudo(i32));
    ex_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)c
        * magnitudo(i32));
    /* incipe post passum infra primum: arcus 0 ibi incipit */
    dum (d.percursus[initium] % II == ZEPHYRUM)
    {
        initium++;
    }
    per (q = I; q <= m; q++)
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
    (vacuum)matrix_nulla(p, c, c, piscina, &matrix_plena);
    per (k = ZEPHYRUM; k < c; k++)
    {
        Polynomium valores[III];
               i32 columnae[III];
               i32 r;

        si (d.transitus[k].signum > ZEPHYRUM)
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
