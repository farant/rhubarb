/* interpolatio.c - Aestimatio, interpolatio Lagrange modulo p,
 * restitutio Sinica trans primos
 *
 * Lagrange per polynomium magistrum M(t) = prod (t - x_i) et divisionem
 * syntheticam M / (t - x_i): O(n^2) operationes, nulla matrix.
 * Restitutio per congruentia_restitue (Garner, symmetricus). Primi
 * PRIMUM enumerantur (filtro residui) donec summa floor(log2 p) >=
 * limes + 2, ita productum > 2^(limes + 1) certe; deinde unus
 * additus. Vide lib/interpolatio.worklog.md.
 */
#include "interpolatio.h"
#include "congruentia.h"
#include "magnus.h"
#include "sors.h"


/* ==================================================
 * Interpolatio Lagrange modulo p
 * ================================================== */

b32
interpolatio_lagrange_modulo (
    constans i32* x,
    constans i32* y,
             i32  numerus,
             i32  p,
         Piscina* piscina,
             i32* coefficientes)
{
    i32* magister;      /* M(t), numerus + 1 coefficientes */
    i32* quotiens;      /* M(t) / (t - x_i), numerus coefficientes */
    i32* summa;
    i32  i;
    i32  j;

    si (numerus == ZEPHYRUM || !congruentia_est_primus(p))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (x[i] >= p || y[i] >= p)
        {
            redde FALSUM;
        }
    }
    magister  = (i32*)piscina_allocare(piscina, (memoriae_index)(numerus
        + I) * magnitudo(i32));
    quotiens  = (i32*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(i32));
    summa     = (i32*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(i32));
    per (j = ZEPHYRUM; j <= numerus; j++)
    {
        magister[j] = ZEPHYRUM;
    }
    magister[ZEPHYRUM] = I;
    /* M <- M (t - x_i): gradus crescit unum per punctum */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        per (j = i + I; j > ZEPHYRUM; j--)
        {
            magister[j] = congruentia_subtrahe(magister[j - I],
                congruentia_multiplica(x[i], magister[j], p), p);
        }
        magister[ZEPHYRUM] = congruentia_subtrahe(ZEPHYRUM,
            congruentia_multiplica(x[i], magister[ZEPHYRUM], p), p);
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        summa[j] = ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        i32 differentiae = ZEPHYRUM; /* q(x_i) = prod_(j != i) (x_i - x_j) */
        i32 inversa;
        i32 pondus;

        /* divisio synthetica: q[n-1] = M[n], q[j-1] = M[j] + x_i q[j] */
        quotiens[numerus - I] = magister[numerus];
        per (j = numerus - I; j > ZEPHYRUM; j--)
        {
            quotiens[j - I] = congruentia_adde(magister[j],
                congruentia_multiplica(x[i], quotiens[j], p), p);
        }
        per (j = numerus; j > ZEPHYRUM; j--)
        {
            differentiae =
                congruentia_adde(congruentia_multiplica(differentiae,
                x[i], p), quotiens[j - I], p);
        }
        si (!congruentia_inversa(differentiae, p, &inversa))
        {
            redde FALSUM;           /* productum nullum: puncta iterata */
        }
        pondus = congruentia_multiplica(y[i], inversa, p);
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            summa[j] = congruentia_adde(summa[j],
                congruentia_multiplica(pondus, quotiens[j], p), p);
        }
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        coefficientes[j] = summa[j];
    }
    redde VERUM;
}


/* ==================================================
 * Aestimatio et restitutio
 * ================================================== */

InterpolatioQuaestio
interpolatio_quaestio_ordinaria (
    vacuum)
{
    InterpolatioQuaestio q;

    q.gradus_imus            = ZEPHYRUM;
    q.gradus_summus          = ZEPHYRUM;
    q.passus                 = I;
    q.limes_bitorum          = ZEPHYRUM;
    q.residuum_primi         = ZEPHYRUM;
    q.modulus_primi          = ZEPHYRUM;
    q.primus_limes           = (i32)0xFFFFFFFFUL;
    q.puncta_addita          = I;
    q.conatus_maximi         = (i32)0x100000UL;
    q.modus                  = INTERPOLATIO_PROBATUM;
    q.bita_securitatis       = LXIV;
    q.semen                  = ZEPHYRUM;
    q.semen_origo            = INTERPOLATIO_SEMEN_DATUM;
    q.verificationis_imus    = (i32)0x40000000UL;
    q.verificationis_summus  = (i32)0x80000000UL;
    redde q;
}

/* floor(log2 p), p >= 2 */
interior i32
_bita_infra (
    i32 p)
{
    i32 bita = ZEPHYRUM;

    dum (p > I)
    {
        p >>= I;
        bita++;
    }
    redde bita;
}

/* maximus divisor communis; (0, 0) -> 0 */
interior i32
_divisor_communis (
    i32 a,
    i32 b)
{
    dum (b != ZEPHYRUM)
    {
        i32 t = a % b;

        a = b;
        b = t;
    }
    redde a;
}

/* primus p satis y distincta dat: y = x^passus, x != 0, sumit (p - 1) /
 * mdc(passus, p - 1) valores; minus quam necessaria -> primus omittitur
 * (aliter conatus toti consumerentur) */
interior b32
_primus_aptus (
    constans InterpolatioQuaestio* q,
                              i32  p,
                              s64  necessaria)
{
    redde (s64)((p - I) / _divisor_communis(q->passus, p - I))
        >= necessaria;
}

/* primus proximus < limes: filtro residui (gradibus moduli, non omnes
 * impares - filtrum admissibile a vocante probatum) et aptus; 0 si
 * nullus. Idem semper pro eisdem argumentis (duo transitus). */
interior i32
_primus_proximus (
    constans InterpolatioQuaestio* q,
                              i32  limes,
                              s64  necessaria)
{
    si (q->modulus_primi < II)
    {
        i32 p = limes;

        per (;;)
        {
            p = congruentia_primus_infra(p);
            si (p == ZEPHYRUM)
            {
                redde ZEPHYRUM;
            }
            si (_primus_aptus(q, p, necessaria))
            {
                redde p;
            }
        }
    }
    alioquin
    {
        s64 m = (s64)q->modulus_primi;
        s64 r = (s64)(q->residuum_primi % q->modulus_primi);
        s64 n;

        si (limes == ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        /* maximus n < limes cum n = r (mod m) */
        n = (s64)limes - (s64)I;
        n = n - ((n % m - r + m) % m);
        dum (n >= (s64)II)
        {
            si (   congruentia_est_primus((i32)n)
                && _primus_aptus(q, (i32)n, necessaria))
            {
                redde (i32)n;
            }
            n -= m;
        }
        redde ZEPHYRUM;
    }
}

/* polynomium (coefficientes c[0..k-1] in y) in y mod p, Horner */
interior i32
_horner (
    constans i32* c,
             i32  k,
             i32  y,
             i32  p)
{
    i32 valor = ZEPHYRUM;
    i32 j;

    per (j = k; j > ZEPHYRUM; j--)
    {
        valor = congruentia_adde(congruentia_multiplica(valor, y, p),
            c[j - I], p);
    }
    redde valor;
}

/* g(y) = x^(-imus) f(x) mod p ex puncto x (!= 0) et valore f(x); FALSUM
 * si x non invertibilis */
interior b32
_g_ex_valore (
    constans InterpolatioQuaestio* q,
                              i32  punctum,
                              i32  valor,
                              i32  p,
                              i32* g)
{
    si (q->gradus_imus <= ZEPHYRUM)
    {
        *g = congruentia_multiplica(valor, congruentia_potentia(punctum,
            (i64)(-(s64)q->gradus_imus), p), p);
    }
    alioquin
    {
        i32 inversa;

        si (!congruentia_inversa(punctum, p, &inversa))
        {
            redde FALSUM;
        }
        *g = congruentia_multiplica(valor, congruentia_potentia(inversa,
            (i64)q->gradus_imus, p), p);
    }
    redde VERUM;
}

/* coefficientes g in y = x^passus pro uno primo: puncta colliguntur
 * (x == 0 et y iteratum omissa; y iteratum valorem eundem dare debet),
 * Lagrange in primis K, addita probantur. FALSUM si ERRATUM, punctum
 * aut valor >= p, conatus exhausti, allocatio deficit aut probatio
 * discordat. *vocationes += vocationes aestimatoris. */
interior b32
_primum_aestimare (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                              i32  numerus_coefficientium,
                              i32  p,
                          Piscina* piscina,
                              i32* coefficientes,
                              i64* vocationes)
{
    i32  necessaria = numerus_coefficientium + q->puncta_addita;
    i32* puncta_y   = (i32*)piscina_conari_allocare(piscina,
        (memoriae_index)necessaria * magnitudo(i32));
    i32* valores_g  = (i32*)piscina_conari_allocare(piscina,
        (memoriae_index)necessaria * magnitudo(i32));
    i32 collecta   = ZEPHYRUM;
    i32 index;
    i32 j;

    si (puncta_y == NIHIL || valores_g == NIHIL)
    {
        redde FALSUM;
    }
    per (index = ZEPHYRUM; index < q->conatus_maximi && collecta
        < necessaria; index++)
    {
        i32 punctum  = ZEPHYRUM;
        i32 valor    = ZEPHYRUM;
        s32 status   = aestimare(contextus, index, p, &punctum, &valor);
        i32 y;
        i32 g;
        b32 iteratum = FALSUM;

        (*vocationes)++;
        si (status == INTERPOLATIO_INUTILE)
        {
            perge;
        }
        si (status != INTERPOLATIO_UTILE || punctum >= p || valor >= p)
        {
            redde FALSUM;
        }
        si (punctum == ZEPHYRUM)
        {
            perge;                  /* x^passus nihil distinguit */
        }
        y = congruentia_potentia(punctum, (i64)q->passus, p);
        si (!_g_ex_valore(q, punctum, valor, p, &g))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < collecta; j++)
        {
            si (puncta_y[j] == y)
            {
                si (valores_g[j] != g)
                {
                    redde FALSUM;   /* passus aut fenestra falsa */
                }
                iteratum = VERUM;
            }
        }
        si (iteratum)
        {
            perge;
        }
        puncta_y[collecta]   = y;
        valores_g[collecta]  = g;
        collecta++;
    }
    si (collecta < necessaria)
    {
        redde FALSUM;
    }
    si (!interpolatio_lagrange_modulo(puncta_y, valores_g,
        numerus_coefficientium, p, piscina, coefficientes))
    {
        redde FALSUM;
    }
    per (j = numerus_coefficientium; j < necessaria; j++)
    {
        si (_horner(coefficientes, numerus_coefficientium, puncta_y[j],
            p)
            != valores_g[j])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* quaestio valida? Omnia ante primam aestimationem iudicantur. */
interior b32
_quaestio_valida (
    constans InterpolatioQuaestio* q)
{
    s64 imus     = (s64)q->gradus_imus;
    s64 summus   = (s64)q->gradus_summus;
    s64 maximus  = (s64)POLYNOMIUM_EXPONENS_MAXIMUS;

    si (   q->passus == ZEPHYRUM || q->conatus_maximi == ZEPHYRUM
        || summus < imus || imus < -maximus || summus > maximus
        || q->limes_bitorum > INTERPOLATIO_LIMES_MAXIMUS)
    {
        redde FALSUM;
    }
    si ((summus - imus) % (s64)q->passus != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   (summus - imus) / (s64)q->passus + (s64)I
        + (s64)q->puncta_addita
        > (s64)0x7FFFFFFFL)
    {
        redde FALSUM;
    }
    si (   q->modulus_primi >= II
        && _divisor_communis(q->residuum_primi % q->modulus_primi,
        q->modulus_primi)
        != I)
    {
        redde FALSUM;               /* filtrum nullos primos (praeter
                                     * fortasse unum) admittit */
    }
    redde VERUM;
}

/* primi restitutionis (summa floor(log2 p) >= limes + 1 => productum >=
 * 2^(limes + 1) > 2 max |c|, |c| <= 2^limes - 1), deinde unus additus:
 * (*primi)[0 .. *k_maximus], ultimus additus. Duo transitus (numerare,
 * scribere). FALSUM si primi desunt. */
interior b32
_primos_enumerare (
    constans InterpolatioQuaestio*  q,
                              s64   necessaria,
                          Piscina*  piscina,
                              i32** primi,
                              i32*  k_maximus)
{
    i32 numerus  = ZEPHYRUM;
    s64 bita     = ZEPHYRUM;
    i32 p        = q->primus_limes;
    i32 j;

    dum (bita < (s64)q->limes_bitorum + (s64)I)
    {
        p = _primus_proximus(q, p, necessaria);
        si (p == ZEPHYRUM)
        {
            redde FALSUM;
        }
        bita += (s64)_bita_infra(p);
        numerus++;
    }
    *primi = (i32*)piscina_allocare(piscina, (memoriae_index)(numerus
        + I)
        * magnitudo(i32));
    p = q->primus_limes;
    per (j = ZEPHYRUM; j <= numerus; j++)
    {
        p = _primus_proximus(q, p, necessaria);
        si (p == ZEPHYRUM)
        {
            redde FALSUM;
        }
        (*primi)[j] = p;
    }
    *k_maximus = numerus;
    redde VERUM;
}

/* coefficiens k ex residuis primorum 0 .. numerus - 1 (Garner,
 * symmetricus); columna: numerus i32 a vocante */
interior b32
_coefficientem_restituere (
    constans i32* residua,
    constans i32* primi,
             i32  numerus,
             i32  numerus_coefficientium,
             i32  k,
             i32* columna,
         Piscina* piscina,
          Magnus* exitus)
{
    i32 j;

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        columna[j] = residua[(memoriae_index)j
            * (memoriae_index)numerus_coefficientium
                + (memoriae_index)k];
    }
    redde congruentia_restitue(columna, primi, numerus, VERUM, piscina,
        exitus);
}

/* restitutio ad limitem: primi 0 .. k_maximus - 1; additus (k_maximus)
 * confirmare debet et |c| < 2^limes */
interior b32
_restituere_ad_limitem (
    constans InterpolatioQuaestio* q,
                     constans i32* residua,
                     constans i32* primi,
                              i32  k_maximus,
                              i32  numerus_coefficientium,
                          Piscina* piscina,
                           Magnus* coefficientes)
{
    i32* columna = (i32*)piscina_allocare(piscina,
        (memoriae_index)k_maximus * magnitudo(i32));
    i32 k;

    per (k = ZEPHYRUM; k < numerus_coefficientium; k++)
    {
        Magnus c;

        si (!_coefficientem_restituere(residua, primi, k_maximus,
            numerus_coefficientium, k, columna, piscina, &c))
        {
            redde FALSUM;
        }
        si (   magnus_residuum_parvum(c, primi[k_maximus])
            != residua[(memoriae_index)k_maximus
            * (memoriae_index)numerus_coefficientium
                + (memoriae_index)k]
            || magnus_bitorum(c) > q->limes_bitorum)
        {
            redde FALSUM;
        }
        coefficientes[k] = c;
    }
    redde VERUM;
}

/* exitus ex coefficientibus fenestrae: densum solum a coefficiente non
 * nullo primo ad ultimum (memoria ex effectu, non ex fenestra) */
interior b32
_polynomium_formare (
    constans InterpolatioQuaestio* q,
                  constans Magnus* coefficientes,
                              i32  numerus_coefficientium,
                          Piscina* piscina,
                       Polynomium* exitus)
{
    Magnus* densa;
       s64  primus_nonnullus   = (s64)-I;
       s64  ultimus_nonnullus  = (s64)-I;
       s64  amplitudo;
       i32  k;

    per (k = ZEPHYRUM; k < numerus_coefficientium; k++)
    {
        si (magnus_signum(coefficientes[k]) != ZEPHYRUM)
        {
            si (primus_nonnullus < ZEPHYRUM)
            {
                primus_nonnullus = (s64)k;
            }
            ultimus_nonnullus = (s64)k;
        }
    }
    si (primus_nonnullus < ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    amplitudo = (ultimus_nonnullus - primus_nonnullus) * (s64)q->passus
        + (s64)I;
    densa = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)amplitudo * magnitudo(Magnus));
    si (densa == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; (s64)k < amplitudo; k++)
    {
        densa[k] = magnus_ex_s64(ZEPHYRUM);
    }
    per (k = (i32)primus_nonnullus; (s64)k <= ultimus_nonnullus; k++)
    {
        densa[(memoriae_index)((s64)k - primus_nonnullus)
            * (memoriae_index)q->passus] = coefficientes[k];
    }
    redde polynomium_ex_coefficientibus(densa, (i32)amplitudo, (s32)(
        (s64)q->gradus_imus + primus_nonnullus * (s64)q->passus),
        piscina,
        exitus);
}

interior vacuum
_relatio_initium (
                 InterpolatioRelatio* r,
       constans InterpolatioQuaestio* q,
    constans InterpolatioVerificator* v)
{
    r->quaestio                   = *q;
    r->genus                      = INTERPOLATIO_PROBATUM;
    r->status                     = INTERPOLATIO_STATUS_NULLUS;
    r->primi                      = NIHIL;
    r->numerus_primorum           = ZEPHYRUM;
    r->primus_additus             = ZEPHYRUM;
    r->k_maximus                  = ZEPHYRUM;
    r->aestimationes              = ZEPHYRUM;
    r->probationes_sui            = ZEPHYRUM;
    r->probationes_certificantes  = ZEPHYRUM;
    r->probationes_requisitae     = ZEPHYRUM;
    r->bita_erroris               = ZEPHYRUM;
    r->tractus                    = NIHIL;
    r->numerus_tractuum           = ZEPHYRUM;
    r->titulus_verificatoris      = v != NIHIL ? v->titulus : NIHIL;
    r->communia                   = v != NIHIL ? v->communia : NIHIL;
    r->generator                  = INTERPOLATIO_GENERATOR;
}

/* PROBATUM: primi omnes aestimantur, deinde restitutio ad limitem (idem
 * ordo et idem effectus ac ante verificationem) */
interior b32
_probatum (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                          Piscina* piscina,
                       Polynomium* exitus,
              InterpolatioRelatio* relatio)
{
       i32 numerus_coefficientium = (i32)(((s64)q->gradus_summus
           - (s64)q->gradus_imus) / (s64)q->passus + (s64)I);
       s64 necessaria = (s64)numerus_coefficientium
           + (s64)q->puncta_addita;
       i32* primi;
       i32  k_maximus;
       i32* residua;
    Magnus* coefficientes;
       i32  j;

    si (!_primos_enumerare(q, necessaria, piscina, &primi, &k_maximus))
    {
        redde FALSUM;
    }
    relatio->primi      = primi;
    relatio->k_maximus  = k_maximus;
    residua = (i32*)piscina_conari_allocare(piscina, (memoriae_index)(
        k_maximus + I) * (memoriae_index)numerus_coefficientium
        * magnitudo(i32));
    si (residua == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j <= k_maximus; j++)
    {
        si (!_primum_aestimare(aestimare, contextus, q,
            numerus_coefficientium, primi[j], piscina, residua
            + (memoriae_index)j * (memoriae_index)numerus_coefficientium,
            &relatio->aestimationes))
        {
            redde FALSUM;
        }
    }
    coefficientes = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)numerus_coefficientium * magnitudo(Magnus));
    si (   coefficientes == NIHIL
        || !_restituere_ad_limitem(q, residua, primi, k_maximus,
        numerus_coefficientium, piscina, coefficientes)
        || !_polynomium_formare(q, coefficientes,
        numerus_coefficientium,
        piscina, exitus))
    {
        redde FALSUM;
    }
    relatio->genus             = INTERPOLATIO_PROBATUM;
    relatio->status            = INTERPOLATIO_AD_LIMITEM;
    relatio->numerus_primorum  = k_maximus;
    relatio->primus_additus    = primi[k_maximus];
    redde VERUM;
}

b32
interpolatio_polynomium (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                          Piscina* piscina,
                       Polynomium* exitus)
{
    InterpolatioRelatio relatio;

    si (aestimare == NIHIL || !_quaestio_valida(q))
    {
        redde FALSUM;
    }
    _relatio_initium(&relatio, q, NIHIL);
    redde _probatum(aestimare, contextus, q, piscina, exitus, &relatio);
}


/* ==================================================
 * Verificatio
 * ================================================== */

#define _AMBITUS_IMUS              ((i32)0x40000000UL)  /* 2^30 */
#define _AMBITUS_SUMMUS            ((i32)0x80000000UL)  /* 2^31 */
#define _PRIMI_AMBITUS_MINIMI      ((i64)0x1000000UL)   /* 2^24: N_MIN */
#define _LATITUDO_NUMERANDA        ((i32)0x1000000UL)   /* 2^24 */
#define _CONATUS_PRIMI_MAXIMI      ((i32)0x100000UL)    /* 2^20 */
#define _INUTILIA_ITERATA_MAXIMA   LXIV
#define _BITA_ERRORIS_MAXIMA       ((i32)0x400)         /* 1024 */

/* filtrum residui quaestionis (modulus < 2: omnes) */
interior b32
_filtrum_admittit (
    constans InterpolatioQuaestio* q,
                              i32  v)
{
    si (q->modulus_primi < II)
    {
        redde VERUM;
    }
    redde v % q->modulus_primi == q->residuum_primi % q->modulus_primi;
}

/* N: primi probationum eligibiles. Ambitus ordinarius [2^30, 2^31):
 * N_MIN = 2^24 (cribrum T0: 25348870 primi 1 mod 4, 25348667 3 mod 4,
 * 50697537 omnes - margo > 8 * 10^6 ultra primos exclusos, <= CLX);
 * filtrum aliud refutatur. Ambitus alius (probationibus): latitudo <=
 * 2^24, primi numerantur, minus k_maximus + 1 (exclusi). */
interior b32
_primi_ambitus (
    constans InterpolatioQuaestio* q,
                              i32  k_maximus,
                              i64* numerus)
{
    i32 imus    = q->verificationis_imus;
    i32 summus  = q->verificationis_summus;
    i64 n       = ZEPHYRUM;
    i32 v;

    si (imus < V || summus <= imus)
    {
        redde FALSUM;
    }
    si (imus == _AMBITUS_IMUS && summus == _AMBITUS_SUMMUS)
    {
        si (q->modulus_primi >= II && q->modulus_primi != IV)
        {
            redde FALSUM;
        }
        *numerus = _PRIMI_AMBITUS_MINIMI;
        redde VERUM;
    }
    si (summus - imus > _LATITUDO_NUMERANDA)
    {
        redde FALSUM;
    }
    per (v = imus; v < summus; v++)
    {
        si (_filtrum_admittit(q, v) && congruentia_est_primus(v))
        {
            n++;
        }
    }
    si (n <= (i64)k_maximus + (i64)I)
    {
        redde FALSUM;
    }
    *numerus = n - (i64)k_maximus - (i64)I;
    redde VERUM;
}

b32
interpolatio_probationes_necessariae (
      constans InterpolatioQuaestio*  q,
    constans InterpolatioVerificator* verificator,
                                 i32  k_maximus,
                            Piscina*  piscina,
                                i32*  probationes,
                                i32*  bita_erroris)
{
       i64 numerus;
       i32 coefficientes;
       i32 factores;
       s64 basis;
    Magnus a;
    Magnus b;
    Magnus sinistra;
    Magnus dextra;
       i32 m;
       i32 beta;

    si (   q == NIHIL || verificator == NIHIL || probationes == NIHIL
        || bita_erroris == NIHIL || !_quaestio_valida(q)
        || q->bita_securitatis < I || q->bita_securitatis > CCLVI
        || verificator->multiplicitas < I
        || verificator->inutilia_maxima >= q->verificationis_imus
        || !_primi_ambitus(q, k_maximus, &numerus))
    {
        redde FALSUM;
    }
    si (k_maximus <= I)
    {
        *probationes   = ZEPHYRUM;
        *bita_erroris  = ZEPHYRUM;
        redde VERUM;
    }
    coefficientes = (i32)(((s64)q->gradus_summus - (s64)q->gradus_imus)
        / (s64)q->passus + (s64)I);
    /* factores primi >= q_min numeri < 2^(limes + 1) */
    factores = (q->limes_bitorum + I)
        / _bita_infra(q->verificationis_imus);
    basis = (s64)q->verificationis_imus
        - (s64)verificator->inutilia_maxima;
    /* eps = c (K - 1) / basis + factores / N = a / b */
    a = magnus_adde(magnus_multiplica(magnus_multiplica(magnus_ex_s64(
        (s64)verificator->multiplicitas),
        magnus_ex_s64((s64)coefficientes
        - (s64)I), piscina), magnus_ex_s64((s64)numerus), piscina),
        magnus_multiplica(magnus_ex_s64((s64)factores),
        magnus_ex_s64(basis),
        piscina), piscina);
    b = magnus_multiplica(magnus_ex_s64(basis),
        magnus_ex_s64((s64)numerus),
        piscina);
    si (magnus_compara(a, b) >= ZEPHYRUM)
    {
        redde FALSUM;               /* eps >= 1: nulla m */
    }
    /* sinistra = (k - 1) 2^s a^m, dextra = b^m */
    sinistra = magnus_multiplica(magnus_ex_s64((s64)k_maximus - (s64)I),
        magnus_potentia(magnus_ex_s64(II), q->bita_securitatis,
        piscina),
        piscina);
    dextra = magnus_ex_s64((s64)I);
    per (m = I; m <= CCLVI; m++)
    {
        sinistra  = magnus_multiplica(sinistra, a, piscina);
        dextra    = magnus_multiplica(dextra, b, piscina);
        si (magnus_compara(sinistra, dextra) <= ZEPHYRUM)
        {
            frange;
        }
    }
    si (m > CCLVI)
    {
        redde FALSUM;
    }
    /* beta maximus cum (k - 1) a^m 2^beta <= b^m (beta >= s) */
    beta = q->bita_securitatis;
    dum (beta < _BITA_ERRORIS_MAXIMA)
    {
        Magnus duplum = magnus_adde(sinistra, sinistra, piscina);

        si (magnus_compara(duplum, dextra) > ZEPHYRUM)
        {
            frange;
        }
        sinistra = duplum;
        beta++;
    }
    *probationes   = m;
    *bita_erroris  = beta;
    redde VERUM;
}

interior b32
_primus_exclusus (
    constans i32* exclusi,
             i32  numerus,
             i32  v)
{
    i32 j;

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        si (exclusi[j] == v)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* probatio una, forma documentata in capite (primus, deinde index):
 * aestimator in (index, q), candidatus g(y) per Horner residuorum.
 * *tractus scribitur (primus, index, transiit). FALSUM si ERRATUM,
 * punctum aut valor >= q, aut conatus exhausti. */
interior b32
_probare_in_puncto (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                  constans Magnus* coefficientes,
                              i32  numerus_coefficientium,
                    constans i32*  exclusi,
                              i32  numerus_exclusorum,
                             Sors* sors,
              InterpolatioTractus* tractus)
{
    i32 latitudo  = q->verificationis_summus - q->verificationis_imus;
    i32 primus    = ZEPHYRUM;
    i32 conatus;

    per (conatus = ZEPHYRUM; conatus < _CONATUS_PRIMI_MAXIMI; conatus++)
    {
        i32 v = q->verificationis_imus + sors_intra(sors, latitudo);

        si (   _filtrum_admittit(q, v) && congruentia_est_primus(v)
            && !_primus_exclusus(exclusi, numerus_exclusorum, v))
        {
            primus = v;
            frange;
        }
    }
    si (primus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (conatus = ZEPHYRUM; conatus
        <= _INUTILIA_ITERATA_MAXIMA; conatus++)
    {
        i32 index    = sors_intra(sors, primus);
        i32 punctum  = ZEPHYRUM;
        i32 valor    = ZEPHYRUM;
        s32 status   = aestimare(contextus, index, primus, &punctum,
            &valor);
        i32 g;
        i32 y;
        i32 expectatum = ZEPHYRUM;
        i32 j;

        si (status == INTERPOLATIO_INUTILE)
        {
            perge;
        }
        si (   status != INTERPOLATIO_UTILE || punctum >= primus
            || valor  >= primus)
        {
            redde FALSUM;
        }
        si (punctum == ZEPHYRUM)
        {
            perge;
        }
        si (!_g_ex_valore(q, punctum, valor, primus, &g))
        {
            redde FALSUM;
        }
        y = congruentia_potentia(punctum, (i64)q->passus, primus);
        per (j = numerus_coefficientium; j > ZEPHYRUM; j--)
        {
            expectatum =
                congruentia_adde(congruentia_multiplica(expectatum,
                y, primus), magnus_residuum_parvum(coefficientes[j - I],
                primus), primus);
        }
        tractus->primus    = primus;
        tractus->index     = index;
        tractus->transiit  = expectatum == g;
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_coefficientes_aequales (
    constans Magnus* a,
    constans Magnus* b,
                i32  numerus)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!magnus_aequalis(a[k], b[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* VERIFICATUM: stadia 1 .. k_max - 1 cum probationibus (caput), deinde
 * ad limitem ut PROBATUM */
interior b32
_verificatum (
              InterpolatioAestimator  aestimare,
                              vacuum* contextus,
    constans InterpolatioVerificator* verificator,
       constans InterpolatioQuaestio* q,
                             Piscina* piscina,
                          Polynomium* exitus,
                 InterpolatioRelatio* relatio)
{
       i32 numerus_coefficientium = (i32)(((s64)q->gradus_summus
           - (s64)q->gradus_imus) / (s64)q->passus + (s64)I);
       s64 necessaria = (s64)numerus_coefficientium
           + (s64)q->puncta_addita;
       i32* primi;
       i32  k_maximus;
       i32  m;
       i32  bita;
       i32* residua;
       i32* columna;
    Magnus* coefficientes;
    Magnus* discors;
       b32  habet_discordem = FALSUM;
      Sors  sors;
       i32  k;
       i32  j;

    si (   !_primos_enumerare(q, necessaria, piscina, &primi,
        &k_maximus)
        || !interpolatio_probationes_necessariae(q, verificator,
        k_maximus, piscina, &m, &bita))
    {
        redde FALSUM;
    }
    relatio->primi                   = primi;
    relatio->k_maximus               = k_maximus;
    relatio->probationes_requisitae  = m;
    residua = (i32*)piscina_conari_allocare(piscina, (memoriae_index)(
        k_maximus + I) * (memoriae_index)numerus_coefficientium
        * magnitudo(i32));
    coefficientes = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)numerus_coefficientium * magnitudo(Magnus));
    discors = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)numerus_coefficientium * magnitudo(Magnus));
    si (residua == NIHIL || coefficientes == NIHIL || discors == NIHIL)
    {
        redde FALSUM;
    }
    columna = (i32*)piscina_allocare(piscina, (memoriae_index)k_maximus
        * magnitudo(i32));
    relatio->tractus = (InterpolatioTractus*)piscina_allocare(piscina,
        (memoriae_index)(k_maximus + I) * (memoriae_index)(m + II)
        * magnitudo(InterpolatioTractus));
    sors_seminare(&sors, q->semen, ZEPHYRUM);

    per (k = I; k < k_maximus; k++)
    {
        InterpolatioTractus* tractus;
                        b32  aptus = VERUM;

        si (!_primum_aestimare(aestimare, contextus, q,
            numerus_coefficientium, primi[k - I], piscina, residua
            + (memoriae_index)(k - I)
            * (memoriae_index)numerus_coefficientium,
            &relatio->aestimationes))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < numerus_coefficientium; j++)
        {
            si (!_coefficientem_restituere(residua, primi, k,
                numerus_coefficientium, j, columna, piscina,
                &coefficientes[j]))
            {
                redde FALSUM;
            }
            si (magnus_bitorum(coefficientes[j]) > q->limes_bitorum)
            {
                aptus = FALSUM;     /* falsus per limitem probatum */
                frange;
            }
        }
        si (!aptus)
        {
            perge;
        }
        tractus = &relatio->tractus[relatio->numerus_tractuum];
        tractus->gradus = k;
        tractus->certificans = FALSUM;
        si (!_probare_in_puncto(aestimare, contextus, q, coefficientes,
            numerus_coefficientium, primi, k, &sors, tractus))
        {
            redde FALSUM;
        }
        relatio->numerus_tractuum++;
        relatio->probationes_sui++;
        si (!tractus->transiit)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < m; j++)
        {
            tractus = &relatio->tractus[relatio->numerus_tractuum];
            tractus->gradus = k;
            tractus->certificans = VERUM;
            si (!_probare_in_puncto(verificator->aestimare,
                verificator->contextus, q, coefficientes,
                numerus_coefficientium, primi, k, &sors, tractus))
            {
                redde FALSUM;
            }
            relatio->numerus_tractuum++;
            relatio->probationes_certificantes++;
            si (!tractus->transiit)
            {
                frange;
            }
        }
        si (j == m)
        {
            si (!_polynomium_formare(q, coefficientes,
                numerus_coefficientium, piscina, exitus))
            {
                redde FALSUM;
            }
            relatio->genus             = INTERPOLATIO_VERIFICATUM;
            relatio->status            = INTERPOLATIO_ACCEPTUM;
            relatio->numerus_primorum  = k;
            relatio->bita_erroris      = bita;
            redde VERUM;
        }
        /* probatio sui transiit, certificans reiecit */
        si (   habet_discordem && _coefficientes_aequales(discors,
            coefficientes, numerus_coefficientium))
        {
            relatio->status = INTERPOLATIO_DISCORDIA;
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < numerus_coefficientium; j++)
        {
            discors[j] = coefficientes[j];
        }
        habet_discordem = VERUM;
    }

    /* ad limitem: primus k_max et additus */
    per (j = k_maximus - I; j <= k_maximus; j++)
    {
        si (!_primum_aestimare(aestimare, contextus, q,
            numerus_coefficientium, primi[j], piscina, residua
            + (memoriae_index)j * (memoriae_index)numerus_coefficientium,
            &relatio->aestimationes))
        {
            redde FALSUM;
        }
    }
    si (!_restituere_ad_limitem(q, residua, primi, k_maximus,
        numerus_coefficientium, piscina, coefficientes))
    {
        redde FALSUM;
    }
    si (   habet_discordem && _coefficientes_aequales(discors,
        coefficientes, numerus_coefficientium))
    {
        relatio->status = INTERPOLATIO_DISCORDIA;
        redde FALSUM;               /* effectum probatum reiecit */
    }
    si (!_polynomium_formare(q, coefficientes, numerus_coefficientium,
        piscina, exitus))
    {
        redde FALSUM;
    }
    relatio->genus             = INTERPOLATIO_PROBATUM;
    relatio->status            = INTERPOLATIO_AD_LIMITEM;
    relatio->numerus_primorum  = k_maximus;
    relatio->primus_additus    = primi[k_maximus];
    redde VERUM;
}

b32
interpolatio_polynomium_relatum (
              InterpolatioAestimator  aestimare,
                              vacuum* contextus,
    constans InterpolatioVerificator* verificator,
       constans InterpolatioQuaestio* q,
                             Piscina* piscina,
                          Polynomium* exitus,
                 InterpolatioRelatio* relatio)
{
    InterpolatioRelatio  localis;
    InterpolatioRelatio* r = relatio != NIHIL ? relatio : &localis;

    si (q == NIHIL)
    {
        redde FALSUM;
    }
    _relatio_initium(r, q, q->modus == INTERPOLATIO_VERIFICATUM
        ? verificator : NIHIL);
    si (aestimare == NIHIL || !_quaestio_valida(q))
    {
        redde FALSUM;
    }
    si (q->modus == INTERPOLATIO_PROBATUM)
    {
        redde _probatum(aestimare, contextus, q, piscina, exitus, r);
    }
    si (   q->modus != INTERPOLATIO_VERIFICATUM || verificator == NIHIL
        || verificator->aestimare == NIHIL
        || q->semen_origo > INTERPOLATIO_SEMEN_FORTUITUM)
    {
        redde FALSUM;
    }
    redde _verificatum(aestimare, contextus, verificator, q, piscina,
        exitus, r);
}


/* ==================================================
 * Promotio
 * ================================================== */

/* coefficientes fenestrae (in y) ex polynomio; FALSUM si coefficiens non
 * nullus extra fenestram aut extra classem passus */
interior b32
_coefficientes_ex_polynomio (
    constans InterpolatioQuaestio* q,
              constans Polynomium* f,
                              i32  numerus_coefficientium,
                           Magnus* coefficientes)
{
    s32 e;
    i32 k;

    per (k = ZEPHYRUM; k < numerus_coefficientium; k++)
    {
        coefficientes[k] = magnus_ex_s64(ZEPHYRUM);
    }
    si (polynomium_est_nullum(*f))
    {
        redde VERUM;
    }
    per (e = polynomium_gradus_imus(*f); e
        <= polynomium_gradus_summus(*f);
        e++)
    {
        Magnus c = polynomium_coefficiens(*f, e);
           s64 d = (s64)e - (s64)q->gradus_imus;

        si (magnus_signum(c) == ZEPHYRUM)
        {
            perge;
        }
        si (   d < ZEPHYRUM || (s64)e > (s64)q->gradus_summus
            || d % (s64)q->passus != ZEPHYRUM)
        {
            redde FALSUM;
        }
        coefficientes[d / (s64)q->passus] = c;
    }
    redde VERUM;
}

b32
interpolatio_promovere (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
     constans InterpolatioRelatio* relatio,
              constans Polynomium* verificatum,
                          Piscina* piscina,
                       Polynomium* exitus,
              InterpolatioRelatio* relatio_nova)
{
       i32  numerus_coefficientium;
       s64  necessaria;
       i32* primi;
       i32  k_maximus;
       i32  k;
       i32* residua;
    Magnus* coefficientes;
    Magnus* verificati;
       i32  i;
       i32  j;

    si (   q == NIHIL || relatio == NIHIL || verificatum == NIHIL
        || relatio_nova == NIHIL || relatio_nova == relatio)
    {
        redde FALSUM;
    }
    _relatio_initium(relatio_nova, q, NIHIL);
    si (   aestimare       == NIHIL || !_quaestio_valida(q)
        || relatio->genus  != INTERPOLATIO_VERIFICATUM
        || relatio->status != INTERPOLATIO_ACCEPTUM
        || relatio->primi  == NIHIL)
    {
        redde FALSUM;
    }
    numerus_coefficientium = (i32)(((s64)q->gradus_summus
        - (s64)q->gradus_imus) / (s64)q->passus + (s64)I);
    necessaria = (s64)numerus_coefficientium + (s64)q->puncta_addita;
    si (!_primos_enumerare(q, necessaria, piscina, &primi, &k_maximus))
    {
        redde FALSUM;
    }
    k = relatio->numerus_primorum;
    si (k < I || k >= k_maximus)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < k; j++)
    {
        si (primi[j] != relatio->primi[j])
        {
            redde FALSUM;           /* series mutata (via, limes, codex) */
        }
    }
    verificati = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)numerus_coefficientium * magnitudo(Magnus));
    coefficientes = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)numerus_coefficientium * magnitudo(Magnus));
    residua = (i32*)piscina_conari_allocare(piscina, (memoriae_index)(
        k_maximus + I) * (memoriae_index)numerus_coefficientium
        * magnitudo(i32));
    si (   verificati == NIHIL || coefficientes == NIHIL
        || residua    == NIHIL
        || !_coefficientes_ex_polynomio(q, verificatum,
        numerus_coefficientium, verificati))
    {
        redde FALSUM;
    }
    relatio_nova->primi      = primi;
    relatio_nova->k_maximus  = k_maximus;
    /* residua primorum iam usorum ex verificato ipso */
    per (j = ZEPHYRUM; j < k; j++)
    {
        per (i = ZEPHYRUM; i < numerus_coefficientium; i++)
        {
            residua[(memoriae_index)j * (memoriae_index)numerus_coefficientium
                + (memoriae_index)i] =
                magnus_residuum_parvum(verificati[i],
                primi[j]);
        }
    }
    per (j = k; j <= k_maximus; j++)
    {
        si (!_primum_aestimare(aestimare, contextus, q,
            numerus_coefficientium, primi[j], piscina, residua
            + (memoriae_index)j * (memoriae_index)numerus_coefficientium,
            &relatio_nova->aestimationes))
        {
            redde FALSUM;
        }
    }
    si (   !_restituere_ad_limitem(q, residua, primi, k_maximus,
        numerus_coefficientium, piscina, coefficientes)
        || !_coefficientes_aequales(coefficientes, verificati,
        numerus_coefficientium)
        || !_polynomium_formare(q, coefficientes,
        numerus_coefficientium,
        piscina, exitus))
    {
        redde FALSUM;
    }
    relatio_nova->genus             = INTERPOLATIO_PROBATUM;
    relatio_nova->status            = INTERPOLATIO_PROMOTUM;
    relatio_nova->numerus_primorum  = k_maximus;
    relatio_nova->primus_additus    = primi[k_maximus];
    redde VERUM;
}
