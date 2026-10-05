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
    /* 013 B1: magnitudo initialis nuntiata (tabula CC x C) */
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA,
        "superficies_latitudo"));
    si (insula_attributum(repo, INSULA_EPHEMERA,
        "superficies_latitudo"))
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(*insula_attributum(repo,
            INSULA_EPHEMERA, "superficies_latitudo"), "200");
        CREDO_CHORDA_AEQUALIS_LITERIS(*insula_attributum(repo,
            INSULA_EPHEMERA, "superficies_altitudo"), "100");
    }
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

    imprimere("\n--- 013 B3b: tabula ad fenestram (scala servata)"
              " ---\n");
    {
        TabulaPixelorum* t2 = tabula_pixelorum_creare_nuda(piscina, CC,
            C);
                    i32* prima;

        t2->scala  = 2.0f;   /* fenestra CD x CC punctorum */
        prima      = t2->pixela;
        CREDO_VERUM(tabula_pixelorum_ad_fenestram(t2, piscina, CCC,
            CLX));
        CREDO_AEQUALIS_I32(t2->latitudo, CL);
        CREDO_AEQUALIS_I32(t2->altitudo, LXXX);
        CREDO_VERUM(t2->scala == 2.0f);
        CREDO_AEQUALIS_I32(t2->fenestra_latitudo, CCC);
        /* minor: memoria eadem */
        CREDO_VERUM(t2->pixela == prima);
        CREDO_VERUM(tabula_pixelorum_ad_fenestram(t2, piscina, DC, CD));
        CREDO_AEQUALIS_I32(t2->latitudo, CCC);
        CREDO_AEQUALIS_I32(t2->altitudo, CC);
        /* maior: memoria nova */
        CREDO_VERUM(t2->pixela != prima);
        CREDO_VERUM(t2->capacitas >= CCC * CC);
        /* pixelum ultimum scribibile (memoria vera) */
        tabula_pixelorum_ponere_pixelum(t2, CCC - I, CC - I, VII);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t2,
            CCC - I, CC - I), VII);
    }

    imprimere("\n--- 013 B3b: glutinum - eventus in pixela nostra"
              " ---\n");
    {
        TabulaPixelorum* t3 = tabula_pixelorum_creare_nuda(piscina, CC,
            C);
          LudusFenestra* lf_aptata;
                 chorda* a;

        t3->scala = 2.0f;
        lf_aptata = ludus_fenestra_creare(piscina, d, figurae, ZEPHYRUM,
            NIHIL,
            NIHIL, t3);
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
        /* puncta fenestrae */
        e.datum.mutare_magnitudinem.latitudo = D;
        e.datum.mutare_magnitudinem.altitudo = CCC;
        ludus_fenestra_tractare(lf_aptata, &e, M + CC);
        CREDO_AEQUALIS_I32(t3->latitudo, CCL);
        CREDO_AEQUALIS_I32(t3->altitudo, CL);
        a = insula_attributum(repo, INSULA_EPHEMERA,
            "superficies_latitudo");
        CREDO_NON_NIHIL(a);
        si (a)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(*a, "250");
        }
        a = insula_attributum(repo, INSULA_EPHEMERA,
            "superficies_altitudo");
        CREDO_NON_NIHIL(a);
        si (a)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(*a, "150");
        }
        /* quadrum post mutationem: tabula nova tota pingitur */
        ludus_quadrum(lf_aptata, M + CCC);
        CREDO_VERUM(lf_aptata->mensurae.quadra >= I);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
