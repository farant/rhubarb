/* canalis_posix.c - canalis super AF_UNIX (canalis-spec II.1)
 *
 * Directorium 0700 est murus verus: socket inter bind et chmod modum
 * umask habet, sed nemo alius ad directorium pervenit. uid paris:
 * getpeereid (macOS, BSD), SO_PEERCRED (Linux - struct ucred sub
 * _GNU_SOURCE, canalis P1). Fossae FD_CLOEXEC: filii (processus_*)
 * canalem non hereditant. */
#include "postulata_posix.h"
#ifdef __linux__
#define _GNU_SOURCE 1
#endif
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "tcp.h"
#include "canalis.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define CANALIS_VIA_MAXIMA  CCLVI
#define CANALIS_TERGUM      XVI

structura CanalisAuditor {
      integer fd;
    character via[CANALIS_VIA_MAXIMA];
};

interior CanalisResultus
_recusare (
    constans character* causa,
               Piscina* piscina)
{
    CanalisResultus r;

    r.successus         = FALSUM;
    r.auditor           = NIHIL;
    r.error_descriptio  = chorda_ex_literis(causa, piscina);
    redde r;
}

interior TcpResultus
_tcp_recusare (
               TcpError  error,
     constans character* causa,
                Piscina* piscina)
{
    TcpResultus r;

    r.successus         = FALSUM;
    r.connexio          = NIHIL;
    r.error             = error;
    r.error_descriptio  = chorda_ex_literis(causa, piscina);
    redde r;
}

/* limes sun_path cum NUL: via longitudinis >= limes recusatur */
interior memoriae_index
_limes_viae (vacuum)
{
    structura sockaddr_un a;

    redde magnitudo(a.sun_path);
}

/* inscriptio ex via (longitudo iam probata) */
interior vacuum
_inscriptio (
    constans character* via,
    structura sockaddr_un* a)
{
    memset(a, ZEPHYRUM, magnitudo(*a));
    a->sun_family = AF_UNIX;
    memcpy(a->sun_path, via, strlen(via) + I);
}

interior vacuum
_cloexec (
    integer fd)
{
    integer v = fcntl(fd, F_GETFD);

    si (v >= ZEPHYRUM)
    {
        (vacuum)fcntl(fd, F_SETFD, v | FD_CLOEXEC);
    }
}

interior b32
_non_blocans (
    integer fd)
{
    integer v = fcntl(fd, F_GETFL);

    redde v >= ZEPHYRUM
        && fcntl(fd, F_SETFL, v | O_NONBLOCK) == ZEPHYRUM;
}

CanalisResultus
canalis_audire (
    constans character* via,
               Piscina* piscina)
{
         CanalisResultus  r;
          CanalisAuditor* a;
             FilumStatus  st;
    structura sockaddr_un  inscriptio;
               character  parens[CANALIS_VIA_MAXIMA];
               character  causa[CCLVI];
      constans character* ultima;
          memoriae_index  longitudo;
                 integer  fd;

    si (!via || !via[ZEPHYRUM] || !piscina)
    {
        redde _recusare("via canalis vacua", piscina);
    }
    longitudo = strlen(via);
    si (longitudo >= _limes_viae())
    {
        sprintf(causa, "via canalis longior quam sun_path patitur "
            "(%lu octeti, limes %lu cum NUL)",
            (insignatus longus)longitudo,
            (insignatus longus)_limes_viae());
        redde _recusare(causa, piscina);
    }
    ultima = strrchr(via, '/');
    si (!ultima)
    {
        redde _recusare("via canalis sine directorio", piscina);
    }
    si (ultima == via)
    {
        strcpy(parens, "/");
    }
    alioquin
    {
        memcpy(parens, via, (memoriae_index)(ultima - via));
        parens[ultima - via] = '\0';
    }

    /* directorium: absens -> 0700; praesens -> ipsum, nemini alii */
    si (filum_status(parens, &st))
    {
        si (!st.est_directorium)
        {
            redde _recusare("parens canalis non directorium", piscina);
        }
        si ((st.modus & 077) != ZEPHYRUM)
        {
            sprintf(causa, "directorium canalis apertum (%o) - 0700 "
                "postulatur", (insignatus integer)st.modus);
            redde _recusare(causa, piscina);
        }
    }
    alioquin si (!filum_directorium_creare_cum_modo(parens, 0700))
    {
        sprintf(causa, "directorium canalis creari nequit: %s",
            filum_error_recens() ? filum_error_recens() : "?");
        redde _recusare(causa, piscina);
    }

    /* in via: socket vetus (sera vocantis: nullius vivi) delendum;
     * aliud numquam */
    si (filum_status(via, &st))
    {
        si (!st.est_socket)
        {
            redde _recusare("in via canalis aliud quam socket - non "
                "deletur", piscina);
        }
        si (!filum_delere(via))
        {
            redde _recusare("socket vetus deleri nequit", piscina);
        }
    }

    fd = socket(AF_UNIX, SOCK_STREAM, ZEPHYRUM);
    si (fd < ZEPHYRUM)
    {
        redde _recusare(strerror(errno), piscina);
    }
    _cloexec(fd);
    _inscriptio(via, &inscriptio);
    si (bind(fd, (structura sockaddr*)(vacuum*)&inscriptio,
            (socklen_t)magnitudo(inscriptio)) != ZEPHYRUM)
    {
        sprintf(causa, "bind: %s", strerror(errno));
        close(fd);
        redde _recusare(causa, piscina);
    }
    si (   !filum_modum_ponere(via, 0600)
        || listen(fd, CANALIS_TERGUM) != ZEPHYRUM
        || !_non_blocans(fd))
    {
        sprintf(causa, "socket parari nequit: %s", strerror(errno));
        close(fd);
        (vacuum)filum_delere(via);
        redde _recusare(causa, piscina);
    }

    a = (CanalisAuditor*)piscina_allocare(piscina,
        (i64)magnitudo(CanalisAuditor));
    si (!a)
    {
        close(fd);
        (vacuum)filum_delere(via);
        redde _recusare("memoria deest", piscina);
    }
    a->fd = fd;
    memcpy(a->via, via, longitudo + I);
    r.successus         = VERUM;
    r.auditor           = a;
    r.error_descriptio  = chorda_ex_literis("", piscina);
    redde r;
}

TcpResultus
canalis_accipere (
    CanalisAuditor* auditor,
           Piscina* piscina)
{
    TcpResultus r;
        integer fd;

    si (!auditor || auditor->fd < ZEPHYRUM || !piscina)
    {
        redde _tcp_recusare(TCP_ERROR_IO, "auditor nullus", piscina);
    }
    fd = accept(auditor->fd, NIHIL, NIHIL);
    si (fd < ZEPHYRUM)
    {
        si (   errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR
            || errno == ECONNABORTED)
        {
            redde _tcp_recusare(TCP_ERROR_ITERUM, "", piscina);
        }
        redde _tcp_recusare(TCP_ERROR_IO, strerror(errno), piscina);
    }
    /* BSD hereditat O_NONBLOCK, Linux non: utrimque ponitur */
    _cloexec(fd);
    si (!_non_blocans(fd))
    {
        close(fd);
        redde _tcp_recusare(TCP_ERROR_IO, strerror(errno), piscina);
    }
    r.connexio = tcp_ex_fossa(fd, piscina);
    si (!r.connexio)
    {
        close(fd);
        redde _tcp_recusare(TCP_ERROR_IO, "memoria deest", piscina);
    }
    r.successus         = VERUM;
    r.error             = TCP_OK;
    r.error_descriptio  = chorda_ex_literis("", piscina);
    redde r;
}

integer
canalis_fossa (
    CanalisAuditor* auditor)
{
    redde auditor ? auditor->fd : -I;
}

vacuum
canalis_claudere (
    CanalisAuditor* auditor)
{
    si (!auditor)
    {
        redde;
    }
    si (auditor->fd >= ZEPHYRUM)
    {
        close(auditor->fd);
        auditor->fd = -I;
    }
    (vacuum)filum_delere(auditor->via);
}

TcpResultus
canalis_connectere (
    constans character* via,
               Piscina* piscina)
{
              TcpResultus r;
    structura sockaddr_un  inscriptio;
                character causa[CCLVI];
                  integer fd;

    si (   !via || !via[ZEPHYRUM] || !piscina
        || strlen(via) >= _limes_viae())
    {
        redde _tcp_recusare(TCP_ERROR_CONNEXIO, "via canalis invalida",
            piscina);
    }
    fd = socket(AF_UNIX, SOCK_STREAM, ZEPHYRUM);
    si (fd < ZEPHYRUM)
    {
        redde _tcp_recusare(TCP_ERROR_IO, strerror(errno), piscina);
    }
    _cloexec(fd);
    _inscriptio(via, &inscriptio);
    si (connect(fd, (structura sockaddr*)(vacuum*)&inscriptio,
            (socklen_t)magnitudo(inscriptio)) != ZEPHYRUM)
    {
        sprintf(causa, "connect: %s", strerror(errno));
        close(fd);
        redde _tcp_recusare(TCP_ERROR_CONNEXIO, causa, piscina);
    }
    r.connexio = tcp_ex_fossa(fd, piscina);
    si (!r.connexio)
    {
        close(fd);
        redde _tcp_recusare(TCP_ERROR_IO, "memoria deest", piscina);
    }
    r.successus         = VERUM;
    r.error             = TCP_OK;
    r.error_descriptio  = chorda_ex_literis("", piscina);
    redde r;
}

b32
canalis_par_uid (
    TcpConnexio* connexio,
            i32* uid)
{
    integer fd = (integer)tcp_obtinere_fd(connexio);

    si (fd < ZEPHYRUM || !uid)
    {
        redde FALSUM;
    }
#ifdef __linux__
    {
        structura ucred  cr;
              socklen_t  n = (socklen_t)magnitudo(cr);

        si (   getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &cr, &n)
                   != ZEPHYRUM
            || n != (socklen_t)magnitudo(cr))
        {
            redde FALSUM;
        }
        *uid = (i32)cr.uid;
    }
#else
    {
        uid_t u;
        gid_t g;

        si (getpeereid(fd, &u, &g) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        *uid = (i32)u;
    }
#endif
    redde VERUM;
}

b32
canalis_par_admissus (
    TcpConnexio* connexio)
{
    i32 uid = ZEPHYRUM;

    redde canalis_par_uid(connexio, &uid) && uid == (i32)geteuid();
}
