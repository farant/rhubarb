/* statuae.h - latus statuarum: simulacra GLB animata in latere vici,
 * dithering per quadrum (project-specs/statuae-spec.md)
 *
 * PICTURA latus implet sub ordine capitis. CAPUT: sagittae et numerus
 * (prior 1/2 proxima), titulus statuae, delectus cycli, delectus modi
 * dithering, commutatores 'lit' / 'flat' et '2x'. Si caput in ordine
 * uno non capit, delectus et commutatores in ordinem secundum eunt.
 *
 * STATUAE ex volumine (statuae_repositorium.h): index ordine
 * additionis; statua visa semel legitur (statua_glb.h) cum fit visa.
 * Latus volumen_seq_ultimum semel per secundam inspicit - statua ab
 * alio processu addita (bin/statuae) sine restitutione apparet.
 * Statua quam lector recusat titulum et causam ostendit; sagittae
 * tamen valent.
 *
 * TEMPUS cycli ex horologio (porta), non ex numero quadrorum:
 * dithering tardum quadra omittit, cyclum non retardat. Quadrum
 * novum fit solum cum quid mutatum est (tempus, visus, statua,
 * cyclus, modus, mensura). Pulsus: XXXIII ms dum cyclus currit aut
 * statua trahitur; aliter M ms (inspectio voluminis sola).
 *
 * GESTUS: tractus in pictura gyrat (dextrorsum / sinistrorsum sine
 * fine) et inclinat (ad +-LXXXV gradus); dimidius gradus per pixelum.
 * Ictus primus (latus nondum focatum) iam trahit.
 *
 * CLAVES: h l gyrus, j k inclinatio, r visus ordinarius, [ ] statua
 * prior / proxima, c delectus cycli, d delectus modi, f lit / flat,
 * 2 dimidia mensura, Spatium quies / cursus. Delectus apertus claves
 * suas sumit (j k Reditus Esc).
 *
 * STATUS in ramo (manifestum apps/statuae/organum.stml, persistens):
 * statua, cyclus, modus, planum, duplex, quiescit, gyrus,
 * inclinatio. Positio in cyclo, tractus et quadrum KINETICA sunt: in
 * montatione, numquam in statu.
 */
#ifndef STATUAE_H
#define STATUAE_H

/* <aedilis corpus="lib/statuae.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "elenchus.h"
#include "fasti.h"
#include "imago_typus.h"
#include "volumen.h"
#include "organum_montatio.h"
#include "statua.h"

/* pulsus (ms): dum movetur; inspectio voluminis */
#define STATUAE_MORA_MOTUS        XXXIII
#define STATUAE_MORA_INSPECTIONIS M

/* tractus: decimae gradus per pixelum (V = dimidius gradus) */
#define STATUAE_TRACTUS_DECIMAE   V
/* claves h l j k: decimae gradus per ictum (CL = XV gradus) */
#define STATUAE_GRADUS_CLAVIS     CL

/* optiones delectus plurimae (cycli ultra hunc numerum non
 * ostenduntur) et longitudo textus optionis */
#define STATUAE_OPTIONES_MAXIMAE  XVI
#define STATUAE_OPTIONIS_TEXTUS   LXIV

/* horologium monotonicum (ms); probationes suum praebent. nunc NIHIL:
 * tempus stat (cyclus in initio manet) */
nomen structura {
       s64  (*nunc)(vacuum* ctx);
    vacuum* ctx;
} StatuaePorta;

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Volumen* volumen;      /* NIHIL = nullae statuae */
            StatuaePorta  porta;
                     i32  latitudo;     /* superficies montationis */
                     i32  altitudo;
    /* index statuarum (relectus cum seq voluminis mutatur) */
                 Piscina* piscina_indicis;
                     Xar* index;        /* de StatuaeInscriptio */
                     s64  seq_visum;
                     s64  inspectio_ms; /* horologium inspectionis
                                         * ultimae */
                     b32 inspectum;    /* semel saltem */
    /* statua visa (lecta semel) */
                 Piscina* piscina_statuae;
                  Statua* statua;       /* NIHIL: nulla aut recusata */
                  chorda  sigillum;     /* massa statuae lectae */
                  chorda  causa;        /* recusatio lectoris; vacua
                                         * si statua lecta */
    /* tempus cycli (kineticum) */
                     s64 origo_ms;     /* horologium ad tempus cycli
                                         * 0 */
                     s32 quietum_ms;   /* tempus cycli dum quiescit */
    /* tractus (kineticus) */
                     b32 trahitur;
                     s32 tractus_x;    /* punctum prius */
                     s32 tractus_y;
                     s32 gyrus;        /* decimae gradus, dum
                                         * trahitur; aliter status */
                     s32 inclinatio;
    /* quadrum ultimum et clavis eius */
                 Piscina* piscina_quadri;
                   Imago  quadrum;      /* pixela NIHIL = nullum */
                  chorda  quadri_titulus;  /* "statuae:<id rami>" */
               character  quadri_clavis[CXXVIII];
    /* optiones delectus aperti (vivae dum menu pingitur) */
                Elenchus menu;
             ElenchusRes menu_res[STATUAE_OPTIONES_MAXIMAE];
               character menu_textus[STATUAE_OPTIONES_MAXIMAE]
                              [STATUAE_OPTIONIS_TEXTUS];
} StatuaeMontatio;

/* montare in ramo <statuae id> (manifestum infixum). volumen NIHIL
 * licet (latus vacuum); porta NIHIL = tempus stat; systemata NIHIL
 * licet */
b32
statuae_montare (
          StatuaeMontatio* m,
                  Piscina* piscina,
      InternamentumChorda* intern,
                  Volumen* volumen,
       InsulaRepositorium* repo,
       constans character* id,
    constans StatuaePorta* porta,
                      i32  latitudo,
                      i32  altitudo,
          StatusSystemata* systemata);

/* piscinas montationis (index, statua, quadrum) reddere */
vacuum
statuae_dimittere (
    StatuaeMontatio* m);

/* compositor (dispensator; facies vici); ctx = StatuaeMontatio* */
Componens*
statuae_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* pulsus: volumen inspicit (semel per secundam), statuam visam legit
 * si mutata, quadrum reficit si clavis eius mutata. VERUM = repinge */
b32
statuae_pulsare (
    vacuum* ctx);

/* ms usque ad pulsum proximum (vide STATUAE_MORA_*) */
Mora
statuae_mora (
    vacuum* ctx);

/* fons imaginum (facies vici): quadrum sub titulo suo; NIHIL si
 * titulus alius aut nullum quadrum */
constans Imago*
statuae_imago_fons (
    chorda  provenientia,
    vacuum* ctx);

/* ictus primus (latus nondum focatum): si in pictura, tractus statim
 * incipit. VERUM = actum */
b32
statuae_ictus_primus (
                vacuum* ctx,
    InsulaRepositorium* repo,
                 Motus* motus,
             Componens* arbor,
      constans Eventus* ev);

/* titulus loci: titulus statuae visae; vacuus si nulla */
chorda
statuae_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* STATUAE_H */
