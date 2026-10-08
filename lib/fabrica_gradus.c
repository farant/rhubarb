/* fabrica_gradus.c - gradus fabricae (fabrica-6 T5-T7): areae,
 * ambitus basis, registrum generum graduum, explicatio membrorum,
 * areae orphanae, composita graduum et VERDICTUM compositi. Pars
 * bibliothecae fabrica (fabrica-6 H2; vide fabrica.c). */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * GRADUS (fabrica-6 T5): areae, ambitus basis, membrum agere
 * ================================================== */

#define AREAE_DIRECTORIUM "build/fabrica/area/"
#define PATH_BASIS        "/usr/bin:/bin:/usr/sbin:/sbin"

/* nomina quae basis ponit (et FABRICA_LECTIONES, protocollum libri) -
 * lectio eorum numquam recusatur */
hic_manens constans character* constans _ambitus_basis_nomina[] = {
    "PATH", "HOME", "TMPDIR", "RHUBARB_RADIX", "FABRICA_LECTIONES",
    NIHIL
};

chorda
fabrica_area_via (
     chorda  actio,
     chorda  membrum,
    Piscina* piscina)
{
    redde chorda_concatenare(fabricae_iungere(piscina,
        AREAE_DIRECTORIUM, actio,
        "/"), fabricae_iungere(piscina, "", membrum, "/"), piscina);
}

chorda
fabrica_acta_via (
     chorda  titulus,
    Piscina* piscina)
{
    redde fabricae_iungere(piscina, "build/fabrica/acta/", titulus,
        ".log");
}

/* area actionis syntheticae (titulus = "<actio>/<membrum>") */
chorda
fabricae_area_membri (
    constans FabricaActio* actio,
                  Piscina* piscina)
{
    redde fabricae_iungere(piscina, AREAE_DIRECTORIUM, actio->titulus,
        "/");
}

interior s32
_chordas_ordinare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(*(constans chorda*)a, *(constans chorda*)b);
}

interior b32
_nomen_basis (
    chorda titulus)
{
    i32 k;

    per (k = ZEPHYRUM; _ambitus_basis_nomina[k] != NIHIL; k++)
    {
        si (chorda_aequalis_literis(titulus, _ambitus_basis_nomina[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

Xar*
fabrica_ambitum_basis (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
        constans character* area,
                   Piscina* piscina)
{
       Xar* ambitus;
    chorda  valor;
    chorda  radix;
    chorda  tmp;
       i32  i;

    ambitus = fabricae_xar_chordarum(piscina);
    si (ambitus == NIHIL)
    {
        redde NIHIL;
    }
    radix = sutura->radix.mensura > ZEPHYRUM ? sutura->radix
        : chorda_ex_literis(".", piscina);
    fabricae_chordam_addere(ambitus,
        chorda_ex_literis("PATH=" PATH_BASIS,
        piscina));
    si (   sutura->ambitus != NIHIL
        && sutura->ambitus(sutura->datum, "HOME", piscina, &valor))
    {
        fabricae_chordam_addere(ambitus, fabricae_iungere(piscina,
            "HOME=", valor, ""));
    }
    fabricae_chordam_addere(ambitus, fabricae_iungere(piscina,
        "RHUBARB_RADIX=", radix,
        ""));
    /* TMPDIR absoluta ubi radix nota (processus 'cd' facere possunt) */
    tmp = fabricae_iungere(piscina, "", chorda_ex_literis(area,
        piscina),
        "tmp");
    si (sutura->radix.mensura > ZEPHYRUM)
    {
        tmp = chorda_concatenare(fabricae_iungere(piscina, "",
            sutura->radix,
            "/"),
            tmp, piscina);
    }
    fabricae_chordam_addere(ambitus, fabricae_iungere(piscina,
        "TMPDIR=", tmp, ""));
    per (i = ZEPHYRUM; actio->ambitus != NIHIL
         && i < xar_numerus(actio->ambitus); i++)
    {
        chorda titulus = *(constans chorda*)xar_obtinere(actio->ambitus,
            i);

        si (   _nomen_basis(titulus) || sutura->ambitus == NIHIL
            || !sutura->ambitus(sutura->datum, chorda_ut_cstr(titulus,
                   piscina), piscina, &valor))
        {
            perge;   /* basis vincit; absens = non positum */
        }
        fabricae_chordam_addere(ambitus,
            chorda_concatenare(fabricae_iungere(piscina,
            "",
            titulus, "="), valor, piscina));
    }
    xar_ordinare(ambitus, _chordas_ordinare);
    redde ambitus;
}

Xar*
fabrica_ambitum_non_declaratum (
    constans FabricaActio* actio,
             constans Xar* lectiones,
                  Piscina* piscina)
{
    Xar* nomina;
    i32  i;
    i32  j;

    nomina = fabricae_xar_chordarum(piscina);
    per (i = ZEPHYRUM; nomina != NIHIL && lectiones != NIHIL
         && i < xar_numerus(lectiones); i++)
    {
        constans FabricaLectio* lectio = (constans FabricaLectio*)
            xar_obtinere(lectiones, i);
                           b32 admissum;

        si (   lectio->genus != LECTIO_AMBITUS
            || _nomen_basis(lectio->via))
        {
            perge;
        }
        admissum = FALSUM;
        per (j = ZEPHYRUM; actio->ambitus != NIHIL
             && j < xar_numerus(actio->ambitus); j++)
        {
            si (chorda_aequalis(lectio->via,
                    *(constans chorda*)xar_obtinere(actio->ambitus, j)))
            {
                admissum = VERUM;
            }
        }
        per (j = ZEPHYRUM; !admissum && j < xar_numerus(nomina); j++)
        {
            si (chorda_aequalis(lectio->via,
                    *(constans chorda*)xar_obtinere(nomina, j)))
            {
                admissum = VERUM;   /* iam nominatum */
            }
        }
        si (!admissum)
        {
            fabricae_chordam_addere(nomina, lectio->via);
        }
    }
    redde nomina;
}


/* ==================================================
 * GRADUS (fabrica-6 T5): registrum, explicatio, orphana
 * ================================================== */

/* REGISTRUM GRADUUM (fabrica-6 T5; T6c: probationes_c) */
interior constans FabricaGradus* constans _genera_graduum[] = {
    &fabricae_gradus_probationes_c,
    NIHIL
};

i32
fabrica_graduum_numerus (vacuum)
{
    i32 n;

    n = ZEPHYRUM;
    dum (_genera_graduum[n] != NIHIL)
    {
        n++;
    }
    redde n;
}

constans FabricaGradus*
fabrica_gradus_obtinere (
    i32 index)
{
    si (index >= fabrica_graduum_numerus())
    {
        redde NIHIL;
    }
    redde _genera_graduum[index];
}

constans FabricaGradus*
fabrica_gradus_invenire (
    chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < fabrica_graduum_numerus(); i++)
    {
        si (chorda_aequalis_literis(titulus,
                _genera_graduum[i]->titulus))
        {
            redde _genera_graduum[i];
        }
    }
    redde NIHIL;
}

interior s32
_membra_ordinare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(((constans FabricaMembrum*)a)->titulus,
        ((constans FabricaMembrum*)b)->titulus);
}

/* actio synthetica membri: iudicium in area sua (vide caput) */
interior b32
_membrum_actionem_facere (
     constans FabricaSutura* sutura,
      constans FabricaActio* parens,
    constans FabricaMembrum* membrum,
                    Piscina* piscina,
               FabricaActio* actio_out,
                     chorda* causa_out)
{
    constans FabricaStrategia* strategia;
               FabricaMembrum* copia;
                FabricaExitus* exitus;
                       chorda  area;
                       chorda  causa;

    memset(actio_out, ZEPHYRUM, magnitudo(FabricaActio));
    copia = (FabricaMembrum*)piscina_allocare(piscina,
        magnitudo(FabricaMembrum));
    strategia =
        fabrica_strategia_invenire(chorda_ex_literis("verdictum",
        piscina));
    si (copia == NIHIL || strategia == NIHIL)
    {
        *causa_out = chorda_ex_literis("memoria deficit (membrum)",
            piscina);
        redde FALSUM;
    }
    *copia = *membrum;
    actio_out->titulus    = chorda_concatenare(fabricae_iungere(piscina,
        "",
        parens->titulus, "/"), membrum->titulus, piscina);
    actio_out->genus      = FABRICA_ACTIO_IUDICIUM;
    actio_out->lectiones  = VERUM;
    actio_out->sedes      = parens->sedes;
    actio_out->gradus     = parens->gradus;
    actio_out->attributa  = parens->attributa;
    actio_out->ambitus    = parens->ambitus;
    actio_out->post       = parens->post;
    actio_out->membrum    = copia;
    /* mandatum in clave memoriae: genus gradus et fons membri */
    actio_out->mandatum = fabricae_xar_chordarum(piscina);
    actio_out->ingressus = xar_creare(piscina,
        (i32)magnitudo(FabricaIngressus));
    actio_out->exitus = xar_creare(piscina,
        (i32)magnitudo(FabricaExitus));
    actio_out->praecondiciones = fabricae_xar_chordarum(piscina);
    actio_out->vestigia = xar_creare(piscina,
        (i32)magnitudo(FabricaLocus));
    actio_out->communia = xar_creare(piscina,
        (i32)magnitudo(FabricaLocus));
    si (   actio_out->mandatum == NIHIL || actio_out->ingressus == NIHIL
        || actio_out->exitus == NIHIL || actio_out->vestigia == NIHIL
        || actio_out->communia == NIHIL
        || actio_out->praecondiciones == NIHIL)
    {
        *causa_out = chorda_ex_literis("memoria deficit (membrum)",
            piscina);
        redde FALSUM;
    }
    fabricae_chordam_addere(actio_out->mandatum, chorda_ex_literis(
        parens->gradus->titulus, piscina));
    fabricae_chordam_addere(actio_out->mandatum, membrum->fons);
    causa = chorda_ex_literis("", piscina);
    si (   parens->gradus->ingressus != NIHIL
        && !parens->gradus->ingressus(sutura, parens, copia, piscina,
               actio_out->ingressus, &causa))
    {
        *causa_out = chorda_concatenare(fabricae_iungere(piscina, "",
            parens->sedes, ": ingressus membri "),
            fabricae_iungere(piscina, "",
            actio_out->titulus, chorda_ut_cstr(fabricae_iungere(piscina,
            ": ",
            causa, ""), piscina)), piscina);
        redde FALSUM;
    }
    area = fabrica_area_via(parens->titulus, membrum->titulus, piscina);
    exitus = (FabricaExitus*)xar_addere(actio_out->exitus);
    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    exitus->via = fabricae_iungere(piscina, "", area,
        "verdictum.txt");
    exitus->scriptura = exitus->via;
    exitus->strategia = strategia;
    exitus->genus      = fabrica_genus_invenire(chorda_ex_literis(
        strategia->genus_ordinarium, piscina));
    /* sine '/' finali: locus ARBOR 'via/' ipse addit */
    fabricae_locum_addere(actio_out->vestigia, FABRICA_LOCUS_ARBOR,
        chorda_sectio(area, ZEPHYRUM, area.mensura - I),
        chorda_ex_literis("", piscina));
    redde VERUM;
}

Xar*
fabrica_gradus_explicare (
    constans FabricaSutura* sutura,
              constans Xar* actiones,
                   Piscina* piscina,
                    chorda* causa_out)
{
    Xar* explicata;
    i32  i;
    i32  j;

    explicata = xar_creare(piscina, (i32)magnitudo(FabricaActio));
    si (explicata == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        constans FabricaActio* actio = (constans FabricaActio*)
            xar_obtinere(actiones, i);
                          Xar* membra;
                       chorda  causa;

        *(FabricaActio*)xar_addere(explicata) = *actio;
        si (actio->gradus == NIHIL || actio->membrum != NIHIL)
        {
            perge;
        }
        membra  = xar_creare(piscina, (i32)magnitudo(FabricaMembrum));
        causa   = chorda_ex_literis("", piscina);
        si (   membra == NIHIL || actio->gradus->membra == NIHIL
            || !actio->gradus->membra(sutura, actio, piscina, membra,
                   &causa))
        {
            *causa_out = chorda_concatenare(fabricae_iungere(piscina,
                "",
                actio->sedes, ": membra recusata: "), causa, piscina);
            redde NIHIL;
        }
        xar_ordinare(membra, _membra_ordinare);
        per (j = ZEPHYRUM; j < xar_numerus(membra); j++)
        {
            constans FabricaMembrum* membrum =
                (constans FabricaMembrum*)
                xar_obtinere(membra, j);
                       FabricaActio synthetica;

            si (   j > ZEPHYRUM
                && chorda_aequalis(membrum->titulus,
                       ((constans FabricaMembrum*)xar_obtinere(membra,
                       j - I))->titulus))
            {
                *causa_out =
                    chorda_concatenare(fabricae_iungere(piscina, "",
                    actio->sedes, ": membrum duplex: "),
                    fabricae_iungere(piscina,
                    "", actio->titulus,
                    chorda_ut_cstr(fabricae_iungere(piscina,
                    "/", membrum->titulus, ""), piscina)), piscina);
                redde NIHIL;
            }
            si (!_membrum_actionem_facere(sutura, actio, membrum,
                piscina,
                    &synthetica, causa_out))
            {
                redde NIHIL;
            }
            *(FabricaActio*)xar_addere(explicata) = synthetica;
        }
    }
    redde explicata;
}

/* actio gradus (parens) aut membrum titulo dato? */
interior b32
_titulus_gradus_est (
    constans Xar* actiones,
          chorda  titulus,
             b32  membrum)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        constans FabricaActio* actio = (constans FabricaActio*)
            xar_obtinere(actiones, i);

        si (   actio->gradus             != NIHIL
            && (actio->membrum != NIHIL) == membrum
            && chorda_aequalis(actio->titulus, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

Xar*
fabrica_areas_orphanas (
    constans FabricaSutura* sutura,
              constans Xar* actiones,
                   Piscina* piscina)
{
    Xar* orphana;
    Xar* actiones_areae;
    i32  i;
    i32  j;

    orphana = fabricae_xar_chordarum(piscina);
    si (   orphana == NIHIL || sutura->enumerare == NIHIL
        || !sutura->enumerare(sutura->datum, AREAE_DIRECTORIUM, piscina,
               &actiones_areae))
    {
        redde orphana;
    }
    per (i = ZEPHYRUM; i < xar_numerus(actiones_areae); i++)
    {
        chorda actio = *(constans chorda*)xar_obtinere(actiones_areae,
            i);
        chorda  directorium;
           Xar* membra;

        directorium = fabricae_iungere(piscina, AREAE_DIRECTORIUM,
            actio, "/");
        si (!_titulus_gradus_est(actiones, actio, FALSUM))
        {
            fabricae_chordam_addere(orphana, directorium);
            perge;
        }
        si (!sutura->enumerare(sutura->datum,
            chorda_ut_cstr(directorium,
                piscina), piscina, &membra))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(membra); j++)
        {
            chorda membrum = *(constans chorda*)xar_obtinere(membra, j);

            si (!_titulus_gradus_est(actiones, chorda_concatenare(
                    fabricae_iungere(piscina, "", actio, "/"), membrum,
                    piscina),
                    VERUM))
            {
                fabricae_chordam_addere(orphana,
                    fabricae_iungere(piscina, "",
                    chorda_concatenare(directorium, membrum, piscina),
                    "/"));
            }
        }
    }
    redde orphana;
}


/* ==================================================
 * COMPOSITA GRADUUM et VERDICTUM COMPOSITI (fabrica-6 T7)
 * ================================================== */

Xar*
fabrica_gradus_composita (
    constans Xar* actiones,
         Piscina* piscina)
{
    Xar* composita;
    i32  i;
    i32  j;

    composita = xar_creare(piscina, (i32)magnitudo(FabricaCompositum));
    per (i = ZEPHYRUM; composita != NIHIL && i < xar_numerus(actiones);
         i++)
    {
        constans FabricaActio* parens = (constans FabricaActio*)
            xar_obtinere(actiones, i);
           FabricaCompositum* compositum;

        si (parens->gradus == NIHIL || parens->membrum != NIHIL)
        {
            perge;
        }
        compositum = (FabricaCompositum*)xar_addere(composita);
        si (compositum == NIHIL)
        {
            redde NIHIL;
        }
        compositum->titulus  = parens->titulus;
        compositum->sedes    = parens->sedes;
        compositum->partes   = xar_creare(piscina,
            (i32)magnitudo(FabricaPars));
        per (j = ZEPHYRUM; j < xar_numerus(actiones); j++)
        {
            constans FabricaActio* membrum = (constans FabricaActio*)
                xar_obtinere(actiones, j);
                     FabricaPars* pars;

            /* membrum parentis: titulus 'parens/...' */
            si (   membrum->membrum         == NIHIL
                || membrum->titulus.mensura <= parens->titulus.mensura
                || !chorda_incipit(membrum->titulus, parens->titulus)
                || membrum->titulus.datum[parens->titulus.mensura]
                   != '/')
            {
                perge;
            }
            pars = (FabricaPars*)xar_addere(compositum->partes);
            si (pars == NIHIL)
            {
                redde NIHIL;
            }
            pars->forma    = FABRICA_PARS_ACTIO;
            pars->titulus  = membrum->titulus;
            pars->sedes    = parens->sedes;
        }
    }
    redde composita;
}

/* verdictum membri 'build/fabrica/area/<id>/verdictum.txt' -> id */
interior chorda
_partem_nominare (
     chorda  artificium,
    Piscina* piscina)
{
    chorda praefixum;
    chorda suffixum;

    praefixum  = chorda_ex_literis(AREAE_DIRECTORIUM, piscina);
    suffixum   = chorda_ex_literis("/verdictum.txt", piscina);
    si (   artificium.mensura > praefixum.mensura + suffixum.mensura
        && chorda_incipit(artificium, praefixum)
        && chorda_aequalis(chorda_sectio(artificium,
               artificium.mensura - suffixum.mensura,
               artificium.mensura), suffixum))
    {
        redde chorda_sectio(artificium, praefixum.mensura,
            artificium.mensura - suffixum.mensura);
    }
    redde artificium;
}

chorda
fabrica_compositum_verdictum (
    constans Xar* iudicia,
          chorda  titulus,
         Piscina* piscina)
{
       chorda verdictum;
       chorda nomina;
    character numeri[LXIV];
          i32 recentia  = ZEPHYRUM;
          i32 ceterae   = ZEPHYRUM;
          i32 i;

    nomina = chorda_ex_literis("", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(iudicia); i++)
    {
        constans FabricaIudicium* iudicium = (constans FabricaIudicium*)
            xar_obtinere(iudicia, i);

        si (iudicium->status == FABRICA_RECENS)
        {
            recentia++;
            perge;
        }
        si (ceterae < III)
        {
            nomina = chorda_concatenare(ceterae
                > ZEPHYRUM ? fabricae_iungere(
                piscina, "", nomina, ", ") : nomina,
                _partem_nominare(iudicium->artificium, piscina),
                piscina);
        }
        ceterae++;
    }
    sprintf(numeri, ": %u/%u", (insignatus integer)recentia,
        (insignatus integer)xar_numerus(iudicia));
    verdictum = fabricae_iungere(piscina, "", titulus, numeri);
    si (ceterae > ZEPHYRUM)
    {
        verdictum = chorda_concatenare(fabricae_iungere(piscina, "",
            verdictum,
            " - non recentia: "), nomina, piscina);
        si (ceterae > III)
        {
            verdictum = fabricae_iungere(piscina, "", verdictum, " +");
            verdictum = chorda_concatenare(verdictum,
                numerus_romanus_exprimere((i64)(ceterae - III), NIHIL,
                piscina), piscina);
        }
    }
    redde verdictum;
}
