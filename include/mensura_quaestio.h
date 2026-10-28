/*
 * mensura_quaestio.h - quaestiones nominatae super cursus (T5)
 *
 * Spec par. V: quaestiones cum parametris in C, NULLA lingua
 * quaestionum (regula canonis). Functiones purae super Xar de Cursus
 * (ex mensura_cursus_legere) et, pro actis, super introitus
 * diurnae - nec disci nec ambitus memores. 'bin/mensor quaestio NOMEN
 * par=valor' parametra legit, functionem vocat, ordines imprimit;
 * silva.cursus() involucrum tenue est.
 *
 * Quaestiones acceptationis (spec par. I) -> functiones:
 *   1 quo tempus cursus ivit          cursus_quaerere_filii
 *   2 qui generatores omni iudicio    cursus_quaerere_summa (nota
 *     iterum currunt                    'IUDICIUM')
 *   3 commissumne X tardius fecit     cursus_quaerere_tendentia
 *   4 quando frigida ultimo transiit,  cursus_quaerere_historia
 *     qua arbore, quo ramo             (status transiit, numerus I)
 *   5 quae scripta numquam currunt    cursus_quaerere_numquam
 *   6 quid X effudit cum ultimo       historia (status fractus,
 *     fractum est                       numerus I) + diurna_legere
 * Franus 10-13: cursus_quaerere_commissiones (tentamina, culpa).
 */

#ifndef MENSURA_QUAESTIO_H
#define MENSURA_QUAESTIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "mensura.h"
#include "diurna.h"

/* status ut mascula (filtrum) */
#define CURSUS_MASCULA_TRANSIIT     I
#define CURSUS_MASCULA_FRACTUS      II
#define CURSUS_MASCULA_INTERRUPTUS  IV
#define CURSUS_MASCULA_OMNIS        VII

/* filtrum commune: campi vacui / ZEPHYRUM = non filtrat */
nomen structura {
    chorda titulus;          /* aequalis */
    chorda praefixum;        /* titulus INCIPIT (sectiones membri:
                               * 'MEMBRUM#') */
    chorda arbor;
    chorda ramus;
    chorda nota;             /* nota INCIPIT hoc praefixo */
       i32 status;           /* mascula; ZEPHYRUM = omnis */
       s64 post_ms;          /* initium >= post_ms (ms ab epocha) */
       i32 numerus;          /* ordines maximi; ZEPHYRUM = omnes */
} CursusFiltrum;

/* index cursuum: per id, filii per parentem - semel structus */
nomen structura CursusIndex CursusIndex;

CursusIndex*
cursus_indicem_struere (
    constans Xar* cursus,      /* Xar de Cursus */
         Piscina* piscina);

/* cursus per id; NIHIL si ignotus */
constans Cursus*
cursus_invenire (
    constans CursusIndex* index,
                  chorda  id);

/* HISTORIA: cursus filtrati, RECENTISSIMI primum. Xar de (Cursus*) */
Xar*
cursus_quaerere_historia (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina);

/* TENDENTIA: duratio per commissum (cursus transeuntes tituli
 * filtrati), ordine apparitionis primae commissi. CLAVIS COMMISSI (T5,
 * ex datis veris): cursus intra commissionem transeuntem = commissum
 * quod ea PEPERIT (per_commissionem); cursus sordidus aliter =
 * 'parens+' (mutationes nondum commissae super parentem); mundus =
 * commissum HEAD. Sine hoc 'commissum X tardius' de mutatione
 * SEQUENTE loquebatur (portae eius super X currebant). */
nomen structura {
    chorda commissum;
       b32 per_commissionem;
       i32 numerus;
       s64 mediana_ms;
       s64 minima_ms;
       s64 maxima_ms;
       s64 primum;           /* initium cursus primi sub commisso */
} CursusTendentia;

Xar*
cursus_quaerere_tendentia (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina);

/* FILII: arbor cursus (praeordine), filii ordine durationis
 * descendentis; pars = millesimae durationis parentis. profunditas
 * maxima ZEPHYRUM = tota. */
nomen structura {
    constans Cursus* cursus;
                i32  profunditas;   /* radix ZEPHYRUM */
                i32  pars;          /* permille parentis; radix M */
} CursusNodus;

Xar*
cursus_quaerere_filii (
    constans CursusIndex* index,
                  chorda  id,
                     i32  profunditas_maxima,
                 Piscina* piscina);

/* SUMMA: per titulum (cursus filtrati) - numerus, ms summa, parentes
 * distincti (tituli). Ordine ms summae descendentis. Cum nota
 * 'IUDICIUM': regenerationes iudicis (quaestio II). */
nomen structura {
    chorda titulus;
       i32 numerus;
       s64 ms_summa;
       i32 parentes;
       s64 octeti;            /* solum cursus_quaerere_discus */
} CursusSumma;

Xar*
cursus_quaerere_summa (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina);

/* COMMISSIONES: tentamen quodque (titulus 'commissio'), recentissimum
 * primum; culpa = descendens FRACTUS profundissimus (NIHIL si
 * transiit); commissum ex nota 'commissum HASH'. */
nomen structura {
    constans Cursus* commissio;
    constans Cursus* culpa;
             chorda  commissum;
} CursusCommissio;

Xar*
cursus_quaerere_commissiones (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina);

/* FUSIO (budget B0, 2026-10-27): fusio ULTIMA (titulus 'fusio';
 * filtrum: arbor) et commissio ultima eiusdem arboris quae ANTE eam
 * incipit (commissio fusionis main in ramum). Filii utriusque (gradus
 * unus: phases et portae) ordine durationis descendentis. Fusio sine
 * fine (status INTERRUPTUS) = currens aut interrupta - tempus currens
 * vocans ex horologio computat (functio pura, nullum horologium).
 * FALSUM si nulla fusio congruit. */
nomen structura {
    constans Cursus* fusio;
    constans Cursus* commissio;          /* NIHIL si nulla ante */
                Xar* filii_fusionis;     /* Xar de (constans Cursus*) */
                Xar* filii_commissionis; /* vacuum si commissio NIHIL */
} CursusFusio;

b32
cursus_quaerere_fusio (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina,
                CursusFusio* exitus);

/* STATISTICA: per titulum (cursus filtrati) - cursus, mediana et
 * maxima transeuntium, fracti; ordine medianae descendentis. Sectiones
 * membri = praefixum 'MEMBRUM#'. */
nomen structura {
    chorda titulus;
       i32 numerus;
       i32 fracti;
       s64 mediana_ms;
       s64 maxima_ms;
} CursusStatistica;

Xar*
cursus_quaerere_statistica (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina);

/* LENTESCENTES (Franus 10-14: "tests consistently getting slower"):
 * per titulum cursus transeuntes ordine temporis; mediana ULTIMORUM
 * 'fenestra' contra medianam PRIORUM (saltem 'fenestra'). Nominatur si
 * ratio >= limes_permille ET ultimorum saltem fenestra - I singuli
 * supra medianam priorem (constantia: ictus unus machinae occupatae
 * non numeratur). Ordine rationis descendentis. CAVE: onus machinae
 * omnia simul tardat - post meridiem tota lenta hic non filtratur. */
nomen structura {
    chorda titulus;
       i32 priores;
       s64 mediana_prior_ms;
       s64 mediana_recens_ms;
       i32 supra;            /* ultimorum supra medianam priorem */
       i32 ratio_permille;   /* recens / prior * M */
} CursusLentescens;

Xar*
cursus_quaerere_lentescentes (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                        i32  fenestra,
                        i32  limes_permille,
                    Piscina* piscina);

/* NUMQUAM: tituli universi (Xar de chorda) quibus nullus cursus est.
 * Cursus titulum spectat si titulus aequalis est aut argv primum ea
 * via (aut .../via) incipit. */
Xar*
cursus_quaerere_numquam (
    constans CursusIndex* index,
            constans Xar* universum,
                 Piscina* piscina);

/* DISCUS: octeti corporum per titulum ex introitibus diurnae (Xar de
 * DiurnaIntroitus), corpora deleta non numerata; ordine octetorum. */
Xar*
cursus_quaerere_discus (
    constans Xar* introitus,
         Piscina* piscina);

/* NIHIL CONGRUIT (2026-10-15): quaestio vacua cum filtro tituli
 * (titulus aut praefixum) tacebat - 'titulus erratus' et 'nullus
 * cursus fractus' idem videbantur (probatio_tcp_servus quaesita, sed
 * titulus est 'probationes_radicis/probatio_tcp_servus'). *noti_out =
 * cursus quorum titulus filtro TITULI solo congruit (status, post,
 * nota, arbor, ramus neglecti). Si nulli: tituli SIMILES (quaesitum
 * intra titulum, aut segmentum ultimum post '/', '#', ':' aequale),
 * ordine numeri cursuum descendentis, maximi 'maximi' (CursusSumma:
 * titulus, numerus). Xar vacuum si noti aut nihil simile; NIHIL si
 * filtrum tituli nullum. */
Xar*
cursus_quaerere_similes (
      constans CursusIndex* index,
    constans CursusFiltrum* filtrum,
                       i32  maximi,
                       i32* noti_out,
                   Piscina* piscina);

/* CURRENTES (colloquia T8): cursus APERTI (initium sine fine) ut
 * arbor, cum custode et progressu. Purum: vitam custodis vocator
 * iudicat (mensor: processus_vivus) - hic solum pid. Radices
 * recentissimae primum, praeordine. */
nomen structura {
    constans Cursus* cursus;
                i32  profunditas;   /* radix aperta ZEPHYRUM */
                s32  custos;        /* pid; ZEPHYRUM = ignotus (ante T8) */
                i32  perfecti;      /* filii finiti: portae sub
                                     * commissione, membra acta (nota non
                                     * IUDICIUM) sub porta */
                i32 fracti;        /* ex perfectis */
                i32 debiti;        /* consilium.portae; ZEPHYRUM = ignoti */
                i32 iudicata;      /* fabrica.iudicata ultima */
                i32 ambitus;       /* fabrica.ambitus; ZEPHYRUM = non
                                     * fabrica */
                s64 minima_ms;     /* historia tituli (V ultimi
                                     * transeuntes); ZEPHYRUM = nulla */
                s64 maxima_ms;
             chorda colloquium;    /* id sessionis Claude radicis;
                                    * vacua = ignota (homo, ante T9) */
             chorda fracta;        /* tituli filiorum fractorum (non
                                    * IUDICIUM), spatiis iuncti */
} CursusCurrens;

Xar*
cursus_quaerere_currentes (
       constans CursusIndex* index,
               constans Xar* mensurae,   /* Xar de Mensura */
     constans CursusFiltrum* filtrum,   /* arbor, ramus */
                    Piscina* piscina);

/* GRAVES (credo memoria T3): per titulum sectionis - cursus filtrati
 * quibus facta memoriae credo adhaerent (mensura.h, MENSURA_CREDO) -
 * maxima trans cursus, octeti. Pondus = maior arenae apicis et apicis
 * aucti: quod tectum sectionis (T4) iudicat. Ordine ponderis
 * descendentis; aequalitas ordine titulorum; filtrum->numerus =
 * ordines maximi. Cursus sine factis absunt. */
nomen structura {
    chorda titulus;
       i32 numerus;          /* cursus mensi */
       s64 arena_apex;
       s64 apex_auctus;
       s64 vestigium;
       s64 pondus;
} CursusGravis;

Xar*
cursus_quaerere_graves (
       constans CursusIndex* index,
               constans Xar* mensurae,   /* Xar de Mensura */
     constans CursusFiltrum* filtrum,
                    Piscina* piscina);

/* VITA CUSTODIS (colloquia T10a; olim in tools/mensor.c): iudicium
 * vocatoris (processus_vivus) - hic solum nomen et forma */
nomen enumeratio {
    CURSUS_VITA_VIVA = ZEPHYRUM,
    CURSUS_VITA_MORTUA,
    CURSUS_VITA_IGNOTA
} CursusVita;

/* LINEA CURRENTIS (colloquia T10a): contractus inter 'mensor quaestio
 * currentes -machina' et lectores (vicus T10b) - forma in bibliotheca
 * una. Campi TAB: CURRENS profunditas id parens titulus arbor ramus
 * initium custos vita perfecti fracti debiti iudicata ambitus
 * minima_ms maxima_ms colloquium fracta. */
nomen structura {
           i32 profunditas;
        chorda id;
        chorda parens;
        chorda titulus;
        chorda arbor;
        chorda ramus;
           s64 initium;
           s32 custos;
    CursusVita vita;
           i32 perfecti;
           i32 fracti;
           i32 debiti;
           i32 iudicata;
           i32 ambitus;
           s64 minima_ms;
           s64 maxima_ms;
        chorda colloquium;
        chorda fracta;
} CursusCurrensLinea;

/* linea cum '\n'; vacua si campus impurus (TAB, linea nova) */
chorda
cursus_currentem_ad_lineam (
    constans CursusCurrens* r,
                CursusVita  vita,
                   Piscina* piscina);

/* FALSUM si non linea CURRENS aut deformis; chordae in lineam
 * spectant (non copiantur) */
b32
cursus_currentem_ex_linea (
                 chorda  linea,
     CursusCurrensLinea* exitus);

#endif /* MENSURA_QUAESTIO_H */
