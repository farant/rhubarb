/* nuntium_schema.h - Lingua Schematis Nuntii
 *
 * Legit definitionem schematis declarativam et construit AST.
 * Schema describunt nuntia cum campis typatis et numeris tag.
 *
 * Exemplum schematis:
 *   nuntium Persona {
 *       textus identificator = 1;
 *       varint aetas = 2;
 *   }
 *
 * Usus:
 *   NuntiumSchemaNodus* radix = nuntium_schema_legere(piscina, fons, longitudo);
 *   si (radix) { ... radix->datum.schema.definitiones ... }
 */

#ifndef NUNTIUM_SCHEMA_H
#define NUNTIUM_SCHEMA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"

/* ================================================
 * Genus Campi - Typus campi in nuntio
 * ================================================ */

nomen enumeratio {
    NUNTIUM_CAMPUS_TEXTUS,
    NUNTIUM_CAMPUS_VARINT,
    NUNTIUM_CAMPUS_SVARINT,
    NUNTIUM_CAMPUS_FIXUM32,
    NUNTIUM_CAMPUS_FIXUM64,
    NUNTIUM_CAMPUS_BYTES,
    NUNTIUM_CAMPUS_F32,
    NUNTIUM_CAMPUS_F64
} NuntiumCampusGenus;

/* ================================================
 * Genus Nodi AST
 * ================================================ */

nomen enumeratio {
    NUNTIUM_SCHEMA_NODUS_SCHEMA,
    NUNTIUM_SCHEMA_NODUS_DEFINITIO,
    NUNTIUM_SCHEMA_NODUS_CAMPUS
} NuntiumSchemaNodusGenus;

/* ================================================
 * Nodus AST
 * ================================================ */

nomen structura NuntiumSchemaNodus NuntiumSchemaNodus;
structura NuntiumSchemaNodus {
    NuntiumSchemaNodusGenus genus;
    unio {
        structura {
            Xar* definitiones;   /* Xar de NuntiumSchemaNodus* */
        } schema;
        structura {
            chorda* titulus;     /* Nomen nuntii (internatum) */
            Xar*    campi;       /* Xar de NuntiumSchemaNodus* */
        } definitio;
        structura {
            NuntiumCampusGenus campus_genus;
            chorda*            titulus;   /* Nomen campi (internatum) */
            s32                tag;       /* Numerus tag */
        } campus;
    } datum;
};

/* ================================================
 * Contextus Parsationis (pro reductio callback)
 * ================================================ */

nomen structura {
    constans character* fons;
    s32                 longitudo_fontis;
    Piscina*            piscina;
    Xar*                nodi;    /* Xar de NuntiumSchemaNodus* */
} NuntiumSchemaContextus;

/* ================================================
 * Functio Principalis
 * ================================================ */

/* Legere schema ex fonte textuali
 *
 * piscina    - Piscina pro allocationibus
 * fons       - Fons textualis schematis
 * longitudo  - Longitudo fontis in bytes
 *
 * Redde: NuntiumSchemaNodus* (radix, genus = SCHEMA), vel NIHIL si error
 */
NuntiumSchemaNodus*
nuntium_schema_legere(
    Piscina*             piscina,
    constans character*  fons,
    s32                  longitudo);

#endif /* NUNTIUM_SCHEMA_H */
