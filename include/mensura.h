/*
 * mensura.h - Mensurae nidificatae, per cursum ligatae
 *
 * Quaestio Franis (2026-08-13): metra probationum, compilationis,
 * modulorum singulorum - cum machina et commisso adnexis, ut per
 * tempus depingi possint.
 *
 * ==========================================================
 * NON EST METRUM (01KYAMZVFN)
 * ==========================================================
 *
 * Metrum parcatum est bibliotheca serierum temporalium forma RRD:
 * exempla CONTINUA, CUM DAMNO consulto, magnitudine fixa, resolutio
 * senescens degradatur. Aptum est ad "quantum memoriae per mensem".
 *
 * Mensura contraria est: eventa DISCRETA, SINE DAMNO, cursui uni
 * ligata, NIDIFICATA. "Quod commissum compilationem tardiorem
 * fecit" quaestio est quam RRD respondere NON potest, quia
 * exemplum illud in medium per aetatem fusum erit.
 *
 * Sorores sunt, non idem. Nomina distincta consulto.
 *
 * ==========================================================
 * NIDIFICATIO
 * ==========================================================
 *
 * Mensura quaeque parentem habere potest. Ita tempus compilationis
 * TOTIUS operis mensuras singulorum modulorum CONTINET, et summa
 * partium contra totum probari potest.
 *
 * Trans processus nidificatio GRATIS venit: sessio et parens in
 * AMBITU vivunt (MENSOR_SESSIO, MENSOR_PARENS), quos proles sponte
 * hereditat. Intra processum unum (ansa concharum quae per plagulas
 * currit) heredidatio nihil dat - ibi parens EXPLICITE dandus est.
 *
 * ==========================================================
 * DIARIUM PRIMUM, VOLUMEN POSTEA
 * ==========================================================
 *
 * Scriptio in volumen sqlite aperit et transactionem committit -
 * decem millisecunda fortasse. Per plagulas CL id secunda addit ad
 * ipsam aedificationem quam metimur: instrumentum quod rem mutat
 * quam metitur inutile est, et mox exstinguitur.
 *
 * Ergo via calida est APPENDIX PLAGULAE PLANAE (diarium): linea
 * una, sine sqlite. Deinde mensura_condere totum fascem in volumen
 * transactione una condit, cursu perfecto.
 */

#ifndef MENSURA_H
#define MENSURA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "volumen.h"

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * Mensura - Momentum unum mensuratum
 *
 * @sessio:   clavis cursus (omnes mensurae unius cursus eam ferunt)
 * @id:       clavis huius mensurae (parens aliarum esse potest)
 * @parens:   VACUA = radix
 * @titulus:  quid mensuratum sit, e.g. "compilatio.lib/chorda.c"
 * @valor:    numerus
 * @unitas:   "secunda", "octeti", "numerus" - SINE HAC axis
 *            depingi non potest nec valores tuto sumi
 * @momentum: millisecunda ab epocha (horologium muri)
 */
nomen structura {
    chorda sessio;
    chorda id;
    chorda parens;
    chorda titulus;
    duplex valor;
    chorda unitas;
    s64    momentum;
} Mensura;

nomen structura {
    b32    successus;
    i32    conditae;
    chorda causa;
} MensuraFructus;

/* ============================================================
 * Claves
 * ============================================================ */

/*
 * mensura_sessionem_novam - Clavis cursus nova
 *
 * Tempore praefixa, ergo ordine temporis ordinatur.
 */
chorda
mensura_sessionem_novam (Piscina* piscina);

/*
 * mensura_id_novum - Clavis mensurae nova
 *
 * Unica etiam inter processus simul currentes (tempus + pid +
 * numerator intra processum).
 */
chorda
mensura_id_novum (Piscina* piscina);

/*
 * mensura_momentum_nunc - Millisecunda ab epocha
 *
 * s64 quia deltae temporis natura signatae sunt.
 */
s64
mensura_momentum_nunc (vacuum);

/* ============================================================
 * Forma lineae (NUCLEUS PURUS - probabilis sine plagula)
 * ============================================================ */

/*
 * mensura_ad_lineam - Mensuram in lineam TSV vertere
 *
 * Campi: sessio TAB id TAB parens TAB titulus TAB valor TAB
 *        unitas TAB momentum, linea nova terminata.
 *
 * Redde: chorda vacua si titulus/unitas TAB aut lineam novam
 *        continet - forma quae se ipsam frangere sinit datum
 *        tacite corrumpit.
 */
chorda
mensura_ad_lineam (constans Mensura* mensura, Piscina* piscina);

/*
 * mensura_ex_linea - Lineam in mensuram vertere
 *
 * Redde: FALSUM si linea malformata est (campi pauciores, valor
 *        non numerus).
 */
b32
mensura_ex_linea (chorda linea, Mensura* exitus, Piscina* piscina);

/* ============================================================
 * Diarium et conditio
 * ============================================================ */

/*
 * mensura_annotare - Mensuram in diarium APPENDERE
 *
 * Via calida: apertio, appensio, clausura. Nullum sqlite.
 */
b32
mensura_annotare (constans character* via_diarii,
                  constans Mensura*   mensura,
                  Piscina*            piscina);

/*
 * mensura_condere - Diarium in volumen transferre
 *
 * Omnes lineae in actis voluminis conduntur (genus "mensura"),
 * transactione una. Diarium POST successum truncatur.
 *
 * Lineae malformatae NON silentur: numerantur et causa eas nominat.
 */
MensuraFructus
mensura_condere (constans character* via_diarii,
                 Volumen*            volumen,
                 Piscina*            piscina);

#endif /* MENSURA_H */
