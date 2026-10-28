/* navigator.h - NAVIGATOR in latere vici (genus "navigator"; spec
 * project-specs/navigator-spec.md N3.2). Manifestum
 * apps/navigator/organum.stml; bibliotheca (navigator_bibliotheca.h)
 * per ligamen a parente, communis omnibus lateribus; status visus in
 * ramo.
 *
 * INDEX (N3.4: FAVORES solum, more Yahoo): RADIX "Saved" - grex
 * "Bookmarks", linea per dominium radicale ("wikipedia.org", numerus
 * favorum); Enter (aut duplex) descendit. Sub eo grex "Recent"
 * (N3.4b): articuli nuper aperti (navigator_recentes), titulus et
 * hospes; Enter aperit, b favorem vertit; sine favoribus "Bookmarks"
 * nuntium "No bookmarks yet" habet. DOMINIUM: favores eius per
 * hospitem grex (titulus gregis in pagina continuationis iteratus),
 * linea per articulum - titulus versionis ultimae, "3 versions".
 * Claves: j k (electus), [ ] (paginae), Enter (articulus, versio
 * ultima, pagina servata), b (favor electi), Esc (ad radicem; in radice
 * latus per vicum). Dominium radicale = duo nomina ultima hospitis
 * (Vicipaedia recta; co.uk etc. Public Suffix List postea).
 * PAGINA: articulus paginatus (navigator_paginae.h + lector_paginare),
 * in linea ima titulus, hospes, "3/7", stella (clara si favor; ictus
 * vertit). Claves: j k (pagina proxima / prior), b (favor), Esc (ad
 * indicem). Extractio et paginae in cache per
 * (articulus, versio, area).
 * NEXUS (N4.1): textus nexuum (notae ARTICULUS_NEXUS - in articulo et
 * in indice nexuum) colore nexuum vici pingitur (COLORATIO_LINK, idem
 * ac #rubricae in editore: thema_color_ex_indice_colorationis);
 * linea quaeque in segmenta per notas finditur, ergo nexus qui in
 * lineam sequentem fluit in utraque coloratur.
 * MUS IN NEXU (N4.2): ictus in nexu (articulus aut index nexuum) eum
 * aperit per viam URL - servatus statim, Vicipaedia petitur, alienus
 * "Unsupported site: <hospes/via>" in pede (rubrum). Ictus: linea ->
 * columna (runae, fons lineae) -> offset -> nota NEXUS; extra nexum
 * nihil. Historia vici (Ctrl+O) ad articulum priorem. Clavis N4.3.
 * POSITIO (N3.4c): omnis motus paginae et apertio positionem servat
 * (navigator_positionem_scribere: versio, octetus primus paginae,
 * index nexuum?). Apertio sine pagina nominata (index, URL, quaestio,
 * argumentum nudum) paginam eam continentem restituit - area mutata
 * quoque; versio alia: pagina prima. Locus cum pagina (historia,
 * comes, notae) paginam suam tenet nec positionem mutat.
 * NOTAE: locus "navigator/pagina/<articulus>?versio=<v>&pagina=3" -
 * versio in loco, ergo comes (Cmd+T) versionis suae est (spec V.11,
 * V.16); notae in pede ut lector (notas_habet hospitis).
 * APERIRE (N3.3): 'o' (index, pagina) aut globulus "Open URL..." in
 * capite indicis promptam "url:" aperit; Enter: URL. $nav(URL) idem
 * (argumentum cum "://"). Articulus iam servatus statim aperitur, sine
 * rete (servatum, non vivum - spec I; renovatio N3.6). Aliter
 * navigator_petere in reactore hospitis; dum petitur nota "Fetching
 * <hospes/via>..." (obscura); SERVATA -> articulus apertus (versio
 * fixa); NON_INVENTA "Not found", IGNOTA "Unsupported site", RETE causa
 * - nota (rubra). Petitio nova pendentem abrumpit.
 * QUAESTIO (N3.3b): textus sine "://" in prompta = quaestio
 * Vicipaediae (navigator_quaerere); "la: Roma" hospitem eligit
 * (praefixum linguae), aliter hospes articuli currentis (pagina) aut
 * en.wikipedia.org. Nota "Searching <hospes> for '<verba>'...";
 * EVENTUS: linea per eventum (titulus - descriptio), claves j k [ ]
 * Enter (articulus per viam URL: servatus statim, aliter petitur), Esc
 * (ad paginam si articulus, aliter indicem); mus: ictus eligit, duplex
 * aperit. Nulli: "No results". Restitutio in eventibus quaestionem
 * iterat. */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

/* <aedilis corpus="lib/navigator.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "navigator_bibliotheca.h"
#include "navigator_petitio.h"
#include "reactor.h"

nomen structura {
              InsulaRamus  ramus;
           ActioRegistrum* actiones;
          FiguraRegistrum* figurae;
     NavigatorBibliotheca* bibliotheca;   /* ligamen (a parente) */
                  Piscina* piscina;
      InternamentumChorda* intern;
                   vacuum* figurae_ctx;   /* privatum: cache */
                      b32  (*notas_habet)(vacuum* ctx, chorda locus);
                   vacuum* notas_habet_ctx;
    /* N3.3: reactor hospitis pro petitionibus (vicus: reactor vici);
     * NIHIL aut redditum NIHIL = petere non potest ("offline") */
                 Reactor* (*reactor)(vacuum* ctx);
                   vacuum* reactor_ctx;
    /* PROBATIONES: optiones petitionis (radix_fontis, agens); NIHIL =
     * ordinariae (agens ex ~/.rhubarb/navigator/agens) */
    constans NavigatorPetitioOptiones* optiones_petitionis;
} NavigatorMontatio;

/* montare in ramo <navigator id="..."> (manifestum infixum);
 * superficies latitudo x altitudo (0 = ordinaria); systemata NIHIL
 * licet */
b32
navigator_montare (
       NavigatorMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
    NavigatorBibliotheca* bibliotheca,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = NavigatorMontatio* */
Componens*
navigator_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum: URI semitae, identitas articuli nuda (-> versio ultima,
 * pagina servata - N3.4c), vacuum (index). Mala: FALSUM, nihil
 * mutatum. */
b32
navigator_argumentum_ponere (
      NavigatorMontatio* m,
     constans character* argumentum);

/* facies.pulsare: VERUM semel post petitionem finitam (vicus
 * recomponit); ceterum FALSUM. Petitiones finitae hic dimittuntur. */
b32
navigator_pulsare (
    vacuum* ctx);

/* facies.mora: 0 si petitio finita pulsum exspectat, aliter -1
 * (reactor fossas petitionis ipse vigilat) */
s32
navigator_mora (
    vacuum* ctx);

/* facies.semitam_secuta (historia, $via, restitutio) */
b32
navigator_semitam_secuta (
    vacuum* ctx);

/* facies.locus */
chorda
navigator_locus (
     vacuum* ctx,
    Piscina* piscina);

/* "Rhubarb - en.wikipedia.org - 3/7"; index: "Saved" (radix), "Saved -
 * wikipedia.org" (dominium) */
chorda
navigator_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* NAVIGATOR_H */
