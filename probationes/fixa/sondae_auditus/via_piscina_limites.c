#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static void scr(const char* t, chorda c)
{
    unsigned int k;
    printf("%s -> ", t);
    if (!c.datum) { printf("<NIHIL m=%u>\n", (unsigned)c.mensura); return; }
    printf("'"); for (k = 0; k < c.mensura; k++) putchar(c.datum[k]); printf("'\n");
}
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda a = chorda_ex_literis("foo/../bar", p);
    chorda b = chorda_ex_literis("x.txt", p);
    chorda t[2]; pid_t pid; int st;
    t[0] = a; t[1] = b;
    scr("normalizare(NIHIL piscina)", via_normalizare(a, NIHIL));
    scr("absoluta(NIHIL piscina)", via_absoluta(b, NIHIL));
    scr("directorium('x.txt', NIHIL)", via_directorium(b, NIHIL));
    scr("directorium('foo/../bar', NIHIL)", via_directorium(a, NIHIL));
    scr("nomen('x.txt', NIHIL)", via_nomen(b, NIHIL));
    scr("iungere(NIHIL piscina)", via_iungere(t, 2, NIHIL));
    scr("directorium_currens(NIHIL)", via_directorium_currens(NIHIL));
    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        Piscina* f = piscina_generare_certae_magnitudinis("certa", 64);
        chorda r = via_iungere(t, 2, f);
        printf("child: iungere in 64-byte fixed piscina returned mensura=%u datum=%p\n", (unsigned)r.mensura, (void*)r.datum);
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
    printf("fixed piscina child: exited=%d code=%d signaled=%d sig=%d\n", WIFEXITED(st), WIFEXITED(st) ? WEXITSTATUS(st) : -1, WIFSIGNALED(st), WIFSIGNALED(st) ? WTERMSIG(st) : 0);
    piscina_destruere(p);
    return 0;
}
