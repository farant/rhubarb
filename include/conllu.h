/* conllu.h - Lector CoNLL-U: treebanks Universal Dependencies
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
 *
 * Origo: oratio/fontes/oratio_conllu (oratio T13, T38 a), in ordinem
 * domus translatum 2026-10-17 (knotapel D130, decretum 4 a): mores
 * idem, praefixum oratio_ deletum, vitium proprium (ConlluVitium),
 * caput numeratum, proprietates FEATS, arbor dependentiae.
 */

#ifndef CONLLU_H
#define CONLLU_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "dependentia.h"

nomen structura {
                   i32  linea;   /* I-basata */
    constans character* causa;
} ConlluVitium;

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
       s32 caput;          /* HEAD numeratum; -1 si '_' aut non numerus */
} ConlluLexema;

nomen structura {
    chorda  id;          /* '# sent_id = ...' (vacua si absens) */
    chorda  textus;      /* '# text = ...' (vacua si absens) */
       Xar* lexemata;   /* ConlluLexema ordine plagulae (rangae inclusae) */
       i32  linea;       /* linea primae lineae sententiae */
    /* '# newdoc id = ...' ultimum visum ante aut in sententia (vacua si
     * nullum) - forma declarata per documentum (Perseus PHI) */
    chorda documentum;
} ConlluSententia;

/* Fontem legere: Xar de ConlluSententia (sententiae vacuae omissae).
 * NIHIL = vitium (linea, causa); causa "memoria" si allocatio fallit
 * (vitium semper ponitur cum NIHIL redditur, nisi vitium NIHIL). */
Xar*
conllu_legere (
         Piscina* piscina,
          chorda  fons,
    ConlluVitium* vitium);

/* Textus sententiae: '# text' si adest, aliter ex formis
 * reconstructus (spatium post lexema nisi SpaceAfter=No; rangae
 * superficiem dant, verba earum omittuntur). Copia in piscina. */
chorda
conllu_textus (
                     Piscina* piscina,
    constans ConlluSententia* sententia);

/* An campus MISC 'SpaceAfter=No' ferat. */
b32
conllu_spatium_post (
    chorda misc);

/* FEATS: valor clavis ("Case" -> "Nom" ex "Case=Nom|Number=Sing");
 * vacua si clavis absens aut campus '_'. Clavis tota congruere debet
 * ("Case" non "Case[psor]"). */
chorda
conllu_proprietas (
                chorda  feats,
    constans character* clavis);

nomen structura {
          DependentiaArbor  arbor;
    constans ConlluLexema** verba;   /* verba[k - 1] = lexema verbi k */
} ConlluArbor;

/* Arbor verborum syntacticorum (rangae omissae; nodi vacui iam omissi):
 * capita ex HEAD (caput -1 -> caput extra numerum, ut
 * dependentia_validare id reiciat). FALSUM si ID verborum non 1..n
 * consecutivi aut memoria; arbor NON validatur. */
b32
conllu_arbor (
                     Piscina* piscina,
    constans ConlluSententia* sententia,
                 ConlluArbor* exitus);

#endif /* CONLLU_H */
