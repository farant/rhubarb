/* probatio_custodia.c - custodia (hospitium v3 T7): tesserae per
 * labores reactoris (hash, verificatio, PHC), tectum concurrentiae
 * cum timer interim currente, usor ignotus = hash plenum (gradus
 * aequales), parametri veteres -> renovanda, sessiones, CSRF, origo,
 * frenum (tempus iniectum), secretum servi (0600, modus latior
 * recusatus). Parametri productionis (19 MiB) - ~30 ms per hash. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "lectiones.h"
#include "moneta.h"
#include "reactor.h"
#include "argon2id.h"
#include "custodia.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define VICES_MAXIMAE (C * M)

nomen structura {
     Piscina* p;
         i32  hashes;
      chorda  phc;
         i32  verificationes;
         b32  bona;
         b32  renovanda;
} Exitus;

interior vacuum
_hash_venit (
     vacuum* datum,
     chorda  phc)
{
    Exitus* e = (Exitus*)datum;

    e->hashes++;
    e->phc.mensura  = phc.mensura;
    e->phc.datum    = (i8*)piscina_allocare(e->p, (i64)phc.mensura + I);
    si (phc.mensura > ZEPHYRUM)
    {
        memcpy(e->phc.datum, phc.datum, (size_t)phc.mensura);
    }
}

interior vacuum
_verificatio_venit (
     vacuum* datum,
        b32  bona,
        b32  renovanda)
{
    Exitus* e = (Exitus*)datum;

    e->verificationes++;
    e->bona       = bona;
    e->renovanda  = renovanda;
}

interior Custodia*
_custodia (
    CredoContextus* c,
           Reactor* r,
                i8  sal_secreti)
{
    CustodiaConfiguratio cfg;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    memset(cfg.secretum, sal_secreti, CUSTODIA_SECRETUM);
    redde custodia_creare(&cfg, r, c->piscina);
}

/* ansam agere donec *signum >= n */
interior b32
_currere (
    Reactor* r,
        i32* signum,
        i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < VICES_MAXIMAE && *signum < n; k++)
    {
        reactor_poll(r, I);
    }
    redde *signum >= n;
}

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior vacuum
_sectio_hash_verifica (CredoContextus* c)
{
              Reactor* r       = reactor_creare(c->piscina);
             Custodia* custos  = _custodia(c, r, VII);
               Exitus  e;
    Argon2idParametri  par;
                   i8  sal[LXIV];
                   i8  digestum[LXIV];
                  i32  sn;
                  i32  dn;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.p = c->piscina;
    CREDO_NECESSE_NON_NIHIL(custos);
    CREDO_FALSUM(custodia_tesseram_digerere(custos, _c("brevis",
        c->piscina),
        _hash_venit, &e));
    CREDO_VERUM(custodia_tesseram_digerere(custos, _c("tessera bona",
        c->piscina), _hash_venit, &e));
    CREDO_AEQUALIS_I32(e.hashes, ZEPHYRUM);   /* reditus in ansa */
    CREDO_VERUM(_currere(r, &e.hashes, I));
    CREDO_VERUM(argon2id_phc_legere(e.phc, &par, sal, &sn, digestum,
        &dn));
    CREDO_AEQUALIS_I32(par.m_cost_kib, 19456);
    CREDO_AEQUALIS_I32(par.t_cost, II);
    CREDO_AEQUALIS_I32(par.lanae, I);
    CREDO_AEQUALIS_I32(sn, XVI);
    CREDO_AEQUALIS_I32(dn, XXXII);

    CREDO_VERUM(custodia_tesseram_verificare(custos, e.phc,
        _c("tessera bona",
        c->piscina), _verificatio_venit, &e));
    CREDO_VERUM(_currere(r, &e.verificationes, I));
    CREDO_VERUM(e.bona);
    CREDO_FALSUM(e.renovanda);
    CREDO_VERUM(custodia_tesseram_verificare(custos, e.phc,
        _c("tessera mala",
        c->piscina), _verificatio_venit, &e));
    CREDO_VERUM(_currere(r, &e.verificationes, II));
    CREDO_FALSUM(e.bona);

    /* tessera > MXXIV octeti: recusata sine reditu */
    {
        character* longa = (character*)piscina_allocare(c->piscina,
                               MXXV + I);

        memset(longa, 'a', MXXV);
        longa[MXXV] = '\0';
        CREDO_FALSUM(custodia_tessera_valida(_c(longa, c->piscina)));
        CREDO_FALSUM(custodia_tesseram_verificare(custos, e.phc,
            _c(longa,
            c->piscina), _verificatio_venit, &e));
    }
    CREDO_VERUM(custodia_tessera_valida(_c("octo1234", c->piscina)));
    CREDO_FALSUM(custodia_tessera_valida(_c("septem7", c->piscina)));
    reactor_destruere(r);
}

interior vacuum
_timer_ictus (
    vacuum* datum)
{
    (*(i32*)datum)++;
}

/* V hashes: numquam plus quam II simul; omnes finiunt; timer currit */
interior vacuum
_sectio_concurrentia (CredoContextus* c)
{
     Reactor* r       = reactor_creare(c->piscina);
    Custodia* custos  = _custodia(c, r, VII);
      Exitus  e;
         i32  ictus    = ZEPHYRUM;
         i32  maximum  = ZEPHYRUM;
         i32  k;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.p = c->piscina;
    CREDO_NECESSE_NON_NIHIL(custos);
    (vacuum)reactor_timer_adicere(r, I, VERUM, _timer_ictus, &ictus);
    per (k = ZEPHYRUM; k < V; k++)
    {
        CREDO_VERUM(custodia_tesseram_digerere(custos,
            _c("tessera concurrens",
            c->piscina), _hash_venit, &e));
    }
    per (k = ZEPHYRUM; k < VICES_MAXIMAE && e.hashes < V; k++)
    {
        i32 n;

        reactor_poll(r, I);
        n = custodia_opera_currentia(custos);
        si (n > maximum)
        {
            maximum = n;
        }
    }
    CREDO_AEQUALIS_I32(e.hashes, V);
    CREDO_AEQUALIS_I32(maximum, II);
    CREDO_AEQUALIS_I32(custodia_opera_currentia(custos), ZEPHYRUM);
    CREDO_VERUM(ictus > ZEPHYRUM);
    reactor_destruere(r);
}

/* usor ignotus: gradus aequales verificationi verae, bona FALSUM */
interior vacuum
_sectio_ignotus (CredoContextus* c)
{
     Reactor* r       = reactor_creare(c->piscina);
    Custodia* custos  = _custodia(c, r, VII);
      Exitus  e;
         s64  ante;
         s64  verus;
         s64  ficticius;
         s64  malus;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.p = c->piscina;
    CREDO_NECESSE_NON_NIHIL(custos);
    CREDO_VERUM(custodia_tesseram_digerere(custos, _c("tessera vera",
        c->piscina), _hash_venit, &e));
    CREDO_VERUM(_currere(r, &e.hashes, I));

    ante = custodia_gradus_summa(custos);
    CREDO_VERUM(custodia_tesseram_verificare(custos, e.phc,
        _c("tessera vera",
        c->piscina), _verificatio_venit, &e));
    CREDO_VERUM(_currere(r, &e.verificationes, I));
    verus = custodia_gradus_summa(custos) - ante;
    CREDO_VERUM(e.bona);

    ante = custodia_gradus_summa(custos);
    CREDO_VERUM(custodia_tesseram_verificare(custos, _c("", c->piscina),
        _c("tessera vera", c->piscina), _verificatio_venit, &e));
    CREDO_VERUM(_currere(r, &e.verificationes, II));
    ficticius = custodia_gradus_summa(custos) - ante;
    CREDO_FALSUM(e.bona);

    ante = custodia_gradus_summa(custos);
    CREDO_VERUM(custodia_tesseram_verificare(custos,
        _c("$argon2id$malum",
        c->piscina), _c("tessera vera", c->piscina), _verificatio_venit,
        &e));
    CREDO_VERUM(_currere(r, &e.verificationes, III));
    malus = custodia_gradus_summa(custos) - ante;
    CREDO_FALSUM(e.bona);

    CREDO_VERUM(verus > C);
    CREDO_VERUM(ficticius == verus);
    CREDO_VERUM(malus == verus);
    reactor_destruere(r);
}

/* phc cum t = 1 (configuratio t = 2): verificat, renovanda */
interior vacuum
_sectio_renovanda (CredoContextus* c)
{
              Reactor* r       = reactor_creare(c->piscina);
             Custodia* custos  = _custodia(c, r, VII);
               Exitus  e;
    Argon2idParametri  par;
                   i8  sal[XVI];
                   i8  digestum[XXXII];
              Piscina* p = piscina_generare_dynamicum("renovanda", LXIV
                             * MXXIV);

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.p = c->piscina;
    CREDO_NECESSE_NON_NIHIL(custos);
    memset(&par, ZEPHYRUM, magnitudo(par));
    par.t_cost      = I;
    par.m_cost_kib  = 19456;
    par.lanae       = I;
    par.longitudo   = XXXII;
    memset(sal, V, XVI);
    CREDO_VERUM(argon2id_computare(&par, (constans i8*)"vetus tessera",
        XIII, sal, XVI, digestum, p));
    piscina_destruere(p);
    CREDO_VERUM(custodia_tesseram_verificare(custos,
        argon2id_phc_scribere(&par,
        sal, XVI, digestum, c->piscina), _c("vetus tessera",
        c->piscina),
        _verificatio_venit, &e));
    CREDO_VERUM(_currere(r, &e.verificationes, I));
    CREDO_VERUM(e.bona);
    CREDO_VERUM(e.renovanda);
    reactor_destruere(r);
}

interior vacuum
_sectio_sessio (CredoContextus* c)
{
    CustodiaSessio a;
    CustodiaSessio b;
          Sigillum s;
               i32 i;

    (vacuum)c;
    CREDO_VERUM(custodia_sessionem_creare(&a));
    CREDO_VERUM(custodia_sessionem_creare(&b));
    CREDO_AEQUALIS_I32((i32)strlen(a.signum), XLIII);
    CREDO_VERUM(strcmp(a.signum, b.signum) != ZEPHYRUM);
    per (i = ZEPHYRUM; i < XLIII; i++)
    {
        character k = a.signum[i];

        CREDO_VERUM((k >= 'A' && k <= 'Z') || (k >= 'a' && k <= 'z')
            || (k >= '0' && k <= '9') || k == '-' || k == '_');
    }
    s = sigillum_computare(a.signum, XLIII);
    CREDO_VERUM(sigillum_aequale(&s, &a.sigillum));
    s = custodia_signi_sigillum(chorda_ex_literis(a.signum,
        c->piscina));
    CREDO_VERUM(sigillum_aequale(&s, &a.sigillum));
}

interior vacuum
_sectio_csrf (CredoContextus* c)
{
           Reactor* r       = reactor_creare(c->piscina);
          Custodia* custos  = _custodia(c, r, VII);
          Custodia* alia    = _custodia(c, r, IX);
    CustodiaSessio  a;
    CustodiaSessio  b;
            chorda  t;
            chorda  u;
            chorda  mutatum;

    CREDO_NECESSE_NON_NIHIL(custos);
    CREDO_VERUM(custodia_sessionem_creare(&a));
    CREDO_VERUM(custodia_sessionem_creare(&b));
    t = custodia_csrf(custos, &a.sigillum, c->piscina);
    u = custodia_csrf(custos, &a.sigillum, c->piscina);
    CREDO_AEQUALIS_I32(t.mensura, XLIII);
    CREDO_VERUM(chorda_aequalis(t, u));
    CREDO_VERUM(custodia_csrf_verificare(custos, &a.sigillum, t));
    CREDO_FALSUM(custodia_csrf_verificare(custos, &b.sigillum, t));
    CREDO_FALSUM(custodia_csrf_verificare(alia, &a.sigillum, t));
    mutatum.mensura  = t.mensura;
    mutatum.datum    = (i8*)piscina_allocare(c->piscina, t.mensura);
    memcpy(mutatum.datum, t.datum, (size_t)t.mensura);
    mutatum.datum[V] = mutatum.datum[V] == 'A' ? 'B' : 'A';
    CREDO_FALSUM(custodia_csrf_verificare(custos, &a.sigillum,
        mutatum));
    mutatum = t;
    mutatum.mensura--;
    CREDO_FALSUM(custodia_csrf_verificare(custos, &a.sigillum,
        mutatum));
    CREDO_FALSUM(custodia_csrf_verificare(custos, &a.sigillum, _c("",
        c->piscina)));
    reactor_destruere(r);
}

interior vacuum
_sectio_origo (CredoContextus* c)
{
    Piscina* p      = c->piscina;
     chorda  https  = _c("https", p);
     chorda  h      = _c("exemplar.golden.earth", p);

    CREDO_VERUM(custodia_origo_congruit(_c(
        "https://exemplar.golden.earth", p), https, h));
    CREDO_VERUM(custodia_origo_congruit(_c(
        "HTTPS://Exemplar.Golden.Earth", p), https, h));
    CREDO_FALSUM(custodia_origo_congruit(_c(
        "https://exemplar.golden.earth:8443", p), https, h));
    CREDO_VERUM(custodia_origo_congruit(_c("http://127.0.0.1:3010", p),
        _c("http", p), _c("127.0.0.1:3010", p)));
    CREDO_FALSUM(custodia_origo_congruit(_c(
        "http://exemplar.golden.earth", p), https, h));
    CREDO_FALSUM(custodia_origo_congruit(_c(
        "https://exemplar.golden.earth.malum.example", p), https, h));
    CREDO_FALSUM(custodia_origo_congruit(_c(
        "https://exemplar.golden.earth/", p), https, h));
    CREDO_FALSUM(custodia_origo_congruit(_c("null", p), https, h));
    CREDO_FALSUM(custodia_origo_congruit(_c("", p), https, h));
}

interior vacuum
_sectio_frenum (CredoContextus* c)
{
     Reactor* r       = reactor_creare(c->piscina);
    Custodia* custos  = _custodia(c, r, VII);
      chorda  ip      = _c("1.2.3.4", c->piscina);
      chorda  alius   = _c("5.6.7.8", c->piscina);
         s64  t       = 1000000;
         s32  mora;
         s32  prior = ZEPHYRUM;
         i32  k;

    CREDO_NECESSE_NON_NIHIL(custos);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        custodia_frenum_notare(custos, ip, FALSUM, t);
        CREDO_AEQUALIS_S32(custodia_frenum(custos, ip, t), ZEPHYRUM);
    }
    /* V..IX defectus: mora crescens */
    per (k = IV; k < IX; k++)
    {
        custodia_frenum_notare(custos, ip, FALSUM, t);
        mora = custodia_frenum(custos, ip, t);
        CREDO_VERUM(mora > prior);
        prior = mora;
    }
    CREDO_AEQUALIS_S32(custodia_frenum(custos, alius, t), ZEPHYRUM);
    custodia_frenum_notare(custos, ip, FALSUM, t);   /* X */
    CREDO_VERUM(custodia_frenum(custos, ip, t) < ZEPHYRUM);
    /* fenestra (CM s post ultimum) praeterita: purgatum */
    CREDO_VERUM(custodia_frenum(custos, ip, t + CM * M - I) < ZEPHYRUM);
    CREDO_AEQUALIS_S32(custodia_frenum(custos, ip, t + CM * M + I),
        ZEPHYRUM);
    /* successus purgat */
    per (k = ZEPHYRUM; k < VII; k++)
    {
        custodia_frenum_notare(custos, alius, FALSUM, t);
    }
    CREDO_VERUM(custodia_frenum(custos, alius, t) > ZEPHYRUM);
    custodia_frenum_notare(custos, alius, VERUM, t);
    CREDO_AEQUALIS_S32(custodia_frenum(custos, alius, t), ZEPHYRUM);
    reactor_destruere(r);
}

interior vacuum
_sectio_secretum (CredoContextus* c)
{
     character n[MONETA_NANOID_PLAGULAE_MENSURA];
     character via[CXXVIII];
            i8 a[CUSTODIA_SECRETUM];
            i8 b[CUSTODIA_SECRETUM];
            i8 nihil[CUSTODIA_SECRETUM];
    structura stat st;

    (vacuum)c;
    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(via, "/tmp/custodia_secretum_%s", n);
    memset(nihil, ZEPHYRUM, magnitudo(nihil));
    CREDO_VERUM(custodia_secretum_parare(via, a));
    CREDO_VERUM(memcmp(a, nihil, CUSTODIA_SECRETUM) != ZEPHYRUM);
    CREDO_VERUM(lectiones_stat(via, &st) == ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)(st.st_mode & 0777), 0600);
    CREDO_VERUM(st.st_size == CUSTODIA_SECRETUM);
    CREDO_VERUM(custodia_secretum_parare(via, b));
    CREDO_VERUM(memcmp(a, b, CUSTODIA_SECRETUM) == ZEPHYRUM);
    /* modus latior recusatur */
    CREDO_VERUM(filum_modum_ponere(via, 0644));
    CREDO_FALSUM(custodia_secretum_parare(via, b));
    (vacuum)filum_delere(via);
    /* mensura mala recusatur */
    CREDO_VERUM(filum_scribere_literis(via, "brevis"));
    CREDO_VERUM(filum_modum_ponere(via, 0600));
    CREDO_FALSUM(custodia_secretum_parare(via, b));
    (vacuum)filum_delere(via);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "hash_verifica", _sectio_hash_verifica, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "concurrentia", _sectio_concurrentia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "renovanda", _sectio_renovanda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sessio", _sectio_sessio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "csrf", _sectio_csrf, NIHIL, NIHIL, NIHIL, NIHIL },
    { "origo", _sectio_origo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "frenum", _sectio_frenum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "secretum", _sectio_secretum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("custodia", SECTIONES);
}
