/* probatio_componens.c - Probationes arboris componentium */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* bottone;
              Componens* tabula;
                  Fines  f;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    radix = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    CREDO_NON_NIHIL (radix);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = CDLXXX; f.altitudo =
                                                             CCC;
    componens_ponere_fines(radix, f);

    bottone = componens_creare(piscina, intern, "servare",
        PARTES_BOTTONE);
    f.x = X; f.y = X; f.latitudo = L; f.altitudo = XX;
    componens_ponere_fines(bottone, f);
    componens_ponere_actio(bottone, "documentum.servare");
    componens_ponere_titulum(bottone, "Servare");
    componens_ponere_focusabilis(bottone, VERUM);
    componens_addere_liberum(radix, bottone);

    tabula = componens_creare(piscina, intern, "tabula", PARTES_TABULA);
    f.x = C; f.y = X; f.latitudo = CCC; f.altitudo = CC;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "penicillus.ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);

    CREDO_AEQUALIS_I32 (componens_numerus_liberorum(radix), II);
    CREDO_VERUM (componens_liberum(radix, I) == tabula);
    CREDO_VERUM (tabula->parens == radix);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* bottone;
              Componens* tabula;
              Componens* inventus;
                  Fines  f;
                 chorda  quaesitum;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    radix  = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    f.x    = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = CDLXXX; f.altitudo =
                                                                CCC;
    componens_ponere_fines(radix, f);

    bottone = componens_creare(piscina, intern, "servare",
        PARTES_BOTTONE);
    f.x = X; f.y = X; f.latitudo = L; f.altitudo = XX;
    componens_ponere_fines(bottone, f);
    componens_ponere_actio(bottone, "documentum.servare");
    componens_ponere_titulum(bottone, "Servare");
    componens_ponere_focusabilis(bottone, VERUM);
    componens_addere_liberum(radix, bottone);

    tabula = componens_creare(piscina, intern, "tabula", PARTES_TABULA);
    f.x = C; f.y = X; f.latitudo = CCC; f.altitudo = CC;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "penicillus.ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);

    quaesitum  = chorda_ex_literis("tabula", piscina);
    inventus   = componens_invenire_per_id(radix, quaesitum);
    CREDO_VERUM (inventus == tabula);
    quaesitum  = chorda_ex_literis("nemo", piscina);
    inventus   = componens_invenire_per_id(radix, quaesitum);
    CREDO_NIHIL (inventus);
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  textus;

    piscina = c->piscina;

    CREDO_VERUM (partes_ex_titulo("bottone") == PARTES_BOTTONE);
    textus = chorda_ex_literis(partes_titulus(PARTES_TABULA), piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (textus, "tabula");
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* bottone;
              Componens* tabula;
              Componens* inventus;
              Componens* radix2;
                  Fines  f;
                 chorda  textus;
                 chorda  quaesitum;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    radix  = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    f.x    = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = CDLXXX; f.altitudo =
                                                                CCC;
    componens_ponere_fines(radix, f);

    bottone = componens_creare(piscina, intern, "servare",
        PARTES_BOTTONE);
    f.x = X; f.y = X; f.latitudo = L; f.altitudo = XX;
    componens_ponere_fines(bottone, f);
    componens_ponere_actio(bottone, "documentum.servare");
    componens_ponere_titulum(bottone, "Servare");
    componens_ponere_focusabilis(bottone, VERUM);
    componens_addere_liberum(radix, bottone);

    tabula = componens_creare(piscina, intern, "tabula", PARTES_TABULA);
    f.x = C; f.y = X; f.latitudo = CCC; f.altitudo = CC;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "penicillus.ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);

    textus = componens_scribere_stml(radix, piscina, intern, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    radix2 = componens_legere_stml(chorda_ut_cstr(textus, piscina),
                                   piscina, intern);
    CREDO_NON_NIHIL (radix2);
    CREDO_VERUM (componens_aequalis(radix, radix2));
    quaesitum  = chorda_ex_literis("servare", piscina);
    inventus   = componens_invenire_per_id(radix2, quaesitum);
    CREDO_NON_NIHIL (inventus);
    CREDO_VERUM (inventus->partes == PARTES_BOTTONE);
    CREDO_VERUM (inventus->focusabilis);
    CREDO_CHORDA_AEQUALIS_LITERIS (inventus->actio,
        "documentum.servare");
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* bottone;
              Componens* tabula;
              Componens* inventus;
              Componens* radix2;
                  Fines  f;
                 chorda  textus;
                 chorda  quaesitum;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    radix  = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    f.x    = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = CDLXXX; f.altitudo =
                                                                CCC;
    componens_ponere_fines(radix, f);

    bottone = componens_creare(piscina, intern, "servare",
        PARTES_BOTTONE);
    f.x = X; f.y = X; f.latitudo = L; f.altitudo = XX;
    componens_ponere_fines(bottone, f);
    componens_ponere_actio(bottone, "documentum.servare");
    componens_ponere_titulum(bottone, "Servare");
    componens_ponere_focusabilis(bottone, VERUM);
    componens_addere_liberum(radix, bottone);

    tabula = componens_creare(piscina, intern, "tabula", PARTES_TABULA);
    f.x = C; f.y = X; f.latitudo = CCC; f.altitudo = CC;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "penicillus.ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);

    textus = componens_scribere_stml(radix, piscina, intern, VERUM);
    radix2 = componens_legere_stml(chorda_ut_cstr(textus, piscina),
                                   piscina, intern);
    quaesitum  = chorda_ex_literis("servare", piscina);
    inventus   = componens_invenire_per_id(radix2, quaesitum);

    inventus->fines.x = inventus->fines.x + I;
    CREDO_FALSUM (componens_aequalis(radix, radix2));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "aedificationem", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invenire_per_id", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "partes tituli", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "circuitum STML", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "inaequalitatem", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("componens", SECTIONES);
}
