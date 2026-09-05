/* briar_amalgama.h - Plagula UNA sola compilanda (effugium, spec
 * par. 5): '<t>.c' iuxta plagulam thistle, sine include/ nec lib/ -
 * clang eam cum vexillis solis aedificat. Contentum ordine: capita
 * clausurae
 * ordine DEPENDENTIAE (profunditate prima, post-ordine: caput post
 * omnia quae includit; postulata_posix.h PRIMUM si adest - codex 85),
 * caput regionum genitum, fontes bibliothecarum, <t>_regiones.c,
 * <t>.c (principale). Probatio (si adest) plagula ALTERA
 * 'probatio_<t>.c' eadem lege, unitate probationis pro principali.
 *
 * Inclusiones locales ('#include "x.h"') lineis VACUIS substituuntur
 * (numeri linearum servati); inclusiones systematis manent in loco
 * suo (hoisting caput condicionale platformae alienae traheret -
 * lib/filum.c <io.h>). '#line 1 "via"' plagulam quamque aperit:
 * error clang plagulam originalem nominat.
 *
 * STATICA scopi plagulae inter fontes collidunt (_est_spatium in
 * decem fontibus lib): OMNE staticum cuiusque fontis renominatur
 * '#define nomen nomen_<stirps>' ante textum, '#undef' post (macra
 * plagulae quoque #undef) - ordines 'lib/' tabulae
 * corpus.symbola.tsv (tools/corpus_infixum.sh; nexus silvae), nulla
 * parsura hic. Omnia, non collidentia sola: regiones usoris quoque
 * collidere possunt.
 *
 * Recusationes v1 (causa nominat plagulam): forma vitrea (capsula,
 * .m, frameworks - par. 9), venditorium in clausura (vexilla severa
 * sqlite non compilant), Objective-C in clausura.
 */

#ifndef BRIAR_AMALGAMA_H
#define BRIAR_AMALGAMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silex.h"
#include "briar_fabrica.h"

nomen structura {
       b32  successus;
    chorda  causa;
       Xar* plagulae;   /* BriarPlagula: '<t>.c' [, 'probatio_<t>.c'] */
} BriarAmalgamaFructus;

/* fructus fabricae sanus (genitae + clausura) -> plagulae amalgamae;
 * statica e 'corpus.symbola.tsv' fontis; via_thistle in titulo */
BriarAmalgamaFructus
briar_amalgamare (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus,
              constans SilexFons* fons,
              constans character* via_thistle);

/* plagulas in directorium scribere (iuxta plagulam thistle); plagula
 * exsistens quae NON a briar genita est non superscribitur (causa) */
b32
briar_amalgama_scribere (
                          Piscina* piscina,
    constans BriarAmalgamaFructus* fructus,
               constans character* directorium,
                           chorda* causa);

/* linea inclusionis localis? ('#include "x.h"', spatia permissa,
 * ut silex eam legit); nomen capitis redditum */
b32
briar_amalgama_inclusio_localis (
    chorda  linea,
    chorda* nomen_capitis);

#endif /* BRIAR_AMALGAMA_H */
