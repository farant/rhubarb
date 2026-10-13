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

/* apex: SUMMA apicum omnium piscinarum temporariarum vocationis -
 * limes superior usus simul vivi, numquam minor (recensio T5: maximus
 * singularum piscinarum simul vivarum minor erat). Piscinae successivae
 * compactionis LLL: maximus veterum BIS additur, nam transcriptio
 * veterem et novam simul tenet. Vocatio publica quaeque a nihilo
 * incipit, etiam si statim refutat. */
interior memoriae_index _apex_officinarum = ZEPHYRUM;

memoriae_index
reticulum_apex_officinarum (vacuum)
{
    redde _apex_officinarum;
}

interior vacuum
_apex_incipe (vacuum)
{
    _apex_officinarum = ZEPHYRUM;
}

/* piscina temporaria; NIHIL si memoria deest */
interior Piscina*
_officina_aperi (vacuum)
{
    redde piscina_generare_dynamicum("reticulum",
        (memoriae_index)65536);
}

/* apicem piscinae summae addit, deinde destruit */
interior vacuum
_officina_claude (
    Piscina* opus)
{
    si (opus != NIHIL)
    {
        _apex_officinarum += piscina_summa_apex_usus(opus);
        piscina_destruere(opus);
    }
}

/* numerus in memoriae_index: n * n in i32 (insignatus XXXII bitorum)
 * ad n = 65536 in nihilum volvitur (recensio T5) */
interior Magnus*
_magni (
    memoriae_index  numerus,
           Piscina* opus)
{
    redde (Magnus*)piscina_allocare(opus,
        numerus * (memoriae_index)magnitudo(Magnus)
        + (memoriae_index)I);
}

interior memoriae_index
_quadratum (
    i32 n)
{
    redde (memoriae_index)n * (memoriae_index)n;
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
    /* plures vectores quam dimensiones: semper dependentes - refutatio
     * statim, ante matricem Gram (recensio T5: 3000 x 2 ~648 MB) */
    si (!est_forma && matrix_lineae(m) > matrix_columnae(m))
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
 * divisio non exacta (vitium internum). d et integri (soli valores
 * vivi) in opus; temporaria cuiusque elementi in piscina propria, post
 * elementum refecta (recensio T5: sine refectione n = 80, 10^25 ~424
 * MB, ~n^4). */
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
         Piscina* temporaria;
  PiscinaNotatio  nota;

    temporaria = _officina_aperi();
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    nota         = piscina_notare(temporaria);
    d            = _magni((memoriae_index)n + I, opus);
    lam          = _magni(_quadratum(n), opus);
    d[ZEPHYRUM]  = magnus_ex_s64(I);
    per (k = ZEPHYRUM; k < n; k++)
    {
        per (j = ZEPHYRUM; j <= k; j++)
        {
            piscina_reficere(temporaria, nota);
            u = _productum(fons, k, j, temporaria);
            per (i = ZEPHYRUM; i < j; i++)
            {
                si (!_divide_exacte(magnus_subtrahe(
                    magnus_multiplica(d[i + I], u, temporaria),
                    magnus_multiplica(lam[k * n + i], lam[j * n + i],
                        temporaria), temporaria), d[i], temporaria, &u))
                {
                    _officina_claude(temporaria);
                    redde FALSUM;
                }
            }
            u = magnus_transcribe(u, opus);
            si (j < k)
            {
                lam[k * n + j] = u;
            }
            alioquin
            {
                si (magnus_signum(u) <= ZEPHYRUM)
                {
                    _officina_claude(temporaria);
                    redde FALSUM;
                }
                d[k + I] = u;
            }
        }
    }
    _officina_claude(temporaria);
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
    _apex_incipe();
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
    _apex_incipe();
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
    _apex_incipe();
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
    _apex_incipe();
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

/* status in piscinam novam transcribere (valores vivi soli) */
interior vacuum
_reductio_transcribe (
    Reductio* r,
     Piscina* nova)
{
    Magnus* forma    = _magni(_quadratum(r->n), nova);
    Magnus* u        = _magni(_quadratum(r->n), nova);
    Magnus* d        = _magni((memoriae_index)r->n + I, nova);
    Magnus* integri  = _magni(_quadratum(r->n), nova);
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
 * (compactio): vocans piscinam currentem destruit. *apex_veterum: apex
 * maximus piscinarum per compactionem destructarum (veterum). */
interior b32
_reductio_curre_nucleus (
               Reductio* r,
               Piscina** opus,
    ReticulumStatistica* statistica,
         memoriae_index* apex_veterum)
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
            si (piscina_summa_apex_usus(*opus) > *apex_veterum)
            {
                *apex_veterum = piscina_summa_apex_usus(*opus);
            }
            piscina_destruere(*opus);
            *opus = nova;
            si (II * piscina_summa_usus(nova) > limes)
            {
                limes = II * piscina_summa_usus(nova);
            }
        }
    }
    redde VERUM;
}

interior b32
_reductio_curre (
               Reductio*  r,
                Piscina** opus,
    ReticulumStatistica*  statistica)
{
    memoriae_index apex_veterum = ZEPHYRUM;
               b32 bene;

    bene = _reductio_curre_nucleus(r, opus, statistica, &apex_veterum);
    _apex_officinarum += II * apex_veterum;
    redde bene;
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
    r->forma    = _magni(_quadratum(n), opus);
    r->u        = _magni(_quadratum(n), opus);
    r->d        = _magni((memoriae_index)n + I, opus);
    r->integri  = _magni(_quadratum(n), opus);
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
    _apex_incipe();
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
    _apex_incipe();
    redde _reduce_publicum(forma, VERUM, delta, piscina, forma_reducta,
        u,
        statistica);
}


/* ==================================================
 * Enumeratio (Fincke-Pohst integra)
 *
 * Super formam LLL-reductam (delta 99/100) G' = u G u^T; x coefficientes
 * in basi reducta. Cum d et integri Gram-Schmidt integrae formae G',
 * Q(x) = summa_k t_k^2 / (d_k d_(k+1)), t_k = d_(k+1) x_k + s_k,
 * s_k = summa_(j > k) integri_jk x_j. Sine fractionibus: F_n = 1,
 * F_k = F_(k+1) d_k d_(k+1); N_n = R, N_k = N_(k+1) d_k d_(k+1) -
 * t_k^2 F_(k+1) = (R - summa_(i >= k) Q_i) F_k. x_k admissibilis sse
 * N_k >= 0; in folio Q(x) = R - N_0 / F_0 (divisio exacta - aliter
 * vitium internum). Intervallum admissibile x_k centrum -s_k / d_(k+1)
 * habet: si integer proximus centro non admittitur, nullus.
 *
 * Signum: dum omnes x_j (j > k) nulli sunt, x_k >= 0 tantum - ergo par
 * {v, -v} semel numeratur; vector nullus excluditur.
 *
 * Memoria: gradus quisque piscinam notat et post quemque x_k reficit;
 * x_k ipse ANTE notam allocatur (aliter refectio eum deleret). Servata
 * in piscina propria (repositorium); locus vectoris expulsi vectori
 * novo redit, ergo solum membra magnorum (extra s64) quae expelluntur
 * relinquuntur. Insertiones ~2.5-3 x limes (Python, Z^2 et Z^3).
 * ================================================== */

nomen structura {
     Magnus  norma;
     Magnus* vector;               /* 'columnae' elementa */
} Servatum;

nomen structura {
          i32  n;
       Magnus* d;                  /* n + 1, formae reductae */
       Magnus* integri;            /* n x n */
       Magnus* f;                  /* n + 1: F_k */
       Magnus  radius;
       Magnus* x;                  /* n, in basi reducta */
       Magnus* transitus;          /* n x columnae: vector = x transitus */
          i32  columnae;
          i64  numerus;
          i64  nodi;
          i64  limes_nodorum;      /* 0 = sine limite */
          b32  abruptum;
          b32  vitium;
          i32  limes_servandi;
          i32  servati;
          i32  capacitas;
     Servatum* servata;            /* in repositorio */
      Piscina* repositorium;
      Piscina* opus;
} Enumeratio;

/* -1, 0, +1: ordo canonicus (norma, deinde lexicographice) */
interior s32
_servatum_compara (
     constans Enumeratio* e,
                  Magnus  norma,
         constans Magnus* vector,
       constans Servatum* s)
{
    s32 c = magnus_compara(norma, s->norma);
    i32 j;

    per (j = ZEPHYRUM; c == ZEPHYRUM && j < e->columnae; j++)
    {
        c = magnus_compara(vector[j], s->vector[j]);
    }
    redde c;
}

interior Servatum*
_servata_nova (
         i32  capacitas,
     Piscina* piscina)
{
    redde (Servatum*)piscina_allocare(piscina,
        (memoriae_index)capacitas * (memoriae_index)magnitudo(Servatum)
        + (memoriae_index)I);
}

/* vectorem in ordinem canonicum inserere, si inter primos limes */
interior vacuum
_serva (
         Enumeratio* e,
             Magnus  norma,
    constans Magnus* vector)
{
     Servatum* s;
       Magnus* locus = NIHIL;
          i32  infima;
          i32  suprema;
          i32  media;
          i32  i;

    /* locus: primus index cuius servatum novo maius est */
    infima   = ZEPHYRUM;
    suprema  = e->servati;
    dum (infima < suprema)
    {
        media = infima + (suprema - infima) / II;
        si (_servatum_compara(e, norma, vector, &e->servata[media])
            > ZEPHYRUM)
        {
            infima = media + I;
        }
        alioquin
        {
            suprema = media;
        }
    }
    si (infima >= e->limes_servandi)
    {
        redde;
    }
    si (e->servati == e->capacitas)
    {
        /* capacitas duplicatur usque ad limitem */
        i32 nova_capacitas = e->capacitas > e->limes_servandi / II
            ? e->limes_servandi : e->capacitas * II;

        si (nova_capacitas > e->capacitas)
        {
            s = _servata_nova(nova_capacitas, e->repositorium);
            per (i = ZEPHYRUM; i < e->servati; i++)
            {
                s[i] = e->servata[i];
            }
            e->servata    = s;
            e->capacitas  = nova_capacitas;
        }
    }
    si (e->servati == e->capacitas)
    {
        /* ultimus excidit; locus eius vectori novo redit */
        locus = e->servata[e->servati - I].vector;
        e->servati--;
    }
    per (i = e->servati; i > infima; i--)
    {
        e->servata[i] = e->servata[i - I];
    }
    si (locus == NIHIL)
    {
        locus = _magni(e->columnae, e->repositorium);
    }
    per (i = ZEPHYRUM; i < e->columnae; i++)
    {
        locus[i] = magnus_transcribe(vector[i], e->repositorium);
    }
    e->servata[infima].norma = magnus_transcribe(norma,
        e->repositorium);
    e->servata[infima].vector = locus;
    e->servati++;
}

/* folium: x != 0 cum Q(x) <= R; N = N_0 */
interior vacuum
_folium (
    Enumeratio* e,
        Magnus  n_infimus)
{
    Magnus  q;
    Magnus  norma;
    Magnus* vector;
       i32  i;
       i32  j;
       b32  negandum = FALSUM;
       s32  sg;

    si (!_divide_exacte(n_infimus, e->f[ZEPHYRUM], e->opus, &q))
    {
        e->vitium = VERUM;
        redde;
    }
    norma = magnus_subtrahe(e->radius, q, e->opus);
    e->numerus++;
    si (e->limes_servandi == ZEPHYRUM)
    {
        redde;
    }
    si (   e->servati == e->limes_servandi
        && magnus_compara(norma,
            e->servata[e->servati - I].norma) > ZEPHYRUM)
    {
        redde;
    }
    vector = _magni(e->columnae, e->opus);
    per (j = ZEPHYRUM; j < e->columnae; j++)
    {
        vector[j] = magnus_ex_s64(ZEPHYRUM);
        per (i = ZEPHYRUM; i < e->n; i++)
        {
            vector[j] = magnus_adde(vector[j],
                magnus_multiplica(e->x[i],
                e->transitus[i * e->columnae + j], e->opus), e->opus);
        }
    }
    /* signum canonicum: coordinata prima non nulla positiva */
    per (j = ZEPHYRUM; j < e->columnae; j++)
    {
        sg = magnus_signum(vector[j]);
        si (sg != ZEPHYRUM)
        {
            negandum = sg < ZEPHYRUM;
            frange;
        }
    }
    si (negandum)
    {
        per (j = ZEPHYRUM; j < e->columnae; j++)
        {
            vector[j] = magnus_nega(vector[j], e->opus);
        }
    }
    _serva(e, norma, vector);
}

/* gradus k (n - 1 ... 0); n_superior = N_(k+1); nulli: x_j = 0 pro
 * omni j > k */
interior vacuum
_gradus (
    Enumeratio* e,
           s32  k,
        Magnus  n_superior,
           b32  nulli)
{
             Magnus  s;
             Magnus  centrum;
             Magnus  x;
             Magnus  t;
             Magnus  n_k;
             Magnus  d_sequens;
     PiscinaNotatio  nota;
                s32  directio;
                i32  j;
            Piscina* opus = e->opus;

    si (k < (s32)ZEPHYRUM)
    {
        si (!nulli)
        {
            _folium(e, n_superior);
        }
        redde;
    }
    d_sequens  = e->d[k + I];
    s          = magnus_ex_s64(ZEPHYRUM);
    per (j = (i32)k + I; j < e->n; j++)
    {
        s = magnus_adde(s, magnus_multiplica(e->integri[j * e->n
            + (i32)k],
            e->x[j], opus), opus);
    }
    /* integer proximus -s / d_(k+1): pavimentum((d - 2 s) / (2 d)) */
    centrum = magnus_ex_s64(ZEPHYRUM);
    si (!nulli)
    {
        (vacuum)magnus_divide(magnus_subtrahe(d_sequens,
            magnus_multiplica(
            magnus_ex_s64(II), s, opus), opus), magnus_multiplica(
            magnus_ex_s64(II), d_sequens, opus), opus, &centrum, NIHIL);
    }
    per (directio = I; directio >= -(s32)I; directio -= II)
    {
        x = directio > ZEPHYRUM ? centrum : magnus_subtrahe(centrum,
            magnus_ex_s64(I), opus);
        dum (VERUM)
        {
            nota = piscina_notare(opus);
            t = magnus_adde(magnus_multiplica(d_sequens, x, opus), s,
                opus);
            n_k  = magnus_subtrahe(magnus_multiplica(n_superior,
                magnus_multiplica(e->d[k], d_sequens, opus), opus),
                magnus_multiplica(magnus_multiplica(t, t, opus),
                e->f[k + I], opus), opus);
            si (magnus_signum(n_k) < ZEPHYRUM)
            {
                piscina_reficere(opus, nota);
                frange;
            }
            si (   e->limes_nodorum != ZEPHYRUM
                && e->nodi          == e->limes_nodorum)
            {
                piscina_reficere(opus, nota);
                e->abruptum = VERUM;
                redde;
            }
            e->nodi++;
            e->x[k] = x;
            _gradus(e, k - (s32)I, n_k, nulli && magnus_signum(x)
                == ZEPHYRUM);
            piscina_reficere(opus, nota);
            si (e->abruptum || e->vitium)
            {
                redde;
            }
            /* post refectionem: x novus supra notam veterem, vivus */
            x = magnus_adde(x, magnus_ex_s64((s64)directio), opus);
        }
        e->x[k] = magnus_ex_s64(ZEPHYRUM);
        si (nulli)
        {
            frange;                 /* x_k >= 0 tantum */
        }
    }
}

/* nucleus enumerationis. minimum: radius = min diagonalis formae
 * reductae (limes superior minimi). FALSUM si refutatur aut vitium. */
interior b32
_enumera_publicum (
                        Matrix  m,
                           b32  est_forma,
    constans ReticulumQuaestio* quaestio,
                           b32  minimum,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus)
{
                   Fons  fons;
                   Fons  fons_reducta;
               Reductio  r;
           Orthogonalis  o;
             Enumeratio  e;
                Piscina* opus;
                Fractio  dl;
    ReticulumStatistica  stat;
                 Matrix  g;
                 Matrix  vectores;
                 Magnus* normae = NIHIL;
                 Magnus  v;
                    i32  i;
                    i32  j;
                    i32  c;

    si (   quaestio == NIHIL || exitus == NIHIL
        || (!minimum && magnus_signum(quaestio->radius_quadratus)
            < ZEPHYRUM)
        || !_fons_para(m, est_forma, &fons))
    {
        redde FALSUM;
    }
    opus = _officina_aperi();
    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    si (   !_delta_para(NIHIL, opus, &dl)
        || !_reductio_para(&fons, dl, opus, &r)
        || !_reductio_curre(&r, &opus, &stat)
        || !_matrix_ex_magnis(r.forma, r.n, opus, &g)
        || !_fons_para(g, VERUM, &fons_reducta)
        || !_orthogonaliza(&fons_reducta, opus, &o))
    {
        _officina_claude(opus);
        redde FALSUM;
    }
    e.n               = r.n;
    e.d               = o.determinantes;
    e.integri         = o.integri;
    e.numerus         = ZEPHYRUM;
    e.nodi            = ZEPHYRUM;
    e.limes_nodorum   = quaestio->limes_nodorum;
    e.abruptum        = FALSUM;
    e.vitium          = FALSUM;
    e.limes_servandi  = quaestio->limes_servandi;
    e.servati         = ZEPHYRUM;
    e.opus            = opus;
    e.columnae        = est_forma ? r.n : matrix_columnae(m);
    e.x               = _magni(r.n, opus);
    e.f               = _magni((memoriae_index)r.n + I, opus);
    e.transitus       = _magni((memoriae_index)r.n
        * (memoriae_index)e.columnae, opus);
    per (i = ZEPHYRUM; i < r.n; i++)
    {
        e.x[i] = magnus_ex_s64(ZEPHYRUM);
        per (c = ZEPHYRUM; c < e.columnae; c++)
        {
            /* forma: transitus = u; basis: basis reducta = u * basis */
            si (est_forma)
            {
                v = IN_U(&r, i, c);
            }
            alioquin
            {
                v = magnus_ex_s64(ZEPHYRUM);
                per (j = ZEPHYRUM; j < r.n; j++)
                {
                    v = magnus_adde(v, magnus_multiplica(IN_U(&r, i, j),
                        *_elementum(m, j, c), opus), opus);
                }
            }
            e.transitus[i * e.columnae + c] = v;
        }
    }
    e.f[r.n] = magnus_ex_s64(I);
    per (i = r.n; i > ZEPHYRUM; i--)
    {
        e.f[i - I] = magnus_multiplica(e.f[i], magnus_multiplica(
            e.d[i - I], e.d[i], opus), opus);
    }
    si (minimum)
    {
        e.radius = IN_FORMA(&r, ZEPHYRUM, ZEPHYRUM);
        per (i = I; i < r.n; i++)
        {
            si (magnus_compara(IN_FORMA(&r, i, i), e.radius) < ZEPHYRUM)
            {
                e.radius = IN_FORMA(&r, i, i);
            }
        }
    }
    alioquin
    {
        e.radius = quaestio->radius_quadratus;
    }
    e.capacitas = e.limes_servandi < (i32)XVI ? e.limes_servandi
        : (i32)XVI;
    e.repositorium = piscina_generare_dynamicum("reticulum",
        (memoriae_index)65536);
    si (e.repositorium == NIHIL)
    {
        _officina_claude(opus);
        redde FALSUM;
    }
    e.servata = _servata_nova(e.capacitas, e.repositorium);

    _gradus(&e, (s32)r.n - (s32)I, e.radius, VERUM);

    si (   e.vitium
        || !matrix_nulla(&ANULUS_INTEGRORUM, e.servati, e.columnae,
            piscina, &vectores))
    {
        _officina_claude(e.repositorium);
        _officina_claude(opus);
        redde FALSUM;
    }
    si (e.servati > ZEPHYRUM)
    {
        normae = _magni(e.servati, piscina);
    }
    per (i = ZEPHYRUM; i < e.servati; i++)
    {
        normae[i] = magnus_transcribe(e.servata[i].norma, piscina);
        per (c = ZEPHYRUM; c < e.columnae; c++)
        {
            v = magnus_transcribe(e.servata[i].vector[c], piscina);
            matrix_pone(&vectores, i, c, &v);
        }
    }
    _officina_claude(e.repositorium);
    _officina_claude(opus);
    exitus->numerus   = (i64)e.numerus;
    exitus->completa  = !e.abruptum;
    exitus->nodi      = (i64)e.nodi;
    exitus->servati   = e.servati;
    exitus->vectores  = vectores;
    exitus->normae    = normae;
    redde VERUM;
}

b32
reticulum_enumera (
                        Matrix  basis,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus)
{
    _apex_incipe();
    redde _enumera_publicum(basis, FALSUM, quaestio, FALSUM, piscina,
        exitus);
}

b32
reticulum_formam_enumera (
                        Matrix  forma,
    constans ReticulumQuaestio* quaestio,
                       Piscina* piscina,
           ReticulumEnumeratio* exitus)
{
    _apex_incipe();
    redde _enumera_publicum(forma, VERUM, quaestio, FALSUM, piscina,
        exitus);
}

b32
reticulum_minimum (
      Matrix  basis,
     Piscina* piscina,
      Matrix* vector,
      Magnus* norma)
{
      ReticulumQuaestio quaestio;
    ReticulumEnumeratio e;

    _apex_incipe();
    si (matrix_lineae(basis) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    quaestio.radius_quadratus  = magnus_ex_s64(ZEPHYRUM);
    quaestio.limes_servandi    = I;
    quaestio.limes_nodorum     = ZEPHYRUM;
    si (   !_enumera_publicum(basis, FALSUM, &quaestio, VERUM, piscina,
            &e)
        || e.servati != I)
    {
        redde FALSUM;
    }
    si (vector != NIHIL)
    {
        *vector = e.vectores;       /* 1 x columnae */
    }
    si (norma != NIHIL)
    {
        *norma = e.normae[ZEPHYRUM];
    }
    redde VERUM;
}


/* ==================================================
 * Relationes integrae
 *
 * Reticulum {x in Z^n : a x = 0}: basis eius (matrix_reticulum_nuclei,
 * Hermite cum certificato - omnis relatio integra combinatio integra
 * est, non solum relationes rationales), in lineas transposita, deinde
 * LLL. Super Q: linea quaeque per communem multiplicem denominatorum
 * multiplicatur - nucleus idem.
 * ================================================== */

/* a super Q -> matrix super Z eodem nucleo; a super Z redditur */
interior b32
_integra_lineis (
      Matrix  a,
     Piscina* opus,
      Matrix* exitus)
{
    constans Fractio* f;
              Matrix  z;
              Magnus  multiplex;
              Magnus  d;
              Magnus  x;
                 i32  i;
                 i32  j;

    si (matrix_anulus(a) == &ANULUS_INTEGRORUM)
    {
        *exitus = a;
        redde VERUM;
    }
    si (   matrix_anulus(a) != &ANULUS_RATIONALIUM
        || !matrix_nulla(&ANULUS_INTEGRORUM, matrix_lineae(a),
            matrix_columnae(a), opus, &z))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < matrix_lineae(a); i++)
    {
        multiplex = magnus_ex_s64(I);
        per (j = ZEPHYRUM; j < matrix_columnae(a); j++)
        {
            f = (constans Fractio*)matrix_elementum(a, i, j);
            d = fractio_denominator(*f);
            /* mcm(m, d) = m d / mcd(m, d) */
            si (!_divide_exacte(magnus_multiplica(multiplex, d, opus),
                magnus_divisor_communis(multiplex, d, opus), opus,
                &multiplex))
            {
                redde FALSUM;
            }
        }
        per (j = ZEPHYRUM; j < matrix_columnae(a); j++)
        {
            f = (constans Fractio*)matrix_elementum(a, i, j);
            si (!_divide_exacte(magnus_multiplica(fractio_numerator(*f),
                multiplex, opus), fractio_denominator(*f), opus, &x))
            {
                redde FALSUM;
            }
            matrix_pone(&z, i, j, &x);
        }
    }
    *exitus = z;
    redde VERUM;
}

b32
reticulum_relationes (
              Matrix  a,
    constans Fractio* delta,
             Piscina* piscina,
              Matrix* exitus)
{
    Piscina* opus;
     Matrix  z;
     Matrix  nucleus;
     Matrix  lineae;
     Matrix  reducta;
    Fractio  dl;
        b32  bene;

    _apex_incipe();
    opus = _officina_aperi();
    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    bene = _delta_para(delta, opus, &dl)
        && _integra_lineis(a, opus, &z)
        && matrix_reticulum_nuclei(z, opus, &nucleus)
        && matrix_transposita(nucleus, opus, &lineae);
    si (bene && matrix_lineae(lineae) == ZEPHYRUM)
    {
        bene = matrix_nulla(&ANULUS_INTEGRORUM, ZEPHYRUM,
            matrix_columnae(a), piscina, &reducta);
    }
    alioquin si (bene)
    {
        /* reductio piscinam suam aperit; apex = maximus utriusque */
        bene = _reduce_publicum(lineae, FALSUM, &dl, piscina, &reducta,
            NIHIL, NIHIL);
    }
    _officina_claude(opus);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = reducta;
    redde VERUM;
}
