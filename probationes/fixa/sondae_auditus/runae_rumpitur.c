/* runae_rumpitur: casus UAX #29 manu (exspectatio: '÷' = 1, '×' = 0) */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <string.h>

typedef struct { const char* tit; int n; long r[8]; const char* exp; } Cas;

#define RI 0x1F1E6
#define EP 0x1F468
#define ZJ 0x200D
#define KA 0x0915
#define TA 0x0924
#define VR 0x094D
#define NK 0x093C
static const Cas CASUS_T[] = {
  {"RI x5", 5, {RI,RI,RI,RI,RI}, "0101"},
  {"RI a RI RI", 4, {RI,'a',RI,RI}, "110"},
  {"RI Ext RI", 3, {RI,0x0308,RI}, "01"},
  {"EP Ext ZWJ EP", 4, {EP,0x0308,ZJ,EP}, "000"},
  {"ZWJ EP", 2, {ZJ,EP}, "1"},
  {"a ZWJ EP", 3, {'a',ZJ,EP}, "01"},
  {"EP ZWJ ZWJ EP", 4, {EP,ZJ,ZJ,EP}, "001"},
  {"EP Ext a ZWJ EP", 5, {EP,0x0308,'a',ZJ,EP}, "1001"},
  {"EP ZWJ EP ZWJ EP", 5, {EP,ZJ,EP,ZJ,EP}, "0000"},
  {"EP Linker ZWJ EP", 4, {EP,VR,ZJ,EP}, "000"},
  {"EP Mod ZWJ EP", 4, {EP,0x1F3FB,ZJ,EP}, "000"},
  {"EP Mc ZWJ EP", 4, {EP,0x093F,ZJ,EP}, "001"},
  {"KA VR TA", 3, {KA,VR,TA}, "00"},
  {"VR KA", 2, {VR,KA}, "1"},
  {"a VR KA", 3, {'a',VR,KA}, "01"},
  {"KA NK KA", 3, {KA,NK,KA}, "01"},
  {"KA VR ZWJ TA", 4, {KA,VR,ZJ,TA}, "000"},
  {"KA VR NK VR TA", 5, {KA,VR,NK,VR,TA}, "0000"},
  {"KA NK VR TA", 4, {KA,NK,VR,TA}, "000"},
  {"KA VR VR TA", 4, {KA,VR,VR,TA}, "000"},
  {"KA VR Mc TA", 4, {KA,VR,0x093F,TA}, "001"},
  {"KA VR 0300 TA", 4, {KA,VR,0x0300,TA}, "000"},
  {"KA 0301(Ext none?) VR TA", 4, {KA,0x0301,VR,TA}, "000"},
  {"Tamil KA VR SSA", 3, {0x0B95,0x0BCD,0x0BB7}, "01"},
  {"KA VR TA VR KA", 5, {KA,VR,TA,VR,KA}, "0000"},
  {"CR LF", 2, {13,10}, "1"},
  {"LF CR", 2, {10,13}, "1"},
  {"CR Ext", 2, {13,0x0308}, "1"},
  {"Prepend LF", 2, {0x0600,10}, "1"},
  {"Prepend a", 2, {0x0600,'a'}, "0"},
  {"Prepend Ext", 2, {0x0600,0x0308}, "0"},
  {"Ctrl Ext", 2, {1,0x0308}, "1"},
  {"L V T", 3, {0x1100,0x1161,0x11A8}, "00"},
  {"L LV", 2, {0x1100,0xAC00}, "0"},
  {"LV V T", 3, {0xAC00,0x1161,0x11A8}, "00"},
  {"LVT T", 2, {0xAC01,0x11A8}, "0"},
  {"LVT V", 2, {0xAC01,0x1161}, "1"},
  {"T L", 2, {0x11A8,0x1100}, "1"},
  {"V L", 2, {0x1161,0x1100}, "1"},
  {"L D7B0(V ext)", 2, {0x1100,0xD7B0}, "0"},
  {"-1 a", 2, {-1,'a'}, "1"},
  {"a -1", 2, {'a',-1}, "1"},
  {"-1 Ext", 2, {-1,0x0308}, "1"},
  {"110000 Ext", 2, {0x110000,0x0308}, "1"},
  {"D800 Ext", 2, {0xD800,0x0308}, "1"},
  {"a D800", 2, {'a',0xD800}, "1"},
  {"a DFFF", 2, {'a',0xDFFF}, "1"},
  {"a ZWSP", 2, {'a',0x200B}, "1"},
  {"a ZWNJ", 2, {'a',0x200C}, "0"},
  {"a SPACE", 2, {'a',' '}, "1"},
  {"a FFFE", 2, {'a',0xFFFE}, "1"},
  {"a E0001(tag)", 2, {'a',0xE0001}, "1"},
  {"a E0020(tag)", 2, {'a',0xE0020}, "0"},
  {"a E0080(DI unassigned)", 2, {'a',0xE0080}, "1"},
  {"a E01F0 (Cn GCB?)", 2, {'a',0xE01F0}, "1"},
  {"INT_MIN a", 2, {(long)(-2147483647L-1),'a'}, "1"},
};

int main(void)
{
    int k, j, mala = 0;
    for (k = 0; k < (int)(sizeof(CASUS_T)/sizeof(CASUS_T[0])); k++) {
        RunaeRuptura rr;
        char obt[16];
        runae_rupturam_initiare(&rr);
        for (j = 1; j < CASUS_T[k].n; j++)
            obt[j-1] = runae_rumpitur((s32)CASUS_T[k].r[j-1], (s32)CASUS_T[k].r[j], &rr) ? '1' : '0';
        obt[CASUS_T[k].n - 1] = 0;
        printf("%-26s exp %-6s obt %-6s %s\n", CASUS_T[k].tit, CASUS_T[k].exp, obt,
            strcmp(obt, CASUS_T[k].exp) ? "DISCORDIA" : "ok");
        if (strcmp(obt, CASUS_T[k].exp)) mala++;
    }
    /* status mendax */
    {
        RunaeRuptura rr; int a1, a2, a3;
        rr.status = 0xFFFFFFFFu;
        a1 = runae_rumpitur('a', 'b', &rr);
        printf("status FFFFFFFF: a|b %d status post %08X\n", a1, (unsigned)rr.status);
        rr.status = 0xFFFFFFFFu;
        a2 = runae_rumpitur(RI, RI, &rr);
        printf("status FFFFFFFF: RI|RI %d status post %08X\n", a2, (unsigned)rr.status);
        rr.status = 0xFFFFFFFFu;
        a3 = runae_rumpitur(ZJ, EP, &rr);
        printf("status FFFFFFFF: ZWJ|EP %d status post %08X\n", a3, (unsigned)rr.status);
        rr.status = 0xFFFFFFFFu;
        a3 = runae_rumpitur(VR, KA, &rr);
        printf("status FFFFFFFF: VR|KA %d status post %08X\n", a3, (unsigned)rr.status);
        /* par bis vocatum */
        runae_rupturam_initiare(&rr);
        a1 = runae_rumpitur(RI, RI, &rr);
        a2 = runae_rumpitur(RI, RI, &rr);
        printf("RI RI bis (idem par): %d %d\n", a1, a2);
    }
    printf("discordiae %d\n", mala);
    return 0;
}
