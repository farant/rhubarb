#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda c; char* big; i8* ar; unsigned int k; struct stat st;
    const char* real_dir = "build/sonda";
    /* 1. embedded NUL: "build/sonda\0nonexistent_garbage" */
    c.datum = (i8*)"build/sonda\0nonexistent_garbage_zzz"; c.mensura = 35;
    printf("existit('build/sonda\\0nonexistent_garbage_zzz') = %d (expected 0: no such path)\n", (int)via_existit(c));
    c.datum = (i8*)"/etc/hosts\0/nope"; c.mensura = 16;
    printf("existit('/etc/hosts\\0/nope') = %d\n", (int)via_existit(c));
    c.datum = (i8*)"\0anything"; c.mensura = 9;
    printf("existit('\\0anything') = %d\n", (int)via_existit(c));
    /* 2. not NUL-terminated inside arena: "build/sondaXYZ" with mensura 11 */
    ar = (i8*)piscina_allocare(p, 32);
    memcpy(ar, "build/sondaXYZ", 14);
    c.datum = ar; c.mensura = 11;
    printf("existit(slice 'build/sonda' of 'build/sondaXYZ') = %d (expected 1)\n", (int)via_existit(c));
    /* 3. empty / NIHIL */
    c.datum = NIHIL; c.mensura = 0; printf("existit(NIHIL) = %d\n", (int)via_existit(c));
    c = chorda_ex_literis("", p); printf("existit('') = %d\n", (int)via_existit(c));
    /* 4. long existing path made of "./" repeats (> 1024, < 4096, and >= 4096) */
    big = (char*)malloc(9000);
    {
        unsigned int lens[3] = { 1100, 4094, 4200 };
        int j;
        for (j = 0; j < 3; j++) {
            unsigned int lnum = lens[j];
            for (k = 0; k + 2 <= lnum - 11; k += 2) { big[k] = '.'; big[k+1] = '/'; }
            memcpy(big + k, real_dir, 11); k += 11;
            big[k] = 0;
            c.datum = (i8*)big; c.mensura = (i32)k;
            printf("existit(len %u './'*n + build/sonda) = %d ; stat()=%d\n", k, (int)via_existit(c), stat(big, &st));
        }
    }
    /* 5. directory and dangling symlink */
    unlink("build/sonda/via_existit_casus/pendens");
    symlink("/nonexistent/target/zz", "build/sonda/via_existit_casus/pendens");
    c = chorda_ex_literis("build/sonda/via_existit_casus/pendens", p);
    printf("existit(dangling symlink) = %d (lstat=%d)\n", (int)via_existit(c), lstat("build/sonda/via_existit_casus/pendens", &st));
    c = chorda_ex_literis("/", p);
    printf("existit('/') = %d\n", (int)via_existit(c));
    free(big);
    piscina_destruere(p);
    return 0;
}
