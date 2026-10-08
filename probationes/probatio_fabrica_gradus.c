/* probatio_fabrica_gradus.c - probationes fabricae (fabrica-6 H3),
 * pars gradus: membra, areae, ambitus, post et composita,
 * probationes_c, registrum graduum
 *
 * Mundus fictus (discus in memoria, sutura ficta):
 * probationes/fabrica_mundus_fictus.h. Spec:
 * project-specs/fabrica-spec-v2.md et sequentes. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "sigillum.h"
#include "internamentum.h"
#include "fabrica.h"
#include "credo.h"
#include "fabrica_mundus_fictus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * GRADUS (fabrica-6 T5): genus gradus LUDICRUM (non registratum) et
 * sutura areae ficta. Membra = plagulae '.c' directorii 'exemplar';
 * ingressus staticus = fons membri; agere = in_area_currere cum argv
 * ("ludicrum", fons). Cursus fictus scribit liber (L fons, L
 * data/communis.txt, L productum in area, extra), productum in area,
 * plagulam alienam (si posita); codex I pro fonte fracto.
 * ================================================== */

hic_manens constans character* _gradus_fractus       = NIHIL;

hic_manens constans character* _gradus_liber_extra   = "";

hic_manens constans character* _gradus_alienum       = NIHIL;

/* T7: membrum cuius cursus productum 'post' (build/corpus.lst) legit */
hic_manens constans character* _gradus_lectio_fons   = NIHIL;

hic_manens                 i32 _gradus_cursus        = ZEPHYRUM;

hic_manens                 i32 _gradus_areae_paratae = ZEPHYRUM;

hic_manens        constans Xar* _gradus_ambitus_ultimus = NIHIL;

hic_manens            character _gradus_area_ultima[CCLVI];

interior b32
_ludicrum_membra (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       Xar* membra_out,
                    chorda* causa_out)
{
    constans FabricaAttributum* exemplar = NIHIL;
                           Xar* nomina;
                           i32  i;

    per (i = ZEPHYRUM; actio->attributa != NIHIL
         && i < xar_numerus(actio->attributa); i++)
    {
        constans FabricaAttributum* a = (constans FabricaAttributum*)
            xar_obtinere(actio->attributa, i);

        si (chorda_aequalis_literis(a->titulus, "exemplar"))
        {
            exemplar = a;
        }
    }
    si (exemplar == NIHIL)
    {
        *causa_out = chorda_ex_literis("ludicrum sine exemplari",
            piscina);
        redde FALSUM;
    }
    si (!sutura->enumerare(sutura->datum,
        chorda_ut_cstr(exemplar->valor,
            piscina), piscina, &nomina))
    {
        *causa_out = chorda_ex_literis("exemplar absens", piscina);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
                 chorda  introitus = *(chorda*)xar_obtinere(nomina, i);
         FabricaMembrum* membrum;

        si (   introitus.mensura < III
            || !chorda_aequalis_literis(chorda_sectio(introitus,
                   introitus.mensura - II, introitus.mensura), ".c"))
        {
            perge;
        }
        membrum = (FabricaMembrum*)xar_addere(membra_out);
        si (membrum == NIHIL)
        {
            redde FALSUM;
        }
        membrum->titulus  = chorda_sectio(introitus, ZEPHYRUM,
            introitus.mensura - II);
        membrum->fons     = chorda_concatenare(exemplar->valor,
            introitus,
            piscina);
    }
    redde VERUM;
}

interior b32
_ludicrum_ingressus (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
    constans FabricaMembrum* membrum,
                    Piscina* piscina,
                        Xar* ingressus_out,
                     chorda* causa_out)
{
    FabricaIngressus* ingressus;

    (vacuum)sutura;
    (vacuum)actio;
    (vacuum)causa_out;
    ingressus = (FabricaIngressus*)xar_addere(ingressus_out);
    si (ingressus == NIHIL)
    {
        redde FALSUM;
    }
    ingressus->genus    = mundi_genus("fasciculus", piscina);
    ingressus->via      = membrum->fons;
    ingressus->suffixa  = chorda_ex_literis("", piscina);
    redde VERUM;
}

interior b32
_ludicrum_agere (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
    constans FabricaMembrum* membrum,
         constans character* area,
               constans Xar* ambitus,
                    Piscina* piscina,
               FabricaActum* actum_out)
{
    Xar* argv;

    argv = xar_creare(piscina, (i32)magnitudo(chorda));
    *(chorda*)xar_addere(argv) = chorda_ex_literis("ludicrum", piscina);
    *(chorda*)xar_addere(argv) = membrum->fons;
    redde sutura->in_area_currere(sutura->datum, argv, area, ambitus,
        chorda_ut_cstr(fabrica_liber_via(actio->titulus, piscina),
            piscina),
        chorda_ut_cstr(fabrica_acta_via(actio->titulus, piscina),
            piscina),
        piscina, actum_out);
}

interior constans FabricaGradus _gradus_ludicrum = {
    "ludicrum", _ludicrum_membra, _ludicrum_ingressus, _ludicrum_agere,
    VERUM, FABRICA_SUMPTUS_VILIS
};

/* membra duplicia (recusatio 'membrum duplex') */
interior b32
_duplex_membra (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       Xar* membra_out,
                    chorda* causa_out)
{
    i32 i;

    (vacuum)sutura;
    (vacuum)actio;
    (vacuum)causa_out;
    per (i = ZEPHYRUM; i < II; i++)
    {
        FabricaMembrum* membrum = (FabricaMembrum*)xar_addere(
            membra_out);

        si (membrum == NIHIL)
        {
            redde FALSUM;
        }
        membrum->titulus  = chorda_ex_literis("idem", piscina);
        membrum->fons     = chorda_ex_literis("", piscina);
    }
    redde VERUM;
}

interior constans FabricaGradus _gradus_duplex = {
    "duplex", _duplex_membra, NIHIL, _ludicrum_agere, VERUM,
    FABRICA_SUMPTUS_VILIS
};

/* area ficta: plagulae sub area deletae (VACUA), numeratur */
interior b32
_area_parare_ficta (
                vacuum* datum,
    constans character* area)
{
     DiscusFictus* discus = (DiscusFictus*)datum;
           chorda  praefixum;
              i32  i;

    _gradus_areae_paratae++;
    praefixum = chorda_ex_literis(area, discus->piscina);
    per (i = ZEPHYRUM; i < xar_numerus(discus->fasciculi); i++)
    {
        FasciculusFictus* f = (FasciculusFictus*)xar_obtinere(
            discus->fasciculi, i);

        si (chorda_incipit(f->via, praefixum))
        {
            f->deletum = VERUM;
        }
    }
    redde VERUM;
}

interior b32
_in_area_currere_fictum (
                vacuum* datum,
          constans Xar* argv,
    constans character* area,
          constans Xar* ambitus,
    constans character* liber_via,
    constans character* acta_via,
               Piscina* piscina,
          FabricaActum* actum_out)
{
     DiscusFictus* discus = (DiscusFictus*)datum;
           chorda  fons;
        character  productum[CCLVI];
        character  liber[DXII];

    (vacuum)acta_via;
    _gradus_cursus++;
    _gradus_ambitus_ultimus = ambitus;
    sprintf(_gradus_area_ultima, "%s", area);
    fons = *(constans chorda*)xar_obtinere(argv, I);
    sprintf(productum, "%sprobatio", area);
    mundi_ponere(discus, productum, "binarium\n");
    sprintf(liber, "L\t%.*s\nL\tdata/communis.txt\nL\t%s\n%s",
        (integer)fons.mensura, (constans character*)fons.datum,
        productum, _gradus_liber_extra);
    si (   _gradus_lectio_fons != NIHIL
        && chorda_aequalis_literis(fons, _gradus_lectio_fons))
    {
        strcat(liber, "L\tbuild/corpus.lst\n");
    }
    mundi_ponere(discus, liber_via, liber);
    si (_gradus_alienum != NIHIL)
    {
        mundi_ponere(discus, _gradus_alienum, "alienum\n");
    }
    actum_out->duratio_ms  = I;
    actum_out->codex       = (   _gradus_fractus != NIHIL
                              && chorda_aequalis_literis(fons,
                                     _gradus_fractus)) ? I : ZEPHYRUM;
    actum_out->cauda       = chorda_ex_literis(actum_out->codex != 0
        ? "probatio fracta" : "", piscina);
    redde VERUM;
}

/* "N=V" in ambitu? */
interior b32
_ambitus_habet (
          constans Xar* ambitus,
    constans character* par)
{
    i32 i;

    per (i = ZEPHYRUM; ambitus != NIHIL
        && i < xar_numerus(ambitus); i++)
    {
        si (chorda_aequalis_literis(*(constans chorda*)xar_obtinere(
                ambitus, i), par))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* actio explicata per titulum (NIHIL si nulla) */
interior FabricaActio*
_actio_explicata (
          constans Xar* actiones,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; actiones != NIHIL && i < xar_numerus(actiones);
         i++)
    {
        FabricaActio* a = (FabricaActio*)xar_obtinere(actiones, i);

        si (chorda_aequalis_literis(a->titulus, titulus))
        {
            redde a;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * PROBATIONES_C (fabrica-6 T6c): sutura C ficta - clausurae per
 * tabulam, compilatio = plagula 'obj:<fons>', nexus (argv[0] "clang")
 * scribit binarium post '-o', cursus scribit librum (L
 * data/communis.txt) et codicem e fracto
 * ================================================== */

hic_manens                 i32 _c_compilationes   = ZEPHYRUM;

hic_manens                 i32 _c_nexus           = ZEPHYRUM;

hic_manens                 i32 _c_nexus_compagis  = ZEPHYRUM;

hic_manens                 i32 _c_cursus          = ZEPHYRUM;

hic_manens            character _c_cursus_ultimus[CCLVI];

hic_manens constans character* _c_fractus_cursus      = NIHIL;

hic_manens constans character* _c_fractus_compilatio  = NIHIL;

interior vacuum
_c_addere (
                   Xar* xar,
    constans character* valor,
               Piscina* piscina)
{
    *(chorda*)xar_addere(xar) = chorda_ex_literis(valor, piscina);
}

interior b32
_clausura_c_ficta (
                vacuum* datum,
    constans character* scopus,
               Piscina* piscina,
      FabricaClausuraC* clausura_out,
                chorda* causa_out)
{
    (vacuum)datum;
    clausura_out->fontes         = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->capita         = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->vexilla_nexus  = xar_creare(piscina,
        (i32)magnitudo(chorda));
    clausura_out->facultates     = xar_creare(piscina,
        (i32)magnitudo(chorda));
    _c_addere(clausura_out->fontes, scopus, piscina);
    si (strcmp(scopus, "t/probatio_a.c") == ZEPHYRUM)
    {
        _c_addere(clausura_out->fontes, "lib/x.c", piscina);
        _c_addere(clausura_out->capita, "include/x.h", piscina);
        _c_addere(clausura_out->vexilla_nexus, "-framework Cocoa",
            piscina);
    }
    alioquin si (strcmp(scopus, "t/probatio_b.c") == ZEPHYRUM)
    {
        _c_addere(clausura_out->fontes, "lib/y.c", piscina);
        _c_addere(clausura_out->capita, "include/y.h", piscina);
    }
    alioquin si (strcmp(scopus, "t/probatio_c.c") == ZEPHYRUM)
    {
        _c_addere(clausura_out->facultates, "fenestra", piscina);
    }
    alioquin
    {
        *causa_out = chorda_ex_literis("clausura ficta ignota",
            piscina);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_compilare_fictum (
                vacuum* datum,
    constans character* fons,
    constans character* obiectum,
               Piscina* piscina,
                chorda* erratum_out)
{
    character contentum[CCLVI];

    _c_compilationes++;
    si (   _c_fractus_compilatio               != NIHIL
        && strcmp(fons, _c_fractus_compilatio) == ZEPHYRUM)
    {
        *erratum_out = chorda_ex_literis("error: ficta", piscina);
        redde FALSUM;
    }
    sprintf(contentum, "obj:%s\n", fons);
    mundi_ponere((DiscusFictus*)datum, obiectum, contentum);
    redde VERUM;
}

interior b32
_in_area_currere_c (
                vacuum* datum,
          constans Xar* argv,
    constans character* area,
          constans Xar* ambitus,
    constans character* liber_via,
    constans character* acta_via,
               Piscina* piscina,
          FabricaActum* actum_out)
{
     DiscusFictus* discus = (DiscusFictus*)datum;
           chorda  primum;
              i32  i;

    (vacuum)area;
    (vacuum)ambitus;
    (vacuum)acta_via;
    primum = *(constans chorda*)xar_obtinere(argv, ZEPHYRUM);
    actum_out->duratio_ms = I;
    actum_out->codex = ZEPHYRUM;
    actum_out->cauda = chorda_ex_literis("", piscina);
    si (chorda_aequalis_literis(primum, "clang"))
    {
        b32 compages  = FALSUM;
        b32 cocoa     = FALSUM;

        _c_nexus++;
        mundi_ponere(discus, liber_via, "");
        per (i = ZEPHYRUM; i + I < xar_numerus(argv); i++)
        {
            chorda verbum = *(constans chorda*)xar_obtinere(argv, i);

            si (chorda_aequalis_literis(verbum, "-o"))
            {
                mundi_ponere(discus, chorda_ut_cstr(*(constans chorda*)
                    xar_obtinere(argv, i + I), piscina), "binarium\n");
            }
            si (chorda_aequalis_literis(verbum, "-framework"))
            {
                compages = VERUM;
            }
        }
        per (i = ZEPHYRUM; i < xar_numerus(argv); i++)
        {
            si (chorda_aequalis_literis(*(constans chorda*)xar_obtinere(
                    argv, i), "Cocoa"))
            {
                cocoa = VERUM;
            }
        }
        si (compages && cocoa)
        {
            _c_nexus_compagis++;
        }
        redde VERUM;
    }
    _c_cursus++;
    sprintf(_c_cursus_ultimus, "%.*s", (integer)primum.mensura,
        (constans character*)primum.datum);
    mundi_ponere(discus, liber_via, "L\tdata/communis.txt\n");
    si (   _c_fractus_cursus                            != NIHIL
        && strstr(_c_cursus_ultimus, _c_fractus_cursus) != NIHIL)
    {
        actum_out->codex  = I;
        actum_out->cauda  = chorda_ex_literis("probatio fracta",
            piscina);
    }
    redde VERUM;
}

/* fixa conformitatis GENERUM GRADUUM registratorum (chassis T5):
 * probationes_c = sectio PROBATIONES_C (fabrica-6 T6c) */
hic_manens constans character* constans _fixa_graduum[] = {
    "probationes_c",
    NIHIL
};


s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum(
        "probatio_fabrica_gradus", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * GRADUS (fabrica-6 T5): membra ut actiones syntheticae - id
     * stabilis, area propria, ambitus basis solus, transitus servati,
     * defectus iterum currunt, scriptura extra aream et ambitus non
     * declaratus recusati, areae orphanae nuntiatae
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio  parens;
           FabricaAttributum* attributum;
                FabricaActio* membrum_a;
                FabricaActio* membrum_b;
              FabricaSanatio* sanatio;
             FabricaIudicium  iudicium;
                         Xar* actiones;
                         Xar* explicata;
                         Xar* ordo;
                         Xar* electa;
                         Xar* sanationes;
                         Xar* ambitus;
                         Xar* orphana;
                         Xar* lectiones;
                      chorda  causa;
                         i32  cursus;
          constans character* nomina_t[] = { "a.c", "b.c",
              "notae.txt" };
          constans character* nomina_areae[] = { "probationes_t",
              "vetus_actio" };
          constans character* nomina_membrorum[] = { "a", "b", "c" };
          constans character* VERDICTUM_A =
              "build/fabrica/area/probationes_t/a/verdictum.txt";
          constans character* VERDICTUM_B =
              "build/fabrica/area/probationes_t/b/verdictum.txt";

        imprimere("\n--- Probans gradus (membra, areae, ambitus)"
            " ---\n");
        causa = chorda_ex_literis("", piscina);
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.lectiones_legere = mundi_lectiones_legere;
        sutura.lectiones_scribere = mundi_lectiones_scribere;
        sutura.ambitus = mundi_ambitus_fictum;
        sutura.species = mundi_species_ficta;
        sutura.verdictum_ponere = mundi_verdictum_ponere;
        sutura.area_parare = _area_parare_ficta;
        sutura.in_area_currere = _in_area_currere_fictum;
        sutura.exitus_noti = tabula_dispersa_creare_chorda(piscina, 16);
        mundi_ambitus_ficti[0] = "HOME";
        mundi_ambitus_ficti[1] = "/home/u";
        /* OMNIA: in ambitu fabricae, non declarata - non transit */
        mundi_ambitus_ficti[2] = "OMNIA";
        mundi_ambitus_ficti[3] = "1";
        mundi_ambitus_ficti[4] = "CREDO_FILTRUM";
        mundi_ambitus_ficti[5] = "x";
        mundi_ambitus_ficti[6] = NIHIL;
        mundi_ponere(&discus, "t/a.c", "a I\n");
        mundi_ponere(&discus, "t/b.c", "b I\n");
        mundi_ponere(&discus, "data/communis.txt", "communis I\n");
        mundi_directorium_ponere(&discus, "t/", nomina_t, III);

        memset(&parens, ZEPHYRUM, magnitudo(parens));
        parens.titulus   = chorda_ex_literis("probationes_t", piscina);
        parens.genus     = FABRICA_ACTIO_IUDICIUM;
        parens.sedes     = chorda_ex_literis("t.stml:3", piscina);
        parens.mandatum  = xar_creare(piscina, (i32)magnitudo(chorda));
        parens.ingressus  = xar_creare(piscina,
            (i32)magnitudo(FabricaIngressus));
        parens.exitus     = xar_creare(piscina,
            (i32)magnitudo(FabricaExitus));
        parens.gradus     = &_gradus_ludicrum;
        parens.attributa  = xar_creare(piscina,
            (i32)magnitudo(FabricaAttributum));
        attributum = (FabricaAttributum*)xar_addere(parens.attributa);
        attributum->titulus = chorda_ex_literis("exemplar", piscina);
        attributum->valor = chorda_ex_literis("t/", piscina);
        parens.ambitus = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(parens.ambitus) =
            chorda_ex_literis("CREDO_FILTRUM", piscina);
        actiones = xar_creare(piscina, (i32)magnitudo(FabricaActio));
        *(FabricaActio*)xar_addere(actiones) = parens;

        /* I. explicatio: parens + membra 'a', 'b' (notae.txt non
         * membrum), ordine, id stabilis, area, verdictum, vestigium */
        explicata = fabrica_gradus_explicare(&sutura, actiones, piscina,
            &causa);
        CREDO_NON_NIHIL(explicata);
        si (explicata == NIHIL)
        {
            imprimere("  explicare: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        }
        CREDO_AEQUALIS_I32(explicata != NIHIL
            ? xar_numerus(explicata) : ZEPHYRUM, III);
        membrum_a = _actio_explicata(explicata, "probationes_t/a");
        membrum_b = _actio_explicata(explicata, "probationes_t/b");
        CREDO_NON_NIHIL(membrum_a);
        CREDO_NON_NIHIL(membrum_b);
        CREDO_VERUM(chorda_aequalis_literis(fabrica_area_via(
            chorda_ex_literis("probationes_t", piscina),
            chorda_ex_literis("a", piscina), piscina),
            "build/fabrica/area/probationes_t/a/"));
        si (membrum_a != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)membrum_a->genus,
                (i32)FABRICA_ACTIO_IUDICIUM);
            CREDO_VERUM(membrum_a->lectiones);
            CREDO_NON_NIHIL(membrum_a->membrum);
            CREDO_VERUM(chorda_aequalis_literis(
                ((FabricaExitus*)xar_obtinere(membrum_a->exitus,
                ZEPHYRUM))->via, VERDICTUM_A));
            CREDO_VERUM(chorda_aequalis_literis(
                ((FabricaIngressus*)xar_obtinere(membrum_a->ingressus,
                ZEPHYRUM))->via, "t/a.c"));
        }

        /* II. ambitus basis: PATH fixum, HOME, TMPDIR in area, radix,
         * declarata; ambitus fabricae non declaratus NON transit */
        ambitus = fabrica_ambitum_basis(&sutura, membrum_a,
            "build/fabrica/area/probationes_t/a/", piscina);
        CREDO_VERUM(_ambitus_habet(ambitus,
            "PATH=/usr/bin:/bin:/usr/sbin:/sbin"));
        CREDO_VERUM(_ambitus_habet(ambitus, "HOME=/home/u"));
        CREDO_VERUM(_ambitus_habet(ambitus,
            "TMPDIR=build/fabrica/area/probationes_t/a/tmp"));
        CREDO_VERUM(_ambitus_habet(ambitus, "RHUBARB_RADIX=."));
        CREDO_VERUM(_ambitus_habet(ambitus, "CREDO_FILTRUM=x"));
        CREDO_FALSUM(_ambitus_habet(ambitus, "OMNIA=1"));
        CREDO_AEQUALIS_I32(ambitus != NIHIL ? xar_numerus(ambitus)
            : ZEPHYRUM, V);
        si (ambitus != NIHIL && xar_numerus(ambitus) == V)
        {
            CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(
                ambitus, ZEPHYRUM), "CREDO_FILTRUM=x"));
        }

        /* III. sanare membra: ambo transeunt in areis suis */
        ordo    = fabrica_ordinare(explicata, piscina, &causa);
        electa  = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis(VERDICTUM_A,
            piscina);
        *(chorda*)xar_addere(electa) = chorda_ex_literis(VERDICTUM_B,
            piscina);
        _gradus_cursus = ZEPHYRUM;
        sanationes = ordo != NIHIL ? fabrica_sanare(&sutura, ordo,
            electa,
            FALSUM, piscina, &causa) : NIHIL;
        CREDO_NON_NIHIL(sanationes);
        sanatio = sanationes != NIHIL
            ? mundi_sanatio_invenire(sanationes,
            "probationes_t/a") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
            si (sanatio->eventus != FABRICA_SANATUM)
            {
                imprimere("  membrum a: %.*s\n",
                    (integer)sanatio->causa.mensura,
                    (constans character*)sanatio->causa.datum);
            }
        }
        CREDO_AEQUALIS_I32(_gradus_cursus, II);
        CREDO_AEQUALIS_I32(_gradus_areae_paratae, II);
        CREDO_VERUM(strcmp(_gradus_area_ultima,
            "build/fabrica/area/probationes_t/b/") == ZEPHYRUM);
        CREDO_FALSUM(_ambitus_habet(_gradus_ambitus_ultimus,
            "OMNIA=1"));
        CREDO_NON_NIHIL(mundi_fasciculum_invenire(&discus,
            VERDICTUM_A));

        /* IV. transitus servati: iudicium RECENS, sanare nihil
         * currit */
        si (membrum_a != NIHIL)
        {
            iudicium = fabrica_iudicare(&sutura, membrum_a,
                (FabricaExitus*)xar_obtinere(membrum_a->exitus,
                ZEPHYRUM), VERUM, piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_RECENS);
        }
        cursus = _gradus_cursus;
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_gradus_cursus, cursus);

        /* V. fons membri a mutatus (ingressus staticus): a solum
         * iterum currit; lectio dynamica mutata nominatur */
        mundi_ponere(&discus, "t/a.c", "a II\n");
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_gradus_cursus, cursus + I);
        mundi_ponere(&discus, "data/communis.txt", "communis II\n");
        si (membrum_b != NIHIL)
        {
            iudicium = fabrica_iudicare(&sutura, membrum_b,
                (FabricaExitus*)xar_obtinere(membrum_b->exitus,
                ZEPHYRUM), VERUM, piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_STALUM);
            CREDO_VERUM(mundi_continet(iudicium.causa,
                "data/communis.txt",
                piscina));
        }
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_gradus_cursus, cursus + III);

        /* VI. defectus numquam servatur: b fractum, iterum currit */
        _gradus_fractus = "t/b.c";
        mundi_ponere(&discus, "t/b.c", "b II\n");
        cursus = _gradus_cursus;
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = sanationes != NIHIL
            ? mundi_sanatio_invenire(sanationes,
            "probationes_t/b") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
        }
        CREDO_NIHIL(mundi_fasciculum_invenire(&discus, VERDICTUM_B));
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_gradus_cursus, cursus + II);
        _gradus_fractus = NIHIL;

        /* VII. scriptura extra aream: FRACTUM nominatum */
        _gradus_alienum = "data/alienum.txt";
        mundi_ponere(&discus, "t/a.c", "a III\n");
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = sanationes != NIHIL
            ? mundi_sanatio_invenire(sanationes,
            "probationes_t/a") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa,
                "data/alienum.txt",
                piscina));
        }
        _gradus_alienum = NIHIL;

        /* VIII. ambitus non declaratus: FRACTUM nominatum; basis et
         * declarata transeunt */
        _gradus_liber_extra = "E\tPATH\nE\tCREDO_FILTRUM\tx\n"
            "E\tVARIABILIS_ALIENA\n";
        mundi_ponere(&discus, "t/a.c", "a IV\n");
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = sanationes != NIHIL
            ? mundi_sanatio_invenire(sanationes,
            "probationes_t/a") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa,
                "ambitus non declaratus: VARIABILIS_ALIENA", piscina));
            CREDO_FALSUM(mundi_continet(sanatio->causa, "PATH",
                piscina));
            CREDO_FALSUM(mundi_continet(sanatio->causa, "CREDO_FILTRUM",
                piscina));
        }
        _gradus_liber_extra = "E\tPATH\nE\tCREDO_FILTRUM\tx\n";
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = sanationes != NIHIL
            ? mundi_sanatio_invenire(sanationes,
            "probationes_t/a") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
        }
        _gradus_liber_extra = "";
        lectiones = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
        {
            FabricaLectio* l = (FabricaLectio*)xar_addere(lectiones);

            l->genus  = LECTIO_AMBITUS;
            l->via    = chorda_ex_literis("ALIA", piscina);
            l         = (FabricaLectio*)xar_addere(lectiones);
            l->genus  = LECTIO_AMBITUS;
            l->via    = chorda_ex_literis("ALIA", piscina);
            l         = (FabricaLectio*)xar_addere(lectiones);
            l->genus  = LECTIO_AMBITUS;
            l->via    = chorda_ex_literis("TMPDIR", piscina);
            l         = (FabricaLectio*)xar_addere(lectiones);
            l->genus  = LECTIO_LEGIT;
            l->via    = chorda_ex_literis("t/a.c", piscina);
        }
        CREDO_AEQUALIS_I32(xar_numerus(fabrica_ambitum_non_declaratum(
            membrum_a != NIHIL ? membrum_a : &parens, lectiones,
            piscina)), I);

        /* IX. orphana: area actionis absentis, area membri deleti */
        mundi_directorium_ponere(&discus, "build/fabrica/area/",
            nomina_areae,
            II);
        mundi_directorium_ponere(&discus,
            "build/fabrica/area/probationes_t/",
            nomina_membrorum, III);
        orphana = fabrica_areas_orphanas(&sutura, explicata, piscina);
        CREDO_NON_NIHIL(orphana);
        CREDO_AEQUALIS_I32(orphana != NIHIL ? xar_numerus(orphana)
            : ZEPHYRUM, II);
        si (orphana != NIHIL && xar_numerus(orphana) == II)
        {
            CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(
                orphana, ZEPHYRUM),
                "build/fabrica/area/probationes_t/c/"));
            CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(
                orphana, I), "build/fabrica/area/vetus_actio/"));
        }

        /* X. membra duplicia: recusatio nominata */
        ((FabricaActio*)xar_obtinere(actiones, ZEPHYRUM))->gradus =
            &_gradus_duplex;
        causa = chorda_ex_literis("", piscina);
        CREDO_NIHIL(fabrica_gradus_explicare(&sutura, actiones, piscina,
            &causa));
        CREDO_VERUM(mundi_continet(causa, "membrum duplex", piscina));
        CREDO_VERUM(mundi_continet(causa, "t.stml:3", piscina));
        mundi_ambitus_ficti[0] = NIHIL;
    }


    /* ==================================================
     * POST ET COMPOSITA (fabrica-6 T7): <post actio=/> ordinat
     * (productor ante membra, sanatus primum) sed non clavat - membrum
     * quod productum legit per vestigium suum clavatur, alterum non;
     * compositum gradus per actionem, verdictum '<titulus>: N/M'
     * ================================================== */

    {
                 DiscusFictus  discus;
                FabricaSutura  sutura;
          InternamentumChorda* intern = internamentum_creare(piscina);
                 FabricaActio  parens;
            FabricaAttributum* attributum;
                 FabricaActio* membrum_a;
                 FabricaActio* membrum_b;
                 FabricaActio* g;
                          Xar* actiones;
                          Xar* explicata;
                          Xar* ordo;
                          Xar* electa;
                          Xar* sanationes;
                          Xar* composita;
                          Xar* iudicia;
                       chorda  causa;
                       chorda  verdictum;
                          s32  locus_g         = -I;
                          s32  locus_a         = -I;
                          s32  locus_sanati_g  = -I;
                          s32  locus_sanati_a  = -I;
                          i32  k;
           constans character* nomina_t[] = { "a.c", "b.c" };
           constans character* VA =
               "build/fabrica/area/probationes_t/a/verdictum.txt";
          constans character* VB =
              "build/fabrica/area/probationes_t/b/verdictum.txt";

        imprimere("\n--- Probans post et composita (T7) ---\n");
        causa = chorda_ex_literis("", piscina);
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.lectiones_legere = mundi_lectiones_legere;
        sutura.lectiones_scribere = mundi_lectiones_scribere;
        sutura.ambitus = mundi_ambitus_fictum;
        sutura.species = mundi_species_ficta;
        sutura.verdictum_ponere = mundi_verdictum_ponere;
        sutura.area_parare = _area_parare_ficta;
        sutura.in_area_currere = _in_area_currere_fictum;
        sutura.exitus_noti = tabula_dispersa_creare_chorda(piscina, 16);
        (vacuum)tabula_dispersa_inserere(sutura.exitus_noti,
            chorda_ex_literis("build/corpus.lst", piscina), NIHIL);
        mundi_ponere(&discus, "t/a.c", "a I\n");
        mundi_ponere(&discus, "t/b.c", "b I\n");
        mundi_ponere(&discus, "data/communis.txt", "communis I\n");
        mundi_ponere(&discus, "src.txt", "fons I\n");
        mundi_directorium_ponere(&discus, "t/", nomina_t, II);
        mundi_scriptum_addere(&discus, "gen_g", "build/corpus.lst",
            "src.txt",
            "corpus:", 0, FALSUM);
        _gradus_lectio_fons = "t/a.c";

        /* I. declaratio: <post actio="g"/>; post ignotum recusatur */
        actiones = fabrica_declarationes_legere(chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"p\" genus=\"iudicium\">\n"
            "    <probationes_c exemplar=\"t/probatio_*.c\"/>\n"
            "    <post actio=\"g\"/>\n"
            "  </actio>\n</aedificatio>\n", piscina), "t.stml",
            piscina, intern, &causa);
        CREDO_NON_NIHIL(actiones);
        si (actiones != NIHIL)
        {
            FabricaActio* p = (FabricaActio*)xar_obtinere(actiones,
                ZEPHYRUM);

            CREDO_AEQUALIS_I32(p->post != NIHIL ? xar_numerus(p->post)
                : ZEPHYRUM, I);
            causa = chorda_ex_literis("", piscina);
            CREDO_FALSUM(fabrica_praecondiciones_probare(actiones,
                piscina, &causa));
            CREDO_VERUM(mundi_continet(causa, "post", piscina));
            CREDO_VERUM(mundi_continet(causa, "'g'", piscina));
        }

        /* actiones: parens gradus (post g) et productor g */
        memset(&parens, ZEPHYRUM, magnitudo(parens));
        parens.titulus   = chorda_ex_literis("probationes_t", piscina);
        parens.genus     = FABRICA_ACTIO_IUDICIUM;
        parens.sedes     = chorda_ex_literis("t.stml:3", piscina);
        parens.mandatum  = xar_creare(piscina, (i32)magnitudo(chorda));
        parens.ingressus  = xar_creare(piscina,
            (i32)magnitudo(FabricaIngressus));
        parens.exitus     = xar_creare(piscina,
            (i32)magnitudo(FabricaExitus));
        parens.gradus     = &_gradus_ludicrum;
        parens.attributa  = xar_creare(piscina,
            (i32)magnitudo(FabricaAttributum));
        attributum = (FabricaAttributum*)xar_addere(parens.attributa);
        attributum->titulus = chorda_ex_literis("exemplar", piscina);
        attributum->valor = chorda_ex_literis("t/", piscina);
        parens.post = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(parens.post) = chorda_ex_literis("g",
            piscina);
        g = mundi_actio_scripta(piscina, "g", "gen_g", "src.txt",
            "build/corpus.lst", "regeneratio");
        actiones = xar_creare(piscina, (i32)magnitudo(FabricaActio));
        *(FabricaActio*)xar_addere(actiones) = parens;
        *(FabricaActio*)xar_addere(actiones) = *g;
        causa = chorda_ex_literis("", piscina);
        CREDO_VERUM(fabrica_praecondiciones_probare(actiones, piscina,
            &causa));
        explicata = fabrica_gradus_explicare(&sutura, actiones, piscina,
            &causa);
        CREDO_NON_NIHIL(explicata);
        membrum_a = _actio_explicata(explicata, "probationes_t/a");
        membrum_b = _actio_explicata(explicata, "probationes_t/b");
        CREDO_NON_NIHIL(membrum_a);
        CREDO_NON_NIHIL(membrum_b);
        si (membrum_a != NIHIL)
        {
            CREDO_AEQUALIS_I32(membrum_a->post != NIHIL
                ? xar_numerus(membrum_a->post) : ZEPHYRUM, I);
        }

        /* II. ordo: productor g ante membra (declaratus post ea) */
        ordo = explicata != NIHIL ? fabrica_ordinare(explicata, piscina,
            &causa) : NIHIL;
        CREDO_NON_NIHIL(ordo);
        per (k = ZEPHYRUM; ordo != NIHIL && k < xar_numerus(ordo); k++)
        {
            FabricaActio* a = *(FabricaActio**)xar_obtinere(ordo, k);

            si (chorda_aequalis_literis(a->titulus, "g"))
            {
                locus_g = (s32)k;
            }
            si (chorda_aequalis_literis(a->titulus, "probationes_t/a"))
            {
                locus_a = (s32)k;
            }
        }
        CREDO_VERUM(locus_g >= ZEPHYRUM && locus_a > locus_g);

        /* III. sanare membra nominata: g (exitus absens) PRIMUM
         * sanatur */
        electa = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis(VA, piscina);
        *(chorda*)xar_addere(electa) = chorda_ex_literis(VB, piscina);
        sanationes = ordo != NIHIL ? fabrica_sanare(&sutura, ordo,
            electa,
            FALSUM, piscina, &causa) : NIHIL;
        CREDO_NON_NIHIL(sanationes);
        per (k = ZEPHYRUM; sanationes != NIHIL
             && k < xar_numerus(sanationes); k++)
        {
            FabricaSanatio* s =
                (FabricaSanatio*)xar_obtinere(sanationes,
                k);

            si (chorda_aequalis_literis(s->actio->titulus, "g"))
            {
                locus_sanati_g = (s32)k;
                CREDO_AEQUALIS_I32((i32)s->eventus,
                    (i32)FABRICA_SANATUM);
            }
            si (chorda_aequalis_literis(s->actio->titulus,
                    "probationes_t/a"))
            {
                locus_sanati_a = (s32)k;
                CREDO_AEQUALIS_I32((i32)s->eventus,
                    (i32)FABRICA_SANATUM);
            }
        }
        CREDO_VERUM(locus_sanati_g >= ZEPHYRUM
            && locus_sanati_a > locus_sanati_g);
        CREDO_VERUM(mundi_contentum_est(&discus, "build/corpus.lst",
            "corpus:fons I\n"));

        /* IV. productum mutatum: a (legit) STALUM, b (non legit)
         * RECENS */
        mundi_ponere(&discus, "build/corpus.lst", "corpus:alterum\n");
        si (membrum_a != NIHIL && membrum_b != NIHIL)
        {
            FabricaIudicium ia;
            FabricaIudicium ib;

            ia = fabrica_iudicare(&sutura, membrum_a,
                (FabricaExitus*)xar_obtinere(membrum_a->exitus,
                ZEPHYRUM), VERUM, piscina);
            ib = fabrica_iudicare(&sutura, membrum_b,
                (FabricaExitus*)xar_obtinere(membrum_b->exitus,
                ZEPHYRUM), VERUM, piscina);
            CREDO_AEQUALIS_I32((i32)ia.status, (i32)FABRICA_STALUM);
            CREDO_VERUM(mundi_continet(ia.causa, "build/corpus.lst",
                piscina));
            CREDO_AEQUALIS_I32((i32)ib.status, (i32)FABRICA_RECENS);

            /* V. compositum gradus et verdictum eius */
            composita = fabrica_gradus_composita(explicata, piscina);
            CREDO_AEQUALIS_I32(composita != NIHIL
                ? xar_numerus(composita) : ZEPHYRUM, I);
            si (composita != NIHIL && xar_numerus(composita) == I)
            {
                FabricaCompositum* c = (FabricaCompositum*)xar_obtinere(
                    composita, ZEPHYRUM);
                               Xar* partes;

                CREDO_VERUM(chorda_aequalis_literis(c->titulus,
                    "probationes_t"));
                CREDO_AEQUALIS_I32(xar_numerus(c->partes), II);
                partes = fabrica_compositum_explicare(composita,
                    explicata, c->titulus, piscina, &causa);
                CREDO_AEQUALIS_I32(partes != NIHIL ? xar_numerus(partes)
                    : ZEPHYRUM, II);
            }
            iudicia = xar_creare(piscina,
                (i32)magnitudo(FabricaIudicium));
            *(FabricaIudicium*)xar_addere(iudicia) = ia;
            *(FabricaIudicium*)xar_addere(iudicia) = ib;
            verdictum = fabrica_compositum_verdictum(iudicia,
                chorda_ex_literis("probationes_t", piscina), piscina);
            CREDO_VERUM(chorda_aequalis_literis(verdictum,
                "probationes_t: 1/2 - non recentia: probationes_t/a"));
            si (!chorda_aequalis_literis(verdictum,
                    "probationes_t: 1/2 - non recentia: "
                    "probationes_t/a"))
            {
                imprimere("  verdictum: %.*s\n",
                    (integer)verdictum.mensura,
                    (constans character*)verdictum.datum);
            }
            xar_vacare(iudicia);
            *(FabricaIudicium*)xar_addere(iudicia) = ib;
            *(FabricaIudicium*)xar_addere(iudicia) = ib;
            CREDO_VERUM(chorda_aequalis_literis(
                fabrica_compositum_verdictum(iudicia, chorda_ex_literis(
                "probationes_t", piscina), piscina),
                "probationes_t: 2/2"));
        }
        _gradus_lectio_fons = NIHIL;
    }


    /* ==================================================
     * PROBATIONES_C (fabrica-6 T6c): elementum gradus in declaratione,
     * membra per exemplar/praeter, clavis = clausura + vexilla +
     * identitas clang + vestigium cursus; mutatio fontis bibliothecae,
     * capitis, probationis aut lectionis membra SOLA tangentia iterum
     * nectit et currit; facultas: nexum, non cursum; defectus cursus et
     * compilationis FRACTUM nominatum
     * ================================================== */

    {
                 DiscusFictus  discus;
                FabricaSutura  sutura;
          InternamentumChorda* intern = internamentum_creare(piscina);
               FabricaSanatio* sanatio;
                 FabricaActio* membrum_a;
                 FabricaActio* membrum_c;
                          Xar* actiones;
                          Xar* explicata;
                          Xar* ordo;
                          Xar* electa;
                          Xar* sanationes;
                       chorda  causa;
                          i32  nexus;
                          i32  cursus;
                          i32  k;
           constans character* nomina_t[] = { "notae.txt",
              "probatio_a.c", "probatio_b.c", "probatio_c.c",
              "probatio_zeta.c" };
          constans character* DECLARATIO =
              "<aedificatio>\n"
              "  <actio titulus=\"probationes_t\" genus=\"iudicium\">\n"
              "    <probationes_c exemplar=\"t/probatio_*.c\""
              " praeter=\"probatio_zeta.c\"/>\n"
              "    <ambitus variabilis=\"CREDO_FILTRUM\"/>\n"
              "  </actio>\n"
              "</aedificatio>\n";
          constans character* verdicta[] = {
              "build/fabrica/area/probationes_t/probatio_a/"
              "verdictum.txt",
              "build/fabrica/area/probationes_t/probatio_b/"
              "verdictum.txt",
              "build/fabrica/area/probationes_t/probatio_c/"
              "verdictum.txt"
          };

        imprimere("\n--- Probans probationes_c (gradus C) ---\n");
        causa = chorda_ex_literis("", piscina);
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.lectiones_legere = mundi_lectiones_legere;
        sutura.lectiones_scribere = mundi_lectiones_scribere;
        sutura.ambitus = mundi_ambitus_fictum;
        sutura.species = mundi_species_ficta;
        sutura.identitas = mundi_identitatem_fictam_dare;
        sutura.verdictum_ponere = mundi_verdictum_ponere;
        sutura.area_parare = _area_parare_ficta;
        sutura.in_area_currere = _in_area_currere_c;
        sutura.clausura_c = _clausura_c_ficta;
        sutura.compilare = _compilare_fictum;
        sutura.exitus_noti = tabula_dispersa_creare_chorda(piscina, 16);
        mundi_ponere(&discus, "t/probatio_a.c", "a I\n");
        mundi_ponere(&discus, "t/probatio_b.c", "b I\n");
        mundi_ponere(&discus, "t/probatio_c.c", "c I\n");
        mundi_ponere(&discus, "t/probatio_zeta.c", "zeta\n");
        mundi_ponere(&discus, "lib/x.c", "x I\n");
        mundi_ponere(&discus, "lib/y.c", "y I\n");
        mundi_ponere(&discus, "include/x.h", "xh I\n");
        mundi_ponere(&discus, "include/y.h", "yh I\n");
        mundi_ponere(&discus, "aedilis.stml", "<aedilis/>\n");
        mundi_ponere(&discus, "data/communis.txt", "communis I\n");
        mundi_directorium_ponere(&discus, "t/", nomina_t, V);

        /* I. declaratio: elementum gradus et ambitus legitur */
        actiones = fabrica_declarationes_legere(chorda_ex_literis(
            DECLARATIO, piscina), "t.stml", piscina, intern, &causa);
        CREDO_NON_NIHIL(actiones);
        si (actiones == NIHIL)
        {
            imprimere("  declaratio: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        }
        si (actiones != NIHIL && xar_numerus(actiones) == I)
        {
            FabricaActio* parens = (FabricaActio*)xar_obtinere(actiones,
                ZEPHYRUM);

            CREDO_VERUM(parens->gradus == fabrica_gradus_invenire(
                chorda_ex_literis("probationes_c", piscina)));
            CREDO_NON_NIHIL(parens->gradus);
            CREDO_AEQUALIS_I32(parens->attributa != NIHIL
                ? xar_numerus(parens->attributa) : ZEPHYRUM, II);
            CREDO_AEQUALIS_I32(parens->ambitus != NIHIL
                ? xar_numerus(parens->ambitus) : ZEPHYRUM, I);
        }
        causa = chorda_ex_literis("", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\">\n"
            "    <probationes_c exemplar=\"t/probatio_*.c\"/>\n"
            "  </actio>\n</aedificatio>\n", piscina), "t.stml", piscina,
            intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "gradus extra iudicium",
            piscina));
        causa = chorda_ex_literis("", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"iudicium\">\n"
            "    <probationes_c exemplar=\"t/probatio_*.c\"/>\n"
            "    <probationes_c exemplar=\"u/probatio_*.c\"/>\n"
            "  </actio>\n</aedificatio>\n", piscina), "t.stml", piscina,
            intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "gradus plures", piscina));

        /* II. explicatio: a, b, c (zeta praeter, notae non congruit) */
        causa = chorda_ex_literis("", piscina);
        explicata = actiones
            != NIHIL ? fabrica_gradus_explicare(&sutura,
            actiones, piscina, &causa) : NIHIL;
        CREDO_NON_NIHIL(explicata);
        si (explicata == NIHIL)
        {
            imprimere("  explicare: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        }
        CREDO_AEQUALIS_I32(explicata != NIHIL ? xar_numerus(explicata)
            : ZEPHYRUM, IV);
        membrum_a = _actio_explicata(explicata,
            "probationes_t/probatio_a");
        membrum_c = _actio_explicata(explicata,
            "probationes_t/probatio_c");
        CREDO_NON_NIHIL(membrum_a);
        CREDO_NON_NIHIL(membrum_c);
        CREDO_NIHIL(_actio_explicata(explicata,
            "probationes_t/probatio_zeta"));
        si (membrum_a != NIHIL)
        {
            b32 x_c = FALSUM;
            b32 x_h = FALSUM;
            b32 cfg = FALSUM;

            per (k = ZEPHYRUM; k
                < xar_numerus(membrum_a->ingressus); k++)
            {
                chorda v = ((FabricaIngressus*)xar_obtinere(
                    membrum_a->ingressus, k))->via;

                x_c = x_c || chorda_aequalis_literis(v, "lib/x.c");
                x_h = x_h || chorda_aequalis_literis(v, "include/x.h");
                cfg = cfg || chorda_aequalis_literis(v, "aedilis.stml");
            }
            CREDO_VERUM(x_c);
            CREDO_VERUM(x_h);
            CREDO_VERUM(cfg);
        }

        /* III. sanare: III nexa, II currunt (c facultas), V
         * compilata */
        ordo    = explicata != NIHIL ? fabrica_ordinare(explicata,
            piscina, &causa) : NIHIL;
        electa  = xar_creare(piscina, (i32)magnitudo(chorda));
        per (k = ZEPHYRUM; k < III; k++)
        {
            *(chorda*)xar_addere(electa) =
                chorda_ex_literis(verdicta[k],
                piscina);
        }
        sanationes = ordo != NIHIL ? fabrica_sanare(&sutura, ordo,
            electa,
            FALSUM, piscina, &causa) : NIHIL;
        CREDO_NON_NIHIL(sanationes);
        per (k = ZEPHYRUM; sanationes != NIHIL && k < III; k++)
        {
            constans character* tituli[] = { "probationes_t/probatio_a",
                "probationes_t/probatio_b",
                    "probationes_t/probatio_c" };

            sanatio = mundi_sanatio_invenire(sanationes, tituli[k]);
            CREDO_NON_NIHIL(sanatio);
            si (sanatio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                    (i32)FABRICA_SANATUM);
                si (sanatio->eventus != FABRICA_SANATUM)
                {
                    imprimere("  %s: %.*s\n", tituli[k],
                        (integer)sanatio->causa.mensura,
                        (constans character*)sanatio->causa.datum);
                }
            }
        }
        CREDO_AEQUALIS_I32(_c_nexus, III);
        CREDO_AEQUALIS_I32(_c_cursus, II);
        CREDO_AEQUALIS_I32(_c_compilationes, V);
        CREDO_AEQUALIS_I32(_c_nexus_compagis, I);
        CREDO_VERUM(mundi_contentum_est(&discus, verdicta[II],
            "probationes_t/probatio_c: transiit (nexus solum: facultas "
            "fenestra)\n"));

        /* IV. nihil mutatum: nihil nectitur, nihil currit */
        nexus   = _c_nexus;
        cursus  = _c_cursus;
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_c_nexus, nexus);
        CREDO_AEQUALIS_I32(_c_cursus, cursus);

        /* V. fons bibliothecae a solius: a solum */
        mundi_ponere(&discus, "lib/x.c", "x II\n");
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_c_nexus, nexus + I);
        CREDO_AEQUALIS_I32(_c_cursus, cursus + I);
        CREDO_VERUM(strstr(_c_cursus_ultimus, "probatio_a") != NIHIL);

        /* VI. caput b solius: b solum */
        mundi_ponere(&discus, "include/y.h", "yh II\n");
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_c_nexus, nexus + II);
        CREDO_AEQUALIS_I32(_c_cursus, cursus + II);
        CREDO_VERUM(strstr(_c_cursus_ultimus, "probatio_b") != NIHIL);

        /* VII. probatio c (facultas) mutata: nectitur, non currit */
        mundi_ponere(&discus, "t/probatio_c.c", "c II\n");
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_c_nexus, nexus + III);
        CREDO_AEQUALIS_I32(_c_cursus, cursus + II);

        /* VIII. lectio cursus (data/communis.txt) mutata: a et b,
         * non c */
        mundi_ponere(&discus, "data/communis.txt", "communis II\n");
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_c_nexus, nexus + V);
        CREDO_AEQUALIS_I32(_c_cursus, cursus + IV);

        /* IX. cursus b fractus: FRACTUM, deinde iterum currit */
        _c_fractus_cursus = "probatio_b";
        mundi_ponere(&discus, "t/probatio_b.c", "b II\n");
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = sanationes
            != NIHIL ? mundi_sanatio_invenire(sanationes,
            "probationes_t/probatio_b") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
        }
        cursus = _c_cursus;
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        CREDO_AEQUALIS_I32(_c_cursus, cursus + I);
        _c_fractus_cursus = NIHIL;

        /* X. compilatio fracta: FRACTUM cum errato, nihil currit */
        _c_fractus_compilatio = "lib/x.c";
        mundi_ponere(&discus, "lib/x.c", "x III\n");
        cursus = _c_cursus;
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = sanationes
            != NIHIL ? mundi_sanatio_invenire(sanationes,
            "probationes_t/probatio_a") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "error: ficta",
                piscina));
            CREDO_VERUM(mundi_continet(sanatio->causa, "lib/x.c",
                piscina));
        }
        CREDO_FALSUM(strstr(_c_cursus_ultimus, "probatio_a") != NIHIL
            && _c_cursus > cursus);
        _c_fractus_compilatio = NIHIL;

        /* XI (fabrica-6 T8). PER UNDAS (agere_simul): fractura membri
         * fratres NON sistit - b et c currunt etsi a fractum est
         * (olim 'post fracturam: nova non incipiuntur', VIII OMISSI) */
        sutura.agere_simul  = mundi_agere_simul;
        _c_fractus_cursus   = "probatio_a";
        mundi_ponere(&discus, "t/probatio_a.c", "a IV\n");
        mundi_ponere(&discus, "t/probatio_b.c", "b IV\n");
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = sanationes
            != NIHIL ? mundi_sanatio_invenire(sanationes,
            "probationes_t/probatio_a") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
        }
        sanatio = sanationes
            != NIHIL ? mundi_sanatio_invenire(sanationes,
            "probationes_t/probatio_b") : NIHIL;
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
        }
        sutura.agere_simul  = NIHIL;
        _c_fractus_cursus   = NIHIL;
        (vacuum)membrum_c;
    }


    /* ==================================================
     * CHASSIS GRADUUM (fabrica-6 T5): omne genus gradus registratum
     * fixum conformitatis habet (T6: probationes_c)
     * ================================================== */

    {
        i32 g;

        imprimere("\n--- Probans chassis: registrum graduum ---\n");
        CREDO_NIHIL(fabrica_gradus_obtinere(fabrica_graduum_numerus()));
        CREDO_NIHIL(fabrica_gradus_invenire(chorda_ex_literis(
            "ludicrum", piscina)));
        per (g = ZEPHYRUM; g < fabrica_graduum_numerus(); g++)
        {
            constans FabricaGradus* gradus = fabrica_gradus_obtinere(g);
                               b32  fixum = FALSUM;
                               i32  k;

            per (k = ZEPHYRUM; _fixa_graduum[k] != NIHIL; k++)
            {
                si (strcmp(_fixa_graduum[k], gradus->titulus)
                    == ZEPHYRUM)
                {
                    fixum = VERUM;
                }
            }
            si (!fixum)
            {
                imprimere("  FRACTUM: gradus '%s' sine fixo "
                    "conformitatis\n", gradus->titulus);
            }
            CREDO_VERUM(fixum);
        }
    }

    imprimere("\n");

    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
