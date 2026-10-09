/* fabrica_probationes_c.c - genus gradus probationes_c (fabrica-6
 * T6c): probatio C per membrum. Pars bibliothecae fabrica (fabrica-6
 * H2; vide fabrica.c). */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>


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

/* clavis statica: fontes et capita clausurae, instrumenta domus
 * (linea ingressus provenientiae binariorum), aedilis.stml (vexilla),
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
    causa                 = chorda_ex_literis("", piscina);
    clausura.instrumenta  = NIHIL;
    si (   sutura->clausura_c != NIHIL
        && sutura->clausura_c(sutura->datum, chorda_ut_cstr(
               membrum->fons, piscina), piscina, &clausura, &causa))
    {
        /* fons aut caput sub build/ = productum praecondicionis
         * (capsula speculi: tempus et commissum fert) - in clave per
         * EXSTANTIAM solam, ut porta_aedilis (approximatio nominata,
         * Franus 2026-10-06; fabrica-7 T4): contentum omni generatione
         * mutatum membra semper iterum curreret */
        per (i = ZEPHYRUM; i < xar_numerus(clausura.fontes); i++)
        {
            chorda via = *(chorda*)xar_obtinere(clausura.fontes, i);

            si (!chorda_incipit(via, chorda_ex_literis("build/",
                    piscina)))
            {
                _ingressum_c_addere(ingressus_out, "fasciculus", via,
                    piscina);
            }
        }
        per (i = ZEPHYRUM; i < xar_numerus(clausura.capita); i++)
        {
            chorda via = *(chorda*)xar_obtinere(clausura.capita, i);

            si (!chorda_incipit(via, chorda_ex_literis("build/",
                    piscina)))
            {
                _ingressum_c_addere(ingressus_out, "fasciculus", via,
                    piscina);
            }
        }
        /* fabrica-7 T2: binaria domus quae probatio currit
         * (<aedilis instrumentum/>) - identitas eorum in clave */
        per (i = ZEPHYRUM; clausura.instrumenta != NIHIL
             && i < xar_numerus(clausura.instrumenta); i++)
        {
            _ingressum_c_addere(ingressus_out, "instrumentum_domus",
                *(chorda*)xar_obtinere(clausura.instrumenta, i),
                piscina);
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


/* ==================================================
 * SECTIONES (fabrica-7 T1): plagula CREDO_VERDICTA (<area>credo.tsv)
 * legitur post cursum; suita non conversa (plagula absens) -> plagula
 * SYNTHETICA a gradu scripta, sectio una 'totum' (exitus ex codice,
 * numeri ex compendio effusionis) - lector unus, forma una
 * ================================================== */

hic_manens constans character* constans _exitus_sectionis[] = {
    "TRANSIIT", "FRACTA", "VACUA", "ABORTA", "OMISSA", "NOTA_FRACTA",
    "INOPINATA", NIHIL
};

/* campus effugitus ('\t' '\n' '\\' ut credo scribit) -> literae */
interior chorda
_campum_solvere (
      chorda  campus,
     Piscina* piscina)
{
    ChordaAedificator* aedificator;
                  i32  i;

    aedificator = chorda_aedificator_creare(piscina, campus.mensura
        + I);
    si (aedificator == NIHIL)
    {
        redde campus;
    }
    per (i = ZEPHYRUM; i < campus.mensura; i++)
    {
        character c = (character)campus.datum[i];

        si (c == '\\' && i + I < campus.mensura)
        {
            i++;
            c = (character)campus.datum[i];
            c = c == 't' ? '\t' : c == 'n' ? '\n' : c;
        }
        (vacuum)chorda_aedificator_appendere_character(aedificator, c);
    }
    redde chorda_aedificator_finire(aedificator);
}

/* versus -> campi per '\t' (crudi); numerus camporum (plures quam
 * 'maximus' -> maximus + I, ut deformis sit) */
interior i32
_campos_findere (
     chorda  versus,
     chorda* campi,
        i32  maximus)
{
    i32 n;
    i32 initium;
    i32 i;

    n        = ZEPHYRUM;
    initium  = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= versus.mensura; i++)
    {
        si (i < versus.mensura && versus.datum[i] != '\t')
        {
            perge;
        }
        si (n >= maximus)
        {
            redde maximus + I;
        }
        campi[n] = chorda_sectio(versus, initium, i);
        n++;
        initium = i + I;
    }
    redde n;
}

/* numerus decimalis (cifrae solae, IX ad summum) */
interior b32
_numerum_legere (
    chorda  campus,
       i32* numerus_out)
{
    i32 valor;
    i32 i;

    si (campus.mensura == ZEPHYRUM || campus.mensura > IX)
    {
        redde FALSUM;
    }
    valor = ZEPHYRUM;
    per (i = ZEPHYRUM; i < campus.mensura; i++)
    {
        si (campus.datum[i] < '0' || campus.datum[i] > '9')
        {
            redde FALSUM;
        }
        valor = valor * X + (i32)(campus.datum[i] - '0');
    }
    *numerus_out = valor;
    redde VERUM;
}

interior b32
_exitum_noscere (
    chorda exitus)
{
    i32 i;

    per (i = ZEPHYRUM; _exitus_sectionis[i] != NIHIL; i++)
    {
        si (chorda_aequalis_literis(exitus, _exitus_sectionis[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* "<via>:<linea>: <quid>" */
interior chorda
_lineam_recusare (
    constans character* via,
                   i32  linea,
    constans character* quid,
               Piscina* piscina)
{
    character sedes[XXXII];

    sprintf(sedes, ":%u: ", (insignatus integer)linea);
    redde fabricae_iungere(piscina, "", chorda_concatenare(
        chorda_ex_literis(via, piscina), chorda_ex_literis(sedes,
        piscina), piscina), quid);
}

b32
fabrica_sectiones_legere (
    constans FabricaSutura* sutura,
        constans character* via,
                   Piscina* piscina,
          FabricaSectiones* sectiones_out,
                    chorda* causa_out)
{
    chorda contentum;
       i32 initium;
       i32 linea;
       i32 i;

    sectiones_out->sectiones   = xar_creare(piscina,
        (i32)magnitudo(FabricaSectio));
    sectiones_out->completa    = FALSUM;
    sectiones_out->synthetica  = FALSUM;
    si (   sutura->legere == NIHIL
        || !sutura->legere(sutura->datum, via, piscina, &contentum))
    {
        *causa_out = fabricae_iungere(piscina, "plagula sectionum "
            "absens: ", chorda_ex_literis(via, piscina), "");
        redde FALSUM;
    }
    initium  = ZEPHYRUM;
    linea    = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= contentum.mensura; i++)
    {
        chorda versus;
        chorda campi[XII];
           i32 n;

        si (i < contentum.mensura && contentum.datum[i] != '\n')
        {
            perge;
        }
        linea++;
        versus   = chorda_sectio(contentum, initium, i);
        initium  = i + I;
        si (versus.mensura == ZEPHYRUM)
        {
            perge;
        }
        n = _campos_findere(versus, campi, XI);
        si (chorda_aequalis_literis(campi[ZEPHYRUM], "SYNTHETICA"))
        {
            sectiones_out->synthetica = VERUM;
        }
        alioquin si (chorda_aequalis_literis(campi[ZEPHYRUM], "SUITA"))
        {
            si (n != VIII)
            {
                *causa_out = _lineam_recusare(via, linea, "linea SUITA "
                    "deformis (campi VIII exspectati)", piscina);
                redde FALSUM;
            }
            sectiones_out->completa = VERUM;
        }
        alioquin si (chorda_aequalis_literis(campi[ZEPHYRUM], "SECTIO"))
        {
            FabricaSectio* sectio;

            si (n != X)
            {
                *causa_out = _lineam_recusare(via, linea,
                    "linea SECTIO "
                    "deformis (campi X exspectati)", piscina);
                redde FALSUM;
            }
            sectio =
                (FabricaSectio*)xar_addere(sectiones_out->sectiones);
            sectio->titulus  = _campum_solvere(campi[II], piscina);
            sectio->exitus   = campi[III];
            si (!_exitum_noscere(sectio->exitus))
            {
                *causa_out = _lineam_recusare(via, linea, "exitus "
                    "sectionis ignotus", piscina);
                redde FALSUM;
            }
            si (   !_numerum_legere(campi[IV], &sectio->praeteriti)
                || !_numerum_legere(campi[V], &sectio->totales)
                || !_numerum_legere(campi[VI], &sectio->ms))
            {
                *causa_out = _lineam_recusare(via, linea, "numerus "
                    "sectionis deformis", piscina);
                redde FALSUM;
            }
            sectio->fractura = chorda_ex_literis("", piscina);
            si (campi[VII].mensura > ZEPHYRUM)
            {
                sectio->fractura = chorda_concatenare(
                    fabricae_iungere(piscina, "", _campum_solvere(
                    campi[VII], piscina), " "), fabricae_iungere(
                    piscina, "", _campum_solvere(campi[VIII], piscina),
                    " "), piscina);
                sectio->fractura = chorda_concatenare(sectio->fractura,
                    _campum_solvere(campi[IX], piscina), piscina);
            }
        }
    }
    redde VERUM;
}

/* numerus post 'clavis' in linea ULTIMA quae ab ea incipit (compendium
 * credo: "Totalis:    7"); FALSUM si nulla */
interior b32
_compendii_numerum (
                chorda  effusio,
    constans character* clavis,
                   i32* numerus_out)
{
    i32 longitudo;
    i32 i;
    b32 inventus;

    longitudo  = (i32)strlen(clavis);
    inventus   = FALSUM;
    per (i = ZEPHYRUM; i + longitudo <= effusio.mensura; i++)
    {
        i32 j;
        i32 finis;

        si (   (i > ZEPHYRUM && effusio.datum[i - I] != '\n')
            || memcmp(effusio.datum + i, clavis, longitudo) != ZEPHYRUM)
        {
            perge;
        }
        j = i + longitudo;
        dum (j < effusio.mensura && effusio.datum[j] == ' ')
        {
            j++;
        }
        finis = j;
        dum (   finis < effusio.mensura && effusio.datum[finis] >= '0'
             && effusio.datum[finis] <= '9')
        {
            finis++;
        }
        inventus = _numerum_legere(chorda_sectio(effusio, j, finis),
            numerus_out) || inventus;
    }
    redde inventus;
}

/* plagula synthetica: SYNTHETICA, SECTIO 'totum', SUITA (credo) */
interior chorda
_syntheticam_formare (
    constans FabricaMembrum* membrum,
                        b32  transiit,
                        i32  praeteriti,
                        i32  totales,
                        i32  ms,
                    Piscina* piscina)
{
     ChordaAedificator* a;
    constans character* exitus = transiit ? "TRANSIIT" : "FRACTA";

    a = chorda_aedificator_creare(piscina, CCLVI);
    si (a == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    (vacuum)chorda_aedificator_appendere_literis(a, "SYNTHETICA\t");
    (vacuum)chorda_aedificator_appendere_chorda(a, membrum->titulus);
    (vacuum)chorda_aedificator_appendere_literis(a, "\nSECTIO\t");
    (vacuum)chorda_aedificator_appendere_chorda(a, membrum->titulus);
    (vacuum)chorda_aedificator_appendere_literis(a, "\ttotum\t");
    (vacuum)chorda_aedificator_appendere_literis(a, exitus);
    (vacuum)chorda_aedificator_appendere_character(a, '\t');
    (vacuum)chorda_aedificator_appendere_i32(a, praeteriti);
    (vacuum)chorda_aedificator_appendere_character(a, '\t');
    (vacuum)chorda_aedificator_appendere_i32(a, totales);
    (vacuum)chorda_aedificator_appendere_character(a, '\t');
    (vacuum)chorda_aedificator_appendere_i32(a, ms);
    (vacuum)chorda_aedificator_appendere_literis(a, "\t\t\t\nSUITA\t");
    (vacuum)chorda_aedificator_appendere_chorda(a, membrum->titulus);
    (vacuum)chorda_aedificator_appendere_character(a, '\t');
    (vacuum)chorda_aedificator_appendere_literis(a, exitus);
    (vacuum)chorda_aedificator_appendere_literis(a,
        transiit ? "\t1\t1\t"
        : "\t0\t1\t");
    (vacuum)chorda_aedificator_appendere_i32(a, praeteriti);
    (vacuum)chorda_aedificator_appendere_character(a, '\t');
    (vacuum)chorda_aedificator_appendere_i32(a, totales);
    (vacuum)chorda_aedificator_appendere_character(a, '\t');
    (vacuum)chorda_aedificator_appendere_i32(a, ms);
    (vacuum)chorda_aedificator_appendere_character(a, '\n');
    redde chorda_aedificator_finire(a);
}

/* post cursum: plagula sectionum legitur (absens -> synthetica
 * scribitur) et actum explicat - transitus: nota "sectiones N/N" aut
 * "totum P/T" (deterministica: ms numquam); fractura: sectiones
 * fractae priores (III ad summum) caudae praeponuntur; SUITA absens
 * -> fractum etiam codice 0 (ruina post sectionem ultimam notatam).
 * FALSUM numquam: plagula deformis = membrum fractum. */
interior vacuum
_sectiones_explicare (
     constans FabricaSutura* sutura,
    constans FabricaMembrum* membrum,
                     chorda  area_c,
         constans character* acta,
                    Piscina* piscina,
               FabricaActum* actum_out)
{
    FabricaSectiones sectiones;
              chorda via;
              chorda contentum;
              chorda causa;
              chorda nota;
                 i32 transeuntes;
                 i32 fractae;
                 i32 i;

    via = fabricae_iungere(piscina, "", area_c, "credo.tsv");
    si (   sutura->legere == NIHIL || !sutura->legere(sutura->datum,
            chorda_ut_cstr(via, piscina), piscina, &contentum))
    {
        chorda effusio;
           i32 praeteriti  = ZEPHYRUM;
           i32 totales     = ZEPHYRUM;
           b32 numeri      = FALSUM;
        chorda synthetica;

        si (   sutura->legere != NIHIL
            && sutura->legere(sutura->datum, acta, piscina, &effusio))
        {
            numeri = _compendii_numerum(effusio, "Totalis:", &totales)
                  && _compendii_numerum(effusio, "Praeteriti:",
                     &praeteriti);
        }
        synthetica = _syntheticam_formare(membrum, actum_out->codex
            == ZEPHYRUM, praeteriti, totales, actum_out->duratio_ms,
            piscina);
        si (sutura->verdictum_ponere != NIHIL)
        {
            (vacuum)sutura->verdictum_ponere(sutura->datum,
                chorda_ut_cstr(via, piscina), &synthetica);
        }
        si (actum_out->codex == ZEPHYRUM)
        {
            character numerus[LXIV];

            sprintf(numerus, "totum %u/%u", (insignatus integer)
                praeteriti, (insignatus integer)totales);
            actum_out->cauda = chorda_ex_literis(numeri ? numerus
                : "totum", piscina);
        }
        redde;
    }
    causa = chorda_ex_literis("", piscina);
    si (!fabrica_sectiones_legere(sutura, chorda_ut_cstr(via, piscina),
            piscina, &sectiones, &causa))
    {
        actum_out->codex  = actum_out->codex == ZEPHYRUM ? I
            : actum_out->codex;
        actum_out->cauda  = chorda_concatenare(fabricae_iungere(piscina,
            "", causa, actum_out->cauda.mensura > ZEPHYRUM ? "; " : ""),
            actum_out->cauda, piscina);
        redde;
    }
    transeuntes  = ZEPHYRUM;
    fractae      = ZEPHYRUM;
    nota         = chorda_ex_literis("", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(sectiones.sectiones); i++)
    {
        FabricaSectio* s = (FabricaSectio*)xar_obtinere(
            sectiones.sectiones, i);
           character numeri[LXIV];

        si (chorda_aequalis_literis(s->exitus, "TRANSIIT"))
        {
            transeuntes++;
            perge;
        }
        fractae++;
        si (fractae > III)
        {
            perge;
        }
        sprintf(numeri, " %u/%u", (insignatus integer)s->praeteriti,
            (insignatus integer)s->totales);
        nota = fabricae_iungere(piscina, "", nota, fractae > I ? ", "
            : "");
        nota = chorda_concatenare(nota, fabricae_iungere(piscina,
            "sectio '", s->titulus, "' "), piscina);
        nota = chorda_concatenare(nota, fabricae_iungere(piscina, "",
            s->exitus, numeri), piscina);
        si (s->fractura.mensura > ZEPHYRUM)
        {
            nota = chorda_concatenare(nota, fabricae_iungere(piscina,
                " ad ", s->fractura, ""), piscina);
        }
    }
    si (fractae > III)
    {
        character reliquae[XXXII];

        sprintf(reliquae, ", +%u aliae", (insignatus integer)(fractae
            - III));
        nota = fabricae_iungere(piscina, "", nota, reliquae);
    }
    si (!sectiones.completa)
    {
        nota = fabricae_iungere(piscina, "", nota, nota.mensura
            > ZEPHYRUM ? "; SUITA absens (ruina post sectionem ultimam "
            "notatam)" : "SUITA absens (ruina post sectionem ultimam "
            "notatam)");
        actum_out->codex = actum_out->codex == ZEPHYRUM ? I
            : actum_out->codex;
    }
    si (actum_out->codex == ZEPHYRUM)
    {
        character numeri[LXIV];

        sprintf(numeri, "sectiones %u/%u", (insignatus integer)
            transeuntes, (insignatus integer)xar_numerus(
            sectiones.sectiones));
        actum_out->cauda = chorda_ex_literis(numeri, piscina);
        redde;
    }
    si (nota.mensura > ZEPHYRUM)
    {
        actum_out->cauda = chorda_concatenare(fabricae_iungere(piscina,
            "", nota, actum_out->cauda.mensura > ZEPHYRUM ? "; " : ""),
            actum_out->cauda, piscina);
    }
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
                 Xar* ambitus_cursus;
              chorda  verdicta;
                 b32  positum;
  constans character* acta;
                 i32  i;

    area_c = chorda_ex_literis(area, piscina);
    si (   sutura->clausura_c == NIHIL || sutura->compilare == NIHIL
        || sutura->in_area_currere == NIHIL)
    {
        actum_out->cauda = chorda_ex_literis("sutura sine clausura_c, "
            "compilare aut in_area_currere", piscina);
        redde FALSUM;
    }
    causa                 = chorda_ex_literis("", piscina);
    clausura.instrumenta  = NIHIL;
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
    /* ambitus cursus = basis + CREDO_VERDICTA (fabrica-7 T1; gradus
     * ipse, non actio, declarat); ordo nominum servatur */
    ambitus_cursus = fabricae_xar_chordarum(piscina);
    verdicta = fabricae_iungere(piscina, "CREDO_VERDICTA=", area_c,
        "credo.tsv");
    positum = FALSUM;
    per (i = ZEPHYRUM; ambitus != NIHIL
        && i < xar_numerus(ambitus); i++)
    {
        chorda par = *(constans chorda*)xar_obtinere(ambitus, i);

        si (!positum && chorda_comparare(par, verdicta) > ZEPHYRUM)
        {
            fabricae_chordam_addere(ambitus_cursus, verdicta);
            positum = VERUM;
        }
        fabricae_chordam_addere(ambitus_cursus, par);
    }
    si (!positum)
    {
        fabricae_chordam_addere(ambitus_cursus, verdicta);
    }
    acta = chorda_ut_cstr(fabrica_acta_via(actio->titulus, piscina),
        piscina);
    si (!sutura->in_area_currere(sutura->datum, argv, area,
            ambitus_cursus, liber, acta, piscina, actum_out))
    {
        redde FALSUM;
    }
    si (actum_out->codex == ZEPHYRUM)
    {
        actum_out->cauda = chorda_ex_literis("", piscina);
    }
    _sectiones_explicare(sutura, membrum, area_c, acta, piscina,
        actum_out);
    redde VERUM;
}

constans FabricaGradus fabricae_gradus_probationes_c = {
    "probationes_c", _probationes_c_membra, _probationes_c_ingressus,
    _probationes_c_agere, VERUM, FABRICA_SUMPTUS_CARUS
};
