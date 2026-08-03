/* <sutura/> interfacies webview portabilis - implementatio per-platformam */
/* vitrea.h - vitreae fenestrarum: WKWebView super Fenestram
 * (vitrea Phasis B; spec: project-specs/vitrea-spec-v2.md)
 *
 * Stratum applicationum webview possessum: fenestra nativa +
 * WKWebView systematis + pons chordarum JS<->C. Frons intra
 * binarium per capsulam vehitur et super schema capsula://
 * servitur; posterius est C89 rhubarb planum. FINIS PERPETUUS:
 * vitrea est adhaesio texturae + origo assetorum + transportus
 * chordarum, IN AETERNUM - menus, dialogi, notificationes
 * numquam vitreae sunt.
 *
 * VITREA FENESTRAM NON CREAT: vitrea_creare texturam WKWebView ut
 * contentView fenestrae datae installat (visus pixellorum
 * fenestrae depellitur - Fenestra vitream hospitans est
 * SOLUM-TEXTURA per contractum). ORDO DESTRUCTIONIS:
 * vitrea_destruere ANTE fenestra_destruere.
 *
 * ORIGO: capsula://radix/<via> - auctoritas ficta 'radix' URLs
 * relativa sanat (verdictum calibrationis 2026-07-20). Portitor
 * vias normalizat ('/' ductor demitur; vacua -> via_initialis),
 * claves capsulae exacte congruit, responsa NSHTTPURLResponse-200
 * reddit (responsum planum fetch status=0 daret - verdictum
 * geminum). Paginae capsula:// sunt secure contexts.
 *
 * PONS: nuntii scripti JS in caudam anuli (CCLVI) copiantur -
 * tractatores C intra vocamina WebKit NUMQUAM currunt; app
 * explicite exhaurit (vitrea_obtinere_nuntium) suo gyro ansae.
 * Superfluitas = amissio + clamor (nuntii_amissi +
 * internuntius._superfluum in JS). Nuntius adveniens eventum
 * syntheticum ponit ut pumpa obstructa (fenestra_expectare_
 * eventus) expergiscatur.
 *
 * MEMORIA NUNTIORUM: chordae exhaustae vivunt in piscina nuntiorum
 * vitreae; piscina REFICITUR cum nuntius ad caudam VACUAM advenit
 * - ergo chordae exhaustae valent donec nuntius novus post
 * exhaustionem completam adveniat. TRACTA STATIM, NUMQUAM
 * RECONDE.
 *
 * NAVIGATIO (Phasis C): navigatio ad originem configuratam SOLAM
 * conceditur (schema+hospes+portus; 'about' exemptum - paginae
 * internae WebKit); cetera recusantur et numerantur
 * (navigationes_recusatae). Nexus externi in v1 mortui sunt -
 * aperire-externum bibliotheca futura est (spec par XIV). */

#ifndef VITREA_H
#define VITREA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fenestra.h"
#include "capsula.h"

nomen structura Vitrea Vitrea;

nomen enumeratio {
    VITREA_ORIGO_CAPSULA = 0,
    VITREA_ORIGO_URL             /* modus-dev: URL http vivum */
} VitreaOrigo;

nomen structura {
    VitreaOrigo         origo;
    Capsula*            capsula;        /* ORIGO_CAPSULA */
    constans character* via_initialis;  /* "index.html" */
    constans character* url;            /* ORIGO_URL */
    b32                 inspectabilis;  /* Inspector Safari;
                                         * macOS 13.3+ */
} VitreaConfiguratio;

/* numeratores fructus (mensura est productum) */
nomen structura {
    i64 nuntii_recepti;       /* nuntii pontis JS->C */
    i64 nuntii_amissi;        /* cauda plena - amissi */
    i64 aestimationes;        /* evaluationes JS C->JS */
    i64 petitiones_capsulae;  /* petitiones schematis servitae */
    i64 petitiones_fractae;   /* 404 - via in capsula absens */
    i64 navigationes_recusatae; /* navigationes extra originem */
} VitreaFructus;

nomen enumeratio {
    VITREA_NUNTIUS_PONS = 0,   /* nuntius pontis JS->C (textus) */
    VITREA_NUNTIUS_INTERITUS   /* processus interretialis obiit -
                                * app decernit (plerumque
                                * vitrea_recargare) */
} VitreaNuntiusGenus;

Vitrea*
vitrea_creare (
    Piscina*                     piscina,
    Fenestra*                    fenestra,
    constans VitreaConfiguratio* configuratio);

/* ANTE fenestra_destruere vocanda */
vacuum
vitrea_destruere (
    Vitrea* vitrea);

/* caudam pontis exhaurire: VERUM dum nuntii manent. Chordae
 * redditae valent per contractum memoriae supra - tracta statim.
 * Pro INTERITU chorda vacua est. EXHAURI AD VACUUM in quoque gyro
 * ansae. */
b32
vitrea_obtinere_nuntium (
    Vitrea*             vitrea,
    chorda*             nuntium,
    VitreaNuntiusGenus* genus);

/* aestimatio JS cruda (filum principale; ignis-et-obliviscere) */
vacuum
vitrea_aestimare (
    Vitrea* vitrea,
    chorda  javascript);

/* missor pro internuntio: textum effugit, in
 * internuntius._accipere("...") involvit, aestimat. datum =
 * Vitrea*. Usus:
 *   inx = internuntius_creare(piscina, vitrea_missor, vitrea); */
vacuum
vitrea_missor (
    vacuum* datum,
    chorda  textus);

/* aestimator seam-formis (gemellus vitrea_missor): datum = Vitrea*,
 * JS crudum evaluat. Pro consumptoribus sine typo Vitrea (e.g.
 * speculum). Usus:
 *   speculum_creare(piscina, &capsula, inx, vitrea_aestimator, vitrea); */
vacuum
vitrea_aestimator (
    vacuum* datum,
    chorda  js);

vacuum
vitrea_recargare (
    Vitrea* vitrea);

VitreaFructus
vitrea_fructus (
    constans Vitrea* vitrea);

#endif /* VITREA_H */
