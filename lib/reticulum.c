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
 * Reductio LLL integra (Cohen, Algorithmus 2.6.7)
 *
 * Super formam CURRENTEM g (= u * forma_initialis * u^T) et
 * transformationem u: reductio b_k <- b_k - q b_l est linea et
 * columna k formae minus q lineam et columnam l; permutatio b_k <->
 * b_(k-1) lineas et columnas permutat. Basis: forma = basis basis^T
 * semel, reducta = u * basis in fine. Indices 0-basati: vector k (Cohen
 * k + 1), d[i] = determinans Gram primorum i (ut Cohen),
 * integri[k * n + j] = lambda Cohen (k + 1, j + 1).
 * ================================================== */

nomen structura {
        i32  n;
     Magnus* forma;                /* n x n, symmetrica, currens */
     Magnus* u;                    /* n x n */
     Magnus* d;                    /* n + 1 */
     Magnus* integri;              /* n x n, j < k */
     Magnus  p;                    /* delta = p / q */
     Magnus  q;
} Reductio;

#define IN_FORMA(r, i, j)  ((r)->forma[(i) * (r)->n + (j)])
#define IN_U(r, i, j)  ((r)->u[(i) * (r)->n + (j)])
#define IN_INTEGRIS(r, k, j)  ((r)->integri[(k) * (r)->n + (j)])

interior Magnus*
_magni (
         i32  numerus,
     Piscina* opus)
{
    redde (Magnus*)piscina_allocare(opus,
        (memoriae_index)numerus * (memoriae_index)magnitudo(Magnus)
        + (memoriae_index)I);
}

/* status in piscinam novam transcribere (valores vivi soli) */
interior vacuum
_reductio_transcribe (
    Reductio* r,
     Piscina* nova)
{
    Magnus* forma    = _magni(r->n * r->n, nova);
    Magnus* u        = _magni(r->n * r->n, nova);
    Magnus* d        = _magni(r->n + I, nova);
    Magnus* integri  = _magni(r->n * r->n, nova);
       i32  i;
       i32  j;

    per (i = ZEPHYRUM; i < r->n; i++)
    {
        per (j = ZEPHYRUM; j < r->n; j++)
        {
            forma[i * r->n + j] = magnus_transcribe(IN_FORMA(r, i, j),
                nova);
            u[i * r->n + j] = magnus_transcribe(IN_U(r, i, j),
                nova);
            si (j < i)
            {
                integri[i * r->n + j] = magnus_transcribe(IN_INTEGRIS(r,
                    i, j),
                    nova);
            }
        }
    }
    per (i = ZEPHYRUM; i <= r->n; i++)
    {
        d[i] = magnus_transcribe(r->d[i], nova);
    }
    r->forma    = forma;
    r->u        = u;
    r->d        = d;
    r->integri  = integri;
    r->p        = magnus_transcribe(r->p, nova);
    r->q        = magnus_transcribe(r->q, nova);
}

/* Gram-Schmidt incrementalis vectoris k; FALSUM si d_(k+1) <= 0 */
interior b32
_reductio_orthogonaliza (
    Reductio* r,
         i32  k,
     Piscina* opus)
{
    Magnus u;
       i32 i;
       i32 j;

    per (j = ZEPHYRUM; j <= k; j++)
    {
        u = IN_FORMA(r, k, j);
        per (i = ZEPHYRUM; i < j; i++)
        {
            si (!_divide_exacte(magnus_subtrahe(
                magnus_multiplica(r->d[i + I], u, opus),
                magnus_multiplica(IN_INTEGRIS(r, k, i), IN_INTEGRIS(r,
                j, i), opus), opus),
                r->d[i], opus, &u))
            {
                redde FALSUM;
            }
        }
        si (j < k)
        {
            IN_INTEGRIS(r, k, j) = u;
        }
        alioquin
        {
            si (magnus_signum(u) <= ZEPHYRUM)
            {
                redde FALSUM;
            }
            r->d[k + I] = u;
        }
    }
    redde VERUM;
}

/* RED(k, l): b_k <- b_k - q b_l, q integer proximus lambda_kl /
 * d_(l+1); nihil si 2 |lambda_kl| <= d_(l+1). VERUM si mutatum. */
interior b32
_reductio_red (
    Reductio* r,
         i32  k,
         i32  l,
     Piscina* opus)
{
    Magnus duo  = magnus_ex_s64(II);
    Magnus dl   = r->d[l + I];
    Magnus lam  = IN_INTEGRIS(r, k, l);
    Magnus q;
    Magnus diagonalis_nova;
    Magnus v;
       i32 j;

    si (magnus_compara(magnus_multiplica(duo, magnus_absolutum(lam,
        opus),
        opus), dl) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* q = pavimentum((2 lambda + d) / (2 d)), d > 0 */
    (vacuum)magnus_divide(magnus_adde(magnus_multiplica(duo, lam, opus),
        dl, opus), magnus_multiplica(duo, dl, opus), opus, &q, NIHIL);
    /* forma: G_kk ex valoribus veteribus */
    diagonalis_nova = magnus_adde(magnus_subtrahe(IN_FORMA(r, k, k),
        magnus_multiplica(duo,
        magnus_multiplica(q, IN_FORMA(r, k, l), opus), opus), opus),
        magnus_multiplica(magnus_multiplica(q, q, opus), IN_FORMA(r, l,
        l),
            opus), opus);
    per (j = ZEPHYRUM; j < r->n; j++)
    {
        si (j != k)
        {
            v = magnus_subtrahe(IN_FORMA(r, k, j), magnus_multiplica(q,
                IN_FORMA(r, l, j), opus), opus);
            IN_FORMA(r, k, j) = v;
            IN_FORMA(r, j, k) = v;
        }
        IN_U(r, k, j) = magnus_subtrahe(IN_U(r, k, j),
            magnus_multiplica(q,
            IN_U(r, l, j), opus), opus);
    }
    IN_FORMA(r, k, k) = diagonalis_nova;
    IN_INTEGRIS(r, k, l) = magnus_subtrahe(lam, magnus_multiplica(q, dl,
        opus),
        opus);
    per (j = ZEPHYRUM; j < l; j++)
    {
        IN_INTEGRIS(r, k, j) = magnus_subtrahe(IN_INTEGRIS(r, k, j),
            magnus_multiplica(q,
            IN_INTEGRIS(r, l, j), opus), opus);
    }
    redde VERUM;
}

/* SWAP(k): b_k <-> b_(k-1), k >= 1; k_max = index maximus orthogonalis.
 * FALSUM si divisio non exacta (vitium internum). */
interior b32
_reductio_permuta (
    Reductio* r,
         i32  k,
         i32  k_max,
     Piscina* opus)
{
    Magnus t;
    Magnus lam;
    Magnus b;
       i32 i;
       i32 j;

    per (j = ZEPHYRUM; j < r->n; j++)
    {
        t         = IN_U(r, k, j);
        IN_U(r, k, j) = IN_U(r, k - I, j);
        IN_U(r, k - I, j) = t;
        t         = IN_FORMA(r, k, j);
        IN_FORMA(r, k, j) = IN_FORMA(r, k - I, j);
        IN_FORMA(r, k - I, j) = t;
    }
    per (i = ZEPHYRUM; i < r->n; i++)
    {
        t         = IN_FORMA(r, i, k);
        IN_FORMA(r, i, k) = IN_FORMA(r, i, k - I);
        IN_FORMA(r, i, k - I) = t;
    }
    per (j = ZEPHYRUM; j < k - I; j++)
    {
        t            = IN_INTEGRIS(r, k, j);
        IN_INTEGRIS(r, k, j) = IN_INTEGRIS(r, k - I, j);
        IN_INTEGRIS(r, k - I, j) = t;
    }
    lam = IN_INTEGRIS(r, k, k - I);
    si (!_divide_exacte(magnus_adde(magnus_multiplica(r->d[k - I],
        r->d[k + I], opus), magnus_multiplica(lam, lam, opus), opus),
        r->d[k], opus, &b))
    {
        redde FALSUM;
    }
    per (i = k + I; i <= k_max; i++)
    {
        t = IN_INTEGRIS(r, i, k);
        si (!_divide_exacte(magnus_subtrahe(magnus_multiplica(
            r->d[k + I], IN_INTEGRIS(r, i, k - I), opus),
            magnus_multiplica(lam, t,
            opus), opus), r->d[k], opus, &IN_INTEGRIS(r, i, k)))
        {
            redde FALSUM;
        }
        si (!_divide_exacte(magnus_adde(magnus_multiplica(b, t, opus),
            magnus_multiplica(lam, IN_INTEGRIS(r, i, k), opus), opus),
            r->d[k + I],
            opus, &IN_INTEGRIS(r, i, k - I)))
        {
            redde FALSUM;
        }
    }
    r->d[k] = b;
    redde VERUM;
}

/* Lovasz fracta: q (d_(k+1) d_(k-1) + lambda^2) < p d_k^2 */
interior b32
_reductio_lovasz_fracta (
    constans Reductio* r,
                  i32  k,
             Piscina* opus)
{
    Magnus lam = IN_INTEGRIS(r, k, k - I);

    redde magnus_compara(magnus_multiplica(r->q, magnus_adde(
        magnus_multiplica(r->d[k + I], r->d[k - I], opus),
        magnus_multiplica(lam, lam, opus), opus), opus),
        magnus_multiplica(r->p, magnus_multiplica(r->d[k], r->d[k],
            opus), opus)) < ZEPHYRUM;
}

/* LLL super r (forma et u iam posita). *opus mutari potest
 * (compactio): vocans piscinam currentem destruit. */
interior b32
_reductio_curre (
               Reductio* r,
               Piscina** opus,
    ReticulumStatistica* statistica)
{
               i32  k      = I;
               i32  k_max  = ZEPHYRUM;
               s32  l;
    memoriae_index  limes  = (memoriae_index)1048576;
           Piscina* nova;

    statistica->permutationes  = ZEPHYRUM;
    statistica->reductiones    = ZEPHYRUM;
    r->d[ZEPHYRUM]             = magnus_ex_s64(I);
    si (r->n == ZEPHYRUM)
    {
        redde VERUM;
    }
    r->d[I] = IN_FORMA(r, ZEPHYRUM, ZEPHYRUM);
    si (magnus_signum(r->d[I]) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (k < r->n)
    {
        si (k > k_max)
        {
            k_max = k;
            si (!_reductio_orthogonaliza(r, k, *opus))
            {
                redde FALSUM;
            }
        }
        si (_reductio_red(r, k, k - I, *opus))
        {
            statistica->reductiones++;
        }
        si (_reductio_lovasz_fracta(r, k, *opus))
        {
            si (!_reductio_permuta(r, k, k_max, *opus))
            {
                redde FALSUM;
            }
            statistica->permutationes++;
            si (k > I)
            {
                k--;
            }
        }
        alioquin
        {
            per (l = (s32)k - (s32)II; l >= (s32)ZEPHYRUM; l--)
            {
                si (_reductio_red(r, k, (i32)l, *opus))
                {
                    statistica->reductiones++;
                }
            }
            k++;
        }
        /* compactio: valores vivi in piscinam novam */
        si (piscina_summa_usus(*opus) > limes)
        {
            nova = piscina_generare_dynamicum("reticulum",
                (memoriae_index)65536);
            si (nova == NIHIL)
            {
                redde FALSUM;
            }
            _reductio_transcribe(r, nova);
            _officina_claude(*opus);
            *opus = nova;
            si (II * piscina_summa_usus(nova) > limes)
            {
                limes = II * piscina_summa_usus(nova);
            }
        }
    }
    redde VERUM;
}

/* status initialis: forma (basis basis^T aut data), u = identitas */
interior b32
_reductio_para (
    constans Fons* fons,
          Fractio  delta,
          Piscina* opus,
         Reductio* r)
{
    i32 n = fons->n;
    i32 i;
    i32 j;

    r->n        = n;
    r->forma    = _magni(n * n, opus);
    r->u        = _magni(n * n, opus);
    r->d        = _magni(n + I, opus);
    r->integri  = _magni(n * n, opus);
    r->p        = fractio_numerator(delta);
    r->q        = fractio_denominator(delta);
    /* omnes d positi: compactio omnes transcribit, etiam d_k vectorum
     * nondum orthogonalium */
    per (i = ZEPHYRUM; i <= n; i++)
    {
        r->d[i] = magnus_ex_s64(ZEPHYRUM);
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = ZEPHYRUM; j <= i; j++)
        {
            IN_FORMA(r, i, j) = _productum(fons, i, j, opus);
            IN_FORMA(r, j, i) = IN_FORMA(r, i, j);
        }
        per (j = ZEPHYRUM; j < n; j++)
        {
            IN_U(r, i, j)= magnus_ex_s64(i == j ? I : ZEPHYRUM);
            IN_INTEGRIS(r, i, j) = magnus_ex_s64(ZEPHYRUM);
        }
    }
    redde VERUM;
}

/* matrix super Z ex tabula n x n, in piscina */
interior b32
_matrix_ex_magnis (
    constans Magnus* tabula,
                i32  n,
            Piscina* piscina,
             Matrix* exitus)
{
    Magnus x;
       i32 i;
       i32 j;

    si (!matrix_nulla(&ANULUS_INTEGRORUM, n, n, piscina, exitus))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = ZEPHYRUM; j < n; j++)
        {
            x = magnus_transcribe(tabula[i * n + j], piscina);
            matrix_pone(exitus, i, j, &x);
        }
    }
    redde VERUM;
}

interior b32
_reduce_publicum (
                 Matrix  m,
                    b32  est_forma,
       constans Fractio* delta,
                Piscina* piscina,
                 Matrix* reducta,
                 Matrix* u,
    ReticulumStatistica* statistica)
{
                   Fons  fons;
               Reductio  r;
                Piscina* opus;
                Fractio  dl;
    ReticulumStatistica  stat;
                 Matrix  u_opus;
                 Matrix  red;
                 Matrix  u_exitus;

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
        || !_reductio_para(&fons, dl, opus, &r)
        || !_reductio_curre(&r, &opus, &stat))
    {
        _officina_claude(opus);
        redde FALSUM;
    }
    si (est_forma)
    {
        si (   !_matrix_ex_magnis(r.forma, r.n, piscina, &red)
            || (u != NIHIL && !_matrix_ex_magnis(r.u, r.n, piscina,
                &u_exitus)))
        {
            _officina_claude(opus);
            redde FALSUM;
        }
    }
    alioquin
    {
        si (   !_matrix_ex_magnis(r.u, r.n, opus, &u_opus)
            || !matrix_multiplica(u_opus, m, piscina, &red)
            || (u != NIHIL && !_matrix_ex_magnis(r.u, r.n, piscina,
                &u_exitus)))
        {
            _officina_claude(opus);
            redde FALSUM;
        }
    }
    _officina_claude(opus);
    *reducta = red;
    si (u != NIHIL)
    {
        *u = u_exitus;
    }
    si (statistica != NIHIL)
    {
        *statistica = stat;
    }
    redde VERUM;
}

b32
reticulum_reduce (
                 Matrix  basis,
       constans Fractio* delta,
                Piscina* piscina,
                 Matrix* reducta,
                 Matrix* u,
    ReticulumStatistica* statistica)
{
    redde _reduce_publicum(basis, FALSUM, delta, piscina, reducta, u,
        statistica);
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
    redde _reduce_publicum(forma, VERUM, delta, piscina, forma_reducta,
        u,
        statistica);
}


/* ==================================================
 * Nondum facta (consilium T3-T4): FALSUM
 * ================================================== */

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
