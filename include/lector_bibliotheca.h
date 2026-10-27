/* lector_bibliotheca.h - bibliotheca codicum EPUB (lector L3):
 * directorium ~/.rhubarb/codices/ (vicus: RHUBARB_CODICES vincit),
 * liber unus per dc:identifier (fila eiusdem identitatis: primum nomine
 * fili), fila .epub non legibilia nominata (vitium), cetera ignorata.
 * Positio legendi per librum in volumine (plagula "lector/positio/
 * <identitas %XX>"; ultima mutatio vincit, ex quovis latere). Communis
 * omnibus lateribus: vicus semel aperit. */

#ifndef LECTOR_BIBLIOTHECA_H
#define LECTOR_BIBLIOTHECA_H

/* <aedilis corpus="lib/lector_bibliotheca.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "codex.h"

nomen structura LectorBibliotheca LectorBibliotheca;   /* opaca */

nomen structura {
    chorda via;         /* filum */
    chorda identitas;   /* vacua si non legitur */
    chorda titulus;     /* dc:title; nomen fili si vacuum */
    chorda auctor;
    chorda vitium;      /* non vacuum = non legitur */
} LectorLiber;

nomen structura {
       i32 caput;
       i32 initium;     /* initium sententiae (octeti in capite) */
    chorda vestigium;   /* octeti primi sententiae (XXXII summum) */
} LectorPositio;

/* directorium legere; volumen NIHIL licet (positiones non servantur) */
LectorBibliotheca*
lector_bibliotheca_aperire (
     constans character* via,
                Volumen* volumen,
                Piscina* piscina,
    InternamentumChorda* intern);

i32
lector_bibliotheca_numerus (
    constans LectorBibliotheca* b);

/* liber k: legibiles ordine tituli (sine casu litterarum), deinde non
 * legibiles ordine fili; NIHIL extra ambitum */
constans LectorLiber*
lector_bibliotheca_liber (
    constans LectorBibliotheca* b,
                           i32  k);

/* per identitatem; NIHIL si abest */
constans LectorLiber*
lector_bibliotheca_invenire (
    constans LectorBibliotheca* b,
                        chorda  identitas);

/* codex libri, semel apertus et servatus; NIHIL et *vitium si non */
Codex*
lector_bibliotheca_codex (
          LectorBibliotheca* b,
       constans LectorLiber* liber,
                     chorda* vitium);

/* positio servata libri; FALSUM si nulla (aut sine volumine) */
b32
lector_positionem_legere (
    LectorBibliotheca* b,
               chorda  identitas,
        LectorPositio* positio,
              Piscina* piscina);

/* positionem servare (plagula; ultima vincit) */
b32
lector_positionem_scribere (
         LectorBibliotheca* b,
                    chorda  identitas,
    constans LectorPositio* positio);

/* progressus 0..100 (octeti capitum ante positionem / omnes); -1 si
 * nulla positio aut liber non legitur. Longitudines capitum semel per
 * librum mensae et in volumine servatae (plagula lector/longitudines/
 * <id>, signum: capita et mensura fili - liber redonatus remetitur). */
s32
lector_progressus (
          LectorBibliotheca* b,
       constans LectorLiber* liber);

/* emendatio ancorata (lector L4): genus, caput, positio (octeti in
 * textu capitis), vestigium (octeti primi sententiae ibi incipientis,
 * XXXII summum) */
nomen structura {
    CodexEmendationisGenus genus;
                       i32 caput;
                       i32 positio;
                    chorda vestigium;
} LectorEmendatio;

/* emendationes libri (Xar de LectorEmendatio, ordine scripturae);
 * vacuum si nullae; NIHIL sine volumine. Plagula "lector/emendationes/
 * <identitas %XX>" */
Xar*
lector_emendationes_legere (
    LectorBibliotheca* b,
               chorda  identitas,
              Piscina* piscina);

/* omnes scribere (ultima vincit) */
b32
lector_emendationes_scribere (
    LectorBibliotheca* b,
               chorda  identitas,
                  Xar* emendationes);

/* favores libri (Xar de LectorPositio); plagula "lector/favores/
 * <identitas %XX>" */
Xar*
lector_favores_legere (
    LectorBibliotheca* b,
               chorda  identitas,
              Piscina* piscina);

b32
lector_favores_scribere (
    LectorBibliotheca* b,
               chorda  identitas,
                  Xar* favores);

/* versio emendationum et favorum in processu: crescit cum quaevis
 * scribuntur (quovis latere) - latera capita in cache renovant (L4) */
i32
lector_bibliotheca_versio (
    constans LectorBibliotheca* b);

#endif /* LECTOR_BIBLIOTHECA_H */
