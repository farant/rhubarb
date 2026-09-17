/* crusta_registrum_coctum.c
 *
 * Registrum generum COCTUM grammaticae 'crusta' - GENERATUM, NE MANU
 * EDITES. Fons: crusta/grammatica/crusta.registrum.stml
 * (materia/coquere.sh). Series LOCORUM plana; quodque genus fenestram
 * suam per loci_offset + loci_numerus nominat. Genera L, loci CLXXVI.
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

    /* functio (23..28) */
    { "tok_verbum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "interiecta", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "corpus", (s32)MATERIA_LOCUS_NODUS },

    /* grex (29..32) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* crustula (33..36) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* conditio (37..43) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "probatio", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_deinde", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "rami", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* ramus (44..47) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "probatio", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_deinde", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* iteratio (48..55) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "interiecta", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_in", (s32)MATERIA_LOCUS_TOKEN },
    { "verba", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "separator", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cursus", (s32)MATERIA_LOCUS_NODUS },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* cyclus (56..62) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_parenthesis", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_parenthesis_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "separator", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cursus", (s32)MATERIA_LOCUS_NODUS },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* repetitio (63..66) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "probatio", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "cursus", (s32)MATERIA_LOCUS_NODUS },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* cursus (67..69) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* electio (70..76) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "verbum", (s32)MATERIA_LOCUS_NODUS },
    { "interiecta", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_in", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* optio (77..81) */
    { "tok_parenthesis", (s32)MATERIA_LOCUS_TOKEN },
    { "exemplaria", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_parenthesis_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_terminator", (s32)MATERIA_LOCUS_TOKEN },

    /* arithmetica (82..87) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "post_aperturam", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "post_expressionem", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* iudicium (88..93) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "post_aperturam", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "post_expressionem", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "redirectiones", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* socius (94..96) */
    { "tok_verbum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "imperium", (s32)MATERIA_LOCUS_NODUS },

    /* verbum (97..97) */
    { "partes", (s32)MATERIA_LOCUS_LISTA_NODUS },

    /* pars-litteralis (98..98) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-effugium (99..99) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-continuatio (100..100) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-simplex (101..101) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-gemina (102..104) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "partes", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-effugia (105..105) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-versa (106..108) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "partes", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-parametrum (109..110) */
    { "tok_sigillum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-expansio (111..117) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_praefixum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_subscriptum", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "argumenta", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-substitutio (118..120) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-arithmetica (121..125) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "post_aperturam", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "post_expressionem", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-processus (126..128) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* pars-domus (129..129) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* numerus (130..130) */
    { "tok", (s32)MATERIA_LOCUS_TOKEN },

    /* variabilis (131..132) */
    { "tok_titulus", (s32)MATERIA_LOCUS_TOKEN },
    { "tok_subscriptum", (s32)MATERIA_LOCUS_TOKEN },

    /* praeposita (133..135) */
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "post_signum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "operandum", (s32)MATERIA_LOCUS_NODUS },

    /* postposita (136..138) */
    { "operandum", (s32)MATERIA_LOCUS_NODUS },
    { "post_operandum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },

    /* binaria (139..143) */
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "post_sinistrum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "post_signum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* ternaria (144..152) */
    { "probatio", (s32)MATERIA_LOCUS_NODUS },
    { "post_probationem", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_quaestio", (s32)MATERIA_LOCUS_TOKEN },
    { "post_quaestionem", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "post_sinistrum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_colon", (s32)MATERIA_LOCUS_TOKEN },
    { "post_colon", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* inclusa (153..157) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "post_aperturam", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "post_expressionem", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },

    /* iudicium-praeposita (158..160) */
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "post_signum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "operandum", (s32)MATERIA_LOCUS_NODUS },

    /* iudicium-binaria (161..165) */
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "post_sinistrum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "post_signum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* iudicium-coniuncta (166..170) */
    { "sinister", (s32)MATERIA_LOCUS_NODUS },
    { "post_sinistrum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_operator", (s32)MATERIA_LOCUS_TOKEN },
    { "post_signum", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "dexter", (s32)MATERIA_LOCUS_NODUS },

    /* iudicium-inclusa (171..175) */
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "post_aperturam", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "expressio", (s32)MATERIA_LOCUS_NODUS },
    { "post_expressionem", (s32)MATERIA_LOCUS_LISTA_NODUS },
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
    { "functio", (i32)23, (i32)6 },
    { "grex", (i32)29, (i32)4 },
    { "crustula", (i32)33, (i32)4 },
    { "conditio", (i32)37, (i32)7 },
    { "ramus", (i32)44, (i32)4 },
    { "iteratio", (i32)48, (i32)8 },
    { "cyclus", (i32)56, (i32)7 },
    { "repetitio", (i32)63, (i32)4 },
    { "cursus", (i32)67, (i32)3 },
    { "electio", (i32)70, (i32)7 },
    { "optio", (i32)77, (i32)5 },
    { "arithmetica", (i32)82, (i32)6 },
    { "iudicium", (i32)88, (i32)6 },
    { "socius", (i32)94, (i32)3 },
    { "verbum", (i32)97, (i32)1 },
    { "pars-litteralis", (i32)98, (i32)1 },
    { "pars-effugium", (i32)99, (i32)1 },
    { "pars-continuatio", (i32)100, (i32)1 },
    { "pars-simplex", (i32)101, (i32)1 },
    { "pars-gemina", (i32)102, (i32)3 },
    { "pars-effugia", (i32)105, (i32)1 },
    { "pars-versa", (i32)106, (i32)3 },
    { "pars-parametrum", (i32)109, (i32)2 },
    { "pars-expansio", (i32)111, (i32)7 },
    { "pars-substitutio", (i32)118, (i32)3 },
    { "pars-arithmetica", (i32)121, (i32)5 },
    { "pars-processus", (i32)126, (i32)3 },
    { "pars-domus", (i32)129, (i32)1 },
    { "numerus", (i32)130, (i32)1 },
    { "variabilis", (i32)131, (i32)2 },
    { "praeposita", (i32)133, (i32)3 },
    { "postposita", (i32)136, (i32)3 },
    { "binaria", (i32)139, (i32)5 },
    { "ternaria", (i32)144, (i32)9 },
    { "inclusa", (i32)153, (i32)5 },
    { "iudicium-praeposita", (i32)158, (i32)3 },
    { "iudicium-binaria", (i32)161, (i32)5 },
    { "iudicium-coniuncta", (i32)166, (i32)5 },
    { "iudicium-inclusa", (i32)171, (i32)5 },
};

constans MateriaRegistrumCoctum CRUSTA_REGISTRUM = {
    GENERA_COCTA,
    (i32)(magnitudo(GENERA_COCTA) / magnitudo(GENERA_COCTA[0])),
    LOCI_COCTI,
    (i32)(magnitudo(LOCI_COCTI) / magnitudo(LOCI_COCTI[0]))
};
