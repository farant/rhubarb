/* html_registrum.h - Vocabularium nodorum HTML
 *
 * Enumeratio generum et tabulae COCTAE generantur ex
 * html/grammatica/html.registrum.stml (materia/coquere.sh):
 * HtmlGenus, HTML_GENUS_NUMERUS_GENERUM, HTML_REGISTRUM. Genera
 * APPENDUNTUR, numquam interponuntur (ordo = enumeratio = offsets;
 * sigillum canonis pinna, manu mota cum causa). Hic manent quae
 * clientis sunt: enumerationes LOCORUM nominatae.
 *
 * Vocabularium ex html-arbor-spec.md par. XI.3 (genera X). Nulla
 * genera structuralia; robustitas per genus SPEC-DEFINITUM unum
 * (elementum-malum: tag clausurae sine pari). Ordo locorum intra
 * genus = ordo EMISSIONIS: attributa ante tok_finis, liberi ante
 * tok_clausura. Locus 'synthesis' (O7a) index est: emissor eum
 * transit, visio cocta legit.
 */

#ifndef HTML_REGISTRUM_H
#define HTML_REGISTRUM_H

#include "latina.h"
#include "materia_registrum.h"
#include "html_registrum_coctum.h"


/* ==================================================
 * Indices LOCORUM intra nodum
 *
 * Registrum fenestram nominat (loci_offset + loci_numerus); hi
 * indices INTRA fenestram sunt, quos materia_nodus_ponere et
 * materia_nodus_appendere accipiunt. Probatio hos contra TITULOS
 * tabulae retiret, ne tertia tabula manu scripta fiant quae per se
 * labi possit.
 * ================================================== */

nomen enumeratio {
    HTML_DOCUMENTUM_LIBERI = 0,
    HTML_DOCUMENTUM_CAUDA
} HtmlLocusDocumenti;

nomen enumeratio {
    HTML_DOCTYPE_TOK = 0
} HtmlLocusDoctype;

nomen enumeratio {
    HTML_ELEMENTUM_TOK_APERTURA = 0,
    HTML_ELEMENTUM_ATTRIBUTA,
    HTML_ELEMENTUM_TOK_FINIS,
    HTML_ELEMENTUM_LIBERI,
    HTML_ELEMENTUM_TOK_CLAUSURA,
    HTML_ELEMENTUM_TOK_CLAUSURA_FINIS,
    HTML_ELEMENTUM_SYNTHESIS,
    HTML_ELEMENTUM_SEDES,
    HTML_ELEMENTUM_EXEMPLAR,
    HTML_ELEMENTUM_PRAECEDENS
} HtmlLocusElementi;

/* Locus 'exemplar' (O7c, 2026-09-16; in elementum): REFERENTIA ad
 * elementum ORIGINALE cuius hoc elementum iterum apertum est (WHATWG
 * 'create an element for the token for which node was created' -
 * adoption agency, reconstructio listae formantium activorum).
 * Lexemata nulla; titulus et attributa per referentiam leguntur;
 * emissor nihil scribit, visio cocta elementum ut originale imprimit.
 * Referentia semper ad originale VERUM (exemplar exemplaris nusquam).
 *
 * Locus 'praecedens' (O7c; in elementum): REFERENTIA ad fratrem DOM
 * PRIOREM nodi ab adoption agency in avum communem moti (spec gradus
 * XIV: post elementum formans clausum appenditur). Cum sede solum;
 * visio cocta nodum post praecedentem in liberis sedis ponit - ordo
 * octetorum id dare non potest (frater prior serius natus esse potest,
 * '<b><a><div>x</b>y</a>'). Absens = regula sedis sola (ante tabulam
 * continentem, aliter post omnes). */

/* Locus 'sedes' (O7b, 2026-09-15; in elementum, textus, referentia,
 * commentarium):
 * REFERENTIA ad parentem DOM nodi cuius octeti alibi iacent - nodus in
 * arbore octetorum ubi octeti sunt manet, visio cocta eum sub sede
 * imprimit: ANTE liberum sedis quod tabula est et eum continet
 * (foster parenting), aliter POST liberos omnes (contentum post
 * '</body>'/'</html>' in body, contentum capitis post '</head>' in
 * head, fragmentum contextu tabulae; commentarium post body cum
 * contentum iam in body rediit). Absens = parens octetorum est
 * parens DOM. Index eius in textu, referentia et commentario I (post
 * 'tok'). */

/* Valores loci 'synthesis' (O7a, 2026-09-15): elementum a spec HTML5
 * FICTUM, lexemata nulla - involucra html/head/body et partes
 * tabulae tbody/tr/colgroup; O7c (2026-09-16): p quod '</p>' sine p
 * in scopo fingit et br quod '</br>' fingit (clausura vera in ficto
 * cadit). Titulus per html_arbor_synthesis_titulus (html_arbor.h).
 * NULLA numquam scribitur: locus absens = elementum verum
 * (tok_apertura eius). APPENDUNTUR solum (index in STML scriptus). */
nomen enumeratio {
    HTML_SYNTHESIS_NULLA = 0,
    HTML_SYNTHESIS_RADIX,             /* html */
    HTML_SYNTHESIS_CAPUT,             /* head */
    HTML_SYNTHESIS_CORPUS,            /* body */
    HTML_SYNTHESIS_CORPUS_TABULAE,    /* tbody */
    HTML_SYNTHESIS_ORDO,              /* tr */
    HTML_SYNTHESIS_COLUMNAE,          /* colgroup */
    HTML_SYNTHESIS_PARAGRAPHUS,       /* p  (O7c) */
    HTML_SYNTHESIS_FRACTURA,          /* br (O7c) */
    HTML_SYNTHESIS_NUMERUS
} HtmlSynthesis;

nomen enumeratio {
    HTML_ATTRIBUTUM_TOK_NOMEN = 0,
    HTML_ATTRIBUTUM_TOK_AEQUALE,
    HTML_ATTRIBUTUM_TOK_VALOR
} HtmlLocusAttributi;

nomen enumeratio {
    HTML_TEXTUS_TOK = 0,
    HTML_TEXTUS_SEDES
} HtmlLocusTextus;

nomen enumeratio {
    HTML_REFERENTIA_TOK = 0,
    HTML_REFERENTIA_SEDES
} HtmlLocusReferentiae;

nomen enumeratio {
    HTML_CRUDUS_TOK = 0
} HtmlLocusCrudi;

nomen enumeratio {
    HTML_COMMENTARIUM_TOK = 0,
    HTML_COMMENTARIUM_SEDES
} HtmlLocusCommentarii;

nomen enumeratio {
    HTML_CDATA_TOK = 0
} HtmlLocusCdata;

nomen enumeratio {
    HTML_MALUM_TOKENS = 0
} HtmlLocusMali;

nomen enumeratio {
    HTML_SPATII_OMISSI_TOKENS = 0
} HtmlLocusSpatiiOmissi;

#endif /* HTML_REGISTRUM_H */
