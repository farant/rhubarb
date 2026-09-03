/* md_decoctum.h - Decoctio textus markdown: effugia et entia
 *
 * CommonMark par. 2.4 (effugia '\' + interpunctio ASCII) et par. 2.5
 * (entia '&nomen;', '&#N;', '&#xH;'). Lexemata DERIVATA (spec par. III,
 * fons I) valores decoctos ferunt; crudum in arbore manet.
 *
 * Entia nominata: tabula PARTIALIS (~CL communia) - HTML5 MMCCXXXI habet.
 * Ens ignotum litterae manent (lex CommonMark pro nominibus invalidis;
 * pro validis sed hic absentibus = LACUNA NOMINATA quam oraculum html
 * (B3) per sectionem 'Entity references' ostendet).
 */

#ifndef MD_DECOCTUM_H
#define MD_DECOCTUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* Textum decoquere: copia nova in piscina. *mutatus = an differat a
 * crudo (VERUM = lexema derivatum meretur; FALSUM = crudum sufficit).
 * Numquam NIHIL.datum nisi memoria deficit. */
chorda
md_decoquere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   b32* mutatus);

/* Ens ab 'ab' ('&' ibi): octeti UTF-8 in exitus (capacitas >= VIII),
 * *longitudo, *post (index post ';'). FALSUM = non ens validum. */
b32
md_ens_decoquere (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             character* exitus,
                   i32* longitudo,
                   s32* post);

#endif /* MD_DECOCTUM_H */
