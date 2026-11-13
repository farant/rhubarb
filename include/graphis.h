/*
 * graphis.h - graphis: descriptio formarum planarum in picturam
 *
 * Pictura stans (CCCLX x CCXL, XVI colores Aquinatis) ex plagula
 * STML fit: arbor formarum planarum, quaeque cum coloribus, exemplari
 * et vi. Functio pura: plagula eadem, octeti idem in omni machina;
 * arithmetica fluitans nulla. Nihil de scaenis, de mundo aut de vico
 * scit (project-specs/graphis-spec.md 5, 10-17).
 *
 * DESCRIPTIO:
 *
 *   <pictura titulus="casa">
 *     <rectangulum fines="64 34 232 116" colores="9 15"
 *                  exemplar="horizontalis_latus" vis="0.45"
 *                  stilus="1 0"/>
 *     <polygonum puncta="0,0 64,34 64,150 0,204" colores="1 9"/>
 *     <ovum titulus="stragulum" fines="96 186 150 40"
 *           colores="6 15" exemplar="rhombus_lineatus" vis="1"/>
 *     <linea puncta="64,34 296,34" stilus="3 1"/>
 *     <grex locus="196 150"> ... </grex>
 *   </pictura>
 *
 * COORDINATAE: pixela integra, x dextrorsum, y deorsum, origo in
 * angulo sinistro summo; -4096 .. 4096 (punctum decimale recusatur).
 * 'grex' locum suum omnibus intra se addit. Forma posterior priorem
 * tegit. Fundus ante formas: color V (albus).
 *
 * FORMAE:
 *
 *   rectangulum  fines="x y latitudo altitudo"
 *   ovum         fines="x y latitudo altitudo" (ellipsis inscripta)
 *   polygonum    puncta="x,y x,y ..." (III .. CCLVI; clauditur ipsum)
 *   linea        puncta="x,y x,y ..." (II .. CCLVI; stilus necessarius,
 *                colores non accipit)
 *
 * ATTRIBUTA FORMAE:
 *
 *   colores   unus aut duo. Color: index palettae 0 .. 15 aut
 *             #rrggbb. Unus: planus. Duo: primus bitos positos
 *             exemplaris pingit, secundus vacuos; '-' loco secundi:
 *             vacui quod subest relinquunt (tegumentum)
 *             (tegumentum sine titulo dominum pixeli non mutat)
 *   exemplar  nomen ex tabula (graphis_exemplar_titulus): nomina
 *             exemplaria.h litteris minoribus sine praefixo
 *             (solidus, tessellatum, latericium, textum ...).
 *             Ordinarium: solidus
 *   vis       quantum exemplaris appareat: 0, aut 0.4 .. 1 (tres
 *             loci decimales summum). 0: color planus lenis.
 *             Ordinarium: 0.6 si colores duo et exemplar datum,
 *             aliter 0. Tegumentum semper 1
 *   stilus    "crassitudo color": margo (aut linea ipsa), post
 *             impletionem pictus, numquam exemplari; crassitudo
 *             I .. XVI
 *   titulus   forma regio fit: pixela eius (cum margine) in tabula
 *             dominorum ei tribuuntur, nisi forma posterior ea tegit
 *
 * COLOR FONTIS pixeli: planus x (1 - vis) + pixelum exemplaris x vis,
 * ubi planus = duo colores secundum densitatem exemplaris mixti
 * (biti positi / LXIV). Color unus: is ipse.
 *
 * PICTURA: fons illustratus totus semel per
 * dithering_atkinson_colorum (omnes XVI colores) transit. Fons qui
 * solos colores palettae tenet, luce plena, immutatus exit.
 *
 * LUX (graphis_lumen.h): attributa radicis lux, tinctura, umbra;
 * elementum <lumen> (liber radicis solum); attributum formae
 * lucens="verum". Sine his pictura immutata manet. Partes ('usus')
 * hic nondum sunt: T3 eas addit; usque tum recusantur ut ignotae.
 *
 * RECUSAT, cum linea: plagulam quae STML non est; radicem aliam
 * quam <pictura>; elementum aut attributum ignotum; numerum absentem,
 * non integrum aut extra fines; colorem aut exemplar ignotum;
 * magnitudinem formae nullam aut negativam; polygonum minus III
 * punctis; formam sine coloribus et sine stilo; vim inter 0 et 0.4;
 * formam totam extra picturam; titulum iteratum aut vacuum; plus
 * quam CCLV titulos; formam titulatam quae post omnes formas minus
 * LXIV pixela possidet; attributa 'latitudo' aut 'altitudo' radicis
 * quae CCCLX et CCXL non sunt (abesse licet).
 */

#ifndef GRAPHIS_H
#define GRAPHIS_H

/* <aedilis corpus="lib/graphis.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "graphis_lumen.h"

#define GRAPHIS_LATITUDO          CCCLX
#define GRAPHIS_ALTITUDO          CCXL
#define GRAPHIS_TITULI_MAXIMI     CCLV
#define GRAPHIS_REGIO_MINIMA      LXIV
#define GRAPHIS_COLOR_FUNDI       V

/* tabulae picturae: omnes GRAPHIS_LATITUDO x GRAPHIS_ALTITUDO,
 * ordine linearum a summo */
nomen structura {
        i8* fons;       /* rgba, IV octeti per pixelum (alpha CCLV):
                         * color ante picturam */
        i8* larva;      /* vis per pixelum, 0 .. CCLV; margo et
                         * color planus 0 */
         i8* domini;     /* 0 = nullus; n = tituli[n - I] */
     chorda* tituli;     /* ordine plagulae */
        i32  titulorum;
         i8* lucentes;   /* non 0 = pixelum formae lucentis */
 GraphisLux  lux;        /* lux ut descriptio eam dedit */
} GraphisTabula;

nomen structura {
              b32  successus;
    GraphisTabula* tabula;   /* NIHIL si recusata */
               i8* indices;  /* palettae 0 .. 15, unus per pixelum;
                              * NIHIL si recusata */
               i8* illustratus; /* rgba post lucem (graphis_lumen.h):
                              * hic per Atkinson transit; NIHIL si
                              * recusata */
           chorda error;    /* causa recusationis, Anglice, sine
                              * linea nova; vacua si successus */
              i32 linea;    /* linea plagulae (ab I); 0 si ignota
                              * aut successus */
} GraphisFructus;

/*
 * graphis_legere - descriptionem in picturam vertere
 *
 * @textus:  plagula tota (non mutatur; tituli in piscinam exscribuntur)
 * @piscina: tabulae, indices et nuntius erroris
 *
 * Redde: successus VERUM cum tabula et indicibus, aut FALSUM cum
 * errore et linea
 */
GraphisFructus
graphis_legere (
     chorda  textus,
    Piscina* piscina);

/*
 * graphis_exemplar_titulus - nomen exemplaris in descriptione
 *
 * @id: 0 .. EXEMPLAR_NUMERUS - I (exemplaria.h)
 *
 * Redde: nomen (C, staticum); NIHIL extra fines
 */
constans character*
graphis_exemplar_titulus (
    i32 id);

#endif /* GRAPHIS_H */
