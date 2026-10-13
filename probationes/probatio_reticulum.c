/* probatio_reticulum.c - probationes reticulorum (project-specs/
 * reticulum-plan.md)
 *
 * T1: Gram-Schmidt integra contra Gram-Schmidt LIBRORUM (fractiones,
 * b*_i = b_i - sum mu_ij b*_j) in hac probatione rescriptam - alia via,
 * eadem mathesis; productum normarum contra determinantem Gram
 * (Bareiss, matrix.h); iudicium reductionis contra iudicium rationale
 * rescriptum. Casus manu (et Python Fractions) computati: exemplum
 * LLL vulgare [1,1,1; -1,0,2; 3,5,6] et basis eius reducta (mu = 1/2
 * exacte), aequalitas Lovasz ad delta = 1/2. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "magnus.h"
#include "fractio.h"
#include "matrix.h"
#include "reticulum.h"
#include <stdio.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior Matrix
_matrix (
       constans Anulus* anulus,
    constans character* textus,
               Piscina* piscina)
{
    Matrix m;

    si (!matrix_ex_chorda(anulus, chorda_ex_literis(textus, piscina),
        piscina, &m))
    {
        imprimere("FRACTA: matrix mala: %s\n", textus);
        m.anulus    = NIHIL;
        m.lineae    = ZEPHYRUM;
        m.columnae  = ZEPHYRUM;
        m.elementa  = NIHIL;
    }
    redde m;
}

interior Matrix
_z (
    constans character* textus,
               Piscina* piscina)
{
    redde _matrix(&ANULUS_INTEGRORUM, textus, piscina);
}

interior Fractio
_f (
    constans character* textus,
               Piscina* piscina)
{
    Fractio f = fractio_ex_s64(ZEPHYRUM);

    (vacuum)fractio_ex_chorda(chorda_ex_literis(textus, piscina),
        piscina,
        &f);
    redde f;
}

interior b32
_fractio_est (
               Fractio  a,
    constans character* textus,
               Piscina* piscina)
{
    redde fractio_aequalis(a, _f(textus, piscina));
}

interior constans Fractio*
_mu (
    Matrix m,
       i32 i,
       i32 j)
{
    redde (constans Fractio*)matrix_elementum(m, i, j);
}

/* generator lineare congruens proprius (determinismus); status per
 * sectionem, numquam inter sectiones */
interior s64
_alea (
    i64* semen,
    s64  modulus)
{
    *semen = (*semen * (i64)1103515245 + (i64)12345) & (i64)0x7fffffff;
    redde (s64)(*semen % (i64)modulus);
}

/* matrix integra lineae x columnae, elementa in [-limes, limes]; si
 * magna, quaeque multiplicatur per 10^XXV (via magnorum) */
interior Matrix
_basis_fortuita (
        i64* semen,
        i32  lineae,
        i32  columnae,
        s64  limes,
        b32  magna,
    Piscina* piscina)
{
    Matrix m;
    Magnus x;
    Magnus scala = magnus_potentia(magnus_ex_s64(X), XXV, piscina);
       i32 i;
       i32 j;

    (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, lineae, columnae, piscina,
        &m);
    per (i = ZEPHYRUM; i < lineae; i++)
    {
        per (j = ZEPHYRUM; j < columnae; j++)
        {
            x = magnus_ex_s64(_alea(semen, II * limes + I) - limes);
            si (magna)
            {
                x = magnus_adde(magnus_multiplica(x, scala, piscina),
                    magnus_ex_s64(_alea(semen, VII)), piscina);
            }
            matrix_pone(&m, i, j, &x);
        }
    }
    redde m;
}

/* diagonalis magna et crescens (30 + 10 i + 0..4) super elementa
 * parva: basis prope orthogonalis, Lovasz plerumque tenet - ergo
 * reducta etiam in dimensionibus altis */
interior vacuum
_diagonalem_auge (
     Matrix  b,
        i64* semen,
    Piscina* piscina)
{
    Magnus x;
       i32 i;

    per (i = ZEPHYRUM; i < matrix_lineae(b); i++)
    {
        x = magnus_adde(*(constans Magnus*)matrix_elementum(b, i, i),
            magnus_ex_s64(XXX + X * (s64)i + _alea(semen, V)), piscina);
        matrix_pone(&b, i, i, &x);
    }
}


/* ==================================================
 * Gram-Schmidt librorum (oraculum rescriptum)
 * ================================================== */

/* mu[i * n + j], normae[i]; FALSUM si vectores dependentes */
interior b32
_gs_librorum (
     Matrix  b,
    Piscina* piscina,
    Fractio* mu,
    Fractio* normae)
{
        i32  n = matrix_lineae(b);
        i32  m = matrix_columnae(b);
    Fractio* stellae;
    Fractio  summa;
    Fractio  x;
        i32  i;
        i32  j;
        i32  k;

    stellae = (Fractio*)piscina_allocare(piscina,
        (memoriae_index)(n * m
            + I) * (memoriae_index)magnitudo(Fractio));
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (k = ZEPHYRUM; k < m; k++)
        {
            stellae[i * m + k] = fractio_ex_magno(
                *(constans Magnus*)matrix_elementum(b, i, k));
        }
        per (j = ZEPHYRUM; j < i; j++)
        {
            summa = fractio_ex_s64(ZEPHYRUM);
            per (k = ZEPHYRUM; k < m; k++)
            {
                summa = fractio_adde(summa, fractio_multiplica(
                    fractio_ex_magno(
                        *(constans Magnus*)matrix_elementum(b, i, k)),
                    stellae[j * m + k], piscina), piscina);
            }
            si (!fractio_divide(summa, normae[j], piscina, &x))
            {
                redde FALSUM;
            }
            mu[i * n + j] = x;
            per (k = ZEPHYRUM; k < m; k++)
            {
                stellae[i * m + k] = fractio_subtrahe(stellae[i * m
                    + k],
                    fractio_multiplica(x, stellae[j * m + k], piscina),
                    piscina);
            }
        }
        mu[i * n + i]  = fractio_ex_s64(I);
        summa          = fractio_ex_s64(ZEPHYRUM);
        per (k = ZEPHYRUM; k < m; k++)
        {
            summa = fractio_adde(summa, fractio_multiplica(
                stellae[i * m + k], stellae[i * m + k], piscina),
                piscina);
        }
        si (fractio_signum(summa) == ZEPHYRUM)
        {
            redde FALSUM;
        }
        normae[i] = summa;
    }
    redde VERUM;
}

/* iudicium librorum: |mu_ij| <= 1/2, Lovasz in fractionibus */
interior b32
_reductum_librorum (
     Matrix  b,
    Fractio  delta,
    Piscina* piscina)
{
        i32  n = matrix_lineae(b);
    Fractio* mu;
    Fractio* normae;
    Fractio  dimidium;
    Fractio  dextra;
        i32  i;
        i32  j;

    mu     = (Fractio*)piscina_allocare(piscina,
        (memoriae_index)(n * n
            + I) * (memoriae_index)magnitudo(Fractio));
    normae = (Fractio*)piscina_allocare(piscina,
        (memoriae_index)(n + I) * (memoriae_index)magnitudo(Fractio));
    (vacuum)fractio_ex_s64_s64(I, II, piscina, &dimidium);
    si (!_gs_librorum(b, piscina, mu, normae))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (fractio_compara(fractio_absolutum(mu[i * n + j],
                piscina),
                dimidium, piscina) > ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }
    per (i = I; i < n; i++)
    {
        dextra = fractio_multiplica(fractio_subtrahe(delta,
            fractio_multiplica(mu[i * n + i - I], mu[i * n + i - I],
                piscina), piscina), normae[i - I], piscina);
        si (fractio_compara(normae[i], dextra, piscina) < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* forma = b b^T */
interior Matrix
_forma (
     Matrix  b,
    Piscina* piscina)
{
    Matrix t;
    Matrix g;

    (vacuum)matrix_transposita(b, piscina, &t);
    (vacuum)matrix_multiplica(b, t, piscina, &g);
    redde g;
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior vacuum
_probare_manu (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Matrix  b;
     Matrix  mu;
    Fractio* normae;

    /* exemplum LLL vulgare */
    b = _z("[1, 1, 1; -1, 0, 2; 3, 5, 6]", piscina);
    CREDO_NECESSE (reticulum_orthogonaliza(b, piscina, &mu, &normae));
    CREDO_AEQUALIS_I32 (matrix_lineae(mu), III);
    CREDO_VERUM (_fractio_est(*_mu(mu, I, ZEPHYRUM), "1/3", piscina));
    CREDO_VERUM (_fractio_est(*_mu(mu, II, ZEPHYRUM), "14/3", piscina));
    CREDO_VERUM (_fractio_est(*_mu(mu, II, I), "13/14", piscina));
    CREDO_VERUM (_fractio_est(*_mu(mu, I, I), "1", piscina));
    CREDO_VERUM (_fractio_est(*_mu(mu, ZEPHYRUM, I), "0", piscina));
    CREDO_VERUM (_fractio_est(normae[ZEPHYRUM], "3", piscina));
    CREDO_VERUM (_fractio_est(normae[I], "14/3", piscina));
    CREDO_VERUM (_fractio_est(normae[II], "9/14", piscina));
    /* basis eius reducta: mu_32 = 1/2 exacte */
    b = _z("[0, 1, 0; 1, 0, 1; -1, 0, 2]", piscina);
    CREDO_NECESSE (reticulum_orthogonaliza(b, piscina, &mu, &normae));
    CREDO_VERUM (_fractio_est(*_mu(mu, II, I), "1/2", piscina));
    CREDO_VERUM (_fractio_est(*_mu(mu, I, ZEPHYRUM), "0", piscina));
    CREDO_VERUM (_fractio_est(normae[II], "9/2", piscina));
    /* exitus NIHIL tolerantur */
    CREDO_VERUM (reticulum_orthogonaliza(b, piscina, NIHIL, NIHIL));
    /* basis vacua */
    b = _z("[]", piscina);
    CREDO_NECESSE (reticulum_orthogonaliza(b, piscina, &mu, &normae));
    CREDO_AEQUALIS_I32 (matrix_lineae(mu), ZEPHYRUM);
    CREDO_NIHIL (normae);
}

interior vacuum
_probare_iudicium_manu (
    CredoContextus* c)
{
    Piscina* piscina       = c->piscina;
    Fractio  tres_quartae  = _f("3/4", piscina);
    Fractio  dimidium      = _f("1/2", piscina);
    Fractio  unum          = _f("1", piscina);
     Matrix  b;
        b32  r = FALSUM;

    b = _z("[1, 1, 1; -1, 0, 2; 3, 5, 6]", piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, NIHIL, piscina, &r));
    CREDO_FALSUM (r);
    /* mu = 1/2 exacte: magnitudine reducta (limes inclusus) */
    b = _z("[0, 1, 0; 1, 0, 1; -1, 0, 2]", piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, NIHIL, piscina, &r));
    CREDO_VERUM (r);
    CREDO_NECESSE (reticulum_est_reductum(b, &unum, piscina, &r));
    CREDO_VERUM (r);
    /* mu = 3/2: non magnitudine reducta */
    b = _z("[1, 1; 1, 2]", piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, &tres_quartae, piscina,
        &r));
    CREDO_FALSUM (r);
    /* Lovasz: 9 contra (delta - 1/4) 16 - transit 3/4, cadit 99/100 */
    b = _z("[4, 0; 2, 3]", piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, &tres_quartae, piscina,
        &r));
    CREDO_VERUM (r);
    CREDO_NECESSE (reticulum_est_reductum(b, NIHIL, piscina, &r));
    CREDO_FALSUM (r);
    /* Lovasz AEQUALITAS ad delta = 1/2: 1/2 = (1/2 - 1/4) 2 */
    b = _z("[1, 1; 0, 1]", piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, &dimidium, piscina, &r));
    CREDO_VERUM (r);
    CREDO_NECESSE (reticulum_est_reductum(b, &tres_quartae, piscina,
        &r));
    CREDO_FALSUM (r);
    /* Lovasz cum mu_k,k-1, non mu_k,0: mu_20 = 0, mu_21 = 1/2, 29 >=
     * (99/100 - 1/4) 36 = 26.64 (sed 29 < 99/100 * 36) */
    b = _z("[1, 0, 0, 0; 0, 6, 0, 0; 0, 3, 5, 2]", piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, NIHIL, piscina, &r));
    CREDO_VERUM (r);
    /* orthogonalis: reducta pro omni delta */
    b = _z("[0, 1; 1, 0]", piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, &unum, piscina, &r));
    CREDO_VERUM (r);
    /* basis vacua et vector unus */
    CREDO_NECESSE (reticulum_est_reductum(_z("[]", piscina), NIHIL,
        piscina, &r));
    CREDO_VERUM (r);
    CREDO_NECESSE (reticulum_est_reductum(_z("[7, -3]", piscina), NIHIL,
        piscina, &r));
    CREDO_VERUM (r);
}

interior vacuum
_probare_refutationes (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
    Fractio  quarta   = _f("1/4", piscina);
    Fractio  supra    = _f("101/100", piscina);
    Fractio  unum     = _f("1", piscina);
     Matrix  mu;
    Fractio* normae  = NIHIL;
        b32  r       = VERUM;

    /* lineae dependentes, vector nullus */
    CREDO_FALSUM (reticulum_orthogonaliza(_z("[1, 2; 2, 4]", piscina),
        piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_orthogonaliza(_z("[1, 2, 3; 0, 0, 0]",
        piscina), piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_orthogonaliza(_z("[1, 0; 0, 1; 1, 1]",
        piscina), piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_est_reductum(_z("[1, 2; 2, 4]", piscina),
        NIHIL, piscina, &r));
    CREDO_VERUM (r);                     /* exitus non tactus */
    /* anulus non Z */
    CREDO_FALSUM (reticulum_orthogonaliza(_matrix(&ANULUS_RATIONALIUM,
        "[1/2, 0; 0, 1]", piscina), piscina, &mu, &normae));
    /* delta extra (1/4, 1] */
    CREDO_FALSUM (reticulum_est_reductum(_z("[1, 0; 0, 1]", piscina),
        &quarta, piscina, &r));
    CREDO_FALSUM (reticulum_est_reductum(_z("[1, 0; 0, 1]", piscina),
        &supra, piscina, &r));
    CREDO_VERUM (reticulum_est_reductum(_z("[1, 0; 0, 1]", piscina),
        &unum, piscina, &r));
    /* formae: non symmetrica, non definita, non quadrata */
    CREDO_FALSUM (reticulum_formam_orthogonaliza(_z("[2, 1; 0, 2]",
        piscina), piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_formam_orthogonaliza(_z("[1, 2; 2, 1]",
        piscina), piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_formam_orthogonaliza(_z("[0, 0; 0, 1]",
        piscina), piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_formam_orthogonaliza(_z("[-1, 0; 0, 1]",
        piscina), piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_formam_orthogonaliza(_z("[1, 0, 0; 0, 1, 0]",
        piscina), piscina, &mu, &normae));
    CREDO_FALSUM (reticulum_formam_est_reducta(_z("[1, 2; 2, 1]",
        piscina), NIHIL, piscina, &r));
}

interior vacuum
_probare_formam (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Matrix  mu;
    Fractio* normae;
        b32  r = FALSUM;

    /* A_2: [2, -1; -1, 2], mu = -1/2, normae 2, 3/2 */
    CREDO_NECESSE (reticulum_formam_orthogonaliza(_z("[2, -1; -1, 2]",
        piscina), piscina, &mu, &normae));
    CREDO_VERUM (_fractio_est(*_mu(mu, I, ZEPHYRUM), "-1/2", piscina));
    CREDO_VERUM (_fractio_est(normae[ZEPHYRUM], "2", piscina));
    CREDO_VERUM (_fractio_est(normae[I], "3/2", piscina));
    /* Lovasz: 3/2 >= (99/100 - 1/4) 2 = 37/25 */
    CREDO_NECESSE (reticulum_formam_est_reducta(_z("[2, -1; -1, 2]",
        piscina), NIHIL, piscina, &r));
    CREDO_VERUM (r);
}

/* forma b b^T eadem reddit quae basis b */
interior vacuum
_probare_formam_contra_basim (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Matrix  b;
     Matrix  mu_b;
     Matrix  mu_f;
    Fractio* nb;
    Fractio* nf;
        b32  rb;
        b32  rf;
        i32  exemplum;
        i32  i;
        i32  j;
        i32  concordant  = ZEPHYRUM;
        i64  semen       = (i64)MMXXVI;

    per (exemplum = ZEPHYRUM; exemplum < XL; exemplum++)
    {
        i32 n = (i32)(I + _alea(&semen, V));

        b = _basis_fortuita(&semen, n, n + (i32)_alea(&semen, III), IX,
            exemplum % IV == III,
            piscina);
        si (!reticulum_orthogonaliza(b, piscina, &mu_b, &nb))
        {
            perge;               /* dependentes: raro */
        }
        CREDO_NECESSE (reticulum_formam_orthogonaliza(_forma(b,
            piscina),
            piscina, &mu_f, &nf));
        CREDO_VERUM (matrix_aequalis(mu_b, mu_f));
        per (i = ZEPHYRUM; i < n; i++)
        {
            CREDO_VERUM (fractio_aequalis(nb[i], nf[i]));
        }
        CREDO_NECESSE (reticulum_est_reductum(b, NIHIL, piscina, &rb));
        CREDO_NECESSE (reticulum_formam_est_reducta(_forma(b, piscina),
            NIHIL, piscina, &rf));
        CREDO_VERUM (rb == rf);
        (vacuum)j;
        concordant++;
    }
    imprimere("    exempla concordantia: %d\n", (int)concordant);
    CREDO_MAIOR_I32 (concordant, XXX);
}

/* contra Gram-Schmidt librorum et determinantem Gram (Bareiss) */
interior vacuum
_probare_contra_libros (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Matrix  b;
     Matrix  mu;
    Fractio* normae;
    Fractio* mu_l;
    Fractio* normae_l;
    Fractio  productum;
     Magnus  det;
    Fractio  delta = _f("3/4", piscina);
        b32  r;
        i32  exemplum;
        i32  i;
        i32  j;
        i32  probati  = ZEPHYRUM;
        i32  reducti  = ZEPHYRUM;
        i32  alti     = ZEPHYRUM;           /* reductae cum n >= III */
        i64  semen    = (i64)MMXXVII;

    per (exemplum = ZEPHYRUM; exemplum < LX; exemplum++)
    {
        i32 n      = (i32)(I + _alea(&semen, VI));
        b32 magna  = (exemplum % V == IV);

        /* parvae entradae ut bases reductae quoque occurrant */
        b = _basis_fortuita(&semen, n, n + (i32)_alea(&semen, II),
            exemplum % II ? II : XX,
            magna, piscina);
        si (exemplum % II == I && !magna)        /* elementa +-2 */
        {
            _diagonalem_auge(b, &semen, piscina);   /* prope orthogonalis */
        }
        mu_l     = (Fractio*)piscina_allocare(piscina,
            (memoriae_index)(n * n) * (memoriae_index)magnitudo(Fractio));
        normae_l = (Fractio*)piscina_allocare(piscina,
            (memoriae_index)n * (memoriae_index)magnitudo(Fractio));
        si (!_gs_librorum(b, piscina, mu_l, normae_l))
        {
            CREDO_FALSUM (reticulum_orthogonaliza(b, piscina, &mu,
                &normae));
            perge;
        }
        CREDO_NECESSE (reticulum_orthogonaliza(b, piscina, &mu,
            &normae));
        productum = fractio_ex_s64(I);
        per (i = ZEPHYRUM; i < n; i++)
        {
            per (j = ZEPHYRUM; j <= i; j++)
            {
                CREDO_VERUM (fractio_aequalis(*_mu(mu, i, j),
                    mu_l[i * n + j]));
            }
            CREDO_VERUM (fractio_aequalis(normae[i], normae_l[i]));
            productum = fractio_multiplica(productum, normae[i],
                piscina);
        }
        CREDO_NECESSE (matrix_determinans(_forma(b, piscina), piscina,
            &det));
        CREDO_VERUM (fractio_aequalis(productum,
            fractio_ex_magno(det)));
        CREDO_NECESSE (reticulum_est_reductum(b, &delta, piscina, &r));
        CREDO_VERUM (r == _reductum_librorum(b, delta, piscina));
        reducti  += r ? I : ZEPHYRUM;
        alti     += (r && n >= III) ? I : ZEPHYRUM;
        probati++;
    }
    imprimere("    bases probatae %d, reductae %d (n >= 3: %d)\n",
        (int)probati, (int)reducti, (int)alti);
    CREDO_MAIOR_I32 (probati, L);
    CREDO_MAIOR_I32 (reducti, ZEPHYRUM);    /* iudicium utrumque videt */
    CREDO_MINOR_I32 (reducti, probati);
    CREDO_MAIOR_I32 (alti, II);   /* Lovasz in k >= 2 contra libros */
    CREDO_MAIOR_I32 ((i32)reticulum_apex_officinarum(), ZEPHYRUM);
}

/* functiones T3-T4 nondum factae: FALSUM, exitus non tacti */
interior vacuum
_probare_nondum (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
     Matrix  b        = _z("[1, 0; 0, 1]", piscina);
     Matrix  r;
     Magnus  norma;

    r.anulus = NIHIL;
    CREDO_FALSUM (reticulum_minimum(b, piscina, &r, &norma));
    CREDO_FALSUM (reticulum_relationes(b, NIHIL, piscina, &r));
    CREDO_NIHIL (r.anulus);
}


/* ==================================================
 * T2: reductio LLL
 * ================================================== */

/* |det u| = 1 */
interior b32
_unimodularis (
     Matrix  u,
    Piscina* piscina)
{
    Magnus det;

    si (!matrix_determinans(u, piscina, &det))
    {
        redde FALSUM;
    }
    redde magnus_aequalis(magnus_absolutum(det, piscina),
        magnus_ex_s64(I));
}

/* idem reticulum: formae Hermite aequales (lineae) */
interior b32
_idem_reticulum (
     Matrix  a,
     Matrix  b,
    Piscina* piscina)
{
    Matrix hermite_prima;
    Matrix hermite_secunda;

    si (   !matrix_forma_hermite(a, piscina, &hermite_prima, NIHIL)
        || !matrix_forma_hermite(b, piscina, &hermite_secunda, NIHIL))
    {
        redde FALSUM;
    }
    redde matrix_aequalis(hermite_prima, hermite_secunda);
}

/* reducta = u b, u unimodularis, idem reticulum, reducta LLL-reducta */
interior vacuum
_proprietates (
     Matrix  b,
    Fractio* delta,
     Matrix  reducta,
     Matrix  u,
    Piscina* piscina)
{
    Matrix ub;
       b32 r = FALSUM;

    CREDO_NECESSE (matrix_multiplica(u, b, piscina, &ub));
    CREDO_VERUM (matrix_aequalis(ub, reducta));
    CREDO_VERUM (_unimodularis(u, piscina));
    CREDO_VERUM (_idem_reticulum(b, reducta, piscina));
    CREDO_NECESSE (reticulum_est_reductum(reducta, delta, piscina, &r));
    CREDO_VERUM (r);
}

interior vacuum
_lll_manu (
    CredoContextus* c)
{
                Piscina* piscina       = c->piscina;
                Fractio  tres_quartae  = _f("3/4", piscina);
                 Matrix  b;
                 Matrix  r;
                 Matrix  u;
    ReticulumStatistica  s;

    /* exemplum vulgare: [0,1,0; 1,0,1; -1,0,2], II permutationes, III
     * reductiones (Python, LLL librorum eodem ordine) */
    b = _z("[1, 1, 1; -1, 0, 2; 3, 5, 6]", piscina);
    CREDO_NECESSE (reticulum_reduce(b, &tres_quartae, piscina, &r, &u,
        &s));
    CREDO_VERUM (matrix_aequalis(r, _z("[0, 1, 0; 1, 0, 1; -1, 0, 2]",
        piscina)));
    CREDO_AEQUALIS_S64 ((s64)s.permutationes, (s64)II);
    CREDO_AEQUALIS_S64 ((s64)s.reductiones, (s64)III);
    _proprietates(b, &tres_quartae, r, u, piscina);
    CREDO_NECESSE (reticulum_reduce(b, NIHIL, piscina, &r, &u, &s));
    CREDO_VERUM (matrix_aequalis(r, _z("[0, 1, 0; 1, 0, 1; -1, 0, 2]",
        piscina)));
    /* iam reducta: nihil mutatur */
    b = _z("[1, 0, 0; 0, 1, 0; 0, 0, 1]", piscina);
    CREDO_NECESSE (reticulum_reduce(b, NIHIL, piscina, &r, &u, &s));
    CREDO_VERUM (matrix_aequalis(r, b));
    CREDO_VERUM (matrix_aequalis(u, b));
    CREDO_AEQUALIS_S64 ((s64)s.permutationes, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64 ((s64)s.reductiones, (s64)ZEPHYRUM);
    /* exitus optionales NIHIL; basis vacua et vector unus */
    b = _z("[1, 1, 1; -1, 0, 2; 3, 5, 6]", piscina);
    CREDO_VERUM (reticulum_reduce(b, NIHIL, piscina, &r, NIHIL, NIHIL));
    CREDO_NECESSE (reticulum_reduce(_z("[]", piscina), NIHIL, piscina,
        &r,
        &u, &s));
    CREDO_AEQUALIS_I32 (matrix_lineae(r), ZEPHYRUM);
    b = _z("[-4, 6, 2]", piscina);
    CREDO_NECESSE (reticulum_reduce(b, NIHIL, piscina, &r, &u, &s));
    CREDO_VERUM (matrix_aequalis(r, b));
}

interior vacuum
_lll_fortuita (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
                Fractio  deltae[III];
                 Matrix  b;
                 Matrix  r;
                 Matrix  u;
                    i64  semen = (i64)MMXXVIII;
                    i32  exemplum;
                    i32  probati  = ZEPHYRUM;
                    i32  mutati   = ZEPHYRUM;
    ReticulumStatistica  s;

    deltae[ZEPHYRUM]  = _f("3/4", piscina);
    deltae[I]         = _f("99/100", piscina);
    deltae[II]        = _f("1", piscina);
    per (exemplum = ZEPHYRUM; exemplum < LX; exemplum++)
    {
        i32 n = (i32)(I + _alea(&semen, VIII));

        b = _basis_fortuita(&semen, n, n + (i32)_alea(&semen, III),
            exemplum % III == ZEPHYRUM ? (s64)M : (s64)XX,
            exemplum % IV == III, piscina);
        si (!reticulum_orthogonaliza(b, piscina, NIHIL, NIHIL))
        {
            CREDO_FALSUM (reticulum_reduce(b, NIHIL, piscina, &r, &u,
                &s));
            perge;
        }
        CREDO_NECESSE (reticulum_reduce(b, &deltae[exemplum % III],
            piscina,
            &r, &u, &s));
        _proprietates(b, &deltae[exemplum % III], r, u, piscina);
        mutati += (s.permutationes > ZEPHYRUM) ? I : ZEPHYRUM;
        probati++;
    }
    imprimere("    bases reductae %d, permutatione mutatae %d\n",
        (int)probati, (int)mutati);
    CREDO_MAIOR_I32 (probati, L);
    CREDO_MAIOR_I32 (mutati, XXX);
}

/* subset-sum (Lagarias-Odlyzko): b_i = (e_i, N a_i), b_(n+1) = (0, N s);
 * x reperitur ut linea +-(x, 0) (casus ex Python probatus) */
interior vacuum
_summam_partium (
    CredoContextus* c)
{
    hic_manens constans character* PONDERA[X] = {
        "898159780889", "1011707717353", "1089925756095",
            "1033311238886",
        "550786322915", "638293147116", "863764036341", "660637959597",
        "560277001298", "1089569543265"
    };
    hic_manens constans s32 SECRETUM[X] = { I, ZEPHYRUM, I, I, ZEPHYRUM,
        ZEPHYRUM, I, ZEPHYRUM, I, I };
                Piscina* piscina  = c->piscina;
                 Magnus  scala    = magnus_ex_s64((s64)1048576);
                 Magnus  summa    = magnus_ex_s64(ZEPHYRUM);
                 Magnus  a;
                 Magnus  x;
                 Matrix  b;
                 Matrix  r;
                    i32  i;
                    i32  j;
                    b32  inventum = FALSUM;
    ReticulumStatistica  s;

    CREDO_NECESSE (matrix_nulla(&ANULUS_INTEGRORUM, XI, XI, piscina,
        &b));
    per (i = ZEPHYRUM; i < X; i++)
    {
        CREDO_NECESSE (magnus_ex_chorda(chorda_ex_literis(PONDERA[i],
            piscina), piscina, &a));
        x = magnus_ex_s64(I);
        matrix_pone(&b, i, i, &x);
        x = magnus_multiplica(scala, a, piscina);
        matrix_pone(&b, i, X, &x);
        si (SECRETUM[i])
        {
            summa = magnus_adde(summa, a, piscina);
        }
    }
    x = magnus_multiplica(scala, summa, piscina);
    matrix_pone(&b, X, X, &x);
    CREDO_NECESSE (reticulum_reduce(b, NIHIL, piscina, &r, NIHIL, &s));
    per (i = ZEPHYRUM; i < XI && !inventum; i++)
    {
        s32 signum = ZEPHYRUM;
        b32 congruit =
            magnus_signum(*(constans Magnus*)matrix_elementum(r,
            i, X)) == ZEPHYRUM;

        per (j = ZEPHYRUM; j < X && congruit; j++)
        {
            s64 v = ZEPHYRUM;

            (vacuum)magnus_ad_s64(*(constans Magnus*)matrix_elementum(r,
                i,
                j), &v);
            si (signum == ZEPHYRUM && v != ZEPHYRUM)
            {
                signum = (s32)v;
            }
            congruit = (v == (s64)signum * (s64)SECRETUM[j]);
        }
        inventum = congruit && signum != ZEPHYRUM;
    }
    imprimere("    permutationes %ld\n", (long)s.permutationes);
    CREDO_VERUM (inventum);
}

/* forma b b^T: eadem u ac basis (nucleus communis), forma_reducta =
 * u forma u^T; formae E_8, A_4 */
interior vacuum
_formam_reducere (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
                 Matrix  b;
                 Matrix  g;
                 Matrix  rb;
                 Matrix  ub;
                 Matrix  rg;
                 Matrix  u_formae;
                 Matrix  t;
                 Matrix  productum;
                 Magnus  det;
                    i64  semen = (i64)MMXXIX;
                    i32  exemplum;
                    i32  probati = ZEPHYRUM;
                    b32  r;
    ReticulumStatistica  sb;
    ReticulumStatistica  sg;

    per (exemplum = ZEPHYRUM; exemplum < XXX; exemplum++)
    {
        i32 n = (i32)(I + _alea(&semen, VII));

        b = _basis_fortuita(&semen, n, n + (i32)_alea(&semen, II), L,
            exemplum % V == IV, piscina);
        si (!reticulum_reduce(b, NIHIL, piscina, &rb, &ub, &sb))
        {
            perge;
        }
        g = _forma(b, piscina);
        CREDO_NECESSE (reticulum_formam_reduce(g, NIHIL, piscina, &rg,
            &u_formae,
            &sg));
        CREDO_VERUM (matrix_aequalis(ub, u_formae));
        CREDO_VERUM (sb.permutationes == sg.permutationes
            && sb.reductiones == sg.reductiones);
        CREDO_VERUM (matrix_aequalis(rg, _forma(rb, piscina)));
        probati++;
    }
    CREDO_MAIOR_I32 (probati, XX);
    /* E_8 (Cartan, unimodularis) et A_4 */
    g = _z("[2, -1, 0, 0, 0, 0, 0, 0; -1, 2, -1, 0, 0, 0, 0, 0;"
        " 0, -1, 2, -1, 0, 0, 0, -1; 0, 0, -1, 2, -1, 0, 0, 0;"
        " 0, 0, 0, -1, 2, -1, 0, 0; 0, 0, 0, 0, -1, 2, -1, 0;"
        " 0, 0, 0, 0, 0, -1, 2, 0; 0, 0, -1, 0, 0, 0, 0, 2]", piscina);
    CREDO_NECESSE (matrix_determinans(g, piscina, &det));
    CREDO_VERUM (magnus_aequalis(det, magnus_ex_s64(I)));
    CREDO_NECESSE (reticulum_formam_reduce(g, NIHIL, piscina, &rg,
        &u_formae,
        &sg));
    CREDO_VERUM (_unimodularis(u_formae, piscina));
    CREDO_NECESSE (matrix_transposita(u_formae, piscina, &t));
    CREDO_NECESSE (matrix_multiplica(u_formae, g, piscina, &productum));
    CREDO_NECESSE (matrix_multiplica(productum, t, piscina,
        &productum));
    CREDO_VERUM (matrix_aequalis(productum, rg));
    CREDO_NECESSE (reticulum_formam_est_reducta(rg, NIHIL, piscina,
        &r));
    CREDO_VERUM (r);
    g = _z("[2, -1, 0, 0; -1, 2, -1, 0; 0, -1, 2, -1; 0, 0, -1, 2]",
        piscina);
    CREDO_NECESSE (reticulum_formam_reduce(g, NIHIL, piscina, &rg,
        &u_formae,
        &sg));
    CREDO_NECESSE (reticulum_formam_est_reducta(rg, NIHIL, piscina,
        &r));
    CREDO_VERUM (r);
}

interior vacuum
_probare_lll_refutationes (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
    Fractio  quarta   = _f("1/4", piscina);
     Matrix  r;
     Matrix  u;

    r.anulus = NIHIL;
    u.anulus = NIHIL;
    CREDO_FALSUM (reticulum_reduce(_z("[1, 2; 2, 4]", piscina), NIHIL,
        piscina, &r, &u, NIHIL));
    CREDO_FALSUM (reticulum_reduce(_z("[1, 0; 0, 1; 1, 1]", piscina),
        NIHIL,
        piscina, &r, &u, NIHIL));
    CREDO_FALSUM (reticulum_reduce(_z("[1, 0; 0, 1]", piscina), &quarta,
        piscina, &r, &u, NIHIL));
    CREDO_FALSUM (reticulum_reduce(_matrix(&ANULUS_RATIONALIUM,
        "[1/2, 0; 0, 1]", piscina), NIHIL, piscina, &r, &u, NIHIL));
    CREDO_FALSUM (reticulum_formam_reduce(_z("[1, 2; 2, 1]", piscina),
        NIHIL, piscina, &r, &u, NIHIL));
    CREDO_FALSUM (reticulum_formam_reduce(_z("[2, 1; 0, 2]", piscina),
        NIHIL, piscina, &r, &u, NIHIL));
    CREDO_NIHIL (r.anulus);
    CREDO_NIHIL (u.anulus);
}

/* basis maior (n = XX, elementa ~10^25): compactio piscinae temporariae */
interior vacuum
_lll_memoriam (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
                 Matrix  b;
                 Matrix  r;
                 Matrix  u;
                    i64  semen = (i64)MMXXX;
    ReticulumStatistica  s;

    b = _basis_fortuita(&semen, XX, XX, C, VERUM, piscina);
    CREDO_NECESSE (reticulum_reduce(b, NIHIL, piscina, &r, &u, &s));
    imprimere("    n = 20: permutationes %ld, reductiones %ld, apex %lu\n",
        (long)s.permutationes, (long)s.reductiones,
        (unsigned long)reticulum_apex_officinarum());
    /* sine compactione ~8.9 MB (mensum 2026-10-13), cum ea ~1.1 MB */
    CREDO_VERUM (reticulum_apex_officinarum()
        < (memoriae_index)3 * (memoriae_index)1048576);
    _proprietates(b, NIHIL, r, u, piscina);
}

/* LLL sub termino: vitium quod gyrum infinitum facit (d_k non
 * renovatus, plantatio T2) suitam aliter in aeternum tenet - planta
 * horas II pependit antequam manu occisa est. Sectio prius in filio
 * currit (XXX s; sectiones LLL simul ~0.25 s), deinde in parente,
 * quae numerat - sed solum si filius transiit: aliter parens ipse
 * penderet. */
interior vacuum
_sub_termino (
    CredoContextus* c,
       CredoFunctio corpus)
{
    memoriae_index fracti_ante = credo_numerus_fracti();

    CREDO_NON_PENDET (corpus(c), XXX * M);
    si (credo_numerus_fracti() == fracti_ante)
    {
        corpus(c);
    }
}

interior vacuum
_probare_lll_manu (
    CredoContextus* c)
{
    _sub_termino(c, _lll_manu);
}

interior vacuum
_probare_lll_fortuita (
    CredoContextus* c)
{
    _sub_termino(c, _lll_fortuita);
}

interior vacuum
_probare_summam_partium (
    CredoContextus* c)
{
    _sub_termino(c, _summam_partium);
}

interior vacuum
_probare_formam_reducere (
    CredoContextus* c)
{
    _sub_termino(c, _formam_reducere);
}

interior vacuum
_probare_lll_memoriam (
    CredoContextus* c)
{
    _sub_termino(c, _lll_memoriam);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "manu", _probare_manu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iudicium manu", _probare_iudicium_manu, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "refutationes", _probare_refutationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "formae", _probare_formam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma contra basim", _probare_formam_contra_basim, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "contra libros", _probare_contra_libros, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "nondum facta", _probare_nondum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "LLL manu", _probare_lll_manu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "LLL fortuita", _probare_lll_fortuita, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "summa partium", _probare_summam_partium, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "formam reducere", _probare_formam_reducere, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "LLL refutationes", _probare_lll_refutationes, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "LLL memoria", _probare_lll_memoriam, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("reticulum", SECTIONES);
}
