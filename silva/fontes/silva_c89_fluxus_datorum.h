/* silva_c89_fluxus_datorum.h - Tabulae dataflow super CFG (FLUXUS-1
 * chunk A; specificatio project-specs/silva-fluxus-1-spec-v2.md)
 *
 * Extractio eventorum def/usus ordine aestimationis per blocum -
 * substratum puncti-fixi (gradus B), iudicii initiationis (gradus C,
 * codices 71/72/73) et catenarum def-usus (codex 62 re-arma,
 * decretum 01KXRBS0VQ). Codex-est-basis-datorum: tabulae RETENTAE,
 * quisque campus columna quaestionis futurae.
 *
 * FONS UNICUS: granula sententiarum (fluxus post chunk 0 -
 * conditiones quoque granula sunt). INVARIANTUM PRAETERMISSIONIS:
 * ambulator subtrees BINARIUM(&&,||)/TERNARIUS integros
 * praetermittit - operanda eorum in blocis praecedentibus granulata.
 *
 * PINNAE CALIBRATIONIS LIGANTES (spec §2): &x = eventum def
 * positione-fluxus (const T* parametrum = NULLUM eventum - s04f);
 * scriptio membri/elementi = DEFINITIO variabilis TOTAE (s19);
 * lectiones membrorum/elementorum aggregati invisibiles; magnitudo
 * praetermissa (s18); conversio ad vacuum IDENTIFICATORIS NUDI
 * (per parentheses) = nullum eventum (pinna s01c: (void)x non usus
 * - formae aliae, e.g. (void)f(x), normaliter ambulant); *p legit
 * p; p[i]/p->a legit p (monstrator scalaris); a[i]/a.b aggregati
 * invisibile praeter scriptionem (= def totius).
 *
 * SUTURA FACTORUM (exemplar XI-1): datorum semanticam numquam
 * includit - facta symbolorum per involucra vocatoris fluunt.
 */

#ifndef SILVA_C89_FLUXUS_DATORUM_H
#define SILVA_C89_FLUXUS_DATORUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_nodus.h"
#include "silva_c89_fluxus.h"

/* ==================================================
 * Eventa
 * ================================================== */

nomen enumeratio {
    FLUXUS_EVENTUM_USUS = 0,       /* lectio valoris */
    FLUXUS_EVENTUM_DEFINITIO,      /* scriptio (assignatio, initiator,
                                    * scriptio membri/elementi =
                                    * totius variabilis - pinna s19) */
    FLUXUS_EVENTUM_DEFINITIO_LOCI  /* &x contextu non-constanti
                                    * (idioma initialisationis C) */
} FluxusEventumGenus;

/* Eventum unum. variabilis = index densus in tabulam variabilium;
 * -1 = OMNES variabiles (folium ERROR/AMBIGUUS opacum: def-omnia,
 * abstentio conservativa). */
nomen structura {
    s32                  variabilis;
    s32                  genus;        /* FluxusEventumGenus */
    constans SilvaNodus* nodus;        /* sedes (diagnostica) */
    b32                  in_initiatore_proprio;  /* int x = x */
} FluxusEventum;

/* ==================================================
 * Tabulae
 * ================================================== */

/* Variabilis tracta (index in xar = index densus eventorum) */
nomen structura {
    constans vacuum*     identitas;    /* symbolum semanticae (opacum) */
    chorda               titulus;      /* nomen (nuntia gradus C) */
    constans SilvaNodus* declarans;    /* sedes declarationis */
    b32                  parametrum;   /* initiata in introitu */
} FluxusVariabilis;

/* Blocus parallelus (index = FluxusBlocus.index). Status
 * punctorum-fixorum (chunk B): verba i64 (numerus_verborum in
 * FluxusDatorum), bitum positum = variabilis initiata. may = unio
 * super praedecessores (semita ALIQUA initiavit), must = intersectio
 * (semitae OMNES). Introitus functionis: bita parametrorum sola.
 * Bloci inattingibiles numquam processi: may 0, must omnia-posita
 * (elementum summum) - contributio nulla, paritas s04d
 * structuralis. */
nomen structura {
    Xar* eventa;                       /* FluxusEventum (valore) */
    i64* may_introitus;
    i64* may_exitus;
    i64* must_introitus;
    i64* must_exitus;
} FluxusDatorumBlocus;

structura FluxusDatorum {
    constans FluxusFunctionis* fluxus; /* graphus fons */
    Xar* variabiles;                   /* FluxusVariabilis (valore) */
    Xar* bloci;                        /* FluxusDatorumBlocus (valore,
                                        * parallelus fluxus->bloci) */
    i32  numerus_verborum;             /* verba i64 per statum */
};

/* ==================================================
 * Sutura factorum symbolorum (politica vocatoris)
 * ================================================== */

/* Facta identificatoris/declaratoris resoluti - vocator (semantica
 * involucris, probationes resolutore manuali) implet. */
nomen structura {
    constans vacuum*     identitas;   /* symbolum (opacum, non NIHIL) */
    chorda               titulus;
    constans SilvaNodus* declarans;
    b32 localis_automata;   /* VARIABILE + profunditas>0 +
                             * !(STATICA|EXTERNA) = TRACTANDA */
    b32 parametrum;         /* initiata in introitu (tractatur) */
    b32 aggregatum;         /* structura/unio/acies: lectio membri
                             * invisibilis, scriptio = def totius */
    b32 acies;              /* acies: identificator positione valoris
                             * DECADIT (T[] -> T*) = eventum LOCI,
                             * non usus (contactus corporis primus:
                             * sprintf(tabula,...) - clang tacet,
                             * decasus = locus sumptus) */
} FluxusSymbolumFacta;

nomen structura {
    /* facta pro folio-identificatore AUT declaratore toto
     * (initiatus/nudus - nexus declaratorum semanticae);
     * FALSUM = non resolutum (nullum eventum emittitur) */
    b32 (*symbolum)(vacuum* contextus, constans SilvaNodus* nodus,
        FluxusSymbolumFacta* facta);
    /* estne parametrum positionis datae vocationis T* constans?
     * (VERUM => &x ibi NULLUM eventum - pinna s04f); functio_folium
     * = expressio functionis vocationis (folium nominis plerumque);
     * ignotum/variadicum/K&R = FALSUM (non-constans, silens) */
    b32 (*parametrum_constans)(vacuum* contextus,
        constans SilvaNodus* functio_folium, i32 index);
    /* estne typus expressionis acies? (accessus membri aciei
     * positione valoris DECADIT = LOCI totius - memset(r.c,...);
     * NIHIL = numquam acies, lectio membri invisibilis manet) */
    b32 (*expressio_acies)(vacuum* contextus,
        constans SilvaNodus* nodus);
    /* lectio canonica nodi AMBIGUI; NIHIL = identitas */
    constans SilvaNodus* (*canonicum)(vacuum* contextus,
        constans SilvaNodus* nodus);
    vacuum* contextus;
} FluxusDatorumAuxilia;

/* ==================================================
 * API
 * ================================================== */

/* Tabulas dataflow pro graphu uno aedificare. Piscina vocatoris
 * omnia possidet. Auxilia NIHIL non licent (sine factis nulla
 * extractio) - sed campi singuli NIHIL licent (symbolum NIHIL =
 * tabulae vacuae, robustum). Numquam NIHIL reddit nisi fluxus NIHIL.
 * Vocator monstratorem in FluxusFunctionis.datorum reponit. */
FluxusDatorum*
silva_c89_fluxus_datorum_aedificare (
    Piscina*                       piscina,
    constans FluxusFunctionis*     fluxus,
    constans FluxusDatorumAuxilia* auxilia);

#endif /* SILVA_C89_FLUXUS_DATORUM_H */
