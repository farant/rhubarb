/* xml_registrum.h - Registrum generum xml: caput generatum et
 * enumerationes locorum manu scriptae
 *
 * XmlGenus, XML_GENUS_NUMERUS_GENERUM, XML_REGISTRUM et
 * XML_DIAGNOSTICA ex xml_registrum_coctum.h veniunt (GENERATUM ex
 * xml/grammatica/xml.registrum.stml per ./materia/coquere.sh).
 * Hic: enumerationes locorum per genus (ordo = ordo declarationis;
 * probatio_xml_registrum quamque per titulos asserit).
 *
 * Genera quae formam locorum communem habent enumerationem communem
 * habent (commentarium socios nominat); tabula locorum nominatorum
 * portae registri quodque genus seorsum enumerat.
 */

#ifndef XML_REGISTRUM_H
#define XML_REGISTRUM_H

#include "latina.h"
#include "xml_registrum_coctum.h"


/* ==================================================
 * Loci per genus
 * ================================================== */

nomen enumeratio { XML_DOCUMENTUM_LIBERI = 0, XML_DOCUMENTUM_CAUDA }
    XmlDocumentumLocus;

/* signum-ordinis, referentia-parametri, textus, referentia, cdata,
 * commentarium, instructio */
nomen enumeratio { XML_LEXEMA_TOK = 0 } XmlLexemaLocus;

/* declaratio-xml */
nomen enumeratio {
    XML_DECLARATIO_XML_TOK_APERTURA = 0, XML_DECLARATIO_XML_ATTRIBUTA,
    XML_DECLARATIO_XML_TOK_CLAUSURA
} XmlDeclaratioXmlLocus;

nomen enumeratio {
    XML_DOCTYPE_TOK_APERTURA = 0, XML_DOCTYPE_TOK_TITULUS,
    XML_DOCTYPE_ID_EXTERNUM, XML_DOCTYPE_PARS_INTERNA,
    XML_DOCTYPE_TOK_CLAUSURA
} XmlDoctypeLocus;

/* pars-interna */
nomen enumeratio {
    XML_PARS_INTERNA_TOK_APERTURA = 0, XML_PARS_INTERNA_LIBERI,
    XML_PARS_INTERNA_TOK_CLAUSURA
} XmlParsInternaLocus;

nomen enumeratio {
    XML_DECLARATIO_TOK_APERTURA = 0, XML_DECLARATIO_PARTES,
    XML_DECLARATIO_TOK_CLAUSURA
} XmlDeclaratioLocus;

nomen enumeratio {
    XML_ELEMENTUM_TOK_APERTURA = 0, XML_ELEMENTUM_ATTRIBUTA,
    XML_ELEMENTUM_TOK_FINIS, XML_ELEMENTUM_LIBERI,
    XML_ELEMENTUM_TOK_CLAUSURA, XML_ELEMENTUM_TOK_CLAUSURA_FINIS
} XmlElementumLocus;

nomen enumeratio {
    XML_ATTRIBUTUM_TOK_TITULUS = 0, XML_ATTRIBUTUM_TOK_AEQUALE,
    XML_ATTRIBUTUM_TOK_VALOR
} XmlAttributumLocus;

nomen enumeratio { XML_MALUM_TOKENS = 0 } XmlMalumLocus;

#endif /* XML_REGISTRUM_H */
