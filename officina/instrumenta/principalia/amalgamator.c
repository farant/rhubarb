/* amalgamator.c - Manifestum officinae + principale (dev-time,
 * M1a C)
 *
 * Usus: amalgamator <radix_repositorii> <via_exitus>
 *
 * MANIFESTUM EST CODEX; MECHANISMUS in silva/instrumenta/
 * silva_amalgama.{h,c} vivit (tessera exemplar). Vendicata:
 * latina (verbatim) + piscina + chorda + chorda_aedificator + xar.
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

/* Excludenda: messis -Wunused-function - functio nova consumpta =
 * introitum delere (compilatio id clamat) */
interior constans character* constans EXCLUDENDA_PISCINAE[] = {
    "_catena_alveus_vacare",
    "piscina_conari_allocare",
    "piscina_conari_allocare_ordinatum",
    "piscina_generare_certae_magnitudinis",
    "piscina_notare",
    "piscina_potesne_allocare",
    "piscina_reficere",
    "piscina_reliqua_antequam_cresca_alvei",
    "piscina_summa_apex_usus",
    "piscina_summa_inutilis_allocatus",
    "piscina_summa_usus",
    "piscina_vacare",
    NIHIL
};
interior constans character* constans EXCLUDENDA_CHORDAE[] = {
    "_extrahere_verba",
    "chorda_aequalis",
    "chorda_aequalis_case_insensitivus",
    "chorda_camelus",
    "chorda_character_ad",
    "chorda_concatenare",
    "chorda_continet",
    "chorda_duplicare",
    "chorda_ex_buffer",
    "chorda_ex_bytes_legibilis",
    "chorda_ex_f64",
    "chorda_ex_s32",
    "chorda_fissio",
    "chorda_fissio_chorda",
    "chorda_friare",
    "chorda_incipit",
    "chorda_invenire",
    "chorda_invenire_index",
    "chorda_invenire_ultimum",
    "chorda_invenire_ultimum_index",
    "chorda_invertere",
    "chorda_iungere",
    "chorda_kebab",
    "chorda_maiuscula",
    "chorda_minuscula",
    "chorda_numerare_occurrentia",
    "chorda_pascalis",
    "chorda_pascalis_serpens",
    "chorda_praecidere",
    "chorda_praecidere_dextram",
    "chorda_praecidere_sinistram",
    "chorda_praecidi_laterale",
    "chorda_sectio",
    "chorda_serpens",
    "chorda_substituere",
    "chorda_terminatur",
    "chorda_ut_cstr",
    "chorda_ut_f64",
    "chorda_ut_i32",
    "chorda_ut_s32",
    "chorda_vacua",
    NIHIL
};
interior constans character* constans EXCLUDENDA_AEDIFICATORIS[] = {
    "_evadere_json",
    "_format_duplex",
    "_format_integer_s32",
    "chorda_aedificator_reset",
    "chorda_aedificator_spectare",
    "chorda_aedificator_appendere_evasus_json",
    "chorda_aedificator_appendere_f64",
    "chorda_aedificator_appendere_hex_i32",
    "chorda_aedificator_appendere_indentationem",
    "chorda_aedificator_appendere_lineam_novam",
    "chorda_aedificator_appendere_literis_evasus_json",
    "chorda_aedificator_appendere_s32",
    "chorda_aedificator_destruere",
    "chorda_aedificator_indentatio_gradus",
    "chorda_aedificator_longitudo",
    "chorda_aedificator_pop_indentationem",
    "chorda_aedificator_push_indentationem",
    NIHIL
};
interior constans character* constans EXCLUDENDA_TABULAE[] = {
    "tabula_dispersa_continet",
    "tabula_dispersa_continet_literis",
    "tabula_dispersa_delere",
    "tabula_dispersa_iterator_initium",
    "tabula_dispersa_iterator_proximum",
    "tabula_dispersa_numerus",
    "tabula_dispersa_status_imprimere", "tabula_dispersa_vacare",
    "tabula_friare_multiplicatio",
    "tabula_dispersa_invenire_literis", "_friare_literis",
    "_invenire_slotum_literis", "_aequalis_literis_chorda", NIHIL
};

interior constans character* constans EXCLUDENDA_XARIS[] = {
    "xar_addere_multos",
    "xar_capacitas",
    "xar_continet",
    "xar_creare_cum_magnitudine",
    "xar_destruere",
    "xar_invenire",
    "xar_invenire_index",
    "xar_invertere",
    "xar_iterator_finis",
    "xar_iterator_initium",
    "xar_iterator_proximum",
    "xar_mutare",
    "xar_obtinere_vel_creare",
    "xar_ordinare",
    "xar_ponere",
    "xar_ponere_vexilla",
    "xar_quaerere_binarie",
    "xar_quaerere_binarie_index",
    "xar_removere_cum_ultimo",
    "xar_status_imprimere",
    "xar_tollere",
    "xar_vacare",
    "xar_vacuum_est",
    NIHIL
};

interior constans AmalgamaPlagula CAPITA_VENDICATA[] = {
    { "include/piscina.h",            NIHIL, EXCLUDENDA_PISCINAE,      FALSUM, VERUM },
    { "include/chorda.h",             NIHIL, EXCLUDENDA_CHORDAE,       FALSUM, VERUM },
    { "include/chorda_aedificator.h", NIHIL, EXCLUDENDA_AEDIFICATORIS, FALSUM, VERUM },
    { "include/friatio.h",            SERVANDA_FRIATIONIS, NIHIL,      FALSUM, VERUM },
    { "include/tabula_dispersa.h",    NIHIL, EXCLUDENDA_TABULAE,       FALSUM, VERUM },
    { "include/xar.h",                NIHIL, EXCLUDENDA_XARIS,         FALSUM, VERUM }
};

interior constans AmalgamaPlagula CORPORA_VENDICATA[] = {
    { "lib/piscina.c",            NIHIL, EXCLUDENDA_PISCINAE,      VERUM, VERUM },
    { "lib/chorda.c",             NIHIL, EXCLUDENDA_CHORDAE,       VERUM, VERUM },
    { "lib/chorda_aedificator.c", NIHIL, EXCLUDENDA_AEDIFICATORIS, VERUM, VERUM },
    { "lib/friatio.c",            SERVANDA_FRIATIONIS, NIHIL,      VERUM, VERUM },
    { "lib/tabula_dispersa.c",    NIHIL, EXCLUDENDA_TABULAE,       VERUM, VERUM },
    { "lib/xar.c",                NIHIL, EXCLUDENDA_XARIS,         VERUM, VERUM }
};

interior constans AmalgamaPlagula CAPITA_OFFICINAE[] = {
    { "officina/fontes/officina_medulla.h",        NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_medulla_textus.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_regio.h",          NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_conexio.h",        NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_machinula.h",      NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_demissio.h",       NIHIL, NIHIL, FALSUM, FALSUM }
};

interior constans AmalgamaPlagula CORPORA_OFFICINAE[] = {
    { "officina/fontes/officina_medulla.c",        NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_medulla_textus.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_regio.c",          NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_conexio.c",        NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_machinula.c",      NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_demissio.c",       NIHIL, NIHIL, VERUM, FALSUM }
};

/* Typi quos officina.h possidet: unitates typedef cadunt ex
 * capitibus internis (definitio una per TU) */
interior constans character* constans CADENDA_TYPEDEF[] = {
    "Piscina", "chorda", "Xar",
    "MedullaTypus", "MedullaOp", "MedullaOperandumGenus",
    "MedullaOperandum", "MedullaInstructio", "MedullaBloccus",
    "MedullaParametrum", "MedullaFunctio", "MedullaSymbolumGenus",
    "MedullaSymbolum", "MedullaRelocatio", "MedullaDatum",
    "MedullaModulus", "MedullaLineae",
    "Regio",
    "ConexioSymbolumGenus", "ConexioDescriptor", "ConexioSymbolum",
    "ConexioFunctioNexa", "Conexio",
    "MachinulaExitusGenus", "MachinulaExitus", "Machinula", NIHIL
};

interior constans character* constans CADENDA_DEFINITIO[] = {
    NIHIL
};

/* Functiones vendicatae publicae manentes (hospes eas vocat) */
interior constans character* constans NON_STATICA[] = {
    "piscina_generare_dynamicum", "piscina_destruere",
    "xar_numerus", "xar_obtinere", "xar_obtinere_s", NIHIL
};

interior constans Renominatio TYPI_EXACTI[] = {
    { "Piscina",           "OfficinaPiscina" },
    { "PiscinaNotatio",    "OfficinaPiscinaNotatio" },
    { "chorda",            "OfficinaChorda" },
    { "chorda_fissio_fructus", "OfficinaChordaFissioFructus" },
    { "ChordaAedificator", "OfficinaChordaAedificator" },
    { "TabulaDispersa",    "OfficinaTabulaDispersa" },
    { "Xar",               "OfficinaXar" },
    { "XarIterator",       "OfficinaXarIterator" },
    { "XarComparator",     "OfficinaXarComparator" }
};

/* Longissimum primum! */
interior constans Renominatio PRAEFIXA_FUNCTIONUM[] = {
    { "chorda_aedificator_", "officina_chorda_aedificator_" },
    { "tabula_dispersa_",    "officina_tabula_dispersa_" },
    { "piscina_",            "officina_piscina_" },
    { "friatio_",            "officina_friatio_" },
    { "chorda_",             "officina_chorda_" },
    { "tabula_",             "officina_tabula_" },
    { "xar_",                "officina_xar_" }
};

interior constans character* constans PROOEMIUM =
    "/* officina.c - GENERATUM (amalgamator) - NE MANU EDITES\n"
    " *\n"
    " * Medulla (IR registrorum) + forma textualis + demissio in\n"
    " * plagula una (SQLite modo). DEPENDENTIA EXTERNA UNICA:\n"
    " * silva.h (officina a silva pendet EX ARCHITECTURA) -\n"
    " * compila cum -I<radix>/silva/amalgama et conecte silva.c.\n"
    " * Fons veritatis: officina/fontes/ + bibliothecae vendicatae\n"
    " * in lib/. Regenerare: officina/amalgamare.sh\n"
    " */\n\n"
    "#include \"silva.h\"\n\n";

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
    piscina = piscina_generare_dynamicum("amalgamator_officinae",
        33554432);
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
    manifestum.capita_propria = CAPITA_OFFICINAE;
    manifestum.numerus_capitum_propriorum = NUMERUS(CAPITA_OFFICINAE);
    manifestum.corpora_propria = CORPORA_OFFICINAE;
    manifestum.numerus_corporum_propriorum = NUMERUS(CORPORA_OFFICINAE);
    manifestum.cadenda_typedef = CADENDA_TYPEDEF;
    manifestum.cadenda_definitio = CADENDA_DEFINITIO;
    manifestum.non_statica = NON_STATICA;
    manifestum.typi_exacti = TYPI_EXACTI;
    manifestum.numerus_typorum = NUMERUS(TYPI_EXACTI);
    manifestum.praefixa_functionum = PRAEFIXA_FUNCTIONUM;
    manifestum.numerus_praefixorum = NUMERUS(PRAEFIXA_FUNCTIONUM);
    manifestum.via_capitis = "officina/amalgama/officina.h";
    manifestum.prooemium = PROOEMIUM;
    manifestum.assertiones = NIHIL;
    manifestum.latina_datum_via_capitis = NIHIL;
    manifestum.latina_datum_via_corporis = NIHIL;
    manifestum.latina_datum_prooemium = NIHIL;
    manifestum.latina_datum_custos = NIHIL;
    manifestum.latina_datum_titulus_textus = NIHIL;
    manifestum.latina_datum_titulus_mensurae = NIHIL;

    si (!silva_amalgama_currere(piscina, argv[I], argv[II],
            &manifestum))
    {
        piscina_destruere(piscina);
        redde I;
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
