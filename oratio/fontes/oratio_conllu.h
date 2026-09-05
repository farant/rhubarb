/* oratio_conllu.h - Lector CoNLL-U (T13): treebanks UD ut oracula
 *
 * Forma (universaldependencies.org/format): linea lexematis = X campi
 * tabulatis divisi (ID FORM LEMMA UPOS XPOS FEATS HEAD DEPREL DEPS
 * MISC), commentaria '#' (sent_id, text, ...), linea vacua sententiam
 * claudit. ID: numerus, RANGA 'a-b' (lexema superficiei cum verbis
 * scissis a..b sequentibus, encliticum: pronumque = pronum + que; UPOS
 * '_'), NODUS VACUUS 'a.b' (ellipsis: OMITTITUR). Linea cum campis
 * non X aut ID malo lectionem SISTIT cum linea (vitium). Chordae
 * fontem referunt, non copiantur. '_' in campo = absens, ut in fonte
 * (non vertitur).
 */

#ifndef ORATIO_CONLLU_H
#define ORATIO_CONLLU_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_vocabularium.h"

nomen structura {
       i32 a;              /* ID (aut initium rangae) */
       i32 b;              /* finis rangae (== a si simplex) */
       b32 ranga;          /* VERUM = linea 'a-b' (superficies), verba sequuntur */
    chorda forma;
    chorda lemma;
    chorda upos;
    chorda xpos;
    chorda feats;
    chorda head;
    chorda deprel;
    chorda deps;
    chorda misc;
       b32 spatium_post;   /* FALSUM si MISC 'SpaceAfter=No' */
       i32 linea;          /* plagulae CoNLL-U */
} OratioConlluLexema;

nomen structura {
    chorda  id;          /* '# sent_id = ...' (vacua si absens) */
    chorda  textus;      /* '# text = ...' (vacua si absens) */
       Xar* lexemata;   /* OratioConlluLexema ordine plagulae (rangae inclusae) */
       i32  linea;       /* linea primae lineae sententiae */
} OratioConlluSententia;

/* Fontem legere: Xar de OratioConlluSententia (sententiae vacuae
 * omissae). NIHIL = vitium (plagula "conllu", linea, causa) aut memoria. */
Xar*
oratio_conllu_legere (
                     Piscina* piscina,
                      chorda  fons,
    OratioVocabulariumVitium* vitium);

/* Textus sententiae: '# text' si adest, aliter ex formis
 * reconstructus (spatium post lexema nisi SpaceAfter=No; rangae
 * superficiem dant, verba earum omittuntur). Copia in piscina. */
chorda
oratio_conllu_textus (
                           Piscina* piscina,
    constans OratioConlluSententia* sententia);

/* An campus MISC 'SpaceAfter=No' ferat. */
b32
oratio_conllu_spatium_post (
    chorda misc);

#endif /* ORATIO_CONLLU_H */
