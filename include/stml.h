#ifndef STML_H
#define STML_H

#include "latina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"

/* ==================================================
 * STML - Structured Text Markup Language
 *
 * XML superset with modern features:
 * - Boolean attributes: <button disabled>
 * - Raw content tags: <code!>literal <>&</code>
 * - Capture operators: <div (> <) wrap> <= sandwich =>
 *
 * Proprietates:
 * - DOM-stylus arbor
 * - Omnes chordae internatae
 * - Omnes allocationes ex Piscina
 * - No entity references (use raw tags instead)
 * ================================================== */

/* ==================================================
 * Genera Nodorum
 * ================================================== */

nomen enumeratio {
    STML_NODUS_DOCUMENTUM   = I,
    STML_NODUS_ELEMENTUM    = II,
    STML_NODUS_TEXTUS       = III,
    STML_NODUS_COMMENTUM    = IV,
    STML_NODUS_PROCESSIO    = V,    /* <?xml ...?> */
    STML_NODUS_DOCTYPE      = VI
} StmlNodusGenus;

/* ==================================================
 * Directio Captionis (Capture Operators)
 * ================================================== */

nomen enumeratio {
    STML_CAPTIO_NIHIL       = ZEPHYRUM,
    STML_CAPTIO_ANTE        = I,    /* forward <tag (> */
    STML_CAPTIO_RETRO       = II,   /* backward <) tag> */
    STML_CAPTIO_FARCIMEN    = III   /* sandwich <= tag => */
} StmlCaptioDirectio;

/* ==================================================
 * Status Parsationis
 * ================================================== */

nomen enumeratio {
    STML_SUCCESSUS                   = ZEPHYRUM,
    STML_ERROR_MEMORIA               = I,
    STML_ERROR_SYNTAXIS              = II,
    STML_ERROR_TAG_NON_CLAUSUM       = III,
    STML_ERROR_TAG_IMPROPRIE         = IV,
    STML_ERROR_ATTRIBUTUM            = V,
    STML_ERROR_VACUUM_INPUT          = VI,
    STML_ERROR_CAPTIO                = VII   /* Capture operator error */
} StmlStatus;

/* ==================================================
 * Genera Tokenum (Internal)
 * ================================================== */

nomen enumeratio {
    STML_TOKEN_APERIRE       = I,     /* <tag */
    STML_TOKEN_CLAUDERE      = II,    /* </tag> */
    STML_TOKEN_AUTO_CLAUDERE = III,   /* <tag/> */
    STML_TOKEN_CRUDUS        = IV,    /* <tag! (raw content) */
    STML_TOKEN_TEXTUS        = V,
    STML_TOKEN_COMMENTUM     = VI,
    STML_TOKEN_PROCESSIO     = VII,   /* <?...?> */
    STML_TOKEN_DOCTYPE       = VIII,
    STML_TOKEN_CAPTIO_ANTE   = IX,    /* <tag (> */
    STML_TOKEN_CAPTIO_RETRO  = X,     /* <) tag> */
    STML_TOKEN_FARCIMEN      = XI,    /* <= tag => */
    STML_TOKEN_FINIS         = XII    /* EOF */
} StmlTokenGenus;

/* ==================================================
 * Structurae
 * ================================================== */

/* StmlAttributum - Par titulus-valor
 * "Attribute name-value pair"
 * For boolean attributes, valor is interned "true"
 */
nomen structura {
    chorda* titulus;    /* Attribute name (interned) */
    chorda* valor;      /* Attribute value (interned) */
} StmlAttributum;

/* StmlNodus - Nodus in arbore STML
 * "Node in STML tree"
 */
nomen structura StmlNodus {
    StmlNodusGenus           genus;
    chorda*                  titulus;         /* Tag name for elements */
    chorda*                  valor;           /* Text/comment/PI content */
    Xar*                     attributa;       /* Xar de StmlAttributum */
    Xar*                     liberi;          /* Xar de StmlNodus* */
    structura StmlNodus*     parens;

    /* STML-specific fields */
    b32                      crudus;          /* Raw content tag (! suffix) */
    StmlCaptioDirectio       captio_directio;
    i32                      captio_numerus;  /* Number of capture parens */
} StmlNodus;

/* StmlResultus - Resultus parsationis
 * "Parsing result"
 */
nomen structura {
    b32         successus;
    StmlNodus*  radix;            /* Root document node */
    StmlNodus*  elementum_radix;  /* First element child */
    StmlStatus  status;
    i32         linea_erroris;
    i32         columna_erroris;
    chorda      error;
} StmlResultus;

/* ==================================================
 * Parsatio - Legere STML
 * ================================================== */

/* Legere STML ex chorda
 * "Parse STML from string"
 */
StmlResultus
stml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Legere STML ex literis C
 * "Parse STML from C string"
 */
StmlResultus
stml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* ==================================================
 * Quaestio - Invenire in Arbore
 * ================================================== */

/* Invenire primum liberum cum titulo
 * "Find first child with tag name"
 */
StmlNodus*
stml_invenire_liberum(
    StmlNodus*           nodus,
    constans character*  titulus);

/* Invenire omnes liberos cum titulo
 * "Find all children with tag name"
 * Redde: Xar de StmlNodus*
 */
Xar*
stml_invenire_omnes_liberos(
    StmlNodus*           nodus,
    constans character*  titulus,
    Piscina*             piscina);

/* Capere attributum per titulum
 * "Get attribute value by name"
 * Redde: chorda* ad valor, vel NIHIL si non inventum
 */
chorda*
stml_attributum_capere(
    StmlNodus*           nodus,
    constans character*  titulus);

/* Verificare si nodus habet attributum
 * "Check if node has attribute"
 * Useful for boolean attributes
 */
b32
stml_attributum_habet(
    StmlNodus*           nodus,
    constans character*  titulus);

/* Capere textum internum (concatenatum)
 * "Get inner text content"
 */
chorda
stml_textus_internus(
    StmlNodus* nodus,
    Piscina*   piscina);

/* Numerus liberorum
 * "Number of children"
 */
i32
stml_numerus_liberorum(
    StmlNodus* nodus);

/* Capere liberum ad indicem
 * "Get child at index"
 */
StmlNodus*
stml_liberum_ad_indicem(
    StmlNodus* nodus,
    i32        index);

/* ==================================================
 * Constructio - Creare Nodos
 * ================================================== */

/* Creare elementum
 * "Create element node"
 */
StmlNodus*
stml_elementum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);

/* Creare elementum crudum (raw content)
 * "Create raw content element"
 */
StmlNodus*
stml_elementum_crudum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);

/* Creare nodum textus
 * "Create text node"
 */
StmlNodus*
stml_textum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);

/* Creare nodum textus ex chorda
 * "Create text node from chorda"
 */
StmlNodus*
stml_textum_creare_ex_chorda(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda               textus);

/* Creare commentum
 * "Create comment node"
 */
StmlNodus*
stml_commentum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);

/* Addere attributum ad elementum
 * "Add attribute to element"
 */
b32
stml_attributum_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    constans character*  valor);

/* Addere attributum boolean (valor = "true")
 * "Add boolean attribute"
 */
b32
stml_attributum_boolean_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);

/* Addere attributum cum chorda valor
 * "Add attribute with chorda value"
 */
b32
stml_attributum_addere_chorda(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    chorda               valor);

/* Addere liberum ad elementum
 * "Add child to element"
 */
b32
stml_liberum_addere(
    StmlNodus* parens,
    StmlNodus* liberum);

/* Addere textum ad elementum (commoditas)
 * "Add text content to element"
 */
b32
stml_textum_addere(
    StmlNodus*           parens,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);

/* ==================================================
 * Scriptio - Scribere STML
 * ================================================== */

/* Scribere nodum ad chordam
 * "Serialize node to string"
 *
 * pulchrum: si VERUM, addere indentationem et lineae novae
 */
chorda
stml_scribere(
    StmlNodus* nodus,
    Piscina*   piscina,
    b32        pulchrum);

/* Scribere nodum ad ChordaAedificator
 * "Serialize node to string builder"
 */
b32
stml_scribere_ad_aedificator(
    StmlNodus*          nodus,
    ChordaAedificator*  aedificator,
    b32                 pulchrum,
    i32                 indentatio);

#endif /* STML_H */
