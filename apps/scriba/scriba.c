/* scriba.c - scriba: pagina vim (folium LXVIII x LVI)
 *
 * Compositio communis (scriba_applicatio) + forma pro testa (testa.h);
 * gemellus scriba_terminalis.c (scriba-plan S3). -fumus: volumen
 * temporarium, XXX quadra (testa), exitus; -volumen <via>
 * (ordinarie scriba.volumen); -imago <via> (testa).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "testa.h"
#include "scriba_applicatio.h"
#include <stdio.h>

/* LXXX x LX cellulae: folium cum margine et linea status */
#define SCRIBA_LATITUDO   CDLXXX
#define SCRIBA_ALTITUDO   CDLXXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA             II

interior vacuum
_scribam_finire (
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
       ScribaApplicatio* app;
                    b32  fumus;

    intern = internamentum_creare(a->piscina);
    thema_initiare();
    vol = scriba_volumen_aperire(a->piscina, a->argc, a->argv, &fumus);
    si (!vol)
    {
        fprintf(stderr, "scriba: volumen aperiri non potuit\n");
        redde FALSUM;
    }
    app = (ScribaApplicatio*)piscina_allocare(a->piscina,
        magnitudo(ScribaApplicatio));
    si (   !app
        || !scriba_applicatio_aedificare(app, a->piscina, intern, vol,
               SCRIBA_LATITUDO, SCRIBA_ALTITUDO))
    {
        redde FALSUM;
    }
    forma->titulus    = "scriba";
    forma->latitudo   = SCRIBA_LATITUDO;
    forma->altitudo   = SCRIBA_ALTITUDO;
    forma->scala      = SCALA;
    forma->d          = app->d;
    forma->figurae    = app->figurae;
    forma->thema      = ZEPHYRUM;
    forma->finis      = _scribam_finire;
    forma->finis_ctx  = vol;
    redde VERUM;
}
