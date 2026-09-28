/* toml_valor.h - Forma valoris cocti toml (typi soli, sine corpore)
 *
 * Arbor (toml_arbor) octetos servat; coctio (toml_scalaris valores
 * singulos, toml_coctum tabulas) inde valores typatos facit. Hic
 * forma sola, ut consumptores (coctio, oraculum, API toml.h) eam sine
 * machina includant - exemplar materia_diagnosticum.h.
 *
 * Integer s64 (TOML: LXIV biti signati; extra fines = diagnosticum,
 * numquam truncatio). Fluitans f64 (inf, nan comprehensa). Tempus
 * quattuor generum; fractio secundi ad IX digitos, reliqui abscisi.
 * Omnis valor nodum arboris suum fert (sedes diagnosticorum, T8).
 */

#ifndef TOML_VALOR_H
#define TOML_VALOR_H

#include "latina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "materia_nodus.h"

nomen enumeratio {
    TOML_VALOR_TABULA = 0,
    TOML_VALOR_SERIES,
    TOML_VALOR_CHORDA,
    TOML_VALOR_INTEGER,
    TOML_VALOR_FLUITANS,
    TOML_VALOR_BOOLEAN,
    TOML_VALOR_TEMPUS
} TomlGenusValoris;

/* RFC 3339 per TOML 1.0: cum zona (Z aut +HH:MM), locale (dies et
 * hora sine zona), dies solus, hora sola */
nomen enumeratio {
    TOML_TEMPUS_CUM_ZONA = 0,
    TOML_TEMPUS_LOCALE,
    TOML_DIES_LOCALIS,
    TOML_HORA_LOCALIS
} TomlGenusTemporis;

nomen structura {
    TomlGenusTemporis genus;
                  s32 annus;
                  s32 mensis;
                  s32 dies;
                  s32 hora;
                  s32 minutum;
                  s32 secundum;        /* 0-60 (secundum intercalare) */
                  s32 nanosecunda;     /* fractio ad IX digitos */
                  s32 zona_minuta;     /* CUM_ZONA solum; Z = 0 */
} TomlTempus;

/* Modus tabulae (regulae toml_coctum): IMPLICITA per clavem punctatam
 * aut caput profundius nata; EXPLICITA per caput [a]; PUNCTATA per
 * 'a.b = v' definita; INLINEA { } clausa post scriptionem */
nomen enumeratio {
    TOML_TABULA_IMPLICITA = 0,
    TOML_TABULA_EXPLICITA,
    TOML_TABULA_PUNCTATA,
    TOML_TABULA_INLINEA
} TomlModusTabulae;

nomen structura TomlValor TomlValor;

structura TomlValor {
          TomlGenusValoris  genus;
     constans MateriaNodus* nodus;       /* T8: sedes in arbore */
    unio {
                  chorda chorda_valor;  /* decodita */
                     s64 integer_valor;
                     f64 fluitans_valor;
                     b32 boolean_valor;
              TomlTempus tempus_valor;
        structura {
                       Xar* claves;      /* chorda, ordine fontis */
                       Xar* valores;     /* TomlValor* */
            TabulaDispersa* index;       /* clavis -> TomlValor* */
          TomlModusTabulae  modus;
        } tabula;
                     Xar* series;        /* TomlValor* */
    } datum;
                       b32 series_tabularum;   /* nata per [[ ]] */
};

#endif /* TOML_VALOR_H */
