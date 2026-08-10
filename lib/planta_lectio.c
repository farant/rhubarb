/* GENERATUM a canon_coquere e natura/cocta/planta.canon - NOLI MANU MUTARE.
 * Vide caput pro legibus folii. */
#include "planta_lectio.h"

PlantaIndividua*
planta_individua_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaIndividua* res;

    (vacuum)intern;
    res = (PlantaIndividua*)piscina_allocare(piscina,
        magnitudo(PlantaIndividua));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->planta = xar_creare(piscina, (i32)magnitudo(PlantaPlanta*));
    res->rosaceae = xar_creare(piscina, (i32)magnitudo(PlantaRosaceae*));
    res->rosa = xar_creare(piscina, (i32)magnitudo(PlantaRosa*));
    res->rosa_canina = xar_creare(piscina, (i32)magnitudo(PlantaRosaCanina*));
    res->malus = xar_creare(piscina, (i32)magnitudo(PlantaMalus*));
    res->malus_domestica = xar_creare(piscina, (i32)magnitudo(PlantaMalusDomestica*));
    res->granny_smith = xar_creare(piscina, (i32)magnitudo(PlantaGrannySmith*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "planta"))
            {
                PlantaPlanta* filius;
                PlantaPlanta** locus;

                filius = planta_planta_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaPlanta**)xar_addere(res->planta);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "rosaceae"))
            {
                PlantaRosaceae* filius;
                PlantaRosaceae** locus;

                filius = planta_rosaceae_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaceae**)xar_addere(res->rosaceae);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "rosa"))
            {
                PlantaRosa* filius;
                PlantaRosa** locus;

                filius = planta_rosa_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosa**)xar_addere(res->rosa);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "rosa-canina"))
            {
                PlantaRosaCanina* filius;
                PlantaRosaCanina** locus;

                filius = planta_rosa_canina_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaCanina**)xar_addere(res->rosa_canina);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "malus"))
            {
                PlantaMalus* filius;
                PlantaMalus** locus;

                filius = planta_malus_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaMalus**)xar_addere(res->malus);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "malus-domestica"))
            {
                PlantaMalusDomestica* filius;
                PlantaMalusDomestica** locus;

                filius = planta_malus_domestica_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaMalusDomestica**)xar_addere(res->malus_domestica);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "granny-smith"))
            {
                PlantaGrannySmith* filius;
                PlantaGrannySmith** locus;

                filius = planta_granny_smith_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaGrannySmith**)xar_addere(res->granny_smith);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaIndividua*
planta_individua_legere(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              causa)
{
    StmlResultus r;

    si (causa)
    {
        causa->datum   = NIHIL;
        causa->mensura = ZEPHYRUM;
    }

    r = stml_legere(fons, piscina, intern);
    si (!r.successus || !r.elementum_radix)
    {
        si (causa)
        {
            *causa = chorda_ex_literis(
                "stml parsari nequit", piscina);
        }
        redde NIHIL;
    }
    si (!r.elementum_radix->titulus ||
        !chorda_aequalis_literis(*r.elementum_radix->titulus,
                                 "individua"))
    {
        si (causa)
        {
            *causa = chorda_ex_literis(
                "radix non est <individua>", piscina);
        }
        redde NIHIL;
    }

    redde planta_individua_legere_nodum(r.elementum_radix, piscina,
        intern);
}

PlantaPlanta*
planta_planta_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlanta* res;

    (vacuum)intern;
    res = (PlantaPlanta*)piscina_allocare(piscina,
        magnitudo(PlantaPlanta));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nomen_v = stml_attributum_capere(nodus, "nomen");

    res->habitus = PLANTA_PLANTA_HABITUS_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "habitus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "herba"))
            {
                res->habitus = PLANTA_PLANTA_HABITUS_HERBA;
            }
            alioquin si (chorda_aequalis_literis(*v, "frutex"))
            {
                res->habitus = PLANTA_PLANTA_HABITUS_FRUTEX;
            }
            alioquin si (chorda_aequalis_literis(*v, "arbor"))
            {
                res->habitus = PLANTA_PLANTA_HABITUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*v, "scandens"))
            {
                res->habitus = PLANTA_PLANTA_HABITUS_SCANDENS;
            }
        }
    }

    res->durata = PLANTA_PLANTA_DURATA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "durata");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "annua"))
            {
                res->durata = PLANTA_PLANTA_DURATA_ANNUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "biennis"))
            {
                res->durata = PLANTA_PLANTA_DURATA_BIENNIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "perennis"))
            {
                res->durata = PLANTA_PLANTA_DURATA_PERENNIS;
            }
        }
    }

    res->folia_permanentia = PLANTA_PLANTA_FOLIA_PERMANENTIA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "folia-permanentia");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "decidua"))
            {
                res->folia_permanentia = PLANTA_PLANTA_FOLIA_PERMANENTIA_DECIDUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sempervirens"))
            {
                res->folia_permanentia = PLANTA_PLANTA_FOLIA_PERMANENTIA_SEMPERVIRENS;
            }
        }
    }

    res->altitudo_matura = ZEPHYRUM;
    res->altitudo_matura_praesens = FALSUM;
    {
        chorda* v;
        s32 tv;

        v = stml_attributum_capere(nodus, "altitudo-matura");
        si (v && chorda_ut_s32(*v, &tv))
        {
            res->altitudo_matura = (s64)tv;
            res->altitudo_matura_praesens = VERUM;
        }
    }

    res->status_vita = PLANTA_PLANTA_STATUS_VITA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-vita");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "semen"))
            {
                res->status_vita = PLANTA_PLANTA_STATUS_VITA_SEMEN;
            }
            alioquin si (chorda_aequalis_literis(*v, "germinans"))
            {
                res->status_vita = PLANTA_PLANTA_STATUS_VITA_GERMINANS;
            }
            alioquin si (chorda_aequalis_literis(*v, "iuvenis"))
            {
                res->status_vita = PLANTA_PLANTA_STATUS_VITA_IUVENIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "florens"))
            {
                res->status_vita = PLANTA_PLANTA_STATUS_VITA_FLORENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructifera"))
            {
                res->status_vita = PLANTA_PLANTA_STATUS_VITA_FRUCTIFERA;
            }
            alioquin si (chorda_aequalis_literis(*v, "quiescens"))
            {
                res->status_vita = PLANTA_PLANTA_STATUS_VITA_QUIESCENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "mortua"))
            {
                res->status_vita = PLANTA_PLANTA_STATUS_VITA_MORTUA;
            }
        }
    }

    res->auctor_nominis = stml_attributum_capere(nodus, "auctor-nominis");

    res->nomen_publicatum_in = stml_attributum_capere(nodus, "nomen-publicatum-in");

    res->pollinatur_a = stml_attributum_capere(nodus, "pollinatur-a");

    res->anima = PLANTA_PLANTA_ANIMA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "anima");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "vegetativa"))
            {
                res->anima = PLANTA_PLANTA_ANIMA_VEGETATIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sensitiva"))
            {
                res->anima = PLANTA_PLANTA_ANIMA_SENSITIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "rationalis"))
            {
                res->anima = PLANTA_PLANTA_ANIMA_RATIONALIS;
            }
        }
    }

    res->massa = stml_attributum_capere(nodus, "massa");

    res->perituram = FALSUM;
    res->perituram_praesens = FALSUM;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "perituram");
        si (v)
        {
            res->perituram = (b32)chorda_aequalis_literis(*v, "verum");
            res->perituram_praesens = VERUM;
        }
    }

    res->status_conditio = PLANTA_PLANTA_STATUS_CONDITIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-conditio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "recens"))
            {
                res->status_conditio = PLANTA_PLANTA_STATUS_CONDITIO_RECENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "vetus"))
            {
                res->status_conditio = PLANTA_PLANTA_STATUS_CONDITIO_VETUS;
            }
            alioquin si (chorda_aequalis_literis(*v, "corrupta"))
            {
                res->status_conditio = PLANTA_PLANTA_STATUS_CONDITIO_CORRUPTA;
            }
        }
    }

    res->sita_in = stml_attributum_capere(nodus, "sita-in");
    res->radix = NIHIL;
    res->caulis = NIHIL;
    res->folium = NIHIL;
    res->flos = NIHIL;
    res->fructus = NIHIL;
    res->semen = NIHIL;
    res->generat = xar_creare(piscina, (i32)magnitudo(PlantaPlantaGenerat*));
    res->laborat = xar_creare(piscina, (i32)magnitudo(PlantaPlantaLaborat*));
    res->historia = NIHIL;
    res->nota = xar_creare(piscina, (i32)magnitudo(PlantaNota*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "radix"))
            {
                si (!res->radix)
                {
                    res->radix =
                        planta_planta_radix_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "caulis"))
            {
                si (!res->caulis)
                {
                    res->caulis =
                        planta_planta_caulis_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "folium"))
            {
                si (!res->folium)
                {
                    res->folium =
                        planta_planta_folium_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "flos"))
            {
                si (!res->flos)
                {
                    res->flos =
                        planta_planta_flos_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "fructus"))
            {
                si (!res->fructus)
                {
                    res->fructus =
                        planta_planta_fructus_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "semen"))
            {
                si (!res->semen)
                {
                    res->semen =
                        planta_planta_semen_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "generat"))
            {
                PlantaPlantaGenerat* filius;
                PlantaPlantaGenerat** locus;

                filius = planta_planta_generat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaPlantaGenerat**)xar_addere(res->generat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "laborat"))
            {
                PlantaPlantaLaborat* filius;
                PlantaPlantaLaborat** locus;

                filius = planta_planta_laborat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaPlantaLaborat**)xar_addere(res->laborat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "historia"))
            {
                si (!res->historia)
                {
                    res->historia =
                        planta_planta_historia_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "nota"))
            {
                PlantaNota* filius;
                PlantaNota** locus;

                filius = planta_nota_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaNota**)xar_addere(res->nota);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaPlantaRadix*
planta_planta_radix_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaRadix* res;

    (vacuum)intern;
    res = (PlantaPlantaRadix*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaRadix));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaPlantaCaulis*
planta_planta_caulis_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaCaulis* res;

    (vacuum)intern;
    res = (PlantaPlantaCaulis*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaCaulis));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaPlantaFolium*
planta_planta_folium_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaFolium* res;

    (vacuum)intern;
    res = (PlantaPlantaFolium*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaFolium));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaPlantaFlos*
planta_planta_flos_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaFlos* res;

    (vacuum)intern;
    res = (PlantaPlantaFlos*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaFlos));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaPlantaFructus*
planta_planta_fructus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaFructus* res;

    (vacuum)intern;
    res = (PlantaPlantaFructus*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaFructus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaPlantaSemen*
planta_planta_semen_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaSemen* res;

    (vacuum)intern;
    res = (PlantaPlantaSemen*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaSemen));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaPlantaGenerat*
planta_planta_generat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaGenerat* res;

    (vacuum)intern;
    res = (PlantaPlantaGenerat*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaGenerat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaPlantaLaborat*
planta_planta_laborat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaLaborat* res;

    (vacuum)intern;
    res = (PlantaPlantaLaborat*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaLaborat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaPlantaHistoria*
planta_planta_historia_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaPlantaHistoria* res;

    (vacuum)intern;
    res = (PlantaPlantaHistoria*)piscina_allocare(piscina,
        magnitudo(PlantaPlantaHistoria));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->eventum = xar_creare(piscina, (i32)magnitudo(PlantaHistoriaEventum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "eventum"))
            {
                PlantaHistoriaEventum* filius;
                PlantaHistoriaEventum** locus;

                filius = planta_historia_eventum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaHistoriaEventum**)xar_addere(res->eventum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaRosaceae*
planta_rosaceae_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceae* res;

    (vacuum)intern;
    res = (PlantaRosaceae*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceae));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nomen_v = stml_attributum_capere(nodus, "nomen");

    res->habitus = PLANTA_ROSACEAE_HABITUS_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "habitus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "herba"))
            {
                res->habitus = PLANTA_ROSACEAE_HABITUS_HERBA;
            }
            alioquin si (chorda_aequalis_literis(*v, "frutex"))
            {
                res->habitus = PLANTA_ROSACEAE_HABITUS_FRUTEX;
            }
            alioquin si (chorda_aequalis_literis(*v, "arbor"))
            {
                res->habitus = PLANTA_ROSACEAE_HABITUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*v, "scandens"))
            {
                res->habitus = PLANTA_ROSACEAE_HABITUS_SCANDENS;
            }
        }
    }

    res->durata = PLANTA_ROSACEAE_DURATA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "durata");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "annua"))
            {
                res->durata = PLANTA_ROSACEAE_DURATA_ANNUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "biennis"))
            {
                res->durata = PLANTA_ROSACEAE_DURATA_BIENNIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "perennis"))
            {
                res->durata = PLANTA_ROSACEAE_DURATA_PERENNIS;
            }
        }
    }

    res->folia_permanentia = PLANTA_ROSACEAE_FOLIA_PERMANENTIA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "folia-permanentia");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "decidua"))
            {
                res->folia_permanentia = PLANTA_ROSACEAE_FOLIA_PERMANENTIA_DECIDUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sempervirens"))
            {
                res->folia_permanentia = PLANTA_ROSACEAE_FOLIA_PERMANENTIA_SEMPERVIRENS;
            }
        }
    }

    res->altitudo_matura = ZEPHYRUM;
    res->altitudo_matura_praesens = FALSUM;
    {
        chorda* v;
        s32 tv;

        v = stml_attributum_capere(nodus, "altitudo-matura");
        si (v && chorda_ut_s32(*v, &tv))
        {
            res->altitudo_matura = (s64)tv;
            res->altitudo_matura_praesens = VERUM;
        }
    }

    res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-vita");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "semen"))
            {
                res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_SEMEN;
            }
            alioquin si (chorda_aequalis_literis(*v, "germinans"))
            {
                res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_GERMINANS;
            }
            alioquin si (chorda_aequalis_literis(*v, "iuvenis"))
            {
                res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_IUVENIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "florens"))
            {
                res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_FLORENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructifera"))
            {
                res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_FRUCTIFERA;
            }
            alioquin si (chorda_aequalis_literis(*v, "quiescens"))
            {
                res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_QUIESCENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "mortua"))
            {
                res->status_vita = PLANTA_ROSACEAE_STATUS_VITA_MORTUA;
            }
        }
    }

    res->auctor_nominis = stml_attributum_capere(nodus, "auctor-nominis");

    res->nomen_publicatum_in = stml_attributum_capere(nodus, "nomen-publicatum-in");

    res->pollinatur_a = stml_attributum_capere(nodus, "pollinatur-a");

    res->anima = PLANTA_ROSACEAE_ANIMA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "anima");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "vegetativa"))
            {
                res->anima = PLANTA_ROSACEAE_ANIMA_VEGETATIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sensitiva"))
            {
                res->anima = PLANTA_ROSACEAE_ANIMA_SENSITIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "rationalis"))
            {
                res->anima = PLANTA_ROSACEAE_ANIMA_RATIONALIS;
            }
        }
    }

    res->massa = stml_attributum_capere(nodus, "massa");

    res->perituram = FALSUM;
    res->perituram_praesens = FALSUM;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "perituram");
        si (v)
        {
            res->perituram = (b32)chorda_aequalis_literis(*v, "verum");
            res->perituram_praesens = VERUM;
        }
    }

    res->status_conditio = PLANTA_ROSACEAE_STATUS_CONDITIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-conditio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "recens"))
            {
                res->status_conditio = PLANTA_ROSACEAE_STATUS_CONDITIO_RECENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "vetus"))
            {
                res->status_conditio = PLANTA_ROSACEAE_STATUS_CONDITIO_VETUS;
            }
            alioquin si (chorda_aequalis_literis(*v, "corrupta"))
            {
                res->status_conditio = PLANTA_ROSACEAE_STATUS_CONDITIO_CORRUPTA;
            }
        }
    }

    res->sita_in = stml_attributum_capere(nodus, "sita-in");
    res->radix = NIHIL;
    res->caulis = NIHIL;
    res->folium = NIHIL;
    res->flos = NIHIL;
    res->fructus = NIHIL;
    res->semen = NIHIL;
    res->generat = xar_creare(piscina, (i32)magnitudo(PlantaRosaceaeGenerat*));
    res->laborat = xar_creare(piscina, (i32)magnitudo(PlantaRosaceaeLaborat*));
    res->historia = NIHIL;
    res->nota = xar_creare(piscina, (i32)magnitudo(PlantaNota*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "radix"))
            {
                si (!res->radix)
                {
                    res->radix =
                        planta_rosaceae_radix_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "caulis"))
            {
                si (!res->caulis)
                {
                    res->caulis =
                        planta_rosaceae_caulis_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "folium"))
            {
                si (!res->folium)
                {
                    res->folium =
                        planta_rosaceae_folium_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "flos"))
            {
                si (!res->flos)
                {
                    res->flos =
                        planta_rosaceae_flos_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "fructus"))
            {
                si (!res->fructus)
                {
                    res->fructus =
                        planta_rosaceae_fructus_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "semen"))
            {
                si (!res->semen)
                {
                    res->semen =
                        planta_rosaceae_semen_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "generat"))
            {
                PlantaRosaceaeGenerat* filius;
                PlantaRosaceaeGenerat** locus;

                filius = planta_rosaceae_generat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaceaeGenerat**)xar_addere(res->generat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "laborat"))
            {
                PlantaRosaceaeLaborat* filius;
                PlantaRosaceaeLaborat** locus;

                filius = planta_rosaceae_laborat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaceaeLaborat**)xar_addere(res->laborat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "historia"))
            {
                si (!res->historia)
                {
                    res->historia =
                        planta_rosaceae_historia_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "nota"))
            {
                PlantaNota* filius;
                PlantaNota** locus;

                filius = planta_nota_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaNota**)xar_addere(res->nota);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaRosaceaeRadix*
planta_rosaceae_radix_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeRadix* res;

    (vacuum)intern;
    res = (PlantaRosaceaeRadix*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeRadix));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaceaeCaulis*
planta_rosaceae_caulis_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeCaulis* res;

    (vacuum)intern;
    res = (PlantaRosaceaeCaulis*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeCaulis));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaceaeFolium*
planta_rosaceae_folium_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeFolium* res;

    (vacuum)intern;
    res = (PlantaRosaceaeFolium*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeFolium));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaceaeFlos*
planta_rosaceae_flos_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeFlos* res;

    (vacuum)intern;
    res = (PlantaRosaceaeFlos*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeFlos));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaceaeFructus*
planta_rosaceae_fructus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeFructus* res;

    (vacuum)intern;
    res = (PlantaRosaceaeFructus*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeFructus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaceaeSemen*
planta_rosaceae_semen_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeSemen* res;

    (vacuum)intern;
    res = (PlantaRosaceaeSemen*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeSemen));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaceaeGenerat*
planta_rosaceae_generat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeGenerat* res;

    (vacuum)intern;
    res = (PlantaRosaceaeGenerat*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeGenerat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaRosaceaeLaborat*
planta_rosaceae_laborat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeLaborat* res;

    (vacuum)intern;
    res = (PlantaRosaceaeLaborat*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeLaborat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaRosaceaeHistoria*
planta_rosaceae_historia_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaceaeHistoria* res;

    (vacuum)intern;
    res = (PlantaRosaceaeHistoria*)piscina_allocare(piscina,
        magnitudo(PlantaRosaceaeHistoria));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->eventum = xar_creare(piscina, (i32)magnitudo(PlantaHistoriaEventum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "eventum"))
            {
                PlantaHistoriaEventum* filius;
                PlantaHistoriaEventum** locus;

                filius = planta_historia_eventum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaHistoriaEventum**)xar_addere(res->eventum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaRosa*
planta_rosa_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosa* res;

    (vacuum)intern;
    res = (PlantaRosa*)piscina_allocare(piscina,
        magnitudo(PlantaRosa));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nomen_v = stml_attributum_capere(nodus, "nomen");

    res->habitus = PLANTA_ROSA_HABITUS_FRUTEX;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "habitus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "herba"))
            {
                res->habitus = PLANTA_ROSA_HABITUS_HERBA;
            }
            alioquin si (chorda_aequalis_literis(*v, "frutex"))
            {
                res->habitus = PLANTA_ROSA_HABITUS_FRUTEX;
            }
            alioquin si (chorda_aequalis_literis(*v, "arbor"))
            {
                res->habitus = PLANTA_ROSA_HABITUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*v, "scandens"))
            {
                res->habitus = PLANTA_ROSA_HABITUS_SCANDENS;
            }
        }
    }

    res->durata = PLANTA_ROSA_DURATA_PERENNIS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "durata");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "annua"))
            {
                res->durata = PLANTA_ROSA_DURATA_ANNUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "biennis"))
            {
                res->durata = PLANTA_ROSA_DURATA_BIENNIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "perennis"))
            {
                res->durata = PLANTA_ROSA_DURATA_PERENNIS;
            }
        }
    }

    res->folia_permanentia = PLANTA_ROSA_FOLIA_PERMANENTIA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "folia-permanentia");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "decidua"))
            {
                res->folia_permanentia = PLANTA_ROSA_FOLIA_PERMANENTIA_DECIDUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sempervirens"))
            {
                res->folia_permanentia = PLANTA_ROSA_FOLIA_PERMANENTIA_SEMPERVIRENS;
            }
        }
    }

    res->altitudo_matura = ZEPHYRUM;
    res->altitudo_matura_praesens = FALSUM;
    {
        chorda* v;
        s32 tv;

        v = stml_attributum_capere(nodus, "altitudo-matura");
        si (v && chorda_ut_s32(*v, &tv))
        {
            res->altitudo_matura = (s64)tv;
            res->altitudo_matura_praesens = VERUM;
        }
    }

    res->status_vita = PLANTA_ROSA_STATUS_VITA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-vita");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "semen"))
            {
                res->status_vita = PLANTA_ROSA_STATUS_VITA_SEMEN;
            }
            alioquin si (chorda_aequalis_literis(*v, "germinans"))
            {
                res->status_vita = PLANTA_ROSA_STATUS_VITA_GERMINANS;
            }
            alioquin si (chorda_aequalis_literis(*v, "iuvenis"))
            {
                res->status_vita = PLANTA_ROSA_STATUS_VITA_IUVENIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "florens"))
            {
                res->status_vita = PLANTA_ROSA_STATUS_VITA_FLORENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructifera"))
            {
                res->status_vita = PLANTA_ROSA_STATUS_VITA_FRUCTIFERA;
            }
            alioquin si (chorda_aequalis_literis(*v, "quiescens"))
            {
                res->status_vita = PLANTA_ROSA_STATUS_VITA_QUIESCENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "mortua"))
            {
                res->status_vita = PLANTA_ROSA_STATUS_VITA_MORTUA;
            }
        }
    }

    res->auctor_nominis = stml_attributum_capere(nodus, "auctor-nominis");

    res->nomen_publicatum_in = stml_attributum_capere(nodus, "nomen-publicatum-in");

    res->pollinatur_a = stml_attributum_capere(nodus, "pollinatur-a");

    res->anima = PLANTA_ROSA_ANIMA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "anima");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "vegetativa"))
            {
                res->anima = PLANTA_ROSA_ANIMA_VEGETATIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sensitiva"))
            {
                res->anima = PLANTA_ROSA_ANIMA_SENSITIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "rationalis"))
            {
                res->anima = PLANTA_ROSA_ANIMA_RATIONALIS;
            }
        }
    }

    res->massa = stml_attributum_capere(nodus, "massa");

    res->perituram = FALSUM;
    res->perituram_praesens = FALSUM;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "perituram");
        si (v)
        {
            res->perituram = (b32)chorda_aequalis_literis(*v, "verum");
            res->perituram_praesens = VERUM;
        }
    }

    res->status_conditio = PLANTA_ROSA_STATUS_CONDITIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-conditio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "recens"))
            {
                res->status_conditio = PLANTA_ROSA_STATUS_CONDITIO_RECENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "vetus"))
            {
                res->status_conditio = PLANTA_ROSA_STATUS_CONDITIO_VETUS;
            }
            alioquin si (chorda_aequalis_literis(*v, "corrupta"))
            {
                res->status_conditio = PLANTA_ROSA_STATUS_CONDITIO_CORRUPTA;
            }
        }
    }

    res->sita_in = stml_attributum_capere(nodus, "sita-in");
    res->radix = NIHIL;
    res->caulis = NIHIL;
    res->folium = NIHIL;
    res->flos = NIHIL;
    res->fructus = NIHIL;
    res->semen = NIHIL;
    res->generat = xar_creare(piscina, (i32)magnitudo(PlantaRosaGenerat*));
    res->laborat = xar_creare(piscina, (i32)magnitudo(PlantaRosaLaborat*));
    res->historia = NIHIL;
    res->nota = xar_creare(piscina, (i32)magnitudo(PlantaNota*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "radix"))
            {
                si (!res->radix)
                {
                    res->radix =
                        planta_rosa_radix_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "caulis"))
            {
                si (!res->caulis)
                {
                    res->caulis =
                        planta_rosa_caulis_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "folium"))
            {
                si (!res->folium)
                {
                    res->folium =
                        planta_rosa_folium_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "flos"))
            {
                si (!res->flos)
                {
                    res->flos =
                        planta_rosa_flos_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "fructus"))
            {
                si (!res->fructus)
                {
                    res->fructus =
                        planta_rosa_fructus_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "semen"))
            {
                si (!res->semen)
                {
                    res->semen =
                        planta_rosa_semen_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "generat"))
            {
                PlantaRosaGenerat* filius;
                PlantaRosaGenerat** locus;

                filius = planta_rosa_generat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaGenerat**)xar_addere(res->generat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "laborat"))
            {
                PlantaRosaLaborat* filius;
                PlantaRosaLaborat** locus;

                filius = planta_rosa_laborat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaLaborat**)xar_addere(res->laborat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "historia"))
            {
                si (!res->historia)
                {
                    res->historia =
                        planta_rosa_historia_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "nota"))
            {
                PlantaNota* filius;
                PlantaNota** locus;

                filius = planta_nota_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaNota**)xar_addere(res->nota);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaRosaRadix*
planta_rosa_radix_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaRadix* res;

    (vacuum)intern;
    res = (PlantaRosaRadix*)piscina_allocare(piscina,
        magnitudo(PlantaRosaRadix));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaCaulis*
planta_rosa_caulis_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaulis* res;

    (vacuum)intern;
    res = (PlantaRosaCaulis*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaulis));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaFolium*
planta_rosa_folium_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaFolium* res;

    (vacuum)intern;
    res = (PlantaRosaFolium*)piscina_allocare(piscina,
        magnitudo(PlantaRosaFolium));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaFlos*
planta_rosa_flos_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaFlos* res;

    (vacuum)intern;
    res = (PlantaRosaFlos*)piscina_allocare(piscina,
        magnitudo(PlantaRosaFlos));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaFructus*
planta_rosa_fructus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaFructus* res;

    (vacuum)intern;
    res = (PlantaRosaFructus*)piscina_allocare(piscina,
        magnitudo(PlantaRosaFructus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaSemen*
planta_rosa_semen_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaSemen* res;

    (vacuum)intern;
    res = (PlantaRosaSemen*)piscina_allocare(piscina,
        magnitudo(PlantaRosaSemen));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaGenerat*
planta_rosa_generat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaGenerat* res;

    (vacuum)intern;
    res = (PlantaRosaGenerat*)piscina_allocare(piscina,
        magnitudo(PlantaRosaGenerat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaRosaLaborat*
planta_rosa_laborat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaLaborat* res;

    (vacuum)intern;
    res = (PlantaRosaLaborat*)piscina_allocare(piscina,
        magnitudo(PlantaRosaLaborat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaRosaHistoria*
planta_rosa_historia_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaHistoria* res;

    (vacuum)intern;
    res = (PlantaRosaHistoria*)piscina_allocare(piscina,
        magnitudo(PlantaRosaHistoria));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->eventum = xar_creare(piscina, (i32)magnitudo(PlantaHistoriaEventum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "eventum"))
            {
                PlantaHistoriaEventum* filius;
                PlantaHistoriaEventum** locus;

                filius = planta_historia_eventum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaHistoriaEventum**)xar_addere(res->eventum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaRosaCanina*
planta_rosa_canina_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCanina* res;

    (vacuum)intern;
    res = (PlantaRosaCanina*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCanina));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nomen_v = stml_attributum_capere(nodus, "nomen");

    res->habitus = PLANTA_ROSA_CANINA_HABITUS_FRUTEX;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "habitus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "herba"))
            {
                res->habitus = PLANTA_ROSA_CANINA_HABITUS_HERBA;
            }
            alioquin si (chorda_aequalis_literis(*v, "frutex"))
            {
                res->habitus = PLANTA_ROSA_CANINA_HABITUS_FRUTEX;
            }
            alioquin si (chorda_aequalis_literis(*v, "arbor"))
            {
                res->habitus = PLANTA_ROSA_CANINA_HABITUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*v, "scandens"))
            {
                res->habitus = PLANTA_ROSA_CANINA_HABITUS_SCANDENS;
            }
        }
    }

    res->durata = PLANTA_ROSA_CANINA_DURATA_PERENNIS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "durata");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "annua"))
            {
                res->durata = PLANTA_ROSA_CANINA_DURATA_ANNUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "biennis"))
            {
                res->durata = PLANTA_ROSA_CANINA_DURATA_BIENNIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "perennis"))
            {
                res->durata = PLANTA_ROSA_CANINA_DURATA_PERENNIS;
            }
        }
    }

    res->folia_permanentia = PLANTA_ROSA_CANINA_FOLIA_PERMANENTIA_DECIDUA;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "folia-permanentia");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "decidua"))
            {
                res->folia_permanentia = PLANTA_ROSA_CANINA_FOLIA_PERMANENTIA_DECIDUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sempervirens"))
            {
                res->folia_permanentia = PLANTA_ROSA_CANINA_FOLIA_PERMANENTIA_SEMPERVIRENS;
            }
        }
    }

    res->altitudo_matura = 3;
    res->altitudo_matura_praesens = FALSUM;
    {
        chorda* v;
        s32 tv;

        v = stml_attributum_capere(nodus, "altitudo-matura");
        si (v && chorda_ut_s32(*v, &tv))
        {
            res->altitudo_matura = (s64)tv;
            res->altitudo_matura_praesens = VERUM;
        }
    }

    res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-vita");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "semen"))
            {
                res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_SEMEN;
            }
            alioquin si (chorda_aequalis_literis(*v, "germinans"))
            {
                res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_GERMINANS;
            }
            alioquin si (chorda_aequalis_literis(*v, "iuvenis"))
            {
                res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_IUVENIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "florens"))
            {
                res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_FLORENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructifera"))
            {
                res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_FRUCTIFERA;
            }
            alioquin si (chorda_aequalis_literis(*v, "quiescens"))
            {
                res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_QUIESCENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "mortua"))
            {
                res->status_vita = PLANTA_ROSA_CANINA_STATUS_VITA_MORTUA;
            }
        }
    }

    res->auctor_nominis = stml_attributum_capere(nodus, "auctor-nominis");

    res->nomen_publicatum_in = stml_attributum_capere(nodus, "nomen-publicatum-in");

    res->pollinatur_a = stml_attributum_capere(nodus, "pollinatur-a");

    res->anima = PLANTA_ROSA_CANINA_ANIMA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "anima");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "vegetativa"))
            {
                res->anima = PLANTA_ROSA_CANINA_ANIMA_VEGETATIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sensitiva"))
            {
                res->anima = PLANTA_ROSA_CANINA_ANIMA_SENSITIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "rationalis"))
            {
                res->anima = PLANTA_ROSA_CANINA_ANIMA_RATIONALIS;
            }
        }
    }

    res->massa = stml_attributum_capere(nodus, "massa");

    res->perituram = FALSUM;
    res->perituram_praesens = FALSUM;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "perituram");
        si (v)
        {
            res->perituram = (b32)chorda_aequalis_literis(*v, "verum");
            res->perituram_praesens = VERUM;
        }
    }

    res->status_conditio = PLANTA_ROSA_CANINA_STATUS_CONDITIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-conditio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "recens"))
            {
                res->status_conditio = PLANTA_ROSA_CANINA_STATUS_CONDITIO_RECENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "vetus"))
            {
                res->status_conditio = PLANTA_ROSA_CANINA_STATUS_CONDITIO_VETUS;
            }
            alioquin si (chorda_aequalis_literis(*v, "corrupta"))
            {
                res->status_conditio = PLANTA_ROSA_CANINA_STATUS_CONDITIO_CORRUPTA;
            }
        }
    }

    res->sita_in = stml_attributum_capere(nodus, "sita-in");
    res->radix = NIHIL;
    res->caulis = NIHIL;
    res->folium = NIHIL;
    res->flos = NIHIL;
    res->fructus = NIHIL;
    res->semen = NIHIL;
    res->generat = xar_creare(piscina, (i32)magnitudo(PlantaRosaCaninaGenerat*));
    res->laborat = xar_creare(piscina, (i32)magnitudo(PlantaRosaCaninaLaborat*));
    res->historia = NIHIL;
    res->nota = xar_creare(piscina, (i32)magnitudo(PlantaNota*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "radix"))
            {
                si (!res->radix)
                {
                    res->radix =
                        planta_rosa_canina_radix_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "caulis"))
            {
                si (!res->caulis)
                {
                    res->caulis =
                        planta_rosa_canina_caulis_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "folium"))
            {
                si (!res->folium)
                {
                    res->folium =
                        planta_rosa_canina_folium_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "flos"))
            {
                si (!res->flos)
                {
                    res->flos =
                        planta_rosa_canina_flos_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "fructus"))
            {
                si (!res->fructus)
                {
                    res->fructus =
                        planta_rosa_canina_fructus_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "semen"))
            {
                si (!res->semen)
                {
                    res->semen =
                        planta_rosa_canina_semen_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "generat"))
            {
                PlantaRosaCaninaGenerat* filius;
                PlantaRosaCaninaGenerat** locus;

                filius = planta_rosa_canina_generat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaCaninaGenerat**)xar_addere(res->generat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "laborat"))
            {
                PlantaRosaCaninaLaborat* filius;
                PlantaRosaCaninaLaborat** locus;

                filius = planta_rosa_canina_laborat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaRosaCaninaLaborat**)xar_addere(res->laborat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "historia"))
            {
                si (!res->historia)
                {
                    res->historia =
                        planta_rosa_canina_historia_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "nota"))
            {
                PlantaNota* filius;
                PlantaNota** locus;

                filius = planta_nota_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaNota**)xar_addere(res->nota);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaRosaCaninaRadix*
planta_rosa_canina_radix_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaRadix* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaRadix*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaRadix));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaCaninaCaulis*
planta_rosa_canina_caulis_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaCaulis* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaCaulis*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaCaulis));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaCaninaFolium*
planta_rosa_canina_folium_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaFolium* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaFolium*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaFolium));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaCaninaFlos*
planta_rosa_canina_flos_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaFlos* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaFlos*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaFlos));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaCaninaFructus*
planta_rosa_canina_fructus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaFructus* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaFructus*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaFructus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaCaninaSemen*
planta_rosa_canina_semen_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaSemen* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaSemen*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaSemen));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaRosaCaninaGenerat*
planta_rosa_canina_generat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaGenerat* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaGenerat*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaGenerat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaRosaCaninaLaborat*
planta_rosa_canina_laborat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaLaborat* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaLaborat*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaLaborat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaRosaCaninaHistoria*
planta_rosa_canina_historia_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaRosaCaninaHistoria* res;

    (vacuum)intern;
    res = (PlantaRosaCaninaHistoria*)piscina_allocare(piscina,
        magnitudo(PlantaRosaCaninaHistoria));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->eventum = xar_creare(piscina, (i32)magnitudo(PlantaHistoriaEventum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "eventum"))
            {
                PlantaHistoriaEventum* filius;
                PlantaHistoriaEventum** locus;

                filius = planta_historia_eventum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaHistoriaEventum**)xar_addere(res->eventum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaMalus*
planta_malus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalus* res;

    (vacuum)intern;
    res = (PlantaMalus*)piscina_allocare(piscina,
        magnitudo(PlantaMalus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nomen_v = stml_attributum_capere(nodus, "nomen");

    res->habitus = PLANTA_MALUS_HABITUS_ARBOR;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "habitus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "herba"))
            {
                res->habitus = PLANTA_MALUS_HABITUS_HERBA;
            }
            alioquin si (chorda_aequalis_literis(*v, "frutex"))
            {
                res->habitus = PLANTA_MALUS_HABITUS_FRUTEX;
            }
            alioquin si (chorda_aequalis_literis(*v, "arbor"))
            {
                res->habitus = PLANTA_MALUS_HABITUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*v, "scandens"))
            {
                res->habitus = PLANTA_MALUS_HABITUS_SCANDENS;
            }
        }
    }

    res->durata = PLANTA_MALUS_DURATA_PERENNIS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "durata");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "annua"))
            {
                res->durata = PLANTA_MALUS_DURATA_ANNUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "biennis"))
            {
                res->durata = PLANTA_MALUS_DURATA_BIENNIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "perennis"))
            {
                res->durata = PLANTA_MALUS_DURATA_PERENNIS;
            }
        }
    }

    res->folia_permanentia = PLANTA_MALUS_FOLIA_PERMANENTIA_DECIDUA;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "folia-permanentia");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "decidua"))
            {
                res->folia_permanentia = PLANTA_MALUS_FOLIA_PERMANENTIA_DECIDUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sempervirens"))
            {
                res->folia_permanentia = PLANTA_MALUS_FOLIA_PERMANENTIA_SEMPERVIRENS;
            }
        }
    }

    res->altitudo_matura = ZEPHYRUM;
    res->altitudo_matura_praesens = FALSUM;
    {
        chorda* v;
        s32 tv;

        v = stml_attributum_capere(nodus, "altitudo-matura");
        si (v && chorda_ut_s32(*v, &tv))
        {
            res->altitudo_matura = (s64)tv;
            res->altitudo_matura_praesens = VERUM;
        }
    }

    res->status_vita = PLANTA_MALUS_STATUS_VITA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-vita");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "semen"))
            {
                res->status_vita = PLANTA_MALUS_STATUS_VITA_SEMEN;
            }
            alioquin si (chorda_aequalis_literis(*v, "germinans"))
            {
                res->status_vita = PLANTA_MALUS_STATUS_VITA_GERMINANS;
            }
            alioquin si (chorda_aequalis_literis(*v, "iuvenis"))
            {
                res->status_vita = PLANTA_MALUS_STATUS_VITA_IUVENIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "florens"))
            {
                res->status_vita = PLANTA_MALUS_STATUS_VITA_FLORENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructifera"))
            {
                res->status_vita = PLANTA_MALUS_STATUS_VITA_FRUCTIFERA;
            }
            alioquin si (chorda_aequalis_literis(*v, "quiescens"))
            {
                res->status_vita = PLANTA_MALUS_STATUS_VITA_QUIESCENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "mortua"))
            {
                res->status_vita = PLANTA_MALUS_STATUS_VITA_MORTUA;
            }
        }
    }

    res->auctor_nominis = stml_attributum_capere(nodus, "auctor-nominis");

    res->nomen_publicatum_in = stml_attributum_capere(nodus, "nomen-publicatum-in");

    res->pollinatur_a = stml_attributum_capere(nodus, "pollinatur-a");

    res->anima = PLANTA_MALUS_ANIMA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "anima");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "vegetativa"))
            {
                res->anima = PLANTA_MALUS_ANIMA_VEGETATIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sensitiva"))
            {
                res->anima = PLANTA_MALUS_ANIMA_SENSITIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "rationalis"))
            {
                res->anima = PLANTA_MALUS_ANIMA_RATIONALIS;
            }
        }
    }

    res->massa = stml_attributum_capere(nodus, "massa");

    res->perituram = FALSUM;
    res->perituram_praesens = FALSUM;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "perituram");
        si (v)
        {
            res->perituram = (b32)chorda_aequalis_literis(*v, "verum");
            res->perituram_praesens = VERUM;
        }
    }

    res->status_conditio = PLANTA_MALUS_STATUS_CONDITIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-conditio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "recens"))
            {
                res->status_conditio = PLANTA_MALUS_STATUS_CONDITIO_RECENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "vetus"))
            {
                res->status_conditio = PLANTA_MALUS_STATUS_CONDITIO_VETUS;
            }
            alioquin si (chorda_aequalis_literis(*v, "corrupta"))
            {
                res->status_conditio = PLANTA_MALUS_STATUS_CONDITIO_CORRUPTA;
            }
        }
    }

    res->sita_in = stml_attributum_capere(nodus, "sita-in");
    res->radix = NIHIL;
    res->caulis = NIHIL;
    res->folium = NIHIL;
    res->flos = NIHIL;
    res->fructus = NIHIL;
    res->semen = NIHIL;
    res->generat = xar_creare(piscina, (i32)magnitudo(PlantaMalusGenerat*));
    res->laborat = xar_creare(piscina, (i32)magnitudo(PlantaMalusLaborat*));
    res->historia = NIHIL;
    res->nota = xar_creare(piscina, (i32)magnitudo(PlantaNota*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "radix"))
            {
                si (!res->radix)
                {
                    res->radix =
                        planta_malus_radix_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "caulis"))
            {
                si (!res->caulis)
                {
                    res->caulis =
                        planta_malus_caulis_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "folium"))
            {
                si (!res->folium)
                {
                    res->folium =
                        planta_malus_folium_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "flos"))
            {
                si (!res->flos)
                {
                    res->flos =
                        planta_malus_flos_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "fructus"))
            {
                si (!res->fructus)
                {
                    res->fructus =
                        planta_malus_fructus_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "semen"))
            {
                si (!res->semen)
                {
                    res->semen =
                        planta_malus_semen_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "generat"))
            {
                PlantaMalusGenerat* filius;
                PlantaMalusGenerat** locus;

                filius = planta_malus_generat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaMalusGenerat**)xar_addere(res->generat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "laborat"))
            {
                PlantaMalusLaborat* filius;
                PlantaMalusLaborat** locus;

                filius = planta_malus_laborat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaMalusLaborat**)xar_addere(res->laborat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "historia"))
            {
                si (!res->historia)
                {
                    res->historia =
                        planta_malus_historia_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "nota"))
            {
                PlantaNota* filius;
                PlantaNota** locus;

                filius = planta_nota_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaNota**)xar_addere(res->nota);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaMalusRadix*
planta_malus_radix_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusRadix* res;

    (vacuum)intern;
    res = (PlantaMalusRadix*)piscina_allocare(piscina,
        magnitudo(PlantaMalusRadix));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusCaulis*
planta_malus_caulis_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusCaulis* res;

    (vacuum)intern;
    res = (PlantaMalusCaulis*)piscina_allocare(piscina,
        magnitudo(PlantaMalusCaulis));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusFolium*
planta_malus_folium_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusFolium* res;

    (vacuum)intern;
    res = (PlantaMalusFolium*)piscina_allocare(piscina,
        magnitudo(PlantaMalusFolium));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusFlos*
planta_malus_flos_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusFlos* res;

    (vacuum)intern;
    res = (PlantaMalusFlos*)piscina_allocare(piscina,
        magnitudo(PlantaMalusFlos));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusFructus*
planta_malus_fructus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusFructus* res;

    (vacuum)intern;
    res = (PlantaMalusFructus*)piscina_allocare(piscina,
        magnitudo(PlantaMalusFructus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusSemen*
planta_malus_semen_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusSemen* res;

    (vacuum)intern;
    res = (PlantaMalusSemen*)piscina_allocare(piscina,
        magnitudo(PlantaMalusSemen));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusGenerat*
planta_malus_generat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusGenerat* res;

    (vacuum)intern;
    res = (PlantaMalusGenerat*)piscina_allocare(piscina,
        magnitudo(PlantaMalusGenerat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaMalusLaborat*
planta_malus_laborat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusLaborat* res;

    (vacuum)intern;
    res = (PlantaMalusLaborat*)piscina_allocare(piscina,
        magnitudo(PlantaMalusLaborat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaMalusHistoria*
planta_malus_historia_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusHistoria* res;

    (vacuum)intern;
    res = (PlantaMalusHistoria*)piscina_allocare(piscina,
        magnitudo(PlantaMalusHistoria));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->eventum = xar_creare(piscina, (i32)magnitudo(PlantaHistoriaEventum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "eventum"))
            {
                PlantaHistoriaEventum* filius;
                PlantaHistoriaEventum** locus;

                filius = planta_historia_eventum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaHistoriaEventum**)xar_addere(res->eventum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaMalusDomestica*
planta_malus_domestica_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomestica* res;

    (vacuum)intern;
    res = (PlantaMalusDomestica*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomestica));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nomen_v = stml_attributum_capere(nodus, "nomen");

    res->habitus = PLANTA_MALUS_DOMESTICA_HABITUS_ARBOR;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "habitus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "herba"))
            {
                res->habitus = PLANTA_MALUS_DOMESTICA_HABITUS_HERBA;
            }
            alioquin si (chorda_aequalis_literis(*v, "frutex"))
            {
                res->habitus = PLANTA_MALUS_DOMESTICA_HABITUS_FRUTEX;
            }
            alioquin si (chorda_aequalis_literis(*v, "arbor"))
            {
                res->habitus = PLANTA_MALUS_DOMESTICA_HABITUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*v, "scandens"))
            {
                res->habitus = PLANTA_MALUS_DOMESTICA_HABITUS_SCANDENS;
            }
        }
    }

    res->durata = PLANTA_MALUS_DOMESTICA_DURATA_PERENNIS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "durata");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "annua"))
            {
                res->durata = PLANTA_MALUS_DOMESTICA_DURATA_ANNUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "biennis"))
            {
                res->durata = PLANTA_MALUS_DOMESTICA_DURATA_BIENNIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "perennis"))
            {
                res->durata = PLANTA_MALUS_DOMESTICA_DURATA_PERENNIS;
            }
        }
    }

    res->folia_permanentia = PLANTA_MALUS_DOMESTICA_FOLIA_PERMANENTIA_DECIDUA;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "folia-permanentia");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "decidua"))
            {
                res->folia_permanentia = PLANTA_MALUS_DOMESTICA_FOLIA_PERMANENTIA_DECIDUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sempervirens"))
            {
                res->folia_permanentia = PLANTA_MALUS_DOMESTICA_FOLIA_PERMANENTIA_SEMPERVIRENS;
            }
        }
    }

    res->altitudo_matura = 5;
    res->altitudo_matura_praesens = FALSUM;
    {
        chorda* v;
        s32 tv;

        v = stml_attributum_capere(nodus, "altitudo-matura");
        si (v && chorda_ut_s32(*v, &tv))
        {
            res->altitudo_matura = (s64)tv;
            res->altitudo_matura_praesens = VERUM;
        }
    }

    res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-vita");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "semen"))
            {
                res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_SEMEN;
            }
            alioquin si (chorda_aequalis_literis(*v, "germinans"))
            {
                res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_GERMINANS;
            }
            alioquin si (chorda_aequalis_literis(*v, "iuvenis"))
            {
                res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_IUVENIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "florens"))
            {
                res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_FLORENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructifera"))
            {
                res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_FRUCTIFERA;
            }
            alioquin si (chorda_aequalis_literis(*v, "quiescens"))
            {
                res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_QUIESCENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "mortua"))
            {
                res->status_vita = PLANTA_MALUS_DOMESTICA_STATUS_VITA_MORTUA;
            }
        }
    }

    res->auctor_nominis = stml_attributum_capere(nodus, "auctor-nominis");

    res->nomen_publicatum_in = stml_attributum_capere(nodus, "nomen-publicatum-in");

    res->pollinatur_a = stml_attributum_capere(nodus, "pollinatur-a");

    res->anima = PLANTA_MALUS_DOMESTICA_ANIMA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "anima");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "vegetativa"))
            {
                res->anima = PLANTA_MALUS_DOMESTICA_ANIMA_VEGETATIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sensitiva"))
            {
                res->anima = PLANTA_MALUS_DOMESTICA_ANIMA_SENSITIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "rationalis"))
            {
                res->anima = PLANTA_MALUS_DOMESTICA_ANIMA_RATIONALIS;
            }
        }
    }

    res->massa = stml_attributum_capere(nodus, "massa");

    res->perituram = FALSUM;
    res->perituram_praesens = FALSUM;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "perituram");
        si (v)
        {
            res->perituram = (b32)chorda_aequalis_literis(*v, "verum");
            res->perituram_praesens = VERUM;
        }
    }

    res->status_conditio = PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-conditio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "recens"))
            {
                res->status_conditio = PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_RECENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "vetus"))
            {
                res->status_conditio = PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_VETUS;
            }
            alioquin si (chorda_aequalis_literis(*v, "corrupta"))
            {
                res->status_conditio = PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_CORRUPTA;
            }
        }
    }

    res->sita_in = stml_attributum_capere(nodus, "sita-in");
    res->radix = NIHIL;
    res->caulis = NIHIL;
    res->folium = NIHIL;
    res->flos = NIHIL;
    res->fructus = NIHIL;
    res->semen = NIHIL;
    res->generat = xar_creare(piscina, (i32)magnitudo(PlantaMalusDomesticaGenerat*));
    res->laborat = xar_creare(piscina, (i32)magnitudo(PlantaMalusDomesticaLaborat*));
    res->historia = NIHIL;
    res->nota = xar_creare(piscina, (i32)magnitudo(PlantaNota*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "radix"))
            {
                si (!res->radix)
                {
                    res->radix =
                        planta_malus_domestica_radix_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "caulis"))
            {
                si (!res->caulis)
                {
                    res->caulis =
                        planta_malus_domestica_caulis_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "folium"))
            {
                si (!res->folium)
                {
                    res->folium =
                        planta_malus_domestica_folium_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "flos"))
            {
                si (!res->flos)
                {
                    res->flos =
                        planta_malus_domestica_flos_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "fructus"))
            {
                si (!res->fructus)
                {
                    res->fructus =
                        planta_malus_domestica_fructus_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "semen"))
            {
                si (!res->semen)
                {
                    res->semen =
                        planta_malus_domestica_semen_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "generat"))
            {
                PlantaMalusDomesticaGenerat* filius;
                PlantaMalusDomesticaGenerat** locus;

                filius = planta_malus_domestica_generat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaMalusDomesticaGenerat**)xar_addere(res->generat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "laborat"))
            {
                PlantaMalusDomesticaLaborat* filius;
                PlantaMalusDomesticaLaborat** locus;

                filius = planta_malus_domestica_laborat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaMalusDomesticaLaborat**)xar_addere(res->laborat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "historia"))
            {
                si (!res->historia)
                {
                    res->historia =
                        planta_malus_domestica_historia_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "nota"))
            {
                PlantaNota* filius;
                PlantaNota** locus;

                filius = planta_nota_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaNota**)xar_addere(res->nota);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaMalusDomesticaRadix*
planta_malus_domestica_radix_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaRadix* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaRadix*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaRadix));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusDomesticaCaulis*
planta_malus_domestica_caulis_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaCaulis* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaCaulis*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaCaulis));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusDomesticaFolium*
planta_malus_domestica_folium_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaFolium* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaFolium*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaFolium));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusDomesticaFlos*
planta_malus_domestica_flos_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaFlos* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaFlos*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaFlos));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusDomesticaFructus*
planta_malus_domestica_fructus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaFructus* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaFructus*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaFructus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusDomesticaSemen*
planta_malus_domestica_semen_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaSemen* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaSemen*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaSemen));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaMalusDomesticaGenerat*
planta_malus_domestica_generat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaGenerat* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaGenerat*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaGenerat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaMalusDomesticaLaborat*
planta_malus_domestica_laborat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaLaborat* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaLaborat*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaLaborat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaMalusDomesticaHistoria*
planta_malus_domestica_historia_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaMalusDomesticaHistoria* res;

    (vacuum)intern;
    res = (PlantaMalusDomesticaHistoria*)piscina_allocare(piscina,
        magnitudo(PlantaMalusDomesticaHistoria));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->eventum = xar_creare(piscina, (i32)magnitudo(PlantaHistoriaEventum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "eventum"))
            {
                PlantaHistoriaEventum* filius;
                PlantaHistoriaEventum** locus;

                filius = planta_historia_eventum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaHistoriaEventum**)xar_addere(res->eventum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaGrannySmith*
planta_granny_smith_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmith* res;

    (vacuum)intern;
    res = (PlantaGrannySmith*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmith));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nomen_v = stml_attributum_capere(nodus, "nomen");

    res->habitus = PLANTA_GRANNY_SMITH_HABITUS_ARBOR;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "habitus");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "herba"))
            {
                res->habitus = PLANTA_GRANNY_SMITH_HABITUS_HERBA;
            }
            alioquin si (chorda_aequalis_literis(*v, "frutex"))
            {
                res->habitus = PLANTA_GRANNY_SMITH_HABITUS_FRUTEX;
            }
            alioquin si (chorda_aequalis_literis(*v, "arbor"))
            {
                res->habitus = PLANTA_GRANNY_SMITH_HABITUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*v, "scandens"))
            {
                res->habitus = PLANTA_GRANNY_SMITH_HABITUS_SCANDENS;
            }
        }
    }

    res->durata = PLANTA_GRANNY_SMITH_DURATA_PERENNIS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "durata");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "annua"))
            {
                res->durata = PLANTA_GRANNY_SMITH_DURATA_ANNUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "biennis"))
            {
                res->durata = PLANTA_GRANNY_SMITH_DURATA_BIENNIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "perennis"))
            {
                res->durata = PLANTA_GRANNY_SMITH_DURATA_PERENNIS;
            }
        }
    }

    res->folia_permanentia = PLANTA_GRANNY_SMITH_FOLIA_PERMANENTIA_DECIDUA;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "folia-permanentia");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "decidua"))
            {
                res->folia_permanentia = PLANTA_GRANNY_SMITH_FOLIA_PERMANENTIA_DECIDUA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sempervirens"))
            {
                res->folia_permanentia = PLANTA_GRANNY_SMITH_FOLIA_PERMANENTIA_SEMPERVIRENS;
            }
        }
    }

    res->altitudo_matura = 5;
    res->altitudo_matura_praesens = FALSUM;
    {
        chorda* v;
        s32 tv;

        v = stml_attributum_capere(nodus, "altitudo-matura");
        si (v && chorda_ut_s32(*v, &tv))
        {
            res->altitudo_matura = (s64)tv;
            res->altitudo_matura_praesens = VERUM;
        }
    }

    res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-vita");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "semen"))
            {
                res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_SEMEN;
            }
            alioquin si (chorda_aequalis_literis(*v, "germinans"))
            {
                res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_GERMINANS;
            }
            alioquin si (chorda_aequalis_literis(*v, "iuvenis"))
            {
                res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_IUVENIS;
            }
            alioquin si (chorda_aequalis_literis(*v, "florens"))
            {
                res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_FLORENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructifera"))
            {
                res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_FRUCTIFERA;
            }
            alioquin si (chorda_aequalis_literis(*v, "quiescens"))
            {
                res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_QUIESCENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "mortua"))
            {
                res->status_vita = PLANTA_GRANNY_SMITH_STATUS_VITA_MORTUA;
            }
        }
    }

    res->auctor_nominis = stml_attributum_capere(nodus, "auctor-nominis");

    res->nomen_publicatum_in = stml_attributum_capere(nodus, "nomen-publicatum-in");

    res->pollinatur_a = stml_attributum_capere(nodus, "pollinatur-a");

    res->anima = PLANTA_GRANNY_SMITH_ANIMA_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "anima");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "vegetativa"))
            {
                res->anima = PLANTA_GRANNY_SMITH_ANIMA_VEGETATIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "sensitiva"))
            {
                res->anima = PLANTA_GRANNY_SMITH_ANIMA_SENSITIVA;
            }
            alioquin si (chorda_aequalis_literis(*v, "rationalis"))
            {
                res->anima = PLANTA_GRANNY_SMITH_ANIMA_RATIONALIS;
            }
        }
    }

    res->massa = stml_attributum_capere(nodus, "massa");

    res->perituram = FALSUM;
    res->perituram_praesens = FALSUM;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "perituram");
        si (v)
        {
            res->perituram = (b32)chorda_aequalis_literis(*v, "verum");
            res->perituram_praesens = VERUM;
        }
    }

    res->status_conditio = PLANTA_GRANNY_SMITH_STATUS_CONDITIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "status-conditio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "recens"))
            {
                res->status_conditio = PLANTA_GRANNY_SMITH_STATUS_CONDITIO_RECENS;
            }
            alioquin si (chorda_aequalis_literis(*v, "vetus"))
            {
                res->status_conditio = PLANTA_GRANNY_SMITH_STATUS_CONDITIO_VETUS;
            }
            alioquin si (chorda_aequalis_literis(*v, "corrupta"))
            {
                res->status_conditio = PLANTA_GRANNY_SMITH_STATUS_CONDITIO_CORRUPTA;
            }
        }
    }

    res->sita_in = stml_attributum_capere(nodus, "sita-in");
    res->radix = NIHIL;
    res->caulis = NIHIL;
    res->folium = NIHIL;
    res->flos = NIHIL;
    res->fructus = NIHIL;
    res->semen = NIHIL;
    res->generat = xar_creare(piscina, (i32)magnitudo(PlantaGrannySmithGenerat*));
    res->laborat = xar_creare(piscina, (i32)magnitudo(PlantaGrannySmithLaborat*));
    res->historia = NIHIL;
    res->nota = xar_creare(piscina, (i32)magnitudo(PlantaNota*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "radix"))
            {
                si (!res->radix)
                {
                    res->radix =
                        planta_granny_smith_radix_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "caulis"))
            {
                si (!res->caulis)
                {
                    res->caulis =
                        planta_granny_smith_caulis_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "folium"))
            {
                si (!res->folium)
                {
                    res->folium =
                        planta_granny_smith_folium_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "flos"))
            {
                si (!res->flos)
                {
                    res->flos =
                        planta_granny_smith_flos_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "fructus"))
            {
                si (!res->fructus)
                {
                    res->fructus =
                        planta_granny_smith_fructus_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "semen"))
            {
                si (!res->semen)
                {
                    res->semen =
                        planta_granny_smith_semen_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "generat"))
            {
                PlantaGrannySmithGenerat* filius;
                PlantaGrannySmithGenerat** locus;

                filius = planta_granny_smith_generat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaGrannySmithGenerat**)xar_addere(res->generat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "laborat"))
            {
                PlantaGrannySmithLaborat* filius;
                PlantaGrannySmithLaborat** locus;

                filius = planta_granny_smith_laborat_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaGrannySmithLaborat**)xar_addere(res->laborat);
                    *locus = filius;
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "historia"))
            {
                si (!res->historia)
                {
                    res->historia =
                        planta_granny_smith_historia_legere_nodum(
                            l, piscina, intern);
                }
            }
            alioquin si (chorda_aequalis_literis(*l->titulus,
                    "nota"))
            {
                PlantaNota* filius;
                PlantaNota** locus;

                filius = planta_nota_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaNota**)xar_addere(res->nota);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaGrannySmithRadix*
planta_granny_smith_radix_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithRadix* res;

    (vacuum)intern;
    res = (PlantaGrannySmithRadix*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithRadix));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaGrannySmithCaulis*
planta_granny_smith_caulis_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithCaulis* res;

    (vacuum)intern;
    res = (PlantaGrannySmithCaulis*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithCaulis));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaGrannySmithFolium*
planta_granny_smith_folium_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithFolium* res;

    (vacuum)intern;
    res = (PlantaGrannySmithFolium*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithFolium));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaGrannySmithFlos*
planta_granny_smith_flos_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithFlos* res;

    (vacuum)intern;
    res = (PlantaGrannySmithFlos*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithFlos));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaGrannySmithFructus*
planta_granny_smith_fructus_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithFructus* res;

    (vacuum)intern;
    res = (PlantaGrannySmithFructus*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithFructus));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaGrannySmithSemen*
planta_granny_smith_semen_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithSemen* res;

    (vacuum)intern;
    res = (PlantaGrannySmithSemen*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithSemen));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->nota = stml_attributum_capere(nodus, "nota");

    redde res;
}

PlantaGrannySmithGenerat*
planta_granny_smith_generat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithGenerat* res;

    (vacuum)intern;
    res = (PlantaGrannySmithGenerat*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithGenerat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaGrannySmithLaborat*
planta_granny_smith_laborat_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithLaborat* res;

    (vacuum)intern;
    res = (PlantaGrannySmithLaborat*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithLaborat));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->ad = stml_attributum_capere(nodus, "ad");

    redde res;
}

PlantaGrannySmithHistoria*
planta_granny_smith_historia_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaGrannySmithHistoria* res;

    (vacuum)intern;
    res = (PlantaGrannySmithHistoria*)piscina_allocare(piscina,
        magnitudo(PlantaGrannySmithHistoria));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->eventum = xar_creare(piscina, (i32)magnitudo(PlantaHistoriaEventum*));
    {
        i32 n;
        i32 i;

        n = stml_numerus_liberorum(nodus);
        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(nodus, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !l->titulus)
            {
                perge;
            }
            si (chorda_aequalis_literis(*l->titulus,
                    "eventum"))
            {
                PlantaHistoriaEventum* filius;
                PlantaHistoriaEventum** locus;

                filius = planta_historia_eventum_legere_nodum(l, piscina, intern);
                si (filius)
                {
                    locus = (PlantaHistoriaEventum**)xar_addere(res->eventum);
                    *locus = filius;
                }
            }
        }
    }

    redde res;
}

PlantaNota*
planta_nota_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaNota* res;

    (vacuum)intern;
    res = (PlantaNota*)piscina_allocare(piscina,
        magnitudo(PlantaNota));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;
    res->textus = chorda_praecidere(
        stml_textus_normalizatus(nodus, piscina));

    redde res;
}

PlantaHistoriaEventum*
planta_historia_eventum_legere_nodum(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    PlantaHistoriaEventum* res;

    (vacuum)intern;
    res = (PlantaHistoriaEventum*)piscina_allocare(piscina,
        magnitudo(PlantaHistoriaEventum));
    si (!res)
    {
        redde NIHIL;
    }
    res->nodus = nodus;

    res->quando = stml_attributum_capere(nodus, "quando");

    res->actio = PLANTA_HISTORIA_EVENTUM_ACTIO_ABSENS;
    {
        chorda* v;

        v = stml_attributum_capere(nodus, "actio");
        si (v)
        {
            si (chorda_aequalis_literis(*v, "germinare"))
            {
                res->actio = PLANTA_HISTORIA_EVENTUM_ACTIO_GERMINARE;
            }
            alioquin si (chorda_aequalis_literis(*v, "florere"))
            {
                res->actio = PLANTA_HISTORIA_EVENTUM_ACTIO_FLORERE;
            }
            alioquin si (chorda_aequalis_literis(*v, "fructificare"))
            {
                res->actio = PLANTA_HISTORIA_EVENTUM_ACTIO_FRUCTIFICARE;
            }
            alioquin si (chorda_aequalis_literis(*v, "propagari"))
            {
                res->actio = PLANTA_HISTORIA_EVENTUM_ACTIO_PROPAGARI;
            }
            alioquin si (chorda_aequalis_literis(*v, "nominari"))
            {
                res->actio = PLANTA_HISTORIA_EVENTUM_ACTIO_NOMINARI;
            }
        }
    }

    res->certitudo = stml_attributum_capere(nodus, "certitudo");

    res->fons = stml_attributum_capere(nodus, "fons");

    res->nota = stml_attributum_capere(nodus, "nota");
    res->textus = chorda_praecidere(
        stml_textus_normalizatus(nodus, piscina));

    redde res;
}

