/* insula.h - Insulae: status in tribus generibus, porta una per genus
 *
 * Veritas est ARBOR STML in piscina, per hoc repositorium sola
 * accessa (ludus-brainstorm.md XIII, DECISUS). Genera: DURABILIS
 * (quod basis datorum teneret) et EPHEMERA (quod restitutiones
 * supervivit). MOTUS non est insula - structura in memoria
 * (motus.h), in quiete huc effusa.
 *
 * PORTA UNICA: mutare_durabile / mutare_ephemera. Mutator in
 * DUPLICATO currit (scribere+legere in piscinam alteram), canon
 * iudicat, deinde permutatio - scriptura aut tota aut nulla.
 *
 * LEX REHYDRATIONIS: post omnem scripturam per portam textus
 * ultimus servatur; restituere arborem in memoria cum eo confert -
 * discrepantia = MENDACIUM (scriptor extra portam) - deinde ex
 * textu honesto reficit.
 *
 * <componens/>-lint L1: extra has portas nemo insulam mutat.
 */

#ifndef INSULA_H
#define INSULA_H

/* <aedilis corpus="lib/insula.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "xar.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen enumeratio {
    INSULA_DURABILIS = ZEPHYRUM,
    INSULA_EPHEMERA,
    INSULA_GENUS_NUMERUS
} InsulaGenus;

/* Mutator: in DUPLICATO radicis currit; piscina et intern eius
 * sunt in quibus nodi novi nascantur. */
nomen vacuum (*InsulaMutator)(
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* Vocatur post omnem scripturam per portam successam - pictor P3
 * hic acta appendit. */
nomen vacuum (*InsulaActarius)(
    InsulaGenus  genus,
     StmlNodus*  radix,
        vacuum*  ctx);

nomen structura {
    chorda attributum;
    chorda dominus;     /* scriptor solus qui attributum mutat */
    chorda elementum;   /* ramus (R1): vacua = radix */
    chorda id;
} InsulaDominus;

nomen structura {
                Piscina* piscina;      /* repositorii ipsius */
    InternamentumChorda* intern;
                         /* ping-pong per genus */
                Piscina* piscinae[INSULA_GENUS_NUMERUS][II];
                    i32  activa[INSULA_GENUS_NUMERUS];
              StmlNodus* radices[INSULA_GENUS_NUMERUS];
                 chorda  textus_ultimus[INSULA_GENUS_NUMERUS];
                    i32  versio[INSULA_GENUS_NUMERUS];
                  Canon* canones[INSULA_GENUS_NUMERUS];
         InsulaActarius  actarius;
                 vacuum* actarius_ctx;
                    b32  mendacium;
                 chorda  causa;
                    Xar* domini[INSULA_GENUS_NUMERUS];
                 chorda  scriptor;      /* currens; vacuus = anonymus */
                    Xar* canones_ramorum[INSULA_GENUS_NUMERUS]; /* R1 */
} InsulaRepositorium;

/* RAMUS (insula-rami-plan R1): repositorium + liberum radicis per
 * elementum et id (vacua = radix ipsa). Lectio et scriptura relativae
 * ad nodum rami - applicatio locum suum ignorat; duo rami eiusdem
 * generis duae instantiae. Domini et canones per ramum; canon radicis
 * ramos canonem proprium habentes non videt (iudicium suum habent).
 * Ramus radicis == API vetus. */
nomen structura {
     InsulaRepositorium* repo;
                 chorda  elementum;   /* vacua = radix */
                 chorda  id;
} InsulaRamus;


/* ==================================================
 * Creatio et lectio
 * ================================================== */

InsulaRepositorium*
insula_repositorium_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* durabilis_stml,
     constans character* ephemera_stml);

/* LECTIO SOLA. Mutare per hunc pointer = mendacium. */
StmlNodus*
insula_radix (
    InsulaRepositorium* repo,
           InsulaGenus  genus);

/* Attributum radicis; NIHIL si abest. Pointer in internamentum. */
chorda*
insula_attributum (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus);

i32
insula_versio (
    constans InsulaRepositorium* repo,
                    InsulaGenus  genus);

chorda
insula_scribere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
               Piscina* piscina);


/* ==================================================
 * Rami (R1)
 * ================================================== */

InsulaRamus
insula_ramus_radix (
    InsulaRepositorium* repo);

/* liberum radicis <elementum id="id"/> (elementum NIHIL = radix) */
InsulaRamus
insula_ramus (
    InsulaRepositorium* repo,
    constans character* elementum,
    constans character* id);

/* LECTIO SOLA; NIHIL si ramus in genere abest */
StmlNodus*
insula_ramus_nodus (
    constans InsulaRamus* ramus,
             InsulaGenus  genus);

chorda*
insula_ramus_attributum (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
      constans character* titulus);

/* porta rami: mutator NODUM RAMI in duplicato radicis accipit; domini
 * et canones (radicis et ramorum) iudicant ut semper. FALSUM si ramus
 * abest (causa nominata). */
b32
mutare_ramum (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
           InsulaMutator  fn,
                  vacuum* ctx);

b32
insula_ramus_dominum_ponere (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
      constans character* attributum,
      constans character* dominus);

/* ut insula_dominos_legere, lineae ad ramum datum */
i32
insula_ramus_dominos_legere (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
               StmlNodus* domini);

/* canon nodum rami iudicat (radix eius = elementum rami); canon
 * radicis eum non videt */
vacuum
insula_ramus_canonem_ponere (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
                   Canon* canon);


/* ==================================================
 * Portae
 * ================================================== */

b32
mutare_durabile (
    InsulaRepositorium* repo,
         InsulaMutator  fn,
                vacuum* ctx);

b32
mutare_ephemera (
    InsulaRepositorium* repo,
         InsulaMutator  fn,
                vacuum* ctx);

/* Attributum PONERE - substituit si adest, addit si abest
 * (stml_attributum_addere semper appendit; capere primum reddit).
 * Forma stml_attributum_addere servata ut mutator argumenta sua
 * transferat. Intra portam solum honestum. */
b32
insula_attributum_ponere (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
     constans character* valor);


/* ==================================================
 * Restitutio et mendacium
 * ================================================== */

/* VERUM = honestum. FALSUM = arbor extra portam mutata erat:
 * mendacium notatur, arbor ex textu honesto refecta. */
b32
insula_restituere (
    InsulaRepositorium* repo);

b32
insula_mendacium (
    constans InsulaRepositorium* repo);

chorda
insula_causa (
    constans InsulaRepositorium* repo);

vacuum
insula_ponere_canonem (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
                 Canon* canon);

vacuum
insula_ponere_actarium (
    InsulaRepositorium* repo,
        InsulaActarius  fn,
                vacuum* ctx);


/* ==================================================
 * Domini (brainstorm XVI §2: lectio globalis, scriptura possessa)
 * ================================================== */

/* Scriptor currens (nomen actionis, 'dispensator', 'motus', ...);
 * vacuus = anonymus. Dispensator eum circa tractatorem ponit. */
vacuum
insula_scriptorem_ponere (
    InsulaRepositorium* repo,
                chorda  scriptor);

/* Attributum radicis generis dato uni scriptori addicere. FALSUM si
 * attributum vacuum. */
b32
insula_dominum_ponere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* attributum,
    constans character* dominus);

/* <domini><dominus genus attributum scriptor/>...</domini>: lineae
 * generis dati leguntur; redde numerum lectarum. */
i32
insula_dominos_legere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
             StmlNodus* domini);

/* Attributum radicis tollere (par insula_attributum_ponere). */
b32
insula_attributum_tollere (
              StmlNodus* nodus,
     constans character* titulus);

#endif /* INSULA_H */
