/* scriba_liber.h - liber paginarum scribae (vicus-latera S2b)
 *
 * Paginae NOMINATAE in volumine: index in plagula 'scriba/paginae'
 * (<paginae><pagina nomen=.../>...</paginae>, ordo = ordo navigandi),
 * textus cuiusque paginae in spatio 'paginae/' + nomen (historia).
 * Documentum UNUM per nomen: visus omnes eiusdem paginae idem tenent
 * (scriba_liber_pagina semel aperit aut creat, deinde idem reddit).
 * Sine limite numeri (decisio XIV; libro_paginarum veteri C erant).
 * Liber legatum vetus (lib/libro_paginarum.c) non tangit. */

#ifndef SCRIBA_LIBER_H
#define SCRIBA_LIBER_H

/* <aedilis corpus="lib/scriba_liber.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "scriba_documentum.h"

nomen structura ScribaLiber ScribaLiber;

/* index legitur; absens: pagina "1" sola (scribitur). latitudo et
 * altitudo (cellulae) paginarum NOVARUM. NIHIL si index malus. */
ScribaLiber*
scriba_liber_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo);

/* documentum paginae (aperitur aut creatur, semel); NIHIL si nomen
 * ignotum */
ScribaDocumentum*
scriba_liber_pagina (
    ScribaLiber* l,
         chorda  titulus);

i32
scriba_liber_numerus (
    constans ScribaLiber* l);

/* nomen paginae ad indicem; vacuum si index extra */
chorda
scriba_liber_nomen (
    constans ScribaLiber* l,
                     i32  index);

/* index nominis; -1 si ignotum */
s32
scriba_liber_index (
    constans ScribaLiber* l,
                  chorda  titulus);

/* pagina nova ad finem; nomen = numerus proximus nondum adhibitus
 * ("2", "3"...); index servatur. Vacuum si scriptura deficit. */
chorda
scriba_liber_pagina_nova (
    ScribaLiber* l);

/* pagina nominata: exstans aut nova ad finem (vicus-latera S3c,
 * '$scriba(nomen)'); nomen '[a-z0-9_-]+'. NIHIL si nomen invalidum aut
 * scriptura deficit. */
ScribaDocumentum*
scriba_liber_paginam_condere (
    ScribaLiber* l,
         chorda  titulus);

#endif /* SCRIBA_LIBER_H */
