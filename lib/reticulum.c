/* reticulum.c - Reticula integra EXACTA (vide include/reticulum.h)
 *
 * Nucleus: Gram-Schmidt INTEGRA (de Weger; Cohen, Algorithmus 2.6.7)
 * ex productis interioribus solis, ergo basis et forma (matrix Gram)
 * eundem cursum habent. d_0 = 1, d_k = determinans Gram primorum k
 * vectorum; integri_kj = d_(j+1) mu_kj pro j < k. Omnes integri,
 * divisiones exactae (residuum non nullum = vitium internum ->
 * FALSUM). d_k > 0 pro omni k sse vectores independentes (basis) aut
 * forma definita positiva (criterium Sylvester).
 *
 * Vide lib/reticulum.worklog.md. */
#include "reticulum.h"
#include "anulus.h"


/* ==================================================
 * Piscinae temporariae
 * ================================================== */

interior memoriae_index _apex_officinarum = ZEPHYRUM;

memoriae_index
reticulum_apex_officinarum (vacuum)
{
    redde _apex_officinarum;
}

/* piscina temporaria vocationis publicae; NIHIL si memoria deest */
interior Piscina*
_officina_aperi (vacuum)
{
    _apex_officinarum = ZEPHYRUM;
    redde piscina_generare_dynamicum("reticulum",
        (memoriae_index)65536);
}

interior vacuum
_officina_claude (
    Piscina* opus)
{
    si (opus != NIHIL)
    {
        si (piscina_summa_apex_usus(opus) > _apex_officinarum)
        {
            _apex_officinarum = piscina_summa_apex_usus(opus);
        }
        piscina_destruere(opus);
    }
}


/* ==================================================
 * Fons productorum interiorum
 * ================================================== */

nomen structura {
    Matrix matrix;
       b32 est_forma;              /* VERUM: matrix Gram; FALSUM: basis */
       i32 n;                      /* numerus vectorum */
} Fons;

interior constans Magnus*
_elementum (
    Matrix m,
       i32 linea,
       i32 columna)
{
    redde (constans Magnus*)matrix_elementum(m, linea, columna);
}

/* basis: lineae super Z; forma: quadrata, symmetrica, super Z */
interior b32
_fons_para (
     Matrix  m,
        b32  est_forma,
       Fons* fons)
{
    i32 i;
    i32 j;

    si (matrix_anulus(m) != &ANULUS_INTEGRORUM)
    {
        redde FALSUM;
    }
    si (est_forma)
    {
        si (matrix_lineae(m) != matrix_columnae(m))
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < matrix_lineae(m); i++)
        {
            per (j = ZEPHYRUM; j < i; j++)
            {
                si (!magnus_aequalis(*_elementum(m, i, j),
                    *_elementum(m, j, i)))
                {
                    redde FALSUM;
                }
            }
        }
    }
    fons->matrix     = m;
    fons->est_forma  = est_forma;
    fons->n          = matrix_lineae(m);
    redde VERUM;
}

/* productum interius b_i . b_j */
interior Magnus
_productum (
     constans Fons* fons,
               i32  i,
               i32  j,
           Piscina* opus)
{
    Magnus summa;
       i32 k;

    si (fons->est_forma)
    {
        redde *_elementum(fons->matrix, i, j);
    }
    summa = magnus_ex_s64(ZEPHYRUM);
    per (k = ZEPHYRUM; k < matrix_columnae(fons->matrix); k++)
    {
        summa = magnus_adde(summa, magnus_multiplica(
            *_elementum(fons->matrix, i, k),
            *_elementum(fons->matrix, j, k), opus), opus);
    }
    redde summa;
}


/* ==================================================
 * Gram-Schmidt integra
 * ================================================== */

nomen structura {
        i32  n;
     Magnus* determinantes;        /* n + 1: d_0 = 1 */
     Magnus* integri;              /* n x n, integri[k * n + j], j < k */
} Orthogonalis;

/* a / b exacte; FALSUM si b nullus aut residuum non nullum */
interior b32
_divide_exacte (
      Magnus  a,
      Magnus  b,
     Piscina* opus,
      Magnus* exitus)
{
    Magnus quotiens;
    Magnus residuum;

    si (   !magnus_divide(a, b, opus, &quotiens, &residuum)
        || magnus_signum(residuum) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *exitus = quotiens;
    redde VERUM;
}

/* FALSUM si d_k <= 0 pro aliquo k (dependentes / non definita) aut
 * divisio non exacta (vitium internum) */
interior b32
_orthogonaliza (
    constans Fons* fons,
          Piscina* opus,
     Orthogonalis* exitus)
{
       i32  n = fons->n;
    Magnus* d;
    Magnus* lam;
       i32  k;
       i32  j;
       i32  i;
    Magnus  u;

    d   = (Magnus*)piscina_allocare(opus,
        (memoriae_index)(n + I) * (memoriae_index)magnitudo(Magnus));
    lam = (Magnus*)piscina_allocare(opus,
        (memoriae_index)n * (memoriae_index)n
        * (memoriae_index)magnitudo(Magnus) + I);
    d[ZEPHYRUM] = magnus_ex_s64(I);
    per (k = ZEPHYRUM; k < n; k++)
    {
        per (j = ZEPHYRUM; j <= k; j++)
        {
            u = _productum(fons, k, j, opus);
            per (i = ZEPHYRUM; i < j; i++)
            {
                si (!_divide_exacte(magnus_subtrahe(
                    magnus_multiplica(d[i + I], u, opus),
                    magnus_multiplica(lam[k * n + i], lam[j * n + i],
                        opus), opus), d[i], opus, &u))
                {
                    redde FALSUM;
                }
            }
            si (j < k)
            {
                lam[k * n + j] = u;
            }
            alioquin
            {
                si (magnus_signum(u) <= ZEPHYRUM)
                {
                    redde FALSUM;
                }
                d[k + I] = u;
            }
        }
    }
    exitus->n              = n;
    exitus->determinantes  = d;
    exitus->integri        = lam;
    redde VERUM;
}

/* coefficientes mu et normae |b*_i|^2 in piscina vocantis */
interior b32
_orthogonaliza_publica (
      Matrix   m,
         b32   est_forma,
     Piscina*  piscina,
      Matrix*  coefficientes,
     Fractio** normae)
{
             Fons  fons;
     Orthogonalis  o;
          Piscina* opus;
           Matrix  mu;
          Fractio* nr = NIHIL;
          Fractio  f;
              i32  k;
              i32  j;
              b32  bene = FALSUM;

    si (!_fons_para(m, est_forma, &fons))
    {
        redde FALSUM;
    }
    opus = _officina_aperi();
    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    si (!_orthogonaliza(&fons, opus, &o))
    {
        _officina_claude(opus);
        redde FALSUM;
    }
    si (coefficientes != NIHIL)
    {
        si (!matrix_nulla(&ANULUS_RATIONALIUM, o.n, o.n, piscina, &mu))
        {
            _officina_claude(opus);
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < o.n; k++)
        {
            per (j = ZEPHYRUM; j < k; j++)
            {
                si (!fractio_ex_magnis(o.integri[k * o.n + j],
                    o.determinantes[j + I], opus, &f))
                {
                    _officina_claude(opus);
                    redde FALSUM;
                }
                f = fractio_transcribe(f, piscina);
                matrix_pone(&mu, k, j, &f);
            }
            f = fractio_ex_s64(I);
            matrix_pone(&mu, k, k, &f);
        }
    }
    si (normae != NIHIL && o.n > ZEPHYRUM)
    {
        nr = (Fractio*)piscina_allocare(piscina,
            (memoriae_index)o.n * (memoriae_index)magnitudo(Fractio));
        per (k = ZEPHYRUM; k < o.n; k++)
        {
            si (!fractio_ex_magnis(o.determinantes[k + I],
                o.determinantes[k], opus, &f))
            {
                _officina_claude(opus);
                redde FALSUM;
            }
            nr[k] = fractio_transcribe(f, piscina);
        }
    }
    bene = VERUM;
    _officina_claude(opus);
    si (coefficientes != NIHIL)
    {
        *coefficientes = mu;
    }
    si (normae != NIHIL)
    {
        *normae = nr;
    }
    redde bene;
}

b32
reticulum_orthogonaliza (
      Matrix   basis,
     Piscina*  piscina,
      Matrix*  coefficientes,
     Fractio** normae)
{
    redde _orthogonaliza_publica(basis, FALSUM, piscina, coefficientes,
        normae);
}

b32
reticulum_formam_orthogonaliza (
      Matrix   forma,
     Piscina*  piscina,
      Matrix*  coefficientes,
     Fractio** normae)
{
    redde _orthogonaliza_publica(forma, VERUM, piscina, coefficientes,
        normae);
}


/* ==================================================
 * Iudicium reductionis
 * ================================================== */

/* delta NIHIL -> 99/100; FALSUM si non 1/4 < delta <= 1 */
interior b32
_delta_para (
    constans Fractio* delta,
             Piscina* opus,
             Fractio* exitus)
{
    Fractio quarta;
    Fractio d;

    si (delta == NIHIL)
    {
        redde fractio_ex_s64_s64(XCIX, C, opus, exitus);
    }
    d = *delta;
    si (!fractio_ex_s64_s64(I, IV, opus, &quarta))
    {
        redde FALSUM;
    }
    si (   fractio_compara(d, quarta, opus) <= ZEPHYRUM
        || fractio_compara(d, fractio_ex_s64(I), opus) > ZEPHYRUM)
    {
        redde FALSUM;
    }
    *exitus = d;
    redde VERUM;
}

/* integre: 2 |integri_kj| <= d_(j+1) (j < k), et
 * q (d_(k+1) d_(k-1) + integri_k,k-1^2) >= p d_k^2 (k >= 1), delta =
 * p/q - forma Lovasz multiplicata per d_k d_(k-1) > 0 */
interior b32
_est_reductum (
    constans Orthogonalis* o,
                  Fractio  delta,
                  Piscina* opus)
{
    Magnus p    = fractio_numerator(delta);
    Magnus q    = fractio_denominator(delta);
    Magnus duo  = magnus_ex_s64(II);
    Magnus lam;
    Magnus sinistra;
    Magnus dextra;
       i32 n = o->n;
       i32 k;
       i32 j;

    per (k = I; k < n; k++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            lam = o->integri[k * n + j];
            si (magnus_compara(magnus_multiplica(duo,
                magnus_absolutum(lam, opus), opus),
                o->determinantes[j + I]) > ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
        lam      = o->integri[k * n + (k - I)];
        sinistra = magnus_multiplica(q, magnus_adde(
            magnus_multiplica(o->determinantes[k + I],
                o->determinantes[k - I], opus),
            magnus_multiplica(lam, lam, opus), opus), opus);
        dextra   = magnus_multiplica(p, magnus_multiplica(
            o->determinantes[k], o->determinantes[k], opus), opus);
        si (magnus_compara(sinistra, dextra) < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_est_reductum_publicum (
              Matrix  m,
                 b32  est_forma,
    constans Fractio* delta,
                 b32* exitus)
{
             Fons  fons;
     Orthogonalis  o;
          Piscina* opus;
          Fractio  dl;
              b32  iudicium;

    si (!_fons_para(m, est_forma, &fons))
    {
        redde FALSUM;
    }
    opus = _officina_aperi();
    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    si (   !_delta_para(delta, opus, &dl)
        || !_orthogonaliza(&fons, opus, &o))
    {
        _officina_claude(opus);
        redde FALSUM;
    }
    iudicium = _est_reductum(&o, dl, opus);
    _officina_claude(opus);
    *exitus = iudicium;
    redde VERUM;
}

b32
reticulum_est_reductum (
              Matrix  basis,
    constans Fractio* delta,
             Piscina* piscina,
                 b32* exitus)
{
    (vacuum)piscina;
    redde _est_reductum_publicum(basis, FALSUM, delta, exitus);
}

b32
reticulum_formam_est_reducta (
              Matrix  forma,
    constans Fractio* delta,
             Piscina* piscina,
                 b32* exitus)
{
    (vacuum)piscina;
    redde _est_reductum_publicum(forma, VERUM, delta, exitus);
}


/* ==================================================
 * Nondum facta (consilium T2-T4): FALSUM
 * ================================================== */

b32
reticulum_reduce (
                 Matrix  basis,
       constans Fractio* delta,
                Piscina* piscina,
                 Matrix* reducta,
                 Matrix* u,
    ReticulumStatistica* statistica)
{
    (vacuum)basis;
    (vacuum)delta;
    (vacuum)piscina;
    (vacuum)reducta;
    (vacuum)u;
    (vacuum)statistica;
    redde FALSUM;
}

b32
reticulum_formam_reduce (
                 Matrix  forma,
       constans Fractio* delta,
                Piscina* piscina,
                 Matrix* forma_reducta,
                 Matrix* u,
    ReticulumStatistica* statistica)
{
    (vacuum)forma;
    (vacuum)delta;
    (vacuum)piscina;
    (vacuum)forma_reducta;
    (vacuum)u;
    (vacuum)statistica;
    redde FALSUM;
}

b32
reticulum_enumera (
                        Matrix  basis,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus)
{
    (vacuum)basis;
    (vacuum)quaestio;
    (vacuum)piscina;
    (vacuum)exitus;
    redde FALSUM;
}

b32
reticulum_formam_enumera (
                        Matrix  forma,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus)
{
    (vacuum)forma;
    (vacuum)quaestio;
    (vacuum)piscina;
    (vacuum)exitus;
    redde FALSUM;
}

b32
reticulum_minimum (
      Matrix  basis,
     Piscina* piscina,
      Matrix* vector,
      Magnus* norma)
{
    (vacuum)basis;
    (vacuum)piscina;
    (vacuum)vector;
    (vacuum)norma;
    redde FALSUM;
}

b32
reticulum_relationes (
              Matrix  a,
    constans Fractio* delta,
             Piscina* piscina,
              Matrix* exitus)
{
    (vacuum)a;
    (vacuum)delta;
    (vacuum)piscina;
    (vacuum)exitus;
    redde FALSUM;
}
