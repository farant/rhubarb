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
 * - Raw line capture: <tag! (>reliquum lineae crudum (ad '\n')
 * - Fragmenta capturantia: <#id (> / <# (> - fragmentum fratres
 *   sequentes capit (directio ANTE sola); saccharum <(> =
 *   fragmentum anonymum capturans, scriptor ad <# (> normalizat
 * - Clausura anonyma: </> elementum apertum proximum claudit
 *   (elementa sola - crudi et fragmenta clausuram expressam
 *   tenent); forma authoris in scriptura SERVATUR (octetim)
 *
 * Proprietates:
 * - DOM-stylus arbor
 * - Omnes chordae internatae
 * - Omnes allocationes ex Piscina
 * - Entia (&lt; &gt; &amp; &quot; &apos;) in TEXTU solo:
 *   scriptor effugit, lector solvit (symmetricum). Valores
 *   attributorum CRUDI ambabus viis - quota duplex (") in valore
 *   attributi irrepraesentabilis; tags crudi verbatim.
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
    STML_NODUS_DOCTYPE      = VI,
    STML_NODUS_TRANSCLUSIO  = VII   /* <<selector>> */
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
    STML_TOKEN_FINIS         = XII,   /* EOF */
    STML_TOKEN_FRAGMENTUM_APERIRE   = XIII,  /* <#> or <#id> */
    STML_TOKEN_FRAGMENTUM_CLAUDERE  = XIV,   /* </#> */
    STML_TOKEN_FRAGMENTUM_AUTO      = XV,    /* <#/> aut <#id/>; cum
                                                captio_numerus > 0 =
                                                <# (> / <#id (> / <(> */
    STML_TOKEN_TRANSCLUSIO          = XVI    /* <<selector>> */
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
    chorda*                  valor;           /* Text/comment/PI/transclusion content */
    Xar*                     attributa;       /* Xar de StmlAttributum */
    Xar*                     liberi;          /* Xar de StmlNodus* */
    structura StmlNodus*     parens;

    /* STML-specific fields */
    b32                      crudus;          /* Raw content tag (! suffix) */
    StmlCaptioDirectio       captio_directio;
    i32                      captio_numerus;  /* Number of capture parens */
    b32                      clausura_anonyma; /* Clausum per </> - scriptor
                                                * formam authoris servat */

    /* Fragment fields */
    b32                      fragmentum;      /* Is this a fragment? <#> or <#id> */
    chorda*                  fragmentum_id;   /* Fragment ID (NIHIL for anonymous) */
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

/* Capere textum internum (concatenatum), VERBATIM
 * "Get inner text content"
 *
 * Omnes nodos textus posterorum ordine documenti concatenat,
 * SINE mutatione - id quod textContent (DOM) et string() (XPath)
 * significant. Spatium album CONTENTUS est.
 *
 * MIGRATIO 2026-08-06: normalizatio prius in PARSATIONE fiebat
 * (nodi praecidebantur, nodi spatii albi solius abiciebantur).
 * Gradu falso stabat: circuitum frangebat et contentum mixtum
 * conglutinabat ('salve <b>munde</b> iterum' -> 'salvemundeiterum';
 * optiones -> 'disciplinastructuracryptographica'). Nunc arbor
 * documentum fideliter refert et normalizatio HIC eligitur.
 */
chorda
stml_textus_internus(
    StmlNodus* nodus,
    Piscina*   piscina);

/* Capere textum internum NORMALIZATUM
 * "Get inner text content, whitespace-normalized"
 *
 * Idem quod stml_textus_internus, deinde spatium album
 * normalizatum: indentatio communis remota, lineae vacuae
 * initiales/finales abscisae, textus unius lineae praecisus.
 * Hoc vult qui STML manu scriptum legit et prosam sine
 * indentatione fontis expectat.
 */
chorda
stml_textus_normalizatus(
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
 * Navigatio - Traversal
 * ================================================== */

/* Capere indicem nodi inter fratres
 * "Get index of node among siblings"
 * Redde: -1 si non habet parentem
 */
s32
stml_index_inter_fratres(
    StmlNodus* nodus);

/* Capere proximum fratrem
 * "Get next sibling"
 */
StmlNodus*
stml_frater_proximus(
    StmlNodus* nodus);

/* Capere priorem fratrem
 * "Get previous sibling"
 */
StmlNodus*
stml_frater_prior(
    StmlNodus* nodus);

/* Capere primum liberum
 * "Get first child"
 */
StmlNodus*
stml_primus_liberum(
    StmlNodus* nodus);

/* Capere ultimum liberum
 * "Get last child"
 */
StmlNodus*
stml_ultimus_liberum(
    StmlNodus* nodus);

/* Capere omnes fratres (excludens se)
 * "Get all siblings (excluding self)"
 * Redde: Xar de StmlNodus*
 */
Xar*
stml_fratres(
    StmlNodus* nodus,
    Piscina*   piscina);

/* Capere omnes maiores (parens, avus, etc.)
 * "Get all ancestors"
 * Redde: Xar de StmlNodus* (a parente ad radicem)
 */
Xar*
stml_maiores(
    StmlNodus* nodus,
    Piscina*   piscina);

/* Invenire proximum maiorem congruentem cum selectore
 * "Find closest ancestor matching selector"
 * Nota: includit se si congruit
 */
StmlNodus*
stml_proximus_maior(
    StmlNodus*           nodus,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);

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

/* ==================================================
 * Tituli - Labels (class-like attribute)
 *
 * Labels stored in "labels" attribute (or "class" for HTML compat)
 * as space-separated strings, e.g. labels="foo bar baz"
 * ================================================== */

/* Verificare si nodus habet titulum
 * "Check if node has label"
 */
b32
stml_titulum_habet(
    StmlNodus*          nodus,
    constans character* titulum);

/* Numerus titulorum
 * "Number of labels"
 */
i32
stml_titulos_numerus(
    StmlNodus* nodus);

/* Capere omnes titulos ut Xar de chorda
 * "Get all labels as array of strings"
 */
Xar*
stml_titulos_capere(
    StmlNodus* nodus,
    Piscina*   piscina);

/* Addere titulum ad nodum
 * "Add label to node"
 * Redde: VERUM si additum, FALSUM si iam existit
 */
b32
stml_titulum_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum);

/* Removere titulum ex nodo
 * "Remove label from node"
 * Redde: VERUM si remotum, FALSUM si non inventum
 */
b32
stml_titulum_removere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum);

/* Commutare titulum (toggle)
 * "Toggle label on node"
 * Redde: VERUM si nunc habet, FALSUM si nunc non habet
 */
b32
stml_titulum_commutare(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum);

/* ==================================================
 * Mutatio - Tree Manipulation
 * ================================================== */

/* Alias pro stml_liberum_addere (appendChild) */
#define stml_appendere stml_liberum_addere

/* Addere liberum ad initium
 * "Prepend child"
 */
b32
stml_praeponere(
    StmlNodus* parens,
    StmlNodus* liberum,
    Piscina*   piscina);

/* Inserere ante nodum
 * "Insert before node"
 */
b32
stml_inserere_ante(
    StmlNodus* nodus,
    StmlNodus* novum,
    Piscina*   piscina);

/* Inserere post nodum
 * "Insert after node"
 */
b32
stml_inserere_post(
    StmlNodus* nodus,
    StmlNodus* novum,
    Piscina*   piscina);

/* Removere nodum ex parente
 * "Remove node from parent"
 * Nodus manet in memoria (piscina), sed non in arbore
 */
b32
stml_removere(
    StmlNodus* nodus,
    Piscina*   piscina);

/* Vacare omnes liberos
 * "Remove all children"
 */
vacuum
stml_vacare_liberos(
    StmlNodus* nodus);

/* Substituere nodum cum alio
 * "Replace node with another"
 */
b32
stml_substituere(
    StmlNodus* vetus,
    StmlNodus* novum,
    Piscina*   piscina);

/* ==================================================
 * Duplicatio - Cloning
 * ================================================== */

/* Duplicare nodum (profunde)
 * "Deep clone node"
 * Includit omnes liberos, attributa, etc.
 */
StmlNodus*
stml_duplicare(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Duplicare nodum (superficialiter)
 * "Shallow clone node"
 * Non includit liberos
 */
StmlNodus*
stml_duplicare_superficialiter(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* ==================================================
 * Strictum - forma BENE FORMATA super parsationem
 *
 * Parser consulto LENIS est. Haec probationes ea nominant quae
 * parsationem transeunt sed documentum TACITE ambiguum reddunt.
 * Politia vocantis est: parser eas non cogit, quia STML fragmenta
 * et usus alios legitime fert.
 *
 * QUOD HIC NON EST, et cur (mensuratum 2026-08-06):
 * - attributum sine quotis (nomen=valor): recte intellectum,
 *   ad nomen="valor" normalizatum. Lenitas, non vitium.
 * - ens ignotum (&ignotum;): ut textus litteralis tractatum,
 *   consulto. Circuitus mutat, sensus non.
 * Utrumque probatum et ACCEPTUM - non omne discrimen ab XML
 * defectus est.
 * ================================================== */

nomen enumeratio {
    /* idem attributum bis in uno elemento. PERICULOSISSIMUM:
     * ambo servantur, capere PRIMUM reddit - unde plagula unum
     * dicit et omnis lector alterum adhibet */
    STML_STRICTUM_ATTRIBUTUM_DUPLICATUM = I,
    /* elementa radicis plura quam unum - reliqua tacite ignorantur
     * ab omni vocante qui elementum_radix legit */
    STML_STRICTUM_RADICES_PLURES        = II,
    /* textus non-albus extra radicem */
    STML_STRICTUM_TEXTUS_EXTRA_RADICEM  = III,
    /* elementum sine nomine: '<>' titulum vacuum parit */
    STML_STRICTUM_TITULUS_VACUUS        = IV
} StmlStrictumGenus;

nomen structura {
    StmlStrictumGenus  genus;
    StmlNodus*         nodus;   /* ubi inventum */
    chorda*            causa;   /* nomen attributi duplicati, vel NIHIL */
} StmlStrictumVitium;

/* Probare arborem parsatam. radix = nodus DOCUMENTI (resultus.radix).
 * Redde: Xar de StmlStrictumVitium (vacuum si sanum) */
Xar*
stml_strictum(
    StmlNodus* radix,
    Piscina*   piscina);

/* Nuntius legibilis pro genere vitii */
constans character*
stml_strictum_nuntius(
    StmlStrictumGenus genus);

#endif /* STML_H */
