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
    FLUXUS_EVENTUM_DEFINITIO_LOCI, /* &x contextu non-constanti
                                    * (idioma initialisationis C) */
    FLUXUS_EVENTUM_LOCI_ACCUMULAT, /* &x ad parametrum accumulantem
                                    * (contractus, 01KY3JWF): callee
                                    * pointee LEGIT ante scriptionem
                                    * - vocator initiare debet; index
                                    * 71 ante bits iudicat, deinde ut
                                    * DEFINITIO_LOCI tractatur */

    /* Membra ut pseudo-variabiles (01KYMYW75S): eventa ADDITIVA -
     * consumptores exsistentes (bita may/must, formae, iudicium
     * initiationis) genera explicite cribrant, haec praetereunt.
     * Scriptio membri definitionem TOTIUS adhuc emittit (pinna s19
     * intacta); eventum membri PRAETEREA sequitur - ordo
     * dele-deinde-pone (def basis membra omnia ad fines typi,
     * deinde membrum scriptum refinat). Basis folium directum
     * solum (profunditas I); sagitta numquam (pointee extra
     * scopum); variabilis = ordo membri (membrum_est) in tabula. */
    FLUXUS_EVENTUM_MEMBRUM_USUS,       /* lectio c.m (nodus =
                                        * ACCESSUS) */
    FLUXUS_EVENTUM_MEMBRUM_DEFINITIO   /* scriptio c.m directa
                                        * (post def totius) */
} FluxusEventumGenus;

/* Forma valoris definitionis (fluxus formae, 01KXRBS0VQ/01KYJQ7THM):
 * reticulum IGNOTA(0) < {SUBTRACTIO, ALIA} < MIXTA. Classificatio
 * SYNTACTICA ad tempus extractionis (binarium minus / assignatio
 * composita minus -> SUBTRACTIO; assignatio simplex in dextrum
 * recursat; crementa/ternarius/cetera -> ALIA, v1 angustum). */
nomen enumeratio {
    FLUXUS_FORMA_IGNOTA = 0,   /* nulla definitio attingit (iunctionis
                                * identitas; usus = negotium 71/72) */
    FLUXUS_FORMA_SUBTRACTIO,
    FLUXUS_FORMA_ALIA,
    FLUXUS_FORMA_MIXTA         /* iunctio dissentiens aut def-omnia */
} FluxusForma;

/* Stirps signata valoris definitionis (vestigatio generum, codex 82
 * arcus fluxus): reticulum IGNOTA(0) < {NEUTRA, PRIMA+n} < AMISSA.
 * Classificatio ad tempus extractionis per involucrum auxiliorum
 * (stirps_valoris) - ambitu caeca: defs sui-referentes (t = t - m)
 * debiliter classificant, iunctio conservative delet (silet,
 * numquam mentitur). Ordinales PRIMA+n = tabula stirpium vocatoris
 * (semantica); datorum semanticam numquam videt. */
nomen enumeratio {
    FLUXUS_STIRPS_IGNOTA = 0,  /* nulla definitio attingit
                                * (iunctionis identitas) */
    FLUXUS_STIRPS_NEUTRA,      /* valor insignatus definitus
                                * (litera, erasio conversione,
                                * expressio inresoluta) */
    FLUXUS_STIRPS_AMISSA,      /* iunctio dissentiens / def-omnia
                                * (provenientia perit) */
    FLUXUS_STIRPS_PRIMA        /* ordinales vocatoris: PRIMA + index
                                * tabulae stirpium */
} FluxusStirps;

/* Eventum unum. variabilis = index densus in tabulam variabilium;
 * -1 = OMNES variabiles (folium ERROR/AMBIGUUS opacum: def-omnia,
 * abstentio conservativa). */
nomen structura {
    s32                  variabilis;
    s32                  genus;        /* FluxusEventumGenus */
    constans SilvaNodus* nodus;        /* sedes (diagnostica) */
    b32                  in_initiatore_proprio;  /* int x = x */
    constans SilvaNodus* fons_valoris; /* expressio valoris definitionis
                                        * (NIHIL alias) - columna
                                        * quaestionis futurae */
    s32                  forma;        /* FluxusForma valoris (DEFINITIO
                                        * sola; IGNOTA alias) */
    s32                  stirps;       /* FluxusStirps valoris per
                                        * stirps_valoris (DEFINITIO
                                        * genera; IGNOTA alias) */
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
    b32                  effugit;      /* locus sumptus usquam (eventum
                                        * LOCI ullum): forma PERPETUO
                                        * MIXTA - scriptio per alias
                                        * post effugium invisibilis
                                        * (v1 sanum) */

    /* ordo membri (01KYMYW75S): identitas = symbolum BASIS (collisio
     * consulta - inquisitiones basium ordines membrorum praetereunt),
     * titulus_membri discriminat. basis = index ordinis basis in hac
     * tabula (effugium/parametrum basis ibi consulenda). Analysis
     * initiationis ordines membrorum numquam videt (eventa membrorum
     * genera propria ferunt). */
    b32                  membrum_est;
    s32                  basis;        /* -1 nisi membrum */
    chorda               titulus_membri;
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
    s32* formae_introitus;             /* FluxusForma per variabilem
                                        * (fluxus formae - punctum
                                        * fixum alterum, iunctio
                                        * reticuli non bitalis) */
    s32* formae_exitus;
    s32* stirpes_introitus;            /* FluxusStirps per variabilem
                                        * (vestigatio generum -
                                        * punctum fixum tertium,
                                        * eadem forma reticuli) */
    s32* stirpes_exitus;
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
    /* estne parametrum positionis datae ACCUMULANS (contractus
     * modus="accumulat" - callee pointee legit ante scriptionem)?
     * VERUM => &x eventum LOCI_ACCUMULAT pro DEFINITIO_LOCI.
     * NIHIL = numquam. CAVE: struct sine memset construitur -
     * OMNIS sedes constructionis hoc campum ponere debet (laqueus
     * vocator-initiat, lectio 2026-07-21). */
    b32 (*parametrum_accumulat)(vacuum* contextus,
        constans SilvaNodus* functio_folium, i32 index);
    /* estne typus expressionis acies? (accessus membri aciei
     * positione valoris DECADIT = LOCI totius - memset(r.c,...);
     * NIHIL = numquam acies, lectio membri invisibilis manet) */
    b32 (*expressio_acies)(vacuum* contextus,
        constans SilvaNodus* nodus);
    /* stirps signata valoris definitionis (FluxusStirps: NEUTRA aut
     * PRIMA+n tabulae vocatoris); NIHIL = semper NEUTRA. CAVE:
     * struct sine memset construitur - OMNIS sedes constructionis
     * hoc campum ponere debet (laqueus vocator-initiat). */
    s32 (*stirps_valoris)(vacuum* contextus,
        constans SilvaNodus* expressio);
    /* classificatio AMBITU-CONSCIA (v2 fluxus, ianua decisi 1):
     * ut stirps_valoris sed cum statu currenti per variabilem
     * (tabula exitus medio-renovata, ordine eventorum = status
     * verus) - defs sui-referentes (t = t - m in ansa)
     * provenientiam servant. NIHIL = columna caeca ev->stirps
     * adhibetur. Terminatio: introitus per iunctionem monotone
     * non-descendens in reticulo finito - recomputationes finitae
     * etiam classificatione non-monotona. CAVE vocator-initiat. */
    s32 (*stirps_valoris_ambitu)(vacuum* contextus,
        constans SilvaNodus* expressio,
        constans FluxusDatorum* datorum,
        constans s32* stirpes);
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
