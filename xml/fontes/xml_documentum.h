/* xml_documentum.h - XML 1.0 + Namespaces 1.0: API publica
 *
 * Lege documentum, quaere successum et diagnostica, ambula visionem
 * coctam. Super xml_coctum (visio: referentiae expansae, attributa
 * normata et praestituta, lineae normatae, spatia nominum soluta) et
 * xml_arbor (arbor materiae, octetim fidelis). Spec:
 * project-specs/xml-arbor-spec.md par. V.
 *
 * NOMEN API: 'xml_documentum_*' et accessores 'xml_*' super
 * XmlCoctus - nomina include/xml.h veteris (XmlNodus, xml_legere,
 * xml_invenire_liberum ...) consulto vitantur dum id vivit (spec X10).
 *
 * USUS:
 *   XmlDocumentum* d = xml_documentum_legere(textus, piscina);
 *   si (!xml_documentum_successus(d)) {
 *       xml_documentum_diagnostica_scribere(d, "a.xml", aedificator);
 *   }
 *   radix = xml_documentum_radix(d);
 *   per (f = xml_filius(radix, "part"); f; f = xml_frater(f, "part"))
 *       ...
 *
 * TITULI: titulus = QName UT SCRIPTUS ('dc:title'), comparatio octetim;
 * NIHIL = quilibet. Spatia nominum: xml_elementi_spatium/localis, et
 * xml_attributum_spatii(e, uri, localis, ...).
 *
 * LIBERI: xml_liberum/xml_liberorum_numerus reddunt OMNES (textus,
 * commentaria, instructiones, elementa); xml_filius/xml_frater solum
 * elementa. Textus spatii solius servatur (lex octetorum: nihil
 * abicitur) - qui elementa sola vult xml_filius adhibet.
 *
 * Omnia NIHIL-tuta: argumentum NIHIL -> NIHIL / FALSUM / 0.
 */

#ifndef XML_DOCUMENTUM_H
#define XML_DOCUMENTUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "xml_coctum.h"

nomen structura XmlDocumentum XmlDocumentum;   /* opacum */

/* textus NON copiatur (in piscina vivere debet dum documentum vivit).
 * Numquam NIHIL nisi memoria deficit: documentum malum diagnostica
 * fert, radicem non. */
XmlDocumentum*
xml_documentum_legere (
      chorda  textus,
     Piscina* piscina);

/* VERUM = bene formatum (XML 1.0 + Namespaces 1.0) */
b32
xml_documentum_successus (
    constans XmlDocumentum* documentum);

/* Xar de MateriaDiagnosticum, ordine (initium, codex); vacuum si
 * sanum */
Xar*
xml_documentum_diagnostica (
    constans XmlDocumentum* documentum);

/* diagnostica reddita ('via:linea:columna: causa', excerptum, caret)
 * per materia_pictor; FALSUM = memoria deficit */
b32
xml_documentum_diagnostica_scribere (
    constans XmlDocumentum* documentum,
        constans character* via,
         ChordaAedificator* exitus);

/* elementum radicis; NIHIL si non bene formatum */
constans XmlCoctus*
xml_documentum_radix (
    constans XmlDocumentum* documentum);

/* visio tota (documentum coctum, notationes, arbor materiae) */
constans XmlCoctum*
xml_documentum_coctum (
    constans XmlDocumentum* documentum);


/* ==================================================
 * Nodi cocti
 * ================================================== */

XmlCoctusGenus
xml_nodi_genus (
    constans XmlCoctus* nodus);

/* elementum: QName ut scriptus; instructio: scopus */
chorda
xml_titulus (
    constans XmlCoctus* nodus);

/* elementum: pars localis et URI spatii (mensura 0 si nullum) */
chorda
xml_elementi_localis (
    constans XmlCoctus* elementum);

chorda
xml_elementi_spatium (
    constans XmlCoctus* elementum);

/* textus nodi textus aut commentarii, data instructionis */
chorda
xml_textus (
    constans XmlCoctus* nodus);

i32
xml_liberorum_numerus (
    constans XmlCoctus* nodus);

constans XmlCoctus*
xml_liberum (
    constans XmlCoctus* nodus,
                   i32  index);

/* primus filius ELEMENTUM titulo dato (NIHIL = quilibet) */
constans XmlCoctus*
xml_filius (
    constans XmlCoctus* elementum,
    constans character* titulus);

/* frater proximus ELEMENTUM titulo dato (NIHIL = quilibet) */
constans XmlCoctus*
xml_frater (
    constans XmlCoctus* nodus,
    constans character* titulus);

/* numerus filiorum ELEMENTORUM titulo dato (NIHIL = omnes) */
i32
xml_filiorum_numerus (
    constans XmlCoctus* elementum,
    constans character* titulus);

/* attributum per QName: VERUM et *valor si adest (praestituta
 * incluso) */
b32
xml_attributum (
    constans XmlCoctus* elementum,
    constans character* titulus,
                chorda* valor);

/* attributum per nomen expansum (uri vacuum = sine spatio) */
b32
xml_attributum_spatii (
    constans XmlCoctus* elementum,
    constans character* uri,
    constans character* localis,
                chorda* valor);

/* textus omnis descendentium ordine documenti (commentaria et
 * instructiones praeteruntur); FALSUM = memoria deficit */
b32
xml_textus_totus (
    constans XmlCoctus* elementum,
               Piscina* piscina,
                chorda* textus);

/* sedes fontis nodi: linea et columna (I-basatae, columna octetis)
 * initii eius. Nodus ex entitate expansa sedem non habet: sedes
 * proximi maioris qui habet redditur (referentia '&a;' aut elementum
 * continens). FALSUM = nodus NIHIL aut nulla sedes (linea, columna
 * non mutantur). Clientes (musicxml) diagnostica sua ita locant. */
b32
xml_locus (
    constans XmlCoctus* nodus,
                   i32* linea,
                   i32* columna);

#endif /* XML_DOCUMENTUM_H */
