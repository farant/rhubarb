/* auscultatio.h - Carmen auditum per numeros: apex, tonsura, sonoritas
 *
 * Agens non audit: carmen offline redditur (via eadem ac vicus et
 * auditio: concentus + cantor, exitus scripturae, pulsus X ms) et
 * numeri eius leguntur - quod auris de mixtura diceret ("nimis sonat",
 * "bassus non auditur", "mensura tacet") in numeris.
 *
 * DUO GRADUS: reddere (sonus, deterministicus) et metiri (purum super
 * exempla - probationes exempla ficta dant, numeri exacti).
 *
 * QUADRUM 0 = PULSUS 0: reddere margines initii abscindit, ut mensurae
 * ex mappa temporis carminis (cantus_quadrum_pulsus) directe secentur.
 *
 * dBFS = 20 log10(x), 1.0 = 0 dBFS; mensura TACITA: rms < -60 dBFS
 * (AUSCULTATIO_TACITUM).
 *
 * USUS:
 *   si (auscultatio_reddere(carmen, NIHIL, piscina, &x, &n)
 *       && auscultatio_metiri(carmen, x, n, piscina, &s))
 *       ... s.apex, s.tonsa, s.rms, s.mensurae[m - 1] ...
 */
/* <aedilis corpus="lib/auscultatio.c"/> */
#ifndef AUSCULTATIO_H
#define AUSCULTATIO_H

#include "latina.h"
#include "piscina.h"
#include "carmen.h"
#include "cantor.h"

/* limen silentii: -60 dBFS (amplitudo rms) */
#define AUSCULTATIO_TACITUM 0.001f
/* cauda remissionis post finem carminis, quadra (capacitas prima) */
#define AUSCULTATIO_CAUDA (II * SONUS_FREQUENTIA)
/* cauda maxima dum aliquid vivit (reverberatio: capacitas duplicatur
 * usque ad hunc finem) - custodia contra sonum sine fine */
#define AUSCULTATIO_CAUDA_MAXIMA (XX * SONUS_FREQUENTIA)

nomen structura {
     f32 apex;       /* |exemplum| maximum */
     i32 tonsa;      /* exempla |x| > 1.0 (tonsura; 1.0 ipsum licitum,
                       * ubi limitator summae apices ponit) */
     f32  rms;        /* totius */
     f32* mensurae;   /* rms per mensuram (e piscina); [m - 1] */
     i32  numerus;    /* mensurae carminis */
} AuscultatioSummarium;

/* carmen semel offline: exempla stereo intercalata (e piscina), quadrum
 * 0 = pulsus 0, cauda inclusa (remissio, reverberatio) dum vivit,
 * ad AUSCULTATIO_CAUDA_MAXIMA.
 * resolutor NIHIL = ordinarius cantoris. FALSUM: memoria deest */
b32
auscultatio_reddere (
             constans Carmen*  carmen,
    constans CantorResolutor*  resolutor,
                     Piscina*  piscina,
                         f32** exempla,
                         i32*  quadra);

/* ut auscultatio_reddere, et census mixturae in fine redditionis
 * (apex ante limitatorem, limitata, reductio - limitator summae,
 * mixtura.h); census NIHIL = non petitur */
b32
auscultatio_reddere_censum (
             constans Carmen*  carmen,
    constans CantorResolutor*  resolutor,
                     Piscina*  piscina,
                         f32** exempla,
                         i32*  quadra,
               MixturaCensus*  census);

/* purum: exempla (stereo, quadra) in mensuras carminis (mappa temporis
 * et metri) secantur; mensura ultra exempla = 0. FALSUM: memoria
 * deest */
b32
auscultatio_metiri (
                 constans Carmen* carmen,
                    constans f32* exempla,
                             i32  quadra,
                         Piscina* piscina,
            AuscultatioSummarium* summarium);

/* pars sola (indice) redditur et metitur: aequilibrium mixturae.
 * FALSUM: index extra partes, memoria deest */
b32
auscultatio_partem (
             constans Carmen* carmen,
                         i32  pars,
    constans CantorResolutor* resolutor,
                     Piscina* piscina,
        AuscultatioSummarium* summarium);

/* mensurae (1 = prima) in quibus notae partis non mutae sonant (in
 * exemplari) sed summarium tacet - pars sine instrumento, lucrum 0;
 * numerus TOTUS redditur, 'capacitas' scribuntur */
i32
auscultatio_tacitae (
                  constans Carmen* carmen,
    constans AuscultatioSummarium* summarium,
                              i32* mensurae,
                              i32  capacitas);

#endif /* AUSCULTATIO_H */
