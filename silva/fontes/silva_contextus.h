/* silva_contextus.h - Contextus hospitis (Phase 7 Chunk A)
 *
 * Obiectum diu vivens et reusabile hospitis (spec-v2 par 8.2): fines
 * (limina dura configurabilia), intermissio (pergere - interrogatio
 * determinata, nulla horologia), lexica (plagulae definitionum ante
 * fontem principalem processatae - latina.h hac via "compiled-in"),
 * praebenda (contentum includendum). RECEPTUM est, non status:
 * quaeque parsura expansionem recentem creat et receptum applicat
 * (silva_parsare_cum_contextu).
 *
 * Praeposita configurationum nominata ("macos", "c89-nudum" -
 * par 10.2.5) = petra miliaria config-query, NOMINATA.
 *
 * Strata inferiora (expansor, motor GLR) contextum numquam noscunt -
 * gubernator campos crudos deorsum copiat (velamen: silva speculum
 * nescit, expansor contextum nescit).
 */

#ifndef SILVA_CONTEXTUS_H
#define SILVA_CONTEXTUS_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"


/* ==================================================
 * Fines - limina dura (par 8.2). 0 = infinitum.
 * Defaltae generosae in creare (habitus tree-sitter:
 * semper activae; hospes tollere potest).
 * ================================================== */

nomen structura {
    i32 lexemata;               /* fluxus expansus maximus */
    i32 generationes;           /* generationes expansionis maximae */
    i32 profunditas_includendi; /* profunditas includendi maxima */
    i32 profunditas_regionum;   /* profunditas regionum maxima */
    i32 frons;                  /* frons GSS maxima (per segmentum) */
} SilvaFines;


/* ==================================================
 * Intermissio - interrogatio progressus
 *
 * FALSUM redditum = intermitte. CONTRACTUS: functio monotona sit
 * (semel FALSUM, semper FALSUM) - fistula pluribus locis
 * interrogat (fixpoint expansionis, exhaustio GLR, circuitus
 * secans) et post primam negationem caudam in nodum ERROR vertit
 * (arbor completa manet, reconstructio byte-exacta tenet).
 * ================================================== */

nomen b32 (*SilvaPergereFunctio)(vacuum* datum);


/* ==================================================
 * Contextus
 * ================================================== */

/* Plagula recepta (via + textus in piscinam contextus COPIATI -
 * contextus vocatorem suum supervivit) */
nomen structura {
    constans character* via;
    constans character* textus;
                   i32  mensura;
} SilvaContextusPlagula;

nomen structura SilvaContextus SilvaContextus;

structura SilvaContextus {
    Piscina*            piscina;
    SilvaFines          fines;
    SilvaPergereFunctio pergere;         /* NIHIL = numquam rogare */
    vacuum*             pergere_datum;
    i32                 passus_pergendi; /* intervallum lexematum (GLR) */
    Xar*                lexica;          /* SilvaContextusPlagula (per
                                          * valorem) - definitiones ante
                                          * fontem processatae */
    Xar* praebenda;       /* SilvaContextusPlagula (per
                                          * valorem) - includenda */
};

SilvaContextus*
silva_contextus_creare (
    Piscina* piscina);

/* Lexicon: plagula definitionum (e.g. latina.h) ante fontem
 * principalem processata. Definitiones eius a positione 0 fontis
 * valent; provenientia ad plagulam lexici ducit (fons verus). */
b32
silva_contextus_lexicon_addere (
        SilvaContextus* contextus,
    constans character* via,
    constans character* textus,
                   i32  mensura);

/* Lexicon latinum compilatum (silva_latina_datum - GENERATUM ex
 * include/latina.h per amalgatorem; fons veritatis unus) */
b32
silva_contextus_latinam_addere (
    SilvaContextus* contextus);

/* Contentum includendum (via #include resolvendum) */
b32
silva_contextus_praebere (
        SilvaContextus* contextus,
    constans character* via,
    constans character* textus,
                   i32  mensura);

/* passus <= 1 = quovis lexemate (GLR); expansio per generationem
 * interrogat, gubernator per segmentum - passus ibi non attinet */
vacuum
silva_contextus_pergere_ponere (
         SilvaContextus* contextus,
    SilvaPergereFunctio  pergere,
                 vacuum* datum,
                    i32  passus);

#endif /* SILVA_CONTEXTUS_H */
