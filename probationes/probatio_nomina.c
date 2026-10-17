/* probatio_nomina.c - resolutio nominum non-obstruens (http-vocatio
 * II.1): localhost sine rete, IP litteralis sine quaestione, finis
 * numquam intra incipere, abrumpere tacet, nihil in reactore post
 * finem. Portus in addressu ipso (R3): octeti II-III big-endian in
 * sockaddr_in et sockaddr_in6 (BSD: sa_len, sa_family, portus). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "nomina.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORTUS_PROBATIONIS (VIII * M + LXXX)   /* 8080 */

nomen structura {
                i32 vocatus;
                i32 numerus;
             chorda error;
    NominaAddressus primus;
                b32 quattuor;       /* AF_INET inter responsa */
                i32 portus_primi;
} Exitus;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_finis (
                      vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Exitus* x = (Exitus*)datum;
       i32  k;

    x->vocatus++;
    x->numerus  = numerus;
    x->error    = error;
    si (numerus > ZEPHYRUM)
    {
        constans i8* o = addressus[ZEPHYRUM].octeti;

        x->primus        = addressus[ZEPHYRUM];
        x->portus_primi  = (i32)(insignatus character)o[II] * CCLVI
            + (i32)(insignatus character)o[III];
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (addressus[k].familia == AF_INET)
        {
            x->quattuor = VERUM;
        }
    }
}

interior vacuum
_exspectare (
    Reactor* r,
     Exitus* x,
        s64  tectum_ms)
{
    s64 initium = _ms_nunc();

    dum (x->vocatus == ZEPHYRUM && _ms_nunc() - initium < tectum_ms)
    {
        (vacuum)reactor_poll(r, L);
    }
}

interior vacuum
_sectio_hospes_localis (
    CredoContextus* c)
{
            Reactor* r = reactor_creare(c->piscina);
     NominaQuaestio* q;
             chorda  error;
             Exitus  x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    q = nomina_quaerere_incipere(r, chorda_ex_literis("localhost",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina,
        &error);
    CREDO_NECESSE_NON_NIHIL(q);
    /* numquam intra incipere */
    CREDO_AEQUALIS_I32(x.vocatus, ZEPHYRUM);
    _exspectare(r, &x, III * M);
    CREDO_AEQUALIS_I32(x.vocatus, I);
    CREDO_AEQUALIS_I32(x.error.mensura, ZEPHYRUM);
    CREDO_VERUM(x.numerus >= I);
    CREDO_VERUM(x.quattuor);
    CREDO_AEQUALIS_I32(x.portus_primi, PORTUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* IP litteralis: sine DNS, finis post transitum primum, non ante */
interior vacuum
_sectio_litteralis (
    CredoContextus* c)
{
            Reactor* r = reactor_creare(c->piscina);
     NominaQuaestio* q;
             chorda  error;
             Exitus  x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    q = nomina_quaerere_incipere(r, chorda_ex_literis("127.0.0.1",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina,
        &error);
    CREDO_NECESSE_NON_NIHIL(q);
    CREDO_AEQUALIS_I32(x.vocatus, ZEPHYRUM);
    /* nulla fossa DNS */
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(x.vocatus, I);
    CREDO_AEQUALIS_I32(x.numerus, I);
    CREDO_AEQUALIS_I32((i32)x.primus.familia, (i32)AF_INET);
    CREDO_AEQUALIS_I32(x.portus_primi, PORTUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* abrumpere statim: finis numquam, nihil relictum; iterum: nihil */
interior vacuum
_sectio_abruptum (
    CredoContextus* c)
{
            Reactor* r = reactor_creare(c->piscina);
     NominaQuaestio* q;
             chorda  error;
             Exitus  x;
                s64  initium;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    q = nomina_quaerere_incipere(r, chorda_ex_literis("localhost",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina,
        &error);
    CREDO_NECESSE_NON_NIHIL(q);
    nomina_quaerere_abrumpere(q);
    nomina_quaerere_abrumpere(q);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
    initium = _ms_nunc();
    dum (_ms_nunc() - initium < CC)
    {
        (vacuum)reactor_poll(r, L);
    }
    CREDO_AEQUALIS_I32(x.vocatus, ZEPHYRUM);
}

/* hospes vacuus: NIHIL + error nominatus, nihil in reactore */
interior vacuum
_sectio_vacuus (
    CredoContextus* c)
{
            Reactor* r = reactor_creare(c->piscina);
             chorda  error;
             Exitus  x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    CREDO_VERUM(nomina_quaerere_incipere(r, chorda_ex_literis("",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina,
        &error) == NIHIL);
    CREDO_VERUM(error.mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}


interior NominaAddressus
_addressus_familiae (
    s32 familia,
    i32 nota)
{
    NominaAddressus a;

    memset(&a, ZEPHYRUM, magnitudo(a));
    a.familia = familia;
    a.mensura = nota;      /* signum ordinis (non sockaddr verum) */
    redde a;
}

/* RFC 8305 par. 4: familiae alternantur, incipiens a familia PRIMA
 * responsi; ordo intra familiam servatur */
interior vacuum
_sectio_alternare (
    CredoContextus* c)
{
    NominaAddressus a[V];

    (vacuum)c;
    a[ZEPHYRUM]  = _addressus_familiae(AF_INET6, I);
    a[I]         = _addressus_familiae(AF_INET6, II);
    a[II]        = _addressus_familiae(AF_INET6, III);
    a[III]       = _addressus_familiae(AF_INET, IV);
    a[IV]        = _addressus_familiae(AF_INET, V);
    nomina_familias_alternare(a, V);
    CREDO_AEQUALIS_I32(a[ZEPHYRUM].mensura, I);
    CREDO_AEQUALIS_I32(a[I].mensura, IV);
    CREDO_AEQUALIS_I32(a[II].mensura, II);
    CREDO_AEQUALIS_I32(a[III].mensura, V);
    CREDO_AEQUALIS_I32(a[IV].mensura, III);
    /* IPv4 primum: idem, ab IPv4 */
    a[ZEPHYRUM]  = _addressus_familiae(AF_INET, I);
    a[I]         = _addressus_familiae(AF_INET, II);
    a[II]        = _addressus_familiae(AF_INET6, III);
    nomina_familias_alternare(a, III);
    CREDO_AEQUALIS_I32(a[ZEPHYRUM].mensura, I);
    CREDO_AEQUALIS_I32(a[I].mensura, III);
    CREDO_AEQUALIS_I32(a[II].mensura, II);
    /* familia una aut numerus parvus: immutatum */
    nomina_familias_alternare(a, I);
    CREDO_AEQUALIS_I32(a[ZEPHYRUM].mensura, I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "localhost", _sectio_hospes_localis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "litteralis", _sectio_litteralis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abruptum", _sectio_abruptum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacuus", _sectio_vacuus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alternare", _sectio_alternare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("nomina", SECTIONES);
}
