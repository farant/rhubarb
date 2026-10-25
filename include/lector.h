/* lector.h - LECTOR in latere vici (genus "lector"; lector L3, spec
 * project-specs/lector-spec.md). Manifestum apps/lector/organum.stml;
 * bibliotheca (lector_bibliotheca.h) per ligamen a parente, communis
 * omnibus lateribus; status visus in ramo.
 *
 * BIBLIOTHECA: linea per librum - titulus, auctor, progressus ("37%",
 * "new"); non legibiles obscuri ("unreadable"). Claves: j k (electus),
 * [ ] (paginae), Enter (liber ad positionem suam), Esc (latus per
 * vicum).
 * INDEX: capita libri (prooemium "Beginning"), caput positionis
 * notatum. Claves: j k, [ ], Enter (caput a pagina prima), Esc (ad
 * bibliothecam).
 * PAGINA: caput paginatum (lector_paginae.h), titulus capitis et "3/12"
 * in linea ima. Claves: [ ] (pagina; trans capita), t (index), Esc (ad
 * indicem). Omnis motus positionem libri scribit (sententia prima quae
 * in pagina incipit; nulla -> sententia continens). Liber apertus =
 * positio eius (codex_reperire); amissa -> initium capitis cum nota
 * "position not found". s: ad sententiam (positio libri si in pagina,
 * aliter sententia prima paginae), f (favores).
 * SENTENTIA (L4): sententia sola in medio (lector_sententiam_
 * disponere), titulus capitis supra, "sentence 12/153" infra
 * (emendationes amissae numeratae, "..." si praecisa). Claves: j k
 * (sententia; trans capita; positio scribitur), s et Esc (ad paginam),
 * t (index), J (iungere cum sequente in eadem clausula; limes per
 * fissuram servatam factus fissuram tollit), * (favor vertitur; iuncta
 * favor est si pars ulla, '*' partes omnes tollit), i (modus
 * findendi: cursor in verbo secundo, w l / b h per verba, Enter
 * fissuram pendentem ante verbum vertit - partes linea vacua
 * separatae, Esc servat: fissura ubi iunctura partes coniunxit
 * iuncturam tollit, aliter FINDERE), f (favores). Emendationes et
 * favores libri (lector_bibliotheca) per caput applicantur; latera
 * capita renovant cum lector_bibliotheca_versio crescit.
 * FAVORES: per caput (titulus gregis, in pagina continuationis
 * iteratus), sententiae involutae (6x8); favores consecutivi (etiam
 * trans paragraphum, numquam trans caput) ordo unus - servantur per
 * sententiam, ergo '*' in media cursum findit. Claves: j k, [ ],
 * Enter (ad sententiam primam ordinis - motus), Esc (unde venit).
 * Lustratio positionem non movet. */

#ifndef LECTOR_H
#define LECTOR_H

/* <aedilis corpus="lib/lector.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "lector_bibliotheca.h"

nomen structura {
            InsulaRamus  ramus;
         ActioRegistrum* actiones;
        FiguraRegistrum* figurae;
      LectorBibliotheca* bibliotheca;   /* ligamen (a parente) */
                Piscina* piscina;
    InternamentumChorda* intern;
                 vacuum* figurae_ctx;   /* privatum: codex, caput,
                                         * paginae in cache */
} LectorMontatio;

/* montare in ramo <lector id="..."> (manifestum infixum); superficies
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet */
b32
lector_montare (
         LectorMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
     constans character* id,
      LectorBibliotheca* bibliotheca,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = LectorMontatio* */
Componens*
lector_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum lateris: URI semitae ("lector/bibliotheca",
 * "lector/pagina/<id>?caput=4&pagina=2"), identitas libri nuda (->
 * liber ad positionem suam), vacuum (bibliotheca). Semita mala aut
 * liber ignotus: FALSUM, nihil mutatum. */
b32
lector_argumentum_ponere (
         LectorMontatio* m,
     constans character* argumentum);

/* facies.semitam_secuta (historia, $via, restitutio): caput et pagina
 * nominata = motus (positio scribitur); non nominata = positio libri */
b32
lector_semitam_secuta (
    vacuum* ctx);

/* facies.locus: semita loci currentis cum capite et pagina
 * ("lector/pagina/<id>?caput=4&pagina=2"); index, bibliotheca: semita
 * sua */
chorda
lector_locus (
     vacuum* ctx,
    Piscina* piscina);

/* facies.locus_titulus: "Cranford - CHAPTER I. OUR SOCIETY - 3/12";
 * index: titulus libri; bibliotheca: "Library" */
chorda
lector_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* LECTOR_H */
