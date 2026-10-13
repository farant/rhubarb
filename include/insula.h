/* insula.h - Insula: status organorum in arbore UNA, porta una
 *
 * Veritas est ARBOR STML in piscina, per hoc repositorium sola
 * accessa (ludus-brainstorm.md XIII, DECISUS). C2 (status-plan,
 * 2026-10-12): genera DURABILIS / EPHEMERA sublata - vita per campum
 * in manifesto (organum: persistens, transiens, probatio), non per
 * arborem; documenta (quod basis datorum teneret) in voluminibus
 * (historia). MOTUS non est insula - structura in memoria (motus.h),
 * in quiete huc effusa.
 *
 * PORTA UNICA: insula_mutare (et mutare_ramum). Mutator in
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
    StmlNodus*  radix,
       vacuum*  ctx);

nomen structura {
    chorda attributum;
    chorda dominus;     /* scriptor solus qui attributum mutat */
    chorda systemata;   /* S5a: systemata quoque licita, spatiis
                         * separata ("relatio"); vacua = nulla */
    chorda via;         /* S8: ramus domini; vacua = radix */
} InsulaDominus;

nomen structura {
                Piscina* piscina;      /* repositorii ipsius */
    InternamentumChorda* intern;
                Piscina* piscinae[II];      /* alternae (scriptura) */
                    i32  activa;
              StmlNodus* radix;
                 chorda  textus_ultimus;
                    i32  versio;
                  Canon* canon;
         InsulaActarius  actarius;
                 vacuum* actarius_ctx;
                    b32  mendacium;
                 chorda  causa;
                    Xar* domini;
                 chorda  scriptor;      /* currens; vacuus = anonymus */
                    Xar* canones_ramorum; /* R1 */
} InsulaRepositorium;

/* RAMUS (insula-rami-plan R1; status-plan S8: quavis profunditate):
 * repositorium + via a radice - segmenta "elementum:id" per '/'
 * iuncta, internata (vacua = radix). elementum et id = segmentum
 * ultimum. Lectio et scriptura relativae ad nodum rami - applicatio
 * locum suum ignorat; duo rami eiusdem generis duae instantiae.
 * Domini et canones per ramum; canon nodum suum iudicat sine ramis
 * descendentibus qui canonem proprium habent (proximus vincit).
 * Ramus radicis == API vetus. */
nomen structura {
     InsulaRepositorium* repo;
                 chorda  via;         /* S8: internata; vacua = radix */
                 chorda  elementum;   /* segmentum ultimum */
                 chorda  id;
} InsulaRamus;


/* ==================================================
 * Creatio et lectio
 * ================================================== */

InsulaRepositorium*
insula_repositorium_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* stml);

/* LECTIO SOLA. Mutare per hunc pointer = mendacium. */
StmlNodus*
insula_radix (
    InsulaRepositorium* repo);

/* Attributum radicis; NIHIL si abest. Pointer in internamentum. */
chorda*
insula_attributum (
    InsulaRepositorium* repo,
    constans character* titulus);

i32
insula_versio (
    constans InsulaRepositorium* repo);

chorda
insula_scribere (
    InsulaRepositorium* repo,
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

/* S8: ramus liberi - <elementum id="id"/> inter liberos nodi rami
 * parentis. Organum quod liberos montat ramum SUUM dat; insula_ramus
 * = insula_ramus_liberi radicis. */
InsulaRamus
insula_ramus_liberi (
    constans InsulaRamus* parens,
      constans character* elementum,
      constans character* id);

/* LECTIO SOLA; NIHIL si ramus abest */
StmlNodus*
insula_ramus_nodus (
    constans InsulaRamus* ramus);

chorda*
insula_ramus_attributum (
    constans InsulaRamus* ramus,
      constans character* titulus);

/* porta rami: mutator NODUM RAMI in duplicato radicis accipit; domini
 * et canones (radicis et ramorum) iudicant ut semper. FALSUM si ramus
 * abest (causa nominata). */
b32
mutare_ramum (
    constans InsulaRamus* ramus,
           InsulaMutator  fn,
                  vacuum* ctx);

b32
insula_ramus_dominum_ponere (
    constans InsulaRamus* ramus,
      constans character* attributum,
      constans character* dominus);

/* ut insula_dominos_legere, lineae ad ramum datum */
i32
insula_ramus_dominos_legere (
    constans InsulaRamus* ramus,
               StmlNodus* domini);

/* ramum initiare (T1a): ramus liberi - elementum (STML) nodo
 * PARENTIS additur si abest (exstans intactum); parens absens =
 * FALSUM (causa nominata, S8); ramus radicis - attributa elementi quae
 * radici absunt adduntur. Per portam: canones et domini iudicant
 * (canonem rami ANTE initiationem ponere; dominos POST, ne scriptura
 * initialis anonyma recusetur). */
b32
insula_ramum_initiare (
    constans InsulaRamus* ramus,
      constans character* elementum_stml);

/* canon nodum rami iudicat (radix eius = elementum rami); rami
 * descendentes cum canone proprio ei absunt (quavis profunditate,
 * S8), et canonibus superioribus hic ramus */
vacuum
insula_ramus_canonem_ponere (
    constans InsulaRamus* ramus,
                   Canon* canon);


/* ==================================================
 * Portae
 * ================================================== */

/* porta UNA (olim mutare_durabile / mutare_ephemera): mutator RADICEM
 * in duplicato accipit */
b32
insula_mutare (
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

/* Attributum radicis uni scriptori addicere. FALSUM si attributum
 * vacuum. */
b32
insula_dominum_ponere (
    InsulaRepositorium* repo,
    constans character* attributum,
    constans character* dominus);

/* <domini><dominus attributum scriptor [systemata]/>...</domini>:
 * omnes lineae leguntur - 'genus' (forma vetus, ante C2) IGNORATUR;
 * redde numerum lectarum. systemata
 * (S5a, status-plan): nomina systematum quae attributum quoque
 * scribere licet - possessio pro valoribus, systemata pro aspectibus
 * transversis (census X). */
i32
insula_dominos_legere (
    InsulaRepositorium* repo,
             StmlNodus* domini);

/* Attributum radicis tollere (par insula_attributum_ponere). */
b32
insula_attributum_tollere (
              StmlNodus* nodus,
     constans character* titulus);

#endif /* INSULA_H */
