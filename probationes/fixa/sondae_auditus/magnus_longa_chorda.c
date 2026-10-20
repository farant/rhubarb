/* sonda: magnus_ex_chorda / magnus_ad_chordam super textus longos (ex
 * plagula argv[1], linea per numerum) - itus reditusque, bitorum,
 * residuum_parvum mod 2^32-1 et 10^9+7. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    FILE* f;
    char* buf;
    long cap = 4000000;
    Piscina* p = piscina_generare_dynamicum("sonda", 1 << 20);
    if (argc < 2) return 2;
    f = fopen(argv[1], "r");
    if (!f) return 2;
    buf = (char*)malloc((size_t)cap);
    while (fgets(buf, (int)cap, f)) {
        size_t len_ = strlen(buf);
        Magnus m;
        chorda c;
        if (len_ && buf[len_ - 1] == '\n') buf[--len_] = 0;
        if (!magnus_ex_chorda(chorda_ex_buffer((i8*)buf, (i32)len_), p, &m)) {
            printf("PARSEFAIL\n");
            continue;
        }
        c = magnus_ad_chordam(m, p);
        printf("len=%lu same=%d bits=%u rp1=%u rp2=%u outlen=%u\n", (unsigned long)len_,
            (size_t)c.mensura == len_ && memcmp(c.datum, buf, len_) == 0,
            (unsigned)magnus_bitorum(m),
            (unsigned)magnus_residuum_parvum(m, 0xFFFFFFFFU),
            (unsigned)magnus_residuum_parvum(m, 1000000007U), (unsigned)c.mensura);
        fflush(stdout);
    }
    free(buf);
    fclose(f);
    piscina_destruere(p);
    return 0;
}
