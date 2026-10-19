#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "volumen.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Volumen* v = volumen_temporarium(p, "sonda_volumen_r1");
    chorda via = chorda_ex_literis("a.txt", p);
    chorda cont = chorda_ex_literis("salve", p);
    chorda vacua_via;
    b32 res;
    b32 inv;
    s64 seq;
    chorda c;

    si (v == NIHIL) { imprimere("volumen NIHIL\n"); redde 1; }

    res = volumen_plagulam_condere(v, via, cont, "genita");
    imprimere("origo 'genita': %d\n", (int)res);
    res = volumen_plagulam_condere(v, via, cont, "");
    imprimere("origo '': %d err=%s\n", (int)res,
        volumen_error(v) ? volumen_error(v) : "(nullus)");

    seq = volumen_actum_appendere(v, "", chorda_ex_literis("{}", p));
    imprimere("actum genus '': seq=%ld err=%s\n", (long)seq,
        volumen_error(v) ? volumen_error(v) : "(nullus)");
    seq = volumen_actum_appendere(v, "x", chorda_ex_literis("", p));
    imprimere("actum datum '': seq=%ld\n", (long)seq);

    vacua_via = chorda_ex_literis("", p);
    res = volumen_plagulam_condere(v, vacua_via, cont, "genita");
    imprimere("via '' prima: %d\n", (int)res);
    res = volumen_plagulam_condere(v, vacua_via, chorda_ex_literis("aliud", p), "genita");
    imprimere("via '' secunda: %d summa_plag=%ld\n", (int)res,
        (long)volumen_summa_plagularum(v));
    c = volumen_plagulam_promere(v, vacua_via, p, &inv);
    imprimere("promere via '': inventum=%d mensura=%d\n", (int)inv, (int)c.mensura);

    volumen_claudere(v);
    piscina_destruere(p);
    redde 0;
}
