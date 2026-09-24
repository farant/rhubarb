/* compendium.h - COMPENDIUM CAPITIS: declarationes gradus supremi
 * plagulae PRINCIPALIS parsurae silvae (functiones, typi, variabilia,
 * constantes enumerationum) cum extentis commentarii ducentis et
 * declarationis totae; ordo plagulae; contractio declarationis in
 * lineam unam.
 *
 * Communis legato (caput, symbola) et briar (-bibliotheca
 * -functiones): extractio UNA, ne instrumenta duo de eodem capite
 * dissentiant. Supra API publicum silvae SOLUM (silva/amalgama/
 * silva.h) - silva ipsa congelata manet. Ex legatus.c excisum
 * (_extenta_ex_semantica, _declarationem_contrahere) 2026-09-24.
 */
#ifndef COMPENDIUM_H
#define COMPENDIUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "silva.h"

/* Declaratio una gradus supremi. Extenta in BYTES plagulae
 * principalis (-1 = absens); lineae 1-basatae. */
nomen structura {
    constans SemanticaSymbolum* symbolum;  /* vivit dum parsura -
                                            * signatura legati */
                         chorda titulus;   /* in piscina, NUL post */
                            s32 genus;     /* SemanticaSymbolumGenus */
                            b32 est_definitio; /* functio cum corpore */
                            i32 linea_a;
                            i32 linea_b;
                            s32 commentarium_initium;
                            s32 commentarium_finis;
                            s32 corpus_initium; /* declaratio TOTA */
                            s32 corpus_finis;
} CompendiumDeclaratio;

/* Omnes declarationes gradus supremi plagulae principalis, ordine
 * symbolorum silvae (registrationes: prototypus et definitio
 * eiusdem functionis = duae). Symbola systematis, implicita et
 * plagularum inclusarum exclusa. Typedef structurae/enumerationis
 * ad radicem DEFINITIONIS repungitur. NIHIL = memoria. */
Xar*
compendium_declarationes (
      constans SilvaParsura* parsura,
    constans SilvaSemantica* semantica,
                    Piscina* piscina);

/* Ordo plagulae: Xar de i32 (indices in declarationes), linea
 * ascendens; declaratio eadem (corpus_initium idem - constantes
 * enumerationis cum typo suo) SEMEL, typo praelato; corpus absens
 * omittitur. NIHIL = memoria. */
Xar*
compendium_ordo (
         Xar* declarationes,
     Piscina* piscina);

/* Declarationem in lineam unam: spatia alba in unum, nullum post
 * '(' nec ante ')'; si '{' adest (definitio, structura,
 * enumeratio): caput + " {...} " + cauda post '}' ultimum (nomen
 * typedefi). Ultra tectum octetorum: praecisa + " ...". */
chorda
compendium_contrahere (
     chorda  corpus,
        i32  tectum,
    Piscina* piscina);

#endif /* COMPENDIUM_H */
