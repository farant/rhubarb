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
                 i32  cellula_latitudo;  /* margo cellula extra paginam;
                                          * 0 = I pixelum (olim) */
                 i32 cellula_altitudo;
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
figura_prospectus (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> quadratum lineae status (PARTES_BOTTONE): titulus dicit
 * quid pingatur - "instrumentum:<nomen>" (icon 1-bit XVI x XVI),
 * "color:<index>" (palette Aquinas, XVI), "color:-1" (nullus: crux),
 * "exemplar:<n>" (exemplar 1-bit colore textus; optio palettae),
 * "exemplar:<n>:<primus>:<secundus>" (exemplar coloribus veris, ut
 * pingetur - quadratum et optiones palettae; nullus = fundus
 * quadrati; ambo nulli: 1-bit), "magnitudo:<n>" (discus diametri n
 * centratus colore textus; n > XVI: numerus),
 * "magnitudo:aspergillum:<m>" (guttae in disco crescente),
 * "magnitudo:spongia:<n>" (quadratum hebes; magnitudo fixa).
 * Margo colore marginis; titulus in ":electum" desinens (optio
 * electa in palette, P1b): margo colore accentus. */
vacuum
figura_quadrati (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> palette (PARTES_DIALOGUS, P1b): fundus superficiei et
 * margo; optiones = quadrata filia (figura_quadrati) */
vacuum
figura_palettae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* <purus/> linea status: fundus et titulus (textus post quadratum
 * filium ultimum, cellula interposita) */
vacuum
figura_tituli (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

#endif /* PICTOR_FIGURAE_H */
