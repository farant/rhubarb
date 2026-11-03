/* probatio_interpolatio.c - Probationes interpolationis et restitutionis
 *
 * Oraculum in probatione ipsa: polynomia nota (coefficientes magni
 * fortuiti, exponentes Laurentiani, passus) per Horner proprium
 * aestimantur et polynomium restitutum toti nota aequale esse debet.
 * Aestimatores mendaces, erronei, inutiles, puncta iterata, filtrum
 * primorum, limes in margine. Vide lib/interpolatio.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include "magnus.h"
#include "congruentia.h"
#include "polynomium.h"
#include "interpolatio.h"


/* ==================================================
 * Auxilia
 * ================================================== */

/* polynomium notum: c[k] pro t^(imus + passus k) */
nomen structura {
    constans Magnus* coefficientes;
                i32  numerus;
                s32  imus;
                i32  passus;
} Notum;

/* contextus aestimatoris: notum, deinde mendacia et numeri vocationum */
nomen structura {
    constans Notum* notum;
               i32  vocationes;
               i32  mendax_primus;       /* 0 = nullus */
               i32  mendax_index;        /* index mendax in omni primo */
               b32  mendax_index_activus;
               i32  erratum_ad;          /* vocatio erronea (1-basata), 0 nulla */
               b32  inutile_semper;
               b32  puncta_iterata;      /* x = index/2 + 1 */
               b32  oppositi;            /* x = j, deinde p - j */
               i32  vitium_ad;           /* vocatio vitiosa (1-basata), 0 nulla */
               i32  vitium_genus;        /* 0 punctum 0, 1 punctum p, 2 valor p,
                                          * 3 status ignotus */
               i32 inutile_a_primo;     /* primus ordine (1-basatus) INUTILE totus */
               i32 erratum_a_primo;     /* ERRATUM in vocatione prima huius primi */
               i32 vocationes_primi;
               i32 primi_visi[XVI];
               i32 numerus_primorum;
               b32 mendax_omnis;         /* valor + 1 semper (verificator
                                          * mendax) */
               b32 speculum;             /* f(1/x) loco f(x) */
} Contextus;

/* x^e mod p, e signatus (x != 0) */
interior i32
_potentia_signata (
    i32 x,
    s64 e,
    i32 p)
{
    i32 inversa;

    si (e >= ZEPHYRUM)
    {
        redde congruentia_potentia(x, (i64)e, p);
    }
    (vacuum)congruentia_inversa(x, p, &inversa);
    redde congruentia_potentia(inversa, (i64)(-e), p);
}

/* f(x) mod p per summam directam (non Horner bibliothecae) */
interior i32
_aestima_notum (
    constans Notum* n,
               i32  x,
               i32  p)
{
    i32 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < n->numerus; k++)
    {
        s64 e = (s64)n->imus + (s64)n->passus * (s64)k;

        summa = congruentia_adde(summa, congruentia_multiplica(
            magnus_residuum_parvum(n->coefficientes[k], p),
            _potentia_signata(x, e, p), p), p);
    }
    redde summa;
}

interior s32
_aestimator (
    vacuum* contextus,
       i32  index,
       i32  p,
       i32* punctum,
       i32* valor)
{
    Contextus* c = (Contextus*)contextus;
          i32  x;
          i32  v;

    c->vocationes++;
    si (   c->numerus_primorum                    == ZEPHYRUM
        || c->primi_visi[c->numerus_primorum - I] != p)
    {
        si (c->numerus_primorum < XVI)
        {
            c->primi_visi[c->numerus_primorum] = p;
        }
        c->numerus_primorum++;
        c->vocationes_primi = ZEPHYRUM;
    }
    c->vocationes_primi++;
    si (   c->erratum_a_primo  != ZEPHYRUM
        && c->numerus_primorum == c->erratum_a_primo
        && c->vocationes_primi == I)
    {
        redde INTERPOLATIO_ERRATUM;
    }
    si (   c->inutile_a_primo  != ZEPHYRUM
        && c->numerus_primorum == c->inutile_a_primo)
    {
        redde INTERPOLATIO_INUTILE;
    }
    si (c->erratum_ad != ZEPHYRUM && c->vocationes == c->erratum_ad)
    {
        redde INTERPOLATIO_ERRATUM;
    }
    si (c->inutile_semper)
    {
        redde INTERPOLATIO_INUTILE;
    }
    x = c->puncta_iterata ? index / II + I : index + I;
    si (c->oppositi)
    {
        x = index / II + I;
        si (x < p && index % II == I)
        {
            x = p - x;
        }
    }
    si (x >= p)
    {
        redde INTERPOLATIO_INUTILE;
    }
    si (c->speculum)
    {
        i32 inversa;

        (vacuum)congruentia_inversa(x, p, &inversa);
        v = _aestima_notum(c->notum, inversa, p);
    }
    alioquin
    {
        v = _aestima_notum(c->notum, x, p);
    }
    si (c->mendax_omnis)
    {
        v = congruentia_adde(v, I, p);
    }
    si (c->vitium_ad != ZEPHYRUM && c->vocationes == c->vitium_ad)
    {
        si (c->vitium_genus == ZEPHYRUM)
        {
            x = ZEPHYRUM;
        }
        alioquin si (c->vitium_genus == I)
        {
            x = p;
        }
        alioquin si (c->vitium_genus == II)
        {
            v = p;
        }
        alioquin
        {
            redde VII;
        }
    }
    si (c->mendax_primus != ZEPHYRUM && p == c->mendax_primus)
    {
        v = congruentia_adde(v, I, p);
    }
    si (c->mendax_index_activus && index == c->mendax_index)
    {
        v = congruentia_adde(v, I, p);
    }
    *punctum  = x;
    *valor    = v;
    redde INTERPOLATIO_UTILE;
}

interior vacuum
_contextus_initium (
         Contextus* c,
    constans Notum* n)
{
    i32 j;

    c->notum                 = n;
    c->vocationes            = ZEPHYRUM;
    c->mendax_primus         = ZEPHYRUM;
    c->mendax_index          = ZEPHYRUM;
    c->mendax_index_activus  = FALSUM;
    c->erratum_ad            = ZEPHYRUM;
    c->inutile_semper        = FALSUM;
    c->puncta_iterata        = FALSUM;
    c->oppositi              = FALSUM;
    c->vitium_ad             = ZEPHYRUM;
    c->vitium_genus          = ZEPHYRUM;
    c->inutile_a_primo       = ZEPHYRUM;
    c->erratum_a_primo       = ZEPHYRUM;
    c->vocationes_primi      = ZEPHYRUM;
    c->numerus_primorum      = ZEPHYRUM;
    c->mendax_omnis          = FALSUM;
    c->speculum              = FALSUM;
    per (j = ZEPHYRUM; j < XVI; j++)
    {
        c->primi_visi[j] = ZEPHYRUM;
    }
}

/* magnus fortuitus |m| < 2^bita, signo fortuito */
interior Magnus
_magnus_fortuitus (
       Sors* s,
        i32  bita,
    Piscina* piscina)
{
    Magnus m        = magnus_ex_s64(ZEPHYRUM);
       i32 reliqua  = bita;

    dum (reliqua > ZEPHYRUM)
    {
        i32 lata    = reliqua >= XXXII ? XXXII : reliqua;
        i32 verbum  = sors_proximum(s);

        si (lata < XXXII)
        {
            verbum &= (i32)(((i32)I << lata) - (i32)I);
        }
        m = magnus_adde(magnus_multiplica(m, magnus_ex_s64(
            (s64)((i64)I << lata)), piscina),
            magnus_ex_s64((s64)verbum),
            piscina);
        reliqua -= lata;
    }
    si (sors_intra(s, II) == ZEPHYRUM)
    {
        m = magnus_nega(m, piscina);
    }
    redde m;
}

/* polynomium Notum in forma Polynomium (densum cum zephyris) */
interior b32
_notum_ad_polynomium (
    constans Notum* n,
           Piscina* piscina,
        Polynomium* exitus)
{
       i32  amplitudo = (n->numerus - I) * n->passus + I;
    Magnus* densa     = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)amplitudo * magnitudo(Magnus));
    i32 k;

    per (k = ZEPHYRUM; k < amplitudo; k++)
    {
        densa[k] = magnus_ex_s64(ZEPHYRUM);
    }
    per (k = ZEPHYRUM; k < n->numerus; k++)
    {
        densa[k * n->passus] = n->coefficientes[k];
    }
    redde polynomium_ex_coefficientibus(densa, amplitudo, n->imus,
        piscina,
        exitus);
}

interior InterpolatioQuaestio
_quaestio_noti (
    constans Notum* n,
               i32  limes_bitorum)
{
    InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();

    q.gradus_imus    = n->imus;
    q.gradus_summus  = n->imus + (s32)(n->passus * (n->numerus - I));
    q.passus         = n->passus;
    q.limes_bitorum  = limes_bitorum;
    redde q;
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior vacuum
_probare_ordinariam (
    CredoContextus* c)
{
    InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();

    (vacuum)c;
    CREDO_AEQUALIS_S32(q.gradus_imus, ZEPHYRUM);
    CREDO_AEQUALIS_S32(q.gradus_summus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(q.passus, I);
    CREDO_AEQUALIS_I32(q.limes_bitorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32(q.modulus_primi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(q.primus_limes, (i32)0xFFFFFFFFUL);
    CREDO_AEQUALIS_I32(q.puncta_addita, I);
    CREDO_AEQUALIS_I32(q.conatus_maximi, (i32)0x100000UL);
    CREDO_AEQUALIS_I32(q.modus, INTERPOLATIO_PROBATUM);
    CREDO_AEQUALIS_I32(q.bita_securitatis, LXIV);
    CREDO_VERUM(q.semen == ZEPHYRUM);
    CREDO_AEQUALIS_I32(q.semen_origo, INTERPOLATIO_SEMEN_DATUM);
    CREDO_AEQUALIS_I32(q.verificationis_imus, (i32)0x40000000UL);
    CREDO_AEQUALIS_I32(q.verificationis_summus, (i32)0x80000000UL);
}

/* Lagrange: polynomia fortuita mod p ex punctis distinctis fortuitis */
interior vacuum
_probare_lagrange_fortuitam (
    CredoContextus* c)
{
         Piscina* piscina = c->piscina;
    constans i32  primi[] = { 17U, 998244353U, 4294967291U,
        4294967161U };
    Sors s;
     i32 t;

    sors_seminare(&s, (i64)MMXXVI, ZEPHYRUM);
    per (t = ZEPHYRUM; t < CC; t++)
    {
        i32  p        = primi[t % IV];
        i32  limes    = p < CCI ? p : CCI;
        i32  numerus  = sors_intra(&s, limes) + I;
        i32* coef   = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus
            * magnitudo(i32));
        i32* x      = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus
            * magnitudo(i32));
        i32* y      = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus
            * magnitudo(i32));
        i32* r      = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus
            * magnitudo(i32));
        i32 i;
        i32 j;

        per (j = ZEPHYRUM; j < numerus; j++)
        {
            coef[j] = sors_intra(&s, p);
        }
        /* puncta distincta: p parvus -> permutatio prima; aliter fortuita
         * cum reiectione */
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            b32 novum;

            fac
            {
                x[i] = p == XVII ? (i * VII + III) % p : sors_intra(&s,
                    p);
                novum = VERUM;
                per (j = ZEPHYRUM; j < i; j++)
                {
                    si (x[j] == x[i])
                    {
                        novum = FALSUM;
                    }
                }
            } dum (!novum);
            y[i] = ZEPHYRUM;
            per (j = numerus; j > ZEPHYRUM; j--)
            {
                y[i] = congruentia_adde(congruentia_multiplica(y[i],
                    x[i], p),
                    coef[j - I], p);
            }
        }
        CREDO_NECESSE(interpolatio_lagrange_modulo(x, y, numerus, p,
            piscina,
            r));
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            CREDO_AEQUALIS_I32(r[j], coef[j]);
        }
    }
}

interior vacuum
_probare_lagrange_refutationes (
    CredoContextus* c)
{
         Piscina* piscina    = c->piscina;
    constans i32  x_iter[]   = { 1U, 2U, 1U };
    constans i32  x_bona[]   = { 1U, 2U, 3U };
    constans i32  x_magna[]  = { 1U, 2U, 17U };
    constans i32  y_bona[]   = { 5U, 6U, 7U };
    constans i32  y_magna[]  = { 5U, 6U, 17U };
             i32  r[III]     = { 99U, 99U, 99U };

    CREDO_FALSUM(interpolatio_lagrange_modulo(x_iter, y_bona, III, XVII,
        piscina, r));
    CREDO_AEQUALIS_I32(r[ZEPHYRUM], 99U);
    CREDO_FALSUM(interpolatio_lagrange_modulo(x_bona, y_bona, III, XV,
        piscina, r));
    CREDO_FALSUM(interpolatio_lagrange_modulo(x_magna, y_bona, III,
        XVII,
        piscina, r));
    CREDO_FALSUM(interpolatio_lagrange_modulo(x_bona, y_magna, III,
        XVII,
        piscina, r));
    CREDO_FALSUM(interpolatio_lagrange_modulo(x_bona, y_bona, ZEPHYRUM,
        XVII, piscina, r));
    CREDO_AEQUALIS_I32(r[ZEPHYRUM], 99U);
    /* XVIII puncta modulo XVII: iteratum necessario */
    {
        i32 x18[XVIII];
        i32 y18[XVIII];
        i32 r18[XVIII];
        i32 j;

        per (j = ZEPHYRUM; j < XVIII; j++)
        {
            x18[j] = j % XVII;
            y18[j] = j % XVII;
            r18[j] = 99U;
        }
        CREDO_FALSUM(interpolatio_lagrange_modulo(x18, y18, XVIII, XVII,
            piscina, r18));
        CREDO_AEQUALIS_I32(r18[ZEPHYRUM], 99U);
    }
    CREDO_VERUM(interpolatio_lagrange_modulo(x_bona, y_bona, III, XVII,
        piscina, r));
    /* 5, 6, 7 in 1, 2, 3: recta 4 + t */
    CREDO_AEQUALIS_I32(r[ZEPHYRUM], IV);
    CREDO_AEQUALIS_I32(r[I], I);
    CREDO_AEQUALIS_I32(r[II], ZEPHYRUM);
}

/* polynomia Laurentiana fortuita: coefficientes usque ad CC bita,
 * passus 1, 2, 4, imus negativus */
interior vacuum
_probare_polynomium_fortuitum (
    CredoContextus* c)
{
         Piscina* piscina   = c->piscina;
    constans i32  passus[]  = { 1U, 2U, 4U };
            Sors  s;
             i32  t;

    sors_seminare(&s, (i64)MMXXVI, I);
    per (t = ZEPHYRUM; t < XXIV; t++)
    {
                       Notum  n;
                   Contextus  ctx;
        InterpolatioQuaestio  q;
                  Polynomium  expectatum;
                  Polynomium  f;
                      Magnus* coef;
                         i32  bita = sors_intra(&s, CC)
                             + I;
                         i32 k;

        n.numerus  = sors_intra(&s, XL) + I;
        n.imus     = sors_inter(&s, -XX, V);
        n.passus   = passus[t % III];
        coef      = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)n.numerus * magnitudo(Magnus));
        per (k = ZEPHYRUM; k < n.numerus; k++)
        {
            coef[k] = sors_intra(&s, V)
                == ZEPHYRUM ? magnus_ex_s64(ZEPHYRUM)
                : _magnus_fortuitus(&s, bita, piscina);
        }
        n.coefficientes = coef;
        _contextus_initium(&ctx, &n);
        q = _quaestio_noti(&n, bita);
        CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
        CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
            piscina,
            &f));
        CREDO_VERUM(polynomium_aequalis(f, expectatum));
    }
}

/* limes in margine: coefficientes +-(2^CC - 1) cum limite CC exacti;
 * limes CXCIX refutatur (limes probatus falsus) */
interior vacuum
_probare_limitem (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[III];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;
                  Magnus  apex;

    apex = magnus_subtrahe(magnus_potentia(magnus_ex_s64(II), CC,
        piscina),
        magnus_ex_s64(I), piscina);
    coef[ZEPHYRUM]   = apex;
    coef[I]          = magnus_nega(apex, piscina);
    coef[II]         = magnus_ex_s64(-VII);
    n.coefficientes  = coef;
    n.numerus        = III;
    n.imus           = -II;
    n.passus         = I;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));

    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, CC);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));

    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, CXCIX);
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
}

/* mendacia: primus unus totus (primus additus capit), punctum unum
 * (punctum additum capit) */
interior vacuum
_probare_mendacia (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[IV];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  f;
              Polynomium  signum;
                     i32  primi_omnes[XVI];
                     i32  numerus_omnium;
                     i32  j;

    coef[ZEPHYRUM]   = magnus_ex_s64(III);
    coef[I]          = magnus_ex_s64(-I);
    coef[II]         = magnus_ex_s64(ZEPHYRUM);
    coef[III]        = magnus_ex_s64(XLII);
    n.coefficientes  = coef;
    n.numerus        = IV;
    n.imus           = -I;
    n.passus         = I;
    signum = polynomium_constans(magnus_ex_s64(DCLXVI),
        piscina);

    /* cursus verus: primi omnes noti */
    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, C);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    numerus_omnium = ctx.numerus_primorum;
    CREDO_NECESSE(numerus_omnium >= III && numerus_omnium <= XVI);
    per (j = ZEPHYRUM; j < numerus_omnium; j++)
    {
        primi_omnes[j] = ctx.primi_visi[j];
    }

    /* quisque primus mendax per se refutatur */
    per (j = ZEPHYRUM; j < numerus_omnium; j++)
    {
        f = signum;
        _contextus_initium(&ctx, &n);
        ctx.mendax_primus = primi_omnes[j];
        CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q,
            piscina,
            &f));
        CREDO_VERUM(polynomium_aequalis(f, signum));
    }

    /* punctum unum mendax in OMNI primo (mendacium constans): index <
     * K interpolationem corrumpit; index K (punctum additum) solum per
     * probationem puncti additi capitur - restitutio ipsa recta esset */
    per (j = ZEPHYRUM; j < V; j++)
    {
        _contextus_initium(&ctx, &n);
        ctx.mendax_index_activus  = VERUM;
        ctx.mendax_index          = j;
        CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q,
            piscina,
            &f));
    }
}

/* ERRATUM in vocatione tertia: FALSUM post tres vocationes exacte;
 * INUTILE semper: FALSUM post conatus_maximi vocationes */
interior vacuum
_probare_erratum_et_inutile (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[II];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  f;

    coef[ZEPHYRUM]   = magnus_ex_s64(V);
    coef[I]          = magnus_ex_s64(-II);
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    q                = _quaestio_noti(&n, X);

    _contextus_initium(&ctx, &n);
    ctx.erratum_ad = III;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    CREDO_AEQUALIS_I32(ctx.vocationes, III);

    _contextus_initium(&ctx, &n);
    ctx.inutile_semper  = VERUM;
    q.conatus_maximi    = L;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    CREDO_AEQUALIS_I32(ctx.vocationes, L);
}

/* puncta iterata (x = index/2 + 1; passus 2: etiam -x idem y) omittuntur */
interior vacuum
_probare_puncta_iterata (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[V];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;
                     i32  k;

    per (k = ZEPHYRUM; k < V; k++)
    {
        coef[k] = magnus_ex_s64((s64)k * (s64)k - (s64)VII);
    }
    n.coefficientes  = coef;
    n.numerus        = V;
    n.imus           = -III;
    n.passus         = II;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    ctx.puncta_iterata  = VERUM;
    q                   = _quaestio_noti(&n, VIII);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
}

/* filtrum primorum: p = 1 (mod 4) infra XVIII -> 17, deinde 13 */
interior vacuum
_probare_filtrum_primorum (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[II];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;
                     i32  j;

    coef[ZEPHYRUM]   = magnus_ex_s64(-I);
    coef[I]          = magnus_ex_s64(I);
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));

    _contextus_initium(&ctx, &n);
    q                 = _quaestio_noti(&n, II);
    q.residuum_primi  = I;
    q.modulus_primi   = IV;
    q.primus_limes    = XVIII;
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(ctx.numerus_primorum, II);
    CREDO_AEQUALIS_I32(ctx.primi_visi[ZEPHYRUM], XVII);
    CREDO_AEQUALIS_I32(ctx.primi_visi[I], XIII);

    /* primi magni: omnes 1 (mod 4) */
    _contextus_initium(&ctx, &n);
    q.primus_limes   = (i32)0xFFFFFFFFUL;
    q.limes_bitorum  = CXXVIII;
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_NECESSE(ctx.numerus_primorum >= V
        && ctx.numerus_primorum <= XVI);
    per (j = ZEPHYRUM; j < ctx.numerus_primorum; j++)
    {
        CREDO_AEQUALIS_I32(ctx.primi_visi[j] % IV, I);
        CREDO_VERUM(congruentia_est_primus(ctx.primi_visi[j]));
    }

    /* primi desunt: infra IV unus solus (3), filtrum nullum, limes magnus */
    _contextus_initium(&ctx, &n);
    q               = _quaestio_noti(&n, C);
    q.primus_limes  = IV;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
}

/* gradus 0: numerus integer CCC bitorum trans primos */
interior vacuum
_probare_scalarem (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[I];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  f;
                    Sors  s;

    sors_seminare(&s, (i64)MMXXVI, II);
    coef[ZEPHYRUM]   = _magnus_fortuitus(&s, CCC, piscina);
    n.coefficientes  = coef;
    n.numerus        = I;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, CCC);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(magnus_aequalis(polynomium_coefficiens(f, ZEPHYRUM),
        coef[ZEPHYRUM]));
    CREDO_AEQUALIS_S32(polynomium_gradus_imus(f), ZEPHYRUM);
    CREDO_AEQUALIS_S32(polynomium_gradus_summus(f), ZEPHYRUM);
}

/* quaestiones invalidae: exitus intactus */
interior vacuum
_probare_quaestiones_invalidas (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[II];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  f;
              Polynomium  signum;

    coef[ZEPHYRUM]   = magnus_ex_s64(I);
    coef[I]          = magnus_ex_s64(II);
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = II;
    signum = polynomium_constans(magnus_ex_s64(DCLXVI),
        piscina);
    _contextus_initium(&ctx, &n);

    f         = signum;
    q         = _quaestio_noti(&n, X);
    q.passus  = ZEPHYRUM;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                = _quaestio_noti(&n, X);
    q.gradus_summus  = -I;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                = _quaestio_noti(&n, X);
    q.gradus_summus  = III;              /* 3 - 0 non divisibile per 2 */
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                 = _quaestio_noti(&n, X);
    q.conatus_maximi  = ZEPHYRUM;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q = _quaestio_noti(&n, X);
    CREDO_FALSUM(interpolatio_polynomium(NIHIL, &ctx, &q, piscina, &f));
    /* recensio T1: puncta_addita exundans (M1), fenestra extra fines
     * exponentium (M5), limes nimius (L1), filtrum non admissibile (M4) */
    q                = _quaestio_noti(&n, X);
    q.puncta_addita  = (i32)0xFFFFFFFFUL;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q              = _quaestio_noti(&n, X);
    q.gradus_imus  = -(s32)POLYNOMIUM_EXPONENS_MAXIMUS - II;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                = _quaestio_noti(&n, X);
    q.gradus_summus  = (s32)POLYNOMIUM_EXPONENS_MAXIMUS + II;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                = _quaestio_noti(&n, X);
    q.gradus_imus    = (s32)(-(s64)0x7FFFFFFFL - (s64)I);
    q.gradus_summus  = (s32)0x7FFFFFFFL;
    q.passus         = I;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                = _quaestio_noti(&n, X);
    q.limes_bitorum  = (i32)0xFFFFFFFFUL;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                = _quaestio_noti(&n, X);
    q.limes_bitorum  = INTERPOLATIO_LIMES_MAXIMUS + I;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                 = _quaestio_noti(&n, X);
    q.residuum_primi  = II;
    q.modulus_primi   = IV;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    q                 = _quaestio_noti(&n, X);
    q.residuum_primi  = ZEPHYRUM;
    q.modulus_primi   = IV;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, signum));
    CREDO_AEQUALIS_I32(ctx.vocationes, ZEPHYRUM);
}

/* recensio T1 M3: margo limitis primorum. Primi parvi (263, 257, 251,
 * 241) bitum superfluum primorum prope 2^32 non habent: summa floor(log2)
 * 8 + 8 + 7 >= 17 = limes + 1 tres primos poscit; laxitas nulla aut
 * log2 sursum duos tantum daret et primus additus refutaret. */
interior vacuum
_probare_marginem_primorum (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[II];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;

    coef[ZEPHYRUM]   = magnus_ex_s64(LXV * M + DXXXV);
    coef[I]          = magnus_ex_s64(-(s64)(LXV * M + DXXXV));
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    q               = _quaestio_noti(&n, XVI);
    q.primus_limes  = CCLXIV;
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(ctx.numerus_primorum, IV);
    CREDO_AEQUALIS_I32(ctx.primi_visi[ZEPHYRUM], CCLXIII);
    CREDO_AEQUALIS_I32(ctx.primi_visi[I], CCLVII);
    CREDO_AEQUALIS_I32(ctx.primi_visi[II], CCLI);
    CREDO_AEQUALIS_I32(ctx.primi_visi[III], CCXLI);
}

/* recensio T1 M4, L2: residuum >= modulus reducitur (5 mod 4 = 1 mod
 * 4); primi qui satis y distincta non dant omittuntur (passus 8: 17 dat
 * 16/8 = 2 < 3) */
interior vacuum
_probare_filtrum_et_aptitudinem (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[II];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;

    coef[ZEPHYRUM]   = magnus_ex_s64(-I);
    coef[I]          = magnus_ex_s64(I);
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    q                 = _quaestio_noti(&n, II);
    q.residuum_primi  = V;
    q.modulus_primi   = IV;
    q.primus_limes    = XVIII;
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(ctx.numerus_primorum, II);
    CREDO_AEQUALIS_I32(ctx.primi_visi[ZEPHYRUM], XVII);
    CREDO_AEQUALIS_I32(ctx.primi_visi[I], XIII);

    /* 3 + 5 t^8, passus 8: 17 omittitur; 13 (3 bita), 11 (3) >= 5;
     * additus 7 */
    coef[ZEPHYRUM]  = magnus_ex_s64(III);
    coef[I]         = magnus_ex_s64(V);
    n.passus        = VIII;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    q                 = _quaestio_noti(&n, IV);
    q.primus_limes    = XVIII;
    q.conatus_maximi  = C;
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(ctx.numerus_primorum, III);
    CREDO_AEQUALIS_I32(ctx.primi_visi[ZEPHYRUM], XIII);
    CREDO_AEQUALIS_I32(ctx.primi_visi[I], XI);
    CREDO_AEQUALIS_I32(ctx.primi_visi[II], VII);
}

/* recensio T1 L3a, N2: x et p - x idem y (passus 2) - omittitur, sed
 * valor congruere debet; passus falsus (exponens -2 extra classem)
 * statim in pari primo refutatur */
interior vacuum
_probare_oppositos (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[IV];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;
                  Magnus  densa[VII];
                     i32  k;

    coef[ZEPHYRUM]   = magnus_ex_s64(V);
    coef[I]          = magnus_ex_s64(-II);
    coef[II]         = magnus_ex_s64(VII);
    coef[III]        = magnus_ex_s64(I);
    n.coefficientes  = coef;
    n.numerus        = IV;
    n.imus           = -III;
    n.passus         = II;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    ctx.oppositi  = VERUM;
    q             = _quaestio_noti(&n, IV);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(ctx.vocationes, XVIII);

    /* 5 t^-3 + t^-2 + t^3 cum passu 2 declarato: x et -x different */
    per (k = ZEPHYRUM; k < VII; k++)
    {
        densa[k] = magnus_ex_s64(ZEPHYRUM);
    }
    densa[ZEPHYRUM]  = magnus_ex_s64(V);
    densa[I]         = magnus_ex_s64(I);
    densa[VI]        = magnus_ex_s64(I);
    n.coefficientes  = densa;
    n.numerus        = VII;
    n.imus           = -III;
    n.passus         = I;
    _contextus_initium(&ctx, &n);
    ctx.oppositi  = VERUM;
    q             = _quaestio_noti(&n, IV);
    q.passus      = II;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    CREDO_AEQUALIS_I32(ctx.vocationes, II);
}

/* recensio T1 L3b: punctum 0 omittitur (INUTILE, M2); punctum p, valor
 * p, status ignotus = ERRATUM post duas vocationes */
interior vacuum
_probare_puncta_vitiosa (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[III];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;
              Polynomium  signum;
                     i32  mundae;
                     i32  genus;

    coef[ZEPHYRUM]   = magnus_ex_s64(IV);
    coef[I]          = magnus_ex_s64(ZEPHYRUM);
    coef[II]         = magnus_ex_s64(-IX);
    n.coefficientes  = coef;
    n.numerus        = III;
    n.imus           = -I;
    n.passus         = I;
    signum           = polynomium_constans(magnus_ex_s64(DCLXVI),
        piscina);
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    q = _quaestio_noti(&n, VIII);

    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    mundae = ctx.vocationes;

    _contextus_initium(&ctx, &n);
    ctx.vitium_ad     = II;
    ctx.vitium_genus  = ZEPHYRUM;
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(ctx.vocationes, mundae + I);

    per (genus = I; genus <= III; genus++)
    {
        f = signum;
        _contextus_initium(&ctx, &n);
        ctx.vitium_ad     = II;
        ctx.vitium_genus  = genus;
        CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q,
            piscina, &f));
        CREDO_AEQUALIS_I32(ctx.vocationes, II);
        CREDO_VERUM(polynomium_aequalis(f, signum));
    }
}

/* recensio T1 L3c-e, L3g: polynomium nullum; coefficiens unus extra 0;
 * exponentes magni; sine punctis additis */
interior vacuum
_probare_formas_extremas (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[III];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  expectatum;
              Polynomium  f;
                     s32  initia[II];
                     i32  j;

    /* nullum, limes 0 et X */
    coef[ZEPHYRUM]   = magnus_ex_s64(ZEPHYRUM);
    coef[I]          = magnus_ex_s64(ZEPHYRUM);
    coef[II]         = magnus_ex_s64(ZEPHYRUM);
    n.coefficientes  = coef;
    n.numerus        = III;
    n.imus           = -II;
    n.passus         = I;
    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, ZEPHYRUM);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_est_nullum(f));
    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, X);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_est_nullum(f));

    /* K = 1: -3 t^-7 et 2^90 t^9 */
    coef[ZEPHYRUM]  = magnus_ex_s64(-III);
    n.numerus       = I;
    n.imus          = -VII;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, II);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    coef[ZEPHYRUM]  = magnus_potentia(magnus_ex_s64(II), XC, piscina);
    n.imus          = IX;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    q = _quaestio_noti(&n, XCI);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));

    /* exponentes magni, passus 4, K = 3: imus -1000001 et +999999 */
    initia[ZEPHYRUM]  = -(s32)(M * M + I);
    initia[I]         = (s32)(M * M - I);
    per (j = ZEPHYRUM; j < II; j++)
    {
        coef[ZEPHYRUM]  = magnus_ex_s64(VII);
        coef[I]         = magnus_ex_s64(-III);
        coef[II]        = magnus_ex_s64(XI);
        n.numerus       = III;
        n.imus          = initia[j];
        n.passus        = IV;
        CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
        _contextus_initium(&ctx, &n);
        q = _quaestio_noti(&n, V);
        CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
            piscina, &f));
        CREDO_VERUM(polynomium_aequalis(f, expectatum));
    }

    /* sine punctis additis: K vocationes per primum exacte */
    n.imus    = -II;
    n.passus  = I;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    q                = _quaestio_noti(&n, XL);
    q.puncta_addita  = ZEPHYRUM;
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(ctx.vocationes, III * ctx.numerus_primorum);
}

/* recensio T1 L3f: defectus in primo ADDITO: INUTILE totus (conatus
 * consumuntur), ERRATUM in vocatione eius prima */
interior vacuum
_probare_defectum_additi (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
                  Magnus  coef[II];
                   Notum  n;
               Contextus  ctx;
    InterpolatioQuaestio  q;
              Polynomium  f;
              Polynomium  signum;
                     i32  primi;
                     i32  restitutionis;

    coef[ZEPHYRUM]   = magnus_ex_s64(XIII);
    coef[I]          = magnus_ex_s64(-I);
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    signum           = polynomium_constans(magnus_ex_s64(DCLXVI),
        piscina);
    q                 = _quaestio_noti(&n, LXX);
    q.conatus_maximi  = XL;

    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_polynomium(_aestimator, &ctx, &q,
        piscina,
        &f));
    primi          = ctx.numerus_primorum;
    restitutionis  = primi - I;
    CREDO_NECESSE(primi >= II);

    f = signum;
    _contextus_initium(&ctx, &n);
    ctx.inutile_a_primo = primi;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    CREDO_AEQUALIS_I32(ctx.vocationes, restitutionis * III + XL);
    CREDO_VERUM(polynomium_aequalis(f, signum));

    _contextus_initium(&ctx, &n);
    ctx.erratum_a_primo = primi;
    CREDO_FALSUM(interpolatio_polynomium(_aestimator, &ctx, &q, piscina,
        &f));
    CREDO_AEQUALIS_I32(ctx.vocationes, restitutionis * III + I);
    CREDO_VERUM(polynomium_aequalis(f, signum));
}


/* ==================================================
 * Verificatio (interpolatio-verificatio-plan T1)
 * ================================================== */

/* verificator ex aestimatore probationis: x = index + 1 (x = q
 * INUTILE: r = 1), y = x^passus (c = passus) */
interior InterpolatioVerificator
_verificator (
     Contextus* ctx,
           i32  passus)
{
    InterpolatioVerificator v;

    v.aestimare        = _aestimator;
    v.contextus        = ctx;
    v.multiplicitas    = passus;
    v.inutilia_maxima  = I;
    v.titulus          = "probatio";
    v.communia         = "nihil";
    redde v;
}

interior InterpolatioQuaestio
_quaestio_verificandi (
    constans Notum* n,
               i32  limes_bitorum,
               i64  semen)
{
    InterpolatioQuaestio q = _quaestio_noti(n, limes_bitorum);

    q.modus = INTERPOLATIO_VERIFICATUM;
    q.semen = semen;
    redde q;
}

/* k verus: primus k cum prod primi[0..k-1] > 2 max |c| */
interior i32
_k_verus (
    constans Notum* n,
      constans i32* primi,
               i32  numerus,
           Piscina* piscina)
{
    Magnus maximus  = magnus_ex_s64(ZEPHYRUM);
    Magnus M_k      = magnus_ex_s64(I);
    Magnus bis;
       i32 k;

    per (k = ZEPHYRUM; k < n->numerus; k++)
    {
        Magnus a = magnus_absolutum(n->coefficientes[k], piscina);

        si (magnus_compara(a, maximus) > ZEPHYRUM)
        {
            maximus = a;
        }
    }
    bis = magnus_adde(maximus, maximus, piscina);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        M_k = magnus_multiplica(M_k, magnus_ex_s64((s64)primi[k]),
            piscina);
        si (magnus_compara(bis, M_k) < ZEPHYRUM)
        {
            redde k + I;
        }
    }
    redde ZEPHYRUM;
}

/* notum "magnum": VI coefficientes C bitorum, limes CC (k_max VII, k
 * verus IV in primis prope 2^32) */
interior vacuum
_notum_magnum (
      Notum* n,
     Magnus* coef,
    Piscina* piscina)
{
    Sors s;
     i32 k;

    sors_seminare(&s, (i64)MMXXVI, III);
    per (k = ZEPHYRUM; k < VI; k++)
    {
        coef[k] = _magnus_fortuitus(&s, C, piscina);
    }
    coef[ZEPHYRUM]   = magnus_adde(magnus_potentia(magnus_ex_s64(II),
        XCIX, piscina), coef[ZEPHYRUM], piscina);
    n->coefficientes  = coef;
    n->numerus        = VI;
    n->imus           = -II;
    n->passus         = I;
}

/* VERIFICATUM = PROBATUM in polynomiis fortuitis (eadem XXIV ac
 * "polynomium fortuitum"); relatio sibi constans */
interior vacuum
_probare_verificatum_aequale (
    CredoContextus* c)
{
         Piscina* piscina   = c->piscina;
    constans i32  passus[]  = { 1U, 2U, 4U };
            Sors  s;
             i32  t;

    sors_seminare(&s, (i64)MMXXVI, I);
    per (t = ZEPHYRUM; t < XXIV; t++)
    {
                          Notum  n;
                      Contextus  ctx;
                      Contextus  ctx_v;
           InterpolatioQuaestio  q;
        InterpolatioVerificator  v;
            InterpolatioRelatio  r;
                     Polynomium  expectatum;
                     Polynomium  f;
                         Magnus* coef;
                            i32  bita = sors_intra(&s, CC) + I;
                            i32  k;

        n.numerus  = sors_intra(&s, XL) + I;
        n.imus     = sors_inter(&s, -XX, V);
        n.passus   = passus[t % III];
        coef = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)n.numerus
            * magnitudo(Magnus));
        per (k = ZEPHYRUM; k < n.numerus; k++)
        {
            coef[k] = sors_intra(&s, V) == ZEPHYRUM
                ? magnus_ex_s64(ZEPHYRUM) : _magnus_fortuitus(&s, bita,
                piscina);
        }
        n.coefficientes = coef;
        CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
        _contextus_initium(&ctx, &n);
        _contextus_initium(&ctx_v, &n);
        v = _verificator(&ctx_v, n.passus);
        q = _quaestio_verificandi(&n, bita, (i64)t + (i64)I);
        CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx,
            &v, &q, piscina, &f, &r));
        CREDO_VERUM(polynomium_aequalis(f, expectatum));
        si (r.genus == INTERPOLATIO_VERIFICATUM)
        {
            CREDO_AEQUALIS_I32(r.status, INTERPOLATIO_ACCEPTUM);
            CREDO_VERUM(r.numerus_primorum < r.k_maximus);
            CREDO_AEQUALIS_I32(r.primus_additus, ZEPHYRUM);
            CREDO_VERUM(r.bita_erroris >= LXIV);
        }
        alioquin
        {
            CREDO_AEQUALIS_I32(r.status, INTERPOLATIO_AD_LIMITEM);
            CREDO_AEQUALIS_I32(r.numerus_primorum, r.k_maximus);
            CREDO_VERUM(r.primus_additus != ZEPHYRUM);
        }
        CREDO_AEQUALIS_I32(r.k_maximus, r.numerus_primorum
            + (r.genus == INTERPOLATIO_VERIFICATUM ? r.k_maximus
            - r.numerus_primorum : ZEPHYRUM));
        CREDO_VERUM(r.generator != NIHIL);
    }
}

/* modulus parvus: notum magnum (C bita, limes CC) - VERIFICATUM in k
 * vero (> 1) primis exacte, effectus rectus. Plantae "probatio Q cum Q"
 * et "primus probationis inter usos" hic rubent (stadium 1 acciperent) */
interior vacuum
_probare_modulum_parvum (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[VI];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r;
                 Polynomium  expectatum;
                 Polynomium  f;
                        i32  k_verus;

    _notum_magnum(&n, coef, piscina);
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    _contextus_initium(&ctx_v, &n);
    v = _verificator(&ctx_v, I);
    q = _quaestio_verificandi(&n, CC, (i64)VII);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    k_verus = _k_verus(&n, r.primi, r.k_maximus, piscina);
    CREDO_AEQUALIS_I32(r.genus, INTERPOLATIO_VERIFICATUM);
    CREDO_AEQUALIS_I32(r.k_maximus, VII);
    CREDO_AEQUALIS_I32(k_verus, IV);
    CREDO_AEQUALIS_I32(r.numerus_primorum, k_verus);
    CREDO_VERUM(r.aestimationes == (i64)k_verus * (i64)(VI + I));
}

/* verificator numerans: probationes sui = k verus (una per stadium),
 * certificantes = m = relatio, vocationes verificatoris = m. Planta
 * "ansa m - 1" hic rubet. */
interior vacuum
_probare_numerationem (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[VI];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r;
                 Polynomium  f;

    _notum_magnum(&n, coef, piscina);
    _contextus_initium(&ctx, &n);
    _contextus_initium(&ctx_v, &n);
    v = _verificator(&ctx_v, I);
    q = _quaestio_verificandi(&n, CC, (i64)XI);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    CREDO_NECESSE(r.genus == INTERPOLATIO_VERIFICATUM);
    CREDO_AEQUALIS_I32(r.probationes_requisitae, IV);
    CREDO_AEQUALIS_I32(r.probationes_sui, r.numerus_primorum);
    CREDO_AEQUALIS_I32(r.probationes_certificantes,
        r.probationes_requisitae);
    CREDO_AEQUALIS_I32(ctx_v.vocationes, r.probationes_requisitae);
    CREDO_VERUM((i64)ctx.vocationes - r.aestimationes
        == (i64)r.probationes_sui);
    CREDO_AEQUALIS_I32(r.numerus_tractuum, r.probationes_sui
        + r.probationes_certificantes);
}

/* tractus referentiales: forma documentata in capite ab hac probatione
 * reimplementata reddit (primus, index) omnes ex semine. Plantae "q
 * idem omnibus", "index in [0, 2^16)", "ambitus a 2" hic rubent. */
interior vacuum
_probare_tractus_iterum (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[VI];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r;
                 Polynomium  f;
                       Sors  s;
                        i32  t;

    _notum_magnum(&n, coef, piscina);
    _contextus_initium(&ctx, &n);
    _contextus_initium(&ctx_v, &n);
    v = _verificator(&ctx_v, I);
    q = _quaestio_verificandi(&n, CC, (i64)XIII);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    CREDO_NECESSE(r.numerus_tractuum >= VI);
    sors_seminare(&s, q.semen, ZEPHYRUM);
    per (t = ZEPHYRUM; t < r.numerus_tractuum; t++)
    {
        i32 primus = ZEPHYRUM;
        i32 index;

        fac
        {
            b32 usus = FALSUM;
            i32 j;
            i32 cand = q.verificationis_imus + sors_intra(&s,
                q.verificationis_summus - q.verificationis_imus);

            per (j = ZEPHYRUM; j < r.tractus[t].gradus; j++)
            {
                si (r.primi[j] == cand)
                {
                    usus = VERUM;
                }
            }
            si (congruentia_est_primus(cand) && !usus)
            {
                primus = cand;
            }
        } dum (primus == ZEPHYRUM);
        /* aestimator: x = index + 1, x = q INUTILE */
        fac
        {
            index = sors_intra(&s, primus);
        } dum (index + I >= primus);
        CREDO_AEQUALIS_I32(r.tractus[t].primus, primus);
        CREDO_AEQUALIS_I32(r.tractus[t].index, index);
    }
}

/* verificator mendax (semper falsus), principalis rectus: DISCORDIA,
 * numquam acceptatio, exitus intactus */
interior vacuum
_probare_verificator_mendax (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[III];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r;
                 Polynomium  f;
                 Polynomium  signum;

    coef[ZEPHYRUM] = magnus_ex_s64(V);
    coef[I] = magnus_ex_s64(-XIII);
    coef[II] = magnus_ex_s64(II);
    n.coefficientes = coef;
    n.numerus = III;
    n.imus = -I;
    n.passus = I;
    signum = polynomium_constans(magnus_ex_s64(DCLXVI), piscina);
    f = signum;
    _contextus_initium(&ctx, &n);
    _contextus_initium(&ctx_v, &n);
    ctx_v.mendax_omnis  = VERUM;
    v                   = _verificator(&ctx_v, I);
    q                   = _quaestio_verificandi(&n, C, (i64)XVII);
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    CREDO_AEQUALIS_I32(r.status, INTERPOLATIO_DISCORDIA);
    CREDO_VERUM(polynomium_aequalis(f, signum));
    CREDO_VERUM(r.k_maximus >= III);
}

/* polynomium strictum: 2^80 - 1 cum limite LXXX - k verus = k_max = III,
 * ergo AD_LIMITEM, genus PROBATUM, primi et additus idem ac PROBATUM */
interior vacuum
_probare_polynomium_strictum (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[II];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r;
        InterpolatioRelatio  rp;
                 Polynomium  expectatum;
                 Polynomium  f;
                        i32  j;

    coef[ZEPHYRUM]  = magnus_subtrahe(magnus_potentia(magnus_ex_s64(II),
        LXXX, piscina), magnus_ex_s64(I), piscina);
    coef[I]          = magnus_ex_s64(-III);
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    CREDO_NECESSE(_notum_ad_polynomium(&n, piscina, &expectatum));
    _contextus_initium(&ctx, &n);
    _contextus_initium(&ctx_v, &n);
    v = _verificator(&ctx_v, I);
    q = _quaestio_verificandi(&n, LXXX, (i64)XIX);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    CREDO_VERUM(polynomium_aequalis(f, expectatum));
    CREDO_AEQUALIS_I32(r.genus, INTERPOLATIO_PROBATUM);
    CREDO_AEQUALIS_I32(r.status, INTERPOLATIO_AD_LIMITEM);
    CREDO_AEQUALIS_I32(r.k_maximus, III);
    CREDO_AEQUALIS_I32(r.numerus_primorum, III);
    _contextus_initium(&ctx, &n);
    q.modus = INTERPOLATIO_PROBATUM;
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &rp));
    CREDO_AEQUALIS_I32(rp.numerus_primorum, r.numerus_primorum);
    CREDO_AEQUALIS_I32(rp.primus_additus, r.primus_additus);
    per (j = ZEPHYRUM; j < r.numerus_primorum; j++)
    {
        CREDO_AEQUALIS_I32(rp.primi[j], r.primi[j]);
    }
    CREDO_VERUM(rp.aestimationes == r.aestimationes);
}

/* verificator speculi (f(1/x)): DISCORDIA. Praecondicio: f non
 * symmetricum (f(2) != f(1/2)). Planta "aestimator principalis ut
 * certificans" hic rubet. (Speculum per aestimatores laqueus: T2.) */
interior vacuum
_probare_speculum (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[III];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r;
                 Polynomium  f;
                        i32  dimidium;

    coef[ZEPHYRUM]   = magnus_ex_s64(IV);
    coef[I]          = magnus_ex_s64(-I);
    coef[II]         = magnus_ex_s64(IX);
    n.coefficientes  = coef;
    n.numerus        = III;
    n.imus           = -IV;
    n.passus         = IV;
    (vacuum)congruentia_inversa(II, 998244353U, &dimidium);
    CREDO_NECESSE(_aestima_notum(&n, II, 998244353U)
        != _aestima_notum(&n, dimidium, 998244353U));
    _contextus_initium(&ctx, &n);
    _contextus_initium(&ctx_v, &n);
    ctx_v.speculum  = VERUM;
    v               = _verificator(&ctx_v, IV);
    q               = _quaestio_verificandi(&n, C, (i64)XXIII);
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    CREDO_AEQUALIS_I32(r.status, INTERPOLATIO_DISCORDIA);
}

/* semina: duo semina primos probationum diversos; semen idem tractus
 * eosdem. Planta "semen neglectum" hic rubet. */
interior vacuum
_probare_semina (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[VI];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r1;
        InterpolatioRelatio  r2;
        InterpolatioRelatio  r3;
                 Polynomium  f;
                        i32  t;

    _notum_magnum(&n, coef, piscina);
    _contextus_initium(&ctx_v, &n);
    v = _verificator(&ctx_v, I);
    q = _quaestio_verificandi(&n, CC, (i64)I);
    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r1));
    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r3));
    q.semen = (i64)II;
    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r2));
    CREDO_NECESSE(r1.numerus_tractuum > ZEPHYRUM && r2.numerus_tractuum
        > ZEPHYRUM);
    CREDO_VERUM(r1.tractus[ZEPHYRUM].primus
        != r2.tractus[ZEPHYRUM].primus);
    CREDO_AEQUALIS_I32(r3.numerus_tractuum, r1.numerus_tractuum);
    per (t = ZEPHYRUM; t < r1.numerus_tractuum; t++)
    {
        CREDO_AEQUALIS_I32(r3.tractus[t].primus, r1.tractus[t].primus);
        CREDO_AEQUALIS_I32(r3.tractus[t].index, r1.tractus[t].index);
    }
}

/* vector limitis: K, limes, k_max, c, r, ambitus, s -> m */
interior i32
_m_vectoris (
    i32  coefficientes,
    i32  limes,
    i32  k_maximus,
    i32  multiplicitas,
    i32  inutilia,
    i32  ambitus_imus,
    i32  ambitus_summus,
    i32  s,
    b32* bene,
    i32* bita)
{
       InterpolatioQuaestio  q = interpolatio_quaestio_ordinaria();
    InterpolatioVerificator  v;
                        i32  m = ZEPHYRUM;
                    Piscina* piscina = piscina_generare_dynamicum(
                        "vector", 65536);

    q.gradus_imus            = ZEPHYRUM;
    q.gradus_summus          = (s32)coefficientes - (s32)I;
    q.limes_bitorum          = limes;
    q.residuum_primi         = I;
    q.modulus_primi          = IV;
    q.bita_securitatis       = s;
    q.verificationis_imus    = ambitus_imus;
    q.verificationis_summus  = ambitus_summus;
    v.aestimare              = _aestimator;
    v.contextus              = NIHIL;
    v.multiplicitas          = multiplicitas;
    v.inutilia_maxima        = inutilia;
    v.titulus                = "vector";
    v.communia               = "nihil";
    *bene = interpolatio_probationes_necessariae(&q, &v, k_maximus,
        piscina, &m, bita);
    piscina_destruere(piscina);
    redde m;
}

/* vectores limitis (emendatio T0: m EXACTE): 14x801 -> 4, 11x102 -> 4,
 * 3x50 -> 3 (recipe floor/ceil 4 daret); vectores ubi error unius biti m
 * mutat (c 8/4, k_max - 1 / k_max, limes + 1 / limes, q_min ambitus
 * angusti / 2^30); limites refutati */
interior vacuum
_probare_vectores_limitis (
    CredoContextus* c)
{
    constans i32 A30 = (i32)0x40000000UL;
    constans i32 A31 = (i32)0x80000000UL;
             b32 bene;
             i32 bita;

    (vacuum)c;
    CREDO_AEQUALIS_I32(_m_vectoris(DLXI, DCLXII, XXIII, VIII, IX, A30,
        A31,
        LXIV, &bene, &bita), IV);
    CREDO_VERUM(bene && bita >= LXIV);
    CREDO_AEQUALIS_I32(_m_vectoris(LXXIII, LXXXII, III, VIII, IX, A30,
        A31,
        LXIV, &bene, &bita), IV);
    CREDO_VERUM(bene && bita >= LXIV);
    CREDO_AEQUALIS_I32(_m_vectoris(XXXVII, XXXIV, II, VIII, IX, A30,
        A31,
        LXIV, &bene, &bita), III);
    CREDO_VERUM(bene && bita >= LXIV);
    /* c: 8 -> 4, 4 -> 3 */
    CREDO_AEQUALIS_I32(_m_vectoris(IX, C, V, VIII, IX, A30, A31, LXIV,
        &bene, &bita), IV);
    CREDO_AEQUALIS_I32(_m_vectoris(IX, C, V, IV, IX, A30, A31, LXIV,
        &bene, &bita), III);
    /* unio per k_max - 1 stadia: 4 (per k_max: 5) */
    CREDO_AEQUALIS_I32(_m_vectoris(DCCXCVII, CCC, XXX, VIII, IX, A30,
        A31,
        LXIV, &bene, &bita), IV);
    /* floor((limes + 1)/30): 4 (floor(limes/30): 3) */
    CREDO_AEQUALIS_I32(_m_vectoris(XVIII, XXIX, X, VIII, IX, A30, A31,
        LXIV, &bene, &bita), IV);
    /* ambitus angustus [4096, 8192), 1 mod 4: 233 primi, N = 229 -> 3
     * (2^30 fixum: 1) */
    CREDO_AEQUALIS_I32(_m_vectoris(V, XX, III, IV, I, ((i32)0x1000UL),
        (i32)8192, XVI, &bene, &bita), III);
    CREDO_VERUM(bene && bita >= XVI);
    /* k_max <= 1: nulla probatio */
    CREDO_AEQUALIS_I32(_m_vectoris(XXXVII, XIV, I, VIII, IX, A30, A31,
        LXIV, &bene, &bita), ZEPHYRUM);
    CREDO_VERUM(bene && bita == ZEPHYRUM);
    /* refutationes */
    (vacuum)_m_vectoris(XXXVII, XXXIV, II, VIII, IX, A30, A31, ZEPHYRUM,
        &bene, &bita);
    CREDO_FALSUM(bene);
    (vacuum)_m_vectoris(XXXVII, XXXIV, II, VIII, IX, A30, A31, CCLVII,
        &bene, &bita);
    CREDO_FALSUM(bene);
    (vacuum)_m_vectoris(XXXVII, XXXIV, II, ZEPHYRUM, IX, A30, A31, LXIV,
        &bene, &bita);
    CREDO_FALSUM(bene);
    (vacuum)_m_vectoris(XXXVII, XXXIV, II, VIII, IX, A30, A30, LXIV,
        &bene, &bita);
    CREDO_FALSUM(bene);
    (vacuum)_m_vectoris(XXXVII, XXXIV, II, VIII, IX, A30,
        A30 + (i32)0x2000000UL, LXIV, &bene, &bita);
    CREDO_FALSUM(bene);             /* latitudo 2^25 > 2^24 */
    (vacuum)_m_vectoris(CCC, CCC, III, VIII, I, LXIV, CXXVIII, LXIV,
        &bene, &bita);
    CREDO_FALSUM(bene);             /* eps >= 1 */
}

/* calibratio: P = parvum + p1 T(x), T = (x - 1)...(x - XL): candidatus
 * stadii 1 est parvum, D = -p1 T, radices x = 1..XL in OMNI q - casus
 * pessimus quem limes permittit. Ambitus angustus [1024, 4096): rata
 * observata transituum stadii 1 > 0 et <= eps formulae (limes vivus) */
interior vacuum
_probare_limitem_vivum (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  T[XLI];
                     Magnus  coef[XLI];
                      Notum  n;
                     Magnus  p1;
                        i32  limes          = ZEPHYRUM;
                        i32  transitus      = ZEPHYRUM;
                        i32  summa          = ZEPHYRUM;
                        i32  primi_ambitus  = ZEPHYRUM;
                        i32  k_maximus      = ZEPHYRUM;
                        i32  v;
                        i32  i;
                        i32  j;
                        f64  periculum;

    p1 =
        magnus_ex_s64((s64)congruentia_primus_infra((i32)0xFFFFFFFFUL));
    T[ZEPHYRUM] = magnus_ex_s64(I);
    per (j = I; j <= XL; j++)
    {
        T[j] = magnus_ex_s64(ZEPHYRUM);
    }
    per (i = I; i <= XL; i++)
    {
        /* T <- T (x - i) */
        per (j = i; j > ZEPHYRUM; j--)
        {
            T[j] = magnus_subtrahe(T[j - I], magnus_multiplica(
                magnus_ex_s64((s64)i), T[j], piscina), piscina);
        }
        T[ZEPHYRUM] = magnus_nega(magnus_multiplica(magnus_ex_s64(
            (s64)i), T[ZEPHYRUM], piscina), piscina);
    }
    per (j = ZEPHYRUM; j <= XL; j++)
    {
        coef[j] = magnus_adde(magnus_ex_s64((s64)(j % V) - (s64)II),
            magnus_multiplica(p1, T[j], piscina), piscina);
        si (magnus_bitorum(coef[j]) > limes)
        {
            limes = magnus_bitorum(coef[j]);
        }
    }
    n.coefficientes  = coef;
    n.numerus        = XLI;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    per (v = M + XXIV; v < ((i32)0x1000UL); v++)
    {
        si (congruentia_est_primus(v))
        {
            primi_ambitus++;
        }
    }
    per (i = I; i <= DCCC; i++)
    {
                 PiscinaNotatio nota =
                     piscina_notare(piscina);
                      Contextus ctx;
                      Contextus ctx_v;
           InterpolatioQuaestio q;
        InterpolatioVerificator ver;
            InterpolatioRelatio r;
                     Polynomium f;
                            i32 t;

        _contextus_initium(&ctx, &n);
        _contextus_initium(&ctx_v, &n);
        ver = _verificator(&ctx_v, I);
        q = _quaestio_verificandi(&n, limes, (i64)i);
        q.bita_securitatis = VIII;
        q.verificationis_imus = M + XXIV;
        q.verificationis_summus = ((i32)0x1000UL);
        CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx,
            &ver, &q, piscina, &f, &r));
        k_maximus = r.k_maximus;
        per (t = ZEPHYRUM; t < r.numerus_tractuum; t++)
        {
            si (r.tractus[t].gradus == I && !r.tractus[t].certificans)
            {
                summa++;
                si (r.tractus[t].transiit)
                {
                    transitus++;
                }
            }
        }
        piscina_reficere(piscina, nota);
    }
    periculum = (f64)XL / (f64)(M + XXIV - I) + (f64)((limes + I) / X)
        / (f64)(primi_ambitus - k_maximus - I);
    CREDO_NECESSE(summa == DCCC);
    CREDO_VERUM(transitus > ZEPHYRUM);
    CREDO_VERUM((f64)transitus / (f64)summa <= periculum);
}

/* promotio: verificatum deinde promotum = PROBATUM directe; aestimationes
 * = probatum - verificatum; verificatum corruptum +M_k aut +1 -> FALSUM;
 * series in relatione mutata -> FALSUM; PROBATUM non promovetur.
 * Plantae "promotio a primo 1" et "promotio sine comparatione R = Q"
 * hic rubent. */
interior vacuum
_probare_promotionem (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[VI];
                      Notum  n;
                  Contextus  ctx;
                  Contextus  ctx_v;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  rv;
        InterpolatioRelatio  rp;
        InterpolatioRelatio  rn;
        InterpolatioRelatio  mutata;
                 Polynomium  fv;
                 Polynomium  fp;
                 Polynomium  fn;
                 Polynomium  signum;
                 Polynomium  corruptum;
                     Magnus  M_k = magnus_ex_s64(I);
                     Magnus  densa[VI];
                        i32  primi_mutati[XVI];
                        i32  j;

    _notum_magnum(&n, coef, piscina);
    signum = polynomium_constans(magnus_ex_s64(DCLXVI), piscina);
    _contextus_initium(&ctx_v, &n);
    v = _verificator(&ctx_v, I);
    q = _quaestio_verificandi(&n, CC, (i64)XXIX);
    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &fv, &rv));
    CREDO_NECESSE(rv.genus == INTERPOLATIO_VERIFICATUM);
    q.modus = INTERPOLATIO_PROBATUM;
    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_polynomium_relatum(_aestimator, &ctx,
        NIHIL,
        &q, piscina, &fp, &rp));

    _contextus_initium(&ctx, &n);
    CREDO_NECESSE(interpolatio_promovere(_aestimator, &ctx, &q, &rv,
        &fv,
        piscina, &fn, &rn));
    CREDO_VERUM(polynomium_aequalis(fn, fp));
    CREDO_AEQUALIS_I32(rn.genus, INTERPOLATIO_PROBATUM);
    CREDO_AEQUALIS_I32(rn.status, INTERPOLATIO_PROMOTUM);
    CREDO_AEQUALIS_I32(rn.numerus_primorum, rp.numerus_primorum);
    CREDO_AEQUALIS_I32(rn.primus_additus, rp.primus_additus);
    CREDO_VERUM(rn.aestimationes == rp.aestimationes
        - rv.aestimationes);
    CREDO_VERUM((i64)ctx.vocationes == rn.aestimationes);

    /* +M_k in coefficiente uno: residua primorum usorum eadem */
    per (j = ZEPHYRUM; j < rv.numerus_primorum; j++)
    {
        M_k = magnus_multiplica(M_k, magnus_ex_s64((s64)rv.primi[j]),
            piscina);
    }
    per (j = ZEPHYRUM; j < VI; j++)
    {
        densa[j] = polynomium_coefficiens(fv, n.imus + (s32)j);
    }
    densa[II] = magnus_adde(densa[II], M_k, piscina);
    CREDO_NECESSE(polynomium_ex_coefficientibus(densa, VI, n.imus,
        piscina,
        &corruptum));
    fn = signum;
    _contextus_initium(&ctx, &n);
    CREDO_FALSUM(interpolatio_promovere(_aestimator, &ctx, &q, &rv,
        &corruptum, piscina, &fn, &rn));
    CREDO_VERUM(polynomium_aequalis(fn, signum));
    densa[II] = magnus_subtrahe(densa[II], M_k, piscina);
    densa[II] = magnus_adde(densa[II], magnus_ex_s64(I), piscina);
    CREDO_NECESSE(polynomium_ex_coefficientibus(densa, VI, n.imus,
        piscina,
        &corruptum));
    _contextus_initium(&ctx, &n);
    CREDO_FALSUM(interpolatio_promovere(_aestimator, &ctx, &q, &rv,
        &corruptum, piscina, &fn, &rn));
    CREDO_VERUM(polynomium_aequalis(fn, signum));

    /* series mutata */
    mutata = rv;
    per (j = ZEPHYRUM; j < rv.numerus_primorum; j++)
    {
        primi_mutati[j] = rv.primi[j];
    }
    primi_mutati[ZEPHYRUM] = congruentia_primus_infra(primi_mutati[
        ZEPHYRUM]);
    mutata.primi = primi_mutati;
    _contextus_initium(&ctx, &n);
    CREDO_FALSUM(interpolatio_promovere(_aestimator, &ctx, &q, &mutata,
        &fv, piscina, &fn, &rn));
    CREDO_AEQUALIS_I32(ctx.vocationes, ZEPHYRUM);
    /* PROBATUM non promovetur; relatio_nova eadem refutatur */
    CREDO_FALSUM(interpolatio_promovere(_aestimator, &ctx, &q, &rp, &fp,
        piscina, &fn, &rn));
    CREDO_FALSUM(interpolatio_promovere(_aestimator, &ctx, &q, &rv, &fv,
        piscina, &fn, &rv));
}

/* quaestiones VERIFICATUM invalidae: FALSUM sine aestimatione */
interior vacuum
_probare_verificationem_invalidam (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                     Magnus  coef[II];
                      Notum  n;
                  Contextus  ctx;
       InterpolatioQuaestio  q;
    InterpolatioVerificator  v;
        InterpolatioRelatio  r;
                 Polynomium  f;

    coef[ZEPHYRUM]   = magnus_ex_s64(III);
    coef[I]          = magnus_ex_s64(-I);
    n.coefficientes  = coef;
    n.numerus        = II;
    n.imus           = ZEPHYRUM;
    n.passus         = I;
    _contextus_initium(&ctx, &n);
    v = _verificator(&ctx, I);

    q = _quaestio_verificandi(&n, C, (i64)I);
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx,
        NIHIL,
        &q, piscina, &f, &r));
    q.bita_securitatis = ZEPHYRUM;
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    q              = _quaestio_verificandi(&n, C, (i64)I);
    q.semen_origo  = II;
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    q                 = _quaestio_verificandi(&n, C, (i64)I);
    q.residuum_primi  = I;
    q.modulus_primi   = III;
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    q                        = _quaestio_verificandi(&n, C, (i64)I);
    q.verificationis_summus  = q.verificationis_imus;
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    q        = _quaestio_verificandi(&n, C, (i64)I);
    q.modus  = VII;
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    q                = _quaestio_verificandi(&n, C, (i64)I);
    v.multiplicitas  = ZEPHYRUM;
    CREDO_FALSUM(interpolatio_polynomium_relatum(_aestimator, &ctx, &v,
        &q, piscina, &f, &r));
    CREDO_AEQUALIS_I32(ctx.vocationes, ZEPHYRUM);
}


/* ==================================================
 * Suita
 * ================================================== */

interior constans CredoSectio SECTIONES[] = {
    { "quaestio ordinaria", _probare_ordinariam, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "lagrange fortuita", _probare_lagrange_fortuitam, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "lagrange refutationes", _probare_lagrange_refutationes, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "polynomium fortuitum", _probare_polynomium_fortuitum, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { "limes in margine", _probare_limitem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mendacia", _probare_mendacia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "erratum et inutile", _probare_erratum_et_inutile, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "puncta iterata", _probare_puncta_iterata, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "filtrum primorum", _probare_filtrum_primorum, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "scalaris", _probare_scalarem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo primorum", _probare_marginem_primorum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "filtrum et aptitudo", _probare_filtrum_et_aptitudinem, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "puncta opposita", _probare_oppositos, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "puncta vitiosa", _probare_puncta_vitiosa, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "formae extremae", _probare_formas_extremas, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "defectus additi", _probare_defectum_additi, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "quaestiones invalidae", _probare_quaestiones_invalidas, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "verificatum aequale", _probare_verificatum_aequale, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "modulus parvus", _probare_modulum_parvum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "verificator numerans", _probare_numerationem, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "tractus referentiales", _probare_tractus_iterum, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "verificator mendax", _probare_verificator_mendax, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { "polynomium strictum", _probare_polynomium_strictum, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "verificator speculi", _probare_speculum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "semina", _probare_semina, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vectores limitis", _probare_vectores_limitis, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "calibratio", _probare_limitem_vivum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "promotio", _probare_promotionem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "verificatio invalida", _probare_verificationem_invalidam, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("interpolatio", SECTIONES);
}
