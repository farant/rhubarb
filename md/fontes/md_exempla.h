/* md_exempla.h - Lector exemplorum spec.txt (CommonMark et GFM)
 *
 * Forma: linea XXXII gravium + ' example', fons markdown, linea '.',
 * html exspectatum, linea XXXII gravium. Tabulae ut '→' (U+2192)
 * scriptae - hic in '\t' vertuntur (spec_tests.py idem facit). Sectio
 * = titulus '## ' aut '# ' proximus supra. Lector UNUS ambabus
 * plagulis (md/probationes/fixa/FONTES.md).
 */

#ifndef MD_EXEMPLA_H
#define MD_EXEMPLA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen structura {
       i32 numerus;    /* I-basatus, ordine plagulae */
    chorda sectio;     /* titulus sectionis (sine '## '), visus in fontem */
    chorda markdown;   /* fons exempli, copia in piscina, '→' -> '\t' */
    chorda html;       /* html exspectatum, item */
       i32 linea;      /* linea limitis aperientis, I-basata */
    chorda extensio;   /* GFM: verbum post 'example' ("table", "strikethrough"
                        * ...), vacua in exemplis nucleis */
} MdExemplum;

/* Xar de MdExemplum (per valorem). NIHIL = memoria deficit. Limes
 * apertus sine clausura: exemplum omittitur (numerus non crescit). */
Xar*
md_exempla_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* MD_EXEMPLA_H */
