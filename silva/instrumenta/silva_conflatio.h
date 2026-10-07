/* silva_conflatio.h - Plagula C UNA ex clausura (dev-time; extractum
 * ex briar_amalgama 2026-10-07)
 *
 * Mechanismus PURUS: plagulae (via + contentum) et tabula staticorum
 * datae, textus redditur - nihil legit nec scribit. Capita ordine
 * dependentiae (profunditate prima, post-ordine; postulata_posix.h
 * primum), fontes bibliothecarum cum staticis PER PLAGULAM renominatis
 * ('#define s s_<stirps>' ante, '#undef s' post; macra plagulae #undef
 * solum), inclusiones locales lineis vacuis substitutae (numeri
 * linearum servati), '#line 1 "via"' plagulam quamque aperit.
 *
 * Consumptores: briar -amalgama (briar_amalgama.c: membra, plagulae
 * genitae, prooemium, recusationes, scriptor), knotapel/archive.sh
 * (principalia/conflator.c). Tabula staticorum = corpus.symbola.tsv
 * (tools/corpus_infixum.sh, ex nexu silvae).
 *
 * USUS:
 *   ConflatioContextus c;
 *   c.radices_clientium = SILEX_RADICES_CLIENTIUM;
 *   (vacuum)conflatio_statica_legere(piscina, tabula, &c);
 *   capita = conflatio_capita_ordinare(piscina, &c, clausura);
 *   fontes = conflatio_fontes_ordinare(piscina, &c, clausura, capita);
 *   ... conflatio_plagulam_emittere / conflatio_fontem_emittere
 */

/* <aedilis corpus="silva/instrumenta/silva_conflatio.c"/> */
#ifndef SILVA_CONFLATIO_H
#define SILVA_CONFLATIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "xar.h"

/* plagula clausurae: via relativa ad radicem + contentum */
nomen structura {
    chorda via;
    chorda contentum;
} ConflatioPlagula;

/* staticum fontis: genus functio | variabile | typedef | constans |
 * macro (macro: '#undef' post solum, nullum '#define') */
nomen structura {
    chorda symbolum;
    chorda genus;
} ConflatioStaticum;

nomen structura {
    /* radices ubi caput et fons in eodem directorio habitant, cum '/'
     * finali, NIHIL-terminatae (SILEX_RADICES_CLIENTIUM); praeter
     * 'include/' + 'lib/' */
    constans character* constans* radices_clientium;
    /* via fontis -> Xar de ConflatioStaticum (conflatio_statica_legere)
     */
    TabulaDispersa* statica;
} ConflatioContextus;

/* linea inclusionis localis? ('#include "x.h"', spatia permissa);
 * nomen capitis redditum */
b32
conflatio_inclusio_localis (
    chorda  linea,
    chorda* nomen_capitis);

/* caput vendicatum: 'include/' aut radix clientis + '.h' */
b32
conflatio_caput_est (
    constans ConflatioContextus* contextus,
                         chorda  via);

/* fons vendicatus: 'lib/' aut radix clientis + '.c' */
b32
conflatio_fons_est (
    constans ConflatioContextus* contextus,
                         chorda  via);

/* textus tabulae (lineae 'symbolum\tgenus\tvia', '#' commentaria;
 * viae non fontium praetermissae) -> contextus->statica. FALSUM si
 * tabula creari nequit. Chordae in textu manent (non copiantur). */
b32
conflatio_statica_legere (
               Piscina* piscina,
                chorda  textus,
    ConflatioContextus* contextus);

/* clausura (Xar de ConflatioPlagula) -> capita ordine dependentiae:
 * Xar de (constans ConflatioPlagula*) */
Xar*
conflatio_capita_ordinare (
                        Piscina* piscina,
    constans ConflatioContextus* contextus,
                            Xar* clausura);

/* fontes: gemellus 'lib/<stirps>.c' cuiusque capitis post caput suum
 * (ordine capitum), reliqui ordine clausurae; Xar de (constans
 * ConflatioPlagula*) */
Xar*
conflatio_fontes_ordinare (
                        Piscina* piscina,
    constans ConflatioContextus* contextus,
                            Xar* clausura,
                            Xar* capita);

/* '#line 1 "via"' + textus, inclusiones locales lineis vacuis; linea
 * ultima semper '\n' terminata */
vacuum
conflatio_plagulam_emittere (
              ChordaAedificator* aedificator,
      constans ConflatioPlagula* plagula);

/* fons cum staticis suis (contextus->statica, suffixum = stirps viae:
 * 'lib/x.c' -> '_x') */
vacuum
conflatio_fontem_emittere (
              ChordaAedificator* aedificator,
    constans ConflatioContextus* contextus,
      constans ConflatioPlagula* plagula);

/* plagula cum staticis DATIS (Xar de ConflatioStaticum; NIHIL licet):
 * commentarium unius lineae 'via: nota' si statica adsunt, '#define s
 * s_suffixum' (macra praetermissa), plagula, '#undef s' omnium */
vacuum
conflatio_renominatam_emittere (
            ChordaAedificator* aedificator,
    constans ConflatioPlagula* plagula,
                          Xar* statica,
                       chorda  suffixum,
           constans character* nota);

/* 'lib/x.c' -> 'x' */
chorda
conflatio_stirps (
    chorda via);

#endif /* SILVA_CONFLATIO_H */
