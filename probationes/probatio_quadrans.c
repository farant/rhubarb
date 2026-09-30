/* probatio_quadrans.c - quadrans Q1: nucleus purus (project-specs/
 * quadrans-plan.md)
 *
 * I.   XIV glyphi mixti (bita 1-14) ex nigro et albo: glyphus, color
 *      litterae (obscurus), fundi (clarus) - tabula EX OpenTUI
 *      supersampling.wgsl hic iterum scripta, non ex codice nostro.
 * II.  Bloccus uniformis = spatium cum fundo = colore eius.
 * III. EXTREMA contra MEDIA (tres gradus grisei).
 * IV.  Aequalitates: distantiae (par primum), luminositas (primus
 *      paris obscurus).
 * V.   Alpha contra fundum; regio impar (pixela extra = fundus);
 *      regio extra imaginem; imago NIHIL; mensurae.
 * VI.  Error reconstructionis (Q2): casus manu computati; optiones
 *      ordinariae = MEDIA (decisio D2 mensurata).
 * VII. Mensura D2 super imagines (photographiae Frani + synthetica):
 *      MEDIA <= EXTREMA in omni imagine (theorema: media gregis errorem
 *      quadratum pro partitione fixa minuit - violatio = vitium); aurum
 *      errorum commissum solum decrescit (QUADRANS_AURUM_SCRIBERE=1
 *      scribit).
 * VIII. DIMIDIUM (Q3): figura superior, spatium, altitudo impar,
 *      exactitudo (error 0 in resolutione propria); comparatio aequa
 *      modorum pro EODEM rete cellularum (80x48): reconstructio in
 *      reticulum 160x96 contra referentiam eandem (in aurum quoque).
 */
#include "latina.h"
#include "piscina.h"
#include "imago_typus.h"
#include "imago.h"
#include "imago_opus.h"
#include "quadrans.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AURUM_VIA "probationes/fixa/quadrans/aurum_errores.txt"
#define MENSURA_LAT CLX
#define MENSURA_ALT XCVI

/* quadrantChars OpenTUI (bita TL 8, TR 4, BL 2, BR 1) */
hic_manens constans s32 FIGURAE[XVI] = {
    0x20,   0x2597, 0x2596, 0x2584, 0x259D, 0x2590, 0x259E, 0x259F,
    0x2598, 0x259A, 0x258C, 0x2599, 0x2580, 0x259C, 0x259B, 0x2588
};

interior Imago
_imago (
    Piscina* piscina,
        i32  latitudo,
        i32  altitudo)
{
    Imago im;

    im.latitudo = latitudo;
    im.altitudo = altitudo;
    im.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(latitudo * altitudo * IV));
    memset(im.pixela, ZEPHYRUM,
        (memoriae_index)(latitudo * altitudo * IV));
    redde im;
}

/* pixelum ponere: color 0x00RRGGBB, alpha */
interior vacuum
_pixelum (
     Imago* im,
       i32  x,
       i32  y,
       i32  color,
       i32  alpha)
{
    i8* p = im->pixela + (y * im->latitudo + x) * IV;

    p[0] = (i8)((color >> XVI) & 0xFF);
    p[1] = (i8)((color >> VIII) & 0xFF);
    p[2] = (i8)(color & 0xFF);
    p[3] = (i8)alpha;
}

/* Bloccum 2x2 (TL, TR, BL, BR) opacum computare */
interior QuadransCellula
_bloccus (
              Piscina* piscina,
                  i32  tl,
                  i32  tr,
                  i32  bl,
                  i32  br,
      QuadransColores  colores)
{
               Imago im  = _imago(piscina, II, II);
    QuadransOptiones o   = quadrans_optiones_ordinariae();
     QuadransCellula c;

    _pixelum(&im, ZEPHYRUM, ZEPHYRUM, tl, 0xFF);
    _pixelum(&im, I, ZEPHYRUM, tr, 0xFF);
    _pixelum(&im, ZEPHYRUM, I, bl, 0xFF);
    _pixelum(&im, I, I, br, 0xFF);
    o.colores         = colores;
    c.runa            = -I;
    c.color_litterae  = 0xDEAD;
    c.color_fundi     = 0xDEAD;
    quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, II, II, &o, &c);
    redde c;
}

interior vacuum
_cella (
    QuadransCellula c,
                s32 runa,
                i32 litterae,
                i32 fundi,
    constans character* causa)
{
    si (   c.runa        != runa || c.color_litterae != litterae
        || c.color_fundi != fundi)
    {
        imprimere("  FRACTA: %s: U+%04X %06x/%06x, exspectata U+%04X "
            "%06x/%06x\n", causa, (insignatus integer)c.runa,
            (insignatus integer)c.color_litterae,
            (insignatus integer)c.color_fundi,
            (insignatus integer)runa, (insignatus integer)litterae,
            (insignatus integer)fundi);
    }
    CREDO_AEQUALIS_S32 (c.runa, runa);
    CREDO_AEQUALIS_I32 (c.color_litterae, litterae);
    CREDO_AEQUALIS_I32 (c.color_fundi, fundi);
}

/* Error unius imaginis sub colore dato (regio tota) */
interior i32
_error_imaginis (
            Piscina* piscina,
     constans Imago* im,
    QuadransColores  colores)
{
    QuadransOptiones  o = quadrans_optiones_ordinariae();
     QuadransCellula* c;
                 i32  lat;
                 i32  alt;

    o.colores = colores;
    quadrans_mensurare(&o, im->latitudo, im->altitudo, &lat, &alt);
    c = (QuadransCellula*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt) * magnitudo(QuadransCellula));
    quadrans_computare(im, ZEPHYRUM, ZEPHYRUM, im->latitudo,
        im->altitudo,
        &o, c);
    redde quadrans_error(im, ZEPHYRUM, ZEPHYRUM, im->latitudo,
        im->altitudo, &o, c);
}

/* Error DIMIDII contra referentiam R (latitudo R = 2 x latitudo
 * cellularum aut minus uno): pixelum R (px, py) = cellula (px / 2,
 * py / 2), superius si py par (figura -> color litterae) */
interior i32
_error_dimidii_contra (
              constans Imago* referentia,
    constans QuadransCellula* c,
                         i32  lat)
{
    i64 summa = ZEPHYRUM;
    i64 n;
    i32 px;
    i32 py;

    per (py = ZEPHYRUM; py < referentia->altitudo; py++)
    {
        per (px = ZEPHYRUM; px < referentia->latitudo; px++)
        {
            constans QuadransCellula* q = &c[(py / II) * lat + px / II];
                         constans i8* r = referentia->pixela
                             + (py * referentia->latitudo + px) * IV;
            i32 color = (q->runa == 0x2580 && (py % II) == ZEPHYRUM)
                ? q->color_litterae : q->color_fundi;
            i32 k;

            per (k = ZEPHYRUM; k < III; k++)
            {
                s32 d = (s32)r[k]
                    - (s32)((color >> (XVI - VIII * k)) & 0xFF);

                summa += (i64)(d * d);
            }
        }
    }
    n = (i64)referentia->latitudo * (i64)referentia->altitudo * III;
    redde (i32)((summa * C + n / II) / n);
}

/* Aurum: lineae 'titulus extrema media'; -1 si abest */
interior vacuum
_aurum_legere (
    constans character* titulus,
                   s32* extrema,
                   s32* media)
{
          FILE* f = fopen(AURUM_VIA, "r");
     character  t[LXIV];
        longus  e;
        longus  m;

    *extrema  = -I;
    *media    = -I;
    si (f == NIHIL)
    {
        redde;
    }
    dum (fscanf(f, "%63s %ld %ld", t, &e, &m) == III)
    {
        si (strcmp(t, titulus) == ZEPHYRUM)
        {
            *extrema  = (s32)e;
            *media    = (s32)m;
        }
    }
    fclose(f);
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
         i32  bita;

    piscina = piscina_generare_dynamicum("probatio_quadrans", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. Glyphi mixti (nigrum/album) ---\n");
    per (bita = I; bita < XV; bita++)
    {
        i32 k;

        per (k = ZEPHYRUM; k < II; k++)
        {
            QuadransColores colores = (k == ZEPHYRUM)
                ? QUADRANS_EXTREMA : QUADRANS_MEDIA;
            QuadransCellula c = _bloccus(piscina,
                (bita & VIII) ? ZEPHYRUM : 0xFFFFFF,
                (bita & IV) ? ZEPHYRUM : 0xFFFFFF,
                (bita & II) ? ZEPHYRUM : 0xFFFFFF,
                (bita & I) ? ZEPHYRUM : 0xFFFFFF, colores);
            character causa[XLVIII];

            sprintf(causa, "bita %u (%s)", (insignatus integer)bita,
                k == ZEPHYRUM ? "extrema" : "media");
            _cella(c, FIGURAE[bita], ZEPHYRUM, 0xFFFFFF, causa);
        }
    }

    imprimere("\n--- II. Uniformis ---\n");
    _cella(_bloccus(piscina, 0x336699, 0x336699, 0x336699, 0x336699,
        QUADRANS_EXTREMA), 0x20, 0x336699, 0x336699, "uniformis");

    imprimere("\n--- III. Extrema contra media ---\n");
    /* 0, 10, 200, 255 griseum: par (TL, BR); TR obscuro propior, BL
     * claro -> bita 12 (superius) */
    _cella(_bloccus(piscina, ZEPHYRUM, 0x0A0A0A, 0xC8C8C8, 0xFFFFFF,
        QUADRANS_EXTREMA), 0x2580, ZEPHYRUM, 0xFFFFFF, "extrema");
    /* media: (0+10)/2 = 5; (200+255)/2 = 227,5 -> 228 (sursum) */
    _cella(_bloccus(piscina, ZEPHYRUM, 0x0A0A0A, 0xC8C8C8, 0xFFFFFF,
        QUADRANS_MEDIA), 0x2580, 0x050505, 0xE4E4E4, "media");

    imprimere("\n--- IV. Aequalitates ---\n");
    /* distantiae aequales: par primum (TL, TR) -> bita 10 */
    _cella(_bloccus(piscina, ZEPHYRUM, 0xFFFFFF, ZEPHYRUM, 0xFFFFFF,
        QUADRANS_EXTREMA), 0x258C, ZEPHYRUM, 0xFFFFFF,
        "distantiae aequales");
    /* luminositas aequalis (0,38,239) et (209,0,0) = 444334: primus
     * paris obscurus */
    _cella(_bloccus(piscina, 0x0026EF, 0xD10000, 0x0026EF, 0xD10000,
        QUADRANS_EXTREMA), 0x258C, 0x0026EF, 0xD10000,
        "luminositas aequalis (caeruleum primum)");
    _cella(_bloccus(piscina, 0xD10000, 0x0026EF, 0xD10000, 0x0026EF,
        QUADRANS_EXTREMA), 0x258C, 0xD10000, 0x0026EF,
        "luminositas aequalis (rubrum primum)");
    /* diagonalis A B / B A: par primum (TL, TR) -> A obscurus -> bita 9
     * (TL + BR); par ultimum (BL, BR) B obscurum daret (bita 6) - hic
     * ordo paris vere refert */
    _cella(_bloccus(piscina, 0x0026EF, 0xD10000, 0xD10000, 0x0026EF,
        QUADRANS_EXTREMA), 0x259A, 0x0026EF, 0xD10000,
        "par primum inter aequales (diagonalis)");

    imprimere("\n--- V. Alpha, regiones, mensurae ---\n");
    {
                Imago im  = _imago(piscina, III, III);
     QuadransOptiones o   = quadrans_optiones_ordinariae();
      QuadransCellula c[IV];
                  i32 lat;
                  i32 alt;
                  i32 x;
                  i32 y;

        /* alpha 0 ubique -> fundus; alpha 128 album super nigrum ->
         * (255 * 128 + 127) / 255 = 128 */
        o.fundus = 0x102030;
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, II, II, &o, c);
        _cella(c[0], 0x20, 0x102030, 0x102030, "alpha 0 = fundus");
        per (y = ZEPHYRUM; y < II; y++)
        {
            per (x = ZEPHYRUM; x < II; x++)
            {
                _pixelum(&im, x, y, 0xFFFFFF, 0x80);
            }
        }
        o.fundus = ZEPHYRUM;
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, II, II, &o, c);
        _cella(c[0], 0x20, 0x808080, 0x808080, "alpha 128");

        /* regio impar III x III rubra, fundus caeruleus: columna et
         * ordo ultimi dimidii extra -> caeruleum (obscurius) */
        per (y = ZEPHYRUM; y < III; y++)
        {
            per (x = ZEPHYRUM; x < III; x++)
            {
                _pixelum(&im, x, y, 0xFF0000, 0xFF);
            }
        }
        o.fundus = 0x0000FF;
        quadrans_mensurare(&o, III, III, &lat, &alt);
        CREDO_AEQUALIS_I32 (lat, II);
        CREDO_AEQUALIS_I32 (alt, II);
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, III, III, &o, c);
        _cella(c[0], 0x20, 0xFF0000, 0xFF0000, "impar (0,0) plenum");
        _cella(c[1], 0x2590, 0x0000FF, 0xFF0000, "impar (1,0) dextra");
        _cella(c[2], 0x2584, 0x0000FF, 0xFF0000, "impar (0,1) infra");
        _cella(c[3], 0x259F, 0x0000FF, 0xFF0000, "impar (1,1) angulus");

        /* regio partim extra imaginem (x = -1): sinistra fundus */
        quadrans_computare(&im, -I, ZEPHYRUM, II, II, &o, c);
        _cella(c[0], 0x258C, 0x0000FF, 0xFF0000,
            "regio extra sinistram");
        /* imago NIHIL: omnia fundus */
        quadrans_computare(NIHIL, ZEPHYRUM, ZEPHYRUM, II, II, &o, c);
        _cella(c[0], 0x20, 0x0000FF, 0x0000FF, "imago nulla");

        quadrans_mensurare(&o, IV, II, &lat, &alt);
        CREDO_AEQUALIS_I32 (lat, II);
        CREDO_AEQUALIS_I32 (alt, I);
        quadrans_mensurare(&o, ZEPHYRUM, ZEPHYRUM, &lat, &alt);
        CREDO_AEQUALIS_I32 (lat, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (alt, ZEPHYRUM);
    }

    imprimere("\n--- VI. Error reconstructionis ---\n");
    {
                   Imago im  = _imago(piscina, II, II);
        QuadransOptiones o   = quadrans_optiones_ordinariae();
         QuadransCellula c;
                     i32 k;

        /* decisio D2 (mensura VII): MEDIA ordinaria */
        CREDO_AEQUALIS_I32 ((i32)o.colores, (i32)QUADRANS_MEDIA);
        /* tres gradus grisei (III supra), manu: EXTREMA 9375 / 12 =
         * 781,25 -> 78125; MEDIA 4689 / 12 = 390,75 -> 39075 */
        _pixelum(&im, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, 0xFF);
        _pixelum(&im, I, ZEPHYRUM, 0x0A0A0A, 0xFF);
        _pixelum(&im, ZEPHYRUM, I, 0xC8C8C8, 0xFF);
        _pixelum(&im, I, I, 0xFFFFFF, 0xFF);
        per (k = ZEPHYRUM; k < II; k++)
        {
            o.colores = (k == ZEPHYRUM) ? QUADRANS_EXTREMA
                                        : QUADRANS_MEDIA;
            quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, II, II, &o, &c);
            CREDO_AEQUALIS_I32 (quadrans_error(&im, ZEPHYRUM, ZEPHYRUM,
                II, II, &o, &c), (k == ZEPHYRUM) ? 78125 : 39075);
        }
        /* bloccus duorum colorum: reconstructio perfecta */
        _pixelum(&im, I, ZEPHYRUM, ZEPHYRUM, 0xFF);
        _pixelum(&im, ZEPHYRUM, I, 0xFFFFFF, 0xFF);
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, II, II, &o, &c);
        CREDO_AEQUALIS_I32 (quadrans_error(&im, ZEPHYRUM, ZEPHYRUM, II,
            II, &o, &c), ZEPHYRUM);
        /* regio vacua */
        CREDO_AEQUALIS_I32 (quadrans_error(&im, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, &o, &c), ZEPHYRUM);
    }

    imprimere("\n--- VII. Mensura D2 (EXTREMA contra MEDIA) ---\n");
    {
        hic_manens constans character* constans TITULI[] = {
            "assumptio", "christus_sculptus", "gradiens", "tabula",
            "strepitus"
        };
        b32 scribere = (b32)(getenv("QUADRANS_AURUM_SCRIBERE")
            != NIHIL);
        FILE* aurum = scribere ? fopen(AURUM_VIA, "w") : NIHIL;
         i32  k;

        per (k = ZEPHYRUM; k < V; k++)
        {
            Imago im;
              s32 extrema;
              s32 media;
              s32 aurum_extrema;
              s32 aurum_media;

            si (k < II)
            {
                   character via[CXXVIII];
                ImagoFructus f;

                sprintf(via, "probationes/fixa/quadrans/%s.jpg",
                    TITULI[k]);
                f = imago_caricare_ex_file(via, piscina);
                CREDO_VERUM (f.successus);
                si (!f.successus)
                {
                    perge;
                }
                im = imago_scalare_ad_limites(&f.imago, MENSURA_LAT,
                    MENSURA_ALT, IMAGO_SCALA_AREA, piscina);
            }
            alioquin
            {
                i32 px;
                i32 py;
                i32 semen = 12345;

                im = _imago(piscina, MENSURA_LAT, MENSURA_ALT);
                per (py = ZEPHYRUM; py < MENSURA_ALT; py++)
                {
                    per (px = ZEPHYRUM; px < MENSURA_LAT; px++)
                    {
                        i32 color;

                        si (k == II)
                        {
                            color = ((px * 0xFF / (MENSURA_LAT - I))
                                << XVI)
                                | ((py * 0xFF / (MENSURA_ALT - I))
                                    << VIII)
                                | 0x80;
                        }
                        alioquin si (k == III)
                        {
                            color = ((px + py) % II) ? 0xFFFFFF
                                                     : ZEPHYRUM;
                        }
                        alioquin
                        {
                            semen = semen * 1103515245u + 12345u;
                            color = (semen >> VIII) & 0xFFFFFF;
                        }
                        _pixelum(&im, px, py, color, 0xFF);
                    }
                }
            }
            extrema = (s32)_error_imaginis(piscina, &im,
                QUADRANS_EXTREMA);
            media = (s32)_error_imaginis(piscina, &im,
                QUADRANS_MEDIA);
            imprimere("  %-18s %4ux%-3u extrema %9d  media %9d  "
                "(%3d%%)\n",
                TITULI[k], (insignatus integer)im.latitudo,
                (insignatus integer)im.altitudo, (integer)extrema,
                (integer)media,
                (integer)(extrema > ZEPHYRUM ? media * C
                    / extrema : C));
            CREDO_VERUM (media <= extrema);
            si (aurum != NIHIL)
            {
                fprintf(aurum, "%s %d %d\n", TITULI[k],
                    (integer)extrema,
                    (integer)media);
            }
            _aurum_legere(TITULI[k], &aurum_extrema, &aurum_media);
            si (!scribere)
            {
                si (aurum_extrema < ZEPHYRUM || extrema > aurum_extrema
                    || media > aurum_media)
                {
                    imprimere("  FRACTA: %s: aurum %d/%d\n", TITULI[k],
                        (integer)aurum_extrema, (integer)aurum_media);
                }
                CREDO_VERUM (aurum_extrema >= ZEPHYRUM);
                CREDO_VERUM (extrema <= aurum_extrema);
                CREDO_VERUM (media <= aurum_media);
            }
        }
        si (aurum != NIHIL)
        {
            fclose(aurum);
            imprimere("  aurum scriptum: %s\n", AURUM_VIA);
        }
    }

    imprimere("\n--- VIII. DIMIDIUM ---\n");
    {
                Imago im  = _imago(piscina, I, III);
     QuadransOptiones o   = quadrans_optiones_ordinariae();
      QuadransCellula c[II];
                  i32 lat;
                  i32 alt;

        o.modus = QUADRANS_DIMIDIUM;
        quadrans_mensurare(&o, III, III, &lat, &alt);
        CREDO_AEQUALIS_I32 (lat, III);
        CREDO_AEQUALIS_I32 (alt, II);
        _pixelum(&im, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, 0xFF);
        _pixelum(&im, ZEPHYRUM, I, 0xFFFFFF, 0xFF);
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, I, II, &o, c);
        _cella(c[0], 0x2580, ZEPHYRUM, 0xFFFFFF,
            "dimidium nigrum/album");
        _pixelum(&im, ZEPHYRUM, I, ZEPHYRUM, 0xFF);
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, I, II, &o, c);
        _cella(c[0], 0x20, ZEPHYRUM, ZEPHYRUM, "dimidium aequale");
        /* altitudo impar: pixelum inferius ultimum = fundus */
        _pixelum(&im, ZEPHYRUM, ZEPHYRUM, 0xFF0000, 0xFF);
        _pixelum(&im, ZEPHYRUM, I, 0xFF0000, 0xFF);
        _pixelum(&im, ZEPHYRUM, II, 0xFF0000, 0xFF);
        o.fundus = 0x0000FF;
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, I, III, &o, c);
        _cella(c[0], 0x20, 0xFF0000, 0xFF0000, "dimidium rubrum");
        _cella(c[1], 0x2580, 0xFF0000, 0x0000FF, "dimidium impar");
    }
    {
        hic_manens constans character* constans PHOTOGRAPHIAE[] = {
            "assumptio", "christus_sculptus"
        };
        b32 scribere = (b32)(getenv("QUADRANS_AURUM_SCRIBERE")
            != NIHIL);
        FILE* aurum = scribere ? fopen(AURUM_VIA, "a") : NIHIL;
         i32  k;

        per (k = ZEPHYRUM; k < II; k++)
        {
                   character  via[CXXVIII];
                   character  titulus[LXIV];
                ImagoFructus  f;
                       Imago  referentia;
                       Imago  dimidia;
            QuadransOptiones  o = quadrans_optiones_ordinariae();
             QuadransCellula* c;
                         i32  lat;
                         i32  alt;
                         s32  quadrantes;
                         s32  dimidium;
                         s32  aurum_q;
                         s32  aurum_d;

            sprintf(via, "probationes/fixa/quadrans/%s.jpg",
                PHOTOGRAPHIAE[k]);
            f = imago_caricare_ex_file(via, piscina);
            CREDO_VERUM (f.successus);
            si (!f.successus)
            {
                perge;
            }
            referentia = imago_scalare_ad_limites(&f.imago, MENSURA_LAT,
                MENSURA_ALT, IMAGO_SCALA_AREA, piscina);
            quadrantes = (s32)_error_imaginis(piscina, &referentia,
                QUADRANS_MEDIA);
            dimidia = imago_scalare(&f.imago,
                (referentia.latitudo + I) / II, referentia.altitudo,
                IMAGO_SCALA_AREA, piscina);
            o.modus = QUADRANS_DIMIDIUM;
            quadrans_mensurare(&o, dimidia.latitudo, dimidia.altitudo,
                &lat, &alt);
            c = (QuadransCellula*)piscina_allocare(piscina,
                (memoriae_index)(lat * alt)
                    * magnitudo(QuadransCellula));
            quadrans_computare(&dimidia, ZEPHYRUM, ZEPHYRUM,
                dimidia.latitudo, dimidia.altitudo, &o, c);
            /* exactitudo in resolutione propria */
            CREDO_AEQUALIS_I32 (quadrans_error(&dimidia, ZEPHYRUM,
                ZEPHYRUM, dimidia.latitudo, dimidia.altitudo, &o, c),
                ZEPHYRUM);
            dimidium = (s32)_error_dimidii_contra(&referentia, c, lat);
            imprimere("  %-18s idem rete: quadrantes %6d  "
                "dimidium %6d\n",
                PHOTOGRAPHIAE[k], (integer)quadrantes,
                (integer)dimidium);
            sprintf(titulus, "%s_modi", PHOTOGRAPHIAE[k]);
            si (aurum != NIHIL)
            {
                fprintf(aurum, "%s %d %d\n", titulus,
                    (integer)quadrantes, (integer)dimidium);
            }
            _aurum_legere(titulus, &aurum_q, &aurum_d);
            si (!scribere)
            {
                CREDO_VERUM (aurum_q >= ZEPHYRUM);
                CREDO_VERUM (quadrantes <= aurum_q);
                CREDO_VERUM (dimidium <= aurum_d);
            }
        }
        si (aurum != NIHIL)
        {
            fclose(aurum);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
