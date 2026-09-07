/* briar_facies.h - Redditio litterata: arbor thistle -> pagina HTML
 * una, sui sufficiens (spec par. 4.6).
 *
 * Gemella fabricae in alteram partem versa: '-amalgama' plagulam in
 * unum .c pro compilatore complicat, '-html' eam in paginam pro
 * lectore explicat. Ambae eundem BriarFabricaFructus in memoria
 * consumunt; neutra clang currit, neutra proiectum scribit.
 *
 * Nihil novi computatur: grex fragmentorum cum usibus
 * (briar_contextus), textus contextus cum tabula linearum
 * (briar_nexus), classificatio lexematum (silva), declarationes
 * bibliothecae in semantica regionis - omnia iam parata.
 *
 * VESTIS UT PARAMETRUM: involucrum/styli/scriptum e capsula infixa
 * veniunt in binario, e literalibus in porta. Ergo modulus nihil
 * legit, nihil scribit (structurae intrant, chorda exit) et CSS
 * verum aurea portae movere NON potest.
 *
 * Lex paginae: SINE JavaScript legitur. Omnis nexus (fragmentum ->
 * definitio, definitio -> usus, symbolum domesticum -> sedes) est
 * ancora; unicum scriptum est tabella declarationum.
 */

#ifndef BRIAR_FACIES_H
#define BRIAR_FACIES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "briar_fabrica.h"

/* involucrum notas sex fert, quaeque semel substituta:
 * titulus, styli, caput, corpus, symbola, scriptum.
 * Nota absens = recusatio quae eam nominat, numquam silentium. */
nomen structura {
    chorda involucrum;  /* facies.html */
    chorda styli;       /* facies.css */
    chorda scriptum;    /* facies.js */
    chorda exemplar;    /* md/html/md-html.stml (programma md_html) */
} BriarVestis;

/* pagina una; vacua + causa si vestis vitiosa est */
chorda
briar_faciem_fingere (
                          Piscina* piscina,
              InternamentumChorda* intern,
                              Xar* nexus,       /* BriarNexusRes */
                              Xar* fragmenta,   /* BriarFragmentum */
     constans BriarFabricaFructus* fructus,
                           chorda  fons,        /* octeti thistle */
               constans character* via,
             constans BriarVestis* vestis,
                           chorda* causa);

#endif /* BRIAR_FACIES_H */
