/* scaena_latus.h - latus scaenae ($scene): pictura stans cum rebus
 * tangendis, in latere vici (project-specs/graphis-spec.md 20-24)
 *
 * DISPOSITIO (superficies CDLXXX x DLXXVI, omnia semel aucta):
 *
 *   +--------------------------------------+
 *   | < 1/2 >  hut-hearth           hearth |  caput (ordo unus)
 *   +--------------------------------------+
 *   |                                      |
 *   |        pictura CDLXXX x CCCXX        |
 *   |                                      |
 *   +--------------------------------------+
 *   | descriptio prospectus, in lineas     |  textus
 *   | fracta                               |
 *   |                                      |
 *   | textus rei novissime tactae          |
 *   +--------------------------------------+
 *
 * CAPUT: sagittae et numerus (prior 1/2 proxima), titulus
 * prospectus; ad dextram, obscurior, titulus rei sub indice muris
 * (aut clavibus electae). PICTURA: indices palettae prospectus,
 * medio lateris; latus angustius eam utrimque aequaliter secat
 * (pars media manet), brevius ab imo; numquam extra scribit. Res
 * in parte secta mure non tanguntur, clavibus (j k Enter) tamen.
 * TEXTUS: descriptio; linea vacua; textus rei tactae. Linea una
 * vacua inter picturam et textum; duae cellae litterarum ad
 * sinistram et dextram.
 * MARGO: cella una circa omnia (VI x VIII); intra eum fundus planus
 * - textura foci lateris solum in margine apparet. Pictura in
 * latere CDLXXX lato margines laterales tegit.
 * Lineae quae in latus non cadunt non pinguntur (in hoc gradu nulla
 * volutatio).
 *
 * PROSPECTUS ex volumine (scaena_repositorium.h), ordine
 * additionis. Initio: titulus status; aliter primus indicis. Latus
 * volumen_seq_ultimum semel per secundam inspicit: prospectus ab
 * instrumento (bin/scaena condere) mutatus sine restitutione
 * apparet.
 *
 * MUS: motus super picturam rem sub indice nominat (caput). Ictus
 * sinister in re: textus eius in parte textus ostenditur. Si res
 * exitum in prospectum alium habet, latus eo transit ET textus
 * exitus sub descriptione nova manet ("You turn your back to the
 * fire."). Exitus "mundus": textus solus (in hoc gradu nullum
 * ostium in $world). Ictus extra res: nihil. Sagittae capitis:
 * prospectus prior / proximus.
 *
 * CLAVES: [ ] prospectus prior / proximus (circulo; ad omnia
 * inspicienda sine exitibus). j k rem proximam / priorem eligunt
 * (titulus in capite, ut mus); Enter electam tangit.
 *
 * NUNTII loco picturae: prospectus nulli in volumine ("No scenes
 * yet" et mandatum bin/scaena condere); massa quam lector recusat.
 *
 * STATUS in ramo (manifestum apps/scaena/organum.stml, persistens):
 *   prospectus  titulus prospectus ostensi
 *   tactum      res novissime tacta: "<prospectus>/<res>" (prospectus
 *               in quo res EST - post exitum non is qui ostenditur);
 *               vacuum = nulla. Mutato prospectu per [ ] aut
 *               sagittas vacatur.
 * Res sub indice (mus, j k) KINETICA est: in montatione, numquam in
 * statu.
 *
 * HOSPES: latus aliud (mundus) scaenam in ramo SUO ostendit
 * (project-specs/ostium-spec.md 12). Hospes campos 'prospectus' et
 * 'tactum' in manifesto suo habet et actionem scriptoris nominat.
 * Montatio hospitis organum non legit, registra non creat: hospes
 * eventus et figuram ipse huc mittit.
 *
 * DIFFERT a latere $scene:
 *   - sagittae, numerus "1/2" et claves [ ] absunt: prospectus per
 *     exitus solos mutatur;
 *   - 'prospectus' ignotus NON in primum indicis cadit: nuntius
 *     "This scene cannot be shown:" cum causa, et sub eo monitum
 *     hospitis (e.g. "E: go back");
 *   - exitus "mundus" tactus nihil scribit et FORAS reddit: hospes
 *     scaenam claudit (in $scene textus solus manet).
 * Cetera eadem: caput (titulus prospectus, res sub indice), pictura,
 * descriptio, textus rei tactae, mus, j k Enter.
 *
 * Hospes scaena_latus_pulsare in pulsu suo et ante figuram vocat;
 * scaena_latus_imago_fons et scaena_latus_dimittere ut semper.
 */
#ifndef SCAENA_LATUS_H
#define SCAENA_LATUS_H

/* <aedilis corpus="lib/scaena_latus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "fasti.h"
#include "imago_typus.h"
#include "volumen.h"
#include "organum_montatio.h"
#include "scaena.h"

/* pulsus (ms): inspectio voluminis */
#define SCAENA_LATUS_MORA_INSPECTIONIS M

nomen enumeratio {
    SCAENA_LATUS_NIHIL = ZEPHYRUM,  /* eventus nihil mutavit */
    SCAENA_LATUS_MUTATUM,
    SCAENA_LATUS_FORAS              /* exitus "mundus" tactus */
} ScaenaLatusEventum;

nomen structura {
            InsulaRamus  ramus;
         ActioRegistrum* actiones;
        FiguraRegistrum* figurae;
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;      /* NIHIL = nihil ostenditur */
                    i32  latitudo;     /* superficies montationis */
                    i32  altitudo;
    /* index (relectus cum seq voluminis mutatur) */
                Piscina* piscina_indicis;
                    Xar* index;        /* de ScaenaInscriptio */
                    s64  seq_visum;
    /* prospectus lectus et pictura eius */
                Piscina* piscina_prospectus;
       ScaenaProspectus  prospectus;
                    b32  lectus;       /* prospectus valet */
                 chorda  sigillum_lecti;
                 chorda  causa;        /* massa recusata; vacua si
                                        * bonus aut nullus */
                  Imago quadrum;      /* RGBA; pixela NIHIL = nullum */
                 chorda quadri_titulus;  /* "scaena:<id rami>" */
    /* textus rei tactae (ex prospectu suo lectus) */
                 chorda tacti_textus;
                 chorda tacti_clavis; /* status 'tactum' cui textus
                                        * respondet */
    /* kinetica */
                    i32 super;        /* res sub indice: 0 nulla,
                                        * n = res[n - I] */
    /* hospes */
     constans character* scriptor;     /* actio quae campos scribit */
                    b32  hospes;
     constans character* monitum;      /* sub nuntio recusationis;
                                        * NIHIL = nullum */
                    b32 foras;        /* kineticum: exitus "mundus"
                                        * in eventu hoc tactus */
} ScaenaLatusMontatio;

/*
 * scaena_latus_montare - latus parare
 *
 * @volumen: NIHIL licet (nuntius "No scenes yet")
 * @id:      id rami; NIHIL = radix repositorii
 *
 * Redde: FALSUM si argumenta NIHIL, manifestum deest aut memoria
 * deficit
 */
b32
scaena_latus_montare (
    ScaenaLatusMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata);

/* piscinas proprias lateris destruere */
vacuum
scaena_latus_dimittere (
    ScaenaLatusMontatio* m);

Componens*
scaena_latus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* pulsus: volumen inspicit; VERUM si index aut prospectus mutatus */
b32
scaena_latus_pulsare (
    vacuum* ctx);

/* mora usque ad pulsum proximum (ms): inspectio si volumen adest;
 * -1 = nullus */
Mora
scaena_latus_mora (
    vacuum* ctx);

constans Imago*
scaena_latus_imago_fons (
    chorda  provenientia,
    vacuum* ctx);

/* titulus loci humanus: titulus prospectus ostensi */
chorda
scaena_latus_titulus (
     vacuum* ctx,
    Piscina* piscina);

/*
 * scaena_latus_montare_hospitem - scaenam in ramo hospitis parare
 *
 * @ramus:          ramus hospitis (campi 'prospectus', 'tactum')
 * @scriptor:       actio hospitis quae eos scribit ("mundus.agere");
 *                  litterae diutius vivant quam montatio
 * @quadri_titulus: provenientia picturae, hospiti propria
 * @monitum:        linea sub "cannot be shown"; NIHIL = nulla
 *
 * Redde: FALSUM si argumenta NIHIL aut memoria deficit
 */
b32
scaena_latus_montare_hospitem (
    ScaenaLatusMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
            InsulaRamus  ramus,
     constans character* scriptor,
     constans character* quadri_titulus,
     constans character* monitum,
                    i32  latitudo,
                    i32  altitudo);

/* eventum hospitis tractare (mus in area "pictura", claves) */
ScaenaLatusEventum
scaena_latus_agere_hospitis (
    ScaenaLatusMontatio* m,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev);

/* latus totum pingere: fundus, caput, pictura, textus aut nuntius */
vacuum
scaena_latus_pingere_hospitis (
    ScaenaLatusMontatio* m,
     constans Componens* c,
                Mandata* md);

/* areas ictus scaenae radici hospitis addere, actione hospitis */
vacuum
scaena_latus_areas_addere (
    ScaenaLatusMontatio* m,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* actio);

/* VERUM si prospectus status lectus est et ostenditur */
b32
scaena_latus_valet (
    constans ScaenaLatusMontatio* m);

#endif /* SCAENA_LATUS_H */
