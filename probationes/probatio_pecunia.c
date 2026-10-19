/* probatio_pecunia.c - pecunia (pecunia-plan): tabula monetarum
 * GENERATA ex indicibus ISO 4217 fixis (probationes/fixa/iso4217/)
 * + supplementum, quaestio codicum (sine casu, nihil aliud
 * condonatur), vitia nominata. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "pecunia.h"
#include "pecunia_monetae.h"
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "tabula", _sectio_tabula, NIHIL, NIHIL, NIHIL, NIHIL },
    { "codices", _sectio_codices, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pecunia", SECTIONES);
}
