/* probatio_vicus_iussa.c - iussa in pagina scribae (vicus-latera S3b)
 *
 * Compositio VERA (vicus_applicatio, tabula 3 = scriba | scriba).
 * Linea III sinistri: "$dies $foo $dies(x)". I: '$dies' colore
 * accentus pingitur, '$foo' (ignotum) non. II: ictus in '$dies' id
 * dato hodierno substituit (consumens, forma "MM/DD/YYYY"), cursor
 * post datum; latus alterum post pulsum idem videt. III: 'u' signum
 * restituit (commissio una). IV: ictus in '$foo' cursorem solum
 * ponit. V: '$dies(x)' (error: argumenta) nihil mutat. */
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
#include "tabula_characterum.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "scriba_documentum.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINEA III

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior ScribaMontatio*
visus (
    Vicus* v,
      i32  quod)
{
    redde (ScribaMontatio*)vicus_latus(vicus_activa(v), quod)->montatio;
}

/* punctum schirmi cellulae (linea, columna) paginae sinistrae */
interior vacuum
ictus (
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

/* initium lineae folii laboris est textus? */
interior b32
linea_est (
        ScribaMontatio* sm,
                   s32  linea,
    constans character* textus)
{
    constans TabulaCharacterum* t;

    t = &sm->actiones_ctx.laboris;
    redde memcmp(t->cellulae + (i32)linea * t->latitudo, textus,
        strlen(textus)) == ZEPHYRUM;
}

interior s32
attributum (
                 Vicus* v,
    constans character* titulus)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;
            s32  n;

    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    a = insula_ramus_attributum(&r, INSULA_EPHEMERA, titulus);
    si (!a || !chorda_ut_s32(*a, &n))
    {
        redde -I;
    }
    redde n;
}

/* pixela coloris dati in cellulis [a, b) lineae paginae sinistrae */
interior i32
pixela_coloris (
    TabulaPixelorum* tp,
                s32  linea,
                s32  a,
                s32  b,
                i32  color)
{
    s32 x;
    s32 y;
    i32 n;

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
    ScribaMontatio* sin;
    ScribaMontatio* dex;
         character  hodie[XXXII];
            time_t  nunc;
     structura tm* tm;

    piscina = piscina_generare_dynamicum("probatio_vicus_iussa",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_iussa");
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app.vicus;
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    sin = visus(v, VICUS_SINISTRUM);
    dex = visus(v, VICUS_DEXTRUM);
    CREDO_NON_NIHIL(sin->actiones_ctx.iussa);
    CREDO_VERUM(sin->actiones_ctx.iussa == dex->actiones_ctx.iussa);
    ictus(m, LINEA, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$dies $foo $dies(x)"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));

    imprimere("\n--- I: '$dies' colore accentus, '$foo' non ---\n");
    {
        TabulaPixelorum* tp;
          LudusFenestra* lf;
                    i32  accentus;

        tp = tabula_pixelorum_creare_nuda(piscina, CDLXXX, CDLXXX
            + VICUS_ALTITUDO_TABULARUM);
        lf = ludus_fenestra_creare(piscina, app.d, vicus_figurae(v),
            ZEPHYRUM, vicus_imago_fons, v, tp);
        CREDO_NON_NIHIL(lf);
        ludus_quadrum(lf, M);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf,
            "build/probatio_vicus_iussa.png"));
        accentus = (i32)color_ad_pixelum(thema_color(
            COLOR_ACCENT_PRIMARY));
        CREDO_VERUM(pixela_coloris(tp, LINEA, ZEPHYRUM, V, accentus)
            > ZEPHYRUM);
        CREDO_AEQUALIS_I32(pixela_coloris(tp, LINEA, VI, X, accentus),
            ZEPHYRUM);
        CREDO_VERUM(pixela_coloris(tp, LINEA, XI, XIX, accentus)
            > ZEPHYRUM);
    }

    imprimere("\n--- II: ictus in '$dies' - datum hodiernum ---\n");
    nunc  = time(NIHIL);
    tm    = localtime(&nunc);
    sprintf(hodie, "%02d/%02d/%04d $foo", tm->tm_mon + I, tm->tm_mday,
        tm->tm_year + MCM);
    ictus(m, LINEA, II);
    CREDO_VERUM(linea_est(sin, LINEA, hodie));
    CREDO_AEQUALIS_S32(attributum(v, "cursor_linea"), LINEA);
    CREDO_AEQUALIS_S32(attributum(v, "cursor_columna"), X);
    /* commissum: latus alterum post pulsum */
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(linea_est(dex, LINEA, hodie));

    imprimere("\n--- III: 'u' signum restituit ---\n");
    CREDO_VERUM(manus_ludus_scribere(m, "u"));
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));

    imprimere("\n--- IV: '$foo' ignotum - cursor solus ---\n");
    ictus(m, LINEA, VII);
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));
    CREDO_AEQUALIS_S32(attributum(v, "cursor_columna"), VII);

    imprimere("\n--- V: '$dies(x)' - error, nihil mutatur ---\n");
    ictus(m, LINEA, XIII);
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
