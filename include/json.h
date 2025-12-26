/* json.h - Bibliotheca JSON (Parser et Aedificator)
 *
 * Parser et aedificator JSON completus cum:
 * - Objecta imbricata
 * - Tabulata mixta
 * - Numeri integri (s64) et fluitantes (f64)
 * - Chordae cum unicode escapes (\uXXXX)
 *
 * USUS PARSING:
 *   JsonResultus res = json_legere_literis("{\"clavis\": 42}", piscina);
 *   si (res.successus) {
 *       JsonValor* val = json_objectum_capere(res.radix, "clavis");
 *       s64 num = json_ad_integer(val);
 *   }
 *
 * USUS BUILDING:
 *   JsonValor* obj = json_objectum_creare(piscina);
 *   json_objectum_ponere(obj, "clavis", json_integer_creare(piscina, 42));
 *   chorda output = json_scribere(obj, piscina);
 */

#ifndef JSON_H
#define JSON_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "chorda_aedificator.h"


/* ========================================================================
 * GENERA VALORUM
 * ======================================================================== */

nomen enumeratio {
    JSON_NULLUM     = 0,
    JSON_BOOLEAN    = I,
    JSON_INTEGER    = II,
    JSON_FLUITANS   = III,
    JSON_CHORDA     = IV,
    JSON_TABULATUM  = V,      /* Array */
    JSON_OBJECTUM   = VI      /* Object */
} JsonGenus;


/* ========================================================================
 * TYPI PRINCIPALES
 * ======================================================================== */

/* JsonValor - Valor JSON (tagged union)
 *
 * Quodque valor JSON habet genus et datum.
 * Pro objectis et tabulatis, datum continet Xar*.
 */
nomen structura JsonValor {
    JsonGenus genus;
    unio {
        b32     boolean_valor;
        s64     integer_valor;
        f64     fluitans_valor;
        chorda  chorda_valor;      /* Chorda (non-interned) */
        Xar*    tabulatum;         /* Array de JsonValor* */
        Xar*    objectum;          /* Array de JsonPar */
    } datum;
    Piscina* piscina;              /* Pro mutationes */
} JsonValor;

/* JsonPar - Par clavis-valor pro objectis
 * clavis est pointer ad chorda internata (via internamentum_globale)
 */
nomen structura {
    chorda*    clavis;    /* Pointer ad chorda internata */
    JsonValor* valor;
} JsonPar;

/* JsonResultus - Resultus parsing */
nomen structura {
    b32        successus;
    JsonValor* radix;
    chorda     error;
    i32        linea;
    i32        columna;
} JsonResultus;

/* JsonObjectumIterator - Iterator pro objectis */
nomen structura {
    JsonValor* objectum;
    i32        index;
} JsonObjectumIterator;


/* ========================================================================
 * FUNCTIONES - PARSING
 * ======================================================================== */

/* Legere JSON ex chorda
 *
 * input:   Textus JSON
 * piscina: Arena pro allocationibus
 *
 * Redde: Resultus cum radix valor vel error
 */
JsonResultus
json_legere(
    chorda   input,
    Piscina* piscina);

/* Legere JSON ex C string */
JsonResultus
json_legere_literis(
    constans character* input,
    Piscina*            piscina);


/* ========================================================================
 * FUNCTIONES - VERIFICATIO GENERIS
 * ======================================================================== */

JsonGenus json_genus(JsonValor* valor);

b32 json_est_nullum(JsonValor* valor);
b32 json_est_boolean(JsonValor* valor);
b32 json_est_integer(JsonValor* valor);
b32 json_est_fluitans(JsonValor* valor);
b32 json_est_chorda(JsonValor* valor);
b32 json_est_tabulatum(JsonValor* valor);
b32 json_est_objectum(JsonValor* valor);


/* ========================================================================
 * FUNCTIONES - EXTRACTIO VALORUM
 * ======================================================================== */

/* Extrahere valorem cum default si genus non congruit */

b32    json_ad_boolean(JsonValor* valor);    /* FALSUM si non boolean */
s64    json_ad_integer(JsonValor* valor);    /* 0 si non integer */
f64    json_ad_fluitans(JsonValor* valor);   /* 0.0 si non fluitans */
chorda json_ad_chorda(JsonValor* valor);     /* vacua si non chorda */


/* ========================================================================
 * FUNCTIONES - ACCESSUS TABULATI (ARRAY)
 * ======================================================================== */

/* Obtinere numerum elementorum */
i32 json_tabulatum_numerus(JsonValor* valor);

/* Obtinere elementum ad indicem (0-indexed)
 * Redde NIHIL si index invalidus vel non tabulatum
 */
JsonValor* json_tabulatum_obtinere(JsonValor* valor, i32 index);


/* ========================================================================
 * FUNCTIONES - ACCESSUS OBJECTI
 * ======================================================================== */

/* Obtinere numerum clavium */
i32 json_objectum_numerus(JsonValor* valor);

/* Obtinere valorem pro clave (linear search)
 * Redde NIHIL si clavis non inventa vel non objectum
 */
JsonValor* json_objectum_capere(JsonValor* valor, constans character* clavis);

/* Obtinere valorem pro clave (chorda) */
JsonValor* json_objectum_capere_chorda(JsonValor* valor, chorda clavis);

/* Verificare si clavis existit */
b32 json_objectum_habet(JsonValor* valor, constans character* clavis);

/* Obtinere par ad indicem (pro iteratione) */
JsonPar* json_objectum_par_obtinere(JsonValor* valor, i32 index);

/* Iterator pro objectis */
JsonObjectumIterator json_objectum_iterator(JsonValor* valor);
b32 json_objectum_iterator_proxima(
    JsonObjectumIterator* iter,
    chorda*               clavis_ex,
    JsonValor**           valor_ex);


/* ========================================================================
 * FUNCTIONES - CREARE VALORES
 * ======================================================================== */

JsonValor* json_nullum_creare(Piscina* piscina);
JsonValor* json_boolean_creare(Piscina* piscina, b32 valor);
JsonValor* json_integer_creare(Piscina* piscina, s64 valor);
JsonValor* json_fluitans_creare(Piscina* piscina, f64 valor);
JsonValor* json_chorda_creare(Piscina* piscina, chorda valor);
JsonValor* json_chorda_creare_literis(Piscina* piscina, constans character* valor);

/* Creare containers vacuos */
JsonValor* json_tabulatum_creare(Piscina* piscina);
JsonValor* json_objectum_creare(Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - MUTARE CONTAINERS
 * ======================================================================== */

/* Addere elementum ad tabulatum */
vacuum json_tabulatum_addere(JsonValor* tabulatum, JsonValor* valor);

/* Ponere par in objectum (overwrite si clavis existit) */
vacuum json_objectum_ponere(
    JsonValor*          objectum,
    constans character* clavis,
    JsonValor*          valor);

vacuum json_objectum_ponere_chorda(
    JsonValor* objectum,
    chorda     clavis,
    JsonValor* valor);


/* ========================================================================
 * FUNCTIONES - SERIALIZATION
 * ======================================================================== */

/* Scribere JSON ad chorda (compactum) */
chorda json_scribere(JsonValor* valor, Piscina* piscina);

/* Scribere JSON ad chorda (pulchrum, cum indentatio) */
chorda json_scribere_pulchrum(JsonValor* valor, Piscina* piscina);

/* Scribere ad aedificator existentem */
vacuum json_scribere_ad_aedificator(
    JsonValor*         valor,
    ChordaAedificator* aed);

vacuum json_scribere_ad_aedificator_pulchrum(
    JsonValor*         valor,
    ChordaAedificator* aed,
    i32                indentatio);


#endif /* JSON_H */
