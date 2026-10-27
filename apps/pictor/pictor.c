/* pictor.c - pictor: editor rasterum (P3: tabula + penicillus)
 *
 * Compositio communis (pictor_applicatio) + forma pro testa (testa.h:
 * principale a testa - macOS fenestra, Android NativeActivity);
 * gemellus pictor_terminalis.c (A4 moduli 013). -fumus: volumen
 * temporarium, XXX quadra (testa), exitus; -imago <via> (testa).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "testa.h"
#include "pictor_applicatio.h"
#include <stdio.h>

#define PICTOR_LATITUDO   (DC + XL)
#define PICTOR_ALTITUDO   CDLXXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA             II

interior vacuum
_pictorem_finire (
    vacuum* ctx)
{
    volumen_claudere((Volumen*)ctx);
}

b32
applicatio_initium (
    constans TestaAmbitus* a,
              TestaForma* forma)
{
    InternamentumChorda* intern;
                Volumen* vol;
       PictorApplicatio* app;
                    b32  fumus;
                    i32  latitudo;
                    i32  altitudo;

    intern = internamentum_creare(a->piscina);
    thema_initiare();
    vol = pictor_volumen_aperire(a->piscina, a->argc, a->argv, &fumus);
    si (!vol)
    {
        fprintf(stderr, "pictor: volumen aperiri non potuit\n");
        redde FALSUM;
    }
    /* superficies a testa imposita (Android: area contenti): documentum
     * NOVUM ei aptatur - pertinet aspectui ubi natum est (D4); macOS:
     * fenestra nostra, 640 x 480 */
    latitudo = PICTOR_LATITUDO;
    altitudo = PICTOR_ALTITUDO;
    si (   a->superficies_latitudo >= SCALA
        && a->superficies_altitudo >= SCALA)
    {
        latitudo = a->superficies_latitudo / SCALA;
        altitudo = a->superficies_altitudo / SCALA;
    }
    /* R3: app in piscina - fons_ctx (&montatio.figurae_ctx) ultra
     * initium vivit */
    app = (PictorApplicatio*)piscina_allocare(a->piscina,
        magnitudo(PictorApplicatio));
    si (   !app
        || !pictor_applicatio_aedificare(app, a->piscina, intern, vol,
               latitudo, altitudo))
    {
        redde FALSUM;
    }
    /* pinacotheca D8: pictura ultima (solum applicatio fenestrae;
     * terminalis non - spec D11) */
    (vacuum)pictor_picturam_restituere(app);
    /* L5: Cmd+V imaginem e clipboard (servitium testae) in stratum
     * novum glutinat */
    app->montatio.actiones_ctx.imago_capere  = a->imago_glutinata;
    app->montatio.actiones_ctx.imago_ctx     = a->imago_ctx;
    /* testa-ios C1b: electio imaginis (telephonum) - quadratum
     * imaginis solum ubi testa servitium praebet; photographia electa
     * ut depositio redit */
    si (a->imaginem_eligere)
    {
        app->montatio.actiones_ctx.imaginem_eligere      =
            a->imaginem_eligere;
        app->montatio.actiones_ctx.imaginem_eligere_ctx  =
            a->imaginem_eligere_ctx;
        app->montatio.compositio.imago_eligibilis         = VERUM;
    }
    /* pinacotheca D1: quadratum ⋯ (menu picturarum) in applicatione
     * pictor ubique - vicus et terminalis id non ponunt */
    app->montatio.compositio.elenchus_praesto = VERUM;

    forma->titulus    = "pictor";
    forma->latitudo   = latitudo;
    forma->altitudo   = altitudo;
    forma->scala      = SCALA;
    forma->d          = app->d;
    forma->figurae    = app->figurae;
    forma->thema      = ZEPHYRUM;
    forma->fons       = pictor_imago_fons;
    forma->fons_ctx   = &app->montatio.figurae_ctx;
    forma->finis      = _pictorem_finire;
    forma->finis_ctx  = vol;
    redde VERUM;
}

/* introitus: testa (testa.h) - macOS fenestra et ansa; Android
 * ingressus suus (ANativeActivity_onCreate) */
s32
principale (
      integer   argc,
    character** argv)
{
    redde testa_principale(argc, argv);
}
