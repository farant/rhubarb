/* xml_arbor.h - Aedificator xml: lexemata -> arbor materiae
 *
 * ITERATIVUS (acervus graduum; nulla recursio - profunditas elementorum
 * sine limite acervi C). Totalis (X6): omnis octetus in arbore; quod
 * grammatica ponere non potest 'malum' fit (lexemata contigua eiusdem
 * loci in malum UNUM), et aedificator pergit - ergo omnia errata
 * relata, non primum. emissio(parsatio(x)) == x pro omni x.
 *
 * LEX ORDINIS LOCORUM: locus posterior scriptus locum priorem claudit -
 * lexema quod post 'pars_interna' doctypi aut post tag finalem
 * advenit nodum non intrat (emissio ordine locorum it) sed nodum
 * claudit et in parente ponitur.
 *
 * ELEMENTA: '<titulus' elementum aperit (statim parenti appenditur);
 * attributa; '>' contentum aperit, '/>' elementum claudit. '</titulus'
 * elementum apertum EIUSDEM tituli quaerit in acervo: summum - tag
 * finalis; profundius - elementa intermedia sine tag finali clauduntur
 * (diagnosticum 'elementum/tag-finalis-deest', WFC: Element Type
 * Match); nullum - tag finalis solus in malum
 * ('elementum/tag-finalis-solus'). Tag initialis sine '>' (lexema
 * structurale sequitur): elementum APERTUM manet, absentia '>' ex
 * declaratione derivatur. Finis fontis: elementa aperta tag finali
 * carent (diagnosticum).
 *
 * ORDO DOCUMENTI (post aedificationem, in liberis documenti): elementum
 * radicis unum ('documentum/radix-deest', 'documentum/radix-altera');
 * doctype unum ante radicem ('documentum/doctype'); textus extra
 * radicem spatia sola ('documentum/textus'); referentia et cdata extra
 * radicem ('documentum/contentum').
 *
 * LIGATOR (trivia): SPATIA omnia ANTE lexematis sequentis (cauda FINIS
 * reliqua fert). Trivia solum intra tags et declarationes nascuntur
 * (lector), ergo regula una sufficit.
 */

#ifndef XML_ARBOR_H
#define XML_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_nodus.h"

/* codices diagnosticorum aedificatoris (MateriaDiagnosticum.codex) */
#define XML_CODEX_TAG_FINALIS_DEEST   "elementum/tag-finalis-deest"
#define XML_CODEX_TAG_FINALIS_SOLUS   "elementum/tag-finalis-solus"
#define XML_CODEX_TAG_FINALIS_FINIS   "elementum/tok_clausura_finis"
#define XML_CODEX_RADIX_DEEST         "documentum/radix-deest"
#define XML_CODEX_RADIX_ALTERA        "documentum/radix-altera"
#define XML_CODEX_DOCTYPE_LOCUS       "documentum/doctype"
#define XML_CODEX_TEXTUS_EXTRA        "documentum/textus"
#define XML_CODEX_CONTENTUM_EXTRA     "documentum/contentum"

nomen structura {
    Xar* diagnostica;          /* MateriaDiagnosticum emissa (ordine
                                * inventionis); derivata (absentiae,
                                * mala) NON hic - materia_diagnostica */
    i32 mala;                 /* nodi malum */
    i32 profunditas_maxima;   /* acervus graduum */
    b32 sana;                 /* nulla emissa, nulla mala */
} XmlParsura;

/* Fragmenta (textus substitutionis entitatum, spec par. V):
 *   XML_FRAGMENTUM_CONTENTUM      entitas generalis in contento:
 *                                 documentum cuius liberi contentum
 *                                 sunt; ordo documenti NON iudicatur
 *                                 (textus et elementa plura licent);
 *                                 tag finalis solus aut tag non clausus
 *                                 = contentum non aequilibratum
 *   XML_FRAGMENTUM_PARS_INTERNA   entitas parametri inter
 *                                 declarationes: nodus pars-interna
 *                                 (sine '[' et ']') cuius liberi
 *                                 declarationes sunt */
nomen enumeratio {
    XML_FRAGMENTUM_CONTENTUM = 0,
    XML_FRAGMENTUM_PARS_INTERNA
} XmlFragmentum;

MateriaNodus*
xml_arbor_parsare_fragmentum (
                Piscina* piscina,
     constans character* fons,
                    s32  mensura,
          XmlFragmentum  genus,
             XmlParsura* relatio);

/* fons NON copiatur (lexemata in eum monstrant); relatio NIHIL licet.
 * NIHIL = memoria deficit. */
MateriaNodus*
xml_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   s32  mensura,
            XmlParsura* relatio);

#endif /* XML_ARBOR_H */
