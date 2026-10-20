#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include <stdio.h>

/* unicitas super= : '*', 'r/item', 'item' (moderatio) */
interior constans character* DOC =
    "<r><item a=\"x\"/><item a=\"x\"/></r>";

interior vacuum
probare(constans character* super_val, Piscina* p, InternamentumChorda* intern)
{
    character fons[1024];
    chorda causa;
    Canon* c;
    StmlResultus res;
    Xar* vitia;
    causa.datum = NIHIL; causa.mensura = 0;
    sprintf(fons,
        "<canon dialectus=\"x\" versio=\"1\">"
        "<elementum nomen=\"r\" radix=\"verum\">"
        "<liberum nomen=\"item\"/></elementum>"
        "<elementum nomen=\"item\">"
        "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
        "<unicitas nomen=\"u\" attributum=\"a\" super=\"%s\"/>"
        "</canon>", super_val);
    c = canon_legere(chorda_ex_literis(fons, p), p, intern, &causa);
    si (!c) { imprimere("super=%-8s canon RECUSATUS: %.*s\n", super_val,
        (integer)causa.mensura, causa.datum ? (constans character*)causa.datum : ""); redde; }
    res = stml_legere(chorda_ex_literis(DOC, p), p, intern);
    vitia = canon_iudicare(c, res.radix, p);
    imprimere("super=%-8s canon lectus; vitia=%d\n", super_val,
        (integer)xar_numerus(vitia));
}

integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    chorda cat, r1, r2, rad;
    probare("item", p, intern);
    probare("*", p, intern);
    probare("r/item", p, intern);

    /* claim 2: registrum */
    cat = chorda_ex_literis(".genera\tnatura/natura.canon\n<aedilis>\taedilis.canon\n", p);
    r1 = canon_registrum_quaerere(cat, "x/y.genera", p);
    rad = chorda_ex_literis("aedilis", p);
    r2 = canon_registrum_quaerere_radice(cat, &rad, p);
    imprimere("cat=[%p,%p) r1=%p len=%u intra=%d sequens='%c'\n",
        (vacuum*)cat.datum, (vacuum*)(cat.datum + cat.mensura),
        (vacuum*)r1.datum, (unsigned)r1.mensura,
        (integer)(r1.datum >= cat.datum && r1.datum < cat.datum + cat.mensura),
        (character)r1.datum[r1.mensura]);
    imprimere("r2=%p len=%u intra=%d sequens='%c'\n",
        (vacuum*)r2.datum, (unsigned)r2.mensura,
        (integer)(r2.datum >= cat.datum && r2.datum < cat.datum + cat.mensura),
        (character)r2.datum[r2.mensura]);
    redde 0;
}
