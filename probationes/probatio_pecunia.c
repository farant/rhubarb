/* probatio_pecunia.c - pecunia (pecunia-plan): tabula monetarum
 * GENERATA ex indicibus ISO 4217 fixis (probationes/fixa/iso4217/)
 * + supplementum, quaestio codicum (sine casu, nihil aliud
 * condonatur), vitia nominata; arithmetica quae recusat (T2:
 * superfluxus, monetae diversae, exitus intactus, proprietates);
 * rotundatio (T3) contra oraculum decimal Pythonis FIXUM
 * (probationes/fixa/pecunia/rotundatio.txt, sha256 infra; scriptum a
 * tools/pecunia_rotundatio_oraculum.py), solvibilis, cambium. */
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
        PecuniaModus modus;
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pecunia", SECTIONES);
}
