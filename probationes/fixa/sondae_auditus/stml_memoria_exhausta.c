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

static char doc[200000];

static void probare_casum(const char* tag, memoriae_index mag, int n_elem)
{
    pid_t pid;
    int st;
    pid = fork();
    if (pid == 0)
    {
        Piscina* pp;
        InternamentumChorda* it;
        StmlResultus r;
        chorda in;
        int i, pos = 0;
        alarm(5);
        pos += sprintf(doc + pos, "<r>");
        for (i = 0; i < n_elem; i++) pos += sprintf(doc + pos, "<e%d k=\"v%d\">t%d</e%d>", i, i, i, i);
        pos += sprintf(doc + pos, "</r>");
        pp = piscina_generare_certae_magnitudinis("sonda", mag);
        it = internamentum_creare(pp);
        in.datum = (i8*)doc; in.mensura = (i32)pos;
        r = stml_legere(in, pp, it);
        printf("%s\tCHILD_RETURNED\tstatus=%d\tsucc=%d\tradix=%p\n", tag, (int)r.status, (int)r.successus, (void*)r.radix);
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
    if (WIFSIGNALED(st))
        printf("%s\tmag=%lu\tn=%d\tCHILD_SIGNAL=%d%s\n", tag, (unsigned long)mag, n_elem, WTERMSIG(st), WTERMSIG(st) == SIGALRM ? " (HANG>5s)" : "");
    else
        printf("%s\tmag=%lu\tn=%d\tCHILD_EXIT=%d\n", tag, (unsigned long)mag, n_elem, WEXITSTATUS(st));
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    probare_casum("SATIS", 1u << 20, 50);
    probare_casum("ANGUSTA_8K", 8192, 500);
    probare_casum("ANGUSTA_64K", 65536, 3000);
    return 0;
}
