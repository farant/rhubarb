#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "scriba_applicatio.h"
#include "tabula_characterum.h"
#include <stdio.h>

s32
principale (vacuum)
{
    Piscina* p;
    InternamentumChorda* in;
    Volumen* vol;
    ScribaApplicatio app;
    TabulaCharacterum* folium;

    p = piscina_generare_dynamicum("sonda", 65536);
    in = internamentum_creare(p);
    vol = volumen_temporarium(p, "sonda_scriba_app");
    si (!vol)
    {
        imprimere("volumen deficit\n");
        redde 1;
    }
    si (!scriba_applicatio_aedificare(&app, p, in, vol, 600, 400))
    {
        imprimere("aedificare deficit\n");
        volumen_claudere(vol);
        redde 1;
    }
    folium = scriba_documentum_tabula(app.doc);
    imprimere("folium novum (superficies 600x400): %u x %u (caput dicit 68 x 56)\n",
        (insignatus integer)folium->latitudo, (insignatus integer)folium->altitudo);
    volumen_claudere(vol);
    redde 0;
}
