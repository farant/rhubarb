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
#include "xar.h"

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

/* A6c: liber in spatio dato: index in plagula '<praefixum>/paginae',
 * textus paginae in spatio '<praefixum>/paginae/' + nomen (e.g.
 * "dies/2026-10-14": liber diei). scriba_liber_aperire = liber
 * communis vici (index 'scriba/paginae', paginae 'paginae/' - mos
 * prior, intactus) */
ScribaLiber*
scriba_liber_aperire_in_spatio (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
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

/* lineae textus libri in spatio dato (e.g. "dies/2026-10-19"),
 * LECTIO SOLA: liber absens = nullae lineae, nihil scribitur (index
 * non creatur - scriba_liber_aperire_in_spatio paginam "1" scriberet).
 * Paginae ordine indicis; lineae vacuae (aut spatiis solis) omissae;
 * maximum = lineae summae (0 = omnes). Xar de chorda in exitus. */
b32
scriba_liber_lineas_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                    i32  maximum,
                    Xar* exitus);

/* D5 (indagatio S2): nomina paginarum, LECTIO SOLA, ordine indicis;
 * praefixum NIHIL = liber communis vici (index 'scriba/paginae').
 * Liber absens = nulla (VERUM); FALSUM si index malus. Xar de chorda
 * in exitus. */
b32
scriba_liber_nomina_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                    Xar* exitus);

/* D5: lineae paginae unius, LECTIO SOLA (vacuae omissae, ut
 * scriba_liber_lineas_legere); praefixum NIHIL = liber communis.
 * Pagina absens = nullae lineae. */
b32
scriba_liber_paginam_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda  titulus,
                    Xar* exitus);

/* comites P2: praefixum libri ("comes/nomenclator/1"; liber in spatio
 * aperto, scriba_liber_aperire_in_spatio); liber communis (index
 * 'scriba/paginae'): vacuum */
chorda
scriba_liber_praefixum (
    constans ScribaLiber* l);

/* comites P5b: nota libri comitis in plagula '<praefixum>/comes'
 * (<comes><titulus>..</titulus><semita>..</semita></comes>; textus
 * liberorum, non attributa - scriptor attributa cruda scribit): titulus comitis ("Notes ·
 * Thursday, October 22") et semita PLENA visus comitati (vacua: visus
 * sine semitis, e.g. terminale - attributum omittitur). Scribitur
 * solum si mutata (sequela actum non addit). */
b32
scriba_liber_comitem_notare (
                Volumen* volumen,
    InternamentumChorda* intern,
     constans character* praefixum,
                 chorda  titulus,
                 chorda  semita,
                Piscina* piscina);

/* LECTIO SOLA; nota absens aut mala: FALSUM, *titulus et *semita
 * vacuae */
b32
scriba_liber_comitem_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda* titulus,
                 chorda* semita);

/* pensa K2: textus libri TOTUS cum lineis vacuis (sectiones separant),
 * LECTIO SOLA: pagina quaeque lineis suis (vacuae finales demptae),
 * paginae "\n\n" separatae; liber absens = textus vacuus (VERUM);
 * FALSUM si index malus. praefixum NIHIL = liber communis */
b32
scriba_liber_textum_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda* exitus);

#endif /* SCRIBA_LIBER_H */
