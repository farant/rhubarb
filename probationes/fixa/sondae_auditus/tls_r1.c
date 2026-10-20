/* sonda: TlsOptiones.timeout_ms = 0 dicitur 'infinitus' - an XXX s? */
#include "latina.h"
#include "piscina.h"
#include "tls.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

interior vacuum
experiri (constans character* titulus, TlsVectura v, i32 mora, integer portus_auditor, Piscina* p)
{
    TlsOptiones o;
    TlsResultus r;
    time_t t0, t1;

    memset(&o, 0, sizeof(o));
    o.timeout_ms = mora;
    o.verificare_certificatum = FALSUM;
    o.vectura = v;
    t0 = time(NIHIL);
    r = tls_connectere_cum_optionibus("127.0.0.1", (i32)portus_auditor, &o, p);
    t1 = time(NIHIL);
    imprimere("%s timeout_ms=%d: successus=%d error=%d tempus=%ld s\n",
        titulus, (int)mora, (int)r.successus, (int)r.error, (long)(t1 - t0));
}

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    integer fd;
    structura sockaddr_in sa;
    socklen_t lon = sizeof(sa);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa.sin_port = 0;
    bind(fd, (structura sockaddr*)&sa, sizeof(sa));
    listen(fd, 16);   /* numquam accept(): nucleus connexionem accipit, nemo respondet */
    getsockname(fd, (structura sockaddr*)&sa, &lon);
    imprimere("auditor portus %d\n", (int)ntohs(sa.sin_port));

    experiri("tls13   ", TLS_VECTURA_TLS13, 1000, (integer)ntohs(sa.sin_port), p);
    experiri("tls13   ", TLS_VECTURA_TLS13, 0, (integer)ntohs(sa.sin_port), p);
    experiri("bearssl ", TLS_VECTURA_BEARSSL, 0, (integer)ntohs(sa.sin_port), p);
    close(fd);
    redde 0;
}
