/* http_r8: tempus petitionis connexionem (connect) planam non terminat */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include "http_r_servus.h"

int main(void)
{
    pid_t q; int st; double t0 = nunc_ms();
    fflush(stdout);
    q = fork();
    if (q == 0)
    {
        Piscina* p = piscina_generare_dynamicum("sonda", 65536);
        HttpPetitio* pet = http_petitio_creare(p, HTTP_GET, "http://10.255.255.1:81/");
        HttpResultus res;
        http_petitio_tempus_ponere(pet, 500);
        res = http_exsequi(pet, p);
        printf("  filius: successus=%d error=%d desc='%.*s' post %.0f ms\n", (int)res.successus, (int)res.error,
               (int)res.error_descriptio.mensura, res.error_descriptio.datum ? (const char*)res.error_descriptio.datum : "",
               nunc_ms() - t0);
        fflush(stdout);
        _exit(0);
    }
    for (;;)
    {
        if (waitpid(q, &st, WNOHANG) == q) { printf("filius finivit post %.0f ms\n", nunc_ms() - t0); break; }
        if (nunc_ms() - t0 > 5000) { printf("filius ADHUC pendet post %.0f ms (tempus 500 ms) - necatur\n", nunc_ms() - t0); kill(q, SIGKILL); waitpid(q, &st, 0); break; }
        usleep(50000);
    }
    return 0;
}
