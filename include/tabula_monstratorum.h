/* tabula_monstratorum.h - tabula monstratorum (pointer -> pointer map)
 *
 * Clavis = monstrator ipse (identitas, non contentum): sedes nodi,
 * structurae, cuiuslibet obiecti. Valor = monstrator. Pro tabulis
 * lateralibus quae obiecto notam affigunt sine campo in obiecto
 * (typatio nodi, nexus symboli).
 *
 * Contra tabula_dispersa (claves chordae, friatio et comparatio per
 * functiones): loci XVI octetorum {clavis, valor}, friatio monstratoris
 * interna, comparatio ==, addressing apertum linearum, factor oneris
 * III/IV integer. Nulla deletio, nulla iteratio - ergo ordo nullus
 * observari potest.
 *
 * CLAVIS NIHIL vetatur (locus vacuus = clavis NIHIL): invenire NIHIL
 * reddit, locus NIHIL reddit.
 *
 * USUS (invenire aut addere, quaestione una):
 *   vacuum** locus = tabula_monstratorum_locus(t, nodus);
 *   si (locus == NIHIL) ... allocatio fracta ...
 *   si (*locus == NIHIL) *locus = valorem_novum_facere(...);
 *   valor = *locus;
 */
#ifndef TABULA_MONSTRATORUM_H
#define TABULA_MONSTRATORUM_H

#include "latina.h"
#include "piscina.h"

nomen structura TabulaMonstratorum TabulaMonstratorum;

/* capacitas = loci initiales (ad potentiam II sursum, minimum XVI);
 * tabula crescit per duplicationem. NIHIL si piscina NIHIL aut
 * allocatio fracta. */
TabulaMonstratorum*
tabula_monstratorum_creare (
    Piscina* piscina,
        i32  capacitas);

/* valor clavis; NIHIL si absens (aut tabula/clavis NIHIL) */
vacuum*
tabula_monstratorum_invenire (
    constans TabulaMonstratorum* tabula,
                constans vacuum* clavis);

/* INVENIRE AUT ADDERE: monstrator ad valorem clavis. Clavis nova
 * additur cum valore NIHIL - vocans eum statim implet. NIHIL si
 * crescentia fracta (aut tabula/clavis NIHIL).
 *
 * VALET USQUE AD VOCATIONEM PROXIMAM locus (crescentia locos movet):
 * scribe per eum ante additionem alteram. Allocationes piscinae
 * interim licent - loci non moventur nisi tabula crescit. Clavis
 * cuius valor NIHIL manet invenire NIHIL reddit sed in numero est. */
vacuum**
tabula_monstratorum_locus (
    TabulaMonstratorum* tabula,
       constans vacuum* clavis);

/* claves additae */
i32
tabula_monstratorum_numerus (
    constans TabulaMonstratorum* tabula);

#endif /* TABULA_MONSTRATORUM_H */
