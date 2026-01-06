#ifndef ARBOR_TYPUS_H
#define ARBOR_TYPUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "arbor_syntaxis.h"

/* ==================================================
 * ARBOR TYPUS - Type Resolution
 *
 * Resolvit references typorum, aedificat graphum typorum.
 * Separatus passus post parsing.
 *
 * PHILOSOPHIA:
 * - Typi primitivi (void, char, int, etc.)
 * - Typi compositi (pointer, array, function)
 * - Typi aggregati (struct, union, enum)
 * - Typedef resolution
 * - Scope tracking pro variabilibus localibus
 *
 * EXEMPLUM:
 *   ArborTypusResolver* res = arbor_typus_creare(piscina, intern);
 *   arbor_typus_resolvere(res, radix);
 *   ArborTypus* t = arbor_typus_expressionis(res, expr);
 * ================================================== */

/* ==================================================
 * Type Kinds
 * ================================================== */

nomen enumeratio {
    ARBOR_TYPUS_VOID,
    ARBOR_TYPUS_CHAR,
    ARBOR_TYPUS_SHORT,
    ARBOR_TYPUS_INT,
    ARBOR_TYPUS_LONG,
    ARBOR_TYPUS_FLOAT,
    ARBOR_TYPUS_DOUBLE,
    ARBOR_TYPUS_STRUCT,
    ARBOR_TYPUS_UNION,
    ARBOR_TYPUS_ENUM,
    ARBOR_TYPUS_POINTER,
    ARBOR_TYPUS_ARRAY,
    ARBOR_TYPUS_FUNCTION,
    ARBOR_TYPUS_TYPEDEF
} ArborTypusGenus;

/* ==================================================
 * Type Structure
 * ================================================== */

nomen structura ArborTypus ArborTypus;

structura ArborTypus {
    ArborTypusGenus    genus;
    chorda*            titulus;         /* Pro struct/union/enum/typedef tag */
    ArborTypus*        basis;           /* Pro pointer/array/typedef */
    s32                array_mensura;   /* -1 pro unsized [] */
    Xar*               parametra;       /* Functio param types (Xar of ArborTypus*) */
    ArborTypus*        reditum;         /* Functio return type */
    Xar*               membra;          /* Struct/union members (Xar of ArborMembrum*) */
    b32                est_const;
    b32                est_volatile;
    b32                est_signed;
    b32                est_unsigned;
};

/* ==================================================
 * Struct/Union Member
 * ================================================== */

nomen structura {
    chorda*            titulus;
    ArborTypus*        typus;
    i32                bitfield_width;  /* -1 si non bitfield */
} ArborMembrum;

/* ==================================================
 * Symbol in Scope
 * ================================================== */

nomen structura {
    chorda*            titulus;
    ArborTypus*        typus;
    ArborNodus*        declaratio;      /* AST node where declared */
} ArborSymbolum;

/* ==================================================
 * Type Resolver State (opaque)
 * ================================================== */

nomen structura ArborTypusResolver ArborTypusResolver;

/* ==================================================
 * API - Creation
 * ================================================== */

/* Creare type resolver */
ArborTypusResolver* arbor_typus_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* ==================================================
 * API - Resolution
 * ================================================== */

/* Resolvere typos in AST (modifies nodes, sets typus_resolutum) */
vacuum arbor_typus_resolvere(
    ArborTypusResolver*   res,
    ArborNodus*           radix);

/* ==================================================
 * API - Queries
 * ================================================== */

/* Obtinere typum expressionis */
ArborTypus* arbor_typus_expressionis(
    ArborTypusResolver*   res,
    ArborNodus*           expr);

/* Obtinere declarationem identificatoris */
ArborNodus* arbor_typus_declaratio(
    ArborTypusResolver*   res,
    chorda*               titulus);

/* Obtinere typum identificatoris */
ArborTypus* arbor_typus_identificatoris(
    ArborTypusResolver*   res,
    chorda*               titulus);

/* ==================================================
 * API - Type Utilities
 * ================================================== */

/* Obtinere nomen genus */
constans character* arbor_typus_genus_nomen(ArborTypusGenus genus);

/* Imprimere typum pro depuratione */
vacuum arbor_typus_imprimere(ArborTypus* typus);

/* Comparare duos typos (structural equality) */
b32 arbor_typus_aequalis(ArborTypus* a, ArborTypus* b);

/* Obtinere basis typum (unwrap typedef) */
ArborTypus* arbor_typus_basis(ArborTypus* typus);

/* Est typus integer? */
b32 arbor_typus_est_integer(ArborTypus* typus);

/* Est typus arithmetic? (integer vel floating) */
b32 arbor_typus_est_arithmeticus(ArborTypus* typus);

/* Est typus scalar? (arithmetic vel pointer) */
b32 arbor_typus_est_scalaris(ArborTypus* typus);

#endif /* ARBOR_TYPUS_H */
