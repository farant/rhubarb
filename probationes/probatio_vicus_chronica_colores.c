/* probatio_vicus_chronica_colores.c - chronica C6: tags <event>,
 * <person>, <place> in pagina scribae colorantur (compositio VERA,
 * tabula 3, visus sinister cum libro): eventus bonus - tags aperiens
 * et claudens colore accentus secundi, descriptio non; eventus
 * vitiosus totus colore erroris; declaratio bona regio tota accentu;
 * declaratio non clausa errore. Pixela numerantur in cellulis. */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "eventus.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"

#include <stdlib.h>
#include <string.h>

/* ictus in cellulam (linea, columna) paginae sinistrae */
interior vacuum
ictus (
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

/* linea scribitur: ictus ad initium, inserere, Esc */
interior vacuum
scribere (
             ManusLudus* m,
                    s32  linea,
     constans character* textus)
{
    ictus(m, linea, ZEPHYRUM);
    (vacuum)manus_ludus_scribere(m, "i");
    (vacuum)manus_ludus_scribere(m, textus);
    (vacuum)manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM);
}

/* pixela coloris in cellulis [a, b) lineae paginae sinistrae */
interior i32
pixela_coloris (
    VicusApplicatio* app,
                s32  linea,
                s32  a,
                s32  b,
                i32  color)
{
    TabulaPixelorum* tp;
      LudusFenestra* lf;
                s32  x;
                s32  y;
                i32  n;

    tp = tabula_pixelorum_creare_nuda(app->piscina, CDLXXX, CDLXXX
        + VICUS_ALTITUDO_TABULARUM);
    lf = ludus_fenestra_creare(app->piscina, app->d,
        vicus_figurae(app->vicus), ZEPHYRUM, vicus_imago_fons,
        app->vicus, tp);
    si (!lf)
    {
        redde ZEPHYRUM;
    }
    ludus_quadrum(lf, M);
    n = ZEPHYRUM;
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        per (x = a * VI; x < b * VI; x++)
        {
            si ((i32)tabula_pixelorum_obtinere_pixelum(tp, (i32)(VI
                + x),
                    (i32)(VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII
                    + y)) == color)
            {
                n++;
            }
        }
    }
    redde n;
}

interior vacuum
_sectio_colores (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
        VicusApplicatio  app;
                  Vicus* v;
             ManusLudus* m;
                    i32  accentus;
                    i32  error;
                    i32  textus;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina,
        "probatio_vicus_chronica_colores");
    CREDO_NECESSE(vicus_applicatio_aedificare(&app, piscina, intern,
        vol, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app.vicus;
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    /* folium sinistrum angustum (folii_columnae): lineae breves */
    scribere(m, ZEPHYRUM, "<event year=\"1\">Ab</event>");
    scribere(m, I, "<event month=\"3\">x</event>");
    scribere(m, II, "<person>Moses</person>");
    scribere(m, III, "<place>Roma");
    (vacuum)vicus_pulsare(v);
    accentus =
        (i32)color_ad_pixelum(thema_color(COLOR_ACCENT_SECONDARY));
    error   = (i32)color_ad_pixelum(thema_color(COLOR_ERROR));
    textus  = (i32)color_ad_pixelum(thema_color(COLOR_TEXT));
    CREDO_FALSUM(accentus == (i32)color_ad_pixelum(thema_color(
        COLOR_TEXT)));
    CREDO_FALSUM(error
        == (i32)color_ad_pixelum(thema_color(COLOR_TEXT)));

    CREDO_GRADUS("eventus bonus: tags accentu, descriptio non");
    CREDO_VERUM(pixela_coloris(&app, ZEPHYRUM, ZEPHYRUM, XVI, accentus)
        > ZEPHYRUM);
    /* descriptio textu (glyphus '>' cellulae XV pixelum unum in XVI
     * mittit: numerus, non nihil) */
    CREDO_VERUM(pixela_coloris(&app, ZEPHYRUM, XVI, XVIII, textus)
        > pixela_coloris(&app, ZEPHYRUM, XVI, XVIII, accentus));
    CREDO_AEQUALIS_I32(pixela_coloris(&app, ZEPHYRUM, XVII, XVIII,
        accentus), ZEPHYRUM);
    CREDO_VERUM(pixela_coloris(&app, ZEPHYRUM, XVIII, XXVI, accentus)
        > ZEPHYRUM);
    CREDO_AEQUALIS_I32(pixela_coloris(&app, ZEPHYRUM, ZEPHYRUM, XXVI,
        error), ZEPHYRUM);

    CREDO_GRADUS("eventus sine anno: totus errore");
    CREDO_VERUM(pixela_coloris(&app, I, ZEPHYRUM, IV, error)
        > ZEPHYRUM);
    CREDO_VERUM(pixela_coloris(&app, I, XVII, XVIII, error) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(pixela_coloris(&app, I, ZEPHYRUM, XXVI,
        accentus), ZEPHYRUM);

    CREDO_GRADUS("persona: regio tota accentu");
    CREDO_VERUM(pixela_coloris(&app, II, ZEPHYRUM, VIII, accentus)
        > ZEPHYRUM);
    CREDO_VERUM(pixela_coloris(&app, II, VIII, XIII, accentus)
        > ZEPHYRUM);

    CREDO_GRADUS("locus non clausus: errore");
    CREDO_VERUM(pixela_coloris(&app, III, ZEPHYRUM, XI, error)
        > ZEPHYRUM);
    CREDO_AEQUALIS_I32(pixela_coloris(&app, III, ZEPHYRUM, XI,
        accentus), ZEPHYRUM);
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "colores", _sectio_colores, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_chronica_colores", SECTIONES);
}
