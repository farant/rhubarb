#ifndef XML_H
#define XML_H

#include "latina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"

/* ==================================================
 * XML - Bibliotheca XML Generalis
 *
 * "Extensible Markup Language" parser et scriptor
 *
 * Proprietates:
 * - DOM-stylus arbor (non SAX/streaming)
 * - Omnes chordae internatae
 * - Omnes allocationes ex Piscina
 * - Supportat: elementa, textus, commenta
 * - Non supportat: namespaces, DTD, CDATA
 * ================================================== */

/* ==================================================
 * Genera Nodorum
 * ================================================== */

nomen enumeratio {
    XML_NODUS_ELEMENTUM = I,    /* <tag>...</tag> */
    XML_NODUS_TEXTUS    = II,   /* text content */
    XML_NODUS_COMMENTUM = III   /* <!-- comment --> */
} XmlNodusGenus;

/* ==================================================
 * Status Parsationis
 * ================================================== */

nomen enumeratio {
    XML_SUCCESSUS                = ZEPHYRUM,
    XML_ERROR_MEMORIA            = I,
    XML_ERROR_SYNTAXIS           = II,
    XML_ERROR_TAG_NON_CLAUSUM    = III,
    XML_ERROR_TAG_IMPROPRIE      = IV,
    XML_ERROR_ATTRIBUTUM         = V,
    XML_ERROR_EFFUGIUM           = VI,
    XML_ERROR_VACUUM_INPUT       = VII
} XmlStatus;

/* ==================================================
 * Structurae
 * ================================================== */

/* XmlAttributum - Par titulus-valor
 * "Attribute name-value pair"
 */
nomen structura {
    chorda* titulus;  /* Titulus attributi (internatum) */
    chorda* valor;    /* Valor attributi (internatum) */
} XmlAttributum;

/* XmlNodus - Nodus in arbore XML
 * "Node in XML tree"
 */
nomen structura XmlNodus {
    XmlNodusGenus        genus;       /* Genus nodi */
    chorda*              titulus;     /* Titulus tagi (pro elementis) */
    chorda*              valor;       /* Contentum textuale */
    Xar*                 attributa;   /* Xar de XmlAttributum */
    Xar*                 liberi;      /* Xar de XmlNodus* */
    structura XmlNodus*  parens;      /* Nodus parentis */
} XmlNodus;

/* XmlResultus - Resultus parsationis
 * "Parsing result"
 */
nomen structura {
    b32        successus;
    XmlNodus*  radix;
    XmlStatus  status;
    i32        linea_erroris;
    i32        columna_erroris;
    chorda     error;
} XmlResultus;

/* ==================================================
 * Parsatio - Legere XML
 * ================================================== */

/* Legere XML ex chorda
 * "Parse XML from string"
 */
XmlResultus
xml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Legere XML ex literis C
 * "Parse XML from C string"
 */
XmlResultus
xml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* ==================================================
 * Quaestio - Invenire in Arbore
 * ================================================== */

/* Invenire primum liberum cum titulo
 * "Find first child with tag name"
 */
XmlNodus*
xml_invenire_liberum(
    XmlNodus*           nodus,
    constans character* titulus);

/* Invenire omnes liberos cum titulo
 * "Find all children with tag name"
 * Redde: Xar de XmlNodus*
 */
Xar*
xml_invenire_omnes_liberos(
    XmlNodus*           nodus,
    constans character* titulus,
    Piscina*            piscina);

/* Capere attributum per titulum
 * "Get attribute value by name"
 * Redde: chorda* ad valor, vel NIHIL si non inventum
 */
chorda*
xml_attributum_capere(
    XmlNodus*           nodus,
    constans character* titulus);

/* Verificare si nodus habet attributum
 * "Check if node has attribute"
 */
b32
xml_attributum_habet(
    XmlNodus*           nodus,
    constans character* titulus);

/* Capere textum internum (concatenatum)
 * "Get inner text content"
 */
chorda
xml_textus_internus(
    XmlNodus* nodus,
    Piscina*  piscina);

/* Numerus liberorum
 * "Number of children"
 */
i32
xml_numerus_liberorum(
    XmlNodus* nodus);

/* Capere liberum ad indicem
 * "Get child at index"
 */
XmlNodus*
xml_liberum_ad_indicem(
    XmlNodus* nodus,
    i32       index);

/* ==================================================
 * Constructio - Creare Nodos
 * ================================================== */

/* Creare elementum
 * "Create element node"
 */
XmlNodus*
xml_elementum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);

/* Creare nodum textus
 * "Create text node"
 */
XmlNodus*
xml_textum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);

/* Creare nodum textus ex chorda
 * "Create text node from chorda"
 */
XmlNodus*
xml_textum_creare_ex_chorda(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda               textus);

/* Creare commentum
 * "Create comment node"
 */
XmlNodus*
xml_commentum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);

/* Addere attributum ad elementum
 * "Add attribute to element"
 */
b32
xml_attributum_addere(
    XmlNodus*            nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    constans character*  valor);

/* Addere attributum cum chorda valor
 * "Add attribute with chorda value"
 */
b32
xml_attributum_addere_chorda(
    XmlNodus*            nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    chorda               valor);

/* Addere liberum ad elementum
 * "Add child to element"
 */
b32
xml_liberum_addere(
    XmlNodus* parens,
    XmlNodus* liberum);

/* Addere textum ad elementum (commoditas)
 * "Add text content to element"
 */
b32
xml_textum_addere(
    XmlNodus*            parens,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);

/* ==================================================
 * Scriptio - Scribere XML
 * ================================================== */

/* Scribere nodum ad chordam
 * "Serialize node to string"
 *
 * pulchrum: si VERUM, addere indentationem et lineae novae
 */
chorda
xml_scribere(
    XmlNodus* nodus,
    Piscina*  piscina,
    b32       pulchrum);

/* Scribere nodum ad ChordaAedificator
 * "Serialize node to string builder"
 */
b32
xml_scribere_ad_aedificator(
    XmlNodus*          nodus,
    ChordaAedificator* aedificator,
    b32                pulchrum,
    i32                indentatio);

#endif /* XML_H */
