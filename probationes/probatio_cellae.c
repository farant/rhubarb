/* probatio_cellae.c - probationes cellarum (project-specs/cellae-plan.md)
 *
 * T2 catalogus: clausura ordine D82/D84 contra clausuram fluitantem
 * eorum (in hac probatione rescriptam) elementum per elementum: ordo,
 * valores (decimales certi extensionis), profunditates, torsiones;
 * zeta_8 (XXIV) et zeta_12 truncatus ad MMMMXCVI (D82). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "extensio.h"
#include "quaternio.h"
#include "cellae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

interior Quaternio
_q (
       constans Anulus* anulus,
    constans character* textus,
               Piscina* piscina)
{
    Quaternio q;

    si (!quaternio_ex_chorda(anulus, chorda_ex_literis(textus, piscina),
        piscina, &q))
    {
        imprimere("FRACTA: quaternio malus: %s\n", textus);
        q.anulus = NIHIL;
        q.partes = NIHIL;
    }
    redde q;
}

/* generatores D82/D84: (g1, g1*, g2, g2*) */
interior vacuum
_generatores (
       constans Anulus* anulus,
    constans character* co,
    constans character* si_textus,
    constans character* si_negatum,
               Piscina* piscina,
             Quaternio* g)
{
    character textus[128];

    sprintf(textus, "[%s, %s, 0, 0]", co, si_textus);
    g[ZEPHYRUM] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, %s, 0, 0]", co, si_negatum);
    g[I] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, 0, 0, %s]", co, si_negatum);
    g[II] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, 0, 0, %s]", co, si_textus);
    g[III] = _q(anulus, textus, piscina);
}

hic_manens constans s32 TORSIONES[IV] = { I, -(s32)I, I, -(s32)I };


/* ==================================================
 * Clausura fluitans D82/D84 (rescripta)
 * ================================================== */

#define F_MAX 4096

nomen structura { duplex a, b, c, d; } FQ;

hic_manens FQ  f_cat[F_MAX];
hic_manens i32 f_prof[F_MAX];
hic_manens s32 f_torsio[F_MAX];
hic_manens i32 f_n;

interior s32
_f_invenire (
    constans FQ* q)
{
    i32 i;

    per (i = ZEPHYRUM; i < f_n; i++)
    {
        si (   fabs(f_cat[i].a - q->a) < 1e-10
            && fabs(f_cat[i].b - q->b)
            < 1e-10 && fabs(f_cat[i].c - q->c) < 1e-10
            && fabs(f_cat[i].d - q->d) < 1e-10)
        {
            redde (s32)i;
        }
        si (   fabs(f_cat[i].a + q->a) < 1e-10
            && fabs(f_cat[i].b + q->b)
            < 1e-10 && fabs(f_cat[i].c + q->c) < 1e-10
            && fabs(f_cat[i].d + q->d) < 1e-10)
        {
            redde (s32)i;
        }
    }
    redde -(s32)I;
}

interior vacuum
_f_clausura (
    duplex angulus,
       i32 limes)
{
        FQ g[IV];
    duplex co = cos(angulus);
    duplex sn = sin(angulus);
       i32 prior;
       i32 i;
       i32 k;
       i32 gradus;
       i32 r;

    g[ZEPHYRUM].a = co; g[ZEPHYRUM].b = sn; g[ZEPHYRUM].c = 0;
    g[ZEPHYRUM].d = 0;
    g[I] = g[ZEPHYRUM]; g[I].b = -sn;
    g[II].a = co; g[II].b = 0; g[II].c = 0; g[II].d = -sn;
    g[III] = g[II]; g[III].d = sn;
    f_cat[ZEPHYRUM].a = 1; f_cat[ZEPHYRUM].b = 0; f_cat[ZEPHYRUM].c = 0;
    f_cat[ZEPHYRUM].d = 0;
    f_prof[ZEPHYRUM] = ZEPHYRUM;
    f_torsio[ZEPHYRUM] = ZEPHYRUM;
    f_n = I;
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (_f_invenire(&g[k]) < 0 && f_n < limes)
        {
            f_cat[f_n]     = g[k];
            f_prof[f_n]    = ZEPHYRUM;
            f_torsio[f_n]  = TORSIONES[k];
            f_n++;
        }
    }
    gradus  = I;
    r       = I;
    fac
    {
        prior = f_n;
        per (i = ZEPHYRUM; i < prior; i++)
        {
            per (k = ZEPHYRUM; k < IV; k++)
            {
                constans FQ* p = &f_cat[i];
                constans FQ* h = &g[k];
                         FQ  pr;

                pr.a = p->a * h->a - p->b * h->b - p->c * h->c
                    - p->d * h->d;
                pr.b = p->a * h->b + p->b * h->a + p->c * h->d
                    - p->d * h->c;
                pr.c = p->a * h->c - p->b * h->d + p->c * h->a
                    + p->d * h->b;
                pr.d = p->a * h->d + p->b * h->c - p->c * h->b
                    + p->d * h->a;
                si (_f_invenire(&pr) < 0 && f_n < limes)
                {
                    f_cat[f_n]     = pr;
                    f_prof[f_n]    = r;
                    f_torsio[f_n]  = f_torsio[i] + TORSIONES[k];
                    f_n++;
                }
            }
        }
        si (f_n > prior)
        {
            gradus++;
        }
        r++;
    } dum (f_n > prior && gradus < XVI);
}

/* pars k exacta ad duplicem per decimales certos */
interior duplex
_ad_duplicem (
    Quaternio  q,
          i32  k,
      Piscina* piscina)
{
    constans Algebraicus* a = (constans Algebraicus*)quaternio_pars(q,
        k);
                  chorda textus = algebraicus_ad_ostendendum(*a, XVIII,
                      piscina);
            character alveus[64];

    memcpy(alveus, textus.datum, (size_t)textus.mensura);
    alveus[textus.mensura] = '\0';
    redde strtod(alveus, NIHIL);
}

/* exactus = fluitans D82/D84: ordo, valores, profunditates, torsiones */
interior b32
_concordat (
    constans Catalogus* c,
               Piscina* piscina)
{
    i32 i;

    si ((i32)catalogus_numerus(c) != f_n)
    {
        imprimere("  numerus %d contra %d\n", (int)catalogus_numerus(c),
            (int)f_n);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < f_n; i++)
    {
        Quaternio q          = catalogus_elementum(c, i);
        PiscinaNotatio nota  = piscina_notare(piscina);
        b32 bene = fabs(_ad_duplicem(q, ZEPHYRUM, piscina) - f_cat[i].a)
            < 1e-12 && fabs(_ad_duplicem(q, I, piscina) - f_cat[i].b)
            < 1e-12 && fabs(_ad_duplicem(q, II, piscina) - f_cat[i].c)
            < 1e-12 && fabs(_ad_duplicem(q, III, piscina) - f_cat[i].d)
            < 1e-12 && catalogus_profunditas(c, i) == f_prof[i]
            && catalogus_torsio(c, i) == f_torsio[i];

        piscina_reficere(piscina, nota);
        si (!bene)
        {
            imprimere("  discrepantia in elemento %d\n", (int)i);
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * PROBARE
 * ================================================== */

interior vacuum
_probare_zeta_octo (
    CredoContextus* c)
{
            Piscina* piscina  = c->piscina;
           Extensio* k2       = extensio_quadratica(II, piscina);
    constans Anulus* q2       = extensio_anulus(k2);
          Quaternio  g[IV];
          Catalogus* cat              = NIHIL;
                i32  distributio[IV]  = { 0, 0, 0, 0 };
                i32  nulla            = ZEPHYRUM;
                i32  i;

    imprimere("\n--- Clausura zeta_8 (D84) ---\n");
    _generatores(q2, "a/2", "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, (i32)F_MAX, XVI,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), XXIV);
    CREDO_AEQUALIS_I32 (catalogus_gradus(cat), IV);
    CREDO_FALSUM (catalogus_truncatus(cat));
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        distributio[catalogus_profunditas(cat, i)]++;
        nulla += catalogus_nullum(cat, i) ? I : ZEPHYRUM;
    }
    CREDO_VERUM (distributio[ZEPHYRUM] == V && distributio[I] == X
        && distributio[II] == VIII && distributio[III] == I);
    CREDO_AEQUALIS_I32 (nulla, IX);
    _f_clausura(M_PI / 4.0, (i32)F_MAX);
    CREDO_VERUM (_concordat(cat, piscina));
}

interior vacuum
_probare_zeta_duodecim (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                   Extensio* k3 = extensio_quadratica(III,
                       piscina);
            constans Anulus* q3 = extensio_anulus(k3);
                  Quaternio  g[IV];
                  Catalogus* cat = NIHIL;
                        i32  distributio[XVI];
    hic_manens constans i32  D82[IX] = { 5, 12, 34, 76, 148, 289, 576,
        1152, 1804 };
    i32 i;
    b32 bene = VERUM;

    imprimere("\n--- Clausura zeta_12 truncata (D82) ---\n");
    _generatores(q3, "a/2", "1/2", "-1/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, (i32)F_MAX, XVI,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), (i32)F_MAX);
    CREDO_VERUM (catalogus_truncatus(cat));
    per (i = ZEPHYRUM; i < XVI; i++) distributio[i] = ZEPHYRUM;
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        distributio[catalogus_profunditas(cat, i)]++;
    }
    per (i = ZEPHYRUM; i < IX; i++)
    {
        bene = bene && distributio[i] == D82[i];
    }
    CREDO_VERUM (bene);
    _f_clausura(M_PI / 6.0, (i32)F_MAX);
    CREDO_VERUM (_concordat(cat, piscina));
}

interior vacuum
_probare_limites (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                   Extensio* k2 = extensio_quadratica(II,
                       piscina);
            constans Anulus* q2 = extensio_anulus(k2);
                  Quaternio  g[IV];
                  Catalogus* cat           = NIHIL;
                  Catalogus* pars          = NIHIL;
    hic_manens constans i32  INDICES[III]  = { 7, 0, 3 };
    hic_manens constans i32  MALI[II]      = { 2, 2 };

    imprimere("\n--- Limes, gradus, pars, refutationes ---\n");
    _generatores(q2, "a/2", "a/2", "-a/2", piscina, g);
    /* limes X in medio gradus I: truncatus */
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, X, ZEPHYRUM,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), X);
    CREDO_VERUM (catalogus_truncatus(cat));
    /* gradus maximi II: gradus 0 et I (V + X), non truncatus */
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, II,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), XV);
    CREDO_FALSUM (catalogus_truncatus(cat));
    /* pars: ordo indicum servatur */
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM,
        ZEPHYRUM, piscina, &cat));
    CREDO_NECESSE (catalogus_pars(cat, INDICES, III, piscina, &pars));
    CREDO_AEQUALIS_I32 (catalogus_numerus(pars), III);
    CREDO_VERUM (quaternio_aequalis(catalogus_elementum(pars, ZEPHYRUM),
        catalogus_elementum(cat, VII)));
    CREDO_VERUM (catalogus_profunditas(pars, II)
        == catalogus_profunditas(cat, III));
    CREDO_FALSUM (catalogus_pars(cat, MALI, II, piscina, &pars));
    CREDO_FALSUM (catalogus_pars(cat, INDICES, ZEPHYRUM, piscina,
        &pars));
    /* extra: anulus NIHIL, profunditas -1 */
    CREDO_VERUM (catalogus_elementum(cat, C).anulus == NIHIL);
    CREDO_AEQUALIS_S32 ((s32)catalogus_profunditas(cat, C), -(s32)I);
    /* refutationes: numerus 0; anuli mixti; anulus sine ordine */
    CREDO_FALSUM (catalogus_clausura(g, TORSIONES, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM, piscina, &cat));
    {
         Extensio* k3 = extensio_quadratica(III, piscina);
        Quaternio  mixti[II];

        mixti[ZEPHYRUM] = g[ZEPHYRUM];
        mixti[I] = _q(extensio_anulus(k3), "[a/2, 1/2, 0, 0]", piscina);
        CREDO_FALSUM (catalogus_clausura(mixti, TORSIONES, II, ZEPHYRUM,
            ZEPHYRUM, piscina, &cat));
        mixti[ZEPHYRUM] = _q(&ANULUS_POLYNOMIORUM, "[0, t, 0, 0]",
            piscina);
        CREDO_FALSUM (catalogus_clausura(mixti, TORSIONES, I, ZEPHYRUM,
            ZEPHYRUM, piscina, &cat));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "zeta_8", _probare_zeta_octo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "zeta_12", _probare_zeta_duodecim, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites", _probare_limites, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cellae", SECTIONES);
}
