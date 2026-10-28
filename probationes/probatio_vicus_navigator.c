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
#include "dispensator.h"
#include "manus_ludus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGINA_RHEI \
    "<html><head><title>Rheum</title></head><body>" \
    "<section data-mw-section-id=\"0\"><p>Rheum est genus plantarum." \
    "</p></section></body></html>"

#define PAGINA_BETAE \
    "<html><head><title>Beta</title></head><body>" \
    "<section data-mw-section-id=\"0\"><p>Beta est olus.</p>" \
    "</section></body></html>"

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

/* N4.2: nexus secutus (via URL, ut ictus) historiam vici impellit -
 * Ctrl+O ad articulum priorem */
interior vacuum
_probare_retro (
    CredoContextus* c)
{
                         Mundus* x = (Mundus*)c->datum;
                          Vicus* v;
                     VicusLatus* l;
              NavigatorMontatio* m;
                     ManusLudus* manus;
    constans NavigatorArticulus* beta;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", chorda_ut_cstr(
        x->articulus, c->piscina)));
    (vacuum)vicus_pulsare(v);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    m = (NavigatorMontatio*)l->montatio;
    CREDO_VERUM(campus_est(m, "articulus", chorda_ut_cstr(x->articulus,
        c->piscina)));
    (vacuum)navigator_versionem_offerre(m->bibliotheca,
        chorda_ex_literis("https://la.wikipedia.org/wiki/Beta",
        c->piscina), chorda_ex_literis(
        "https://la.wikipedia.org/wiki/Beta", c->piscina), "vicipaedia",
        chorda_ex_literis(PAGINA_BETAE, c->piscina),
        vicipaedia_articulus, NIHIL);
    beta = navigator_articulum_invenire(m->bibliotheca,
        chorda_ex_literis("https://la.wikipedia.org/wiki/Beta",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(beta);

    CREDO_GRADUS("nexus ad Betam (servatam): aperitur");
    CREDO_VERUM(navigator_argumentum_ponere(m,
        "https://la.wikipedia.org/wiki/Beta"));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "articulus", chorda_ut_cstr(
        beta->identitas, c->piscina)));

    CREDO_GRADUS("Ctrl+O: Rheum iterum");
    manus = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    (vacuum)manus_ludus_clavem(manus, 'o', (i32)MOD_IMPERIUM);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(campus_est(m, "articulus", chorda_ut_cstr(x->articulus,
        c->piscina)));
}

#define PAGINA_NEXUS \
    "<html><head><title>Nexus</title></head><body>" \
    "<section data-mw-section-id=\"0\"><p>Vide <a " \
    "rel=\"mw:WikiLink\" href=\"./Rheum\">Rheum</a> nunc.</p>" \
    "</section></body></html>"

/* N4.2 (Franus 2026-10-28: ictus in vico nihil faciebat): latus in
 * vico non in origine schirmi - ictus in nexu per manum ad punctum
 * schirmi (ex pictura lateris) Rheum aperit */
interior vacuum
_probare_ictus_nexus (
    CredoContextus* c)
{
                         Mundus* x = (Mundus*)c->datum;
                          Vicus* v;
                     VicusLatus* l;
              NavigatorMontatio* m;
                     ManusLudus* manus;
                      Componens* radix;
                      Componens* textus;
                        Punctum  locale;
                        Punctum  schirmum;
    constans NavigatorArticulus* nexus;
                            s32  lateris;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", NIHIL));
    (vacuum)vicus_pulsare(v);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    m = (NavigatorMontatio*)l->montatio;
    (vacuum)navigator_versionem_offerre(m->bibliotheca,
        chorda_ex_literis("https://la.wikipedia.org/wiki/Nexus",
        c->piscina), chorda_ex_literis(
        "https://la.wikipedia.org/wiki/Nexus", c->piscina),
        "vicipaedia",
        chorda_ex_literis(PAGINA_NEXUS, c->piscina),
        vicipaedia_articulus, NIHIL);
    nexus = navigator_articulum_invenire(m->bibliotheca,
        chorda_ex_literis("https://la.wikipedia.org/wiki/Nexus",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(nexus);
    CREDO_VERUM(navigator_argumentum_ponere(m, chorda_ut_cstr(
        nexus->identitas, c->piscina)));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);

    CREDO_GRADUS("latus non in origine: ictus ad punctum nexus");
    radix = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("navigator", c->piscina));
    CREDO_NECESSE_NON_NIHIL(radix);
    CREDO_VERUM(radix->fines.x > ZEPHYRUM || radix->fines.y > ZEPHYRUM);
    /* punctum ex dispositione (pictura sola in vico per spatium
     * lateris registratur): componens "textus", margo lateris ut
     * _margines (columnae - XL) / II in [I, X]; linea paragraphi post
     * titulum articuli (XVI px) et intervallum (VIII): y XXIV; nexus a
     * columna V ("Vide ") */
    textus = componens_invenire_per_id(radix,
        chorda_ex_literis("textus",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(textus);
    lateris   = (radix->fines.latitudo / VI - II - XL) / II;
    lateris   = lateris > X ? X : lateris < I ? I : lateris;
    locale.x  = textus->fines.x + lateris * VI + V * VI + III;
    locale.y  = textus->fines.y + XXIV + IV;
    manus     = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    schirmum = manus_ludus_ad_schirmum(manus, radix, locale);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "articulus", chorda_ut_cstr(x->articulus,
        c->piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera", _probare_latera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "articulus", _probare_articulus, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "retro", _probare_retro, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "ictus nexus", _probare_ictus_nexus, mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_navigator", SECTIONES);
}
