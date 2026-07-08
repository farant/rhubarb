/* amalgamator.c - Manifestum silvae + principale (dev-time)
 *
 * Usus: amalgamator <radix_repositorii> <via_exitus>
 * e.g.: amalgamator .. amalgama/silva.c
 *
 * MANIFESTUM EST CODEX: ordines expliciti infra (capita ordine
 * topologico, corpora ordine corporum - S42; servanda/excludenda per
 * plagulam - S44; CADENDA = typi quos silva.h possidet; renominationes
 * exactae + praefixa). MECHANISMUS in instrumenta/silva_amalgama.{h,c}
 * vivit (Phase 7 Chunk C tesserae extractum - proiecta plura, machina
 * una); porta extractionis: silva.c byte-identicum.
 */
#include "latina.h"
#include "piscina.h"
#include "silva_amalgama.h"
#include <stdio.h>

#define NUMERUS(series) ((i32)(magnitudo(series) / magnitudo((series)[0])))

/* ==================================================
 * Manifestum
 * ================================================== */

interior constans character* constans SERVANDA_FRIATIONIS[] = {
    "friatio_fnv1a", NIHIL
};

/* S44 (finis): static internalizatio x -Werror functiones inusitatas
 * in defectus compilationis vertit - manifestum functiones NON
 * consumptas nominatim demittit. Indices ex compilatore ipso lectae
 * (-Wunused-function harvest); functio nova consumpta = introitum
 * delere (compilatio id clamat). */
interior constans character* constans EXCLUDENDA_PISCINAE[] = {
    "piscina_conari_allocare", "piscina_conari_allocare_ordinatum",
    "piscina_generare_certae_magnitudinis", "piscina_notare",
    "piscina_potesne_allocare", "piscina_reficere",
    "piscina_reliqua_antequam_cresca_alvei",
    "piscina_summa_inutilis_allocatus",
    "piscina_vacare", "_catena_alveus_vacare",
    NIHIL
    /* additiones I: summa_usus + summa_apex_usus resurrectae
     * (telemetria publica silva.h) - in NON_STATICA quoque */
};

interior constans character* constans EXCLUDENDA_CHORDAE[] = {
    "chorda_aequalis", "chorda_aequalis_case_insensitivus",
    "chorda_aequalis_literis", "chorda_camelus",
    "chorda_character_ad", "chorda_concatenare", "chorda_continet",
    "chorda_duplicare", "chorda_ex_buffer",
    "chorda_ex_bytes_legibilis", "chorda_ex_f64", "chorda_ex_s32",
    "chorda_fissio", "chorda_fissio_chorda", "chorda_friare",
    "chorda_incipit", "chorda_invenire", "chorda_invenire_index",
    "chorda_invenire_ultimum", "chorda_invenire_ultimum_index",
    "chorda_invertere", "chorda_iungere", "chorda_kebab",
    "chorda_maiuscula", "chorda_minuscula", "chorda_pascalis",
    "chorda_pascalis_serpens", "chorda_praecidere",
    "chorda_praecidere_dextram", "chorda_praecidere_sinistram",
    "chorda_praecidi_laterale", "chorda_serpens",
    "chorda_substituere", "chorda_terminatur", "chorda_ut_cstr",
    "chorda_ut_f64", "chorda_ut_i32", "chorda_ut_s32",
    "chorda_vacua", "chorda_numerare_occurrentia", "chorda_sectio",
    "_extrahere_verba", NIHIL
};

interior constans character* constans EXCLUDENDA_AEDIFICATORIS[] = {
    "chorda_aedificator_appendere_f64",
    "chorda_aedificator_appendere_hex_i32",
    "chorda_aedificator_appendere_i32",
    "chorda_aedificator_appendere_indentationem",
    "chorda_aedificator_appendere_lineam_novam",
    "chorda_aedificator_appendere_literis_evasus_json",
    "chorda_aedificator_appendere_repetita",
    "chorda_aedificator_appendere_s32",
    "chorda_aedificator_destruere",
    "chorda_aedificator_indentatio_gradus",
    "chorda_aedificator_longitudo",
    "chorda_aedificator_pop_indentationem",
    "chorda_aedificator_push_indentationem",
    "chorda_aedificator_reset", "chorda_aedificator_spectare",
    "chorda_aedificator_appendere_evasus_json",
    "_format_integer_s32", "_format_integer_i32", "_format_duplex",
    "_evadere_json", NIHIL
};

interior constans character* constans EXCLUDENDA_TABULAE[] = {
    "tabula_dispersa_continet_literis",
    "tabula_dispersa_iterator_initium",
    "tabula_dispersa_iterator_proximum", "tabula_dispersa_numerus",
    "tabula_dispersa_status_imprimere", "tabula_dispersa_vacare",
    "tabula_friare_multiplicatio",
    "tabula_dispersa_invenire_literis", "_friare_literis",
    "_invenire_slotum_literis", "_aequalis_literis_chorda", NIHIL
};

interior constans character* constans EXCLUDENDA_XARIS[] = {
    "xar_addere_multos", "xar_capacitas", "xar_continet",
    "xar_copiare_ad_tabulam",
    "xar_destruere", "xar_invenire", "xar_invertere",
    "xar_iterator_finis", "xar_iterator_initium",
    "xar_iterator_proximum", "xar_obtinere_s", "xar_ponere",
    "xar_ponere_vexilla", "xar_quaerere_binarie",
    "xar_quaerere_binarie_index", "xar_removere_cum_ultimo",
    "xar_removere_ultimum", "xar_status_imprimere", "xar_tollere",
    "xar_vacuum_est", "xar_obtinere_vel_creare", "xar_invenire_index",
    NIHIL
};

/* Ordo capitum (S42: topologia capitum != topologia corporum) */
interior constans AmalgamaPlagula CAPITA_VENDICATA[] = {
    { "include/piscina.h",            NIHIL, EXCLUDENDA_PISCINAE,      FALSUM, VERUM },
    { "include/chorda.h",             NIHIL, EXCLUDENDA_CHORDAE,       FALSUM, VERUM },
    { "include/chorda_aedificator.h", NIHIL, EXCLUDENDA_AEDIFICATORIS, FALSUM, VERUM },
    { "include/friatio.h",            SERVANDA_FRIATIONIS, NIHIL,     FALSUM, VERUM },
    { "include/tabula_dispersa.h",    NIHIL, EXCLUDENDA_TABULAE,       FALSUM, VERUM },
    { "include/xar.h",                NIHIL, EXCLUDENDA_XARIS,         FALSUM, VERUM }
};

interior constans AmalgamaPlagula CORPORA_VENDICATA[] = {
    { "lib/piscina.c",            NIHIL, EXCLUDENDA_PISCINAE,      VERUM, VERUM },
    { "lib/chorda.c",             NIHIL, EXCLUDENDA_CHORDAE,       VERUM, VERUM },
    { "lib/chorda_aedificator.c", NIHIL, EXCLUDENDA_AEDIFICATORIS, VERUM, VERUM },
    { "lib/friatio.c",            SERVANDA_FRIATIONIS, NIHIL,     VERUM, VERUM },
    { "lib/tabula_dispersa.c",    NIHIL, EXCLUDENDA_TABULAE,       VERUM, VERUM },
    { "lib/xar.c",                NIHIL, EXCLUDENDA_XARIS,         VERUM, VERUM }
};

/* Capita silvae: post vendicata (typi vendicati retenti - e.g.
 * SilvaTabulaDispersa in definitionibus SilvaOraculum/SilvaExpansio -
 * ante usum stare debent). silva.h typos CADENDA possidet; cetera
 * (typi interni, prototypa, macros) transeunt. */
interior constans AmalgamaPlagula CAPITA_SILVAE[] = {
    { "silva/fontes/silva_token.h",           NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_lexema.h",          NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_expandere.h",       NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_conditio.h",        NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_nodus.h",           NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_tabulae.h",         NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_tabulae_c89.h",     NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_glr.h",             NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_commissio.h",       NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_contextus.h",       NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_latina_datum.h",    NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_parsare.h",         NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_scribere.h",        NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_c89_oraculum.h",    NIHIL, NIHIL, FALSUM, FALSUM },
    { "silva/fontes/silva_quaestio.h",        NIHIL, NIHIL, FALSUM, FALSUM }
};

interior constans AmalgamaPlagula CORPORA_SILVAE[] = {
    { "silva/fontes/silva_token.c",           NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_lexema.c",          NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_expandere.c",       NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_conditio.c",        NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_nodus.c",           NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_tabulae_c89.c",     NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_glr.c",             NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_commissio.c",       NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_contextus.c",       NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_latina_datum.c",    NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_parsare.c",         NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_scribere.c",        NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_c89_oraculum.c",    NIHIL, NIHIL, VERUM, FALSUM },
    { "silva/fontes/silva_quaestio.c",        NIHIL, NIHIL, VERUM, FALSUM }
};

/* CADENDA: tituli typorum quos silva.h POSSIDET - unitates typedef
 * eorundem titulorum ex capitibus internis cadunt (definitio una per
 * TU - C89 typedef duplicata vetat; prototypa duplicata et macros
 * identica LICENT, ergo solum typi cadunt). */
interior constans character* constans CADENDA_TYPEDEF[] = {
    "Piscina", "chorda", "Xar",
    "SilvaCaecatio", "SilvaOrigoGenus", "SilvaOrigo", "SilvaToken",
    "SilvaLexemaGenus", "SilvaLocusSpecies", "SilvaValorGenus",
    "SilvaListaProspectus", "SilvaValor", "SilvaNodus",
    "SilvaTabActioGenus", "SilvaTabActio", "SilvaTabGoto",
    "SilvaTabStatus", "SilvaTabSymbolum", "SilvaTabProductio",
    "SilvaTabPraelata",
    "SilvaTabLocus", "SilvaTabGenus", "SilvaRegistrumCoctum",
    "SilvaTabulaCocta", "SilvaGLRConstructor", "SilvaPraecommissio",
    "SilvaDeclaratioVista",
    "SilvaGLRFabricaAmbigui", "SilvaOraculum",
    "SilvaResolutioResponsum", "SilvaResolutor",
    "SilvaResolutioGenus", "SilvaResolutioEventum", "SilvaCommissio",
    "SilvaExpansio", "SilvaGrammatica", "SilvaParsura",
    "SilvaScriptura", "SilvaFines", "SilvaPergereFunctio",
    "SilvaContextusPlagula", "SilvaContextus", "SilvaRamusGenus", "SilvaInclusioVista",
    "SilvaRamusVista", "SilvaMacroVista",
    "SilvaQuaestio", "SilvaQuaestioResultatum",
    NIHIL
};

/* Definitiones tag quas silva.h PLENE possidet (definitio structurae
 * interna quoque cadit). SilvaOraculum/SilvaExpansio NON hic: silva.h
 * eas solum praenuntiat - definitiones internae manent (interna eas
 * dereferunt). */
interior constans character* constans CADENDA_DEFINITIO[] = {
    "SilvaToken", "SilvaValor", "SilvaNodus", "SilvaCaecatio",
    "SilvaContextus", NIHIL
};

/* Functiones vendicatae quae PUBLICAE manent (extern, in silva.h) */
interior constans character* constans NON_STATICA[] = {
    "piscina_generare_dynamicum", "piscina_destruere",
    "piscina_summa_usus", "piscina_summa_apex_usus",
    "xar_numerus", "xar_obtinere", NIHIL
};

/* Renominationes typorum (exactae). Typi symbola nexus non creant,
 * sed silva.h eosdem SilvaXxx nominabit - corpus et caput consentire
 * debent. */

interior constans Renominatio TYPI_EXACTI[] = {
    { "Piscina",               "SilvaPiscina" },
    { "PiscinaNotatio",        "SilvaPiscinaNotatio" },
    { "chorda",                "SilvaChorda" },
    { "chorda_fissio_fructus", "silva_chorda_fissio_fructus" },
    { "ChordaAedificator",     "SilvaChordaAedificator" },
    { "TabulaDispersa",        "SilvaTabulaDispersa" },
    { "TabulaIterator",        "SilvaTabulaIterator" },
    { "Slotus",                "SilvaSlotus" },
    { "SlotusStatus",          "SilvaSlotusStatus" },
    { "Xar",                   "SilvaXar" },
    { "XarIterator",           "SilvaXarIterator" },
    { "XarComparator",         "SilvaXarComparator" }
};

/* Praefixa functionum - longissimum primum (chorda_aedificator_
 * ante chorda_!) */
interior constans Renominatio PRAEFIXA_FUNCTIONUM[] = {
    { "chorda_aedificator_", "silva_chorda_aedificator_" },
    { "tabula_dispersa_",    "silva_tabula_dispersa_" },
    { "friatio_",            "silva_friatio_" },
    { "piscina_",            "silva_piscina_" },
    { "chorda_",             "silva_chorda_" },
    { "xar_",                "silva_xar_" }
};

/* Prooemium exitus */
interior constans character* constans PROOEMIUM =
    "/* silva.c - GENERATUM (amalgamator) - NE MANU EDITES\n"
    " *\n"
    " * Substratum silvae in plagula una (SQLite modo).\n"
    " * Fons veritatis: silva/fontes/ + bibliothecae\n"
    " * vendicatae in lib/. Regenerare: silva/amalgamare.sh\n"
    " */\n\n";

/* Assertio derivae (C89 more): silva.h magnitudinem seriei
 * segmentorum SilvaXar 64 codificat - macro internum consentire
 * DEBET aut compilatio frangitur */
interior constans character* constans ASSERTIONES =
    "\n/* assertio derivae: silva.h SilvaXar.segmenta[64] ==\n"
    " * XAR_MAXIMUS_SEGMENTORUM internum */\n"
    "typedef char silva_assertio_xar_segmentorum[\n"
    "    (XAR_MAXIMUS_SEGMENTORUM == 64) ? 1 : -1];\n";

/* Prooemium capitis dati latini (byte-identicum emissioni pristinae) */
interior constans character* constans LATINA_DATUM_PROOEMIUM =
    "/* silva_latina_datum.h - Textus latina.h ut datum (Phase 7 Chunk A)\n"
    " *\n"
    " * GENERATUM ex include/latina.h per amalgamatorem - NE MANU MUTES\n"
    " * (regeneratur per silva/amalgamare.sh). Copia compilata definitionum\n"
    " * latinarum: silva_contextus_latinam_addere eam praebet - \"compiled-in\n"
    " * defaults\" interview ad litteram, sine fonte veritatis secundo (datum\n"
    " * IPSA plagula vendicata est).\n"
    " */\n";

s32 principale (s32 argc, character** argv)
{
    Piscina* piscina;
    AmalgamaManifestum manifestum;

    si (argc < III)
    {
        fprintf(stderr,
            "usus: amalgamator <radix_repositorii> <via_exitus>\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("amalgamator", 33554432);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "amalgamator: piscina non generata\n");
        redde I;
    }

    manifestum.capita_vendicata = CAPITA_VENDICATA;
    manifestum.numerus_capitum_vendicatorum = NUMERUS(CAPITA_VENDICATA);
    manifestum.corpora_vendicata = CORPORA_VENDICATA;
    manifestum.numerus_corporum_vendicatorum =
        NUMERUS(CORPORA_VENDICATA);
    manifestum.capita_propria = CAPITA_SILVAE;
    manifestum.numerus_capitum_propriorum = NUMERUS(CAPITA_SILVAE);
    manifestum.corpora_propria = CORPORA_SILVAE;
    manifestum.numerus_corporum_propriorum = NUMERUS(CORPORA_SILVAE);
    manifestum.cadenda_typedef = CADENDA_TYPEDEF;
    manifestum.cadenda_definitio = CADENDA_DEFINITIO;
    manifestum.non_statica = NON_STATICA;
    manifestum.typi_exacti = TYPI_EXACTI;
    manifestum.numerus_typorum = NUMERUS(TYPI_EXACTI);
    manifestum.praefixa_functionum = PRAEFIXA_FUNCTIONUM;
    manifestum.numerus_praefixorum = NUMERUS(PRAEFIXA_FUNCTIONUM);
    manifestum.via_capitis = "silva/amalgama/silva.h";
    manifestum.prooemium = PROOEMIUM;
    manifestum.assertiones = ASSERTIONES;
    manifestum.latina_datum_via_capitis =
        "silva/fontes/silva_latina_datum.h";
    manifestum.latina_datum_via_corporis =
        "silva/fontes/silva_latina_datum.c";
    manifestum.latina_datum_prooemium = LATINA_DATUM_PROOEMIUM;
    manifestum.latina_datum_custos = "SILVA_LATINA_DATUM_H";
    manifestum.latina_datum_titulus_textus = "silva_latina_textus";
    manifestum.latina_datum_titulus_mensurae = "silva_latina_mensura";

    si (!silva_amalgama_currere(piscina, argv[I], argv[II],
            &manifestum))
    {
        piscina_destruere(piscina);
        redde I;
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
