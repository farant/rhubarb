/* http_r5: http_exsequi - extensiones fragmentorum chunked */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include "http_r_servus.h"

static void casum(const char* corpus)
{
    int fd, portus; pid_t q; char r0[512]; const char* resp[1]; char url[64];
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpResultus res; HttpPetitio* pet;
    portus = auscultare(&fd);
    sprintf(r0, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n%s", corpus);
    resp[0] = r0;
    q = servire(fd, "servus", resp, NULL, 1);
    close(fd);
    sprintf(url, "http://127.0.0.1:%d/", portus);
    pet = http_petitio_creare(p, HTTP_GET, url);
    res = http_exsequi(pet, p);
    printf("  successus=%d error=%d desc='%.*s'", (int)res.successus, (int)res.error,
           (int)res.error_descriptio.mensura, res.error_descriptio.datum ? (const char*)res.error_descriptio.datum : "");
    if (res.successus) {
        int k;
        printf(" corpus(%u)='", (unsigned)res.responsum->corpus.mensura);
        for (k = 0; k < (int)res.responsum->corpus.mensura; k++) {
            char c = (char)res.responsum->corpus.datum[k];
            if (c == '\r') printf("\\r"); else if (c == '\n') printf("\\n"); else putchar(c);
        }
        printf("'");
    }
    printf("\n"); fflush(stdout);
    exspectare(q);
    piscina_destruere(p);
}

int main(void)
{
    printf("MODERATIO sine extensione:\n");
    casum("5\r\nhello\r\n0\r\n\r\n");
    printf("CASUS cum extensione ';x=y' (RFC 9112 7.1.1):\n");
    casum("5;x=y\r\nhello\r\n0\r\n\r\n");
    return 0;
}
