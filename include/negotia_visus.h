/* negotia_visus.h - NEGOTIA in latere vici (genus "negotia"; iussum
 * $projects, $projects(<negotium>)); negotia-plan T3. Manifestum
 * apps/negotia/organum.stml; Negotia* communis per vicum (in volumine
 * vici pigre apertum, a parente datum); status visus in ramo.
 *
 * INDEX ("Projects"): linea per negotium, ordine activitatis -
 * titulus, summarium ("3 shipped · 2 partial · 4 planned · 2
 * conversations"). Vacuum: "No projects yet - create one with
 * `negotia creare`".
 *
 * NEGOTIUM ("<titulus>"): facultates ordine creationis - globulus
 * status (planned / partial / shipped), titulus. Vacuum: "No features
 * yet - add one with `negotia facultas`".
 *
 * FACULTAS ("<negotium> · <facultas>"): titulus, globulus status,
 * corpus (ad latitudinem lateris involutum), deinde "History": linea
 * per revisionem, recentissima prima - "2026-11-04 23:38 · shipped ·
 * <linea prima corporis>". Corpus et historia lineae unius seriei,
 * per paginas secta (numquam extra latus pictae); sagittae et [ ]
 * paginas vertunt. Esc ad facultates (electa = facultas haec); Esc
 * ibi ut supra.
 *
 * CHARTA COLLOQUII (genus virtuale "colloquium" anuli; T6): sessio
 * Claude finita aut ignota - titulus, directorium, tempus ultimum
 * ("2026-11-04 21:12"), linea status ultima; ignota: "Not in the
 * session log" cum id nudo. Directorium ex libro praeconii, aliter ex
 * nexu negotii (negotia_colloquii_via). Bottone "Resume" (clavis r):
 * porta colloquium_resumere(sessio, via); sine via obscurus. Sessio
 * non finita (non CLAUSUM, eventus intra XV minuta): "May still be
 * running outside tmux" - r primum armat, secundum resumit. Locus
 * comitis = "terminale/claude/<sessio>" (notae colloquii ipsius).
 * Argumentum "colloquium/<sessio>" (sessio [0-9A-Za-z-]+).
 *
 * PAGINA ADUMBRATIONIS (visus "adumbratio"; negotia-paginae T3):
 * titulus = titulus paginae; caput "Mock-up · <id VI>". Infra
 * adumbratio ab angulo sinistro supero ad mensuram declaratam
 * (absens: regio tota), ad regionem praecisa (coetus cum sectione).
 * Documentum malum: pagina vitii ("Mock-up error" + nuntius), numquam
 * vacuum. Redditio servata dum pagina (id, seq) et regio eaedem;
 * revisio alterius processus per pulsum (negotia_renovare -> seq
 * nova) iterum redditur. Locus comitis "negotia/pagina/<id plenum>";
 * titulus loci "<negotium> · <pagina>". Argumentum
 * "adumbratio/<pagina>" (id plenum aut suffixum unicum); pagina
 * ignota aut pictoris (T4): FALSUM, nihil mutatum.
 * REVISIONES (T3b): Cmd+K recentior, Cmd+J vetustior (negotia_
 * paginae_revisiones; recentissima summa). Campus "revisio" = seq
 * revisionis visae; vacuus = recentissima, quae revisionem novam
 * sequitur; vetus fixa manet dum revisiones crescunt. Cmd+K ad
 * recentissimam campum purgat. Extremis nihil fit. Caput
 * "Mock-up · <id VI> · rev N/M" (M > I solum). Argumentum
 * "adumbratio/<pagina>" revisionem purgat (nexus = recentissima).
 *
 * MUS PRIMUM: linea sub mure illuminatur (campus super); ictus unus
 * aperit (negotium in indice, facultas in negotio); sagittae paginarum
 * ictu. Claves: j k (electa), [ ] (paginae), Enter (aperire), Esc
 * (latus prius per vicum).
 *
 * Negotium ex indice apertum = porta negotium_aperire (vicus: anulus
 * tabulae = negotium, colloquia eius, deinde paginae activitate
 * recentissima prima - adumbratio ("negotia", "adumbratio/<id
 * plenum>"), pictor ("pictor", "negotia/<id plenum>"; activitas =
 * maximum seq paginae et pictor_documentum_ultimum); notae post T5 -
 * et latus novum in acervo). Anulus tabulae activae renovatur cum
 * negotia_versio movetur aut tabula mutatur; frons manet. Activitas
 * in renovatione legitur; negotia_versio omnia acta numerat, ergo
 * ictus picturae anulum pulsu proximo reordinat (frons et positio
 * manent).
 * $add-pictor / $add-pictor(titulus) (Franus 2026-11-06; in quavis
 * pagina scribae): negotium = anulus tabulae activae a ("negotia",
 * id) incipiens; pagina pictoris + documentum vacuum conduntur,
 * anulus renovatur, pagina in fronte. Titulus ordinarius "mood
 * board". Consumitur. Sine anulo negotii: nihil conditur, "add-pictor:
 * no project in this tab". Sine porta visus in loco mutatur
 * (historia impelle). Pulsus per M ms: negotia_renovare - scripturae
 * aliorum processuum (bin/negotia, alia arbor) intra secundum
 * apparent. */

#ifndef NEGOTIA_VISUS_H
#define NEGOTIA_VISUS_H

/* <aedilis corpus="lib/negotia_visus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "negotia.h"
#include "colloquia.h"

/* portae hospitis; VERUM si acta */
nomen structura {
    vacuum* ctx;
    b32 (*negotium_aperire)(vacuum* ctx, chorda negotium);
    /* T6: colloquium ex libro praeconii; FALSUM si ignotum */
    b32 (*colloquium_legere)(vacuum* ctx, chorda sessio, Piscina* p,
        Colloquium* out);
    /* T6: terminale "claude --resume" in directorio via */
    b32 (*colloquium_resumere)(vacuum* ctx, chorda sessio, chorda via);
} NegotiaVisusPortae;

nomen structura {
                    InsulaRamus  ramus;
                 ActioRegistrum* actiones;
                FiguraRegistrum* figurae;
                        Negotia* negotia;  /* communis (a parente) */
    constans NegotiaVisusPortae* portae;  /* NIHIL licet */
                            s64  versio_visa; /* negotia_versio ad
                                              * pulsum ultimum */
                      Piscina* piscina;
          InternamentumChorda* intern;
                       vacuum* figurae_ctx; /* privatum */
} NegotiaMontatio;

/* montare in ramo <negotia id="..."> (manifestum infixum); superficies
 * latitudo x altitudo (0 = ordinaria); portae, systemata NIHIL
 * licent */
b32
negotia_visus_montare (
                NegotiaMontatio* m,
                        Piscina* piscina,
            InternamentumChorda* intern,
             InsulaRepositorium* repo,
             constans character* id,
                        Negotia* negotia,
    constans NegotiaVisusPortae* portae,
                            i32  latitudo,
                            i32  altitudo,
                StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = NegotiaMontatio* */
Componens*
negotia_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum lateris: URI semitae ("negotia/index",
 * "negotia/negotium/<id>") aut id negotii nudum / suffixum unicum
 * (-> negotia/negotium/<id plenum>) aut vacuum (-> index). Ignotum
 * aut ambiguum: FALSUM, nihil mutatum */
b32
negotia_visus_argumentum_ponere (
       NegotiaMontatio* m,
    constans character* argumentum);

/* pulsus lateris visibilis: negotia_renovare; VERUM si negotia_versio
 * mota est post pulsum huius lateris ultimum (Negotia* communis: latus
 * alterum renovationem consumere potest) */
b32
negotia_visus_reficere (
    NegotiaMontatio* m);

/* quies: M ms (scripturae alienae signum non mittunt) */
s64
negotia_visus_mora (
    constans NegotiaMontatio* m);

/* comites (facies.locus; ctx = NegotiaMontatio*): charta colloquii ->
 * "terminale/claude/<sessio>"; pagina adumbrationis ->
 * "negotia/pagina/<id plenum>"; aliter vacua (locus ordinarius) */
chorda
negotia_visus_locus (
     vacuum* ctx,
    Piscina* piscina);

/* comites P1 (facies.locus_titulus; ctx = NegotiaMontatio*):
 * "Projects" aut titulus negotii aperti; pagina adumbrationis ->
 * "<negotium> · <pagina>" */
chorda
negotia_visus_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

/* comites P9 (facies.clavem_sumere; ctx = NegotiaMontatio*): clavis
 * Super quam tabula praeceptorum in visu currenti habet (Cmd+J /
 * Cmd+K in pagina adumbrationis); agere FALSUM = quaestio sola,
 * agere VERUM = praeceptum agitur */
b32
negotia_visus_clavem_sumere (
               vacuum* ctx,
     constans Eventus* ev,
                  b32  agere);

#endif /* NEGOTIA_VISUS_H */
