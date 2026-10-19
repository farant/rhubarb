/* runae_columnae: runae_columnam_quaerere casus limitis + NIHIL */
#include "latina.h"
#include "runae.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static void quaere(const char* tit, const char* b, int n, unsigned c, int exp_off, unsigned exp_col)
{
    i8* buf = (i8*)malloc((size_t)(n ? n : 1));
    const i8* r; i32 col = 999;
    memcpy(buf, b, (size_t)n);
    r = runae_columnam_quaerere(buf, buf + n, RUNAE_POLITICA_GRAPHEMATUM, c, &col);
    printf("%-28s col %u -> off %d col %u (exp %d %u) %s\n", tit, c, (int)(r - buf), (unsigned)col,
        exp_off, exp_col, ((int)(r - buf) == exp_off && (unsigned)col == exp_col) ? "ok" : "DISCORDIA");
    free(buf);
}

static void alarma(int s) { (void)s; printf("PENDET: latitudo_textus(NIHIL, finis) non redit post 2 s\n"); fflush(stdout); _exit(3); }

int main(void)
{
    quaere("ZWSP a, col 0", "\xE2\x80\x8B" "a", 4, 0, 3, 0);
    quaere("C1 a, col 0", "\xC2\x85" "a", 3, 0, 2, 0);
    quaere("a ZWSP, col 1", "a\xE2\x80\x8B", 4, 1, 4, 1);
    quaere("a 0301 0301 (one), col 1", "a\xCC\x81\xCC\x81", 5, 1, 5, 1);
    quaere("zhong, col 1", "\xE4\xB8\xAD", 3, 1, 0, 0);
    quaere("zhong a, col 2", "\xE4\xB8\xAD" "a", 4, 2, 3, 2);
    quaere("vacuum, col 0", "", 0, 0, 0, 0);
    quaere("abc, col FFFFFFFF", "abc", 3, 0xFFFFFFFFu, 3, 3);
    quaere("abc, col 3", "abc", 3, 3, 3, 3);
    quaere("NUL a, col 0", "\x00" "a", 2, 0, 0, 0);
    quaere("NUL a, col 1", "\x00" "a", 2, 1, 1, 1);
    quaere("CR LF a, col 1", "\r\na", 3, 1, 1, 1);
    quaere("C0AF a, col 1", "\xC0\xAF" "a", 3, 1, 1, 1);
    quaere("0301 lone, col 0", "\xCC\x81", 2, 0, 2, 0);
    {
        static const i8 t[3] = { 'a', 'b', 'c' };
        const i8* r; i32 col = 999, lat = 999;
        r = runae_columnam_quaerere(t + 2, t, RUNAE_POLITICA_GRAPHEMATUM, 5, &col);
        printf("inversum: off-from-initium %d col %u\n", (int)(r - (t + 2)), (unsigned)col);
        printf("latitudo_textus inversum %u\n", (unsigned)runae_latitudo_textus(t + 2, t, RUNAE_POLITICA_GRAPHEMATUM));
        r = runae_graphema_proximum(t + 2, t, &lat);
        printf("graphema inversum %d lat %u\n", (int)(r - (t + 2)), (unsigned)lat);
        r = runae_unitas_proxima(NIHIL, NIHIL, RUNAE_POLITICA_GRAPHEMATUM, &lat);
        printf("unitas(NIHIL,NIHIL) %p lat %u\n", (const void*)r, (unsigned)lat);
        r = runae_graphema_proximum(NIHIL, t + 3, &lat);
        printf("graphema(NIHIL, t+3) -> %s lat %u\n", r == NIHIL ? "NIHIL (nullus progressus)" : "alius", (unsigned)lat);
        r = runae_unitas_proxima(NIHIL, t + 3, RUNAE_POLITICA_GRAPHEMATUM, &lat);
        printf("unitas(NIHIL, t+3) -> %s lat %u\n", r == NIHIL ? "NIHIL (nullus progressus)" : "alius", (unsigned)lat);
        fflush(stdout);
        signal(SIGALRM, alarma);
        alarm(2);
        printf("latitudo_textus(NIHIL, t+3) = %u\n", (unsigned)runae_latitudo_textus(NIHIL, t + 3, RUNAE_POLITICA_GRAPHEMATUM));
        alarm(0);
    }
    return 0;
}
