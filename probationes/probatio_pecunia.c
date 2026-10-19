/* probatio_pecunia.c - pecunia (pecunia-plan): tabula monetarum
 * GENERATA ex indicibus ISO 4217 fixis (probationes/fixa/iso4217/)
 * + supplementum, quaestio codicum (sine casu, nihil aliud
 * condonatur), vitia nominata; arithmetica quae recusat (T2:
 * superfluxus, monetae diversae, exitus intactus, proprietates). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "pecunia.h"
#include "pecunia_monetae.h"
#include "sors.h"
#include <string.h>

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* moneta quaesita; FALSUM si non inventa */
interior constans PecuniaMonetaInformatio*
_informatio (
    constans character* codex,
               Piscina* p)
{
    PecuniaMoneta m;

    si (pecunia_moneta_ex_codice(_c(codex, p), &m) != PECUNIA_BENE)
    {
        redde NIHIL;
    }
    redde pecunia_moneta_informatio(m);
}

interior vacuum
_sectio_tabula (CredoContextus* c)
{
    constans PecuniaMonetaInformatio* m;
                                 i32  i;
                                 i32  euro = ZEPHYRUM;

    m = _informatio("USD", c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32(m->exponens, II);
    CREDO_AEQUALIS_S32(m->numerus, DCCCXL);
    CREDO_AEQUALIS_S32((s32)m->status, (s32)PECUNIA_MONETA_CURRENS);
    CREDO_VERUM(strcmp(m->titulus, "US Dollar") == ZEPHYRUM);

    m = _informatio("JPY", c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32(m->exponens, ZEPHYRUM);
    m = _informatio("KWD", c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32(m->exponens, III);
    m = _informatio("CLF", c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32(m->exponens, IV);

    /* supplementum: historica et extra ISO */
    m = _informatio("BGN", c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32(m->exponens, II);
    CREDO_AEQUALIS_S32(m->numerus, CMLXXV);   /* ex indice tertio */
    CREDO_AEQUALIS_S32((s32)m->status, (s32)PECUNIA_MONETA_HISTORICA);
    m = _informatio("MRO", c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32((s32)m->status, (s32)PECUNIA_MONETA_HISTORICA);
    m = _informatio("GGP", c->piscina);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_AEQUALIS_S32(m->numerus, ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)m->status, (s32)PECUNIA_MONETA_EXTRA_ISO);

    /* CLXV currentes (CLXXVIII - XIII N.A.) + X supplementi */
    CREDO_AEQUALIS_I32(PECUNIA_MONETAE_NUMERUS, CLXXV);
    per (i = ZEPHYRUM; i < PECUNIA_MONETAE_NUMERUS; i++)
    {
        si (strcmp(PECUNIA_MONETAE[i].codex, "EUR") == ZEPHYRUM)
        {
            euro++;
        }
        si (i > ZEPHYRUM)
        {
            CREDO_VERUM(strcmp(PECUNIA_MONETAE[i - I].codex,
                PECUNIA_MONETAE[i].codex) < ZEPHYRUM);
        }
        CREDO_VERUM(PECUNIA_MONETAE[i].exponens >= ZEPHYRUM
            && PECUNIA_MONETAE[i].exponens <= IV);
        /* omnis ordo per codicem suum invenitur (margines inclusi) */
        CREDO_VERUM(_informatio(PECUNIA_MONETAE[i].codex, c->piscina)
            == &PECUNIA_MONETAE[i]);
    }
    CREDO_AEQUALIS_I32(euro, I);
    CREDO_AEQUALIS_I32(PECUNIA_MONETAE_SINE_UNITATE_NUMERUS, XIII);
}

/* codex malus: vitium nominatum, exitus intactus */
interior vacuum
_codex_malus (
    constans character* codex,
         PecuniaVitium  exspectatum,
              Piscina* p)
{
    PecuniaMoneta m = { "ZZZ" };

    CREDO_AEQUALIS_S32((s32)pecunia_moneta_ex_codice(_c(codex, p), &m),
        (s32)exspectatum);
    CREDO_VERUM(memcmp(m.codex, "ZZZ", IV) == ZEPHYRUM);
}

interior vacuum
_sectio_codices (CredoContextus* c)
{
    PecuniaMoneta a;
    PecuniaMoneta b;
    PecuniaMoneta d;

    CREDO_AEQUALIS_S32((s32)pecunia_moneta_ex_codice(_c("usd",
        c->piscina), &a), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S32((s32)pecunia_moneta_ex_codice(_c("Usd",
        c->piscina), &b), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S32((s32)pecunia_moneta_ex_codice(_c("USD",
        c->piscina), &d), (s32)PECUNIA_BENE);
    CREDO_VERUM(memcmp(a.codex, "USD", IV) == ZEPHYRUM);
    CREDO_VERUM(pecunia_moneta_aequalis(a, b));
    CREDO_VERUM(pecunia_moneta_aequalis(b, d));
    CREDO_AEQUALIS_S32(pecunia_moneta_exponens(a), II);

    _codex_malus(" usd", PECUNIA_VITIUM_MONETA_IGNOTA, c->piscina);
    _codex_malus("usd ", PECUNIA_VITIUM_MONETA_IGNOTA, c->piscina);
    _codex_malus("usdd", PECUNIA_VITIUM_MONETA_IGNOTA, c->piscina);
    _codex_malus("us", PECUNIA_VITIUM_MONETA_IGNOTA, c->piscina);
    _codex_malus("", PECUNIA_VITIUM_MONETA_IGNOTA, c->piscina);
    _codex_malus("U$D", PECUNIA_VITIUM_MONETA_IGNOTA, c->piscina);
    _codex_malus("BTC", PECUNIA_VITIUM_MONETA_IGNOTA, c->piscina);
    _codex_malus("XAU", PECUNIA_VITIUM_MONETA_SINE_UNITATE, c->piscina);
    _codex_malus("xxx", PECUNIA_VITIUM_MONETA_SINE_UNITATE, c->piscina);
}

interior vacuum
_sectio_vitia (CredoContextus* c)
{
    s32 i;
    s32 k;

    (vacuum)c;
    per (i = ZEPHYRUM; i <= (s32)PECUNIA_VITIUM_RATIONES; i++)
    {
        constans character* t =
            pecunia_vitium_titulus((PecuniaVitium)i);

        CREDO_NECESSE_NON_NIHIL(t);
        CREDO_VERUM(t[ZEPHYRUM] != '\0');
        per (k = ZEPHYRUM; k < i; k++)
        {
            CREDO_VERUM(strcmp(t, pecunia_vitium_titulus(
                (PecuniaVitium)k)) != ZEPHYRUM);
        }
    }
}

#define MAXIMA PECUNIA_QUANTITAS_MAXIMA
#define MINIMA PECUNIA_QUANTITAS_MINIMA

interior PecuniaMoneta
_moneta (
    constans character* codex,
               Piscina* p)
{
    PecuniaMoneta m = { "ZZZ" };

    (vacuum)pecunia_moneta_ex_codice(_c(codex, p), &m);
    redde m;
}

interior Pecunia
_p (
                   s64  quantitas,
    constans character* codex,
               Piscina* p)
{
    Pecunia x;

    x.moneta     = _moneta(codex, p);
    x.quantitas  = quantitas;
    redde x;
}

/* exitus praeimpletus signo: post vitium intactus esse debet */
interior Pecunia
_signum_intactum (vacuum)
{
    Pecunia x;

    memcpy(x.moneta.codex, "QQQ", IV);
    x.quantitas = (s64)DCCLXXVII;
    redde x;
}

interior b32
_intactum (
    Pecunia x)
{
    redde x.quantitas == (s64)DCCLXXVII
        && memcmp(x.moneta.codex, "QQQ", IV) == ZEPHYRUM;
}

interior vacuum
_sectio_unitates (CredoContextus* c)
{
    Pecunia x;
    Pecunia y = _signum_intactum();

    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(-MCCXXXIV,
        _moneta("USD", c->piscina), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, -(s64)(CXXIII * M + CD));
    CREDO_VERUM(memcmp(x.moneta.codex, "USD", IV) == ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(V,
        _moneta("JPY", c->piscina), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(L * M));
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(I,
        _moneta("CLF", c->piscina), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)I);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(I,
        _moneta("KWD", c->piscina), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)X);

    /* margines: USD factor C */
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(MAXIMA / C,
        _moneta("USD", c->piscina), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(-(MAXIMA / C),
        _moneta("USD", c->piscina), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(MAXIMA / C + I,
        _moneta("USD", c->piscina), &y),
        (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_VERUM(_intactum(y));
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(-(MAXIMA / C) - I,
        _moneta("USD", c->piscina), &y),
        (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_VERUM(_intactum(y));
    /* CLF factor I: omnis s64 */
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(MINIMA,
        _moneta("CLF", c->piscina), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, MINIMA);
    /* moneta non ex pecunia_moneta_ex_codice */
    CREDO_AEQUALIS_S32((s32)pecunia_ex_unitatibus(I,
        _moneta("ZZZ", c->piscina), &y),
        (s32)PECUNIA_VITIUM_MONETA_IGNOTA);
    CREDO_VERUM(_intactum(y));
}

interior vacuum
_sectio_arithmetica (CredoContextus* c)
{
    Piscina* p = c->piscina;
    Pecunia  x;
    Pecunia  y = _signum_intactum();
        s32  r;

    x = pecunia_nihil(_moneta("USD", p));
    CREDO_AEQUALIS_S64(x.quantitas, (s64)ZEPHYRUM);
    CREDO_VERUM(memcmp(x.moneta.codex, "USD", IV) == ZEPHYRUM);

    CREDO_AEQUALIS_S32((s32)pecunia_adde(_p(X * M, "USD", p),
        _p(XXV * M, "USD", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(XXXV * M));
    CREDO_AEQUALIS_S32((s32)pecunia_subtrahe(_p(X * M, "USD", p),
        _p(XXV * M, "USD", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, -(s64)(XV * M));
    CREDO_AEQUALIS_S32((s32)pecunia_nega(_p(X * M, "USD", p), &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, -(s64)(X * M));

    /* monetae diversae: numquam convertuntur */
    CREDO_AEQUALIS_S32((s32)pecunia_adde(_p(I, "USD", p),
        _p(I, "EUR", p), &y), (s32)PECUNIA_VITIUM_MONETAE_DIVERSAE);
    CREDO_VERUM(_intactum(y));
    CREDO_AEQUALIS_S32((s32)pecunia_subtrahe(_p(I, "USD", p),
        _p(I, "EUR", p), &y), (s32)PECUNIA_VITIUM_MONETAE_DIVERSAE);
    CREDO_VERUM(_intactum(y));

    /* superfluxus in omni margine */
    CREDO_AEQUALIS_S32((s32)pecunia_adde(_p(MAXIMA, "USD", p),
        _p(I, "USD", p), &y), (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_AEQUALIS_S32((s32)pecunia_adde(_p(MINIMA, "USD", p),
        _p(-I, "USD", p), &y), (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_AEQUALIS_S32((s32)pecunia_subtrahe(_p(MINIMA, "USD", p),
        _p(I, "USD", p), &y), (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_AEQUALIS_S32((s32)pecunia_subtrahe(_p(ZEPHYRUM, "USD", p),
        _p(MINIMA, "USD", p), &y), (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_AEQUALIS_S32((s32)pecunia_nega(_p(MINIMA, "USD", p), &y),
        (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_VERUM(_intactum(y));
    /* margines ipsi licent */
    CREDO_AEQUALIS_S32((s32)pecunia_adde(_p(MAXIMA - I, "USD", p),
        _p(I, "USD", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, MAXIMA);
    CREDO_AEQUALIS_S32((s32)pecunia_subtrahe(_p(-I, "USD", p),
        _p(MAXIMA, "USD", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, MINIMA);

    /* comparatio et signum */
    CREDO_AEQUALIS_S32((s32)pecunia_compara(_p(I, "USD", p),
        _p(II, "USD", p), &r), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S32(r, -I);
    CREDO_AEQUALIS_S32((s32)pecunia_compara(_p(II, "USD", p),
        _p(II, "USD", p), &r), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S32(r, ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)pecunia_compara(_p(MAXIMA, "USD", p),
        _p(MINIMA, "USD", p), &r), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S32(r, I);
    r = XLII;
    CREDO_AEQUALIS_S32((s32)pecunia_compara(_p(I, "USD", p),
        _p(I, "EUR", p), &r), (s32)PECUNIA_VITIUM_MONETAE_DIVERSAE);
    CREDO_AEQUALIS_S32(r, XLII);
    CREDO_AEQUALIS_S32(pecunia_signum(_p(-V, "USD", p)), -I);
    CREDO_AEQUALIS_S32(pecunia_signum(_p(ZEPHYRUM, "USD", p)),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32(pecunia_signum(_p(V, "USD", p)), I);
    CREDO_VERUM(pecunia_aequalis(_p(V, "USD", p), _p(V, "USD", p)));
    CREDO_FALSUM(pecunia_aequalis(_p(V, "USD", p), _p(V, "EUR", p)));
    CREDO_FALSUM(pecunia_aequalis(_p(V, "USD", p), _p(VI, "USD", p)));
}

interior vacuum
_sectio_summa_mixta (CredoContextus* c)
{
    Piscina* p = c->piscina;
    Pecunia  a[III];
    Pecunia  x;
    Pecunia  y = _signum_intactum();

    a[ZEPHYRUM]  = _p(I * M, "USD", p);
    a[I]         = _p(II * M, "USD", p);
    a[II]        = _p(III * M, "USD", p);
    CREDO_AEQUALIS_S32((s32)pecunia_summa(a, III, &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(VI * M));
    CREDO_VERUM(memcmp(x.moneta.codex, "USD", IV) == ZEPHYRUM);

    /* moneta aliena in ultimo loco */
    a[II] = _p(III * M, "EUR", p);
    CREDO_AEQUALIS_S32((s32)pecunia_summa(a, III, &y),
        (s32)PECUNIA_VITIUM_MONETAE_DIVERSAE);
    CREDO_VERUM(_intactum(y));

    /* superfluxus in medio cursu, quamvis summa finalis capiat:
     * nulla circumvolutio, nullus ordo mutatus */
    a[ZEPHYRUM]  = _p(MAXIMA, "USD", p);
    a[I]         = _p(I, "USD", p);
    a[II]        = _p(-I, "USD", p);
    CREDO_AEQUALIS_S32((s32)pecunia_summa(a, III, &y),
        (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_VERUM(_intactum(y));

    CREDO_AEQUALIS_S32((s32)pecunia_summa(a, ZEPHYRUM, &y),
        (s32)PECUNIA_VITIUM_FORMA);
    CREDO_VERUM(_intactum(y));
}

/* quantitas fortuita in [-10^15, 10^15] */
interior s64
_fortuita (
    Sors* s)
{
    i64 decies = (i64)M * M * M * M * M;   /* 10^15 */

    redde (s64)(sors_proximum(s) % (II * decies + I)) - (s64)decies;
}

interior vacuum
_sectio_proprietates (CredoContextus* c)
{
    Piscina* p = c->piscina;
       Sors  s;
        s32  i;

    sors_seminare(&s, (i64)0x9EC, ZEPHYRUM);
    per (i = ZEPHYRUM; i < V * M; i++)
    {
        Pecunia a = _p(_fortuita(&s), "USD", p);
        Pecunia b = _p(_fortuita(&s), "USD", p);
        Pecunia ab;
        Pecunia ba;
        Pecunia r;
        Pecunia n;

        CREDO_NECESSE(pecunia_adde(a, b, &ab) == PECUNIA_BENE);
        CREDO_NECESSE(pecunia_adde(b, a, &ba) == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(ab, ba));
        CREDO_NECESSE(pecunia_subtrahe(ab, b, &r) == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(r, a));
        CREDO_NECESSE(pecunia_nega(a, &n) == PECUNIA_BENE);
        CREDO_NECESSE(pecunia_nega(n, &r) == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(r, a));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tabula", _sectio_tabula, NIHIL, NIHIL, NIHIL, NIHIL },
    { "codices", _sectio_codices, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unitates", _sectio_unitates, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arithmetica", _sectio_arithmetica, NIHIL, NIHIL, NIHIL, NIHIL },
    { "summa_mixta", _sectio_summa_mixta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "proprietates", _sectio_proprietates, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pecunia", SECTIONES);
}
