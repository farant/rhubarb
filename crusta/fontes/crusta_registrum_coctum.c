/* crusta_registrum_coctum.c
 *
 * Registrum generum COCTUM grammaticae 'crusta' - GENERATUM, NE MANU
 * EDITES. Fons: crusta/grammatica/crusta.registrum.stml
 * (materia/coquere.sh). Series LOCORUM plana; quodque genus fenestram
 * suam per loci_offset + loci_numerus nominat. Genera L, loci CL.
 */

#include "crusta_registrum_coctum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_COCTI[] = {
    /* programma (0..1) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cauda", (s32)MATERIA_LOCUS_TOKEN },

    /* separator (2..2) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* malum (3..3) */
    { "tokens", (s32)MATERIA_LOCUS_LISTA_TOKEN },

    /* heredoc (4..6) */
    { "partes", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_delimitator", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_finis", (s32)MATERIA_LOCUS_TOKEN },

    /* catena (7..7) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* pipa (8..9) */
    { "praefixa", (s32)MATERIA_LOCUS_LISTA_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* operator (10..10) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* imperium (11..11) */
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* assignatio (12..15) */
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_subscriptum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "valor", (s32)MATERIA_LOCUS_NODUS },

    /* tabulatum (16..18) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* redirectio (19..22) */
    { "tok_fd", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "scopus", (s32)MATERIA_LOCUS_NODUS },
    { "corpus", (s32)MATERIA_LOCUS_REFERENTIA },

    /* functio (23..27) */
    { "tok_verbum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "corpus", (s32)MATERIA_LOCUS_NODUS },

    /* grex (28..31) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* crustula (32..35) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* conditio (36..42) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "probatio", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_deinde", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "rami", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* ramus (43..46) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "probatio", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_deinde", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* iteratio (47..53) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_in", (s32)MATERIA_LOCUS_TOKEN },
    { "verba", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_separator", (s32)MATERIA_LOCUS_TOKEN },
    { "cursus", (s32)MATERIA_LOCUS_NODUS },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* cyclus (54..60) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_parenthesis", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_parenthesis_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_separator", (s32)MATERIA_LOCUS_TOKEN },
    { "cursus", (s32)MATERIA_LOCUS_NODUS },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* repetitio (61..64) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "probatio", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cursus", (s32)MATERIA_LOCUS_NODUS },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* cursus (65..67) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* electio (68..73) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "verbum", (s32)MATERIA_LOCUS_NODUS },
    { "tok_in", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* optio (74..78) */
    { "tok_parenthesis", (s32)MATERIA_LOCUS_TOKEN },
    { "exemplaria", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_parenthesis_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_terminator", (s32)MATERIA_LOCUS_TOKEN },

    /* arithmetica (79..82) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* iudicium (83..86) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* socius (87..89) */
    { "tok_verbum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "imperium", (s32)MATERIA_LOCUS_NODUS },

    /* verbum (90..90) */
    { "partes", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* pars-litteralis (91..91) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-effugium (92..92) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-continuatio (93..93) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-simplex (94..94) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-gemina (95..97) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "partes", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-effugia (98..98) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-versa (99..101) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "partes", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-parametrum (102..103) */
    { "tok_sigillum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-expansio (104..110) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_praefixum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_subscriptum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "argumenta", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-substitutio (111..113) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-arithmetica (114..116) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-processus (117..119) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-domus (120..120) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* numerus (121..121) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* variabilis (122..123) */
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_subscriptum", (s32)MATERIA_LOCUS_TOKEN },

    /* praeposita (124..125) */
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "operandum", (s32)MATERIA_LOCUS_NODUS },

    /* postposita (126..127) */
    { "operandum", (s32)MATERIA_LOCUS_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },

    /* binaria (128..130) */
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* ternaria (131..135) */
    { "probatio", (s32)MATERIA_LOCUS_NODUS },
    { "tok_quaestio", (s32)MATERIA_LOCUS_TOKEN },
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "tok_colon", (s32)MATERIA_LOCUS_TOKEN },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* inclusa (136..138) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* iudicium-praeposita (139..140) */
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "operandum", (s32)MATERIA_LOCUS_NODUS },

    /* iudicium-binaria (141..143) */
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* iudicium-coniuncta (144..146) */
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* iudicium-inclusa (147..149) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
};

hic_manens constans MateriaTabGenus GENERA_COCTA[] = {
    /* titulus, offset, numerus */
    { "programma", (i32)0, (i32)2 },
    { "separator", (i32)2, (i32)1 },
    { "malum", (i32)3, (i32)1 },
    { "heredoc", (i32)4, (i32)3 },
    { "catena", (i32)7, (i32)1 },
    { "pipa", (i32)8, (i32)2 },
    { "operator", (i32)10, (i32)1 },
    { "imperium", (i32)11, (i32)1 },
    { "assignatio", (i32)12, (i32)4 },
    { "tabulatum", (i32)16, (i32)3 },
    { "redirectio", (i32)19, (i32)4 },
    { "functio", (i32)23, (i32)5 },
    { "grex", (i32)28, (i32)4 },
    { "crustula", (i32)32, (i32)4 },
    { "conditio", (i32)36, (i32)7 },
    { "ramus", (i32)43, (i32)4 },
    { "iteratio", (i32)47, (i32)7 },
    { "cyclus", (i32)54, (i32)7 },
    { "repetitio", (i32)61, (i32)4 },
    { "cursus", (i32)65, (i32)3 },
    { "electio", (i32)68, (i32)6 },
    { "optio", (i32)74, (i32)5 },
    { "arithmetica", (i32)79, (i32)4 },
    { "iudicium", (i32)83, (i32)4 },
    { "socius", (i32)87, (i32)3 },
    { "verbum", (i32)90, (i32)1 },
    { "pars-litteralis", (i32)91, (i32)1 },
    { "pars-effugium", (i32)92, (i32)1 },
    { "pars-continuatio", (i32)93, (i32)1 },
    { "pars-simplex", (i32)94, (i32)1 },
    { "pars-gemina", (i32)95, (i32)3 },
    { "pars-effugia", (i32)98, (i32)1 },
    { "pars-versa", (i32)99, (i32)3 },
    { "pars-parametrum", (i32)102, (i32)2 },
    { "pars-expansio", (i32)104, (i32)7 },
    { "pars-substitutio", (i32)111, (i32)3 },
    { "pars-arithmetica", (i32)114, (i32)3 },
    { "pars-processus", (i32)117, (i32)3 },
    { "pars-domus", (i32)120, (i32)1 },
    { "numerus", (i32)121, (i32)1 },
    { "variabilis", (i32)122, (i32)2 },
    { "praeposita", (i32)124, (i32)2 },
    { "postposita", (i32)126, (i32)2 },
    { "binaria", (i32)128, (i32)3 },
    { "ternaria", (i32)131, (i32)5 },
    { "inclusa", (i32)136, (i32)3 },
    { "iudicium-praeposita", (i32)139, (i32)2 },
    { "iudicium-binaria", (i32)141, (i32)3 },
    { "iudicium-coniuncta", (i32)144, (i32)3 },
    { "iudicium-inclusa", (i32)147, (i32)3 },
};

constans MateriaRegistrumCoctum CRUSTA_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};
