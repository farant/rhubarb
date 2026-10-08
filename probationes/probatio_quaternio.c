/* probatio_quaternio.c - Probationes quaternionum super anulos
 *
 * Oracula a codice independentia: (1) tabula Hamilton (i^2 = j^2 = k^2
 * = ijk = -1, ij = k = -ji, ...); (2) identitates super elementa
 * fortuita in Z: N(pq) = N(p) N(q), (pq)r = p(qr), conj(pq) = conj(q)
 * conj(p), p conj(p) = N; (3) matrix M = N R: M M^T = N^2 I, det M =
 * N^3, rotare(q, v) = M v; (4) ordines grupporum notorum per clausuram
 * EXACTAM: tetraedricus binarius (unitates Hurwitz) XXIV super Q,
 * icosaedricus binarius (icosiani) CXX super Q(sqrt 5); (5) geometria
 * manu: eadem rotatio/axis, ordo angulorum 0 < 90 < 180, directio
 * proxima cum et sine antipodibus; refutationes sine ordine.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "sors.h"
#include "magnus.h"
#include "fractio.h"
#include "polynomium.h"
#include "anulus.h"
#include "matrix.h"
#include "extensio.h"
#include "quaternio.h"
#include <stdio.h>

hic_manens Piscina* piscina;

interior Quaternio
_q (
       constans Anulus* anulus,
    constans character* textus)
{
    Quaternio q;

    q.anulus = NIHIL;
    si (!quaternio_ex_chorda(anulus, chorda_ex_literis(textus, piscina),
        piscina, &q))
    {
        imprimere("FRACTA: quaternio malus: %s\n", textus);
    }
    redde q;
}

interior b32
_aequalis_textui (
             Quaternio  q,
    constans character* textus)
{
    redde chorda_aequalis_literis(quaternio_ad_chordam(q, piscina),
        textus);
}

interior Quaternio
_productum (
    Quaternio p,
    Quaternio q)
{
    Quaternio r;

    r.anulus = NIHIL;
    (vacuum)quaternio_multiplica(p, q, piscina, &r);
    redde r;
}

interior Quaternio
_fortuitus (
    Sors* s)
{
       Magnus partes[IV];
          i32 k;
    Quaternio q;

    per (k = ZEPHYRUM; k < IV; k++)
    {
        partes[k] = magnus_ex_s64((s64)sors_inter(s, -IX, IX));
    }
    q.anulus = NIHIL;
    (vacuum)quaternio_ex_partibus(&ANULUS_INTEGRORUM, &partes[ZEPHYRUM],
        &partes[I], &partes[II], &partes[III], piscina, &q);
    redde q;
}

/* ordo clausurae generatorum (deduplicatio exacta); 0 si > limes */
interior i32
_ordo_clausurae (
    constans Quaternio* generatores,
                   i32  numerus,
                   i32  limes)
{
    Quaternio* elementa = (Quaternio*)piscina_allocare(piscina,
        (memoriae_index)limes * magnitudo(Quaternio));
    i32 n       = ZEPHYRUM;
    i32 cursor  = ZEPHYRUM;
    i32 g;

    (vacuum)quaternio_unum(generatores[ZEPHYRUM].anulus, piscina,
        &elementa[n++]);
    dum (cursor < n)
    {
        per (g = ZEPHYRUM; g < numerus; g++)
        {
            Quaternio x = _productum(elementa[cursor], generatores[g]);
                  i32 j;
                  b32 novum = VERUM;

            per (j = ZEPHYRUM; j < n; j++)
            {
                si (quaternio_aequalis(elementa[j], x))
                {
                    novum = FALSUM;
                    frange;
                }
            }
            si (novum)
            {
                si (n >= limes)
                {
                    redde ZEPHYRUM;
                }
                elementa[n++] = x;
            }
        }
        cursor++;
    }
    redde n;
}

s32 principale (vacuum)
{
    constans Anulus* z = &ANULUS_INTEGRORUM;
    constans Anulus* q = &ANULUS_RATIONALIUM;

    piscina = piscina_generare_dynamicum("probatio_quaternio", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * TABULA HAMILTON
     * ================================================== */

    {
        Quaternio i = _q(z, "[0, 1, 0, 0]");
        Quaternio j = _q(z, "[0, 0, 1, 0]");
        Quaternio k = _q(z, "[0, 0, 0, 1]");

        imprimere("\n--- Tabula Hamilton ---\n");
        CREDO_VERUM (_aequalis_textui(_productum(i, i),
            "[-1, 0, 0, 0]"));
        CREDO_VERUM (_aequalis_textui(_productum(j, j),
            "[-1, 0, 0, 0]"));
        CREDO_VERUM (_aequalis_textui(_productum(k, k),
            "[-1, 0, 0, 0]"));
        CREDO_VERUM (_aequalis_textui(_productum(_productum(i, j), k),
            "[-1, 0, 0, 0]"));
        CREDO_VERUM (_aequalis_textui(_productum(i, j),
            "[0, 0, 0, 1]"));
        CREDO_VERUM (_aequalis_textui(_productum(j, i),
            "[0, 0, 0, -1]"));
        CREDO_VERUM (_aequalis_textui(_productum(j, k),
            "[0, 1, 0, 0]"));
        CREDO_VERUM (_aequalis_textui(_productum(k, j),
            "[0, -1, 0, 0]"));
        CREDO_VERUM (_aequalis_textui(_productum(k, i),
            "[0, 0, 1, 0]"));
        CREDO_VERUM (_aequalis_textui(_productum(i, k),
            "[0, 0, -1, 0]"));
        {
            Quaternio x;

            CREDO_FALSUM (quaternio_ex_chorda(q, chorda_ex_literis(
                " [ 1/2 ,a, -3,0 ] ", piscina), piscina, &x));
        }
        CREDO_VERUM (_aequalis_textui(_q(q, " [ 1/2 , -3 ,0, 7/4 ] "),
            "[1/2, -3, 0, 7/4]"));
        CREDO_VERUM (_aequalis_textui(_q(q, "[1/2, -3, 0, 7/4]"),
            "[1/2, -3, 0, 7/4]"));
    }


    /* ==================================================
     * IDENTITATES (elementa fortuita in Z)
     * ================================================== */

    {
        Sors s;
         b32 norma_bene       = VERUM;
         b32 triplex_bene     = VERUM;
         b32 conjugatum_bene  = VERUM;
         b32 matrix_bene      = VERUM;
         b32 rotatio_bene     = VERUM;
         i32 iteratio;

        imprimere("\n--- Identitates in Z ---\n");
        sors_seminare(&s, CCCXXXIII, I);
        per (iteratio = ZEPHYRUM; iteratio < C; iteratio++)
        {
            PiscinaNotatio nota  = piscina_notare(piscina);
                 Quaternio a     = _fortuitus(&s);
                 Quaternio b     = _fortuitus(&s);
                 Quaternio c     = _fortuitus(&s);
                 Quaternio ca;
                 Quaternio cb;
                 Quaternio c_ab;
                    Magnus na;
                    Magnus nb;
                    Magnus n_ab;

            /* N(ab) = N(a) N(b) */
            (vacuum)quaternio_norma(a, piscina, &na);
            (vacuum)quaternio_norma(b, piscina, &nb);
            (vacuum)quaternio_norma(_productum(a, b), piscina, &n_ab);
            si (!magnus_aequalis(n_ab, magnus_multiplica(na, nb,
                piscina)))
            {
                norma_bene = FALSUM;
            }
            /* (ab)c = a(bc) */
            si (!quaternio_aequalis(_productum(_productum(a, b), c),
                _productum(a, _productum(b, c))))
            {
                triplex_bene = FALSUM;
            }
            /* conj(ab) = conj(b) conj(a); a conj(a) = N(a) */
            (vacuum)quaternio_conjugatum(a, piscina, &ca);
            (vacuum)quaternio_conjugatum(b, piscina, &cb);
            (vacuum)quaternio_conjugatum(_productum(a, b), piscina,
                &c_ab);
            si (!quaternio_aequalis(c_ab, _productum(cb, ca)))
            {
                conjugatum_bene = FALSUM;
            }
            {
                   Magnus nullum = magnus_ex_s64(ZEPHYRUM);
                Quaternio n;

                (vacuum)quaternio_ex_partibus(z, &na, &nullum, &nullum,
                    &nullum, piscina, &n);
                si (!quaternio_aequalis(_productum(a, ca), n))
                {
                    conjugatum_bene = FALSUM;
                }
            }
            /* M = N R: M M^T = N^2 I, det M = N^3, rotare = M v */
            si (!quaternio_est_nullum(a))
            {
                   Matrix m;
                   Matrix mt;
                   Matrix m_mt;
                   Matrix exspectata;
                   Magnus det  = magnus_ex_s64(ZEPHYRUM);
                   Magnus n2   = magnus_multiplica(na, na, piscina);
                Quaternio rotatum;
                      i32 r;
                      i32 col;

                (vacuum)quaternio_matrix(a, piscina, &m);
                (vacuum)matrix_transposita(m, piscina, &mt);
                (vacuum)matrix_multiplica(m, mt, piscina, &m_mt);
                (vacuum)matrix_nulla(z, III, III, piscina, &exspectata);
                per (r = ZEPHYRUM; r < III; r++)
                {
                    matrix_pone(&exspectata, r, r, &n2);
                }
                si (   !matrix_aequalis(m_mt, exspectata)
                    || !matrix_determinans(m, piscina, &det)
                    || !magnus_aequalis(det, magnus_multiplica(n2, na,
                    piscina)))
                {
                    matrix_bene = FALSUM;
                }
                (vacuum)quaternio_rotare(a, b, piscina, &rotatum);
                per (r = ZEPHYRUM; r < III; r++)
                {
                    Magnus summa = magnus_ex_s64(ZEPHYRUM);

                    per (col = ZEPHYRUM; col < III; col++)
                    {
                        summa = magnus_adde(summa, magnus_multiplica(
                            *(constans Magnus*)matrix_elementum(m, r,
                            col),
                            *(constans Magnus*)quaternio_pars(b, col
                                + I),
                            piscina), piscina);
                    }
                    si (!magnus_aequalis(summa, *(constans Magnus*)
                        quaternio_pars(rotatum, r + I)))
                    {
                        rotatio_bene = FALSUM;
                    }
                }
                /* pars realis: N(a) b_0 */
                si (!magnus_aequalis(*(constans Magnus*)quaternio_pars(
                    rotatum, ZEPHYRUM), magnus_multiplica(na, *(constans
                    Magnus*)quaternio_pars(b, ZEPHYRUM), piscina)))
                {
                    rotatio_bene = FALSUM;
                }
            }
            piscina_reficere(piscina, nota);
        }
        CREDO_VERUM (norma_bene);
        CREDO_VERUM (triplex_bene);
        CREDO_VERUM (conjugatum_bene);
        CREDO_VERUM (matrix_bene);
        CREDO_VERUM (rotatio_bene);
    }


    /* ==================================================
     * INVERSUM SUPER Q; REFUTATIONES
     * ================================================== */

    {
        Quaternio a = _q(q, "[1/2, -3, 2/7, 5]");
        Quaternio inv;
        Quaternio unum;
        Quaternio x;

        imprimere("\n--- Inversum, refutationes ---\n");
        (vacuum)quaternio_unum(q, piscina, &unum);
        CREDO_VERUM (quaternio_inversum(a, piscina, &inv)
            && quaternio_aequalis(_productum(a, inv), unum)
            && quaternio_aequalis(_productum(inv, a), unum));
        CREDO_FALSUM (quaternio_inversum(_q(q, "[0, 0, 0, 0]"), piscina,
            &x));
        /* Z non corpus */
        CREDO_FALSUM (quaternio_inversum(_q(z, "[1, 1, 0, 0]"), piscina,
            &x));
        /* anuli mixti */
        CREDO_FALSUM (quaternio_multiplica(_q(z, "[1, 0, 0, 0]"), a,
            piscina, &x));
        CREDO_FALSUM (quaternio_aequalis(_q(z, "[1, 0, 0, 0]"), _q(q,
            "[1, 0, 0, 0]")));
        CREDO_FALSUM (quaternio_ex_chorda(z, chorda_ex_literis(
            "[1, 2, 3]", piscina), piscina, &x));
        CREDO_FALSUM (quaternio_ex_chorda(z, chorda_ex_literis(
            "[1, 2, 3, 4, 5]", piscina), piscina, &x));
        CREDO_FALSUM (quaternio_ex_chorda(z, chorda_ex_literis(
            "1, 2, 3, 4", piscina), piscina, &x));
        CREDO_FALSUM (quaternio_ex_chorda(z, chorda_ex_literis(
            "[1/2, 0, 0, 0]", piscina), piscina, &x));
    }


    /* ==================================================
     * GRUPPI FINITI PER CLAUSURAM EXACTAM
     * ================================================== */

    {
              Quaternio  generatores[II];
               Extensio* k5 = extensio_quadratica(V, piscina);
        constans Anulus* q5 = extensio_anulus(k5);

        imprimere("\n--- Gruppi finiti ---\n");
        /* tetraedricus binarius: i et (1 + i + j + k)/2 (Hurwitz) */
        generatores[ZEPHYRUM]  = _q(q, "[0, 1, 0, 0]");
        generatores[I]         = _q(q, "[1/2, 1/2, 1/2, 1/2]");
        CREDO_AEQUALIS_I32 (_ordo_clausurae(generatores, II, C), XXIV);
        /* quaternio sine ordine finito: (1 + 2i) generat infinitum */
        generatores[ZEPHYRUM] = _q(q, "[3/5, 4/5, 0, 0]");
        CREDO_AEQUALIS_I32 (_ordo_clausurae(generatores, I, C),
            ZEPHYRUM);
        /* icosaedricus binarius: i et (phi + phi^-1 i + j)/2, phi =
         * (1 + sqrt 5)/2, phi^-1 = (sqrt 5 - 1)/2 */
        generatores[ZEPHYRUM] = _q(q5, "[0, 1, 0, 0]");
        generatores[I] = _q(q5, "[(a + 1)/4, (a - 1)/4, 1/2, 0]");
        CREDO_AEQUALIS_I32 (_ordo_clausurae(generatores, II, CC), CXX);
    }


    /* ==================================================
     * GEOMETRIA
     * ================================================== */

    {
        Quaternio i = _q(z, "[0, 1, 0, 0]");
        Quaternio j = _q(z, "[0, 0, 1, 0]");
        Quaternio k = _q(z, "[0, 0, 0, 1]");
        Quaternio directiones[III];
              i32 index  = VII;
              s32 s      = VII;

        imprimere("\n--- Geometria ---\n");
        /* eadem rotatio: q, -q, 2q */
        CREDO_VERUM (quaternio_eadem_rotatio(_q(z, "[1, 2, 3, 4]"),
            _q(z,
            "[-2, -4, -6, -8]"), piscina));
        CREDO_FALSUM (quaternio_eadem_rotatio(_q(z, "[1, 2, 3, 4]"),
            _q(z,
            "[1, 2, 3, 5]"), piscina));
        CREDO_FALSUM (quaternio_eadem_rotatio(_q(z, "[0, 0, 0, 0]"), i,
            piscina));
        CREDO_FALSUM (quaternio_eadem_rotatio(i, _q(z, "[0, 0, 0, 0]"),
            piscina));
        /* eadem axis: i et 3 - 5i; non i et j; non partes reales
         * solae */
        CREDO_VERUM (quaternio_eadem_axis(i, _q(z, "[3, -5, 0, 0]"),
            piscina));
        CREDO_VERUM (quaternio_eadem_axis(_q(z, "[1, 1, 2, 3]"), _q(z,
            "[7, -2, -4, -6]"), piscina));
        CREDO_FALSUM (quaternio_eadem_axis(i, j, piscina));
        /* i, k: solum minor (b, d) non nullus */
        CREDO_FALSUM (quaternio_eadem_axis(i, k, piscina));
        CREDO_FALSUM (quaternio_eadem_axis(j, k, piscina));
        CREDO_FALSUM (quaternio_eadem_axis(_q(z, "[5, 0, 0, 0]"), i,
            piscina));
        /* anguli: 1 (0), 1 + i (90), i (180) */
        CREDO_VERUM (quaternio_compara_angulum(_q(z, "[1, 1, 0, 0]"), i,
            piscina, &s) && s == -I);
        CREDO_VERUM (quaternio_compara_angulum(_q(z, "[1, 0, 0, 0]"),
            _q(z,
            "[1, 1, 0, 0]"), piscina, &s) && s == -I);
        CREDO_VERUM (quaternio_compara_angulum(_q(z, "[1, 1, 0, 0]"),
            _q(z,
            "[-3, 0, 3, 0]"), piscina, &s) && s == ZEPHYRUM);
        CREDO_VERUM (quaternio_compara_angulum(j, _q(z, "[1, 0, 0, 1]"),
            piscina, &s) && s == I);
        /* directio proxima */
        directiones[ZEPHYRUM]  = i;
        directiones[I]         = j;
        directiones[II]        = k;
        CREDO_VERUM (quaternio_proximus(_q(z, "[0, 2, 1, 0]"),
            directiones,
            III, FALSUM, piscina, &index) && index == ZEPHYRUM);
        CREDO_VERUM (quaternio_proximus(_q(z, "[0, -3, 1, 0]"),
            directiones,
            III, VERUM, piscina, &index) && index == ZEPHYRUM);
        CREDO_VERUM (quaternio_proximus(_q(z, "[0, -3, 1, 0]"),
            directiones,
            III, FALSUM, piscina, &index) && index == I);
        CREDO_VERUM (quaternio_proximus(_q(z, "[9, 1, 1, 5]"),
            directiones,
            III, FALSUM, piscina, &index) && index == II);
        /* paritas: v = (0, 1, 1, 0) inter i et j aequaliter - index
         * primus */
        CREDO_VERUM (quaternio_proximus(_q(z, "[0, 1, 1, 0]"),
            directiones,
            II, FALSUM, piscina, &index) && index == ZEPHYRUM);
        /* directiones non normatae: (0, 10, 0, 0) contra (0, 1, 1, 0)
         * pro v = (0, 1, 1, 0) - cos 1/sqrt 2 contra 1 */
        directiones[ZEPHYRUM]  = _q(z, "[0, 10, 0, 0]");
        directiones[I]         = _q(z, "[0, 1, 1, 0]");
        CREDO_VERUM (quaternio_proximus(_q(z, "[0, 1, 1, 0]"),
            directiones,
            II, FALSUM, piscina, &index) && index == I);
        /* refutationes: sine ordine (Z[t]), v nullum, numerus 0 */
        CREDO_FALSUM (quaternio_proximus(_q(z, "[1, 0, 0, 0]"),
            directiones, II, FALSUM, piscina, &index));
        CREDO_FALSUM (quaternio_proximus(i, directiones, ZEPHYRUM,
            FALSUM,
            piscina, &index));
        {
            Quaternio p_i = _q(&ANULUS_POLYNOMIORUM, "[0, t, 0, 0]");
            Quaternio p_j = _q(&ANULUS_POLYNOMIORUM, "[0, 0, 1, 0]");

            directiones[ZEPHYRUM] = p_i;
            CREDO_FALSUM (quaternio_proximus(p_j, directiones, I,
                FALSUM,
                piscina, &index));
            CREDO_FALSUM (quaternio_compara_angulum(p_i, p_j, piscina,
                &s));
            /* eadem axis sine ordine licet */
            CREDO_VERUM (quaternio_eadem_axis(p_i,
                _q(&ANULUS_POLYNOMIORUM,
                "[1, t^2, 0, 0]"), piscina));
        }
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
