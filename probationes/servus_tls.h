/* servus_tls.h - servus HTTPS localis pro probationibus TLS sine rete
 *
 * ORACULUM: /opt/homebrew/bin/openssl s_server ut processus filius (non
 * nexus - ut bash pro crusta). '-WWW' fasciculos directorii (TMPDIR)
 * per HTTPS reddit ("HTTP/1.0 200 ok", corpus usque ad clausuram);
 * certificata ex probationes/fixa/tls (tools/tls_fixa_generare.sh, CA
 * probationis). Portus: tcp_servus in portu 0 liberum invenit, claudit,
 * s_server eum accipit (fenestra parva - iteratur). Paratus cum
 * connexio TCP ad eum succedit.
 *
 * Functiones INTERIORES (more ludus_toy.h): includens AMBOS aditus
 * vocare debet - servum_tls_incipere et servum_tls_claudere. */
#ifndef SERVUS_TLS_H
#define SERVUS_TLS_H

#include "latina.h"
#include "piscina.h"
#include "tcp.h"
#include "processus.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVUS_TLS_ORACULUM "/opt/homebrew/bin/openssl"

nomen structura {
    Processus* processus;
          i32  portus;
} ServusTls;

/* portus liber in 127.0.0.1 (tcp_servus portu 0, statim clausus) */
interior i32
_servi_tls_portus (
    Piscina* p)
{
    TcpServusOptiones o = tcp_servus_optiones_default();
    TcpServusResultus sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &o, p);
                  i32 portus;

    si (!sr.successus)
    {
        redde ZEPHYRUM;
    }
    portus = tcp_servus_obtinere_portum(sr.servus);
    tcp_servus_claudere(sr.servus);
    redde portus;
}

/* connexio obstruens brevis ad portum: servus auscultat? */
interior b32
_servi_tls_auscultat (
         i32  portus,
     Piscina* p)
{
    TcpOptiones o = tcp_optiones_default();
    TcpResultus r;

    o.timeout_ms = C;
    r = tcp_connectere_cum_optionibus("127.0.0.1", portus, &o, p);
    si (!r.successus)
    {
        redde FALSUM;
    }
    tcp_claudere(r.connexio);
    redde VERUM;
}

/* s_server in directorio dir cum certificato fixo <nomen>.pem/.key et
 * vexillis additis (e.g. "-tls1_1 -cipher DEFAULT:@SECLEVEL=0": servus
 * TLS 1.1 solus, tls-bearssl T6); VERUM si intra II s auscultat. fixa:
 * via absoluta ad probationes/fixa/tls */
interior b32
servum_tls_incipere_cum (
             ServusTls* s,
    constans character* dir,
    constans character* fixa,
    constans character* certificatum,
    constans character* additamenta,
               Piscina* p)
{
              character  mandatum[MXXIV];
              character  portus_c[XXIV];
     constans character* argumenta[V];
                    i32  conatus;
                    i32  k;
            FilumStatus  status;

    memset(s, ZEPHYRUM, magnitudo(*s));
    si (   !filum_status(SERVUS_TLS_ORACULUM, &status)
        || !status.potest_exsequi)
    {
        redde FALSUM;
    }
    per (conatus = ZEPHYRUM; conatus < III; conatus++)
    {
        s->portus = _servi_tls_portus(p);
        sprintf(portus_c, "%u", (insignatus integer)s->portus);
        sprintf(mandatum, "cd '%s' && exec " SERVUS_TLS_ORACULUM
            " s_server -accept %s -cert '%s/%s.pem' -key '%s/%s.key'"
            " -WWW -quiet %s", dir, portus_c, fixa, certificatum, fixa,
            certificatum, additamenta ? additamenta : "");
        argumenta[ZEPHYRUM]  = "/bin/sh";
        argumenta[I]         = "-c";
        argumenta[II]        = mandatum;
        argumenta[III]       = NIHIL;
        s->processus         = processus_incipere(argumenta, LX * M, p);
        si (!s->processus)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < XL; k++)
        {
            si (_servi_tls_auscultat(s->portus, p))
            {
                redde VERUM;
            }
            (vacuum)processus_pulsare(s->processus);
            (vacuum)usleep(L * M);
        }
        processus_abrumpere(s->processus);
        s->processus = NIHIL;
    }
    redde FALSUM;
}

/* ut servum_tls_incipere_cum sine vexillis additis */
interior b32
servum_tls_incipere (
             ServusTls* s,
    constans character* dir,
    constans character* fixa,
    constans character* certificatum,
               Piscina* p)
{
    redde servum_tls_incipere_cum(s, dir, fixa, certificatum, "", p);
}

interior vacuum
servum_tls_claudere (
    ServusTls* s)
{
    si (s && s->processus)
    {
        processus_abrumpere(s->processus);
        s->processus = NIHIL;
    }
}

#endif /* SERVUS_TLS_H */
