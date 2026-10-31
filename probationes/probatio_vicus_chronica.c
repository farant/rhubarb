/* probatio_vicus_chronica.c - chronica in vico (C3): genus
 * 'chronica' montatur super fontes registri communis (eventa libri
 * communis vici), argumentum annum aperit, locus annum nominat, ictus
 * in sagitta pedis ad punctum schirmi VERUM (latus non in 0,0) annum
 * movet. */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "chronica.h"
#include "scriba_applicatio.h"
#include "thema.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "credo.h"

#include <stdlib.h>
#include <string.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
} Mundus;

/* volumen cum pagina libri communis: eventa 1225 et 1274 */
interior vacuum
mundum_parare (
    CredoContextus* c)
{
                 Mundus* x;
    InternamentumChorda* intern;
            ScribaLiber* l;
       ScribaDocumentum* d;
      TabulaCharacterum  folium;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    c->datum = x;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_chronica");
    si (!x->volumen)
    {
        redde;
    }
    intern = internamentum_creare(c->piscina);
    l = scriba_liber_aperire(c->piscina, intern, x->volumen, LXXX, XX);
    d = l ? scriba_liber_paginam_condere(l,
        chorda_ex_literis("historia",
        c->piscina)) : NIHIL;
    si (d)
    {
        tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
            d->latitudo, d->altitudo,
            "<event year=\"1225\">Thomas nascitur</event>\n"
            "<event year=\"1274\">Thomas moritur</event>");
        (vacuum)scriba_documentum_committere(d, &folium);
    }
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
    redde x && x->volumen
        && vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM);
}

/* latus frontale dextrum tabulae activae, si chronica */
interior ChronicaMontatio*
chronica_frontalis (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata
        || !chorda_aequalis_literis(l->genus, "chronica"))
    {
        redde NIHIL;
    }
    redde (ChronicaMontatio*)l->montatio;
}

interior b32
campus_est (
      ChronicaMontatio* m,
    constans character* titulus,
    constans character* valor)
{
    chorda* a = insula_ramus_attributum(&m->ramus, titulus);

    redde a && chorda_aequalis_literis(*a, valor);
}

interior vacuum
_probare_latus (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
    ChronicaMontatio* m;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica",
        "chronica/annus/1225"));
    (vacuum)vicus_pulsare(v);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_VERUM(campus_est(m, "annus", "1225"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(m, c->piscina),
        "chronica/annus/1225");
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus_titulus(m, c->piscina),
        "Timeline - AD 1225 - 1/1");

    CREDO_GRADUS("argumentum nudum: flumen");
    CREDO_VERUM(chronica_argumentum_ponere(m, "1274"));
    CREDO_VERUM(campus_est(m, "visus", "flumen"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(m, c->piscina),
        "chronica/flumen/1225");
}

/* ictus ad punctum schirmi: sagitta dextra pedis -> annus proximus */
interior vacuum
_probare_ictum (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
    ChronicaMontatio* m;
           Componens* radix;
           Componens* sagitta;
          ManusLudus* manus;
             Punctum  locale;
             Punctum  schirmum;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica",
        "chronica/annus/1225"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    radix = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("chronica", c->piscina));
    CREDO_NECESSE_NON_NIHIL(radix);
    sagitta = componens_invenire_per_id(radix,
        chorda_ex_literis("sagitta_proxima", c->piscina));
    CREDO_NECESSE_NON_NIHIL(sagitta);
    locale.x  = sagitta->fines.x + sagitta->fines.latitudo / II;
    locale.y  = sagitta->fines.y + sagitta->fines.altitudo / II;
    manus     = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    schirmum = manus_ludus_ad_schirmum(manus, radix, locale);
    CREDO_VERUM(schirmum.x > locale.x);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "annus", "1274"));
}

/* C4a: ictus in textu eventus ad punctum schirmi eum eligit. Punctum
 * ex dispositione fluminis: columna eventuum post quintam partem
 * areae (limbus VI + margo IV = X px a sinistro), linea prima XII px
 * a summo (limbus VIII + margo IV) - lib/chronica.c */
interior vacuum
_probare_electionem_ictu (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
    ChronicaMontatio* m;
           Componens* radix;
          ManusLudus* manus;
             Punctum  locale;
             Punctum  schirmum;
              chorda* a;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica", "1225"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    radix = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("chronica", c->piscina));
    CREDO_NECESSE_NON_NIHIL(radix);
    locale.x  = X + (radix->fines.latitudo - XX) / V + III;
    locale.y  = XII + III;
    manus     = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    schirmum = manus_ludus_ad_schirmum(manus, radix, locale);
    CREDO_VERUM(schirmum.x > locale.x);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    a = insula_ramus_attributum(&m->ramus, "electus");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(chorda_continet(*a, chorda_ex_literis("historia#",
        c->piscina)));
    CREDO_FALSUM(chorda_continet(*a, chorda_ex_literis("#p",
        c->piscina)));
}

/* C4b: liber comitis diei cum eventu anni 1100 in linea 1 et nota
 * comitis (semita calendarii) */
interior b32
comitem_parare (
            Mundus* x,
    CredoContextus* c)
{
    InternamentumChorda* intern;
            ScribaLiber* l;
       ScribaDocumentum* d;
      TabulaCharacterum  folium;

    intern = internamentum_creare(c->piscina);
    l = scriba_liber_aperire_in_spatio(c->piscina, intern, x->volumen,
        "comes/calendarium/dies/2026-10-30", LXXX, XX);
    d = l ? scriba_liber_pagina(l, chorda_ex_literis("1", c->piscina))
          : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        d->latitudo, d->altitudo,
        "Nota diei\n<event year=\"1100\">Comes diei</event>");
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM
        && scriba_liber_comitem_notare(x->volumen, intern,
               "comes/calendarium/dies/2026-10-30", chorda_ex_literis(
               "Notes \302\267 Fri 30 Oct 2026", c->piscina),
               chorda_ex_literis("calendarium/dies/2026-10-30",
               c->piscina), c->piscina);
}

interior ScribaMontatio*
scriba_sinistra (
    Vicus* v)
{
    VicusLatus* s = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);

    redde s && s->montata && chorda_aequalis_literis(s->genus, "scriba")
        ? (ScribaMontatio*)s->montatio : NIHIL;
}

interior b32
genus_dextrum (
                 Vicus* v,
    constans character* genus)
{
    VicusLatus* l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);

    redde l && l->montata && chorda_aequalis_literis(l->genus, genus);
}

interior b32
campus_scribae (
        ScribaMontatio* m,
    constans character* titulus,
    constans character* valor)
{
    chorda* a = insula_ramus_attributum(&m->ramus, titulus);

    redde a && chorda_aequalis_literis(*a, valor);
}

/* eventus comitis: visus comitatus dexter, comes in sinistro ad
 * paginam, cursor in linea; Ctrl+O dextra chronicam reddit */
interior vacuum
_probare_domum_comitis (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
          ManusLudus* manus;
      ScribaMontatio* sm;
    ChronicaMontatio* m;

    CREDO_NECESSE(x && x->volumen && comitem_parare(x, c));
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica",
        "chronica/annus/1100"));
    (vacuum)vicus_pulsare(v);
    manus = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    (vacuum)manus_ludus_clavem(manus, '\t', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    (vacuum)manus_ludus_clavem(manus, '\r', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);

    CREDO_GRADUS("dextra: visus comitatus (calendarium)");
    CREDO_VERUM(genus_dextrum(v, "calendarium"));

    CREDO_GRADUS("sinistra: comes ad paginam, cursor in linea eventus");
    sm = scriba_sinistra(v);
    CREDO_NECESSE_NON_NIHIL(sm);
    CREDO_CHORDA_AEQUALIS_LITERIS(scriba_liber_praefixum(sm->liber),
        "comes/calendarium/dies/2026-10-30");
    CREDO_VERUM(campus_scribae(sm, "pagina", "1"));
    CREDO_VERUM(campus_scribae(sm, "cursor_linea", "1"));

    CREDO_GRADUS("focus dexter, Ctrl+O: chronica eodem anno");
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    (vacuum)manus_ludus_clavem(manus, 'o', (i32)MOD_IMPERIUM);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(genus_dextrum(v, "chronica"));
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_VERUM(campus_est(m, "annus", "1100"));
}

/* eventus libri communis: pagina in sinistro ad lineam, chronica
 * dextra manet */
interior vacuum
_probare_domum_communem (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
          ManusLudus* manus;
      ScribaMontatio* sm;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica",
        "chronica/annus/1274"));
    (vacuum)vicus_pulsare(v);
    manus = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    (vacuum)manus_ludus_clavem(manus, '\t', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    (vacuum)manus_ludus_clavem(manus, '\r', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(genus_dextrum(v, "chronica"));
    sm = scriba_sinistra(v);
    CREDO_NECESSE_NON_NIHIL(sm);
    CREDO_VERUM(campus_scribae(sm, "pagina", "historia"));
    CREDO_VERUM(campus_scribae(sm, "cursor_linea", "1"));
}

/* ictus in eventu electo ad punctum schirmi domum aperit (linea prima
 * fluminis, ut 'electio ictu') */
interior vacuum
_probare_domum_ictu (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
           Componens* radix;
          ManusLudus* manus;
      ScribaMontatio* sm;
             Punctum  locale;
             Punctum  schirmum;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica", "1225"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    radix = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("chronica", c->piscina));
    CREDO_NECESSE_NON_NIHIL(radix);
    locale.x  = X + (radix->fines.latitudo - XX) / V + III;
    locale.y  = XII + III;
    manus     = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    schirmum = manus_ludus_ad_schirmum(manus, radix, locale);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    sm = scriba_sinistra(v);
    CREDO_NECESSE_NON_NIHIL(sm);
    CREDO_VERUM(campus_scribae(sm, "pagina", "historia"));
    CREDO_VERUM(campus_scribae(sm, "cursor_linea", "0"));
}

/* C5b: p index personarum, Ctrl+O redit ad annum; persona aperta
 * locum suum habet (comes eius) */
interior vacuum
_probare_personas_historia (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
          ManusLudus* manus;
    ChronicaMontatio* m;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica",
        "chronica/annus/1274"));
    (vacuum)vicus_pulsare(v);
    manus = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);

    CREDO_GRADUS("p: index personarum");
    (vacuum)manus_ludus_clavem(manus, 'p', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "visus", "personae"));

    CREDO_GRADUS("Ctrl+O: visus anni, annus idem");
    (vacuum)manus_ludus_clavem(manus, 'o', (i32)MOD_IMPERIUM);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_VERUM(campus_est(m, "visus", "annus"));
    CREDO_VERUM(campus_est(m, "annus", "1274"));

    CREDO_GRADUS("persona: locus suus (comes)");
    CREDO_VERUM(chronica_argumentum_ponere(m,
        "chronica/persona/Tom%C3%A1s"));
    (vacuum)vicus_pulsare(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus_locus(v, vicus_latus(
        vicus_activa(v), VICUS_DEXTRUM), c->piscina),
        "chronica/persona/Tom%C3%A1s");
    (vacuum)manus_ludus_clavem(manus, 'o', (i32)MOD_IMPERIUM);
    (vacuum)vicus_pulsare(v);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_VERUM(campus_est(m, "visus", "annus"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latus", _probare_latus, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "ictus", _probare_ictum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "electio ictu", _probare_electionem_ictu, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "domum comitis", _probare_domum_comitis, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "domum communem", _probare_domum_communem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "domum ictu", _probare_domum_ictu, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "personae historia", _probare_personas_historia, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_chronica", SECTIONES);
}
