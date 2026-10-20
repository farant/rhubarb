#include "postulata_posix.h"
#include "latina.h"
#include "friatio.h"
#include <stdio.h>
#include <sys/mman.h>
int main(void)
{
    SHA1Contextus a; SHA256Contextus b; unsigned char d[32]; int q;
    const i8* p = (const i8*)mmap(NULL, 0xFFFFFFFFUL, PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);
    sha1_initiare(&a); sha1_addere(&a, (const i8*)"a", 1);
    sha1_addere(&a, p, 0x80000000U); sha1_addere(&a, p + 0x80000000UL, 0x7FFFFFFFU);
    sha1_finire(&a, d); for (q = 0; q < 20; q++) printf("%02x", d[q]); printf("\n");
    sha256_initiare(&b); sha256_addere(&b, (const i8*)"a", 1);
    sha256_addere(&b, p, 0x80000000U); sha256_addere(&b, p + 0x80000000UL, 0x7FFFFFFFU);
    sha256_finire(&b, d); for (q = 0; q < 32; q++) printf("%02x", d[q]); printf("\n");
    return 0;
}
