/* probatio_nuntium_generare.c - Probationes Generationis Codicis ex Schemate
 *
 * Probat nuntium_schema_generare_caput() et nuntium_schema_generare_corpus()
 */

#include "latina.h"
#include "nuntium_schema.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Auxiliaris: verificare si chorda continet subchorda */
hic_manens b32
_continet(chorda s, constans character* acus)
{
    i32 acus_longitudo;
    i32 i;

    acus_longitudo = (i32)strlen(acus);
    si (acus_longitudo > s.mensura) redde FALSUM;

    per (i = ZEPHYRUM; i <= s.mensura - acus_longitudo; i++)
    {
        si (memcmp(s.datum + i, acus, (memoriae_index)acus_longitudo) == ZEPHYRUM)
            redde VERUM;
    }
    redde FALSUM;
}

s32 principale(vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_nuntium_generare", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Generare caput pro nuntio simplice
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus id = 1;"
            "  varint aetas = 2;"
            "  textus appellatio = 3;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda caput;

        imprimere("\n--- Probans I: generare caput simplex ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        caput = nuntium_schema_generare_caput(
            piscina, radix, "persona_gen.h");
        CREDO_VERUM(caput.mensura > ZEPHYRUM);

        /* Verificare custos */
        CREDO_VERUM(_continet(caput, "#ifndef PERSONA_GEN_H"));
        CREDO_VERUM(_continet(caput, "#define PERSONA_GEN_H"));

        /* Verificare includere */
        CREDO_VERUM(_continet(caput, "#include \"latina.h\""));
        CREDO_VERUM(_continet(caput, "#include \"nuntium.h\""));

        /* Verificare tag constantia */
        CREDO_VERUM(_continet(caput, "PERSONA_TAG_ID"));
        CREDO_VERUM(_continet(caput, "PERSONA_TAG_AETAS"));
        CREDO_VERUM(_continet(caput, "PERSONA_TAG_APPELLATIO"));

        /* Verificare struct */
        CREDO_VERUM(_continet(caput, "} Persona;"));
        CREDO_VERUM(_continet(caput, "chorda*"));
        CREDO_VERUM(_continet(caput, "i64"));

        /* Verificare prototypi */
        CREDO_VERUM(_continet(caput, "persona_scribere"));
        CREDO_VERUM(_continet(caput, "persona_legere"));

        /* Verificare clausura */
        CREDO_VERUM(_continet(caput, "#endif"));

        imprimere("  caput generatum -> OK\n");
    }

    /* ========================================================
     * PROBARE II: Generare corpus pro nuntio simplice
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus id = 1;"
            "  varint aetas = 2;"
            "  textus appellatio = 3;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda corpus;

        imprimere("\n--- Probans II: generare corpus simplex ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        corpus = nuntium_schema_generare_corpus(
            piscina, radix, "persona_gen.h");
        CREDO_VERUM(corpus.mensura > ZEPHYRUM);

        /* Verificare includere */
        CREDO_VERUM(_continet(corpus, "#include \"persona_gen.h\""));
        CREDO_VERUM(_continet(corpus, "#include <string.h>"));

        /* Verificare functio scribere */
        CREDO_VERUM(_continet(corpus, "persona_scribere"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_chorda"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_varint"));
        CREDO_VERUM(_continet(corpus, "PERSONA_TAG_ID"));
        CREDO_VERUM(_continet(corpus, "PERSONA_TAG_AETAS"));

        /* Verificare functio legere */
        CREDO_VERUM(_continet(corpus, "persona_legere"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_chorda"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_varint"));
        CREDO_VERUM(_continet(corpus, "piscina_allocare"));
        CREDO_VERUM(_continet(corpus, "memcpy"));
        CREDO_VERUM(_continet(corpus, "nuntium_saltare_field"));

        /* Verificare error verificatio */
        CREDO_VERUM(_continet(corpus, "nuntium_lector_error"));

        imprimere("  corpus generatum -> OK\n");
    }

    /* ========================================================
     * PROBARE III: Omnia VIII genera camporum
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Omnia {"
            "  textus  a = 1;"
            "  varint  b = 2;"
            "  svarint c = 3;"
            "  fixum32 d = 4;"
            "  fixum64 e = 5;"
            "  bytes   f = 6;"
            "  f32     g = 7;"
            "  f64     h = 8;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda caput;
        chorda corpus;

        imprimere("\n--- Probans III: omnia VIII genera camporum ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        caput = nuntium_schema_generare_caput(
            piscina, radix, "omnia_gen.h");
        CREDO_VERUM(caput.mensura > ZEPHYRUM);

        /* Verificare typi in struct */
        CREDO_VERUM(_continet(caput, "chorda*"));   /* textus, bytes */
        CREDO_VERUM(_continet(caput, "i64"));        /* varint, fixum64 */
        CREDO_VERUM(_continet(caput, "s64"));        /* svarint */
        CREDO_VERUM(_continet(caput, "i32"));        /* fixum32 */
        CREDO_VERUM(_continet(caput, "f32"));        /* f32 */
        CREDO_VERUM(_continet(caput, "f64"));        /* f64 */

        corpus = nuntium_schema_generare_corpus(
            piscina, radix, "omnia_gen.h");
        CREDO_VERUM(corpus.mensura > ZEPHYRUM);

        /* Verificare functiones scribere */
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_chorda"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_varint"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_svarint"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_32bit"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_64bit"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_bytes"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_f32"));
        CREDO_VERUM(_continet(corpus, "nuntium_scribere_f64"));

        /* Verificare functiones legere */
        CREDO_VERUM(_continet(corpus, "nuntium_legere_chorda"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_varint"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_svarint"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_32bit"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_64bit"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_bytes"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_f32"));
        CREDO_VERUM(_continet(corpus, "nuntium_legere_f64"));

        imprimere("  VIII genera -> OK\n");
    }

    /* ========================================================
     * PROBARE IV: Nuntia multiplicia in uno schemate
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus id = 1;"
            "  varint aetas = 2;"
            "}"
            "nuntium Ordo {"
            "  textus id = 1;"
            "  fixum64 pretium = 2;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda caput;
        chorda corpus;

        imprimere("\n--- Probans IV: nuntia multiplicia ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        caput = nuntium_schema_generare_caput(
            piscina, radix, "multi_gen.h");
        CREDO_VERUM(caput.mensura > ZEPHYRUM);

        /* Ambae struct */
        CREDO_VERUM(_continet(caput, "} Persona;"));
        CREDO_VERUM(_continet(caput, "} Ordo;"));

        /* Tag constantia pro ambabus */
        CREDO_VERUM(_continet(caput, "PERSONA_TAG_ID"));
        CREDO_VERUM(_continet(caput, "PERSONA_TAG_AETAS"));
        CREDO_VERUM(_continet(caput, "ORDO_TAG_ID"));
        CREDO_VERUM(_continet(caput, "ORDO_TAG_PRETIUM"));

        /* Prototypi pro ambabus */
        CREDO_VERUM(_continet(caput, "persona_scribere"));
        CREDO_VERUM(_continet(caput, "persona_legere"));
        CREDO_VERUM(_continet(caput, "ordo_scribere"));
        CREDO_VERUM(_continet(caput, "ordo_legere"));

        corpus = nuntium_schema_generare_corpus(
            piscina, radix, "multi_gen.h");
        CREDO_VERUM(corpus.mensura > ZEPHYRUM);

        /* Ambae functiones in corpore */
        CREDO_VERUM(_continet(corpus, "persona_scribere"));
        CREDO_VERUM(_continet(corpus, "persona_legere"));
        CREDO_VERUM(_continet(corpus, "ordo_scribere"));
        CREDO_VERUM(_continet(corpus, "ordo_legere"));

        imprimere("  nuntia multiplicia -> OK\n");
    }

    /* ========================================================
     * PROBARE V: Input invalidum -> chorda vacua
     * ======================================================== */
    {
        chorda caput;
        chorda corpus;

        imprimere("\n--- Probans V: input invalidum ---\n");

        caput = nuntium_schema_generare_caput(piscina, NIHIL, "test.h");
        CREDO_VERUM(caput.mensura == ZEPHYRUM);

        corpus = nuntium_schema_generare_corpus(piscina, NIHIL, "test.h");
        CREDO_VERUM(corpus.mensura == ZEPHYRUM);

        imprimere("  input invalidum -> chorda vacua -> OK\n");
    }

    /* ========================================================
     * PROBARE VI: Tag numeri in tag constantia
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Magnum {"
            "  varint campus_primus = 100;"
            "  textus campus_secundus = 255;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda caput;

        imprimere("\n--- Probans VI: tag numeri ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        caput = nuntium_schema_generare_caput(
            piscina, radix, "magnum_gen.h");
        CREDO_VERUM(caput.mensura > ZEPHYRUM);

        /* Tag numeri debent esse in output */
        CREDO_VERUM(_continet(caput, "MAGNUM_TAG_CAMPUS_PRIMUS"));
        CREDO_VERUM(_continet(caput, "MAGNUM_TAG_CAMPUS_SECUNDUS"));
        CREDO_VERUM(_continet(caput, "100"));
        CREDO_VERUM(_continet(caput, "255"));

        imprimere("  tag numeri -> OK\n");
    }

    /* ========================================================
     * PROBARE VII: Struct campi in corpore habent nomina originalia
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus appellatio = 1;"
            "  varint aetas = 2;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda corpus;

        imprimere("\n--- Probans VII: nomina camporum originalia in corpore ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        corpus = nuntium_schema_generare_corpus(
            piscina, radix, "persona_gen.h");

        /* Struct campi nomina originalia (minuscula) */
        CREDO_VERUM(_continet(corpus, "nuntium->appellatio"));
        CREDO_VERUM(_continet(corpus, "nuntium->aetas"));
        CREDO_VERUM(_continet(corpus, "n->appellatio"));
        CREDO_VERUM(_continet(corpus, "n->aetas"));

        /* Tag constantia maiuscula */
        CREDO_VERUM(_continet(corpus, "PERSONA_TAG_APPELLATIO"));
        CREDO_VERUM(_continet(corpus, "PERSONA_TAG_AETAS"));

        imprimere("  nomina originalia -> OK\n");
    }

    /* ========================================================
     * PROBARE VIII: Caput habet struct cum campis et padding
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus id = 1;"
            "  varint aetas = 2;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda caput;

        imprimere("\n--- Probans VIII: struct campi in capite ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        caput = nuntium_schema_generare_caput(
            piscina, radix, "persona_gen.h");

        /* Struct habet campi nomina originalia */
        CREDO_VERUM(_continet(caput, "id;"));
        CREDO_VERUM(_continet(caput, "aetas;"));

        imprimere("  struct campi -> OK\n");
    }

    /* ========================================================
     * PROBARE IX: Scribere corpus continet null-guard pro chorda
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus appellatio = 1;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda corpus;

        imprimere("\n--- Probans IX: null-guard pro chorda in scribere ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        corpus = nuntium_schema_generare_corpus(
            piscina, radix, "persona_gen.h");

        /* Chorda campi habent null guard */
        CREDO_VERUM(_continet(corpus, "si (nuntium->appellatio)"));

        imprimere("  null-guard -> OK\n");
    }

    /* ========================================================
     * PROBARE X: Legere corpus continet memset ad zephyrum
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus id = 1;"
            "}";
        NuntiumSchemaNodus* radix;
        chorda corpus;

        imprimere("\n--- Probans X: memset in legere ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        corpus = nuntium_schema_generare_corpus(
            piscina, radix, "persona_gen.h");

        /* Legere incipit cum allocatione et memset */
        CREDO_VERUM(_continet(corpus, "piscina_allocare(piscina"));
        CREDO_VERUM(_continet(corpus, "memset(n, 0"));

        imprimere("  memset -> OK\n");
    }

    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}
