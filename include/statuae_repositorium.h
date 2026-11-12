/*
 * statuae_repositorium.h - statuae in volumine: addere, removere,
 * enumerare
 *
 * Statua in volumine vici habitat ut OCTETI ORIGINALES plagulae suae
 * (massa sigillo addressata) cum actu qui titulum et sigillum nominat
 * (project-specs/statuae-spec.md, decretum 20). Index statuarum ex
 * actis derivatur; nihil aliud conditur. Forma plagulae hic non
 * legitur: vocator (bin/statuae) octetos per lectorem probat ANTE
 * additionem.
 *
 * Acta (datum JSON):
 *   statua-addita   {"titulus": "...", "massa": "<hex>",
 *                    "forma": "glb"} (+ "genus": "locus" | "res")
 *   statua-remota   {"titulus": "..."}
 *
 * Titulus statuam nominat: additio tituli noti octetos substituit et
 * locum in ordine servat; remotio et additio nova eam in finem ponit.
 * Massa remotae manet (historia eam spectat).
 *
 * Solum acta PROPRIA huius voluminis leguntur (volumen.h, lex
 * communionis): statua in alia machina addita hic non apparet.
 *
 * Processus alius (bin/statuae) addit dum vicus currit: latus
 * volumen_seq_ultimum spectat et indicem relegit cum mutatur.
 */

#ifndef STATUAE_REPOSITORIUM_H
#define STATUAE_REPOSITORIUM_H

/* <aedilis corpus="lib/statuae_repositorium.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"

#define STATUAE_ACTUM_ADDITUM    "statua-addita"
#define STATUAE_ACTUM_REMOTUM    "statua-remota"

/* titulus: I..LXIV octeti, sine '/' et sine characteribus regiminis */
#define STATUAE_TITULUS_MAXIMUS  LXIV

/* genus inscriptionis: persona (statua animata, latus $models et
 * personae mundi), locus (statua loci, latus $world) aut res (res
 * plastica: puteus, saepes; latus $models in grege rerum). In acto
 * "genus":"locus" aut "genus":"res"; absens = statua (acta ante
 * mundum scripta). Vicus qui genus non novit id ut statuam legit -
 * ergo vicus novus ANTE rem primam in volumine currat. */
nomen enumeratio {
    STATUAE_GENUS_STATUA = ZEPHYRUM,
    STATUAE_GENUS_LOCUS,
    STATUAE_GENUS_RES,
    STATUAE_GENERUM
} StatuaeGenus;

nomen structura {
          chorda titulus;
          chorda sigillum_hex;   /* massa: volumen_massam_promere */
    StatuaeGenus genus;
} StatuaeInscriptio;

/* titulus validus? (longitudo, '/', characteres regiminis) */
b32
statuae_titulus_validus (
    chorda titulus);

/*
 * statuae_addere - octetos condere et statuam inscribere
 *
 * Massa et actum in transactione una (transactionem vocatoris
 * iungit si tenet). Titulus iam notus: octeti substituuntur, locus
 * manet. Octeti iidem sub titulo eodem: nihil scribitur, VERUM.
 *
 * Redde: FALSUM si titulus invalidus, octeti vacui aut volumen
 * recusat (tunc nihil scriptum)
 */
b32
statuae_addere (
    Volumen* volumen,
     chorda  titulus,
     chorda  octeti);

/* statuam ex indice tollere; VERUM etiam si titulus aberat (tunc
 * nihil scribitur) */
b32
statuae_removere (
    Volumen* volumen,
     chorda  titulus);

/* index STATUARUM (genus STATUA; loci et res non apparent), ordine
 * additionis; Xar de StatuaeInscriptio (chordae ex piscina). Numquam
 * NIHIL: volumen sine statuis Xar vacuum dat */
Xar*
statuae_enumerare (
    Volumen* volumen,
    Piscina* piscina);

/*
 * statuae_addere_generis - ut statuae_addere, genere dato
 *
 * Tituli per genera OMNIA unici: titulus qui in genere alio iam est
 * recusatur (nihil scribitur). statuae_addere = genus STATUA.
 * statuae_removere genus quodlibet tollit.
 *
 * Redde: FALSUM etiam si genus ignotum
 */
b32
statuae_addere_generis (
         Volumen* volumen,
          chorda  titulus,
          chorda  octeti,
    StatuaeGenus  genus);

/* index generis unius, ordine additionis; genus ignotum: vacuum.
 * Numquam NIHIL */
Xar*
statuae_enumerare_generis (
         Volumen* volumen,
    StatuaeGenus  genus,
         Piscina* piscina);

#endif /* STATUAE_REPOSITORIUM_H */
