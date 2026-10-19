/* probatio_ludus_imago.c - imagines quadrorum (screenshots) sine
 * fenestra
 *
 * I: tabula_pixelorum_in_imaginem - octeti R, G, B, A exacti (per
 * translationes, non ordinem octetorum). II: PNG scriptum et relectum
 * idem. III: ludus_fenestra_imaginem_scribere - ante quadrum FALSUM;
 * pictor in tabula NUDA (nulla fenestra) pictus, PNG in
 * build/probatio_ludus_imago_pictor.png (oculis inspiciendum). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "imago.h"
#include "imago_png.h"
#include "tabula_pixelorum.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "pictor_applicatio.h"
#include "pictor_figurae.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_PICTORIS "build/probatio_ludus_imago_pictor.png"
#define VIA_PROBAE   "build/probatio_ludus_imago_quattuor.png"

interior b32
octeti_sunt (
    constans Imago* im,
               i32  index,
               i32  r,
               i32  g,
               i32  b,
               i32  a)
{
    constans i8* p;

    p = im->pixela + index * IV;
    redde (i32)p[ZEPHYRUM] == r && (i32)p[I] == g && (i32)p[II] == b
        && (i32)p[III] == a;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
            Piscina* piscina;
    TabulaPixelorum* t;
              Imago  im;
       ImagoFructus  lecta;
         PngFructus  scripta;

    piscina  = c->piscina;
    t        = tabula_pixelorum_creare_nuda(piscina, II, II);
    CREDO_NON_NIHIL(t);
    tabula_pixelorum_ponere_pixelum(t, ZEPHYRUM, ZEPHYRUM,
        RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
    tabula_pixelorum_ponere_pixelum(t, I, ZEPHYRUM,
        RGB(ZEPHYRUM, CCLV, ZEPHYRUM));
    tabula_pixelorum_ponere_pixelum(t, ZEPHYRUM, I,
        RGB(ZEPHYRUM, ZEPHYRUM, CCLV));
    tabula_pixelorum_ponere_pixelum(t, I, I, RGBA(X, XX, XXX, XL));
    CREDO_VERUM(tabula_pixelorum_in_imaginem(t, piscina, &im));
    CREDO_AEQUALIS_I32(im.latitudo, II);
    CREDO_AEQUALIS_I32(im.altitudo, II);
    CREDO_VERUM(octeti_sunt(&im, ZEPHYRUM, CCLV, ZEPHYRUM, ZEPHYRUM,
                            CCLV));
    CREDO_VERUM(octeti_sunt(&im, I, ZEPHYRUM, CCLV, ZEPHYRUM, CCLV));
    CREDO_VERUM(octeti_sunt(&im, II, ZEPHYRUM, ZEPHYRUM, CCLV, CCLV));
    CREDO_VERUM(octeti_sunt(&im, III, X, XX, XXX, XL));
    CREDO_FALSUM(tabula_pixelorum_in_imaginem(NIHIL, piscina, &im));

    CREDO_GRADUS("II: PNG scriptum et relectum");
    scripta = imago_png_scribere(&im, VIA_PROBAE, piscina);
    CREDO_VERUM(scripta.successus);
    lecta = imago_caricare_ex_file(VIA_PROBAE, piscina);
    CREDO_VERUM(lecta.successus);
    CREDO_AEQUALIS_I32(lecta.imago.latitudo, II);
    CREDO_AEQUALIS_I32(lecta.imago.altitudo, II);
    CREDO_VERUM(memcmp(lecta.imago.pixela, im.pixela, XVI) == ZEPHYRUM);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorApplicatio  app;
        TabulaPixelorum* tabula;
          LudusFenestra* lf;
           ImagoFructus  lecta;
                    i32  i;
                    i32  diversi;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_ludus_imago");
    CREDO_NON_NIHIL(vol);
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        DC + XL, CDLXXX));
    tabula = tabula_pixelorum_creare_nuda(piscina, DC + XL, CDLXXX);
    lf = ludus_fenestra_creare(piscina, app.d, app.figurae, ZEPHYRUM,
        pictor_imago_fons, &app.montatio.figurae_ctx, tabula);
    CREDO_NON_NIHIL(lf);
    /* ante quadrum nihil est quod scribatur */
    CREDO_FALSUM(ludus_fenestra_imaginem_scribere(lf, VIA_PICTORIS));
    ludus_quadrum(lf, ZEPHYRUM);
    ludus_quadrum(lf, C);
    CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf, VIA_PICTORIS));
    /* scriptio fracta non tacetur */
    CREDO_FALSUM(ludus_fenestra_imaginem_scribere(lf,
        "/nusquam/directorium/imago.png"));
    lecta = imago_caricare_ex_file(VIA_PICTORIS, piscina);
    CREDO_VERUM(lecta.successus);
    CREDO_AEQUALIS_I32(lecta.imago.latitudo, DC + XL);
    CREDO_AEQUALIS_I32(lecta.imago.altitudo, CDLXXX);
    /* aliquid pictum: pixela a primo diversa */
    diversi = ZEPHYRUM;
    per (i = I; i < (DC + XL) * CDLXXX; i++)
    {
        si (memcmp(lecta.imago.pixela + i * IV, lecta.imago.pixela,
                   IV) != ZEPHYRUM)
        {
            diversi++;
        }
    }
    CREDO_VERUM(diversi > M);
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I: tabula in imaginem", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "III: pictor sine fenestra", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("ludus_imago", SECTIONES);
}
