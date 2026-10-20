/* componens_r1.c - sonda: spatium et claudit per STML iter perduntur? */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include <stdio.h>

integer
principale (vacuum)
{
               Piscina* piscina;
    InternamentumChorda* intern;
             Componens* radix;
             Componens* mons;
             Componens* folium;
             Componens* lectum;
                 chorda  s;
                 chorda  sp;
                 chorda  idf;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    intern  = internamentum_creare(piscina);
    radix   = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    mons    = componens_creare(piscina, intern, "mons", PARTES_NULLUM);
    folium  = componens_creare(piscina, intern, "folium", PARTES_BOTTONE);
    componens_ponere_spatium(mons, "sinister");
    folium->claudit = VERUM;
    componens_addere_liberum(radix, mons);
    componens_addere_liberum(mons, folium);

    s = componens_scribere_stml(radix, piscina, intern, VERUM);
    imprimere("%.*s\n", (integer)s.mensura, (character*)s.datum);
    lectum = componens_legere_stml(chorda_ut_cstr(s, piscina), piscina,
        intern);
    sp  = chorda_ex_literis("sinister", piscina);
    idf = chorda_ex_literis("folium", piscina);
    imprimere("ante: in_spatio=%s claudit=%d\n",
        componens_invenire_in_spatio(radix, sp, idf) ? "inventum" : "NIHIL",
        (integer)folium->claudit);
    imprimere("post: in_spatio=%s claudit=%d spatium_mensura=%d\n",
        componens_invenire_in_spatio(lectum, sp, idf) ? "inventum" : "NIHIL",
        (integer)componens_liberum(componens_liberum(lectum, 0), 0)->claudit,
        (integer)componens_liberum(lectum, 0)->spatium.mensura);
    imprimere("aequalis(ante, post) = %d\n",
        (integer)componens_aequalis(radix, lectum));
    redde 0;
}
