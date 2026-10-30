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
    v = _aestima_notum(c->notum, x, p);
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("interpolatio", SECTIONES);
}
