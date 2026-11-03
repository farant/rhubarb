/* xml_registrum_coctum.h - GENERATUM, NE MANU EDITES
 *
 * Registrum generum COCTUM grammaticae 'xml'. Fons:
 * xml/grammatica/xml.registrum.stml (materia/coquere.sh). Genera XV,
 * loci XXXIII.
 */

#ifndef XML_REGISTRUM_COCTUM_H
#define XML_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* Radix: prologus (declaratio-xml, doctype, commentaria,
     * instructiones, textus spatiorum), elementum radicis, epilogus,
     * mala - ordine octetorum + lexema FINIS */
    XML_GENUS_DOCUMENTUM = 0,
    /* BOM UTF-8 (EF BB BF) ad octetum primum */
    XML_GENUS_SIGNUM_ORDINIS,
    /* &lt;?xml version=... encoding=... standalone=...?&gt; ad
     * initium solum (post BOM) */
    XML_GENUS_DECLARATIO_XML,
    /* &lt;!DOCTYPE titulus [SYSTEM 's' | PUBLIC 'p' 's'] [pars
     * interna]&gt; */
    XML_GENUS_DOCTYPE,
    /* [ declarationes, referentiae parametri, commentaria,
     * instructiones ] */
    XML_GENUS_PARS_INTERNA,
    /* &lt;!ELEMENT, &lt;!ATTLIST, &lt;!ENTITY, &lt;!NOTATION: genus
     * lexematis aperturae dicit, partes xml_dtd legit */
    XML_GENUS_DECLARATIO,
    /* %titulus; inter declarationes partis internae */
    XML_GENUS_REFERENTIA_PARAMETRI,
    /* &lt;titulus attributa&gt; liberi &lt;/titulus&gt;, aut
     * &lt;titulus attributa/&gt; (tok_finis '/&gt;', liberi et
     * clausura nulli) */
    XML_GENUS_ELEMENTUM,
    /* titulus = 'valor' (et pseudo-attributa declarationis XML) */
    XML_GENUS_ATTRIBUTUM,
    /* Characteres contenti (spatia incluse) usque ad '&lt;' aut
     * '&amp;' */
    XML_GENUS_TEXTUS,
    /* &amp;titulus; aut &amp;#N; aut &amp;#xH;: coctio expandit */
    XML_GENUS_REFERENTIA,
    /* &lt;![CDATA[ ... ]]&gt;: textus crudus */
    XML_GENUS_CDATA,
    /* &lt;!-- ... --&gt;: contentum, non trivium */
    XML_GENUS_COMMENTARIUM,
    /* &lt;?scopus data?&gt;: coctio scopum et data dividit */
    XML_GENUS_INSTRUCTIO,
    /* Totalitas (X6): lexemata quae grammatica ponere non potuit -
     * octeti manent, sanitas negatur */
    XML_GENUS_MALUM,

    XML_GENUS_NUMERUS_GENERUM
} XmlGenus;

externus constans MateriaRegistrumCoctum XML_REGISTRUM;

externus constans MateriaDiagnosticaCocta XML_DIAGNOSTICA;

#endif /* XML_REGISTRUM_COCTUM_H */
