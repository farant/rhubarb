/* toml_arbor.h - Aedificator toml: lexemata -> arbor materiae
 *
 * ITERATIVUS (acervus graduum; nulla recursio - profunditas
 * uncorum sine limite C acervi). Totalis (T4): omnis octetus in
 * arbore; quod grammatica ponere non potest 'malum' fit, et aedificator
 * ad lineam novam proximam (extra uncos) aut ad ',' / claudentem
 * (intra uncos) recuperat - ergo omnia errata relata, non primum.
 * emissio(parsatio(x)) == x pro omni x, valido aut invalido.
 *
 * MODUS ab aedificatore eligitur (lector functio modi et positionis):
 * CLAVIS in documento, capite, clave, tabula compacta, post valorem;
 * VALOR post '=' et intra seriem. Profunditas lectoris = series et
 * tabulae compactae apertae; sententia_aperta = sententia aperta (linea
 * nova vacua aut commentarii solius trivium est).
 *
 * CLAVIS STRICTA: segmentum (punctum segmentum)* - segmentum post
 * segmentum aut punctum post punctum clavem finit (deinde malum).
 * Punctum caudale ('a. = 1') in arbore manet; coctio iudicat.
 *
 * LIGATOR (trivia): post lineam novam terminantem aut in initio
 * documenti trivia omnia ANTE lexematis sequentis (commentarium supra
 * clavem clavis est); aliter usque ad lineam novam PRIMAM (inclusam)
 * POST prioris, reliqua ANTE sequentis ('a = 1 # c' commentum valoris
 * est). Vexillum initium_lineae per regulam lectoris STML (trivium
 * LINEA intervenit; LINEA_FINIS substantiva non).
 */

#ifndef TOML_ARBOR_H
#define TOML_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "materia_nodus.h"

nomen structura {
    i32 mala;                 /* nodi malum */
    i32 clausurae_absentes;   /* ] ]] } et chordae non clausae */
    i32 absentiae;            /* clavis, '=' aut valor absens */
    i32 profunditas_maxima;   /* acervus graduum */
    b32 sana;                 /* omnia tria nulla */
} TomlParsura;

/* fons NON copiatur (lexemata in eum monstrant); relatio NIHIL licet.
 * NIHIL = memoria deficit. */
MateriaNodus*
toml_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   s32  mensura,
           TomlParsura* relatio);

#endif /* TOML_ARBOR_H */
