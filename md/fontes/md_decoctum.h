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

/* Titulus nexus (par. 4.7): CMXCIX characteres SUMMUM intra uncos -
 * longior titulus non est (ergo nec definitio nec referentia) */
#define MD_TITULUS_MAXIMUS ((s32)CMXCIX)

/* Clavis tituli nexus normalizata (par. 4.7): trim, cursus spatiorum
 * (spatium, tabula, linea nova) -> unum, minusculae ASCII (plicatio
 * Unicode = lacuna nominata). Copia in piscina. Titulus longior quam
 * MD_TITULUS_MAXIMUS = clavis VACUA (datum NIHIL), nihil allocatum:
 * uncus clausus contentum ad aperturam suam normalizabat - nidificatio
 * '[[[...]]]' L milium = tempus et memoria quadratica (~3.5 s, ~2.5 GB;
 * md totalitas sub onere 'KILL (mora excessa)', 2026-10-15) */
chorda
md_clavem_normalizare (
               Piscina* piscina,
    constans character* fons,
                   s32  ab,
                   s32  ad);

/* URL codificare (CommonMark: href in html - cmark houdini_escape_href):
 * octeti non tuti in '%XX' (spatium, '"', '<', '>', '\\', '`', '[', ']',
 * '{', '}', '|', '^', imperia, octeti >= 0x80); '%' servatur (numquam
 * bis codificatur), '&' servatur (effugium attributi est, non URL).
 * Chorda eadem redditur si nihil codificandum. */
chorda
md_url_codificare (
    Piscina* piscina,
     chorda  url);

#endif /* MD_DECOCTUM_H */
