/* md_registrum.h - Vocabularium nodorum markdown (md-arbor-spec par. IV)
 *
 * MANU SCRIPTUM ut css_registrum.h: enumeratio typum VERUM habet et
 * comitem MD_GENUS_NUMERUS; probatio congruentiam cum tabula per
 * TITULOS et CONTIGUITATEM offsetuum asserit.
 *
 * MODELLUM LINEARUM (spec par. III): quaeque linea a bloco FOLIO
 * uno possidetur; marcae continentium ('>', marcae listarum,
 * indentatio) sunt PRAEFIXA lineae in qua apparent; continentia
 * (citatio, lista, elementum) octetos NULLOS possident. Lineae vacuae
 * sunt bloci 'linea-vacua' continentis aperti. Arbor inlinea octetos
 * suos POSSIDET (folia bloci cum contento inlineo lineas crudas in
 * arborem inlineam SUBSTITUUNT - una forma, refinitio monotona ab
 * A3 ad A7).
 *
 * LOCI DERIVATI (lexema fons I: valor decoctus, url solutus, lingua):
 * adsunt SOLUM cum a crudo differunt; emissor ea omittit
 * (consilium.fons_index = ZEPHYRUM), proiectio ea cum f="1" scribit.
 *
 * Genera APPENDUNTUR, numquam interponuntur: interpositio indices
 * TACITE moveret.
 */

#ifndef MD_REGISTRUM_H
#define MD_REGISTRUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* bloci */
    MD_GENUS_DOCUMENTUM = 0,
    MD_GENUS_PRAEFATIO,        /* YAML front matter, crudum */
    MD_GENUS_LINEA,            /* linea cruda: praefixa + contentum + finis */
    MD_GENUS_LINEA_VACUA,      /* praefixa + finis */
    MD_GENUS_PARAGRAPHUS,
    MD_GENUS_CAPITULUM,
    MD_GENUS_SAEPTUM,          /* blocus codicis (saeptus aut indentatus) */
    MD_GENUS_LIMES,            /* linea saepti aperiens/claudens */
    MD_GENUS_CITATIO,
    MD_GENUS_LISTA,
    MD_GENUS_ELEMENTUM,
    MD_GENUS_DIVISIO,          /* fractura thematica */
    MD_GENUS_TABULA,
    MD_GENUS_ORDO,
    MD_GENUS_CELLA,
    MD_GENUS_HTML,             /* blocus html opacus */
    MD_GENUS_DEFINITIO_NEXUS,

    /* inlinea */
    MD_GENUS_INLINEA,
    MD_GENUS_TEXTUS,
    MD_GENUS_EMPHASIS,
    MD_GENUS_FORTIS,
    MD_GENUS_DELETIO,
    MD_GENUS_VERBATIM,         /* code span */
    MD_GENUS_NEXUS,
    MD_GENUS_IMAGO,
    MD_GENUS_FRACTURA_MOLLIS,
    MD_GENUS_FRACTURA_DURA,
    MD_GENUS_HTML_INLINEUM,

    MD_GENUS_NUMERUS
} MdGenus;


/* ==================================================
 * Indices LOCORUM intra nodum (fenestra generis)
 * ================================================== */

nomen enumeratio {
    MD_DOCUMENTUM_PRAEFATIO = 0,   /* NODUS? praefatio */
    MD_DOCUMENTUM_BLOCI,           /* LISTA_NODUS */
    MD_DOCUMENTUM_FINIS            /* TOKEN FINIS */
} MdLocusDocumenti;

nomen enumeratio {
    MD_PRAEFATIO_APERTUM = 0,      /* NODUS linea '---' */
    MD_PRAEFATIO_LINEAE,           /* LISTA_NODUS linea */
    MD_PRAEFATIO_CLAUSUM           /* NODUS linea '---' */
} MdLocusPraefationis;

nomen enumeratio {
    MD_LINEA_PRAEFIXA = 0,         /* LISTA_TOKEN */
    MD_LINEA_CONTENTUM,            /* TOKEN? */
    MD_LINEA_FINIS                 /* TOKEN? LINEA */
} MdLocusLineae;

nomen enumeratio {
    MD_VACUA_PRAEFIXA = 0,         /* LISTA_TOKEN */
    MD_VACUA_FINIS                 /* TOKEN? LINEA */
} MdLocusVacuae;

nomen enumeratio {
    MD_PARAGRAPHUS_PRAEFIXA = 0,   /* LISTA_TOKEN: praefixa lineae primae */
    MD_PARAGRAPHUS_NUDUS,          /* INDEX 0/1: filius directus elementi STRICTI */
    MD_PARAGRAPHUS_INLINEA         /* NODUS inlinea */
} MdLocusParagraphi;

nomen enumeratio {
    MD_CAPITULUM_PRAEFIXA = 0,     /* LISTA_TOKEN */
    MD_CAPITULUM_GRADUS,           /* INDEX 1..6 */
    MD_CAPITULUM_MARCA,            /* TOKEN? MARCA_ATX (+ spatia) */
    MD_CAPITULUM_INLINEA,          /* NODUS inlinea */
    MD_CAPITULUM_CLAUSUM,          /* TOKEN? CLAUSURA_ATX */
    MD_CAPITULUM_FINIS,            /* TOKEN? LINEA lineae contenti ultimae */
    MD_CAPITULUM_SUBDUCTIO         /* NODUS? linea (setext) */
} MdLocusCapituli;

nomen enumeratio {
    MD_SAEPTUM_FORMA = 0,          /* INDEX: MdFormaSaepti */
    MD_SAEPTUM_APERTUM,            /* NODUS? limes */
    MD_SAEPTUM_LINGUA,             /* TOKEN? DERIVATUM */
    MD_SAEPTUM_LINEAE,             /* LISTA_NODUS linea */
    MD_SAEPTUM_CLAUSUM             /* NODUS? limes */
} MdLocusSaepti;

nomen enumeratio {
    MD_LIMES_PRAEFIXA = 0,         /* LISTA_TOKEN */
    MD_LIMES_SIGNUM,               /* TOKEN SAEPTUM */
    MD_LIMES_INFO,                 /* TOKEN? INFO (crudum, cum spatiis) */
    MD_LIMES_FINIS                 /* TOKEN? LINEA */
} MdLocusLimitis;

nomen enumeratio {
    MD_CITATIO_BLOCI = 0           /* LISTA_NODUS */
} MdLocusCitationis;

nomen enumeratio {
    MD_LISTA_GENUS = 0,            /* INDEX: MdGenusListae */
    MD_LISTA_INITIUM,              /* INDEX: numerus primus (numerata) */
    MD_LISTA_LAXA,                 /* INDEX 0/1 */
    MD_LISTA_ELEMENTA              /* LISTA_NODUS elementum */
} MdLocusListae;

nomen enumeratio {
    MD_ELEMENTUM_OFFICIUM = 0,     /* INDEX: MdOfficium */
    MD_ELEMENTUM_BLOCI             /* LISTA_NODUS */
} MdLocusElementi;

nomen enumeratio {
    MD_DIVISIO_LINEA = 0           /* NODUS linea */
} MdLocusDivisionis;

nomen enumeratio {
    MD_TABULA_CAPUT = 0,           /* NODUS ordo */
    MD_TABULA_SEPARATOR,           /* NODUS linea */
    MD_TABULA_ORDINES              /* LISTA_NODUS ordo */
} MdLocusTabulae;

nomen enumeratio {
    MD_ORDO_PRAEFIXA = 0,          /* LISTA_TOKEN */
    MD_ORDO_CELLAE,                /* LISTA_NODUS cella */
    MD_ORDO_CLAUSUM,               /* TOKEN? PIPA finalis (+ spatia) */
    MD_ORDO_FINIS                  /* TOKEN? LINEA */
} MdLocusOrdinis;

nomen enumeratio {
    MD_CELLA_ORDINATIO = 0,        /* INDEX: MdOrdinatio */
    MD_CELLA_APERTUM,              /* TOKEN? PIPA ducens (+ spatia) */
    MD_CELLA_INLINEA               /* NODUS inlinea */
} MdLocusCellae;

nomen enumeratio {
    MD_HTML_LINEAE = 0             /* LISTA_NODUS linea */
} MdLocusHtml;

nomen enumeratio {
    MD_DEFINITIO_LINEAE = 0,       /* LISTA_NODUS linea */
    MD_DEFINITIO_TITULUS,          /* TOKEN? DERIVATUM (normalizatus) */
    MD_DEFINITIO_URL,              /* TOKEN? DERIVATUM */
    MD_DEFINITIO_DESCRIPTIO        /* TOKEN? DERIVATUM */
} MdLocusDefinitionis;

nomen enumeratio {
    MD_INLINEA_LIBERI = 0          /* LISTA_NODUS */
} MdLocusInlineae;

nomen enumeratio {
    MD_TEXTUS_CRUDUM = 0,          /* LISTA_TOKEN */
    MD_TEXTUS_VALOR                /* TOKEN? DERIVATUM (decoctus) */
} MdLocusTextus;

/* emphasis, fortis, deletio - forma una */
nomen enumeratio {
    MD_ORNATUS_APERTUM = 0,        /* TOKEN DELIMITATOR */
    MD_ORNATUS_LIBERI,             /* LISTA_NODUS */
    MD_ORNATUS_CLAUSUM             /* TOKEN DELIMITATOR */
} MdLocusOrnatus;

nomen enumeratio {
    MD_VERBATIM_APERTUM = 0,       /* TOKEN GRAVIS */
    MD_VERBATIM_CRUDUM,            /* LISTA_TOKEN */
    MD_VERBATIM_VALOR,             /* TOKEN? DERIVATUM (normalizatus) */
    MD_VERBATIM_CLAUSUM            /* TOKEN GRAVIS */
} MdLocusVerbatimi;

/* nexus, imago - forma una */
nomen enumeratio {
    MD_NEXUS_FORMA = 0,            /* INDEX: MdFormaNexus */
    MD_NEXUS_APERTUM,              /* TOKEN NEXUS_APERTUM */
    MD_NEXUS_LIBERI,               /* LISTA_NODUS */
    MD_NEXUS_CAUDA,                /* LISTA_TOKEN: '](..)' '][ref]' '>' */
    MD_NEXUS_URL,                  /* TOKEN? DERIVATUM */
    MD_NEXUS_DESCRIPTIO            /* TOKEN? DERIVATUM */
} MdLocusNexus;

nomen enumeratio {
    MD_MOLLIS_FINIS = 0,           /* TOKEN LINEA */
    MD_MOLLIS_PRAEFIXA             /* LISTA_TOKEN: praefixa lineae sequentis */
} MdLocusMollis;

nomen enumeratio {
    MD_DURA_SIGNUM = 0,            /* TOKEN SPATIA_FINALIA | EFFUGIUM */
    MD_DURA_FINIS,                 /* TOKEN LINEA */
    MD_DURA_PRAEFIXA               /* LISTA_TOKEN */
} MdLocusDurae;

nomen enumeratio {
    MD_HTML_INLINEUM_CRUDUM = 0    /* LISTA_TOKEN */
} MdLocusHtmlInlinei;


/* ==================================================
 * Valores locorum INDEX (omnes >= ZEPHYRUM - index negativus recusatur)
 * ================================================== */

nomen enumeratio {
    MD_LISTA_PUNCTATA = 0,
    MD_LISTA_NUMERATA
} MdGenusListae;

nomen enumeratio {
    MD_OFFICIUM_NULLUM = 0,
    MD_OFFICIUM_APERTUM,
    MD_OFFICIUM_PERFECTUM
} MdOfficium;

nomen enumeratio {
    MD_SAEPTUM_SAEPTUS = 0,
    MD_SAEPTUM_INDENTATUS
} MdFormaSaepti;

nomen enumeratio {
    MD_ORDINATIO_NULLA = 0,
    MD_ORDINATIO_SINISTRA,
    MD_ORDINATIO_MEDIA,
    MD_ORDINATIO_DEXTRA
} MdOrdinatio;

nomen enumeratio {
    MD_NEXUS_INLINEUS = 0,
    MD_NEXUS_REFERENS,
    MD_NEXUS_AUTO
} MdFormaNexus;


externus constans MateriaRegistrumCoctum MD_REGISTRUM;

#endif /* MD_REGISTRUM_H */
