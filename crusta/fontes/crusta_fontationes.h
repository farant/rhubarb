/* crusta_fontationes.h - quae scripta scriptum fontat aut exsequitur
 * (fabrica spec 3 par. III.2, T2)
 *
 * Liber lectionum (FABRICA_LECTIONES) lectiones bash non videt: 'source'
 * et scripta exsecuta extra filum leguntur. Hic ea STATICE derivantur
 * ex arbore crustae - clavis iudicii portae (actio 'iudicium') ea
 * continet, ergo indicem manu scriptum qui putrescat non eget.
 *
 * PROIECTIO summarii effectuum (crusta_effectus.h, effectus-plan T4):
 * situs fontatio et exsecutio (per syntaxim) in genera infra;
 * aestimator ipse in crusta_effectus.c vivit. Regulae eius:
 *
 * AESTIMATOR SYMBOLICUS verborum, non executio. Resolvit:
 *   - partes litterales, apices, effugia simplicia;
 *   - $X / ${X}: definitiones (assignationes) in AMBITU - scriptum
 *     radix et omnia quae fontat (bash eis variabiles communicat);
 *     scriptum EXSECUTUM ambitum novum incipit. Definitiones plures
 *     discordes, aut valor ignotus ('local a="$1"', 'for a in') =
 *     irresolutum. 'local' in functione F: definitiones F solae.
 *   - $0 (radix ambitus), ${BASH_SOURCE[0]} / $BASH_SOURCE (plagula
 *     verbi), $(cd "W" && pwd), $(dirname "W"), $(basename "W"),
 *     $(readlink -f "W") (nexus symbolici non sequuntur).
 * Cetera ($(pwd), ${X:-y}, $1, ~) = irresolutum; praefixum notum
 * servatur (partialis).
 *
 * LOCA: titulus imperii 'source'/'.' -> argumentum primum FONTATUM;
 * 'bash'/'sh' -> argumentum primum non '-' EXSECUTUM; 'exec' 'nohup'
 * 'command' 'env' (VAR=v et optiones praetereuntur) -> verbum
 * sequens ut titulus. Titulus cum '/' aut non staticus = via
 * exsecuta. Titulus staticus sine '/' = PATH, functio, aedificium:
 * praeteritur. Via relativa: cwd = ultimum 'cd W' in catena eadem
 * ('(cd X && bin/y)'), aliter RADIX (portae a radice currunt).
 * Superaestimatio licet: fons intra 'if' numeratur.
 *
 * GENERA exitus (via arbori relativa):
 *   FASCICULUS   scriptum arboris fontatum aut exsecutum (et radix)
 *   INSTRUMENTUM binarium arboris exsecutum (non '#!'), aut sub bin/
 *                nondum structum
 *   PRODUCTUM    exsecutum sub build/ (cursu ipso structum: ingressus
 *                eius per librum lectionum videntur) - via 'dir/' cum stella
 *   EXTERNUM     scriptum FONTATUM extra arborem (via absoluta)
 *   IRRESOLUTUM  locus fontationis/exsecutionis sine via certa, aut
 *                plagula illegibilis / parsura non sana
 * Exsecuta extra arborem (python3, clang absoluti) praetereuntur:
 * identitates eorum alibi (spec 3 par. III.3). */

#ifndef CRUSTA_FONTATIONES_H
#define CRUSTA_FONTATIONES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen enumeratio {
    CRUSTA_FONTATIO_FASCICULUS = 0,
    CRUSTA_FONTATIO_INSTRUMENTUM,
    CRUSTA_FONTATIO_PRODUCTUM,
    CRUSTA_FONTATIO_EXTERNUM,
    CRUSTA_FONTATIO_IRRESOLUTUM
} CrustaFontatioGenus;

nomen structura {
    CrustaFontatioGenus  genus;
              character* via;      /* arbori relativa; EXTERNUM
                                    * absoluta; IRRESOLUTUM textus
                                    * verbi aut causa */
             character* plagula;  /* ubi primum inventum (relativa) */
                   i32  linea;    /* I-basata; 0 = plagula tota */
                   b32  fontatum; /* 'source'/'.' (VERUM) an
                                    * exsecutum */
} CrustaFontatio;

/* Fontationes scripti derivare. radix: via ABSOLUTA arboris (sine '/'
 * finali); scriptum: via arbori relativa aut absoluta. Reddit Xar de
 * CrustaFontatio ordinatum per (genus, via), unicum per (genus, via) -
 * scriptum radix ipsum primus FASCICULUS. NIHIL = memoria deficit aut
 * scriptum radix illegibile (causa_out, si non NIHIL). */
Xar*
crusta_fontationes_derivare (
               Piscina*  piscina,
    constans character*  radix,
    constans character*  scriptum,
    constans character** causa_out);

/* Nomen generis ('fasciculus', 'instrumentum', ...) */
constans character*
crusta_fontatio_genus_titulus (
    CrustaFontatioGenus genus);

#endif /* CRUSTA_FONTATIONES_H */
