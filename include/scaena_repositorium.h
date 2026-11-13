/*
 * scaena_repositorium.h - prospectus in volumine: addere, removere,
 * enumerare, promere
 *
 * Prospectus in volumine vici habitat ut massa sua (scaena.h:
 * scaena_scribere), sigillo addressata, cum actu qui titulum et
 * sigillum nominat. Index ex actis derivatur; nihil aliud conditur.
 * Repositorium a statuis SEPARATUM est: genera actorum sua habet,
 * ergo index statuarum prospectus non videt nec hic statuas, et
 * vicus vetus qui prospectus non novit nihil perperam legit
 * (project-specs/graphis-spec.md 23a).
 *
 * Acta (datum JSON):
 *   scaena-addita   {"titulus": "...", "massa": "<hex>"}
 *   scaena-remota   {"titulus": "..."}
 *
 * Titulus prospectum nominat (idem quem exitus petunt): additio
 * tituli noti massam substituit et locum in ordine servat; massa
 * eadem sub titulo eodem nihil scribit. Remotio et additio nova eum
 * in finem ponit. Massa remotae manet (historia eam spectat).
 *
 * Forma massae hic non legitur: vocator (bin/scaena) eam per
 * scaena_legere probat ANTE additionem.
 *
 * Solum acta PROPRIA huius voluminis leguntur (volumen.h, lex
 * communionis). Processus alius (bin/scaena) addit dum vicus
 * currit: latus volumen_seq_ultimum spectat et indicem relegit.
 */

#ifndef SCAENA_REPOSITORIUM_H
#define SCAENA_REPOSITORIUM_H

/* <aedilis corpus="lib/scaena_repositorium.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"

#define SCAENA_ACTUM_ADDITUM  "scaena-addita"
#define SCAENA_ACTUM_REMOTUM  "scaena-remota"

nomen structura {
    chorda titulus;
    chorda sigillum_hex;   /* massa: volumen_massam_promere */
} ScaenaInscriptio;

/* titulus prospectus: I .. LXIV litterae ex a-z, 0-9, '-' et '_',
 * non "mundus" (regula scaena.h) */
b32
scaena_titulus_validus (
    chorda titulus);

/*
 * scaena_addere - prospectum addere aut substituere
 *
 * @massa: octeti ut scaena_scribere eos dedit
 *
 * Redde: VERUM si prospectus sub titulo cum hac massa in volumine
 * stat (etiam si iam stabat: nihil scriptum); FALSUM si volumen
 * NIHIL, titulus malus, massa vacua, aut scriptura deficit (nihil
 * mutatum)
 */
b32
scaena_addere (
    Volumen* volumen,
     chorda  titulus,
     chorda  massa);

/*
 * scaena_removere - prospectum ex indice tollere
 *
 * Redde: VERUM si titulus in indice erat et remotus est; FALSUM si
 * aberat, titulus malus, aut scriptura deficit
 */
b32
scaena_removere (
    Volumen* volumen,
     chorda  titulus);

/*
 * scaena_enumerare - index prospectuum, ordine additionis
 *
 * Redde: Xar de ScaenaInscriptio in piscina (vacuum si nulli);
 * NIHIL si memoria deficit
 */
Xar*
scaena_enumerare (
    Volumen* volumen,
    Piscina* piscina);

/*
 * scaena_promere - massam prospectus nomine promere
 *
 * Redde: massa in piscina; chorda vacua (datum NIHIL) si titulus in
 * indice non est aut massa legi non potest
 */
chorda
scaena_promere (
    Volumen* volumen,
     chorda  titulus,
    Piscina* piscina);

#endif /* SCAENA_REPOSITORIUM_H */
