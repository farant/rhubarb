/* probatio_briar_fasciculum.c - '-app' (spec par. 4.8) SINE fenestra
 * et SINE decodificatore: consilium ex nexu fixorum (opus III plani 7),
 * scriptor super Imago ficta (opus IV). */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_fasciculum.h"
#include "briar_nexus.h"
#include "chorda.h"
#include "fasciculum.h"
#include "filum.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* FIXA =
    "briar/probationes/fixa/thistle/";

/* fixum legere, nexum texere, consilium reddere */
interior b32
_consilium (
                     Piscina* piscina,
         InternamentumChorda* intern,
          constans character* titulus,
          constans character* via_icon,
    BriarFasciculumConsilium* c,
                      chorda* causa,
                         i32* linea)
{
    constans character* via = chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis(FIXA, piscina),
        chorda_ex_literis(titulus, piscina), piscina), piscina);
                 chorda textus = filum_legere_totum(via, piscina);
          MateriaNodus* doc;
                   Xar* nexus;

    *linea = ZEPHYRUM;
    si (textus.mensura == ZEPHYRUM)
    {
        *causa = chorda_ex_literis("fixum non lectum", piscina);
        redde FALSUM;
    }
    doc    = briar_arbor_parsare(piscina,
        (constans character*)textus.datum, (i32)textus.mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    redde briar_fasciculum_consilium(piscina, nexus, via, via_icon, c,
        causa, linea);
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_briar_fasciculum",
        (memoriae_index)4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans consilium: salve_vitreum ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern,
            "salve_vitreum.thistle", NIHIL, &c, &causa, &linea));
        /* '_' in '-': identitas characteres suos non habet */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.rhubarb.briar.salve-vitreum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "salve");
        CREDO_CHORDA_VACUA(c.versio);
        CREDO_CHORDA_VACUA(c.via_icon);
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_app,
            "briar/probationes/fixa/thistle/salve_vitreum.app");
        CREDO_AEQUALIS_I32(c.linea_briar, ZEPHYRUM);
    }

    imprimere("\n--- Probans consilium: salve (sine fenestra) ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "salve.thistle", NIHIL,
            &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.rhubarb.briar.salve");
        /* nomen e plagula, nulla fenestra */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "salve");
    }

    imprimere("\n--- Probans consilium: briar vestitum ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            NIHIL, &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.exemplum.vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "Vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.versio, "2.1");
        /* icon attributi ad directorium PLAGULAE resolvitur */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_icon,
            "briar/probationes/fixa/thistle/signa/vestitum.png");
        CREDO_AEQUALIS_I32(c.linea_briar, VIII);

        /* vexillum -icon attributum vincit, et intactum manet */
        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            "alibi/icon.png", &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_icon, "alibi/icon.png");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.exemplum.vestitum");
    }

    imprimere("\n--- Probans recusationes cum linea ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_identitas_prava.thistle", NIHIL, &c, &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, IV);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("vitium_malum", piscina));

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_briar_duplex.thistle", NIHIL, &c, &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, VI);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("linea 4", piscina));
    }

    imprimere("\n--- Probans scriptorem ---\n");
    {
        constans character* area =
            "/tmp/probatio_briar_fasciculum";
        constans character* exsecutabile =
            "/tmp/probatio_briar_fasciculum/bin/salve";
        constans character* domus =
            "/tmp/probatio_briar_fasciculum/domus";
        constans character* app =
            "/tmp/probatio_briar_fasciculum/Salve.app";
        constans character* aliena =
            "/tmp/probatio_briar_fasciculum/Alienum.app";
        constans character* planum =
            "/tmp/probatio_briar_fasciculum/Planum.app";
        constans character* stala =
            "/tmp/probatio_briar_fasciculum/Salve.app/Contents/"
            "Resources/vetus.txt";
        BriarFasciculumConsilium c;
                      Fasciculum lectus;
                FasciculumStatus status;
                          chorda sedes;
                          chorda causa;
                          chorda icns;
                           Imago imago;
                             i32 k;

        (vacuum)filum_arborem_delere(area);
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            "/tmp/probatio_briar_fasciculum/bin"));
        CREDO_VERUM(filum_scribere_literis(exsecutabile,
            "#!/bin/sh\n"));
        CREDO_VERUM(filum_modum_ponere(exsecutabile, 0755));

        /* imago ficta LXIV x LXIV: nullus decodificator in porta */
        imago.latitudo = LXIV;
        imago.altitudo = LXIV;
        imago.pixela    = (i8*)piscina_allocare(piscina,
            (memoriae_index)(LXIV * LXIV * IV));
        per (k = ZEPHYRUM; k < (i32)(LXIV * LXIV * IV); k++)
        {
            imago.pixela[k] = (i8)0x7F;
        }

        memset(&c, ZEPHYRUM, magnitudo(c));
        c.identitas  = chorda_ex_literis("org.rhubarb.briar.salve",
            piscina);
        c.titulus  = chorda_ex_literis("Salve", piscina);
        c.versio   = chorda_ex_literis("2.1", piscina);
        c.via_app  = chorda_ex_literis(app, piscina);

        /* I. scriptio prima */
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_VACUA(causa);
        CREDO_VERUM(fasciculum_legere(app, &lectus, &status, &sedes,
            piscina, intern));
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.identitas,
            "org.rhubarb.briar.salve");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.titulus, "Salve");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.versio, "2.1");
        CREDO_VERUM(filum_existit(
            "/tmp/probatio_briar_fasciculum/Salve.app/Contents/MacOS/"
            "salve"));
        icns = filum_legere_totum(
            "/tmp/probatio_briar_fasciculum/Salve.app/Contents/"
            "Resources/salve.icns", piscina);
        CREDO_VERUM(icns.mensura > VIII);
        CREDO_VERUM(icns.mensura > VIII
            && memcmp(icns.datum, "icns", (size_t)IV) == ZEPHYRUM);

        /* II. PROPRIUS reponitur: plagula stala evanescit */
        CREDO_VERUM(filum_scribere_literis(stala, "vetus"));
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_FALSUM(filum_existit(stala));
        CREDO_VERUM(fasciculum_legere(app, &lectus, &status, &sedes,
            piscina, intern));

        /* III. ALIENUS recusatur et intactus manet */
        c.identitas  = chorda_ex_literis("org.aliud.alienum", piscina);
        c.via_app    = chorda_ex_literis(aliena, piscina);
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        c.identitas  = chorda_ex_literis("org.rhubarb.briar.salve",
            piscina);
        CREDO_FALSUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("alienus", piscina));
        CREDO_VERUM(fasciculum_legere(aliena, &lectus, &status, &sedes,
            piscina, intern));
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.identitas,
            "org.aliud.alienum");

        /* IV. plagula quae fasciculus non est: recusata, intacta */
        CREDO_VERUM(filum_scribere_literis(planum, "non fasciculus"));
        c.via_app = chorda_ex_literis(planum, piscina);
        CREDO_FALSUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("neque fasciculus", piscina));
        CREDO_VERUM(filum_existit(planum)
            && !filum_directorium_existit(planum));

        (vacuum)filum_arborem_delere(area);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
