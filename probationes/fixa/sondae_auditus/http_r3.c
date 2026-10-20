/* http_r3: tempus vocantis in redirectione amittitur */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include "http_r_servus.h"

static void exitum(HttpResultus res, double ms)
{
    printf("  successus=%d error=%d desc='%.*s' tempus=%.0f ms", (int)res.successus, (int)res.error,
           (int)res.error_descriptio.mensura, res.error_descriptio.datum ? (const char*)res.error_descriptio.datum : "", ms);
    if (res.successus) printf(" status=%u", (unsigned)res.responsum->status);
    printf("\n"); fflush(stdout);
}

int main(void)
{
    int fa, fb, pa, pb; pid_t qa, qb;
    char ra[256]; const char* respa[1]; const char* respb[1];
    int morab[1];
    char url[64]; double t0;
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpPetitio* pet;

    /* moderatio: servus lentus directe, tempus 500 ms */
    respb[0] = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nok";
    morab[0] = 2500;
    pb = auscultare(&fb);
    qb = servire(fb, "B lentus", respb, morab, 1);
    close(fb);
    sprintf(url, "http://127.0.0.1:%d/", pb);
    printf("MODERATIO: directe ad servum lentum (2500 ms), tempus 500 ms\n"); fflush(stdout);
    pet = http_petitio_creare(p, HTTP_GET, url);
    http_petitio_tempus_ponere(pet, 500);
    t0 = nunc_ms();
    exitum(http_exsequi_cum_redirectionibus(pet, p, 5), nunc_ms() - t0);
    exspectare(qb);

    /* casus: A 302 -> B lentus, tempus 500 ms */
    pa = auscultare(&fa); pb = auscultare(&fb);
    sprintf(ra, "HTTP/1.1 302 Found\r\nLocation: http://127.0.0.1:%d/\r\nContent-Length: 0\r\n\r\n", pb);
    respa[0] = ra;
    qa = servire(fa, "A", respa, NULL, 1);
    qb = servire(fb, "B lentus", respb, morab, 1);
    close(fa); close(fb);
    sprintf(url, "http://127.0.0.1:%d/", pa);
    printf("CASUS: A 302 -> servus lentus (2500 ms), tempus 500 ms\n"); fflush(stdout);
    pet = http_petitio_creare(p, HTTP_GET, url);
    http_petitio_tempus_ponere(pet, 500);
    t0 = nunc_ms();
    exitum(http_exsequi_cum_redirectionibus(pet, p, 5), nunc_ms() - t0);
    exspectare(qa); exspectare(qb);
    piscina_destruere(p);
    return 0;
}
