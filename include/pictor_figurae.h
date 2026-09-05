/* pictor_figurae.h - figurae pictoris (P3: tabula, titulus)
 *
 * <purus/>: figura arborem solam legit. Tabula = imago UNA cuius
 * identitas est sigillum proiectionis (contentu addressata) +
 * ictus pendens (lineae inter puncta componentis) + cursor. Fons
 * imaginum pro rasterizatore: sigillum -> proiectio documenti.
 */

#ifndef PICTOR_FIGURAE_H
#define PICTOR_FIGURAE_H

/* <aedilis corpus="lib/pictor_figurae.c"/> */

#include "latina.h"
#include "chorda.h"
#include "figura.h"
#include "delineare_mandata.h"
#include "pictor_documentum.h"

nomen structura {
    PictorDocumentum* doc;
} PictorFigurae;

vacuum
pictor_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      PictorFigurae* ctx);

/* ImagoFons: provenientia == sigillum hex documenti -> proiectio */
constans Imago*
pictor_imago_fons (
    chorda  provenientia,
    vacuum* ctx);

/* <purus/> */
vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> */
vacuum
figura_tituli (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

#endif /* PICTOR_FIGURAE_H */
