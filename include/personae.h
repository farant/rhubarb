/* personae.h - Personae (contactus): homines et societates, nomina,
 * rubricae, telephona, epistulae, natalis (nomenclator-plan P1).
 *
 * Documentum UNUM (historia, ut agenda): veritas est ACTA in volumine
 * (genus "personae"); personae ex actis proiciuntur. Insulam non novit.
 * Horologium numquam legitur: aetas ex die "hodie" a vocante data.
 *
 * Acta (dies ISO; natalis sine anno "--MM-DD"):
 *   <persona actio="nova" id="7" genus="homo"><@praenomen=>Anna</>
 *     <@cognomen=>Rossi</></persona>
 *   <persona actio="nova" id="8" genus="societas"><@titulus=>...</>
 *     </persona>
 *   <persona actio="nomen" id="7"><@praenomen=>..</><@cognomen=>..</>
 *     </persona>
 *   <persona actio="titulus" id="8"><@titulus=>...</></persona>
 *   <persona actio="genus" id="7" genus="societas"/>
 *   <persona actio="rubricae" id="7" rubricae="familia roma"/>
 *   <persona actio="telephona" id="7"><telephonum><@valor=>...</>
 *     </telephonum>...</persona>                     index totus
 *   <persona actio="epistulae" id="7"><epistula><@valor=>...</>
 *     </epistula>...</persona>                       index totus
 *   <persona actio="natalis" id="7" dies="1986-03-02"/>  dies absens =
 *                                                        nullus
 *   <persona actio="deleta" id="7"/>
 * Id = maximum + 1 (numquam iteratur); persona deleta actum nullum
 * accipit. Nomina et valores (telephona, epistulae) forma elementi
 * attributi semper scribuntur ('"' repraesentabilis).
 *
 * Genus mutatum nomina non mutat: nomen ostensum et clavis ordinis ex
 * genere currenti, nomine altero si hoc vacuum (homo sine nominibus ->
 * titulus; societas sine titulo -> "praenomen cognomen").
 */
#ifndef PERSONAE_H
#define PERSONAE_H

/* <aedilis corpus="lib/personae.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "historia.h"
#include "fasti.h"

nomen enumeratio {
    PERSONA_HOMO = ZEPHYRUM,
    PERSONA_SOCIETAS
} PersonaGenus;

nomen structura {
             s32  id;
    PersonaGenus  genus;
          chorda  praenomen;         /* homo */
          chorda  cognomen;
          chorda  titulus;           /* societas */
             Xar* rubricae;          /* chorda, ordine canonico */
             Xar* telephona;         /* chorda, ordine dato */
             Xar* epistulae;         /* chorda, ordine dato */
             b32  natalis_positus;
            Dies  natalis;           /* annus 0 = sine anno */
} Persona;

nomen structura PersonaeDocumentum PersonaeDocumentum;

/* ut agenda_aperire */
PersonaeDocumentum*
personae_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  intervallum);

/* persona nova; redde id (> 0) aut 0 si recusata (nomen totum
 * vacuum) */
s32
personae_hominem_creare (
    PersonaeDocumentum* doc,
                chorda  praenomen,
                chorda  cognomen);

s32
personae_societatem_creare (
    PersonaeDocumentum* doc,
                chorda  titulus);

/* acta typata: redde seq (> 0) aut 0 si recusatum (persona absens aut
 * deleta, rubrica mala, natalis malus) */
s64
personae_nomen_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
                chorda  praenomen,
                chorda  cognomen);

s64
personae_titulum_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
                chorda  titulus);

s64
personae_genus_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
          PersonaGenus  genus);

/* rubricae spatiis separatae ("a b"); copia tota substituitur */
s64
personae_rubricas_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
                chorda  rubricae);

/* index totus (Xar de chorda; vacua omittuntur; NIHIL = vacuus) */
s64
personae_telephona_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
          constans Xar* telephona);

s64
personae_epistulas_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
          constans Xar* epistulae);

/* natalis NIHIL = nullus; annus 0 = sine anno (29 II licet) */
s64
personae_natalem_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
         constans Dies* natalis);

s64
personae_delere (
    PersonaeDocumentum* doc,
                   s32  id);

b32
personae_revocare (
    PersonaeDocumentum* doc);

b32
personae_reficere (
    PersonaeDocumentum* doc);

b32
personae_verificare (
    PersonaeDocumentum* doc);

chorda
personae_sigillum_hex (
    constans PersonaeDocumentum* doc,
                        Piscina* piscina);

Historia*
personae_historia (
    PersonaeDocumentum* doc);

/* proiectio: personae vivae ordine id; indicia valent usque ad
 * mutationem proximam */
i32
personae_numerus (
    constans PersonaeDocumentum* doc);

constans Persona*
personae_index (
    constans PersonaeDocumentum* doc,
                            i32  index);

/* NIHIL si absens aut deleta */
constans Persona*
personae_persona (
    constans PersonaeDocumentum* doc,
                            s32  id);

/* nomen ostensum: homo "Anna Rossi", societas titulus (vide supra) */
chorda
persona_nomen (
    constans Persona* p,
             Piscina* piscina);

/* clavis ordinis (minusculis): homo "rossi anna", societas titulus */
chorda
persona_clavis (
    constans Persona* p,
             Piscina* piscina);

/* littera tituli in indice: prima clavis maiuscula 'A'-'Z', aliter
 * '#' */
character
persona_littera (
    constans Persona* p);

/* aetas annis completis die hodie; -1 si natalis nullus aut sine
 * anno */
s32
persona_aetas (
    constans Persona* p,
                Dies  hodie);

/* personae vivae ordine clavis (deinde id) - Xar de (constans
 * Persona*) in piscina vocantis */
Xar*
personae_ordinatae (
    constans PersonaeDocumentum* doc,
                        Piscina* piscina);

/* rubricae distinctae personarum vivarum, ordine canonico */
Xar*
personae_rubricae (
    constans PersonaeDocumentum* doc,
                        Piscina* piscina);

#endif /* PERSONAE_H */
