/* toml_oraculum.h - Comparator valoris cocti contra exspectatum
 * toml-test (JSON tagatum) (Q8)
 *
 * Forma toml-test: tabula = objectum JSON; series = tabulatum; scalare
 * = objectum NOTATUM {"type": T, "value": V} - EXACTE duae
 * claves, ambae chordae (tabula TOML cum clavibus 'type' et
 * 'value' valores OBJECTA habet, ergo numquam confunditur). T:
 * string, integer, float, bool, datetime, datetime-local, date-local,
 * time-local.
 *
 * Regulae: objecta per copiam clavium et recursive; tabulata per
 * longitudinem et ordinem; scalaria per GENUS et valorem - integer ut
 * s64 ex chorda; fluitans per valorem (nan == nan; -0.0 et 0.0 per
 * signum chordae exspectatae; inf signatum); tempus per lectorem
 * coctionis ipsius (toml_tempus_legere) et campos omnes (Z == +00:00);
 * chorda per octetos; bool per valorem.
 *
 * Idem comparator aureum tomllib legit (Q9: eadem forma tagata).
 */

#ifndef TOML_ORACULUM_H
#define TOML_ORACULUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "json.h"
#include "toml_valor.h"

nomen structura {
       b32 aequalis;
    chorda via_differentiae;   /* "a.b[2]" ubi primum differunt */
    chorda causa;              /* "genus: integer != float" ... */
} TomlComparatio;

/* Recursio per profunditatem valoris (Q11: totalitas). */
TomlComparatio
toml_oraculum_comparare (
                 Piscina* piscina,
      constans TomlValor* coctum,
               JsonValor* exspectatum);

#endif /* TOML_ORACULUM_H */
