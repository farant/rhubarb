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
#include <string.h>

#define NUMERUS(series) ((i32)(magnitudo(series) / magnitudo((series)[0])))

/* ==================================================
 * Manifestum
 * ================================================== */

/* CAVE: lista stans stat dum clausura viva crescit - stml/
 * internamentum (2026-07-21) vias literis tabulae vivas fecerunt,
 * quarum callees friatio hic addendae erant (semen cascatae
 * signi B - custos PROTECTA nunc clamat) */
interior constans character* constans SERVANDA_FRIATIONIS[] = {
    "friatio_fnv1a", "friatio_fnv1a_literis", NIHIL
};

/* S44 (finis): static internalizatio x -Werror functiones inusitatas
 * in defectus compilationis vertit - manifestum functiones NON
 * consumptas nominatim demittit. Indices ex compilatore ipso lectae
 * (-Wunused-function harvest); functio nova consumpta = introitum
 * delere (compilatio id clamat). */
/* Excludenda: GENERATA (messis compilatoris - punctum fixum
 * unused-function; introitus stali sponte cadunt in regeneratione).
 * Regeneratio: ./tools/amalgama_excludenda_generare.sh silva */
#include "excludenda_generata.h"

/* Listae plagularum: GENERATAE ab aedile (unio clausurarum radicum
 * propriarum; ordo capitum = topologia stabilis - S42 dimidiatum:
 * corpora ordine LIBERA, probatio permutationis Phasis 0). Politica
 * manualis in fontes_politica.sh (tabulae fixturarum numquam
 * amalgantur) et infra (excludenda/servanda/renominationes) manet.
 * Regeneratio: ./tools/amalgama_fontes_generare.sh silva */
#include "fontes_generata.h"

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
    "SilvaRamusVista", "SilvaMacroVista", "SilvaCommentariumVista",
    "SilvaQuaestio", "SilvaQuaestioResultatum",
    "SilvaQuaestioCaptura",
    /* QC (2026-07-22): tabula pseudo opaca + functio usoris -
     * silva.h eas praenuntiat (registro-idioma SilvaQuaestio) */
    "SilvaQuaestioPseudoRegistrum", "SilvaQuaestioPseudoFunctio",
    /* quaestiones nominatae (2026-07-31): bibliotheca selectorum -
     * silva.h structuras transparentes possidet */
    "SilvaQuaestionesGradus", "SilvaQuaestioNominata",
    "SilvaQuaestionesArgumentum", "SilvaQuaestiones",
    "TypusC89Genus", "TypusC89Primitivum", "TypusC89",
    "TypusC89Membrum", "SemanticaSymbolumGenus", "SemanticaSymbolum",
    "SemanticaDiagnosticum", "SilvaSemantica",
    "ExamenSeveritas", "ExamenCodex", "ExamenLicentia",
    /* annotationes STML (2026-07-21): typi stml quos silva.h
     * possidet + typi annotationum proprii + internamentum
     * praenuntiatum (idioma SilvaOraculum) */
    "StmlNodusGenus", "StmlCaptioDirectio", "StmlStatus",
    "StmlAttributum", "StmlNodus", "StmlResultus",
    "InternamentumChorda",
    "SilvaAnnotatioModus", "SilvaAnnotatio",
    /* arbor: STML canonicum (M1, 2026-08-20) - silva.h typos
     * publicos possidet; enumeratio modorum quoque cadit (C89
     * enumeratores duplicatos vetat, non solum typedef) */
    "SilvaArborScriptura", "SilvaArborComparatioModus",
    "SilvaArborDifferentia", "SilvaArborVitium",
    NIHIL
};

/* Definitiones tag quas silva.h PLENE possidet (definitio structurae
 * interna quoque cadit). SilvaOraculum/SilvaExpansio NON hic: silva.h
 * eas solum praenuntiat - definitiones internae manent (interna eas
 * dereferunt). */
interior constans character* constans CADENDA_DEFINITIO[] = {
    "SilvaToken", "SilvaValor", "SilvaNodus", "SilvaCaecatio",
    "SilvaContextus", "TypusC89", "StmlNodus", NIHIL
};

/* Functiones vendicatae quae PUBLICAE manent (extern, in silva.h) */
interior constans character* constans NON_STATICA[] = {
    "piscina_generare_dynamicum", "piscina_destruere",
    "piscina_vacare",
    "piscina_summa_usus", "piscina_summa_apex_usus",
    "xar_numerus", "xar_obtinere",
    /* subcopia stml legendi (annotationes, 2026-07-21) */
    "stml_legere", "stml_invenire_liberum",
    "stml_attributum_capere", "stml_attributum_habet",
    "stml_textus_internus", "stml_numerus_liberorum",
    "stml_liberum_ad_indicem", "internamentum_creare", NIHIL
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
    { "XarComparator",         "SilvaXarComparator" },
    { "StmlNodus",             "SilvaStmlNodus" },
    { "StmlNodusGenus",        "SilvaStmlNodusGenus" },
    { "StmlStatus",            "SilvaStmlStatus" },
    { "StmlCaptioDirectio",    "SilvaStmlCaptioDirectio" },
    { "StmlAttributum",        "SilvaStmlAttributum" },
    { "StmlResultus",          "SilvaStmlResultus" },
    { "InternamentumChorda",   "SilvaInternamentumChorda" }
};

/* Praefixa functionum - longissimum primum (chorda_aedificator_
 * ante chorda_!) */
interior constans Renominatio PRAEFIXA_FUNCTIONUM[] = {
    { "chorda_aedificator_", "silva_chorda_aedificator_" },
    { "tabula_dispersa_",    "silva_tabula_dispersa_" },
    { "internamentum_",      "silva_internamentum_" },
    { "friatio_",            "silva_friatio_" },
    { "selectio_",           "silva_selectio_" },
    { "piscina_",            "silva_piscina_" },
    { "chorda_",             "silva_chorda_" },
    { "stml_",               "silva_stml_" },
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

/* Listas manifesti machinis emittere (tools/amalgama_auditor.sh) */
interior vacuum
_enumerare (vacuum)
{
    i32 i;

    per (i = ZEPHYRUM; i < NUMERUS(CAPITA_VENDICATA); i++)
    {
        imprimere("CV\t%s\n", CAPITA_VENDICATA[i].via);
    }
    per (i = ZEPHYRUM; i < NUMERUS(CORPORA_VENDICATA); i++)
    {
        imprimere("OV\t%s\n", CORPORA_VENDICATA[i].via);
    }
    per (i = ZEPHYRUM; i < NUMERUS(CAPITA_PROPRIA); i++)
    {
        imprimere("CP\t%s\n", CAPITA_PROPRIA[i].via);
    }
    per (i = ZEPHYRUM; i < NUMERUS(CORPORA_PROPRIA); i++)
    {
        imprimere("OP\t%s\n", CORPORA_PROPRIA[i].via);
    }
}

s32 principale (s32 argc, character** argv)
{
    Piscina* piscina;
    AmalgamaManifestum manifestum;

    si (argc == II && strcmp(argv[I], "--enumerare") == ZEPHYRUM)
    {
        _enumerare();
        redde ZEPHYRUM;
    }
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
    manifestum.capita_propria = CAPITA_PROPRIA;
    manifestum.numerus_capitum_propriorum = NUMERUS(CAPITA_PROPRIA);
    manifestum.corpora_propria = CORPORA_PROPRIA;
    manifestum.numerus_corporum_propriorum = NUMERUS(CORPORA_PROPRIA);
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
