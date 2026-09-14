/* briar_fasciculum.h - '-app': thistle in fasciculum .app (spec par.
 * 4.8).
 *
 * Consilium PURUM: ex nexu (elementa briar et fenestra) et via
 * plagulae omnia decernit quae sine disco decerni possunt - identitas,
 * nomen, versio, fons iconis, via fasciculi. Scriptor (opus IV plani
 * 7) pixela IAM decodificata accipit: decodificatio in tools/briar.c
 * vivit, ergo nulla porta briar stb_image nectit (icones D7). */

#ifndef BRIAR_FASCICULUM_H
#define BRIAR_FASCICULUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#define BRIAR_FASCICULUM_PRAEFIXUM "org.rhubarb.briar."

nomen structura {
    chorda identitas;   /* A4: characteres iudicati */
    chorda titulus;     /* A5: fenestra titulus= aut nomen plagulae */
    chorda versio;      /* A5: briar versio=; vacua = ordinaria */
    chorda via_icon;    /* A7 resoluta; vacua = icon infixus */
    chorda via_app;     /* directorium plagulae + t.app */
       i32 linea_briar; /* linea tagi briar; ZEPHYRUM si abest */
} BriarFasciculumConsilium;

/* VERUM + consilium; FALSUM + causa + linea (tagi briar) */
b32
briar_fasciculum_consilium (
                     Piscina* piscina,
                         Xar* nexus,
          constans character* via_thistle,
          constans character* via_icon_vexilli,
    BriarFasciculumConsilium* consilium,
                      chorda* causa,
                         i32* linea_causae);

#endif /* BRIAR_FASCICULUM_H */
