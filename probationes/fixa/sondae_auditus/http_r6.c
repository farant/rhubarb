/* http_r6: Location relativa cum '://' in quaestione tractatur ut absoluta */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include "http_r_servus.h"

int main(void)
{
    int fa, fb, pa, pb; pid_t qa, qb;
    char ra[256]; const char* respa[2]; const char* respb[1];
    char url[64];
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpPetitio* pet; HttpResultus res;
    pa = auscultare(&fa); pb = auscultare(&fb);
    sprintf(ra, "HTTP/1.1 302 Found\r\nLocation: /login?next=http://127.0.0.1:%d/b\r\nContent-Length: 0\r\n\r\n", pb);
    respa[0] = ra;
    respa[1] = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\n\r\nA-login";
    respb[0] = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\n\r\nB-tertius";
    qa = servire(fa, "A (origo; exspectatur GET /login?next=...)", respa, NULL, 2);
    qb = servire(fb, "B (tertius, numquam tangendus)", respb, NULL, 1);
    close(fa); close(fb);
    sprintf(url, "http://127.0.0.1:%d/start", pa);
    pet = http_petitio_creare(p, HTTP_GET, url);
    http_petitio_caput_addere(pet, "Authorization", "Bearer SECRETUM");
    res = http_exsequi_cum_redirectionibus(pet, p, 5);
    printf("  successus=%d error=%d", (int)res.successus, (int)res.error);
    if (res.successus) printf(" status=%u corpus='%.*s'", (unsigned)res.responsum->status,
        (int)res.responsum->corpus.mensura, (const char*)res.responsum->corpus.datum);
    printf("\n"); fflush(stdout);
    exspectare(qa); exspectare(qb);
    piscina_destruere(p);
    return 0;
}
