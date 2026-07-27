/* silva_expandere.h - Expansor silvae (Phase 2)
 *
 * Chunk A: acta (journal) + directivae + definitiones macro.
 * Chunk B: generationes expansionis (caecationes, S13).
 * Chunk C: # et ## in substitutione.
 * Chunk D: includenda + regiones conditionales + custodes.
 *
 * Decisiones (silva-spec-v2.md §8.1/S10 + §10.2, simulationes ①②):
 *   - ACTA pro segmentis: historia status macro = index appendix-solus
 *     eventorum (fons, linea, ±definitio, conditio_id). Tabula viva =
 *     via defalta (default track). macros_ad_lineam = replay actorum
 *     in piscinam vocantis. (TabulaDispersa exemplar non habet - R2.)
 *   - Directiva = lexema CANCELLUM cum initium_lineae VERO: proprietas
 *     LOCALIS (classis defectus arbor2 hic irrepraesentabilis).
 *   - Corpus macro = LAMINA fluxus lexematum fontis: def-site
 *     provenientia gratis (lexemata immutabilia et communia).
 *   - Functio-similis si '(' IMMEDIATE post titulum (adiacentia cruda:
 *     byte_offset paren == byte_offset tituli + longitudo tituli).
 *   - Duae viae (§10.2): via defalta evaluata pellit statum vivum et
 *     acta; OMNES rami retinentur in arbore regionum (rami non sumpti
 *     ut laminae crudae partitionis - quodque lexema uni ramo).
 *   - Custodes (est_custos): plagula custodita nullam regionem gignit -
 *     interior incondicionaliter processatur, custos in SilvaFons.
 *   - Includenda per API praebentur (silva plagulas numquam aperit);
 *     includendum ignotum = "discens" (via memorata, processio pergit).
 */

#ifndef SILVA_EXPANDERE_H
#define SILVA_EXPANDERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"

/* ==================================================
 * Definitio macro
 * ================================================== */

nomen structura {
    chorda*  titulus;       /* nomen macro */
    b32      est_functio;   /* '(' adiacens post titulum */
    b32      est_variadica; /* ... in parametris (C99 - signatum) */
    Xar*     parametra;     /* Xar de chorda* (tituli parametrorum); NIHIL si nulla */
    Xar*     corpus;        /* Xar de SilvaToken* - lamina fluxus fontis */
    s32      fons_index;    /* -1 si ex API */
    i32      linea_def;     /* 0 si ignota */
    b32      ex_api;
} SilvaMacroDef;


/* ==================================================
 * Acta (journal) - historia status macro
 * ================================================== */

nomen enumeratio {
    SILVA_EVENTUM_DEFINITIO = 0,
    SILVA_EVENTUM_DELETIO
} SilvaEventumGenus;

nomen structura {
    SilvaEventumGenus genus;
    s32               fons_index;
    i32               linea;
    chorda*           titulus;
    SilvaMacroDef*    def;          /* NIHIL pro DELETIO */
    i32               conditio_id;  /* 0 = incondicionale */
    s32               positus;      /* index in fluxu reliquorum a quo
                                     * eventum valet (status ad punctum -
                                     * semantica cpp pro #undef medio) */
} SilvaEventum;


/* ==================================================
 * Chunk D - Regiones conditionales (duae viae, §10.2)
 *
 * Regio = #if/#ifdef/#ifndef ... (#elif/#else ...)* #endif.
 * Via defalta: rami ordine evaluantur; primus verus sumitur -
 * lexemata eius normaliter processantur (definitiones cum
 * conditio_id ramī). Rami NON sumpti laminas crudas retinent
 * (partitio: quodque lexema uni ramo pertinet).
 * ================================================== */

nomen structura SilvaRegio SilvaRegio;

nomen enumeratio {
    SILVA_RAMUS_IF = 0,   /* #if    (verba ipsa C sunt) */
    SILVA_RAMUS_IFDEF,    /* #ifdef */
    SILVA_RAMUS_IFNDEF,   /* #ifndef */
    SILVA_RAMUS_ELIF,     /* #elif */
    SILVA_RAMUS_ELSE      /* #else */
} SilvaRamusGenus;

nomen structura {
    SilvaRamusGenus genus;
    i32         conditio_id;    /* index+1 in exp->rami; 0 numquam */
    Xar*        directiva;      /* lexemata lineae directivae */
    Xar*        expressio;      /* lexemata conditionis; NIHIL pro else */
    b32         est_evaluatum;  /* evaluatio temptata et successit */
    s64         valor;          /* fructus evaluationis */
    b32         est_sumptum;    /* sumptus in via defalta */
    b32         est_numquam;    /* #if 0 idioma (litteralis falsa) */
    Xar*        lexemata_cruda; /* ramus non sumptus: lamina cruda; NIHIL si sumptus */
    SilvaRegio* regio;          /* regio continens */
    s32         corpus_initium; /* offset primi lexematis corporis
                                 * (post lineam directivae); -1 si
                                 * corpus vacuum (sim ⑦ C1) */
    s32         corpus_finis;   /* offset post ultimum lexema corporis;
                                 * -1 si vacuum */
} SilvaRamus;

structura SilvaRegio {
    s32         fons_index;
    i32         linea;
    Xar*        rami;           /* Xar de SilvaRamus* */
    SilvaRegio* pater;          /* regio amplectens; NIHIL si suprema */
    Xar*        filiae;         /* Xar de SilvaRegio* (in ramis sumptis) */
    b32         est_imperfecta; /* EOF ante #endif */
    b32         est_ultra_modum; /* profunditas ultra limen (Phase 7):
                                  * NULLUS ramus evaluatur - omnes crudi
                                  * (degradatio determinata; recursio
                                  * cessat, octeti supersunt) */
    Xar*        directiva_finis; /* lexemata lineae #endif; NIHIL si
                                  * imperfecta (sim ⑦ C1). REGIO lineas
                                  * structurales suas POSSIDET (β, sim ⑦
                                  * C2): #if/#elif/#else/#endif numquam
                                  * in directivae_out intrant */
    b32         est_texta;       /* in arborem texta (Phase 7 Chunk B):
                                  * lineae + cruda ex ARBORE emittuntur,
                                  * non ex reinserendis (dominus unus) */
};


/* ==================================================
 * Chunk D - Includenda
 * ================================================== */

nomen structura {
    s32  fons_index;
    Xar* lexemata;      /* fluxus attachatus contenti */
} SilvaIncludendum;

/* Inclusio observata (graphum dependentiarum - "discens") */
nomen structura {
    s32     fons_ex;            /* fons in quo #include stat */
    chorda* via;                /* via petita */
    s32     fons_ad;            /* fons resolutus; -1 = ignotus */
    b32     est_praetermissa;   /* praetermissa (custos vel profunditas) */
} SilvaInclusio;


/* Extentum invocationis functio-similis STRATI 0 (Phase 5 Chunk B,
 * simulatio ⑥ C5): lamina [nomen .. ')'] fluxus quo invocatio
 * collecta est - octeti invocationis cum triviis argumentorum.
 * Memoratur solum cum lexema nominis origo FONS fert (stratum 0):
 * reconstructio fontis hos octetos emittit ubi arbor lexemata
 * expansa fert. */
nomen structura {
    SilvaToken* invocatio;   /* lexema nominis (origo FONS) */
    Xar*        lamina;      /* Xar de SilvaToken* - [nomen, post ')') */
} SilvaExtentumInvocationis;


/* ==================================================
 * Contextus expansionis
 * ================================================== */

nomen structura SilvaExpansio SilvaExpansio;

structura SilvaExpansio {
    Piscina*        piscina;
    Xar*            fontes;     /* Xar de SilvaFons (per valorem) */
    TabulaDispersa* macros;     /* status vivus - via defalta */
    Xar*            acta;       /* Xar de SilvaEventum (per valorem) */
    Xar*            rami;       /* Xar de SilvaRamus* - tabula conditio_id */
    Xar*            regiones;   /* Xar de SilvaRegio* - regiones supremae */
    TabulaDispersa* includenda; /* via (chorda) -> SilvaIncludendum* */
    Xar*            inclusiones; /* Xar de SilvaInclusio (per valorem) */
    Xar*            extenta;    /* Xar de SilvaExtentumInvocationis
                                 * (per valorem) - invocationes
                                 * functio-similes strati 0 */
    i32             profunditas_includendi;
    s32             fons_api;   /* fons syntheticus "<api>"; -1 = nondum */
    TabulaDispersa* tabula_activa; /* tabula temporalis expansionis
                                    * positionalis; NIHIL = tabula viva */

    /* ==== Fines (Phase 7 Chunk A - par 8.2). 0 = infinitum. ====
     * Fines expansionem DEGRADANT, numquam totalitatem: limine tacto
     * lexemata reliqua INEXPANSA fluunt (FONS - se ipsa emittunt),
     * ergo arbor completa et reconstructio byte-exacta etiam sub
     * finibus tenent. Defaltae generosae in creare positae. */
    i32 limen_lexematum;     /* fluxus expansus maximus (per generationem) */
    i32 limen_generationum;  /* generationes expansionis maximae */
    i32 limen_includendi;    /* profunditas includendi maxima */
    i32 limen_regionum;      /* profunditas regionum maxima */

    /* Intermissio (SilvaContextus eam ponit; forma cruda ne stratum
     * inferius contextum noscat): FALSUM redditum = intermitte. */
    b32     (*pergere)(vacuum* datum);   /* NIHIL = numquam rogare */
    vacuum*   pergere_datum;

    /* Status finium (productum, non depuratio) */
    b32 expansio_decisa;      /* expansio trunca (limen lexematum aut
                               * generationum tactum) */
    b32 est_intermissa;       /* pergere FALSUM reddidit */
    b32 fines_tactae;         /* quilibet limen tactus */
    i32 profunditas_regionum; /* numerator vivus (transiens) */
};

/* Defaltae finium (tree-sitter habitus: fines generosae SEMPER
 * activae; hospes eas tollere potest - 0 = infinitum) */
#define SILVA_LIMEN_LEXEMATUM_DEFALTUM    1048576
#define SILVA_LIMEN_GENERATIONUM_DEFALTUM C
#define SILVA_LIMEN_INCLUDENDI_DEFALTUM   XXXII
#define SILVA_LIMEN_REGIONUM_DEFALTUM     LXIV

SilvaExpansio*
silva_expansio_creare (
    Piscina* piscina);

/* Addere fontem; reddit indicem eius */
s32
silva_fons_addere (
    SilvaExpansio*      exp,
    constans character* via,
    b32                 est_syntheticus);


/* Praebere contentum includendum: via -> textus (lexatur statim).
 * Silva plagulas numquam ipsa aperit - vocans contentum praebet.
 * Reddit indicem fontis novi. */
s32
silva_includendum_praebere (
    SilvaExpansio*      exp,
    constans character* via,
    constans character* textus,
    i32                 mensura);


/* ==================================================
 * Processio directivarum (Chunk A + D)
 *
 * Ambulat fluxum attachatum (ex silva_lexare); directivas
 * define/undef/include/if/ifdef/ifndef/elif/else/endif tractat:
 * tabulam vivam et acta renovat, regiones aedificat, includenda
 * resoluta in loco inserit. Custodes ante ambulationem deteguntur
 * (plagula custodita nullam regionem gignit). Reddit fluxum
 * RELIQUUM (lexemata non in directivis consumptis); directivae
 * ipsae in *directivae_out (Xar de Xar de SilvaToken* - una linea
 * logica per introitum) si non NIHIL.
 * ================================================== */

Xar*
silva_expansio_directivas_processare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    Xar**          directivae_out);

/* Quaerere definitionem in tabula viva; NIHIL si absens */
SilvaMacroDef*
silva_expansio_quaerere (
    SilvaExpansio* exp,
    chorda         titulus);

/* ==================================================
 * Iniectio macro per API (fons syntheticus "<api>")
 *
 * Via definitionum compilatarum (latina.h!) et definitionum
 * vocantis. Corpus textus lexatur in piscinam - lexemata
 * corporis sunt lexemata FONTIS synthetici (provenientia
 * plena per catenas normales).
 * ================================================== */

b32
silva_macro_addere (
    SilvaExpansio*      exp,
    constans character* titulus,
    constans character* corpus);

/* parametra: series NIHIL-terminata; "..." = variadica */
b32
silva_macro_functio_addere (
    SilvaExpansio*       exp,
    constans character*  titulus,
    constans character** parametra,
    constans character*  corpus);


/* ==================================================
 * Prospectus macro - oraculum GLR (brainstorm v2:
 * "macro lookahead" pro prioritate furcarum)
 * ================================================== */

nomen structura {
    SilvaLexemaGenus genus;          /* primi lexematis corporis; EOF si vacuum */
    b32              est_vacuum;     /* corpus vacuum */
    b32              est_recursivum; /* primum lexema ipsum nomen macro est */
} SilvaProspectus;

/* Reddit FALSUM si titulus non est macro */
b32
silva_expansio_prospectare (
    SilvaExpansio*   exp,
    chorda           titulus,
    SilvaProspectus* prospectus_out);

/* Status macro ad (fons, linea): replay actorum in piscinam datam.
 * Tabula chorda->SilvaMacroDef*. Eventa ordine appendicis replicantur
 * usque ad primum eventum eiusdem fontis cuius linea >= linea data. */
TabulaDispersa*
silva_expansio_macros_ad_lineam (
    SilvaExpansio* exp,
    Piscina*       piscina,
    s32            fons_index,
    i32            linea);


/* ==================================================
 * Chunk B - Generationes expansionis
 *
 * Generatio una = gradus expansionis C89 plenus (S13):
 * argumenta PLENE prae-expansa intra gradum, substitutio,
 * caecatio extensa (Prosser); RESCAN = generatio proxima.
 * Terminatio per caecationes (finis semanticus, non cap).
 *
 * SIMPLIFICATIO PROSSER PERMANENS (disposita Phase 7 Chunk C):
 * caecatio argumentorum HS_call fertur, non HS_call∩HS_rparen
 * exacta. Classis divergentiae parenthesibus IMPARIBUS in
 * corporibus macrorum functio-similium eget; lustrum solarii
 * (2.3MB C89 veri, 2026-07-03) NULLAM invenit - super-caecatio
 * classis exoticae accepta, evidentia memorata (phase-log).
 * ================================================== */

/* Una generatio: fluxus novus; *mutatum_out VERUM si quid expansum */
Xar*
silva_expansio_generatio (
    SilvaExpansio* exp,
    Xar*           lexemata,
    b32*           mutatum_out);

/* Expansio ad fixum CONTRA STATUM VIVUM: generationes iteratae dum
 * mutationes. Pro seriebus sine positione (argumenta, expressiones
 * conditionum) - status macro unus per totam seriem.
 * strata_out (si non NIHIL): Xar de Xar* - exitus CUIUSQUE
 * generationis mutantis, ordine (strata modeli stratorum). */
Xar*
silva_expansio_expandere (
    SilvaExpansio* exp,
    Xar*           lexemata,
    Xar**          strata_out);

/* Expansio POSITIONALIS fluxus reliquorum (semantica cpp fidelis):
 * eventa actorum per positus applicantur dum fluxus ambulatur, ergo
 * quodque lexema statum macro SUI PUNCTI videt (#undef/redefinitio
 * media honorantur). reliqua DEBET esse fluxus proxime redditus a
 * silva_expansio_directivas_processare huius exp. Haec est via
 * plagularum integrarum; expandere supra pro seriebus solutis. */
Xar*
silva_expansio_expandere_reliqua (
    SilvaExpansio* exp,
    Xar*           reliqua,
    Xar**          strata_out);


/* ==================================================
 * Fenestrae lectionis (additiones II) - vistae angustae pro
 * hospitibus amalgamatis: SilvaExpansio trans limitem opaca manet;
 * hae structurae contractum cathedralem angustum dant (formae
 * internae liberae manent). Index ramorum = conditio_id - 1
 * (tabula plana exp->rami).
 * ================================================== */

nomen structura {
    constans chorda* via;
    s32              fons_ex;
    s32              fons_ad;           /* -1 = ignotus */
    b32              est_praetermissa;
} SilvaInclusioVista;

nomen structura {
    SilvaRamusGenus  genus;
    b32              est_sumptum;
    b32              est_numquam;
    s32              corpus_initium;    /* OCTETI in fonte; -1 vacuum */
    s32              corpus_finis;      /* exclusivus; -1 vacuum */
    s32              fons_index;
    i32              linea;
} SilvaRamusVista;

nomen structura {
    constans chorda* titulus;
    b32              est_functio;
    s32              fons_index;
    i32              linea;
    s32              corpus_initium;    /* OCTETI in fonte (lexema
                                         * primum corporis); -1 =
                                         * vacuum aut ex api.
                                         * Exemplar SilvaRamusVista;
                                         * LEGATUS v0.2 - hover
                                         * multilinearis sine
                                         * scansione '\'. */
    s32              corpus_finis;      /* exclusivus; -1 vacuum */
} SilvaMacroVista;

i32
silva_fontes_numerus (constans SilvaExpansio* exp);

/* Via fontis; NIHIL si extra fines (synthetici titulos reddunt) */
constans chorda*
silva_fons_via (constans SilvaExpansio* exp, s32 fons_index);

i32
silva_inclusiones_numerus (constans SilvaExpansio* exp);

/* Catena inclusionum AD fontem datum: viae ab radice ad
 * includentem proximum (" > " separatae; fons ipse exclusus).
 * 0 = radix ipse / ignotus / spatium deficiens (buffer vacuus). */
insignatus integer
silva_inclusionis_catena_scribere (constans SilvaExpansio* exp,
    s32 fons_index, character* buffer, insignatus integer capacitas);

b32
silva_inclusio_vista (constans SilvaExpansio* exp, i32 index,
    SilvaInclusioVista* vista_out);

i32
silva_rami_numerus (constans SilvaExpansio* exp);

b32
silva_ramus_vista (constans SilvaExpansio* exp, i32 index,
    SilvaRamusVista* vista_out);

/* Definitiones UT ACTAE (acta = fructus: #undef historiam non
 * delet); O(acta) per vocatum - satis ad mensuram v1 */
i32
silva_macros_numerus (constans SilvaExpansio* exp);

b32
silva_macro_vista (constans SilvaExpansio* exp, i32 index,
    SilvaMacroVista* vista_out);

#endif /* SILVA_EXPANDERE_H */
