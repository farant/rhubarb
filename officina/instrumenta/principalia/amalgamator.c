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
#include <string.h>

#define NUMERUS(series) ((i32)(magnitudo(series) / magnitudo((series)[0])))


/* ==================================================
 * Manifestum
 * ================================================== */

interior constans character* constans SERVANDA_FRIATIONIS[] = {
    "friatio_fnv1a", NIHIL
};

/* Excludenda: messis -Wunused-function - functio nova consumpta =
 * introitum delere (compilatio id clamat) */
/* Excludenda: GENERATA (messis compilatoris - punctum fixum
 * unused-function; introitus stali sponte cadunt in regeneratione).
 * Regeneratio: ./tools/amalgama_excludenda_generare.sh officina */
#include "excludenda_generata.h"

/* Listae plagularum: GENERATAE ab aedile (unio clausurarum radicum
 * propriarum; ordo capitum = topologia stabilis). Politica manualis
 * in fontes_politica.sh (silva.{h,c} = dependentia externa;
 * indicium.h = possessio capitis publici) et infra manet.
 * Regeneratio: ./tools/amalgama_fontes_generare.sh officina */
#include "fontes_generata.h"

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

s32
principale (
          s32   argc,
    character** argv)
{
               Piscina* piscina;
    AmalgamaManifestum  manifestum;

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
