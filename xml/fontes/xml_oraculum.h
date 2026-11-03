/* xml_oraculum.h - Forma canonica suitae W3C ex visione cocta
 *
 * Formae duae (lectae ex plagulis out/ suitae, xml/probationes/fixa/
 * xmlts/PROVENIENTIA.md):
 *   PRIMA (James Clark): nulla declaratio XML, nulla commentaria,
 *     nullus DOCTYPE; elementum vacuum = <a></a>; attributa ordine
 *     titulorum (octetorum UTF-8 = punctorum codicis); in textu et
 *     valoribus & < > " -> &amp; &lt; &gt; &quot;, TAB LF CR ->
 *     &#9; &#10; &#13;; instructio = <?scopus data?> (spatium unum
 *     semper).
 *   SECUNDA: si DTD notationes declarat, '<!DOCTYPE radix [\n' +
 *     notationes ordine titulorum ('<!NOTATION n PUBLIC 'p' 's'>' /
 *     PUBLIC 'p' / SYSTEM 's', una per lineam) + ']>\n' LOCO DOCTYPI in
 *     documento (instructio ante DOCTYPE ante eum manet).
 * Ambulatio iterativa.
 */

#ifndef XML_ORACULUM_H
#define XML_ORACULUM_H

#include "latina.h"
#include "chorda_aedificator.h"
#include "xml_coctum.h"

/* FALSUM: documentum non bene formatum (coctum->documentum NIHIL) aut
 * memoria deficit */
b32
xml_oraculum_scribere (
     constans XmlCoctum* coctum,
                Piscina* piscina,
      ChordaAedificator* exitus);

#endif /* XML_ORACULUM_H */
