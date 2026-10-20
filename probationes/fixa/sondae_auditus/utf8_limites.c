/* utf8: bounds - nothing read at/after finis or before initium; NIHIL; inverted; negative mensura */
#include "latina.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static unsigned char* exacta(const unsigned char* s, int n)
{ unsigned char* b = (unsigned char*)malloc((size_t)n); memcpy(b, s, (size_t)n); return b; }

int main(void)
{
    static const unsigned char t1[] = { 0xF0, 0x9F, 0x98 };
    static const unsigned char t2[] = { 0xE2 };
    static const unsigned char t3[] = { 0x41, 0xF0 };
    static const unsigned char t4[] = { 0x80, 0x80, 0x80 };
    static const unsigned char t5[] = { 0x80, 0x80, 0x80, 0x80, 0x80 };
    unsigned char* b; const i8* p; s32 r;

    b = exacta(t1, 3); p = b; r = utf8_decodere(&p, b + 3);
    printf("dec F0 9F 98|fin: r=%d adv=%ld\n", r, (long)(p - b));
    printf("prox F0 9F 98|fin: adv=%ld\n", (long)(utf8_proxima_runa(b, b + 3) - b));
    printf("numerare F0 9F (m=2): %d\n", utf8_numerare_runas(b, 2));
    free(b);
    b = exacta(t2, 1); p = b; r = utf8_decodere(&p, b + 1);
    printf("dec E2|fin: r=%d adv=%ld  numerare=%d\n", r, (long)(p - b), utf8_numerare_runas(b, 1)); free(b);
    b = exacta(t3, 2);
    printf("prox F0@fin-1: adv_from_1=%ld\n", (long)(utf8_proxima_runa(b + 1, b + 2) - (b + 1)));
    printf("prox ptr==fin: same=%d\n", utf8_proxima_runa(b + 2, b + 2) == b + 2);
    printf("prox ptr>fin: ret==ptr %d\n", utf8_proxima_runa(b + 2, b + 1) == b + 2);
    free(b);
    b = exacta(t4, 3);
    printf("prior 80 80 80|ptr: ret_off=%ld\n", (long)(utf8_prior_runa(b + 3, b) - b));
    printf("prior ptr==initium: same=%d\n", utf8_prior_runa(b, b) == b);
    printf("prior ptr<initium: ret==initium %d\n", utf8_prior_runa(b, b + 1) == b + 1);
    printf("prior NIHIL ptr: ret==initium %d\n", utf8_prior_runa(NIHIL, b) == b);
    free(b);
    b = exacta(t5, 5);
    printf("prior 80x5|ptr: ret_off=%ld\n", (long)(utf8_prior_runa(b + 5, b) - b));
    free(b);
    /* decodere NIHIL / inverted */
    b = exacta(t3, 2);
    printf("dec NIHIL ptr: %d\n", utf8_decodere(NIHIL, b + 2));
    p = NIHIL; printf("dec NIHIL *ptr: %d\n", utf8_decodere(&p, b + 2));
    p = b; r = utf8_decodere(&p, NIHIL); printf("dec NIHIL finis: r=%d adv=%ld\n", r, (long)(p - b));
    p = b + 2; r = utf8_decodere(&p, b); printf("dec inverted: r=%d moved=%ld\n", r, (long)(p - (b + 2)));
    p = b + 2; r = utf8_decodere(&p, b + 2); printf("dec empty: r=%d moved=%ld\n", r, (long)(p - (b + 2)));
    printf("prox NIHIL: %d\n", utf8_proxima_runa(NIHIL, b) == NIHIL);
    printf("numerare m=-1: %d  m=S32_MIN: %d  NIHIL,5: %d  m=0: %d\n",
        utf8_numerare_runas(b, -1), utf8_numerare_runas(b, INT_MIN), utf8_numerare_runas(NIHIL, 5),
        utf8_numerare_runas(b, 0));
    free(b);
    return 0;
}
