#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>
#include <string.h>

static void hexa(const char* t, const unsigned char* d, unsigned int k)
{ unsigned int q; printf("%s ", t); for (q = 0; q < k; q++) printf("%02x", d[q]); printf("\n"); }

int main(void)
{
    unsigned char g1[20 + 32], g2[32 + 32], buf[64], ref[32];
    unsigned int q, ok;
    SHA1Contextus c1; SHA256Contextus c2, c3;
    /* custodes: digest in medio */
    memset(g1, 0xAB, sizeof g1); memset(g2, 0xAB, sizeof g2);
    sha1_friare((const i8*)"abc", 3, g1 + 16);
    sha256_friare((const i8*)"abc", 3, g2 + 16);
    ok = 1;
    for (q = 0; q < 16; q++) if (g1[q] != 0xAB || g1[16 + 20 + q] != 0xAB) ok = 0;
    printf("custos_sha1 %s\n", ok ? "intacti" : "LAESI");
    ok = 1;
    for (q = 0; q < 16; q++) if (g2[q] != 0xAB || g2[16 + 32 + q] != 0xAB) ok = 0;
    printf("custos_sha256 %s\n", ok ? "intacti" : "LAESI");
    /* aliasing: digest supra data */
    for (q = 0; q < 64; q++) buf[q] = (unsigned char)(q * 37 + 1);
    sha256_friare(buf, 64, ref);
    sha256_friare(buf, 64, buf);   hexa("alias256", buf, 32); hexa("ref256  ", ref, 32);
    for (q = 0; q < 64; q++) buf[q] = (unsigned char)(q * 37 + 1);
    sha1_friare(buf, 64, ref);
    sha1_friare(buf, 64, buf);     hexa("alias1", buf, 20); hexa("ref1  ", ref, 20);
    /* digest in ctx->buffer */
    sha256_initiare(&c2); sha256_addere(&c2, (const i8*)"abc", 3); sha256_finire(&c2, c2.buffer);
    hexa("in_ctx_buffer256", c2.buffer, 32);
    /* re-initiare post usum cum residuo */
    sha256_initiare(&c2); sha256_addere(&c2, (const i8*)"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 70);
    sha256_initiare(&c2); sha256_addere(&c2, (const i8*)"abc", 3); sha256_finire(&c2, ref);
    hexa("reinit256", ref, 32);
    sha1_initiare(&c1); sha1_addere(&c1, (const i8*)"zzzzzzzzzzz", 11);
    sha1_initiare(&c1); sha1_addere(&c1, (const i8*)"abc", 3); sha1_finire(&c1, ref);
    hexa("reinit1", ref, 20);
    /* intercalati */
    sha256_initiare(&c2); sha256_initiare(&c3);
    sha256_addere(&c2, (const i8*)"a", 1); sha256_addere(&c3, (const i8*)"hello ", 6);
    sha256_addere(&c2, (const i8*)"bc", 2); sha256_addere(&c3, (const i8*)"world", 5);
    sha256_finire(&c3, ref); hexa("inter_hw", ref, 32);
    sha256_finire(&c2, ref); hexa("inter_abc", ref, 32);
    /* finire bis */
    sha256_initiare(&c2); sha256_addere(&c2, (const i8*)"abc", 3); sha256_finire(&c2, ref);
    sha256_finire(&c2, ref); hexa("finire_bis", ref, 32);
    return 0;
}
