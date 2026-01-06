#ifndef ARBOR_FORMATOR_H
#define ARBOR_FORMATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "arbor_syntaxis.h"

/* ===========================================================
 * ARBOR FORMATOR - C89 Code Formatter
 *
 * Duo modi operandi:
 * - Fidelis: Exacta byte-per-byte roundtrip (trivia conservata)
 * - Formatus: Applicare regulas formationis
 *
 * STYLUS (ex lib/piscina.c):
 * - Allman braces
 * - Typi dextrorsum ordinati cum 2-char gutter
 * - Assignationes ordinatae per = signum
 * - Functiones: return type in linea sua, params singuli
 * - Casts: ( Type* ) expr cum spatiis
 * - Return: redde ( x ); cum spatiis
 *
 * EXEMPLUM:
 *   ArborFormatorOptiones opt = arbor_formator_optiones_default();
 *   chorda fructus = arbor_formator_emittere(piscina, radix, &opt);
 * =========================================================== */

/* ===========================================================
 * OPTIONES
 * =========================================================== */

nomen structura {
    b32      latinizare_keywords;     /* Convertere C keywords ad Latinum */
    b32      romanizare_numeros;      /* Convertere numeros ad Romanos */
    i32      max_linea_longitudo;     /* Default: LXXX (80) */
    i32      indentatio_spatia;       /* Default: II (2) */
    chorda*  latina_via;              /* Via ad latina.h pro numeris novis */
} ArborFormatorOptiones;

/* ===========================================================
 * API - Optiones
 * =========================================================== */

/* Obtinere optiones default */
ArborFormatorOptiones arbor_formator_optiones_default (
    vacuum);

/* ===========================================================
 * API - Formatus Mode
 * =========================================================== */

/* Emittere cum regulis formationis */
chorda arbor_formator_emittere (
                   Piscina* piscina,
                ArborNodus* radix,
    ArborFormatorOptiones*  optiones);

/* Emittere singulum nodum/subarborem */
chorda arbor_formator_emittere_nodum (
                   Piscina* piscina,
                ArborNodus* nodus,
    ArborFormatorOptiones*  optiones);

/* Emittere intervallum linearum */
chorda arbor_formator_emittere_intervallum (
                   Piscina* piscina,
                ArborNodus* radix,
                       i32  linea_initium,
                       i32  linea_finis,
    ArborFormatorOptiones*  optiones);

/* ===========================================================
 * API - Fidelis Mode (Roundtrip)
 * =========================================================== */

/* Emittere cum exacta trivia conservatione */
chorda arbor_formator_emittere_fidelis (
       Piscina* piscina,
    ArborNodus* radix);

/* ===========================================================
 * API - Utilitates
 * =========================================================== */

/* Convertere numerum ad Romanum (ZEPHYRUM-MMMMXCVI) */
chorda arbor_formator_numerus_ad_romanum (
    Piscina* piscina,
        s64  valor);

/* Convertere keyword Anglicum ad Latinum */
constans character* arbor_formator_keyword_ad_latinum (
    constans character* anglicum);

/* Convertere keyword Latinum ad Anglicum */
constans character* arbor_formator_keyword_ad_anglicum (
    constans character* latinum);

#endif /* ARBOR_FORMATOR_H */
