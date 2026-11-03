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
#include "pictor_communio.h"
#include <stdio.h>
#include <string.h>

#define PICTOR_LATITUDO   (DC + XL)
#define PICTOR_ALTITUDO   CDLXXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA             II
/* communio-plan-5 C5: servus domus; -servus <url> pro probationibus */
#define SERVUS_DOMUS      "https://exemplar.golden.earth"
#define VIA_MAXIMA        DXII

/* in piscina testae: vivit usque ad finem (R3) */
nomen structura {
              Volumen* volumen;
          Dispensator* d;
       PictorMontatio* montatio;
       PictorCommunio* communio;
} PictorHospes;

interior vacuum
_pictorem_finire (
    vacuum* ctx)
{
    PictorHospes* h = (PictorHospes*)ctx;

    /* communio ante volumen (cliens in eo scribit) */
    pictor_communionem_claudere(h->communio);
    volumen_claudere(h->volumen);
}

/* argumentum post 'nomen' (aut praestitutum) */
interior constans character*
_optio (
                   s32   argc,
             character** argv,
    constans character*  titulus,
    constans character*  praestitutum)
{
    s32 i;

    per (i = I; i + I < argc; i++)
    {
        si (strcmp(argv[i], titulus) == ZEPHYRUM)
        {
            redde argv[i + I];
        }
    }
    redde praestitutum;
}

/* C4: tessera = via voluminis + ".tessera" (0600); fumus numquam
 * communicat; sine veredario (Android) nulla communio */
interior vacuum
_communionem_aperire (
    constans TestaAmbitus* a,
             PictorHospes* h,
                      b32  fumus)
{
             character  via[VIA_MAXIMA];
    constans character* volumen;

    si (fumus || !a->veredarius || !a->reactor)
    {
        redde;
    }
    volumen = _optio(a->argc, a->argv, "-volumen", "pictor.volumen");
    si (strlen(volumen) + VIII >= VIA_MAXIMA)
    {
        redde;
    }
    sprintf(via, "%s.tessera", volumen);
    h->communio = pictor_communionem_aperire(a->piscina, a->reactor,
        a->veredarius, h->volumen, via, _optio(a->argc, a->argv,
        "-servus", SERVUS_DOMUS), NIHIL, NIHIL);
    si (!h->communio || !h->montatio)
    {
        redde;
    }
    /* plan-5 T6/T7: menu, Shared pictures, attributiones, inscriptio;
     * claviatura systematis (iOS) */
    h->montatio->actiones_ctx.claviaturam_ponere =
        a->claviaturam_ponere;
    h->montatio->actiones_ctx.claviatura_ctx = a->claviatura_ctx;
    pictor_communionem_ligare(h->communio, h->montatio, h->d,
        a->machina);
    (vacuum)pictor_communio_inscriptionem_resumere(h->communio);
    pictor_communio_incipere(h->communio);
    (vacuum)pictor_salutationem_ponere(h->montatio);
}

b32
applicatio_initium (
    constans TestaAmbitus* a,
              TestaForma* forma)
{
    InternamentumChorda* intern;
                Volumen* vol;
       PictorApplicatio* app;
           PictorHospes* h;
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
    h = (PictorHospes*)piscina_allocare(a->piscina,
        magnitudo(PictorHospes));
    si (   !app || !h
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
    /* communio-plan-5 T5: communio intra pictorem */
    h->volumen   = vol;
    h->d         = app->d;
    h->montatio  = &app->montatio;
    h->communio  = NIHIL;
    _communionem_aperire(a, h, fumus);

    forma->titulus    = "pictor";
    forma->latitudo   = latitudo;
    forma->altitudo   = altitudo;
    forma->scala      = SCALA;
    forma->d          = app->d;
    forma->figurae    = app->figurae;
    forma->thema      = ZEPHYRUM;
    forma->fons       = pictor_imago_fons;
    forma->fons_ctx   = &app->montatio.figurae_ctx;
    forma->reactor    = a->reactor;
    forma->finis      = _pictorem_finire;
    forma->finis_ctx  = h;
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
