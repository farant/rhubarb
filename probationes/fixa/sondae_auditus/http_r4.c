/* http_r4: parser serveri - Content-Length post caput LXIV-um neglegitur */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include <stdio.h>
#include <string.h>

static void casum(int extra)
{
    static char buf[8192];
    int k, n = 0;
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpParser* par;
    HttpParseResultus r;
    n += sprintf(buf + n, "POST /x HTTP/1.1\r\nHost: h\r\n");
    for (k = 0; k < extra; k++) n += sprintf(buf + n, "X-F%d: v\r\n", k);
    n += sprintf(buf + n, "Content-Length: 5\r\n\r\nhello");
    par = http_parser_creare(p);
    r = http_parser_adicere(par, buf, (i32)n);
    printf("capita ante CL: %d -> successus=%d completa=%d status_sugg=%u",
           extra + 1, (int)r.successus, (int)r.completa, (unsigned)r.status_suggestus);
    if (r.petitio) printf(" capita_numerus=%u content_length=%u corpus='%.*s' reliquiae=%u",
           (unsigned)r.petitio->capita_numerus, (unsigned)r.petitio->content_length,
           (int)r.petitio->corpus.mensura, r.petitio->corpus.datum ? (const char*)r.petitio->corpus.datum : "",
           (unsigned)http_parser_reliquiae(par));
    printf("\n");
    piscina_destruere(p);
}

static void casum_dupl(int extra)
{
    static char buf[8192];
    int k, n = 0;
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpParseResultus r;
    n += sprintf(buf + n, "POST /x HTTP/1.1\r\nHost: h\r\nContent-Length: 5\r\n");
    for (k = 0; k < extra; k++) n += sprintf(buf + n, "X-F%d: v\r\n", k);
    n += sprintf(buf + n, "Content-Length: 0\r\n\r\nhello");
    r = http_petitio_parse(buf, (i32)n, p);
    printf("CL duplex, secundum post %d capita -> successus=%d status_sugg=%u\n",
           extra + 2, (int)r.successus, (unsigned)r.status_suggestus);
    piscina_destruere(p);
}

int main(void)
{
    casum(10);
    casum(62);
    casum(63);
    casum(70);
    casum_dupl(10);
    casum_dupl(70);
    return 0;
}
