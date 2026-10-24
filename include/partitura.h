/* partitura.h - Carmen ut textus: mensura per mensuram, nota per notam
 *
 * Lectori et AGENTI (qui non audit): carmen (carmen.h) in textum
 * redditur ut comparari, quaeri, iudicari possit. Pagina notationis
 * (postea) eadem orthographia utitur.
 *
 * FORMA:
 *   carmen korobeiniki: a minor, 4/4, 144
 *     pars melodia: quadrans, lucrum 0.70
 *     pars bassus: triangulum, lucrum 0.80 [mutus]
 *   mensura 1
 *     melodia  E5:4 B4:8 C5:8 D5:4 C5:8 B4:8
 *     bassus   E2:8 E3:8 E2:8 E3:8 E2:8 E3:8 E2:8 E3:8
 *   mensura 9  metrum 3/4  clavis c maior  tempus 160
 *
 *   nota       littera, alteratio (# b ## bb), octava (C4 = MIDI LX)
 *   :valor     1 2 4 8 16 32 64; '.' punctata (x1.5); 't' trioli
 *              (x2/3); aliter ~pulsus (exempli gratia C4:~768)
 *   r:valor    pausa (spatium inter notas partis), valoribus maximis
 *              primum (r:2. r:8)
 *   [C4 E4]:4  notae simul: eodem pulsu et longitudine, gradu ordinatae
 *   E5:2~      nota trans lineam mensurae; mensura proxima incipit
 *              '~:4' (residuum)
 *   @punctum:8 percussio (praeparatum notae)
 *   mutationes metri, clavis (ad initium mensurae) et temporis (intra
 *   mensuram: 'tempus 90 (+960)' pulsibus ab initio) in titulo mensurae
 *
 * ORTHOGRAPHIA: littera notae (CarmenNota.littera, ab auctore - MML eam
 * semper scribit) vincit; aliter ex clave currente: gradus scalae
 * clavis; extra scalam littera naturalis si est, aliter '#' in clavibus
 * sine b, 'b' in clavibus cum b (A minor: G#).
 *
 * Lucra notarum (MML 'v') nondum monstrantur.
 *
 * USUS:
 *   PartituraForma f;
 *   memset(&f, 0, magnitudo(f));  f.prima = IX;  f.ultima = XII;
 *   f.additae_titulus = "harmonia";              (* linea addita, *)
 *   f.addita = harmonia_lineam_scribere;         (* harmonia.h *)
 *   si (partitura_scribere(carmen, &f, aedificator)) ...
 */
/* <aedilis corpus="lib/partitura.c"/> */
#ifndef PARTITURA_H
#define PARTITURA_H

#include "latina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "carmen.h"

nomen structura {
    character littera;     /* 'A'..'G' */
          s32 alteratio;   /* -2..2 (bb .. ##) */
          s32 octava;      /* C4 = MIDI LX; littera octavam dat (Cb4 =
                            * MIDI LIX) */
} PartituraNota;

nomen structura {
       i32 prima;    /* mensura prima (1 = prima carminis; 0 = 1) */
       i32 ultima;   /* 0 = ultima carminis */
    chorda pars;     /* vacua = partes omnes */
    /* linea addita per mensuram, post partes (analysis: harmonia.h);
     * titulus in columna titulorum partium; NIHIL = nulla. Functio
     * mensuram [initium, finis) pulsuum scribit, sine titulo et sine
     * '\n'; FALSUM = memoria deest */
    constans character* additae_titulus;
    b32 (*addita)(vacuum* ctx, constans Carmen* carmen, s64 initium,
                  s64 finis, ChordaAedificator* textus);
    vacuum* additae_ctx;
} PartituraForma;

/* orthographia notae; clavis ex mappa carminis ad pulsum (absolutum) */
PartituraNota
partitura_orthographia (
        constans Carmen* carmen,
    constans CarmenNota* nota,
                    s64  pulsus);

/* nota ut partitura eam scribit, sine valore: "Bb4", "F#5", "@punctum"
 * (sonitus praeparatus). pulsus = pulsus CARMINIS notae (frustum +
 * nota). FALSUM = memoria deest */
b32
partitura_notam_scribere (
        constans Carmen* carmen,
    constans CarmenNota* nota,
                    s64  pulsus,
      ChordaAedificator* textus);

/* ambitus partis: nota infima et summa altitudinis (sonitus '@'
 * omittuntur - altitudine carent), cum pulsibus carminis ad
 * orthographiam; aequales: prima vincit */
nomen structura {
    constans CarmenNota* infima;           /* NIHIL si numerus 0 */
                    s64  pulsus_infimae;
    constans CarmenNota* summa;
                    s64  pulsus_summae;
                    i32  numerus;          /* notae altitudinis */
} PartituraAmbitus;

PartituraAmbitus
partitura_ambitus (
    constans CarmenPars* pars);

/* tonica clavis ut carmina.stml eam scribit ("a", "eb", "f#"); modus
 * (maior / minor) in clavis->minor. Statica */
constans character*
partitura_clavis_littera (
    constans CarmenClavis* clavis);

/* ':valor' partiturae (sine ':'): 1..64, '.' punctata, 't' trioli,
 * aliter '~pulsus' - lineis additis eadem grammatica */
b32
partitura_valorem_scribere (
                   s64  pulsus,
     ChordaAedificator* textus);

/* textum in aedificatorem appendit; forma NIHIL = totum carmen. FALSUM:
 * pars ignota, mensurae extra carmen (aut ultima < prima), memoria
 * deest */
b32
partitura_scribere (
             constans Carmen* carmen,
     constans PartituraForma* forma,
           ChordaAedificator* textus);

#endif /* PARTITURA_H */
