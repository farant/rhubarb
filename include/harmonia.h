/* harmonia.h - Quid simul sonat: acordum nominatum (analysis, non
 * coniectura)
 *
 * Agens harmoniam in capite portabat dum componebat ('vesperae',
 * 2026-10-23); haec bibliotheca eam NOMINAT: classes altitudinum
 * sonantium (XII bits) cum exemplaribus EXACTE comparantur
 * (acordum.h: tabula exemplarium, acordum_ex_classibus). Quod sonat
 * refertur, non quod implicatur - cantus duarum vocum fere dyades
 * monstrat ('(E)', 'E5'); harmonia implicita (opinio) alia res est.
 *
 * SCRIPTIO: "D", "Bm", "A7", "G/B" (bassus non radix), "A(5)", "(E)"
 * (classis una), "[C E F#]" (ignotum, a basso sursum), "r" (nihil
 * sonat); litterae ex clave ad pulsum (partitura_orthographia).
 *
 * LINEA (partitura.h, linea addita): per tempus mensurae (1 /
 * denominator; 6/8 9/8 12/8: tres octavae) acordum ad initium temporis;
 * aequalia sequentia in signum unum cum valore (D:2 G:2).
 *
 * USUS:
 *   forma.additae[0].titulus  = "harmonia";
 *   forma.additae[0].scribere = harmonia_lineam_scribere;
 *   (vacuum)partitura_scribere(carmen, &forma, aedificator);
 */
/* <aedilis corpus="lib/harmonia.c"/> */
#ifndef HARMONIA_H
#define HARMONIA_H

#include "latina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "acordum.h"
#include "carmen.h"

/* littera notae in classem eius (prima vincit; '\0' ignoratur) */
vacuum
harmonia_litteram_notare (
                Acordum* h,
    constans CarmenNota* nota);

/* quod sonat ad pulsum: partes omnes non mutae (si sola quaedam,
 * solae), sine percussione */
Acordum
harmonia_ad_pulsum (
    constans Carmen* carmen,
                s64  pulsus);

/* nomen acordi (litterae ex clave ad pulsum). FALSUM: memoria deest */
b32
harmonia_scribere (
       constans Carmen* carmen,
      constans Acordum* harmonia,
                   s64  pulsus,
     ChordaAedificator* textus);

/* linea addita partiturae (PartituraAddita.scribere): ctx non
 * adhibetur */
b32
harmonia_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus);

/* linea addita 'acorda' (PartituraAddita.scribere, ctx ignoratur):
 * acorda declarata (Carmen.acorda) ut scripta, per mensuram - "D:2
 * G7:2", 'r' nullum, '-' extra spatia. Harmonia SONANS in linea
 * 'harmonia' manet: discordia oculis patet */
b32
harmonia_acorda_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus);

#endif /* HARMONIA_H */
