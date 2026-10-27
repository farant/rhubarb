/* comitatus.h - Figurae comitandi: acorda et figura -> notae (purum)
 *
 * Inversum explicationis (desideratum ...Q7G5): progressio acordorum
 * scripta et figura (oom-pah-pah, Alberti, harpa fracta) notas partis
 * comitantis generant, cum ductu vocum. Purus - nec Carmen nec liber:
 * carmen.c (<acorda>, <figura>, <comitatus>) initia mensurarum ex metro
 * dat et notas in frustum generatum vertit.
 *
 * ACORDA (comitatus_acorda_legere):
 *   acorda  := mensura { '|' mensura }
 *   mensura := { acordum [ ':' valor ] }
 *   acordum := nomen (acordum.h) | 'r' (nullum: comitatus tacet)
 * Acorda sine valore residuum mensurae aequaliter dividunt (pulsibus
 * integris, residuum ultimo); valores residuum excedentes, mensura
 * vacua, residuum sine acordo: vitia. "D:2 G:2" (linea harmoniae)
 * idem legitur.
 *
 * FIGURA (comitatus_figuram_legere):
 *   figura  := mensura { '|' mensura }
 *   gradus  := vox [':' valor] | '[' vox {vox} ']' [':' valor]
 *            | '{' vox {vox} '}' [':' valor] | 'r' [':' valor] | 'v' N
 *   vox     := 'B'  bassus acordi (post '/' si scriptus)
 *            | 'Q'  quinta acordi (dim, aug: sua; sine quinta: radix +
 *                   VII), proxima basso, in ambitu bassi
 *            | 1..9 vox superior ab ima; ultra numerum classium acordi
 *                   octava supra (in triade 4 = 1 octava supra)
 *   valor   := divisor integrae ut MML (1 2 4 8 16 32 64; 3 6 12 24 48;
 *              5 10 20 40; 15 30 60) { '.' } (ad II); sine valore prior
 *              (initio 4)
 *   [..] simul; {..} fractum: voces ordine scripto intrant, sexagesima
 *   quarta inter singulas, omnes ad finem gradus desinunt (harpa)
 *   'v N' lucrum 0..100 graduum sequentium; mensura figurae quaeque a
 *   100 incipit. Spatia ignorantur.
 *
 * COMPOSITIO (comitatus_componere):
 *   MENSURA FIGURAE: prima ubi acordum ad initium mensurae carminis
 *   differt ab acordo ad initium mensurae PRIORIS (aut primo) -
 *   acordum intra mensuram mutatum et tentum radicem in mensura
 *   proxima accipit (C:2 G:2 | G: G in basso mensurae II); aliter
 *   sequens (circulo): radix in acordo novo, 'Q' in tento. Mensura
 *   figurae longitudinem mensurae carminis aequat (vitium aliter,
 *   offset in figura).
 *   ACORDUM GRADUS = acordum ad initium gradus; nullum: gradus tacet.
 *   BASSUS: classis bassi in ambitu bassi, proxima basso priori (primo:
 *   medio ambitus); aequales: inferior. 'Q' proxima basso currenti in
 *   ambitu, ab eo diversa (si nulla diversa: bassus ipse).
 *   SUPERIORES: positio clausa - classes acordi ascendentes ab
 *   inversione aliqua, intra octavam; figura voces pauciores quam
 *   classes adhibens (V < k) quintam acordi omittit (G7 tribus
 *   vocibus: G B F, sensibilis manet) - quintam ALTERATAM (o, +, h7,
 *   o7, 7#5) numquam: ea acordum facit; deinde radicem si adhuc
 *   pauciores, SOLUM si bassus eam sonat: radix in basso acordi (non
 *   C7/E, F6/A) et vox 'B' in figura aut bassus_sonat (comitatus alius
 *   carminis, lilia: harpa_bassus) - C7 duabus E Bb, C9 tribus E Bb
 *   D. Radix non omissa inter voces adhibitas semper stat. Acorda cum
 *   tono addito sine septima (6, m6, add9) et acorda quinta alterata
 *   ordine converso: radix prius (eadem condicione), deinde quinta -
 *   quinta ea a cognatis distinguit (F6 tribus A C D, non A D F =
 *   Dm/F; Bbadd9 tribus C D F; Caug duabus E G#). Inter inversiones
 *   et octavas quarum voces adhibitae (1..min(V, k); V vox maxima
 *   figurae, k classes positionis) intra ambitum stant, motus minimus
 *   (summa
 *   |a - b| vocum adhibitarum) a positione priore; primo: medium
 *   vocum medio ambitus proximum; aequales: inferior. Ambitus
 *   superiorum et bassi >= XII (octava): positio semper invenitur.
 *   Voces > k octavis supra stant (ambitum excedere licet). Positio
 *   solum cum acordo mutatur.
 *   LITTERAE ex acordo (Acordum.litterae).
 */
/* <aedilis corpus="lib/comitatus.c"/> */
#ifndef COMITATUS_H
#define COMITATUS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "acordum.h"

/* acordum scriptum in tempore (pulsus ut vocans dat) */
nomen structura {
        s64 pulsus;
        s64 longitudo;
    Acordum acordum;
        b32 nullum;     /* 'r' */
        i32 offset;     /* in textu acordorum */
} ComitatusAcordum;

nomen structura {
    s32 voces[IX];      /* -1 'B', -2 'Q', 1..9 superiores */
    i32 numerus_vocum;  /* 0 = pausa */
    s64 pulsus;         /* intra mensuram figurae */
    s64 longitudo;
    b32 fractum;        /* {..} */
    f32 lucrum;         /* 0..1 */
    i32 offset;         /* in textu figurae */
} ComitatusGradus;

nomen structura {
    ComitatusGradus* gradus;            /* ordine */
                i32  numerus;
                s64* longitudines;      /* per mensuram figurae */
                i32* primi;             /* gradus primus per mensuram */
                i32  numerus_mensurarum;
                i32  vox_maxima;        /* V (0 si superiores nullae) */
                b32  bassus;            /* vox 'B' in figura */
} ComitatusFigura;

nomen structura {
    i32 infima;          /* superiores, MIDI */
    i32 summa;
    i32 bassi_infima;
    i32 bassi_summa;
} ComitatusAmbitus;

nomen structura {
          s64 pulsus;
          s64 longitudo;
          i32 nota;       /* MIDI */
          f32 lucrum;
    character littera;    /* 'a'..'g' */
          i32 offset;     /* gradus, in textu figurae */
} ComitatusNota;

nomen structura {
                    i32  offset;   /* in textu lecto */
     constans character* causa;    /* Latina, statica */
} ComitatusVitium;

/* numerus mensurarum textus acordorum ('|' + 1, ultimo vacuo omisso):
 * vocans initia tot mensurarum parat */
i32
comitatus_mensurae (
    chorda textus);

/* acorda in mensuras: initia[0..numerus] (numerus + 1: finis ultimae)
 * pulsus dant. FALSUM: vitium */
b32
comitatus_acorda_legere (
                chorda   textus,
          constans s64*  initia,
                   i32   numerus,
               Piscina*  piscina,
      ComitatusAcordum** acorda,
                   i32*  numerus_acordorum,
       ComitatusVitium*  vitium);

b32
comitatus_figuram_legere (
              chorda  textus,
             Piscina* piscina,
     ComitatusFigura* figura,
     ComitatusVitium* vitium);

/* notae spatii [ab, ad): acorda (ordine, non se tegentia; lacunae =
 * nullum), initia mensurarum carminis spatium tegentia (numerus + 1).
 * bassus_sonat: bassus in parte alia sonat (radicem omittere licet
 * etiam figura sine 'B'). FALSUM: mensura figurae non congruit,
 * ambitus < XII, memoria */
b32
comitatus_componere (
      constans ComitatusFigura*  figura,
     constans ComitatusAcordum*  acorda,
                           i32   numerus_acordorum,
                  constans s64*  initia,
                           i32   numerus_mensurarum,
     constans ComitatusAmbitus*  ambitus,
                           b32   bassus_sonat,
                       Piscina*  piscina,
                 ComitatusNota** notae,
                           i32*  numerus,
               ComitatusVitium*  vitium);

#endif /* COMITATUS_H */
