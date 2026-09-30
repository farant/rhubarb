/* quadrans.h - Imago in cellulas terminalis (quadrantes; project-specs/
 * quadrans-plan.md)
 *
 * PURA: nulla allocatio (exitus a vocante praebitur), nullus status,
 * nulla dependentia praeter latina.h et imago_typus.h. Reddit runas et
 * colores RGB, NON signa tesserae - idem nucleus tesserae, emulatori,
 * fenestrae servit; adaptator ad vocantes pertinet.
 *
 * ALGORITHMUS (OpenTUI supersampling.wgsl, renderQuadrantBlock, e
 * codice lectus 2026-09-30, commissum 7581976f): cellula = pixela 2x2
 * (TL, TR, BL, BR). Par pixelorum MAXIME distantium (distantia RGB
 * quadrata, par primum ordine (0,1) (0,2) (0,3) (1,2) (1,3) (2,3) in
 * aequalitate); luminositate (2126 R + 7152 G + 722 B) minor = OBSCURUS
 * (in aequalitate primus paris), alter = CLARUS; pixelum obscuro
 * propius (aut aeque propinquum) bitum suum ponit (TL 8, TR 4, BL 2, BR
 * 1); glyphus = quadrantes[bita], color litterae = obscurus, fundi =
 * clarus.
 *
 * DIFFERENTIAE CONSULTAE ab OpenTUI:
 * - alpha PRIMUM contra 'fundus' componitur: algorithmus colores opacos
 *   solos videt (cellulae tesserae alpha carent);
 * - bloccus uniformis (quattuor pixela aequalia - solus casus bitorum 0
 *   aut 15) = SPATIUM cum fundo = colore eius (OpenTUI: plenus cum
 *   'media' quae pro pixelis opacis TL solum est); spatium nullam
 *   commissuram glyphi in terminali per fontem pingenti monstrat;
 * - pixela extra imaginem aut regionem = fundus (OpenTUI: niger);
 * - arithmetica integra: exitus in omni machina idem.
 */

#ifndef QUADRANS_H
#define QUADRANS_H

#include "latina.h"
#include "imago_typus.h"

/* Cellula computata: runa (codepoint) + colores 0x00RRGGBB */
nomen structura {
    s32 runa;
    i32 color_litterae;
    i32 color_fundi;
} QuadransCellula;

/* QUADRANTES: 2x2 pixela per cellulam, XVI figurae (supra).
 * DIMIDIUM: 1x2 - pixelum superius = color litterae sub U+2580,
 * inferius = color fundi; pixela aequalia = spatium. Colores EXACTI,
 * resolutio horizontalis dimidia. */
nomen enumeratio {
    QUADRANS_QUADRANTES = 0,
    QUADRANS_DIMIDIUM
} QuadransModus;

/* Colores cellulae mixtae (planum D2): EXTREMA = pixela paris extremi
 * (OpenTUI); MEDIA = media cuiusque gregis - ORDINARIA: Q2 mensuravit
 * errorem in photographiis ad XLVI% EXTREMAE (theorema: media gregis
 * errorem quadratum pro partitione fixa minuit; aurum in
 * probationes/fixa/quadrans/aurum_errores.txt) */
nomen enumeratio {
    QUADRANS_EXTREMA = 0,
    QUADRANS_MEDIA
} QuadransColores;

nomen structura {
      QuadransModus modus;
    QuadransColores colores;
                i32 fundus;    /* 0x00RRGGBB: alpha et extra-fines */
} QuadransOptiones;

/* Optiones ordinariae: QUADRANTES, MEDIA, fundus niger */
QuadransOptiones
quadrans_optiones_ordinariae (vacuum);

/* Cellulae regionis latitudo x altitudo PIXELORUM: in *cellulae_lat x
 * *cellulae_alt (QUADRANTES: utrumque dimidium sursum rotundatum;
 * DIMIDIUM: latitudo eadem, altitudo dimidia sursum rotundata) */
vacuum
quadrans_mensurare (
    constans QuadransOptiones* optiones,
                          i32  latitudo,
                          i32  altitudo,
                          i32* cellulae_lat,
                          i32* cellulae_alt);

/* Regionem [x, x + latitudo) x [y, y + altitudo) pixelorum imaginis in
 * cellulas vertere (DIMIDIUM: cellula (i, j) = pixela (x + i, y + 2j ..
 * +1)); QUADRANTES: cellula (i, j) = pixela (x + 2i .. +1,
 * y + 2j .. +1).
 * exitus: cellulae_lat x cellulae_alt (quadrans_mensurare), ordine
 * linearum, a vocante praebitae. Pixela extra imaginem aut regionem =
 * fundus. imago NIHIL: omnia fundus. */
vacuum
quadrans_computare (
               constans Imago* imago,
                          s32  x,
                          s32  y,
                          i32  latitudo,
                          i32  altitudo,
    constans QuadransOptiones* optiones,
              QuadransCellula* exitus);

/* Error reconstructionis (planum D7b): cellulae (ex quadrans_computare
 * super EANDEM regionem et optiones) in pixela revertuntur - bitum
 * positum = color litterae, ceterum color fundi - et cum pixelis fontis
 * (contra fundum compositis) conferuntur. Reddit errorem quadratum
 * MEDIUM per canalem in CENTESIMIS (integer; 0 = perfecta). Regio
 * vacua: 0. */
i32
quadrans_error (
               constans Imago* imago,
                          s32  x,
                          s32  y,
                          i32  latitudo,
                          i32  altitudo,
    constans QuadransOptiones* optiones,
     constans QuadransCellula* cellulae);

#endif /* QUADRANS_H */
