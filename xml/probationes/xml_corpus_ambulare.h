/* xml_corpus_ambulare.h - Ambulatio corporum xml pro portis
 *
 * Adiumentum probationum (cursor plagulas probationum non-probatio_ ut
 * adiumenta compilat; exemplar toml_corpus_ambulare): duo fontes,
 * ordine -
 *   XML_CORPUS_XMLTS        xml/probationes/fixa/xmlts/selectio.tsv:
 *                           casus selecti (MMI, exclusi inclusi) ex
 *                           xml/build/xmlts/xmlconf/ (arcessitor
 *                           ./xml/xmlts_arcessere.sh); suite absente
 *                           nihil visitur et numeri.xmlts_absens VERUM
 *   XML_CORPUS_SILVESTRIA   silvestria.manifestum: plagulae VIVAE;
 *                           sigillo mutato aut absente OMITTUNTUR CUM
 *                           NUMERO (regula Frani, manifestum solum)
 * Visor plagulam quamque accipit cum linea selectionis (XMLTS; NIHIL
 * pro silvestribus); piscina 'opus' post quamque vacatur (visor
 * numquam ultra eam servet).
 *
 * XML_CASUS: casus inlinei communes portarum (lectoris, arboris, STML,
 * canonis) - Review Focus plani et casus auditus 2026-10-18.
 */

#ifndef XML_CORPUS_AMBULARE_H
#define XML_CORPUS_AMBULARE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

nomen enumeratio {
    XML_CORPUS_XMLTS = 0,
    XML_CORPUS_SILVESTRIA,
    XML_CORPUS_NUMERUS_FONTIUM
} XmlCorpusFons;

/* Linea selectionis (campi TSV, NUL-terminati in buffer ambulatoris) */
nomen structura {
    constans character* id;
    constans character* genus;       /* valid invalid not-wf error */
    constans character* via;         /* relativa ad xmlconf/ */
    constans character* exitus;      /* "-" si nullus */
    constans character* entitates;
    constans character* spatia;
    constans character* exclusio;    /* "-" si nulla */
} XmlCasusSelectus;

nomen vacuum (*XmlCorpusVisor)(vacuum* datum, XmlCorpusFons fons,
    constans character* via, constans XmlCasusSelectus* selectus,
    chorda textus, Piscina* opus);

nomen structura {
    i32 plagulae[XML_CORPUS_NUMERUS_FONTIUM];
    i32 omissae;          /* silvestres sigillo mutatae aut absentes */
    b32 xmlts_absens;     /* suite non arcessita */
} XmlCorpusNumeri;

/* radix = radix repositorii (RHUBARB_RADIX aut "."); fontes =
 * larva (I << XmlCorpusFons) fontium visitandorum */
vacuum
xml_corpus_ambulare (
                 Piscina* piscina,
                 Piscina* opus,
      constans character* radix,
                     i32  fontes,
          XmlCorpusVisor  visor,
                  vacuum* datum,
         XmlCorpusNumeri* numeri);

/* plagulam suitae (via relativa ad xmlconf/) legere; mensura 0 si
 * abest */
chorda
xml_corpus_xmlts_legere (
                Piscina* piscina,
     constans character* radix,
     constans character* via);

/* radix repositorii ex ambitu (RHUBARB_RADIX) aut "." */
constans character*
xml_corpus_radix (vacuum);

externus constans character* constans XML_CASUS[];

i32
xml_casus_numerus (vacuum);

#endif /* XML_CORPUS_AMBULARE_H */
