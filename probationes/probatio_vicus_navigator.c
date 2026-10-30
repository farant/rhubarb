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
#include "navigator_paginae.h"
#include "lector_paginae.h"
#include "codex.h"
#include "fons.h"
#include "scriba_applicatio.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
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

/* N4.3: latere focato (ut j k) Tab per manum in vico nexum eligit -
 * focum non movet - Enter Rheum aperit. (Latere non focato Tab focum
 * vici movet - Tab innatus dispensatoris.) */
interior vacuum
_probare_tabula_nexus (
    CredoContextus* c)
{
                         Mundus* x = (Mundus*)c->datum;
                          Vicus* v;
                     VicusLatus* l;
              NavigatorMontatio* m;
                     ManusLudus* manus;
    constans NavigatorArticulus* nexus;

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
    manus = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);

    CREDO_GRADUS("Tab: nexus electus (latus focum tenet)");
    (vacuum)manus_ludus_clavem(manus, '\t', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(insula_ramus_attributum(&m->ramus, "nexus") != NIHIL);
    CREDO_VERUM(latus_frontale(v) == l);

    CREDO_GRADUS("Enter: Rheum aperitur");
    (vacuum)manus_ludus_clavem(manus, '\r', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "articulus", chorda_ut_cstr(x->articulus,
        c->piscina)));
}

/* N4.5: ~ (latere focato) nexus celat; vicus renatus super idem
 * volumen campum persistentem restituit */
interior vacuum
_probare_nexus_celati (
    CredoContextus* c)
{
                   Mundus* x = (Mundus*)c->datum;
                    Vicus* v;
               VicusLatus* l;
        NavigatorMontatio* m;
               ManusLudus* manus;
          VicusApplicatio  altera;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", chorda_ut_cstr(
        x->articulus, c->piscina)));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    m      = (NavigatorMontatio*)l->montatio;
    manus  = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);

    CREDO_GRADUS("~: nexus celati");
    (vacuum)manus_ludus_clavem(manus, '~', (i32)MOD_SHIFT);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "nexus_occulti", "1"));

    CREDO_GRADUS("vicus renatus: campus restitutus");
    memset(&altera, ZEPHYRUM, magnitudo(altera));
    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    x->app = altera;
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_pulsare(x->app.vicus);
    l = latus_frontale(x->app.vicus);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(campus_est((NavigatorMontatio*)l->montatio,
        "nexus_occulti", "1"));
}

/* N6.2: visus sententiae in vico - renascitur in sententia eadem;
 * comes (Cmd+T) notas loci sententiae scribit, sententia alia sine */
interior vacuum
_probare_sententia_vicus (
    CredoContextus* c)
{
                   Mundus* x = (Mundus*)c->datum;
                    Vicus* v;
               VicusLatus* l;
        NavigatorMontatio* m;
               ManusLudus* manus;
          VicusApplicatio  altera;
                   chorda  sententia;
               VicusLatus* sinister;
           ScribaMontatio* sm;
        TabulaCharacterum  folium;
                   chorda* a;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", chorda_ut_cstr(
        chorda_concatenare(chorda_ex_literis("navigator/sententia/",
        c->piscina), x->articulus, c->piscina), c->piscina)));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    m = (NavigatorMontatio*)l->montatio;
    CREDO_VERUM(campus_est(m, "visus", "sententia"));
    manus = manus_ludus_creare(c->piscina, x->app.d);
    (vacuum)manus_ludus_clavem(manus, 'j', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    a = insula_ramus_attributum(&m->ramus, "sententia");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_FALSUM(chorda_aequalis_literis(*a, "0"));
    sententia = chorda_transcribere(*a, c->piscina);

    CREDO_GRADUS("comes (Cmd+T) in sententia: notae loci, sententia "
        "alia sine");
    CREDO_FALSUM(m->notas_habet(m->notas_habet_ctx, navigator_locus(m,
        c->piscina)));
    CREDO_VERUM(vicus_comitem_commutare(v));
    (vacuum)vicus_pulsare(v);
    sinister  = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    sm        = sinister ? (ScribaMontatio*)sinister->montatio : NIHIL;
    CREDO_NECESSE(sm && sm->doc);
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        sm->doc->latitudo, sm->doc->altitudo, "nota");
    CREDO_VERUM(scriba_documentum_committere(sm->doc, &folium)
        > ZEPHYRUM);
    CREDO_VERUM(m->notas_habet(m->notas_habet_ctx, navigator_locus(m,
        c->piscina)));
    CREDO_VERUM(navigator_argumentum_ponere(m, chorda_ut_cstr(
        chorda_concatenare(chorda_ex_literis("navigator/sententia/",
        c->piscina), chorda_concatenare(x->articulus, chorda_ex_literis(
        "?sententia=0", c->piscina), c->piscina), c->piscina),
        c->piscina)));
    CREDO_FALSUM(m->notas_habet(m->notas_habet_ctx, navigator_locus(m,
        c->piscina)));
    CREDO_VERUM(navigator_argumentum_ponere(m, chorda_ut_cstr(
        chorda_concatenare(chorda_ex_literis("navigator/sententia/",
        c->piscina), chorda_concatenare(x->articulus,
        chorda_concatenare(
        chorda_ex_literis("?sententia=", c->piscina), sententia,
        c->piscina), c->piscina), c->piscina), c->piscina)));
    (vacuum)vicus_pulsare(v);

    CREDO_GRADUS("vicus renatus: visus sententiae, sententia eadem");
    memset(&altera, ZEPHYRUM, magnitudo(altera));
    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    x->app = altera;
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_pulsare(x->app.vicus);
    l = latus_frontale(x->app.vicus);
    CREDO_NECESSE_NON_NIHIL(l);
    m = (NavigatorMontatio*)l->montatio;
    CREDO_VERUM(campus_est(m, "visus", "sententia"));
    CREDO_VERUM(campus_est(m, "sententia", chorda_ut_cstr(sententia,
        c->piscina)));
}

/* N6.3: ictus in nexu visus sententiae ad punctum schirmi verum -
 * dispositio sententiae (centrata) ut latus eam computat: area ex
 * superficie (_margines/_area), lector_sententiam_disponere super
 * caput articuli, sententia II ("Vide Rheum nunc.") */
interior vacuum
_probare_sententia_ictus (
    CredoContextus* c)
{
                         Mundus* x = (Mundus*)c->datum;
                          Vicus* v;
                     VicusLatus* l;
              NavigatorMontatio* m;
                     ManusLudus* manus;
                      Componens* radix;
                      Componens* textus;
                NavigatorTextus* nt;
                            Xar* sententiae;
                            Xar* lineae;
           constans LectorLinea* linea;
                        Punctum  locale;
                        Punctum  schirmum;
    constans NavigatorArticulus* nexus;
                            s32  lateris;
                            s32  supra;
                            s32  latitudo;
                            s32  altitudo;
                            s32  k;
                            b32  praecisa;

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
    nt = navigator_textum_facere(vicipaedia_articulus(chorda_ex_literis(
        PAGINA_NEXUS, c->piscina),
        "https://la.wikipedia.org/wiki/Nexus",
        c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(nt);
    sententiae = codex_sententiae(&nt->caput, NIHIL, ZEPHYRUM,
        c->piscina);
    CREDO_NECESSE(sententiae && xar_numerus(sententiae) >= II);
    CREDO_VERUM(navigator_argumentum_ponere(m, chorda_ut_cstr(
        chorda_concatenare(chorda_ex_literis("navigator/sententia/",
        c->piscina), nexus->identitas, c->piscina), c->piscina)));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    {
        character s[XXIV];

        sprintf(s, "?sententia=%u", (insignatus integer)((constans
            CodexSententia*)xar_obtinere(sententiae, I))->initium);
        CREDO_VERUM(navigator_argumentum_ponere(m, chorda_ut_cstr(
            chorda_concatenare(chorda_concatenare(chorda_ex_literis(
            "navigator/sententia/", c->piscina), nexus->identitas,
            c->piscina), chorda_ex_literis(s, c->piscina), c->piscina),
            c->piscina)));
    }
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(campus_est(m, "visus", "sententia"));

    CREDO_GRADUS("punctum 'Rheum' ex dispositione sententiae");
    radix = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("navigator", c->piscina));
    CREDO_NECESSE_NON_NIHIL(radix);
    textus = componens_invenire_per_id(radix,
        chorda_ex_literis("textus",
        c->piscina));
    CREDO_NECESSE_NON_NIHIL(textus);
    lateris   = (radix->fines.latitudo / VI - II - XL) / II;
    lateris   = lateris > X ? X : lateris < I ? I : lateris;
    supra     = (radix->fines.altitudo / VIII - II - XX) / II;
    supra     = supra > VI ? VI : supra < I ? I : supra;
    latitudo  = (radix->fines.latitudo / VI - II - II * lateris) * VI;
    altitudo  = (radix->fines.altitudo / VIII - II - II * supra) * VIII;
    lineae = lector_sententiam_disponere(&nt->caput, (constans
        CodexSententia*)xar_obtinere(sententiae, I), (i32)latitudo,
        (i32)altitudo, &praecisa, c->piscina);
    CREDO_NECESSE(lineae && xar_numerus(lineae) > ZEPHYRUM);
    linea = (constans LectorLinea*)xar_obtinere(lineae, ZEPHYRUM);
    k = chorda_invenire_index(chorda_sectio(nt->caput.textus,
        linea->initium, linea->finis), chorda_ex_literis("Rheum",
        c->piscina));
    CREDO_NECESSE(k >= ZEPHYRUM);
    locale.x = textus->fines.x + lateris * VI + linea->x + k
        * fons_latitudo(linea->fons) + II;
    locale.y = textus->fines.y + linea->y + II;

    CREDO_GRADUS("ictus: Rheum aperitur");
    manus = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    schirmum = manus_ludus_ad_schirmum(manus, radix, locale);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "articulus", chorda_ut_cstr(x->articulus,
        c->piscina)));
}

/* comites: notae per viam unam scriptae per aliam inveniuntur
 * (quaestio, dominium, sententia vetus extra locum) */
interior vacuum
_probare_comes_via_alia (
    CredoContextus* c)
{
                  Mundus* x = (Mundus*)c->datum;
                   Vicus* v;
              VicusLatus* l;
              VicusLatus* sinister;
       NavigatorMontatio* m;
          ScribaMontatio* sm;
       TabulaCharacterum  folium;
                  chorda  via;

    CREDO_NECESSE(aedificare(x, c));
    v    = x->app.vicus;
    via  = chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        "navigator/pagina/", c->piscina), x->articulus, c->piscina),
        chorda_ex_literis("?pagina=0&sententia=9&quaestio=picard&"
        "dominium=wikipedia.org", c->piscina), c->piscina);
    CREDO_VERUM(vicus_acervo_aperire(v, "navigator", chorda_ut_cstr(via,
        c->piscina)));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    l = latus_frontale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    m = (NavigatorMontatio*)l->montatio;
    CREDO_VERUM(vicus_comitem_commutare(v));
    (vacuum)vicus_pulsare(v);
    sinister  = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    sm        = sinister ? (ScribaMontatio*)sinister->montatio : NIHIL;
    CREDO_NECESSE(sm && sm->doc);
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        sm->doc->latitudo, sm->doc->altitudo,
        "<event year=\"373\">nota</event>");
    CREDO_VERUM(scriba_documentum_committere(sm->doc, &folium)
        > ZEPHYRUM);

    CREDO_GRADUS("via alia (sine quaestione): notae eaedem");
    via = chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        "navigator/pagina/", c->piscina), x->articulus, c->piscina),
        chorda_ex_literis("?pagina=0", c->piscina), c->piscina);
    CREDO_VERUM(navigator_argumentum_ponere(m, chorda_ut_cstr(via,
        c->piscina)));
    CREDO_VERUM(m->notas_habet(m->notas_habet_ctx, navigator_locus(m,
        c->piscina)));
}

/* sanatio: liber comitis sub loco vitioso (ante vicum scriptus) ad
 * locum canonicum transfertur cum navigator primum aperitur */
interior vacuum
_probare_sanatio (
    CredoContextus* c)
{
                  Mundus* x = (Mundus*)c->datum;
     InternamentumChorda* intern;
             ScribaLiber* liber;
        ScribaDocumentum* d;
       TabulaCharacterum  folium;
                  chorda  vetus;
                  chorda  canon;

    CREDO_NECESSE(x && x->volumen && x->articulus.mensura > ZEPHYRUM);
    intern = internamentum_creare(c->piscina);
    vetus = chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        "comes/navigator/pagina/", c->piscina), x->articulus,
        c->piscina), chorda_ex_literis("?pagina=0&quaestio=picard",
        c->piscina), c->piscina);
    canon = chorda_concatenare(chorda_concatenare(chorda_concatenare(
        chorda_ex_literis("comes/navigator/pagina/", c->piscina),
        x->articulus, c->piscina), chorda_concatenare(
        chorda_ex_literis("?versio=", c->piscina), x->versio,
        c->piscina), c->piscina), chorda_ex_literis("&pagina=0",
        c->piscina), c->piscina);
    liber = scriba_liber_aperire_in_spatio(c->piscina, intern,
        x->volumen, chorda_ut_cstr(vetus, c->piscina), LXXX, XX);
    d = liber ? scriba_liber_pagina(liber, chorda_ex_literis("1",
        c->piscina)) : NIHIL;
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        d->latitudo, d->altitudo, "nota vetus");
    CREDO_NECESSE(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_NECESSE(aedificare(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "navigator", NIHIL));
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_NECESSE_NON_NIHIL(latus_frontale(x->app.vicus));
    CREDO_VERUM(scriba_liber_exstat(x->volumen, chorda_ut_cstr(canon,
        c->piscina), c->piscina));
    CREDO_FALSUM(scriba_liber_exstat(x->volumen, chorda_ut_cstr(vetus,
        c->piscina), c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera", _probare_latera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "comes via alia", _probare_comes_via_alia, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "sanatio", _probare_sanatio, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "articulus", _probare_articulus, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "retro", _probare_retro, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "ictus nexus", _probare_ictus_nexus, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "tabula nexus", _probare_tabula_nexus, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "nexus celati", _probare_nexus_celati, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "sententia vicus", _probare_sententia_vicus, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "sententia ictus", _probare_sententia_ictus, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_navigator", SECTIONES);
}
