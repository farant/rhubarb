/* probatio_reticulum.c - probationes reticulorum (project-specs/
 * reticulum-plan.md)
 *
 * T1: Gram-Schmidt integra contra Gram-Schmidt LIBRORUM (fractiones,
 * b*_i = b_i - sum mu_ij b*_j) in hac probatione rescriptam - alia via,
 * eadem mathesis; productum normarum contra determinantem Gram
 * (Bareiss, matrix.h); iudicium reductionis contra iudicium rationale
 * rescriptum. Casus manu (et Python Fractions) computati: exemplum
 * LLL vulgare [1,1,1; -1,0,2; 3,5,6] et basis eius reducta (mu = 1/2
 * exacte), aequalitas Lovasz ad delta = 1/2.
 *
 * T2: invarianta cuiusque LLL recti (u b = reducta, |det u| = 1,
 * formae Hermite aequales, iudicium T1), non effectus fixi tantum.
 * T3: fraternitas bruta in capsa (Cramer, sine Gram-Schmidt) vector
 * pro vectore; radices A_n, D_n, E6-E8; B et U B idem.
 * T4: relationes verae, numerus n - gradus, saturatio contra brutam et
 * coincidentias summarum 2O.
 * Sectiones LLL et sequentes sub _sub_termino (filius prius, XXX s). */
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

/* basis obliqua: u = 8n operationes linearum (linea i += q linea j,
 * |q| <= 1000) super identitatem, b = u diag(1..n). Reductio eius u
 * magnum reddit (extra s64) - compactio LLL vere probatur (recensio
 * T5). */
interior Matrix
_basis_obliqua (
        i64* semen,
        i32  n,
    Piscina* piscina)
{
    Matrix u;
    Matrix b;
    Magnus q;
    Magnus x;
       i32 gradus;
       i32 i;
       i32 j;
       i32 k;

    (vacuum)matrix_identitas(&ANULUS_INTEGRORUM, n, piscina, &u);
    per (gradus = ZEPHYRUM; gradus < VIII * n; gradus++)
    {
        i = (i32)_alea(semen, (s64)n);
        j = (i32)_alea(semen, (s64)n);
        si (i == j)
        {
            perge;
        }
        q = magnus_ex_s64(_alea(semen, MM + I) - (s64)M);
        per (k = ZEPHYRUM; k < n; k++)
        {
            x = magnus_adde(*(constans Magnus*)matrix_elementum(u, i,
                k),
                magnus_multiplica(q,
                *(constans Magnus*)matrix_elementum(
                u, j, k), piscina), piscina);
            matrix_pone(&u, i, k, &x);
        }
    }
    (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, n, n, piscina, &b);
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            x = magnus_multiplica(*(constans Magnus*)matrix_elementum(u,
                i,
                k), magnus_ex_s64((s64)k + I), piscina);
            matrix_pone(&b, i, k, &x);
        }
    }
    redde b;
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
                Fractio  deltae[V];
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
    /* prope 1/4 et 51/100 (recensio T5: regimen non probatum) */
    deltae[III]       = _f("250000000000000001/1000000000000000000",
        piscina);
    deltae[IV]        = _f("51/100", piscina);
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
        CREDO_NECESSE (reticulum_reduce(b, &deltae[exemplum % V],
            piscina,
            &r, &u, &s));
        _proprietates(b, &deltae[exemplum % V], r, u, piscina);
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
    imprimere("    permutationes %ld, reductiones %ld\n",
        (long)s.permutationes, (long)s.reductiones);
    CREDO_VERUM (inventum);
    /* Cohen 2.6.3 librorum (recensio T5, oraculum suum) idem numerat */
    CREDO_AEQUALIS_S64 ((s64)s.permutationes, (s64)CCXVI);
    CREDO_AEQUALIS_S64 ((s64)s.reductiones, (s64)CCLXXXVII);
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
    /* apex = summa (T5): cum compactione ~3.05 MB (veteres bis), sine
     * ea ~8.9 MB (piscina una) - mensum 2026-10-13 */
    CREDO_VERUM (reticulum_apex_officinarum()
        < (memoriae_index)V * (memoriae_index)1048576);
    /* compactio facta (limes 1 MB transitus): piscina vetus > 1 MB
     * bis numerata, ergo apex > 2 MB */
    CREDO_VERUM (reticulum_apex_officinarum()
        > (memoriae_index)II * (memoriae_index)1048576);
    _proprietates(b, NIHIL, r, u, piscina);
    /* basis obliqua (recensio T5): u = CXXVIII operationes linearum
     * |q| <= 1000, b = u diag(1..16) - elementa u extra s64, ergo
     * compactio u vere transcribere debet (porta venenum: planta 'u non
     * transcriptum' usum post liberationem facit) */
    b = _basis_obliqua(&semen, XVI, piscina);
    CREDO_NECESSE (reticulum_reduce(b, NIHIL, piscina, &r, &u, &s));
    imprimere("    obliqua n = 16: permutationes %ld, apex %lu\n",
        (long)s.permutationes,
        (unsigned long)reticulum_apex_officinarum());
    _proprietates(b, NIHIL, r, u, piscina);
}


/* ==================================================
 * T3: enumeratio
 *
 * Oraculum: fraternitas bruta in capsa, sine Gram-Schmidt et sine
 * reductione. max y_i^2 sub y^T G y <= R est R (G^-1)_ii = R
 * det(G sine i) / det G (Cramer), ergo capsa |y_i| <= b_i cum b_i
 * minimo ut b_i^2 det G >= R det(G sine i) - exacta, in integris.
 * ================================================== */

nomen structura {
       i32  numerus;
       i32  columnae;
    Magnus* normae;
    Magnus* vectores;               /* numerus x columnae */
} Bruta;

interior Magnus
_elem (
    Matrix m,
       i32 i,
       i32 j)
{
    redde *(constans Magnus*)matrix_elementum(m, i, j);
}

interior s64
_capsa (
     Matrix  g,
        i32  i,
        s64  radius,
    Piscina* piscina)
{
    Matrix minor;
    Magnus det;
    Magnus det_minoris;
    Magnus dextra;
    Magnus x;
       s64 b = ZEPHYRUM;
       i32 n = matrix_lineae(g);
       i32 a;
       i32 k;
       i32 la = ZEPHYRUM;
       i32 ca;

    (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, n - I, n - I, piscina,
        &minor);
    per (a = ZEPHYRUM; a < n; a++)
    {
        si (a == i)
        {
            perge;
        }
        ca = ZEPHYRUM;
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (k == i)
            {
                perge;
            }
            x = _elem(g, a, k);
            matrix_pone(&minor, la, ca, &x);
            ca++;
        }
        la++;
    }
    (vacuum)matrix_determinans(g, piscina, &det);
    (vacuum)matrix_determinans(minor, piscina, &det_minoris);
    dextra = magnus_multiplica(magnus_ex_s64(radius), det_minoris,
        piscina);
    dum (magnus_compara(magnus_multiplica(magnus_ex_s64(b * b), det,
        piscina), dextra) < ZEPHYRUM)
    {
        b++;
    }
    redde b;
}

interior s32
_bruta_compara (
    constans Bruta* b,
               i32  p,
               i32  q)
{
    s32 c = magnus_compara(b->normae[p], b->normae[q]);
    i32 j;

    per (j = ZEPHYRUM; c == ZEPHYRUM && j < b->columnae; j++)
    {
        c = magnus_compara(b->vectores[p * b->columnae + j],
            b->vectores[q * b->columnae + j]);
    }
    redde c;
}

interior vacuum
_bruta_permuta (
    Bruta* b,
      i32  p,
      i32  q)
{
    Magnus t;
       i32 j;

    t             = b->normae[p];
    b->normae[p]  = b->normae[q];
    b->normae[q]  = t;
    per (j = ZEPHYRUM; j < b->columnae; j++)
    {
        t = b->vectores[p * b->columnae + j];
        b->vectores[p * b->columnae + j]
            = b->vectores[q * b->columnae + j];
        b->vectores[q * b->columnae + j] = t;
    }
}

/* omnes y in capsa; vector exitus (forma: y; basis: y basis) cum
 * 0 < y^T g y <= R et signo canonico, ordine canonico. FALSUM si
 * capsa nimis magna (casus omittitur). */
interior b32
_bruta (
     Matrix  g,
     Matrix  basis,
        b32  est_forma,
        s64  radius,
    Piscina* piscina,
      Bruta* exitus)
{
       i32  n         = matrix_lineae(g);
       i32  columnae  = est_forma ? n : matrix_columnae(basis);
       s64  capsa[VIII];
       s64  y[VIII];
       s64  capacitas = I;
    Magnus  q;
    Magnus  r = magnus_ex_s64(radius);
    Magnus* v;
       i32  i;
       i32  j;
       i32  k;
       s32  sg;
       b32  finis = FALSUM;

    si (n > VIII)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        capsa[i]   = _capsa(g, i, radius, piscina);
        y[i]       = -capsa[i];
        capacitas  *= II * capsa[i] + I;
        si (capacitas > (s64)CC * (s64)M)
        {
            redde FALSUM;
        }
    }
    exitus->numerus   = ZEPHYRUM;
    exitus->columnae  = columnae;
    exitus->normae    = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)capacitas * (memoriae_index)magnitudo(Magnus));
    exitus->vectores  = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)capacitas * (memoriae_index)(columnae + I)
        * (memoriae_index)magnitudo(Magnus));
    dum (!finis)
    {
        q = magnus_ex_s64(ZEPHYRUM);
        per (i = ZEPHYRUM; i < n; i++)
        {
            per (j = ZEPHYRUM; j < n; j++)
            {
                q = magnus_adde(q, magnus_multiplica(magnus_ex_s64(y[i]
                    * y[j]), _elem(g, i, j), piscina), piscina);
            }
        }
        si (   magnus_signum(q) > ZEPHYRUM && magnus_compara(q, r)
            <= ZEPHYRUM)
        {
            v   = &exitus->vectores[exitus->numerus * columnae];
            sg  = ZEPHYRUM;
            per (j = ZEPHYRUM; j < columnae; j++)
            {
                si (est_forma)
                {
                    v[j] = magnus_ex_s64(y[j]);
                }
                alioquin
                {
                    v[j] = magnus_ex_s64(ZEPHYRUM);
                    per (i = ZEPHYRUM; i < n; i++)
                    {
                        v[j] = magnus_adde(v[j], magnus_multiplica(
                            magnus_ex_s64(y[i]), _elem(basis, i, j),
                            piscina), piscina);
                    }
                }
                si (sg == ZEPHYRUM)
                {
                    sg = magnus_signum(v[j]);
                }
            }
            si (sg > ZEPHYRUM)
            {
                exitus->normae[exitus->numerus] = q;
                exitus->numerus++;
            }
        }
        k = ZEPHYRUM;
        dum (k < n && y[k] == capsa[k])
        {
            y[k] = -capsa[k];
            k++;
        }
        si (k == n)
        {
            finis = VERUM;
        }
        alioquin
        {
            y[k]++;
        }
    }
    /* ordo canonicus (insertio) */
    per (i = I; i < exitus->numerus; i++)
    {
        per (k = i; k > ZEPHYRUM && _bruta_compara(exitus, k - I, k)
            > ZEPHYRUM; k--)
        {
            _bruta_permuta(exitus, k - I, k);
        }
    }
    redde VERUM;
}

/* enumeratio = bruta: numerus, completa, primi min(servandi, numerus)
 * vector pro vectore */
interior b32
_congruit (
    constans ReticulumEnumeratio* e,
                  constans Bruta* b,
                             i32  servandi)
{
    i32 exspectati = b->numerus < servandi ? b->numerus : servandi;
    i32 i;
    i32 j;

    si (   !e->completa || e->numerus != (i64)b->numerus
        || e->servati                   != exspectati
        || matrix_lineae(e->vectores)   != exspectati
        || matrix_columnae(e->vectores) != b->columnae)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < exspectati; i++)
    {
        si (!magnus_aequalis(e->normae[i], b->normae[i]))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < b->columnae; j++)
        {
            si (!magnus_aequalis(_elem(e->vectores, i, j),
                b->vectores[i * b->columnae + j]))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior ReticulumQuaestio
_quaestio (
    s64 radius,
    i32 servandi,
    i64 limes_nodorum)
{
    ReticulumQuaestio q;

    q.radius_quadratus  = magnus_ex_s64(radius);
    q.limes_servandi    = servandi;
    q.limes_nodorum     = limes_nodorum;
    redde q;
}

/* matrix Cartan simpliciter laqueata: catena 0..n-1; si ramus_ad >= 0,
 * catena 0..n-2 et nodus n-1 ad nodum ramus_ad */
interior Matrix
_cartan (
         i32  n,
         s32  ramus_ad,
     Piscina* piscina)
{
    Matrix m;
    Magnus duo     = magnus_ex_s64(II);
    Magnus minus   = magnus_ex_s64(-(s64)I);
       i32 catena  = ramus_ad >= ZEPHYRUM ? n - I : n;
       i32 i;

    (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, n, n, piscina, &m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        matrix_pone(&m, i, i, &duo);
    }
    per (i = I; i < catena; i++)
    {
        matrix_pone(&m, i, i - I, &minus);
        matrix_pone(&m, i - I, i, &minus);
    }
    si (ramus_ad >= ZEPHYRUM)
    {
        matrix_pone(&m, n - I, (i32)ramus_ad, &minus);
        matrix_pone(&m, (i32)ramus_ad, n - I, &minus);
    }
    redde m;
}

interior b32
_vectores_aequales (
    Matrix a,
       i32 la,
    Matrix b,
       i32 lb)
{
    i32 j;

    si (matrix_columnae(a) != matrix_columnae(b))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < matrix_columnae(a); j++)
    {
        si (!magnus_aequalis(_elem(a, la, j), _elem(b, lb, j)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_textus_vectoris (
                Matrix  m,
                   i32  linea,
    constans character* textus,
               Piscina* piscina)
{
    Matrix v = _z(textus, piscina);
       i32 j;

    si (matrix_columnae(v) != matrix_columnae(m))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < matrix_columnae(m); j++)
    {
        si (!magnus_aequalis(_elem(m, linea, j), _elem(v, ZEPHYRUM, j)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_enumeratio_manu (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
      ReticulumQuaestio  q;
    ReticulumEnumeratio  e;

    /* Z^2, R = 2: (0,1), (1,0) norma 1; (1,-1), (1,1) norma 2 */
    q = _quaestio(II, X, ZEPHYRUM);
    CREDO_NECESSE (reticulum_formam_enumera(_z("[1, 0; 0, 1]", piscina),
        &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)IV);
    CREDO_VERUM (e.completa);
    CREDO_AEQUALIS_I32 (e.servati, IV);
    CREDO_VERUM (_textus_vectoris(e.vectores, ZEPHYRUM, "[0, 1]",
        piscina));
    CREDO_VERUM (_textus_vectoris(e.vectores, I, "[1, 0]", piscina));
    CREDO_VERUM (_textus_vectoris(e.vectores, II, "[1, -1]", piscina));
    CREDO_VERUM (_textus_vectoris(e.vectores, III, "[1, 1]", piscina));
    CREDO_VERUM (magnus_aequalis(e.normae[ZEPHYRUM], magnus_ex_s64(I)));
    CREDO_VERUM (magnus_aequalis(e.normae[III], magnus_ex_s64(II)));
    /* Z^3, R = 3: 3 + 6 + 4 */
    q = _quaestio(III, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (reticulum_enumera(_z("[1, 0, 0; 0, 1, 0; 0, 0, 1]",
        piscina), &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)XIII);
    CREDO_AEQUALIS_I32 (e.servati, ZEPHYRUM);
    CREDO_NIHIL (e.normae);
    /* A2: radices (0,1), (1,0), (1,1) in coefficientibus */
    q = _quaestio(II, X, ZEPHYRUM);
    CREDO_NECESSE (reticulum_formam_enumera(_z("[2, -1; -1, 2]",
        piscina), &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)III);
    CREDO_VERUM (_textus_vectoris(e.vectores, ZEPHYRUM, "[0, 1]",
        piscina));
    CREDO_VERUM (_textus_vectoris(e.vectores, II, "[1, 1]", piscina));
    /* D4 in Z^4: XXIV radices, XII modulo signo; prima canonice */
    CREDO_NECESSE (reticulum_enumera(_z("[1, -1, 0, 0; 0, 1, -1, 0; "
        "0, 0, 1, -1; 0, 0, 1, 1]", piscina), &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)XII);
    CREDO_VERUM (_textus_vectoris(e.vectores, ZEPHYRUM, "[0, 0, 1, -1]",
        piscina));
    CREDO_VERUM (_textus_vectoris(e.vectores, IX, "[1, 0, 0, 1]",
        piscina));
    /* radius 0 et basis vacua: nihil, completa */
    q = _quaestio(ZEPHYRUM, X, ZEPHYRUM);
    CREDO_NECESSE (reticulum_formam_enumera(_z("[1, 0; 0, 1]", piscina),
        &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)ZEPHYRUM);
    CREDO_VERUM (e.completa);
    CREDO_AEQUALIS_I32 (matrix_lineae(e.vectores), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (matrix_columnae(e.vectores), II);
    q = _quaestio(V, X, ZEPHYRUM);
    CREDO_NECESSE (reticulum_enumera(_z("[]", piscina), &q, piscina,
        &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)ZEPHYRUM);
    CREDO_VERUM (e.completa);
}

/* systemata radicum: A_n n(n+1)/2, D_n n(n-1), E6 36, E7 63, E8 120
 * (modulo signo); E8 per seriem theta: 240, 2160, 6720 in normis
 * 2, 4, 6 */
interior vacuum
_enumeratio_radicum (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
      ReticulumQuaestio  q        = _quaestio(II, ZEPHYRUM, ZEPHYRUM);
    ReticulumEnumeratio  e;
                 Matrix  e8 = _cartan(VIII, II, piscina);
                    i32  n;

    per (n = I; n <= VIII; n++)
    {
        CREDO_NECESSE (reticulum_formam_enumera(_cartan(n, -(s32)I,
            piscina), &q, piscina, &e));
        CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)(n * (n + I) / II));
        CREDO_VERUM (e.completa);
    }
    per (n = IV; n <= VIII; n++)
    {
        CREDO_NECESSE (reticulum_formam_enumera(_cartan(n, (s32)n
            - (s32)III, piscina), &q, piscina, &e));
        CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)(n * (n - I)));
    }
    CREDO_NECESSE (reticulum_formam_enumera(_cartan(VI, II, piscina),
        &q,
        piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)XXXVI);
    CREDO_NECESSE (reticulum_formam_enumera(_cartan(VII, II, piscina),
        &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)LXIII);
    CREDO_NECESSE (reticulum_formam_enumera(e8, &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)CXX);
    q = _quaestio(IV, ZEPHYRUM, ZEPHYRUM);
    CREDO_NECESSE (reticulum_formam_enumera(e8, &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)MCC);
    q = _quaestio(VI, IV * M + D, ZEPHYRUM);
    CREDO_NECESSE (reticulum_formam_enumera(e8, &q, piscina, &e));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)(IV * M + DLX));
    CREDO_AEQUALIS_I32 (e.servati, IV * M + D);
    CREDO_VERUM (magnus_aequalis(e.normae[CXIX], magnus_ex_s64(II)));
    CREDO_VERUM (magnus_aequalis(e.normae[CXX], magnus_ex_s64(IV)));
    CREDO_VERUM (magnus_aequalis(e.normae[MCXCIX], magnus_ex_s64(IV)));
    CREDO_VERUM (magnus_aequalis(e.normae[MCC], magnus_ex_s64(VI)));
}

/* contra bruta: formae et bases fortuitae (dimensio 1-4, elementa
 * parva), vector pro vectore; regimen numeratum: casus cum normis
 * aequalibus (ordo lexicographicus refert) et cum vectoribus multis */
interior vacuum
_enumeratio_contra_bruta (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
                    i64  semen    = (i64)MMXXXI;
                    i32  exemplum;
                    i32  probati   = ZEPHYRUM;
                    i32  aequales  = ZEPHYRUM;
                    i32  multi     = ZEPHYRUM;
                    i32  i;
                 Matrix  b;
                 Matrix  g;
                  Bruta  br;
      ReticulumQuaestio  q;
    ReticulumEnumeratio  e;
                    s64  radius;
                    b32  est_forma;

    per (exemplum = ZEPHYRUM; exemplum < CC; exemplum++)
    {
        i32 n = (i32)(I + _alea(&semen, IV));

        est_forma = exemplum % II == ZEPHYRUM;
        b = _basis_fortuita(&semen, n, est_forma ? n
            : n + (i32)_alea(&semen, III), III, FALSUM, piscina);
        si (!reticulum_orthogonaliza(b, piscina, NIHIL, NIHIL))
        {
            perge;
        }
        g       = _forma(b, piscina);
        radius  = _alea(&semen, XXI);
        si (!_bruta(g, b, est_forma, radius, piscina, &br))
        {
            perge;
        }
        q = _quaestio(radius, (i32)M, ZEPHYRUM);
        si (est_forma)
        {
            CREDO_NECESSE (reticulum_formam_enumera(g, &q, piscina,
                &e));
        }
        alioquin
        {
            CREDO_NECESSE (reticulum_enumera(b, &q, piscina, &e));
        }
        CREDO_VERUM (_congruit(&e, &br, (i32)M));
        probati++;
        multi += br.numerus >= (i32)V ? I : ZEPHYRUM;
        per (i = I; i < br.numerus; i++)
        {
            si (magnus_aequalis(br.normae[i], br.normae[i - I]))
            {
                aequales++;
                frange;
            }
        }
    }
    imprimere("    contra bruta %d, cum normis aequalibus %d, cum >= 5 "
        "vectoribus %d\n", (int)probati, (int)aequales, (int)multi);
    CREDO_MAIOR_I32 (probati, CL);
    CREDO_MAIOR_I32 (aequales, XL);
    CREDO_MAIOR_I32 (multi, XL);
}

/* enumeratio reticulum solum videt, non basim: b et u b (u
 * unimodularis fortuita, elementa magna) idem reddunt */
interior vacuum
_enumeratio_invarians (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
                    i64  semen    = (i64)MMXXXII;
                    i32  exemplum;
                    i32  gradus;
                    i32  i;
                    i32  j;
                    i32  k;
                    i32  probati = ZEPHYRUM;
                 Matrix  b;
                 Matrix  u;
                 Matrix  ub;
                 Magnus  x;
                 Magnus  multiplicator;
      ReticulumQuaestio  q;
    ReticulumEnumeratio  e1;
    ReticulumEnumeratio  e2;

    per (exemplum = ZEPHYRUM; exemplum < XXX; exemplum++)
    {
        i32 n = (i32)(II + _alea(&semen, IV));

        b = _basis_fortuita(&semen, n, n + (i32)_alea(&semen, II), IV,
            FALSUM, piscina);
        si (!reticulum_orthogonaliza(b, piscina, NIHIL, NIHIL))
        {
            perge;
        }
        (vacuum)matrix_identitas(&ANULUS_INTEGRORUM, n, piscina, &u);
        per (gradus = ZEPHYRUM; gradus < XL; gradus++)
        {
            /* linea i += k linea j */
            i = (i32)_alea(&semen, (s64)n);
            j = (i32)_alea(&semen, (s64)n);
            si (i == j)
            {
                perge;
            }
            multiplicator = magnus_ex_s64(_alea(&semen, VII) - III);
            per (k = ZEPHYRUM; k < n; k++)
            {
                x = magnus_adde(_elem(u, i, k), magnus_multiplica(
                    multiplicator, _elem(u, j, k), piscina), piscina);
                matrix_pone(&u, i, k, &x);
            }
        }
        CREDO_NECESSE (matrix_multiplica(u, b, piscina, &ub));
        q = _quaestio(XL + _alea(&semen, XL), (i32)M, ZEPHYRUM);
        CREDO_NECESSE (reticulum_enumera(b, &q, piscina, &e1));
        CREDO_NECESSE (reticulum_enumera(ub, &q, piscina, &e2));
        CREDO_AEQUALIS_S64 ((s64)e1.numerus, (s64)e2.numerus);
        CREDO_VERUM (matrix_aequalis(e1.vectores, e2.vectores));
        per (i = ZEPHYRUM; i < e1.servati; i++)
        {
            CREDO_VERUM (magnus_aequalis(e1.normae[i], e2.normae[i]));
        }
        probati++;
    }
    imprimere("    bases permutatae %d\n", (int)probati);
    CREDO_MAIOR_I32 (probati, XX);
}

/* -1, 0, +1: ordo canonicus lineae i matricis a contra lineam j b */
interior s32
_ordo_canonicus (
                Matrix  a,
       constans Magnus* na,
                   i32  i,
                Matrix  b,
       constans Magnus* nb,
                   i32  j)
{
    s32 c = magnus_compara(na[i], nb[j]);
    i32 k;

    per (k = ZEPHYRUM; c == ZEPHYRUM && k < matrix_columnae(a); k++)
    {
        c = magnus_compara(_elem(a, i, k), _elem(b, j, k));
    }
    redde c;
}

/* servati cursus (etiam incompleti, recensio T5): hic limes servandi >=
 * numerus, ergo omnes; quisque norma vera y^T G y <= R, signo
 * canonico, ordine stricto, et in indice toto (quaestio binaria) */
interior b32
_servati_abrupti (
    constans ReticulumEnumeratio* e,
                          Matrix  forma,
    constans ReticulumEnumeratio* tota,
                             s64  radius,
                         Piscina* piscina)
{
    Magnus q;
       i32 n = matrix_columnae(forma);
       i32 i;
       i32 j;
       i32 k;
       i32 infima;
       i32 suprema;
       i32 media;
       s32 sg;
       s32 c;

    si (e->servati != (i32)e->numerus)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < e->servati; i++)
    {
        q   = magnus_ex_s64(ZEPHYRUM);
        sg  = ZEPHYRUM;
        per (j = ZEPHYRUM; j < n; j++)
        {
            si (sg == ZEPHYRUM)
            {
                sg = magnus_signum(_elem(e->vectores, i, j));
            }
            per (k = ZEPHYRUM; k < n; k++)
            {
                q = magnus_adde(q, magnus_multiplica(magnus_multiplica(
                    _elem(e->vectores, i, j), _elem(e->vectores, i, k),
                    piscina), _elem(forma, j, k), piscina), piscina);
            }
        }
        si (   sg <= ZEPHYRUM || !magnus_aequalis(q, e->normae[i])
            || magnus_compara(q, magnus_ex_s64(radius)) > ZEPHYRUM
            || (i > ZEPHYRUM && _ordo_canonicus(e->vectores, e->normae,
                i - I, e->vectores, e->normae, i) >= ZEPHYRUM))
        {
            redde FALSUM;
        }
        infima   = ZEPHYRUM;
        suprema  = tota->servati;
        c        = I;
        dum (infima < suprema && c != ZEPHYRUM)
        {
            media = infima + (suprema - infima) / II;
            c = _ordo_canonicus(e->vectores, e->normae, i,
                tota->vectores,
                tota->normae, media);
            si (c > ZEPHYRUM)
            {
                infima = media + I;
            }
            alioquin si (c < ZEPHYRUM)
            {
                suprema = media;
            }
        }
        si (c != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* limites: servandi (primi canonice) et nodorum (completa, limes
 * inferior, monotonia) */
interior vacuum
_enumeratio_limitum (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
                 Matrix  e8       = _cartan(VIII, II, piscina);
      ReticulumQuaestio  q;
    ReticulumEnumeratio  tota;
    ReticulumEnumeratio  e;
                    i64  limes;
                    i64  prior = ZEPHYRUM;
                    i32  i;
                    i32  j;
                    b32  completa = FALSUM;

    q = _quaestio(IV, MCC, ZEPHYRUM);
    CREDO_NECESSE (reticulum_formam_enumera(e8, &q, piscina, &tota));
    CREDO_AEQUALIS_I32 (tota.servati, MCC);
    per (i = ZEPHYRUM; i < IV; i++)
    {
        i32 servandi = i == ZEPHYRUM ? ZEPHYRUM : i == I ? I
            : i == II ? III : CXXI;

        q = _quaestio(IV, servandi, ZEPHYRUM);
        CREDO_NECESSE (reticulum_formam_enumera(e8, &q, piscina, &e));
        CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)MCC);
        CREDO_AEQUALIS_I32 (e.servati, servandi);
        per (j = ZEPHYRUM; j < servandi; j++)
        {
            CREDO_VERUM (magnus_aequalis(e.normae[j], tota.normae[j]));
            CREDO_VERUM (_vectores_aequales(e.vectores, j,
                tota.vectores, j));
        }
    }
    q = _quaestio(IV, ZEPHYRUM, I);
    CREDO_NECESSE (reticulum_formam_enumera(e8, &q, piscina, &e));
    CREDO_FALSUM (e.completa);
    per (limes = I; !completa; limes *= II)
    {
        q = _quaestio(IV, MM, limes);
        CREDO_NECESSE (reticulum_formam_enumera(e8, &q, piscina, &e));
        CREDO_VERUM (e.numerus >= prior);
        /* completa FALSUM: nodus non visitatus - numerus iam verus esse
         * potest (recensio T5), ergo <= tantum */
        CREDO_VERUM (e.numerus <= (i64)MCC);
        si (!e.completa)
        {
            CREDO_AEQUALIS_S64 ((s64)e.nodi, (s64)limes);
        }
        CREDO_VERUM (_servati_abrupti(&e, e8, &tota, IV, piscina));
        prior     = e.numerus;
        completa  = e.completa;
    }
    CREDO_AEQUALIS_S64 ((s64)prior, (s64)MCC);
    /* limes exactus: reticulum [1], R = 9 - arbor nodos IV habet
     * (x = 0, 1, 2, 3; folium x = 0 nullum, non numeratur). Limes IV
     * perficit (III vectores), limes III non (x = 3 non visitatur). */
    q = _quaestio(IX, ZEPHYRUM, IV);
    CREDO_NECESSE (reticulum_enumera(_z("[1]", piscina), &q, piscina,
        &e));
    CREDO_VERUM (e.completa);
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)III);
    CREDO_AEQUALIS_S64 ((s64)e.nodi, (s64)IV);
    q = _quaestio(IX, ZEPHYRUM, III);
    CREDO_NECESSE (reticulum_enumera(_z("[1]", piscina), &q, piscina,
        &e));
    CREDO_FALSUM (e.completa);
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)II);
    CREDO_AEQUALIS_S64 ((s64)e.nodi, (s64)III);
}

/* vector brevissimus: contra bruta; deinde limes LLL |b_1|^2 <=
 * alpha^(n-1) lambda_1^2, alpha = 4/(4 delta - 1) = 4q/(4p - q), in
 * integris: (4p - q)^(n-1) |b_1|^2 <= (4q)^(n-1) lambda_1^2 */
interior vacuum
_minimum (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
                Fractio  deltae[III];
                    i64  semen = (i64)MMXXXIII;
                    i32  exemplum;
                    i32  i;
                    i32  probati_bruta = ZEPHYRUM;
                    i32  probati_limes = ZEPHYRUM;
                 Matrix  b;
                 Matrix  g;
                 Matrix  v;
                 Matrix  r;
                 Magnus  norma;
                 Magnus  b1;
                 Magnus  p;
                 Magnus  qq;
                 Magnus  diag;
                  Bruta  br;
                    s64  radius;

    CREDO_NECESSE (reticulum_minimum(_z("[1, 0, 0; 0, 1, 0; 0, 0, 1]",
        piscina), piscina, &v, &norma));
    CREDO_VERUM (magnus_aequalis(norma, magnus_ex_s64(I)));
    CREDO_VERUM (_textus_vectoris(v, ZEPHYRUM, "[0, 0, 1]", piscina));
    CREDO_FALSUM (reticulum_minimum(_z("[]", piscina), piscina, &v,
        &norma));
    deltae[ZEPHYRUM]  = _f("3/4", piscina);
    deltae[I]         = _f("99/100", piscina);
    deltae[II]        = _f("1", piscina);
    per (exemplum = ZEPHYRUM; exemplum < LX; exemplum++)
    {
        i32 n = (i32)(I + _alea(&semen, VI));

        b = _basis_fortuita(&semen, n, n + (i32)_alea(&semen, II),
            exemplum % II == ZEPHYRUM ? (s64)III : (s64)XX, FALSUM,
            piscina);
        si (!reticulum_orthogonaliza(b, piscina, NIHIL, NIHIL))
        {
            perge;
        }
        CREDO_NECESSE (reticulum_minimum(b, piscina, &v, &norma));
        /* contra bruta: radius = diagonalis minima formae datae */
        g     = _forma(b, piscina);
        diag  = _elem(g, ZEPHYRUM, ZEPHYRUM);
        per (i = I; i < n; i++)
        {
            si (magnus_compara(_elem(g, i, i), diag) < ZEPHYRUM)
            {
                diag = _elem(g, i, i);
            }
        }
        si (   magnus_ad_s64(diag, &radius)
            && _bruta(g, b, FALSUM, radius, piscina, &br))
        {
            CREDO_MAIOR_I32 (br.numerus, ZEPHYRUM);
            CREDO_VERUM (magnus_aequalis(norma, br.normae[ZEPHYRUM]));
            per (i = ZEPHYRUM; i < br.columnae; i++)
            {
                CREDO_VERUM (magnus_aequalis(_elem(v, ZEPHYRUM, i),
                    br.vectores[i]));
            }
            probati_bruta++;
        }
        /* limes LLL */
        CREDO_NECESSE (reticulum_reduce(b, &deltae[exemplum % III],
            piscina, &r, NIHIL, NIHIL));
        b1 = magnus_ex_s64(ZEPHYRUM);
        per (i = ZEPHYRUM; i < matrix_columnae(r); i++)
        {
            b1 = magnus_adde(b1, magnus_multiplica(_elem(r, ZEPHYRUM,
                i),
                _elem(r, ZEPHYRUM, i), piscina), piscina);
        }
        p   = fractio_numerator(deltae[exemplum % III]);
        qq  = fractio_denominator(deltae[exemplum % III]);
        CREDO_VERUM (magnus_compara(
            magnus_multiplica(magnus_potentia(magnus_subtrahe(
                magnus_multiplica(magnus_ex_s64(IV), p, piscina), qq,
                piscina), (i32)(n - I), piscina), b1, piscina),
            magnus_multiplica(magnus_potentia(magnus_multiplica(
                magnus_ex_s64(IV), qq, piscina), (i32)(n - I), piscina),
                norma, piscina)) <= ZEPHYRUM);
        /* minimum non maius quam |b_1|^2 */
        CREDO_VERUM (magnus_compara(norma, b1) <= ZEPHYRUM);
        probati_limes++;
    }
    imprimere("    minima contra bruta %d, limes LLL %d\n",
        (int)probati_bruta, (int)probati_limes);
    CREDO_MAIOR_I32 (probati_bruta, XX);
    CREDO_MAIOR_I32 (probati_limes, XL);
}

interior vacuum
_probare_enumerationem_refutationes (
    CredoContextus* c)
{
                Piscina* piscina   = c->piscina;
      ReticulumQuaestio  q         = _quaestio(IV, X, ZEPHYRUM);
      ReticulumQuaestio  negativa  = _quaestio(-(s64)I, X, ZEPHYRUM);
    ReticulumEnumeratio  e;
                 Matrix  v;
                 Magnus  norma;

    e.numerus  = (i64)LXXVII;
    v.anulus   = NIHIL;
    CREDO_FALSUM (reticulum_formam_enumera(_z("[1, 0; 0, 1]", piscina),
        &negativa, piscina, &e));
    CREDO_FALSUM (reticulum_formam_enumera(_z("[1, 0; 0, 1]", piscina),
        NIHIL, piscina, &e));
    CREDO_FALSUM (reticulum_formam_enumera(_z("[1, 0; 0, 1]", piscina),
        &q, piscina, NIHIL));
    CREDO_FALSUM (reticulum_enumera(_z("[1, 2; 2, 4]", piscina), &q,
        piscina, &e));
    CREDO_FALSUM (reticulum_formam_enumera(_z("[1, 2; 2, 1]", piscina),
        &q, piscina, &e));
    CREDO_FALSUM (reticulum_formam_enumera(_z("[2, 1; 0, 2]", piscina),
        &q, piscina, &e));
    CREDO_FALSUM (reticulum_enumera(_matrix(&ANULUS_RATIONALIUM,
        "[1/2, 0; 0, 1]", piscina), &q, piscina, &e));
    CREDO_FALSUM (reticulum_minimum(_z("[1, 2; 2, 4]", piscina),
        piscina,
        &v, &norma));
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)LXXVII);
    CREDO_NIHIL (v.anulus);
}

/* memoria: Z^6 multiplicatum per 10^10 in LX columnis - normae ~10^21
 * extra s64, ergo nodus quisque allocat; vectores lati. Numerus idem ac
 * Z^6 (XI DCCCXCVI). Mensum 2026-10-13: numeratio ~35 KB (sine
 * refectione per gradus ~24 MB); L servati ~78 KB (sine loco expulsi
 * reddito ~286 KB). */
interior vacuum
_enumerationis_memoriam (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
      ReticulumQuaestio  q;
    ReticulumEnumeratio  e;
                 Matrix  z6;
                 Magnus  scala = magnus_potentia(magnus_ex_s64(X), X,
                     piscina);
                 Magnus x;
                    i32 i;

    (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, VI, LX, piscina, &z6);
    per (i = ZEPHYRUM; i < VI; i++)
    {
        x = scala;
        matrix_pone(&z6, i, i, &x);
    }
    q = _quaestio(XVI, I, ZEPHYRUM);
    q.radius_quadratus = magnus_multiplica(magnus_ex_s64(XVI),
        magnus_multiplica(scala, scala, piscina), piscina);
    CREDO_NECESSE (reticulum_enumera(z6, &q, piscina, &e));
    imprimere("    Z^6 x 10^10: numerus %ld, apex %lu\n",
        (long)e.numerus,
        (unsigned long)reticulum_apex_officinarum());
    CREDO_AEQUALIS_S64 ((s64)e.numerus, (s64)XI * (s64)M
        + (s64)DCCCXCVI);
    CREDO_VERUM (reticulum_apex_officinarum()
        < (memoriae_index)1048576);
    q.limes_servandi = L;
    CREDO_NECESSE (reticulum_enumera(z6, &q, piscina, &e));
    imprimere("    Z^6 x 10^10, L servati: apex %lu\n",
        (unsigned long)reticulum_apex_officinarum());
    CREDO_AEQUALIS_I32 (e.servati, L);
    CREDO_VERUM (reticulum_apex_officinarum()
        < (memoriae_index)CLX * (memoriae_index)M);
}


/* ==================================================
 * T4: relationes
 *
 * Iudicia sine oraculo relationum: (1) quaeque linea relatio est
 * (a r^T = 0); (2) numerus linearum = n - gradus(a) (Bareiss); (3)
 * reducta cum delta (iudicium T1); (4) SATURATA: omnis relatio integra
 * inventa (bruta in capsa; coincidentiae summarum 2O) in reticulo
 * linearum est - forma Hermite (r; v) eadem ac r cum linea nulla.
 * ================================================== */

interior b32
_relationes_verae (
     Matrix  a,
     Matrix  r,
    Piscina* piscina)
{
    Matrix rt;
    Matrix p;
       i32 i;
       i32 j;

    si (   !matrix_transposita(r, piscina, &rt)
        || !matrix_multiplica(a, rt, piscina, &p))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < matrix_lineae(p); i++)
    {
        per (j = ZEPHYRUM; j < matrix_columnae(p); j++)
        {
            si (magnus_signum(_elem(p, i, j)) != ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* v (vector longitudinis columnae r) in reticulo linearum r? */
interior b32
_in_reticulo (
              Matrix  r,
     constans Magnus* v,
             Piscina* piscina)
{
    Matrix aucta;
    Matrix hermite_linearum;
    Matrix hermite_auctae;
    Magnus x;
       i32 k = matrix_lineae(r);
       i32 n = matrix_columnae(r);
       i32 i;
       i32 j;

    (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, k + I, n, piscina, &aucta);
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = ZEPHYRUM; j < n; j++)
        {
            x = _elem(r, i, j);
            matrix_pone(&aucta, i, j, &x);
        }
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        x = v[j];
        matrix_pone(&aucta, k, j, &x);
    }
    si (   !matrix_forma_hermite(r, piscina, &hermite_linearum, NIHIL)
        || !matrix_forma_hermite(aucta, piscina, &hermite_auctae,
        NIHIL))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        si (magnus_signum(_elem(hermite_auctae, k, j)) != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = ZEPHYRUM; j < n; j++)
        {
            si (!magnus_aequalis(_elem(hermite_auctae, i, j),
                _elem(hermite_linearum, i, j)))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* iudicia (1)-(3) */
interior vacuum
_relationes_iudica (
               Matrix  a,
               Matrix  r,
    constans Fractio* delta,
             Piscina* piscina)
{
    i32 gradus    = ZEPHYRUM;
    b32 reductum  = FALSUM;

    CREDO_VERUM (_relationes_verae(a, r, piscina));
    CREDO_NECESSE (matrix_gradus(a, piscina, &gradus));
    CREDO_AEQUALIS_I32 (matrix_lineae(r), matrix_columnae(a) - gradus);
    CREDO_AEQUALIS_I32 (matrix_columnae(r), matrix_columnae(a));
    si (matrix_lineae(r) > ZEPHYRUM)
    {
        CREDO_NECESSE (reticulum_est_reductum(r, delta, piscina,
            &reductum));
        CREDO_VERUM (reductum);
    }
}

interior vacuum
_relationes_manu (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
      Matrix  a;
      Matrix  r;
      Matrix  v;
      Magnus  norma;
      Magnus  quaesita[V];

    /* [1, 1]: +-(1, -1) */
    a = _z("[1, 1]", piscina);
    CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
    _relationes_iudica(a, r, NIHIL, piscina);
    CREDO_VERUM (magnus_aequalis(magnus_absolutum(_elem(r, ZEPHYRUM,
        ZEPHYRUM), piscina), magnus_ex_s64(I)));
    /* alpha = sqrt2 + sqrt3, potentiae 0..4 in basi (1, sqrt2, sqrt3,
     * sqrt6): alpha^2 = 5 + 2 sqrt6, alpha^3 = 11 sqrt2 + 9 sqrt3,
     * alpha^4 = 49 + 20 sqrt6; polynomium minimum t^4 - 10 t^2 + 1 */
    a = _z("[1, 0, 5, 0, 49; 0, 1, 0, 11, 0; 0, 1, 0, 9, 0; "
        "0, 0, 2, 0, 20]", piscina);
    CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
    _relationes_iudica(a, r, NIHIL, piscina);
    CREDO_AEQUALIS_I32 (matrix_lineae(r), I);
    CREDO_VERUM (_textus_vectoris(r, ZEPHYRUM, "[1, 0, -10, 0, 1]",
        piscina) || _textus_vectoris(r, ZEPHYRUM, "[-1, 0, 10, 0, -1]",
        piscina));
    /* 1 + sqrt2, potentiae 0..4: relationes t^k (t^2 - 2t - 1),
     * brevissima canonice (0, 0, 1, 2, -1), norma 6 (Python bruta) */
    a = _z("[1, 1, 3, 7, 17; 0, 1, 2, 5, 12]", piscina);
    CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
    _relationes_iudica(a, r, NIHIL, piscina);
    CREDO_NECESSE (reticulum_minimum(r, piscina, &v, &norma));
    CREDO_VERUM (magnus_aequalis(norma, magnus_ex_s64(VI)));
    CREDO_VERUM (_textus_vectoris(v, ZEPHYRUM, "[0, 0, 1, 2, -1]",
        piscina));
    quaesita[ZEPHYRUM]  = magnus_ex_s64(I);
    quaesita[I]         = magnus_ex_s64(II);
    quaesita[II]        = magnus_ex_s64(-(s64)I);
    quaesita[III]       = magnus_ex_s64(ZEPHYRUM);
    quaesita[IV]        = magnus_ex_s64(ZEPHYRUM);
    CREDO_VERUM (_in_reticulo(r, quaesita, piscina));
    /* super Q: x/2 + y/3 = 0 -> +-(2, -3) */
    CREDO_NECESSE (reticulum_relationes(_matrix(&ANULUS_RATIONALIUM,
        "[1/2, 1/3]", piscina), NIHIL, piscina, &r));
    CREDO_AEQUALIS_I32 (matrix_lineae(r), I);
    CREDO_VERUM (_textus_vectoris(r, ZEPHYRUM, "[2, -3]", piscina)
        || _textus_vectoris(r, ZEPHYRUM, "[-2, 3]", piscina));
    CREDO_NECESSE (reticulum_relationes(_matrix(&ANULUS_RATIONALIUM,
        "[1/2, 1/4, 3; 1/6, 0, 1]", piscina), NIHIL, piscina, &r));
    CREDO_VERUM (_relationes_verae(_z("[6, 3, 36; 1, 0, 6]", piscina),
        r,
        piscina));
    CREDO_AEQUALIS_I32 (matrix_lineae(r), I);
    /* nullae lineae: Z^3 totum; gradus plenus: nihil; columnae nullae */
    CREDO_NECESSE (matrix_nulla(&ANULUS_INTEGRORUM, ZEPHYRUM, III,
        piscina, &a));
    CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
    CREDO_AEQUALIS_I32 (matrix_lineae(r), III);
    CREDO_VERUM (_unimodularis(r, piscina));
    a = _z("[1, 0; 0, 1]", piscina);
    CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
    CREDO_AEQUALIS_I32 (matrix_lineae(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (matrix_columnae(r), II);
    CREDO_NECESSE (matrix_nulla(&ANULUS_INTEGRORUM, II, ZEPHYRUM,
        piscina, &a));
    CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
    CREDO_AEQUALIS_I32 (matrix_lineae(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (matrix_columnae(r), ZEPHYRUM);
}

/* 2O: XXIV puncta modulo signo, duplicata, in (Z + Z sqrt2)^4 -> Z^8
 * (coordinata quaternionis m: lineae 2m rationalis, 2m+1 sqrt2):
 * 2e_m (IV); (1, +-1, +-1, +-1) (VIII); sqrt2 (e_a +- e_b) (XII).
 * Nucleus gradus XVI. Coincidentiae: s_a p_a + s_b p_b = +-(s_c p_c +
 * s_d p_d) - relatio quaeque in reticulo esse debet. Python: CCXVI
 * paria (eadem + opposita); divisio inter 'eadem' et 'opposita' ab
 * ordine punctorum pendet (p - q contra q - p), summa non. */
interior vacuum
_relationes_octaedri (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
         s64  puncta[XXIV][VIII];
         s64  summae[DLII][VIII];
         i32  par_a[DLII];
         i32  par_b[DLII];
         s64  sig_b[DLII];
         i32  numerus_summarum  = ZEPHYRUM;
         i32  p                 = ZEPHYRUM;
         i32  i;
         i32  j;
         i32  k;
         i32  m;
         s64  sb;
         s64  sc;
         b32  eadem;
         b32  opposita;
         i32  coincidentiae  = ZEPHYRUM;
         i32  oppositae      = ZEPHYRUM;
         i32  extra          = ZEPHYRUM;
      Matrix  a;
      Matrix  r;
      Magnus  x;
      Magnus  relatio[XXIV];

    per (i = ZEPHYRUM; i < XXIV; i++)
    {
        per (k = ZEPHYRUM; k < VIII; k++)
        {
            puncta[i][k] = ZEPHYRUM;
        }
    }
    per (m = ZEPHYRUM; m < IV; m++)
    {
        puncta[p++][II * m] = II;
    }
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        puncta[p][ZEPHYRUM] = I;
        per (m = I; m < IV; m++)
        {
            puncta[p][II * m] = ((i >> (m - I)) & I) ? -(s64)I : (s64)I;
        }
        p++;
    }
    per (i = ZEPHYRUM; i < IV; i++)
    {
        per (j = i + I; j < IV; j++)
        {
            per (sb = I; sb >= -(s64)I; sb -= II)
            {
                puncta[p][II * i + I] = I;
                puncta[p][II * j + I] = sb;
                p++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (p, XXIV);
    (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, VIII, XXIV, piscina, &a);
    per (i = ZEPHYRUM; i < XXIV; i++)
    {
        per (k = ZEPHYRUM; k < VIII; k++)
        {
            x = magnus_ex_s64(puncta[i][k]);
            matrix_pone(&a, k, i, &x);
        }
    }
    CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
    _relationes_iudica(a, r, NIHIL, piscina);
    CREDO_AEQUALIS_I32 (matrix_lineae(r), XVI);
    /* summae signatae p_a + s p_b, a < b */
    per (i = ZEPHYRUM; i < XXIV; i++)
    {
        per (j = i + I; j < XXIV; j++)
        {
            per (sb = I; sb >= -(s64)I; sb -= II)
            {
                per (k = ZEPHYRUM; k < VIII; k++)
                {
                    summae[numerus_summarum][k] = puncta[i][k]
                        + sb * puncta[j][k];
                }
                par_a[numerus_summarum] = i;
                par_b[numerus_summarum] = j;
                sig_b[numerus_summarum] = sb;
                numerus_summarum++;
            }
        }
    }
    per (i = ZEPHYRUM; i < numerus_summarum; i++)
    {
        per (j = i + I; j < numerus_summarum; j++)
        {
            eadem     = VERUM;
            opposita  = VERUM;
            per (k = ZEPHYRUM; k < VIII; k++)
            {
                eadem     = eadem && summae[i][k] == summae[j][k];
                opposita  = opposita && summae[i][k] == -summae[j][k];
            }
            si (!eadem && !opposita)
            {
                perge;
            }
            /* relatio: p_a + s_b p_b -+ (p_c + s_d p_d) = 0 */
            sc = eadem ? -(s64)I : (s64)I;
            per (k = ZEPHYRUM; k < XXIV; k++)
            {
                relatio[k] = magnus_ex_s64(ZEPHYRUM);
            }
            relatio[par_a[i]] = magnus_adde(relatio[par_a[i]],
                magnus_ex_s64(I), piscina);
            relatio[par_b[i]] = magnus_adde(relatio[par_b[i]],
                magnus_ex_s64(sig_b[i]), piscina);
            relatio[par_a[j]] = magnus_adde(relatio[par_a[j]],
                magnus_ex_s64(sc), piscina);
            relatio[par_b[j]] = magnus_adde(relatio[par_b[j]],
                magnus_ex_s64(sc * sig_b[j]), piscina);
            si (!_in_reticulo(r, relatio, piscina))
            {
                extra++;
            }
            si (eadem)
            {
                coincidentiae++;
            }
            alioquin
            {
                oppositae++;
            }
        }
    }
    imprimere("    2O: coincidentiae %d, oppositae %d, extra reticulum %d\n",
        (int)coincidentiae, (int)oppositae, (int)extra);
    CREDO_AEQUALIS_I32 (coincidentiae + oppositae, CCXVI);
    /* apex = SUMMA (T5): relationes = piscina sua + reductio, ergo maior
     * quam reductio sola eiusdem basis nuclei (maximus aequalis esset) */
    {
                Matrix nucleus;
                Matrix in_lineis;
                Matrix red;
        memoriae_index apex_relationum;

        CREDO_NECESSE (reticulum_relationes(a, NIHIL, piscina, &r));
        apex_relationum = reticulum_apex_officinarum();
        CREDO_NECESSE (matrix_reticulum_nuclei(a, piscina, &nucleus));
        CREDO_NECESSE (matrix_transposita(nucleus, piscina,
            &in_lineis));
        CREDO_NECESSE (reticulum_reduce(in_lineis, NIHIL, piscina, &red,
            NIHIL, NIHIL));
        CREDO_VERUM (matrix_aequalis(r, red));
        CREDO_VERUM (apex_relationum > reticulum_apex_officinarum());
    }
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
}

/* fortuitae: a (k x n, k < n <= 7), delta 3/4, 99/100, 1; saturatio
 * contra brutam in capsa [-2, 2]^n */
interior vacuum
_relationes_fortuitae (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
     Fractio  deltae[III];
         i64  semen = (i64)MMXXXIV;
         i32  exemplum;
         i32  i;
         i32  j;
         i32  k;
         s64  y[VII];
         b32  finis;
         b32  nulla;
         i32  probati  = ZEPHYRUM;
         i32  membra   = ZEPHYRUM;
         i32  extra    = ZEPHYRUM;
      Matrix  a;
      Matrix  r;
      Matrix  nucleus;
      Matrix  in_lineis;
      Matrix  exspectata;
      Matrix  r_quarta;
      Matrix  r_una;
         i32  diversae = ZEPHYRUM;
      Magnus  s;
      Magnus  v[VII];

    deltae[ZEPHYRUM]  = _f("3/4", piscina);
    deltae[I]         = _f("99/100", piscina);
    deltae[II]        = _f("1", piscina);
    per (exemplum = ZEPHYRUM; exemplum < XL; exemplum++)
    {
        i32 n       = (i32)(II + _alea(&semen, VI));
        i32 lineae  = (i32)(I + _alea(&semen, (s64)(n - I)));

        a = _basis_fortuita(&semen, lineae, n, exemplum % II == ZEPHYRUM
            ? (s64)II : (s64)V, FALSUM, piscina);
        CREDO_NECESSE (reticulum_relationes(a, &deltae[exemplum % III],
            piscina, &r));
        _relationes_iudica(a, r, &deltae[exemplum % III], piscina);
        /* compositio: relationes = LLL(delta) basis nuclei; et delta
         * refert - 3/4 et 1 saepe differunt */
        CREDO_NECESSE (matrix_reticulum_nuclei(a, piscina, &nucleus));
        CREDO_NECESSE (matrix_transposita(nucleus, piscina,
            &in_lineis));
        si (matrix_lineae(in_lineis) > ZEPHYRUM)
        {
            CREDO_NECESSE (reticulum_reduce(in_lineis, &deltae[exemplum
                % III], piscina, &exspectata, NIHIL, NIHIL));
            CREDO_VERUM (matrix_aequalis(r, exspectata));
        }
        CREDO_NECESSE (reticulum_relationes(a, &deltae[ZEPHYRUM],
            piscina,
            &r_quarta));
        CREDO_NECESSE (reticulum_relationes(a, &deltae[II], piscina,
            &r_una));
        diversae += matrix_aequalis(r_quarta, r_una) ? ZEPHYRUM : I;
        /* omnes x in [-2, 2]^n cum a x = 0 */
        per (i = ZEPHYRUM; i < n; i++)
        {
            y[i] = -(s64)II;
        }
        finis = FALSUM;
        dum (!finis)
        {
            nulla = VERUM;
            per (k = ZEPHYRUM; nulla && k < lineae; k++)
            {
                s = magnus_ex_s64(ZEPHYRUM);
                per (j = ZEPHYRUM; j < n; j++)
                {
                    s = magnus_adde(s, magnus_multiplica(magnus_ex_s64(
                        y[j]), _elem(a, k, j), piscina), piscina);
                }
                nulla = magnus_signum(s) == ZEPHYRUM;
            }
            si (nulla)
            {
                per (j = ZEPHYRUM; j < n; j++)
                {
                    v[j] = magnus_ex_s64(y[j]);
                }
                si (!_in_reticulo(r, v, piscina))
                {
                    extra++;
                }
                membra++;
            }
            k = ZEPHYRUM;
            dum (k < n && y[k] == (s64)II)
            {
                y[k] = -(s64)II;
                k++;
            }
            si (k == n)
            {
                finis = VERUM;
            }
            alioquin
            {
                y[k]++;
            }
        }
        probati++;
    }
    imprimere("    relationes fortuitae %d, relationes brutae %d, delta "
        "3/4 et 1 diversae %d\n", (int)probati, (int)membra,
        (int)diversae);
    CREDO_MAIOR_I32 (diversae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    CREDO_MAIOR_I32 (membra, CC);
}

interior vacuum
_probare_relationum_refutationes (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
    Fractio  quarta   = _f("1/4", piscina);
     Matrix  r;

    r.anulus = NIHIL;
    CREDO_FALSUM (reticulum_relationes(_z("[1, 1]", piscina), &quarta,
        piscina, &r));
    CREDO_FALSUM (reticulum_relationes(_matrix(&ANULUS_POLYNOMIORUM,
        "[t, 1]", piscina), NIHIL, piscina, &r));
    CREDO_NIHIL (r.anulus);
}


/* ==================================================
 * T5: emendationes recensionis
 * ================================================== */

/* basis alta (plures lineae quam columnae) statim refutatur, sine
 * memoria; apex vocationis refutatae nullus, non prioris (recensio T5:
 * 3000 x 2 ~648 MB ante FALSUM; 65536 x 1 exundatio acervi) */
interior vacuum
_recensio_refutationes (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
      ReticulumQuaestio  q        = _quaestio(IV, X, ZEPHYRUM);
    ReticulumEnumeratio  e;
                 Matrix  alta;
                 Matrix  r;
                 Matrix  u;
                 Magnus  unum = magnus_ex_s64(I);
                 Magnus  norma;
                    b32  iudicium;
                    i32  i;

    /* prius basis alta modica (M x II): si refutatio statim deest,
     * sectio hic abrumpitur - antequam LXX M lineae ~100 GB poscant */
    CREDO_NECESSE (matrix_nulla(&ANULUS_INTEGRORUM, M, II, piscina,
        &alta));
    per (i = ZEPHYRUM; i < M; i++)
    {
        matrix_pone(&alta, i, ZEPHYRUM, &unum);
    }
    CREDO_FALSUM (reticulum_reduce(alta, NIHIL, piscina, &r, &u,
        NIHIL));
    CREDO_NECESSE (reticulum_apex_officinarum() == ZEPHYRUM);
    CREDO_NECESSE (matrix_nulla(&ANULUS_INTEGRORUM, (i32)LXX * (i32)M,
        I,
        piscina, &alta));
    per (i = ZEPHYRUM; i < (i32)LXX * (i32)M; i++)
    {
        matrix_pone(&alta, i, ZEPHYRUM, &unum);
    }
    CREDO_NECESSE (reticulum_formam_enumera(_cartan(VIII, II, piscina),
        &q,
        piscina, &e));
    CREDO_VERUM (reticulum_apex_officinarum() > ZEPHYRUM);
    CREDO_FALSUM (reticulum_reduce(alta, NIHIL, piscina, &r, &u,
        NIHIL));
    CREDO_VERUM (reticulum_apex_officinarum() == ZEPHYRUM);
    CREDO_FALSUM (reticulum_orthogonaliza(alta, piscina, NIHIL, NIHIL));
    CREDO_FALSUM (reticulum_est_reductum(alta, NIHIL, piscina,
        &iudicium));
    CREDO_FALSUM (reticulum_enumera(alta, &q, piscina, &e));
    CREDO_FALSUM (reticulum_minimum(alta, piscina, &r, &norma));
    CREDO_VERUM (reticulum_apex_officinarum() == ZEPHYRUM);
    CREDO_FALSUM (reticulum_reduce(_z("[1, 0, 0; 0, 1, 0; 0, 0, 1; "
        "1, 1, 1]", piscina), NIHIL, piscina, &r, &u, NIHIL));
    /* refutationes aliae: apex nullus, non prioris */
    CREDO_NECESSE (reticulum_formam_enumera(_cartan(VIII, II, piscina),
        &q,
        piscina, &e));
    CREDO_FALSUM (reticulum_reduce(_matrix(&ANULUS_RATIONALIUM,
        "[1/2, 0; 0, 1]", piscina), NIHIL, piscina, &r, &u, NIHIL));
    CREDO_VERUM (reticulum_apex_officinarum() == ZEPHYRUM);
    CREDO_NECESSE (reticulum_formam_enumera(_cartan(VIII, II, piscina),
        &q,
        piscina, &e));
    CREDO_FALSUM (reticulum_minimum(_z("[]", piscina), piscina, &r,
        &norma));
    CREDO_VERUM (reticulum_apex_officinarum() == ZEPHYRUM);
}

/* Gram-Schmidt integra cum piscina temporaria per elementum refecta:
 * n = 40, elementa ~10^27. Mensum 2026-10-13: ~0.47 MB; sine
 * refectione ~28 MB (recensio T5) */
interior vacuum
_recensio_memoria_gs (
    CredoContextus* c)
{
     Piscina* piscina  = c->piscina;
         i64  semen    = (i64)MMXXXV;
      Matrix  b;
         b32  iudicium;

    b = _basis_fortuita(&semen, XL, XL, C, VERUM, piscina);
    CREDO_NECESSE (reticulum_est_reductum(b, NIHIL, piscina,
        &iudicium));
    imprimere("    est_reductum n = 40, 10^27: apex %lu\n",
        (unsigned long)reticulum_apex_officinarum());
    CREDO_VERUM (reticulum_apex_officinarum()
        < (memoriae_index)IV * (memoriae_index)1048576);
}

/* scala (recensio T5: enumeratio cum elementis magnis contra nihil
 * probata): enumeratio(S b, S^2 R) = S enumeratio(b, R), S = 10^20 -
 * F_k, N_k, centra magna, bases non diagonales */
interior vacuum
_recensio_scala (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
                    i64  semen    = (i64)MMXXXVI;
                 Magnus  s = magnus_potentia(magnus_ex_s64(X), XX,
                     piscina);
                 Magnus s2 = magnus_multiplica(s, s, piscina);
                 Magnus x;
                 Matrix b;
                 Matrix sb;
      ReticulumQuaestio q;
      ReticulumQuaestio qs;
    ReticulumEnumeratio e1;
    ReticulumEnumeratio e2;
                    s64 radius;
                    i32 exemplum;
                    i32 i;
                    i32 j;
                    i32 probati   = ZEPHYRUM;
                    i32 vectores  = ZEPHYRUM;
                    b32 bene;

    per (exemplum = ZEPHYRUM; exemplum < XL; exemplum++)
    {
        i32 n = (i32)(I + _alea(&semen, IV));

        b = _basis_fortuita(&semen, n, n + (i32)_alea(&semen, III), III,
            FALSUM, piscina);
        si (!reticulum_orthogonaliza(b, piscina, NIHIL, NIHIL))
        {
            perge;
        }
        (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, n, matrix_columnae(b),
            piscina, &sb);
        per (i = ZEPHYRUM; i < n; i++)
        {
            per (j = ZEPHYRUM; j < matrix_columnae(b); j++)
            {
                x = magnus_multiplica(_elem(b, i, j), s, piscina);
                matrix_pone(&sb, i, j, &x);
            }
        }
        radius  = _alea(&semen, XXI);
        q       = _quaestio(radius, (i32)M, ZEPHYRUM);
        qs      = q;
        qs.radius_quadratus = magnus_multiplica(q.radius_quadratus, s2,
            piscina);
        CREDO_NECESSE (reticulum_enumera(b, &q, piscina, &e1));
        CREDO_NECESSE (reticulum_enumera(sb, &qs, piscina, &e2));
        bene = e1.numerus == e2.numerus && e1.completa && e2.completa
            && e1.servati == e2.servati;
        per (i = ZEPHYRUM; bene && i < e1.servati; i++)
        {
            bene = magnus_aequalis(magnus_multiplica(e1.normae[i], s2,
                piscina), e2.normae[i]);
            per (j = ZEPHYRUM; bene && j < matrix_columnae(b); j++)
            {
                bene = magnus_aequalis(magnus_multiplica(_elem(
                    e1.vectores, i, j), s, piscina), _elem(e2.vectores,
                    i,
                    j));
            }
        }
        CREDO_VERUM (bene);
        probati++;
        vectores += e1.servati;
    }
    imprimere("    scala 10^20: casus %d, vectores %d\n", (int)probati,
        (int)vectores);
    CREDO_MAIOR_I32 (probati, XXX);
    CREDO_MAIOR_I32 (vectores, C);
}

/* relationes super Q, lineae plures, denominatores mixti (recensio T5):
 * contra matricem integram quam probatio ipsa facit (linea per
 * PRODUCTUM denominatorum, non mcm - via alia, nucleus idem) */
interior vacuum
_recensio_relationes_q (
    CredoContextus* c)
{
     Piscina* piscina  = c->piscina;
         i64  semen    = (i64)MMXXXVII;
      Matrix  a_rationalis;
      Matrix  a_integra;
      Matrix  r_rationalis;
      Matrix  r_integra;
     Fractio  f;
      Magnus  productum;
      Magnus  x;
         s64  numeratores[VI];
         s64  denominatores[VI];
         i32  exemplum;
         i32  i;
         i32  j;
         i32  probati = ZEPHYRUM;

    per (exemplum = ZEPHYRUM; exemplum < XXX; exemplum++)
    {
        i32 lineae  = (i32)(I + _alea(&semen, III));
        i32 n       = lineae + (i32)(I + _alea(&semen, III));

        (vacuum)matrix_nulla(&ANULUS_RATIONALIUM, lineae, n, piscina,
            &a_rationalis);
        (vacuum)matrix_nulla(&ANULUS_INTEGRORUM, lineae, n, piscina,
            &a_integra);
        per (i = ZEPHYRUM; i < lineae; i++)
        {
            productum = magnus_ex_s64(I);
            per (j = ZEPHYRUM; j < n; j++)
            {
                numeratores[j]    = _alea(&semen, XI) - V;
                denominatores[j]  = I + _alea(&semen, VI);
                (vacuum)fractio_ex_s64_s64(numeratores[j],
                    denominatores[j], piscina, &f);
                matrix_pone(&a_rationalis, i, j, &f);
                productum = magnus_multiplica(productum, magnus_ex_s64(
                    denominatores[j]), piscina);
            }
            per (j = ZEPHYRUM; j < n; j++)
            {
                (vacuum)magnus_divide(magnus_multiplica(magnus_ex_s64(
                    numeratores[j]), productum, piscina), magnus_ex_s64(
                    denominatores[j]), piscina, &x, NIHIL);
                matrix_pone(&a_integra, i, j, &x);
            }
        }
        CREDO_NECESSE (reticulum_relationes(a_rationalis, NIHIL,
            piscina, &r_rationalis));
        CREDO_NECESSE (reticulum_relationes(a_integra, NIHIL, piscina,
            &r_integra));
        CREDO_VERUM (_relationes_verae(a_integra, r_rationalis,
            piscina));
        CREDO_AEQUALIS_I32 (matrix_lineae(r_rationalis),
            matrix_lineae(r_integra));
        si (matrix_lineae(r_rationalis) > ZEPHYRUM)
        {
            CREDO_VERUM (_idem_reticulum(r_rationalis, r_integra,
                piscina));
        }
        probati++;
    }
    CREDO_AEQUALIS_I32 (probati, XXX);
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

interior vacuum
_probare_enumeratio_manu (
    CredoContextus* c)
{
    _sub_termino(c, _enumeratio_manu);
}

interior vacuum
_probare_enumeratio_radicum (
    CredoContextus* c)
{
    _sub_termino(c, _enumeratio_radicum);
}

interior vacuum
_probare_enumeratio_contra_bruta (
    CredoContextus* c)
{
    _sub_termino(c, _enumeratio_contra_bruta);
}

interior vacuum
_probare_enumeratio_invarians (
    CredoContextus* c)
{
    _sub_termino(c, _enumeratio_invarians);
}

interior vacuum
_probare_enumeratio_limitum (
    CredoContextus* c)
{
    _sub_termino(c, _enumeratio_limitum);
}

interior vacuum
_probare_minimum (
    CredoContextus* c)
{
    _sub_termino(c, _minimum);
}

interior vacuum
_probare_relationes_manu (
    CredoContextus* c)
{
    _sub_termino(c, _relationes_manu);
}

interior vacuum
_probare_relationes_octaedri (
    CredoContextus* c)
{
    _sub_termino(c, _relationes_octaedri);
}

interior vacuum
_probare_relationes_fortuitae (
    CredoContextus* c)
{
    _sub_termino(c, _relationes_fortuitae);
}

interior vacuum
_probare_recensio_refutationes (
    CredoContextus* c)
{
    _sub_termino(c, _recensio_refutationes);
}

interior vacuum
_probare_recensio_memoria_gs (
    CredoContextus* c)
{
    _sub_termino(c, _recensio_memoria_gs);
}

interior vacuum
_probare_recensio_scala (
    CredoContextus* c)
{
    _sub_termino(c, _recensio_scala);
}

interior vacuum
_probare_recensio_relationes_q (
    CredoContextus* c)
{
    _sub_termino(c, _recensio_relationes_q);
}

interior vacuum
_probare_enumerationis_memoriam (
    CredoContextus* c)
{
    _sub_termino(c, _enumerationis_memoriam);
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
    { "enumeratio manu", _probare_enumeratio_manu, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "radices", _probare_enumeratio_radicum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "enumeratio contra bruta", _probare_enumeratio_contra_bruta,
        NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "enumeratio invarians", _probare_enumeratio_invarians, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "limites enumerationis", _probare_enumeratio_limitum, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { "minimum", _probare_minimum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enumeratio refutationes", _probare_enumerationem_refutationes,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "enumeratio memoria", _probare_enumerationis_memoriam, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "relationes manu", _probare_relationes_manu, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "relationes 2O", _probare_relationes_octaedri, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "relationes fortuitae", _probare_relationes_fortuitae, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "relationes refutationes", _probare_relationum_refutationes,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "recensio refutationes", _probare_recensio_refutationes, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "recensio memoria GS", _probare_recensio_memoria_gs, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "recensio scala", _probare_recensio_scala, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "recensio relationes Q", _probare_recensio_relationes_q, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("reticulum", SECTIONES);
}
