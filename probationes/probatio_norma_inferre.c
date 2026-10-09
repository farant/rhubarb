/* probatio_norma_inferre.c - inferentia: regulae (B1.1), leges
 * sanitatis et coniunctionis (norma-spec-3 §VI 1-2) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "norma.h"
#include "norma_inferre.h"

#include <stdio.h>
#include <string.h>

interior chorda
_js (
      Norma* n,
    Piscina* p)
{
    redde json_scribere(norma_json_schema(n, p), p);
}

/* exempla (literae JSON, NIHIL-terminata) -> inferentia */
interior Inferentia*
_inferentia (
    constans character* constans* exempla,
                            Piscina* p)
{
    Inferentia* inf = inferentia_creare(p, NIHIL);
           i32  i;

    per (i = 0; exempla[i]; i++)
    {
        inferentia_addere(inf, json_legere_literis(exempla[i],
            p).radix);
    }
    redde inf;
}

interior chorda
_adumbratio (
    constans character* constans* exempla,
                            Piscina* p)
{
    redde _js(inferentia_normam(_inferentia(exempla, p), p), p);
}

interior vacuum
probatio_genera(Piscina* p)
{
    constans character* constans integra[]  = { "1", "2", NIHIL };
    constans character* constans mixta[]    = { "1", "2.5", NIHIL };
    constans character* constans cum_nullo[] = { "\"a\"", "null",
        NIHIL };
    constans character* constans nulla[]      = { "null", NIHIL };
    constans character* constans discordia[]  = { "1", "\"a\"", NIHIL };
    constans character* constans veritates[] = { "true", "false",
        NIHIL };

    imprimere("\n--- Probans genera ---\n");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(integra, p),
        "{\"type\":\"integer\"}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(mixta, p),
        "{\"type\":\"number\"}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(cum_nullo, p),
        "{\"type\":[\"string\",\"null\"]}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(nulla, p),
        "{\"type\":\"null\"}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(discordia, p), "{}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(veritates, p),
        "{\"type\":\"boolean\"}");
}

interior vacuum
probatio_objecta_et_tabulata(Piscina* p)
{
    constans character* constans objecta[] = {
        "{\"b\":1,\"a\":\"x\",\"u\":{\"x\":1}}",
        "{\"a\":\"y\",\"u\":{\"x\":2,\"y\":\"s\"}}",
        NIHIL };
    constans character* constans tabulata[] = { "[1,2]", "[3]", NIHIL };
    constans character* constans vacua[] = { "[]", NIHIL };
         Norma* n;
    NormaVisus  v;

    imprimere("\n--- Probans objecta et tabulata ---\n");
    /* requisitum = in omni instantia; ordo clavium ordinatus; NOTANDUM */
    n = inferentia_normam(_inferentia(objecta, p), p);
    v = norma_visus(n);
    CREDO_VERUM(v.modus == NORMA_NOTANDUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(n, p),
        "{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\"},"
        "\"b\":{\"type\":\"integer\"},\"u\":{\"type\":\"object\","
        "\"properties\":{\"x\":{\"type\":\"integer\"},\"y\":{\"type\":"
        "\"string\"}},\"required\":[\"x\"]}},\"required\":[\"a\",\"u\"]}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(tabulata, p),
        "{\"type\":\"array\",\"items\":{\"type\":\"integer\"}}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(vacua, p),
        "{\"type\":\"array\",\"items\":{}}");
}

interior vacuum
probatio_formae_et_secreta(Piscina* p)
{
    constans character* constans dies[] = {
        "\"2026-10-09T01:02:03Z\"", "\"2026-10-09T01:02:04Z\"",
        "\"2026-10-09T01:02:05Z\"", "\"2026-10-09T01:02:06Z\"",
        "\"2026-10-09T01:02:07Z\"", NIHIL };
    constans character* constans dies_pauci[] = {
        "\"2026-10-09T01:02:03Z\"", "\"2026-10-09T01:02:04Z\"", NIHIL };
    constans character* constans dies_unus_fractus[] = {
        "\"2026-10-09T01:02:03Z\"", "\"2026-10-09T01:02:04Z\"",
        "\"2026-10-09T01:02:05Z\"", "\"2026-10-09T01:02:06Z\"",
        "\"heri\"", NIHIL };
    constans character* constans secreta[] = {
        "{\"t\":\"SECRETUM_I\"}", "{\"t\":\"SECRETUM_II\"}",
        "{\"t\":\"SECRETUM_III\"}", "{\"t\":\"SECRETUM_IV\"}",
        "{\"t\":\"SECRETUM_V\"}", "{\"t\":\"SECRETUM_I\",\"n\":7}",
            NIHIL };
    chorda s;

    imprimere("\n--- Probans formas et secreta ---\n");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(dies, p),
        "{\"type\":\"string\",\"format\":\"date-time\"}");
    /* exempla minima V */
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(dies_pauci, p),
        "{\"type\":\"string\"}");
    /* unum fractum: nulla forma */
    CREDO_CHORDA_AEQUALIS_LITERIS(_adumbratio(dies_unus_fractus, p),
        "{\"type\":\"string\"}");
    /* valores numquam exeunt (electio et fines non rogati) */
    s = _adumbratio(secreta, p);
    CREDO_FALSUM(chorda_continet(s, chorda_ex_literis("SECRETUM", p)));
    CREDO_FALSUM(chorda_continet(s, chorda_ex_literis("enum", p)));
    CREDO_FALSUM(chorda_continet(s, chorda_ex_literis("minimum", p)));
}

interior vacuum
probatio_limites(Piscina* p)
{
    Inferentia* inf = inferentia_creare(p, NIHIL);

    imprimere("\n--- Probans limites ---\n");
    CREDO_NIHIL(inferentia_normam(inf, p));
    inferentia_addere(inf, NIHIL);
    CREDO_AEQUALIS_I32(inferentia_numerus(inf), 0);
    inferentia_addere(inf, json_legere_literis("{}", p).radix);
    CREDO_AEQUALIS_I32(inferentia_numerus(inf), I);
    CREDO_NON_NIHIL(inferentia_normam(inf, p));
}

/* corpus legum: varium (objecta nidificata, null, tabulata mixta) */
hic_manens constans character* constans _corpus[] = {
    "{\"id\":\"m1\",\"n\":1,\"t\":[1,2],\"o\":{\"a\":true},\"x\":null}",
    "{\"id\":\"m2\",\"n\":2.5,\"t\":[],\"o\":{\"a\":false,\"b\":\"s\"}}",
    "{\"id\":\"m3\",\"n\":3,\"t\":[4],\"o\":{\"a\":true},\"x\":\"v\"}",
    "{\"id\":\"m4\",\"n\":4,\"o\":{\"a\":true,\"c\":[{\"k\":1}]}}",
    "{\"id\":\"m5\",\"n\":5,\"t\":[5,6,7],\"o\":{\"a\":false,"
        "\"c\":[{\"k\":2,\"l\":\"z\"},{\"k\":3}]},\"x\":null}",
    "{\"id\":\"m6\",\"n\":-1,\"o\":{\"a\":true,\"b\":\"t\"},\"y\":{\"z\":1}}",
    NIHIL
};

interior vacuum
probatio_lex_sanitatis(Piscina* p)
{
    Norma* n = inferentia_normam(_inferentia(_corpus, p), p);
      i32  i;
      i32  fracta = 0;

    imprimere("\n--- Lex I: omne exemplum schema suum transit ---\n");
    per (i = 0; _corpus[i]; i++)
    {
        NormaIudicium j = norma_iudicare(n,
            json_legere_literis(_corpus[i], p).radix, p);

        si (!j.validum || xar_numerus(j.notae) > 0)
        {
            fracta++;
        }
    }
    CREDO_AEQUALIS_I32(fracta, 0);
}

interior vacuum
probatio_lex_coniunctionis(Piscina* p)
{
    Inferentia* totum   = _inferentia(_corpus, p);
    Inferentia* a       = inferentia_creare(p, NIHIL);
    Inferentia* b       = inferentia_creare(p, NIHIL);
    Inferentia* versum  = inferentia_creare(p, NIHIL);
           i32  i;
           i32  n;

    imprimere("\n--- Lex II: coniunctio et ordo ---\n");
    per (n = 0; _corpus[n]; n++)
    {
    }
    per (i = 0; i < n; i++)
    {
        inferentia_addere(i < III ? a : b,
            json_legere_literis(_corpus[i], p).radix);
        inferentia_addere(versum,
            json_legere_literis(_corpus[n - I - i], p).radix);
    }
    CREDO_CHORDA_AEQUALIS(_js(inferentia_normam(inferentia_coniungere(a,
        b, p), p), p), _js(inferentia_normam(totum, p), p));
    CREDO_CHORDA_AEQUALIS(_js(inferentia_normam(versum, p), p),
        _js(inferentia_normam(totum, p), p));
    CREDO_AEQUALIS_I32(inferentia_numerus(inferentia_coniungere(a, b,
        p)),
        n);
    /* coniunctio neutram mutat */
    CREDO_AEQUALIS_I32(inferentia_numerus(a), III);
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma_inferre",
        M * M);
        b32 successus;

    credo_aperire(p);
    probatio_genera(p);
    probatio_objecta_et_tabulata(p);
    probatio_formae_et_secreta(p);
    probatio_limites(p);
    probatio_lex_sanitatis(p);
    probatio_lex_coniunctionis(p);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
