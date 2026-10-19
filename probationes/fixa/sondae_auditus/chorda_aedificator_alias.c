#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    ChordaAedificator* a;
    chorda v, s;
    b32 r;

    /* house empty chorda from chorda_ex_literis("") */
    a = chorda_aedificator_creare(pp, 16);
    v = chorda_ex_literis("", pp);
    printf("EX_LITERIS_VACUA\tdatum_nihil=%d\tmensura=%u\n", v.datum == NIHIL, v.mensura);
    r = chorda_aedificator_appendere_chorda(a, v);
    printf("APP_CHORDA_EX_LITERIS_VACUA\tr=%d\n", (int)r);

    /* self append, exactly forcing growth: cap 8, content 5, append self -> 10 > 8 */
    a = chorda_aedificator_creare(pp, 8);
    chorda_aedificator_appendere_literis(a, "abcde");
    v = chorda_aedificator_spectare(a);
    r = chorda_aedificator_appendere_chorda(a, v);
    s = chorda_aedificator_spectare(a);
    printf("SELF_GROW\tr=%d\tmoved=%d\t[%.*s]\n", (int)r, s.datum != v.datum, (int)s.mensura, (char*)s.datum);

    /* self append, no growth */
    a = chorda_aedificator_creare(pp, 64);
    chorda_aedificator_appendere_literis(a, "abcde");
    v = chorda_aedificator_spectare(a);
    r = chorda_aedificator_appendere_chorda(a, v);
    s = chorda_aedificator_spectare(a);
    printf("SELF_NOGROW\tr=%d\t[%.*s]\n", (int)r, (int)s.mensura, (char*)s.datum);

    /* self suffix append overlapping region */
    a = chorda_aedificator_creare(pp, 64);
    chorda_aedificator_appendere_literis(a, "abcdefgh");
    v = chorda_aedificator_spectare(a);
    v.datum += 2; v.mensura = 6;
    r = chorda_aedificator_appendere_chorda(a, v);
    s = chorda_aedificator_spectare(a);
    printf("SELF_SUFFIX\tr=%d\t[%.*s]\n", (int)r, (int)s.mensura, (char*)s.datum);

    /* evasus_json self with growth (control chars => 6x) */
    a = chorda_aedificator_creare(pp, 4);
    chorda_aedificator_appendere_literis(a, "\x01\"\\");
    v = chorda_aedificator_spectare(a);
    r = chorda_aedificator_appendere_evasus_json(a, v);
    s = chorda_aedificator_spectare(a);
    {
        i32 k; printf("JSON_SELF\tr=%d\tlen=%u\thex=", (int)r, s.mensura);
        for (k = 0; k < s.mensura; k++) printf("%02x", (unsigned)(unsigned char)s.datum[k]);
        printf("\n");
    }

    /* stale view after growth: still inside arena, read old bytes */
    a = chorda_aedificator_creare(pp, 4);
    chorda_aedificator_appendere_literis(a, "abcd");
    v = chorda_aedificator_spectare(a);
    chorda_aedificator_truncare(a, 0);
    chorda_aedificator_appendere_literis(a, "WXYZ!");
    printf("STALE_VIEW\t[%.*s]\tlive=[%.*s]\n", (int)v.mensura, (char*)v.datum,
        (int)chorda_aedificator_spectare(a).mensura, (char*)chorda_aedificator_spectare(a).datum);
    return 0;
}
