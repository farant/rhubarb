/* briar_plagulae.h - Membra aedificationis: plagulae per elementum
 * bibliothecae adductae (spec par. 3.5, v1.11; lapide
 * feature-requests/015). Elementum STML 'bibliotheca' cum attributo
 * 'via' (relativa contra directorium plagulae IMPORTANTIS, numquam
 * contra directorium currens; absoluta ut data) plagulam aliam
 * nominat; eius regiones C PLANAE (non fragmentum, non munus
 * "probatio", sine methodus) importanti praebentur.
 *
 * Collectio TRANSITIVA: elementa membrorum quoque sequuntur; membra
 * ordine POST-ORDINIS redduntur (folia primum), quodque SEMEL (via
 * absoluta normalizata = identitas); radix ipsa membrum non est.
 * Circulus refutatur - politia, non mechanica: typi per capita genita
 * inter se inclusa non compilarent (spec par. 3.5, nota B6).
 *
 * Membrum quodque POST dependentia sua perficitur: silva eius cum
 * membris VISIBILIBUS solis (dependentia transitiva - frater non
 * importatus non videtur), deinde partitio (briar_membrum_partiri),
 * nomina publica, textus parsurae cum membris interioribus insertis.
 *
 * Refutationes (causa + via plagulae importantis + linea elementi):
 * via absens, suffixum non '.thistle', plagula non exsistens, circulus
 * (catena nominata cum remedio), membrum sine regione C plana,
 * titulus iteratus (plagulae genitae eiusdem nominis), regio membri
 * non parsata aut non partita (causa eius, linea eius).
 */

#ifndef BRIAR_PLAGULAE_H
#define BRIAR_PLAGULAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "silex.h"
#include "briar_silva.h"

/* membrum aedificationis */
nomen structura {
     constans character* via;              /* absoluta, normalizata */
     constans character* titulus;          /* basis sine ".thistle" */
                 chorda  octeti;           /* plagula tota (clavis) */
           MateriaNodus* documentum;
                    Xar* nexus;             /* BriarNexusRes, silva
                                             * texta */
     constans character* via_importantis;  /* primus qui adduxit */
                    i32  linea_elementi;
    /* post dependentia perfecta (silva cum visibilibus sola): */
                 chorda caput;            /* include/<t>_regiones.h */
                 chorda caput_parsurae;   /* sine lineis, membra
                                             * interiora inserta */
                 chorda  corpus;           /* fontes/<t>_regiones.c */
                    Xar* derivata;         /* chorda: capita derivata */
                    Xar* nomina;           /* BriarNomenPublicum */
                    Xar* statica;          /* BriarNomenPublicum:
                                             * interiora (amalgama) */
                    Xar* visibilia;        /* BriarMembrum*: dependentia
                                             * transitiva, sola visa */
} BriarMembrum;

/* refutatio collectionis: causa + ubi (plagula et linea elementi) */
nomen structura {
                 chorda  causa;
     constans character* via;
                    i32  linea;
} BriarMembraCausa;

/* Membra radicis colligere (Xar de BriarMembrum in *membra, ordine
 * post-ordinis). via_radicis relativa contra directorium currens
 * absoluta fit. FALSUM: *causa posita, *membra NIHIL. */
b32
briar_membra_colligere (
                 Piscina*  piscina,
      constans character*  via_radicis,
                     Xar*  nexus_radicis,
     InternamentumChorda*  intern,
      constans SilexFons*  fons,
                     Xar** membra,
        BriarMembraCausa*  causa);

/* collectio LEVIS (clavis ante parsuram, tools/briar.c): eadem
 * ambulatio, eadem ordo post-ordinis, eaedem refutationes structurae
 * (via, circulus, absens, sine regione plana, titulus iteratus), sed
 * SINE silva et partitione - membra titulum, viam, octeti, nexum,
 * visibilia ferunt; caput/corpus/nomina/statica NIHIL. Stampa
 * (briar_membra_stampa) eadem ac collectionis plenae. */
b32
briar_membra_colligere_levia (
                 Piscina*  piscina,
      constans character*  via_radicis,
                     Xar*  nexus_radicis,
     InternamentumChorda*  intern,
                     Xar** membra,
        BriarMembraCausa*  causa);

/* tabula derivationis ex membris (Xar de BriarMembrum): nomina
 * publica -> caput, caput -> textus parsurae (briar_silva). Radix
 * omnia membra videt (ea ipsa adduxit). */
BriarSilvaMembra*
briar_membra_silvae (
    Piscina* piscina,
        Xar* membra);

/* stampa clavis cache cum membris (spec par. 3.5: clavis omnes
 * bibliothecas transitive continet): stampa + " membra " + XVI hex
 * SHA-256 (titulus, mensura, octeti membri cuiusque, post-ordine).
 * Sine membris stampa IPSA redditur - claves plagularum sine
 * bibliotheca non moventur. */
constans character*
briar_membra_stampa (
               Piscina* piscina,
    constans character* stampa,
                   Xar* membra);

#endif /* BRIAR_PLAGULAE_H */
