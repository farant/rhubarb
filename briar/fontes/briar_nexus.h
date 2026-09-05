/* briar_nexus.h - Arbores interiores partium documenti thistle,
 * identitate nodi ligatae (exemplar materiae: arbor exterior octetos
 * ut lexema UNUM crudum tenet, arbor interior seorsum).
 *
 * Tria genera in plano I: prosa (et regio 'md') -> arbor markdown
 * (md_arbor_parsare super octetos lexematis); elementum STML ->
 * StmlNodus (stml_legere super extensionem); regio cruda -> tag
 * apertus ut '<nomen attrs/>' parsatus (attributa sine grammatica
 * secunda). Positiones: linea_initium = linea .thistle primi octeti
 * (prosa/elementum: pars; regio: contentum); vitia STML in lineas
 * .thistle translata (linea_erroris). Regiones C per silvam = planum
 * II (fabrica).
 */

#ifndef BRIAR_NEXUS_H
#define BRIAR_NEXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "stml.h"

nomen enumeratio {
    BRIAR_NEXUS_MD = 0,      /* prosa aut <md!>: arbor_md */
    BRIAR_NEXUS_STML,        /* elementum: elementum (radix) */
    BRIAR_NEXUS_REGIO        /* regio cruda: elementum = tag apertus */
} BriarNexusGenus;

nomen structura {
    constans MateriaNodus* pars;
           BriarNexusGenus  genus;
             MateriaNodus* arbor_md;        /* MD; NIHIL alias */
                StmlNodus* elementum;       /* STML/REGIO; NIHIL si vitium */
                   chorda  titulus;         /* STML/REGIO: nomen; MD: vacua */
                   chorda  contentum;       /* REGIO: octeti contenti (vacua licet) */
                      i32  linea_initium;   /* linea .thistle (I-basata) */
                      i32  linea_erroris;   /* ZEPHYRUM = sanum */
                   chorda  causa;           /* nuntius parsatoris; vacua = sanum */
} BriarNexusRes;

/* Xar de BriarNexusRes, ordine partium; NIHIL = memoria */
Xar*
briar_nexus_texere (
                  Piscina* piscina,
    constans MateriaNodus* documentum,
      InternamentumChorda* intern);

/* attributum tagi (STML/REGIO); vacua si absens aut vitium */
chorda
briar_nexus_attributum (
    constans BriarNexusRes* res,
        constans character* titulus);

b32
briar_nexus_attributum_habet (
    constans BriarNexusRes* res,
        constans character* titulus);

b32
briar_nexus_titulus_est (
    constans BriarNexusRes* res,
        constans character* literae);

#endif /* BRIAR_NEXUS_H */
