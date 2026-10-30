/* chronica_fontes.h - eventa omnium librorum in tabulam unam
 * (chronica C2; project-specs/chronica-spec.md)
 *
 * Fontes: libri comites ('comes/<locus>/paginae', locus '/' continet)
 * et liber communis vici (index 'scriba/paginae', paginae
 * 'paginae/<n>'). Conspectus unus per librum
 * (lib/conspectus_eventa_*.stml), non quaerendi:
 *
 *   eventa_comitis/<locus>    eventa libri comitis
 *   eventa_libri/<pagina>     eventa paginae libri communis
 *
 * Pagina mutata inscriptionem unam irritat; ceterae RECENTES manent.
 * Tabula = unio omnium, COPIATA in piscinam suam (conspectus per
 * tectum eiecti eam non tangunt); reaedificatur solum si auditor
 * clavem nuntiavit post aedificationem priorem. Eventa vitiosa
 * omittuntur (C6 ea in scriba colorat). LECTIO SOLA. */

#ifndef CHRONICA_FONTES_H
#define CHRONICA_FONTES_H

/* <aedilis corpus="lib/chronica_fontes.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "conspectus.h"
#include "chronica_eventus.h"

nomen structura {
    ChronicaEventus eventus;
             chorda liber;          /* "comes/<locus>"; vacua =
                                      * communis */
             chorda pagina;         /* nomen paginae */
             chorda titulus_libri;  /* nota comitis ("Notes ·
                                      * Thomas Aquinas"), absens:
                                      * locus; communis: "Notebook" */
} ChronicaRes;

/* Ordo: annus ascendens; intra annum res cum mense primum (mensis,
 * dies; dies absens ante diem 1), deinde ceterae ordine libri -
 * communis primum, comites ordine loci (octeti), paginae ordine
 * indicis, eventa ordine textus. */
nomen structura {
    Xar* res;    /* ChronicaRes */
    Xar* anni;   /* s32 distincti, ascendentes */
} ChronicaTabula;

nomen structura ChronicaFontes ChronicaFontes;   /* opacum */

/* structura in piscina vocatoris; tabula in piscina propria (in
 * reaedificatione et destructione liberata). NIHIL si argumenta
 * mala. */
ChronicaFontes*
chronica_fontes_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen);

/* declarationes duas cum operibus et auditoribus ponere (ante
 * conspectus_parare); registrum memoratur. *vitia SEMPER impletum. */
b32
chronica_registrare (
    ConspectusRegistrum*  repertorium,
         ChronicaFontes*  fontes,
                    Xar** vitia);

/* tabula recens: conspectus_renovare (auditores acta nova audiunt),
 * reaedificat si quid stalum, aliter EANDEM reddit. Valet usque ad
 * chronica_tabula proximam quae reaedificat. Post registrare numquam
 * NIHIL (tabula vacua licet). */
constans ChronicaTabula*
chronica_tabula (
    ChronicaFontes* fontes);

/* res personae / loci: titulus praecisus, congruentia exacta
 * (octeti), ordine tabulae. Xar de ChronicaRes in piscina data;
 * chordae in tabula - valent ut tabula. */
Xar*
chronica_res_personae (
    constans ChronicaTabula* tabula,
                     chorda  titulus,
                    Piscina* piscina);

Xar*
chronica_res_loci (
    constans ChronicaTabula* tabula,
                     chorda  titulus,
                    Piscina* piscina);

/* piscinam tabulae reddere; NIHIL nihil agit */
vacuum
chronica_fontes_destruere (
    ChronicaFontes* fontes);

#endif /* CHRONICA_FONTES_H */
