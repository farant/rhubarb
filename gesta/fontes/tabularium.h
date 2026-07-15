/* tabularium.h - servus MCP tabularii (K1 chunk C)
 *
 * Machina servi SUPER gesta: quinque instrumenta
 * (addere/gerere/quaerere/res/census) super vecturam tabellarii
 * (framing linearum MCP stdio). Sceletum ex legato copiatum-
 * adaptatum (~250 lineae; SEMEN lib/mcp_servus futuri - extractio
 * cum servo tertio, spec-v2 par XVII).
 *
 * Divisio machina/principale (exemplar legati): haec unitas =
 * machina probabilis (tractare per nuntium, effusio FILE* data -
 * probatio tmpfile capit); instrumenta/tabularium_principale.c =
 * involucrum tenue stdin/stdout.
 *
 * RESIDENS LEVIS: gesta + scrinium + tabellarius + json - NIHIL
 * silvae/praeparatoris (exploratio MCP: totum pondus legati post
 * tractatores sedet, non in vectura). Index ancorarum = nexus.tsv
 * PIGRE oneratus (via in configuratione - probatio fixturam dat);
 * residens-ad-residentem legati IMPOSSIBILE ante LEGATUSD. */

#ifndef TABULARIUM_H
#define TABULARIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "gesta.h"
#include <stdio.h>

nomen structura {
    constans character* radix;         /* pro nuntiis */
    constans character* via_scrinii;   /* tabularium.db */
    constans character* via_annalium;  /* gesta/annales/...jsonl */
    constans character* via_nexus;     /* build/nexus.tsv; NIHIL =
                                        * ancorae sine resolutione */
    constans character* via_tabulae;   /* tabula.md (proiectio status
                                        * legibilis, post scripturas
                                        * regenerata); NIHIL = sine
                                        * tabula */
} TabulariumConfiguratio;

nomen structura Tabularium Tabularium;

/* NIHIL = fractum (piscina vivit quamdiu servus) */
Tabularium*
tabularium_creare (
    Piscina*                        piscina,
    constans TabulariumConfiguratio* cfg);

/* nuntium unum tractare: corpus lineae -> responsa in effusionem
 * scripta (tabellarius_lineam_scribere). pn = piscina nuntii
 * (effimera, vocantis). FALSUM = numquam (semper respondet aut
 * tacet in nuntiationibus) - fructus pro symmetria futura. */
b32
tabularium_tractare (
    Tabularium* tabularium,
    Piscina*    pn,
    chorda      corpus,
    FILE*       effusio);

/* circuitus stdio integer (principale eum vocat); fructus = codex
 * exitus (EOF cliente claudente = 0 ordinatus) */
integer
tabularium_currere (
    constans TabulariumConfiguratio* cfg,
    FILE*                            intra,
    FILE*                            extra);

#endif /* TABULARIUM_H */
