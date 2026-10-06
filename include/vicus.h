/* vicus.h - hospes applicationum: repositorium unum, tabulae,
 * montationes
 * (insula-rami-plan T1b)
 *
 * Vicus (via Romana insularum) repositorium UNUM possidet: radices
 * <vicus> (durabilis, ephemera), liberi = montationes applicationum
 * (rami, canonibus suis iudicati). Volumen UNUM: documenta tabularum in
 * spatiis suis (id), index tabularum in plagula 'vicus/tabulae'
 * (tabulae cum activa, tabula cum id genus titulus - configuratio,
 * non historia: superscribitur). Vicus nullam applicationem novit:
 * principale genera registrat (titulus -> magnitudo montationis +
 * functio montandi).
 *
 * Genus ignotum in indice (volumen ab hospite recentiore): tabula
 * servatur, non montatur, causa nominatur. Addere generis ignoti
 * recusatur. */

#ifndef VICUS_H
#define VICUS_H

/* <aedilis corpus="lib/vicus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"

/* montatio applicationis in sedem (magnitudinis registratae) */
nomen b32 (*VicusMontator)(
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo);

nomen structura {
            chorda titulus;
    memoriae_index mensura;     /* montationis (sedes) */
     VicusMontator montare;
} VicusGenus;

nomen structura {
                 chorda  id;
                 chorda  genus;
                 chorda  titulus;
    constans VicusGenus* descriptio;   /* NIHIL = genus ignotum */
                 vacuum* montatio;       /* sedes montationis */
                    b32  montata;
} VicusTabula;

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
     constans character* radix;       /* praefixum viarum canonum */
                    i32  latitudo;
                    i32  altitudo;
     InsulaRepositorium* repo;
                    Xar* genera;      /* Xar de VicusGenus */
                    Xar* tabulae;     /* Xar de VicusTabula */
                 chorda  activa;
                 chorda  causa;
} Vicus;

Vicus*
vicus_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo);

b32
vicus_genus_addere (
                  Vicus* v,
     constans character* titulus,
         memoriae_index  mensura,
          VicusMontator  montare);

/* repositorium creatur, index e volumine legitur (absens: ordinarius,
 * qui scribitur), tabulae montantur. FALSUM si index malus aut
 * repositorium deficit; tabula generis ignoti praeteritur (causa). */
b32
vicus_aperire (
                  Vicus* v,
     constans character* index_ordinarius);

/* tabulam novam montare et indicem servare; FALSUM si genus ignotum,
 * id geminum aut montatio deficit */
b32
vicus_tabulam_addere (
                  Vicus* v,
     constans character* genus,
     constans character* id,
     constans character* titulus);

i32
vicus_numerus_tabularum (
    constans Vicus* v);

VicusTabula*
vicus_tabula (
    constans Vicus* v,
               i32  index);

/* tabula activa; NIHIL si nulla */
VicusTabula*
vicus_activa (
    constans Vicus* v);

/* activam ponere (insula ephemera et index servatus); FALSUM si id
 * ignotum */
b32
vicus_activam_ponere (
                  Vicus* v,
     constans character* id);

chorda
vicus_causa (
    constans Vicus* v);

#endif /* VICUS_H */
