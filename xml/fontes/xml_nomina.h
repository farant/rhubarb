/* xml_nomina.h - Characteres et tituli XML (XML 1.0 ed. V, par. 2.2,
 * 2.3)
 *
 * Editio V NameStartChar / NameChar ut INDICEM FIXUM intervallorum
 * definit, a proprietatibus Unicode consulto independentem (editiones
 * I-IV classes Appendicis B adhibebant) - ergo tabula parva manu
 * scripta hic, super decodationem utf8.h; nihil ex runae.
 *
 * Purum: nulla allocatio, nullus status; totale (runa < 0 aut >
 * U+10FFFF = FALSUM ubique).
 *
 *   Char          #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD]
 *                 | [#x10000-#x10FFFF]
 *   NameStartChar ":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] |
 *                 [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] |
 *                 [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F]
 *                 | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF]
 *                 | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
 *   NameChar      NameStartChar | "-" | "." | [0-9] | #xB7 |
 *                 [#x0300-#x036F] | [#x203F-#x2040]
 *   PubidChar     #x20 | #xD | #xA | [a-zA-Z0-9] |
 *                 [-'()+,./:=?;!*#@$_%]
 *   S             (#x20 | #x9 | #xD | #xA)+
 */

#ifndef XML_NOMINA_H
#define XML_NOMINA_H

#include "latina.h"

/* Char (par. 2.2) */
b32
xml_runa_licita (
    s32 runa);

/* NameStartChar */
b32
xml_runa_initialis (
    s32 runa);

/* NameChar */
b32
xml_runa_tituli (
    s32 runa);

/* PubidChar */
b32
xml_runa_pubid (
    s32 runa);

/* S (runa una) */
b32
xml_spatium (
    s32 runa);

/* Longitudo in octetis tituli (Name) qui ab octetis[0] incipit: 0 si
 * runa prima NameStartChar non est (aut UTF-8 invalida). Nmtoken
 * (nmtoken VERUM): NameChar quaevis initio. Sequentia UTF-8 invalida
 * titulum finit. */
s32
xml_titulum_metiri (
    constans character* octeti,
                   s32  mensura,
                   b32  nmtoken);

#endif /* XML_NOMINA_H */
