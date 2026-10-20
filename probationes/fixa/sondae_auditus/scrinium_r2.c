/* sonda: scrinium_error vivit "vita scrinii"? */
#include "latina.h"
#include "piscina.h"
#include "scrinium.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* migr_malae[] = {
    "CREATE TABLE a (x INTEGER)",
    "CREATE TABLE a (x INTEGER)"   /* duplex -> vitium */
};

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Scrinium* s = scrinium_aperire(p, ":memory:");
    constans character* m;
    character copia[256];
    b32 r;

    /* casus A: vitium directum, deinde vocatio alia successa */
    r = scrinium_exsequi(s, "SELECT * FROM nusquam");
    m = scrinium_error(s);
    strncpy(copia, m, 255); copia[255] = '\0';
    imprimere("A exsequi=%d error='%s'\n", (integer)r, copia);
    (vacuum)scrinium_exsequi(s, "SELECT 1");
    imprimere("A post vocationem successam, eadem regula: '%s' (scrinium_error nunc: '%s')\n",
        m, scrinium_error(s));

    /* casus B: migrare fractum - nuntius post ROLLBACK */
    r = scrinium_migrare(s, migr_malae, 2);
    imprimere("B migrare=%d error='%s'\n", (integer)r, scrinium_error(s));
    scrinium_claudere(s);
    piscina_destruere(p);
    redde 0;
}
