/* toml_registrum.h - Registrum generum toml: caput generatum et
 * enumerationes locorum manu scriptae
 *
 * TomlGenus, TOML_GENUS_NUMERUS_GENERUM, TOML_REGISTRUM et
 * TOML_DIAGNOSTICA ex toml_registrum_coctum.h veniunt (GENERATUM ex
 * toml/grammatica/toml.registrum.stml per ./materia/coquere.sh).
 * Hic: enumerationes locorum per genus (ordo = ordo declarationis;
 * probatio_toml_registrum quamque per titulos asserit).
 *
 * Genera quae formam locorum communem habent enumerationem communem
 * habent (commentarium socios nominat); tabula locorum nominatorum
 * portae registri quodque genus seorsum enumerat.
 */

#ifndef TOML_REGISTRUM_H
#define TOML_REGISTRUM_H

#include "latina.h"
#include "toml_registrum_coctum.h"


/* ==================================================
 * Loci per genus
 * ================================================== */

nomen enumeratio { TOML_DOCUMENTUM_LIBERI = 0, TOML_DOCUMENTUM_CAUDA }
    TomlDocumentumLocus;

/* caput-tabulae et caput-seriei */
nomen enumeratio {
    TOML_CAPUT_TOK_APERTURA = 0, TOML_CAPUT_CLAVIS,
        TOML_CAPUT_TOK_CLAUSURA
} TomlCaputLocus;

nomen enumeratio { TOML_PAR_CLAVIS = 0, TOML_PAR_TOK_SIGNUM,
    TOML_PAR_VALOR }
    TomlParLocus;

nomen enumeratio { TOML_CLAVIS_PARTES = 0 } TomlClavisLocus;

/* chorda, numerus, boolean, tempus, comma, linea */
nomen enumeratio { TOML_LEXEMA_TOK = 0 } TomlLexemaLocus;

/* series et tabula-compacta */
nomen enumeratio {
    TOML_INCLUSA_TOK_APERTURA = 0, TOML_INCLUSA_LIBERI,
    TOML_INCLUSA_TOK_CLAUSURA
} TomlInclusaLocus;

nomen enumeratio { TOML_MALUM_TOKENS = 0 } TomlMalumLocus;

#endif /* TOML_REGISTRUM_H */
