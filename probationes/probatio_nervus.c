/* probatio_nervus.c - probationes nervus (project-specs/nervus-plan.md)
 *
 * T1: constructio, refutationes, aestimatio cum forma, figura. Oracula
 * a codice independentia:
 * (1) retia manu computata: portae AND / OR / XOR in {0,1}^2,
 *     |x| = relu(x) + relu(-x), max(x, y) = y + relu(x - y), rete
 *     constans, exitus qui introitus est, valores 2^200;
 * (2) aestimator ALTER in hac probatione: recursio memorata ab
 *     exitibus retro, arithmetica s64 cum custodia exundationis (non
 *     Magnus, non ordine instructionum), contra retia fortuita;
 * (3) theorema: rete sine addendis positive homogeneum est,
 *     f(t x) = t f(x) et forma eadem pro t > 0 - probatum cum
 *     t = 2^200;
 * (4) figura manu computata (profunditas, latitudo).
 * Oraculum Pythonicum (ex definitione) semel extra portas currit:
 * vide lib/nervus.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sors.h"
#include "magnus.h"
#include "nervus.h"
#include <stdio.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior Magnus
_m (
    s64 valor)
{
    redde magnus_ex_s64(valor);
}

interior b32
_est (
    Magnus a,
       s64 valor)
{
    redde magnus_aequalis(a, magnus_ex_s64(valor));
}

/* instructio cum terminis s64; registrum novum aut -1 */
interior s32
_adde (
          Nervus* rete,
     NervusGenus  genus,
             i32  termini,
    constans s32* fontes,
    constans s64* coefficientes,
             s64  addendum)
{
    Magnus c[XVI];
       s32 r = -I;
       i32 k;

    per (k = ZEPHYRUM; k < termini; k++)
    {
        c[k] = magnus_ex_s64(coefficientes[k]);
    }
    si (genus == NERVUS_RELU)
    {
        si (!nervus_relu(rete, termini, fontes, c,
            magnus_ex_s64(addendum), &r))
        {
            redde -I;
        }
    }
    alioquin
    {
        si (!nervus_affinis(rete, termini, fontes, c,
            magnus_ex_s64(addendum), &r))
        {
            redde -I;
        }
    }
    redde r;
}

/* porta in {0,1}^2: s = x + y, a = relu(s - 1); exitus
 * AND = a, OR = s - a, XOR = s - 2a */
interior Nervus*
_portae (
    Piscina* piscina)
{
    Nervus* rete;
       s32  f[II];
       s64  c[II];
       s32  s;
       s32  a;
       s32  e[III];

    (vacuum)nervus_creare(II, piscina, &rete);
    f[ZEPHYRUM]  = ZEPHYRUM;
    f[I]         = I;
    c[ZEPHYRUM]  = I;
    c[I]         = I;
    s            = _adde(rete, NERVUS_AFFINIS, II, f, c, ZEPHYRUM);
    f[ZEPHYRUM]  = s;
    a            = _adde(rete, NERVUS_RELU, I, f, c, -I);
    e[ZEPHYRUM]  = a;
    f[ZEPHYRUM]  = s;
    f[I]         = a;
    c[I]         = -I;
    e[I]         = _adde(rete, NERVUS_AFFINIS, II, f, c, ZEPHYRUM);
    c[I]         = -II;
    e[II]        = _adde(rete, NERVUS_AFFINIS, II, f, c, ZEPHYRUM);
    (vacuum)nervus_exitus(rete, III, e);
    redde rete;
}

/* |x| = relu(x) + relu(-x) */
interior Nervus*
_absolutum (
    Piscina* piscina)
{
    Nervus* rete;
       s32  f[II];
       s64  c[II];
       s32  e;

    (vacuum)nervus_creare(I, piscina, &rete);
    f[ZEPHYRUM] = ZEPHYRUM;
    c[ZEPHYRUM] = I;
    (vacuum)_adde(rete, NERVUS_RELU, I, f, c, ZEPHYRUM);
    c[ZEPHYRUM]  = -I;
    (vacuum)_adde(rete, NERVUS_RELU, I, f, c, ZEPHYRUM);
    f[ZEPHYRUM]  = I;
    f[I]         = II;
    c[ZEPHYRUM]  = I;
    c[I]         = I;
    e            = _adde(rete, NERVUS_AFFINIS, II, f, c, ZEPHYRUM);
    (vacuum)nervus_exitus(rete, I, &e);
    redde rete;
}


/* ==================================================
 * Aestimator alter: recursio memorata, s64 custodita
 * ================================================== */

#define ALTER_LIMES ((s64)1 << L)

nomen structura {
     constans Nervus* rete;
                 s64* valores;
                  i8* facta;
                  i8* activa;    /* per registrum */
                 b32  exundavit;
} Alter;

interior s64
_alter_valor (
    Alter* a,
      s32  registrum_quaesitum)
{
                          s64  summa;
                          i32  j;
    constans NervusInstructio* instructio;

    si (a->facta[registrum_quaesitum])
    {
        redde a->valores[registrum_quaesitum];
    }
    instructio = &a->rete->instructiones[(i32)registrum_quaesitum
        - a->rete->introitus];
    si (!magnus_ad_s64(instructio->addendum, &summa))
    {
        a->exundavit  = VERUM;
        summa         = ZEPHYRUM;
    }
    per (j = ZEPHYRUM; j < instructio->termini; j++)
    {
        s64 v = _alter_valor(a, instructio->fontes[j]);
        s64 c = ZEPHYRUM;

        /* |c| <= 2^10, |v| <= 2^50: c v et summa (<= 2^16 termini)
         * in s64 capiunt */
        si (   !magnus_ad_s64(instructio->coefficientes[j], &c)
            || c > MXXIV || c < -MXXIV
            || v > ALTER_LIMES || v < -ALTER_LIMES)
        {
            a->exundavit = VERUM;
            perge;
        }
        summa += c * v;
    }
    si (instructio->genus == NERVUS_RELU)
    {
        a->activa[registrum_quaesitum] = (i8)(summa > ZEPHYRUM);
        si (summa < ZEPHYRUM)
        {
            summa = ZEPHYRUM;
        }
    }
    a->valores[registrum_quaesitum]  = summa;
    a->facta[registrum_quaesitum]    = I;
    redde summa;
}

/* rete fortuitum: introitus 1..4, instructiones 1..30, termini 0..4,
 * coefficientes -3..3, addenda -5..5 (aut 0 si sine_addendis) */
interior Nervus*
_fortuitum (
        Sors* s,
         b32  sine_addendis,
     Piscina* piscina)
{
    Nervus* rete;
       i32  introitus  = (i32)sors_intra(s, IV) + I;
       i32  numerus    = (i32)sors_intra(s, XXX) + I;
       i32  k;
       i32  j;
       s32  e[VI];
       i32  exitus = (i32)sors_intra(s, VI) + I;

    (vacuum)nervus_creare(introitus, piscina, &rete);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        s32 f[IV];
        s64 c[IV];
        i32 termini = (i32)sors_intra(s, V);
        i32 definita = introitus + k;
        s64 b = sine_addendis ? ZEPHYRUM : (s64)sors_inter(s, -V, V);

        per (j = ZEPHYRUM; j < termini; j++)
        {
            f[j] = (s32)sors_intra(s, definita);
            c[j] = (s64)sors_inter(s, -III, III);
        }
        (vacuum)_adde(rete, sors_intra(s, II) == ZEPHYRUM
            ? NERVUS_AFFINIS : NERVUS_RELU, termini, f, c, b);
    }
    per (k = ZEPHYRUM; k < exitus; k++)
    {
        e[k] = (s32)sors_intra(s, introitus + numerus);
    }
    (vacuum)nervus_exitus(rete, exitus, e);
    redde rete;
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior vacuum
_probare_constructionem (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
     Nervus* rete;
     Nervus* intactum = NIHIL;
        s32  f[II];
     Magnus  coef[II];
        s32  r = XCIX;
        s32  e[II];

    CREDO_FALSUM (nervus_creare(NERVUS_REGISTRA_MAXIMA + I, piscina,
        &intactum));
    CREDO_NIHIL (intactum);
    CREDO_FALSUM (nervus_creare(II, NIHIL, &rete));
    CREDO_FALSUM (nervus_creare(II, piscina, NIHIL));
    CREDO_VERUM (nervus_creare(II, piscina, &rete));
    CREDO_AEQUALIS_I32 (rete->introitus, II);
    CREDO_AEQUALIS_I32 (rete->numerus, ZEPHYRUM);
    CREDO_VERUM (nervus_validus(rete));
    /* registra successiva: 2, 3, 4 */
    f[ZEPHYRUM]     = ZEPHYRUM;
    f[I]            = I;
    coef[ZEPHYRUM]  = _m(I);
    coef[I]         = _m(-II);
    CREDO_VERUM (nervus_affinis(rete, II, f, coef, _m(III), &r));
    CREDO_AEQUALIS_S32 (r, II);
    f[ZEPHYRUM] = II;
    CREDO_VERUM (nervus_relu(rete, I, f, coef, _m(ZEPHYRUM), &r));
    CREDO_AEQUALIS_S32 (r, III);
    CREDO_VERUM (nervus_affinis(rete, ZEPHYRUM, NIHIL, NIHIL, _m(VII),
        &r));
    CREDO_AEQUALIS_S32 (r, IV);
    CREDO_AEQUALIS_I32 (rete->relu, I);
    /* refutationes: rete non mutatum, registrum non tactum */
    r            = XCIX;
    f[ZEPHYRUM]  = V;     /* nondum definitum (registra 0..4) */
    CREDO_FALSUM (nervus_affinis(rete, I, f, coef, _m(ZEPHYRUM), &r));
    f[ZEPHYRUM] = -I;
    CREDO_FALSUM (nervus_relu(rete, I, f, coef, _m(ZEPHYRUM), &r));
    CREDO_FALSUM (nervus_affinis(rete, I, NIHIL, coef, _m(ZEPHYRUM),
        &r));
    f[ZEPHYRUM] = ZEPHYRUM;
    CREDO_FALSUM (nervus_affinis(rete, I, f, NIHIL, _m(ZEPHYRUM), &r));
    CREDO_FALSUM (nervus_affinis(rete, NERVUS_TERMINI_MAXIMI + I, f,
        coef, _m(ZEPHYRUM), &r));
    CREDO_FALSUM (nervus_affinis(rete, I, f, coef, _m(ZEPHYRUM),
        NIHIL));
    CREDO_FALSUM (nervus_affinis(NIHIL, I, f, coef, _m(ZEPHYRUM), &r));
    CREDO_AEQUALIS_S32 (r, XCIX);
    CREDO_AEQUALIS_I32 (rete->numerus, III);
    CREDO_AEQUALIS_I32 (rete->relu, I);
    /* fons = ultimum definitum licet */
    f[ZEPHYRUM] = IV;
    CREDO_VERUM (nervus_affinis(rete, I, f, coef, _m(ZEPHYRUM), &r));
    CREDO_AEQUALIS_S32 (r, V);
    /* exitus */
    e[ZEPHYRUM]  = V;
    e[I]         = ZEPHYRUM;
    CREDO_VERUM (nervus_exitus(rete, II, e));
    CREDO_AEQUALIS_I32 (rete->exitus_numerus, II);
    e[ZEPHYRUM] = VI;
    CREDO_FALSUM (nervus_exitus(rete, II, e));
    e[ZEPHYRUM] = -I;
    CREDO_FALSUM (nervus_exitus(rete, II, e));
    CREDO_FALSUM (nervus_exitus(rete, II, NIHIL));
    CREDO_FALSUM (nervus_exitus(rete, NERVUS_REGISTRA_MAXIMA + I, e));
    CREDO_AEQUALIS_I32 (rete->exitus_numerus, II);
    CREDO_AEQUALIS_S32 (rete->exitus[ZEPHYRUM], V);
    CREDO_VERUM (nervus_exitus(rete, ZEPHYRUM, NIHIL));
    CREDO_AEQUALIS_I32 (rete->exitus_numerus, ZEPHYRUM);
    CREDO_VERUM (nervus_validus(rete));
    /* crescere ultra capacitatem primam (XVI) */
    {
        i32 k;
        b32 omnes = VERUM;

        per (k = ZEPHYRUM; k < C; k++)
        {
            f[ZEPHYRUM] = (s32)(k + I);
            si (   !nervus_relu(rete, I, f, coef, _m(ZEPHYRUM), &r)
                || r != (s32)(k + VI))
            {
                omnes = FALSUM;
            }
        }
        CREDO_VERUM (omnes);
        CREDO_AEQUALIS_I32 (rete->numerus, CIV);
        CREDO_AEQUALIS_I32 (rete->relu, CI);
        CREDO_VERUM (nervus_validus(rete));
    }
    /* fines exacti: termini = MAXIMI accipitur; registra usque ad
     * MAXIMA - 1 */
    {
        Nervus* latum   = NIHIL;
        Nervus* plenum  = NIHIL;
           s32* ff;
        Magnus* cc;
        Magnus  x[I];
        Magnus  y[I];
           s32  q = XCIX;
           i32  k;

        ff = (s32*)piscina_allocare(piscina,
            (memoriae_index)NERVUS_TERMINI_MAXIMI * magnitudo(s32));
        cc = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)NERVUS_TERMINI_MAXIMI * magnitudo(Magnus));
        per (k = ZEPHYRUM; k < NERVUS_TERMINI_MAXIMI; k++)
        {
            ff[k] = ZEPHYRUM;
            cc[k] = _m(I);
        }
        (vacuum)nervus_creare(I, piscina, &latum);
        CREDO_VERUM (nervus_affinis(latum, NERVUS_TERMINI_MAXIMI, ff,
            cc,
            _m(ZEPHYRUM), &q));
        (vacuum)nervus_exitus(latum, I, &q);
        x[ZEPHYRUM] = _m(III);
        CREDO_VERUM (nervus_aestimare(latum, x, y, NIHIL, piscina));
        CREDO_VERUM (_est(y[ZEPHYRUM],
            (s64)III * NERVUS_TERMINI_MAXIMI));
        /* introitus = MAXIMA: rete validum, sed nulla instructio */
        CREDO_VERUM (nervus_creare(NERVUS_REGISTRA_MAXIMA, piscina,
            &plenum));
        CREDO_VERUM (nervus_validus(plenum));
        CREDO_FALSUM (nervus_affinis(plenum, ZEPHYRUM, NIHIL, NIHIL,
            _m(I), &q));
        /* introitus = MAXIMA - 1: una instructio, deinde plenum */
        CREDO_VERUM (nervus_creare(NERVUS_REGISTRA_MAXIMA - I, piscina,
            &plenum));
        CREDO_VERUM (nervus_affinis(plenum, ZEPHYRUM, NIHIL, NIHIL,
            _m(I),
            &q));
        CREDO_AEQUALIS_S32 (q, (s32)(NERVUS_REGISTRA_MAXIMA - I));
        CREDO_FALSUM (nervus_affinis(plenum, ZEPHYRUM, NIHIL, NIHIL,
            _m(I), &q));
        CREDO_AEQUALIS_I32 (plenum->numerus, I);
    }
    /* coefficientes transcripti: valor vocantis mutatus rete non
     * tangit */
    {
        Nervus* parvum;
        Magnus  cc[I];
        Magnus  x[I];
        Magnus  y[I];
           s32  g[I];
           s32  q;

        (vacuum)nervus_creare(I, piscina, &parvum);
        g[ZEPHYRUM]   = ZEPHYRUM;
        cc[ZEPHYRUM]  = magnus_potentia(_m(II), CC, piscina);
        (vacuum)nervus_affinis(parvum, I, g, cc, _m(ZEPHYRUM), &q);
        (vacuum)nervus_exitus(parvum, I, &q);
        cc[ZEPHYRUM]  = _m(I);
        g[ZEPHYRUM]   = XL;
        x[ZEPHYRUM]   = _m(III);
        CREDO_VERUM (nervus_aestimare(parvum, x, y, NIHIL, piscina));
        CREDO_VERUM (magnus_aequalis(y[ZEPHYRUM], magnus_multiplica(
            _m(III), magnus_potentia(_m(II), CC, piscina), piscina)));
    }
}

/* retia manu corrupta: validus et aestimare refutant */
interior vacuum
_probare_corruptionem (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
      Nervus* rete;
      Nervus  copia;
      Magnus  x[II];
      Magnus  y[III];
         s32  f[I];
         s32  exitus_unum;
         s32* fontes_primi;

    x[ZEPHYRUM]  = _m(I);
    x[I]         = _m(I);
    y[ZEPHYRUM]  = _m(XCIX);
    rete         = _portae(piscina);
    CREDO_VERUM (nervus_validus(rete));
    /* copia capitis; alvei communes manu restituuntur */
    copia         = *rete;
    exitus_unum   = rete->exitus[I];
    fontes_primi  = rete->instructiones[ZEPHYRUM].fontes;
    /* fons ad se ipsum */
    f[ZEPHYRUM] = rete->instructiones[I].fontes[ZEPHYRUM];
    rete->instructiones[I].fontes[ZEPHYRUM] = III;
    CREDO_FALSUM (nervus_validus(rete));
    CREDO_FALSUM (nervus_aestimare(rete, x, y, NIHIL, piscina));
    CREDO_VERUM (_est(y[ZEPHYRUM], XCIX));
    rete->instructiones[I].fontes[ZEPHYRUM] = -I;
    CREDO_FALSUM (nervus_validus(rete));
    rete->instructiones[I].fontes[ZEPHYRUM] = f[ZEPHYRUM];
    CREDO_VERUM (nervus_validus(rete));
    /* genus ignotum */
    rete->instructiones[ZEPHYRUM].genus = (NervusGenus)VII;
    CREDO_FALSUM (nervus_validus(rete));
    rete->instructiones[ZEPHYRUM].genus = NERVUS_AFFINIS;
    /* relu male numeratum */
    rete->relu = II;
    CREDO_FALSUM (nervus_validus(rete));
    *rete = copia;
    /* exitus extra fines */
    rete->exitus[I] = (s32)(rete->introitus + rete->numerus);
    CREDO_FALSUM (nervus_validus(rete));
    rete->exitus[I] = exitus_unum;
    CREDO_VERUM (nervus_validus(rete));
    /* numeri et alvei */
    rete->numerus = rete->capacitas + I;
    CREDO_FALSUM (nervus_validus(rete));
    *rete                = copia;
    rete->instructiones  = NIHIL;
    CREDO_FALSUM (nervus_validus(rete));
    *rete         = copia;
    rete->exitus  = NIHIL;
    CREDO_FALSUM (nervus_validus(rete));
    *rete                                 = copia;
    rete->instructiones[ZEPHYRUM].fontes  = NIHIL;
    CREDO_FALSUM (nervus_validus(rete));
    rete->instructiones[ZEPHYRUM].fontes  = fontes_primi;
    rete->introitus                       = NERVUS_REGISTRA_MAXIMA;
    CREDO_FALSUM (nervus_validus(rete));
    *rete = copia;
    CREDO_FALSUM (nervus_validus(NIHIL));
    /* restitutum iterum validum et recte aestimat */
    CREDO_VERUM (nervus_validus(rete));
    CREDO_VERUM (nervus_aestimare(rete, x, y, NIHIL, piscina));
    CREDO_VERUM (_est(y[ZEPHYRUM], I) && _est(y[I], I)
        && _est(y[II], ZEPHYRUM));
}

interior vacuum
_probare_manu (
    CredoContextus* c)
{
     Piscina* piscina    = c->piscina;
      Nervus* portae     = _portae(piscina);
      Nervus* absolutum  = _absolutum(piscina);
      Magnus  x[II];
      Magnus  y[III];
         s32  a;
         s32  b;
         b32  omnes = VERUM;

    /* AND, OR, XOR in {0,1}^2 */
    per (a = ZEPHYRUM; a <= I; a++)
    {
        per (b = ZEPHYRUM; b <= I; b++)
        {
            x[ZEPHYRUM]  = _m(a);
            x[I]         = _m(b);
            si (   !nervus_aestimare(portae, x, y, NIHIL, piscina)
                || !_est(y[ZEPHYRUM], a & b)
                || !_est(y[I], a | b)
                || !_est(y[II], a ^ b))
            {
                omnes = FALSUM;
            }
        }
    }
    CREDO_VERUM (omnes);
    /* |x| in -50..50 et 2^200 */
    omnes = VERUM;
    per (a = -L; a <= L; a++)
    {
        x[ZEPHYRUM] = _m(a);
        si (   !nervus_aestimare(absolutum, x, y, NIHIL, piscina)
            || !_est(y[ZEPHYRUM], a < ZEPHYRUM ? -a : a))
        {
            omnes = FALSUM;
        }
    }
    CREDO_VERUM (omnes);
    {
        Magnus magnum = magnus_potentia(_m(II), CC, piscina);

        x[ZEPHYRUM] = magnus_nega(magnum, piscina);
        CREDO_VERUM (nervus_aestimare(absolutum, x, y, NIHIL, piscina));
        CREDO_VERUM (magnus_aequalis(y[ZEPHYRUM], magnum));
    }
    /* max(x, y) = y + relu(x - y), etiam magnis */
    {
        Nervus* maximum;
           s32  f[II];
           s64  cc[II];
           s32  t;
           s32  e;
        Magnus  magnum = magnus_potentia(_m(III), CXXVIII, piscina);

        (vacuum)nervus_creare(II, piscina, &maximum);
        f[ZEPHYRUM] = ZEPHYRUM;
        f[I] = I;
        cc[ZEPHYRUM] = I;
        cc[I] = -I;
        t = _adde(maximum, NERVUS_RELU, II, f, cc, ZEPHYRUM);
        f[ZEPHYRUM] = I;
        f[I] = t;
        cc[I] = I;
        e = _adde(maximum, NERVUS_AFFINIS, II, f, cc, ZEPHYRUM);
        (vacuum)nervus_exitus(maximum, I, &e);
        x[ZEPHYRUM]  = magnum;
        x[I]         = magnus_adde(magnum, _m(I), piscina);
        CREDO_VERUM (nervus_aestimare(maximum, x, y, NIHIL, piscina));
        CREDO_VERUM (magnus_aequalis(y[ZEPHYRUM], x[I]));
        x[I] = magnus_subtrahe(magnum, _m(I), piscina);
        CREDO_VERUM (nervus_aestimare(maximum, x, y, NIHIL, piscina));
        CREDO_VERUM (magnus_aequalis(y[ZEPHYRUM], magnum));
    }
    /* rete constans (introitus 0) et exitus qui introitus est */
    {
        Nervus* constans_rete;
        Nervus* identitas;
           s32  e;
           s32  exitus_bini[II];

        (vacuum)nervus_creare(ZEPHYRUM, piscina, &constans_rete);
        CREDO_VERUM (nervus_affinis(constans_rete, ZEPHYRUM, NIHIL,
            NIHIL,
            _m(-XLII), &e));
        CREDO_AEQUALIS_S32 (e, ZEPHYRUM);
        (vacuum)nervus_exitus(constans_rete, I, &e);
        CREDO_VERUM (nervus_aestimare(constans_rete, NIHIL, y, NIHIL,
            piscina));
        CREDO_VERUM (_est(y[ZEPHYRUM], -XLII));
        (vacuum)nervus_creare(II, piscina, &identitas);
        exitus_bini[ZEPHYRUM]  = I;
        exitus_bini[I]         = I;
        CREDO_VERUM (nervus_exitus(identitas, II, exitus_bini));
        x[ZEPHYRUM]  = _m(V);
        x[I]         = _m(-VII);
        CREDO_VERUM (nervus_aestimare(identitas, x, y, NIHIL, piscina));
        CREDO_VERUM (_est(y[ZEPHYRUM], -VII) && _est(y[I], -VII));
    }
    /* refutationes aestimationis: exitus non tangitur */
    y[ZEPHYRUM] = _m(XCIX);
    CREDO_FALSUM (nervus_aestimare(portae, NIHIL, y, NIHIL, piscina));
    CREDO_FALSUM (nervus_aestimare(portae, x, NIHIL, NIHIL, piscina));
    CREDO_FALSUM (nervus_aestimare(portae, x, y, NIHIL, NIHIL));
    CREDO_VERUM (_est(y[ZEPHYRUM], XCIX));
}

interior vacuum
_probare_formam (
    CredoContextus* c)
{
     Piscina* piscina    = c->piscina;
      Nervus* absolutum  = _absolutum(piscina);
      Nervus* portae     = _portae(piscina);
 NervusForma  forma;
 NervusForma  aliena;
      Magnus  x[II];
      Magnus  y[III];

    CREDO_VERUM (nervus_forma_creare(absolutum, piscina, &forma));
    CREDO_AEQUALIS_I32 (forma.numerus, II);
    CREDO_NON_NIHIL (forma.bitus);
    /* x = 5: relu(x) activa, relu(-x) non */
    x[ZEPHYRUM] = _m(V);
    CREDO_VERUM (nervus_aestimare(absolutum, x, y, &forma, piscina));
    CREDO_VERUM (nervus_forma_bitus(&forma, ZEPHYRUM));
    CREDO_FALSUM (nervus_forma_bitus(&forma, I));
    /* x = -3: contrarium; bitus prior deletur */
    x[ZEPHYRUM] = _m(-III);
    CREDO_VERUM (nervus_aestimare(absolutum, x, y, &forma, piscina));
    CREDO_FALSUM (nervus_forma_bitus(&forma, ZEPHYRUM));
    CREDO_VERUM (nervus_forma_bitus(&forma, I));
    /* x = 0: argumentum 0 -> non activa (conventio) */
    x[ZEPHYRUM] = _m(ZEPHYRUM);
    CREDO_VERUM (nervus_aestimare(absolutum, x, y, &forma, piscina));
    CREDO_FALSUM (nervus_forma_bitus(&forma, ZEPHYRUM));
    CREDO_FALSUM (nervus_forma_bitus(&forma, I));
    CREDO_FALSUM (nervus_forma_bitus(&forma, II));
    CREDO_FALSUM (nervus_forma_bitus(NIHIL, ZEPHYRUM));
    /* forma alius retis refutatur, exitus non tangitur */
    CREDO_VERUM (nervus_forma_creare(portae, piscina, &aliena));
    CREDO_AEQUALIS_I32 (aliena.numerus, I);
    y[ZEPHYRUM] = _m(XCIX);
    CREDO_FALSUM (nervus_aestimare(absolutum, x, y, &aliena, piscina));
    CREDO_VERUM (_est(y[ZEPHYRUM], XCIX));
    /* rete sine ReLU: forma numerus 0, bitus non NIHIL */
    {
          Nervus* linearis;
     NervusForma  vacua;
             s32  f[I];
          Magnus  cc[I];
             s32  r;

        (vacuum)nervus_creare(I, piscina, &linearis);
        f[ZEPHYRUM]   = ZEPHYRUM;
        cc[ZEPHYRUM]  = _m(II);
        (vacuum)nervus_affinis(linearis, I, f, cc, _m(ZEPHYRUM), &r);
        (vacuum)nervus_exitus(linearis, I, &r);
        CREDO_VERUM (nervus_forma_creare(linearis, piscina, &vacua));
        CREDO_AEQUALIS_I32 (vacua.numerus, ZEPHYRUM);
        CREDO_NON_NIHIL (vacua.bitus);
        CREDO_VERUM (nervus_aestimare(linearis, x, y, &vacua, piscina));
    }
    /* XVII ReLU: bitus trans octetos (k = 7, 8, 16) */
    {
          Nervus* multae;
     NervusForma  forma_multa;
             s32  f[I];
          Magnus  cc[I];
             s32  r;
             i32  k;
             b32  recte = VERUM;

        (vacuum)nervus_creare(I, piscina, &multae);
        f[ZEPHYRUM]   = ZEPHYRUM;
        cc[ZEPHYRUM]  = _m(I);
        per (k = ZEPHYRUM; k < XVII; k++)
        {
            /* relu(x - k): activa si x > k */
            (vacuum)nervus_relu(multae, I, f, cc, _m(-(s64)k), &r);
        }
        (vacuum)nervus_exitus(multae, I, &r);
        CREDO_VERUM (nervus_forma_creare(multae, piscina,
            &forma_multa));
        CREDO_AEQUALIS_I32 (forma_multa.numerus, XVII);
        x[ZEPHYRUM] = _m(VIII);     /* activae k = 0..7 */
        CREDO_VERUM (nervus_aestimare(multae, x, y, &forma_multa,
            piscina));
        per (k = ZEPHYRUM; k < XVII; k++)
        {
            si (nervus_forma_bitus(&forma_multa, k) != (k < VIII))
            {
                recte = FALSUM;
            }
        }
        CREDO_VERUM (recte);
        x[ZEPHYRUM] = _m(XX);       /* omnes activae */
        CREDO_VERUM (nervus_aestimare(multae, x, y, &forma_multa,
            piscina));
        CREDO_VERUM (nervus_forma_bitus(&forma_multa, XVI));
        x[ZEPHYRUM] = _m(-I);       /* nulla */
        CREDO_VERUM (nervus_aestimare(multae, x, y, &forma_multa,
            piscina));
        recte = VERUM;
        per (k = ZEPHYRUM; k < XVII; k++)
        {
            si (nervus_forma_bitus(&forma_multa, k))
            {
                recte = FALSUM;
            }
        }
        CREDO_VERUM (recte);
    }
}

interior vacuum
_probare_figuram (
    CredoContextus* c)
{
      Piscina* piscina = c->piscina;
 NervusFigura  fig;
       Nervus* rete;

    /* portae: s, a = relu(s-1), OR, XOR */
    CREDO_VERUM (nervus_figura(_portae(piscina), &fig));
    CREDO_AEQUALIS_I32 (fig.instructiones, IV);
    CREDO_AEQUALIS_I32 (fig.relu, I);
    CREDO_VERUM (fig.termini == (i64)VII);
    CREDO_AEQUALIS_I32 (fig.profunditas, III);
    CREDO_AEQUALIS_I32 (fig.profunditas_relu, I);
    /* vivi: x,y -> s -> s,a -> s,a,OR -> a,OR,XOR : III */
    CREDO_AEQUALIS_I32 (fig.latitudo, III);
    /* |x|: x -> x,r1 -> r1,r2 -> r3 : II */
    CREDO_VERUM (nervus_figura(_absolutum(piscina), &fig));
    CREDO_AEQUALIS_I32 (fig.profunditas, II);
    CREDO_AEQUALIS_I32 (fig.profunditas_relu, I);
    CREDO_AEQUALIS_I32 (fig.latitudo, II);
    /* catena X ReLU: profunditas X, latitudo I */
    {
        s32 f[X];
        s64 cc[X];
        s32 r = ZEPHYRUM;
        i32 k;

        (vacuum)nervus_creare(I, piscina, &rete);
        cc[ZEPHYRUM] = I;
        per (k = ZEPHYRUM; k < X; k++)
        {
            f[ZEPHYRUM]  = r;
            r            = _adde(rete, NERVUS_RELU, I, f, cc, ZEPHYRUM);
        }
        (vacuum)nervus_exitus(rete, I, &r);
        CREDO_VERUM (nervus_figura(rete, &fig));
        CREDO_AEQUALIS_I32 (fig.profunditas, X);
        CREDO_AEQUALIS_I32 (fig.profunditas_relu, X);
        CREDO_AEQUALIS_I32 (fig.latitudo, I);
        /* stratum latum: X ReLU ex x, deinde summa */
        (vacuum)nervus_creare(I, piscina, &rete);
        per (k = ZEPHYRUM; k < X; k++)
        {
            f[ZEPHYRUM] = ZEPHYRUM;
            (vacuum)_adde(rete, NERVUS_RELU, I, f, cc, -(s64)k);
        }
        per (k = ZEPHYRUM; k < X; k++)
        {
            f[k]   = (s32)(k + I);
            cc[k]  = I;
        }
        r = _adde(rete, NERVUS_AFFINIS, X, f, cc, ZEPHYRUM);
        (vacuum)nervus_exitus(rete, I, &r);
        CREDO_VERUM (nervus_figura(rete, &fig));
        CREDO_AEQUALIS_I32 (fig.profunditas, II);
        CREDO_AEQUALIS_I32 (fig.latitudo, X);
        CREDO_VERUM (fig.termini == (i64)XX);
    }
    /* rete vacuum; introitus non lectus non vivit */
    {
        s32 e = ZEPHYRUM;

        (vacuum)nervus_creare(III, piscina, &rete);
        CREDO_VERUM (nervus_figura(rete, &fig));
        CREDO_AEQUALIS_I32 (fig.profunditas, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (fig.latitudo, ZEPHYRUM);
        (vacuum)nervus_exitus(rete, I, &e);
        CREDO_VERUM (nervus_figura(rete, &fig));
        CREDO_AEQUALIS_I32 (fig.latitudo, I);
    }
    CREDO_FALSUM (nervus_figura(NIHIL, &fig));
    CREDO_FALSUM (nervus_figura(_portae(piscina), NIHIL));
}

interior vacuum
_probare_contra_alterum (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
        Sors  s;
         i32  t;
         i32  comparata     = ZEPHYRUM;
         i32  discrepantia  = ZEPHYRUM;
         i32  exundata      = ZEPHYRUM;

    sors_seminare(&s, (i64)MMXXVI, I);
    per (t = ZEPHYRUM; t < M; t++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                Nervus* rete = _fortuitum(&s, FALSUM, piscina);
           NervusForma  forma;
                Magnus  x[IV];
                Magnus  y[VI];
                   i32  registra = rete->introitus + rete->numerus;
                 Alter  a;
                   i32  k;
                   s32  r     = ZEPHYRUM;
                   b32  idem  = VERUM;

        per (k = ZEPHYRUM; k < rete->introitus; k++)
        {
            x[k] = _m((s64)sors_inter(&s, -XX, XX));
        }
        (vacuum)nervus_forma_creare(rete, piscina, &forma);
        si (!nervus_aestimare(rete, x, y, &forma, piscina))
        {
            idem = FALSUM;
        }
        a.rete       = rete;
        a.exundavit  = FALSUM;
        a.valores    = (s64*)piscina_allocare(piscina,
            (memoriae_index)registra * magnitudo(s64));
        a.facta      = (i8*)piscina_allocare(piscina,
            (memoriae_index)registra);
        a.activa     = (i8*)piscina_allocare(piscina,
            (memoriae_index)registra);
        per (k = ZEPHYRUM; k < registra; k++)
        {
            a.facta[k]   = (i8)(k < rete->introitus);
            a.activa[k]  = ZEPHYRUM;
            si (k < rete->introitus)
            {
                (vacuum)magnus_ad_s64(x[k], &a.valores[k]);
            }
        }
        /* exitus, deinde omnes ReLU (pro forma) */
        per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
        {
            s64 v = _alter_valor(&a, rete->exitus[k]);
            s64 w = ZEPHYRUM;

            si (!a.exundavit && (!magnus_ad_s64(y[k], &w) || v != w))
            {
                idem = FALSUM;
            }
        }
        per (k = ZEPHYRUM; k < rete->numerus; k++)
        {
            si (rete->instructiones[k].genus == NERVUS_RELU)
            {
                (vacuum)_alter_valor(&a, (s32)(rete->introitus + k));
                si (!a.exundavit && nervus_forma_bitus(&forma, (i32)r)
                    != (a.activa[rete->introitus + k] != ZEPHYRUM))
                {
                    idem = FALSUM;
                }
                r++;
            }
        }
        si (a.exundavit)
        {
            exundata++;
        }
        alioquin
        {
            comparata++;
            si (!idem)
            {
                discrepantia++;
            }
        }
        piscina_reficere(piscina, nota);
    }
    imprimere("  contra alterum: %u comparata, %u exundata, %u "
        "discrepantia\n", comparata, exundata, discrepantia);
    CREDO_AEQUALIS_I32 (discrepantia, ZEPHYRUM);
    CREDO_MAIOR_I32 (comparata, CM);
}

interior vacuum
_probare_homogeneitatem (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
        Sors  s;
         i32  t;
         b32  omnes = VERUM;
      Magnus  scala = magnus_potentia(_m(II), CC, piscina);

    sors_seminare(&s, (i64)MMXXVI + I, I);
    per (t = ZEPHYRUM; t < D; t++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                Nervus* rete = _fortuitum(&s, VERUM, piscina);
           NervusForma  fa;
           NervusForma  fb;
                Magnus  x[IV];
                Magnus  tx[IV];
                Magnus  y[VI];
                Magnus  ty[VI];
                   i32  k;

        per (k = ZEPHYRUM; k < rete->introitus; k++)
        {
            x[k]   = _m((s64)sors_inter(&s, -C, C));
            tx[k]  = magnus_multiplica(x[k], scala, piscina);
        }
        (vacuum)nervus_forma_creare(rete, piscina, &fa);
        (vacuum)nervus_forma_creare(rete, piscina, &fb);
        si (   !nervus_aestimare(rete, x, y, &fa, piscina)
            || !nervus_aestimare(rete, tx, ty, &fb, piscina))
        {
            omnes = FALSUM;
        }
        per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
        {
            si (!magnus_aequalis(ty[k], magnus_multiplica(y[k], scala,
                piscina)))
            {
                omnes = FALSUM;
            }
        }
        per (k = ZEPHYRUM; k < rete->relu; k++)
        {
            si (nervus_forma_bitus(&fa, k) != nervus_forma_bitus(&fb,
                k))
            {
                omnes = FALSUM;
            }
        }
        piscina_reficere(piscina, nota);
    }
    CREDO_VERUM (omnes);
}

/* gyrus cum notare / reficere: usus piscinae stabilis */
interior vacuum
_probare_memoriam (
    CredoContextus* c)
{
       Piscina* piscina  = c->piscina;
        Nervus* portae   = _portae(piscina);
        Magnus  x[II];
        Magnus  y[III];
           i32  t;
memoriae_index  ante;
memoriae_index  post = ZEPHYRUM;

    x[ZEPHYRUM]  = _m(I);
    x[I]         = _m(ZEPHYRUM);
    {
        PiscinaNotatio nota = piscina_notare(piscina);

        (vacuum)nervus_aestimare(portae, x, y, NIHIL, piscina);
        piscina_reficere(piscina, nota);
    }
    ante = piscina_summa_usus(piscina);
    per (t = ZEPHYRUM; t < M; t++)
    {
        PiscinaNotatio nota = piscina_notare(piscina);

        (vacuum)nervus_aestimare(portae, x, y, NIHIL, piscina);
        piscina_reficere(piscina, nota);
        post = piscina_summa_usus(piscina);
    }
    CREDO_VERUM (ante == post);
}

interior CredoSectio SECTIONES[] = {
    { "constructio", _probare_constructionem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "corruptio", _probare_corruptionem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "manu", _probare_manu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma", _probare_formam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "figura", _probare_figuram, NIHIL, NIHIL, NIHIL, NIHIL },
    { "contra alterum", _probare_contra_alterum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "homogeneitas", _probare_homogeneitatem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "memoria", _probare_memoriam, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("nervus", SECTIONES);
}
