/* amalgamator.c - Manifestum tesserae + principale (dev-time,
 * Phase C)
 *
 * Usus: amalgamator <radix_repositorii> <via_exitus>
 * e.g.: amalgamator ../.. amalgama/tessera.c
 *
 * MANIFESTUM EST CODEX; MECHANISMUS in silva/instrumenta/
 * silva_amalgama.{h,c} vivit (dependentia trans-plicaturarum
 * dev-time solum - bootstrap lexatoris silvae, sicut generator).
 *
 * Vendicata: latina (verbatim) + piscina + chorda_aedificator +
 * utf8. NB chorda.{h,c} NON vendicantur: tessera functionem chordae
 * NULLAM vocat (aedificator quoque nullam) - tessera.h typum
 * TesseraChorda solus possidet. Graphum clausum, minimum.
 */
#include "latina.h"
#include "piscina.h"
#include "silva_amalgama.h"
#include <stdio.h>

#define NUMERUS(series) ((i32)(magnitudo(series) / magnitudo((series)[0])))

/* ==================================================
 * Manifestum
 * ================================================== */

/* Excludenda: messis fixpunctorum -Wunused-function (S44 modo
 * silvae) - functio nova consumpta = introitum delere (compilatio
 * id clamat) */
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
interior constans character* constans EXCLUDENDA_AEDIFICATORIS[] = {
    "_evadere_json",
    "_format_duplex",
    "_format_integer_s32",
    "chorda_aedificator_appendere_chorda",
    "chorda_aedificator_appendere_evasus_json",
    "chorda_aedificator_appendere_f64",
    "chorda_aedificator_appendere_hex_i32",
    "chorda_aedificator_appendere_indentationem",
    "chorda_aedificator_appendere_lineam_novam",
    "chorda_aedificator_appendere_literis_evasus_json",
    "chorda_aedificator_appendere_repetita",
    "chorda_aedificator_appendere_s32",
    "chorda_aedificator_destruere",
    "chorda_aedificator_finire",
    "chorda_aedificator_indentatio_gradus",
    "chorda_aedificator_longitudo",
    "chorda_aedificator_pop_indentationem",
    "chorda_aedificator_push_indentationem",
    NIHIL
};
interior constans character* constans EXCLUDENDA_UTF8[] = {
    "utf8_numerare_runas",
    "utf8_prior_runa",
    "utf8_proxima_runa",
    NIHIL
};

interior constans AmalgamaPlagula CAPITA_VENDICATA[] = {
    { "include/piscina.h",            NIHIL, EXCLUDENDA_PISCINAE,      FALSUM, VERUM },
    { "include/chorda_aedificator.h", NIHIL, EXCLUDENDA_AEDIFICATORIS, FALSUM, VERUM },
    { "include/utf8.h",               NIHIL, EXCLUDENDA_UTF8,          FALSUM, VERUM }
};

interior constans AmalgamaPlagula CORPORA_VENDICATA[] = {
    { "lib/piscina.c",            NIHIL, EXCLUDENDA_PISCINAE,      VERUM, VERUM },
    { "lib/chorda_aedificator.c", NIHIL, EXCLUDENDA_AEDIFICATORIS, VERUM, VERUM },
    { "lib/utf8.c",               NIHIL, EXCLUDENDA_UTF8,          VERUM, VERUM }
};

interior constans AmalgamaPlagula CAPITA_TESSERAE[] = {
    { "tessera/fontes/tessera_cellula.h",       NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_pons.h",          NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_pons_memoriae.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_pons_posix.h",    NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_eventum.h",       NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_opus.h",          NIHIL, NIHIL, FALSUM, FALSUM }
};

interior constans AmalgamaPlagula CORPORA_TESSERAE[] = {
    { "tessera/fontes/tessera_cellula.c",       NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_pons_memoriae.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_pons_posix.c",    NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_eventum.c",       NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_opus.c",          NIHIL, NIHIL, VERUM, FALSUM }
};

/* Typi quos tessera.h possidet: unitates typedef cadunt ex
 * capitibus internis (definitio una per TU) */
interior constans character* constans CADENDA_TYPEDEF[] = {
    "Piscina", "ChordaAedificator",
    "TesseraCellula", "TesseraStilus", "TesseraLineaGenus",
    "TesseraPons", "TesseraPonsMemoriae",
    "TesseraFructus", "TesseraOpus",
    "TesseraEventumGenus", "TesseraClavis", "TesseraMusGenus",
    "TesseraEventum", "TesseraLector", NIHIL
};

/* Definitiones tag quas tessera.h PLENE possidet */
interior constans character* constans CADENDA_DEFINITIO[] = {
    "TesseraPons", "TesseraOpus", NIHIL
};

/* Functiones vendicatae publicae manentes */
interior constans character* constans NON_STATICA[] = {
    "piscina_generare_dynamicum", "piscina_destruere", NIHIL
};

interior constans Renominatio TYPI_EXACTI[] = {
    { "Piscina",           "TesseraPiscina" },
    { "PiscinaNotatio",    "TesseraPiscinaNotatio" },
    { "chorda",            "TesseraChorda" },
    { "ChordaAedificator", "TesseraChordaAedificator" }
};

/* Longissimum primum! */
interior constans Renominatio PRAEFIXA_FUNCTIONUM[] = {
    { "chorda_aedificator_", "tessera_chorda_aedificator_" },
    { "piscina_",            "tessera_piscina_" },
    { "chorda_",             "tessera_chorda_" },
    { "utf8_",               "tessera_utf8_" }
};

interior constans character* constans PROOEMIUM =
    "/* tessera.c - GENERATUM (amalgamator) - NE MANU EDITES\n"
    " *\n"
    " * Bibliotheca terminalis tessellata in plagula una (SQLite\n"
    " * modo). Capita POSIX (termios etc.) sublata infra - tessera\n"
    " * bibliotheca terminalis EST. Fons veritatis: tessera/fontes/\n"
    " * + bibliothecae vendicatae in lib/. Regenerare:\n"
    " * tessera/amalgamare.sh\n"
    " */\n\n";

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
    piscina = piscina_generare_dynamicum("amalgamator_tesserae",
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
    manifestum.capita_propria = CAPITA_TESSERAE;
    manifestum.numerus_capitum_propriorum = NUMERUS(CAPITA_TESSERAE);
    manifestum.corpora_propria = CORPORA_TESSERAE;
    manifestum.numerus_corporum_propriorum = NUMERUS(CORPORA_TESSERAE);
    manifestum.cadenda_typedef = CADENDA_TYPEDEF;
    manifestum.cadenda_definitio = CADENDA_DEFINITIO;
    manifestum.non_statica = NON_STATICA;
    manifestum.typi_exacti = TYPI_EXACTI;
    manifestum.numerus_typorum = NUMERUS(TYPI_EXACTI);
    manifestum.praefixa_functionum = PRAEFIXA_FUNCTIONUM;
    manifestum.numerus_praefixorum = NUMERUS(PRAEFIXA_FUNCTIONUM);
    manifestum.via_capitis = "tessera/amalgama/tessera.h";
    manifestum.prooemium = PROOEMIUM;
    manifestum.assertiones = NIHIL;  /* macra duplicata identica =
                                      * custodia derivae gratuita */
    manifestum.latina_datum_via_capitis = NIHIL;   /* silvae solum */
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
