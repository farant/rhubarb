/* http_r1: Location longitudinis unius ("/") in _resolvere_url_relativum */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include "http_r_servus.h"

static void casum(const char* loc)
{
    int fd; int portus; pid_t pid;
    char r0[256]; const char* resp[2];
    char url[64];
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpPetitio* pet; HttpResultus res;
    portus = auscultare(&fd);
    sprintf(r0, "HTTP/1.1 302 Found\r\nLocation: %s\r\nContent-Length: 0\r\n\r\n", loc);
    resp[0] = r0;
    resp[1] = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok";
    pid = servire(fd, "servus", resp, NULL, 2);
    close(fd);
    sprintf(url, "http://127.0.0.1:%d/a/b", portus);
    printf("CASUS Location '%s' (mensura %d):\n", loc, (int)strlen(loc));
    fflush(stdout);
    pet = http_petitio_creare(p, HTTP_GET, url);
    res = http_exsequi_cum_redirectionibus(pet, p, 5);
    printf("  successus=%d error=%d desc='%.*s'", (int)res.successus, (int)res.error,
           (int)res.error_descriptio.mensura, res.error_descriptio.datum ? (const char*)res.error_descriptio.datum : "");
    if (res.successus) printf(" status=%u corpus='%.*s'", (unsigned)res.responsum->status,
           (int)res.responsum->corpus.mensura, (const char*)res.responsum->corpus.datum);
    printf("\n"); fflush(stdout);
    exspectare(pid);
    piscina_destruere(p);
}

int main(void)
{
    casum("/x");
    casum("/");
    return 0;
}
