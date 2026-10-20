/* http_r7: caput Host automaticum portum non-ordinarium omittit */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "http.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    HttpPetitio* pet = http_petitio_creare(p, HTTP_GET, "http://exemplum.org:8080/x");
    chorda o = http_petitio_octeti(pet);
    printf("portus=%u\nocteti:\n%.*s\n", (unsigned)http_petitio_portus(pet), (int)o.mensura, (const char*)o.datum);
    pet = http_petitio_creare(p, HTTP_GET, "https://exemplum.org:8443/x");
    o = http_petitio_octeti(pet);
    printf("portus=%u\nocteti:\n%.*s\n", (unsigned)http_petitio_portus(pet), (int)o.mensura, (const char*)o.datum);
    piscina_destruere(p);
    return 0;
}
