/* silva_unitates.h - scansor unitatum supremarum C89 (lexemate
 * conscius) - MECHANISMUS COMMUNIS instrumentorum.
 *
 * Extractum ex amalgamatore 2026-08-17 (ibi natus, Phase 6):
 * consumptores = amalgamator (emissio, S41, CADENDA) et differre
 * (differentia semantica per unitates). PORTA extractionis:
 * silva.c byte-identicum regenerat, suite tota tenet.
 *
 * Unitas = vel linea directivae (# initio lineae logicae) vel
 * unitas suprema C89 (functio, datum, typus, definitio tag) -
 * limites profunditate parium ()/{}. Tituli heuristice lecti;
 * Latine ET anglice (interior/static, nomen/typedef, structura/
 * struct...). Lexemata: Xar de SilvaToken* ex silva_lexare.
 */

#ifndef SILVA_UNITATES_H
#define SILVA_UNITATES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"

/* finis lineae logicae (directivae): index post lexema ultimum */
i32
silva_lineam_finire (
    Xar* lexemata,
    i32  i,
    i32  n);

/* Unitas suprema: [i, finis). titulus_out = identificator ante '('
 * primum (functio) aut ante '='/'[' aut ultimus ante ';' (datum,
 * typus) aut post structura/unio/enumeratio (definitio tag) aut in
 * '(*IDENT)' (typedef indicis functionis). est_definitio_out:
 * corpus functionis {...} habet. */
i32
silva_unitatem_finire (
       Xar* lexemata,
       i32  i,
       i32  n,
    chorda* titulus_out,
       b32* est_functio_out,
       b32* est_statica_out,
       b32* est_definitio_out,
       b32* est_typedef_out,
       b32* est_tag_def_out);

/* enumeratio tota plagulae: directivae lineatim (titulus pro
 * define/undef/ifdef/ifndef = identificator sequens; pro include =
 * "via" aut <via> textu), ceterae per silva_unitatem_finire.
 * EOF non fit unitas. */
nomen structura {
    chorda titulus;         /* vacua si sine titulo */
       i32 lexema_primum;   /* index in lexemata */
       i32 lexema_finis;    /* exclusivum */
       b32 est_directiva;
       b32 est_functio;
       b32 est_statica;
       b32 est_definitio;
       b32 est_typedef;
       b32 est_tag_def;
} SilvaUnitas;

/* Xar de SilvaUnitas ordine fontis; NIHIL = memoria defecit */
Xar*
silva_unitates_scandere (
    Piscina* piscina,
        Xar* lexemata);

#endif /* SILVA_UNITATES_H */
