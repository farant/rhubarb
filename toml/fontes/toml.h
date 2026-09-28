/* toml.h - TOML 1.0: API publica (Q10)
 *
 * Legere, iudicare, quaerere. Sub hoc: toml_arbor (arbor octetis
 * fidelis), toml_coctum (tabulae et valores, regulae), toml_scalaris
 * (valores singuli), diagnostica per materia_pictor_scribere.
 *
 *   TomlDocumentum* doc = toml_legere(textus, "config.toml", piscina);
 *   si (!toml_successus(doc))
 *       imprimere("%.*s", ..., toml_diagnostica_scribere(...));
 *   si (toml_chorda(doc, "llama-server.versio", &versio)) ...
 *
 * Documentum cum vitiis TAMEN coquitur quantum potest: quaestiones
 * super eo licent (valores definitionum primarum), successus FALSUM.
 *
 * VIA CLAVIUM per lectorem toml (modus CLAVIS) legitur, ut clavis in
 * fonte: 'a.b', 'a . b', 'a."b.c"' (segmentum quotatum cum puncto),
 * '"kA"' (effugia). Via vacua, prava ('a..b', 'a.', '.a', cauda
 * aliena), per seriem ('arr.x') aut in non-tabulam ('x.y', x chorda)
 * -> NIHIL / FALSUM. Series per toml_seriei_* enumerantur.
 *
 * TEXTUS NON COPIATUR: lexemata in eum monstrant; textus documentum
 * vivere debet (ut omnes clientes materiae).
 *
 * TRANSITIO (usque ad Q12): include/toml.h vetus (lib/toml.c) nomina
 * eadem fert; numquam in binario uno. Q12 consumptores migrat et vetus
 * delet.
 */

#ifndef TOML_H
#define TOML_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "toml_valor.h"

nomen structura TomlDocumentum TomlDocumentum;

/* numquam NIHIL (nisi memoria deficit); 'via' nomen fontis in
 * diagnosticis (NIHIL licet) */
TomlDocumentum*
toml_legere (
                 chorda  textus,
     constans character* via,
                Piscina* piscina);

b32
toml_successus (
    constans TomlDocumentum* doc);

/* Xar de MateriaDiagnosticum, ordine octetorum (syntaxis, scalaria,
 * structura) */
Xar*
toml_diagnostica (
    constans TomlDocumentum* doc);

/* omnia diagnostica ut textus humanus (materia_pictor_scribere per
 * unumquodque, ordine octetorum); excerptum FALSUM = capita sola */
chorda
toml_diagnostica_scribere (
                    Piscina* piscina,
    constans TomlDocumentum* doc,
                        b32  excerptum);

/* tabula radicis */
constans TomlValor*
toml_radix (
    constans TomlDocumentum* doc);

constans TomlValor*
toml_quaerere (
    constans TomlDocumentum* doc,
         constans character* via_clavium);

/* VERUM = adest ET genus congruit; aliter FALSUM et exitus intactus */
b32
toml_chorda (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                     chorda* exitus);

b32
toml_integer (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                        s64* exitus);

b32
toml_fluitans (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                        f64* exitus);

b32
toml_boolean (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                        b32* exitus);

b32
toml_tempus (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                 TomlTempus* exitus);

/* Enumeratio (ordo fontis). Genus alienum aut index extra -> 0 /
 * chorda vacua / NIHIL. */
i32
toml_tabulae_numerus (
    constans TomlValor* tabula);

chorda
toml_tabulae_clavis (
    constans TomlValor* tabula,
                   i32  index);

constans TomlValor*
toml_tabulae_valor (
    constans TomlValor* tabula,
                   i32  index);

i32
toml_seriei_numerus (
    constans TomlValor* series);

constans TomlValor*
toml_seriei_elementum (
    constans TomlValor* series,
                   i32  index);

#endif /* TOML_H */
