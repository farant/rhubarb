/* mandatum.h - Mandata delineandi: sex primitiva et index eorum
 *
 * Effusio PURA functionis pingere (ludus-brainstorm.md XV par. 1).
 * Nulla identitas, nulla actio, nulla pars in primitivis - ea in
 * arbore componentium vivunt (componens.h). Coetus solus nidificat:
 * sectio et translatio proprietates coetus sunt, non mandata
 * push/pop, ergo discrepantia structuraliter impossibilis.
 *
 * In memoria: structurae typatae in xar; STML est SERIALIZATIO
 * (imago, inspectio, quaestio, exspectationes replay).
 *
 * USUS:
 *   Mandata* m = mandata_creare(piscina, intern);
 *   i32 c = mandata_coetus_incipere(m, fines, VERUM, 0, 0, I, id);
 *   mandata_rectangulum(m, fines, color, VERUM);
 *   mandata_coetus_finire(m, c);
 *   chorda s = mandata_scribere_stml(m, piscina, VERUM);
 */

#ifndef MANDATUM_H
#define MANDATUM_H

/* <aedilis corpus="lib/mandatum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"


/* ==================================================
 * GEOMETRIA COMMUNIS (componens.h quoque utitur)
 * ================================================== */

nomen structura { s32 x; s32 y; } Punctum;

nomen structura { s32 x; s32 y; s32 latitudo; s32 altitudo; } Fines;

b32
fines_continet (
      Fines f,
    Punctum p);


/* ==================================================
 * TYPI
 * ================================================== */

nomen enumeratio {
    MANDATUM_COETUS = ZEPHYRUM,
    MANDATUM_RECTANGULUM,
    MANDATUM_LINEA,
    MANDATUM_POLYGONUM,
    MANDATUM_IMAGO,
    MANDATUM_TEXTUS,
    MANDATUM_GENUS_NUMERUS
} MandatumGenus;

/* Color: signum thematis (thema.h resolvit), index palettae, aut
 * RGBA literalis (0xRRGGBBAA). Rasterizator solus resolvit. */
nomen enumeratio {
    COLOR_MANDATI_THEMA = ZEPHYRUM,
    COLOR_MANDATI_INDEX,
    COLOR_MANDATI_RGBA
} ColorMandatiGenus;

nomen structura {
    ColorMandatiGenus genus;
                  i32 valor;
} ColorMandati;

nomen structura {
    MandatumGenus  genus;
            Fines  fines;
     ColorMandati  color;
              b32  impletum;            /* rectangulum/polygonum */
              i32  crassitudo;          /* linea */
          Punctum* puncta;              /* linea (II) / polygonum (n) */
              i32  numerus_punctorum;
           /* textus: contentum; imago: fons */
           chorda textus;
              i32 fons;                /* textus: index fontis */
    /* coetus */
              b32 sectio;              /* praecidere ad fines */
          Punctum translatio;
              i32 scala;               /* >= I, integer */
           /* id componentis; INSPECTORI SOLI */
           chorda provenientia;
              i32 magnitudo_arboris;   /* coetus: mandata in subarbore
                                           se incluso; alia: I */
} Mandatum;

nomen structura {
                    Xar* elementa;      /* Xar de Mandatum */
                Piscina* piscina;
    InternamentumChorda* intern;
} Mandata;


/* ==================================================
 * INDEX
 * ================================================== */

Mandata*
mandata_creare (
                Piscina* piscina,
    InternamentumChorda* intern);

vacuum
mandata_vacare (
    Mandata* m);

i32
mandata_numerus (
    constans Mandata* m);

Mandatum*
mandata_obtinere (
    constans Mandata* m,
                 i32  index);


/* ==================================================
 * EMISSIO
 * ================================================== */

vacuum
mandata_rectangulum (
         Mandata* m,
           Fines  fines,
    ColorMandati  color,
             b32  impletum);

vacuum
mandata_linea (
         Mandata* m,
         Punctum  a,
         Punctum  b,
             i32  crassitudo,
    ColorMandati  color);

vacuum
mandata_polygonum (
              Mandata* m,
     constans Punctum* puncta,
                  i32  numerus,
         ColorMandati  color,
                  b32  impletum);

vacuum
mandata_imago (
     Mandata* m,
      chorda  fons,
       Fines  fines);

vacuum
mandata_textus (
         Mandata* m,
             s32  x,
             s32  y,
          chorda  textus,
             i32  fons,
    ColorMandati  color);

/* Redde indicem coetus; finire eum cum eodem indice. */
i32
mandata_coetus_incipere (
     Mandata* m,
       Fines  fines,
         b32  sectio,
         s32  translatio_x,
         s32  translatio_y,
         i32  scala,
      chorda  provenientia);

vacuum
mandata_coetus_finire (
     Mandata* m,
         i32  index);


/* ==================================================
 * SERIALIZATIO ET COMPARATIO
 * ================================================== */

chorda
mandata_scribere_stml (
    constans Mandata* m,
             Piscina* piscina,
                 b32  pulchrum);

Mandata*
mandata_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern);

b32
mandata_aequalia (
    constans Mandata* a,
    constans Mandata* b);


#endif /* MANDATUM_H */
