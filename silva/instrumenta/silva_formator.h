/* silva_formator.h - machina lint formatoris (regulae formandi G1)
 *
 * Regulae: project-specs/formator-regulae.md (nomina stabilia -
 * divergentiae se nominant). Tranche prima = regulae fluminis
 * crudi: R5 (tabulae), R6 (spatium post claves), R12 (spatia
 * caudae), R14 (vexilla), R16 (longitudo). Regulae structurales
 * (arbore egentes) in tranche secunda veniunt.
 *
 * Hic machina sola (probabilis, silici portabilis); modi et CLI
 * in instrumenta/principalia/formator.c - exemplar differre.
 */

#ifndef SILVA_FORMATOR_H
#define SILVA_FORMATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_contextus.h"

/* G2: emendatio - editio spatiorum ut spatium [a, b) in textu
 * ORIGINALI (coordinatae 1-basatae; finis exclusivus) cum textu
 * inserendo. Applicator solus octetos spatiales delere et spatia/
 * lineas novas inserere potest - custodia contractus in strato
 * infimo (commenta et lexemata physice intacta manent).
 *
 * tolerans: spatium octetos non-spatiales LICITE continere potest
 * (iunctura trans commentum aut continuationem '\\') - custodia
 * spatialis id tacite dilatat pro recusatione clamosa (quae vitiis
 * detectorum reservatur). */
nomen structura {
       i32 linea_a;
       i32 columna_a;
       i32 linea_b;
       i32 columna_b;
       b32 tolerans;
    chorda insertum;
} FormatorEmendatio;

/* nuntius ordinationum ancoram nominat: "(ancora 'x' l.N + II)" */
nomen structura {
     constans character* regula;      /* nomen stabile regulae */
     constans character* nuntius;     /* descriptio brevis */
                    i32  linea;       /* 1-basata */
                    i32  columna;     /* 1-basata */
                    s32  inventum;    /* valor inventus */
                    s32  exspectatum; /* valor exspectatus */
                    i32  numerus_emendationum; /* 0 = non fixabilis */
      FormatorEmendatio  emendationes[II];
} FormatorDivergentia;

/* Lint super fontem unum: Xar de FormatorDivergentia (vacuum =
 * conformis; NIHIL solum si piscina fracta). Fons vivere debet
 * quamdiu divergentiae vivunt (visus, nulla copia).
 *
 * contextus: diu vivens, inter plagulas reusabilis - vocator
 * capita praebet ut typedef alieni resolvantur (R7 sine eis
 * ordines typorum ignotorum exemptos facit). NIHIL = contextus
 * internus latina-solum. Divergentiae SEMPER ad fontem principem
 * solum pertinent (contentum capitum numquam flagratur). */
Xar*
formator_lint (
               Piscina* piscina,
        SilvaContextus* contextus,
    constans character* fons,
                   i32  mensura);

/* G2 -scribere: emendationes lint ad punctum fixum applicare.
 * Successus = textus formatus (copia in piscina; fons intactus);
 * recusatio (querela nominata, textus = copia originalis) si
 * series lexematum mutaretur aut punctum fixum intra XII
 * iterationes non attingitur. Divergentiae sine emendatione
 * (gradus LINT, non-fixabiles) honeste supersunt - lint super
 * fructum eas adhuc nominat. */
nomen structura {
                 chorda  textus;
                    b32  successus;
                    b32  mutatum;
                    i32  iterationes;
                    i32  applicatae;   /* emendationes summa */
     constans character* querela;      /* NIHIL nisi recusatum */
} FormatorScriptum;

FormatorScriptum
formator_scribere (
               Piscina* piscina,
        SilvaContextus* contextus,
    constans character* fons,
                   i32  mensura);

/* Ambitus nominatus (-intra): functiones quarum extenta SOLA
 * iudicantur - editio plagulae communis baseline magno intra
 * functionem novam solam. Extentum = a linea post nodum radicis
 * priorem (commentarium ducens et intervalla supra ad functionem
 * infra pertinent) ad lineam ultimam definitionis; prototypa
 * radicis eiusdem tituli quoque. NOMINA, non lineae: passus
 * punctum-fixum quaque iteratione reparsat et lineae labuntur -
 * titulus manet.
 *
 * inventae [numerus] (NIHIL licet - scribere_intra suas facit):
 * exitus, titulus in radice visus (definitio aut prototypum).
 * Ignotus ambitum tacite non angustat: scribere_intra recusat
 * (querela 'functio intra ignota'), lint_intra per inventae
 * clamare sinit. Parsura fracta = nihil inventum = recusatio. */
nomen structura {
    constans chorda* functiones;
                i32  numerus;
                b32* inventae;
} FormatorIntra;

Xar*
formator_lint_intra (
                   Piscina* piscina,
            SilvaContextus* contextus,
        constans character* fons,
                       i32  mensura,
    constans FormatorIntra* intra);

/* Extenta functionum radicis (definitiones et prototypa vera) ordine
 * fontis - IDEM ambitus quem -intra iudicat: a linea post nodum
 * radicis priorem (commentarium ducens, intervalla supra) ad lineam
 * ultimam. Consumptor: -lineae (uncus commissionis: lineae mutatae ->
 * functiones nominatae -> scriptura intra eas), pythonica. Typi cum
 * corpore (structura/unio/enumeratio) quoque. Nodi in ramis SUMPTIS
 * conditionalium in ambitu plagulae ut radices ambulantur (custos post
 * inclusionem, #ifdef platformae) - rami omissi non. Xar de
 * FormatorExtentum; vacuum si parsura fracta; NIHIL si piscina.
 * genus: "definitio" | "prototypum" | "structura" | "unio" |
 * "enumeratio" (typi cum corpore: titulus = declarator typedef primus,
 * aliter tag). */
nomen structura {
    chorda titulus;
       i32 linea_a;
       i32 linea_nodi;  /* linea prima nodi ipsius (specificatores/
                         * titulus) - corpus sine commentario ducente */
       i32 linea_b;
       b32 definitio;   /* VERUM = definitio, FALSUM = prototypum */
    constans character* genus; /* vide supra */
} FormatorExtentum;

Xar*
formator_extenta (
               Piscina* piscina,
        SilvaContextus* contextus,
    constans character* fons,
                   i32  mensura);

FormatorScriptum
formator_scribere_intra (
                   Piscina* piscina,
            SilvaContextus* contextus,
        constans character* fons,
                       i32  mensura,
    constans FormatorIntra* intra);

#endif /* SILVA_FORMATOR_H */
