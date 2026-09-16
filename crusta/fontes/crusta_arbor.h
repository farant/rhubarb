/* crusta_arbor.h - Aedificator arboris crustae (spec par. V)
 *
 * crusta_arbor_parsare: fons -> MateriaNodus* (programma) contra
 * CRUSTA_REGISTRUM. ITERATIVUS: acervus gradum (framarum) qui EST
 * acervus modorum lectoris (decretum 01M2NJ1JR7); recursio linguae
 * ($( ) intra verbum) = gradus pulsus, numquam recursio C. Totalitas
 * (C12): omnis octetus in arborem cadit; lexema quod grammatica ponere
 * non potest 'malum' fit; sanitas = mala nulla et clausurae nullae
 * absentes. Lexemata ficta numquam: locus absens = significatio.
 *
 * SENTENTIA PENDENS: nodus sententiae (imperium, pipa, catena) listae
 * suae appenditur cum gradus eius CLAUDITUR, non cum aperitur - ita
 * '&&' et '|' rem finitam in catenam/pipam involvunt sine motu nodi
 * (listae materiae solum appendunt). Ordo octetorum servatur quia
 * nihil aliud interea listae appenditur.
 *
 * LIGATOR (C7): trivia pendentia post lineam novam ULTIMAM dividuntur
 * - [0, k] 'post' lexematis prioris, cetera 'ante' sequentis; sine
 * priore omnia 'ante'; sine linea nova omnia 'ante'.
 *
 * Pars I (P3): programma, imperia simplicia, verba ut partes,
 * redirectiones (petitio heredoc), catena/pipa cum operatore solo,
 * separatores, mala. Intermedia nominata: '$((' pars litteralis usque
 * ad P4; verba reservata (praeter '!' et 'time'), '(( ))', '[[ ]]',
 * '(' subcrusta, terminatores optionum -> mala usque ad P4/P5.
 */

#ifndef CRUSTA_ARBOR_H
#define CRUSTA_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_nodus.h"
#include "crusta_registrum.h"

nomen structura {
    i32 mala;                  /* nodi malum */
    i32 clausurae_absentes;    /* constructa sine lexemate clausurae */
    i32 profunditas_maxima;    /* acervus gradum */
    i32 heredoca;              /* petitiones heredoc */
    /* corpora heredoc quorum linea nova in gradu sine lista cecidit
     * (post 'for' ante nomen, intra '(( ))' aut '[[ ]]', post nomen
     * functionis): listae proximae appensa, ORDO OCTETORUM RUPTUS -
     * limes nominatus; sana FALSUM */
    i32 heredoca_transposita;
    /* mala nulla, clausurae omnes, heredoca in sede */
    b32 sana;
} CrustaParsura;

/* relatio NIHIL licet. NIHIL = memoria deficit. */
MateriaNodus*
crusta_arbor_parsare (
                     Piscina* piscina,
          constans character* fons,
                         i32  mensura,
    constans CrustaDialectus* dialectus,
               CrustaParsura* relatio);

/* Idem, cum lexematis: omne lexema quod aedificator accepit (trivia,
 * FINIS, delimitatores heredoc, clausurae backtick) in 'lexemata'
 * (Xar de MateriaToken*) ordine appenditur; lexemata temptationis
 * '$((' recusatae sublata. Valores concatenati = fons (tegumentum
 * lectoris per corpus). lexemata NIHIL = sine memoria. */
MateriaNodus*
crusta_arbor_parsare_cum_lexematis (
                     Piscina* piscina,
          constans character* fons,
                         i32  mensura,
    constans CrustaDialectus* dialectus,
               CrustaParsura* relatio,
                         Xar* lexemata);

/* Valor staticus verbi: partes litterales, effugia, apices decodati
 * (profunditas backtick ex cauda lexematis exuta); FALSUM si pars
 * parametri/expansionis/substitutionis/arithmeticae/processus adest
 * (non staticum) aut memoria deficit. Chorda in piscina. */
b32
crusta_verbum_staticum (
                  Piscina* piscina,
    constans MateriaNodus* verbum,
                   chorda* valor);

/* An pars ulla citata aut effugita sit (delimitator heredoc citatus,
 * regula citata). */
b32
crusta_verbum_citatum (
    constans MateriaNodus* verbum);

/* Verbum primum imperii (titulus imperii); NIHIL si nullum. */
constans MateriaNodus*
crusta_imperium_titulus (
    constans MateriaNodus* imperium);

/* Verba post titulum, ordine (Xar de MateriaNodus*); NIHIL = memoria */
Xar*
crusta_imperium_argumenta (
                  Piscina* piscina,
    constans MateriaNodus* imperium);

#endif /* CRUSTA_ARBOR_H */
