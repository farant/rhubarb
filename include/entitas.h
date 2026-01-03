#ifndef ENTITAS_H
#define ENTITAS_H

#include "latina.h"
#include "chorda.h"
#include "xar.h"
#include "piscina.h"
#include "uuid.h"
#include "internamentum.h"

/* ==================================================
 * Typus Literalis (Literal/Parsed Types)
 * ================================================== */

/* Typus literalis pro proprietatibus
 * Discriminator pro unione valoris parsiti
 */
nomen enumeratio {
    TYPUS_NIHIL   = 0,   /* Non definitus / generic chorda */
    TYPUS_CHORDA  = I,   /* Chorda (default) */
    TYPUS_S32     = II,  /* Signed 32-bit integer */
    TYPUS_S64     = III, /* Signed 64-bit integer */
    TYPUS_F64     = IV,  /* 64-bit floating point */
    TYPUS_B32     = V,   /* Boolean */
    TYPUS_TEMPUS  = VI,  /* Timestamp (s64 milliseconds since epoch) */
    TYPUS_BLOBUM  = VII  /* Binary blob, auto-compressed with flatura */
} TypusLiteralis;

/* Convertere chorda typus literalis ad enum
 * Accepta: "chorda", "s32", "s64", "f64", "b32", "tempus"
 * Redde: TypusLiteralis enum valor, vel TYPUS_NIHIL si non cognitus
 */
TypusLiteralis
typus_literalis_ex_chorda(
    chorda typus);


/* ==================================================
 * Structurae
 * ================================================== */

/* Proprietas - Par clavis-valoris cum typo
 * "Property: key-value pair with type information"
 */
nomen structura {
    chorda*        clavis;            /* Clavis (internata) */
    chorda*        valor;             /* Valor originalis (chorda, internata) */
    chorda*        typus_semanticus;  /* e.g., "Currency::USD", NIHIL si generic */
    TypusLiteralis typus_literalis;   /* Discriminator pro unione */
    b32            parsitus_validus;  /* VERUM si parsing successit */
    unio {
        s32 ut_s32;
        s64 ut_s64;
        f64 ut_f64;
        b32 ut_b32;
        s64 ut_tempus;  /* milliseconds since epoch */
    } parsitus;
} Proprietas;

/* Parsare proprietatem secundum typum specificatum
 * Caches result in prop->parsitus si successus
 * Redde: VERUM si parsing successit, FALSUM si error
 */
b32
proprietas_parsare_ut_typum(
    Proprietas*    prop,
    TypusLiteralis typus);

/* Relatio - Arcus directus ad aliam entitatem
 * "Relationship: directed edge to another entity"
 * Habet ID proprium (UUIDv7) pro quaestionibus et deletionibus
 */
nomen structura {
    chorda* id;            /* ID relationis (UUIDv7, internatum) */
    chorda* origo_id;      /* ID entitatis originis (internatum) */
    chorda* genus;         /* Genus relationis (internatum) */
    chorda* destinatio_id; /* ID entitatis destinationis (internatum) */
} Relatio;

/* Entitas - Entitas cum proprietatibus, relationibus, notis
 * "Entity with properties, relationships, tags"
 */
nomen structura {
    chorda* id;                        /* ID unicum (internatum) */
    chorda* genus;                     /* Nomen generis (internatum) */
    Xar*    proprietates;              /* Xar de Proprietas */
    Xar*    relationes;                /* Xar de Relatio */
    Xar*    notae;                     /* Xar de chorda* (notae internatas) */
    Xar*    proprietas_definitiones;   /* Cache: Xar de Entitas* (ProprietasDefinitio) */
} Entitas;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare novam entitatem
 * Omnes chordae debent iam internatae esse
 *
 * Redde: Entitas nova, vel NIHIL si fractura
 */
Entitas*
entitas_creare(
    Piscina* piscina,
    chorda*  id,
    chorda*  genus);


/* ==================================================
 * Proprietates
 * ================================================== */

/* Ponere proprietatem (clavis-valor par)
 * Si clavis iam existit, valor renovatur (upsert)
 *
 * Redde: VERUM si successus, FALSUM si fractura
 */
b32
entitas_proprietas_ponere(
    Entitas* entitas,
    chorda*  clavis,
    chorda*  valor);

/* Capere valorem proprietatis per clavis (chorda)
 * Quaestio linearis per omnes proprietates
 *
 * Redde: Valor si inventum, NIHIL si non inventum
 */
chorda*
entitas_proprietas_capere(
    Entitas* entitas,
    chorda*  clavis);

/* Capere proprietatem plenam (pro inspectione typi)
 *
 * Redde: Proprietas* si inventum, NIHIL si non inventum
 */
Proprietas*
entitas_proprietas_capere_plena(
    Entitas* entitas,
    chorda*  clavis);

/* Typed accessors - redde VERUM si successus et valor est typi correcti */
b32
entitas_proprietas_capere_s32(
    Entitas* entitas,
    chorda*  clavis,
    s32*     valor);

b32
entitas_proprietas_capere_s64(
    Entitas* entitas,
    chorda*  clavis,
    s64*     valor);

b32
entitas_proprietas_capere_f64(
    Entitas* entitas,
    chorda*  clavis,
    f64*     valor);

b32
entitas_proprietas_capere_b32(
    Entitas* entitas,
    chorda*  clavis,
    b32*     valor);

b32
entitas_proprietas_capere_tempus(
    Entitas* entitas,
    chorda*  clavis,
    s64*     valor);

/* Ponere proprietatem blobum (datum binarium)
 * Datum comprimetur automatice cum flatura gzip
 *
 * entitas: entitas
 * clavis: clavis proprietatis (internata)
 * datum: datum binarium crudus
 * mensura: longitudo dati in bytes
 * piscina: piscina pro allocare datum compressum
 *
 * Redde: VERUM si successus, FALSUM si fractura
 */
b32
entitas_proprietas_ponere_blobum(
    Entitas*     entitas,
    chorda*      clavis,
    const i8*    datum,
    i32          mensura,
    Piscina*     piscina);

/* Capere proprietatem blobum (datum binarium)
 * Datum decomprimetur automatice ex flatura gzip
 *
 * entitas: entitas
 * clavis: clavis proprietatis (internata)
 * datum: [output] indicem ad datum decompressum
 * mensura: [output] longitudo dati decompressum
 * piscina: piscina pro allocare datum decompressum
 *
 * Redde: VERUM si successus, FALSUM si non inventum vel fractura
 */
b32
entitas_proprietas_capere_blobum(
    Entitas*     entitas,
    chorda*      clavis,
    i8**         datum,
    i32*         mensura,
    Piscina*     piscina);

/* Verificare si entitas proprietatem habet
 *
 * Redde: VERUM si habet, FALSUM si non
 */
b32
entitas_proprietas_habet(
    Entitas* entitas,
    chorda*  clavis);

/* Delere proprietatem per clavis
 * Usus swap-and-pop: proprietas ultima movetur ad locum deletum
 *
 * Redde: VERUM si deletum, FALSUM si non inventum
 */
b32
entitas_proprietas_delere(
    Entitas* entitas,
    chorda*  clavis);


/* ==================================================
 * Relationes
 * ================================================== */

/* Addere relationem ad aliam entitatem
 * Generat UUIDv7 pro ID relationis
 *
 * Redde: Relatio* si successus, NIHIL si fractura
 */
Relatio*
entitas_relatio_addere(
    Entitas*             entitas,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              genus,
    chorda*              destinatio_id);

/* Addere relationem cum ID specifico (pro replay eventuum)
 * Non generat UUIDv7 - usare ID provisum
 *
 * Redde: Relatio* si successus, NIHIL si fractura
 */
Relatio*
entitas_relatio_addere_cum_id(
    Entitas*             entitas,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              relatio_id,
    chorda*              genus,
    chorda*              destinatio_id);

/* Capere relationem per ID
 *
 * Redde: Relatio* si inventum, NIHIL si non inventum
 */
Relatio*
entitas_relatio_capere(
    Entitas* entitas,
    chorda*  relatio_id);

/* Capere omnes relationes generis specificati
 * Allocat novum Xar cum relationibus generis
 *
 * Redde: Xar de Relatio*, vel NIHIL si nullae
 */
Xar*
entitas_relationes_generis_capere(
    Entitas* entitas,
    chorda*  genus,
    Piscina* piscina);

/* Delere relationem per ID
 * Usus swap-and-pop: relatio ultima movetur ad locum deletum
 *
 * Redde: VERUM si deletum, FALSUM si non inventum
 */
b32
entitas_relatio_delere(
    Entitas* entitas,
    chorda*  relatio_id);


/* ==================================================
 * Notae (Tags)
 * ================================================== */

/* Addere notam ad entitatem
 *
 * Redde: VERUM si successus, FALSUM si fractura
 */
b32
entitas_nota_addere(
    Entitas* entitas,
    chorda*  nota);

/* Verificare si entitas notam habet
 * Usus aequalitas indicis (chordae internatae)
 *
 * Redde: VERUM si habet, FALSUM si non
 */
b32
entitas_nota_habet(
    Entitas* entitas,
    chorda*  nota);

/* Verificare si entitas notam cum praefixo habet
 * e.g., "#project::" matches "#project::active"
 *
 * Redde: VERUM si habet, FALSUM si non
 */
b32
entitas_nota_cum_praefixo_habet(
    Entitas*             entitas,
    constans character*  praefixum);

/* Delere notam ab entitate
 * Usus swap-and-pop: nota ultima movetur ad locum deletum
 *
 * Redde: VERUM si deletum, FALSUM si non inventum
 */
b32
entitas_nota_delere(
    Entitas* entitas,
    chorda*  nota);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Capere titulum entitatis pro monstrando
 * Ordo praeferentiae: proprietas "name", proprietas "title", id
 *
 * Redde: chorda* ad titulum (non allocata, indicat ad datum internum)
 */
chorda*
entitas_titulum_capere(
    Entitas* entitas);

/* Capere numerum proprietatum
 */
i32
entitas_numerus_proprietatum(
    Entitas* entitas);

/* Capere numerum relationum
 */
i32
entitas_numerus_relationum(
    Entitas* entitas);

/* Capere numerum notarum
 */
i32
entitas_numerus_notarum(
    Entitas* entitas);

/* Imprimere entitatem pro depuratione
 */
vacuum
entitas_imprimere(
    Entitas* entitas);

#endif /* ENTITAS_H */
