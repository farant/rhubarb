/* probatio_vicus_navigator.c - navigator in vico (navigator N3.2):
 * genus 'navigator' montatur, bibliotheca UNA lateribus communis (in
 * volumine vici), latus novum indicem monstrat; argumentum ULID
 * articulum aperit; locus lateris (comites) versionem nominat. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "navigator.h"
#include "navigator_bibliotheca.h"
#include "vicipaedia.h"
#include "thema.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGINA_RHEI \
    "<html><head><title>Rheum</title></head><body>" \
    "<section data-mw-section-id=\"0\"><p>Rheum est genus plantarum." \
    "</p></section></body></html>"

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
             chorda  articulus;   /* ULID articuli servati */
             chorda  versio;
} Mundus;

/* volumen cum articulo uno servato (bibliotheca propria, ut
 * tools/navigator) - vicus eum ex actis legit */
interior vacuum
mundum_parare (
    CredoContextus* c)
{
                        Mundus* x;
          NavigatorBibliotheca* b;
      constans NavigatorVersio* v = NIHIL;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_navigator");
    b = x->volumen ? navigator_bibliotheca_aperire(x->volumen, NIHIL,
        NIHIL, c->piscina) : NIHIL;
    si (   b && navigator_versionem_offerre(b, chorda_ex_literis(
            "https://la.wikipedia.org/wiki/Rheum", c->piscina),
            chorda_ex_literis("https://la.wikipedia.org/wiki/Rheum",
            c->piscina), "vicipaedia", chorda_ex_literis(PAGINA_RHEI,
            c->piscina), vicipaedia_articulus, &v) == NAVIGATOR_NOVUM
        && v)
    {
        x->articulus  = navigator_articulus(b, ZEPHYRUM)->identitas;
        x->versio     = v->identitas;
    }
    c->datum = x;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x = (Mundus*)c->datum;

    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

interior b32
aedificare (
              Mundus* x,
      CredoContextus* c)
{
    redde x && x->volumen && x->articulus.mensura > ZEPHYRUM
        && vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM);
}

/* latus frontale dextrum tabulae activae, si navigator */
interior VicusLatus*
latus_frontale (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "navigator"))
    {
        redde NIHIL;
    }
    redde l;
}

interior b32
campus_est (
     NavigatorMontatio* m,
    constans character* titulus,
    constans character* valor)
{
    chorda* a = insula_ramus_attributum(&m->ramus, titulus);

    redde a && chorda_aequalis_literis(*a, valor);
}

/* latera duo, bibliotheca una; latus novum = index */
interior vacuum
_probare_latera (
    CredoContextus* c)
{
               Mundus* x = (Mundus*)c->datum;
                Vicus* v;
           VicusLatus* l;
    NavigatorMontatio* prima;
    NavigatorMontatio* altera;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", NIHIL));
    (vacuum)vicus_pulsare(v);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    prima = (NavigatorMontatio*)l->montatio;
    CREDO_NECESSE_NON_NIHIL(prima->bibliotheca);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(
        prima->bibliotheca), I);
    CREDO_VERUM(campus_est(prima, "visus", "index"));
    CREDO_GRADUS("N3.3: pulsus et mora; sine petitione vicus quiescit");
    CREDO_VERUM(l->facies.pulsare != NIHIL && l->facies.mora != NIHIL);
    CREDO_VERUM(l->facies.mora(l->facies.pulsare_ctx) == -I);
    CREDO_VERUM(prima->reactor != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus_locus_titulus(v, l,
        c->piscina), "Saved");

    CREDO_NECESSE(vicus_activam_ponere(v, "2"));
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", NIHIL));
    (vacuum)vicus_pulsare(v);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    altera = (NavigatorMontatio*)l->montatio;
    CREDO_VERUM(altera != prima);
    CREDO_VERUM(altera->bibliotheca == prima->bibliotheca);
}

/* argumentum ULID: articulus; locus lateris versionem nominat */
interior vacuum
_probare_articulus (
    CredoContextus* c)
{
               Mundus* x = (Mundus*)c->datum;
                Vicus* v;
           VicusLatus* l;
    NavigatorMontatio* m;
               chorda  locus;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", chorda_ut_cstr(
        x->articulus, c->piscina)));
    (vacuum)vicus_pulsare(v);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    m = (NavigatorMontatio*)l->montatio;
    CREDO_VERUM(campus_est(m, "visus", "pagina"));
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus_locus_titulus(v, l,
        c->piscina), "Rheum - la.wikipedia.org - 1/1");

    CREDO_GRADUS("locus: navigator/pagina/<articulus>?versio=<ultima>"
        " - versio fixa etiam sine nomine (comes versionis suae)");
    locus = vicus_latus_locus(v, l, c->piscina);
    CREDO_VERUM(chorda_incipit(locus, chorda_concatenare(
        chorda_ex_literis("navigator/pagina/", c->piscina),
        x->articulus,
        c->piscina)));
    CREDO_VERUM(chorda_continet(locus, chorda_concatenare(
        chorda_ex_literis("versio=", c->piscina), x->versio,
        c->piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera", _probare_latera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "articulus", _probare_articulus, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_navigator", SECTIONES);
}
