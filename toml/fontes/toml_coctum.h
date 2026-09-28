/* toml_coctum.h - Coctio documenti toml: tabulae, claves, regulae
 * structurae, diagnostica omnia ordine (Q7b)
 *
 * Arbor (toml_arbor) -> TomlValor radix (tabula) + Xar diagnosticorum:
 * syntaxis (declaratio: diagnosticum=/absentia=, per
 * materia_diagnostica_derivare), scalaria (toml_scalaria_iudicare),
 * structura (hic) - omnia per initium ordinata (ordo stabilis).
 * Documentum cum vitiis TAMEN coquitur quantum potest (spec par. V).
 *
 * EXEMPLAR tomllib (oraculum Q9, cum toml-test totaliter concordans),
 * in obiecta translatum (tomllib vexilla per viam clavium tenet; hic
 * modus in tabula ipsa, quod idem est quia elementum novum seriei
 * tabularum tabula RECENS est):
 *   - caput [x]: x IMPLICITA -> EXPLICITA; EXPLICITA aut series
 *     tabularum -> iterata; PUNCTATA -> iam per claves punctatas;
 *     INLINEA aut series statica (etiam in via) -> clausa; valor alius
 *     (etiam in via) -> genus alienum. Via per seriem tabularum in
 *     elementum ULTIMUM descendit.
 *   - caput [[x]]: absens -> series nova; series tabularum -> elementum
 *     appensum; statica -> clausa; tabula aut valor -> genus alienum.
 *   - par a.b.c = v: continentes a, a.b per claves punctatas: absens ->
 *     PUNCTATA in sectione currenti; IMPLICITA -> PUNCTATA; EXPLICITA,
 *     series tabularum, aut PUNCTATA sectionis PRIORIS -> extra
 *     sectionem; INLINEA / statica -> clausa; valor -> genus alienum.
 *     Clavis ultima iam praesens -> iterata (sedes relata: definitio
 *     prima).
 *   - tabula inlinea: intus eaedem regulae (sectio una); post
 *     scriptionem clausa. Linea nova aut comma caudale intus: TOML 1.1,
 *     non 1.0 -> nominatum.
 * Sectio = numerus capitum ante (radix 0); caput novum sectionem
 * novam incipit, ergo PUNCTATA prior 'definita' fit (tomllib
 * finalize_pending).
 */

#ifndef TOML_COCTUM_H
#define TOML_COCTUM_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_nodus.h"
#include "materia_diagnosticum.h"
#include "toml_arbor.h"
#include "toml_valor.h"

/* Codices structurae (TOML_CODEX_* scalaria in toml_scalaris.h) */
#define TOML_CODEX_CLAVIS_ITERATA     "clavis/iterata"
#define TOML_CODEX_CLAVIS_VACUA       "clavis/segmentum-vacuum"
#define TOML_CODEX_GENUS_ALIENUM      "clavis/genus-alienum"
#define TOML_CODEX_TABULA_ITERATA     "tabula/iterata"
#define TOML_CODEX_TABULA_PUNCTATA    "tabula/punctata"
#define TOML_CODEX_EXTRA_SECTIONEM    "tabula/extra-sectionem"
#define TOML_CODEX_CLAUSA             "tabula/clausa"
#define TOML_CODEX_VERSIO_NOVIOR      "tabula-compacta/toml-1.1"

nomen structura {
    TomlValor* radix;         /* tabula; numquam NIHIL nisi memoria */
          Xar* diagnostica;   /* MateriaDiagnosticum, per initium */
          b32  sanum;         /* nullum diagnosticum et parsura sana */
} TomlCoctum;

/* documentum = radix toml_arbor_parsare; parsura NIHIL licet. */
TomlCoctum
toml_coquere (
                  Piscina* piscina,
    constans MateriaNodus* documentum,
     constans TomlParsura* parsura);

#endif /* TOML_COCTUM_H */
