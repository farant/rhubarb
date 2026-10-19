#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* modus 0: <a>^n </a>^n legere; 1: legere+scribere; 2: catena capturarum '<a(> '^n x; 3: fragmenta <#>^n</#>^n; 4: duplicare */
static void probare_casum(const char* tag, int modus, int n)
{
    pid_t pid; int st;
    pid = fork();
    if (pid == 0)
    {
        Piscina* pp; InternamentumChorda* it; StmlResultus r; chorda in, w;
        char* buf; int i, pos = 0;
        alarm(20);
        buf = (char*)malloc((size_t)n * 8 + 16);
        if (modus == 2) { for (i = 0; i < n; i++) { memcpy(buf + pos, "<a(> ", 5); pos += 5; } buf[pos++] = 'x'; }
        else if (modus == 3) { for (i = 0; i < n; i++) { memcpy(buf + pos, "<#>", 3); pos += 3; } for (i = 0; i < n; i++) { memcpy(buf + pos, "</#>", 4); pos += 4; } }
        else { for (i = 0; i < n; i++) { memcpy(buf + pos, "<a>", 3); pos += 3; } for (i = 0; i < n; i++) { memcpy(buf + pos, "</a>", 4); pos += 4; } }
        pp = piscina_generare_dynamicum("sonda", 1 << 20);
        it = internamentum_creare(pp);
        in.datum = (i8*)buf; in.mensura = (i32)pos;
        r = stml_legere(in, pp, it);
        printf("%s\tLEGERE_REDDIT status=%d\n", tag, (int)r.status);
        if (modus == 1 || modus == 2) { w = stml_scribere(r.radix, pp, FALSUM); printf("%s\tSCRIBERE_REDDIT mensura=%u (in %u)\n", tag, w.mensura, in.mensura); }
        if (modus == 4) { StmlNodus* d = stml_duplicare(r.radix, pp, it); printf("%s\tDUPLICARE_REDDIT %p\n", tag, (void*)d); }
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
    if (WIFSIGNALED(st)) printf("%s\tn=%d\tSIGNUM=%d%s\n", tag, n, WTERMSIG(st), WTERMSIG(st) == SIGALRM ? " (HANG)" : "");
    else printf("%s\tn=%d\tEXIT=%d\n", tag, n, WEXITSTATUS(st));
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    probare_casum("ELEM_1e3", 0, 1000);
    probare_casum("ELEM_1e4", 0, 10000);
    probare_casum("ELEM_1e5", 0, 100000);
    probare_casum("ELEM_SCRIB_1e4", 1, 10000);
    probare_casum("CAPT_1e3", 2, 1000);
    probare_casum("CAPT_1e4", 2, 10000);
    probare_casum("CAPT_1e5", 2, 100000);
    probare_casum("FRAG_1e5", 3, 100000);
    probare_casum("DUP_1e4", 4, 10000);
    return 0;
}
