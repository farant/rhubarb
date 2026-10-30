/* xml_coctum.h - Visio cocta xml: quod documentum SIGNIFICAT
 *
 * Arbor (xml_arbor) octetos ut scripti sunt tenet; visio cocta ea
 * significat (spec par. V, X7): referentiae expansae, attributa
 * normalizata et praestituta applicata, lineae normalizatae, spatia
 * nominum soluta. Construitur SOLUM pro documento bene formato -
 * diagnostica tamen OMNIA semper redduntur (X6).
 *
 * GRADUS (ordine):
 *   I    arbor (xml_arbor_parsare) + diagnostica derivata (absentiae,
 *        mala; materia_diagnostica_derivare cum emissis aedificatoris)
 *   II   lexemata singula: characteres (Char, UTF-8), commentaria
 *        ('--'), instructiones (scopus, 'xml' reservatum), CDATA,
 *        declaratio XML (version, encoding X8, standalone, ordo)
 *   III  DTD (pars interna): declarationes per grammaticam suam,
 *        entitates, attributa, notationes; referentiae parametri inter
 *        declarationes (internae expanduntur, externae numquam - X3;
 *        regula par. 5.1: post externam non lectam declarationes
 *        entitatum et attributorum non tractantur nisi standalone)
 *   IV   contentum: textus, referentiae (characterum, praedefinitae,
 *        entitatum internarum - textus substitutionis ut contentum
 *        parsatus, recursio vetita, limites X4), attributa (par.
 *        3.3.3), praestituta, WFC omnes
 *   V    spatia nominum (NS 1.0)
 *
 * AMBULATIO ITERATIVA ubique (profunditas sine limite acervi C).
 */

#ifndef XML_COCTUM_H
#define XML_COCTUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_nodus.h"

/* codices diagnosticorum coctionis ('genus/regula') */
#define XML_CODEX_CHARACTER          "textus/character"
#define XML_CODEX_UTF8               "textus/utf8"
#define XML_CODEX_COMMENTARIUM       "commentarium/forma"
#define XML_CODEX_INSTRUCTIO         "instructio/forma"
#define XML_CODEX_CDATA              "cdata/forma"
#define XML_CODEX_DECLARATIO_XML     "declaratio-xml/forma"
#define XML_CODEX_CODIFICATIO        "declaratio-xml/codificatio"
#define XML_CODEX_DOCTYPE            "doctype/forma"
#define XML_CODEX_DECLARATIO         "declaratio/forma"
#define XML_CODEX_REFERENTIA         "referentia/forma"
#define XML_CODEX_ENTITAS_IGNOTA     "referentia/entitas-ignota"
#define XML_CODEX_RECURSIO           "referentia/recursio"
#define XML_CODEX_NON_PARSATA        "referentia/entitas-non-parsata"
#define XML_CODEX_EXTERNA_IN_VALORE  "referentia/externa-in-valore"
#define XML_CODEX_LIMES              "referentia/limes"
#define XML_CODEX_CONTENTUM_ENTITATIS "referentia/contentum"
#define XML_CODEX_ATTRIBUTUM_BIS     "attributum/bis"
#define XML_CODEX_MINOR_IN_VALORE    "attributum/minor"
#define XML_CODEX_ATTRIBUTUM_FORMA   "attributum/forma"
#define XML_CODEX_PARAMETRUM_INTRA   "pars-interna/parametrum-intra"
#define XML_CODEX_SPATIUM_NOMINUM    "spatia/forma"

nomen enumeratio {
    XML_COCTUS_DOCUMENTUM = 0,
    XML_COCTUS_ELEMENTUM,
    XML_COCTUS_TEXTUS,
    XML_COCTUS_COMMENTARIUM,
    XML_COCTUS_INSTRUCTIO
} XmlCoctusGenus;

nomen structura {
                    chorda  titulus;      /* ut scriptus (QName) */
                    chorda  spatium;      /* URI; mensura 0 si nullum */
                    chorda  localis;      /* pars localis */
                    chorda  valor;        /* normatus (par. 3.3.3) */
                       b32  praestitutum; /* ex <!ATTLIST> */
     constans MateriaNodus* nodus;        /* attributum; NIHIL si
                                           * praestitutum */
} XmlAttributumCoctum;

nomen structura XmlCoctus XmlCoctus;

structura XmlCoctus {
            XmlCoctusGenus genus;
                    chorda titulus;  /* elementum: QName; instructio:
                                       * scopus */
                    chorda spatium;  /* elementum: URI */
                    chorda localis;  /* elementum: pars localis */
                    chorda textus;   /* textus, commentarium, data
                                       * instructionis */
                       b32  cdata;    /* textus (pars) ex CDATA */
                       Xar* attributa; /* XmlAttributumCoctum, ordine */
                       Xar* liberi;    /* XmlCoctus*, ordine */
                 XmlCoctus* parens;
                       i32  index;    /* locus in liberis parentis */
     constans MateriaNodus* nodus;    /* nodus arboris (NIHIL ex
                                       * entitate: nodus referentiae) */
};

nomen structura {
    chorda titulus;
    chorda publicum;
    chorda systema;
       b32 habet_publicum;
       b32 habet_systema;
} XmlNotatio;

nomen structura {
    XmlCoctus* documentum;    /* NIHIL si non bene formatum */
          Xar* diagnostica;   /* MateriaDiagnosticum omnia, ordine
                               * (tractus.initium, codex) */
          b32  bene_formatum;
          Xar* notationes;    /* XmlNotatio, ordine declarationis */
       chorda  titulus_typi;  /* titulus doctypi; mensura 0 si nullus */
          s32  locus_typi;    /* index in liberis documenti cocti ubi
                               * DOCTYPE stetit (post instructiones
                               * partis internae, quae ibi ut liberi
                               * documenti stant); -I si nullus */
    MateriaNodus* arbor;      /* arbor materiae */
} XmlCoctum;

/* fons NON copiatur. spatia: VERUM = Namespaces 1.0 iudicantur et
 * solvuntur (ordinarium); FALSUM = processor sine spatiis (casus
 * suitae NAMESPACE="no"). Numquam NIHIL in campis nisi memoria
 * deficit (diagnostica NIHIL). */
XmlCoctum
xml_coquere (
               Piscina* piscina,
    constans character* fons,
                   s32  mensura,
                   b32  spatia);

#endif /* XML_COCTUM_H */
