/* probatio_tcp_incipere.c - connexio TCP non-blocans (http-vocatio
 * II.2): servus in 127.0.0.1 (portus ephemerus), addressus per nomina,
 * connexio pendens -> parata -> perficere; portus clausus ->
 * error nominatus; fossa post clausuram paris VERE clauditur (R5:
 * olim tcp_claudere close() omittebat); tcp_ex_fossa. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "nomina.h"
#include "tcp.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

nomen structura {
                i32 vocatus;
                i32 numerus;
    NominaAddressus addressus;
} Responsum;

nomen structura {
    i32 parata;
} Signum;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_nomina_finis (
                      vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Responsum* r = (Responsum*)datum;

    (vacuum)error;
    r->vocatus++;
    r->numerus = numerus;
    si (numerus > ZEPHYRUM)
    {
        r->addressus = addressus[ZEPHYRUM];
    }
}

/* 127.0.0.1:portus per nomina (IP litteralis: sine DNS) */
interior b32
_addressum (
            Reactor* r,
                i32  portus,
    NominaAddressus* exitus,
            Piscina* p)
{
    Responsum x;
       chorda error;
          s64 initium;

    memset(&x, ZEPHYRUM, magnitudo(x));
    si (!nomina_quaerere_incipere(r, chorda_ex_literis("127.0.0.1", p),
            portus, _nomina_finis, &x, p, &error))
    {
        redde FALSUM;
    }
    initium = _ms_nunc();
    dum (x.vocatus == ZEPHYRUM && _ms_nunc() - initium < M)
    {
        (vacuum)reactor_poll(r, L);
    }
    *exitus = x.addressus;
    redde x.numerus == I;
}

interior vacuum
_parata_ad_scribendum (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    (vacuum)fossa;
    (vacuum)eventus;
    ((Signum*)datum)->parata++;
}

interior b32
_exspectare_paratam (
        Reactor* r,
    TcpConnexio* conn)
{
    Signum s;
       s64 initium = _ms_nunc();

    s.parata = ZEPHYRUM;
    (vacuum)reactor_adicere(r, (integer)tcp_obtinere_fd(conn),
        REACTOR_SCRIBERE, _parata_ad_scribendum, &s);
    dum (s.parata == ZEPHYRUM && _ms_nunc() - initium < II * M)
    {
        (vacuum)reactor_poll(r, L);
    }
    (vacuum)reactor_removere(r, (integer)tcp_obtinere_fd(conn));
    redde s.parata > ZEPHYRUM;
}

interior TcpServus*
_servum_creare (
    Piscina* p)
{
    TcpServusOptiones o = tcp_servus_optiones_default();
    TcpServusResultus sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &o, p);

    si (   !sr.successus || tcp_servus_auscultare(sr.servus, VIII)
            != TCP_OK)
    {
        redde NIHIL;
    }
    redde sr.servus;
}

interior vacuum
_sectio_connexa (
    CredoContextus* c)
{
            Reactor* r       = reactor_creare(c->piscina);
          TcpServus* servus  = _servum_creare(c->piscina);
    NominaAddressus  a;
        TcpResultus  res;
        TcpResultus  acc;
             chorda  descriptio;
                s64  initium;

    CREDO_NECESSE_NON_NIHIL(servus);
    CREDO_NECESSE(_addressum(r, tcp_servus_obtinere_portum(servus), &a,
        c->piscina));
    initium  = _ms_nunc();
    res      = tcp_incipere(a.octeti, a.mensura, c->piscina);
    CREDO_VERUM(_ms_nunc() - initium < C);     /* non obstat */
    CREDO_NECESSE(res.successus);
    CREDO_NECESSE(_exspectare_paratam(r, res.connexio));
    descriptio.datum    = NIHIL;
    descriptio.mensura  = ZEPHYRUM;
    CREDO_AEQUALIS_I32((i32)tcp_incipere_perficere(res.connexio,
        &descriptio,
        c->piscina), (i32)TCP_OK);
    acc = tcp_servus_accipere(servus, c->piscina);
    CREDO_VERUM(acc.successus);
    tcp_claudere(acc.connexio);
    tcp_claudere(res.connexio);
    tcp_servus_claudere(servus);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}

/* portus sine auscultatore: perficere errorem nominat */
interior vacuum
_sectio_recusata (
    CredoContextus* c)
{
            Reactor* r       = reactor_creare(c->piscina);
          TcpServus* servus  = _servum_creare(c->piscina);
    NominaAddressus  a;
        TcpResultus  res;
             chorda  descriptio;
                i32  portus;

    CREDO_NECESSE_NON_NIHIL(servus);
    portus = tcp_servus_obtinere_portum(servus);
    tcp_servus_claudere(servus);
    CREDO_NECESSE(_addressum(r, portus, &a, c->piscina));
    res = tcp_incipere(a.octeti, a.mensura, c->piscina);
    si (!res.successus)
    {
        /* recusatio statim (loopback) - etiam licet, nominata */
        CREDO_AEQUALIS_I32((i32)res.error, (i32)TCP_ERROR_CONNEXIO);
        CREDO_VERUM(res.error_descriptio.mensura > ZEPHYRUM);
        redde;
    }
    CREDO_NECESSE(_exspectare_paratam(r, res.connexio));
    descriptio.datum    = NIHIL;
    descriptio.mensura  = ZEPHYRUM;
    CREDO_AEQUALIS_I32((i32)tcp_incipere_perficere(res.connexio,
        &descriptio,
        c->piscina), (i32)TCP_ERROR_CONNEXIO);
    CREDO_VERUM(descriptio.mensura > ZEPHYRUM);
    tcp_claudere(res.connexio);
}

/* par claudit, tcp_recipere 0 reddit (clausa), tcp_claudere fossam
 * TAMEN claudit; iterum: nihil */
interior vacuum
_sectio_clausura_paris (
    CredoContextus* c)
{
            Reactor* r       = reactor_creare(c->piscina);
          TcpServus* servus  = _servum_creare(c->piscina);
    NominaAddressus  a;
        TcpResultus  res;
        TcpResultus  acc;
                 i8  capsa[XVI];
                s32  fossa;
                s32  n = -I;
                s64  initium;

    CREDO_NECESSE_NON_NIHIL(servus);
    CREDO_NECESSE(_addressum(r, tcp_servus_obtinere_portum(servus), &a,
        c->piscina));
    res = tcp_incipere(a.octeti, a.mensura, c->piscina);
    CREDO_NECESSE(res.successus);
    CREDO_NECESSE(_exspectare_paratam(r, res.connexio));
    acc = tcp_servus_accipere(servus, c->piscina);
    CREDO_NECESSE(acc.successus);
    tcp_claudere(acc.connexio);
    initium = _ms_nunc();
    dum (n != ZEPHYRUM && _ms_nunc() - initium < M)
    {
        n = tcp_recipere(res.connexio, capsa, XVI);
    }
    CREDO_AEQUALIS_S64((s64)n, (s64)ZEPHYRUM);
    fossa = tcp_obtinere_fd(res.connexio);
    CREDO_NECESSE(fossa >= ZEPHYRUM);
    tcp_claudere(res.connexio);
    CREDO_VERUM(fcntl((integer)fossa, F_GETFD) == -I);
    tcp_claudere(res.connexio);
    tcp_servus_claudere(servus);
}

interior vacuum
_sectio_ex_fossa (
    CredoContextus* c)
{
         integer  pipa[II];
     TcpConnexio* conn;

    CREDO_NECESSE(pipe(pipa) == ZEPHYRUM);
    conn = tcp_ex_fossa(pipa[ZEPHYRUM], c->piscina);
    CREDO_NECESSE_NON_NIHIL(conn);
    CREDO_AEQUALIS_I32((i32)tcp_obtinere_fd(conn), (i32)pipa[ZEPHYRUM]);
    tcp_claudere(conn);
    CREDO_VERUM(fcntl(pipa[ZEPHYRUM], F_GETFD) == -I);
    CREDO_VERUM(tcp_ex_fossa(-I, c->piscina) == NIHIL);
    (vacuum)close(pipa[I]);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "connexa", _sectio_connexa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusata", _sectio_recusata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausura paris", _sectio_clausura_paris, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ex fossa", _sectio_ex_fossa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tcp_incipere", SECTIONES);
}
