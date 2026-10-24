/* functio.h - Functio harmonica: gradus Romani per fenestras et
 * clausulae (desideratum ...5BHW2C)
 *
 * GRADUS: radix acordi fenestrae (explicatio.h) ut gradus clavis ad
 * initium fenestrae. Littera magna: maior, aucta ('+'), septima
 * dominantis; parva: minor, diminuta ('o'), dimidia diminuta ('h7').
 * Inversio ex basso: trias "" 6 64; septima 7 65 43 42. In minore VI
 * et VII elevati in clave sunt (V, viio). Radix extra clavem: gradus
 * supra eam deminutus, 'b' semper (bVII, bVI, bII6 - usus analysis;
 * orthographia clavis Bb in C '#VI' daret). Applicata: maior aut
 * septima dominantis extra clavem cuius radix quinta supra gradum
 * diatonicum (non diminutum, non tonicum) -> "V/V", "V7/ii"; diminuta
 * aut diminuta septima extra clavem semitonio infra -> "viio7/V".
 * Dyas: gradus cum qualitate dyadis ("I(5)", "ii(3)"); classis una
 * "(I)"; ignotum "?"; silentium "-". Sextae augmentatae et cetera
 * chromatica: nondum.
 *
 * CLAUSULA (cadentia; heuristica, sine signis formae): ADVENTUS =
 * fenestra quae in initio mensurae incipit et ad finem eius manet, et
 * pars summa (notae summae in initio) nullam notam in dimidia
 * posteriore mensurae incipit - tenet aut tacet (requies). Melodia
 * quartis currens (korobeiniki) sic non in omni mensura cadit. Genus
 * ex fenestra priore:
 *   perfecta     V aut V7 radicalis -> I radicalis, summa (in initio
 *                adventus) tonica
 *   imperfecta   V(7) aut viio -> I aliter
 *   dimidia      adventus in V
 *   plagalis     IV -> I
 *   deceptiva    V(7) -> vi (minor: VI)
 * I et V etiam dyades (C E, C G; G B), I etiam unisonus (duae voces
 * in tonica desinentes): clausulae quintam saepe omittunt. Alia nullum
 * nomen.
 */
/* <aedilis corpus="lib/functio.c"/> */
#ifndef FUNCTIO_H
#define FUNCTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "carmen.h"
#include "explicatio.h"

nomen enumeratio {
    FUNCTIO_NULLA,
    FUNCTIO_PERFECTA,
    FUNCTIO_IMPERFECTA,
    FUNCTIO_DIMIDIA,
    FUNCTIO_PLAGALIS,
    FUNCTIO_FALLAX
} FunctioClausula;

nomen structura {
                i32 fenestra;        /* index in explicatione */
                s32 gradus;          /* 1..7; 0 = nullus (?, -) */
                s32 alteratio;       /* -1 'b' (radix extra clavem) */
                s32 applicata;       /* gradus cuius dominans; 0 =
                                       * nulla */
          character numeralis[XVI];  /* "V7/V", "bII6", "viio7" */
    FunctioClausula clausula;        /* in fenestra adventus */
} FunctioGradus;

nomen structura {
    constans Explicatio* explicatio;
          FunctioGradus* gradus;      /* per fenestram, ordine */
                    i32  numerus;
} Functio;

/* explicatio prius (explicatio_carminis). FALSUM: memoria deest */
b32
functio_carminis (
        constans Carmen* carmen,
    constans Explicatio* explicatio,
                Piscina* piscina,
                Functio* functio);

/* "perfecta", "dimidia" ...; NIHIL pro NULLA (statica) */
constans character*
functio_clausulae_titulus (
    FunctioClausula clausula);

/* linea addita (PartituraAddita.scribere, ctx = Functio*): "I:2 V7:2
 * | I:1 [perf.]" - valor per fenestram, clausula post adventum */
b32
functio_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus);

/* tertia Picardiae: clavis minor, acordum fenestrae MAIUS in finali
 * (trias maior aut tertia maior), et fenestra aut ultima sonans
 * carminis, aut adventus post dominantem (V, V7) cuius fenestra
 * proxima non in gradu IV stat (ubi V/iv verum est). Ut I legitur
 * (clausula servatur); censura tertiam eius clavem non violare
 * iudicat */
b32
functio_tertia_picardiae (
        constans Carmen* carmen,
    constans Explicatio* explicatio,
                    i32  fenestra);

#endif /* FUNCTIO_H */
