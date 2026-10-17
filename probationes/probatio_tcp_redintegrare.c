/* probatio_tcp_redintegrare.c - tcp_redintegrare (tls13-spec II.3,
 * tls13-plan T7 P9): eundem parem iterum, fossa nova dup2 in numerum
 * VETEREM. Proprietas quaesita (spec AUDIENDA): registratio reactoris
 * vocantis in numero fossae manet et eventus connexionis NOVAE accipit
 * (reactor poll() numero fossae innititur; kqueue registrationem cum
 * fossa clausa amitteret). Par vetus EOF videt. Modus obstruens: mora
 * tcp_recipere honoratur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "tcp.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_exspectare (
    integer fd,
        b32 scribere)
{
    structura pollfd pf;

    pf.fd       = fd;
    pf.events   = (brevis)(scribere ? POLLOUT : POLLIN);
    pf.revents  = ZEPHYRUM;
    (vacuum)poll(&pf, I, D);
}

interior TcpResultus
_accipere (
    TcpServus* servus,
      Piscina* p)
{
    TcpResultus acc;
            s64 initium = _ms_nunc();

    fac
    {
        _exspectare(tcp_servus_obtinere_fd(servus), FALSUM);
        acc = tcp_servus_accipere(servus, p);
    } dum (!acc.successus && acc.error == TCP_ERROR_ITERUM
           && _ms_nunc() - initium < II * M);
    redde acc;
}

interior TcpServus*
_servum_creare (Piscina* p)
{
    TcpServusOptiones so = tcp_servus_optiones_default();
    TcpServusResultus sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &so, p);

    CREDO_NECESSE(sr.successus);
    CREDO_NECESSE(tcp_servus_auscultare(sr.servus, IV) == TCP_OK);
    redde sr.servus;
}

nomen structura {
    i32 vocationes;
} Numerator;

interior vacuum
_legibilis (
    integer  fd,
        i32  eventus,
     vacuum* datum)
{
    (vacuum)fd;
    (vacuum)eventus;
    ((Numerator*)datum)->vocationes++;
}

/* gradatim: fossa F in reactore; redintegratio -> idem F, servus
 * connexionem SECUNDAM accipit, data in ea reactorem per F excitant,
 * prima EOF videt */
interior vacuum
_sectio_gradatim (CredoContextus* c)
{
                TcpServus* servus = _servum_creare(c->piscina);
    structura sockaddr_in  sa;
              TcpResultus  r;
              TcpResultus  prima;
              TcpResultus  secunda;
                  Reactor* reactor = reactor_creare(c->piscina);
                Numerator  numerator;
                  integer  f;
                       i8  octetus[IV];
                      s32  n = -I;
                      i32  k;

    memset(&sa, ZEPHYRUM, magnitudo(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons((i16)tcp_servus_obtinere_portum(servus));
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    r = tcp_incipere((constans i8*)&sa, (i32)magnitudo(sa), c->piscina);
    CREDO_NECESSE(r.successus);
    f = (integer)tcp_obtinere_fd(r.connexio);
    _exspectare(f, VERUM);
    CREDO_NECESSE(tcp_incipere_perficere(r.connexio, NIHIL, c->piscina)
        == TCP_OK);
    prima = _accipere(servus, c->piscina);
    CREDO_NECESSE(prima.successus);
    numerator.vocationes = ZEPHYRUM;
    CREDO_NECESSE(reactor_adicere(reactor, f, (i32)REACTOR_LEGERE,
        _legibilis, &numerator));

    r = tcp_redintegrare(r.connexio, NIHIL, c->piscina);
    CREDO_NECESSE(r.successus);
    CREDO_AEQUALIS_I32((i32)tcp_obtinere_fd(r.connexio), (i32)f);
    _exspectare(f, VERUM);
    CREDO_AEQUALIS_I32((i32)tcp_incipere_perficere(r.connexio, NIHIL,
        c->piscina), (i32)TCP_OK);
    secunda = _accipere(servus, c->piscina);
    CREDO_NECESSE(secunda.successus);
    /* prima: EOF (fossa vetus a dup2 clausa) */
    per (k = ZEPHYRUM; k < XX && n != ZEPHYRUM; k++)
    {
        _exspectare((integer)tcp_obtinere_fd(prima.connexio), FALSUM);
        n = tcp_recipere(prima.connexio, octetus, IV);
    }
    CREDO_AEQUALIS_S32(n, ZEPHYRUM);
    /* data in secunda -> reactor per F */
    (vacuum)reactor_poll(reactor, ZEPHYRUM);
    CREDO_AEQUALIS_I32(numerator.vocationes, ZEPHYRUM);
    CREDO_VERUM(tcp_mittere_omnia(secunda.connexio, (constans i8*)"x",
        I));
    per (k = ZEPHYRUM; k < XX && numerator.vocationes == ZEPHYRUM; k++)
    {
        (vacuum)reactor_poll(reactor, L);
    }
    CREDO_MAIOR_I32(numerator.vocationes, ZEPHYRUM);
    n = tcp_recipere(r.connexio, octetus, IV);
    CREDO_AEQUALIS_S32(n, I);
    CREDO_AEQUALIS_I32((i32)octetus[ZEPHYRUM], (i32)'x');
    tcp_claudere(r.connexio);
    tcp_claudere(prima.connexio);
    tcp_claudere(secunda.connexio);
    tcp_servus_claudere(servus);
}

/* obstruens: connexio cum optionibus; redintegratio cum mora CC ms ->
 * idem numerus, tcp_recipere sine datis post ~CC ms TCP_ITERUM */
interior vacuum
_sectio_obstruens (CredoContextus* c)
{
      TcpServus* servus  = _servum_creare(c->piscina);
    TcpOptiones  opt     = tcp_optiones_default();
    TcpResultus  r;
    TcpResultus  prima;
    TcpResultus  secunda;
        integer  f;
             i8  octetus[IV];
            s32  n;
            s64  initium;

    r = tcp_connectere_cum_optionibus("127.0.0.1",
        tcp_servus_obtinere_portum(servus), &opt, c->piscina);
    CREDO_NECESSE(r.successus);
    f      = (integer)tcp_obtinere_fd(r.connexio);
    prima  = _accipere(servus, c->piscina);
    CREDO_NECESSE(prima.successus);
    opt.timeout_ms  = CC;
    r               = tcp_redintegrare(r.connexio, &opt, c->piscina);
    CREDO_NECESSE(r.successus);
    CREDO_AEQUALIS_I32((i32)tcp_obtinere_fd(r.connexio), (i32)f);
    secunda = _accipere(servus, c->piscina);
    CREDO_NECESSE(secunda.successus);
    initium  = _ms_nunc();
    n        = tcp_recipere(r.connexio, octetus, IV);
    CREDO_AEQUALIS_S32(n, TCP_ITERUM);
    CREDO_VERUM(_ms_nunc() - initium >= CL);
    CREDO_VERUM(_ms_nunc() - initium < M);
    CREDO_VERUM(tcp_mittere_omnia(secunda.connexio, (constans i8*)"y",
        I));
    n = tcp_recipere(r.connexio, octetus, IV);
    CREDO_AEQUALIS_S32(n, I);
    tcp_claudere(r.connexio);
    tcp_claudere(prima.connexio);
    tcp_claudere(secunda.connexio);
    tcp_servus_claudere(servus);
}

/* connexio clausa: par ignotus -> FALSUM, nihil pendet */
interior vacuum
_sectio_clausa (CredoContextus* c)
{
      TcpServus* servus = _servum_creare(c->piscina);
    TcpResultus  r = tcp_connectere("127.0.0.1",
        tcp_servus_obtinere_portum(servus), c->piscina);

    CREDO_NECESSE(r.successus);
    tcp_claudere(r.connexio);
    CREDO_FALSUM(tcp_redintegrare(r.connexio, NIHIL, c->piscina)
        .successus);
    CREDO_FALSUM(tcp_redintegrare(NIHIL, NIHIL, c->piscina).successus);
    tcp_servus_claudere(servus);
}

/* par connexionem RST abrumpit (claudit cum datis non lectis - nucleus
 * RST mittit): getpeername fossae veteris tunc fallit (ENOTCONN) -
 * redintegratio parem ex connexione memoratum adhibet (recensio M4) */
interior vacuum
_sectio_abrupta (CredoContextus* c)
{
         TcpServus* servus = _servum_creare(c->piscina);
       TcpResultus  r;
       TcpResultus  prima;
       TcpResultus  secunda;
                i8  octetus[IV];
               s32  n = I;
               i32  k;

    r = tcp_connectere("127.0.0.1", tcp_servus_obtinere_portum(servus),
        c->piscina);
    CREDO_NECESSE(r.successus);
    (vacuum)tcp_ponere_non_blocans(r.connexio, VERUM);
    prima = _accipere(servus, c->piscina);
    CREDO_NECESSE(prima.successus);
    /* datum servo missum, numquam lectum: close() -> RST */
    CREDO_VERUM(tcp_mittere_omnia(r.connexio, (constans i8*)"z", I));
    (vacuum)poll(NIHIL, ZEPHYRUM, L);
    tcp_claudere(prima.connexio);
    per (k = ZEPHYRUM; k < XX && n > ZEPHYRUM; k++)
    {
        _exspectare((integer)tcp_obtinere_fd(r.connexio), FALSUM);
        n = tcp_recipere(r.connexio, octetus, IV);
        si (n == TCP_ITERUM)
        {
            n = I;
        }
    }
    CREDO_VERUM(n <= ZEPHYRUM);
    r = tcp_redintegrare(r.connexio, NIHIL, c->piscina);
    CREDO_NECESSE(r.successus);
    _exspectare((integer)tcp_obtinere_fd(r.connexio), VERUM);
    CREDO_AEQUALIS_I32((i32)tcp_incipere_perficere(r.connexio, NIHIL,
        c->piscina), (i32)TCP_OK);
    secunda = _accipere(servus, c->piscina);
    CREDO_VERUM(secunda.successus);
    tcp_claudere(r.connexio);
    si (secunda.successus)
    {
        tcp_claudere(secunda.connexio);
    }
    tcp_servus_claudere(servus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "gradatim", _sectio_gradatim, NIHIL, NIHIL, NIHIL, NIHIL },
    { "obstruens", _sectio_obstruens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausa", _sectio_clausa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "post_rst", _sectio_abrupta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tcp_redintegrare", SECTIONES);
}
