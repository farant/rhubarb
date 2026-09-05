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
 * .thistle translata (linea_erroris).
 *
 * Genus quartum (plan 2): regio 'c' per silvam cum expansione -
 * praeludium '#include "latina.h"' (+ 'internuntius.h' +
 * 'hic_manens InternuntiusTractator briar_tractator_exemplar;' si
 * methodus=) textui praepositum; capita clausurae e fonte silicis
 * praebita (numquam discus); arbor + semantica in BriarSilva
 * (briar_silva.h, unitas propria: caput amalgamae silvae enumerationes
 * stml suas fert - stml.h et silva.h in eadem unitate non coeunt, ergo
 * hoc caput NEC stml.h NEC silva.h includit, tags solum praenuntiat).
 * Lineae silvae per briar_nexus_linea_silvae in .thistle:
 * tabula linearum contextus (v1.6, briar_contextus) si adest, alias
 * linea_initium + linea_silvae - praeludium - I.
 */

#ifndef BRIAR_NEXUS_H
#define BRIAR_NEXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_nodus.h"

structura StmlNodus;     /* stml.h - consumptor includat */
structura BriarSilva;    /* briar_silva.h */

nomen enumeratio {
    BRIAR_NEXUS_MD = 0,      /* prosa aut <md!>: arbor_md */
    BRIAR_NEXUS_STML,        /* elementum: elementum (radix) */
    BRIAR_NEXUS_REGIO        /* regio cruda: elementum = tag apertus */
} BriarNexusGenus;

nomen structura {
     constans MateriaNodus* pars;
           BriarNexusGenus  genus;
              MateriaNodus* arbor_md;        /* MD; NIHIL alias */
      structura StmlNodus* elementum;       /* STML/REGIO */
                   chorda titulus;         /* nomen; MD vacua */
                   chorda contentum;       /* REGIO: octeti */
                      i32 linea_initium;   /* .thistle, I-basata */
                      i32 linea_erroris;   /* ZEPHYRUM = sanum */
                   chorda causa;           /* nuntius; vacua = sanum */
     structura BriarSilva* silva;           /* C: arbor + semantica */
                   chorda textus_silvae;   /* C: cum praeludio */
                      i32 praeludium;      /* lineae praepositae */
                      i32 praeludium_octeti;
                   chorda contextus;       /* C radix: textus contextus
                                            * (briar_contextus); alias
                                            * contentum */
                      Xar* lineae;         /* i32 per lineam contextus:
                                            * linea .thistle; NIHIL =
                                            * formula linearis */
                      b32 est_fragmentum;  /* <c! id=...>: numquam
                                            * radix */
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

/* linea .thistle ex linea fontis silvae (praeludio dempto) */
i32
briar_nexus_linea_silvae (
    constans BriarNexusRes* res,
                       i32  linea_silvae);

#endif /* BRIAR_NEXUS_H */
