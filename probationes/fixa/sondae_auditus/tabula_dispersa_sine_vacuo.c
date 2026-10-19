#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

/* hash = first byte (controlled slot), key = 2 bytes {slot, tag} */
static i32 fr(chorda c) { return c.mensura ? (i32)c.datum[0] : 0; }
static i32 fr_const(chorda c) { (void)c; return 0xFFFFFFF0u; }
static s32 cmp(chorda a, chorda b) { return chorda_comparare(a, b); }
static i8 kb[256][2];
static chorda K(int slot, int tag) { chorda c; kb[tag][0]=(i8)slot; kb[tag][1]=(i8)tag; c.datum=kb[tag]; c.mensura=2; return c; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaDispersa* t = tabula_dispersa_creare(p, 16, fr, cmp);
    int idx, vac; void* v; b32 r; i32 coll0;
    for (idx = 0; idx < 13; idx++) tabula_dispersa_inserere(t, K(idx, idx), (void*)(long)(idx+1));
    for (idx = 0; idx < 4; idx++) tabula_dispersa_delere(t, K(idx, idx));
    for (idx = 13; idx < 16; idx++) tabula_dispersa_inserere(t, K(idx, idx), (void*)(long)(idx+1));
    vac = 0; for (idx = 0; idx < (int)t->capacitas; idx++) if (t->sloti[idx].status == SLOT_VACUUM) vac++;
    printf("status: cap=%u numerus=%u del=%u VACUUM=%d\n", t->capacitas, t->numerus, t->numerus_deletorum, vac);
    coll0 = t->collisiones_totales;
    r = tabula_dispersa_invenire(t, K(5, 100), &v);
    printf("invenire absens -> %d (terminavit), collisiones+%u distantia_maxima=%u\n", r, t->collisiones_totales - coll0, t->distantia_maxima);
    for (idx = 4; idx < 16; idx++) { v = 0; r = tabula_dispersa_invenire(t, K(idx, idx), &v); if (!r || (long)v != idx+1) printf("  PERDITUM %d\n", idx); }
    r = tabula_dispersa_inserere(t, K(5, 101), (void*)77);
    printf("inserere novum in tabula sine vacuo -> %d numerus=%u del=%u cap=%u\n", r, t->numerus, t->numerus_deletorum, t->capacitas);
    v = 0; r = tabula_dispersa_invenire(t, K(5,101), &v); printf("  inventum %d %ld\n", r, (long)v);
    for (idx = 4; idx < 16; idx++) { v = 0; r = tabula_dispersa_invenire(t, K(idx, idx), &v); if (!r || (long)v != idx+1) printf("  PERDITUM %d\n", idx); }

    /* collision chain: A,B,C all hash 0 -> slots 0,1,2; delete A; reinsert B; delete B middle; find C */
    t = tabula_dispersa_creare(p, 16, fr, cmp);
    tabula_dispersa_inserere(t, K(0, 1), (void*)1);
    tabula_dispersa_inserere(t, K(0, 2), (void*)2);
    tabula_dispersa_inserere(t, K(0, 3), (void*)3);
    tabula_dispersa_delere(t, K(0, 1));
    tabula_dispersa_inserere(t, K(0, 2), (void*)22);
    printf("reinserere B post delere A: numerus=%u (exsp 2) del=%u\n", t->numerus, t->numerus_deletorum);
    v=0; tabula_dispersa_invenire(t, K(0,2), &v); printf("  B -> %ld (exsp 22)\n", (long)v);
    tabula_dispersa_delere(t, K(0, 2));
    v=0; r = tabula_dispersa_invenire(t, K(0,3), &v); printf("  C post delere medium -> %d %ld\n", r, (long)v);
    tabula_dispersa_inserere(t, K(0, 2), (void*)222);
    v=0; r = tabula_dispersa_invenire(t, K(0,2), &v); printf("  B reinserta -> %d %ld numerus=%u\n", r, (long)v, t->numerus);

    /* constant hash > capacity, 2000 keys through growth */
    {
        static char kk[2000][8]; chorda c; int bad = 0;
        t = tabula_dispersa_creare(p, 16, fr_const, cmp);
        for (idx = 0; idx < 2000; idx++) { sprintf(kk[idx], "%d", idx); c.datum=(i8*)kk[idx]; c.mensura=(i32)strlen(kk[idx]); tabula_dispersa_inserere(t, c, (void*)(long)(idx+1)); }
        for (idx = 0; idx < 2000; idx += 3) { c.datum=(i8*)kk[idx]; c.mensura=(i32)strlen(kk[idx]); tabula_dispersa_delere(t, c); }
        for (idx = 0; idx < 2000; idx++) { c.datum=(i8*)kk[idx]; c.mensura=(i32)strlen(kk[idx]); v=0; r=tabula_dispersa_invenire(t,c,&v); if ((idx%3==0) ? r : (!r || (long)v!=idx+1)) bad++; }
        printf("friatio constans 0xFFFFFFF0, 2000 claves, 667 deletae: errores=%d numerus=%u cap=%u distantia_maxima=%u\n", bad, t->numerus, t->capacitas, t->distantia_maxima);
    }
    return 0;
}
