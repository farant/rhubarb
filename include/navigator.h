/* navigator.h - NAVIGATOR in latere vici (genus "navigator"; spec
 * project-specs/navigator-spec.md N3.2). Manifestum
 * apps/navigator/organum.stml; bibliotheca (navigator_bibliotheca.h)
 * per ligamen a parente, communis omnibus lateribus; status visus in
 * ramo.
 *
 * INDEX: articuli servati per hospitem grex (titulus gregis in pagina
 * continuationis iteratus), linea per articulum - titulus versionis
 * ultimae, "3 versions". Claves: j k (electus), [ ] (paginae), Enter
 * (articulus, versio ultima, pagina prima), Esc (latus per vicum).
 * PAGINA: articulus paginatus (navigator_paginae.h + lector_paginare),
 * in linea ima titulus, hospes, "3/7". Claves: j k (pagina proxima /
 * prior), Esc (ad indicem). Extractio et paginae in cache per
 * (articulus, versio, area).
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
 * - nota (rubra). Textus sine "://": "Not a URL". Petitio nova
 * pendentem abrumpit. */

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
 * pagina prima), vacuum (index). Mala: FALSUM, nihil mutatum. */
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

/* "Rhubarb - en.wikipedia.org - 3/7"; index: "Saved articles" */
chorda
navigator_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* NAVIGATOR_H */
