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
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"

/* linea tabularum (T2b): una, cellula domus VI x VIII */
#define VICUS_CELLULA_LATITUDO  VI
#define VICUS_CELLULA_ALTITUDO  VIII
#define VICUS_ALTITUDO_TABULARUM VICUS_CELLULA_ALTITUDO

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

/* gestum applicationis in Motum ponere (T3a) - e.g.
 * scriba_gestum_ponere */
nomen vacuum (*VicusGestor)(
     Motus* motus,
    vacuum* ctx);

/* pulsus montationis vivae (vicus-latera S1c): mutatum = quadrum
 * pingendum (si activa); finitus = vita exiit (concha), tabula
 * manet sed non iam pulsatur */
nomen structura {
    b32 mutatum;
    b32 finitus;
} VicusPulsus;

nomen VicusPulsus (*VicusPulsator)(
    vacuum* ctx);

/* facies montationis (T2a): quod hospes ab applicatione activa
 * accipit - registra, componere, fons imaginum, gestus (T3a), pulsus
 * (S1c). Ante describere tota nullatur: campi omissi = absentes. */
nomen structura {
      ActioRegistrum* actiones;
     FiguraRegistrum* figurae;
           Componere  componere;
              vacuum* componere_ctx;
           ImagoFons  fons;           /* NIHIL = nullae imagines */
              vacuum* fons_ctx;
         VicusGestor  gestum_ponere;  /* NIHIL = nullus gestus */
              vacuum* gestum_ctx;
       VicusPulsator  pulsare;        /* NIHIL = non vivit */
              vacuum* pulsare_ctx;
                 b32  vivit_in_fundo; /* pulsatur etiam non activa */
} VicusFacies;

nomen vacuum (*VicusDescriptor)(
         vacuum* montatio,
    VicusFacies* facies);

nomen structura {
             chorda titulus;
     memoriae_index mensura;     /* montationis (sedes) */
      VicusMontator montare;
    VicusDescriptor describere;
} VicusGenus;

nomen structura {
                 chorda  id;
                 chorda  genus;
                 chorda  titulus;
    constans VicusGenus* descriptio;   /* NIHIL = genus ignotum */
                 vacuum* montatio;       /* sedes montationis */
                    b32  montata;
            VicusFacies  facies;         /* si montata */
                    b32  finita;         /* concha exiit: titulus
                                          * "[exitus]", non iam
                                          * pulsatur */
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
         ActioRegistrum* actiones;    /* hospitis + activae (T2a) */
        FiguraRegistrum* figurae;
                  Motus* motus;       /* ligatus (T3a); NIHIL nullus */
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
          VicusMontator  montare,
        VicusDescriptor  describere);

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

/* registra hospitis (T2a): dispensatori et glutino SEMEL dantur;
 * in aperire et commutatione vacantur et ex activa implentur -
 * indices numquam mutantur */
ActioRegistrum*
vicus_actiones (
    constans Vicus* v);

FiguraRegistrum*
vicus_figurae (
    constans Vicus* v);

/* ImagoFons compositus: ad fontem tabulae activae delegat (ctx =
 * Vicus*) */
constans Imago*
vicus_imago_fons (
     chorda  provenientia,
     vacuum* ctx);

/* Componere-formata (dispensator.h), ctx = Vicus*: radix (actio
 * 'vicus.magnitudo'), linea tabularum (PARTES_INDEX, figura hospitis
 * ex Vicus legit), arbor applicationis ACTIVAE (componere eius)
 * infra lineam tabularum translata. Applicatio superficiem suam ex
 * ramo legit - hospes eam scribit (aperire, mutatio magnitudinis). */
Componens*
vicus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* Dispensatorem ligare (T3a, T3b) - post vicus_aperire, semel:
 * - Motus eius: ramus = ramus activae (focus, effusio pan/zoom),
 *   gestus = gestus activae. In commutatione: gestus relinquentis et
 *   pan/zoom in ramum RELINQUENTIS effunduntur (gestus non effusus
 *   commutationem recusat), captura et ictus pendens abiciuntur,
 *   deinde ramus et gestus advenientis ponuntur.
 * - destinatio hospitis: Ctrl-A et, dum praefixum pendet, claves et
 *   textus ad radicem (applicatio ea numquam videt); cetera
 *   geometrica. */
vacuum
vicus_dispensatorem_ligare (
          Vicus* v,
    Dispensator* d);

/* Pulsus unus (vicus-latera S1c): activa semper, ceterae solum si
 * vivit_in_fundo (decisio VI: in fundo legitur, non pingitur); finita
 * numquam. VERUM si quadrum pingendum: activa mutata, aut tabula
 * nunc finita (titulus in linea mutatur). Index durabilis non
 * tangitur. */
b32
vicus_pulsare (
    Vicus* v);

chorda
vicus_causa (
    constans Vicus* v);

#endif /* VICUS_H */
