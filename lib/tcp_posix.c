/* tcp_posix.c - Implementatio TCP pro POSIX (macOS, Linux)
 */

/* Feature test macros pro POSIX functiones (getaddrinfo, usleep, etc.) */
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200112L

#include "tcp.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


/* ========================================================================
 * STRUCTURA INTERNA
 * ======================================================================== */

structura TcpConnexio {
    Piscina* piscina;
    integer  fd;              /* Socket file descriptor (signed!) */
    b32      clausa;          /* Connexio clausa? */
};


/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

interior TcpResultus
_creare_error(TcpError error, constans character* msg, Piscina* piscina)
{
    TcpResultus res;
    res.successus = FALSUM;
    res.connexio = NIHIL;
    res.error = error;
    si (msg && piscina)
    {
        res.error_descriptio = chorda_ex_literis(msg, piscina);
    }
    alioquin
    {
        res.error_descriptio.datum = NIHIL;
        res.error_descriptio.mensura = 0;
    }
    redde res;
}

interior vacuum
_applicare_optiones(integer fd, constans TcpOptiones* opt)
{
    integer flag;

    si (!opt)
    {
        redde;
    }

    /* TCP_NODELAY */
    si (opt->nodelay)
    {
        flag = 1;
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, (socklen_t)magnitudo(flag));
    }

    /* SO_KEEPALIVE */
    si (opt->keepalive)
    {
        flag = 1;
        setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &flag, (socklen_t)magnitudo(flag));
    }

    /* Timeout */
    si (opt->timeout_ms > 0)
    {
        structura timeval tv;
        tv.tv_sec = opt->timeout_ms / M;
        tv.tv_usec = (opt->timeout_ms % M) * M;
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, (socklen_t)magnitudo(tv));
        setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, (socklen_t)magnitudo(tv));
    }
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - CONNEXIO
 * ======================================================================== */

TcpOptiones
tcp_optiones_default(vacuum)
{
    TcpOptiones opt;
    opt.timeout_ms = XXX * M;  /* 30 seconds */
    opt.nodelay = VERUM;
    opt.keepalive = FALSUM;
    redde opt;
}

TcpResultus
tcp_connectere(
    constans character* hospes,
    i32                 portus,
    Piscina*            piscina)
{
    TcpOptiones opt = tcp_optiones_default();
    redde tcp_connectere_cum_optionibus(hospes, portus, &opt, piscina);
}

TcpResultus
tcp_connectere_cum_optionibus(
    constans character*   hospes,
    i32                   portus,
    constans TcpOptiones* optiones,
    Piscina*              piscina)
{
    TcpResultus res;
    TcpConnexio* conn;
    structura addrinfo hints;
    structura addrinfo* result;
    structura addrinfo* rp;
    character portus_str[VIII];
    integer status;
    integer fd;

    /* Validare argumenta */
    si (!hospes || !piscina)
    {
        redde _creare_error(TCP_ERROR_CONNEXIO, "Argumenta invalida", piscina);
    }

    /* Convertere portus ad chorda */
    {
        i32 p = portus;
        i32 i = 0;
        character temp[VIII];
        i32 len;

        si (p == 0)
        {
            portus_str[0] = '0';
            portus_str[I] = '\0';
        }
        alioquin
        {
            dum (p > 0 && i < VII)
            {
                temp[i++] = (character)('0' + (p % X));
                p /= X;
            }
            len = i;
            per (i = 0; i < len; i++)
            {
                portus_str[i] = temp[len - I - i];
            }
            portus_str[len] = '\0';
        }
    }

    /* Setup hints pro getaddrinfo */
    memset(&hints, 0, magnitudo(hints));
    hints.ai_family = AF_UNSPEC;      /* IPv4 vel IPv6 */
    hints.ai_socktype = SOCK_STREAM;  /* TCP */
    hints.ai_protocol = IPPROTO_TCP;

    /* DNS resolutio */
    status = getaddrinfo(hospes, portus_str, &hints, &result);
    si (status != 0)
    {
        redde _creare_error(TCP_ERROR_DNS, gai_strerror(status), piscina);
    }

    /* Probare omnes addresses */
    fd = -1;
    per (rp = result; rp != NIHIL; rp = rp->ai_next)
    {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        si (fd == -1)
        {
            perge;
        }

        si (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            /* Successus! */
            frange;
        }

        close(fd);
        fd = -1;
    }

    freeaddrinfo(result);

    si (fd == -1)
    {
        redde _creare_error(TCP_ERROR_CONNEXIO, strerror(errno), piscina);
    }

    /* Applicare optiones */
    _applicare_optiones(fd, optiones);

    /* Allocare connexio */
    conn = (TcpConnexio*)piscina_allocare(piscina, (i64)magnitudo(TcpConnexio));
    si (!conn)
    {
        close(fd);
        redde _creare_error(TCP_ERROR_CONNEXIO, "Allocatio fallita", piscina);
    }

    conn->piscina = piscina;
    conn->fd = fd;
    conn->clausa = FALSUM;

    res.successus = VERUM;
    res.connexio = conn;
    res.error = TCP_OK;
    res.error_descriptio.datum = NIHIL;
    res.error_descriptio.mensura = 0;

    redde res;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - I/O
 * ======================================================================== */

i32
tcp_mittere(
    TcpConnexio*   connexio,
    constans i8*   data,
    i32            mensura)
{
    ssize_t n;

    si (!connexio || connexio->clausa || connexio->fd < 0)
    {
        redde (i32)-1;
    }

    si (!data || mensura <= 0)
    {
        redde 0;
    }

    n = send(connexio->fd, data, (size_t)mensura, 0);

    si (n < 0)
    {
        si (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            redde 0;
        }
        redde (i32)-1;
    }

    redde (i32)n;
}

b32
tcp_mittere_omnia(
    TcpConnexio*   connexio,
    constans i8*   data,
    i32            mensura)
{
    i32 totalis = 0;
    i32 restans = mensura;

    si (!connexio || connexio->clausa || !data || mensura <= 0)
    {
        redde FALSUM;
    }

    dum (restans > 0)
    {
        i32 n = tcp_mittere(connexio, data + totalis, restans);
        si (n < 0)
        {
            redde FALSUM;
        }
        si (n == 0)
        {
            /* Socket non paratus, breve mora et retry */
            usleep(M);  /* 1ms */
            perge;
        }
        totalis += n;
        restans -= n;
    }

    redde VERUM;
}

i32
tcp_recipere(
    TcpConnexio* connexio,
    i8*          buffer,
    i32          capacitas)
{
    ssize_t n;

    si (!connexio || connexio->clausa || connexio->fd < 0)
    {
        redde (i32)-1;
    }

    si (!buffer || capacitas <= 0)
    {
        redde 0;
    }

    n = recv(connexio->fd, buffer, (size_t)capacitas, 0);

    si (n < 0)
    {
        si (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            redde 0;
        }
        redde (i32)-1;
    }

    si (n == 0)
    {
        /* Connexio clausa ab hospite */
        connexio->clausa = VERUM;
    }

    redde (i32)n;
}

vacuum
tcp_claudere(TcpConnexio* connexio)
{
    si (!connexio)
    {
        redde;
    }

    si (connexio->fd >= 0 && !connexio->clausa)
    {
        close(connexio->fd);
        connexio->fd = -1;
        connexio->clausa = VERUM;
    }
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - UTILITAS
 * ======================================================================== */

i32
tcp_obtinere_fd(TcpConnexio* connexio)
{
    si (!connexio)
    {
        redde (i32)-1;
    }
    redde (i32)connexio->fd;
}

b32
tcp_est_valida(TcpConnexio* connexio)
{
    redde connexio != NIHIL && !connexio->clausa && connexio->fd >= 0;
}

constans character*
tcp_error_descriptio(TcpError error)
{
    commutatio (error)
    {
        casus TCP_OK:              redde "OK";
        casus TCP_ERROR_DNS:       redde "DNS resolutio fallita";
        casus TCP_ERROR_CONNEXIO:  redde "Connexio fallita";
        casus TCP_ERROR_TIMEOUT:   redde "Timeout";
        casus TCP_ERROR_CLAUSUM:   redde "Connexio clausa";
        casus TCP_ERROR_IO:        redde "I/O error";
        ordinarius:                redde "Error ignotus";
    }
}


/* ========================================================================
 * STRUCTURA INTERNA - SERVUS
 * ======================================================================== */

structura TcpServus {
    Piscina*   piscina;
    integer    fd;
    b32        auscultans;
    b32        non_blocans;
    TcpAddress ligatum;
};


/* ========================================================================
 * FUNCTIONES INTERNAE - SERVUS
 * ======================================================================== */

interior TcpServusResultus
_creare_servus_error(TcpError error, constans character* msg, Piscina* piscina)
{
    TcpServusResultus res;
    res.successus = FALSUM;
    res.servus = NIHIL;
    res.error = error;
    si (msg && piscina)
    {
        res.error_descriptio = chorda_ex_literis(msg, piscina);
    }
    alioquin
    {
        res.error_descriptio.datum = NIHIL;
        res.error_descriptio.mensura = 0;
    }
    redde res;
}

interior TcpError
_ponere_non_blocans_fd(integer fd, b32 non_blocans)
{
    integer flags;

    flags = fcntl(fd, F_GETFL, 0);
    si (flags == -1)
    {
        redde TCP_ERROR_IO;
    }

    si (non_blocans)
    {
        flags |= O_NONBLOCK;
    }
    alioquin
    {
        flags &= ~O_NONBLOCK;
    }

    si (fcntl(fd, F_SETFL, flags) == -1)
    {
        redde TCP_ERROR_IO;
    }

    redde TCP_OK;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - SERVUS
 * ======================================================================== */

TcpServusOptiones
tcp_servus_optiones_default(vacuum)
{
    TcpServusOptiones opt;
    opt.tergum = CXXVIII;        /* 128 backlog */
    opt.reuti_inscriptio = VERUM;
    opt.non_blocans = VERUM;
    redde opt;
}

TcpServusResultus
tcp_servus_creare(
    i32      portus,
    Piscina* piscina)
{
    TcpServusOptiones opt = tcp_servus_optiones_default();
    redde tcp_servus_creare_cum_optionibus(NIHIL, portus, &opt, piscina);
}

TcpServusResultus
tcp_servus_creare_cum_optionibus(
    constans character*         hospes,
    i32                         portus,
    constans TcpServusOptiones* optiones,
    Piscina*                    piscina)
{
    TcpServusResultus res;
    TcpServus* servus;
    integer fd;
    integer flag;
    structura sockaddr_in addr;
    socklen_t addr_len;

    /* Validare argumenta */
    si (!piscina)
    {
        redde _creare_servus_error(TCP_ERROR_CONNEXIO, "Piscina NIHIL", NIHIL);
    }

    si (!optiones)
    {
        redde _creare_servus_error(TCP_ERROR_CONNEXIO, "Optiones NIHIL", piscina);
    }

    /* Creare socket */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    si (fd == -1)
    {
        redde _creare_servus_error(TCP_ERROR_CONNEXIO, strerror(errno), piscina);
    }

    /* SO_REUSEADDR */
    si (optiones->reuti_inscriptio)
    {
        flag = 1;
        si (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag,
                       (socklen_t)magnitudo(flag)) == -1)
        {
            close(fd);
            redde _creare_servus_error(TCP_ERROR_CONNEXIO,
                                        "SO_REUSEADDR fallita", piscina);
        }
    }

    /* Non-blocking mode */
    si (optiones->non_blocans)
    {
        si (_ponere_non_blocans_fd(fd, VERUM) != TCP_OK)
        {
            close(fd);
            redde _creare_servus_error(TCP_ERROR_IO,
                                        "Non-blocking mode fallita", piscina);
        }
    }

    /* Setup address */
    memset(&addr, 0, magnitudo(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((i16)portus);

    si (hospes != NIHIL && hospes[0] != '\0')
    {
        /* Ligare ad specifica interface */
        structura addrinfo hints;
        structura addrinfo* result;
        integer status;

        memset(&hints, 0, magnitudo(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        status = getaddrinfo(hospes, NIHIL, &hints, &result);
        si (status != 0)
        {
            close(fd);
            redde _creare_servus_error(TCP_ERROR_DNS, gai_strerror(status), piscina);
        }

        memcpy(&addr.sin_addr,
               &((structura sockaddr_in*)result->ai_addr)->sin_addr,
               magnitudo(addr.sin_addr));
        freeaddrinfo(result);
    }
    alioquin
    {
        /* INADDR_ANY - omnes interfaces */
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    /* Bind */
    si (bind(fd, (structura sockaddr*)&addr, (socklen_t)magnitudo(addr)) == -1)
    {
        close(fd);
        redde _creare_servus_error(TCP_ERROR_CONNEXIO, strerror(errno), piscina);
    }

    /* Obtinere actual port (si portus 0) */
    addr_len = (socklen_t)magnitudo(addr);
    si (getsockname(fd, (structura sockaddr*)&addr, &addr_len) == -1)
    {
        close(fd);
        redde _creare_servus_error(TCP_ERROR_IO, strerror(errno), piscina);
    }

    /* Allocare servus */
    servus = (TcpServus*)piscina_allocare(piscina, (i64)magnitudo(TcpServus));
    si (!servus)
    {
        close(fd);
        redde _creare_servus_error(TCP_ERROR_CONNEXIO, "Allocatio fallita", piscina);
    }

    servus->piscina = piscina;
    servus->fd = fd;
    servus->auscultans = FALSUM;
    servus->non_blocans = optiones->non_blocans;
    servus->ligatum.portus = (i32)ntohs(addr.sin_port);

    /* Obtinere hospes string */
    si (hospes != NIHIL && hospes[0] != '\0')
    {
        i32 i;
        per (i = 0; i < CCLV && hospes[i] != '\0'; i++)
        {
            servus->ligatum.hospes[i] = hospes[i];
        }
        servus->ligatum.hospes[i] = '\0';
    }
    alioquin
    {
        servus->ligatum.hospes[0] = '0';
        servus->ligatum.hospes[I] = '.';
        servus->ligatum.hospes[II] = '0';
        servus->ligatum.hospes[III] = '.';
        servus->ligatum.hospes[IV] = '0';
        servus->ligatum.hospes[V] = '.';
        servus->ligatum.hospes[VI] = '0';
        servus->ligatum.hospes[VII] = '\0';
    }

    res.successus = VERUM;
    res.servus = servus;
    res.error = TCP_OK;
    res.error_descriptio.datum = NIHIL;
    res.error_descriptio.mensura = 0;

    redde res;
}

TcpError
tcp_servus_auscultare(
    TcpServus* servus,
    i32        tergum)
{
    si (!servus || servus->fd < 0)
    {
        redde TCP_ERROR_IO;
    }

    si (servus->auscultans)
    {
        /* Iam auscultans */
        redde TCP_OK;
    }

    si (tergum <= 0)
    {
        tergum = CXXVIII;
    }

    si (listen(servus->fd, (integer)tergum) == -1)
    {
        redde TCP_ERROR_IO;
    }

    servus->auscultans = VERUM;
    redde TCP_OK;
}

TcpResultus
tcp_servus_accipere(
    TcpServus* servus,
    Piscina*   piscina)
{
    TcpResultus res;
    TcpConnexio* conn;
    integer client_fd;
    structura sockaddr_in client_addr;
    socklen_t addr_len;

    si (!servus || !piscina)
    {
        redde _creare_error(TCP_ERROR_CONNEXIO, "Argumenta invalida", piscina);
    }

    si (!servus->auscultans)
    {
        redde _creare_error(TCP_ERROR_IO, "Servus non auscultans", piscina);
    }

    addr_len = (socklen_t)magnitudo(client_addr);
    client_fd = accept(servus->fd, (structura sockaddr*)&client_addr, &addr_len);

    si (client_fd == -1)
    {
        si (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            /* Non-blocking et nullae connexiones - non est error */
            redde _creare_error(TCP_ERROR_IO, "Nullae connexiones", piscina);
        }
        redde _creare_error(TCP_ERROR_CONNEXIO, strerror(errno), piscina);
    }

    /* Inherit non-blocking mode */
    si (servus->non_blocans)
    {
        _ponere_non_blocans_fd(client_fd, VERUM);
    }

    /* Allocare connexio */
    conn = (TcpConnexio*)piscina_allocare(piscina, (i64)magnitudo(TcpConnexio));
    si (!conn)
    {
        close(client_fd);
        redde _creare_error(TCP_ERROR_CONNEXIO, "Allocatio fallita", piscina);
    }

    conn->piscina = piscina;
    conn->fd = client_fd;
    conn->clausa = FALSUM;

    res.successus = VERUM;
    res.connexio = conn;
    res.error = TCP_OK;
    res.error_descriptio.datum = NIHIL;
    res.error_descriptio.mensura = 0;

    redde res;
}

integer
tcp_servus_obtinere_fd(TcpServus* servus)
{
    si (!servus)
    {
        redde -1;
    }
    redde servus->fd;
}

i32
tcp_servus_obtinere_portum(TcpServus* servus)
{
    si (!servus)
    {
        redde 0;
    }
    redde servus->ligatum.portus;
}

TcpAddress
tcp_servus_obtinere_address(TcpServus* servus)
{
    TcpAddress addr;

    si (!servus)
    {
        addr.hospes[0] = '\0';
        addr.portus = 0;
        redde addr;
    }

    redde servus->ligatum;
}

vacuum
tcp_servus_claudere(TcpServus* servus)
{
    si (!servus)
    {
        redde;
    }

    si (servus->fd >= 0)
    {
        close(servus->fd);
        servus->fd = -1;
    }

    servus->auscultans = FALSUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - NON-BLOCKING
 * ======================================================================== */

TcpError
tcp_ponere_non_blocans(
    TcpConnexio* connexio,
    b32          non_blocans)
{
    si (!connexio || connexio->fd < 0)
    {
        redde TCP_ERROR_IO;
    }

    redde _ponere_non_blocans_fd(connexio->fd, non_blocans);
}

TcpAddress
tcp_obtinere_peer_address(TcpConnexio* connexio)
{
    TcpAddress addr;
    structura sockaddr_in peer_addr;
    socklen_t addr_len;

    addr.hospes[0] = '\0';
    addr.portus = 0;

    si (!connexio || connexio->fd < 0)
    {
        redde addr;
    }

    addr_len = (socklen_t)magnitudo(peer_addr);
    si (getpeername(connexio->fd, (structura sockaddr*)&peer_addr, &addr_len) == 0)
    {
        inet_ntop(AF_INET, &peer_addr.sin_addr, addr.hospes, magnitudo(addr.hospes));
        addr.portus = (i32)ntohs(peer_addr.sin_port);
    }

    redde addr;
}
