/* toml_registrum_coctum.h
 *
 * Registrum generum COCTUM grammaticae 'toml' - GENERATUM, NE MANU
 * EDITES. Fons: toml/grammatica/toml.registrum.stml
 * (materia/coquere.sh). Genera XIV, loci XXV.
 */

#ifndef TOML_REGISTRUM_COCTUM_H
#define TOML_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* Radix: sententiae (par, caput-tabulae, caput-seriei, linea,
     * malum) ordine octetorum + lexema FINIS (trivia caudalia fert) */
    TOML_GENUS_DOCUMENTUM = 0,
    /* [clavis]: tabula currens mutatur */
    TOML_GENUS_CAPUT_TABULAE,
    /* [[clavis]]: elementum novum seriei tabularum */
    TOML_GENUS_CAPUT_SERIEI,
    /* clavis = valor */
    TOML_GENUS_PAR,
    /* Clavis punctata: segmenta (nuda, gemina, simplex) et puncta
     * ordine octetorum; spatia circa puncta trivia sunt */
    TOML_GENUS_CLAVIS,
    /* Quattuor formae (gemina, simplex, utraque multa): genus
     * lexematis dicit */
    TOML_GENUS_CHORDA,
    /* Integer (decimalis, hex, octalis, binarius) aut fluitans (inf,
     * nan): coctio classem dicit */
    TOML_GENUS_NUMERUS,
    /* true aut false */
    TOML_GENUS_BOOLEAN,
    /* Quattuor genera (cum zona, locale, dies, hora): coctio dicit */
    TOML_GENUS_TEMPUS,
    /* [v, v]: valores, commata, lineae et commentaria intus
     * (trivia); comma caudale licitum */
    TOML_GENUS_SERIES,
    /* {k = v, ...}: una linea (TOML 1.0), clausa post scriptionem
     * (coctio iudicat) */
    TOML_GENUS_TABULA_COMPACTA,
    /* ',' inter valores seriei aut paria tabulae compactae */
    TOML_GENUS_COMMA,
    /* Linea nova quae sententiam terminat (alibi trivium est) */
    TOML_GENUS_LINEA,
    /* Totalitas (T4): lexemata quae grammatica ponere non potuit -
     * octeti manent, sanitas negatur */
    TOML_GENUS_MALUM,

    TOML_GENUS_NUMERUS_GENERUM
} TomlGenus;

externus constans MateriaRegistrumCoctum TOML_REGISTRUM;

externus constans MateriaDiagnosticaCocta TOML_DIAGNOSTICA;

#endif /* TOML_REGISTRUM_COCTUM_H */
