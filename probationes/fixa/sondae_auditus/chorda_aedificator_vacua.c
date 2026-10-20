#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

static void hexdump(const char* tag, chorda s)
{
    i32 k;
    printf("%s\tlen=%u\thex=", tag, s.mensura);
    for (k = 0; k < s.mensura; k++) printf("%02x", (unsigned)(unsigned char)s.datum[k]);
    printf("\n");
}

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    ChordaAedificator* a;
    chorda vac, mal, plena, nul;
    i8 dat[5];
    b32 r;
    dat[0]='a'; dat[1]='\0'; dat[2]=(i8)0xFF; dat[3]=(i8)0x80; dat[4]='z';

    a = chorda_aedificator_creare(pp, 16);
    vac.datum = NIHIL; vac.mensura = 0;
    r = chorda_aedificator_appendere_chorda(a, vac);
    printf("CHORDA_NIHIL_0\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    r = chorda_aedificator_appendere_evasus_json(a, vac);
    printf("JSON_NIHIL_0\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    nul.datum = dat; nul.mensura = 0;
    r = chorda_aedificator_appendere_chorda(a, nul);
    printf("CHORDA_PTR_0\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    mal.datum = NIHIL; mal.mensura = 5;
    r = chorda_aedificator_appendere_chorda(a, mal);
    printf("CHORDA_NIHIL_5\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    r = chorda_aedificator_appendere_evasus_json(a, mal);
    printf("JSON_NIHIL_5\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    r = chorda_aedificator_appendere_literis(a, "");
    printf("LITERIS_VACUA\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    r = chorda_aedificator_appendere_literis(a, NIHIL);
    printf("LITERIS_NIHIL\tr=%d\n", (int)r);
    r = chorda_aedificator_appendere_literis_evasus_json(a, "");
    printf("LIT_JSON_VACUA\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    r = chorda_aedificator_appendere_literis_evasus_json(a, NIHIL);
    printf("LIT_JSON_NIHIL\tr=%d\n", (int)r);
    r = chorda_aedificator_appendere_chorda(NIHIL, nul);
    printf("NIHIL_AED_CHORDA\tr=%d\n", (int)r);
    r = chorda_aedificator_appendere_character(NIHIL, 'x');
    printf("NIHIL_AED_CHAR\tr=%d\n", (int)r);
    r = chorda_aedificator_appendere_s32(NIHIL, 1);
    printf("NIHIL_AED_S32\tr=%d\n", (int)r);
    r = chorda_aedificator_appendere_repetita(NIHIL, 'x', 0);
    printf("NIHIL_AED_REP0\tr=%d\n", (int)r);

    plena.datum = dat; plena.mensura = 5;
    r = chorda_aedificator_appendere_chorda(a, plena);
    printf("CHORDA_NUL_HIGH\tr=%d\n", (int)r);
    r = chorda_aedificator_appendere_character(a, '\0');
    r = r && chorda_aedificator_appendere_character(a, (character)0xE9);
    printf("CHAR_NUL_E9\tr=%d\n", (int)r);
    r = chorda_aedificator_appendere_repetita(a, '\0', 3);
    printf("REP_NUL3\tr=%d\n", (int)r);
    hexdump("CONTENT", chorda_aedificator_spectare(a));

    /* creare edges */
    printf("CREARE_0\t%p\n", (void*)chorda_aedificator_creare(pp, 0));
    printf("CREARE_NIHILP\t%p\n", (void*)chorda_aedificator_creare(NIHIL, 16));
    a = chorda_aedificator_creare(pp, 1);
    {
        i32 k; b32 ok = VERUM;
        for (k = 0; k < 1000; k++) ok = ok && chorda_aedificator_appendere_character(a, (character)('a' + k % 26));
        {
            chorda s = chorda_aedificator_spectare(a);
            b32 good = s.mensura == 1000;
            for (k = 0; k < 1000 && good; k++) good = s.datum[k] == (i8)('a' + k % 26);
            printf("CAP1_1000\tok=%d\tgood=%d\n", (int)ok, (int)good);
        }
    }
    /* spectare on fresh */
    a = chorda_aedificator_creare(pp, 8);
    {
        chorda s = chorda_aedificator_spectare(a);
        printf("SPECT_FRESH\tlen=%u\tdatum_nihil=%d\tgradus=%u\n", s.mensura, s.datum == NIHIL, chorda_aedificator_indentatio_gradus(a));
        s = chorda_aedificator_finire(a);
        printf("FINIRE_FRESH\tlen=%u\tdatum_nihil=%d\n", s.mensura, s.datum == NIHIL);
    }
    {
        chorda s = chorda_aedificator_spectare(NIHIL);
        chorda f = chorda_aedificator_finire(NIHIL);
        chorda_aedificator_destruere(NIHIL);
        printf("NIHIL_SPECT\t%u %d\tFIN %u %d\tLONG %lu\tGRAD %u\n", s.mensura, s.datum==NIHIL, f.mensura, f.datum==NIHIL, (unsigned long)chorda_aedificator_longitudo(NIHIL), chorda_aedificator_indentatio_gradus(NIHIL));
    }
    return 0;
}
