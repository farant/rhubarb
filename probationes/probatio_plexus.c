/* probatio_plexus.c - probationes plexus (project-specs/plexus-plan.md)
 *
 * T1: verba, permutatio, actio in F_n (Artin), aequalitas. Oracula a
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
#include "plexus.h"
#include <stdio.h>
#include <string.h>

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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plexus", SECTIONES);
}
