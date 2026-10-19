/* sonda: organum - sprintf sine limite in buffer CCLVI / DXII */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "organum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens character fons[XVI * MXXIV];
hic_manens character longa[MMXLVIII];

interior vacuum
longam_facere (integer quot)
{
    memset(longa, 'a', (size_t)quot);
    longa[quot] = '\0';
}

integer
principale (integer argc, character** argv)
{
    Piscina* p;
    InternamentumChorda* in;
    Xar* vitia;
    Organum* o;
    integer modus;
    integer quot;
    i32 i;

    modus = argc > I ? atoi(argv[I]) : I;
    quot  = argc > II ? atoi(argv[II]) : CD;
    p  = piscina_generare_dynamicum("sonda", 65536);
    in = internamentum_creare(p);
    longam_facere(quot);
    si (modus == I)
    {
        /* absens longum typo numerus: legibus_iudicare linea 330 */
        sprintf(fons, "<organum genus=\"probatio\" versio=\"1\"><status>"
            "<campus nomen=\"n\" typus=\"numerus\" vita=\"persistens\""
            " absens=\"%s\"/></status></organum>", longa);
    }
    alioquin si (modus == II)
    {
        /* referentia ligamen ignotum longum: linea 305 */
        sprintf(fons, "<organum genus=\"probatio\" versio=\"1\"><status>"
            "<campus nomen=\"n\" typus=\"textus\" vita=\"persistens\""
            " referentia=\"%s/x\"/></status></organum>", longa);
    }
    alioquin
    {
        /* titulus campi longus + vitium: vitium_addere linea 70 */
        sprintf(fons, "<organum genus=\"probatio\" versio=\"1\"><status>"
            "<campus nomen=\"%s\" typus=\"series\" vita=\"persistens\""
            "/></status></organum>", longa);
    }
    o = organum_legere(chorda_ex_literis(fons, p), p, in, &vitia);
    printf("organum %s, vitia %d\n", o ? "lectum" : "NIHIL",
        (integer)xar_numerus(vitia));
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        chorda* c = (chorda*)xar_obtinere(vitia, i);
        printf("  vitium (%d): %.60s...\n", (integer)c->mensura,
            (constans character*)c->datum);
    }
    piscina_destruere(p);
    redde ZEPHYRUM;
}
