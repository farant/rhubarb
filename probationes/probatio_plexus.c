/* probatio_plexus.c - probationes plexus (project-specs/plexus-plan.md)
 *
 * T1: verba, permutatio, actio in F_n (Artin), aequalitas. Sectio
 * "nervus" (nervus-plan T3): plexus_nervus contra ansa_agere. Oracula a
 * codice independentia:
 * (1) imagines manu computatae (sigma_1, sigma_1^-1, sigma_1 sigma_2);
 * (2) actio per SUBSTITUTIONEM litterarum a dextra (algorithmus alius
 *     quam bibliothecae compositio imaginum a sinistra), in hac
 *     probatione scripta;
 * (3) theoremata: productum x_1 ... x_n fixum (Artin); phi_p(x_k)
 *     coniugatum x_{imago[k]}; relationes braidarum; Delta^2 centralis
 *     et non trivialis; Delta sigma_i Delta^-1 = sigma_{n-i} (Garside);
 *     Delta^2 = (sigma_1 ... sigma_{n-1})^n;
 * (4) verba per relationes rescripta aequalia manent; sigma_1^2 et
 *     commutator [sigma_1^2, sigma_2^2] (permutatio et summa nullae)
 *     non trivialia.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "sors.h"
#include "magnus.h"
#include "chorda_aedificator.h"
#include "plexus.h"
#include "polynomium.h"
#include "fractio.h"
#include "matrix.h"
#include "anulus.h"
#include "radices.h"
#include "tabula_nodorum.h"
#include "cyclotomia.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIMES_MAGNUS  100000000UL

interior Plexus
_p (
                   i32  fila,
    constans character* textus,
               Piscina* piscina)
{
    Plexus p;

    si (!plexus_ex_chorda(fila, chorda_ex_literis(textus, piscina),
        piscina, &p))
    {
        imprimere("FRACTA: plexus malus: %s\n", textus);
        p.fila       = fila;
        p.longitudo  = ZEPHYRUM;
        p.litterae   = NIHIL;
    }
    redde p;
}

interior b32
_textus (
                chorda  c,
    constans character* textus)
{
    redde chorda_aequalis_literis(c, textus);
}

interior b32
_aequales (
     Plexus  p,
     Plexus  q,
    Piscina* piscina)
{
    b32 aequales = FALSUM;

    si (!plexus_aequales(p, q, LIMES_MAGNUS, piscina, &aequales))
    {
        imprimere("FRACTA: aequalitas non decisa\n");
        redde FALSUM;
    }
    redde aequales;
}

interior b32
_imago_textui (
                Plexus  p,
                   i32  j,
    constans character* textus,
               Piscina* piscina)
{
    VerbumLiberum w;

    si (!plexus_imago_libera(p, j, LIMES_MAGNUS, piscina, &w))
    {
        redde FALSUM;
    }
    redde _textus(verbum_liberum_ad_chordam(w, piscina), textus);
}

interior Plexus
_fortuitus (
        Sors* s,
         i32  fila,
         i32  longitudo,
     Piscina* piscina)
{
    s32* litterae = (s32*)piscina_allocare(piscina,
        (memoriae_index)(longitudo + I) * magnitudo(s32));
    Plexus p;
       i32 k;

    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        s32 i = (s32)sors_intra(s, fila - I) + I;

        litterae[k] = sors_intra(s, II) == ZEPHYRUM ? i : -i;
    }
    (vacuum)plexus_creare(fila, litterae, longitudo, piscina, &p);
    redde p;
}


/* ==================================================
 * Oraculum: actio per substitutionem (independens)
 * ================================================== */

/* acervus reductionis liberae (scriptus hic, non bibliothecae) */
interior i32
_impelle (
    s32* acervus,
    i32  altitudo,
    s32  littera)
{
    si (altitudo > ZEPHYRUM && acervus[altitudo - I] == -littera)
    {
        redde altitudo - I;
    }
    acervus[altitudo] = littera;
    redde altitudo + I;
}

/* imago litterae x_j^e (e = signum litterae) sub phi_a, a una littera
 * plexus; scribitur in imago[], numerus redditur */
interior i32
_imago_litterae (
    s32  a,
    s32  littera,
    s32* imago)
{
    s32 i = a > ZEPHYRUM ? a : -a;
    s32 j = littera > ZEPHYRUM ? littera : -littera;
    s32 forma[III];
    i32 numerus;
    i32 k;

    si (a > ZEPHYRUM && j == i)
    {
        forma[ZEPHYRUM]  = i;
        forma[I]         = i + I;
        forma[II]        = -i;
        numerus          = III;
    }
    alioquin si (a > ZEPHYRUM && j == i + I)
    {
        forma[ZEPHYRUM]  = i;
        numerus          = I;
    }
    alioquin si (a < ZEPHYRUM && j == i)
    {
        forma[ZEPHYRUM]  = i + I;
        numerus          = I;
    }
    alioquin si (a < ZEPHYRUM && j == i + I)
    {
        forma[ZEPHYRUM]  = -(i + I);
        forma[I]         = i;
        forma[II]        = i + I;
        numerus          = III;
    }
    alioquin
    {
        forma[ZEPHYRUM]  = j;
        numerus          = I;
    }
    si (littera > ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            imago[k] = forma[k];
        }
    }
    alioquin
    {
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            imago[k] = -forma[numerus - I - k];
        }
    }
    redde numerus;
}

/* phi_b(w) per substitutionem, litteris b a DEXTRA: phi_{a_1} o ... o
 * phi_{a_m} = phi_{a_m} primum. Longitudo maxima 'capacitas'. */
interior i32
_agere_substitutione (
           Plexus  b,
     constans s32* w,
              i32  longitudo,
              s32* exitus,
              i32  capacitas,
          Piscina* piscina)
{
    s32* fons     = (s32*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(s32));
    i32 numerus  = longitudo;
    s32 m;
    i32 k;

    memcpy(fons, w, (size_t)longitudo * magnitudo(s32));
    per (m = (s32)b.longitudo - I; m >= ZEPHYRUM; m--)
    {
        i32 altitudo = ZEPHYRUM;

        per (k = ZEPHYRUM; k < numerus; k++)
        {
            s32 imago[III];
            i32 n = _imago_litterae(b.litterae[m], fons[k], imago);
            i32 t;

            per (t = ZEPHYRUM; t < n; t++)
            {
                si (altitudo >= capacitas)
                {
                    redde 0xFFFFFFFFUL;
                }
                altitudo = _impelle(exitus, altitudo, imago[t]);
            }
        }
        memcpy(fons, exitus, (size_t)altitudo * magnitudo(s32));
        numerus = altitudo;
    }
    memcpy(exitus, fons, (size_t)numerus * magnitudo(s32));
    redde numerus;
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior vacuum
_probare_verba (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Plexus  p;
     Plexus  q;
     Plexus  r;
        s32  litterae[IV];

    p = _p(IV, "1 -2 3 -1", piscina);
    CREDO_AEQUALIS_I32 (p.fila, IV);
    CREDO_AEQUALIS_I32 (p.longitudo, IV);
    CREDO_VERUM (_textus(plexus_ad_chordam(p, piscina), "1 -2 3 -1"));
    CREDO_VERUM (_textus(plexus_ad_chordam(plexus_inversum(p, piscina),
        piscina), "1 -3 2 -1"));
    CREDO_VERUM (plexus_summa_exponentium(p) == ZEPHYRUM);
    CREDO_VERUM (plexus_summa_exponentium(_p(III, "1 1 2", piscina))
        == III);
    /* spatia multiplicia et textus vacuus */
    CREDO_VERUM (_textus(plexus_ad_chordam(_p(III, "  1   -2 ",
        piscina),
        piscina), "1 -2"));
    q = _p(III, "", piscina);
    CREDO_AEQUALIS_I32 (q.longitudo, ZEPHYRUM);
    CREDO_VERUM (_textus(plexus_ad_chordam(q, piscina), ""));
    /* refutationes: exitus non tangitur */
    r.fila = XCIX;
    CREDO_FALSUM (plexus_ex_chorda(III, chorda_ex_literis("3", piscina),
        piscina, &r));
    CREDO_FALSUM (plexus_ex_chorda(III, chorda_ex_literis("0", piscina),
        piscina, &r));
    CREDO_FALSUM (plexus_ex_chorda(III, chorda_ex_literis("-3",
        piscina),
        piscina, &r));
    CREDO_FALSUM (plexus_ex_chorda(III, chorda_ex_literis("1x",
        piscina),
        piscina, &r));
    CREDO_FALSUM (plexus_ex_chorda(III, chorda_ex_literis("- 1",
        piscina), piscina, &r));
    CREDO_FALSUM (plexus_ex_chorda(III, chorda_ex_literis("1,2",
        piscina), piscina, &r));
    CREDO_FALSUM (plexus_ex_chorda(I, chorda_ex_literis("", piscina),
        piscina, &r));
    CREDO_FALSUM (plexus_ex_chorda(III,
        chorda_ex_literis("99999999999999999999", piscina), piscina,
        &r));
    litterae[ZEPHYRUM]  = I;
    litterae[I]         = -II;
    CREDO_FALSUM (plexus_creare(II, litterae, II, piscina, &r));
    CREDO_FALSUM (plexus_creare(PLEXUS_FILA_MAXIMA + I, litterae, I,
        piscina, &r));
    CREDO_FALSUM (plexus_creare(III, NIHIL, I, piscina, &r));
    CREDO_VERUM (plexus_creare(III, NIHIL, ZEPHYRUM, piscina, &r)
        && r.longitudo == ZEPHYRUM);
    r.fila = XCIX;
    CREDO_FALSUM (plexus_iungere(_p(III, "1", piscina), _p(IV, "1",
        piscina), piscina, &r));
    CREDO_AEQUALIS_I32 (r.fila, XCIX);
    /* iungere */
    CREDO_VERUM (plexus_iungere(_p(III, "1 2", piscina), _p(III, "-1",
        piscina), piscina, &r)
        && _textus(plexus_ad_chordam(r, piscina), "1 2 -1"));
    /* reductio libera tantum */
    CREDO_VERUM (_textus(plexus_ad_chordam(plexus_reducere(_p(III,
        "1 -1", piscina), piscina), piscina), ""));
    CREDO_VERUM (_textus(plexus_ad_chordam(plexus_reducere(_p(IV,
        "1 2 -2 -1 3", piscina), piscina), piscina), "3"));
    CREDO_VERUM (_textus(plexus_ad_chordam(plexus_reducere(_p(III,
        "1 -2 2 1", piscina), piscina), piscina), "1 1"));
    /* relationes non adhibentur: 1 2 1 -2 -1 -2 manet */
    CREDO_VERUM (_textus(plexus_ad_chordam(plexus_reducere(_p(III,
        "1 2 1 -2 -1 -2", piscina), piscina), piscina),
        "1 2 1 -2 -1 -2"));
    /* dimidia torsio */
    CREDO_VERUM (plexus_dimidia_torsio(IV, piscina, &r)
        && _textus(plexus_ad_chordam(r, piscina), "1 2 3 1 2 1"));
    CREDO_VERUM (plexus_dimidia_torsio(II, piscina, &r)
        && _textus(plexus_ad_chordam(r, piscina), "1"));
    CREDO_FALSUM (plexus_dimidia_torsio(I, piscina, &r));
    CREDO_FALSUM (plexus_dimidia_torsio(PLEXUS_FILA_DIMIDIAE_TORSIONIS
        + I, piscina, &r));
}

interior vacuum
_probare_permutationem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  imago[VIII];
        i32  imago_p[VIII];
        i32  imago_q[VIII];
        i32  imago_iuncti[VIII];
     Plexus  delta;
     Plexus  torsio_plena;
        i32  n;
        i32  k;
        i32  t;

    plexus_permutatio(_p(IV, "2", piscina), imago);
    CREDO_VERUM (imago[ZEPHYRUM] == ZEPHYRUM && imago[I] == II
        && imago[II] == I && imago[III] == III);
    plexus_permutatio(_p(IV, "-2", piscina), imago);
    CREDO_VERUM (imago[I] == II && imago[II] == I);
    /* sigma_1 sigma_2: filum 0 -> positio 1 -> positio 2 */
    plexus_permutatio(_p(III, "1 2", piscina), imago);
    CREDO_VERUM (imago[II] == ZEPHYRUM && imago[ZEPHYRUM] == I
        && imago[I] == II);
    /* homomorphismus: imago_iuncti[q] = imago_p[imago_q[q]] */
    sors_seminare(&s, CCCXXXIII, I);
    per (t = ZEPHYRUM; t < CC; t++)
    {
        Plexus p;
        Plexus q;
        Plexus iunctus;
           b32 bene = VERUM;

        n = II + sors_intra(&s, VII);
        p = _fortuitus(&s, n, sors_intra(&s, XII), piscina);
        q = _fortuitus(&s, n, sors_intra(&s, XII), piscina);
        (vacuum)plexus_iungere(p, q, piscina, &iunctus);
        plexus_permutatio(p, imago_p);
        plexus_permutatio(q, imago_q);
        plexus_permutatio(iunctus, imago_iuncti);
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (imago_iuncti[k] != imago_p[imago_q[k]])
            {
                bene = FALSUM;
            }
        }
        CREDO_VERUM (bene);
    }
    /* Delta invertit ordinem; Delta^2 pura */
    per (n = II; n <= VIII; n++)
    {
        b32 conversa  = VERUM;
        b32 pura      = VERUM;

        (vacuum)plexus_dimidia_torsio(n, piscina, &delta);
        (vacuum)plexus_iungere(delta, delta, piscina, &torsio_plena);
        plexus_permutatio(delta, imago);
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (imago[k] != n - I - k)
            {
                conversa = FALSUM;
            }
        }
        plexus_permutatio(torsio_plena, imago);
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (imago[k] != k)
            {
                pura = FALSUM;
            }
        }
        CREDO_VERUM (conversa);
        CREDO_VERUM (pura);
    }
}

interior vacuum
_probare_imagines_manu (
    CredoContextus* c)
{
          Piscina* piscina = c->piscina;
    VerbumLiberum  w;
           Plexus  p       = _p(III, "1", piscina);

    CREDO_VERUM (_imago_textui(p, I, "1 2 -1", piscina));
    CREDO_VERUM (_imago_textui(p, II, "1", piscina));
    CREDO_VERUM (_imago_textui(p, III, "3", piscina));
    p = _p(III, "-1", piscina);
    CREDO_VERUM (_imago_textui(p, I, "2", piscina));
    CREDO_VERUM (_imago_textui(p, II, "-2 1 2", piscina));
    /* phi_{s1 s2} = phi_{s1} o phi_{s2}:
     *   x1 -> phi_{s1}(x1) = x1 x2 x1^-1
     *   x2 -> phi_{s1}(x2 x3 x2^-1) = x1 x3 x1^-1
     *   x3 -> phi_{s1}(x2) = x1 */
    p = _p(III, "1 2", piscina);
    CREDO_VERUM (_imago_textui(p, I, "1 2 -1", piscina));
    CREDO_VERUM (_imago_textui(p, II, "1 3 -1", piscina));
    CREDO_VERUM (_imago_textui(p, III, "1", piscina));
    /* identitas */
    CREDO_VERUM (_imago_textui(_p(III, "", piscina), II, "2", piscina));
    /* refutationes */
    w.longitudo = XCIX;
    CREDO_FALSUM (plexus_imago_libera(p, ZEPHYRUM, LIMES_MAGNUS,
        piscina,
        &w));
    CREDO_FALSUM (plexus_imago_libera(p, IV, LIMES_MAGNUS, piscina,
        &w));
    CREDO_AEQUALIS_I32 (w.longitudo, XCIX);
}

interior vacuum
_probare_relationes (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        i32  n;
        i32  i;
        i32  j;
     Plexus  delta;
     Plexus  torsio_plena;
     Plexus  p;
     Plexus  q;
        b32  aequales;

    per (n = III; n <= VI; n++)
    {
        per (i = I; i < n; i++)
        {
            s32 a[III];

            /* sigma sigma^-1 = 1 */
            a[ZEPHYRUM]  = (s32)i;
            a[I]         = -(s32)i;
            (vacuum)plexus_creare(n, a, II, piscina, &p);
            CREDO_VERUM (_aequales(p, _p(n, "", piscina), piscina));
            /* sigma_i != sigma_i^-1, sigma_i^2 != 1 */
            (vacuum)plexus_creare(n, a, I, piscina, &p);
            (vacuum)plexus_creare(n, &a[I], I, piscina, &q);
            CREDO_FALSUM (_aequales(p, q, piscina));
            a[I] = (s32)i;
            (vacuum)plexus_creare(n, a, II, piscina, &p);
            CREDO_FALSUM (_aequales(p, _p(n, "", piscina), piscina));
            per (j = I; j < n; j++)
            {
                s32 b[III];
                s32 d[III];

                si (j == i + I)
                {
                    /* s_i s_j s_i = s_j s_i s_j */
                    b[ZEPHYRUM]  = (s32)i;
                    b[I]         = (s32)j;
                    b[II]        = (s32)i;
                    d[ZEPHYRUM]  = (s32)j;
                    d[I]         = (s32)i;
                    d[II]        = (s32)j;
                    (vacuum)plexus_creare(n, b, III, piscina, &p);
                    (vacuum)plexus_creare(n, d, III, piscina, &q);
                    CREDO_VERUM (_aequales(p, q, piscina));
                    /* sigma_i sigma_j != sigma_j sigma_i */
                    (vacuum)plexus_creare(n, b, II, piscina, &p);
                    (vacuum)plexus_creare(n, d, II, piscina, &q);
                    CREDO_FALSUM (_aequales(p, q, piscina));
                }
                si (j >= i + II)
                {
                    b[ZEPHYRUM]  = (s32)i;
                    b[I]         = (s32)j;
                    d[ZEPHYRUM]  = (s32)j;
                    d[I]         = (s32)i;
                    (vacuum)plexus_creare(n, b, II, piscina, &p);
                    (vacuum)plexus_creare(n, d, II, piscina, &q);
                    CREDO_VERUM (_aequales(p, q, piscina));
                }
            }
        }
        /* Delta^2 centralis, non trivialis; Delta sigma_i Delta^-1 =
         * sigma_{n-i}; Delta^2 = (sigma_1 ... sigma_{n-1})^n */
        (vacuum)plexus_dimidia_torsio(n, piscina, &delta);
        (vacuum)plexus_iungere(delta, delta, piscina, &torsio_plena);
        CREDO_FALSUM (_aequales(torsio_plena, _p(n, "", piscina),
            piscina));
        per (i = I; i < n; i++)
        {
               s32 a = (s32)i;
               s32 b = (s32)(n - i);
            Plexus sigma;
            Plexus sigma_conversa;
            Plexus sinistra;
            Plexus dextra;

            (vacuum)plexus_creare(n, &a, I, piscina, &sigma);
            (vacuum)plexus_creare(n, &b, I, piscina, &sigma_conversa);
            (vacuum)plexus_iungere(torsio_plena, sigma, piscina,
                &sinistra);
            (vacuum)plexus_iungere(sigma, torsio_plena, piscina,
                &dextra);
            CREDO_VERUM (_aequales(sinistra, dextra, piscina));
            (vacuum)plexus_iungere(delta, sigma, piscina, &sinistra);
            (vacuum)plexus_iungere(sinistra, plexus_inversum(delta,
                piscina), piscina, &sinistra);
            CREDO_VERUM (_aequales(sinistra, sigma_conversa, piscina));
        }
        {
            s32* litterae = (s32*)piscina_allocare(piscina,
                (memoriae_index)(n * n) * magnitudo(s32));
            i32 k;

            per (k = ZEPHYRUM; k < n * (n - I); k++)
            {
                litterae[k] = (s32)(k % (n - I)) + I;
            }
            (vacuum)plexus_creare(n, litterae, n * (n - I), piscina,
                &p);
            CREDO_VERUM (_aequales(p, torsio_plena, piscina));
        }
    }
    /* B_2 = Z: sigma_1^k = sigma_1^m solum si k = m */
    CREDO_FALSUM (_aequales(_p(II, "1 1 1", piscina), _p(II, "1",
        piscina), piscina));
    CREDO_VERUM (_aequales(_p(II, "1 -1 1", piscina), _p(II, "1",
        piscina), piscina));
    /* refutatio: fila diversa, aequales non tangitur */
    aequales = XCIX;
    CREDO_FALSUM (plexus_aequales(_p(III, "1", piscina), _p(IV, "1",
        piscina), LIMES_MAGNUS, piscina, &aequales));
    CREDO_VERUM (aequales == XCIX);
}

interior vacuum
_probare_contra_substitutionem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  discrepantiae  = ZEPHYRUM;
        i32  comparatae     = ZEPHYRUM;

    sors_seminare(&s, DCCLXXVII, I);
    per (t = ZEPHYRUM; t < CCC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, III);
                Plexus b      = _fortuitus(&s, n, sors_intra(&s, IX),
                    piscina);
                   i32 k;

        per (k = I; k <= n; k++)
        {
            VerbumLiberum  w;
                      s32  x          = (s32)k;
                      i32  capacitas  = 200000;
                      s32* exitus   = (s32*)piscina_allocare(piscina,
                          (memoriae_index)capacitas * magnitudo(s32));
                      i32 numerus;
                      i32 r;
                      b32 idem     = VERUM;

            si (!plexus_imago_libera(b, k, LIMES_MAGNUS, piscina, &w))
            {
                discrepantiae++;
                perge;
            }
            numerus = _agere_substitutione(b, &x, I, exitus, capacitas,
                piscina);
            si (numerus != w.longitudo)
            {
                idem = FALSUM;
            }
            alioquin
            {
                per (r = ZEPHYRUM; r < numerus; r++)
                {
                    si (exitus[r] != w.litterae[r])
                    {
                        idem = FALSUM;
                    }
                }
            }
            si (!idem)
            {
                discrepantiae++;
            }
            comparatae++;
        }
        piscina_reficere(piscina, nota);
    }
    imprimere("  %u imagines comparatae, %u discrepantiae\n",
        comparatae,
        discrepantiae);
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    CREDO_VERUM (comparatae > DCCC);
}

interior vacuum
_probare_invariantia (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  fracta_producti       = ZEPHYRUM;
        i32  fracta_coniugationis  = ZEPHYRUM;

    sors_seminare(&s, MMXXVI, I);
    per (t = ZEPHYRUM; t < CCC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, IV);
                Plexus b    = _fortuitus(&s, n, sors_intra(&s, X),
                    piscina);
                  s32* productum = (s32*)piscina_allocare(piscina,
                      (memoriae_index)LIMES_MAGNUS
                          / C * magnitudo(s32));
                   i32  altitudo  = ZEPHYRUM;
                   i32* imago     = (i32*)piscina_allocare(piscina,
                       (memoriae_index)n * magnitudo(i32));
                   i32 k;

        plexus_permutatio(b, imago);
        per (k = I; k <= n; k++)
        {
            VerbumLiberum w;
                      i32 r;
                      i32 prima;
                      i32 ultima;

            (vacuum)plexus_imago_libera(b, k, LIMES_MAGNUS, piscina,
                &w);
            per (r = ZEPHYRUM; r < w.longitudo; r++)
            {
                altitudo = _impelle(productum, altitudo, w.litterae[r]);
            }
            /* w = u x_j u^-1: reductio cyclica ad litteram unam
             * x_{imago[k-1]+1} */
            prima   = ZEPHYRUM;
            ultima  = w.longitudo - I;
            dum (   prima < ultima
                 && w.litterae[prima] == -w.litterae[ultima])
            {
                prima++;
                ultima--;
            }
            si (   prima             != ultima
                || w.litterae[prima] != (s32)(imago[k - I] + I))
            {
                fracta_coniugationis++;
            }
        }
        /* productum x_1 ... x_n */
        {
            b32 idem = altitudo == n;

            per (k = ZEPHYRUM; idem && k < n; k++)
            {
                si (productum[k] != (s32)(k + I))
                {
                    idem = FALSUM;
                }
            }
            si (!idem)
            {
                fracta_producti++;
            }
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta_producti, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fracta_coniugationis, ZEPHYRUM);
}

/* verbum per relationes rescriptum: insertiones sigma sigma^-1,
 * a b a -> b a b (|a - b| = 1, signa eadem), a b -> b a (|a| - |b| >=
 * 2) */
interior Plexus
_rescribere (
       Sors* s,
     Plexus  p,
    Piscina* piscina)
{
    i32  capacitas = p.longitudo + LXIV;
    s32* l         = (s32*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(s32));
       i32 numerus   = p.longitudo;
       i32 gradus;
    Plexus q;

    memcpy(l, p.litterae, (size_t)p.longitudo * magnitudo(s32));
    per (gradus = ZEPHYRUM; gradus < XXX; gradus++)
    {
        i32 genus = sors_intra(s, III);
        i32 locus;

        si (genus == ZEPHYRUM && numerus + II <= capacitas)
        {
            s32 a = (s32)sors_intra(s, p.fila - I) + I;

            locus = sors_intra(s, numerus + I);
            memmove(l + locus + II, l + locus, (size_t)(numerus - locus)
                * magnitudo(s32));
            l[locus]      = sors_intra(s, II) == ZEPHYRUM ? a : -a;
            l[locus + I]  = -l[locus];
            numerus       += II;
        }
        alioquin si (genus == I && numerus >= III)
        {
            s32 a;
            s32 b;

            locus  = sors_intra(s, numerus - II);
            a      = l[locus];
            b      = l[locus + I];
            si (l[locus + II] == a && (a > ZEPHYRUM) == (b > ZEPHYRUM)
                && (a - b == I || b - a == I))
            {
                l[locus]       = b;
                l[locus + I]   = a;
                l[locus + II]  = b;
            }
        }
        alioquin si (genus == II && numerus >= II)
        {
            s32 a;
            s32 b;
            s32 da;
            s32 db;

            locus  = sors_intra(s, numerus - I);
            a      = l[locus];
            b      = l[locus + I];
            da     = a > ZEPHYRUM ? a : -a;
            db     = b > ZEPHYRUM ? b : -b;
            si (da - db >= II || db - da >= II)
            {
                l[locus]      = b;
                l[locus + I]  = a;
            }
        }
    }
    (vacuum)plexus_creare(p.fila, l, numerus, piscina, &q);
    redde q;
}

interior vacuum
_probare_aequalitatem_fortuitam (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
     Plexus  commutator;

    /* [s_1^2, s_2^2]: permutatio et summa nullae, non trivialis */
    commutator = _p(III, "1 1 2 2 -1 -1 -2 -2", piscina);
    CREDO_FALSUM (_aequales(commutator, _p(III, "", piscina), piscina));
    sors_seminare(&s, XLII, I);
    per (t = ZEPHYRUM; t < CC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, III);
                Plexus p    = _fortuitus(&s, n, sors_intra(&s, VIII),
                    piscina);
                Plexus q    = _rescribere(&s, p, piscina);
                Plexus r;
                   s32 quadratum[II];
                Plexus sigma_quadratum;

        CREDO_VERUM (_aequales(p, q, piscina));
        quadratum[ZEPHYRUM]  = I;
        quadratum[I]         = I;
        (vacuum)plexus_creare(n, quadratum, II, piscina,
            &sigma_quadratum);
        (vacuum)plexus_iungere(q, sigma_quadratum, piscina, &r);
        CREDO_FALSUM (_aequales(p, r, piscina));
        si (n == III)
        {
            (vacuum)plexus_iungere(q, commutator, piscina, &r);
            CREDO_FALSUM (_aequales(p, r, piscina));
        }
        piscina_reficere(piscina, nota);
    }
}

interior vacuum
_probare_limitem (
    CredoContextus* c)
{
          Piscina* piscina = c->piscina;
    VerbumLiberum  w;
           Plexus  p;
              b32  aequales;
              i32  k;
              s32  litterae[XXIV];

    /* (sigma_1 sigma_2^-1)^m: |phi(x_1)| crescit ratione dilatationis
     * (3 + sqrt 5)/2 (Thiffeault 2010, Sect. III B; longitudo verbi sub
     * automorphismo pseudo-Anosov eadem ratione crescit): exacte,
     * |a_{m+1}^2 - 3 a_{m+1} a_m + a_m^2| < a_m^2 / 100 (ratio intra
     * ~0.5% radicis t^2 - 3t + 1) */
    {
        s64 prior = ZEPHYRUM;

        per (k = I; k <= XII; k++)
        {
            i32 t;
            s64 nunc;

            per (t = ZEPHYRUM; t < II * k; t += II)
            {
                litterae[t]      = I;
                litterae[t + I]  = -II;
            }
            (vacuum)plexus_creare(III, litterae, II * k, piscina, &p);
            CREDO_VERUM (plexus_imago_libera(p, I, LIMES_MAGNUS,
                piscina,
                &w));
            nunc = (s64)w.longitudo;
            si (k >= VIII)
            {
                s64 residuum = nunc * nunc - III * nunc * prior
                    + prior * prior;

                si (residuum < ZEPHYRUM)
                {
                    residuum = -residuum;
                }
                CREDO_VERUM (residuum * C < prior * prior);
            }
            prior = nunc;
        }
        imprimere("  |phi(x_1)| = %ld sub (s1 s2^-1)^12\n",
            (longus)prior);
        CREDO_VERUM (prior > 10000L);
    }
    /* limes = summa imaginum - 1: post litteram ultimam summa limitem
     * superat, etsi nullum productum singulum */
    {
        s64 summa = ZEPHYRUM;
        i32 j;

        per (j = I; j <= III; j++)
        {
            (vacuum)plexus_imago_libera(p, j, LIMES_MAGNUS, piscina,
                &w);
            summa += (s64)w.longitudo;
        }
        w.longitudo = XCIX;
        CREDO_FALSUM (plexus_imago_libera(p, I, (i64)(summa - I),
            piscina,
            &w));
        CREDO_AEQUALIS_I32 (w.longitudo, XCIX);
    }
    w.longitudo = XCIX;
    CREDO_FALSUM (plexus_imago_libera(p, I, 1000UL, piscina, &w));
    CREDO_AEQUALIS_I32 (w.longitudo, XCIX);
    /* limes infra fila: statim FALSUM */
    CREDO_FALSUM (plexus_imago_libera(_p(III, "", piscina), I, II,
        piscina, &w));
    CREDO_VERUM (plexus_imago_libera(_p(III, "", piscina), I, III,
        piscina, &w) && w.longitudo == I);
    /* aequalitas sub limite parvo: non decisa, non tacta. q = p C, C
     * commutator purus: r = p C^-1 p^-1 ad actionem pervenit (summa et
     * permutatio nullae) et imagines eius limitem superant */
    aequales = XCIX;
    {
        Plexus q;

        (vacuum)plexus_iungere(p, _p(III, "1 1 2 2 -1 -1 -2 -2",
            piscina), piscina, &q);
        CREDO_FALSUM (plexus_aequales(p, q, 1000UL, piscina,
            &aequales));
        CREDO_VERUM (aequales == XCIX);
        CREDO_VERUM (plexus_aequales(p, q, LIMES_MAGNUS, piscina,
            &aequales) && !aequales);
        aequales = XCIX;
    }
    CREDO_VERUM (aequales == XCIX);
}

/* contractio imaginum: verbum longum cuius imagines parvae manent
 * ((s1 s1^-1)^50000, (s1 s2 s1^-1 s2^-1)^20000) purgamenta multa
 * relinquit, ergo piscina imaginum saepe contrahitur; imagines tamen
 * generatores ipsi esse debent, aut commutatoris theoremate noto. */
interior vacuum
_probare_contractionem (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
             s32* litterae = (s32*)piscina_allocare(piscina,
                 (memoriae_index)100000 * magnitudo(s32));
           Plexus p;
    VerbumLiberum w;
              i32 k;
              b32 aequales = FALSUM;

    per (k = ZEPHYRUM; k < 100000UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = -I;
    }
    (vacuum)plexus_creare(III, litterae, 100000UL, piscina, &p);
    CREDO_VERUM (_imago_textui(p, I, "1", piscina));
    CREDO_VERUM (_imago_textui(p, II, "2", piscina));
    CREDO_VERUM (_imago_textui(p, III, "3", piscina));
    CREDO_VERUM (plexus_aequales(p, _p(III, "", piscina), LIMES_MAGNUS,
        piscina, &aequales) && aequales);
    /* (s1 s2)^900 = Delta^600 = (Delta^2)^300, centralis: phi
     * coniugatio per u^300 (u = x_1 x_2 x_3 aut inversum, idem pro
     * omni k) - imagines lineariter crescunt, purgamenta quadratice */
    per (k = ZEPHYRUM; k < 1800UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = II;
    }
    (vacuum)plexus_creare(III, litterae, 1800UL, piscina, &p);
    {
        /* exspectatum per acervum proprium: u^300 x_k u^-300 libere
         * reductum, u = x_1 x_2 x_3 (signum 0) aut inversum (1) */
        s32* exspectatum = (s32*)piscina_allocare(piscina,
            (memoriae_index)4000 * magnitudo(s32));
        b32 congruit[II];
        s32 signum;
        i32 j;

        congruit[ZEPHYRUM]  = VERUM;
        congruit[I]         = VERUM;
        per (j = I; j <= III; j++)
        {
            CREDO_VERUM (plexus_imago_libera(p, j, LIMES_MAGNUS,
                piscina,
                &w));
            per (signum = ZEPHYRUM; signum < II; signum++)
            {
                i32 altitudo = ZEPHYRUM;
                i32 r;
                i32 t;

                per (r = ZEPHYRUM; r < CCC; r++)
                {
                    per (t = ZEPHYRUM; t < III; t++)
                    {
                        altitudo = _impelle(exspectatum, altitudo,
                            signum == ZEPHYRUM ? (s32)(t + I)
                            : -(s32)(III - t));
                    }
                }
                altitudo = _impelle(exspectatum, altitudo, (s32)j);
                per (r = ZEPHYRUM; r < CCC; r++)
                {
                    per (t = ZEPHYRUM; t < III; t++)
                    {
                        altitudo = _impelle(exspectatum, altitudo,
                            signum == ZEPHYRUM ? -(s32)(III - t)
                            : (s32)(t + I));
                    }
                }
                si (   altitudo != w.longitudo
                    || memcmp(exspectatum, w.litterae, (size_t)altitudo
                    * magnitudo(s32)) != ZEPHYRUM)
                {
                    congruit[signum] = FALSUM;
                }
            }
        }
        CREDO_VERUM (congruit[ZEPHYRUM] || congruit[I]);
    }
}

interior vacuum
_probare_piscinam (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
    memoriae_index  ante;
            Plexus  delta;
            Plexus  torsio_plena;
               b32  aequales = FALSUM;

    (vacuum)plexus_dimidia_torsio(VI, piscina, &delta);
    (vacuum)plexus_iungere(delta, delta, piscina, &torsio_plena);
    ante = piscina_summa_usus(piscina);
    CREDO_VERUM (plexus_aequales(torsio_plena, torsio_plena,
        LIMES_MAGNUS, piscina,
        &aequales) && aequales);
    CREDO_VERUM (plexus_aequales(torsio_plena, delta, LIMES_MAGNUS,
        piscina,
        &aequales) && !aequales);
    CREDO_VERUM (piscina_summa_usus(piscina) == ante);
}


/* ==================================================
 * T2: actio Dynnikov
 * ================================================== */

/* Oraculum fontis: appendix A.2 Thiffeault 2010 (functio Matlab
 * 'loopsigma') verbatim in s64, cum alveis novis separatis (ap, bp
 * fontis), indicibus fontis 1..n-2 (alveus [0] non adhibetur). Scripta
 * ex listatione Matlab, non ex lib/plexus.c. */
interior s64
_pos (
    s64 x)
{
    redde x > ZEPHYRUM ? x : ZEPHYRUM;
}

interior s64
_neg (
    s64 x)
{
    redde x < ZEPHYRUM ? x : ZEPHYRUM;
}

interior vacuum
_regula_fontis (
    s32  littera,
    s64* a,
    s64* b,
    i32  n)
{
    s64 a_nova[XVI];
    s64 b_nova[XVI];
    s32 i = littera > ZEPHYRUM ? littera : -littera;
    s64 c;
    s64 d;
    i32 k;

    per (k = ZEPHYRUM; k < XVI; k++)
    {
        a_nova[k] = a[k];
        b_nova[k] = b[k];
    }
    si (littera > ZEPHYRUM)
    {
        si (i == I)
        {
            b_nova[1] = a[1] + _pos(b[1]);
            a_nova[1] = -b[1] + _pos(b_nova[1]);
        }
        alioquin si (i == (s32)n - I)
        {
            b_nova[n - 2] = a[n - 2] + _neg(b[n - 2]);
            a_nova[n - 2] = -b[n - 2] + _neg(b_nova[n - 2]);
        }
        alioquin
        {
            c = a[i - 1] - a[i] - _pos(b[i]) + _neg(b[i - 1]);
            a_nova[i - 1] = a[i - 1] - _pos(b[i - 1]) - _pos(_pos(b[i])
                + c);
            b_nova[i - 1] = b[i] + _neg(c);
            a_nova[i] = a[i] - _neg(b[i]) - _neg(_neg(b[i - 1])
                - c);
            b_nova[i] = b[i - 1] - _neg(c);
        }
    }
    alioquin
    {
        si (i == I)
        {
            b_nova[1] = -a[1] + _pos(b[1]);
            a_nova[1] = b[1] - _pos(b_nova[1]);
        }
        alioquin si (i == (s32)n - I)
        {
            b_nova[n - 2] = -a[n - 2] + _neg(b[n - 2]);
            a_nova[n - 2] = b[n - 2] - _neg(b_nova[n - 2]);
        }
        alioquin
        {
            d = a[i - 1] - a[i] + _pos(b[i]) - _neg(b[i - 1]);
            a_nova[i - 1] = a[i - 1] + _pos(b[i - 1]) + _pos(_pos(b[i])
                - d);
            b_nova[i - 1] = b[i] - _pos(d);
            a_nova[i] = a[i] + _neg(b[i]) + _neg(_neg(b[i - 1])
                + d);
            b_nova[i] = b[i - 1] + _pos(d);
        }
    }
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        a[k] = a_nova[k];
        b[k] = b_nova[k];
    }
}

/* L(u) ex appendice A.2 (functio Matlab 'loopinter') verbatim:
 * cumb = [0 cumsum(b)]; b0 = -max(|a| + max(b,0) + cumb(1:end-1));
 * bn = -b0 - sum(b); L = sum|b| + sum|a(2:end) - a(1:end-1)| + |a(1)|
 * + |a(end)| + |b0| + |bn| (indices fontis 1..n-2) */
interior s64
_regula_sectionum (
    constans s64* a,
    constans s64* b,
             i32  n)
{
    s64 cumulus  = ZEPHYRUM;
    s64 summa    = ZEPHYRUM;
    s64 maxima   = ZEPHYRUM;
    s64 b0;
    s64 bn;
    s64 l      = ZEPHYRUM;
    i32 j;

    per (j = I; j <= n - II; j++)
    {
        s64 t = (a[j] < ZEPHYRUM ? -a[j] : a[j]) + _pos(b[j]) + cumulus;

        si (j == I || t > maxima)
        {
            maxima = t;
        }
        cumulus += b[j];
    }
    b0 = -maxima;
    per (j = I; j <= n - II; j++)
    {
        summa  += b[j];
        l      += b[j] < ZEPHYRUM ? -b[j] : b[j];
    }
    bn = -b0 - summa;
    per (j = II; j <= n - II; j++)
    {
        s64 d = a[j] - a[j - I];

        l += d < ZEPHYRUM ? -d : d;
    }
    l += (a[I] < ZEPHYRUM ? -a[I] : a[I])
        + (a[n - II] < ZEPHYRUM ? -a[n - II] : a[n - II]);
    l += (b0 < ZEPHYRUM ? -b0 : b0) + (bn < ZEPHYRUM ? -bn : bn);
    redde l;
}

/* ansa ex s64 (indices fontis 1..n-2) */
interior Ansa
_ansa_ex_s64 (
    constans s64* a,
    constans s64* b,
             i32  n,
         Piscina* piscina)
{
    Magnus ma[XVI];
    Magnus mb[XVI];
      Ansa u;
       i32 k;

    per (k = ZEPHYRUM; k + II < n; k++)
    {
        ma[k] = magnus_ex_s64(a[k + I]);
        mb[k] = magnus_ex_s64(b[k + I]);
    }
    (vacuum)ansa_ex_coordinatis(n, ma, mb, piscina, &u);
    redde u;
}

interior s64
_s64 (
    Magnus x)
{
    s64 v = ZEPHYRUM;

    (vacuum)magnus_ad_s64(x, &v);
    redde v;
}

interior vacuum
_probare_ansas_formam (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Ansa  u;
     Magnus  a[IV];
     Magnus  b[IV];
        i32  n;
        i32  k;

    /* ansae circa puncta vicina: L = 2 */
    CREDO_VERUM (ansa_circa(III, II, piscina, &u)
        && _textus(ansa_ad_chordam(u, piscina), "(0; -1)"));
    CREDO_VERUM (ansa_circa(III, I, piscina, &u)
        && _textus(ansa_ad_chordam(u, piscina), "(0; 1)"));
    CREDO_VERUM (ansa_circa(V, III, piscina, &u)
        && _textus(ansa_ad_chordam(u, piscina), "(0 0 0; 0 -1 1)"));
    per (n = III; n <= VIII; n++)
    {
        per (k = I; k < n; k++)
        {
            (vacuum)ansa_circa(n, k, piscina, &u);
            CREDO_VERUM (_s64(ansa_sectiones(u, piscina)) == II);
        }
    }
    /* Thiffeault Fig. 8: n = 5, u = (-1, 1, -2, 0, -1, 0), L = 12 */
    a[ZEPHYRUM]  = magnus_ex_s64(-I);
    a[I]         = magnus_ex_s64(I);
    a[II]        = magnus_ex_s64(-II);
    b[ZEPHYRUM]  = magnus_ex_s64(ZEPHYRUM);
    b[I]         = magnus_ex_s64(-I);
    b[II]        = magnus_ex_s64(ZEPHYRUM);
    CREDO_VERUM (ansa_ex_coordinatis(V, a, b, piscina, &u));
    CREDO_VERUM (_textus(ansa_ad_chordam(u, piscina),
        "(-1 1 -2; 0 -1 0)"));
    CREDO_VERUM (_s64(ansa_sectiones(u, piscina)) == XII);
    /* ansae_aequales: differentia in a sola, in b sola, fila diversa */
    {
        Ansa v;
        Ansa w;

        a[ZEPHYRUM] = magnus_ex_s64(-I);
        b[ZEPHYRUM] = magnus_ex_s64(-I);
        (vacuum)ansa_ex_coordinatis(IV, a, b, piscina, &w);
        b[ZEPHYRUM] = magnus_ex_s64(II);
        (vacuum)ansa_ex_coordinatis(IV, a, b, piscina, &v);
        CREDO_FALSUM (ansae_aequales(v, w));
        b[ZEPHYRUM]  = magnus_ex_s64(-I);
        a[I]         = magnus_ex_s64(V);
        (vacuum)ansa_ex_coordinatis(IV, a, b, piscina, &v);
        CREDO_FALSUM (ansae_aequales(v, w));
        a[I] = magnus_ex_s64(I);
        (vacuum)ansa_ex_coordinatis(IV, a, b, piscina, &v);
        CREDO_VERUM (ansae_aequales(v, w));
        (vacuum)ansa_circa(III, I, piscina, &v);
        CREDO_FALSUM (ansae_aequales(v, w));
        (vacuum)ansa_circa(III, I, piscina, &w);
        (vacuum)ansa_circa(III, II, piscina, &v);
        CREDO_FALSUM (ansae_aequales(v, w));
    }
    /* refutationes */
    u.fila = XCIX;
    CREDO_FALSUM (ansa_circa(II, I, piscina, &u));
    CREDO_FALSUM (ansa_circa(IV, ZEPHYRUM, piscina, &u));
    CREDO_FALSUM (ansa_circa(IV, IV, piscina, &u));
    CREDO_FALSUM (ansa_ex_coordinatis(II, a, b, piscina, &u));
    CREDO_FALSUM (ansa_ex_coordinatis(V, NIHIL, b, piscina, &u));
    CREDO_AEQUALIS_I32 (u.fila, XCIX);
    (vacuum)ansa_circa(IV, I, piscina, &u);
    {
        Ansa v;

        v.fila = XCIX;
        CREDO_FALSUM (ansa_agere(u, _p(III, "1", piscina), piscina,
            &v));
        CREDO_AEQUALIS_I32 (v.fila, XCIX);
    }
}

interior vacuum
_probare_ansas_contra_fontem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  discrepantiae = ZEPHYRUM;

    sors_seminare(&s, CDXLIV, I);
    per (t = ZEPHYRUM; t < MM; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, VI);
                Plexus p    = _fortuitus(&s, n, sors_intra(&s, VIII),
                    piscina);
                   s64 a[XVI];
                   s64 b[XVI];
                  Ansa u;
                  Ansa v;
                  Ansa w;
                   i32 k;

        memset(a, ZEPHYRUM, magnitudo(a));
        memset(b, ZEPHYRUM, magnitudo(b));
        per (k = I; k + II <= n; k++)
        {
            a[k] = (s64)sors_inter(&s, -XX, XX);
            b[k] = (s64)sors_inter(&s, -XX, XX);
        }
        u = _ansa_ex_s64(a, b, n, piscina);
        (vacuum)ansa_agere(u, p, piscina, &v);
        per (k = ZEPHYRUM; k < p.longitudo; k++)
        {
            _regula_fontis(p.litterae[k], a, b, n);
        }
        w = _ansa_ex_s64(a, b, n, piscina);
        si (!ansae_aequales(v, w))
        {
            discrepantiae++;
        }
        /* L(u) contra loopinter */
        si (_s64(ansa_sectiones(w, piscina)) != _regula_sectionum(a, b,
            n))
        {
            discrepantiae++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
}

/* relationes braidarum et inversa in ansis fortuitis (etiam magnis) */
interior vacuum
_probare_ansas_relationes (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  fracta = ZEPHYRUM;
     Magnus  scala;

    (vacuum)magnus_ex_chorda(chorda_ex_literis("1180591620717411303424",
        piscina), piscina, &scala);    /* 2^70 */
    sors_seminare(&s, DLV, I);
    per (t = ZEPHYRUM; t < D; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, V);
                Magnus a[XVI];
                Magnus b[XVI];
                  Ansa u;
                  Ansa v;
                  Ansa w;
                   i32 i;
                   i32 k;
                   b32 magna = sors_intra(&s, II) == ZEPHYRUM;

        per (k = ZEPHYRUM; k + II < n; k++)
        {
            a[k] = magnus_ex_s64((s64)sors_inter(&s, -MM, MM));
            b[k] = magnus_ex_s64((s64)sors_inter(&s, -MM, MM));
            si (magna)
            {
                a[k] = magnus_multiplica(a[k], scala, piscina);
                b[k] = magnus_multiplica(b[k], scala, piscina);
            }
        }
        (vacuum)ansa_ex_coordinatis(n, a, b, piscina, &u);
        per (i = I; i < n; i++)
        {
               s32 l[III];
            Plexus p;
            Plexus q;

            /* s_i s_i^-1 et s_i^-1 s_i */
            l[ZEPHYRUM]  = (s32)i;
            l[I]         = -(s32)i;
            (vacuum)plexus_creare(n, l, II, piscina, &p);
            (vacuum)ansa_agere(u, p, piscina, &v);
            si (!ansae_aequales(u, v))
            {
                fracta++;
            }
            (vacuum)ansa_agere(u, plexus_inversum(p, piscina), piscina,
                &v);
            si (!ansae_aequales(u, v))
            {
                fracta++;
            }
            si (i + I < n)
            {
                /* s_i s_{i+1} s_i = s_{i+1} s_i s_{i+1} */
                l[ZEPHYRUM]  = (s32)i;
                l[I]         = (s32)(i + I);
                l[II]        = (s32)i;
                (vacuum)plexus_creare(n, l, III, piscina, &p);
                l[ZEPHYRUM]  = (s32)(i + I);
                l[I]         = (s32)i;
                l[II]        = (s32)(i + I);
                (vacuum)plexus_creare(n, l, III, piscina, &q);
                (vacuum)ansa_agere(u, p, piscina, &v);
                (vacuum)ansa_agere(u, q, piscina, &w);
                si (!ansae_aequales(v, w))
                {
                    fracta++;
                }
            }
            si (i + II < n)
            {
                /* s_i s_{i+2} = s_{i+2} s_i */
                l[ZEPHYRUM]  = (s32)i;
                l[I]         = -(s32)(i + II);
                (vacuum)plexus_creare(n, l, II, piscina, &p);
                l[ZEPHYRUM]  = -(s32)(i + II);
                l[I]         = (s32)i;
                (vacuum)plexus_creare(n, l, II, piscina, &q);
                (vacuum)ansa_agere(u, p, piscina, &v);
                (vacuum)ansa_agere(u, q, piscina, &w);
                si (!ansae_aequales(v, w))
                {
                    fracta++;
                }
            }
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}

/* homogeneitas positiva: (lambda u) . p = lambda (u . p), lambda = 2^70
 * (actio per partes lineares, homogenea) */
interior vacuum
_probare_ansas_homogeneas (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  fracta = ZEPHYRUM;
     Magnus  scala;

    (vacuum)magnus_ex_chorda(chorda_ex_literis("1180591620717411303424",
        piscina), piscina, &scala);
    sors_seminare(&s, DCLX, I);
    per (t = ZEPHYRUM; t < CC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, V);
                Plexus p    = _fortuitus(&s, n, X + sors_intra(&s, XX),
                    piscina);
                Magnus a[XVI];
                Magnus b[XVI];
                  Ansa u;
                  Ansa u_magna;
                  Ansa v;
                  Ansa v_magna;
                   i32 k;
                   b32 idem = VERUM;

        per (k = ZEPHYRUM; k + II < n; k++)
        {
            a[k] = magnus_ex_s64((s64)sors_inter(&s, -C, C));
            b[k] = magnus_ex_s64((s64)sors_inter(&s, -C, C));
        }
        (vacuum)ansa_ex_coordinatis(n, a, b, piscina, &u);
        per (k = ZEPHYRUM; k + II < n; k++)
        {
            a[k] = magnus_multiplica(a[k], scala, piscina);
            b[k] = magnus_multiplica(b[k], scala, piscina);
        }
        (vacuum)ansa_ex_coordinatis(n, a, b, piscina, &u_magna);
        (vacuum)ansa_agere(u, p, piscina, &v);
        (vacuum)ansa_agere(u_magna, p, piscina, &v_magna);
        per (k = ZEPHYRUM; k + II < n; k++)
        {
            si (   !magnus_aequalis(magnus_multiplica(v.a[k], scala,
                piscina), v_magna.a[k])
                || !magnus_aequalis(magnus_multiplica(v.b[k], scala,
                piscina), v_magna.b[k]))
            {
                idem = FALSUM;
            }
        }
        si (!idem)
        {
            fracta++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}

/* ==== partitio exhaustiva: Artin contra ansas ==== */

nomen structura {
    chorda clavis_libera;
    chorda clavis_ansarum;
} ParClavium;

interior s32
_compara_liberam (
    constans vacuum* x,
    constans vacuum* y)
{
    constans ParClavium* p = (constans ParClavium*)x;
    constans ParClavium* q = (constans ParClavium*)y;
                    s32  r = chorda_comparare(p->clavis_libera,
                        q->clavis_libera);

    redde r != ZEPHYRUM ? r : chorda_comparare(p->clavis_ansarum,
        q->clavis_ansarum);
}

interior s32
_compara_ansarum (
    constans vacuum* x,
    constans vacuum* y)
{
    constans ParClavium* p = (constans ParClavium*)x;
    constans ParClavium* q = (constans ParClavium*)y;

    redde chorda_comparare(p->clavis_ansarum, q->clavis_ansarum);
}

/* clavis Artin: imagines omnium generatorum */
interior chorda
_clavis_libera (
     Plexus  p,
    Piscina* piscina)
{
     ChordaAedificator* ae = chorda_aedificator_creare(piscina, LXIV);
                   i32  j;

    per (j = I; j <= p.fila; j++)
    {
        VerbumLiberum w;

        (vacuum)plexus_imago_libera(p, j, LIMES_MAGNUS, piscina, &w);
        (vacuum)chorda_aedificator_appendere_chorda(ae,
            verbum_liberum_ad_chordam(w, piscina));
        (vacuum)chorda_aedificator_appendere_character(ae, '|');
    }
    redde chorda_aedificator_finire(ae);
}

/* clavis ansarum: imagines E_1 .. E_{m-1} in D_m, plexu translato */
interior chorda
_clavis_ansarum (
     Plexus  p,
        i32  fila,
        i32  translatio,
    Piscina* piscina)
{
     ChordaAedificator* ae = chorda_aedificator_creare(piscina, LXIV);
                Plexus  maior;
                   i32  k;

    (vacuum)plexus_in_maiore(p, fila, translatio, piscina, &maior);
    per (k = I; k < fila; k++)
    {
        Ansa e;
        Ansa imago;

        (vacuum)ansa_circa(fila, k, piscina, &e);
        (vacuum)ansa_agere(e, maior, piscina, &imago);
        (vacuum)chorda_aedificator_appendere_chorda(ae,
            ansa_ad_chordam(imago, piscina));
    }
    redde chorda_aedificator_finire(ae);
}

/* omnia verba longitudinis <= l in B_n; numerus classium Artin,
 * classium ansarum, et discrepantiae (classis Artin cum clavibus
 * ansarum pluribus, aut clavis ansarum in classibus Artin pluribus) */
interior vacuum
_partitio (
         i32  n,
         i32  l,
         i32  fila_disci,
         i32  translatio,
     Piscina* piscina,
         i32* verba,
         i32* classes_liberae,
         i32* classes_ansarum,
         i32* discrepantiae)
{
           i32  litterae_numerus  = II * (n - I);
           i32  numerus           = ZEPHYRUM;
           i32  potentia          = I;
           i32  longitudo;
    ParClavium* pares;
           i32  k;

    per (longitudo = ZEPHYRUM; longitudo <= l; longitudo++)
    {
        numerus   += potentia;
        potentia  *= litterae_numerus;
    }
    pares = (ParClavium*)piscina_allocare(piscina,
        (memoriae_index)numerus
        * magnitudo(ParClavium));
    k         = ZEPHYRUM;
    potentia  = I;
    per (longitudo = ZEPHYRUM; longitudo <= l; longitudo++)
    {
        i32 index;

        per (index = ZEPHYRUM; index < potentia; index++)
        {
               s32 litterae[XVI];
               i32 r = index;
               i32 t;
            Plexus p;

            per (t = ZEPHYRUM; t < longitudo; t++)
            {
                i32 q = r % litterae_numerus;
                s32 i = (s32)(q / II) + I;

                litterae[t]  = (q % II) == ZEPHYRUM ? i : -i;
                r            /= litterae_numerus;
            }
            (vacuum)plexus_creare(n, litterae, longitudo, piscina, &p);
            pares[k].clavis_libera     = _clavis_libera(p, piscina);
            pares[k].clavis_ansarum   = _clavis_ansarum(p, fila_disci,
                translatio, piscina);
            k++;
        }
        potentia *= litterae_numerus;
    }
    *verba = numerus;
    /* ordine Artin (deinde ansarum): classis Artin cum duabus clavibus
     * ansarum = discrepantia */
    qsort(pares, (size_t)numerus, magnitudo(ParClavium),
        _compara_liberam);
    *classes_liberae  = ZEPHYRUM;
    *discrepantiae    = ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   k == ZEPHYRUM
            || !chorda_aequalis(pares[k].clavis_libera,
            pares[k - I].clavis_libera))
        {
            (*classes_liberae)++;
        }
        alioquin si (!chorda_aequalis(pares[k].clavis_ansarum,
                     pares[k - I].clavis_ansarum))
        {
            (*discrepantiae)++;
        }
    }
    qsort(pares, (size_t)numerus, magnitudo(ParClavium),
        _compara_ansarum);
    *classes_ansarum = ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   k == ZEPHYRUM
            || !chorda_aequalis(pares[k].clavis_ansarum,
            pares[k - I].clavis_ansarum))
        {
            (*classes_ansarum)++;
        }
    }
}

interior vacuum
_probare_partitionem (
    CredoContextus* c)
{
    Piscina* piscina       = c->piscina;
        i32  casus_n[III]  = { 3, 4, 5 };
        i32  casus_l[III]  = { 6, 5, 4 };
        i32  t;

    /* D_{n+2}, punctis mediis: partitio eadem ac Artin */
    per (t = ZEPHYRUM; t < III; t++)
    {
        PiscinaNotatio nota = piscina_notare(piscina);
                   i32 verba;
                   i32 classes_liberae;
                   i32 classes_ansarum;
                   i32 discrepantiae;

        _partitio(casus_n[t], casus_l[t], casus_n[t] + II, I, piscina,
            &verba, &classes_liberae, &classes_ansarum, &discrepantiae);
        imprimere("  B_%u, verba <= %u: %u verba, %u classes Artin, %u "
            "classes ansarum (D_%u), %u discrepantiae\n", casus_n[t],
            casus_l[t], verba, classes_liberae, classes_ansarum,
            casus_n[t] + II, discrepantiae);
        CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (classes_ansarum, classes_liberae);
        piscina_reficere(piscina, nota);
    }
    /* D_n ipse: classes ansarum PAUCIORES (Delta^2 non videtur), sed
     * nulla classis Artin scinditur */
    {
        PiscinaNotatio nota = piscina_notare(piscina);
                   i32 verba;
                   i32 classes_liberae;
                   i32 classes_ansarum;
                   i32 discrepantiae;

        _partitio(III, VI, III, ZEPHYRUM, piscina, &verba,
            &classes_liberae,
            &classes_ansarum, &discrepantiae);
        imprimere("  B_3 in D_3: %u classes Artin, %u ansarum\n",
            classes_liberae, classes_ansarum);
        CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
        CREDO_VERUM (classes_ansarum < classes_liberae);
        piscina_reficere(piscina, nota);
    }
}

interior vacuum
_probare_torsionem_in_disco (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        i32  n;

    /* Delta^2 omnes E_k in D_n fixas relinquit, non in D_{n+2} */
    per (n = III; n <= VII; n++)
    {
        Plexus delta;
        Plexus torsio_plena;
        Plexus maior;
           b32 fixa_in_n       = VERUM;
           b32 fixa_in_maiore  = VERUM;
           i32 k;

        (vacuum)plexus_dimidia_torsio(n, piscina, &delta);
        (vacuum)plexus_iungere(delta, delta, piscina, &torsio_plena);
        per (k = I; k < n; k++)
        {
            Ansa e;
            Ansa imago;

            (vacuum)ansa_circa(n, k, piscina, &e);
            (vacuum)ansa_agere(e, torsio_plena, piscina, &imago);
            si (!ansae_aequales(e, imago))
            {
                fixa_in_n = FALSUM;
            }
        }
        (vacuum)plexus_in_maiore(torsio_plena, n + II, I, piscina,
            &maior);
        per (k = I; k < n + II; k++)
        {
            Ansa e;
            Ansa imago;

            (vacuum)ansa_circa(n + II, k, piscina, &e);
            (vacuum)ansa_agere(e, maior, piscina, &imago);
            si (!ansae_aequales(e, imago))
            {
                fixa_in_maiore = FALSUM;
            }
        }
        CREDO_VERUM (fixa_in_n);
        CREDO_FALSUM (fixa_in_maiore);
    }
}

/* L(u) sub (s1 s2^-1)^m in D_3 crescit ratione (3 + sqrt 5)/2; braida
 * IV filorum s3^-1 s2^-1 s3^-1 s2 s1: entropia ~0.83144 (Thiffeault
 * Fig. 10), lambda = e^0.83144 = 2.2966; braida periodica (s1 s2)^3 =
 * Delta^2 L constantem relinquit */
interior vacuum
_probare_incrementum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Ansa  u;
       Ansa  v;
     Plexus  gradus;
        s64  prior;
        s64  nunc;
        i32  m;

    (vacuum)ansa_circa(III, I, piscina, &u);
    gradus  = _p(III, "1 -2", piscina);
    prior   = _s64(ansa_sectiones(u, piscina));
    /* m <= 20: L_m^2 in s64 capit (L_20 ~ 2.3 * 10^8) */
    per (m = I; m <= XX; m++)
    {
        (vacuum)ansa_agere(u, gradus, piscina, &u);
        nunc = _s64(ansa_sectiones(u, piscina));
        si (m >= X)
        {
            s64 residuum = nunc * nunc - III * nunc * prior + prior
                * prior;

            si (residuum < ZEPHYRUM)
            {
                residuum = -residuum;
            }
            CREDO_VERUM (residuum * M < prior * prior);
        }
        prior = nunc;
    }
    imprimere("  L post (s1 s2^-1)^20 = %ld\n", (longus)prior);
    /* IV fila */
    (vacuum)ansa_circa(IV, I, piscina, &u);
    gradus  = _p(IV, "-3 -2 -3 2 1", piscina);
    prior   = _s64(ansa_sectiones(u, piscina));
    per (m = I; m <= XXX; m++)
    {
        (vacuum)ansa_agere(u, gradus, piscina, &u);
        nunc = _s64(ansa_sectiones(u, piscina));
        si (m >= XX)
        {
            /* |10000 L_{m+1} - 22966 L_m| <= 10 L_m (0.1%) */
            s64 differentia = 10000L * nunc - 22966L * prior;

            si (differentia < ZEPHYRUM)
            {
                differentia = -differentia;
            }
            CREDO_VERUM (differentia <= 10L * prior);
        }
        prior = nunc;
    }
    imprimere("  L post (s3^-1 s2^-1 s3^-1 s2 s1)^30 = %ld\n",
        (longus)prior);
    /* periodica */
    (vacuum)ansa_circa(III, I, piscina, &u);
    gradus = _p(III, "1 2", piscina);
    per (m = I; m <= XXX; m++)
    {
        (vacuum)ansa_agere(u, gradus, piscina, &v);
        u = v;
        si (m % III == ZEPHYRUM)
        {
            CREDO_VERUM (_s64(ansa_sectiones(u, piscina)) == II);
        }
    }
}

/* crescentia magna (Magnus) et piscina vocantis non repleta */
interior vacuum
_probare_ansas_magnas (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Ansa  u;
       Ansa  v;
     Plexus  p;
        s32* litterae = (s32*)piscina_allocare(piscina,
            (memoriae_index)400 * magnitudo(s32));
               i32 k;
    memoriae_index ante;
            Magnus longitudo;

    per (k = ZEPHYRUM; k < 400UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = -II;
    }
    (vacuum)plexus_creare(III, litterae, 400UL, piscina, &p);
    (vacuum)ansa_circa(III, I, piscina, &u);
    ante = piscina_summa_usus(piscina);
    CREDO_VERUM (ansa_agere(u, p, piscina, &v));
    /* effectum solum (duo Magnus ~140 bitorum) in piscina vocantis */
    CREDO_VERUM (piscina_summa_usus(piscina) - ante < 4096UL);
    longitudo = ansa_sectiones(v, piscina);
    imprimere("  L post (s1 s2^-1)^200: %u bita\n",
        magnus_bitorum(longitudo));
    /* (3+sqrt5)/2 ^ 200 ~ 2^277.7 */
    CREDO_VERUM (magnus_bitorum(longitudo) >= CCLXX
        && magnus_bitorum(longitudo) <= CCLXXXV);
    /* (s1 s2^-1)^3000: coordinatae ~4165 bitorum, piscina temporaria
     * contrahitur (plures MB purgamentorum); inversum E_1 exacte
     * reddit */
    {
        s32* longae = (s32*)piscina_allocare(piscina,
            (memoriae_index)6000 * magnitudo(s32));
        Plexus q;
          Ansa w;

        per (k = ZEPHYRUM; k < 6000UL; k += II)
        {
            longae[k]      = I;
            longae[k + I]  = -II;
        }
        (vacuum)plexus_creare(III, longae, 6000UL, piscina, &q);
        CREDO_VERUM (ansa_agere(u, q, piscina, &w));
        imprimere("  L post (s1 s2^-1)^3000: %u bita\n",
            magnus_bitorum(ansa_sectiones(w, piscina)));
        /* 3000 log2((3 + sqrt 5)/2) = 4165.2 */
        CREDO_VERUM (magnus_bitorum(ansa_sectiones(w, piscina))
            >= 4160UL
            && magnus_bitorum(ansa_sectiones(w, piscina)) <= 4175UL);
        CREDO_VERUM (ansa_agere(w, plexus_inversum(q, piscina), piscina,
            &w) && ansae_aequales(u, w));
    }
    /* inversum reddit E_1 exacte (via Magnus) */
    CREDO_VERUM (ansa_agere(v, plexus_inversum(p, piscina), piscina, &v)
        && ansae_aequales(u, v));
}

interior vacuum
_probare_aequalitatem_ansis (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  discrepantiae = ZEPHYRUM;
        b32  aequales;

    sors_seminare(&s, CMXCIX, I);
    per (t = ZEPHYRUM; t < CCC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = II + sors_intra(&s, V);
                Plexus p    = _fortuitus(&s, n, sors_intra(&s, IX),
                    piscina);
                Plexus q    = sors_intra(&s, II) == ZEPHYRUM
                    ? _rescribere(&s, p, piscina)
                    : _fortuitus(&s, n, sors_intra(&s, IX), piscina);
                   b32 per_liberum  = FALSUM;
                   b32 per_ansas    = FALSUM;

        (vacuum)plexus_aequales(p, q, LIMES_MAGNUS, piscina,
            &per_liberum);
        si (   !plexus_aequales_ansis(p, q, piscina, &per_ansas)
            || per_ansas != per_liberum)
        {
            discrepantiae++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    /* Delta^2 non trivialis etiam per ansas; B_2 */
    {
        Plexus delta;
        Plexus torsio_plena;

        (vacuum)plexus_dimidia_torsio(IV, piscina, &delta);
        (vacuum)plexus_iungere(delta, delta, piscina, &torsio_plena);
        CREDO_VERUM (plexus_aequales_ansis(torsio_plena, _p(IV, "",
            piscina), piscina, &aequales) && !aequales);
        CREDO_VERUM (plexus_aequales_ansis(_p(II, "1 1 -1", piscina),
            _p(II, "1", piscina), piscina, &aequales) && aequales);
        CREDO_VERUM (plexus_aequales_ansis(_p(II, "1 1", piscina),
            _p(II, "", piscina), piscina, &aequales) && !aequales);
    }
    aequales = XCIX;
    CREDO_FALSUM (plexus_aequales_ansis(_p(III, "1", piscina), _p(IV,
        "1", piscina), piscina, &aequales));
    CREDO_VERUM (aequales == XCIX);
    /* piscina refecta */
    {
        memoriae_index ante  = piscina_summa_usus(piscina);
                Plexus a     = _p(V, "1 2 3 4 -1 -3", piscina);

        ante = piscina_summa_usus(piscina);
        (vacuum)plexus_aequales_ansis(a, a, piscina, &aequales);
        CREDO_VERUM (aequales && piscina_summa_usus(piscina) == ante);
    }
}

interior vacuum
_probare_in_maiore (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Plexus  q;

    CREDO_VERUM (plexus_in_maiore(_p(III, "1 -2", piscina), V, II,
        piscina, &q) && q.fila == V
        && _textus(plexus_ad_chordam(q, piscina), "3 -4"));
    CREDO_VERUM (plexus_in_maiore(_p(III, "1 -2", piscina), III,
        ZEPHYRUM, piscina, &q) && q.fila == III);
    q.fila = XCIX;
    CREDO_FALSUM (plexus_in_maiore(_p(III, "1", piscina), IV, II,
        piscina, &q));
    CREDO_FALSUM (plexus_in_maiore(_p(III, "1", piscina), I, ZEPHYRUM,
        piscina, &q));
    CREDO_AEQUALIS_I32 (q.fila, XCIX);
}


/* ==================================================
 * Recensio T1 (recensor independens, 2026-10-15): M1-M3, L1-L8, API
 * ================================================== */

/* braida pura summa nulla, a praefiltris non decisa: s_i^2 s_j^-2
 * (|i - j| >= 2) aut [s_i^2, s_{i+1}^2] */
interior Plexus
_pura (
        Sors* s,
         i32  n,
     Piscina* piscina)
{
       s32 l[VIII];
    Plexus c;
       s32 i;

    si (n >= IV && sors_intra(s, II) == ZEPHYRUM)
    {
        s32 j;

        i            = (s32)sors_intra(s, n - III) + I;
        j            = i + II + (s32)sors_intra(s, n - II - (i32)i);
        l[ZEPHYRUM]  = i;
        l[I]         = i;
        l[II]        = -j;
        l[III]       = -j;
        (vacuum)plexus_creare(n, l, IV, piscina, &c);
    }
    alioquin
    {
        i            = (s32)sors_intra(s, n - II) + I;
        l[ZEPHYRUM]  = i;
        l[I]         = i;
        l[II]        = i + I;
        l[III]       = i + I;
        l[IV]        = -i;
        l[V]         = -i;
        l[VI]        = -(i + I);
        l[VII]       = -(i + I);
        (vacuum)plexus_creare(n, l, VIII, piscina, &c);
    }
    redde c;
}

/* M2: inaequalitates quas summa et permutatio non decidunt */
interior vacuum
_probare_censurae_aequalitatis (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  fracta = ZEPHYRUM;
        b32  aequales;

    CREDO_FALSUM (_aequales(_p(IV, "2 2 -3 -3", piscina), _p(IV, "",
        piscina), piscina));
    CREDO_FALSUM (_aequales(_p(V, "1 1 -3 -3", piscina), _p(V, "",
        piscina), piscina));
    CREDO_VERUM (plexus_aequales_ansis(_p(IV, "2 2 -3 -3", piscina),
        _p(IV, "", piscina), piscina, &aequales) && !aequales);
    sors_seminare(&s, CCCLXV, I);
    per (t = ZEPHYRUM; t < CC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = IV + sors_intra(&s, III);
                Plexus p    = _fortuitus(&s, n, sors_intra(&s, VIII),
                    piscina);
                Plexus q    = _rescribere(&s, p, piscina);
                Plexus qc;
                   b32 per_liberum  = VERUM;
                   b32 per_ansas    = VERUM;

        (vacuum)plexus_iungere(q, _pura(&s, n, piscina), piscina, &qc);
        si (   !plexus_aequales(p, qc, LIMES_MAGNUS, piscina,
            &per_liberum)
            || per_liberum
            || !plexus_aequales_ansis(p, qc, piscina, &per_ansas)
            || per_ansas)
        {
            fracta++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
    /* piscina refecta cum r non trivialis (actio vere currit) */
    {
                Plexus p = _p(IV, "1 -2 3 2 -1", piscina);
                Plexus q;
        memoriae_index ante;

        (vacuum)plexus_iungere(p, _p(IV, "2 2 -3 -3", piscina), piscina,
            &q);
        ante = piscina_summa_usus(piscina);
        CREDO_VERUM (plexus_aequales(p, q, LIMES_MAGNUS, piscina,
            &aequales) && !aequales);
        CREDO_VERUM (plexus_aequales_ansis(p, q, piscina, &aequales)
            && !aequales);
        CREDO_VERUM (piscina_summa_usus(piscina) == ante);
    }
    /* L2: r vacuum -> VERUM etiam limite nullo */
    aequales = FALSUM;
    CREDO_VERUM (plexus_aequales(_p(III, "1 2 1", piscina), _p(III,
        "1 2 1", piscina), ZEPHYRUM, piscina, &aequales) && aequales);
    aequales = XCIX;
    CREDO_FALSUM (plexus_aequales(_p(III, "1 2 1", piscina), _p(III,
        "2 1 2", piscina), II, piscina, &aequales));
    CREDO_VERUM (aequales == XCIX);
}

/* L1: limes minimus L* ex recurrentia contractus (in probatione):
 * max(fila, longitudo producti ante reductionem, summa imaginum post
 * quamque litteram); VERUM ad L*, FALSUM ad L* - 1 */
interior s64
_limes_minimus (
     Plexus  p,
    Piscina* piscina)
{
    s32** w        = (s32**)piscina_allocare(piscina,
        (memoriae_index)p.fila * magnitudo(s32*));
    i32* longitudo = (i32*)piscina_allocare(piscina,
        (memoriae_index)p.fila * magnitudo(i32));
    s64 maximum   = (s64)p.fila;
    i32 k;

    per (k = ZEPHYRUM; k < p.fila; k++)
    {
        w[k] = (s32*)piscina_allocare(piscina, magnitudo(s32));
        w[k][ZEPHYRUM] = (s32)(k + I);
        longitudo[k] = I;
    }
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        s32 littera = p.litterae[k];
        i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera)
            - I;
        s32* x   = w[i];
        s32* y   = w[i + I];
        i32  lx  = longitudo[i];
        i32  ly  = longitudo[i + I];
        s64  productum = littera > ZEPHYRUM ? II * (s64)lx + (s64)ly
            : II * (s64)ly + (s64)lx;
        s32* novum   = (s32*)piscina_allocare(piscina,
            (memoriae_index)productum * magnitudo(s32));
        i32 altitudo  = ZEPHYRUM;
        s64 summa     = ZEPHYRUM;
        s32 r;
        i32 j;

        si (productum > maximum)
        {
            maximum = productum;
        }
        si (littera > ZEPHYRUM)
        {
            /* x y x^-1 */
            per (r = ZEPHYRUM; r < (s32)lx; r++)
            {
                altitudo = _impelle(novum, altitudo, x[r]);
            }
            per (r = ZEPHYRUM; r < (s32)ly; r++)
            {
                altitudo = _impelle(novum, altitudo, y[r]);
            }
            per (r = (s32)lx - I; r >= ZEPHYRUM; r--)
            {
                altitudo = _impelle(novum, altitudo, -x[r]);
            }
            w[i]              = novum;
            longitudo[i]      = altitudo;
            w[i + I]          = x;
            longitudo[i + I]  = lx;
        }
        alioquin
        {
            /* y^-1 x y */
            per (r = (s32)ly - I; r >= ZEPHYRUM; r--)
            {
                altitudo = _impelle(novum, altitudo, -y[r]);
            }
            per (r = ZEPHYRUM; r < (s32)lx; r++)
            {
                altitudo = _impelle(novum, altitudo, x[r]);
            }
            per (r = ZEPHYRUM; r < (s32)ly; r++)
            {
                altitudo = _impelle(novum, altitudo, y[r]);
            }
            w[i]              = y;
            longitudo[i]      = ly;
            w[i + I]          = novum;
            longitudo[i + I]  = altitudo;
        }
        per (j = ZEPHYRUM; j < p.fila; j++)
        {
            summa += (s64)longitudo[j];
        }
        si (summa > maximum)
        {
            maximum = summa;
        }
    }
    redde maximum;
}

interior vacuum
_probare_censurae_limitis (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  fracta = ZEPHYRUM;

    sors_seminare(&s, DCCCXII, I);
    per (t = ZEPHYRUM; t < CC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, III);
                Plexus p    = t == ZEPHYRUM ? _p(III, "1 -2 1 -2 1",
                    piscina) : _fortuitus(&s, n, sors_intra(&s, XIV),
                    piscina);
                   s64 limes = _limes_minimus(p, piscina);
         VerbumLiberum w;

        si (   !plexus_imago_libera(p, I, (i64)limes, piscina, &w)
            || plexus_imago_libera(p, I, (i64)(limes - I), piscina, &w))
        {
            fracta++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}

/* M3, L3, L5, L8: fines, alveus analysis, structura publica */
interior vacuum
_probare_censurae_finium (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
            Plexus  r;
            Plexus  malus;
               s32  l[II];
               s32  malae[I];
               i32  imago[IV];
     VerbumLiberum  w;
     VerbumLiberum* omnes;
              Ansa  u;
              Ansa  v;
               b32  aequales;
    memoriae_index  ante;

    /* M3: littera == fila per plexus_creare */
    l[ZEPHYRUM] = III;
    CREDO_FALSUM (plexus_creare(III, l, I, piscina, &r));
    l[ZEPHYRUM] = -III;
    CREDO_FALSUM (plexus_creare(III, l, I, piscina, &r));
    /* L5: fines inclusi */
    l[ZEPHYRUM] = I;
    CREDO_VERUM (plexus_creare(PLEXUS_FILA_MAXIMA, l, I, piscina, &r));
    l[ZEPHYRUM]  = PLEXUS_FILA_MAXIMA - I;
    l[I]         = -(PLEXUS_FILA_MAXIMA - I);
    CREDO_VERUM (plexus_creare(PLEXUS_FILA_MAXIMA, l, II, piscina, &r));
    CREDO_VERUM (plexus_ex_chorda(PLEXUS_FILA_MAXIMA,
        chorda_ex_literis("1048575 -1048575", piscina), piscina, &r)
        && r.longitudo == II);
    CREDO_FALSUM (plexus_ex_chorda(PLEXUS_FILA_MAXIMA,
        chorda_ex_literis("1048576", piscina), piscina, &r));
    /* L3: plexus_ex_chorda nihil praeter alveum suum relinquit */
    {
        chorda textus = chorda_ex_literis("1 -2 1 -2 1 -2 1 -2 1 -2",
            piscina);
        memoriae_index alveus = ((memoriae_index)(textus.mensura / II
            + I) * magnitudo(s32) + VII) / VIII * VIII;

        ante = piscina_summa_usus(piscina);
        CREDO_VERUM (plexus_ex_chorda(III, textus, piscina, &r)
            && r.longitudo == X);
        CREDO_VERUM (piscina_summa_usus(piscina) - ante <= alveus);
    }
    /* L8: plexus manu factus invalidus ubique refutatur */
    malae[ZEPHYRUM]  = III;
    malus.fila       = III;
    malus.longitudo  = I;
    malus.litterae   = malae;
    CREDO_FALSUM (plexus_validus(malus));
    imago[ZEPHYRUM] = XCIX;
    CREDO_FALSUM (plexus_permutatio(malus, imago));
    CREDO_AEQUALIS_I32 (imago[ZEPHYRUM], XCIX);
    CREDO_FALSUM (plexus_imago_libera(malus, I, LIMES_MAGNUS, piscina,
        &w));
    CREDO_FALSUM (plexus_imagines_liberae(malus, LIMES_MAGNUS, piscina,
        &omnes));
    CREDO_FALSUM (plexus_aequales(malus, malus, LIMES_MAGNUS, piscina,
        &aequales));
    CREDO_FALSUM (plexus_aequales_ansis(malus, malus, piscina,
        &aequales));
    CREDO_FALSUM (plexus_in_maiore(malus, V, I, piscina, &r));
    (vacuum)ansa_circa(III, I, piscina, &u);
    CREDO_FALSUM (ansa_agere(u, malus, piscina, &v));
    malus.litterae = NIHIL;
    CREDO_FALSUM (plexus_validus(malus));
    malus.longitudo = ZEPHYRUM;
    CREDO_VERUM (plexus_validus(malus));
    malus.fila = I;
    CREDO_FALSUM (plexus_validus(malus));
    CREDO_VERUM (plexus_validus(_p(V, "1 -4 2", piscina)));
    /* ansa invalida */
    u.a = NIHIL;
    CREDO_FALSUM (ansa_agere(u, _p(III, "1", piscina), piscina, &v));
}

/* M1, L6: contractio fit cum purgamenta crescunt, non omni littera */
interior vacuum
_probare_censurae_contractionis (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        s32* litterae = (s32*)piscina_allocare(piscina,
            (memoriae_index)6000 * magnitudo(s32));
            Plexus p;
     VerbumLiberum w;
               i64 ante;
               i32 k;
              Ansa u;
              Ansa v;

    /* fila 20000, (s1 s1^-1)^500: imagines non crescunt, purgamenta
     * parva; olim (recensio M1) contractio omni littera */
    per (k = ZEPHYRUM; k < M; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = -I;
    }
    (vacuum)plexus_creare(20000UL, litterae, M, piscina, &p);
    ante = plexus_contractiones();
    CREDO_VERUM (_imago_textui(p, II, "2", piscina));
    imprimere("  contractiones (fila 20000, M litterae): %lu\n",
        (insignatus longus)(plexus_contractiones() - ante));
    CREDO_VERUM (plexus_contractiones() - ante <= I);
    /* (s1 s2)^900 in B_3: purgamenta quadratica -> contractiones */
    per (k = ZEPHYRUM; k < 1800UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = II;
    }
    (vacuum)plexus_creare(III, litterae, 1800UL, piscina, &p);
    ante = plexus_contractiones();
    CREDO_VERUM (plexus_imago_libera(p, I, LIMES_MAGNUS, piscina, &w));
    imprimere("  contractiones ((s1 s2)^900): %lu\n",
        (insignatus longus)(plexus_contractiones() - ante));
    CREDO_VERUM (plexus_contractiones() - ante >= I);
    /* amortizata: contractio solum post purgamenta quater vivas
     * imagines superantia - longe pauciores quam litterae */
    CREDO_VERUM (plexus_contractiones() - ante < 1800UL / IV);
    /* ansae: (s1 s2^-1)^3000 */
    per (k = ZEPHYRUM; k < 6000UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = -II;
    }
    (vacuum)plexus_creare(III, litterae, 6000UL, piscina, &p);
    (vacuum)ansa_circa(III, I, piscina, &u);
    ante = plexus_contractiones();
    CREDO_VERUM (ansa_agere(u, p, piscina, &v));
    imprimere("  contractiones ansae ((s1 s2^-1)^3000): %lu\n",
        (insignatus longus)(plexus_contractiones() - ante));
    CREDO_VERUM (plexus_contractiones() - ante >= I);
}

/* API: imagines omnes; actio in F_n sinistra, actio in ansas dextra */
interior vacuum
_probare_censurae_actionum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  fracta_omnes     = ZEPHYRUM;
        i32  fracta_sinistra  = ZEPHYRUM;
        i32  fracta_dextra    = ZEPHYRUM;

    sors_seminare(&s, DCCCLXXX, I);
    per (t = ZEPHYRUM; t < CC; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, IV);
                Plexus p    = _fortuitus(&s, n, sors_intra(&s, VII),
                    piscina);
                Plexus q    = _fortuitus(&s, n, sors_intra(&s, VII),
                    piscina);
                Plexus  iunctus;
         VerbumLiberum* omnes;
                   i32  k;

        (vacuum)plexus_iungere(p, q, piscina, &iunctus);
        (vacuum)plexus_imagines_liberae(iunctus, LIMES_MAGNUS, piscina,
            &omnes);
        per (k = I; k <= n; k++)
        {
            VerbumLiberum  una;
            VerbumLiberum  ex_q;
                      s32* composita = (s32*)piscina_allocare(piscina,
                          (memoriae_index)200000 * magnitudo(s32));
                      i32 numerus;

            (vacuum)plexus_imago_libera(iunctus, k, LIMES_MAGNUS,
                piscina,
                &una);
            si (   una.longitudo != omnes[k - I].longitudo
                || memcmp(una.litterae, omnes[k - I].litterae,
                (size_t)una.longitudo * magnitudo(s32)) != ZEPHYRUM)
            {
                fracta_omnes++;
            }
            /* phi_{pq}(x_k) = phi_p(phi_q(x_k)) */
            (vacuum)plexus_imago_libera(q, k, LIMES_MAGNUS, piscina,
                &ex_q);
            numerus = _agere_substitutione(p, ex_q.litterae,
                ex_q.longitudo, composita, 200000UL, piscina);
            si (   numerus != una.longitudo
                || memcmp(composita, una.litterae, (size_t)numerus
                * magnitudo(s32)) != ZEPHYRUM)
            {
                fracta_sinistra++;
            }
        }
        /* u . (pq) = (u . p) . q */
        {
            Ansa u;
            Ansa v;
            Ansa w;
            Ansa x;

            (vacuum)ansa_circa(n, I + sors_intra(&s, n - I), piscina,
                &u);
            (vacuum)ansa_agere(u, iunctus, piscina, &v);
            (vacuum)ansa_agere(u, p, piscina, &w);
            (vacuum)ansa_agere(w, q, piscina, &x);
            si (!ansae_aequales(v, x))
            {
                fracta_dextra++;
            }
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta_omnes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fracta_sinistra, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fracta_dextra, ZEPHYRUM);
    /* actio dextra, non sinistra: (u . q) . p != u . (pq) */
    {
        Ansa u;
        Ansa v;
        Ansa w;
        Ansa x;

        (vacuum)ansa_circa(III, I, piscina, &u);
        (vacuum)ansa_agere(u, _p(III, "1 -2", piscina), piscina, &v);
        (vacuum)ansa_agere(u, _p(III, "-2", piscina), piscina, &w);
        (vacuum)ansa_agere(w, _p(III, "1", piscina), piscina, &x);
        CREDO_FALSUM (ansae_aequales(v, x));
    }
}


/* ==================================================
 * T3: repraesentatio Burau reducta
 * ================================================== */

interior Fractio
_fractio (
         s64  numerator,
         s64  denominator,
     Piscina* piscina)
{
    Fractio q = fractio_ex_s64(ZEPHYRUM);

    (vacuum)fractio_ex_s64_s64(numerator, denominator, piscina, &q);
    redde q;
}

interior Matrix
_matrix_polynomiorum (
    constans character* textus,
               Piscina* piscina)
{
    Matrix m;

    si (!matrix_ex_chorda(&ANULUS_POLYNOMIORUM,
        chorda_ex_literis(textus,
        piscina), piscina, &m))
    {
        imprimere("FRACTA: matrix mala: %s\n", textus);
        (vacuum)matrix_nulla(&ANULUS_POLYNOMIORUM, ZEPHYRUM, ZEPHYRUM,
            piscina, &m);
    }
    redde m;
}

interior b32
_burau_aequalis_textui (
                Plexus  p,
    constans character* textus,
               Piscina* piscina)
{
    Matrix b;

    redde plexus_burau(p, piscina, &b)
        && matrix_aequalis(b, _matrix_polynomiorum(textus, piscina));
}

interior vacuum
_probare_burau_generatores (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Matrix  b;

    CREDO_VERUM (_burau_aequalis_textui(_p(II, "1", piscina), "[-t]",
        piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(II, "-1", piscina),
        "[-t^-1]", piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(III, "1", piscina),
        "[-t, 0; 1, 1]", piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(III, "2", piscina),
        "[1, t; 0, -t]", piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(III, "-1", piscina),
        "[-t^-1, 0; t^-1, 1]", piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(III, "-2", piscina),
        "[1, 1; 0, -t^-1]", piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(IV, "2", piscina),
        "[1, t, 0; 0, -t, 0; 0, 1, 1]", piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(IV, "-2", piscina),
        "[1, 1, 0; 0, -t^-1, 0; 0, t^-1, 1]", piscina));
    CREDO_VERUM (_burau_aequalis_textui(_p(V, "", piscina),
        "[1, 0, 0, 0; 0, 1, 0, 0; 0, 0, 1, 0; 0, 0, 0, 1]", piscina));
    /* refutationes */
    {
        Plexus magnus_plexus;
           s32 l = I;

        (vacuum)plexus_creare(PLEXUS_FILA_BURAU + I, &l, I, piscina,
            &magnus_plexus);
        CREDO_FALSUM (plexus_burau(magnus_plexus, piscina, &b));
        CREDO_FALSUM (plexus_burau_minus_uno(magnus_plexus, piscina,
            &b));
    }
}

/* relationes, homomorphismus B(pq) = B(p) B(q), det B(p) = (-t)^e */
interior vacuum
_probare_burau_relationes (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  n;
        i32  fracta = ZEPHYRUM;

    per (n = III; n <= VII; n++)
    {
        i32 i;

        per (i = I; i < n; i++)
        {
               s32 l[III];
            Plexus p;
            Plexus q;
            Matrix matrix_p;
            Matrix matrix_q;

            si (i + I < n)
            {
                l[ZEPHYRUM]  = (s32)i;
                l[I]         = (s32)(i + I);
                l[II]        = (s32)i;
                (vacuum)plexus_creare(n, l, III, piscina, &p);
                l[ZEPHYRUM]  = (s32)(i + I);
                l[I]         = (s32)i;
                l[II]        = (s32)(i + I);
                (vacuum)plexus_creare(n, l, III, piscina, &q);
                (vacuum)plexus_burau(p, piscina, &matrix_p);
                (vacuum)plexus_burau(q, piscina, &matrix_q);
                si (!matrix_aequalis(matrix_p, matrix_q))
                {
                    fracta++;
                }
            }
            si (i + II < n)
            {
                l[ZEPHYRUM]  = (s32)i;
                l[I]         = -(s32)(i + II);
                (vacuum)plexus_creare(n, l, II, piscina, &p);
                l[ZEPHYRUM]  = -(s32)(i + II);
                l[I]         = (s32)i;
                (vacuum)plexus_creare(n, l, II, piscina, &q);
                (vacuum)plexus_burau(p, piscina, &matrix_p);
                (vacuum)plexus_burau(q, piscina, &matrix_q);
                si (!matrix_aequalis(matrix_p, matrix_q))
                {
                    fracta++;
                }
            }
        }
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
    sors_seminare(&s, CCXXII, I);
    per (t = ZEPHYRUM; t < CL; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 m     = II + sors_intra(&s, V);
                Plexus p    = _fortuitus(&s, m, sors_intra(&s, VIII),
                    piscina);
                Plexus q    = _fortuitus(&s, m, sors_intra(&s, VIII),
                    piscina);
                Plexus iunctus;
                Matrix matrix_p;
                Matrix matrix_q;
                Matrix matrix_iuncti;
                Matrix productum;
            Polynomium determinans;
            Polynomium exspectatus;
                   s64 e = plexus_summa_exponentium(p);

        (vacuum)plexus_iungere(p, q, piscina, &iunctus);
        (vacuum)plexus_burau(p, piscina, &matrix_p);
        (vacuum)plexus_burau(q, piscina, &matrix_q);
        (vacuum)plexus_burau(iunctus, piscina, &matrix_iuncti);
        (vacuum)matrix_multiplica(matrix_p, matrix_q, piscina,
            &productum);
        si (!matrix_aequalis(productum, matrix_iuncti))
        {
            fracta++;
        }
        /* det B(p) = (-t)^e */
        (vacuum)matrix_determinans(matrix_p, piscina, &determinans);
        (vacuum)polynomium_monomium(magnus_ex_s64((e % II == ZEPHYRUM)
            ? I : -I), (s32)e, piscina, &exspectatus);
        si (!polynomium_aequalis(determinans, exspectatus))
        {
            fracta++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}

/* B_3: partitio Burau = partitio Artin (fidelis); B_4: nulla classis
 * Artin scinditur */
interior chorda
_clavis_burau (
     Plexus  p,
    Piscina* piscina)
{
    Matrix b;

    (vacuum)plexus_burau(p, piscina, &b);
    redde matrix_ad_chordam(b, piscina);
}

interior vacuum
_partitio_burau (
         i32  n,
         i32  l,
     Piscina* piscina,
         i32* classes_liberae,
         i32* classes_burau,
         i32* discrepantiae)
{
           i32  litterae_numerus  = II * (n - I);
           i32  numerus           = ZEPHYRUM;
           i32  potentia          = I;
           i32  longitudo;
    ParClavium* pares;
           i32  k;

    per (longitudo = ZEPHYRUM; longitudo <= l; longitudo++)
    {
        numerus   += potentia;
        potentia  *= litterae_numerus;
    }
    pares = (ParClavium*)piscina_allocare(piscina,
        (memoriae_index)numerus
        * magnitudo(ParClavium));
    k         = ZEPHYRUM;
    potentia  = I;
    per (longitudo = ZEPHYRUM; longitudo <= l; longitudo++)
    {
        i32 index;

        per (index = ZEPHYRUM; index < potentia; index++)
        {
               s32 litterae[XVI];
               i32 r = index;
               i32 t;
            Plexus p;

            per (t = ZEPHYRUM; t < longitudo; t++)
            {
                i32 q = r % litterae_numerus;
                s32 i = (s32)(q / II) + I;

                litterae[t]  = (q % II) == ZEPHYRUM ? i : -i;
                r            /= litterae_numerus;
            }
            (vacuum)plexus_creare(n, litterae, longitudo, piscina, &p);
            pares[k].clavis_libera   = _clavis_libera(p, piscina);
            pares[k].clavis_ansarum  = _clavis_burau(p, piscina);
            k++;
        }
        potentia *= litterae_numerus;
    }
    qsort(pares, (size_t)numerus, magnitudo(ParClavium),
        _compara_liberam);
    *classes_liberae  = ZEPHYRUM;
    *discrepantiae    = ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   k == ZEPHYRUM
            || !chorda_aequalis(pares[k].clavis_libera,
            pares[k - I].clavis_libera))
        {
            (*classes_liberae)++;
        }
        alioquin si (!chorda_aequalis(pares[k].clavis_ansarum,
                     pares[k - I].clavis_ansarum))
        {
            (*discrepantiae)++;
        }
    }
    qsort(pares, (size_t)numerus, magnitudo(ParClavium),
        _compara_ansarum);
    *classes_burau = ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   k == ZEPHYRUM
            || !chorda_aequalis(pares[k].clavis_ansarum,
            pares[k - I].clavis_ansarum))
        {
            (*classes_burau)++;
        }
    }
}

interior vacuum
_probare_burau_fidelitatem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        i32  classes_liberae;
        i32  classes_burau;
        i32  discrepantiae;

    _partitio_burau(III, VI, piscina, &classes_liberae, &classes_burau,
        &discrepantiae);
    imprimere("  B_3 <= 6: %u classes Artin, %u Burau, %u discr.\n",
        classes_liberae, classes_burau, discrepantiae);
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (classes_burau, classes_liberae);
    _partitio_burau(IV, V, piscina, &classes_liberae, &classes_burau,
        &discrepantiae);
    imprimere("  B_4 <= 5: %u classes Artin, %u Burau, %u discr.\n",
        classes_liberae, classes_burau, discrepantiae);
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    CREDO_VERUM (classes_burau <= classes_liberae);
}

/* B(-1) integra = B(t) ad t = -1 */
interior vacuum
_probare_burau_minus_uno (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  fracta      = ZEPHYRUM;
    Fractio  minus_unum  = fractio_ex_s64(-I);

    sors_seminare(&s, CCCXXXVII, I);
    per (t = ZEPHYRUM; t < CL; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = II + sors_intra(&s, VI);
                Plexus p    = _fortuitus(&s, n, sors_intra(&s, XII),
                    piscina);
                Matrix b;
                Matrix integra;
                   i32 r;
                   i32 k;

        (vacuum)plexus_burau(p, piscina, &b);
        (vacuum)plexus_burau_minus_uno(p, piscina, &integra);
        per (r = ZEPHYRUM; r + I < n; r++)
        {
            per (k = ZEPHYRUM; k + I < n; k++)
            {
                Fractio valor;

                (vacuum)polynomium_valor(*(constans Polynomium*)
                    matrix_elementum(b, r, k), minus_unum, piscina,
                    &valor);
                si (!fractio_aequalis(valor, fractio_ex_magno(
                    *(constans Magnus*)matrix_elementum(integra, r,
                    k))))
                {
                    fracta++;
                }
            }
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}

/* Alexander clausurae contra tabula_nodorum (laqueus ex codicibus PD
 * KnotInfo): via omnino independens */
interior b32
_alexander_tabulae (
    constans character* titulus,
               Piscina* piscina,
            Polynomium* exitus)
{
    i32 k;

    per (k = ZEPHYRUM; k < (i32)TABULA_NODORUM_NUMERUS; k++)
    {
        si (strcmp(TABULA_NODORUM[k].titulus, titulus) == ZEPHYRUM)
        {
            redde polynomium_ex_chorda(chorda_ex_literis(
                TABULA_NODORUM[k].alexander, piscina), 't', piscina,
                exitus);
        }
    }
    redde FALSUM;
}

interior vacuum
_probare_alexander (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    /* verba braidarum notissima; litterae a sinistra */
    constans character* tituli[VIII] = { "3_1", "4_1", "5_1", "5_2",
        "6_1", "6_2", "6_3", "7_1" };
    constans character* verba[VIII]  = { "1 1 1", "1 -2 1 -2",
        "1 1 1 1 1", "1 1 1 2 -1 2", "1 1 2 -1 -3 2 -3",
        "1 1 1 -2 1 -2", "1 1 -2 1 -2 -2", "1 1 1 1 1 1 1" };
               i32 fila[VIII]   = { 2, 3, 2, 3, 4, 3, 3, 2 };
               i32 k;
        Polynomium a;
        Polynomium b;
              Sors s;
               i32 t;
               i32 fracta = ZEPHYRUM;

    per (k = ZEPHYRUM; k < VIII; k++)
    {
        b32 congruit = FALSUM;

        si (   _alexander_tabulae(tituli[k], piscina, &b)
            && plexus_alexander(_p(fila[k], verba[k], piscina), piscina,
            &a))
        {
            congruit = polynomium_aequalis(a, b);
        }
        imprimere("  %s = clausura '%s': %s (%.*s)\n", tituli[k],
            verba[k], congruit ? "congruit" : "DISCREPAT",
            (integer)polynomium_ad_chordam(a, 't', piscina).mensura,
            polynomium_ad_chordam(a, 't', piscina).datum);
        CREDO_VERUM (congruit);
    }
    /* catena Hopf (s1^2): 1 - t; catena disiuncta (B_2 vacuum): 0;
     * nodus trivialis (s1 in B_2): 1 */
    CREDO_VERUM (plexus_alexander(_p(II, "1 1", piscina), piscina, &a));
    (vacuum)polynomium_ex_chorda(chorda_ex_literis("1 - t", piscina),
        't',
        piscina, &b);
    (vacuum)polynomium_normale(b, piscina, &b);
    CREDO_VERUM (polynomium_aequalis(a, b));
    CREDO_VERUM (plexus_alexander(_p(II, "", piscina), piscina, &a)
        && polynomium_est_nullum(a));
    CREDO_VERUM (plexus_alexander(_p(II, "1", piscina), piscina, &a));
    (vacuum)polynomium_ex_chorda(chorda_ex_literis("1", piscina), 't',
        piscina, &b);
    CREDO_VERUM (polynomium_aequalis(a, b));
    /* motus Markov: coniugatio et stabilizatio Alexander servant */
    sors_seminare(&s, DCCXXXI, I);
    per (t = ZEPHYRUM; t < C; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = II + sors_intra(&s, IV);
                Plexus beta = _fortuitus(&s, n, I + sors_intra(&s,
                    VIII),
                    piscina);
                Plexus w    = _fortuitus(&s, n, sors_intra(&s, V),
                    piscina);
                Plexus coniugatum;
                Plexus maior;
                Plexus stabilitum;
                   s32 littera = sors_intra(&s, II) == ZEPHYRUM
                       ? (s32)n : -(s32)n;
            Polynomium d0;
            Polynomium d1;
            Polynomium d2;

        (vacuum)plexus_iungere(w, beta, piscina, &coniugatum);
        (vacuum)plexus_iungere(coniugatum, plexus_inversum(w, piscina),
            piscina, &coniugatum);
        /* stabilizatio: beta in B_{n+1} deinde sigma_n^{+-1} */
        (vacuum)plexus_in_maiore(beta, n + I, ZEPHYRUM, piscina,
            &maior);
        {
            Plexus sigma_n;

            (vacuum)plexus_creare(n + I, &littera, I, piscina,
                &sigma_n);
            (vacuum)plexus_iungere(maior, sigma_n, piscina,
                &stabilitum);
        }
        si (   !plexus_alexander(beta, piscina, &d0)
            || !plexus_alexander(coniugatum, piscina, &d1)
            || !plexus_alexander(stabilitum, piscina, &d2)
            || !polynomium_aequalis(d0, d1)
            || !polynomium_aequalis(d0, d2))
        {
            fracta++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}

/* radix maxima Burau(-1): exacta in B_3, limes inferior incrementi */
interior b32
_radix_maxima_polynomii (
    constans character* textus,
               Piscina* piscina,
           RadixRealis* exitus)
{
     Polynomium  f;
    RadixRealis* radices;
            i32  numerus;

    si (   !polynomium_ex_chorda(chorda_ex_literis(textus, piscina),
        't',
        piscina, &f)
        || !radices_reales(f, piscina, &radices, &numerus)
        || numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *exitus = radices[numerus - I];
    redde VERUM;
}

interior vacuum
_probare_radicem_burau (
    CredoContextus* c)
{
        Piscina* piscina = c->piscina;
    RadixRealis  rho;
    RadixRealis  exspectata;
            s32  signum = XCIX;
         chorda  decimalis;

    /* s1 s2^-1: (3 + sqrt 5)/2, radix maxima t^2 - 3t + 1 */
    CREDO_VERUM (plexus_burau_radix_maxima(_p(III, "1 -2", piscina),
        piscina, &rho));
    CREDO_VERUM (_radix_maxima_polynomii("t^2 - 3t + 1", piscina,
        &exspectata));
    CREDO_VERUM (radix_compara(rho, exspectata, piscina, &signum)
        && signum == ZEPHYRUM);
    /* (s1 s2^-1)^2: radix t^2 - 7t + 1 */
    CREDO_VERUM (plexus_burau_radix_maxima(_p(III, "1 -2 1 -2",
        piscina),
        piscina, &rho));
    CREDO_VERUM (_radix_maxima_polynomii("t^2 - 7t + 1", piscina,
        &exspectata));
    CREDO_VERUM (radix_compara(rho, exspectata, piscina, &signum)
        && signum == ZEPHYRUM);
    /* s1 s2^-1 Delta^2: B(-1)(Delta^2) = -I in B_3, ergo valores
     * proprii negativi (-lambda, -1/lambda): radix maxima tamen
     * (3 + sqrt 5)/2 per chi(x) chi(-x) */
    CREDO_VERUM (plexus_burau_radix_maxima(_p(III,
        "1 -2 1 2 1 2 1 2", piscina), piscina, &rho));
    CREDO_VERUM (_radix_maxima_polynomii("t^2 - 3t + 1", piscina,
        &exspectata));
    CREDO_VERUM (radix_compara(rho, exspectata, piscina, &signum)
        && signum == ZEPHYRUM);
    {
        Matrix b;

        (vacuum)plexus_burau_minus_uno(_p(III, "1 2 1 2 1 2", piscina),
            piscina, &b);
        CREDO_VERUM (_textus(matrix_ad_chordam(b, piscina),
            "[-1, 0; 0, -1]"));
    }
    /* periodica s1 s2: 1 */
    CREDO_VERUM (plexus_burau_radix_maxima(_p(III, "1 2", piscina),
        piscina, &rho));
    CREDO_VERUM (_radix_maxima_polynomii("t - 1", piscina,
        &exspectata));
    CREDO_VERUM (radix_compara(rho, exspectata, piscina, &signum)
        && signum == ZEPHYRUM);
    /* IV fila (Thiffeault Fig. 10): dilatatio ~2.2966; limes <= */
    CREDO_VERUM (plexus_burau_radix_maxima(_p(IV, "-3 -2 -3 2 1",
        piscina), piscina, &rho));
    (vacuum)radix_ad_chordam(rho, VI, piscina, &decimalis);
    imprimere("  radix maxima Burau(-1), s3^-1 s2^-1 s3^-1 s2 s1: %.*s "
        "(dilatatio ~2.29663)\n", (integer)decimalis.mensura,
        decimalis.datum);
    CREDO_VERUM (radix_compara(rho, radix_ex_fractione(
        _fractio(229664L, 100000L, piscina), piscina), piscina, &signum)
        && signum <= ZEPHYRUM);
}

/* B_3 fortuita: radix Burau(-1) = ratio incrementi L (dilatatio) */
interior vacuum
_probare_radicem_contra_ansas (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  probatae  = ZEPHYRUM;
        i32  fracta    = ZEPHYRUM;

    sors_seminare(&s, CMXI, I);
    per (t = ZEPHYRUM; t < LX; t++)
    {
        PiscinaNotatio nota = piscina_notare(piscina);
                Plexus w    = _fortuitus(&s, III, II + sors_intra(&s,
                    V),
                    piscina);
           RadixRealis rho;
                  Ansa u;
                Magnus prior;
                Magnus nunc;
                   i32 m;
                   s32 signum;

        (vacuum)plexus_burau_radix_maxima(w, piscina, &rho);
        /* solum pseudo-Anosov manifesti: rho > 11/10 */
        si (   !radix_compara(rho, radix_ex_fractione(_fractio(XI, X,
            piscina), piscina), piscina, &signum)
            || signum <= ZEPHYRUM)
        {
            piscina_reficere(piscina, nota);
            perge;
        }
        (vacuum)ansa_circa(III, I, piscina, &u);
        prior  = ansa_sectiones(u, piscina);
        nunc   = prior;
        per (m = I; m <= XL; m++)
        {
            (vacuum)ansa_agere(u, w, piscina, &u);
            prior  = nunc;
            nunc   = ansa_sectiones(u, piscina);
        }
        /* (99/100) L_40 / L_39 <= rho <= (101/100) L_40 / L_39 */
        {
            Fractio ratio;
            Fractio infra;
            Fractio supra;
                s32 s_infra;
                s32 s_supra;

            (vacuum)fractio_ex_magnis(nunc, prior, piscina, &ratio);
            infra = fractio_multiplica(ratio, _fractio(XCIX, C,
                piscina),
                piscina);
            supra = fractio_multiplica(ratio, _fractio(CI, C, piscina),
                piscina);
            si (   !radix_compara(rho, radix_ex_fractione(infra,
                piscina),
                piscina, &s_infra)
                || !radix_compara(rho, radix_ex_fractione(supra,
                piscina),
                piscina, &s_supra)
                || s_infra < ZEPHYRUM || s_supra > ZEPHYRUM)
            {
                fracta++;
            }
        }
        probatae++;
        piscina_reficere(piscina, nota);
    }
    imprimere("  B_3: %u braidae pseudo-Anosov probatae\n", probatae);
    CREDO_VERUM (probatae >= X);
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}

/* contractio in Burau: verba longa, B(w w^-1) = I exacte */
interior vacuum
_probare_burau_longum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        s32* litterae = (s32*)piscina_allocare(piscina,
            (memoriae_index)12000 * magnitudo(s32));
     Plexus w;
     Plexus cum_inverso;
     Matrix b;
     Matrix unitas;
     Matrix integra_w;
     Matrix integra_inversa;
     Matrix productum;
        i64 ante;
        i32 k;

    /* (s1 s2^-1)^300 deinde inversum: polynomia gradus ~600 */
    per (k = ZEPHYRUM; k < 600UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = -II;
    }
    (vacuum)plexus_creare(III, litterae, 600UL, piscina, &w);
    (vacuum)plexus_iungere(w, plexus_inversum(w, piscina), piscina,
        &cum_inverso);
    ante = plexus_contractiones();
    CREDO_VERUM (plexus_burau(cum_inverso, piscina, &b));
    imprimere("  contractiones Burau (w w^-1): %lu\n",
        (insignatus longus)(plexus_contractiones() - ante));
    CREDO_VERUM (plexus_contractiones() - ante >= I);
    (vacuum)matrix_identitas(&ANULUS_POLYNOMIORUM, II, piscina,
        &unitas);
    CREDO_VERUM (matrix_aequalis(b, unitas));
    /* B(-1) integra: (s1 s2^-1)^3000, ~4165 bitorum */
    per (k = ZEPHYRUM; k < 6000UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = -II;
    }
    (vacuum)plexus_creare(III, litterae, 6000UL, piscina, &w);
    ante = plexus_contractiones();
    CREDO_VERUM (plexus_burau_minus_uno(w, piscina, &integra_w));
    CREDO_VERUM (plexus_burau_minus_uno(plexus_inversum(w, piscina),
        piscina, &integra_inversa));
    CREDO_VERUM (plexus_contractiones() - ante >= II);
    CREDO_VERUM (matrix_multiplica(integra_w, integra_inversa, piscina,
        &productum));
    (vacuum)matrix_identitas(&ANULUS_INTEGRORUM, II, piscina, &unitas);
    CREDO_VERUM (matrix_aequalis(productum, unitas));
}


/* ==================================================
 * T4: entropia per incrementum ansarum
 * ================================================== */

interior vacuum
_probare_incrementum_functionis (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
            Plexus  p;
              Ansa  u;
              Ansa  v;
            Magnus* longitudines;
               i32  m;
               b32  idem = VERUM;
    memoriae_index  ante;

    /* (s1 s2^-1)^m in E_1: idem ac iteratio manu ansa_agere (T2) */
    p = _p(III, "1 -2", piscina);
    (vacuum)ansa_circa(III, I, piscina, &u);
    CREDO_VERUM (plexus_incrementum(p, u, XX, piscina, &longitudines));
    v = u;
    per (m = ZEPHYRUM; m <= XX; m++)
    {
        si (!magnus_aequalis(longitudines[m], ansa_sectiones(v,
            piscina)))
        {
            idem = FALSUM;
        }
        (vacuum)ansa_agere(v, p, piscina, &v);
    }
    CREDO_VERUM (idem);
    /* ratio ad (3 + sqrt 5)/2: |L_{m+1}^2 - 3 L_{m+1} L_m + L_m^2| <
     * L_m^2 / 1000, m = 10 .. 19 */
    idem = VERUM;
    per (m = X; m < XX; m++)
    {
        s64 a = _s64(longitudines[m]);
        s64 b = _s64(longitudines[m + I]);
        s64 r = b * b - III * a * b + a * a;

        si ((r < ZEPHYRUM ? -r : r) * M >= a * a)
        {
            idem = FALSUM;
        }
    }
    CREDO_VERUM (idem);
    /* braida IV filorum Thiffeault: ratio 2.2966 (0.1%), m 20 .. 29 */
    (vacuum)ansa_circa(IV, I, piscina, &u);
    CREDO_VERUM (plexus_incrementum(_p(IV, "-3 -2 -3 2 1", piscina), u,
        XXX, piscina, &longitudines));
    idem = VERUM;
    per (m = XX; m < XXX; m++)
    {
        s64 a = _s64(longitudines[m]);
        s64 b = _s64(longitudines[m + I]);
        s64 d = 10000L * b - 22966L * a;

        si ((d < ZEPHYRUM ? -d : d) > 10L * a)
        {
            idem = FALSUM;
        }
    }
    CREDO_VERUM (idem);
    /* reducibilis: s1 in E_2 (torsio circa puncta 1, 2), L_m = 2 + 2m
     * (observatum; crescentia linearis) */
    (vacuum)ansa_circa(III, II, piscina, &u);
    CREDO_VERUM (plexus_incrementum(_p(III, "1", piscina), u, XXX,
        piscina, &longitudines));
    idem = VERUM;
    per (m = ZEPHYRUM; m <= XXX; m++)
    {
        si (_s64(longitudines[m]) != (s64)(II + II * m))
        {
            idem = FALSUM;
        }
    }
    CREDO_VERUM (idem);
    /* periodica: (s1 s2)^3 = Delta^2, L constans 2 */
    (vacuum)ansa_circa(III, I, piscina, &u);
    CREDO_VERUM (plexus_incrementum(_p(III, "1 2 1 2 1 2", piscina), u,
        XX, piscina, &longitudines));
    idem = VERUM;
    per (m = ZEPHYRUM; m <= XX; m++)
    {
        si (_s64(longitudines[m]) != II)
        {
            idem = FALSUM;
        }
    }
    CREDO_VERUM (idem);
    /* piscina vocantis: solum longitudines (periodi + 1 Magnus) */
    (vacuum)ansa_circa(III, I, piscina, &u);
    ante = piscina_summa_usus(piscina);
    CREDO_VERUM (plexus_incrementum(p, u, CC, piscina, &longitudines));
    imprimere("  L post (s1 s2^-1)^200: %u bita; piscina +%lu octeti\n",
        magnus_bitorum(longitudines[CC]), (insignatus longus)
        (piscina_summa_usus(piscina) - ante));
    CREDO_VERUM (piscina_summa_usus(piscina) - ante <= (memoriae_index)
        CCI * magnitudo(Magnus) + CCI * LXIV + 4096UL);
    /* refutationes */
    longitudines = NIHIL;
    CREDO_FALSUM (plexus_incrementum(_p(IV, "1", piscina), u, V,
        piscina,
        &longitudines));
    v    = u;
    v.a  = NIHIL;
    CREDO_FALSUM (plexus_incrementum(p, v, V, piscina, &longitudines));
    {
        Plexus malus  = p;
           s32 mala   = III;

        malus.litterae   = &mala;
        malus.longitudo  = I;
        CREDO_FALSUM (plexus_incrementum(malus, u, V, piscina,
            &longitudines));
    }
    CREDO_VERUM (longitudines == NIHIL);
}

/* oraculum 6: radix Burau(-1) incrementum non superat. g = max_k
 * L^(k)_40 / L^(k)_30 super ansas E_k: radix^10 <= (101/100) g */
interior vacuum
_probare_limitem_contra_incrementum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  exponentiales  = ZEPHYRUM;
        i32  fracta         = ZEPHYRUM;

    sors_seminare(&s, V, I);
    per (t = ZEPHYRUM; t < XL; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, III);
                Plexus p    = _fortuitus(&s, n, III + sors_intra(&s,
                    VI),
                    piscina);
           RadixRealis rho;
           RadixRealis angusta;
               Fractio maxima = fractio_ex_s64(ZEPHYRUM);
               Fractio decima;
                   s32 signum;
                   i32 k;

        (vacuum)plexus_burau_radix_maxima(p, piscina, &rho);
        per (k = I; k < n; k++)
        {
               Ansa  u;
             Magnus* longitudines;
            Fractio  ratio;

            (vacuum)ansa_circa(n, k, piscina, &u);
            (vacuum)plexus_incrementum(p, u, XL, piscina,
                &longitudines);
            (vacuum)fractio_ex_magnis(longitudines[XL],
                longitudines[XXX],
                piscina, &ratio);
            si (fractio_compara(ratio, maxima, piscina) > ZEPHYRUM)
            {
                maxima = ratio;
            }
        }
        /* radix > 1: pars pseudo-Anosov manifesta */
        si (   radix_compara(rho, radix_ex_fractione(fractio_ex_s64(I),
            piscina), piscina, &signum)
            && signum > ZEPHYRUM)
        {
            exponentiales++;
        }
        (vacuum)radix_angusta(rho, _fractio(I, 1000000L, piscina),
            piscina,
            &angusta);
        (vacuum)fractio_potentia(angusta.infra, X, piscina, &decima);
        si (fractio_compara(decima, fractio_multiplica(maxima,
            _fractio(CI,
            C, piscina), piscina), piscina) > ZEPHYRUM)
        {
            fracta++;
        }
        piscina_reficere(piscina, nota);
    }
    imprimere("  XL braidae B_3..B_5: %u cum radice > 1, %u fractae\n",
        exponentiales, fracta);
    CREDO_VERUM (exponentiales >= V);
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
}


/* ==================================================
 * Recensio T5 (2026-10-15): M1-M3, L1-L6
 * ================================================== */

/* M1: aequalitas per ansas memoria O(n) - piscina nova, apex eius */
interior vacuum
_probare_censurae_ansarum_memoriam (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Piscina* propria;
     Plexus  p;
     Plexus  q;
        s32  l[IV];
        b32  aequales = XCIX;

    /* r = s_3997^2 s_3999^-2 in B_4000: purus, summa nulla, ansas
     * E_1 .. ~E_3996 fixas relinquit - ansae fere omnes currunt
     * antequam discrepantia invenitur (olim (n+1) 4n Magnus in piscina
     * vocantis: ~860 MB) */
    propria = piscina_generare_dynamicum("probatio_ansae_n", 65536);
    l[ZEPHYRUM] = 3997L;
    l[I] = 3997L;
    l[II] = -3999L;
    l[III] = -3999L;
    (vacuum)plexus_creare(4000UL, l, IV, propria, &p);
    (vacuum)plexus_creare(4000UL, l, ZEPHYRUM, propria, &q);
    CREDO_VERUM (plexus_aequales_ansis(p, q, propria, &aequales)
        && !aequales);
    imprimere("  apex piscinae vocantis (n = 4000): %lu octeti\n",
        (insignatus longus)piscina_summa_apex_usus(propria));
    CREDO_VERUM (piscina_summa_apex_usus(propria) < 8388608UL);
    /* r vacuum et praefiltra: statim, etiam n = 100000 */
    l[ZEPHYRUM]  = I;
    l[I]         = I;
    (vacuum)plexus_creare(100000UL, l, II, propria, &p);
    CREDO_VERUM (plexus_aequales_ansis(p, p, propria, &aequales)
        && aequales);
    (vacuum)plexus_creare(100000UL, l, I, propria, &q);
    CREDO_VERUM (plexus_aequales_ansis(p, q, propria, &aequales)
        && !aequales);
    piscina_destruere(propria);
    (vacuum)piscina;
}

/* M2, L5: piscinae propriae per apicem (plexus_apex_proprius) */
interior vacuum
_probare_censurae_apicem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        s32* litterae = (s32*)piscina_allocare(piscina,
            (memoriae_index)30000 * magnitudo(s32));
     Plexus  p;
       Ansa  u;
       Ansa  v;
     Magnus* longitudines;
        i64  ante;
        i32  k;

    /* M2: (s1 s2^-1)^15000 in E_1 de D_65536: verbum fila brevius;
     * olim nulla contractio, ~776 MB */
    per (k = ZEPHYRUM; k < 30000UL; k += II)
    {
        litterae[k]      = I;
        litterae[k + I]  = -II;
    }
    (vacuum)plexus_creare(65536UL, litterae, 30000UL, piscina, &p);
    (vacuum)ansa_circa(65536UL, I, piscina, &u);
    plexus_apicem_purgare();
    ante = plexus_contractiones();
    CREDO_VERUM (ansa_agere(u, p, piscina, &v));
    imprimere("  D_65536, (s1 s2^-1)^15000: apex proprius %lu, "
        "contractiones %lu\n",
        (insignatus longus)plexus_apex_proprius(),
        (insignatus longus)(plexus_contractiones() - ante));
    CREDO_VERUM (plexus_contractiones() - ante >= I);
    CREDO_VERUM (plexus_apex_proprius() < 67108864UL);
    /* L5: incrementum per piscinas alternantes: apex parvus */
    (vacuum)ansa_circa(III, I, piscina, &u);
    plexus_apicem_purgare();
    CREDO_VERUM (plexus_incrementum(_p(III, "1 -2", piscina), u, M,
        piscina, &longitudines));
    imprimere("  incrementum M periodorum: apex proprius %lu\n",
        (insignatus longus)plexus_apex_proprius());
    /* ~1.4 KB per gradum; piscina numquam vacata ~250 KB */
    CREDO_VERUM (plexus_apex_proprius() < 65536UL);
}

/* M3, L2, L4: plexus et ansae invalidae; fines */
interior vacuum
_probare_censurae_fines (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Plexus  malus;
     Plexus  bonus  = _p(III, "1 -2", piscina);
        s32  mala   = III;
     Matrix  m;
 Polynomium  a;
RadixRealis  rho;
       Ansa  u;
       Ansa  v;
       Ansa  mala_ansa;
     Magnus* longitudines = NIHIL;
        b32  aequales;
     Magnus  coordinatae[II];

    malus.fila       = III;
    malus.longitudo  = I;
    malus.litterae   = &mala;
    /* M3: functiones Burau, exitus non tactus */
    m.lineae = XCIX;
    CREDO_FALSUM (plexus_burau(malus, piscina, &m));
    CREDO_FALSUM (plexus_burau_minus_uno(malus, piscina, &m));
    CREDO_AEQUALIS_I32 (m.lineae, XCIX);
    a.numerus = XCIX;
    CREDO_FALSUM (plexus_alexander(malus, piscina, &a));
    CREDO_AEQUALIS_I32 (a.numerus, XCIX);
    rho.f.numerus = XCIX;
    CREDO_FALSUM (plexus_burau_radix_maxima(malus, piscina, &rho));
    CREDO_AEQUALIS_I32 (rho.f.numerus, XCIX);
    /* L4: q malus cum p bono, utraque methodus */
    aequales = XCIX;
    CREDO_FALSUM (plexus_aequales(bonus, malus, LIMES_MAGNUS, piscina,
        &aequales));
    CREDO_FALSUM (plexus_aequales_ansis(bonus, malus, piscina,
        &aequales));
    CREDO_FALSUM (plexus_aequales(malus, bonus, LIMES_MAGNUS, piscina,
        &aequales));
    CREDO_VERUM (aequales == XCIX);
    /* L4: ansa_ex_coordinatis supra fines; incrementum periodi et
     * fila (periodi 0) */
    coordinatae[ZEPHYRUM]  = magnus_ex_s64(ZEPHYRUM);
    coordinatae[I]         = magnus_ex_s64(ZEPHYRUM);
    u.fila                 = XCIX;
    CREDO_FALSUM (ansa_ex_coordinatis(PLEXUS_FILA_MAXIMA + I,
        coordinatae,
        coordinatae, piscina, &u));
    CREDO_AEQUALIS_I32 (u.fila, XCIX);
    (vacuum)ansa_circa(III, I, piscina, &u);
    CREDO_FALSUM (plexus_incrementum(bonus, u, 0x7FFFFFFFUL, piscina,
        &longitudines));
    CREDO_FALSUM (plexus_incrementum(_p(IV, "1", piscina), u, ZEPHYRUM,
        piscina, &longitudines));
    CREDO_VERUM (longitudines == NIHIL);
    /* L2: ansa manu facta invalida */
    mala_ansa.fila  = II;
    mala_ansa.a     = coordinatae;
    mala_ansa.b     = coordinatae;
    CREDO_FALSUM (ansa_valida(mala_ansa));
    CREDO_VERUM (magnus_signum(ansa_sectiones(mala_ansa, piscina))
        == ZEPHYRUM);
    CREDO_FALSUM (ansae_aequales(mala_ansa, mala_ansa));
    CREDO_VERUM (ansa_ad_chordam(mala_ansa, piscina).mensura
        == ZEPHYRUM);
    v.fila = XCIX;
    CREDO_FALSUM (ansa_agere(mala_ansa, _p(II, "1", piscina), piscina,
        &v));
    CREDO_AEQUALIS_I32 (v.fila, XCIX);
    mala_ansa.fila  = III;
    mala_ansa.b     = NIHIL;
    CREDO_FALSUM (ansa_valida(mala_ansa));
    CREDO_FALSUM (plexus_incrementum(bonus, mala_ansa, V, piscina,
        &longitudines));
    CREDO_VERUM (ansa_valida(u));
}

/* L1: Burau temporaria non in piscina vocantis */
interior vacuum
_probare_censurae_burau_piscinam (
    CredoContextus* c)
{
            Piscina* piscina = c->piscina;
             Plexus  p;
             Matrix  b;
     memoriae_index  ante;
                s32  l = I;

    (vacuum)plexus_creare(CC, &l, I, piscina, &p);
    ante = piscina_summa_usus(piscina);
    CREDO_VERUM (plexus_burau_minus_uno(p, piscina, &b));
    imprimere("  Burau(-1) n = 200: piscina vocantis +%lu octeti\n",
        (insignatus longus)(piscina_summa_usus(piscina) - ante));
    /* matrix ipsa: 199^2 Magnus (et capita); nihil aliud */
    CREDO_VERUM (piscina_summa_usus(piscina) - ante
        <= (memoriae_index)CXCIX * CXCIX * magnitudo(Magnus) + 65536UL);
}


/* ==================================================
 * Certificatum Burau in radicibus unitatis (D128 recensio M3)
 * ================================================== */

interior vacuum
_probare_burau_circulum (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* nullae[VI] = { "1 2 3", "1 1 2 3",
        "1 2 3 1 2 1 1 2 3 1 2 1",
        "1", "1 1 1 1 1", "1 3" };
     i32 k;
     b32 maior;
    Sors s;
     i32 t;
     i32 fracta_paris       = ZEPHYRUM;
     i32 fracta_incrementi  = ZEPHYRUM;
     i32 certificatae       = ZEPHYRUM;

    /* entropia nulla -> FALSUM (Kolev contrapositive), ordo 2 .. 6 */
    per (k = ZEPHYRUM; k < VI; k++)
    {
        i32 ordo;
        b32 nulla = VERUM;

        per (ordo = II; ordo <= VI; ordo++)
        {
            maior = XCIX;
            si (   !plexus_burau_radius_maior(_p(IV, nullae[k],
                piscina),
                ordo, &maior)
                || maior)
            {
                nulla = FALSUM;
            }
        }
        CREDO_VERUM (nulla);
    }
    /* s1 s3^-1 s2: Burau(-1) caecus ((x-1)^3), Burau(omega) certificat;
     * s3^-1 s2^-1 s3^-1 s2 s1 (Thiffeault) idem */
    CREDO_VERUM (plexus_burau_radius_maior(_p(IV, "1 -3 2", piscina),
        II,
        &maior) && !maior);
    CREDO_VERUM (plexus_burau_radius_maior(_p(IV, "1 -3 2", piscina),
        III,
        &maior) && maior);
    CREDO_VERUM (plexus_burau_radius_maior(_p(IV, "-3 -2 -3 2 1",
        piscina), III, &maior) && maior);
    /* s2^2 s1^-2: torsiones circa curvas secantes signis oppositis
     * (constructio Penner): pseudo-Anosov, certificatum ad ordinem 2 */
    CREDO_VERUM (plexus_burau_radius_maior(_p(IV, "2 2 -1 -1", piscina),
        II, &maior) && maior);
    /* B_3: s1 s2^-1 ordo 2 */
    CREDO_VERUM (plexus_burau_radius_maior(_p(III, "1 -2", piscina), II,
        &maior) && maior);
    /* refutationes, maior non tactus */
    maior = XCIX;
    CREDO_FALSUM (plexus_burau_radius_maior(_p(IV, "1", piscina), I,
        &maior));
    CREDO_FALSUM (plexus_burau_radius_maior(_p(IV, "1", piscina), XIII,
        &maior));
    CREDO_FALSUM (plexus_burau_radius_maior(_p(XXIV, "1", piscina), III,
        &maior));
    CREDO_VERUM (maior == XCIX);
    /* fortuita: ordo 2 = radix_maxima > 1 in B_3, B_4 (p(1) = 0);
     * certificatum (ordo 2 aut 3) -> incrementum exponentiale */
    sors_seminare(&s, CCCLV, I);
    per (t = ZEPHYRUM; t < CL; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, II);
                Plexus p    = _fortuitus(&s, n, II + sors_intra(&s, VI),
                    piscina);
           RadixRealis rho;
                   s32 signum     = ZEPHYRUM;
                   b32 maior_ii   = FALSUM;
                   b32 maior_iii  = FALSUM;
                   i32 r;
                   b32 crescit = FALSUM;

        (vacuum)plexus_burau_radix_maxima(p, piscina, &rho);
        (vacuum)radix_compara(rho, radix_ex_fractione(fractio_ex_s64(I),
            piscina), piscina, &signum);
        (vacuum)plexus_burau_radius_maior(p, II, &maior_ii);
        (vacuum)plexus_burau_radius_maior(p, III, &maior_iii);
        si (maior_ii != (signum > ZEPHYRUM))
        {
            fracta_paris++;
        }
        per (r = I; r < n; r++)
        {
              Ansa  u;
            Magnus* longitudines;

            (vacuum)ansa_circa(n, r, piscina, &u);
            (vacuum)plexus_incrementum(p, u, LX, piscina,
                &longitudines);
            si (magnus_compara(longitudines[LX], magnus_multiplica(
                longitudines[XL], magnus_ex_s64(II), piscina))
                    > ZEPHYRUM)
            {
                crescit = VERUM;
            }
        }
        si ((maior_ii || maior_iii) && !crescit)
        {
            fracta_incrementi++;
        }
        si (maior_iii && !maior_ii)
        {
            certificatae++;
        }
        piscina_reficere(piscina, nota);
    }
    imprimere("  CL braidae: %u certificatae solum ad ordinem 3\n",
        certificatae);
    CREDO_AEQUALIS_I32 (fracta_paris, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fracta_incrementi, ZEPHYRUM);
}

/* plexus_nervus (nervus-plan T3): rete = ansa_agere (ancora D128 Partis
 * A), figura per litteram, verbum vacuum, compositio = iunctio,
 * exemplum Fibonacci, refutationes */
interior vacuum
_probare_nervum (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
       Sors  s;
        i32  t;
        i32  k;
        i32  comparata = ZEPHYRUM;
        i32  discrepantiae = ZEPHYRUM;
     Magnus  scala = magnus_potentia(magnus_ex_s64(II), CC, piscina);

    /* ancora: MMM verba fortuita, n = 3..8, longitudo 0..20,
     * coordinatae |x| <= 1000, quarta quaeque scalata 2^200 */
    sors_seminare(&s, CXXVIII, I);
    per (t = ZEPHYRUM; t < MMM; t++)
    {
        PiscinaNotatio nota  = piscina_notare(piscina);
                   i32 n     = III + sors_intra(&s, VI);
                   i32 m     = n - II;
                Plexus p = _fortuitus(&s, n, sors_intra(&s, XXI),
                    piscina);
               Nervus* rete;
               Magnus  a[VI];
               Magnus  b[VI];
               Magnus  x[XII];
               Magnus  y[XII];
                 Ansa  u;
                 Ansa  v;
                  b32  idem = VERUM;

        per (k = ZEPHYRUM; k < m; k++)
        {
            a[k] = magnus_ex_s64((s64)sors_inter(&s, -M, M));
            b[k] = magnus_ex_s64((s64)sors_inter(&s, -M, M));
            si (t % IV == III)
            {
                a[k] = magnus_multiplica(a[k], scala, piscina);
                b[k] = magnus_multiplica(b[k], scala, piscina);
            }
            x[k]      = a[k];
            x[m + k]  = b[k];
        }
        si (   !ansa_ex_coordinatis(n, a, b, piscina, &u)
            || !ansa_agere(u, p, piscina, &v)
            || !plexus_nervus(p, piscina, &rete)
            || !nervus_aestimare(rete, x, y, NIHIL, piscina))
        {
            idem = FALSUM;
        }
        alioquin
        {
            per (k = ZEPHYRUM; k < m; k++)
            {
                si (   !magnus_aequalis(y[k], v.a[k])
                    || !magnus_aequalis(y[m + k], v.b[k]))
                {
                    idem = FALSUM;
                }
            }
        }
        comparata++;
        si (!idem)
        {
            discrepantiae++;
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_AEQUALIS_I32 (comparata, MMM);
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    /* figura per litteram (D128 Pars A): extremae II ReLU et IV
     * instructiones, mediae V et XI; profunditas IV, ReLU II.
     * Latitudo (registra viva, nervus.h): extremae 2n - 3, mediae
     * 2n + 2 - temporaria affinia (d, pars) numerantur, dissimile
     * "status + V ReLU" (D128) */
    {
        b32 recte = VERUM;
        i32 n;

        per (n = III; n <= VIII; n++)
        {
            s32 littera;

            per (littera = -(s32)(n - I); littera <= (s32)(n - I);
                littera++)
            {
                  Plexus  p;
                  Nervus* rete;
            NervusFigura  fig;
                     i32  i = (i32)(littera > ZEPHYRUM ? littera
                              : -littera);
                     b32 extrema = i == I || i == n - I;

                si (littera == ZEPHYRUM)
                {
                    perge;
                }
                si (   !plexus_creare(n, &littera, I, piscina, &p)
                    || !plexus_nervus(p, piscina, &rete)
                    || !nervus_figura(rete, &fig))
                {
                    recte = FALSUM;
                    perge;
                }
                si (   fig.relu             != (extrema ? II : V)
                    || fig.instructiones    != (extrema ? IV : XI)
                    || fig.profunditas      != IV
                    || fig.profunditas_relu != II
                    || fig.latitudo != (extrema ? II * n - III : II * n
                        + II)
                    || rete->introitus      != II * n - IV
                    || rete->exitus_numerus != II * n - IV)
                {
                    recte = FALSUM;
                }
            }
        }
        CREDO_VERUM (recte);
    }
    /* verbum vacuum: nullae instructiones, exitus = introitus */
    {
        Plexus  p = _p(IV, "", piscina);
        Nervus* rete;
        Magnus  x[IV];
        Magnus  y[IV];

        CREDO_VERUM (plexus_nervus(p, piscina, &rete));
        CREDO_AEQUALIS_I32 (rete->numerus, ZEPHYRUM);
        per (k = ZEPHYRUM; k < IV; k++)
        {
            x[k] = magnus_ex_s64((s64)k - II);
        }
        CREDO_VERUM (nervus_aestimare(rete, x, y, NIHIL, piscina));
        CREDO_VERUM (magnus_aequalis(y[ZEPHYRUM], x[ZEPHYRUM])
            && magnus_aequalis(y[III], x[III]));
    }
    /* compositio: nervus(p) o nervus(q) = nervus(iuncta) */
    {
        b32 recte = VERUM;

        per (t = ZEPHYRUM; t < CC; t++)
        {
            PiscinaNotatio nota  = piscina_notare(piscina);
                       i32 n     = III + sors_intra(&s, IV);
                    Plexus p = _fortuitus(&s, n, sors_intra(&s, X),
                        piscina);
                   Plexus q = _fortuitus(&s, n, sors_intra(&s, X),
                       piscina);
                   Plexus  iuncta;
                   Nervus* rete_p;
                   Nervus* rete_q;
                   Nervus* rete_iunctum;
                   Nervus* compositum;
                   Magnus  x[X];
                   Magnus  y1[X];
                   Magnus  y2[X];

            per (k = ZEPHYRUM; k < II * n - IV; k++)
            {
                x[k] = magnus_ex_s64((s64)sors_inter(&s, -C, C));
            }
            si (   !plexus_iungere(p, q, piscina, &iuncta)
                || !plexus_nervus(p, piscina, &rete_p)
                || !plexus_nervus(q, piscina, &rete_q)
                || !plexus_nervus(iuncta, piscina, &rete_iunctum)
                || !nervus_componere(rete_p, rete_q, piscina,
                &compositum)
                || !nervus_aestimare(compositum, x, y1, NIHIL, piscina)
                || !nervus_aestimare(rete_iunctum, x, y2, NIHIL,
                piscina))
            {
                recte = FALSUM;
            }
            alioquin
            {
                per (k = ZEPHYRUM; k < II * n - IV; k++)
                {
                    si (!magnus_aequalis(y1[k], y2[k]))
                    {
                        recte = FALSUM;
                    }
                }
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (recte);
    }
    /* (s1 s2^-1)^20 in E_1 (n = 3) = (F_39; -F_38), L = 2 F_41
     * (D128 linea "(F_39; 2 F_41)" L bis imprimebat: mstr alveo
     * statico) */
    {
        Plexus  unum  = _p(III, "1 -2", piscina);
        Plexus  w     = _p(III, "", piscina);
        Nervus* rete;
          Ansa  e1;
        Magnus  x[II];
        Magnus  y[II];

        per (k = ZEPHYRUM; k < XX; k++)
        {
            (vacuum)plexus_iungere(w, unum, piscina, &w);
        }
        (vacuum)ansa_circa(III, I, piscina, &e1);
        x[ZEPHYRUM]  = e1.a[ZEPHYRUM];
        x[I]         = e1.b[ZEPHYRUM];
        CREDO_VERUM (plexus_nervus(w, piscina, &rete));
        CREDO_VERUM (nervus_aestimare(rete, x, y, NIHIL, piscina));
        CREDO_VERUM (_s64(y[ZEPHYRUM]) == (s64)LXIII * M * M
            + (s64)CCXLV * M + CMLXXXVI);
        CREDO_VERUM (_s64(y[I]) == -((s64)XXXIX * M * M
            + (s64)LXXXVIII * M + CLXIX));
        {
            Ansa fin;

            (vacuum)ansa_ex_coordinatis(III, &y[ZEPHYRUM], &y[I],
                piscina,
                &fin);
            CREDO_VERUM (_s64(ansa_sectiones(fin, piscina))
                == (s64)CCCXXXI * M * M + (s64)CLX * M + CCLXXXII);
        }
    }
    /* refutationes */
    {
        Plexus  duo    = _p(II, "1", piscina);
        Plexus  malus  = _p(IV, "1 2", piscina);
        Nervus* rete   = NIHIL;

        CREDO_FALSUM (plexus_nervus(duo, piscina, &rete));
        CREDO_FALSUM (plexus_nervus(malus, NIHIL, &rete));
        CREDO_FALSUM (plexus_nervus(malus, piscina, NIHIL));
        malus.litterae[ZEPHYRUM] = IV;
        CREDO_FALSUM (plexus_nervus(malus, piscina, &rete));
        CREDO_NIHIL (rete);
    }
}

interior CredoSectio SECTIONES[] = {
    { "verba", _probare_verba, NIHIL, NIHIL, NIHIL, NIHIL },
    { "permutatio", _probare_permutationem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "imagines manu", _probare_imagines_manu, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "relationes", _probare_relationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "actio contra substitutionem", _probare_contra_substitutionem,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "invariantia fortuita", _probare_invariantia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "aequalitas fortuita", _probare_aequalitatem_fortuitam, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "limes", _probare_limitem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "contractio", _probare_contractionem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "piscina refecta", _probare_piscinam, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ansae forma", _probare_ansas_formam, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ansae contra fontem",
      _probare_ansas_contra_fontem, NIHIL, NIHIL, NIHIL,
          NIHIL },
    { "ansae relationes", _probare_ansas_relationes, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "ansae homogeneae", _probare_ansas_homogeneas, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "partitio contra Artin", _probare_partitionem, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "torsio in disco", _probare_torsionem_in_disco, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "incrementum", _probare_incrementum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ansae magnae", _probare_ansas_magnas, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "aequalitas per ansas", _probare_aequalitatem_ansis, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "in maiore", _probare_in_maiore, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recensio aequalitatis", _probare_censurae_aequalitatis, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "recensio limitis", _probare_censurae_limitis, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "recensio finium", _probare_censurae_finium, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "recensio contractionis", _probare_censurae_contractionis,
        NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "recensio actionum", _probare_censurae_actionum, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "burau generatores", _probare_burau_generatores, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "burau relationes", _probare_burau_relationes, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "burau fidelitas", _probare_burau_fidelitatem, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "burau minus uno", _probare_burau_minus_uno, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "alexander clausurarum", _probare_alexander, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "radix burau", _probare_radicem_burau, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "radix contra ansas", _probare_radicem_contra_ansas, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "burau longum", _probare_burau_longum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "incrementum functionis", _probare_incrementum_functionis, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "limes contra incrementum", _probare_limitem_contra_incrementum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "censura ansarum memoria", _probare_censurae_ansarum_memoriam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "censura apicis", _probare_censurae_apicem, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "censura finium", _probare_censurae_fines, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "censura burau piscinae", _probare_censurae_burau_piscinam, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "burau circulus", _probare_burau_circulum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "nervus", _probare_nervum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plexus", SECTIONES);
}
