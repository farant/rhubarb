/* probatio_ludus_fenestra.c - quadrum unum sine fenestra: pulsus ->
 * pingere -> delineare; mensurae */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "color.h"
#include "thema.h"
#include "fenestra.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
                                    Piscina* piscina;
                        InternamentumChorda* intern;
                         InsulaRepositorium* repo;
                             ActioRegistrum* reg;
                                  ToyStatus  toy;
                                Dispensator* d;
                            FiguraRegistrum* figurae;
                            TabulaPixelorum* t;
                              LudusFenestra* lf;
                                    Eventus  e;
                                        i32  n0;
                                        i32  n1;

    piscina = piscina_generare_dynamicum("probatio_ludus_fenestra",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    reg  = actio_registrum_creare(piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    figurae = figura_registrum_creare(piscina);
    CREDO_VERUM(figura_registrare(figurae, PARTES_BOTTONE, ZEPHYRUM,
                                  figura_finium, NIHIL));
    t  = tabula_pixelorum_creare_nuda(piscina, CC, C);
    lf = ludus_fenestra_creare(piscina, d, figurae, ZEPHYRUM, NIHIL,
        NIHIL,
                               t);
    CREDO_NON_NIHIL(lf);

    imprimere("\n--- Quadrum: pulsus, pingere, delineare ---\n");
    n0 = toy.compositiones;
    ludus_quadrum(lf, M);
    CREDO_AEQUALIS_I32(lf->mensurae.quadra, I);
    /* pulsus recomposuit */
    CREDO_VERUM(toy.compositiones > n0);
    /* coetus x3 + rect */
    CREDO_VERUM(mandata_numerus(lf->mandata) >= III);
    /* b1 (10,10,50,20) fines vacui colore COLOR_BORDER: angulus
     * (10,10) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, X, X),
        color_ad_pixelum(thema_color(COLOR_BORDER)));
        CREDO_VERUM(lf->mensurae.ms_quadri_maximum >= ZEPHYRUM);
    n1 = mandata_numerus(lf->mandata);

    imprimere("\n--- Eventus sine tempore stampatur; quadrum II ---\n");
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x  = XXXV;
    e.datum.mus.y  = XX;
    ludus_fenestra_tractare(lf, &e, M + L);
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "numerus"));
    ludus_quadrum(lf, M + C);
    CREDO_AEQUALIS_I32(lf->mensurae.quadra, II);
    /* piscina quadri vacatur: mandata nova, non accumulata */
    CREDO_AEQUALIS_I32(mandata_numerus(lf->mandata), n1);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
