/* vicus.h - hospes applicationum: repositorium unum, tabulae,
 * montationes
 * (insula-rami-plan T1b)
 *
 * Vicus (via Romana insularum) repositorium UNUM possidet: radices
 * <vicus> (durabilis, ephemera), liberi = montationes applicationum
 * (rami, canonibus suis iudicati). Volumen UNUM: documenta montationum
 * in spatiis suis (id), dispositio in plagula 'vicus/latera'
 * (configuratio, non historia: superscribitur). Vicus nullam
 * applicationem novit: principale genera registrat (titulus ->
 * magnitudo montationis + functio montandi).
 *
 * vicus-latera S2a: tabula = par laterum. SINISTRUM = editor (genus non
 * fixum), DEXTRUM = acervus cuius ultimus in fronte. Latus montatio
 * una est; id eius = via "<tabula>_<latus>_<genus>" = id rami = spatium
 * arboris (actiones, figurae, ids intra resolvuntur). Ictus in latus
 * non focatum id focat (ictus primus solum focat).
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

/* tabulae fixae (vicus-latera decisio IX): index ordinarius tot
 * dat; vicus plures non legit */
#define VICUS_TABULAE    X
#define VICUS_SINISTRUM  ZEPHYRUM
#define VICUS_DEXTRUM    I

/* linea tabularum (T2b): una, cellula domus VI x VIII */
#define VICUS_CELLULA_LATITUDO  VI
#define VICUS_CELLULA_ALTITUDO  VIII
#define VICUS_ALTITUDO_TABULARUM VICUS_CELLULA_ALTITUDO

/* montatio applicationis in sedem (magnitudinis registratae); ctx =
 * contextus generis (vicus_genus_addere; vicus-latera S2b: liber
 * paginarum scribae communis). argumentum (S3c): '$genus(argumentum)'
 * quo latus apertum est; NIHIL = nullum */
nomen b32 (*VicusMontator)(
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx);

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
    /* ictus qui latus focat etiam agit, si latus vult (Franus: iussum
     * uno ictu). Post focum mutatum vocatur (motus iam lateri
     * aptatus); arbor = arbor composita. VERUM = actum. NIHIL = ictus
     * primus focat solum. */
                 b32  (*ictus_primus)(vacuum* ctx,
                          InsulaRepositorium* repo, Motus* motus,
                          Componens* arbor, constans Eventus* ev);
              vacuum* ictus_primus_ctx;
} VicusFacies;

nomen vacuum (*VicusDescriptor)(
         vacuum* montatio,
    VicusFacies* facies);

nomen structura {
             chorda  titulus;
     memoriae_index  mensura;     /* montationis (sedes) */
      VicusMontator  montare;
    VicusDescriptor  describere;
             vacuum* ctx;        /* montatori datur (S2b) */
} VicusGenus;

/* latus (S2a): montatio una. id = via "<tabula>_<latus>_<genus>"
 * ("3_sinistrum_scriba"; canon 'nomen' puncta non admittit) = id rami
 * repositorii = spatium arboris */
nomen structura {
                 chorda id;
                 chorda genus;
                 chorda argumentum;   /* S3c: identitas = genus +
                                        * argumentum; vacuum = nullum */
    constans VicusGenus* descriptio;   /* NIHIL = genus ignotum */
                 vacuum* montatio;       /* sedes montationis */
                    b32  montata;
            VicusFacies  facies;         /* si montata */
                    b32  finita;         /* concha exiit: titulus
                                          * "[exitus]", non iam
                                          * pulsatur */
} VicusLatus;

nomen structura {
        chorda  id;          /* "1".."10" */
    VicusLatus  sinistrum;
           Xar* acervus;     /* Xar de VicusLatus (dextrum); ultimus =
                              * in fronte; numquam vacuus (decisio
                              * VIII) */
           i32 focus;       /* VICUS_SINISTRUM aut VICUS_DEXTRUM */
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
                    Xar* petitiones;  /* S3c: aperitiones in acervo
                                       * pendentes (vicus_pulsare) */
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
        VicusDescriptor  describere,
                 vacuum* ctx);

/* repositorium creatur, dispositio e volumine legitur (absens:
 * ordinaria, quae scribitur), latera montantur. Forma:
 * <tabulae activa="1"><tabula id="1" focus="sinistrum"><latus
 * genus="scriba"/><acervus><latus genus="terminale"/></acervus>
 * </tabula>...</tabulae>; acervus absens = latus sinistrum iteratum.
 * S3c: latus 'id' et 'argumentum' optionalia (absens id = ordinarium
 * "<tabula>_<latus>_<genus>").
 * FALSUM si dispositio mala aut repositorium deficit; latus generis
 * ignoti praeteritur (causa). */
b32
vicus_aperire (
                  Vicus* v,
     constans character* index_ordinarius);

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

/* latus tabulae: VICUS_SINISTRUM aut VICUS_DEXTRUM (= frons
 * acervi); NIHIL si tabula NIHIL */
VicusLatus*
vicus_latus (
    VicusTabula* t,
            i32  latus);

/* latus focatum tabulae activae; NIHIL si nulla */
VicusLatus*
vicus_latus_focatum (
    constans Vicus* v);

/* focum tabulae activae ad latus ponere (ictus idem facit): Motus
 * ramum, spatium, gestum sequitur; pendentia relinquentis effunduntur
 * ut in commutatione tabularum. Dispositio servatur. */
b32
vicus_focum_ponere (
    Vicus* v,
      i32  latus);

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
 * 'vicus.radix'), linea tabularum (PARTES_INDEX, figura hospitis ex
 * Vicus legit), arbores laterum VISIBILIUM tabulae activae (componere
 * cuiusque) infra lineam translatae, sinistrum ad x 0, dextrum post
 * dimidium (cellulis rotundatum), radix cuiusque spatio lateris
 * signata; divisor post latera. Applicatio superficiem suam ex ramo
 * legit - hospes eam scribit (aperire, mutatio magnitudinis). */
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

/* Pulsus unus (vicus-latera S1c, S2a): latera VISIBILIA tabulae
 * activae (sinistrum, frons acervi) semper, cetera (acervi, tabulae
 * aliae) solum si vivit_in_fundo (decisio VI: in fundo legitur, non
 * pingitur); finita numquam. VERUM si quadrum pingendum: latus
 * visibile mutatum, aut latus nunc finitum (titulus in linea
 * mutatur). Dispositio durabilis non tangitur. */
b32
vicus_pulsare (
    Vicus* v);

/* S3c: latus (genus, argumentum) in acervo tabulae activae: iam
 * praesens in frontem venit, aliter montatur (id novum: id
 * ordinarium, deinde '_2', '_3'...) et frons fit; focus ad dextrum;
 * dispositio servatur (id et argumentum cuiusque lateris). PETITIO:
 * applicatur in vicus_pulsare proximo, numquam intra tractationem
 * eventus (iussum ex scriba inter tractationem currit). FALSUM si
 * genus ignotum. */
b32
vicus_acervo_aperire (
                  Vicus* v,
     constans character* genus,
     constans character* argumentum);

chorda
vicus_causa (
    constans Vicus* v);

#endif /* VICUS_H */
