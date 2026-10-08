/* fabrica_probationes_c.c - genus gradus probationes_c (fabrica-6
 * T6c): probatio C per membrum. Pars bibliothecae fabrica (fabrica-6
 * H2; vide fabrica.c). */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "chorda_aedificator.h"


/* ==================================================
 * PROBATIONES_C (fabrica-6 T6c): probatio C per membrum - clausura per
 * sutura->clausura_c, compilatio per thesaurum (sutura->compilare) in
 * <area>obiecta/, nexus et cursus per in_area_currere (cwd = radix).
 * Facultas scopi -> nexus solum (iudicatum, non cursum).
 * ================================================== */

/* valor attributi elementi gradus; *adest_out FALSUM si abest */
interior chorda
_attributum_gradus (
    constans FabricaActio* actio,
       constans character* titulus,
                      b32* adest_out)
{
    chorda vacua;
       i32 i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *adest_out     = FALSUM;
    per (i = ZEPHYRUM; actio->attributa != NIHIL
         && i < xar_numerus(actio->attributa); i++)
    {
        constans FabricaAttributum* a = (constans FabricaAttributum*)
            xar_obtinere(actio->attributa, i);

        si (chorda_aequalis_literis(a->titulus, titulus))
        {
            *adest_out = VERUM;
            redde a->valor;
        }
    }
    redde vacua;
}

/* exemplar "dir/forma": membra = plagulae '.c' directorii formae
 * congruentes, praeter (forma nominis) exclusae */
interior b32
_probationes_c_membra (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       Xar* membra_out,
                    chorda* causa_out)
{
    chorda  exemplar;
    chorda  praeter;
    chorda  directorium;
    chorda  forma;
       b32  adest;
       b32  praeter_adest;
       Xar* nomina;
       s32  finis;
       i32  i;

    exemplar = _attributum_gradus(actio, "exemplar", &adest);
    si (!adest || exemplar.mensura == ZEPHYRUM)
    {
        *causa_out = chorda_ex_literis("probationes_c sine exemplari",
            piscina);
        redde FALSUM;
    }
    praeter  = _attributum_gradus(actio, "praeter", &praeter_adest);
    finis    = -I;
    per (i = ZEPHYRUM; i < exemplar.mensura; i++)
    {
        si (exemplar.datum[i] == '/')
        {
            finis = (s32)i;
        }
    }
    si (finis < ZEPHYRUM)
    {
        *causa_out = fabricae_iungere(piscina,
            "exemplar sine directorio: ",
            exemplar, "");
        redde FALSUM;
    }
    directorium  = chorda_sectio(exemplar, ZEPHYRUM, (i32)finis + I);
    forma        = chorda_sectio(exemplar, (i32)finis + I,
        exemplar.mensura);
    si (   sutura->enumerare == NIHIL
        || !sutura->enumerare(sutura->datum, chorda_ut_cstr(directorium,
               piscina), piscina, &nomina))
    {
        *causa_out = fabricae_iungere(piscina,
            "directorium exemplaris absens: ",
            directorium, "");
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
                 chorda  n = *(constans chorda*)xar_obtinere(nomina, i);
         FabricaMembrum* membrum;

        si (   n.mensura < III
            || !chorda_aequalis_literis(chorda_sectio(n, n.mensura - II,
                   n.mensura), ".c")
            || !fabricae_globus_congruit(chorda_ut_cstr(forma, piscina),
                   chorda_ut_cstr(n, piscina))
            || (   praeter_adest
                && fabricae_globus_congruit(chorda_ut_cstr(praeter,
                piscina),
                       chorda_ut_cstr(n, piscina))))
        {
            perge;
        }
        membrum = (FabricaMembrum*)xar_addere(membra_out);
        si (membrum == NIHIL)
        {
            redde FALSUM;
        }
        membrum->titulus  = chorda_sectio(n, ZEPHYRUM, n.mensura - II);
        membrum->fons     = chorda_concatenare(directorium, n, piscina);
    }
    redde VERUM;
}

interior vacuum
_ingressum_c_addere (
                    Xar* ingressus,
     constans character* genus,
                 chorda  via,
                Piscina* piscina)
{
    FabricaIngressus* novus;

    novus = (FabricaIngressus*)xar_addere(ingressus);
    si (novus == NIHIL)
    {
        redde;
    }
    novus->genus    = fabrica_genus_invenire(chorda_ex_literis(genus,
        piscina));
    novus->via      = via;
    novus->suffixa  = chorda_ex_literis("", piscina);
}

/* clavis statica: fontes et capita clausurae, aedilis.stml (vexilla),
 * identitas clang. Clausura recusata: fons solus - membrum in agere
 * causam aedilis nominat et FRACTUM manet (defectus numquam servatur),
 * declarationes ceterae non recusantur */
interior b32
_probationes_c_ingressus (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
    constans FabricaMembrum* membrum,
                    Piscina* piscina,
                        Xar* ingressus_out,
                     chorda* causa_out)
{
    FabricaClausuraC clausura;
              chorda causa;
                 i32 i;

    (vacuum)actio;
    (vacuum)causa_out;
    causa = chorda_ex_literis("", piscina);
    si (   sutura->clausura_c != NIHIL
        && sutura->clausura_c(sutura->datum, chorda_ut_cstr(
               membrum->fons, piscina), piscina, &clausura, &causa))
    {
        per (i = ZEPHYRUM; i < xar_numerus(clausura.fontes); i++)
        {
            _ingressum_c_addere(ingressus_out, "fasciculus",
                *(chorda*)xar_obtinere(clausura.fontes, i), piscina);
        }
        per (i = ZEPHYRUM; i < xar_numerus(clausura.capita); i++)
        {
            _ingressum_c_addere(ingressus_out, "fasciculus",
                *(chorda*)xar_obtinere(clausura.capita, i), piscina);
        }
    }
    alioquin
    {
        _ingressum_c_addere(ingressus_out, "fasciculus", membrum->fons,
            piscina);
    }
    _ingressum_c_addere(ingressus_out, "fasciculus", chorda_ex_literis(
        "aedilis.stml", piscina), piscina);
    _ingressum_c_addere(ingressus_out, "identitas_clang",
        chorda_ex_literis("clang", piscina), piscina);
    redde VERUM;
}

/* 'lib/x.c' -> 'lib__x.o' (nomen obiecti in area) */
interior chorda
_obiectum_nominare (
     chorda  fons,
    Piscina* piscina)
{
     ChordaAedificator* aedificator;
                   s32  punctum;
                   i32  i;

    punctum = -I;
    per (i = ZEPHYRUM; i < fons.mensura; i++)
    {
        si (fons.datum[i] == '.')
        {
            punctum = (s32)i;
        }
        si (fons.datum[i] == '/')
        {
            punctum = -I;
        }
    }
    aedificator = chorda_aedificator_creare(piscina, CXXVIII);
    si (aedificator == NIHIL)
    {
        redde fons;
    }
    per (i = ZEPHYRUM; i < (punctum >= ZEPHYRUM ? (i32)punctum
         : fons.mensura); i++)
    {
        si (fons.datum[i] == '/')
        {
            (vacuum)chorda_aedificator_appendere_literis(aedificator,
                "__");
        }
        alioquin
        {
            (vacuum)chorda_aedificator_appendere_character(aedificator,
                (character)fons.datum[i]);
        }
    }
    (vacuum)chorda_aedificator_appendere_literis(aedificator, ".o");
    redde chorda_aedificator_finire(aedificator);
}

interior b32
_probationes_c_agere (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
    constans FabricaMembrum* membrum,
         constans character* area,
               constans Xar* ambitus,
                    Piscina* piscina,
               FabricaActum* actum_out)
{
    FabricaClausuraC  clausura;
              chorda  causa;
              chorda  erratum;
              chorda  binarium;
              chorda  area_c;
                 Xar* argv;
           character* liber;
                 i32  i;

    area_c = chorda_ex_literis(area, piscina);
    si (   sutura->clausura_c == NIHIL || sutura->compilare == NIHIL
        || sutura->in_area_currere == NIHIL)
    {
        actum_out->cauda = chorda_ex_literis("sutura sine clausura_c, "
            "compilare aut in_area_currere", piscina);
        redde FALSUM;
    }
    causa = chorda_ex_literis("", piscina);
    si (!sutura->clausura_c(sutura->datum, chorda_ut_cstr(membrum->fons,
            piscina), piscina, &clausura, &causa))
    {
        actum_out->codex  = I;
        actum_out->cauda  = fabricae_iungere(piscina,
            "clausura recusata: ",
            causa, "");
        redde VERUM;
    }
    argv = fabricae_xar_chordarum(piscina);
    fabricae_chordam_addere(argv, chorda_ex_literis("clang", piscina));
    per (i = ZEPHYRUM; i < xar_numerus(clausura.fontes); i++)
    {
        chorda fons = *(chorda*)xar_obtinere(clausura.fontes, i);
        chorda obiectum;

        obiectum = chorda_concatenare(fabricae_iungere(piscina, "",
            area_c,
            "obiecta/"), _obiectum_nominare(fons, piscina), piscina);
        erratum = chorda_ex_literis("", piscina);
        si (!sutura->compilare(sutura->datum, chorda_ut_cstr(fons,
                piscina), chorda_ut_cstr(obiectum, piscina), piscina,
                &erratum))
        {
            actum_out->codex  = I;
            actum_out->cauda  =
                chorda_concatenare(fabricae_iungere(piscina,
                "compilatio fracta: ", fons, ": "), erratum, piscina);
            redde VERUM;
        }
        fabricae_chordam_addere(argv, obiectum);
    }
    /* vexilla nexus: verba spatio separata ("-framework Cocoa") */
    per (i = ZEPHYRUM; i < xar_numerus(clausura.vexilla_nexus); i++)
    {
        chorda v = *(chorda*)xar_obtinere(clausura.vexilla_nexus, i);
           i32 initium = ZEPHYRUM;
           i32 j;

        per (j = ZEPHYRUM; j <= v.mensura; j++)
        {
            si (j == v.mensura || v.datum[j] == ' ')
            {
                si (j > initium)
                {
                    fabricae_chordam_addere(argv, chorda_sectio(v,
                        initium, j));
                }
                initium = j + I;
            }
        }
    }
    binarium = chorda_concatenare(area_c, membrum->titulus, piscina);
    fabricae_chordam_addere(argv, chorda_ex_literis("-o", piscina));
    fabricae_chordam_addere(argv, binarium);
    liber = chorda_ut_cstr(fabrica_liber_via(actio->titulus, piscina),
        piscina);
    si (!sutura->in_area_currere(sutura->datum, argv, area, ambitus,
            liber, chorda_ut_cstr(fabricae_iungere(piscina, "", area_c,
            "nexus.log"), piscina), piscina, actum_out))
    {
        redde FALSUM;
    }
    si (actum_out->codex != ZEPHYRUM)
    {
        actum_out->cauda = fabricae_iungere(piscina, "nexus fractus: ",
            actum_out->cauda, "");
        redde VERUM;
    }
    si (xar_numerus(clausura.facultates) > ZEPHYRUM)
    {
        chorda nota;

        nota = chorda_ex_literis("nexus solum: facultas ", piscina);
        per (i = ZEPHYRUM; i < xar_numerus(clausura.facultates); i++)
        {
            nota = chorda_concatenare(i
                > ZEPHYRUM ? fabricae_iungere(piscina,
                "", nota, ", ") : nota, *(chorda*)xar_obtinere(
                clausura.facultates, i), piscina);
        }
        actum_out->codex = ZEPHYRUM;
        actum_out->cauda = nota;
        redde VERUM;
    }
    argv = fabricae_xar_chordarum(piscina);
    fabricae_chordam_addere(argv, binarium);
    si (!sutura->in_area_currere(sutura->datum, argv, area, ambitus,
            liber, chorda_ut_cstr(fabrica_acta_via(actio->titulus,
            piscina), piscina), piscina, actum_out))
    {
        redde FALSUM;
    }
    si (actum_out->codex == ZEPHYRUM)
    {
        actum_out->cauda = chorda_ex_literis("", piscina);
    }
    redde VERUM;
}

constans FabricaGradus fabricae_gradus_probationes_c = {
    "probationes_c", _probationes_c_membra, _probationes_c_ingressus,
    _probationes_c_agere, VERUM, FABRICA_SUMPTUS_CARUS
};
