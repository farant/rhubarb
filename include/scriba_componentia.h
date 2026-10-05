/* scriba_componentia.h - componere scribae (scriba-plan S2)
 *
 * <componens/> <purus/>: arbor ex insulis (lectio) in piscinam datam;
 * nihil scribit. Columna in CELLULIS (dispositio, ut pictor 013 B3):
 * prospectus (mensa; crescens, praecisus) super lineam status (fixa).
 * In prospectu pagina (PARTES_CAMPUS): folium in pixelis, cellula una
 * ab ora (margo videtur); actio "pagina.clavis", focusabilis.
 *
 * DATA FIGURAE IN ARBORE (exemplar pictoris: PRAEDICATUM_PROPRIUS,
 * puncta = data componentis): pagina.puncta[0] = cursor (x columna,
 * y linea), puncta[1] = ancora selectionis (modo visuali solo);
 * pagina.titulus = modus ("normalis" "inserere" "visualis"). Textus
 * folii per contextum figurae (folium laboris) - ut documentum
 * pictoris.
 *
 * VOLUTIO sine statu: si folium (cum margine) prospectum excedit,
 * visus in cursorem centratur, ad oras folii limitatus (functio pura
 * cursoris; nullus status volutionis). */

#ifndef SCRIBA_COMPONENTIA_H
#define SCRIBA_COMPONENTIA_H

/* <aedilis corpus="lib/scriba_componentia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"

nomen structura {
    i32 fenestra_latitudo;   /* superficies si superficies_* absunt */
    i32 fenestra_altitudo;
    i32 cellula_latitudo;    /* Modulus: pixela = cellulae x cellula */
    i32 cellula_altitudo;
    i32 status_lineae;
} ScribaCompositio;

/* Componere-formata (dispensator.h): ctx = ScribaCompositio* */
Componens*
scriba_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

#endif /* SCRIBA_COMPONENTIA_H */
