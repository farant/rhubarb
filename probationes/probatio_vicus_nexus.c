/* probatio_vicus_nexus.c - nexus '#verbum' in pagina scribae
 * (vicus-latera S3d-1)
 *
 * Compositio VERA (vicus_applicatio, tabula 3, visus sinister).
 * Paginae: 1 = "#next #prev #first #last" / "#2 #9 #notae #nihil"; 2 =
 * linea III "     #notae #prev"; 3 = linea IV "  #notae #first".
 * I: '#2' paginam id II. II: tag '#notae' per paginas cyclus (1 -> 2
 * -> 3 -> 1, circulo), cursor in tag. III: '#nihil' et '#9' nuntium
 * ponunt, pagina manet. IV: '#last' '#first' '#next' '#prev'. V: ictus
 * unus in latus non focatum. VI: nexus colore accentus secundi. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "insula.h"
#include "eventus.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

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

/* Ctrl+Shift+sagitta */
interior vacuum
paginam_mutare (
    Dispensator* d,
       clavis_t  sagitta)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis        = sagitta;
    e.datum.clavis.modificantes  = MOD_IMPERIUM | MOD_SHIFT;
    dispensator_tractare(d, &e);
}

interior b32
pagina_est (
                Volumen* vol,
     constans character* nomen_paginae)
{
    chorda c;
       b32 inventum;

    c = volumen_plagulam_promere(vol, chorda_ex_literis(
        "scriba/visus/3_sinistrum_scriba", piscina), piscina,
        &inventum);
    redde inventum && chorda_aequalis_literis(c, nomen_paginae);
}

interior chorda*
attributum (
                 Vicus* v,
    constans character* titulus)
{
     VicusLatus* l;
    InsulaRamus  r;

    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    redde insula_ramus_attributum(&r, INSULA_EPHEMERA, titulus);
}

interior b32
cursor_est (
    Vicus* v,
      s32  linea,
      s32  columna)
{
     chorda* a;
     chorda* b;
        s32  x;
        s32  y;

    a = attributum(v, "cursor_linea");
    b = attributum(v, "cursor_columna");
    redde a && b && chorda_ut_s32(*a, &y) && chorda_ut_s32(*b, &x)
        && y == linea && x == columna;
}

interior b32
nuntius_est (
                 Vicus* v,
    constans character* textus)
{
    chorda* a;

    a = attributum(v, "nuntius");
    redde a && chorda_aequalis_literis(*a, textus);
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

    tp = tabula_pixelorum_creare_nuda(piscina, CDLXXX, CDLXXX
        + VICUS_ALTITUDO_TABULARUM);
    lf = ludus_fenestra_creare(piscina, app->d,
        vicus_figurae(app->vicus),
        ZEPHYRUM, vicus_imago_fons, app->vicus, tp);
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

s32 principale (vacuum)
{
           Volumen* vol;
   VicusApplicatio  app;
             Vicus* v;
        ManusLudus* m;
               i32  secundus;

    piscina = piscina_generare_dynamicum("probatio_vicus_nexus",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_nexus");
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app.vicus;
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    scribere(m, ZEPHYRUM, "#next #prev #first #last");
    scribere(m, I, "#2 #9 #notae #nihil");
    paginam_mutare(app.d, CLAVIS_DEXTER);
    scribere(m, III, "     #notae #prev");
    paginam_mutare(app.d, CLAVIS_DEXTER);
    scribere(m, IV, "  #notae #first");
    paginam_mutare(app.d, CLAVIS_SINISTER);
    paginam_mutare(app.d, CLAVIS_SINISTER);
    CREDO_VERUM(pagina_est(vol, "1"));

    imprimere("\n--- I: '#2' - pagina id II ---\n");
    ictus(m, I, ZEPHYRUM);
    CREDO_VERUM(pagina_est(vol, "2"));
    CREDO_VERUM(cursor_est(v, ZEPHYRUM, ZEPHYRUM));

    imprimere("\n--- II: tag '#notae' - cyclus, cursor in tag ---\n");
    paginam_mutare(app.d, CLAVIS_SINISTER);
    ictus(m, I, VII);
    CREDO_VERUM(pagina_est(vol, "2"));
    CREDO_VERUM(cursor_est(v, III, V));
    ictus(m, III, VI);
    CREDO_VERUM(pagina_est(vol, "3"));
    CREDO_VERUM(cursor_est(v, IV, II));
    /* circulo: post ultimam prima */
    ictus(m, IV, III);
    CREDO_VERUM(pagina_est(vol, "1"));
    CREDO_VERUM(cursor_est(v, I, VI));

    imprimere("\n--- III: nihil inventum - nuntius ---\n");
    ictus(m, I, XIV);
    CREDO_VERUM(pagina_est(vol, "1"));
    /* tag solum in pagina visa: nulla ALIA */
    CREDO_VERUM(nuntius_est(v, "#nihil: nulla alia pagina"));
    ictus(m, I, III);
    CREDO_VERUM(pagina_est(vol, "1"));
    CREDO_VERUM(nuntius_est(v, "#9: nulla pagina"));

    imprimere("\n--- IV: #last #first #next #prev ---\n");
    ictus(m, ZEPHYRUM, XX);
    CREDO_VERUM(pagina_est(vol, "3"));
    ictus(m, IV, X);
    CREDO_VERUM(pagina_est(vol, "1"));
    ictus(m, ZEPHYRUM, I);
    CREDO_VERUM(pagina_est(vol, "2"));
    ictus(m, III, XIII);
    CREDO_VERUM(pagina_est(vol, "1"));
    /* '#prev' in prima: nihil (ut Ctrl+Shift+Sinister) */
    ictus(m, ZEPHYRUM, VII);
    CREDO_VERUM(pagina_est(vol, "1"));

    imprimere("\n--- V: ictus unus in latus non focatum ---\n");
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    dispensator_recomponere(app.d);
    ictus(m, I, ZEPHYRUM);
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_SINISTRUM));
    CREDO_VERUM(pagina_est(vol, "2"));

    imprimere("\n--- VI: nexus colore accentus secundi ---\n");
    (vacuum)vicus_pulsare(v);
    secundus =
        (i32)color_ad_pixelum(thema_color(COLOR_ACCENT_SECONDARY));
    CREDO_FALSUM(secundus == (i32)color_ad_pixelum(thema_color(
        COLOR_TEXT)));
    CREDO_VERUM(pixela_coloris(&app, III, V, XI, secundus) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(pixela_coloris(&app, III, ZEPHYRUM, V, secundus),
        ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
