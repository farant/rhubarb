/* mundus_latus.h - latus mundi: persona per locum ambulat, in latere
 * vici (project-specs/mundus-spec.md IV.8)
 *
 * PICTURA latus implet sub ordine capitis: pars loci desuper oblique
 * visa (visus fixus, mundus_pictura.h), persona in medio. CAPUT:
 * sagittae et numerus (prior 2/5 proxima) et titulus personae.
 *
 * LOCUS et PERSONAE ex volumine (statuae_repositorium.h): locus est
 * inscriptio prima generis LOCUS; personae sunt inscriptiones generis
 * STATUA, ordine additionis. Latus volumen_seq_ultimum semel per
 * secundam inspicit. Persona initio: titulus status; aliter prima
 * cuius titulus "mushroom" continet; aliter prima. Locus initii:
 * cella gressibilis medio loci proxima (mundus_solum_initium).
 *
 * HUMUS: dithering Atkinson (ordinarium) aut exemplaria; 'g' aut
 * globulus in fine capitis commutat. Regiones humi servatae tunc
 * abiciuntur et modo novo denuo fiunt cum visus eas attingit; persona
 * ubi stat manet.
 *
 * CLAVES: W A S D tentae ambulant, ad SCHIRMUM relatae (W sursum in
 * pictura); duae simul oblique; contrariae se tollunt. [ ] persona
 * prior / proxima (persona nova ubi vetus stabat stat). Claves tentae
 * purgantur cum latus focum amittit (mundus_latus_focus_amissus;
 * VicusFacies.focus_amissus) - liberatio clavis tunc fortasse numquam
 * venit.
 *
 * TEMPUS ex horologio (porta): mundus tempore vero promovetur, gradus
 * ad 0.1 s coactus. Pulsus: XXXIII ms dum clavis tenetur aut persona
 * movetur; aliter M ms (inspectio voluminis sola).
 *
 * OSTIA (project-specs/ostium-spec.md): ostia loci ex volumine
 * leguntur (mundus_ostia.h) cum indicibus - quotiens seq voluminis
 * mutatur, etiam si locus ipse idem manet. Ostium PROPINQUUM est
 * cuius punctum a centro personae non plus radio suo distat (in
 * plano x z); ex pluribus proximum, ex paribus prius in indice.
 * Kineticum: quoque pulsu post gradum mundi computatur.
 *
 * SIGNUM: dum ostium propinquum est, "[E]" in capsa parva pingitur
 * (fundus thematis, limbus et litterae coloris textus), medio
 * horizontali supra punctum ostii: punctum in altitudine soli sui
 * (si ibi solum gressibile non est - ostium in pariete - in
 * altitudine personae) plus MUNDUS_LATUS_SIGNI_ALTITUDO, visu mundi
 * proiectum; capsa ima in puncto proiecto stat. In mundo fixum manet
 * dum persona movetur. Capsa quae extra picturam caderet ad marginem
 * picturae cogitur: ostium propinquum signum SEMPER visibile habet.
 * Area ictus "ostium" capsam tegit.
 *
 * Signum nullum: locus aut persona deest, ostia nulla, nullum
 * propinquum.
 *
 * INTRARE: E dum ostium propinquum est, aut ictus sinister in
 * signo: latus scaenam ostii ostendit. Scribuntur 'prospectus'
 * (titulus scaenae ostii), 'tactum' (vacuum), 'ostium' (punctum et
 * radius ostii). Claves tentae purgantur. E sine ostio propinquo:
 * nihil.
 *
 * INTUS (status 'prospectus' non vacuus): latus est scaena hospitis
 * (scaena_latus.h HOSPES) - caput, pictura, descriptio, res, mus,
 * j k Enter. W A S D, 'g', [ ] et sagittae nihil agunt; mundus non
 * promovetur; signum non pingitur. Titulus loci: titulus prospectus.
 * Locus iterum conditus dum intus: latus intus manet.
 *
 * SCAENA QUAE OSTENDI NON POTEST (deest, remota dum intus, massa
 * recusata): nuntius scaenae cum monito "E: go back". E aut ictus
 * sinister ubivis in latere exit.
 *
 * EXIRE: exitus "mundus" scaenae (aut E ut supra). 'prospectus',
 * 'tactum', 'ostium' vacantur; persona in cella gressibili puncto
 * 'ostium' proxima stat, intra radium eius; si nulla talis (locus
 * mutatus) aut 'ostium' legi non potest, in loco initii. Persona
 * quiescit: clavis ante introitum tenta post exitum non ambulat.
 *
 * RESTITUTIO: latus montatum cum 'prospectus' non vacuo intus
 * incipit; exitus ut supra.
 *
 * NUNTII loco picturae: locus nullus in volumine; locus quem lector
 * recusat; locus sine cella gressibili; personae nullae; persona
 * recusata (sagittae tamen valent).
 *
 * STATUS in ramo (manifestum apps/mundus/organum.stml, persistens):
 *   persona     titulus personae quae ambulat
 *   humus       dithering humi
 *   prospectus  scaena ostensa; vacuum = foris (in mundo)
 *   tactum      ut in scaena_latus.h
 *   ostium      "x z radius" in millesimis: ostium per quod
 *               intratum est
 * Locus personae in mundo, claves tentae et quadrum
 * KINETICA sunt: in montatione, numquam in statu (latus clausum et
 * iterum apertum ab initio incipit).
 */
#ifndef MUNDUS_LATUS_H
#define MUNDUS_LATUS_H

/* <aedilis corpus="lib/mundus_latus.c"/> */

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
#include "statua.h"
#include "mundus_solum.h"
#include "mundus.h"
#include "mundus_pictura.h"
#include "mundus_ostia.h"
#include "scaena_latus.h"

/* pulsus (ms): dum movetur; inspectio voluminis */
#define MUNDUS_LATUS_MORA_MOTUS        XXXIII
#define MUNDUS_LATUS_MORA_INSPECTIONIS M

#define MUNDUS_LATUS_SIGNUM           "[E]"
/* altitudo signi supra solum ostii (persona II alta est) */
#define MUNDUS_LATUS_SIGNI_ALTITUDO   2.5f
/* limbus capsae circa litteras (pixela) */
#define MUNDUS_LATUS_SIGNI_LIMBUS     III
/* sub nuntio scaenae quae ostendi non potest */
#define MUNDUS_LATUS_MONITUM          "E: go back"

/* claves ambulandi: index in MundusLatusMontatio.tentae */
#define MUNDUS_LATUS_SURSUM    ZEPHYRUM   /* w */
#define MUNDUS_LATUS_SINISTRUM I          /* a */
#define MUNDUS_LATUS_DEORSUM   II         /* s */
#define MUNDUS_LATUS_DEXTRUM   III        /* d */

/* horologium (ms); probationes fictum dant */
nomen structura {
       s64  (*nunc)(vacuum* ctx);
    vacuum* ctx;
} MundusLatusPorta;

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Volumen* volumen;      /* NIHIL = nihil ostenditur */
        MundusLatusPorta  porta;
                     i32  latitudo;     /* superficies montationis */
                     i32  altitudo;
                     b32  activi[XVI];  /* colores palettae */
    /* indices (relecti cum seq voluminis mutatur) */
                 Piscina* piscina_indicis;
                     Xar* personae;     /* de StatuaeInscriptio */
                     Xar* loci;         /* de StatuaeInscriptio */
                     Xar* ostia;        /* de MundusOstium; in
                                         * piscina_indicis; NIHIL =
                                         * nulla */
                     s32 ostium_propinquum;  /* index in ostia; -1 =
                                               * nullum (kineticum) */
                     s64 seq_visum;
                     s64 inspectio_ms;
                     b32 inspectum;
    /* locus lectus (statua, tabula gressus) */
                 Piscina* piscina_loci;
                  Statua* locus;        /* NIHIL: nullus aut
                                         * recusatus */
             MundusSolum* solum;
    /* humus loci cum regionibus suis, in piscina propria (modus
     * mutatus: piscina vacatur, humus nova) */
                 Piscina* piscina_humi;
             MundusHumus* humus;        /* NIHIL dum locus nullus */
         MundusHumiModus  humi_modus;   /* modus humi factae */
                  chorda  sigillum_loci;
                  chorda  causa_loci;   /* recusatio aut "nowhere to
                                         * walk"; vacua si bonus */
    /* persona lecta */
                 Piscina* piscina_personae;
                  Statua* persona;      /* NIHIL: nulla aut recusata */
                  chorda  sigillum_personae;
                  chorda  causa_personae;
             MundusForma  forma;
    /* mundus (kineticus) */
                  Mundus mundus;
            MundusCorpus corpus;
                     b32 positum;      /* corpus in loco stat */
                     b32 tentae[IV];   /* claves ambulandi tentae */
                     s64 hora_visa;    /* horologium pulsus ultimi */
                     s64 hora_gradus;  /* horologium gradus mundi
                                         * ultimi */
    /* quadrum ultimum et clavis eius */
                 Piscina* piscina_quadri;
                   Imago  quadrum;      /* pixela NIHIL = nullum */
                  chorda  quadri_titulus;  /* "mundus:<id rami>" */
               character  quadri_clavis[CLX];
    /* scaena hospitata (ostium-spec 12) */
     ScaenaLatusMontatio scaena;       /* valet dum scaena_montata */
                     b32 scaena_montata;
                  chorda scaenae_titulus;  /* "mundus-scaena:<id
                                             * rami>" */
} MundusLatusMontatio;

/*
 * mundus_latus_montare - latus parare
 *
 * @volumen: NIHIL licet (nuntius "no place")
 * @id:      id rami; NIHIL = radix repositorii
 * @porta:   horologium; NIHIL = horologium nullum (tempus 0)
 *
 * Redde: FALSUM si argumenta NIHIL, manifestum deest aut memoria
 * deficit
 */
b32
mundus_latus_montare (
              MundusLatusMontatio* m,
                          Piscina* piscina,
              InternamentumChorda* intern,
                          Volumen* volumen,
               InsulaRepositorium* repo,
               constans character* id,
        constans MundusLatusPorta* porta,
                              i32  latitudo,
                              i32  altitudo,
                  StatusSystemata* systemata);

/* piscinas proprias lateris destruere */
vacuum
mundus_latus_dimittere (
    MundusLatusMontatio* m);

Componens*
mundus_latus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* pulsus: volumen inspicit, mundum promovet, quadrum reficit. VERUM
 * si quid mutatum */
b32
mundus_latus_pulsare (
    vacuum* ctx);

/* mora usque ad pulsum proximum necessarium (ms); -1 = nullus */
Mora
mundus_latus_mora (
    vacuum* ctx);

constans Imago*
mundus_latus_imago_fons (
    chorda  provenientia,
    vacuum* ctx);

/* titulus loci humanus: titulus personae */
chorda
mundus_latus_titulus (
     vacuum* ctx,
    Piscina* piscina);

/*
 * mundus_latus_signum - ubi signum ostii propinqui pingitur
 *
 * @fines: capsa in spatio radicis lateris
 *
 * Redde: FALSUM (fines non scripti) si nullum ostium propinquum est
 * aut quadrum deest
 */
b32
mundus_latus_signum (
    constans MundusLatusMontatio* m,
                           Fines* fines);

/* VERUM si latus scaenam ostendit (status 'prospectus' non vacuus) */
b32
mundus_latus_intus (
    constans MundusLatusMontatio* m);

/* latus focum amisit: claves tentae purgantur, persona sistit */
vacuum
mundus_latus_focus_amissus (
    vacuum* ctx);

#endif /* MUNDUS_LATUS_H */
