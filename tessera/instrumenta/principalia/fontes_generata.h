/* fontes_generata.h - GENERATUM AB AEDILE - NE MANU EDITES
 *
 * Listae plagularum amalgamatoris (tessera) ex clausuris
 * derivatis; politica (servanda/excludenda/exclusa) manualis in
 * fontes_politica.sh + amalgamator.c manet. Ordo capitum =
 * topologia stabilis (praelatio = emissio prior).
 * Regeneratio: ./tools/amalgama_fontes_generare.sh tessera
 */

interior constans AmalgamaPlagula CAPITA_VENDICATA[] = {
    { "include/piscina.h", NIHIL, EXCLUDENDA_PISCINAE, FALSUM, VERUM },
    { "include/chorda_aedificator.h", NIHIL, EXCLUDENDA_AEDIFICATORIS, FALSUM, VERUM },
    { "include/utf8.h", NIHIL, EXCLUDENDA_UTF8, FALSUM, VERUM }
};

interior constans AmalgamaPlagula CORPORA_VENDICATA[] = {
    { "lib/piscina.c", NIHIL, EXCLUDENDA_PISCINAE, VERUM, VERUM },
    { "lib/chorda_aedificator.c", NIHIL, EXCLUDENDA_AEDIFICATORIS, VERUM, VERUM },
    { "lib/utf8.c", NIHIL, EXCLUDENDA_UTF8, VERUM, VERUM }
};

interior constans AmalgamaPlagula CAPITA_PROPRIA[] = {
    { "tessera/fontes/tessera_cellula.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_pons.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_pons_memoriae.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_pons_posix.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_eventum.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "tessera/fontes/tessera_opus.h", NIHIL, NIHIL, FALSUM, FALSUM }
};

interior constans AmalgamaPlagula CORPORA_PROPRIA[] = {
    { "tessera/fontes/tessera_cellula.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_pons_memoriae.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_pons_posix.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_eventum.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "tessera/fontes/tessera_opus.c", NIHIL, NIHIL, VERUM, FALSUM }
};
