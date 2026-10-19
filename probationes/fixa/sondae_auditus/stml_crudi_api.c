#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static chorda CH(const char* s) { chorda c; c.datum = (i8*)(size_t)s; c.mensura = (i32)strlen(s); return c; }

static void sq(const char* tag, const char* t, i32 pos, const char* tit, i32 tl)
{
    i32 f = 777; b32 a, b;
    a = stml_crudi_sequentia_est(CH(t), pos, tit, tl, &f);
    b = stml_crudi_terminator_est(CH(t), pos, tit, tl);
    printf("%s\tseq=%d\tfuga=%u\tterm=%d\n", tag, (int)a, f, (int)b);
}

int main(void)
{
    i32 k, bad = 0;
    char buf[16];
    const char alpha[] = "<\\/Tx> !\t\n\f\v";
    sq("BASIS", "</T>", 0, "T", 1);
    sq("POS_EQ_MENSURA", "</T>", 4, "T", 1);
    sq("POS_HUGE", "</T>", 0xFFFFFFFFu, "T", 1);
    sq("TL_ZERO", "</>", 0, "T", 0);
    sq("TL_ZERO_SP", "</ x", 0, "", 0);
    sq("EOF_DELIM", "</T", 0, "T", 1);
    sq("FORMFEED", "</T\f", 0, "T", 1);
    sq("VTAB", "</T\v", 0, "T", 1);
    sq("CR", "</T\r", 0, "T", 1);
    sq("PRAEFIXUM", "</Tx>", 0, "T", 1);
    sq("CASUS", "</t>", 0, "T", 1);
    sq("N3", "<\\\\\\/T>", 0, "T", 1);
    sq("FUGA_NIHIL", "<\\/T>", 0, "T", 1);
    printf("FUGA_NIHIL_ARG\tseq=%d\n", (int)stml_crudi_sequentia_est(CH("<\\/T>"), 0, "T", 1, NIHIL));
    printf("TIT_NIHIL\tseq=%d\tfert=%d\n", (int)stml_crudi_sequentia_est(CH("</T>"), 0, NIHIL, 1, NIHIL), (int)stml_crudi_terminatorem_fert(CH("</T>"), NIHIL, 1));
    printf("FERT_VACUUM\t%d\n", (int)stml_crudi_terminatorem_fert(CH(""), "T", 1));
    printf("FERT_FINIS\t%d\n", (int)stml_crudi_terminatorem_fert(CH("abc</T"), "T", 1));
    printf("FERT_STRADDLE\t%d\n", (int)stml_crudi_terminatorem_fert(CH("abc</T\nx"), "T", 1));
    printf("FERT_ESC_SOLUM\t%d\n", (int)stml_crudi_terminatorem_fert(CH("a<\\/T>b"), "T", 1));
    /* long backslash run */
    {
        i32 n = 1000000, i; char* big = (char*)malloc((size_t)n + 8); i32 f = 0;
        big[0] = '<'; for (i = 1; i <= n; i++) big[i] = '\\'; memcpy(big + n + 1, "/T>", 4);
        printf("LONGUS\tseq=%d\tfuga=%u\n", (int)stml_crudi_sequentia_est(CH(big), 0, "T", 1, &f), f);
        free(big);
    }
    /* invariant: term == seq && fuga==0, exhaustive short strings */
    srand(7);
    for (k = 0; k < 200000; k++)
    {
        int len = rand() % 8, j; i32 f; b32 a, b;
        for (j = 0; j < len; j++) buf[j] = alpha[rand() % (int)(sizeof(alpha) - 1)];
        buf[len] = 0;
        a = stml_crudi_sequentia_est(CH(buf), 0, "T", 1, &f);
        b = stml_crudi_terminator_est(CH(buf), 0, "T", 1);
        if (b != (a && f == 0)) bad++;
    }
    printf("INVARIANS\tdiscordiae=%d\n", bad);
    return 0;
}
