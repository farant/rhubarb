/* functio.h - Functio harmonica: gradus Romani per fenestras et
 * clausulae (desideratum ...5BHW2C)
 *
 * GRADUS: radix acordi fenestrae (explicatio.h) ut gradus clavis ad
 * initium fenestrae. Littera magna: maior, aucta ('+'), septima
 * dominantis; parva: minor, diminuta ('o'), dimidia diminuta ('h7').
 * Inversio ex basso: trias "" 6 64; septima 7 65 43 42. In minore VI
 * et VII elevati in clave sunt (V, viio). Radix extra clavem: gradus
 * supra eam deminutus, 'b' semper (bVII, bVI, bII6 - usus analysis;
 * orthographia clavis Bb in C '#VI' daret). Applicata: maior, septima
 * aut nona dominantis (9, 7b9) extra clavem cuius radix quinta supra
 * gradum diatonicum (non diminutum, non tonicum) -> "V/V", "V7/ii";
 * diminuta aut diminuta septima extra clavem semitonio infra ->
 * "viio7/V".
 * Dyas: gradus cum qualitate dyadis ("I(5)", "ii(3)"); classis una
 * "(I)"; ignotum "?"; silentium "-". Sextae augmentatae et cetera
 * chromatica: nondum. Toni additi (6, 9): figura inversionis nulla.
 *
 * DUPLEX USUS: acordum sextae (6, m6) in gradu IV ante dominantem
 * (fenestra proxima V, V7, V9, V7b9, viio) ut septima gradus II
 * legitur - F6 in C ante G = "ii65", Dm6 in a minor ante E = "iih65";
 * aliter IVadd6; sexta in gradu I (Iadd6) manet.
 *
 * SUBSTITUTA (subV, tritonus substitutus): forma dominantis CUM
 * septima (7, 9, 7b9, 13, 7#9, 7#11) extra clavem, non applicata,
 * cuius radix semitonio supra radicem fenestrae sonantis proximae
 * stat, meta gradus diatonicus (maior aut minor, etiam I) -> "subV7"
 * (Db7 -> C in C), "subV7/ii" (Eb7 -> Dm), "subV7#11". Sine
 * resolutione manet bII7; trias (Db) manet bII (Neapolitana).
 * Clausula: subV(7) -> I ut V -> I (perfecta si ambo radicales et
 * summa tonica, aliter imperfecta).
 *
 * CLAVIS: clavis ad initium fenestrae (<clavis> carminis), praeter
 * DOMINANTEM PRAEVIAM - fenestra ultima SONANS ante mutationem clavis
 * (fenestra sonans proxima in clave alia incipit; pausae
 * transiliuntur) cuius acordum dominans
 * clavis novae est (forma dominantis: maior, 7, 9, 7b9, (3) in
 * quinta tonicae novae; aut o, o7, h7 semitonio infra eam) in clave
 * nova legitur: D7 ante G maior in F maior = V7, non V7/ii - sic
 * clausula in tonicam novam invenitur (D7 -> G, summa G: perfecta).
 * Linea functionis clavem novam ante dominantem praeviam nominat
 * ("G: V7:1"; minor littera parva: "e: V7:1"). Alia acorda ante
 * mutationem (ii-V: Am7 D7 | G) in clave vetere manent. Tonica
 * clavis veteris (radix = tonica: I, i, Picardia) numquam praevia: C
 * in C maior ante F maior manet I cum clausula sua; D post A7 in d
 * minor ante G maior Picardia est.
 *
 * CLAUSULA (cadentia; heuristica, sine signis formae): ADVENTUS =
 * fenestra quae in initio mensurae incipit et ad finem eius manet, et
 * pars summa (notae summae in initio) nullam notam in dimidia
 * posteriore mensurae incipit - tenet aut tacet (requies). Melodia
 * quartis currens (korobeiniki) sic non in omni mensura cadit. Genus
 * ex fenestra priore:
 *   perfecta     V aut V7 (aut subV) radicalis -> I radicalis, summa
 *                (in initio adventus) tonica
 *   imperfecta   V(7), subV aut viio -> I aliter
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
                b32 praevia;         /* in clave proxima lecta
                                       * (dominans praevia) */
                b32 substituta;      /* subV (tritonus substitutus);
                                       * applicata = meta (0: I) */
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
 * (trias maior, sexta, nona addita, tertia maior), et fenestra aut
 * ultima sonans carminis, aut adventus post dominantem (V, V7) cuius
 * fenestra proxima non in gradu IV stat (ubi V/iv verum est) - prior
 * et proxima in clave FENESTRAE leguntur. Ut I legitur (clausula
 * servatur); censura tertiam eius clavem non violare iudicat */
b32
functio_tertia_picardiae (
        constans Carmen* carmen,
    constans Explicatio* explicatio,
                    i32  fenestra);

#endif /* FUNCTIO_H */
