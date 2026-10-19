#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>

static void hexa(const char* t, const unsigned char* d, unsigned int k)
{ unsigned int q; printf("%s ", t); for (q = 0; q < k; q++) printf("%02x", d[q]); printf("\n"); }

int main(void)
{
    unsigned char d1[20], d2[32];
    chorda vac;
    SHA1Contextus c1; SHA256Contextus c2;
    vac.mensura = 0; vac.datum = NIHIL;
    printf("-- sha1_friare_chorda {0,NIHIL}\n");
    sha1_friare_chorda(vac, d1);   hexa("sha1_vac", d1, 20);
    printf("-- sha256_friare_chorda {0,NIHIL}\n");
    sha256_friare_chorda(vac, d2); hexa("sha256_vac", d2, 32);
    printf("-- addere(NIHIL,0) inter data\n");
    sha1_initiare(&c1); sha1_addere(&c1, (const i8*)"ab", 2); sha1_addere(&c1, NIHIL, 0);
    sha1_addere(&c1, (const i8*)"c", 1); sha1_finire(&c1, d1); hexa("sha1_abc", d1, 20);
    sha256_initiare(&c2); sha256_addere(&c2, NIHIL, 0); sha256_addere(&c2, (const i8*)"abc", 3);
    sha256_finire(&c2, d2); hexa("sha256_abc", d2, 32);
    printf("-- crc32 NIHIL\n");
    printf("crc_nihil %08x\n", crc32_calculare(NIHIL, 0));
    printf("crc_chorda_nihil %08x\n", crc32_calculare_chorda(vac));
    return 0;
}
