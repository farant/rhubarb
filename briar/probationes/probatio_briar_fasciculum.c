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

/* via intra aream sectionis (credo_area): area + "/" + reliquum */
interior constans character*
_in_area (
               Piscina* piscina,
    constans character* area,
    constans character* reliquum)
{
    redde chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(area,
        piscina), chorda_ex_literis(reliquum, piscina), piscina),
        piscina);
}


/* ==================================================
 * PROBARE: consilium: salve_vitreum
 * ================================================== */

interior vacuum
_vitreum_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    InternamentumChorda* intern;

    /* internamentum sectionis proprium (olim unum principalis;
     * nulla fixura communis) */
    intern = internamentum_creare(piscina);
    {
        BriarFasciculumConsilium consilium;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern,
            "salve_vitreum.thistle", NIHIL, &consilium, &causa,
            &linea));
        /* '_' in '-': identitas characteres suos non habet */
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.identitas,
            "org.rhubarb.briar.salve-vitreum");
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.titulus, "salve");
        CREDO_CHORDA_VACUA(consilium.versio);
        CREDO_CHORDA_VACUA(consilium.via_icon);
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.via_app,
            "briar/probationes/fixa/thistle/salve_vitreum.app");
        CREDO_AEQUALIS_I32(consilium.linea_briar, ZEPHYRUM);
    }
}


/* ==================================================
 * PROBARE: consilium: salve (sine fenestra)
 * ================================================== */

interior vacuum
_salve_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    InternamentumChorda* intern;

    /* internamentum sectionis proprium (olim unum principalis;
     * nulla fixura communis) */
    intern = internamentum_creare(piscina);
    {
        BriarFasciculumConsilium consilium;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "salve.thistle", NIHIL,
            &consilium, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.identitas,
            "org.rhubarb.briar.salve");
        /* nomen e plagula, nulla fenestra */
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.titulus, "salve");
    }
}


/* ==================================================
 * PROBARE: consilium: briar vestitum
 * ================================================== */

interior vacuum
_vestitum_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    InternamentumChorda* intern;

    /* internamentum sectionis proprium (olim unum principalis;
     * nulla fixura communis) */
    intern = internamentum_creare(piscina);
    {
        BriarFasciculumConsilium consilium;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            NIHIL, &consilium, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.identitas,
            "org.exemplum.vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.titulus, "Vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.versio, "2.1");
        /* icon attributi ad directorium PLAGULAE resolvitur */
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.via_icon,
            "briar/probationes/fixa/thistle/signa/vestitum.png");
        CREDO_AEQUALIS_I32(consilium.linea_briar, VIII);

        /* vexillum -icon attributum vincit, et intactum manet */
        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            "alibi/icon.png", &consilium, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.via_icon,
            "alibi/icon.png");
        CREDO_CHORDA_AEQUALIS_LITERIS(consilium.identitas,
            "org.exemplum.vestitum");
    }
}


/* ==================================================
 * PROBARE: recusationes cum linea
 * ================================================== */

interior vacuum
_recusationes_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    InternamentumChorda* intern;

    /* internamentum sectionis proprium (olim unum principalis;
     * nulla fixura communis) */
    intern = internamentum_creare(piscina);
    {
        BriarFasciculumConsilium consilium;
                          chorda causa;
                             i32 linea;

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_identitas_prava.thistle", NIHIL, &consilium,
            &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, IV);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("vitium_malum", piscina));

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_briar_duplex.thistle", NIHIL, &consilium,
            &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, VI);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("linea 4", piscina));
    }
}


/* ==================================================
 * PROBARE: scriptor
 * ================================================== */

interior vacuum
_scriptorem_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    InternamentumChorda* intern;
     constans character* area;

    /* area sectionis propria (credo_area; olim /tmp fixum, cursibus
     * concurrentibus communis), post sectionem deleta */
    area = credo_area(c);
    si (area == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    intern = internamentum_creare(piscina);
    {
              constans character* exsecutabile;
              constans character* domus;
              constans character* app;
              constans character* aliena;
              constans character* planum;
              constans character* stala;
        BriarFasciculumConsilium  consilium;
                      Fasciculum  lectus;
                FasciculumStatus  status;
                          chorda  sedes;
                          chorda  causa;
                          chorda  icns;
                           Imago  imago;
                             i32  k;

        exsecutabile  = _in_area(piscina, area, "/bin/salve");
        domus         = _in_area(piscina, area, "/domus");
        app           = _in_area(piscina, area, "/Salve.app");
        aliena        = _in_area(piscina, area, "/Alienum.app");
        planum        = _in_area(piscina, area, "/Planum.app");
        stala         = _in_area(piscina, area,
            "/Salve.app/Contents/Resources/vetus.txt");
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            _in_area(piscina, area, "/bin")));
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

        memset(&consilium, ZEPHYRUM, magnitudo(consilium));
        consilium.identitas  =
            chorda_ex_literis("org.rhubarb.briar.salve",
            piscina);
        consilium.titulus  = chorda_ex_literis("Salve", piscina);
        consilium.versio   = chorda_ex_literis("2.1", piscina);
        consilium.via_app  = chorda_ex_literis(app, piscina);

        /* I. scriptio prima */
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &consilium,
            &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_VACUA(causa);
        CREDO_VERUM(fasciculum_legere(app, &lectus, &status, &sedes,
            piscina, intern));
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.identitas,
            "org.rhubarb.briar.salve");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.titulus, "Salve");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.versio, "2.1");
        CREDO_VERUM(filum_existit(_in_area(piscina, area,
            "/Salve.app/Contents/MacOS/salve")));
        icns = filum_legere_totum(_in_area(piscina, area,
            "/Salve.app/Contents/Resources/salve.icns"), piscina);
        CREDO_VERUM(icns.mensura > VIII);
        CREDO_VERUM(icns.mensura > VIII
            && memcmp(icns.datum, "icns", (size_t)IV) == ZEPHYRUM);

        /* II. PROPRIUS reponitur: plagula stala evanescit */
        CREDO_VERUM(filum_scribere_literis(stala, "vetus"));
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &consilium,
            &imago,
            exsecutabile, domus, &causa));
        CREDO_FALSUM(filum_existit(stala));
        CREDO_VERUM(fasciculum_legere(app, &lectus, &status, &sedes,
            piscina, intern));

        /* III. ALIENUS recusatur et intactus manet */
        consilium.identitas = chorda_ex_literis("org.aliud.alienum",
            piscina);
        consilium.via_app = chorda_ex_literis(aliena, piscina);
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &consilium,
            &imago,
            exsecutabile, domus, &causa));
        consilium.identitas  =
            chorda_ex_literis("org.rhubarb.briar.salve",
            piscina);
        CREDO_FALSUM(briar_fasciculum_scribere(piscina, &consilium,
            &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("alienus", piscina));
        CREDO_VERUM(fasciculum_legere(aliena, &lectus, &status, &sedes,
            piscina, intern));
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.identitas,
            "org.aliud.alienum");

        /* IV. plagula quae fasciculus non est: recusata, intacta */
        CREDO_VERUM(filum_scribere_literis(planum, "non fasciculus"));
        consilium.via_app = chorda_ex_literis(planum, piscina);
        CREDO_FALSUM(briar_fasciculum_scribere(piscina, &consilium,
            &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("neque fasciculus", piscina));
        CREDO_VERUM(filum_existit(planum)
            && !filum_directorium_existit(planum));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "consilium: salve_vitreum", _vitreum_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "consilium: salve (sine fenestra)", _salve_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "consilium: briar vestitum", _vestitum_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "recusationes cum linea", _recusationes_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "scriptorem", _scriptorem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("briar_fasciculum", SECTIONES);
}
