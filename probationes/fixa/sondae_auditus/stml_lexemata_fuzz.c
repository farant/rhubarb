#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static int viol_ext = 0, viol_ord = 0, viol_cov = 0, viol_lc = 0, vacua = 0, tot = 0;
static char ultimum_vitium[512];

static void iudicare(const char* s, i32 len, Piscina* pp, InternamentumChorda* it)
{
    chorda in; Xar* lx; i32 i, n, prev = 0, linea = 1, col = 1, k;
    StmlResultus r;
    in.datum = (i8*)(size_t)s; in.mensura = len;
    lx = stml_lexemata_colligere(in, pp, it);
    r = stml_legere(in, pp, it);
    (void)r;
    tot++;
    if (lx == NIHIL) { vacua++; return; }
    n = xar_numerus(lx);
    for (i = 0; i < n; i++)
    {
        StmlLexema* l = (StmlLexema*)xar_obtinere(lx, i);
        if (l->initium > l->finis || l->finis > (i32)len) { viol_ext++; snprintf(ultimum_vitium, sizeof ultimum_vitium, "EXT [%.*s] lex%u [%u,%u)", (int)len, s, i, l->initium, l->finis); }
        if (l->initium < prev) { viol_ord++; snprintf(ultimum_vitium, sizeof ultimum_vitium, "ORD [%.*s] lex%u [%u,%u) prev=%u", (int)len, s, i, l->initium, l->finis, prev); }
        if (l->initium != prev) { viol_cov++; }
        /* line/col check vs actual offset (LF-only inputs) */
        linea = 1; col = 1;
        for (k = 0; k < (i32)l->initium && k < len; k++) { if (s[k] == '\n') { linea++; col = 1; } else col++; }
        if (l->linea != (i32)linea || l->columna != (i32)col) { viol_lc++; snprintf(ultimum_vitium, sizeof ultimum_vitium, "LC [%.*s] lex%u at %u: %u:%u vs %u:%u", (int)len, s, i, l->initium, l->linea, l->columna, linea, col); }
        prev = l->finis;
    }
    if (prev != len) { viol_cov++; }
}

int main(void)
{
    const char* doc = "<r a=\"1\" b c=v>\n  <x!>raw <\\/x> </x>\n  <!-- c --><?pi?><<sel>>\n  <#f (> <t\\>\n    m\n  </t>\n  <% &k;>q</%> &amp; &x; <= s => <) z>\n  <@m=>v</> <.d/> <a((> <b/><c/>\n</r>\n";
    const char alpha[] = "<>/!-?#%&;=()\"' \nab\\@.*$xT";
    char buf[64];
    i32 doclen = (i32)strlen(doc), i, k;
    Piscina* pp = piscina_generare_dynamicum("sonda", 1 << 20);
    InternamentumChorda* it = internamentum_creare(pp);
    alarm(120);
    ultimum_vitium[0] = 0;
    for (i = 0; i <= doclen; i++) iudicare(doc, i, pp, it);
    printf("PRAEFIXA\ttot=%d vacua=%d ext=%d ord=%d hiatus=%d lincol=%d\tultimum=%s\n", tot, vacua, viol_ext, viol_ord, viol_cov, viol_lc, ultimum_vitium);
    tot = vacua = viol_ext = viol_ord = viol_cov = viol_lc = 0; ultimum_vitium[0] = 0;
    srand(12345);
    for (k = 0; k < 60000; k++)
    {
        int len = 1 + rand() % 24, j;
        for (j = 0; j < len; j++) buf[j] = alpha[rand() % (int)(sizeof(alpha) - 1)];
        iudicare(buf, len, pp, it);
        if ((k % 2000) == 0) { piscina_vacare(pp); it = internamentum_creare(pp); }
    }
    printf("FUZZ\ttot=%d vacua=%d ext=%d ord=%d hiatus=%d lincol=%d\tultimum=%s\n", tot, vacua, viol_ext, viol_ord, viol_cov, viol_lc, ultimum_vitium);
    return 0;
}
