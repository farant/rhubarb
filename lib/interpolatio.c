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

    q.gradus_imus     = ZEPHYRUM;
    q.gradus_summus   = ZEPHYRUM;
    q.passus          = I;
    q.limes_bitorum   = ZEPHYRUM;
    q.residuum_primi  = ZEPHYRUM;
    q.modulus_primi   = ZEPHYRUM;
    q.primus_limes    = (i32)0xFFFFFFFFUL;
    q.puncta_addita   = I;
    q.conatus_maximi  = (i32)0x100000UL;
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

/* primus proximus < limes filtro residui; 0 si nullus */
interior i32
_primus_proximus (
    constans InterpolatioQuaestio* q,
                              i32  limes)
{
    i32 p = limes;

    per (;;)
    {
        p = congruentia_primus_infra(p);
        si (p == ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        si (   q->modulus_primi < II
            || p % q->modulus_primi == q->residuum_primi
                % q->modulus_primi)
        {
            redde p;
        }
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

/* coefficientes g in y = x^passus pro uno primo: puncta colliguntur,
 * Lagrange in primis K, addita probantur. FALSUM si ERRATUM, conatus
 * exhausti aut probatio discordat. */
interior b32
_primum_aestimare (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                              i32  numerus_coefficientium,
                              i32  p,
                          Piscina* piscina,
                              i32* coefficientes)
{
    i32  necessaria = numerus_coefficientium + q->puncta_addita;
    i32* puncta_y   = (i32*)piscina_allocare(piscina,
        (memoriae_index)necessaria * magnitudo(i32));
    i32* valores_g  = (i32*)piscina_allocare(piscina,
        (memoriae_index)necessaria * magnitudo(i32));
    i32 collecta   = ZEPHYRUM;
    i32 index;
    i32 j;

    per (index = ZEPHYRUM; index < q->conatus_maximi && collecta
        < necessaria; index++)
    {
        i32 punctum  = ZEPHYRUM;
        i32 valor    = ZEPHYRUM;
        s32 status   = aestimare(contextus, index, p, &punctum, &valor);
        i32 y;
        i32 g;
        b32 iteratum = FALSUM;

        si (status == INTERPOLATIO_INUTILE)
        {
            perge;
        }
        si (   status  != INTERPOLATIO_UTILE
            || punctum == ZEPHYRUM || punctum >= p || valor >= p)
        {
            redde FALSUM;
        }
        y = congruentia_potentia(punctum, (i64)q->passus, p);
        per (j = ZEPHYRUM; j < collecta; j++)
        {
            si (puncta_y[j] == y)
            {
                iteratum = VERUM;
            }
        }
        si (iteratum)
        {
            perge;
        }
        /* g(y) = x^(-imus) f(x) */
        si (q->gradus_imus <= ZEPHYRUM)
        {
            g = congruentia_multiplica(valor, congruentia_potentia(
                punctum, (i64)(-(s64)q->gradus_imus), p), p);
        }
        alioquin
        {
            i32 inversa;

            si (!congruentia_inversa(punctum, p, &inversa))
            {
                redde FALSUM;
            }
            g = congruentia_multiplica(valor, congruentia_potentia(
                inversa, (i64)q->gradus_imus, p), p);
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

b32
interpolatio_polynomium (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                          Piscina* piscina,
                       Polynomium* exitus)
{
       s64  amplitudo;
       i32  numerus_coefficientium;
       i32  numerus_primorum;      /* primi restitutionis, sine addito */
       i32* primi;
       i32* residua;               /* [primus][coefficiens] */
       i32* columna;
    Magnus* coefficientes;
       i32  bita;
       i32  p;
       i32  j;
       i32  k;

    si (   aestimare == NIHIL || q->passus == ZEPHYRUM
        || q->gradus_summus < q->gradus_imus || q->conatus_maximi
        == ZEPHYRUM)
    {
        redde FALSUM;
    }
    amplitudo = (s64)q->gradus_summus - (s64)q->gradus_imus;
    si (amplitudo % (s64)q->passus != ZEPHYRUM)
    {
        redde FALSUM;
    }
    numerus_coefficientium = (i32)(amplitudo / (s64)q->passus) + I;

    /* primi: summa floor(log2 p) >= limes + 2 => productum > 2^(limes
     * + 1); deinde unus additus */
    numerus_primorum  = ZEPHYRUM;
    bita              = ZEPHYRUM;
    p                 = q->primus_limes;
    dum (bita < q->limes_bitorum + II)
    {
        p = _primus_proximus(q, p);
        si (p == ZEPHYRUM)
        {
            redde FALSUM;
        }
        bita += _bita_infra(p);
        numerus_primorum++;
    }
    primi = (i32*)piscina_allocare(piscina, (memoriae_index)(
        numerus_primorum + I) * magnitudo(i32));
    p = q->primus_limes;
    per (j = ZEPHYRUM; j <= numerus_primorum; j++)
    {
        p = _primus_proximus(q, p);
        si (p == ZEPHYRUM)
        {
            redde FALSUM;
        }
        primi[j] = p;
    }

    residua = (i32*)piscina_allocare(piscina, (memoriae_index)(
        numerus_primorum + I) * (memoriae_index)numerus_coefficientium
        * magnitudo(i32));
    per (j = ZEPHYRUM; j <= numerus_primorum; j++)
    {
        si (!_primum_aestimare(aestimare, contextus, q,
            numerus_coefficientium, primi[j], piscina, residua
            + (memoriae_index)j * (memoriae_index)numerus_coefficientium))
        {
            redde FALSUM;
        }
    }

    /* restitutio et probatio per primum additum et limitem */
    columna       = (i32*)piscina_allocare(piscina,
        (memoriae_index)numerus_primorum * magnitudo(i32));
    coefficientes = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)(amplitudo + I) * magnitudo(Magnus));
    per (k = ZEPHYRUM; k <= (i32)amplitudo; k++)
    {
        coefficientes[k] = magnus_ex_s64(ZEPHYRUM);
    }
    per (k = ZEPHYRUM; k < numerus_coefficientium; k++)
    {
        Magnus c;

        per (j = ZEPHYRUM; j < numerus_primorum; j++)
        {
            columna[j] = residua[(memoriae_index)j
                * (memoriae_index)numerus_coefficientium
                    + (memoriae_index)k];
        }
        si (!congruentia_restitue(columna, primi, numerus_primorum,
            VERUM,
            piscina, &c))
        {
            redde FALSUM;
        }
        si (   magnus_residuum_parvum(c, primi[numerus_primorum])
            != residua[(memoriae_index)numerus_primorum
            * (memoriae_index)numerus_coefficientium
                + (memoriae_index)k]
            || magnus_bitorum(c) > q->limes_bitorum)
        {
            redde FALSUM;
        }
        coefficientes[(memoriae_index)k * (memoriae_index)q->passus] =
            c;
    }
    redde polynomium_ex_coefficientibus(coefficientes, (i32)amplitudo
        + I,
        q->gradus_imus, piscina, exitus);
}
