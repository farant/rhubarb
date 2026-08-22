/* silva_canon_coquere.h - Proiectio grammaticae in canonem STML
 *
 * Soror silva_coquere: illa grammaticam in tabulas C coquit, haec
 * eandem grammaticam in SCHEMA documentorum arboris coquit. Fons
 * unus, exitus duo - ergo schema cum motore putrescere non potest.
 *
 * QUID PROICITUR: vocabularium generum et locorum cum CONTENTO
 * derivato (silva_gen_impletiones_computare: quae genera quem locum
 * implere possint). Involucrum documenti - parsura, fontes, cauda,
 * trivia, origines - derivari NEQUIT: forma scriptoris est, non
 * grammaticae, ergo manu scribitur et porta corporis eam falsificat.
 *
 * Consilium: project-specs/arbor-canon-spec.md par. 4.
 */

#ifndef SILVA_CANON_COQUERE_H
#define SILVA_CANON_COQUERE_H

#include "latina.h"
#include "silva_generare.h"

/* Canonem in <via> scribere.
 *
 * dialectus:       nomen dialecti in involucrum scriptum ("c89").
 * via_grammaticae: pro commento GENERATUM (unde regenerare).
 *
 * Reddit FALSUM si plagula scribi non potest aut si grammatica
 * vocabularium radicis non praebet (clamat, non tacet). */
b32
silva_gen_canonem_scribere (
    SilvaGenGrammatica* grammatica,
    constans character* dialectus,
    constans character* via,
    constans character* via_grammaticae);

#endif /* SILVA_CANON_COQUERE_H */
