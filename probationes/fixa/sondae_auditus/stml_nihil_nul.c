#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static Piscina* pp;
static InternamentumChorda* it;

static void sub(const char* tag, int modus)
{
    pid_t pid; int st;
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
        chorda in; StmlResultus r;
        alarm(5);
        in.datum = (i8*)"<a>x</a>"; in.mensura = 8;
        if (modus == 0) { r = stml_legere(in, NIHIL, it); printf("%s\tstatus=%d\n", tag, (int)r.status); }
        if (modus == 1) { r = stml_legere(in, pp, NIHIL); printf("%s\tstatus=%d succ=%d radix=%p tit=%p\n", tag, (int)r.status, (int)r.successus, (void*)r.radix, r.elementum_radix ? (void*)r.elementum_radix->titulus : NIHIL); }
        if (modus == 2) { StmlNodus* e = stml_elementum_creare(pp, it, NIHIL); printf("%s\te=%p tit=%p\n", tag, (void*)e, e ? (void*)e->titulus : NIHIL); }
        if (modus == 3) { StmlNodus* e = stml_elementum_creare(pp, it, "a"); b32 b = stml_attributum_addere(e, pp, it, "k", NIHIL); chorda w = stml_scribere(e, pp, FALSUM); printf("%s\tret=%d\tscriptum=[%.*s]\n", tag, (int)b, (int)w.mensura, (char*)w.datum); }
        if (modus == 4) { StmlNodus* t = stml_textum_creare(pp, it, NIHIL); printf("%s\tt=%p valor=%p\n", tag, (void*)t, t ? (void*)t->valor : NIHIL); }
        if (modus == 5) { StmlNodus* e = stml_elementum_creare(pp, it, "a"); b32 b = stml_attributum_addere(e, pp, it, NIHIL, "v"); chorda w = stml_scribere(e, pp, FALSUM); printf("%s\tret=%d\tscriptum=[%.*s]\n", tag, (int)b, (int)w.mensura, (char*)w.datum); }
        if (modus == 6) { Xar* x = stml_invenire_omnes_liberos(NIHIL, "a", pp); printf("%s\txar=%p n=%u\n", tag, (void*)x, xar_numerus(x)); }
        if (modus == 7) { StmlNodus* e = stml_elementum_creare(pp, it, "a"); printf("%s\tad_indicem_huge=%p ultimus=%p prior=%p idx=%d\n", tag, (void*)stml_liberum_ad_indicem(e, 0xFFFFFFFFu), (void*)stml_ultimus_liberum(e), (void*)stml_frater_prior(e), (int)stml_index_inter_fratres(e)); }
        if (modus == 8) { StmlNodus* a = stml_elementum_creare(pp, it, "a"); StmlNodus* b = stml_elementum_creare(pp, it, "b"); stml_liberum_addere(a, b); stml_liberum_addere(b, a); printf("%s\tcyclus_addere_ok\n", tag); fflush(stdout); { Xar* m = stml_maiores(a, pp); printf("%s\tmaiores=%u\n", tag, xar_numerus(m)); } }
        if (modus == 9) { StmlNodus* a = stml_elementum_creare(pp, it, "a"); printf("%s\tse_ipsum=%d\n", tag, (int)stml_liberum_addere(a, a)); fflush(stdout); { chorda w = stml_scribere(a, pp, FALSUM); printf("%s\tscriptum_mensura=%u\n", tag, w.mensura); } }
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
    if (WIFSIGNALED(st)) printf("%s\tSIGNUM=%d\n", tag, WTERMSIG(st));
    else if (WEXITSTATUS(st)) printf("%s\tEXIT=%d\n", tag, WEXITSTATUS(st));
}

static void nul(const char* tag, const char* s, i32 len)
{
    chorda in, w; StmlResultus r; chorda v;
    in.datum = (i8*)(size_t)s; in.mensura = len;
    r = stml_legere(in, pp, it);
    w = stml_scribere(r.radix, pp, FALSUM);
    v = r.elementum_radix ? stml_textus_valor(r.elementum_radix, pp) : in;
    printf("%s\tstatus=%d\tfidelis=%d\tin_len=%u out_len=%u\tvalor_len=%u\n", tag, (int)r.status,
        (int)(w.mensura == in.mensura && memcmp(w.datum, in.datum, in.mensura) == 0), in.mensura, w.mensura, r.elementum_radix ? v.mensura : 0);
}

int main(void)
{
    StmlResultus r; chorda e;
    setvbuf(stdout, NULL, _IONBF, 0);
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    e.datum = NIHIL; e.mensura = 0;
    r = stml_legere(e, pp, it); printf("VACUUM_NIHIL\tstatus=%d\n", (int)r.status);
    e.datum = (i8*)"x"; e.mensura = 0;
    r = stml_legere(e, pp, it); printf("VACUUM_DATUM\tstatus=%d\n", (int)r.status);
    r = stml_legere_ex_literis("", pp, it); printf("EX_LITERIS_VACUUM\tstatus=%d\n", (int)r.status);
    r = stml_legere_ex_literis(NIHIL, pp, it); printf("EX_LITERIS_NIHIL\tstatus=%d\n", (int)r.status);
    r = stml_legere_ex_literis("  \n ", pp, it); printf("SOLUM_ALBUM\tstatus=%d\telem=%p\tsucc=%d\n", (int)r.status, (void*)r.elementum_radix, (int)r.successus);
    r = stml_legere_ex_literis("<!-- c -->", pp, it); printf("SOLUM_COMMENTUM\tstatus=%d\telem=%p\n", (int)r.status, (void*)r.elementum_radix);
    printf("LEX_VACUUM\t%p\n", (void*)stml_lexemata_colligere(e, pp, it));
    nul("NUL_TEXTUS", "<a>x\0y</a>", 10);
    nul("NUL_ATTR", "<a k=\"x\0y\"/>", 12);
    nul("NUL_CRUDUS", "<a!>x\0y</a>", 11);
    nul("NUL_COMMENTUM", "<a><!--x\0y--></a>", 17);
    nul("NUL_POST_TITULUM", "<a\0>x</a>", 9);
    nul("D_TACITUS", "<!D><a/>b><c/>", 14);
    sub("PISCINA_NIHIL", 0);
    sub("INTERN_NIHIL", 1);
    sub("CREARE_TITULUS_NIHIL", 2);
    sub("ATTR_VALOR_NIHIL", 3);
    sub("TEXTUS_NIHIL", 4);
    sub("ATTR_TITULUS_NIHIL", 5);
    sub("OMNES_NIHIL", 6);
    sub("INDICES", 7);
    sub("CYCLUS", 8);
    sub("SE_IPSUM", 9);
    return 0;
}
