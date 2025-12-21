#ifndef TOML_H
#define TOML_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"

/* ==================================================
 * TOML - Parser pro Subset TOML
 *
 * Subset sufficit pro nostris usibus:
 * - Chordae simplices: Key = "value"
 * - Numeri integri: Year = 1920 (etiam negativi)
 * - Tabulata chordarum: Tags = ["a", "b"]
 * - Chordae multilineae: Summary = """..."""
 * - Boolean: active = true/false
 * - Tabulae [section] (ignorantur, claves fiunt top-level)
 * ================================================== */

/* ==================================================
 * Genera Valorum
 * ================================================== */

nomen enumeratio {
    TOML_CHORDA     = I,      /* Chorda valor */
    TOML_NUMERUS    = II,     /* Integer valor */
    TOML_TABULATUM  = III,    /* Tabulatum chordarum (array) */
    TOML_BOOLEAN    = IV      /* Boolean valor (true/false) */
} TomlGenus;

/* ==================================================
 * Structurae
 * ================================================== */

/* TomlValor - Valor cum genere */
nomen structura {
    TomlGenus genus;
    unio {
        chorda   chorda_valor;
        s32      numerus_valor;
        Xar*     tabulatum_valor;  /* Xar de chorda */
        b32      boolean_valor;
    } datum;
} TomlValor;

/* TomlIntroitus - Par clavis-valor */
nomen structura {
    chorda     clavis;
    TomlValor  valor;
} TomlIntroitus;

/* TomlDocumentum - Documentum parsatum */
nomen structura {
    Xar*       introitus;       /* Xar de TomlIntroitus */
    Piscina*   piscina;
    b32        successus;
    chorda     error;
    i32        linea_erroris;
} TomlDocumentum;

/* ==================================================
 * Parsatio
 * ================================================== */

/* Legere TOML ex chorda
 * Redde: TomlDocumentum* (semper non-NIHIL, verificare successus)
 */
TomlDocumentum*
toml_legere(
    chorda   input,
    Piscina* piscina);

/* Legere TOML ex literis C */
TomlDocumentum*
toml_legere_literis(
    constans character* input,
    Piscina*            piscina);

/* ==================================================
 * Quaestio
 * ================================================== */

/* Capere valorem per clavim
 * Redde: TomlValor* vel NIHIL si non inventum
 */
TomlValor*
toml_capere(
    TomlDocumentum*     doc,
    constans character* clavis);

/* Commoditates pro typis specificis */

/* Capere chordam (redde chordam vacuam si non inventum) */
chorda
toml_capere_chorda(
    TomlDocumentum*     doc,
    constans character* clavis);

/* Capere numerum (redde 0 si non inventum) */
s32
toml_capere_numerum(
    TomlDocumentum*     doc,
    constans character* clavis);

/* Capere tabulatum (redde NIHIL si non inventum) */
Xar*
toml_capere_tabulatum(
    TomlDocumentum*     doc,
    constans character* clavis);

/* Capere boolean (redde FALSUM si non inventum) */
b32
toml_capere_boolean(
    TomlDocumentum*     doc,
    constans character* clavis);

/* Verificare si clavis existit */
b32
toml_habet(
    TomlDocumentum*     doc,
    constans character* clavis);

/* ==================================================
 * Utilitas
 * ================================================== */

/* Numerus introituum */
i32
toml_numerus_introituum(
    TomlDocumentum* doc);

/* Verificare si parsatio successit */
b32
toml_successus(
    TomlDocumentum* doc);

/* Capere nuntium erroris */
chorda
toml_error(
    TomlDocumentum* doc);

#endif /* TOML_H */
