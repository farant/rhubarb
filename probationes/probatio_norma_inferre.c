/* probatio_norma_inferre.c - inferentia: regulae (B1.1), leges
 * sanitatis et coniunctionis (norma-spec-3 §VI 1-2) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "norma.h"
#include "norma_inferre.h"
#include "norma_gignere.h"
#include "chorda_aedificator.h"
#include "herbarium.h"
#include "norma_stml.h"
#include "filum.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

/* ---- B1.2: discrimen ---- */

interior vacuum
probatio_discrimen(Piscina* p)
{
    constans character* constans bloci[] = {
        "{\"type\":\"text\",\"text\":\"a\"}",
        "{\"type\":\"tool_use\",\"id\":\"t1\",\"name\":\"n\",\"input\":{}}",
        "{\"type\":\"text\",\"text\":\"b\"}",
        "{\"type\":\"tool_use\",\"id\":\"t2\",\"name\":\"m\",\"input\":{}}",
        NIHIL };
    /* clavis tag semel absens: non candidatus */
    constans character* constans absens[] = {
        "{\"type\":\"text\",\"text\":\"a\"}",
        "{\"id\":\"t1\",\"name\":\"n\"}",
        "{\"type\":\"tool_use\",\"id\":\"t2\",\"name\":\"m\"}",
        NIHIL };
    /* claves eaedem in omni partitione: nullum lucrum */
    constans character* constans aequales[] = {
        "{\"type\":\"a\",\"x\":1}", "{\"type\":\"b\",\"x\":2}", NIHIL };
    /* aequalitas lucri: 'type' praefertur */
    constans character* constans par[] = {
        "{\"genus\":\"p\",\"type\":\"p\",\"a\":1}",
        "{\"genus\":\"q\",\"type\":\"q\",\"b\":2}",
        "{\"genus\":\"p\",\"type\":\"p\",\"a\":3}",
        "{\"genus\":\"q\",\"type\":\"q\",\"b\":4}", NIHIL };
    /* valor semel visus non testimonium: quasi-identificator non tag */
    constans character* constans singuli[] = {
        "{\"type\":\"text\",\"text\":\"a\"}",
        "{\"type\":\"tool_use\",\"id\":\"t1\"}", NIHIL };
             character  corpus[XVIII][LXIV];
    constans character* multa[XIX];
                 Norma* n;
            NormaVisus  v;
                   i32  i;

    imprimere("\n--- Probans discrimen ---\n");
    n = inferentia_normam(_inferentia(bloci, p), p);
    v = norma_visus(n);
    CREDO_VERUM(v.genus == NORMA_DISCRIMEN);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.clavis_discriminis, "type");
    CREDO_AEQUALIS_I32(xar_numerus(v.variationes), II);
    si (xar_numerus(v.variationes) == II)
    {
        NormaVariatio* va = (NormaVariatio*)xar_obtinere(v.variationes,
            0);
        NormaVariatio* vb = (NormaVariatio*)xar_obtinere(v.variationes,
            I);

        /* ordo octetorum; variatio ex partitione sua sola, tag implicitum */
        CREDO_CHORDA_AEQUALIS_LITERIS(va->valor, "text");
        CREDO_CHORDA_AEQUALIS_LITERIS(vb->valor, "tool_use");
        CREDO_AEQUALIS_I32(xar_numerus(norma_visus(va->objectum).campi),
            I);
        CREDO_AEQUALIS_I32(xar_numerus(norma_visus(vb->objectum).campi),
            III);
    }
    /* omne exemplum schema suum transit, etiam per discrimen */
    per (i = 0; bloci[i]; i++)
    {
        CREDO_VERUM(norma_iudicare(n, json_legere_literis(bloci[i],
            p).radix, p).validum);
    }
    CREDO_VERUM(norma_visus(inferentia_normam(_inferentia(absens, p),
        p)).genus == NORMA_OBJECTUM);
    CREDO_VERUM(norma_visus(inferentia_normam(_inferentia(aequales, p),
        p)).genus == NORMA_OBJECTUM);
    CREDO_VERUM(norma_visus(inferentia_normam(_inferentia(singuli, p),
        p)).genus == NORMA_OBJECTUM);
    /* lex coniunctionis cum candidatis: 'type' in B solo moritur */
    {
        Inferentia* a      = _inferentia(bloci, p);
        Inferentia* b      = _inferentia(absens, p);
        Inferentia* totum  = _inferentia(bloci, p);

        per (i = 0; absens[i]; i++)
        {
            inferentia_addere(totum, json_legere_literis(absens[i],
                p).radix);
        }
        CREDO_CHORDA_AEQUALIS(_js(inferentia_normam(inferentia_coniungere(
            a, b, p), p), p), _js(inferentia_normam(totum, p), p));
        CREDO_CHORDA_AEQUALIS(_js(inferentia_normam(inferentia_coniungere(
            b, a, p), p), p), _js(inferentia_normam(totum, p), p));
    }
    v = norma_visus(inferentia_normam(_inferentia(par, p), p));
    CREDO_VERUM(v.genus == NORMA_DISCRIMEN);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.clavis_discriminis, "type");
    /* plures quam XVI valores distincti: non discrimen */
    per (i = 0; i < XVIII; i++)
    {
        sprintf(corpus[i], "{\"type\":\"v%u\",\"k%u\":1}",
            (insignatus integer)i, (insignatus integer)i);
        multa[i] = corpus[i];
    }
    multa[XVIII] = NIHIL;
    CREDO_VERUM(norma_visus(inferentia_normam(_inferentia(multa, p),
        p)).genus == NORMA_OBJECTUM);
}

/* sceleton structurae: genus, nullabile, campi (nomen, requisitum),
 * discrimen (clavis, variationes), elementum - non formae, electiones,
 * fines, descriptiones, modi */
interior vacuum
_sceletum (
       constans Norma* n,
    ChordaAedificator* a)
{
    NormaVisus v = norma_visus(n);
           i32 i;

    commutatio (v.genus)
    {
        casus NORMA_LIBERUM:   chorda_aedificator_appendere_literis(a,
                                   "*");
                               frange;
        casus NORMA_NULLUM:    chorda_aedificator_appendere_literis(a,
                                   "null");
                               frange;
        casus NORMA_BOOLEAN:   chorda_aedificator_appendere_literis(a,
                                   "b");
                               frange;
        casus NORMA_INTEGER:   chorda_aedificator_appendere_literis(a,
                                   "i");
                               frange;
        casus NORMA_NUMERUS:   chorda_aedificator_appendere_literis(a,
                                   "n");
                               frange;
        casus NORMA_TEXTUS:    chorda_aedificator_appendere_literis(a,
                                   "s");
                               frange;
        casus NORMA_TABULATUM:
            chorda_aedificator_appendere_literis(a, "[");
            _sceletum(v.elementum, a);
            chorda_aedificator_appendere_literis(a, "]");
            frange;
        casus NORMA_OBJECTUM:
        {
            /* campi ordine octetorum (aedificator ordinem suum habet) */
            i32  numerus = xar_numerus(v.campi);
            i32* ordo = (i32*)malloc((size_t)(numerus
                + I) * magnitudo(i32));
            i32 k;

            per (i = 0; i < numerus; i++)
            {
                ordo[i] = i;
            }
            per (i = I; i < numerus; i++)
            {
                i32 x = ordo[i];

                k = i;
                dum (   k > 0
                     && chorda_comparare(((NormaCampus*)xar_obtinere(
                        v.campi, ordo[k - I]))->titulus,
                        ((NormaCampus*)xar_obtinere(v.campi,
                        x))->titulus)
                        > 0)
                {
                    ordo[k] = ordo[k - I];
                    k--;
                }
                ordo[k] = x;
            }
            chorda_aedificator_appendere_literis(a, "{");
            per (i = 0; i < numerus; i++)
            {
                NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi,
                    ordo[i]);

                chorda_aedificator_appendere_chorda(a, c->titulus);
                chorda_aedificator_appendere_literis(a,
                    c->requiritur ? ":" : "?:");
                _sceletum(c->valor, a);
                chorda_aedificator_appendere_literis(a, ",");
            }
            chorda_aedificator_appendere_literis(a, "}");
            free(ordo);
            frange;
        }
        casus NORMA_DISCRIMEN:
            chorda_aedificator_appendere_literis(a, "D(");
            chorda_aedificator_appendere_chorda(a,
                v.clavis_discriminis);
            chorda_aedificator_appendere_literis(a, "){");
            per (i = 0; i < xar_numerus(v.variationes); i++)
            {
                NormaVariatio* va = (NormaVariatio*)xar_obtinere(
                    v.variationes, i);

                chorda_aedificator_appendere_chorda(a, va->valor);
                chorda_aedificator_appendere_literis(a, "=");
                _sceletum(va->objectum, a);
                chorda_aedificator_appendere_literis(a, ";");
            }
            chorda_aedificator_appendere_literis(a, "}");
            frange;
    }
    si (v.aut_nullum && v.genus != NORMA_NULLUM)
    {
        chorda_aedificator_appendere_literis(a, "|null");
    }
}

interior chorda
_sceletum_chorda (
    constans Norma* n,
           Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, CCLVI);

    _sceletum(n, a);
    redde chorda_aedificator_finire(a);
}

/* schemata nota (ex probatio_norma_gignere, cum unione summa) */
interior Norma*
_schema_responsi (
    Piscina* p)
{
    constans character* constans fines[] = { "end_turn", "tool_use",
        "max_tokens", NIHIL };
    Norma* textus   = norma_objectum(p);
    Norma* petitum  = norma_objectum(p);
    Norma* blocus   = norma_discrimen(p, "type");
    Norma* usus     = norma_objectum(p);
    Norma* r        = norma_objectum(p);

    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_campus(petitum, "name", norma_textus(p), VERUM);
    norma_campus(petitum, "input", norma_modus(norma_objectum(p),
        NORMA_APERTUM), VERUM);
    norma_variatio(blocus, "text", textus);
    norma_variatio(blocus, "tool_use", petitum);
    norma_campus(usus, "input_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(usus, "output_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(r, "id", norma_textus(p), VERUM);
    norma_campus(r, "content", norma_tabulatum(p, blocus), VERUM);
    norma_campus(r, "stop_reason", norma_electio(norma_textus(p),
        fines),
        VERUM);
    norma_campus(r, "usage", usus, VERUM);
    redde r;
}

interior Norma*
_schema_unionis (
    Piscina* p)
{
    Norma* a = norma_objectum(p);
    Norma* b = norma_objectum(p);
    Norma* d = norma_discrimen(p, "kind");

    norma_campus(a, "x", norma_integer(p), VERUM);
    norma_campus(a, "y", norma_textus(p), FALSUM);
    norma_campus(a, "nota", norma_aut_nullum(norma_textus(p)), VERUM);
    norma_campus(b, "z", norma_boolean(p), VERUM);
    norma_campus(b, "w", norma_tabulatum(p, norma_integer(p)), VERUM);
    norma_variatio(d, "alpha", a);
    norma_variatio(d, "beta", b);
    redde d;
}

/* restitutio: CCC valores (TYPICA et FINES) ex schemate noto -> idem
 * sceleton (genera, campi, requisita, discrimina, nullabilia) */
interior vacuum
_restituere (
       Norma* nota,
    constans character* titulus,
     Piscina* p)
{
    Inferentia* inf = inferentia_creare(p, NIHIL);
           s64  semen;
        chorda  ante;
        chorda  post;

    per (semen = 0; semen < CCC; semen++)
    {
        NormaGenitum g = norma_gignere(nota, (semen % II) == 0
            ? NORMA_TYPICA : NORMA_FINES, semen, p);

        inferentia_addere(inf, g.valor);
    }
    ante = _sceletum_chorda(nota, p);
    post = _sceletum_chorda(inferentia_normam(inf, p), p);
    si (!chorda_aequalis(ante, post))
    {
        imprimere("  %s\n    notum:   %.*s\n    inventum: %.*s\n",
            titulus,
            (integer)ante.mensura, (constans character*)ante.datum,
            (integer)post.mensura, (constans character*)post.datum);
    }
    CREDO_CHORDA_AEQUALIS(post, ante);
}

interior vacuum
probatio_lex_restitutionis (
    Piscina* p)
{
    imprimere("\n--- Lex III: restitutio ex norma_gignere ---\n");
    _restituere(_schema_responsi(p),
        "responsum (discrimen in tabulato)",
        p);
    _restituere(_schema_unionis(p), "unio summa (discrimen radicis)",
        p);
}

/* ---- B1.3: positio tabulae similis, electio et fines rogati, fumus ---- */

interior vacuum
probatio_tabula_similis(Piscina* p)
{
    Inferentia* inf = inferentia_creare(p, NIHIL);
     character  exemplum[CXXVIII];
         Norma* n;
    NormaVisus  v;
           i32  i;

    imprimere("\n--- Probans positionem tabulae similem ---\n");
    per (i = 0; i < XL; i++)
    {
        sprintf(exemplum,
            "{\"usage_per_model\":{\"model_%u\":{\"in\":%u}}}",
            (insignatus integer)i, (insignatus integer)i);
        inferentia_addere(inf, json_legere_literis(exemplum, p).radix);
    }
    n = inferentia_normam(inf, p);
    v = norma_visus(((NormaCampus*)xar_obtinere(norma_visus(n).campi,
        0))->valor);
    CREDO_VERUM(v.genus == NORMA_OBJECTUM);
    CREDO_VERUM(v.modus == NORMA_APERTUM);
    CREDO_AEQUALIS_I32(xar_numerus(v.campi), 0);
    CREDO_VERUM(chorda_continet(v.descriptio, chorda_ex_literis(
        "tabulae similis (40 claves distinctae)", p)));
    /* sanitas: sine notis */
    sprintf(exemplum,
        "{\"usage_per_model\":{\"model_novum\":{\"in\":1}}}");
    CREDO_AEQUALIS_I32(xar_numerus(norma_iudicare(n,
        json_legere_literis(
        exemplum, p).radix, p).notae), 0);
}

interior chorda
_cum_optionibus (
    constans character* constans* exempla,
                             b32  electio,
                             b32  fines,
                        Piscina* p)
{
    InferentiaOptiones  o = inferentia_optiones_ordinariae();
            Inferentia* inf;
                   i32  i;

    o.electio  = electio;
    o.fines    = fines;
    inf        = inferentia_creare(p, &o);
    per (i = 0; exempla[i]; i++)
    {
        inferentia_addere(inf, json_legere_literis(exempla[i],
            p).radix);
    }
    redde _js(inferentia_normam(inf, p), p);
}

interior vacuum
probatio_electio_et_fines(Piscina* p)
{
    constans character* constans causae[] = {
        "\"tool_use\"", "\"end_turn\"", "\"end_turn\"",
            "\"max_tokens\"",
        "\"end_turn\"", "\"tool_use\"", NIHIL };
    constans character* constans paucae[] = {
        "\"a\"", "\"b\"", NIHIL };
    constans character* constans multae[] = {
        "\"a\"", "\"b\"", "\"c\"", "\"d\"", "\"e\"", "\"f\"", "\"g\"",
        "\"h\"", "\"i\"", NIHIL };
    constans character* constans numeri[] = {
        "3", "9", "4", "5", "6", NIHIL };
    constans character* constans fluitantes[] = {
        "3", "9.5", "4", "-1.25", "6", NIHIL };
    constans character* constans textus[] = {
        "\"ab\"", "\"abcdef\"", "\"abc\"", "\"abcd\"", "\"abcde\"",
            NIHIL };

    imprimere("\n--- Probans electionem et fines rogatos ---\n");
    /* electio ordine octetorum; non rogata -> nulla */
    CREDO_CHORDA_AEQUALIS_LITERIS(_cum_optionibus(causae, VERUM, FALSUM,
        p),
        "{\"type\":\"string\",\"enum\":[\"end_turn\",\"max_tokens\","
        "\"tool_use\"]}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_cum_optionibus(causae, FALSUM,
        FALSUM, p),
        "{\"type\":\"string\"}");
    /* exempla pauciora quam V; distincti plures quam VIII */
    CREDO_CHORDA_AEQUALIS_LITERIS(_cum_optionibus(paucae, VERUM, FALSUM,
        p),
        "{\"type\":\"string\"}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_cum_optionibus(multae, VERUM, FALSUM,
        p),
        "{\"type\":\"string\"}");
    /* fines observati */
    CREDO_CHORDA_AEQUALIS_LITERIS(_cum_optionibus(numeri, FALSUM, VERUM,
        p),
        "{\"type\":\"integer\",\"minimum\":3,\"maximum\":9}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_cum_optionibus(fluitantes, FALSUM,
        VERUM,
        p), "{\"type\":\"number\",\"minimum\":-1.25,\"maximum\":9.5}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_cum_optionibus(textus, FALSUM, VERUM,
        p),
        "{\"type\":\"string\",\"minLength\":2,\"maxLength\":6}");
}

/* lex IV (fumus): specimina vatis commissa (status CC) -> adumbratio
 * sana, scriptor eam scribit */
interior vacuum
probatio_fumus_speciminum(Piscina* p)
{
    Xar* sp = herbarium_enumerare(p,
        "probationes/fixa/vates/herbarium");
       Inferentia* inf = inferentia_creare(p, NIHIL);
            Norma* n;
    NormaNominata  nn;
           chorda  causa;
              i32  i;
              i32  usa = 0;

    imprimere("\n--- Lex IV: fumus in speciminibus vatis ---\n");
    per (i = 0; i < xar_numerus(sp); i++)
    {
        HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp, i);
             JsonResultus  j;

        si (h->status != CC)
        {
            perge;
        }
        j = json_legere(h->corpus, p);
        si (j.successus)
        {
            inferentia_addere(inf, j.radix);
            usa++;
        }
    }
    CREDO_MAIOR_I32(usa, 0);
    n = inferentia_normam(inf, p);
    CREDO_NON_NIHIL(n);
    per (i = 0; i < xar_numerus(sp); i++)
    {
        HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp, i);
             JsonResultus  j;

        si (h->status != CC)
        {
            perge;
        }
        j = json_legere(h->corpus, p);
        si (j.successus)
        {
            CREDO_VERUM(norma_iudicare(n, j.radix, p).validum);
        }
    }
    nn.titulus  = chorda_ex_literis("responsum", p);
    nn.norma    = n;
    CREDO_MAIOR_I32(norma_stml_scribere(&nn, I, p, &causa).mensura, 0);
}

/* ---- norma-spec-4 B2.1: testimonia ---- */

/* testimonium campi 'titulus' objecti n */
interior chorda
_testimonium_campi (
                   Norma* n,
      constans character* titulus,
    InferentiaTestimonia* t,
                 Piscina* p)
{
    NormaVisus v = norma_visus(n);
           i32 i;

    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);

        si (chorda_aequalis_literis(c->titulus, titulus))
        {
            redde inferentia_commentarius(c->valor, p, t);
        }
    }
    redde chorda_ex_literis("(campus abest)", p);
}

interior vacuum
probatio_testimonia(Piscina* p)
{
    constans character* constans corpus[] = {
        "{\"a\":1,\"b\":\"x\",\"t\":[1]}",
        "{\"a\":2.5,\"t\":[1,2]}",
        "{\"a\":3,\"b\":null,\"t\":[3]}", NIHIL };
    constans character* constans bloci[] = {
        "{\"type\":\"text\",\"text\":\"a\"}",
        "{\"type\":\"tool_use\",\"id\":\"t1\",\"name\":\"n\",\"input\":{}}",
        "{\"type\":\"text\",\"text\":\"b\"}",
        "{\"type\":\"tool_use\",\"id\":\"t2\",\"name\":\"m\",\"input\":{}}",
        NIHIL };
    /* ut specimina vatis: unus valor bis visus */
    constans character* constans rari[] = {
        "{\"type\":\"text\",\"text\":\"a\"}",
        "{\"type\":\"text\",\"text\":\"b\"}",
        "{\"type\":\"text\",\"text\":\"c\"}",
        "{\"type\":\"server_tool_use\",\"id\":\"s\"}", NIHIL };
    constans character* constans dies[] = {
        "\"2026-10-09T01:02:03Z\"", "\"2026-10-09T01:02:04Z\"",
        "\"2026-10-09T01:02:05Z\"", "\"2026-10-09T01:02:06Z\"",
        "\"2026-10-09T01:02:07Z\"", NIHIL };
    constans character* constans secreta[] = {
        "{\"t\":\"SECRETUM_I\",\"k\":{\"type\":\"SECRETUM_X\"}}",
        "{\"t\":\"SECRETUM_II\",\"k\":{\"type\":\"SECRETUM_Y\"}}",
        "{\"t\":\"SECRETUM_III\"}", NIHIL };
    InferentiaTestimonia* t;
                   Norma* n;
           NormaNominata  nn;
                  chorda  causa;
                  chorda  s;

    imprimere("\n--- Probans testimonia ---\n");
    n = inferentia_normam_testatam(_inferentia(corpus, p), p, &t);
    CREDO_CHORDA_AEQUALIS_LITERIS(inferentia_commentarius(n, p, t),
        "3 exempla");
    CREDO_CHORDA_AEQUALIS_LITERIS(_testimonium_campi(n, "a", t, p),
        "visum 3/3; genera: integer 2, fluitans 1");
    CREDO_CHORDA_AEQUALIS_LITERIS(_testimonium_campi(n, "b", t, p),
        "visum 2/3; genera: nullum 1, textus 1; distincti 1");
    CREDO_CHORDA_AEQUALIS_LITERIS(_testimonium_campi(n, "t", t, p),
        "visum 3/3; longitudo 1..2");
    /* discrimen electum */
    n = inferentia_normam_testatam(_inferentia(bloci, p), p, &t);
    CREDO_VERUM(chorda_continet(inferentia_commentarius(n, p, t),
        chorda_ex_literis("discrimen 'type': lucrum 4", p)));
    /* discrimen reiectum, causa nominata */
    n = inferentia_normam_testatam(_inferentia(rari, p), p, &t);
    CREDO_VERUM(chorda_continet(inferentia_commentarius(n, p, t),
        chorda_ex_literis("discrimen 'type' reiectum: valores bis visi 1 "
        "(2 postulati)", p)));
    /* forma */
    n = inferentia_normam_testatam(_inferentia(dies, p), p, &t);
    CREDO_VERUM(chorda_continet(inferentia_commentarius(n, p, t),
        chorda_ex_literis("forma date-time: 5/5", p)));
    /* valores numquam in commentis, per scriptorem */
    n = inferentia_normam_testatam(_inferentia(secreta, p), p, &t);
    nn.titulus = chorda_ex_literis("x", p);
    nn.norma = n;
    s = norma_stml_scribere_cum_commentis(&nn, I, p, &causa,
        inferentia_commentarius, t);
    CREDO_VERUM(s.mensura > 0);
    CREDO_VERUM(chorda_continet(s, chorda_ex_literis("<!-- 3 exempla",
        p)));
    CREDO_FALSUM(chorda_continet(s, chorda_ex_literis("SECRETUM", p)));
}

/* ---- norma-spec-4 B2.2: discrepantia ---- */

/* discrepantiae declaratae contra exempla */
interior Xar*
_comparatio (
              Norma* declarata,
    constans character* constans* exempla,
              Piscina* p)
{
    redde norma_comparare(declarata, _inferentia(exempla, p), p);
}

/* inventum (genus, via) inter discrepantias - tacite */
interior b32
_invenitur (
                       Xar* d,
    NormaDiscrepantiaGenus  genus,
        constans character* via)
{
    i32 i;

    per (i = 0; i < xar_numerus(d); i++)
    {
        NormaDiscrepantia* x = (NormaDiscrepantia*)xar_obtinere(d, i);

        si (x->genus == genus && chorda_aequalis_literis(x->via, via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* inventum (genus, via) inter discrepantias; nuntium imprimit si deest */
interior b32
_habet (
                       Xar* d,
    NormaDiscrepantiaGenus  genus,
        constans character* via,
                   Piscina* p)
{
    i32 i;

    (vacuum)p;
    per (i = 0; i < xar_numerus(d); i++)
    {
        NormaDiscrepantia* x = (NormaDiscrepantia*)xar_obtinere(d, i);

        si (x->genus == genus && chorda_aequalis_literis(x->via, via))
        {
            redde VERUM;
        }
    }
    imprimere("  deest %s %s; inventa:\n",
        norma_discrepantia_descriptio(genus),
        via);
    per (i = 0; i < xar_numerus(d); i++)
    {
        NormaDiscrepantia* x = (NormaDiscrepantia*)xar_obtinere(d, i);

        imprimere("    %.*s %s %.*s\n", (integer)x->via.mensura,
            (constans character*)x->via.datum,
            norma_discrepantia_descriptio(x->genus),
            (integer)x->nuntius.mensura,
            (constans character*)x->nuntius.datum);
    }
    redde FALSUM;
}

interior i32
_numerus_generis (
                       Xar* d,
    NormaDiscrepantiaGenus  genus)
{
    i32 i;
    i32 n = 0;

    per (i = 0; i < xar_numerus(d); i++)
    {
        si (((NormaDiscrepantia*)xar_obtinere(d, i))->genus == genus)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
probatio_discrepantia(Piscina* p)
{
    constans character* constans campi[] = {
        "{\"a\":1,\"b\":\"x\",\"c\":\"y\",\"e\":1,\"a.b\":1}",
        "{\"a\":2.5,\"b\":\"z\",\"n\":null}", NIHIL };
    constans character* constans bloci[] = {
        "{\"type\":\"text\",\"text\":\"a\"}",
        "{\"type\":\"tool_use\",\"id\":\"t1\"}",
        "{\"type\":\"text\",\"text\":\"b\"}",
        "{\"type\":\"tool_use\",\"id\":\"t2\"}", NIHIL };
    constans character* constans sine_tag[] = {
        "{\"type\":\"text\",\"text\":\"a\"}", "{\"text\":\"b\"}",
            NIHIL };
    constans character* constans tabulata[] = {
        "[{\"k\":1,\"l\":2}]", "[{\"k\":3}]", NIHIL };
    Norma* o = norma_objectum(p);
    Norma* d;
    Norma* t;
    Norma* x;
      Xar* r;

    imprimere("\n--- Probans discrepantias ---\n");
    norma_campus(o, "a", norma_integer(p), VERUM);
    norma_campus(o, "b", norma_textus(p), FALSUM);
    norma_campus(o, "c", norma_textus(p), VERUM);
    norma_campus(o, "d", norma_boolean(p), FALSUM);
    norma_campus(o, "n", norma_textus(p), FALSUM);
    norma_campus(o, "a.b", norma_integer(p), FALSUM);
    r = _comparatio(o, campi, p);
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_GENUS_LATIUS, "$.a", p));
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_FORTASSE_REQUISITUM, "$.b",
        p));
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_REQUISITUM_ABSENS, "$.c",
        p));
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_NUMQUAM_VISUM, "$.d", p));
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_NON_DECLARATUM, "$.e", p));
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_NULLUM_NOVUM, "$.n", p));
    /* via clavis rarae: forma iudicis */
    CREDO_AEQUALIS_I32(_numerus_generis(r,
        NORMA_DISCREPANTIA_NON_DECLARATUM), I);
    CREDO_FALSUM(_invenitur(r, NORMA_DISCREPANTIA_NUMQUAM_VISUM,
        "$[\"a.b\"]"));
    CREDO_VERUM(_invenitur(r, NORMA_DISCREPANTIA_FORTASSE_REQUISITUM,
        "$[\"a.b\"]") == FALSUM);
    /* discrimen: variationes contra partitiones */
    d = norma_discrimen(p, "type");
    t = norma_objectum(p);
    x = norma_objectum(p);
    norma_campus(t, "text", norma_textus(p), VERUM);
    norma_campus(x, "thinking", norma_textus(p), VERUM);
    norma_variatio(d, "text", t);
    norma_variatio(d, "thinking", x);
    r = norma_comparare(d, _inferentia(bloci, p), p);
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_VARIATIO_NUMQUAM_VISA, "$",
        p));
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_VARIATIO_NON_DECLARATA,
        "$",
        p));
    /* variatio 'text' contra partitionem suam solam: 'id' alienum non
     * 'non declaratum' */
    CREDO_AEQUALIS_I32(_numerus_generis(r,
        NORMA_DISCREPANTIA_NON_DECLARATUM), 0);
    /* tag non candidatus */
    r = norma_comparare(d, _inferentia(sine_tag, p), p);
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_NON_COMPARABILE, "$", p));
    /* tabulatum: via elementi */
    r = norma_comparare(norma_tabulatum(p,
        norma_campus(norma_objectum(p),
        "k", norma_integer(p), VERUM)), _inferentia(tabulata, p), p);
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_NON_DECLARATUM, "$[].l",
        p));
}

/* lex: exempla contra adumbrationem suam = nulla discrepantia */
interior vacuum
probatio_comparatio_sui(Piscina* p)
{
    Inferentia* inf = _inferentia(_corpus, p);

    imprimere("\n--- Lex: exempla contra adumbrationem suam ---\n");
    CREDO_AEQUALIS_I32(xar_numerus(norma_comparare(inferentia_normam(inf,
        p), inf, p)), 0);
}

/* fumus: lib/vates_responsum.norma contra specimina vatis commissa -
 * schema omnem variationem commissam declarat (2026-10-09, ex captura):
 * specimen cum bloco novo sine schemate renovato hic rubet */
interior vacuum
probatio_comparatio_vatis(Piscina* p)
{
    Xar* sp = herbarium_enumerare(p,
        "probationes/fixa/vates/herbarium");
         Inferentia* inf = inferentia_creare(p, NIHIL);
    NormaStmlLectio  l = norma_stml_legere(filum_legere_totum(
        "lib/vates_responsum.norma", p), NIHIL, 0, p);
    Xar* r;
    i32  i;

    imprimere("\n--- Fumus: responsum declaratum contra specimina ---\n");
    CREDO_VERUM(l.successus);
    per (i = 0; i < xar_numerus(sp); i++)
    {
        HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp, i);
             JsonResultus  j = json_legere(h->corpus, p);

        si (h->status == CC && j.successus)
        {
            inferentia_addere(inf, j.radix);
        }
    }
    r = norma_comparare(norma_stml_quaerere(&l, "responsum"), inf, p);
    CREDO_VERUM(_habet(r, NORMA_DISCREPANTIA_VARIATIO_NUMQUAM_VISA,
        "$.content[]", p));
    CREDO_FALSUM(_habet(r, NORMA_DISCREPANTIA_VARIATIO_NON_DECLARATA,
        "$.content[]", p));
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
    /* B1.2 */
    probatio_discrimen(p);
    probatio_lex_restitutionis(p);
    /* B1.3 */
    probatio_tabula_similis(p);
    probatio_electio_et_fines(p);
    probatio_fumus_speciminum(p);
    /* norma-spec-4 B2.1 */
    probatio_testimonia(p);
    /* norma-spec-4 B2.2 */
    probatio_discrepantia(p);
    probatio_comparatio_sui(p);
    probatio_comparatio_vatis(p);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
