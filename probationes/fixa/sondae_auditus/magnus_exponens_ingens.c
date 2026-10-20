/* sonda: magnus_potentia(2, (i32)-1 = 2^32-1) - refutatio nulla, tempus
 * sine fine? (filius cum alarm(10), piscina certa 64 MB) ; Magnus
 * nullatus {0} ut valor 0. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status = 0;
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Magnus z;
    chorda c;

    memset(&z, 0, sizeof z);
    c = magnus_ad_chordam(magnus_adde(z, magnus_ex_s64(5), p), p);
    printf("zeroed+5=%.*s sg=%d cmp0=%d eq0=%d\n", (int)c.mensura, (const char*)c.datum,
        (int)magnus_signum(z), (int)magnus_compara(z, magnus_ex_s64(0)),
        (int)magnus_aequalis(z, magnus_ex_s64(0)));

    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        Piscina* fixa = piscina_generare_certae_magnitudinis("fixa", (memoriae_index)64 << 20);
        Magnus m;
        alarm(10);
        m = magnus_potentia(magnus_ex_s64(2), (i32)-1, fixa);
        printf("finished bits=%u\n", (unsigned)magnus_bitorum(m));
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) printf("child exit %d\n", WEXITSTATUS(status));
    else printf("child SIGNAL %d (SIGALRM=%d)\n", WTERMSIG(status), SIGALRM);
    piscina_destruere(p);
    return 0;
}
