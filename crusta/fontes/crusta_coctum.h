/* crusta_coctum.h - Visio COCTA arboris crustae: forma normalis
 * 'declare -f' bash 5.2.15 (spec par. VII; plan P11a)
 *
 * Munus quod html_coctum contra html5lib gerit: arbor crustae in textum
 * quem bash ipse imprimit cum functionem deparsat. Oraculum P11b
 * textum nostrum cum 'declare -f' bash comparat; instrumentum
 * ./crusta/coctum.sh formam normalem plagulae imprimit.
 *
 * MACHINA, NON IMPRESSOR PER NODUM (mensuratum ante aedificationem,
 * plan P11a): status fluminis - indentatio, saltus indentationis,
 * profunditas conexionum, modus functionis aut substitutionis, corpora
 * heredoc dilata, vexillum post heredoc - quia bash conexiones (';'
 * '&' '&&' '||' '|') ut arborem nidificatam imprimit et corpora heredoc
 * usque ad conexionem proximam differt. Listae et '&&'/'||'
 * SINISTRORSUM nidificant, pipae DEXTRORSUM; ordo effusionis inde
 * pendet.
 *
 * DUO MODI. Functionis (programma totum et corpora functionum): ';' et
 * linea nova -> ';' + linea nova; grex multilinearis. Substitutionis
 * ('$( )', '<( )', '>( )' - bash 5.2 eas tempore parsurae REPARSAT et
 * textum reimpressum servat; indentatio a zephyro): ';' -> '; ', linea
 * nova manet, grex '{ a; b; }'. Backtick verbatim.
 *
 * Programma = lista in modo functionis gradu zephyro; functio in radice
 * programmatis 'titulus () ' (forma 'declare -f'), alibi 'function
 * titulus () '. Commentaria et lineae vacuae cadunt; mala et nodi
 * heredoc in listis omittuntur (corpora per referentiam redirectionis
 * scribuntur).
 *
 * VISIO est, non documentum: ambulatio RECURSIVA (fixurae oraculi et
 * plagulae domus, non nidi C milium graduum - limes nominatus, ut
 * html_coctum).
 */

#ifndef CRUSTA_COCTUM_H
#define CRUSTA_COCTUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"

/* Programma (aut nodus sententiae quilibet) in forma normali. Chorda in
 * '\n' desinit si non vacua; radix NIHIL aut programma vacuum = chorda
 * vacua (datum non NIHIL). datum NIHIL = memoria deficit. */
chorda
crusta_coctum_scribere (
                  Piscina* piscina,
    constans MateriaNodus* radix);

#endif /* CRUSTA_COCTUM_H */
