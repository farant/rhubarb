/* nuntium_schema_generare.c - Generatio codicis C ex schemate nuntii
 *
 * Ambulat AST schematis et emittit .h et .c fontes.
 * Struct definitionem, tag constantia, _scribere() et _legere() functiones.
 */

#include "nuntium_schema.h"
#include "chorda_aedificator.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <string.h>
#include <stdio.h>

/* ================================================
 * Functiones Auxiliares Internae
 * ================================================ */

/* Reddere C typum pro genere campi */
hic_manens constans character*
_genus_ad_c_typum(NuntiumCampusGenus genus)
{
    commutatio (genus)
    {
        casus NUNTIUM_CAMPUS_TEXTUS:  redde "chorda*";
        casus NUNTIUM_CAMPUS_VARINT:  redde "i64";
        casus NUNTIUM_CAMPUS_SVARINT: redde "s64";
        casus NUNTIUM_CAMPUS_FIXUM32: redde "i32";
        casus NUNTIUM_CAMPUS_FIXUM64: redde "i64";
        casus NUNTIUM_CAMPUS_BYTES:   redde "chorda*";
        casus NUNTIUM_CAMPUS_F32:     redde "f32";
        casus NUNTIUM_CAMPUS_F64:     redde "f64";
        ordinarius:                   redde "i64";
    }
}

/* Verificare si genus est chorda/bytes (index nullable) */
hic_manens b32
_genus_est_chorda(NuntiumCampusGenus genus)
{
    redde (genus == NUNTIUM_CAMPUS_TEXTUS || genus == NUNTIUM_CAMPUS_BYTES);
}

/* Longitudo maxima typi C pro padding tabulari */
hic_manens i32
_longitudo_maxima_typi(NuntiumSchemaNodus* definitio)
{
    i32 maxima;
    i32 i;
    i32 numerus_camporum;

    maxima = ZEPHYRUM;
    numerus_camporum = xar_numerus(definitio->datum.definitio.campi);

    per (i = ZEPHYRUM; i < numerus_camporum; i++)
    {
        NuntiumSchemaNodus** locus;
        constans character* typus;
        i32 longitudo;

        locus = (NuntiumSchemaNodus**)xar_obtinere(
            definitio->datum.definitio.campi, i);
        si (!locus) perge;

        typus = _genus_ad_c_typum((*locus)->datum.campus.campus_genus);
        longitudo = (i32)strlen(typus);
        si (longitudo > maxima) maxima = longitudo;
    }

    redde maxima;
}

/* Emittere unam lineam scribere pro campo */
hic_manens vacuum
_emittere_scribere_campum(
    ChordaAedificator* aed,
    NuntiumSchemaNodus* campus,
    constans character* prefixum_tag,
    constans character* tag_suffixum,
    constans character* campus_titulis)
{
    NuntiumCampusGenus genus;
    i32 gradus;

    genus = campus->datum.campus.campus_genus;
    gradus = chorda_aedificator_indentatio_gradus(aed);

    si (_genus_est_chorda(genus))
    {
        /* Campi chorda* sunt nullable — solum scribere si non NIHIL */
        chorda_aedificator_appendere_indentationem(aed, gradus);
        chorda_aedificator_appendere_literis(aed, "si (nuntium->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed, ")\n");

        chorda_aedificator_appendere_indentationem(aed, gradus);
        chorda_aedificator_appendere_literis(aed, "{\n");
        chorda_aedificator_push_indentationem(aed);

        chorda_aedificator_appendere_indentationem(aed,
            chorda_aedificator_indentatio_gradus(aed));

        si (genus == NUNTIUM_CAMPUS_TEXTUS)
        {
            chorda_aedificator_appendere_literis(aed,
                "si (!nuntium_scribere_chorda(scriptor, ");
            chorda_aedificator_appendere_literis(aed, prefixum_tag);
            chorda_aedificator_appendere_literis(aed, tag_suffixum);
            chorda_aedificator_appendere_literis(aed,
                ", *nuntium->");
            chorda_aedificator_appendere_literis(aed, campus_titulis);
            chorda_aedificator_appendere_literis(aed, "))\n");
        }
        alioquin
        {
            /* bytes */
            chorda_aedificator_appendere_literis(aed,
                "si (!nuntium_scribere_bytes(scriptor, ");
            chorda_aedificator_appendere_literis(aed, prefixum_tag);
            chorda_aedificator_appendere_literis(aed, tag_suffixum);
            chorda_aedificator_appendere_literis(aed, ", nuntium->");
            chorda_aedificator_appendere_literis(aed, campus_titulis);
            chorda_aedificator_appendere_literis(aed, "->datum, nuntium->");
            chorda_aedificator_appendere_literis(aed, campus_titulis);
            chorda_aedificator_appendere_literis(aed, "->mensura))\n");
        }

        chorda_aedificator_appendere_indentationem(aed,
            chorda_aedificator_indentatio_gradus(aed));
        chorda_aedificator_appendere_literis(aed, "    redde FALSUM;\n");

        chorda_aedificator_pop_indentationem(aed);
        chorda_aedificator_appendere_indentationem(aed, gradus);
        chorda_aedificator_appendere_literis(aed, "}\n");
    }
    alioquin
    {
        /* Campi numerici — semper scribere */
        constans character* functio;

        commutatio (genus)
        {
            casus NUNTIUM_CAMPUS_VARINT:  functio = "nuntium_scribere_varint"; frange;
            casus NUNTIUM_CAMPUS_SVARINT: functio = "nuntium_scribere_svarint"; frange;
            casus NUNTIUM_CAMPUS_FIXUM32: functio = "nuntium_scribere_32bit"; frange;
            casus NUNTIUM_CAMPUS_FIXUM64: functio = "nuntium_scribere_64bit"; frange;
            casus NUNTIUM_CAMPUS_F32:     functio = "nuntium_scribere_f32"; frange;
            casus NUNTIUM_CAMPUS_F64:     functio = "nuntium_scribere_f64"; frange;
            ordinarius:                   functio = "nuntium_scribere_varint"; frange;
        }

        chorda_aedificator_appendere_indentationem(aed, gradus);
        chorda_aedificator_appendere_literis(aed, "si (!");
        chorda_aedificator_appendere_literis(aed, functio);
        chorda_aedificator_appendere_literis(aed, "(scriptor, ");
        chorda_aedificator_appendere_literis(aed, prefixum_tag);
        chorda_aedificator_appendere_literis(aed, tag_suffixum);
        chorda_aedificator_appendere_literis(aed, ", nuntium->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed, "))\n");

        chorda_aedificator_appendere_indentationem(aed, gradus);
        chorda_aedificator_appendere_literis(aed, "    redde FALSUM;\n");
    }
}

/* Emittere unum casus legere pro campo */
hic_manens vacuum
_emittere_legere_campum(
    ChordaAedificator* aed,
    NuntiumSchemaNodus* campus,
    constans character* prefixum_tag,
    constans character* tag_suffixum,
    constans character* campus_titulis,
    constans character* variabilis)
{
    NuntiumCampusGenus genus;
    i32 gradus;
    i32 g2;
    i32 g3;
    i32 g4;

    genus = campus->datum.campus.campus_genus;
    gradus = chorda_aedificator_indentatio_gradus(aed);

    chorda_aedificator_appendere_indentationem(aed, gradus);
    chorda_aedificator_appendere_literis(aed, "casus ");
    chorda_aedificator_appendere_literis(aed, prefixum_tag);
    chorda_aedificator_appendere_literis(aed, tag_suffixum);
    chorda_aedificator_appendere_literis(aed, ":\n");

    si (_genus_est_chorda(genus))
    {
        /* Chorda/bytes: legere, allocare copia in piscina */
        chorda_aedificator_appendere_indentationem(aed, gradus);
        chorda_aedificator_appendere_literis(aed, "{\n");
        chorda_aedificator_push_indentationem(aed);

        g2 = chorda_aedificator_indentatio_gradus(aed);

        si (genus == NUNTIUM_CAMPUS_TEXTUS)
        {
            chorda_aedificator_appendere_indentationem(aed, g2);
            chorda_aedificator_appendere_literis(aed,
                "chorda tmp = nuntium_legere_chorda(lector);\n");
        }
        alioquin
        {
            chorda_aedificator_appendere_indentationem(aed, g2);
            chorda_aedificator_appendere_literis(aed,
                "chorda tmp = nuntium_legere_bytes(lector);\n");
        }

        chorda_aedificator_appendere_indentationem(aed, g2);
        chorda_aedificator_appendere_literis(aed, variabilis);
        chorda_aedificator_appendere_literis(aed, "->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed,
            " = (chorda*)piscina_allocare(piscina, "
            "(memoriae_index)magnitudo(chorda));\n");

        chorda_aedificator_appendere_indentationem(aed, g2);
        chorda_aedificator_appendere_literis(aed, "si (");
        chorda_aedificator_appendere_literis(aed, variabilis);
        chorda_aedificator_appendere_literis(aed, "->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed, ")\n");

        chorda_aedificator_appendere_indentationem(aed, g2);
        chorda_aedificator_appendere_literis(aed, "{\n");
        chorda_aedificator_push_indentationem(aed);

        g3 = chorda_aedificator_indentatio_gradus(aed);

        chorda_aedificator_appendere_indentationem(aed, g3);
        chorda_aedificator_appendere_literis(aed, variabilis);
        chorda_aedificator_appendere_literis(aed, "->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed,
            "->datum = (i8*)piscina_allocare(piscina, "
            "(memoriae_index)tmp.mensura);\n");

        chorda_aedificator_appendere_indentationem(aed, g3);
        chorda_aedificator_appendere_literis(aed, "si (");
        chorda_aedificator_appendere_literis(aed, variabilis);
        chorda_aedificator_appendere_literis(aed, "->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed, "->datum)\n");

        chorda_aedificator_appendere_indentationem(aed, g3);
        chorda_aedificator_appendere_literis(aed, "{\n");
        chorda_aedificator_push_indentationem(aed);

        g4 = chorda_aedificator_indentatio_gradus(aed);

        chorda_aedificator_appendere_indentationem(aed, g4);
        chorda_aedificator_appendere_literis(aed, "memcpy(");
        chorda_aedificator_appendere_literis(aed, variabilis);
        chorda_aedificator_appendere_literis(aed, "->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed,
            "->datum, tmp.datum, (memoriae_index)tmp.mensura);\n");

        chorda_aedificator_appendere_indentationem(aed, g4);
        chorda_aedificator_appendere_literis(aed, variabilis);
        chorda_aedificator_appendere_literis(aed, "->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed,
            "->mensura = tmp.mensura;\n");

        chorda_aedificator_pop_indentationem(aed);
        chorda_aedificator_appendere_indentationem(aed, g3);
        chorda_aedificator_appendere_literis(aed, "}\n");

        chorda_aedificator_pop_indentationem(aed);
        chorda_aedificator_appendere_indentationem(aed, g2);
        chorda_aedificator_appendere_literis(aed, "}\n");

        chorda_aedificator_appendere_indentationem(aed, g2);
        chorda_aedificator_appendere_literis(aed, "frange;\n");

        chorda_aedificator_pop_indentationem(aed);
        chorda_aedificator_appendere_indentationem(aed, gradus);
        chorda_aedificator_appendere_literis(aed, "}\n");
    }
    alioquin
    {
        /* Campi numerici */
        constans character* functio;

        commutatio (genus)
        {
            casus NUNTIUM_CAMPUS_VARINT:  functio = "nuntium_legere_varint"; frange;
            casus NUNTIUM_CAMPUS_SVARINT: functio = "nuntium_legere_svarint"; frange;
            casus NUNTIUM_CAMPUS_FIXUM32: functio = "nuntium_legere_32bit"; frange;
            casus NUNTIUM_CAMPUS_FIXUM64: functio = "nuntium_legere_64bit"; frange;
            casus NUNTIUM_CAMPUS_F32:     functio = "nuntium_legere_f32"; frange;
            casus NUNTIUM_CAMPUS_F64:     functio = "nuntium_legere_f64"; frange;
            ordinarius:                   functio = "nuntium_legere_varint"; frange;
        }

        chorda_aedificator_push_indentationem(aed);
        g2 = chorda_aedificator_indentatio_gradus(aed);

        chorda_aedificator_appendere_indentationem(aed, g2);
        chorda_aedificator_appendere_literis(aed, variabilis);
        chorda_aedificator_appendere_literis(aed, "->");
        chorda_aedificator_appendere_literis(aed, campus_titulis);
        chorda_aedificator_appendere_literis(aed, " = ");
        chorda_aedificator_appendere_literis(aed, functio);
        chorda_aedificator_appendere_literis(aed, "(lector);\n");

        chorda_aedificator_appendere_indentationem(aed, g2);
        chorda_aedificator_appendere_literis(aed, "frange;\n");

        chorda_aedificator_pop_indentationem(aed);
    }
}

/* ================================================
 * Generatio Capitis (.h)
 * ================================================ */

chorda
nuntium_schema_generare_caput(
    Piscina*            piscina,
    NuntiumSchemaNodus* radix,
    constans character* caput_titulis)
{
    ChordaAedificator* aed;
    chorda titulis_chorda;
    chorda custos;
    i32 i;
    i32 numerus_definitionum;

    si (!piscina || !radix || !caput_titulis) {
        chorda vacua;
        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde vacua;
    }

    aed = chorda_aedificator_creare(piscina, MMMMXCVI);
    si (!aed) {
        chorda vacua;
        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde vacua;
    }

    /* Construere custos macro ex nomine fili */
    titulis_chorda = chorda_ex_literis(caput_titulis, piscina);
    custos = chorda_maiuscula(titulis_chorda, piscina);

    /* Substituere '.' et '-' cum '_' */
    {
        i32 j;
        per (j = ZEPHYRUM; j < custos.mensura; j++)
        {
            si (custos.datum[j] == '.' || custos.datum[j] == '-')
                custos.datum[j] = '_';
        }
    }

    /* Commentarium */
    chorda_aedificator_appendere_literis(aed,
        "/* Auto-generatum ex schemate nuntii */\n");

    /* Custos #ifndef */
    {
        character* custos_cstr;
        custos_cstr = chorda_ut_cstr(custos, piscina);

        chorda_aedificator_appendere_literis(aed, "#ifndef ");
        chorda_aedificator_appendere_literis(aed, custos_cstr);
        chorda_aedificator_appendere_literis(aed, "\n");
        chorda_aedificator_appendere_literis(aed, "#define ");
        chorda_aedificator_appendere_literis(aed, custos_cstr);
        chorda_aedificator_appendere_literis(aed, "\n\n");
    }

    /* Includere */
    chorda_aedificator_appendere_literis(aed, "#include \"latina.h\"\n");
    chorda_aedificator_appendere_literis(aed, "#include \"chorda.h\"\n");
    chorda_aedificator_appendere_literis(aed, "#include \"piscina.h\"\n");
    chorda_aedificator_appendere_literis(aed, "#include \"nuntium.h\"\n\n");

    numerus_definitionum = xar_numerus(radix->datum.schema.definitiones);

    per (i = ZEPHYRUM; i < numerus_definitionum; i++)
    {
        NuntiumSchemaNodus** def_locus;
        NuntiumSchemaNodus* definitio;
        character* titulus_maiuscula_cstr;
        character* titulus_minuscula_cstr;
        character* titulus_cstr;
        chorda titulus_maiuscula;
        chorda titulus_minuscula;
        i32 j;
        i32 numerus_camporum;
        i32 maxima_typi;

        def_locus = (NuntiumSchemaNodus**)xar_obtinere(
            radix->datum.schema.definitiones, i);
        si (!def_locus) perge;
        definitio = *def_locus;

        titulus_cstr = chorda_ut_cstr(
            *definitio->datum.definitio.titulus, piscina);
        titulus_maiuscula = chorda_maiuscula(
            *definitio->datum.definitio.titulus, piscina);
        titulus_minuscula = chorda_minuscula(
            *definitio->datum.definitio.titulus, piscina);
        titulus_maiuscula_cstr = chorda_ut_cstr(titulus_maiuscula, piscina);
        titulus_minuscula_cstr = chorda_ut_cstr(titulus_minuscula, piscina);

        numerus_camporum = xar_numerus(definitio->datum.definitio.campi);

        /* Tag constantia */
        per (j = ZEPHYRUM; j < numerus_camporum; j++)
        {
            NuntiumSchemaNodus** c_locus;
            NuntiumSchemaNodus* campus;
            character* campus_titulis_cstr;
            chorda campus_maiuscula;

            c_locus = (NuntiumSchemaNodus**)xar_obtinere(
                definitio->datum.definitio.campi, j);
            si (!c_locus) perge;
            campus = *c_locus;

            campus_maiuscula = chorda_maiuscula(
                *campus->datum.campus.titulus, piscina);
            campus_titulis_cstr = chorda_ut_cstr(campus_maiuscula, piscina);

            chorda_aedificator_appendere_literis(aed, "#define ");
            chorda_aedificator_appendere_literis(aed, titulus_maiuscula_cstr);
            chorda_aedificator_appendere_literis(aed, "_TAG_");
            chorda_aedificator_appendere_literis(aed, campus_titulis_cstr);

            /* Padding ad tag numerum */
            chorda_aedificator_appendere_literis(aed, " ");
            chorda_aedificator_appendere_s32(aed, campus->datum.campus.tag);
            chorda_aedificator_appendere_literis(aed, "\n");
        }
        chorda_aedificator_appendere_literis(aed, "\n");

        /* Struct definitio */
        chorda_aedificator_appendere_literis(aed, "nomen structura {\n");

        maxima_typi = _longitudo_maxima_typi(definitio);

        per (j = ZEPHYRUM; j < numerus_camporum; j++)
        {
            NuntiumSchemaNodus** c_locus;
            NuntiumSchemaNodus* campus;
            character* campus_titulis_cstr;
            constans character* c_typus;
            i32 typus_longitudo;
            i32 k;

            c_locus = (NuntiumSchemaNodus**)xar_obtinere(
                definitio->datum.definitio.campi, j);
            si (!c_locus) perge;
            campus = *c_locus;

            campus_titulis_cstr = chorda_ut_cstr(
                *campus->datum.campus.titulus, piscina);
            c_typus = _genus_ad_c_typum(campus->datum.campus.campus_genus);
            typus_longitudo = (i32)strlen(c_typus);

            chorda_aedificator_appendere_literis(aed, "    ");
            chorda_aedificator_appendere_literis(aed, c_typus);

            /* Padding post typum */
            per (k = typus_longitudo; k < maxima_typi + I; k++)
                chorda_aedificator_appendere_character(aed, ' ');

            chorda_aedificator_appendere_literis(aed, campus_titulis_cstr);
            chorda_aedificator_appendere_literis(aed, ";\n");
        }

        chorda_aedificator_appendere_literis(aed, "} ");
        chorda_aedificator_appendere_literis(aed, titulus_cstr);
        chorda_aedificator_appendere_literis(aed, ";\n\n");

        /* Prototypi functionum */
        chorda_aedificator_appendere_literis(aed, "b32 ");
        chorda_aedificator_appendere_literis(aed, titulus_minuscula_cstr);
        chorda_aedificator_appendere_literis(aed,
            "_scribere(NuntiumScriptor* scriptor, constans ");
        chorda_aedificator_appendere_literis(aed, titulus_cstr);
        chorda_aedificator_appendere_literis(aed, "* nuntium);\n");

        chorda_aedificator_appendere_literis(aed, titulus_cstr);
        chorda_aedificator_appendere_literis(aed, "* ");
        chorda_aedificator_appendere_literis(aed, titulus_minuscula_cstr);
        chorda_aedificator_appendere_literis(aed,
            "_legere(NuntiumLector* lector, Piscina* piscina);\n");

        si (i < numerus_definitionum - I)
            chorda_aedificator_appendere_literis(aed, "\n");
    }

    /* Clausura custos */
    {
        character* custos_cstr;
        custos_cstr = chorda_ut_cstr(custos, piscina);

        chorda_aedificator_appendere_literis(aed, "\n#endif /* ");
        chorda_aedificator_appendere_literis(aed, custos_cstr);
        chorda_aedificator_appendere_literis(aed, " */\n");
    }

    redde chorda_aedificator_finire(aed);
}

/* ================================================
 * Generatio Corporis (.c)
 * ================================================ */

chorda
nuntium_schema_generare_corpus(
    Piscina*            piscina,
    NuntiumSchemaNodus* radix,
    constans character* caput_titulis)
{
    ChordaAedificator* aed;
    i32 i;
    i32 numerus_definitionum;

    si (!piscina || !radix || !caput_titulis) {
        chorda vacua;
        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde vacua;
    }

    aed = chorda_aedificator_creare(piscina, MMMMXCVI);
    si (!aed) {
        chorda vacua;
        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde vacua;
    }

    /* Commentarium et includere */
    chorda_aedificator_appendere_literis(aed,
        "/* Auto-generatum ex schemate nuntii */\n");
    chorda_aedificator_appendere_literis(aed, "#include \"");
    chorda_aedificator_appendere_literis(aed, caput_titulis);
    chorda_aedificator_appendere_literis(aed, "\"\n");
    chorda_aedificator_appendere_literis(aed, "#include <string.h>\n\n");

    numerus_definitionum = xar_numerus(radix->datum.schema.definitiones);

    per (i = ZEPHYRUM; i < numerus_definitionum; i++)
    {
        NuntiumSchemaNodus** def_locus;
        NuntiumSchemaNodus* definitio;
        character* titulus_cstr;
        character* titulus_maiuscula_cstr;
        character* titulus_minuscula_cstr;
        chorda titulus_maiuscula;
        chorda titulus_minuscula;
        i32 j;
        i32 numerus_camporum;
        character prefixum_tag[CCLVI];

        def_locus = (NuntiumSchemaNodus**)xar_obtinere(
            radix->datum.schema.definitiones, i);
        si (!def_locus) perge;
        definitio = *def_locus;

        titulus_cstr = chorda_ut_cstr(
            *definitio->datum.definitio.titulus, piscina);
        titulus_maiuscula = chorda_maiuscula(
            *definitio->datum.definitio.titulus, piscina);
        titulus_minuscula = chorda_minuscula(
            *definitio->datum.definitio.titulus, piscina);
        titulus_maiuscula_cstr = chorda_ut_cstr(titulus_maiuscula, piscina);
        titulus_minuscula_cstr = chorda_ut_cstr(titulus_minuscula, piscina);

        numerus_camporum = xar_numerus(definitio->datum.definitio.campi);

        /* Construere prefixum tag: "PERSONA_TAG_" */
        snprintf(prefixum_tag, (memoriae_index)CCLVI, "%s_TAG_",
            titulus_maiuscula_cstr);

        /* ============================
         * Functio _scribere
         * ============================ */
        chorda_aedificator_appendere_literis(aed, "b32\n");
        chorda_aedificator_appendere_literis(aed, titulus_minuscula_cstr);
        chorda_aedificator_appendere_literis(aed,
            "_scribere(NuntiumScriptor* scriptor, constans ");
        chorda_aedificator_appendere_literis(aed, titulus_cstr);
        chorda_aedificator_appendere_literis(aed, "* nuntium)\n");
        chorda_aedificator_appendere_literis(aed, "{\n");

        chorda_aedificator_push_indentationem(aed);

        /* Verificatio null */
        chorda_aedificator_appendere_indentationem(aed,
            chorda_aedificator_indentatio_gradus(aed));
        chorda_aedificator_appendere_literis(aed,
            "si (!scriptor || !nuntium) redde FALSUM;\n\n");

        /* Emittere scribere pro quoque campo */
        per (j = ZEPHYRUM; j < numerus_camporum; j++)
        {
            NuntiumSchemaNodus** c_locus;
            NuntiumSchemaNodus* campus;
            character* campus_titulis_cstr;
            chorda campus_maiuscula;
            character* campus_maiuscula_cstr;

            c_locus = (NuntiumSchemaNodus**)xar_obtinere(
                definitio->datum.definitio.campi, j);
            si (!c_locus) perge;
            campus = *c_locus;

            campus_maiuscula = chorda_maiuscula(
                *campus->datum.campus.titulus, piscina);
            campus_maiuscula_cstr = chorda_ut_cstr(campus_maiuscula, piscina);
            campus_titulis_cstr = chorda_ut_cstr(
                *campus->datum.campus.titulus, piscina);

            _emittere_scribere_campum(aed, campus,
                prefixum_tag, campus_maiuscula_cstr, campus_titulis_cstr);

            si (j < numerus_camporum - I)
                chorda_aedificator_appendere_literis(aed, "\n");
        }

        /* Reddere VERUM */
        chorda_aedificator_appendere_literis(aed, "\n");
        chorda_aedificator_appendere_indentationem(aed,
            chorda_aedificator_indentatio_gradus(aed));
        chorda_aedificator_appendere_literis(aed, "redde VERUM;\n");

        chorda_aedificator_pop_indentationem(aed);
        chorda_aedificator_appendere_literis(aed, "}\n\n");

        /* ============================
         * Functio _legere
         * ============================ */
        chorda_aedificator_appendere_literis(aed, titulus_cstr);
        chorda_aedificator_appendere_literis(aed, "*\n");
        chorda_aedificator_appendere_literis(aed, titulus_minuscula_cstr);
        chorda_aedificator_appendere_literis(aed,
            "_legere(NuntiumLector* lector, Piscina* piscina)\n");
        chorda_aedificator_appendere_literis(aed, "{\n");

        chorda_aedificator_push_indentationem(aed);
        {
            i32 g = chorda_aedificator_indentatio_gradus(aed);

            /* Declarationes variabilium */
            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed, titulus_cstr);
            chorda_aedificator_appendere_literis(aed, "* n;\n");

            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed, "i32 tag;\n");

            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed,
                "i32 wire_type;\n\n");

            /* Verificatio null */
            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed,
                "si (!lector || !piscina) redde NIHIL;\n\n");

            /* Allocare et zephyrum facere */
            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed, "n = (");
            chorda_aedificator_appendere_literis(aed, titulus_cstr);
            chorda_aedificator_appendere_literis(aed,
                "*)piscina_allocare(piscina, "
                "(memoriae_index)magnitudo(");
            chorda_aedificator_appendere_literis(aed, titulus_cstr);
            chorda_aedificator_appendere_literis(aed, "));\n");

            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed,
                "si (!n) redde NIHIL;\n");

            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed, "memset(n, 0, magnitudo(");
            chorda_aedificator_appendere_literis(aed, titulus_cstr);
            chorda_aedificator_appendere_literis(aed, "));\n\n");

            /* Ansa legere camporum */
            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed,
                "dum (nuntium_legere_field(lector, &tag, &wire_type))\n");

            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed, "{\n");
            chorda_aedificator_push_indentationem(aed);
            {
                i32 g2 = chorda_aedificator_indentatio_gradus(aed);

                chorda_aedificator_appendere_indentationem(aed, g2);
                chorda_aedificator_appendere_literis(aed,
                    "commutatio (tag)\n");
                chorda_aedificator_appendere_indentationem(aed, g2);
                chorda_aedificator_appendere_literis(aed, "{\n");

                chorda_aedificator_push_indentationem(aed);

                /* Emittere casus pro quoque campo */
                per (j = ZEPHYRUM; j < numerus_camporum; j++)
                {
                    NuntiumSchemaNodus** c_locus;
                    NuntiumSchemaNodus* campus;
                    character* campus_titulis_cstr;
                    chorda campus_maiuscula;
                    character* campus_maiuscula_cstr;

                    c_locus = (NuntiumSchemaNodus**)xar_obtinere(
                        definitio->datum.definitio.campi, j);
                    si (!c_locus) perge;
                    campus = *c_locus;

                    campus_maiuscula = chorda_maiuscula(
                        *campus->datum.campus.titulus, piscina);
                    campus_maiuscula_cstr = chorda_ut_cstr(
                        campus_maiuscula, piscina);
                    campus_titulis_cstr = chorda_ut_cstr(
                        *campus->datum.campus.titulus, piscina);

                    _emittere_legere_campum(aed, campus,
                        prefixum_tag, campus_maiuscula_cstr,
                        campus_titulis_cstr, "n");
                }

                /* Ordinarius: saltare */
                {
                    i32 g3 = chorda_aedificator_indentatio_gradus(aed);
                    chorda_aedificator_appendere_indentationem(aed, g3);
                    chorda_aedificator_appendere_literis(aed, "ordinarius:\n");
                    chorda_aedificator_push_indentationem(aed);
                    chorda_aedificator_appendere_indentationem(aed,
                        chorda_aedificator_indentatio_gradus(aed));
                    chorda_aedificator_appendere_literis(aed,
                        "nuntium_saltare_field(lector, wire_type);\n");
                    chorda_aedificator_appendere_indentationem(aed,
                        chorda_aedificator_indentatio_gradus(aed));
                    chorda_aedificator_appendere_literis(aed, "frange;\n");
                    chorda_aedificator_pop_indentationem(aed);
                }

                chorda_aedificator_pop_indentationem(aed);

                chorda_aedificator_appendere_indentationem(aed, g2);
                chorda_aedificator_appendere_literis(aed, "}\n");
            }
            chorda_aedificator_pop_indentationem(aed);
            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed, "}\n\n");

            /* Verificare errorem */
            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed,
                "si (nuntium_lector_error(lector)) redde NIHIL;\n");

            chorda_aedificator_appendere_indentationem(aed, g);
            chorda_aedificator_appendere_literis(aed, "redde n;\n");
        }
        chorda_aedificator_pop_indentationem(aed);
        chorda_aedificator_appendere_literis(aed, "}\n");

        si (i < numerus_definitionum - I)
            chorda_aedificator_appendere_literis(aed, "\n");
    }

    redde chorda_aedificator_finire(aed);
}
