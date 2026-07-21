/* fontes_generata.h - GENERATUM AB AEDILE - NE MANU EDITES
 *
 * Listae plagularum amalgamatoris (officina) ex clausuris
 * derivatis; politica (servanda/excludenda/exclusa) manualis in
 * fontes_politica.sh + amalgamator.c manet. Ordo capitum =
 * topologia stabilis (praelatio = emissio prior).
 * Regeneratio: ./tools/amalgama_fontes_generare.sh officina
 */

interior constans AmalgamaPlagula CAPITA_VENDICATA[] = {
    { "include/piscina.h", NIHIL, EXCLUDENDA_PISCINAE, FALSUM, VERUM },
    { "include/chorda.h", NIHIL, EXCLUDENDA_CHORDAE, FALSUM, VERUM },
    { "include/chorda_aedificator.h", NIHIL, EXCLUDENDA_AEDIFICATORIS, FALSUM, VERUM },
    { "include/friatio.h", SERVANDA_FRIATIONIS, NIHIL, FALSUM, VERUM },
    { "include/tabula_dispersa.h", NIHIL, EXCLUDENDA_TABULAE, FALSUM, VERUM },
    { "include/xar.h", NIHIL, EXCLUDENDA_XARIS, FALSUM, VERUM }
};

interior constans AmalgamaPlagula CORPORA_VENDICATA[] = {
    { "lib/piscina.c", NIHIL, EXCLUDENDA_PISCINAE, VERUM, VERUM },
    { "lib/chorda.c", NIHIL, EXCLUDENDA_CHORDAE, VERUM, VERUM },
    { "lib/chorda_aedificator.c", NIHIL, EXCLUDENDA_AEDIFICATORIS, VERUM, VERUM },
    { "lib/friatio.c", SERVANDA_FRIATIONIS, NIHIL, VERUM, VERUM },
    { "lib/tabula_dispersa.c", NIHIL, EXCLUDENDA_TABULAE, VERUM, VERUM },
    { "lib/xar.c", NIHIL, EXCLUDENDA_XARIS, VERUM, VERUM }
};

interior constans AmalgamaPlagula CAPITA_PROPRIA[] = {
    { "officina/fontes/officina_medulla.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_medulla_textus.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_regio.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_conexio.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_machinula.h", NIHIL, NIHIL, FALSUM, FALSUM },
    { "officina/fontes/officina_demissio.h", NIHIL, NIHIL, FALSUM, FALSUM }
};

interior constans AmalgamaPlagula CORPORA_PROPRIA[] = {
    { "officina/fontes/officina_medulla.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_medulla_textus.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_regio.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_conexio.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_machinula.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_demissio.c", NIHIL, NIHIL, VERUM, FALSUM },
    { "officina/fontes/officina_indicium.c", NIHIL, NIHIL, VERUM, FALSUM }
};
