/* probatio_nuntium_schema.c - Probationes Linguae Schematis Nuntii (Phase 6) */

#include "latina.h"
#include "nuntium_schema.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Auxiliaris: obtinere definitio ex radice per indicem */
hic_manens NuntiumSchemaNodus*
_definitio_obtinere(NuntiumSchemaNodus* radix, i32 index)
{
    NuntiumSchemaNodus** locus;
    si (!radix || radix->genus != NUNTIUM_SCHEMA_NODUS_SCHEMA) redde NIHIL;
    locus = (NuntiumSchemaNodus**)xar_obtinere(
        radix->datum.schema.definitiones, index);
    si (!locus) redde NIHIL;
    redde *locus;
}

/* Auxiliaris: obtinere campus ex definitio per indicem */
hic_manens NuntiumSchemaNodus*
_campus_obtinere(NuntiumSchemaNodus* def, i32 index)
{
    NuntiumSchemaNodus** locus;
    si (!def || def->genus != NUNTIUM_SCHEMA_NODUS_DEFINITIO) redde NIHIL;
    locus = (NuntiumSchemaNodus**)xar_obtinere(
        def->datum.definitio.campi, index);
    si (!locus) redde NIHIL;
    redde *locus;
}

s32 principale(vacuum)
{
    Piscina*             piscina;
    NuntiumSchemaNodus*  radix;

    piscina = piscina_generare_dynamicum("probatio_nuntium_schema", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Nuntium simplex cum uno campo
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona { textus identificator = 1; }";
        NuntiumSchemaNodus* def;
        NuntiumSchemaNodus* campus;

        imprimere("\n--- Probans I: nuntium simplex cum uno campo ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);
        CREDO_AEQUALIS_S32((s32)radix->genus, (s32)NUNTIUM_SCHEMA_NODUS_SCHEMA);
        CREDO_AEQUALIS_I32(xar_numerus(radix->datum.schema.definitiones), I);

        def = _definitio_obtinere(radix, ZEPHYRUM);
        CREDO_NON_NIHIL(def);
        CREDO_AEQUALIS_S32((s32)def->genus, (s32)NUNTIUM_SCHEMA_NODUS_DEFINITIO);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def->datum.definitio.titulus, "Persona");
        CREDO_AEQUALIS_I32(xar_numerus(def->datum.definitio.campi), I);

        campus = _campus_obtinere(def, ZEPHYRUM);
        CREDO_NON_NIHIL(campus);
        CREDO_AEQUALIS_S32((s32)campus->genus, (s32)NUNTIUM_SCHEMA_NODUS_CAMPUS);
        CREDO_AEQUALIS_S32((s32)campus->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_TEXTUS);
        CREDO_CHORDA_AEQUALIS_LITERIS(*campus->datum.campus.titulus, "identificator");
        CREDO_AEQUALIS_S32(campus->datum.campus.tag, I);

        imprimere("  nuntium Persona { textus identificator = 1; } -> OK\n");
    }

    /* ========================================================
     * PROBARE II: Nuntium cum campis multiplicibus
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus identificator = 1;"
            "  varint aetas = 2;"
            "  textus titulus_campi = 3;"
            "}";
        NuntiumSchemaNodus* def;
        NuntiumSchemaNodus* c0;
        NuntiumSchemaNodus* c1;
        NuntiumSchemaNodus* c2;

        imprimere("\n--- Probans II: nuntium cum campis multiplicibus ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        def = _definitio_obtinere(radix, ZEPHYRUM);
        CREDO_NON_NIHIL(def);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def->datum.definitio.titulus, "Persona");
        CREDO_AEQUALIS_I32(xar_numerus(def->datum.definitio.campi), III);

        c0 = _campus_obtinere(def, ZEPHYRUM);
        CREDO_AEQUALIS_S32((s32)c0->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_TEXTUS);
        CREDO_CHORDA_AEQUALIS_LITERIS(*c0->datum.campus.titulus, "identificator");
        CREDO_AEQUALIS_S32(c0->datum.campus.tag, I);

        c1 = _campus_obtinere(def, I);
        CREDO_AEQUALIS_S32((s32)c1->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_VARINT);
        CREDO_CHORDA_AEQUALIS_LITERIS(*c1->datum.campus.titulus, "aetas");
        CREDO_AEQUALIS_S32(c1->datum.campus.tag, II);

        c2 = _campus_obtinere(def, II);
        CREDO_AEQUALIS_S32((s32)c2->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_TEXTUS);
        CREDO_CHORDA_AEQUALIS_LITERIS(*c2->datum.campus.titulus, "titulus_campi");
        CREDO_AEQUALIS_S32(c2->datum.campus.tag, III);

        imprimere("  3 campi: textus, varint, textus -> OK\n");
    }

    /* ========================================================
     * PROBARE III: Nuntia multiplicia
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona {"
            "  textus id = 1;"
            "  varint aetas = 2;"
            "}"
            "nuntium Ordo {"
            "  textus id = 1;"
            "  varint genus_ordinis = 2;"
            "}";
        NuntiumSchemaNodus* def0;
        NuntiumSchemaNodus* def1;

        imprimere("\n--- Probans III: nuntia multiplicia ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);
        CREDO_AEQUALIS_I32(xar_numerus(radix->datum.schema.definitiones), II);

        def0 = _definitio_obtinere(radix, ZEPHYRUM);
        CREDO_NON_NIHIL(def0);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def0->datum.definitio.titulus, "Persona");
        CREDO_AEQUALIS_I32(xar_numerus(def0->datum.definitio.campi), II);

        def1 = _definitio_obtinere(radix, I);
        CREDO_NON_NIHIL(def1);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def1->datum.definitio.titulus, "Ordo");
        CREDO_AEQUALIS_I32(xar_numerus(def1->datum.definitio.campi), II);

        imprimere("  Persona + Ordo -> OK\n");
    }

    /* ========================================================
     * PROBARE IV: Omnia VIII genera camporum
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
        NuntiumSchemaNodus* def;
        NuntiumSchemaNodus* c;

        imprimere("\n--- Probans IV: omnia VIII genera camporum ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        def = _definitio_obtinere(radix, ZEPHYRUM);
        CREDO_NON_NIHIL(def);
        CREDO_AEQUALIS_I32(xar_numerus(def->datum.definitio.campi), VIII);

        c = _campus_obtinere(def, ZEPHYRUM);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_TEXTUS);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, I);

        c = _campus_obtinere(def, I);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_VARINT);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, II);

        c = _campus_obtinere(def, II);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_SVARINT);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, III);

        c = _campus_obtinere(def, III);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_FIXUM32);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, IV);

        c = _campus_obtinere(def, IV);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_FIXUM64);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, V);

        c = _campus_obtinere(def, V);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_BYTES);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, VI);

        c = _campus_obtinere(def, VI);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_F32);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, VII);

        c = _campus_obtinere(def, VII);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_F64);
        CREDO_AEQUALIS_S32(c->datum.campus.tag, VIII);

        imprimere("  textus, varint, svarint, fixum32, fixum64, bytes, f32, f64 -> OK\n");
    }

    /* ========================================================
     * PROBARE V: Tag numeri maiores
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Magnum {"
            "  varint campus_primus = 100;"
            "  textus campus_secundus = 255;"
            "}";
        NuntiumSchemaNodus* def;
        NuntiumSchemaNodus* c0;
        NuntiumSchemaNodus* c1;

        imprimere("\n--- Probans V: tag numeri maiores ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        def = _definitio_obtinere(radix, ZEPHYRUM);
        CREDO_NON_NIHIL(def);

        c0 = _campus_obtinere(def, ZEPHYRUM);
        CREDO_AEQUALIS_S32(c0->datum.campus.tag, C);

        c1 = _campus_obtinere(def, I);
        CREDO_AEQUALIS_S32(c1->datum.campus.tag, CCLV);

        imprimere("  tag 100, 255 -> OK\n");
    }

    /* ========================================================
     * PROBARE VI: Input vacuum -> NIHIL
     * ======================================================== */
    {
        imprimere("\n--- Probans VI: input vacuum -> NIHIL ---\n");

        radix = nuntium_schema_legere(piscina, "", ZEPHYRUM);
        CREDO_NIHIL(radix);

        radix = nuntium_schema_legere(piscina, NIHIL, ZEPHYRUM);
        CREDO_NIHIL(radix);

        imprimere("  vacuum -> NIHIL -> OK\n");
    }

    /* ========================================================
     * PROBARE VII: Error — semicolon deficiens
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona { textus id = 1 }";

        imprimere("\n--- Probans VII: semicolon deficiens -> error ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NIHIL(radix);

        imprimere("  semicolon deficiens -> NIHIL -> OK\n");
    }

    /* ========================================================
     * PROBARE VIII: Error — character ignotus
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Persona { textus id @ 1; }";

        imprimere("\n--- Probans VIII: character ignotus -> error ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NIHIL(radix);

        imprimere("  character ignotus -> NIHIL -> OK\n");
    }

    /* ========================================================
     * PROBARE IX: Spatium copiosum et lineae novae
     * ======================================================== */
    {
        constans character* fons =
            "\n\n  nuntium  Persona  {\n"
            "    textus   id   =   1  ;\n"
            "    varint   aetas   =   2  ;\n"
            "  }\n\n";
        NuntiumSchemaNodus* def;

        imprimere("\n--- Probans IX: spatium copiosum et lineae novae ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);

        def = _definitio_obtinere(radix, ZEPHYRUM);
        CREDO_NON_NIHIL(def);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def->datum.definitio.titulus, "Persona");
        CREDO_AEQUALIS_I32(xar_numerus(def->datum.definitio.campi), II);

        imprimere("  spatium copiosum -> OK\n");
    }

    /* ========================================================
     * PROBARE X: Tria nuntia (end-to-end complexus)
     * ======================================================== */
    {
        constans character* fons =
            "nuntium Eventum {"
            "  varint genus_eventum = 1;"
            "  varint tempus = 2;"
            "  textus descriptio = 3;"
            "  bytes datum_binarium = 4;"
            "}"
            "nuntium Persona {"
            "  textus id = 1;"
            "  textus appellatio = 2;"
            "  varint aetas = 3;"
            "}"
            "nuntium Ordo {"
            "  textus id = 1;"
            "  fixum64 pretium = 2;"
            "}";
        NuntiumSchemaNodus* def0;
        NuntiumSchemaNodus* def1;
        NuntiumSchemaNodus* def2;
        NuntiumSchemaNodus* c;

        imprimere("\n--- Probans X: tria nuntia (end-to-end) ---\n");

        radix = nuntium_schema_legere(piscina, fons, (s32)strlen(fons));
        CREDO_NON_NIHIL(radix);
        CREDO_AEQUALIS_I32(xar_numerus(radix->datum.schema.definitiones), III);

        /* Eventum: 4 campi */
        def0 = _definitio_obtinere(radix, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def0->datum.definitio.titulus, "Eventum");
        CREDO_AEQUALIS_I32(xar_numerus(def0->datum.definitio.campi), IV);

        c = _campus_obtinere(def0, ZEPHYRUM);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_VARINT);
        CREDO_CHORDA_AEQUALIS_LITERIS(*c->datum.campus.titulus, "genus_eventum");

        c = _campus_obtinere(def0, III);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_BYTES);
        CREDO_CHORDA_AEQUALIS_LITERIS(*c->datum.campus.titulus, "datum_binarium");
        CREDO_AEQUALIS_S32(c->datum.campus.tag, IV);

        /* Persona: 3 campi */
        def1 = _definitio_obtinere(radix, I);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def1->datum.definitio.titulus, "Persona");
        CREDO_AEQUALIS_I32(xar_numerus(def1->datum.definitio.campi), III);

        /* Ordo: 2 campi */
        def2 = _definitio_obtinere(radix, II);
        CREDO_CHORDA_AEQUALIS_LITERIS(*def2->datum.definitio.titulus, "Ordo");
        CREDO_AEQUALIS_I32(xar_numerus(def2->datum.definitio.campi), II);

        c = _campus_obtinere(def2, I);
        CREDO_AEQUALIS_S32((s32)c->datum.campus.campus_genus, (s32)NUNTIUM_CAMPUS_FIXUM64);
        CREDO_CHORDA_AEQUALIS_LITERIS(*c->datum.campus.titulus, "pretium");
        CREDO_AEQUALIS_S32(c->datum.campus.tag, II);

        imprimere("  Eventum(4) + Persona(3) + Ordo(2) -> OK\n");
    }

    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}
