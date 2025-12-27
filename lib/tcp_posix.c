/* tcp_posix.c - Implementatio TCP pro POSIX (macOS, Linux)
 */

#include "tcp.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
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
