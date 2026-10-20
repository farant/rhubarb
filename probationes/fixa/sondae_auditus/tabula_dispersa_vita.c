#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

static s32 cmp(chorda a, chorda b) { return chorda_comparare(a, b); }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaDispersa* t;
    char scratch[16]; char second[16];
    chorda c, ko; void* v; b32 r; TabulaIterator it;
    unsigned int caps[] = {0u, 1u, 3u, 16u, 17u, 100u, 0x80000001u, 0xFFFFFFFFu};
    int idx;

    /* key lifetime: reused scratch buffer */
    t = tabula_dispersa_creare_chorda(p, 16);
    strcpy(scratch, "alpha"); c.datum=(i8*)scratch; c.mensura=5;
    tabula_dispersa_inserere(t, c, (void*)1);
    strcpy(scratch, "omega");               /* caller reuses buffer */
    printf("post mutationem alvei: continet_literis alpha=%d omega=%d numerus=%u\n",
        tabula_dispersa_continet_literis(t, "alpha"), tabula_dispersa_continet_literis(t, "omega"), t->numerus);
    it = tabula_dispersa_iterator_initium(t); tabula_dispersa_iterator_proximum(&it, &ko, &v);
    printf("  iterator clavis = %.*s datum==scratch:%d\n", (int)ko.mensura, (char*)ko.datum, (void*)ko.datum == (void*)scratch);

    /* duplicate key, different datum: which chorda is kept */
    t = tabula_dispersa_creare_chorda(p, 16);
    strcpy(scratch, "beta"); c.datum=(i8*)scratch; c.mensura=4; tabula_dispersa_inserere(t, c, (void*)1);
    strcpy(second, "beta"); c.datum=(i8*)second; tabula_dispersa_inserere(t, c, (void*)2);
    it = tabula_dispersa_iterator_initium(t); tabula_dispersa_iterator_proximum(&it, &ko, &v);
    printf("duplicata: numerus=%u v=%ld clavis servata = %s\n", t->numerus, (long)v,
        (void*)ko.datum == (void*)scratch ? "PRIMA (vetus)" : ((void*)ko.datum == (void*)second ? "SECUNDA (nova)" : "copia"));

    /* capacitas */
    for (idx = 0; idx < 8; idx++) {
        if (caps[idx] == 0x80000001u || caps[idx] == 0xFFFFFFFFu) {
            t = tabula_dispersa_creare_chorda(p, caps[idx]);
            printf("creare(%u) -> cap=%u mask=%x\n", caps[idx], t ? t->capacitas : 0, t ? t->capacitas_mask : 0);
        } else {
            t = tabula_dispersa_creare_chorda(p, caps[idx]);
            printf("creare(%u) -> cap=%u mask=%x f=%.3f\n", caps[idx], t->capacitas, t->capacitas_mask, tabula_dispersa_factor_oneris(t));
        }
    }
    /* NIHIL args */
    printf("creare NIHIL friatio -> %p, NIHIL comparatio -> %p, NIHIL piscina -> %p\n",
        (void*)tabula_dispersa_creare(p, 16, NULL, cmp), (void*)tabula_dispersa_creare(p, 16, tabula_friare_fnv1a, NULL),
        (void*)tabula_dispersa_creare_chorda(NULL, 16));
    c.datum=(i8*)"a"; c.mensura=1;
    printf("NIHIL tabula: inserere=%d invenire=%d continet=%d literis=%d delere=%d numerus=%u factor=%f\n",
        tabula_dispersa_inserere(NULL, c, NULL), tabula_dispersa_invenire(NULL, c, &v), tabula_dispersa_continet(NULL, c),
        tabula_dispersa_invenire_literis(NULL, "a", &v), tabula_dispersa_delere(NULL, c), tabula_dispersa_numerus(NULL),
        tabula_dispersa_factor_oneris(NULL));
    tabula_dispersa_vacare(NULL);
    it = tabula_dispersa_iterator_initium(NULL);
    printf("iterator NIHIL -> %d; proximum(NIHIL) -> %d\n", tabula_dispersa_iterator_proximum(&it, &c, &v), tabula_dispersa_iterator_proximum(NULL, &c, &v));
    (void)r;
    return 0;
}
