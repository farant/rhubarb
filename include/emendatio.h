/* emendatio.h - Carmen per locum emendatum: (pars, mensura, tempus)
 * -> nota, super librum STML
 *
 * TEXTUS MANET VERITAS: frusta MML ut auctor scripsit (l8, octavae,
 * lineae '|'). Emendatio CHIRURGICA est - signum notae solum in textu
 * frusti mutatur (nodus textus in arbore STML; scriptor non-pulcher
 * reliqua octetim reddit, sonda 10-23) - deinde liber TOTUS per
 * carmina_legere iudicatur: numquam liber semi-validus.
 *
 * LOCUS "M:T" aut "M:T+P": mensura (1 = prima), tempus (1 = primum;
 * unitas = 1/denominator, 6/8 9/8 12/8: tres octavae - ut linea
 * harmoniae), P pulsus additi intra tempus (0 ordinarium).
 *
 * NOTA NOVA "G2", "Bb3", "F#4" (partitura_notam_legere - grammatica
 * partiturae inversa; C4 = MIDI LX). Octava aliena: signum '<' / '>'
 * circumdatur ("d2." -> "<g2.>") - notae sequentes octavam suam
 * servant.
 *
 * MENSURA TOTA (emendatio_mensuram_substituere): sectio inter lineas
 * '|' scriptas textu MML novo substituitur. Textus novus in statu
 * lectoris ad initium mensurae legitur (cantus_status_ad: octava, 'l',
 * 'v', '@'); post eum status finis veteris restituitur ("| o2 b-2. |"
 * + "o3 d2." -> "| o3 d2. o2 |") - mensurae sequentes sonant ut prius.
 *
 * NONDUM: notae motae in tempore (scriptor exemplar -> MML; pagina
 * DAW).
 */
/* <aedilis corpus="lib/emendatio.c"/> */
#ifndef EMENDATIO_H
#define EMENDATIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "carmen.h"
#include "cantus.h"
#include "chorda_aedificator.h"

nomen structura {
    i32 mensura;   /* 1.. */
    i32 tempus;    /* 1.. */
    s64 pulsus;    /* additi intra tempus */
} EmendatioLocus;

/* quod mutatum est (ad monstrandum): textus frusti ante et post */
nomen structura {
       s32 frustum;   /* index in parte */
       i32 offset;    /* in textu frusti VETERE */
    chorda vetus;     /* "d2." */
    chorda novum;     /* "<g2.>" */
} EmendatioMutatio;

/* "3:2", "3:2+480". FALSUM: forma mala */
b32
emendatio_locum_legere (
            chorda  textus,
    EmendatioLocus* locus);

/* nota quae ad locum in parte INCIPIT altitudinem novam accipit,
 * longitudo manet. FALSUM (vitium impletum, *novus intactus): carmen /
 * pars ignota, locus extra metrum, nulla nota ibi incipit (causa
 * nominat notam quae ibi sonat, aut pausam), sonitus '@', nota nova
 * mala aut extra MIDI (MML: octava 0..8, alteratio una), textus non
 * crudus ('<textus!>' - MML '<' fert), liber novus iudicio cadit */
b32
emendatio_notam_ponere (
                     chorda  liber,
                     chorda  carmen,
                     chorda  pars,
    constans EmendatioLocus* locus,
                     chorda  nota,
                    Piscina* piscina,
                     chorda* novus,
           EmendatioMutatio* mutatio,
             CarminisVitium* vitium);

/* mensura partis in textu frusti: sectio inter lineas '|' scriptas
 * (aut initium / finem frusti), spatiis orae exclusis, cum statu
 * lectoris ad initium - textus novus in hoc statu legitur */
nomen structura {
             s32 frustum;    /* index in parte */
             i32 initium;    /* [initium, finis) in textu frusti */
             i32 finis;
          chorda textus;     /* sectio: " o2 b-2. " -> "o2 b-2." */
    CantusStatus status;     /* ad initium */
} EmendatioMensura;

/* FALSUM: pars ignota, mensura non intra frustum unum (frusta absunt
 * aut frustum intra mensuram incipit), linea '|' ad initium / finem
 * non scripta */
b32
emendatio_mensuram_invenire (
     constans Carmen* carmen,
              chorda  pars,
                 i32  mensura,
    EmendatioMensura* inventa,
      CarminisVitium* vitium);

/* mensuram textu MML novo substituit. Textus novus in statu initii
 * legitur; post eum status finis veteris restituitur ('o' 'l' 'v'
 * scripta solum si differunt; nihil si frustum ibi desinit). Textus
 * novus sine '|', longitudo eius = mensura. FALSUM (vitium impletum,
 * *novus intactus): vitia invenire, '|' in textu novo, longitudo
 * diversa, '@' novum ubi finis vetus nullum habet (MML '@' revocare
 * nequit), liber novus iudicio cadit */
b32
emendatio_mensuram_substituere (
               chorda  liber,
               chorda  carmen,
               chorda  pars,
                  i32  mensura,
               chorda  textus,
              Piscina* piscina,
               chorda* novus,
     EmendatioMutatio* mutatio,
       CarminisVitium* vitium);

/* COPIA (emendatio_mensuras_copiare): mensurae M..N partis fontis in
 * mensuras K.. partis metae (eadem aut alia) scribuntur, quaeque per
 * emendatio_mensuram_substituere (lineae scriptae, status restitutus) -
 * SUPERSCRIBIT, numquam inserit (notae sequentes non moventur). Textus
 * fontis ex libro ORIGINALI (meta fontem tegens fontem non mutat).
 *
 * TRANSPOSITIO: diatonice - gradus notae in clave fontis computatur,
 * gradu promotus in clave metae redditur, alteratio ad signaturam
 * servata (C D E +2 in c maior -> E F G; C# +1 -> D#; ex G maior in c
 * maior: F# -> B, gradus VII manet VII; in minore VII elevatus
 * alteratio est: G# +1 -> A#); aut semitonia (exacte, orthographia ex
 * clave metae). REPETITIO (sequentia): repetitio r (1..R) in mensura
 * K + (r - 1) * longitudo, intervallo r-plo (1-2 ad 3, -1, R III:
 * 3-4 gradu infra, 5-6 duobus, 7-8 tribus).
 *
 * SCRIPTOR (emendatio_mensuram_scribere): notae mensurae ut MML eodem
 * rhythmo - longitudo expressa in nota quaque, pausae inter notas
 * (valores maximi primum), 'oN' initio et ubi octava mutatur, 'vN' ubi
 * lucrum mutatur (initio si non 1). Recusat: '@' (sonitus), nota trans
 * lineam mensurae, octava extra MML 0..8, alteratio duplex. */

nomen enumeratio {
    EMENDATIO_DIATONICE,
    EMENDATIO_SEMITONIA
} EmendatioTranspositio;

nomen structura {
                   chorda pars_fons;
                      i32 prima;          /* mensurae fontis */
                      i32 ultima;
                   chorda pars_meta;      /* vacua = pars fontis */
                      i32 meta;           /* mensura metae prima */
    EmendatioTranspositio modus;
                      s32 intervallum;    /* gradus aut semitonia */
                      i32 repetitiones;   /* 1 = copia simplex */
} EmendatioCopia;

/* MML mensurae unius partis (sine '|'), transpositione mutatae;
 * clavis metae ad pulsum 'pulsus_metae'. FALSUM: vitium impletum */
b32
emendatio_mensuram_scribere (
          constans Carmen* carmen,
                   chorda  pars,
                      i32  mensura,
    EmendatioTranspositio  modus,
                      s32  intervallum,
                      s64  pulsus_metae,
        ChordaAedificator* textus,
           CarminisVitium* vitium);

/* copia (et repetitiones) in librum novum; *mutatae = mensurae metae
 * scriptae. FALSUM (vitium impletum, *novus intactus): vitia
 * scriptoris aut substitutionis (meta sine lineis, extra frusta,
 * longitudo, meta in statu '@'), mensurae fontis extra carmen */
b32
emendatio_mensuras_copiare (
                     chorda  liber,
                     chorda  carmen,
    constans EmendatioCopia* copia,
                    Piscina* piscina,
                     chorda* novus,
                        i32* mutatae,
             CarminisVitium* vitium);

#endif /* EMENDATIO_H */
