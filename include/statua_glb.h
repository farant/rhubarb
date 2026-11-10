/*
 * statua_glb.h - lector GLB (glTF 2.0 binarium) in statuam
 *
 * Octeti plagulae -> Statua finita (statua.h), aut recusatio cum
 * causa verbis planis Anglicis (latus eam usori ostendit). Forma
 * plagulae hic tota manet: nihil extra hunc modulum de glTF scit.
 *
 * LEGIT: continens GLB (JSON + BIN unum), scaenam ordinariam, nodos
 * (TRS aut 'matrix'), retia triangulorum (POSITION, NORMAL,
 * TEXCOORD_0; indices 8 / 16 / 32 bit aut nulli), materias (color
 * basis, textura, excisio alpha, emissio, bilateralis), imagines
 * insertas PNG / JPEG, animationes nodorum (LINEAR, STEP).
 *
 * APPROXIMAT (figura et colores picti recti manent): alpha BLEND fit
 * excisio ad 0.5; filtrum lineare pingitur proximum; colores
 * verticum, coordinatae secundae, tabulae metalli / asperitatis /
 * normalium / occlusionis, camerae, luces et extensiones optionales
 * ignotae negleguntur; normales absentes per faciem computantur;
 * textura absens alba est.
 *
 * RECUSAT: pelles (skins), scopos morphicos, extensiones requisitas
 * ignotas, plagulas aut imagines externas (uri), primitiva non
 * triangula, interpolationem CUBICSPLINE, accessores sparsos, et
 * quemlibet offset aut numerum extra fines plagulae.
 *
 * NUMQUAM RUIT: octeti hostiles aut truncati recusationem dant.
 */

#ifndef STATUA_GLB_H
#define STATUA_GLB_H

/* <aedilis corpus="lib/statua_glb.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "statua.h"

nomen structura {
       b32  successus;
    Statua* statua;    /* finita (statua_finire); NIHIL si recusata */
    chorda  error;     /* causa recusationis, Anglice, sine linea
                        * nova; vacua si successus */
} StatuaFructus;

/*
 * statua_glb_legere - octetos GLB in statuam legere
 *
 * @octeti:  plagula tota (non mutatur; statua nihil in ea tenet -
 *           omnia in piscinam copiantur)
 * @piscina: statua, texturae decodificatae et nuntius erroris
 *
 * Redde: successus VERUM et statua finita, aut FALSUM et error
 */
StatuaFructus
statua_glb_legere (
     chorda  octeti,
    Piscina* piscina);

#endif /* STATUA_GLB_H */
