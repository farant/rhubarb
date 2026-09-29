/* briar_silva.h - Genus interius quartum nexus: regiones C thistle
 * per silvam cum expansione. Unitas PROPRIA (non in briar_nexus.c):
 * caput amalgamae silvae enumerationes stml suas fert, ergo stml.h
 * et silva.h in eadem unitate translationis non coeunt - briar_nexus
 * stml tenet, briar_silva silvam; caput nexus tags solum praenuntiat.
 *
 * Praeludium textui regionis praepositum: '#include "latina.h"' +
 * trias stdio/stdlib/string (quae caput genitum quoque fert) (+
 * '#include "internuntius.h"' + 'hic_manens InternuntiusTractator
 * briar_tractator_exemplar;' si methodus= - exemplar contra quod
 * fabrica signaturam methodi probat, silva_c89_typi_compatibiles).
 * Capita clausurae e FONTE silicis per textum praebita (numquam
 * discus). Expansio obligatoria: parsura nuda C domesticum male legit
 * ('principale' macro -> 'main' post expansionem solum).
 *
 * INCLUSIONES DERIVATAE (v1: capita domus sola): parsura PRIMA cum
 * praeludio solo; symbola implicita (functiones et macra functionalia
 * sine declaratione) et typi nominati ignoti (diagnosticum
 * TYPUS_NOMINATUS_IGNOTUS) in tabula corpus.symbola.tsv (symbolum,
 * genus, caput - e fonte silicis) quaeruntur; capita inventa
 * praeludio adduntur (ordine alphabetico) et parsura SECUNDA fit.
 * Tabula absens = nulla derivatio. Symbolum in capitibus duobus =
 * recusatio (linea_erroris = linea tagi, causa nominat ambo).
 */

#ifndef BRIAR_SILVA_H
#define BRIAR_SILVA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "silex.h"
#include "tabula_dispersa.h"
#include "silva.h"
#include "briar_nexus.h"

/* symbolum cuius caput DERIVATUM est: par (titulus, caput). Plagula
 * inclusionem non scribit, ergo lector signum non habet quo sciat
 * unde nomen veniat - hoc par id reddit (facies par. 4.6 F5). */
nomen structura {
    chorda titulus;
    chorda caput;    /* 'piscina.h', sine 'include/' */
} BriarSymbolumDerivatum;

nomen structura BriarSilva {
             SilvaPiscina* piscina;     /* arena silvae (solvere!) */
    constans SilvaParsura* parsura;     /* NIHIL si parsura fracta */
           SilvaSemantica* semantica;   /* symbola + typi */
                      Xar* capita_derivata;   /* chorda, alphabetice */
                      Xar* symbola_derivata; /* pares derivati */
} BriarSilva;

/* Regiones 'c' (quocumque munere) parsare. Reddit numerum regionum
 * parsatarum; -I = memoria aut argumenta (s32: sentinela signata).
 * Regio cum erroribus parsurae: linea_erroris (linea .thistle mortis
 * GLR primae - lapide bugs/001) + causa (summarium + ordines locati
 * cum excerptis, forma silvae una: silva_mortes_scribere) in
 * BriarNexusRes; arbor tamen manet. via_documenti = via .thistle pro
 * ordinibus (NIHIL = "regio"). */
s32
briar_silvam_texere (
                Piscina* piscina,
                    Xar* nexus,
     constans SilexFons* fons,
     constans character* via_documenti);

/* MEMBRA VISIBILIA derivationi (spec par. 3.5, bibliotheca): nomen
 * publicum membri -> caput eius ("folium_regiones.h", in tabulam
 * symbolorum corporis mixtum; nomen quod corpus quoque habet
 * AMBIGUUM fit, ut duo capita corporis); caput -> textus parsurae
 * PROPRIUS (sine lineis, sine custode) quem parsura secunda in
 * praeludio pro inclusione ponit - silex caput genitum non invenit -
 * dependentibus eius primum, quoque SEMEL (rhombus). */
nomen structura {
    TabulaDispersa* nomina;   /* chorda nomen -> chorda* caput */
    TabulaDispersa* textus;   /* chorda caput -> chorda* textus
                               * PROPRIUS (sine custode, sine
                               * inclusionibus membrorum) */
    TabulaDispersa* ordo;     /* chorda caput -> Xar* (chorda capita
                               * dependentium, post-ordo) */
} BriarSilvaMembra;

/* ut briar_silvam_texere, membris visibilibus (NIHIL = nulla) */
s32
briar_silvam_texere_cum_membris (
                      Piscina* piscina,
                          Xar* nexus,
           constans SilexFons* fons,
           constans character* via_documenti,
    constans BriarSilvaMembra* membra);

/* nomen publicum regionis planae et linea .thistle declarationis */
nomen structura {
    chorda titulus;
       i32 linea;
} BriarNomenPublicum;

/* nomina PUBLICA regionum C planarum (briar_nexus_regio_plana) post
 * silvam: scopus fili, non statica, non implicita, non 'main', in
 * textu regionis ipso (praeludium exclusum); quodque semel. Xar de
 * BriarNomenPublicum. */
Xar*
briar_silva_nomina_publica (
    Piscina* piscina,
        Xar* nexus);

/* Parsura C per silvam cum lexico SYSTEMATIS (silva-migratio T16b):
 * capita clausurae (Xar de SilexRes, '.h' sola, praeter 'excludere' -
 * caput principale ipsum; NIHIL licet) contextui praebentur, et
 * lexicon systematis (silva/fontes/systema_c89.h + systema_posix.h e
 * fonte silicis - corpus infixum aut discus) per silva_lexicon
 * componitur, ut examen. Sine eo 'va_arg(va, T)' et 'offsetof(T, m)'
 * ERROR syntaxis erant (lapide bugs/009), FILE/size_t ignoti. Loca
 * TRIA parsurae briar (regiones, caput principale, symbola) hinc
 * pendent - contextus unus, numquam divergens. NIHIL = parsura
 * fracta; *causa (NIHIL licet) annotationem externa pravam nominat. */
SilvaParsura*
briar_silva_parsare (
            SilvaPiscina*  arboris,
                 Piscina*  piscina,
      constans SilexFons*  fons,
            constans Xar*  clausura,
      constans character*  excludere,
      constans character*  via,
      constans character*  textus,
                     i32   mensura,
      constans character** causa);

/* Caput corporis (via e.g. "include/sors.h") ut plagulam
 * PRINCIPALEM parsare: capita clausurae e fonte silicis praebita
 * (numquam discus directe), expansio obligatoria. *textus = textus
 * capitis ipse (extenta compendii in eum spectant). NIHIL si caput
 * abest, clausura fracta aut parsura nulla; errores parsurae in
 * parsura->numerus_errorum manent. Liberare:
 * briar_silvam_capitis_solvere. */
BriarSilva*
briar_silvam_capitis_texere (
               Piscina* piscina,
    constans SilexFons* fons,
    constans character* via,
                chorda* textus);

vacuum
briar_silvam_capitis_solvere (
    BriarSilva* silva);

/* arenas silvae destruere (res->silva deinde NIHIL) */
vacuum
briar_silvam_solvere (
    Xar* nexus);

#endif /* BRIAR_SILVA_H */
