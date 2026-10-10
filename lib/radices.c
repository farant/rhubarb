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
 * Vide lib/radices.worklog.md.
 */
#include "radices.h"

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
       Fractio v;
    Polynomium d = polynomium_nullum();

    si (!polynomium_valor(f, x, piscina, &v))
    {
        redde FALSUM;
    }
    *s = fractio_signum(v);
    si (*s != 0)
    {
        redde VERUM;
    }
    si (   !polynomium_derivata(f, piscina, &d)
        || !polynomium_valor(d, x, piscina, &v))
    {
        redde FALSUM;
    }
    *s = latus > 0 ? fractio_signum(v) : -fractio_signum(v);
    redde *s != 0;               /* radix multiplex: contractus fractus */
}

interior b32
_signum_ad (
    Polynomium  f,
       Fractio  x,
       Piscina* piscina,
           s32* s)
{
    Fractio v;

    si (!polynomium_valor(f, x, piscina, &v))
    {
        redde FALSUM;
    }
    *s = fractio_signum(v);
    redde VERUM;
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

interior b32
_exponentes_ordinarii (
    Polynomium p)
{
    redde polynomium_est_nullum(p)
        || polynomium_gradus_imus(p) >= ZEPHYRUM;
}


/* ==================================================
 * Isolatio (VCA)
 * ================================================== */

nomen structura {
    Polynomium q;
        Magnus c;
           s32 h;
           b32 exacta;             /* radix media: c/2^h exacte */
} Opus;

/* radices positivae g (g(0) != 0, liber quadratis), ordine crescente;
 * negativae = VERUM: g iam speculatum, radices negantur (ordo a vocante
 * invertitur) */
interior b32
_separa_positivas (
     Polynomium  g,
     Polynomium  f,
            b32  negativae,
        Piscina* piscina,
    RadixRealis* radices,
            i32* n)
{
      Opus* acervus;
       i32  capacitas;
       i32  altitudo;
       s32  k;
    Magnus  maximus = magnus_ex_s64(ZEPHYRUM);
       s32  i;

    si (_gradus(g) < I)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i <= _gradus(g); i++)
    {
        Magnus a = magnus_absolutum(polynomium_coefficiens(g, i),
            piscina);

        si (magnus_compara(a, maximus) > 0)
        {
            maximus = a;
        }
    }
    k = (s32)magnus_bitorum(maximus) + I;    /* 2^k > 2 max >= Cauchy */
    /* acervus: profunditas ternis operibus per gradum */
    capacitas = (i32)(III * (LIMES_PROFUNDITATIS + II));
    acervus = (Opus*)piscina_allocare(piscina, (memoriae_index)capacitas
        * magnitudo(Opus));
    acervus[ZEPHYRUM].q = _primitiva(_scala_coefficientium(g,
        magnus_potentia(magnus_ex_s64(II), (i32)k, piscina), FALSUM,
        piscina),
        piscina);
    acervus[ZEPHYRUM].c       = magnus_ex_s64(ZEPHYRUM);
    acervus[ZEPHYRUM].h       = ZEPHYRUM;
    acervus[ZEPHYRUM].exacta  = FALSUM;
    altitudo                  = I;
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
            perge;
        }
        si (!_variationes_unum(o.q, piscina, &v))
        {
            redde FALSUM;
        }
        si (v == ZEPHYRUM)
        {
            perge;
        }
        si (v == I)
        {
            Fractio a = _fractio_binaria(o.c, k, o.h, piscina);
            Fractio b = _fractio_binaria(magnus_adde(o.c,
                magnus_ex_s64(I), piscina),
                k, o.h, piscina);

            radices[*n].f = f;
            radices[*n].infra = negativae ? fractio_nega(b,
                piscina) : a;
            radices[*n].supra = negativae ? fractio_nega(a,
                piscina) : b;
            (*n)++;
            perge;
        }
        si (o.h >= LIMES_PROFUNDITATIS || altitudo + III > capacitas)
        {
            redde FALSUM;
        }
        sinistrum = _primitiva(_scala_coefficientium(o.q,
            magnus_ex_s64(II),
            VERUM, piscina), piscina);
        si (!polynomium_translatum(sinistrum, magnus_ex_s64(I), piscina,
                &dextrum))
        {
            redde FALSUM;
        }
        duplex_c = magnus_multiplica(o.c, magnus_ex_s64(II), piscina);
        per (i = ZEPHYRUM; i <= _gradus(sinistrum); i++)
        {
            summa = magnus_adde(summa, polynomium_coefficiens(sinistrum,
                i),
                piscina);
        }
        /* acervus: dextrum, [radix media], sinistrum */
        acervus[altitudo].q = _primitiva(dextrum, piscina);
        acervus[altitudo].c = magnus_adde(duplex_c, magnus_ex_s64(I),
            piscina);
        acervus[altitudo].h       = o.h + I;
        acervus[altitudo].exacta  = FALSUM;
        altitudo++;
        si (magnus_signum(summa) == 0)
        {
            acervus[altitudo].q = polynomium_nullum();
            acervus[altitudo].c = magnus_adde(duplex_c,
                magnus_ex_s64(I),
                piscina);
            acervus[altitudo].h       = o.h + I;
            acervus[altitudo].exacta  = VERUM;
            altitudo++;
        }
        acervus[altitudo].q       = sinistrum;
        acervus[altitudo].c       = duplex_c;
        acervus[altitudo].h       = o.h + I;
        acervus[altitudo].exacta  = FALSUM;
        altitudo++;
    }
    redde VERUM;
}

b32
radices_reales (
     Polynomium   f,
        Piscina*  piscina,
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
        || !polynomium_pars_libera(f, piscina, &g))
    {
        redde FALSUM;
    }
    gradus = _gradus(g);
    r = (RadixRealis*)piscina_allocare(piscina, (memoriae_index)(gradus
        + I) * magnitudo(RadixRealis));
    neg = (RadixRealis*)piscina_allocare(piscina,
        (memoriae_index)(gradus
        + I) * magnitudo(RadixRealis));
    g0 = g;
    si (   gradus                                             >= I
        && magnus_signum(polynomium_coefficiens(g, ZEPHYRUM)) == 0)
    {
        (vacuum)polynomium_translata(g, -(s32)I, piscina, &g0);   /* g / t */
    }
    si (!_separa_positivas(_scala_coefficientium(g0,
        magnus_ex_s64(-(s64)I),
            FALSUM, piscina), g, VERUM, piscina, neg, &n_neg))
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
 * radix rationalis in medio -> punctum */
interior b32
_seca_medio (
    RadixRealis* a,
        Piscina* piscina)
{
    Fractio m;
        s32 sm = ZEPHYRUM;
        s32 sl = ZEPHYRUM;

    si (_est_exacta(*a, piscina))
    {
        redde VERUM;
    }
    m = fractio_multiplica(fractio_adde(a->infra, a->supra, piscina),
        _dimidium(piscina), piscina);
    si (   !_signum_ad(a->f, m, piscina, &sm)
        || !_signum_iuxta(a->f, a->infra, I, piscina, &sl))
    {
        redde FALSUM;
    }
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

b32
radix_signum_polynomii (
     Polynomium  h,
    RadixRealis  a,
        Piscina* piscina,
            s32* exitus)
{
    Polynomium g = polynomium_nullum();
           s32 iteratio;

    si (!_exponentes_ordinarii(h))
    {
        redde FALSUM;
    }
    si (polynomium_est_nullum(h))
    {
        *exitus = ZEPHYRUM;
        redde VERUM;
    }
    si (_est_exacta(a, piscina))
    {
        redde _signum_ad(h, a.infra, piscina, exitus);
    }
    /* a radix gcd(f, h)? g | f liber quadratis: radicem summum unam in
     * intervallo aperto; adest sse signum g iuxta terminos intra mutatur */
    si (!polynomium_divisor_communis(a.f, h, piscina, &g))
    {
        redde FALSUM;
    }
    si (_gradus(g) >= I)
    {
        s32 sl = ZEPHYRUM;
        s32 sr = ZEPHYRUM;

        si (   !_signum_iuxta(g, a.infra, I, piscina, &sl)
            || !_signum_iuxta(g, a.supra, -(s32)I, piscina, &sr))
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
    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        s32 v = ZEPHYRUM;

        si (_est_exacta(a, piscina))
        {
            redde _signum_ad(h, a.infra, piscina, exitus);
        }
        si (!_variationes_in(h, a.infra, a.supra, piscina, &v))
        {
            redde FALSUM;
        }
        si (v == ZEPHYRUM)
        {
            redde _signum_ad(h, fractio_multiplica(fractio_adde(a.infra,
                a.supra, piscina), _dimidium(piscina), piscina),
                piscina,
                exitus);
        }
        si (!_seca_medio(&a, piscina))
        {
            redde FALSUM;
        }
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
    s32 s = ZEPHYRUM;
    s32 ordo;
    s32 iteratio;

    si (_est_exacta(a, piscina) && _est_exacta(b, piscina))
    {
        *exitus = fractio_compara(a.infra, b.infra, piscina);
        redde VERUM;
    }
    ordo = _ordo_intervallorum(a, b, piscina);
    si (ordo != 0)
    {
        *exitus = ordo;
        redde VERUM;
    }
    /* a radix f_b? */
    si (_est_exacta(a, piscina))
    {
        RadixRealis t = a;

        a = b;
        b = t;
        si (!radix_compara(a, b, piscina, &s))
        {
            redde FALSUM;
        }
        *exitus = -s;
        redde VERUM;
    }
    /* b = q exacta: signum(a - q) = signum (den t - num) in a. Non b.f -
     * punctum medium in radicem incidens f plenum servat, et b.f(a) = 0
     * pro QUAVIS radice b.f */
    si (_est_exacta(b, piscina))
    {
        redde radix_signum_polynomii(radix_ex_fractione(b.infra,
            piscina).f, a, piscina, exitus);
    }
    si (!radix_signum_polynomii(b.f, a, piscina, &s))
    {
        redde FALSUM;
    }
    per (iteratio = ZEPHYRUM; iteratio
        < II * LIMES_BISECTIONUM; iteratio++)
    {
        ordo = _ordo_intervallorum(a, b, piscina);
        si (ordo != 0)
        {
            *exitus = ordo;
            redde VERUM;
        }
        si (s == 0)
        {
            /* a radix f_b: a == b sse a in intervallo b (ibi radix f_b
             * una); b hic numquam exacta (supra), sola a secatur */
            si (   fractio_compara(b.infra, a.infra, piscina) <= 0
                && fractio_compara(a.supra, b.supra, piscina) <= 0)
            {
                *exitus = ZEPHYRUM;
                redde VERUM;
            }
            si (!_seca_medio(&a, piscina))
            {
                redde FALSUM;
            }
        } alioquin
        {
            si (!_seca_medio(&a, piscina) || !_seca_medio(&b, piscina))
            {
                redde FALSUM;
            }
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Angustatio publica, decimalis
 * ================================================== */

b32
radix_angusta (
    RadixRealis  a,
        Fractio  latitudo,
        Piscina* piscina,
    RadixRealis* exitus)
{
    s32 iteratio;

    si (fractio_signum(latitudo) <= 0)
    {
        redde FALSUM;
    }
    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        si (_est_exacta(a, piscina) || fractio_compara(fractio_subtrahe(
                a.supra, a.infra, piscina), latitudo, piscina) < 0)
        {
            *exitus = a;
            redde VERUM;
        }
        si (!_seca_medio(&a, piscina))
        {
            redde FALSUM;
        }
    }
    redde FALSUM;
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

b32
radix_ad_chordam (
    RadixRealis  a,
            i32  digiti,
        Piscina* piscina,
         chorda* exitus)
{
    Fractio scala = fractio_ex_magno(magnus_potentia(magnus_ex_s64(X),
        digiti, piscina));
    b32 negativa = FALSUM;
    s32 iteratio;

    /* signum notum facere: intervallum non 0 transiens */
    per (iteratio = ZEPHYRUM; !_est_exacta(a, piscina)
        && fractio_signum(a.infra) < 0 && fractio_signum(a.supra) > 0;
        iteratio++)
    {
        si (iteratio >= LIMES_BISECTIONUM || !_seca_medio(&a, piscina))
        {
            redde FALSUM;
        }
    }
    si (fractio_signum(a.supra) <= 0 && !(fractio_signum(a.supra) == 0
        && fractio_signum(a.infra) == 0))
    {
        /* a < 0: -a radix f(-t) in (-supra, -infra) */
        RadixRealis b;

        negativa = VERUM;
        b.f = _scala_coefficientium(a.f, magnus_ex_s64(-(s64)I), FALSUM,
            piscina);
        b.infra  = fractio_nega(a.supra, piscina);
        b.supra  = fractio_nega(a.infra, piscina);
        a        = b;
    }
    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        Magnus m = fractio_pavimentum(fractio_multiplica(a.infra, scala,
            piscina), piscina);

        si (   _est_exacta(a, piscina)
            || fractio_compara(fractio_multiplica(a.supra, scala,
            piscina),
            fractio_ex_magno(magnus_adde(m, magnus_ex_s64(I), piscina)),
            piscina) <= 0)
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
                    piscina), fractio_numerator(scala), piscina, &c)
                || !_signum_ad(a.f, c, piscina, &sc))
            {
                redde FALSUM;
            }
            si (   sc == 0
                && fractio_compara(a.infra, c, piscina) < 0
                && fractio_compara(c, a.supra, piscina) < 0)
            {
                a.infra = c;
                a.supra = c;
                perge;
            }
        }
        si (!_seca_medio(&a, piscina))
        {
            redde FALSUM;
        }
    }
    redde FALSUM;
}
