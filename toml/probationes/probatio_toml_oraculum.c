/* probatio_toml_oraculum.c - Oraculum toml-test: valida per valores,
 * invalida per diagnostica (Q8)
 *
 * Comparator primum per casus inlineos (aequalitas et differentia cum
 * via et causa), deinde corpus toml-test 1.0.0: omne VALIDUM parsatur,
 * coquitur, cum .json vicino comparatur; omne INVALIDUM diagnosticum
 * habere debet. Pinnae solum crescunt. ORACULUM_OMNIA=1 omnem
 * fracturam imprimit (via + causa); ORACULUM_EXEMPLUM=<via> casum unum
 * totum.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_coctum.h"
#include "toml_oraculum.h"
#include "toml_corpus_ambulare.h"
#include "json.h"
#include "filum.h"
#include "piscina.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pinnae: numeri ad natum, solum crescunt */
#define PINNA_VALIDA    CCV
#define PINNA_INVALIDA  CDLXXIV


/* ==================================================
 * Adiumenta
 * ================================================== */

interior TomlComparatio
_comparare (
               Piscina* piscina,
    constans character* toml,
    constans character* json)
{
            s32  n = (s32)strlen(toml);
      character* f = (character*)piscina_allocare(piscina, (i64)n + I);
    MateriaNodus* radix;
     TomlParsura  r;
      TomlCoctum  c;
    JsonResultus  j;
    TomlComparatio exitus;

    memcpy(f, toml, (size_t)n + I);
    radix  = toml_arbor_parsare(piscina, f, n, &r);
    c      = toml_coquere(piscina, radix, &r);
    j      = json_legere_literis(json, piscina);
    memset(&exitus, ZEPHYRUM, magnitudo(exitus));
    si (!c.sanum || !j.successus)
    {
        imprimere("    casus non sanus: %s | %s\n", toml, json);
        redde exitus;
    }
    redde toml_oraculum_comparare(piscina, c.radix, j.radix);
}

interior b32
_chorda_est (
                 chorda  c,
     constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde c.mensura == n
        && (n == ZEPHYRUM || memcmp(c.datum, litterae, (size_t)n)
            == ZEPHYRUM);
}


/* ==================================================
 * Casus inlinei
 * ================================================== */

nomen structura {
    constans character* toml;
    constans character* json;
    constans character* via;      /* NIHIL = aequalia */
} CasusComparationis;

#define TI(v)  "{\"type\": \"integer\", \"value\": \"" v "\"}"
#define TF(v)  "{\"type\": \"float\", \"value\": \"" v "\"}"
#define TS(v)  "{\"type\": \"string\", \"value\": \"" v "\"}"

hic_manens constans CasusComparationis CASUS_COMPARATIONIS[] = {
    { "a = 1",          "{\"a\": " TI("1") "}",               NIHIL },
    { "a = 1",          "{\"a\": " TI("2") "}",               "a" },
    { "a = 1.0",        "{\"a\": " TI("1") "}",               "a" },
    { "a = -0.0",       "{\"a\": " TF("0") "}",               "a" },
    { "a = -0.0",       "{\"a\": " TF("-0") "}",              NIHIL },
    { "a = 1e3",        "{\"a\": " TF("1000") "}",            NIHIL },
    { "a = nan",        "{\"a\": " TF("nan") "}",             NIHIL },
    { "a = -inf",       "{\"a\": " TF("-inf") "}",            NIHIL },
    { "a = -inf",       "{\"a\": " TF("inf") "}",             "a" },
    { "a = 9223372036854775807",
      "{\"a\": " TI("9223372036854775807") "}",               NIHIL },
    { "a = -9223372036854775808",
      "{\"a\": " TI("-9223372036854775808") "}",              NIHIL },
    { "a = \"x\\ty\"",  "{\"a\": " TS("x\\ty") "}",           NIHIL },
    { "a = \"x\"",      "{\"a\": " TS("y") "}",               "a" },
    { "a = true",       "{\"a\": {\"type\": \"bool\", \"value\": "
                        "\"true\"}}",                         NIHIL },
    { "a = [1, 2]",     "{\"a\": [" TI("1") ", " TI("3") "]}", "a[1]" },
    { "a = [1, 2]",     "{\"a\": [" TI("1") "]}",             "a" },
    { "a.b.c = 1",      "{\"a\": {\"b\": {\"c\": " TI("1") "}}}",
        NIHIL },
    { "a.b.c = 1",      "{\"a\": {\"b\": {\"d\": " TI("1") "}}}",
      "a.b" },
    { "a = 1",          "{\"a\": " TI("1") ", \"b\": " TI("1") "}",
        "" },
    { "\"\" = 1",       "{\"\": " TI("1") "}",                NIHIL },
    { "[t]\ntype = \"x\"\nvalue = \"y\"",
      "{\"t\": {\"type\": " TS("x") ", \"value\": " TS("y") "}}",
          NIHIL },
    { "[[t]]\nx = 1\n[[t]]\nx = 2",
      "{\"t\": [{\"x\": " TI("1") "}, {\"x\": " TI("2") "}]}", NIHIL },
    { "a = 1979-05-27T07:32:00Z",
      "{\"a\": {\"type\": \"datetime\", \"value\": "
      "\"1979-05-27T07:32:00+00:00\"}}",                      NIHIL },
    { "a = 1979-05-27 07:32:00.5",
      "{\"a\": {\"type\": \"datetime-local\", \"value\": "
      "\"1979-05-27T07:32:00.500\"}}",                        NIHIL },
    { "a = 1979-05-27",
      "{\"a\": {\"type\": \"datetime-local\", \"value\": "
      "\"1979-05-27\"}}",                                     "a" },
    { "a = 07:32:00",
      "{\"a\": {\"type\": \"time-local\", \"value\": "
      "\"07:32:01\"}}",                                       "a" },
    { NIHIL,            NIHIL,                                NIHIL }
};


/* ==================================================
 * Corpus
 * ================================================== */

nomen structura {
    constans character* radix;
                   i32  valida;
                   i32  paria;
                   i32  invalida;
                   i32  reiecta;
                   b32  omnia;
    constans character* exemplum;
} Status;

interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
           Status* st = (Status*)datum;
      TomlParsura  r;
     MateriaNodus* radix;
       TomlCoctum  c;
              b32  loqui;

    si (fons != TOML_CORPUS_TOML_TEST)
    {
        redde;
    }
    loqui = st->exemplum != NIHIL
        && strcmp(st->exemplum, via) == ZEPHYRUM;
    radix = toml_arbor_parsare(opus, (constans character*)textus.datum,
        (s32)textus.mensura, &r);
    c     = toml_coquere(opus, radix, &r);
    si (loqui)
    {
        i32 k;

        imprimere("  EXEMPLUM %s: sanum %d, diagnostica %u\n", via,
            (integer)c.sanum, xar_numerus(c.diagnostica));
        per (k = ZEPHYRUM; k < xar_numerus(c.diagnostica); k++)
        {
            constans MateriaDiagnosticum* d =
                (constans MateriaDiagnosticum*)xar_obtinere(
                    c.diagnostica, k);

            imprimere("    %s @%d: %s\n", d->codex,
                (integer)d->tractus.initium, d->causa);
        }
    }
    si (strncmp(via, "valid/", VI) != ZEPHYRUM)
    {
        st->invalida++;
        si (!c.sanum && xar_numerus(c.diagnostica) > ZEPHYRUM)
        {
            st->reiecta++;
        }
        alioquin si (st->omnia || loqui)
        {
            imprimere("    INVALIDUM ACCEPTUM %s\n", via);
        }
        redde;
    }
    st->valida++;
    {
            character plena[MXXIV];
                  i32 l;
               chorda json;
         JsonResultus j;
       TomlComparatio cmp;

        sprintf(plena, "%s/toml/probationes/fixa/toml-test/tests/%s",
            st->radix, via);
        l = (i32)strlen(plena);
        memcpy(plena + l - IV, "json", V);
        json  = filum_legere_totum(plena, opus);
        j     = json_legere(json, opus);
        si (!j.successus)
        {
            imprimere("    JSON illegibile: %s\n", plena);
            redde;
        }
        si (!c.sanum)
        {
            si (st->omnia || loqui)
            {
                imprimere("    VALIDUM NON SANUM %s\n", via);
            }
            redde;
        }
        cmp = toml_oraculum_comparare(opus, c.radix, j.radix);
        si (cmp.aequalis)
        {
            st->paria++;
        }
        alioquin si (st->omnia || loqui)
        {
            imprimere("    DISPAR %s @ '%.*s': %.*s\n", via,
                (integer)cmp.via_differentiae.mensura,
                (constans character*)cmp.via_differentiae.datum,
                (integer)cmp.causa.mensura,
                (constans character*)cmp.causa.datum);
        }
    }
}


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
                Piscina* opus;
     constans character* radix = lectiones_ambitus("RHUBARB_RADIX");

    piscina = piscina_generare_dynamicum("probatio_toml_oraculum",
        262144);
    opus    = piscina_generare_dynamicum("probatio_toml_oraculum_opus",
        1048576);
    si (!piscina || !opus)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Comparator
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans comparatorem ---\n");
        per (k = ZEPHYRUM; CASUS_COMPARATIONIS[k].toml != NIHIL; k++)
        {
             constans CasusComparationis* cc = &CASUS_COMPARATIONIS[k];
                          TomlComparatio  c = _comparare(opus, cc->toml,
                                                 cc->json);
                                     b32 bene;

            bene = cc->via == NIHIL ? c.aequalis
                : !c.aequalis
                    && _chorda_est(c.via_differentiae, cc->via)
                    && c.causa.mensura > ZEPHYRUM;
            si (!bene)
            {
                imprimere("    casus %u '%s': aequalis %d, via '%.*s', "
                    "causa '%.*s'\n", k, cc->toml, (integer)c.aequalis,
                    (integer)c.via_differentiae.mensura,
                    (constans character*)c.via_differentiae.datum,
                    (integer)c.causa.mensura,
                    (constans character*)c.causa.datum);
            }
            CREDO_VERUM (bene);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Corpus toml-test 1.0.0
     * ================================================== */

    {
                  Status st;
        TomlCorpusNumeri nn;

        imprimere("\n--- Probans corpus toml-test ---\n");
        memset(&st, ZEPHYRUM, magnitudo(st));
        st.radix     = radix != NIHIL ? radix : ".";
        st.omnia     = lectiones_ambitus("ORACULUM_OMNIA") != NIHIL;
        st.exemplum  = lectiones_ambitus("ORACULUM_EXEMPLUM");
        toml_corpus_ambulare(piscina, opus, st.radix, _visor, &st, &nn);
        imprimere("  valida paria %u/%u; invalida reiecta %u/%u "
            "(pinnae %d, %d)\n", st.paria, st.valida, st.reiecta,
            st.invalida, (integer)PINNA_VALIDA,
            (integer)PINNA_INVALIDA);
        CREDO_VERUM (nn.indices_lecti);
        CREDO_AEQUALIS_I32 (st.valida, (i32)CCV);
        CREDO_AEQUALIS_I32 (st.invalida, (i32)CDLXXIV);
        CREDO_MAIOR_AUT_AEQUALIS_I32 (st.paria, (i32)PINNA_VALIDA);
        CREDO_MAIOR_AUT_AEQUALIS_I32 (st.reiecta, (i32)PINNA_INVALIDA);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(opus);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
