#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define DIR_T "build/sonda/filum_tmp_scribere"

static void scribe(const char* via, const char* s)
{
    FILE* f = fopen(via, "wb"); fputs(s, f); fclose(f);
}

static const char* er(void) { const char* e = filum_error_recens(); return e ? e : "NIHIL"; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda c, vac;
    b32 r;
    FilumScriptor* s;
    FilumLector* l;
    system("rm -rf " DIR_T);
    mkdir(DIR_T, 0755);
    scribe(DIR_T "/praesens.txt", "hic\n");

    /* error_recens staleness after successful calls */
    (void)filum_legere_totum(DIR_T "/absens", p);
    r = filum_existit(DIR_T "/praesens.txt");
    printf("err1 post filum_existit successum (r=%d): error=%s\n", (int)r, er());
    (void)filum_legere_totum(DIR_T "/absens", p);
    printf("err2 post filum_mensura successum (m=%lu): error=%s\n", (unsigned long)filum_mensura(DIR_T "/praesens.txt"), er());
    (void)filum_legere_totum(DIR_T "/absens", p);
    printf("err3 post filum_directorium_existit successum (r=%d): error=%s\n", (int)filum_directorium_existit(DIR_T), er());
    l = filum_lector_aperire(DIR_T "/praesens.txt", p);
    (void)filum_legere_totum(DIR_T "/absens", p);
    r = filum_lector_lineam_proximam(l, &c);
    printf("err4 post lineam_proximam successum (r=%d): error=%s\n", (int)r, er());
    filum_lector_claudere(l);
    (void)filum_legere_totum(DIR_T "/absens", p);
    {
        FilumSera* sr = filum_seram_capere(DIR_T "/sera", 0, p);
        printf("err5 post seram_capere successum (sera=%s): error=%s\n", sr ? "ok" : "NIHIL", er());
        _filum_dummy:
        (void)0;
        {
            FilumSera* sr2;
            (void)filum_existit(DIR_T "/praesens.txt");
            /* clear by a successful purging call */
            (void)filum_directorium_creare_si_necesse(DIR_T);
            sr2 = filum_seram_capere(DIR_T "/sera", 0, p);
            printf("err6 seram_capere tempus exhaustum (sera=%s): error=%s\n", sr2 ? "ok" : "NIHIL", er());
        }
        filum_seram_liberare(sr);
    }

    /* empty chorda writes */
    vac.datum = NIHIL; vac.mensura = 0;
    scribe(DIR_T "/vetus.txt", "VETUS");
    r = filum_scribere(DIR_T "/vetus.txt", vac);
    c = filum_legere_totum(DIR_T "/vetus.txt", p);
    printf("scr1 filum_scribere(chorda vacua datum NIHIL): r=%d error=%s mensura_post=%u (superscribere -> 0)\n", (int)r, er(), (unsigned)c.mensura);
    r = filum_scribere(DIR_T "/novum_vacuum.txt", vac);
    printf("scr2 filum_scribere(vacua) in filum novum: r=%d existit=%d\n", (int)r, (int)filum_existit(DIR_T "/novum_vacuum.txt"));
    r = filum_appendere(DIR_T "/vetus.txt", vac);
    printf("scr3 filum_appendere(vacua datum NIHIL): r=%d error=%s\n", (int)r, er());
    s = filum_scriptor_aperire(DIR_T "/sc.txt", FILUM_MODUS_CREARE, p);
    printf("scr4 filum_scriptor_scribere(vacua): r=%d\n", (int)filum_scriptor_scribere(s, vac));
    filum_scriptor_claudere(s);
    r = filum_appendere_firmiter(DIR_T "/firm_vac.txt", vac);
    printf("scr5 filum_appendere_firmiter(vacua): r=%d existit=%d\n", (int)r, (int)filum_existit(DIR_T "/firm_vac.txt"));
    /* embedded NUL */
    c.datum = (i8*)"a\0b"; c.mensura = 3;
    r = filum_scribere(DIR_T "/nul.bin", c);
    printf("scr6 NUL interior: r=%d mensura_disci=%lu\n", (int)r, (unsigned long)filum_mensura(DIR_T "/nul.bin"));

    /* appendere on missing file */
    r = filum_appendere_literis(DIR_T "/absens_app.txt", "x");
    printf("app1 appendere_literis filum absens: r=%d existit_post=%d\n", (int)r, (int)filum_existit(DIR_T "/absens_app.txt"));
    s = filum_scriptor_aperire(DIR_T "/absens_app2.txt", FILUM_MODUS_APPENDERE, p);
    printf("app2 scriptor APPENDERE filum absens: scriptor=%s\n", s ? "non NIHIL (creatum)" : "NIHIL");
    if (s) filum_scriptor_claudere(s);

    /* scriptor modus out of range on an existing file */
    scribe(DIR_T "/servandum.txt", "SERVANDUM");
    s = filum_scriptor_aperire(DIR_T "/servandum.txt", (FilumModus)2, p);
    if (s) filum_scriptor_claudere(s);
    printf("mod1 scriptor modus=2: scriptor=%s mensura_post=%lu (9 = intactum)\n", s ? "non NIHIL" : "NIHIL", (unsigned long)filum_mensura(DIR_T "/servandum.txt"));

    /* lineam_scribere with trailing newline */
    s = filum_scriptor_aperire(DIR_T "/lin.txt", FILUM_MODUS_CREARE, p);
    filum_scriptor_lineam_scribere(s, "abc\n");
    filum_scriptor_lineam_scribere(s, NIHIL);
    filum_scriptor_claudere(s);
    filum_scriptor_claudere(s);
    printf("lin1 lineam_scribere(\"abc\\n\") + (NIHIL): mensura=%lu\n", (unsigned long)filum_mensura(DIR_T "/lin.txt"));
    printf("lin2 scribere post claudere: r=%d error=%s\n", (int)filum_scriptor_scribere_literis(s, "x"), er());

    system("rm -rf " DIR_T);
    piscina_destruere(p);
    return 0;
}
