/*
 * statua_glb_scriptor.h - scriptor GLB: statua in octetos
 *
 * Inversum lectoris (statua_glb.h): Statua finita -> plagula GLB
 * (glTF 2.0 binarium) quam statua_glb_legere in statuam AEQUALEM
 * relegit. Sic res plasticae in volumen eadem forma eunt qua
 * statuae emptae, et omnia quae GLB legunt eas sine mutatione
 * accipiunt (project-specs/plastica-spec.md IV.4).
 *
 * SCRIBIT: nodos omnes (translatio, rotatio, scala; liberi; radices
 * in scaena una), retia (rete unum = mesh unum; tractus unus =
 * primitivum unum cum POSITION, NORMAL, TEXCOORD_0 et indicibus),
 * materias (color basis, textura, excisio alpha ut MASK cum limine,
 * emissio, bilateralis, involutio s et t), texturas ut PNG insertas,
 * signa nodorum ut extras.tags ("walkable", "collidable", "water"),
 * cyclos ut animationes (LINEAR aut STEP) cum titulis suis; canales
 * cycli unius qui tempora eadem habent tabulam temporum unam
 * communicant.
 *
 * AEQUALIS quid significet: lege, scribe, lege iterum - lectio
 * altera primae par est campo pro campo (nodi, retia, tractus,
 * vertices, indices, materiae, pixela texturarum, cycli). Statua
 * manu facta eodem modo redit SI tractus eius vertices proprios
 * contiguos tenent (ut lector et plastica semper faciunt); tractus
 * qui vertices communicant recte pinguntur sed vertices duplicantur.
 * Textura quam nulla materia adhibet non scribitur.
 *
 * NON SERVAT (Statua ea non tenet): nomina materiarum, imagines
 * JPEG ut JPEG (PNG fiunt), colorem emissionis (albus scribitur),
 * dispositionem pristinam plagulae. Nomina nodorum scribuntur si
 * tabula titulorum datur (plastica: PlasticaFructus.partes); lector
 * ea neglegit, instrumenta aliena ea ostendunt.
 *
 * DETERMINATUS: statua eadem, octeti idem. Numeri fluitantes in
 * JSON ita scribuntur ut f32 exacte redeat (lector rotationem iam
 * unitariam non tangit).
 *
 * RECUSAT (chorda vacua): tractum sine triangulis et rete sine
 * tractibus - glTF ea non admittit.
 *
 * Indices XVI bit ubi tractus minus quam 65536 vertices habet,
 * aliter XXXII.
 */

#ifndef STATUA_GLB_SCRIPTOR_H
#define STATUA_GLB_SCRIPTOR_H

/* <aedilis corpus="lib/statua_glb_scriptor.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "statua.h"

/*
 * statua_glb_scribere - statuam in octetos GLB scribere
 *
 * @statua:  finita (statua_finire)
 * @tituli:  nomina nodorum, statua->nodorum chordae (vacua = sine
 *           nomine); NIHIL = nulla nomina
 * @piscina: octeti exitus et laboris
 *
 * Redde: plagula tota; chorda vacua (datum NIHIL) si statua aut
 * piscina NIHIL, statua sine nodis, numerus non finitus in ea, aut
 * memoria deficit
 */
chorda
statua_glb_scribere (
    constans Statua* statua,
    constans chorda* tituli,
            Piscina* piscina);

#endif /* STATUA_GLB_SCRIPTOR_H */
