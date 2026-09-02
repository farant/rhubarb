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
#include <string.h>

#define NUMERUS(series) ((i32)(magnitudo(series) / magnitudo((series)[0])))


/* ==================================================
 * Manifestum
 * ================================================== */

/* Excludenda: messis fixpunctorum -Wunused-function (S44 modo
 * silvae) - functio nova consumpta = introitum delere (compilatio
 * id clamat) */
/* Excludenda: GENERATA (messis compilatoris - punctum fixum
 * unused-function; introitus stali sponte cadunt in regeneratione).
 * Regeneratio: ./tools/amalgama_excludenda_generare.sh tessera */
#include "excludenda_generata.h"

/* Listae plagularum: GENERATAE ab aedile (unio clausurarum radicum
 * propriarum; ordo capitum = topologia stabilis). Politica manualis
 * in fontes_politica.sh (exclusio chordae typus-solum) et infra
 * (excludenda/renominationes) manet.
 * Regeneratio: ./tools/amalgama_fontes_generare.sh tessera */
#include "fontes_generata.h"

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
    manifestum.via_capitis = "tessera/amalgama/tessera.h";
    manifestum.prooemium = PROOEMIUM;
    manifestum.assertiones = NIHIL;  /* macra duplicata identica =
                                      * custodia derivae gratuita */
    manifestum.latina_datum_via_capitis       = NIHIL;   /* silvae solum */
    manifestum.latina_datum_via_corporis      = NIHIL;
    manifestum.latina_datum_prooemium         = NIHIL;
    manifestum.latina_datum_custos            = NIHIL;
    manifestum.latina_datum_titulus_textus    = NIHIL;
    manifestum.latina_datum_titulus_mensurae  = NIHIL;

    si (!silva_amalgama_currere(piscina, argv[I], argv[II],
            &manifestum))
    {
        piscina_destruere(piscina);
        redde I;
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
