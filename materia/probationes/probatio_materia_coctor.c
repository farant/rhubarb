/* probatio_materia_coctor.c - Registrum coctum ex declaratione STML
 *
 * Porta nativitatis bibliothecae materia_coctor: declaratio inlinea
 * (genera II, species VII omnes, nota generis et loci, titulus cum
 * hyphen) in textus OCTETIM exspectatos MANU derivatos ex specie
 * redditionis (non ex instrumento transcriptos); recusationes
 * nominatae (titulus loci iteratus, species ignota, attributum
 * radicis absens, praefixum non maiusculum, titulus non kebab, radix
 * aliena, parsura fracta, genus sine locis solum); comparator
 * recens/rancidus cum linea divergente; tituli specierum ==
 * enumeratio. Culpa plantata nativitatis: offset generis secundi +I
 * (textus .c differt in linea GENERA).
 */

#include "latina.h"
#include "credo.h"
#include "materia_coctor.h"
#include "materia_nodus.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* constans DECLARATIO =
    "<registrum grammatica=\"proba\" praefixum=\"PROBA\"\n"
    "  typus=\"ProbaGenus\" sedes=\"proba/fontes\">\n"
    "  <genus titulus=\"radix\" nota=\"radix arboris probae\">\n"
    "    <locus titulus=\"caput\" species=\"token\"/>\n"
    "    <locus titulus=\"liberi\" species=\"lista-nodus\"\n"
    "      nota=\"ordine fontis\"/>\n"
    "    <locus titulus=\"cauda\" species=\"lista-token\"/>\n"
    "  </genus>\n"
    "  <genus titulus=\"folium-parvum\">\n"
    "    <locus titulus=\"valor\" species=\"nodus\"/>\n"
    "    <locus titulus=\"mixta\" species=\"lista-mixta\"/>\n"
    "    <locus titulus=\"index\" species=\"index\"/>\n"
    "    <locus titulus=\"socius\" species=\"referentia\"/>\n"
    "  </genus>\n"
    "</registrum>\n";

/* eadem declaratio, sedes in materia/build (porta rancoris VI) */
hic_manens constans character* constans DECLARATIO_AEDIFICATA =
    "<registrum grammatica=\"proba\" praefixum=\"PROBA\"\n"
    "  typus=\"ProbaGenus\" sedes=\"materia/build\">\n"
    "  <genus titulus=\"radix\">\n"
    "    <locus titulus=\"caput\" species=\"token\"/>\n"
    "  </genus>\n"
    "</registrum>\n";

/* diagnostica declarata (materia-sedes B1): genus diagnosticum, genus
 * inanis, locus 'vacua' et locus 'absentia' cum gravitate monitum.
 * Causa cum '"', '\' et '??)' per elementum attributi (lector STML
 * entitates in valoribus attributorum NON decodit - '&quot;' verbatim
 * transiret): omnia in littera C effugienda */
hic_manens constans character* constans DECLARATIO_DIAGNOSTICA =
    "<registrum grammatica=\"proba\" praefixum=\"PROBA\""
    " typus=\"ProbaGenus\" sedes=\"x\">"
    "<genus titulus=\"malum\" diagnosticum=\"malum lectum\">"
    "<locus titulus=\"tokens\" species=\"lista-token\"/></genus>"
    "<genus titulus=\"separator\" inanis=\"verum\">"
    "<locus titulus=\"tok\" species=\"token\"/></genus>"
    "<genus titulus=\"grex\">"
    "<locus titulus=\"liberi\" species=\"lista-nodus\""
    " vacua=\"lista vacua\"/>"
    "<locus titulus=\"tok_clausura\" species=\"token\""
    " gravitas=\"monitum\"><@absentia=>'}' \"exspectata\" \\ a?\?)"
    "</></locus>"
    "</genus></registrum>";

hic_manens constans character* constans VIA_DECLARATIONIS =
    "proba/grammatica/proba.registrum.stml";

/* textus .h exspectatus: manu ex specie redditionis (materia_coctor.h)
 * - commentarium capitis involutum ad LXXII (praefixum ' * ', verbum
 * 'EDITES.' lineam I ad LXXIII traheret), enumeratio cum nota generis
 * super membrum suum, comes NUMERUS_GENERUM post lineam vacuam */
hic_manens constans character* constans CAPUT_EXSPECTATUM =
    "/* proba_registrum_coctum.h\n"
    " *\n"
    " * Registrum generum COCTUM grammaticae 'proba' - GENERATUM, "
    "NE MANU\n"
    " * EDITES. Fons: proba/grammatica/proba.registrum.stml\n"
    " * (materia/coquere.sh). Genera II, loci VII.\n"
    " */\n"
    "\n"
    "#ifndef PROBA_REGISTRUM_COCTUM_H\n"
    "#define PROBA_REGISTRUM_COCTUM_H\n"
    "\n"
    "#include \"latina.h\"\n"
    "#include \"materia_registrum.h\"\n"
    "\n"
    "nomen enumeratio {\n"
    "    /* radix arboris probae */\n"
    "    PROBA_GENUS_RADIX = 0,\n"
    "    PROBA_GENUS_FOLIUM_PARVUM,\n"
    "\n"
    "    PROBA_GENUS_NUMERUS_GENERUM\n"
    "} ProbaGenus;\n"
    "\n"
    "externus constans MateriaRegistrumCoctum PROBA_REGISTRUM;\n"
    "\n"
    "#endif /* PROBA_REGISTRUM_COCTUM_H */\n";

/* textus .c exspectatus: series LOCORUM per genus cum linea vacua
 * inter genera et commentario intervalli (offset..ultimus), nota loci
 * super ordinem suum, comma post ordinem ultimum (C89 licet), genera
 * cum offset et numero, coctum ex magnitudinibus */
hic_manens constans character* constans FONS_EXSPECTATUS =
    "/* proba_registrum_coctum.c\n"
    " *\n"
    " * Registrum generum COCTUM grammaticae 'proba' - GENERATUM, "
    "NE MANU\n"
    " * EDITES. Fons: proba/grammatica/proba.registrum.stml\n"
    " * (materia/coquere.sh). Series LOCORUM plana; quodque genus "
    "fenestram\n"
    " * suam per loci_offset + loci_numerus nominat. Genera II, "
    "loci VII.\n"
    " */\n"
    "\n"
    "#include \"proba_registrum_coctum.h\"\n"
    "#include \"materia_nodus.h\"\n"
    "\n"
    "hic_manens constans MateriaTabLocus LOCI_COCTI[] = {\n"
    "    /* radix (0..2) */\n"
    "    { \"caput\", (s32)MATERIA_LOCUS_TOKEN },\n"
    "    /* ordine fontis */\n"
    "    { \"liberi\", (s32)MATERIA_LOCUS_LISTA_NODUS },\n"
    "    { \"cauda\", (s32)MATERIA_LOCUS_LISTA_TOKEN },\n"
    "\n"
    "    /* folium-parvum (3..6) */\n"
    "    { \"valor\", (s32)MATERIA_LOCUS_NODUS },\n"
    "    { \"mixta\", (s32)MATERIA_LOCUS_LISTA_MIXTA },\n"
    "    { \"index\", (s32)MATERIA_LOCUS_INDEX },\n"
    "    { \"socius\", (s32)MATERIA_LOCUS_REFERENTIA },\n"
    "};\n"
    "\n"
    "hic_manens constans MateriaTabGenus GENERA_COCTA[] = {\n"
    "    /* titulus, offset, numerus */\n"
    "    { \"radix\", (i32)0, (i32)3 },\n"
    "    { \"folium-parvum\", (i32)3, (i32)4 },\n"
    "};\n"
    "\n"
    "constans MateriaRegistrumCoctum PROBA_REGISTRUM = {\n"
    "    GENERA_COCTA,\n"
    "    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),\n"
    "    LOCI_COCTI,\n"
    "    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))\n"
    "};\n";

/* linea n-ta textus (I-basata) imprimere - diagnosis divergentiae */
interior vacuum
_lineam_imprimere (
    constans character* titulus,
                chorda  textus,
                   i32  n)
{
    i32 i;
    i32 linea    = I;
    i32 initium  = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= textus.mensura; i++)
    {
        si (i == textus.mensura || textus.datum[i] == (i8)'\n')
        {
            si (linea == n)
            {
                imprimere("    %s %u: |%.*s|\n", titulus, n,
                    (integer)(i - initium),
                    (constans character*)textus.datum + initium);
                redde;
            }
            linea    = linea + I;
            initium  = i + I;
        }
    }
    imprimere("    %s %u: (absens)\n", titulus, n);
}

/* generatum contra exspectatum: aequalitas octetim, linea divergens
 * utrimque impressa si differt */
interior b32
_aequalis_aut_ostende (
               Piscina* piscina,
    constans character* quid,
                chorda  generatum,
    constans character* exspectatum)
{
    chorda e = chorda_ex_literis(exspectatum, piscina);
       i32 linea;

    si (materia_coctio_aequalis(generatum, e, &linea))
    {
        redde VERUM;
    }
    imprimere("    %s divergit linea %u:\n", quid, linea);
    _lineam_imprimere("generatum ", generatum, linea);
    _lineam_imprimere("exspectatum", e, linea);
    redde FALSUM;
}

/* declarationem coquere: FALSUM cum causa impressa (recusatio) */
interior b32
_coquere (
               Piscina* piscina,
    constans character* declaratio,
         MateriaCoctio* coctio)
{
    b32 f = materia_registrum_coquere(piscina,
        chorda_ex_literis(declaratio, piscina), VIA_DECLARATIONIS,
        coctio);

    si (!f && coctio->causa.mensura > ZEPHYRUM)
    {
        imprimere("    recusatio: %.*s (linea %u)\n",
            (integer)coctio->causa.mensura,
            (constans character*)coctio->causa.datum, coctio->linea);
    }
    redde f;
}

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio_coctor",
        4194304);
         b32 praeteritus;

    credo_aperire(piscina);
    imprimere("=== Probatio materia_coctor ===\n");

    {
        s32 k;

        imprimere("\n--- I. Tituli specierum == enumeratio ---\n");
        per (k = ZEPHYRUM; k < (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM;
             k++)
        {
            constans character* t = materia_locus_species_titulus(k);

            CREDO_NON_NIHIL (t);
            CREDO_AEQUALIS_S32 (materia_locus_species_ex_titulo(
                chorda_ex_literis(t, piscina)), k);
        }
        CREDO_AEQUALIS_S32 ((s32)MATERIA_LOCUS_NUMERUS_SPECIERUM,
            (s32)VII);
        CREDO_NIHIL (materia_locus_species_titulus(
            (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM));
        CREDO_VERUM (strcmp(materia_locus_species_titulus(
            (s32)MATERIA_LOCUS_LISTA_TOKEN), "lista-token")
                == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_locus_species_titulus(
            (s32)MATERIA_LOCUS_REFERENTIA), "referentia") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (materia_locus_species_ex_titulo(
            chorda_ex_literis("lista", piscina)), (s32)-I);
    }

    {
        i32 linea;

        imprimere("\n--- II. Comparator: recens aut rancidus ---\n");
        CREDO_VERUM (materia_coctio_aequalis(
            chorda_ex_literis("a\nb\nc\n", piscina),
            chorda_ex_literis("a\nb\nc\n", piscina), &linea));
        CREDO_AEQUALIS_I32 (linea, ZEPHYRUM);
        CREDO_FALSUM (materia_coctio_aequalis(
            chorda_ex_literis("a\nb\nc\n", piscina),
            chorda_ex_literis("a\nb\nX\n", piscina), &linea));
        CREDO_AEQUALIS_I32 (linea, (i32)III);
        /* brevior: divergit post octetum suum ultimum (linea IV) */
        CREDO_FALSUM (materia_coctio_aequalis(
            chorda_ex_literis("a\nb\nc\n", piscina),
            chorda_ex_literis("a\nb\nc\nd", piscina), &linea));
        CREDO_AEQUALIS_I32 (linea, (i32)IV);
        CREDO_FALSUM (materia_coctio_aequalis(
            chorda_ex_literis("a\nb", piscina),
            chorda_ex_literis("a\nb\n", piscina), &linea));
        CREDO_AEQUALIS_I32 (linea, (i32)II);
    }

    {
        MateriaCoctio coctio;

        imprimere("\n--- III. Declaratio in textus exspectatos ---\n");
        CREDO_VERUM (_coquere(piscina, DECLARATIO, &coctio));
        CREDO_AEQUALIS_I32 (coctio.numerus_generum, (i32)II);
        CREDO_AEQUALIS_I32 (coctio.numerus_locorum, (i32)VII);
        CREDO_CHORDA_AEQUALIS_LITERIS (coctio.grammatica, "proba");
        CREDO_CHORDA_AEQUALIS_LITERIS (coctio.praefixum, "PROBA");
        CREDO_CHORDA_AEQUALIS_LITERIS (coctio.typus, "ProbaGenus");
        CREDO_CHORDA_AEQUALIS_LITERIS (coctio.via_capitis,
            "proba/fontes/proba_registrum_coctum.h");
        CREDO_CHORDA_AEQUALIS_LITERIS (coctio.via_fontis,
            "proba/fontes/proba_registrum_coctum.c");
        CREDO_VERUM (_aequalis_aut_ostende(piscina, "caput",
            coctio.caput,
            CAPUT_EXSPECTATUM));
        CREDO_VERUM (_aequalis_aut_ostende(piscina, "fons", coctio.fons,
            FONS_EXSPECTATUS));
        /* ordo GENERA: offset secundi = numerus primi (plantatio) */
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.fons, piscina),
            "{ \"folium-parvum\", (i32)3, (i32)4 },"));
    }

    {
        MateriaCoctio coctio;

        imprimere("\n--- IV. Recusationes nominatae ---\n");
        /* titulus loci iteratus intra genus */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"g\">\n"
            "    <locus titulus=\"a\" species=\"token\"/>\n"
            "    <locus titulus=\"a\" species=\"index\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("iteratus", piscina));
        CREDO_AEQUALIS_I32 (coctio.linea, (i32)V);
        /* idem titulus in generibus DIVERSIS licet */
        CREDO_VERUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"g\">\n"
            "    <locus titulus=\"a\" species=\"token\"/>\n"
            "  </genus>\n"
            "  <genus titulus=\"h\">\n"
            "    <locus titulus=\"a\" species=\"index\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        /* species ignota */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"g\">\n"
            "    <locus titulus=\"a\" species=\"lista\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("species loci ignota", piscina));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("'lista'", piscina));
        /* attributum radicis absens */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" sedes=\"p\">\n"
            "  <genus titulus=\"g\">\n"
            "    <locus titulus=\"a\" species=\"token\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("deest", piscina));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("'typus'", piscina));
        /* praefixum non maiusculum */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"Px\"\n"
            "  typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"g\">\n"
            "    <locus titulus=\"a\" species=\"token\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("non identificator", piscina));
        /* titulus non kebab (maiuscula) */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"Genus\">\n"
            "    <locus titulus=\"a\" species=\"token\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("non kebab", piscina));
        /* elementum ignotum intra registrum */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <locus titulus=\"a\" species=\"token\"/>\n"
            "</registrum>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("elementum ignotum", piscina));
        /* radix aliena */
        CREDO_FALSUM (_coquere(piscina,
            "<canon dialectus=\"x\"/>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("'registrum' exspectata", piscina));
        /* parsura fracta */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\"\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("parsari nequit", piscina));
        /* registrum sine locis (series vacua non compilat) */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"g\"/>\n"
            "</registrum>\n", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("sine locis", piscina));
        /* genus sine locis inter alia LICET: 'loci nulli', numerus 0 */
        CREDO_VERUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"g\"/>\n"
            "  <genus titulus=\"h\">\n"
            "    <locus titulus=\"a\" species=\"token\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.fons, piscina),
            "/* g (loci nulli) */"));
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.fons, piscina),
            "{ \"g\", (i32)0, (i32)0 },"));
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.fons, piscina),
            "{ \"h\", (i32)0, (i32)1 },"));
    }

    {
        MateriaCoctio coctio;

        imprimere("\n--- V. Tituli cum subducto (css: tok_nomen) ---\n");
        CREDO_VERUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\"\n"
            "  sedes=\"p\">\n"
            "  <genus titulus=\"regula-apud\">\n"
            "    <locus titulus=\"tok_nomen\" species=\"token\"/>\n"
            "    <locus titulus=\"tok_terminator\" species=\"token\"/>\n"
            "  </genus>\n"
            "</registrum>\n", &coctio));
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.fons, piscina),
            "{ \"tok_nomen\", (s32)MATERIA_LOCUS_TOKEN },"));
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.caput, piscina),
            "P_GENUS_REGULA_APUD = 0,"));
    }

    {
        constans character* radix = getenv("RHUBARB_RADIX");
                 character  via[1024];
             MateriaRancor  rancor;
                      FILE* f;

        imprimere("\n--- VI. Porta rancoris: recens, rancida, absens ---\n");
        /* declaratio et plagulae generatae in materia/build (sedes) */
        radix = radix != NIHIL ? radix : ".";
        sprintf(via, "%s/materia/build/proba.registrum.stml", radix);
        f = fopen(via, "wb");
        CREDO_NON_NIHIL (f);
        si (f != NIHIL)
        {
            fputs(DECLARATIO_AEDIFICATA, f);
            fclose(f);
        }
        /* absens: plagulae cursus prioris remotae (suite iterata eas
         * inveniret - cursus plenus 2026-09-15 id docuit) */
        sprintf(via, "%s/materia/build/proba_registrum_coctum.h",
            radix);
        remove(via);
        sprintf(via, "%s/materia/build/proba_registrum_coctum.c",
            radix);
        remove(via);
        CREDO_VERUM (materia_registrum_recens(piscina, radix,
            "materia/build/proba.registrum.stml", &rancor));
        CREDO_FALSUM (rancor.recens);
        CREDO_AEQUALIS_I32 (rancor.linea, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (rancor.via,
            "materia/build/proba_registrum_coctum.h");
        /* scriptae ex redditione: recens */
        sprintf(via, "%s/materia/build/proba_registrum_coctum.h",
            radix);
        f = fopen(via, "wb");
        CREDO_NON_NIHIL (f);
        si (f != NIHIL)
        {
            fwrite(rancor.coctio.caput.datum, I,
                (size_t)rancor.coctio.caput.mensura, f);
            fclose(f);
        }
        sprintf(via, "%s/materia/build/proba_registrum_coctum.c",
            radix);
        f = fopen(via, "wb");
        CREDO_NON_NIHIL (f);
        si (f != NIHIL)
        {
            fwrite(rancor.coctio.fons.datum, I,
                (size_t)rancor.coctio.fons.mensura, f);
            fclose(f);
        }
        CREDO_VERUM (materia_registrum_recens(piscina, radix,
            "materia/build/proba.registrum.stml", &rancor));
        CREDO_VERUM (rancor.recens);
        /* fons manu tactus: rancidus cum linea */
        f = fopen(via, "ab");
        CREDO_NON_NIHIL (f);
        si (f != NIHIL)
        {
            fputs("/* manu */\n", f);
            fclose(f);
        }
        CREDO_VERUM (materia_registrum_recens(piscina, radix,
            "materia/build/proba.registrum.stml", &rancor));
        CREDO_FALSUM (rancor.recens);
        CREDO_CHORDA_AEQUALIS_LITERIS (rancor.via,
            "materia/build/proba_registrum_coctum.c");
        CREDO_MAIOR_I32 (rancor.linea, ZEPHYRUM);
        /* declaratio absens: FALSUM cum causa */
        CREDO_FALSUM (materia_registrum_recens(piscina, radix,
            "materia/build/absens.registrum.stml", &rancor));
        CREDO_CHORDA_CONTINET (rancor.causa,
            chorda_ex_literis("absens", piscina));
    }

    {
        MateriaCoctio  coctio;
            character* fons;

        imprimere("\n--- VII. Diagnostica declarata (sedes B1) ---\n");
        CREDO_VERUM (_coquere(piscina, DECLARATIO_DIAGNOSTICA,
            &coctio));
        CREDO_AEQUALIS_I32 (coctio.numerus_diagnosticorum, (i32)III);
        CREDO_AEQUALIS_I32 (coctio.numerus_inanium, (i32)I);
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.caput, piscina),
            "externus constans MateriaDiagnosticaCocta "
            "PROBA_DIAGNOSTICA;\n\n#endif"));
        fons = chorda_ut_cstr(coctio.fons, piscina);
        CREDO_NON_NIHIL (strstr(fons,
            "    { (s32)PROBA_GENUS_MALUM, (s32)-1,\n"
            "      (s32)MATERIA_DIAGNOSTICUM_GENUS,\n"
            "      (s32)MATERIA_GRAVITAS_ERRATUM,\n"
            "      \"malum\",\n"
            "      \"malum lectum\" },\n"));
        CREDO_NON_NIHIL (strstr(fons,
            "    { (s32)PROBA_GENUS_GREX, (s32)0,\n"
            "      (s32)MATERIA_DIAGNOSTICUM_VACUA,\n"
            "      (s32)MATERIA_GRAVITAS_ERRATUM,\n"
            "      \"grex/liberi\",\n"));
        CREDO_NON_NIHIL (strstr(fons,
            "    { (s32)PROBA_GENUS_GREX, (s32)1,\n"
            "      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,\n"
            "      (s32)MATERIA_GRAVITAS_MONITUM,\n"
            "      \"grex/tok_clausura\",\n"
            "      \"'}' \\\"exspectata\\\" \\\\ a?\\?)\" },\n"));
        CREDO_NON_NIHIL (strstr(fons,
            "hic_manens constans s32 INANIA_COCTA[] = {\n"
            "    (s32)PROBA_GENUS_SEPARATOR,\n"
            "};\n"));
        CREDO_NON_NIHIL (strstr(fons,
            "constans MateriaDiagnosticaCocta PROBA_DIAGNOSTICA = {\n"
            "    DIAGNOSTICA_COCTA,\n"));
        /* '??' in causa: trigraphus in littera C nisi effugitur */
        CREDO_VERUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\""
            " sedes=\"p\">"
            "<genus titulus=\"g\" diagnosticum=\"quid?\?)\">"
            "<locus titulus=\"a\" species=\"token\"/></genus>"
            "</registrum>", &coctio));
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.fons, piscina),
            "      \"quid?\\?)\" },\n"));
        CREDO_AEQUALIS_I32 (coctio.numerus_inanium, ZEPHYRUM);
        CREDO_NON_NIHIL (strstr(chorda_ut_cstr(coctio.fons, piscina),
            "    NIHIL,\n    (i32)0\n};\n"));
        /* recusationes nominatae */
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\""
            " sedes=\"p\"><genus titulus=\"g\">"
            "<locus titulus=\"a\" species=\"token\" vacua=\"x\"/>"
            "</genus></registrum>", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("vacua in loco non listae:", piscina));
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\""
            " sedes=\"p\"><genus titulus=\"g\">"
            "<locus titulus=\"a\" species=\"token\" absentia=\"x\""
            " gravitas=\"gravis\"/></genus></registrum>", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("gravitas ignota:", piscina));
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\""
            " sedes=\"p\"><genus titulus=\"g\" inanis=\"falsum\">"
            "<locus titulus=\"a\" species=\"token\"/>"
            "</genus></registrum>", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("inanis non 'verum':", piscina));
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\""
            " sedes=\"p\"><genus titulus=\"g\">"
            "<locus titulus=\"a\" species=\"token\" absentia=\""
            "causa longissima quae sexaginta characteres excedit et "
            "ideo recusatur\"/></genus></registrum>", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("causa longior LX:", piscina));
        CREDO_FALSUM (_coquere(piscina,
            "<registrum grammatica=\"p\" praefixum=\"P\" typus=\"PG\""
            " sedes=\"p\"><genus titulus=\"g\" diagnosticum=\"a\tb\">"
            "<locus titulus=\"a\" species=\"token\"/>"
            "</genus></registrum>", &coctio));
        CREDO_CHORDA_CONTINET (coctio.causa,
            chorda_ex_literis("causa cum charactere moderationis:",
                piscina));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
