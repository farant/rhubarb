/* Named classes for numerus_romanus_legere. One line per case:
 * <label> <VERUM|FALSUM> valor=<v> (valor preset to 777777 to see writes) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void unum(const char* label, const char* bytes, unsigned int len)
{
    chorda s; i32 val = 777777; b32 r;
    s.datum = (i8*)bytes; s.mensura = (i32)len;
    r = numerus_romanus_legere(s, &val);
    printf("%-22s %s valor=%u\n", label, r ? "VERUM" : "FALSUM", val);
}
static void str(const char* x) { unum(x, x, (unsigned int)strlen(x)); }

int main(void)
{
    static const char* words[] = {"DID","MILD","CIVIC","VIM","DIM","LIVID","MIMIC",
        "did","mild","civic","vim","dim","livid","mimic","MIX","mix", NULL};
    static const char* pairs[] = {"IL","IC","ID","IM","VX","VL","VC","VD","VM","XD","XM",
        "LC","LD","LM","DM", NULL};
    static const char* reps[] = {"IIII","XXXX","CCCC","MMMM","VV","LL","DD","III","XXX","CCC","MMM", NULL};
    static const char* order[] = {"IIV","IIX","XXL","XXC","CCD","CCM","IXI","IXV","IVI","VIV",
        "XCX","XCL","XLX","CMC","CMD","CDC","IXIV","XCXL","CMCD","VIX","LXC","DCM","IVX", NULL};
    static const char* casus_[] = {"Xii","iV","Mcm","mCM","xii","mcmxcix","MCMXCIX","i","v","x","l","c","d","m",
        "I","V","X","L","C","D","M","A","N","n","0","iiij","j", " XIV","XIV.","XIV ", NULL};
    static const char* maxs[] = {"MMMCMXCIX","MMMDCCCLXXXVIII","MMMDCM","MMMCMLXC","MMMDCMLXCVIX",
        "mmmdcmlxcvix","MMMDCMLXCVIV", NULL};
    int k;
    char* big;
    chorda s;
    i32 val;

    for (k = 0; words[k]; k++) str(words[k]);
    for (k = 0; pairs[k]; k++) str(pairs[k]);
    for (k = 0; reps[k]; k++) str(reps[k]);
    for (k = 0; order[k]; k++) str(order[k]);
    for (k = 0; casus_[k]; k++) str(casus_[k]);
    for (k = 0; maxs[k]; k++) str(maxs[k]);
    unum("xii-newline", "xii\n", 4);
    /* empty */
    unum("empty-nonNIHIL", "X", 0);
    s.datum = NIHIL; s.mensura = 0; val = 777777;
    printf("%-22s %s valor=%u\n", "empty-NIHIL", numerus_romanus_legere(s, &val) ? "VERUM":"FALSUM", val);
    s.datum = NIHIL; s.mensura = 5; val = 777777;
    printf("%-22s %s valor=%u\n", "NIHIL-mensura5", numerus_romanus_legere(s, &val) ? "VERUM":"FALSUM", val);
    /* valor NIHIL */
    s.datum = (i8*)"XIV"; s.mensura = 3;
    printf("%-22s %s\n", "valorNIHIL-XIV", numerus_romanus_legere(s, NIHIL) ? "VERUM":"FALSUM");
    /* slice: copy into exact-size heap buffer so ASan sees an overread */
    {
        char* h = (char*)malloc(2); h[0]='X'; h[1]='I';
        unum("slice XI of XIV(heap2)", h, 2);
        free(h);
        h = (char*)malloc(3); memcpy(h, "MMM", 3);
        unum("heap MMM exact", h, 3);
        free(h);
        h = (char*)malloc(1); h[0]='I';
        unum("heap I exact", h, 1);
        free(h);
        h = (char*)malloc(2); memcpy(h, "IV", 2);
        unum("heap IV exact", h, 2);
        free(h);
    }
    unum("slice XI from XIV", "XIV", 2);
    unum("slice MM from MMMM", "MMMM", 2);
    unum("X NUL I", "X\0I", 3);
    unum("X NUL", "X\0", 2);
    unum("NUL", "\0", 1);
    unum("0x80", "\x80", 1);
    unum("0xFF", "\xFF", 1);
    unum("Latin1 0xCC", "\xCC", 1);
    unum("U+216B", "\xE2\x85\xAB", 3);
    unum("X 0xC9", "X\xC9", 2);
    /* long input */
    big = (char*)malloc(100000);
    memset(big, 'M', 100000);
    unum("100000 M", big, 100000);
    memset(big, 'I', 100000);
    unum("100000 I", big, 100000);
    memset(big, 'i', 100000);
    unum("100000 i", big, 100000);
    free(big);
    return 0;
}
