#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static void caso(const char* tag, unsigned long magn, int stage)
{
    pid_t pid;
    int st;
    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        Piscina* p = piscina_generare_certae_magnitudinis("certa", magn);
        MultipartAedificator* a;
        chorda c;
        a = multipart_creare(p);
        printf("  [%s] creare -> %s\n", tag, a ? "non-NIHIL" : "NIHIL");
        fflush(stdout);
        if (!a) _exit(0);
        if (stage >= 1) { multipart_addere_textum(a, "t", chorda_ex_literis("val", p)); printf("  [%s] addere ok\n", tag); fflush(stdout); }
        if (stage >= 2) { c = multipart_corpus_finire(a); printf("  [%s] finire mensura=%u datum=%p\n", tag, c.mensura, (void*)c.datum); fflush(stdout); }
        _exit(0);
    }
    waitpid(pid, &st, 0);
    printf("%s (piscina %lu): child exit=%d\n", tag, magn, WIFEXITED(st) ? WEXITSTATUS(st) : -WTERMSIG(st));
}

int main(void)
{
    caso("minima", 256, 2);
    caso("creare_capit", 4096, 2);
    caso("finire_non_capit", 4096 + 200, 2);
    caso("satis", 65536, 2);
    return 0;
}
