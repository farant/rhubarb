/* briar_symbolum.h - Definitio symboli bibliothecae EX CORPORE
 * (spec par. 4.7: dimidium reservatum F5, per pontem spectatoris).
 *
 * FUNCTIO PURA: nulla fenestra, nullus pons, nullus discus - corpus
 * infixum intrat, textus definitionis exit. Ergo porta eam sine ulla
 * re vitrea probat, et id est totum consilium: quod fenestra facit
 * parvum est, quod RESPONDET magnum.
 *
 * CURSOR datur, non intus absconditur: arbores parsatae in eo per
 * plagulam manent, ergo ictus secundus in eadem plagula gratis est.
 * Spectator eum trans vocamina tenet; porta eum inspicit (quot
 * plagulae parsatae sint) et sic cursum ipsum probat.
 *
 * GEMINUM: caput 'piscina.h' -> 'lib/piscina.c' (lex silicis). Caput
 * sine gemello (latina.h: macra sola) NON est vitium - responsum
 * 'non inventum' est.
 */

#ifndef BRIAR_SYMBOLUM_H
#define BRIAR_SYMBOLUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silex.h"

nomen structura BriarCursorSymbolorum BriarCursorSymbolorum;

nomen structura {
       b32 inventum;
    chorda caput;       /* 'piscina.h' ut datum */
    chorda via;         /* 'lib/piscina.c'; vacua si gemellum abest */
    chorda definitio;   /* extensio verbatim; vacua si non inventa */
       i32 linea;       /* in plagula gemella, I-basata */
} BriarSymboliResponsum;

BriarCursorSymbolorum*
briar_cursorem_symbolorum_creare (
               Piscina* piscina,
    constans SilexFons* fons);

/* titulus + caput -> definitio. Responsum 'non inventum' si gemellum
 * abest, si symbolum in gemello non definitur, aut si caput falsum
 * datur - numquam responsum dimidiatum. */
BriarSymboliResponsum
briar_symbolum_quaerere (
    BriarCursorSymbolorum* cursor,
                   chorda  titulus,
                   chorda  caput);

/* quot plagulae parsatae sint - porta cursum per hoc probat */
i32
briar_cursoris_parsurae (
    constans BriarCursorSymbolorum* cursor);

#endif /* BRIAR_SYMBOLUM_H */
