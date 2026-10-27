/* probatio_pecunia.c - pecunia (pecunia-plan): tabula monetarum
 * GENERATA ex indicibus ISO 4217 fixis (probationes/fixa/iso4217/)
 * + supplementum, quaestio codicum (sine casu, nihil aliud
 * condonatur), vitia nominata; arithmetica quae recusat (T2:
 * superfluxus, monetae diversae, exitus intactus, proprietates);
 * rotundatio (T3) contra oraculum decimal Pythonis FIXUM
 * (probationes/fixa/pecunia/rotundatio.txt, sha256 infra; scriptum a
 * tools/pecunia_rotundatio_oraculum.py), solvibilis, cambium;
 * partitio (T4): residuum maximum, paria ad indicem minorem, summa
 * EXACTA, granum scalae aut unitatis; forma canonica stricta et
 * numerus nudus (T5), margines s64, circuitus; forma ostensionis et
 * ingressus humanus per regulas (T6). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "pecunia.h"
#include "pecunia_monetae.h"
#include "sors.h"
#include "fractio.h"
#include "filum.h"
#include "sigillum.h"
#include <stdio.h>
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

#define VIA_ROTUNDATIO "probationes/fixa/pecunia/rotundatio.txt"
#define SIGILLUM_ROTUNDATIONIS \
    "e6982f3be39d530ddf75aa789b8ddf2f152fae176b41baafd5e744183cc722ba"

/* verbum proximum lineae (spatiis separatum) */
interior chorda
_verbum (
    chorda  linea,
       s32* positio)
{
    s32 a;

    dum (   *positio < (s32)linea.mensura
         && linea.datum[*positio] == ' ')
    {
        (*positio)++;
    }
    a = *positio;
    dum (   *positio < (s32)linea.mensura
         && linea.datum[*positio] != ' ')
    {
        (*positio)++;
    }
    redde chorda_sectio(linea, (i32)a, (i32)*positio);
}

interior b32
_modus (
          chorda  t,
    PecuniaModus* m)
{
    si (chorda_aequalis_literis(t, "PAR"))
    {
        *m = PECUNIA_PAR;
    }
    alioquin si (chorda_aequalis_literis(t, "DIMIDIUM_SURSUM"))
    {
        *m = PECUNIA_DIMIDIUM_SURSUM;
    }
    alioquin si (chorda_aequalis_literis(t, "VERSUS_NIHIL"))
    {
        *m = PECUNIA_VERSUS_NIHIL;
    }
    alioquin si (chorda_aequalis_literis(t, "SOLUM"))
    {
        *m = PECUNIA_SOLUM;
    }
    alioquin si (chorda_aequalis_literis(t, "TECTUM"))
    {
        *m = PECUNIA_TECTUM;
    }
    alioquin
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* exspectatum: integer aut SUPERFLUXUS; effectus cum eo congruit? */
interior b32
_congruit (
                chorda  exspectatum,
         PecuniaVitium  v,
               Pecunia  x,
    constans character* codex)
{
    s64 e;

    si (chorda_aequalis_literis(exspectatum, "SUPERFLUXUS"))
    {
        redde v == PECUNIA_VITIUM_SUPERFLUXUS && _intactum(x);
    }
    si (!chorda_ut_s64(exspectatum, &e))
    {
        redde FALSUM;
    }
    redde v == PECUNIA_BENE && x.quantitas == e
        && memcmp(x.moneta.codex, codex, IV) == ZEPHYRUM;
}

interior constans character*
_moneta_exponentis (
    s64 e)
{
    redde e == ZEPHYRUM ? "JPY" : e == II ? "USD" : e == III ? "KWD"
        : e == IV ? "CLF" : "ZZZ";
}

interior vacuum
_sectio_oraculum (CredoContextus* c)
{
      Piscina* p = c->piscina;
       chorda  t = filum_legere_totum(VIA_ROTUNDATIO, p);
     Sigillum  s;
    character  hex[SIGILLUM_HEX_MENSURA];
          s32  initium = ZEPHYRUM;
          s32  i;
          s32  multiplicationes  = ZEPHYRUM;
          s32  unitates          = ZEPHYRUM;
          s32  errores           = ZEPHYRUM;

    CREDO_NECESSE(t.mensura > ZEPHYRUM);
    s = sigillum_computare(t.datum, (memoriae_index)t.mensura);
    sigillum_hex(&s, hex);
    CREDO_VERUM(strcmp(hex, SIGILLUM_ROTUNDATIONIS) == ZEPHYRUM);

    per (i = ZEPHYRUM; i < (s32)t.mensura; i++)
    {
              chorda linea;
                 s32 pos = ZEPHYRUM;
              chorda genus;
        PecuniaModus modus = PECUNIA_PAR;   /* gcc: _modus ponit */
                 b32 bene;

        si (t.datum[i] != '\n')
        {
            continue;
        }
        linea    = chorda_sectio(t, (i32)initium, (i32)i);
        initium  = i + I;
        si (linea.mensura == ZEPHYRUM || linea.datum[ZEPHYRUM] == '#')
        {
            continue;
        }
        genus = _verbum(linea, &pos);
        CREDO_NECESSE(_modus(_verbum(linea, &pos), &modus));
        si (chorda_aequalis_literis(genus, "M"))
        {
                s64 q;
            Fractio f;
            Pecunia x = _signum_intactum();

            CREDO_NECESSE(chorda_ut_s64(_verbum(linea, &pos), &q));
            CREDO_NECESSE(fractio_ex_chorda(_verbum(linea, &pos), p,
                &f));
            bene = _congruit(_verbum(linea, &pos), pecunia_multiplica(
                _p(q, "USD", p), f, modus, p, &x), x, "USD");
            multiplicationes++;
        }
        alioquin
        {
                           s64  e;
                           s64  q;
                       Pecunia  x = _signum_intactum();
            constans character* codex;

            CREDO_NECESSE(chorda_aequalis_literis(genus, "U"));
            CREDO_NECESSE(chorda_ut_s64(_verbum(linea, &pos), &e));
            CREDO_NECESSE(chorda_ut_s64(_verbum(linea, &pos), &q));
            codex = _moneta_exponentis(e);
            bene = _congruit(_verbum(linea, &pos), pecunia_ad_unitatem(
                _p(q, codex, p), modus, &x), x, codex);
            unitates++;
        }
        si (!bene)
        {
            imprimere("  oraculum discrepat: %.*s\n",
                (integer)linea.mensura,
                linea.datum);
            errores++;
        }
    }
    CREDO_AEQUALIS_S32(errores, ZEPHYRUM);
    /* numeri ex fixura: nulla linea tacite praetermissa */
    CREDO_AEQUALIS_S32(multiplicationes, MMMCCCXX);
    CREDO_AEQUALIS_S32(unitates, MDLX);
}

interior vacuum
_sectio_solvibilis (CredoContextus* c)
{
    Piscina* p = c->piscina;

    CREDO_VERUM(pecunia_solvibilis(_p(XXXIV * M + DC, "USD", p)));
    CREDO_VERUM(pecunia_solvibilis(_p(-(XXXIV * M + DC), "USD", p)));
    CREDO_FALSUM(pecunia_solvibilis(_p(XXXIV * M + DXC, "USD", p)));
    CREDO_VERUM(pecunia_solvibilis(_p(CXX * M, "JPY", p)));
    CREDO_FALSUM(pecunia_solvibilis(_p(CXX * M + I, "JPY", p)));
    CREDO_VERUM(pecunia_solvibilis(_p(XII * M + CCCXL, "KWD", p)));
    CREDO_FALSUM(pecunia_solvibilis(_p(XII * M + CCCXLV, "KWD", p)));
    CREDO_VERUM(pecunia_solvibilis(_p(XII * M + CCCXLV, "CLF", p)));
    CREDO_VERUM(pecunia_solvibilis(_p(ZEPHYRUM, "USD", p)));
}

interior vacuum
_sectio_cambium (CredoContextus* c)
{
         Piscina* p = c->piscina;
         Fractio  f;
         Pecunia  x;
         Pecunia  y = _signum_intactum();
    PecuniaModus  modi[V];
             s64  exspectata[V];
             s32  i;

    CREDO_NECESSE(fractio_ex_chorda(_c("9/10", p), p, &f));
    CREDO_AEQUALIS_S32((s32)pecunia_convertere(_p(M * M, "USD", p), f,
        _moneta("EUR", p), PECUNIA_PAR, p, &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(CM * M));
    CREDO_VERUM(memcmp(x.moneta.codex, "EUR", IV) == ZEPHYRUM);

    /* USD 1.0000 ad 1/3: 0.3333... per modos */
    modi[ZEPHYRUM]  = PECUNIA_PAR;
    modi[I]         = PECUNIA_DIMIDIUM_SURSUM;
    modi[II]        = PECUNIA_VERSUS_NIHIL;
    modi[III]       = PECUNIA_SOLUM;
    modi[IV]        = PECUNIA_TECTUM;
    per (i = ZEPHYRUM; i < IV; i++)
    {
        exspectata[i] = (s64)MMMCCCXXXIII;
    }
    exspectata[IV] = (s64)MMMCCCXXXIV;
    CREDO_NECESSE(fractio_ex_chorda(_c("1/3", p), p, &f));
    per (i = ZEPHYRUM; i < V; i++)
    {
        CREDO_AEQUALIS_S32((s32)pecunia_convertere(_p(X * M, "USD", p),
            f,
            _moneta("EUR", p), modi[i], p, &x), (s32)PECUNIA_BENE);
        CREDO_AEQUALIS_S64(x.quantitas, exspectata[i]);
    }

    /* cambium nullum aut negativum: FORMA, exitus intactus */
    CREDO_NECESSE(fractio_ex_chorda(_c("0", p), p, &f));
    CREDO_AEQUALIS_S32((s32)pecunia_convertere(_p(X * M, "USD", p), f,
        _moneta("EUR", p), PECUNIA_PAR, p, &y),
        (s32)PECUNIA_VITIUM_FORMA);
    CREDO_NECESSE(fractio_ex_chorda(_c("-1", p), p, &f));
    CREDO_AEQUALIS_S32((s32)pecunia_convertere(_p(X * M, "USD", p), f,
        _moneta("EUR", p), PECUNIA_PAR, p, &y),
        (s32)PECUNIA_VITIUM_FORMA);
    CREDO_VERUM(_intactum(y));
    /* moneta destinata ignota */
    CREDO_NECESSE(fractio_ex_chorda(_c("2", p), p, &f));
    CREDO_AEQUALIS_S32((s32)pecunia_convertere(_p(X * M, "USD", p), f,
        _moneta("ZZZ", p), PECUNIA_PAR, p, &y),
        (s32)PECUNIA_VITIUM_MONETA_IGNOTA);
    /* superfluxus */
    CREDO_AEQUALIS_S32((s32)pecunia_convertere(_p(MAXIMA, "USD", p), f,
        _moneta("EUR", p), PECUNIA_PAR, p, &y),
        (s32)PECUNIA_VITIUM_SUPERFLUXUS);
    CREDO_VERUM(_intactum(y));
    /* multiplica monetam servat */
    CREDO_AEQUALIS_S32((s32)pecunia_multiplica(_p(X * M, "KWD", p), f,
        PECUNIA_PAR, p, &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(XX * M));
    CREDO_VERUM(memcmp(x.moneta.codex, "KWD", IV) == ZEPHYRUM);
}

/* partes exspectatae (quantitates) cum moneta totius */
interior b32
_partes_sunt (
      constans Pecunia* partes,
          constans s64* exspectatae,
                   s32  n,
    constans character* codex)
{
    s32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        si (   partes[i].quantitas != exspectatae[i]
            || memcmp(partes[i].moneta.codex, codex, IV) != ZEPHYRUM)
        {
            imprimere("  pars %d: %ld pro %ld\n", (integer)i,
                (longus)partes[i].quantitas, (longus)exspectatae[i]);
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_exempla (CredoContextus* c)
{
    Piscina* p = c->piscina;
    Pecunia  x[III];
        s64  e[III];
        s64  r[II];

    /* USD 10.00 / 3 ad centesimos */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(C * M, "USD", p), III,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = (s64)(XXXIII * M + CD);
    e[I]         = (s64)(XXXIII * M + CCC);
    e[II]        = (s64)(XXXIII * M + CCC);
    CREDO_VERUM(_partes_sunt(x, e, III, "USD"));
    /* negativum: magnitudo partitur, signum servatur */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(-(s64)(C * M), "USD",
        p),
        III, PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = -e[ZEPHYRUM];
    e[I]         = -e[I];
    e[II]        = -e[II];
    CREDO_VERUM(_partes_sunt(x, e, III, "USD"));
    /* USD 0.05 rationibus {1, 2}: residua 2/3 contra 1/3 */
    r[ZEPHYRUM]  = I;
    r[I]         = II;
    CREDO_AEQUALIS_S32((s32)pecunia_partiri(_p(D, "USD", p), r, II,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = (s64)CC;
    e[I]         = (s64)CCC;
    CREDO_VERUM(_partes_sunt(x, e, II, "USD"));
    /* USD 1.0000 / 3 in decimillesimis */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(X * M, "USD", p), III,
        PECUNIA_GRANUM_SCALA, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = (s64)MMMCCCXXXIV;
    e[I]         = (s64)MMMCCCXXXIII;
    e[II]        = (s64)MMMCCCXXXIII;
    CREDO_VERUM(_partes_sunt(x, e, III, "USD"));
    /* JPY 100 / 3 ad yen integros */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(M * M, "JPY", p), III,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = (s64)(CCCXL * M);
    e[I]         = (s64)(CCCXXX * M);
    e[II]        = (s64)(CCCXXX * M);
    CREDO_VERUM(_partes_sunt(x, e, III, "JPY"));
    /* rationes ingentes: summa extra s64 in Magno computatur */
    r[ZEPHYRUM]  = MAXIMA;
    r[I]         = MAXIMA;
    CREDO_AEQUALIS_S32((s32)pecunia_partiri(_p(MCCC, "USD", p), r, II,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = (s64)DCC;
    e[I]         = (s64)DC;
    CREDO_VERUM(_partes_sunt(x, e, II, "USD"));
}

interior vacuum
_sectio_degeneres (CredoContextus* c)
{
    Piscina* p = c->piscina;
    Pecunia  x[V];
    Pecunia  y[II];
        s64  e[V];
        s64  r[III];
        s32  i;

    /* n == 0 */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(C, "USD", p), ZEPHYRUM,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_VITIUM_RATIONES);
    /* n == 1: totum */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(MINIMA, "CLF", p), I,
        PECUNIA_GRANUM_SCALA, p, x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x[ZEPHYRUM].quantitas, MINIMA);
    /* totum nullum */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(ZEPHYRUM, "USD", p),
        III,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM] = e[I] = e[II] = ZEPHYRUM;
    CREDO_VERUM(_partes_sunt(x, e, III, "USD"));
    /* plures partes quam grana: USD 0.02 / 5 - paria ad minorem */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(CC, "USD", p), V,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = e[I] = (s64)C;
    e[II]        = e[III] = e[IV] = ZEPHYRUM;
    CREDO_VERUM(_partes_sunt(x, e, V, "USD"));
    /* ratio nulla numquam granum reliquum accipit */
    r[ZEPHYRUM]  = ZEPHYRUM;
    r[I]         = I;
    r[II]        = I;
    CREDO_AEQUALIS_S32((s32)pecunia_partiri(_p(CCC, "USD", p), r, III,
        PECUNIA_GRANUM_UNITAS, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = ZEPHYRUM;
    e[I]         = (s64)CC;
    e[II]        = (s64)C;
    CREDO_VERUM(_partes_sunt(x, e, III, "USD"));

    /* recusationes: exitus intacti */
    per (i = ZEPHYRUM; i < II; i++)
    {
        y[i] = _signum_intactum();
    }
    r[ZEPHYRUM]  = I;
    r[I]         = -I;
    CREDO_AEQUALIS_S32((s32)pecunia_partiri(_p(C, "USD", p), r, II,
        PECUNIA_GRANUM_UNITAS, p, y), (s32)PECUNIA_VITIUM_RATIONES);
    r[ZEPHYRUM]  = ZEPHYRUM;
    r[I]         = ZEPHYRUM;
    CREDO_AEQUALIS_S32((s32)pecunia_partiri(_p(C, "USD", p), r, II,
        PECUNIA_GRANUM_UNITAS, p, y), (s32)PECUNIA_VITIUM_RATIONES);
    CREDO_AEQUALIS_S32((s32)pecunia_partiri(_p(C, "USD", p), NIHIL, II,
        PECUNIA_GRANUM_UNITAS, p, y), (s32)PECUNIA_VITIUM_RATIONES);
    /* USD 10.0050 ad centesimos: summa partium totum esse non potest */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(C * M + L, "USD", p),
        II,
        PECUNIA_GRANUM_UNITAS, p, y),
        (s32)PECUNIA_VITIUM_NON_SOLVIBILIS);
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(C, "ZZZ", p), II,
        PECUNIA_GRANUM_UNITAS, p, y),
        (s32)PECUNIA_VITIUM_MONETA_IGNOTA);
    CREDO_VERUM(_intactum(y[ZEPHYRUM]) && _intactum(y[I]));
    /* in scala idem totum licet */
    CREDO_AEQUALIS_S32((s32)pecunia_dividere(_p(C * M + L, "USD", p),
        II,
        PECUNIA_GRANUM_SCALA, p, x), (s32)PECUNIA_BENE);
    e[ZEPHYRUM]  = (s64)(L * M + XXV);
    e[I]         = (s64)(L * M + XXV);
    CREDO_VERUM(_partes_sunt(x, e, II, "USD"));
}

interior vacuum
_sectio_partitio_proprietates (CredoContextus* c)
{
               Piscina* p = c->piscina;
                  Sors  s;
                   s32  k;
               Pecunia  x[XII];
                   s64  r[XII];
    constans character* codices[IV];

    codices[ZEPHYRUM]  = "USD";
    codices[I]         = "JPY";
    codices[II]        = "KWD";
    codices[III]       = "CLF";
    sors_seminare(&s, (i64)0x5A17, ZEPHYRUM);
    per (k = ZEPHYRUM; k < MMM; k++)
    {
        constans character* codex  = codices[sors_intra(&s, IV)];
                       s32  n      = (s32)sors_intra(&s, XII) + I;
             PecuniaGranum  g     = sors_intra(&s, II) == ZEPHYRUM
                                    ? PECUNIA_GRANUM_SCALA
                                    : PECUNIA_GRANUM_UNITAS;
                   Pecunia totum;
                   Pecunia summa;
                       s64 quantitas;
                       s32 i;
                       s64 maxima = MINIMA;
                       s64 minima = MAXIMA;

        /* totum in +-10^12 granis, solvibile si granum unitatis */
        quantitas = (s64)(sors_proximum(&s) % (II * (i64)M * M * M * M
            + I)) - (s64)M * M * M * M;
        totum = _p(quantitas, codex, p);
        si (g == PECUNIA_GRANUM_UNITAS)
        {
            CREDO_NECESSE(pecunia_ad_unitatem(totum, PECUNIA_SOLUM,
                &totum)
                == PECUNIA_BENE);
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            r[i] = (s64)sors_intra(&s, MI);
        }
        r[ZEPHYRUM] += I;    /* summa positiva */

        CREDO_NECESSE(pecunia_partiri(totum, r, (memoriae_index)n, g, p,
            x)
            == PECUNIA_BENE);
        CREDO_NECESSE(pecunia_summa(x, (memoriae_index)n, &summa)
            == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(summa, totum));
        per (i = ZEPHYRUM; i < n; i++)
        {
            CREDO_VERUM(x[i].quantitas == ZEPHYRUM
                || pecunia_signum(x[i]) == pecunia_signum(totum));
            si (g == PECUNIA_GRANUM_UNITAS)
            {
                CREDO_VERUM(pecunia_solvibilis(x[i]));
            }
        }

        /* aequaliter: partes differunt granum unum ad summum */
        CREDO_NECESSE(pecunia_dividere(totum, (memoriae_index)n, g, p,
            x)
            == PECUNIA_BENE);
        CREDO_NECESSE(pecunia_summa(x, (memoriae_index)n, &summa)
            == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(summa, totum));
        per (i = ZEPHYRUM; i < n; i++)
        {
            maxima = x[i].quantitas > maxima ? x[i].quantitas : maxima;
            minima = x[i].quantitas < minima ? x[i].quantitas : minima;
        }
        CREDO_VERUM(maxima - minima <= (g
            == PECUNIA_GRANUM_SCALA ? (s64)I
            : (s64)(X * M) / (codex[ZEPHYRUM] == 'J' ? I
            : codex[ZEPHYRUM] == 'U' ? C : codex[ZEPHYRUM] == 'K' ? M
            : X * M)));
    }
}

interior vacuum
_textus_est (
             chorda  t,
    constans character* exspectatum)
{
    si (!chorda_aequalis_literis(t, exspectatum))
    {
        imprimere("  textus: '%.*s' pro '%s'\n", (integer)t.mensura,
            t.datum, exspectatum);
    }
    CREDO_VERUM(chorda_aequalis_literis(t, exspectatum));
}

/* lectio canonica recusata cum vitio nominato, exitus intactus */
interior vacuum
_canonica_mala (
    constans character* textus,
         PecuniaVitium  exspectatum,
              Piscina* p)
{
    Pecunia y = _signum_intactum();

    CREDO_AEQUALIS_S32((s32)pecunia_ex_textu(_c(textus, p), &y),
        (s32)exspectatum);
    CREDO_VERUM(_intactum(y));
}

interior vacuum
_numerus_malus (
    constans character* textus,
         PecuniaVitium  exspectatum,
              Piscina* p)
{
    Pecunia y = _signum_intactum();

    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c(textus, p),
        _moneta("USD", p), &y), (s32)exspectatum);
    CREDO_VERUM(_intactum(y));
}

interior vacuum
_sectio_canonica (CredoContextus* c)
{
    Piscina* p = c->piscina;
    Pecunia  x;

    _textus_est(pecunia_ad_textum(_p(CXXIII * M + CD, "USD", p), p),
        "USD 12.3400");
    _textus_est(pecunia_ad_textum(_p(-(s64)(V * M), "USD", p), p),
        "USD -0.5000");
    _textus_est(pecunia_ad_textum(_p(ZEPHYRUM, "USD", p), p),
        "USD 0.0000");
    _textus_est(pecunia_ad_textum(_p(L * M, "JPY", p), p),
        "JPY 5.0000");
    _textus_est(pecunia_ad_textum(_p(I, "CLF", p), p), "CLF 0.0001");

    CREDO_AEQUALIS_S32((s32)pecunia_ex_textu(_c("USD 12.3400", p), &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(CXXIII * M + CD));
    CREDO_VERUM(memcmp(x.moneta.codex, "USD", IV) == ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_textu(_c("USD -0.5000", p), &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, -(s64)(V * M));
    CREDO_AEQUALIS_S32((s32)pecunia_ex_textu(_c("USD 0.0000", p), &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)ZEPHYRUM);

    _canonica_mala("usd 12.3400", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD 12.34", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD 12.34000", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD  12.3400", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD +12.3400", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD 012.3400", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD -0.0000", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD 12.3400 ", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala(" USD 12.3400", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD 1e5", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD 12", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD -", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("USD", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("US 12.3400", PECUNIA_VITIUM_FORMA, p);
    _canonica_mala("XAU 1.0000", PECUNIA_VITIUM_MONETA_SINE_UNITATE, p);
    _canonica_mala("ABC 1.0000", PECUNIA_VITIUM_MONETA_IGNOTA, p);
}

interior vacuum
_sectio_extrema (CredoContextus* c)
{
    Piscina* p = c->piscina;
    Pecunia  x;

    _textus_est(pecunia_ad_textum(_p(MAXIMA, "USD", p), p),
        "USD 922337203685477.5807");
    _textus_est(pecunia_ad_textum(_p(MINIMA, "USD", p), p),
        "USD -922337203685477.5808");
    _textus_est(pecunia_numerus(_p(MAXIMA, "USD", p), p),
        "922337203685477.5807");
    _textus_est(pecunia_numerus(_p(MINIMA, "USD", p), p),
        "-922337203685477.5808");

    CREDO_AEQUALIS_S32((s32)pecunia_ex_textu(_c(
        "USD 922337203685477.5807", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, MAXIMA);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_textu(_c(
        "USD -922337203685477.5808", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, MINIMA);
    _canonica_mala("USD 922337203685477.5808",
        PECUNIA_VITIUM_SUPERFLUXUS, p);
    _canonica_mala("USD -922337203685477.5809",
        PECUNIA_VITIUM_SUPERFLUXUS, p);
    _canonica_mala("USD 99999999999999999999.0000",
        PECUNIA_VITIUM_SUPERFLUXUS, p);

    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("922337203685477.5807",
        p), _moneta("USD", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, MAXIMA);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c(
        "-922337203685477.5808", p), _moneta("USD", p), &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, MINIMA);
    _numerus_malus("922337203685477.5808", PECUNIA_VITIUM_SUPERFLUXUS,
        p);
    _numerus_malus("922337203685478", PECUNIA_VITIUM_SUPERFLUXUS, p);
    /* zephyra ducentia (forma Lunch Money licet) sine pondere, etiam
     * longissima: superfluxum non faciunt */
    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c(
        "0000000000000000000000000000000000000000000001", p),
        _moneta("USD", p), &x), (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(X * M));
}

interior vacuum
_sectio_numeri (CredoContextus* c)
{
          Piscina* p = c->piscina;
          Pecunia  x;
    PecuniaMoneta  nummus = _moneta("USD", p);

    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("12", p), nummus, &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(CXX * M));
    CREDO_VERUM(memcmp(x.moneta.codex, "USD", IV) == ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("12.3", p), nummus,
        &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(CXXIII * M));
    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("-12.3456", p), nummus,
        &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, -(s64)(CXXIII * M + CDLVI));
    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("-0", p), nummus, &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("0.0001", p), nummus,
        &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)I);
    CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("007", p), nummus, &x),
        (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, (s64)(LXX * M));

    _numerus_malus("12.34567", PECUNIA_VITIUM_DECIMALES, p);
    _numerus_malus("+12", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus(".5", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("12.", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("1,234.00", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus(" 12", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("12 ", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("-", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("--1", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("1e5", PECUNIA_VITIUM_FORMA, p);
    _numerus_malus("12.3.4", PECUNIA_VITIUM_FORMA, p);
    /* moneta non ex pecunia_moneta_ex_codice */
    {
        Pecunia y = _signum_intactum();

        CREDO_AEQUALIS_S32((s32)pecunia_ex_numero(_c("1", p),
            _moneta("ZZZ", p), &y), (s32)PECUNIA_VITIUM_MONETA_IGNOTA);
        CREDO_VERUM(_intactum(y));
    }

    _textus_est(pecunia_numerus(_p(-(s64)(V * M), "USD", p), p),
        "-0.5000");
    _textus_est(pecunia_numerus(_p(ZEPHYRUM, "USD", p), p), "0.0000");
}

interior vacuum
_sectio_circuitus (CredoContextus* c)
{
    Piscina* p = c->piscina;
       Sors  s;
        s32  i;

    sors_seminare(&s, (i64)0xC1C, ZEPHYRUM);
    per (i = ZEPHYRUM; i < V * M; i++)
    {
        Pecunia a = _p((s64)sors_proximum(&s), i % II ? "USD" : "JPY",
            p);
        Pecunia b;

        CREDO_NECESSE(pecunia_ex_textu(pecunia_ad_textum(a, p), &b)
            == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(a, b));
        CREDO_NECESSE(pecunia_ex_numero(pecunia_numerus(a, p), a.moneta,
            &b) == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(a, b));
    }
}

/* forma ordinaria cum signo et rationaria optionalibus */
interior PecuniaForma
_forma (
    constans character* signum,
                   b32  rationaria,
               Piscina* p)
{
    PecuniaForma f;

    pecunia_forma_ordinaria(&f);
    f.signum      = _c(signum, p);
    f.rationaria  = rationaria;
    redde f;
}

interior vacuum
_sectio_formae (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
    PecuniaForma  f;
    PecuniaForma  d = _forma("", FALSUM, p);
    PecuniaForma  s = _forma("$", FALSUM, p);
         Pecunia  magnus = _p((s64)MCCXXXIV * M * M * X
                               + (s64)DLXVII * X * M + DCCCXC * X,
                               "USD", p);   /* 1234567.8900 */
         Pecunia minus = _p(-(s64)(CXXIII * M + CD), "USD", p);

    _textus_est(pecunia_formare(magnus, &d, p), "USD 1,234,567.89");
    _textus_est(pecunia_formare(_p((s64)MCCXXXIV * X * M, "JPY", p), &d,
        p), "JPY 1,234");
    _textus_est(pecunia_formare(_p(XII * M + CCCXLV, "KWD", p), &d, p),
        "KWD 1.234");
    _textus_est(pecunia_formare(minus, &d, p), "USD -12.34");
    f = _forma("", VERUM, p);
    _textus_est(pecunia_formare(minus, &f, p), "USD (12.34)");
    _textus_est(pecunia_formare(magnus, &s, p), "$1,234,567.89");
    _textus_est(pecunia_formare(minus, &s, p), "-$12.34");
    f = _forma("$", VERUM, p);
    _textus_est(pecunia_formare(minus, &f, p), "($12.34)");
    f            = _forma("", FALSUM, p);
    f.nota_post  = VERUM;
    _textus_est(pecunia_formare(_p(CXXIII * M + CD, "USD", p), &f, p),
        "12.34 USD");
    _textus_est(pecunia_formare(minus, &f, p), "-12.34 USD");
    f            = _forma("$", FALSUM, p);
    f.nota_post  = VERUM;
    _textus_est(pecunia_formare(_p(CXXIII * M + CD, "USD", p), &f, p),
        "12.34$");
    f        = _forma("", FALSUM, p);
    f.plena  = VERUM;
    _textus_est(pecunia_formare(_p(CXXIII * M + CD, "USD", p), &f, p),
        "USD 12.3400");
    f         = _forma("", FALSUM, p);
    f.coetus  = _c("", p);
    _textus_est(pecunia_formare(magnus, &f, p), "USD 1234567.89");
    f            = _forma("", FALSUM, p);
    f.coetus     = _c(".", p);
    f.decimalis  = _c(",", p);
    _textus_est(pecunia_formare(magnus, &f, p), "USD 1.234.567,89");
    /* separator multi-octetus: U+202F (spatium angustum) */
    f         = _forma("", FALSUM, p);
    f.coetus  = _c("\342\200\257", p);
    _textus_est(pecunia_formare(magnus, &f, p),
        "USD 1\342\200\257" "234\342\200\257" "567.89");
    /* nihil ostensum numquam signum fert (rotundatio ex negativo) */
    _textus_est(pecunia_formare(_p(-(s64)XL, "USD", p), &d, p),
        "USD 0.00");
    _textus_est(pecunia_formare(_p(-(s64)XL, "USD", p), &f, p),
        "USD 0.00");
}

interior vacuum
_sectio_extrema_formae (CredoContextus* c)
{
         Piscina* p = c->piscina;
    PecuniaForma  d = _forma("", FALSUM, p);
    PecuniaForma  r = _forma("$", VERUM, p);
    PecuniaForma  f;

    _textus_est(pecunia_formare(_p(MAXIMA, "USD", p), &d, p),
        "USD 922,337,203,685,477.58");
    _textus_est(pecunia_formare(_p(MINIMA, "USD", p), &d, p),
        "USD -922,337,203,685,477.58");
    _textus_est(pecunia_formare(_p(MINIMA, "USD", p), &r, p),
        "($922,337,203,685,477.58)");
    /* TECTUM super MAXIMA: ostensio non deficit (ad_unitatem
     * superflueret - unitates rotundatae ipsae capiunt) */
    f        = _forma("", FALSUM, p);
    f.modus  = PECUNIA_TECTUM;
    _textus_est(pecunia_formare(_p(MAXIMA, "USD", p), &f, p),
        "USD 922,337,203,685,477.59");
    f        = _forma("", FALSUM, p);
    f.plena  = VERUM;
    _textus_est(pecunia_formare(_p(MINIMA, "USD", p), &f, p),
        "USD -922,337,203,685,477.5808");
    _textus_est(pecunia_formare(_p(MINIMA, "CLF", p), &d, p),
        "CLF -922,337,203,685,477.5808");
}

interior vacuum
_ingressus_est (
    constans character* textus,
    constans PecuniaForma* f,
                   s64  exspectatum,
              Piscina* p)
{
          Pecunia x = _signum_intactum();
    PecuniaVitium v = pecunia_ex_ingressu(_c(textus, p),
        _moneta("USD", p), f, &x);

    si (v != PECUNIA_BENE || x.quantitas != exspectatum)
    {
        imprimere("  ingressus '%s': vitium %d, %ld\n", textus,
            (integer)v,
            (longus)x.quantitas);
    }
    CREDO_AEQUALIS_S32((s32)v, (s32)PECUNIA_BENE);
    CREDO_AEQUALIS_S64(x.quantitas, exspectatum);
    CREDO_VERUM(memcmp(x.moneta.codex, "USD", IV) == ZEPHYRUM);
}

interior vacuum
_ingressus_malus (
    constans character* textus,
    constans PecuniaForma* f,
         PecuniaVitium  exspectatum,
              Piscina* p)
{
          Pecunia x = _signum_intactum();
    PecuniaVitium v = pecunia_ex_ingressu(_c(textus, p),
        _moneta("USD", p), f, &x);

    si (v != exspectatum)
    {
        imprimere("  ingressus malus '%s': vitium %d pro %d\n", textus,
            (integer)v, (integer)exspectatum);
    }
    CREDO_AEQUALIS_S32((s32)v, (s32)exspectatum);
    CREDO_VERUM(_intactum(x));
}

interior vacuum
_sectio_ingressus (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
    PecuniaForma  s = _forma("$", FALSUM, p);

    _ingressus_est("12", &s, (s64)(CXX * M), p);
    _ingressus_est("12.3", &s, (s64)(CXXIII * M), p);
    _ingressus_est("1,234.56", &s, (s64)MCCXXXIV * X * M + LVI * C, p);
    _ingressus_est("$1,234.56", &s, (s64)MCCXXXIV * X * M + LVI * C, p);
    _ingressus_est("  12.34  ", &s, (s64)(CXXIII * M + CD), p);
    _ingressus_est("USD 12.34", &s, (s64)(CXXIII * M + CD), p);
    _ingressus_est("12.34 USD", &s, (s64)(CXXIII * M + CD), p);
    _ingressus_est("usd 12.34", &s, (s64)(CXXIII * M + CD), p);
    _ingressus_est("$ 12.34", &s, (s64)(CXXIII * M + CD), p);
    _ingressus_est("12.34$", &s, (s64)(CXXIII * M + CD), p);
    _ingressus_est("1234567", &s, (s64)MCCXXXIV * M * M * X
        + DLXVII * M * X, p);

    _ingressus_malus("12,34", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("1,23,456", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("1234,567", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("1,2345", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("12.34567", &s, PECUNIA_VITIUM_DECIMALES, p);
    _ingressus_malus("1e5", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("12.3.4", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("   ", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("EUR 12", &s, PECUNIA_VITIUM_MONETAE_DIVERSAE, p);
    _ingressus_malus("USD12", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("12 34", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("922337203685478", &s, PECUNIA_VITIUM_SUPERFLUXUS,
        p);
}

interior vacuum
_sectio_ingressus_signa (
    CredoContextus* c)
{
         Piscina* p      = c->piscina;
    PecuniaForma  s      = _forma("$", FALSUM, p);
    PecuniaForma  n      = _forma("", FALSUM, p);
             s64  minus  = -(s64)(CXXIII * M + CD);

    _ingressus_est("-$12.34", &s, minus, p);
    _ingressus_est("$-12.34", &s, minus, p);
    _ingressus_est("($12.34)", &s, minus, p);
    _ingressus_est("-12.34", &s, minus, p);
    _ingressus_est("(12.34)", &s, minus, p);
    _ingressus_est("USD -12.34", &s, minus, p);
    _ingressus_est("-12.34 USD", &s, minus, p);
    _ingressus_malus("--12", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("-(12)", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("(-12)", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("-$-12", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("$", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("(12.34", &s, PECUNIA_VITIUM_FORMA, p);
    _ingressus_malus("12.34-", &s, PECUNIA_VITIUM_FORMA, p);
    /* signum ignotum formae non licet */
    _ingressus_malus("$12.34", &n, PECUNIA_VITIUM_FORMA, p);
    /* separator U+202F si forma eum habet */
    n.coetus = _c("\342\200\257", p);
    _ingressus_est("1\342\200\257" "234.56", &n,
        (s64)MCCXXXIV * X * M + LVI * C, p);
    _ingressus_malus("1,234.56", &n, PECUNIA_VITIUM_FORMA, p);
}

interior vacuum
_sectio_circuitus_formae (CredoContextus* c)
{
         Piscina* p = c->piscina;
    PecuniaForma  formae[II];
            Sors  s;
             s32  i;

    formae[ZEPHYRUM]  = _forma("", FALSUM, p);
    formae[I]         = _forma("$", VERUM, p);
    sors_seminare(&s, (i64)0xF0A, ZEPHYRUM);
    per (i = ZEPHYRUM; i < MM; i++)
    {
        constans PecuniaForma* f = &formae[i % II];
                      Pecunia  a = _p(_fortuita(&s), "USD", p);
                      Pecunia  rotundatum;
                      Pecunia  lectum;

        CREDO_NECESSE(pecunia_ad_unitatem(a, PECUNIA_PAR, &rotundatum)
            == PECUNIA_BENE);
        CREDO_NECESSE(pecunia_ex_ingressu(pecunia_formare(a, f, p),
            a.moneta, f, &lectum) == PECUNIA_BENE);
        CREDO_VERUM(pecunia_aequalis(lectum, rotundatum));
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
    { "oraculum", _sectio_oraculum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "solvibilis", _sectio_solvibilis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cambium", _sectio_cambium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exempla", _sectio_exempla, NIHIL, NIHIL, NIHIL, NIHIL },
    { "degeneres", _sectio_degeneres, NIHIL, NIHIL, NIHIL, NIHIL },
    { "partitio_proprietates", _sectio_partitio_proprietates, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "canonica", _sectio_canonica, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extrema", _sectio_extrema, NIHIL, NIHIL, NIHIL, NIHIL },
    { "numeri", _sectio_numeri, NIHIL, NIHIL, NIHIL, NIHIL },
    { "circuitus", _sectio_circuitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "formae", _sectio_formae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extrema_formae", _sectio_extrema_formae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ingressus", _sectio_ingressus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ingressus_signa", _sectio_ingressus_signa, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "circuitus_formae", _sectio_circuitus_formae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pecunia", SECTIONES);
}
