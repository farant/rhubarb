/* briar_fasciculum.c - Vide briar_fasciculum.h (spec par. 4.8). */

#include "briar_fasciculum.h"
#include "briar_fabrica.h"
#include "briar_nexus.h"
#include "chorda_aedificator.h"
#include "fasciculum.h"
#include "filum.h"
#include "icones.h"
#include "internamentum.h"
#include "via.h"
#include <string.h>

/* causa ex aedificatore + linea tagi; FALSUM semper */
interior b32
_fasciculi_recusare (
                chorda* causa,
                   i32* linea_causae,
     ChordaAedificator* textus,
                   i32  linea)
{
    *causa         = chorda_aedificator_finire(textus);
    *linea_causae  = linea;
    redde FALSUM;
}

b32
briar_fasciculum_consilium (
                     Piscina* piscina,
                         Xar* nexus,
          constans character* via_thistle,
          constans character* via_icon_vexilli,
    BriarFasciculumConsilium* consilium,
                      chorda* causa,
                         i32* linea_causae)
{
          constans BriarNexusRes* elementum_briar  = NIHIL;
          constans BriarNexusRes* fenestra         = NIHIL;
              constans character* titulus_plagulae;
               ChordaAedificator* aed;
                          chorda  vacua;
                          chorda  directorium;
                          chorda  partes[II];
                             i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    memset(consilium, ZEPHYRUM, magnitudo(*consilium));
    *causa         = vacua;
    *linea_causae  = ZEPHYRUM;

    per (i = ZEPHYRUM; nexus != NIHIL && i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r =
            (constans BriarNexusRes*)xar_obtinere(nexus, i);

        si (r->genus != BRIAR_NEXUS_STML)
        {
            perge;
        }
        si (briar_nexus_titulus_est(r, "briar"))
        {
            si (elementum_briar != NIHIL)
            {
                aed = chorda_aedificator_creare(piscina,
                    (memoriae_index)96);
                chorda_aedificator_appendere_literis(aed,
                    "elementum briar iteratum (primum linea ");
                chorda_aedificator_appendere_s32(aed,
                    (s32)elementum_briar->linea_initium);
                chorda_aedificator_appendere_literis(aed, ")");
                redde _fasciculi_recusare(causa, linea_causae, aed,
                    r->linea_initium);
            }
            elementum_briar = r;
        }
        alioquin si (briar_nexus_titulus_est(r, "fenestra"))
        {
            fenestra = r;
        }
    }

    titulus_plagulae = briar_fabrica_titulus(piscina, via_thistle);

    /* A4: identitas - attributum, alioquin praefixum + nomen */
    si (   elementum_briar != NIHIL
        && briar_nexus_attributum_habet(elementum_briar, "identitas"))
    {
        consilium->identitas = briar_nexus_attributum(elementum_briar,
            "identitas");
    }
    alioquin
    {
        constans character* p;

        aed = chorda_aedificator_creare(piscina, (memoriae_index)64);
        chorda_aedificator_appendere_literis(aed,
            BRIAR_FASCICULUM_PRAEFIXUM);
        per (p = titulus_plagulae; *p != '\0'; p++)
        {
            chorda_aedificator_appendere_character(aed,
                (*p == '_') ? '-' : *p);
        }
        consilium->identitas = chorda_aedificator_finire(aed);
    }
    si (   chorda_vacua(consilium->identitas)
        || !fasciculum_identitas_valida(consilium->identitas))
    {
        aed = chorda_aedificator_creare(piscina, (memoriae_index)128);
        chorda_aedificator_appendere_literis(aed, "identitas '");
        chorda_aedificator_appendere_chorda(aed, consilium->identitas);
        chorda_aedificator_appendere_literis(aed,
            "': characteres soli A-Z a-z 0-9 . -");
        redde _fasciculi_recusare(causa, linea_causae, aed,
            elementum_briar != NIHIL ? elementum_briar->linea_initium
                                     : ZEPHYRUM);
    }

    /* A5: nomen et versio */
    si (fenestra != NIHIL)
    {
        consilium->titulus = briar_nexus_attributum(fenestra,
            "titulus");
    }
    si (chorda_vacua(consilium->titulus))
    {
        consilium->titulus = chorda_ex_literis(titulus_plagulae,
            piscina);
    }
    si (elementum_briar != NIHIL)
    {
        consilium->versio       = briar_nexus_attributum(
            elementum_briar, "versio");
        consilium->linea_briar  = elementum_briar->linea_initium;
    }

    /* A7: vexillum (ad cwd), deinde attributum (ad plagulam), deinde
     * icon infixus (via vacua) */
    directorium = via_directorium(chorda_ex_literis(via_thistle,
        piscina), piscina);
    si (via_icon_vexilli != NIHIL)
    {
        consilium->via_icon = chorda_ex_literis(via_icon_vexilli,
            piscina);
    }
    alioquin si (   elementum_briar != NIHIL
                 && briar_nexus_attributum_habet(elementum_briar,
                        "icon"))
    {
        chorda icon = briar_nexus_attributum(elementum_briar, "icon");

        si (via_est_absoluta(icon))
        {
            consilium->via_icon = icon;
        }
        alioquin
        {
            partes[ZEPHYRUM]     = directorium;
            partes[I]            = icon;
            consilium->via_icon  = via_iungere(partes, II, piscina);
        }
    }

    /* A3: iuxta plagulam */
    partes[ZEPHYRUM]    = directorium;
    partes[I]           = chorda_concatenare(
        chorda_ex_literis(titulus_plagulae, piscina),
        chorda_ex_literis(".app", piscina), piscina);
    consilium->via_app  = via_iungere(partes, II, piscina);
    redde VERUM;
}

/* causa scriptoris: textus, status si non negativus, sedes si adest */
interior b32
_scriptoris_recusare (
                 Piscina* piscina,
                  chorda* causa,
      constans character* textus,
                  chorda  sedes,
                     s32  status)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)160);

    chorda_aedificator_appendere_literis(aed, textus);
    si (status >= ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, " (status ");
        chorda_aedificator_appendere_s32(aed, status);
        chorda_aedificator_appendere_literis(aed, ")");
    }
    si (sedes.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, ": ");
        chorda_aedificator_appendere_chorda(aed, sedes);
    }
    *causa = chorda_aedificator_finire(aed);
    redde FALSUM;
}

b32
briar_fasciculum_scribere (
                               Piscina* piscina,
     constans BriarFasciculumConsilium* consilium,
                        constans Imago* icon,
                    constans character* exsecutabile,
                    constans character* domus,
                                chorda* causa)
{
                 Icones  petitio;
          IconesFructus  fructus;
           IconesStatus  st_icon;
             Fasciculum  f;
             Fasciculum  vetus;
       FasciculumStatus  st_fasciculi;
                 chorda  sedes;
                 chorda  vacua;
                 chorda  titulus_exs;
                 chorda  directorium;
                 chorda  partes_viae[II];
                    Xar* partes;
     constans character* via_icns;
     constans character* via_app;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *causa         = vacua;
    via_app        = chorda_ut_cstr(consilium->via_app, piscina);
    titulus_exs    = via_nomen(chorda_ex_literis(exsecutabile, piscina),
        piscina);

    /* I. pixela in .icns, in domo proiecti */
    memset(&petitio, ZEPHYRUM, magnitudo(Icones));
    petitio.fons           = icon;
    petitio.titulus        = titulus_exs;
    petitio.recidere       = ICONES_RECIDERE_CENTRUM;
    petitio.latera_petita  = ZEPHYRUM;
    si (!icones_reddere(&petitio, &fructus, &st_icon, &sedes, piscina))
    {
        redde _scriptoris_recusare(piscina, causa,
            "icon reddi non potuit", sedes, (s32)st_icon);
    }
    partes_viae[ZEPHYRUM]  = chorda_ex_literis(domus, piscina);
    partes_viae[I]         = chorda_ex_literis("fasciculum", piscina);
    directorium            = via_iungere(partes_viae, II, piscina);
    si (!filum_directorium_creare_cum_parentibus(
            chorda_ut_cstr(directorium, piscina)))
    {
        redde _scriptoris_recusare(piscina, causa,
            "directorium iconis creari non potuit", directorium, -I);
    }
    partes_viae[ZEPHYRUM]  = directorium;
    partes_viae[I]         = chorda_concatenare(titulus_exs,
        chorda_ex_literis(".icns", piscina), piscina);
    via_icns = chorda_ut_cstr(via_iungere(partes_viae, II, piscina),
        piscina);
    si (!icones_icns_scribere(&fructus, via_icns, &st_icon, &sedes,
            piscina))
    {
        redde _scriptoris_recusare(piscina, causa,
            "icon scribi non potuit", sedes, (s32)st_icon);
    }

    /* II. planum fasciculi (purum) */
    memset(&f, ZEPHYRUM, magnitudo(Fasciculum));
    f.identitas     = consilium->identitas;
    f.titulus       = consilium->titulus;
    f.versio        = consilium->versio;
    f.exsecutabile  = exsecutabile;
    f.icon          = via_icns;
    si (!fasciculum_reddere(&f, &partes, &st_fasciculi, &sedes,
        piscina))
    {
        redde _scriptoris_recusare(piscina, causa,
            "planum fasciculi recusatum", sedes, (s32)st_fasciculi);
    }

    /* III. exsistens: PROPRIUS reponitur, cetera recusantur (A6) */
    si (filum_existit(via_app) || filum_directorium_existit(via_app))
    {
        InternamentumChorda* intern = internamentum_creare(piscina);

        si (   !filum_directorium_existit(via_app)
            || !fasciculum_legere(via_app, &vetus, &st_fasciculi,
            &sedes,
                                  piscina, intern))
        {
            redde _scriptoris_recusare(piscina, causa,
                "exsistit neque fasciculus est - non tangitur",
                consilium->via_app, -I);
        }
        si (!chorda_aequalis(vetus.identitas, consilium->identitas))
        {
            redde _scriptoris_recusare(piscina, causa,
                "fasciculus alienus est - non tangitur (identitas)",
                vetus.identitas, -I);
        }
        si (!filum_arborem_delere(via_app))
        {
            redde _scriptoris_recusare(piscina, causa,
                "fasciculus vetus deleri non potuit",
                consilium->via_app,
                -I);
        }
    }

    /* IV. scribere */
    si (!fasciculum_scribere(partes, via_app, &st_fasciculi, &sedes,
        piscina))
    {
        redde _scriptoris_recusare(piscina, causa,
            "fasciculus scribi non potuit", sedes, (s32)st_fasciculi);
    }
    redde VERUM;
}
