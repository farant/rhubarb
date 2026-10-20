/* http_r2: redirectio ad alium hospitem - capita vocantis (Authorization,
 * Cookie, Content-Length) transferuntur */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include "http_r_servus.h"

static void exitum(HttpResultus res)
{
    printf("  successus=%d error=%d desc='%.*s'", (int)res.successus, (int)res.error,
           (int)res.error_descriptio.mensura, res.error_descriptio.datum ? (const char*)res.error_descriptio.datum : "");
    if (res.successus) printf(" status=%u", (unsigned)res.responsum->status);
    printf("\n"); fflush(stdout);
}

int main(void)
{
    int fa, fb, pa, pb; pid_t qa, qb;
    char ra[256]; const char* respa[1]; const char* respb[1];
    char url[64];
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpPetitio* pet;
    pa = auscultare(&fa); pb = auscultare(&fb);
    sprintf(ra, "HTTP/1.1 302 Found\r\nLocation: http://localhost:%d/alibi\r\nContent-Length: 0\r\n\r\n", pb);
    respa[0] = ra;
    respb[0] = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok";
    qa = servire(fa, "A 127.0.0.1", respa, NULL, 1);
    qb = servire(fb, "B localhost (alius hospes)", respb, NULL, 1);
    close(fa); close(fb);
    sprintf(url, "http://127.0.0.1:%d/login", pa);
    printf("CASUS POST cum Authorization, Cookie, Content-Length a vocatore; 302 ad alium hospitem\n");
    fflush(stdout);
    pet = http_petitio_creare(p, HTTP_POST, url);
    http_petitio_caput_addere(pet, "Authorization", "Bearer SECRETUM");
    http_petitio_caput_addere(pet, "Cookie", "sessio=SECRETA");
    http_petitio_caput_addere(pet, "Content-Length", "5");
    http_petitio_corpus_ponere(pet, "hello", 5);
    exitum(http_exsequi_cum_redirectionibus(pet, p, 5));
    exspectare(qa); exspectare(qb);
    piscina_destruere(p);
    return 0;
}
