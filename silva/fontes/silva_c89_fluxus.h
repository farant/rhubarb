/* silva_c89_fluxus.h - Graphus fluxus imperii (FLUXUS-0)
 * (specificatio = project-specs/silva-fluxus-0-spec.md;
 * simulatio = project-specs/silva-simulatio-11.md)
 *
 * CFG granularitate SENTENTIARUM per definitionem functionis, super
 * arborem silvae aedificatus. Bloci sunt CURSUS fluxus (non scopi:
 * corpus nidificatum in eundem blocum recurrit); sententiae folii
 * (expressionis, declarationis, redde...) in blocis habitant,
 * sententiae compositae (si/dum/commutatio...) per MARGINES suas
 * repraesentantur (margo.origo = nodus sententiae compositae).
 * Nodi arboris tantum REFERUNTUR - nihil copiatur (codex-est-basis-
 * datorum: quisque campus columna quaestionis futurae).
 *
 * SUTURA AUXILIORUM (XI-1): canonicalizatio lectionum AMBIGUARUM et
 * aestimatio constantium sunt POLITICA VOCATORIS - semantica
 * involucra sua tradit, probationes aestimatorem litteralem parvum.
 * Membra NIHIL = identitas / nihil-constans.
 *
 * ATTINGIBILITAS: DFS unus ab introitu post constructionem;
 * cadit_attingibilis = margo CADIT cum fonte attingibili exsistit
 * (praedicatum codicis 63 totum - XI-6).
 *
 * VIOLATIONES UT DATA: frange/perge sine contextu et salta ad
 * titulum ignotum in tabulatis fractiones_extra/saltus_ignoti
 * colliguntur - gradus VIOLATIO (codices 66/67) eas legunt, numquam
 * recomputant. Aedificator numquam cadit (vectis robustitatis):
 * nodi ERROR opaci transeunt, AMBIGUI inresoluti opaci manent.
 *
 * INTERNUM v1 (decisio Q4): amalgama modulum includit, silva.h
 * ordines codicum tantum exportat. Via retro: superficies
 * quaestionum solarii involucrum publicat.
 */

#ifndef SILVA_C89_FLUXUS_H
#define SILVA_C89_FLUXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_nodus.h"

/* ==================================================
 * Genera marginum
 * ================================================== */

nomen enumeratio {
    FLUXUS_MARGO_SEQUENS = 0,   /* fluxus rectus */
    FLUXUS_MARGO_VERUS,         /* conditio vera */
    FLUXUS_MARGO_FALSUS,        /* conditio falsa */
    FLUXUS_MARGO_CASUS,         /* dispersio commutationis ad casum */
    FLUXUS_MARGO_ORDINARIUS,    /* semita non-congruentiae: ad gregem
                                 * ordinarium expressum AUT (absente)
                                 * ad blocum post commutationem */
    FLUXUS_MARGO_RUPTURA,       /* frange */
    FLUXUS_MARGO_PERSECUTIO,    /* perge */
    FLUXUS_MARGO_SALTUS,        /* salta ad titulatum */
    FLUXUS_MARGO_REDDITUS,      /* redde -> exitus */
    FLUXUS_MARGO_CADIT,         /* finis functionis apertae -> exitus
                                 * (fons attingibilis = codex 63) */
    FLUXUS_MARGO_LAPSUS         /* finis gregis casus apertus in
                                 * titulum sequentem (codex 64) */
} FluxusMargoGenus;

nomen structura FluxusBlocus FluxusBlocus;

nomen structura {
    s32                  genus;       /* FluxusMargoGenus */
    FluxusBlocus*        destinatio;
    constans SilvaNodus* origo;       /* nodus qui marginem genuit */
} FluxusMargo;

structura FluxusBlocus {
    i32  index;                 /* in tabulato bloci */
    Xar* sententiae;            /* constans SilvaNodus* (folia) */
    Xar* margines;              /* FluxusMargo (valore, exeuntes) */
    b32  attingibilis;          /* post DFS ab introitu */
    constans SilvaNodus* titulus_dux;  /* casus/ordinarius/titulatum
                                 * qui blocum aperit; NIHIL alias
                                 * (sedes flagrationis gradus) */
};

/* Titulus functionis (salta-destinatio) - tabula quaesibilis */
nomen structura {
    chorda               titulus;
    FluxusBlocus*        blocus;
    constans SilvaNodus* nodus;   /* titulatum */
} FluxusTitulus;

nomen structura {
    constans SilvaNodus* definitio;  /* definitio-functionis */
    Xar*          bloci;             /* FluxusBlocus (valore; monstratores
                                      * stabiles - xar segmentatum) */
    FluxusBlocus* introitus;
    FluxusBlocus* exitus;            /* syntheticus; REDDITUS/CADIT huc */
    Xar*          tituli;            /* FluxusTitulus (valore) */
    Xar*          saltus_ignoti;     /* constans SilvaNodus* (salta) - codex 67 */
    Xar*          fractiones_extra;  /* constans SilvaNodus* (frange/perge) - codex 66 */
    b32           cadit_attingibilis;
} FluxusFunctionis;

/* ==================================================
 * Sutura auxiliorum (politica vocatoris)
 * ================================================== */

nomen structura {
    /* lectio canonica nodi AMBIGUI; NIHIL = identitas */
    constans SilvaNodus* (*canonicum)(vacuum* contextus,
        constans SilvaNodus* nodus);
    /* aestimatio expressionis constantis (plicatio!); NIHIL =
     * nihil constans. VERUM + *valor si constans. */
    b32 (*aestimator)(vacuum* contextus,
        constans SilvaNodus* expressio, s64* valor);
    vacuum* contextus;
} FluxusAuxilia;

/* ==================================================
 * API
 * ================================================== */

/* Graphum aedificare pro definitione functionis una. Piscina
 * vocatoris omnia possidet (vita = vita piscinae). Auxilia NIHIL
 * licent (identitas / nihil-constans). Numquam NIHIL reddit nisi
 * definitio ipsa NIHIL aut sine corpore (declaratio). */
FluxusFunctionis*
silva_c89_fluxus_aedificare (
    Piscina*                piscina,
    constans SilvaNodus*    definitio,
    constans FluxusAuxilia* auxilia);

#endif /* SILVA_C89_FLUXUS_H */
