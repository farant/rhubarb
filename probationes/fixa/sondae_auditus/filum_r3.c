#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/resource.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    static char data[100];
    chorda c;
    struct rlimit rl, vetus;
    int r1, r2, r3;
    const char* err1; const char* err2;
    long m1, m2;
    FILE* raw; int rc_fclose;
    (void)p;
    memset(data, 'Q', sizeof data);
    c.datum = (i8*)data; c.mensura = 100;
    remove("build/sonda/tmp_r/r3a.txt");
    remove("build/sonda/tmp_r/r3b.txt");
    remove("build/sonda/tmp_r/r3c.txt");
    signal(SIGXFSZ, SIG_IGN);
    getrlimit(RLIMIT_FSIZE, &vetus);
    rl = vetus; rl.rlim_cur = 10;
    if (setrlimit(RLIMIT_FSIZE, &rl) != 0) { printf("setrlimit fracta\n"); return 1; }

    /* controle: stdio crudum, fclose reddit? */
    raw = fopen("build/sonda/tmp_r/r3c.txt", "wb");
    fwrite(data, 1, 100, raw);
    errno = 0;
    rc_fclose = fclose(raw);
    printf("controle: fclose=%d errno=%d\n", rc_fclose, errno);

    r1 = filum_scribere("build/sonda/tmp_r/r3a.txt", c);
    err1 = filum_error_recens();
    r2 = filum_appendere("build/sonda/tmp_r/r3b.txt", c);
    err2 = filum_error_recens();
    setrlimit(RLIMIT_FSIZE, &vetus);
    m1 = (long)filum_mensura("build/sonda/tmp_r/r3a.txt");
    m2 = (long)filum_mensura("build/sonda/tmp_r/r3b.txt");
    r3 = 0;
    printf("filum_scribere redde=%d err=%s mensura_disco=%ld (exspectatum 100)\n", r1, err1 ? err1 : "NIHIL", m1);
    printf("filum_appendere redde=%d err=%s mensura_disco=%ld (exspectatum 100)\n", r2, err2 ? err2 : "NIHIL", m2);
    (void)r3;
    return 0;
}
