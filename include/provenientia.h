#ifndef PROVENIENTIA_H
#define PROVENIENTIA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"


/* ==================================================
 * PROVENIENTIA - binarium installatum originem suam dicit
 *
 * Omne binarium domus institutum '-provenientia' respondet (spec
 * fabrica v1 par. VI, v2 par. III): sigillum copiae ingressuum actionis
 * quae id struxit, ab installatore in plagulam generatam scriptum
 * (build/fabrica/provenientia/TITULUS.c, per
 * tools/provenientia_scribere.sh). bin/fabrica idem sigillum hodie
 * computat - aequalia = recens.
 *
 * Relatio per ARGUMENTUM datur, numquam per data externa quae
 * bibliotheca peteret: quattuor installatores obiecta build/ CAECE
 * nectunt, et symbolum quod plagula generata sola definit nexum eorum
 * frangeret (plan 1a T7).
 * ================================================== */

nomen structura {
    constans character* artificium;  /* e.g. "bin/manus" */
    constans character* ingressus;   /* LXIV hex */
    constans character* commissum;   /* HASH aut "HASH SORDIDUM" */
} ProvenientiaRelatio;

/* Textus relationis, quattuor lineae:
 *   provenientia 1 / artificium X / ingressus H / commissum C */
chorda
provenientia_textus (
    constans ProvenientiaRelatio* relatio,
                         Piscina* piscina);

/* Si argv '-provenientia' fert (argumentum unicum), relationem in
 * stdout scribit et VERUM reddit - vocans tum exit 0. Aliter FALSUM,
 * nihil scriptum. PRIMUM in principale vocandum (ante omnem laborem et
 * ante custodias cwd). */
b32
provenientia_respondere (
                              s32   argc,
                        character** argv,
     constans ProvenientiaRelatio*  relatio);

#endif /* PROVENIENTIA_H */
