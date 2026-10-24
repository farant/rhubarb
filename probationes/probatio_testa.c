/* probatio_testa.c - contractus applicationis (testa-spec III.2):
 * optiones, titulus processus, forma, cursus incipere -> ludum struere ->
 * finire -> dimittere (finis semel, numquam sine initio). Testa vera
 * (principale) non includitur: testa_cursus.h solum. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"
#include "testa_cursus.h"
#include "ludus_toy.h"
#include "credo.h"
#include <string.h>

/* applicatio probanda: status totus in piscina ambitus (R3) */
nomen structura {
    ToyStatus toy;
          i32 fines;      /* quoties finis vocatus */
          i32 pulsus;
} AppProbanda;

interior vacuum
_finire_probandam (
    vacuum* ctx)
{
    ((AppProbanda*)ctx)->fines += I;
}

interior b32
_pulsare_probandam (
    vacuum* ctx)
{
    ((AppProbanda*)ctx)->pulsus += I;
    redde FALSUM;
}

/* initium verum: toy dispensator, figurae, forma plena */
interior b32
_initium_probandum (
    constans TestaAmbitus* a,
               TestaForma* forma)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
            AppProbanda* app;

    app = (AppProbanda*)piscina_allocare(a->piscina,
        magnitudo(AppProbanda));
    si (!app)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(AppProbanda));
    intern  = internamentum_creare(a->piscina);
    thema_initiare();
    repo  = insula_repositorium_creare(a->piscina, intern,
        "<insula/>");
    reg   = actio_registrum_creare(a->piscina, intern);
    toy_registrare(reg, &app->toy);
    forma->titulus   = "probanda";
    forma->latitudo  = CC;
    forma->altitudo  = C;
    forma->scala     = II;
    forma->d             = dispensator_creare(a->piscina, intern, repo,
                               reg, toy_componere, &app->toy, CCC);
    forma->figurae       = figura_registrum_creare(a->piscina);
    si (   !forma->figurae
        || !figura_registrare(forma->figurae, PARTES_BOTTONE, ZEPHYRUM,
               figura_finium, NIHIL))
    {
        redde FALSUM;
    }
    forma->pulsator      = _pulsare_probandam;
    forma->pulsator_ctx  = app;
    forma->finis         = _finire_probandam;
    forma->finis_ctx     = app;
    redde VERUM;
}

/* initium deficiens (volumen non apertum, e.g.) */
interior b32
_initium_deficiens (
    constans TestaAmbitus* a,
               TestaForma* forma)
{
    (vacuum)a;
    forma->finis = _finire_probandam;
    redde FALSUM;
}

/* initium verum sed forma prava (scala ZEPHYRUM) - finis tamen
 * vocandus: applicatio statum (volumen) iam aperuit */
interior b32
_initium_scala_nulla (
    constans TestaAmbitus* a,
               TestaForma* forma)
{
    redde _initium_probandum(a, forma)
        && (forma->scala = ZEPHYRUM, VERUM);
}

interior vacuum
_ambitum_ponere (
     TestaAmbitus* a,
          Piscina* piscina)
{
    memset(a, ZEPHYRUM, magnitudo(TestaAmbitus));
    a->piscina        = piscina;
    a->sedes_datorum  = ".";
}

interior vacuum
_sectio_optiones (
    CredoContextus* c)
{
    /* argv mutabile (-Wwrite-strings: litterae constantes sunt) */
        character  a_programma[]  = "bin/pictor";
        character  a_volumen[]    = "-volumen";
        character  a_via[]        = "x.volumen";
        character  a_fumus[]      = "-fumus";
        character  a_imago[]      = "-imago";
        character  a_png[]        = "y.png";
        character* omnia[V];
        character* sola[II];
    TestaOptiones  o;

    (vacuum)c;
    omnia[ZEPHYRUM]  = a_programma;
    omnia[I]         = a_volumen;
    omnia[II]        = a_via;
    omnia[III]       = a_fumus;
    omnia[IV]        = a_imago;
    testa_optiones_legere(V, omnia, &o);
    CREDO_GRADUS("-imago ultimum sine via: NIHIL");
    CREDO_VERUM(o.fumus);
    CREDO_NIHIL(o.via_imaginis);

    sola[ZEPHYRUM]  = a_programma;
    sola[I]         = a_imago;
    testa_optiones_legere(II, sola, &o);
    CREDO_FALSUM(o.fumus);
    CREDO_NIHIL(o.via_imaginis);

    omnia[IV - I]  = a_imago;
    omnia[IV]      = a_png;
    testa_optiones_legere(V, omnia, &o);
    CREDO_GRADUS("-imago y.png: via; -fumus absens");
    CREDO_FALSUM(o.fumus);
    CREDO_NON_NIHIL(o.via_imaginis);
    CREDO_VERUM(strcmp(o.via_imaginis, "y.png") == ZEPHYRUM);
}

interior vacuum
_sectio_titulus (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(strcmp(testa_titulus_processus("bin/pictor"),
        "pictor") == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus("vicus"), "vicus")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus(NIHIL), "applicatio")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus(""), "applicatio")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus("bin/"), "applicatio")
        == ZEPHYRUM);
}

interior vacuum
_sectio_cursus (
    CredoContextus* c)
{
          TestaAmbitus  a;
           TestaCursus  cursus;
    constans character* causa;
       TabulaPixelorum* t;
           AppProbanda* app;
               Eventus  e;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    causa = NIHIL;
    CREDO_VERUM(testa_cursum_incipere(&cursus, &a, _initium_probandum,
        &causa));
    CREDO_NIHIL(causa);
    app = (AppProbanda*)cursus.forma.finis_ctx;

    t = tabula_pixelorum_creare_nuda(c->piscina, CC, C);
    CREDO_VERUM(testa_cursum_ludum_struere(&cursus, c->piscina, t));
    CREDO_NON_NIHIL(cursus.lf);
    CREDO_GRADUS("pulsus formae in ludo positus");
    CREDO_VERUM(cursus.lf->pulsator == _pulsare_probandam);
    CREDO_GRADUS("ludum struere iterum: recusatur (ludus unus)");
    CREDO_FALSUM(testa_cursum_ludum_struere(&cursus, c->piscina, t));

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_SUSPENSIO;
    ludus_fenestra_tractare(cursus.lf, &e, (s64)M);
    ludus_quadrum(cursus.lf, (s64)M);
    CREDO_AEQUALIS_I32(cursus.lf->mensurae.quadra, I);

    testa_cursum_finire(&cursus);
    CREDO_AEQUALIS_I32(app->fines, ZEPHYRUM);
    testa_cursum_dimittere(&cursus);
    CREDO_AEQUALIS_I32(app->fines, I);
}

/* Review Focus 5: finis semel, quotquot exitus */
interior vacuum
_sectio_dimittere_semel (
    CredoContextus* c)
{
          TestaAmbitus  a;
           TestaCursus  cursus;
    constans character* causa;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    CREDO_VERUM(testa_cursum_incipere(&cursus, &a, _initium_probandum,
        &causa));
    testa_cursum_dimittere(&cursus);
    testa_cursum_dimittere(&cursus);
    CREDO_AEQUALIS_I32(((AppProbanda*)cursus.forma.finis_ctx)->fines,
        I);
}

interior vacuum
_sectio_initium_deficiens (
    CredoContextus* c)
{
          TestaAmbitus  a;
           TestaCursus  cursus;
    constans character* causa;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    causa = NIHIL;
    CREDO_FALSUM(testa_cursum_incipere(&cursus, &a, _initium_deficiens,
        &causa));
    CREDO_NON_NIHIL(causa);
    CREDO_GRADUS("initium FALSUM: finis numquam (status nullus)");
    testa_cursum_dimittere(&cursus);
    CREDO_FALSUM(cursus.finitus);
}

interior vacuum
_sectio_forma_prava (
    CredoContextus* c)
{
          TestaAmbitus  a;
           TestaCursus  cursus;
    constans character* causa;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    causa = NIHIL;
    CREDO_FALSUM(testa_cursum_incipere(&cursus, &a,
        _initium_scala_nulla,
        &causa));
    CREDO_NON_NIHIL(causa);
    CREDO_VERUM(strcmp(causa, "forma: scala < I") == ZEPHYRUM);
    CREDO_GRADUS("forma prava post initium verum: finis tamen semel");
    testa_cursum_dimittere(&cursus);
    CREDO_AEQUALIS_I32(((AppProbanda*)cursus.forma.finis_ctx)->fines,
        I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "optiones testae", _sectio_optiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "titulus processus", _sectio_titulus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "cursus: incipere, ludum struere, quadrum, finire, dimittere",
        _sectio_cursus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dimittere semel", _sectio_dimittere_semel, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "initium deficiens", _sectio_initium_deficiens, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "forma prava", _sectio_forma_prava, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("testa", SECTIONES);
}
