/* xml_lector.h - Lector xml: lexemata ex octetis, modo SUO
 *
 * MODI (spec par. III, X5): lector modum ipse eligit - XML ita factum
 * est ut sine responso parsatoris legatur: in contento '<' et '&'
 * omnia decernunt, intra tags citationes. Aedificator modum numquam
 * ponit, lexema numquam relabellatur.
 *
 *   CONTENTUM       TEXTUS usque ad '<', '&', ']]>'; REFERENTIA
 *                   (&titulus; &#N; &#xH;); '<titulus' TAG_APERTURA,
 *                   '</titulus' TAG_CLAUSURA (-> TAG); '<!--'
 *                   COMMENTARIUM, '<![CDATA[' CDATA, '<?' INSTRUCTIO
 *                   (lexemata tota usque ad claudentem aut finem
 *                   fontis); '<!DOCTYPE' (-> DOCTYPE); '<?xml' + S ad
 *                   initium solum (post BOM) DECLARATIO_XML (->
 *                   DECLARATIO_XML); BOM ad octetum 0 SIGNUM_ORDINIS
 *   TAG             SPATIA, TITULUS, '=', VALOR, '>' et '/>' (->
 *                   CONTENTUM)
 *   DECLARATIO_XML  SPATIA, TITULUS, '=', VALOR, '?>' (-> CONTENTUM)
 *   DOCTYPE         SPATIA, TITULUS, VALOR, '[' (-> PARS_INTERNA),
 *                   '>' (-> CONTENTUM)
 *   PARS_INTERNA    SPATIA, '<!ELEMENT' '<!ATTLIST' '<!ENTITY'
 *                   '<!NOTATION' DECLARATIO (-> DECLARATIO), '%t;'
 *                   REFERENTIA_PARAMETRI, COMMENTARIUM, INSTRUCTIO,
 *                   ']' (-> DOCTYPE)
 *   DECLARATIO      SPATIA, TITULUS (Nmtoken), VALOR, '#t'
 *                   VERBUM_PRAEDEFINITUM, '%t;', DELIM ( ) | , ? * + %,
 *                   '>' (-> PARS_INTERNA)
 *
 * RECUPERATIO: '<' in TAG, DECLARATIO_XML aut DOCTYPE modum CONTENTUM
 * reddit et iterum legitur (tag non clausum: aedificator absentiam
 * '>' nuntiat); '<' aut ']' in DECLARATIO modum PARS_INTERNA reddit.
 * VALOR in TAG et DECLARATIO_XML ante '<' desinit si citatio non
 * clauditur ('<' in valore attributi vitium est utique, WFC: No < in
 * Attribute Values); in DOCTYPE et DECLARATIO usque ad citationem
 * claudentem aut finem fontis (valor entitatis '<' legitime fert).
 *
 * OMNIS OCTETUS IN LEXEMATE (totalitas, X6): concatenatio omnium
 * lexematum = fons (porta lectoris id asserit). Quod nulla regula
 * accipit IGNOTUM fit: sequentia UTF-8 una (aut octetus unus si
 * invalida), aut '<' / '</' / '<!' / '<![' / ']]>' / '&' sine forma.
 *
 * VALIDITAS NON HIC: characteres extra Char et UTF-8 invalida intra
 * TEXTUS, VALOR, commentaria, instructiones, CDATA in lexemate manent
 * - iudicium characterum post lectionem (spec par. III). Lineae: '\n'
 * lineam novam incipit; columnae in octetis.
 */

#ifndef XML_LECTOR_H
#define XML_LECTOR_H

#include "latina.h"
#include "piscina.h"
#include "materia_token.h"
#include "xml_lexicon.h"

nomen enumeratio {
    XML_MODUS_CONTENTUM = 0,
    XML_MODUS_TAG,
    XML_MODUS_DECLARATIO_XML,
    XML_MODUS_DOCTYPE,
    XML_MODUS_PARS_INTERNA,
    XML_MODUS_DECLARATIO,
    XML_MODUS_NUMERUS_MODORUM
} XmlModus;

nomen structura {
                Piscina* piscina;
     constans character* fons;
                    s32  mensura;
                    s32  cursor;
                    i32  linea;
                    s32  linea_initium;
               XmlModus  modus;
                    b32  fragmentum;  /* BOM, <?xml non agnoscuntur */
      MateriaTokenForma  forma;
} XmlLector;

/* fons NON copiatur: lexemata in eum monstrant. fons NIHIL cum
 * mensura 0 licet (plagula vacua). */
b32
xml_lector_incipere (
             XmlLector* lector,
               Piscina* piscina,
    constans character* fons,
                   s32  mensura);

/* Fragmentum (textus substitutionis entitatis): modo dato incipit
 * (CONTENTUM aut PARS_INTERNA), BOM et '<?xml' non agnoscuntur. */
b32
xml_lector_incipere_fragmentum (
              XmlLector* lector,
                Piscina* piscina,
     constans character* fons,
                    s32  mensura,
               XmlModus  modus);

/* Lexema proximum; FINIS (vacuum) in fine, iterum et iterum. NIHIL =
 * memoria deficit. */
MateriaToken*
xml_lector_proximum (
    XmlLector* lector);

/* Modus in quo lexema PROXIMUM legetur */
XmlModus
xml_lector_modus (
    constans XmlLector* lector);

#endif /* XML_LECTOR_H */
