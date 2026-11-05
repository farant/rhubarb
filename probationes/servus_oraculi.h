/* servus_oraculi.h - oraculum HTTP/2 Go (tools/oracula/http2_servus.go,
 * bibliotheca norma sola) pro probationibus: binarium construere,
 * servum in portu libero incipere (GODEBUG=http2debug=2 -> tabula),
 * claudere. Sine 'go' suita II reddere debet (spec D5). Usus:
 * probatio_apns_vivum (http2-plan T11), probatio_apns_mittens (pulsus-plan
 * T4). */
#ifndef SERVUS_ORACULI_H
#define SERVUS_ORACULI_H

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "filum.h"
#include "processus.h"
#include "tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

nomen structura {
     Processus* processus;
           i32  portus;
} ServusOraculi;

interior constans character*
oraculi_via_quaerere (vacuum)
{
    hic_manens constans character* constans LOCI[III] = {
        "/usr/local/go/bin/go", "/opt/homebrew/bin/go", "/usr/bin/go"
    };
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (filum_existit(LOCI[k]))
        {
            redde LOCI[k];
        }
    }
    redde NIHIL;
}

/* go build -o <binarium> (GOCACHE in area) */
interior b32
servum_oraculi_construere (
    constans character* area,
    constans character* binarium,
               Piscina* p)
{
      constans character* argumenta[VI];
               character  cella[MXXIV];
       ProcessusResultus  pr;

    si (!oraculi_via_quaerere() || strlen(area) > CM)
    {
        redde FALSUM;
    }
    sprintf(cella, "%s/gocache", area);
    (vacuum)setenv("GOCACHE", cella, I);
    (vacuum)setenv("GOTELEMETRY", "off", I);
    argumenta[ZEPHYRUM]  = oraculi_via_quaerere();
    argumenta[I]         = "build";
    argumenta[II]        = "-o";
    argumenta[III]       = binarium;
    argumenta[IV]        = "tools/oracula/http2_servus.go";
    argumenta[V]         = NIHIL;
    pr                   = processus_exsequi(argumenta, CXX * M, p);
    redde pr.successus && pr.codex_exitus == ZEPHYRUM;
}

/* portus liber: auscultator temporarius in 0, portus lectus, clausus */
interior i32
_servi_oraculi_portus (
    Piscina* p)
{
    TcpServusOptiones o  = tcp_servus_optiones_default();
    TcpServusResultus sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &o, p);
    i32 portus;

    si (!sr.successus)
    {
        redde ZEPHYRUM;
    }
    portus = (i32)tcp_servus_obtinere_portum(sr.servus);
    tcp_servus_claudere(sr.servus);
    redde portus;
}

interior b32
_servus_oraculi_auscultat (
         i32  portus,
     Piscina* p)
{
    TcpOptiones o = tcp_optiones_default();
    TcpResultus r;

    o.timeout_ms = CC;
    r = tcp_connectere_cum_optionibus("127.0.0.1", portus, &o, p);
    si (!r.successus)
    {
        redde FALSUM;
    }
    tcp_claudere(r.connexio);
    redde VERUM;
}

/* servus cum certificato <fixa>/localhost.{pem,key} in portu dato (0 =
 * liber); tabula formarum in <tabula>; vexilla addita (e.g. "-modus
 * iterare"). Portus datus: servus renatus in eodem portu (apns_mittens
 * "renata"). */
interior b32
servum_oraculi_incipere_portu (
           ServusOraculi* s,
      constans character* binarium,
      constans character* fixa,
      constans character* tabula,
      constans character* vexilla,
                     i32  portus,
                 Piscina* p)
{
          character  mandatum[MMXLVIII];
 constans character* argumenta[IV];
                i32  k;

    s->processus = NIHIL;
    s->portus = portus
        != ZEPHYRUM ? portus : _servi_oraculi_portus(p);
    si (   s->portus == ZEPHYRUM || strlen(binarium) + strlen(fixa) * II
        + strlen(tabula) + strlen(vexilla) > MD)
    {
        redde FALSUM;
    }
    sprintf(mandatum, "GODEBUG=http2debug=2 exec '%s' -portus %u"
        " -cert '%s/localhost.pem' -clavis '%s/localhost.key' %s"
        " 2> '%s'", binarium, (insignatus integer)s->portus, fixa, fixa,
        vexilla, tabula);
    argumenta[ZEPHYRUM]  = "/bin/sh";
    argumenta[I]         = "-c";
    argumenta[II]        = mandatum;
    argumenta[III]       = NIHIL;
    s->processus         = processus_incipere(argumenta, CXX * M, p);
    si (!s->processus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < LX; k++)
    {
        si (_servus_oraculi_auscultat(s->portus, p))
        {
            redde VERUM;
        }
        (vacuum)processus_pulsare(s->processus);
        (vacuum)usleep(L * M);
    }
    redde FALSUM;
}

interior b32
servum_oraculi_incipere (
           ServusOraculi* s,
      constans character* binarium,
      constans character* fixa,
      constans character* tabula,
      constans character* vexilla,
                 Piscina* p)
{
    redde servum_oraculi_incipere_portu(s, binarium, fixa, tabula,
        vexilla, ZEPHYRUM, p);
}

interior vacuum
servum_oraculi_claudere (
    ServusOraculi* s)
{
    si (s && s->processus)
    {
        processus_abrumpere(s->processus);
        s->processus = NIHIL;
    }
}

#endif /* SERVUS_ORACULI_H */
