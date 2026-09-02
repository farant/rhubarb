/* silva_coquere.h - Coctio tabularum: tabulae in fontem C
 *
 * Emittit tabulas generatoris ut fontem C legibilem Latine commentatum
 * (norma eadem ac codex manu scriptus - decisio interview). Duas
 * plagulas scribit: <basis>.h (declaratio externa) et <basis>.c
 * (series statica + SilvaTabulaCocta). Commenta GENERANTUR ex ipsis
 * tabulis (nomina terminalium, id productionum) - commenta generata
 * putrescere non possunt (R3).
 *
 * Actiones per statum ORDINATAE emittuntur - regeneratio stabilis,
 * differentiae parvae (plagulae generatae committuntur).
 */

#ifndef SILVA_COQUERE_H
#define SILVA_COQUERE_H

#include "latina.h"
#include "silva_generare.h"

/* Coquere tabulas in <basis_via>.h + <basis_via>.c.
 * praefixum: praefixum symbolorum C ("SILVA_SCELETUM").
 * via_grammaticae: pro commento GENERATUM (unde regenerare).
 * Reddit FALSUM si plagulae scribi non possunt. */
b32
silva_gen_coquere (
    SilvaGenGrammatica* grammatica,
        SilvaGenTabula* tabula,
    constans character* praefixum,
    constans character* basis_via,
    constans character* via_grammaticae);

/* Superficies publica generata (officina pre-M1): enum generum +
 * prototypa accessorum VANILLA inter signa ">>>"/"<<<" in plagulas
 * manu scriptas splicare (via_capitis = silva.h: enum custodito +
 * prototypa; via_hospitis = hospes.c: tabula ACCESSORES typis
 * imposita). Signa desunt = FALSUM (clamat). Idem generator ambas
 * copias scribit - deriva structuraliter impossibilis. */
b32
silva_gen_splicere_publica (
               Piscina* piscina,
    SilvaGenGrammatica* grammatica,
    constans character* praefixum,
    constans character* via_capitis,
    constans character* via_hospitis);

#endif /* SILVA_COQUERE_H */
