/* radices.c - Radices reales polynomiorum integrorum, exacte
 *
 * VCA (Vincent - Collins - Akritas): radices positivae f in (0, 2^k)
 * (limes Cauchy: |radix| < 1 + max|a_i| < 2^k) per Q(x) = f(2^k x) in
 * (0, 1). Pro Q: v = variationes signorum (x+1)^n Q(1/(x+1)) (regula
 * Cartesii: radices in (0, 1) <= v, paritate eadem; v = 0 nullae, v = 1
 * una). v >= 2: bisectio - L(x) = 2^n Q(x/2) (dimidium sinistrum), R(x)
 * = L(x + 1) (dextrum); Q(1/2) = 0 sse L(1) = summa coefficientium L =
 * 0: radix rationalis in medio, exacte reddita (Cartesius radices in
 * terminis non numerat, ergo filii non turbantur). Acervus: dextrum,
 * radix media, sinistrum - tracta sinistrum primum: ordo crescens. f
 * liber quadratis, ergo terminatur. Negativae per f(-t); 0 per f(0).
 *
 * Termini intervallorum dyadici; radicem SUAM numquam continent, sed
 * radices ALIAS f esse possunt (radix media exacte reddita terminus
 * filiorum fit: sqrt3 in (1, 2) ubi 1 et 2 radices sunt). f liber
 * quadratis, ergo radices simplices: signum iuxta terminum x (intra
 * intervallum) = signum f(x), aut si f(x) = 0 signum f'(x) (dextrum)
 * vel -signum f'(x) (sinistrum). Bisectio et probatio radicis communis
 * hoc signo utuntur.
 *
 * Memoria: in piscina vocantis effecta sola (radices, f, termini,
 * chorda); comparatio et signum nihil ibi allocant. Temporaria in
 * piscinis propriis: VCA acervum habet LIFO per notas (opus extractum
 * cum omnibus post eum allocatis liberatur) et opus post quodque opus
 * refectum; ansae angustationis opus post quamque iterationem reficiunt,
 * termini novi in status transcribuntur. Porta venenum (0xA5) lectionem
 * post refectionem capit.
 * Vide lib/radices.worklog.md.
 */
#include "radices.h"
#include "radices_interna.h"

/* limites: separatio 2^-2048 longe ultra usum (geminae Mignotte gradu
 * 7 ~2^-25); limes vitium in ansam lentam (minuta) non vertit */
#define LIMES_PROFUNDITATIS  4096      /* bisectiones VCA */
#define LIMES_BISECTIONUM    2048      /* angustatio per radicem */


/* ==================================================
 * Auxilia polynomiorum
 * ================================================== */

interior s32
_gradus (
    Polynomium p)
{
    redde polynomium_est_nullum(p) ? -(s32)I : polynomium_gradus_summus(p);
}

/* p / contentum (signum servatur) */
interior Polynomium
_primitiva (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g;

    si (polynomium_est_nullum(p))
    {
        redde p;
    }
    g = polynomium_contentum(p, piscina);
    si (magnus_compara(g, magnus_ex_s64(I)) != ZEPHYRUM)
    {
        (vacuum)polynomium_divide_exacte(p, polynomium_constans(g,
            piscina),
            piscina, &p);
    }
    redde p;
}

/* t^n p(1/t), n = gradus */
interior Polynomium
_reversum (
    Polynomium  p,
       Piscina* piscina)
{
    Polynomium r = polynomium_inversum(p, piscina);
    Polynomium e = polynomium_nullum();

    (vacuum)polynomium_translata(r, polynomium_gradus_summus(p),
        piscina, &e);
    redde e;
}

/* a_i -> a_i m^i, id est p(m t); inversa: a_i -> a_i m^(n - i), id est
 * m^n p(t/m) */
interior Polynomium
_scala_coefficientium (
    Polynomium  p,
        Magnus  m,
           b32  inversa,
       Piscina* piscina)
{
           s32  n = _gradus(p);
        Magnus* potentiae;
        Magnus* c;
    Polynomium  e = polynomium_nullum();
           s32  i;

    si (n < ZEPHYRUM)
    {
        redde p;
    }
    potentiae = (Magnus*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(Magnus));
    c = (Magnus*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * magnitudo(Magnus));
    potentiae[ZEPHYRUM] = magnus_ex_s64(I);
    per (i = I; i <= n; i++)
    {
        potentiae[i] = magnus_multiplica(potentiae[i - I], m, piscina);
    }
    per (i = ZEPHYRUM; i <= n; i++)
    {
        c[i] = magnus_multiplica(polynomium_coefficiens(p, i),
            potentiae[inversa ? n - i : i], piscina);
    }
    (vacuum)polynomium_ex_coefficientibus(c, (i32)(n + I), ZEPHYRUM,
        piscina, &e);
    redde e;
}

/* variationes signorum coefficientium (nulla omissa) */
interior s32
_variationes (
    Polynomium p)
{
    s32 n      = _gradus(p);
    s32 prior  = ZEPHYRUM;
    s32 v      = ZEPHYRUM;
    s32 i;

    per (i = ZEPHYRUM; i <= n; i++)
    {
        s32 s = magnus_signum(polynomium_coefficiens(p, i));

        si (s != 0)
        {
            si (prior != 0 && s != prior)
            {
                v++;
            }
            prior = s;
        }
    }
    redde v;
}

/* limes Cartesii radicum Q in (0, 1): variationes (x+1)^n Q(1/(x+1)) */
interior b32
_variationes_unum (
    Polynomium  q,
       Piscina* piscina,
           s32* v)
{
    Polynomium t = polynomium_nullum();

    si (_gradus(q) < I)
    {
        *v = ZEPHYRUM;
        redde VERUM;
    }
    si (!polynomium_translatum(_reversum(q, piscina), magnus_ex_s64(I),
        piscina,
            &t))
    {
        redde FALSUM;
    }
    *v = _variationes(t);
    redde VERUM;
}

/* limes Cartesii radicum h in (l, r), l < r rationales */
interior b32
_variationes_in (
    Polynomium  h,
       Fractio  l,
       Fractio  r,
       Piscina* piscina,
           s32* v)
{
    Magnus d = magnus_multiplica(fractio_denominator(l),
        fractio_denominator(r), piscina);
    Fractio df = fractio_ex_magno(d);
     Magnus lz = fractio_numerator(fractio_multiplica(l, df,
         piscina));
     Magnus w = fractio_numerator(fractio_multiplica(
         fractio_subtrahe(r, l, piscina), df, piscina));
    Polynomium p = polynomium_nullum();

    /* d^n h(y/d) integrum; y = l d + w x, x in (0, 1) */
    si (!polynomium_translatum(
        _scala_coefficientium(h, d, VERUM, piscina), lz, piscina, &p))
    {
        redde FALSUM;
    }
    p = _primitiva(_scala_coefficientium(p, w, FALSUM, piscina),
        piscina);
    redde _variationes_unum(p, piscina, v);
}

interior b32
_exponentes_ordinarii (
    Polynomium p)
{
    redde polynomium_est_nullum(p)
        || polynomium_gradus_imus(p) >= ZEPHYRUM;
}

/* signum f(p/q), q > 0: signum sum_i c_i p^i q^(d - i) - Horner
 * homogeneus in integris, sine divisore communi (Fractio in omni
 * multiplicatione divisorem communem quaerebat: XC centesimae temporis
 * angustationis, worklog 2026-10-10) */
interior b32
_signum_ad (
    Polynomium  f,
       Fractio  x,
       Piscina* piscina,
           s32* s)
{
       s32 d = _gradus(f);
    Magnus p = fractio_numerator(x);
    Magnus q = fractio_denominator(x);
    Magnus summa;
    Magnus potentia_q = magnus_ex_s64(I);
       s32 i;

    si (!_exponentes_ordinarii(f))
    {
        redde FALSUM;
    }
    si (d < ZEPHYRUM)
    {
        *s = ZEPHYRUM;
        redde VERUM;
    }
    summa = polynomium_coefficiens(f, d);
    per (i = d - I; i >= ZEPHYRUM; i--)
    {
        potentia_q  = magnus_multiplica(potentia_q, q, piscina);
        summa       = magnus_adde(magnus_multiplica(summa, p, piscina),
            magnus_multiplica(polynomium_coefficiens(f, i), potentia_q,
            piscina), piscina);
    }
    *s = magnus_signum(summa);
    redde VERUM;
}

/* signum f iuxta x: dextrum (latus > 0) aut sinistrum (latus < 0);
 * f liber quadratis */
interior b32
_signum_iuxta (
    Polynomium  f,
       Fractio  x,
           s32  latus,
       Piscina* piscina,
           s32* s)
{
    Polynomium d   = polynomium_nullum();
           s32 sd  = ZEPHYRUM;

    si (!_signum_ad(f, x, piscina, s))
    {
        redde FALSUM;
    }
    si (*s != 0)
    {
        redde VERUM;
    }
    si (   !polynomium_derivata(f, piscina, &d)
        || !_signum_ad(d, x, piscina, &sd))
    {
        redde FALSUM;
    }
    *s = latus > 0 ? sd : -sd;
    redde *s != 0;               /* radix multiplex: contractus fractus */
}

/* c 2^k / 2^h */
interior Fractio
_fractio_binaria (
     Magnus  c,
        s32  k,
        s32  h,
    Piscina* piscina)
{
    Fractio q = fractio_ex_s64(ZEPHYRUM);

    (vacuum)fractio_ex_magnis(magnus_multiplica(c, magnus_potentia(
        magnus_ex_s64(II), (i32)k, piscina), piscina), magnus_potentia(
        magnus_ex_s64(II), (i32)h, piscina), piscina, &q);
    redde q;
}


/* ==================================================
 * Isolatio (VCA)
 * ================================================== */

nomen structura {
        Polynomium q;
            Magnus c;
               s32 h;
               b32 exacta;         /* radix media: c/2^h exacte */
    PiscinaNotatio nota;           /* acervi, ante q et c huius operis */
} Opus;

/* piscina temporaria; NIHIL si memoria deest */
interior Piscina*
_piscina_temporaria (vacuum)
{
    redde piscina_generare_dynamicum("radices", (memoriae_index)65536);
}

/* apices piscinarum temporariarum (radices_interna.h): probationes
 * refectiones metiuntur, non piscinam vocantis solam */
interior memoriae_index _apex_acervi     = ZEPHYRUM;
interior memoriae_index _apex_officinae  = ZEPHYRUM;

vacuum
radicum_apices_purgare (vacuum)
{
    _apex_acervi     = ZEPHYRUM;
    _apex_officinae  = ZEPHYRUM;
}

memoriae_index
radicum_apex_acervi (vacuum)
{
    redde _apex_acervi;
}

memoriae_index
radicum_apex_officinae (vacuum)
{
    redde _apex_officinae;
}

/* piscinam destruere, apicem eius notare (NIHIL toleratur) */
interior vacuum
_piscina_destrue (
           Piscina* piscina,
    memoriae_index* apex)
{
    si (piscina != NIHIL)
    {
        si (piscina_summa_apex_usus(piscina) > *apex)
        {
            *apex = piscina_summa_apex_usus(piscina);
        }
        piscina_destruere(piscina);
    }
}

/* piscinae temporariae functionis publicae */
interior vacuum
_piscinae_destrue (
    Piscina* status,
    Piscina* opus)
{
    _piscina_destrue(status, &_apex_officinae);
    _piscina_destrue(opus, &_apex_officinae);
}

/* opus in acervum: nota ante q et c (ordo LIFO: opus extractum cum
 * omnibus post eum allocatis per notam suam liberatur) */
interior vacuum
_opus_pone (
          Opus* o,
    Polynomium  q,
        Magnus  c,
           s32  h,
           b32  exacta,
       Piscina* acervus_piscina)
{
    o->nota    = piscina_notare(acervus_piscina);
    o->q       = polynomium_transcribe(q, acervus_piscina);
    o->c       = magnus_transcribe(c, acervus_piscina);
    o->h       = h;
    o->exacta  = exacta;
}

/* radices positivae g (g(0) != 0, liber quadratis), ordine crescente;
 * negativae = VERUM: g iam speculatum, radices negantur (ordo a vocante
 * invertitur). Termini in piscina; polynomia acervi in acervus_piscina
 * (LIFO per notas), cetera in opus_piscina (post quodque opus refecta):
 * memoria = profunditas x polynomium, non summa omnium operum */
interior b32
_separa_intra (
     Polynomium  g,
     Polynomium  f,
            b32  negativae,
        Piscina* piscina,
        Piscina* acervus_piscina,
        Piscina* opus_piscina,
    RadixRealis* radices,
            i32* n)
{
              Opus* acervus;
               i32  capacitas;
               i32  altitudo;
               s32  k;
            Magnus  maximus = magnus_ex_s64(ZEPHYRUM);
               s32  i;
    PiscinaNotatio  basis;

    si (_gradus(g) < I)
    {
        redde VERUM;
    }
    /* acervus: profunditas ternis operibus per gradum */
    capacitas = (i32)(III * (LIMES_PROFUNDITATIS + II));
    acervus = (Opus*)piscina_allocare(acervus_piscina,
        (memoriae_index)capacitas * magnitudo(Opus));
    basis = piscina_notare(opus_piscina);
    per (i = ZEPHYRUM; i <= _gradus(g); i++)
    {
        Magnus a = magnus_absolutum(polynomium_coefficiens(g, i),
            opus_piscina);

        si (magnus_compara(a, maximus) > 0)
        {
            maximus = a;
        }
    }
    k = (s32)magnus_bitorum(maximus) + I;    /* 2^k > 2 max >= Cauchy */
    _opus_pone(&acervus[ZEPHYRUM], _primitiva(_scala_coefficientium(g,
        magnus_potentia(magnus_ex_s64(II), (i32)k, opus_piscina),
        FALSUM,
        opus_piscina), opus_piscina), magnus_ex_s64(ZEPHYRUM), ZEPHYRUM,
        FALSUM, acervus_piscina);
    piscina_reficere(opus_piscina, basis);
    altitudo = I;
    dum (altitudo > ZEPHYRUM)
    {
              Opus o = acervus[--altitudo];
               s32 v = ZEPHYRUM;
        Polynomium sinistrum;
        Polynomium dextrum  = polynomium_nullum();
            Magnus summa    = magnus_ex_s64(ZEPHYRUM);
            Magnus duplex_c;

        si (o.exacta)
        {
            Fractio m = _fractio_binaria(o.c, k, o.h, piscina);

            radices[*n].f = f;
            radices[*n].infra = negativae ? fractio_nega(m,
                piscina) : m;
            radices[*n].supra = radices[*n].infra;
            (*n)++;
            piscina_reficere(acervus_piscina, o.nota);
            perge;
        }
        si (!_variationes_unum(o.q, opus_piscina, &v))
        {
            redde FALSUM;
        }
        si (v == I)
        {
            Fractio a = _fractio_binaria(o.c, k, o.h, piscina);
            Fractio b = _fractio_binaria(magnus_adde(o.c,
                magnus_ex_s64(I), opus_piscina),
                k, o.h, piscina);

            radices[*n].f = f;
            radices[*n].infra = negativae ? fractio_nega(b,
                piscina) : a;
            radices[*n].supra = negativae ? fractio_nega(a,
                piscina) : b;
            (*n)++;
        }
        si (v <= I)
        {
            piscina_reficere(acervus_piscina, o.nota);
            piscina_reficere(opus_piscina, basis);
            perge;
        }
        si (o.h >= LIMES_PROFUNDITATIS || altitudo + III > capacitas)
        {
            redde FALSUM;
        }
        sinistrum = _primitiva(_scala_coefficientium(o.q,
            magnus_ex_s64(II),
            VERUM, opus_piscina), opus_piscina);
        si (!polynomium_translatum(sinistrum, magnus_ex_s64(I),
                opus_piscina, &dextrum))
        {
            redde FALSUM;
        }
        duplex_c = magnus_multiplica(o.c, magnus_ex_s64(II),
            opus_piscina);
        per (i = ZEPHYRUM; i <= _gradus(sinistrum); i++)
        {
            summa = magnus_adde(summa, polynomium_coefficiens(sinistrum,
                i),
                opus_piscina);
        }
        /* o.q mortuum (filii in opus_piscina); acervus: dextrum, [radix
         * media], sinistrum */
        piscina_reficere(acervus_piscina, o.nota);
        _opus_pone(&acervus[altitudo++], _primitiva(dextrum,
            opus_piscina),
            magnus_adde(duplex_c, magnus_ex_s64(I), opus_piscina), o.h
                + I,
            FALSUM, acervus_piscina);
        si (magnus_signum(summa) == 0)
        {
            _opus_pone(&acervus[altitudo++], polynomium_nullum(),
                magnus_adde(duplex_c, magnus_ex_s64(I), opus_piscina),
                o.h + I, VERUM, acervus_piscina);
        }
        _opus_pone(&acervus[altitudo++], sinistrum, duplex_c, o.h + I,
            FALSUM, acervus_piscina);
        piscina_reficere(opus_piscina, basis);
    }
    redde VERUM;
}

interior b32
_separa_positivas (
     Polynomium  g,
     Polynomium  f,
            b32  negativae,
        Piscina* piscina,
    RadixRealis* radices,
            i32* n)
{
     Piscina* acervus_piscina  = _piscina_temporaria();
     Piscina* opus_piscina     = _piscina_temporaria();
         b32  bene             = FALSUM;

    si (acervus_piscina != NIHIL && opus_piscina != NIHIL)
    {
        bene = _separa_intra(g, f, negativae, piscina, acervus_piscina,
            opus_piscina, radices, n);
    }
    _piscina_destrue(acervus_piscina, &_apex_acervi);
    _piscina_destrue(opus_piscina, &_apex_officinae);
    redde bene;
}

interior b32
_reales_intra (
     Polynomium   f,
        Piscina*  piscina,
        Piscina*  opus,
    RadixRealis** exitus,
            i32*  numerus)
{
     Polynomium  g = polynomium_nullum();
     Polynomium  g0;
    RadixRealis* r;
    RadixRealis* neg;
            i32  n      = ZEPHYRUM;
            i32  n_neg  = ZEPHYRUM;
            s32  gradus;

    si (   polynomium_est_nullum(f) || !_exponentes_ordinarii(f)
        || !polynomium_pars_libera(f, opus, &g))
    {
        redde FALSUM;
    }
    gradus = _gradus(g);
    r = (RadixRealis*)piscina_allocare(piscina, (memoriae_index)(gradus
        + I) * magnitudo(RadixRealis));
    neg = (RadixRealis*)piscina_allocare(opus,
        (memoriae_index)(gradus
        + I) * magnitudo(RadixRealis));
    g0 = g;
    si (   gradus                                             >= I
        && magnus_signum(polynomium_coefficiens(g, ZEPHYRUM)) == 0)
    {
        (vacuum)polynomium_translata(g, -(s32)I, opus, &g0);   /* g / t */
    }
    g = polynomium_transcribe(g, piscina);   /* f omnium radicum */
    si (!_separa_positivas(_scala_coefficientium(g0,
        magnus_ex_s64(-(s64)I),
            FALSUM, opus), g, VERUM, piscina, neg, &n_neg))
    {
        redde FALSUM;
    }
    {
        s32 j;

        per (j = (s32)n_neg - I; j >= ZEPHYRUM; j--)
        {
            r[n++] = neg[j];
        }
    }
    si (   gradus                                             >= I
        && magnus_signum(polynomium_coefficiens(g, ZEPHYRUM)) == 0)
    {
        r[n].f      = g;
        r[n].infra  = fractio_ex_s64(ZEPHYRUM);
        r[n].supra  = r[n].infra;
        n++;
    }
    si (!_separa_positivas(g0, g, FALSUM, piscina, r, &n))
    {
        redde FALSUM;
    }
    *exitus   = r;
    *numerus  = n;
    redde VERUM;
}

b32
radices_reales (
     Polynomium   f,
        Piscina*  piscina,
    RadixRealis** exitus,
            i32*  numerus)
{
     Piscina* opus = _piscina_temporaria();
         b32  bene;

    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    bene = _reales_intra(f, piscina, opus, exitus, numerus);
    _piscina_destrue(opus, &_apex_officinae);
    redde bene;
}

RadixRealis
radix_ex_fractione (
    Fractio  q,
    Piscina* piscina)
{
    RadixRealis r;
         Magnus c[II];

    c[ZEPHYRUM]  = magnus_nega(fractio_numerator(q), piscina);
    c[I]         = fractio_denominator(q);
    r.f          = polynomium_nullum();
    (vacuum)polynomium_ex_coefficientibus(c, II, ZEPHYRUM, piscina,
        &r.f);
    r.infra = q;
    r.supra = q;
    redde r;
}


/* ==================================================
 * Angustatio
 * ================================================== */

interior b32
_est_exacta (
    RadixRealis  a,
        Piscina* piscina)
{
    redde fractio_compara(a.infra, a.supra, piscina) == 0;
}

interior Fractio
_dimidium (
    Piscina* piscina)
{
    Fractio d = fractio_ex_s64(ZEPHYRUM);

    (vacuum)fractio_ex_s64_s64(I, II, piscina, &d);
    redde d;
}

/* bisectio una (f radicem unam in (infra, supra), termini non radices);
 * radix rationalis in medio -> punctum. Computatio in opus, terminus
 * novus in status: vocans opus post quamque iterationem reficit */
interior b32
_seca_medio (
    RadixRealis* a,
        Piscina* opus,
        Piscina* status)
{
    Fractio m;
        s32 sm = ZEPHYRUM;
        s32 sl = ZEPHYRUM;

    si (_est_exacta(*a, opus))
    {
        redde VERUM;
    }
    m = fractio_multiplica(fractio_adde(a->infra, a->supra, opus),
        _dimidium(opus), opus);
    si (   !_signum_ad(a->f, m, opus, &sm)
        || !_signum_iuxta(a->f, a->infra, I, opus, &sl))
    {
        redde FALSUM;
    }
    m = fractio_transcribe(m, status);
    si (sm == 0)
    {
        a->infra = m;
        a->supra = m;
    } alioquin si (sm == sl)
    {
        a->infra = m;
    } alioquin
    {
        a->supra = m;
    }
    redde VERUM;
}

/* a < b per intervalla disiuncta: -1 / +1, aliter 0 (incertum) */
interior s32
_ordo_intervallorum (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* piscina)
{
    si (   fractio_compara(a.supra, b.infra, piscina) <= 0
        && !(_est_exacta(a, piscina) && _est_exacta(b, piscina)
        && fractio_compara(a.supra, b.infra, piscina) == 0))
    {
        redde -(s32)I;
    }
    si (   fractio_compara(b.supra, a.infra, piscina) <= 0
        && !(_est_exacta(a, piscina) && _est_exacta(b, piscina)
        && fractio_compara(b.supra, a.infra, piscina) == 0))
    {
        redde I;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Signum polynomii, comparatio
 * ================================================== */

interior b32
_signum_polynomii_intra (
     Polynomium  h,
    RadixRealis  a,
        Piscina* status,
        Piscina* opus,
            s32* exitus)
{
        Polynomium g = polynomium_nullum();
               s32 iteratio;
    PiscinaNotatio basis;

    si (_est_exacta(a, opus))
    {
        redde _signum_ad(h, a.infra, opus, exitus);
    }
    /* a radix gcd(f, h)? g | f liber quadratis: radicem summum unam in
     * intervallo aperto; adest sse signum g iuxta terminos intra mutatur */
    si (!polynomium_divisor_communis(a.f, h, opus, &g))
    {
        redde FALSUM;
    }
    si (_gradus(g) >= I)
    {
        s32 sl = ZEPHYRUM;
        s32 sr = ZEPHYRUM;

        si (   !_signum_iuxta(g, a.infra, I, opus, &sl)
            || !_signum_iuxta(g, a.supra, -(s32)I, opus, &sr))
        {
            redde FALSUM;
        }
        si (sl != sr)
        {
            *exitus = ZEPHYRUM;
            redde VERUM;
        }
    }
    /* h(a) != 0: angusta donec h nullam radicem in intervallo habet */
    basis = piscina_notare(opus);
    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        s32 v = ZEPHYRUM;

        si (_est_exacta(a, opus))
        {
            redde _signum_ad(h, a.infra, opus, exitus);
        }
        si (!_variationes_in(h, a.infra, a.supra, opus, &v))
        {
            redde FALSUM;
        }
        si (v == ZEPHYRUM)
        {
            redde _signum_ad(h, fractio_multiplica(fractio_adde(a.infra,
                a.supra, opus), _dimidium(opus), opus), opus, exitus);
        }
        si (!_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_signum_polynomii (
     Polynomium  h,
    RadixRealis  a,
        Piscina* piscina,
            s32* exitus)
{
     Piscina* status;
     Piscina* opus;
         b32  bene = FALSUM;

    (vacuum)piscina;             /* exitus s32: nihil in piscina vocantis */
    si (!_exponentes_ordinarii(h))
    {
        redde FALSUM;
    }
    si (polynomium_est_nullum(h))
    {
        *exitus = ZEPHYRUM;
        redde VERUM;
    }
    status  = _piscina_temporaria();
    opus    = _piscina_temporaria();
    si (status != NIHIL && opus != NIHIL)
    {
        bene = _signum_polynomii_intra(h, a, status, opus, exitus);
    }
    _piscinae_destrue(status, opus);
    redde bene;
}

interior b32
_compara_intra (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* status,
        Piscina* opus,
            s32* exitus)
{
               s32 s = ZEPHYRUM;
               s32 ordo;
               s32 iteratio;
    PiscinaNotatio basis;

    si (_est_exacta(a, opus) && _est_exacta(b, opus))
    {
        *exitus = fractio_compara(a.infra, b.infra, opus);
        redde VERUM;
    }
    ordo = _ordo_intervallorum(a, b, opus);
    si (ordo != 0)
    {
        *exitus = ordo;
        redde VERUM;
    }
    /* a radix f_b? */
    si (_est_exacta(a, opus))
    {
        si (!_compara_intra(b, a, status, opus, &s))
        {
            redde FALSUM;
        }
        *exitus = -s;
        redde VERUM;
    }
    /* b = q exacta: signum(a - q) = signum (den t - num) in a. Non b.f -
     * punctum medium in radicem incidens f plenum servat, et b.f(a) = 0
     * pro QUAVIS radice b.f */
    si (_est_exacta(b, opus))
    {
        redde radix_signum_polynomii(radix_ex_fractione(b.infra,
            opus).f, a, opus, exitus);
    }
    si (!radix_signum_polynomii(b.f, a, opus, &s))
    {
        redde FALSUM;
    }
    basis = piscina_notare(opus);
    per (iteratio = ZEPHYRUM; iteratio
        < II * LIMES_BISECTIONUM; iteratio++)
    {
        ordo = _ordo_intervallorum(a, b, opus);
        si (ordo != 0)
        {
            *exitus = ordo;
            redde VERUM;
        }
        si (s == 0)
        {
            /* a radix f_b: a == b sse a in intervallo b (ibi radix f_b
             * una); b hic numquam exacta (supra), sola a secatur */
            si (   fractio_compara(b.infra, a.infra, opus) <= 0
                && fractio_compara(a.supra, b.supra, opus) <= 0)
            {
                *exitus = ZEPHYRUM;
                redde VERUM;
            }
            si (!_seca_medio(&a, opus, status))
            {
                redde FALSUM;
            }
        } alioquin
        {
            si (   !_seca_medio(&a, opus, status)
                || !_seca_medio(&b, opus, status))
            {
                redde FALSUM;
            }
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_compara (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* piscina,
            s32* exitus)
{
     Piscina* status  = _piscina_temporaria();
     Piscina* opus    = _piscina_temporaria();
         b32  bene    = FALSUM;

    (vacuum)piscina;             /* exitus s32: nihil in piscina vocantis */
    si (status != NIHIL && opus != NIHIL)
    {
        bene = _compara_intra(a, b, status, opus, exitus);
    }
    _piscinae_destrue(status, opus);
    redde bene;
}


/* ==================================================
 * Angustatio publica, decimalis
 * ================================================== */

interior b32
_angusta_intra (
    RadixRealis  a,
        Fractio  latitudo,
        Piscina* status,
        Piscina* opus,
    RadixRealis* exitus)
{
               s32 iteratio;
    PiscinaNotatio basis = piscina_notare(opus);

    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        si (   _est_exacta(a, opus) || fractio_compara(fractio_subtrahe(
                a.supra, a.infra, opus), latitudo, opus) < 0)
        {
            *exitus = a;
            redde VERUM;
        }
        si (!_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_angusta (
    RadixRealis  a,
        Fractio  latitudo,
        Piscina* piscina,
    RadixRealis* exitus)
{
        Piscina* status;
        Piscina* opus;
    RadixRealis  e;
            b32  bene = FALSUM;

    si (fractio_signum(latitudo) <= 0)
    {
        redde FALSUM;
    }
    status  = _piscina_temporaria();
    opus    = _piscina_temporaria();
    si (   status != NIHIL && opus != NIHIL
        && _angusta_intra(a, latitudo, status, opus, &e))
    {
        exitus->f      = a.f;
        exitus->infra  = fractio_transcribe(e.infra, piscina);
        exitus->supra  = fractio_transcribe(e.supra, piscina);
        bene           = VERUM;
    }
    _piscinae_destrue(status, opus);
    redde bene;
}

/* m (>= 0) / 10^d ut textus "i.dddd" */
interior chorda
_decimalis (
     Magnus  m,
        i32  digiti,
        b32  negativa,
    Piscina* piscina)
{
    chorda c = magnus_ad_chordam(m, piscina);
       i32 integra = c.mensura > digiti ? c.mensura
           - digiti : ZEPHYRUM;
       i32 longitudo = (integra > ZEPHYRUM ? integra : I)
           + (digiti > ZEPHYRUM ? I + digiti : ZEPHYRUM)
           + (negativa ? I : ZEPHYRUM);
    i8* datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)longitudo);
       i32 j = ZEPHYRUM;
       i32 i;
    chorda e;

    si (negativa)
    {
        datum[j++] = (i8)'-';
    }
    si (integra == ZEPHYRUM)
    {
        datum[j++] = (i8)'0';
    }
    per (i = ZEPHYRUM; i < integra; i++)
    {
        datum[j++] = c.datum[i];
    }
    si (digiti > ZEPHYRUM)
    {
        datum[j++] = (i8)'.';
    }
    per (i = ZEPHYRUM; i < digiti; i++)
    {
        /* digiti post punctum: zephyra praefixa si m < 10^(d-1) */
        s32 positio = (s32)c.mensura - (s32)digiti + (s32)i;

        datum[j++] = positio >= ZEPHYRUM ? c.datum[positio] : (i8)'0';
    }
    e.datum    = datum;
    e.mensura  = j;
    redde e;
}

interior b32
_ad_chordam_intra (
    RadixRealis  a,
            i32  digiti,
        Piscina* piscina,
        Piscina* status,
        Piscina* opus,
         chorda* exitus)
{
           Fractio scala =
               fractio_ex_magno(magnus_potentia(magnus_ex_s64(X),
               digiti, status));
               b32 negativa = FALSUM;
               s32 iteratio;
    PiscinaNotatio basis = piscina_notare(opus);

    /* signum notum facere: intervallum non 0 transiens */
    per (iteratio = ZEPHYRUM; !_est_exacta(a, opus)
        && fractio_signum(a.infra) < 0 && fractio_signum(a.supra) > 0;
        iteratio++)
    {
        si (   iteratio >= LIMES_BISECTIONUM
            || !_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    si (fractio_signum(a.supra) <= 0 && !(fractio_signum(a.supra) == 0
        && fractio_signum(a.infra) == 0))
    {
        /* a < 0: -a radix f(-t) in (-supra, -infra) */
        RadixRealis b;

        negativa = VERUM;
        b.f = _scala_coefficientium(a.f, magnus_ex_s64(-(s64)I), FALSUM,
            status);
        b.infra  = fractio_nega(a.supra, status);
        b.supra  = fractio_nega(a.infra, status);
        a        = b;
    }
    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        Magnus m = fractio_pavimentum(fractio_multiplica(a.infra, scala,
            opus), opus);

        si (   _est_exacta(a, opus)
            || fractio_compara(fractio_multiplica(a.supra, scala,
            opus),
            fractio_ex_magno(magnus_adde(m, magnus_ex_s64(I), opus)),
            opus) <= 0)
        {
            *exitus = _decimalis(m, digiti, negativa, piscina);
            redde VERUM;
        }
        /* radix ipsa (m + 1) / 10^d? tunc supra numquam infra eam
         * descendit (bisectio dyadica 1/5 numquam attingit) */
        {
            Fractio c   = fractio_ex_s64(ZEPHYRUM);
                s32 sc  = ZEPHYRUM;

            si (   !fractio_ex_magnis(magnus_adde(m, magnus_ex_s64(I),
                    opus), fractio_numerator(scala), opus, &c)
                || !_signum_ad(a.f, c, opus, &sc))
            {
                redde FALSUM;
            }
            si (   sc == 0
                && fractio_compara(a.infra, c, opus) < 0
                && fractio_compara(c, a.supra, opus) < 0)
            {
                a.infra = fractio_transcribe(c, status);
                a.supra = a.infra;
                piscina_reficere(opus, basis);
                perge;
            }
        }
        si (!_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_ad_chordam (
    RadixRealis  a,
            i32  digiti,
        Piscina* piscina,
         chorda* exitus)
{
     Piscina* status  = _piscina_temporaria();
     Piscina* opus    = _piscina_temporaria();
         b32  bene    = FALSUM;

    si (status != NIHIL && opus != NIHIL)
    {
        bene = _ad_chordam_intra(a, digiti, piscina, status, opus,
            exitus);
    }
    _piscinae_destrue(status, opus);
    redde bene;
}
