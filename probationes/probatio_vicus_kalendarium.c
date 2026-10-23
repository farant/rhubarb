/* probatio_vicus_kalendarium.c - calendarium in vico (agenda-plan
 * A2a): genus 'calendarium' montatur cum argumento, hodie ex
 * horologio vici (fictum), latera duo documentum agendae UNUM
 * communicant. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "kalendarium.h"
#include "agenda.h"
#include "personae.h"
#include "nomenclator.h"
#include "sancti_visus.h"
#include "thema.h"
#include "manus_ludus.h"
#include "insula.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "dispensator.h"
#include "motus.h"
#include "figura.h"
#include "mandatum.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
        VicusTempus  tempus;
} Mundus;

interior vacuum
horologium_fictum (
         vacuum* ctx,
    VicusTempus* tempus)
{
    *tempus = *(VicusTempus*)ctx;
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    /* sancti (fons dierum): fixtura - XXI I agnes, XXVIII I thomas, III
     * VI charles; nullus in X (natales soli) */
    (vacuum)setenv("RHUBARB_SANCTI", "probationes/fixa/sancti", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_kalendarium");
    c->datum = x;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

/* latus frontale tabulae activae, si calendarium */
interior KalendariumMontatio*
kalendarium_frontale (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "calendarium"))
    {
        redde NIHIL;
    }
    redde (KalendariumMontatio*)l->montatio;
}

interior b32
dies_est (
    Dies d,
     s32 annus,
     s32 mensis,
     s32 dies)
{
    redde d.annus == annus && d.mensis == mensis && d.dies == dies;
}

/* montatio cum argumento; hodie ex horologio; documentum commune */
interior vacuum
_probare_latera (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
    KalendariumMontatio* prima;
    KalendariumMontatio* altera;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v                 = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = X;
    x->tempus.dies    = XIV;
    vicus_horologium_ponere(v, horologium_fictum, &x->tempus);

    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-12"));
    (vacuum)vicus_pulsare(v);
    prima = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(prima);
    CREDO_VERUM(dies_est(kalendarium_dies_electus(prima), MMXXVI, XII,
        I));
    CREDO_NON_NIHIL(prima->documentum);

    /* argumentum mensis hodierni: hodie ex horologio vici */
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-10"));
    (vacuum)vicus_pulsare(v);
    altera = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_VERUM(altera != prima);
    CREDO_VERUM(dies_est(kalendarium_dies_electus(altera), MMXXVI, X,
        XIV));
    /* documentum agendae unum */
    CREDO_VERUM(altera->documentum == prima->documentum);
}

/* '$agenda' (A4a): verbum in pagina scribae - latus calendarii cum
 * argumento "agenda" (visus agendae) in fronte post pulsum */
interior vacuum
_probare_agendam (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
             ManusLudus* m;
    KalendariumMontatio* k;
                 chorda* visus;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);
    /* ictus in pagina sinistra (focus), scribere, Esc, ictus in
     * verbo */
    (vacuum)manus_ludus_premere_ad(m, VI + III, VICUS_ALTITUDO_TABULARUM
        + VIII + II * VIII + IV);
    CREDO_VERUM(manus_ludus_scribere(m, "i$agenda"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV);
    (vacuum)vicus_pulsare(v);
    k = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(k);
    visus = insula_ramus_attributum(&k->ramus, "visus");
    CREDO_VERUM(visus && chorda_aequalis_literis(*visus, "agenda"));
}

/* notae diei per vicum (A6d-2): latus calendarii, Enter (visus diei),
 * ictus in charta notarum, "ix" Esc - gestus scribae nidificatae per
 * faciem lateris (kalendarium_gestum_ponere); 'x' in libro XIV
 * voluminis vici */
interior vacuum
_probare_notas (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
             ManusLudus* m;
              Componens* notae;
                Punctum  locale;
                Punctum  schirmus;
            ScribaLiber* l;
       ScribaDocumentum* d;
                    i32  i;
                    b32  inventa;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v                 = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = X;
    x->tempus.dies    = XIV;
    vicus_horologium_ponere(v, horologium_fictum, &x->tempus);
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-10"));
    (vacuum)vicus_pulsare(v);
    CREDO_NECESSE_NON_NIHIL(kalendarium_frontale(v));
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    /* focus in latus calendarii (ictus in eius radice), Enter */
    CREDO_NECESSE(manus_ludus_premere(m, "#kalendarium"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
    dispensator_recomponere(x->app.d);
    notae = manus_ludus_invenire(m, "#notae");
    CREDO_NECESSE_NON_NIHIL(notae);
    locale.x = XL;
    locale.y = XXX;
    schirmus = manus_ludus_ad_schirmum(m, notae, locale);
    CREDO_VERUM(manus_ludus_premere_ad(m, schirmus.x, schirmus.y));
    CREDO_VERUM(manus_ludus_scribere(m, "ix"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)motus_gestum_effundere(dispensator_motus(x->app.d),
        x->app.vicus->repo);
    l = scriba_liber_aperire_in_spatio(c->piscina,
        internamentum_creare(c->piscina), x->volumen, "dies/2026-10-14",
        LXXII, XXX);
    d = l ? scriba_liber_pagina(l, chorda_ex_literis("1", c->piscina))
          : NIHIL;
    CREDO_NECESSE_NON_NIHIL(d);
    inventa = FALSUM;
    per (i = ZEPHYRUM; i < d->latitudo * d->altitudo; i++)
    {
        si (scriba_documentum_tabula(d)->cellulae[i] == 'x')
        {
            inventa = VERUM;
        }
    }
    CREDO_VERUM(inventa);
    /* A6e: Ctrl-Shift-Dextra in notis -> pagina II; pulsus vici
     * indicium reficit (facies.pulsare calendarii) */
    {
            Eventus  e;
        InsulaRamus  rs;
             chorda* a;

        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
        e.datum.clavis.clavis        = CLAVIS_DEXTER;
        e.datum.clavis.modificantes  = (i32)(MOD_IMPERIUM | MOD_SHIFT);
        dispensator_tractare(x->app.d, &e);
        (vacuum)vicus_pulsare(v);
        rs = insula_ramus_ex_via(x->app.vicus->repo,
            dispensator_motus(x->app.d)->spatium_intimum);
        a  = insula_ramus_attributum(&rs, "pagina_positio");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "2"));
        /* D5 S3c: pagina notarum in semita calendarii - Ctrl+O ad
         * paginam I (historia dextra), Ctrl+I ad II */
        (vacuum)manus_ludus_clavem(m, 'o', MOD_IMPERIUM);
        (vacuum)vicus_pulsare(v);
        a  = insula_ramus_attributum(&rs, "pagina_positio");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));
        (vacuum)manus_ludus_clavem(m, 'i', MOD_IMPERIUM);
        (vacuum)vicus_pulsare(v);
        a  = insula_ramus_attributum(&rs, "pagina_positio");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "2"));
    }
}

/* ictus dexter in medio componentis (selector manus) */
interior b32
dexter_in (
                Mundus* x,
            ManusLudus* m,
    constans character* selector)
{
     Componens* c;
       Punctum  locale;
       Punctum  schirmus;
       Eventus  e;

    c = manus_ludus_invenire(m, selector);
    si (!c)
    {
        redde FALSUM;
    }
    locale.x = c->fines.latitudo / II;
    locale.y = c->fines.altitudo / II;
    schirmus = manus_ludus_ad_schirmum(m, c, locale);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = schirmus.x;
    e.datum.mus.y       = schirmus.y;
    e.datum.mus.botton  = MUS_DEXTER;
    dispensator_tractare(x->app.d, &e);
    dispensator_recomponere(x->app.d);
    redde VERUM;
}

/* menu contextuale per vicum (M2c): dimissio claudit - Esc (visus
 * diei manet: non ad mensem), ictus extra (deglutitus: globulus "nova"
 * promptam non aperit), res electa (agit et claudit) */
interior vacuum
_probare_elenchum (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
             ManusLudus* m;
    KalendariumMontatio* k;
                 chorda* a;
              Componens* r;
                Punctum  locale;
                Punctum  schirmus;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v                 = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = X;
    x->tempus.dies    = XIV;
    vicus_horologium_ponere(v, horologium_fictum, &x->tempus);
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-10"));
    (vacuum)vicus_pulsare(v);
    k = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(k);
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#kalendarium"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
    dispensator_recomponere(x->app.d);
    /* Esc: dimissio claudit, visus diei manet */
    CREDO_NECESSE(dexter_in(x, m, "#index_vacuum"));
    CREDO_NON_NIHIL(manus_ludus_invenire(m, "#elenchus"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    dispensator_recomponere(x->app.d);
    CREDO_NIHIL(manus_ludus_invenire(m, "#elenchus"));
    a = insula_ramus_attributum(&k->ramus, "visus");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "dies"));
    /* ictus extra ("nova"): clausum, deglutitus (prompta nulla) */
    CREDO_NECESSE(dexter_in(x, m, "#index_vacuum"));
    CREDO_NON_NIHIL(manus_ludus_invenire(m, "#elenchus"));
    CREDO_VERUM(manus_ludus_premere(m, "#nova"));
    dispensator_recomponere(x->app.d);
    CREDO_NIHIL(manus_ludus_invenire(m, "#elenchus"));
    a = insula_ramus_attributum(&k->ramus, "prompta");
    CREDO_FALSUM(a && chorda_aequalis_literis(*a, "nova"));
    /* res "New item" (elenchus.0): prompta nova, clausum */
    CREDO_NECESSE(dexter_in(x, m, "#index_vacuum"));
    r = manus_ludus_invenire(m, "#elenchus");
    CREDO_NECESSE_NON_NIHIL(r);
    r         = componens_liberum(r, ZEPHYRUM);
    locale.x  = r->fines.latitudo / II;
    locale.y  = r->fines.altitudo / II;
    schirmus  = manus_ludus_ad_schirmum(m, r, locale);
    /* motus super rem: illuminat, menu manet (dimissio post: solum
     * pressio claudit - menu sub muris olim evanescebat, Franus
     * 10-14) */
    CREDO_VERUM(manus_ludus_movere(m, schirmus.x, schirmus.y));
    dispensator_recomponere(x->app.d);
    CREDO_NON_NIHIL(manus_ludus_invenire(m, "#elenchus"));
    a = insula_ramus_attributum(&k->ramus, "elenchus_electa");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "0"));
    CREDO_VERUM(manus_ludus_premere_ad(m, schirmus.x, schirmus.y));
    dispensator_recomponere(x->app.d);
    CREDO_NIHIL(manus_ludus_invenire(m, "#elenchus"));
    a = insula_ramus_attributum(&k->ramus, "prompta");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "nova"));
}

/* P6: natales per vicum - Anna Rossi (natalis 14 X MCMLXXXVI) in
 * documento personarum voluminis vici; calendarium (fons
 * nomenclatoris) ordinem "n0" in visu diei XIV ostendit, nullum in
 * XV; P6b: duplex in natali latus nomenclatoris aperit (montatum,
 * deinde exstans: Luca XX) */
interior vacuum
_probare_natales (
    CredoContextus* c)
{
                  Mundus* x;
                   Vicus* v;
              ManusLudus* m;
      PersonaeDocumentum* doc;
                    Dies  d;
                  chorda  id;
                     i32  numerator;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    doc = personae_aperire(c->piscina, internamentum_creare(c->piscina),
        x->volumen, "personae", ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(doc);
    numerator = ZEPHYRUM;
    personae_monetam_ponere(doc, moneta_numerator, &numerator);
    id = personae_hominem_creare(doc, chorda_ex_literis("Anna",
        c->piscina), chorda_ex_literis("Rossi", c->piscina));
    d  = fasti_dies(MCMLXXXVI, X, XIV);
    CREDO_NECESSE(id.mensura > ZEPHYRUM && personae_natalem_ponere(doc,
        id, &d) > ZEPHYRUM);
    id = personae_hominem_creare(doc, chorda_ex_literis("Luca",
        c->piscina), chorda_ex_literis("Bianchi", c->piscina));
    d  = fasti_dies(ZEPHYRUM, X, XX);
    CREDO_NECESSE(chorda_aequalis_literis(id, "2")
        && personae_natalem_ponere(doc, id, &d) > ZEPHYRUM);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v                 = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = X;
    x->tempus.dies    = XIV;
    vicus_horologium_ponere(v, horologium_fictum, &x->tempus);
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-10"));
    (vacuum)vicus_pulsare(v);
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#kalendarium"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(manus_ludus_invenire(m, "#n0") != NIHIL);
    CREDO_VERUM(manus_ludus_clavem(m, 'l', ZEPHYRUM));
    dispensator_recomponere(x->app.d);
    CREDO_FALSUM(manus_ludus_invenire(m, "#n0") != NIHIL);
    /* P6b: ictus duplex in natali (dies XIV) latus nomenclatoris in
     * frontem fert, Anna aperta */
    CREDO_VERUM(manus_ludus_clavem(m, 'h', ZEPHYRUM));
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    (vacuum)vicus_pulsare(v);
    {
          VicusLatus* l;
         InsulaRamus  r;
              chorda* a;

        l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
        CREDO_NECESSE(l && l->montata);
        CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "nomenclator");
        r = ((NomenclatorMontatio*)l->montatio)->ramus;
        a = insula_ramus_attributum(&r, "persona_aperta");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));
    }
    /* latus nomenclatoris iam praesens: calendarium in frontem, dies XX
     * (Luca, sine anno), duplex - argumentum_ponere lateris
     * exstantis */
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-10"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#kalendarium"));
    CREDO_VERUM(manus_ludus_scribere(m, "llllll"));
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    (vacuum)vicus_pulsare(v);
    {
          VicusLatus* l;
         InsulaRamus  r;
              chorda* a;

        l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
        CREDO_NECESSE(l && l->montata);
        CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "nomenclator");
        r = ((NomenclatorMontatio*)l->montatio)->ramus;
        a = insula_ramus_attributum(&r, "persona_aperta");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "2"));
    }
}

/* textus pictus vici continens fragmentum? */
interior b32
pictum_in_vico (
                 Mundus* x,
     constans character* fragmentum,
                Piscina* p)
{
               Mandata* md;
     constans Mandatum* t;
                   i32  i;

    md = mandata_creare(p, x->app.intern);
    pingere(dispensator_arbor(x->app.d), vicus_figurae(x->app.vicus),
        ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        t = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   t->genus == MANDATUM_TEXTUS
            && chorda_continet(t->textus, chorda_ex_literis(fragmentum,
                p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* sancti-plan V2: fons dierum sanctorum - festum (generale/usa) in
 * visu diei ordo signatus; duplex latus sanctorum cum sancto aperit
 * (origo argumentum), Esc latus tollit et calendarium (dies idem)
 * redit */
interior vacuum
_probare_sanctos (
    CredoContextus* c)
{
                  Mundus* x;
                   Vicus* v;
              ManusLudus* m;
     KalendariumMontatio* k;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v                 = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = I;
    x->tempus.dies    = XXV;
    vicus_horologium_ponere(v, horologium_fictum, &x->tempus);
    /* visus diei XXV (argumentum ISO): nullus sanctus */
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-01-25"));
    (vacuum)vicus_pulsare(v);
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    k = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(dies_est(kalendarium_dies_electus(k), MMXXVI, I, XXV));
    CREDO_NECESSE(manus_ludus_premere(m, "#kalendarium"));
    CREDO_FALSUM(manus_ludus_invenire(m, "#n0") != NIHIL);
    /* XXVIII: thomas (pulsus: historia diem XXVIII notat) */
    CREDO_VERUM(manus_ludus_scribere(m, "lll"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(manus_ludus_invenire(m, "#n0") != NIHIL);
    CREDO_FALSUM(manus_ludus_invenire(m, "#n1") != NIHIL);
    /* titulus: nomen et aetas */
    CREDO_VERUM(pictum_in_vico(x, "Thomas Aquinas \302\267 c. 1225"
        "\342\200\2231274", c->piscina));
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    (vacuum)vicus_pulsare(v);
    {
          VicusLatus* l;
         InsulaRamus  r;
              chorda* a;

        l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
        CREDO_NECESSE(l && l->montata);
        CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "sancti");
        r = ((SanctorumMontatio*)l->montatio)->ramus;
        a = insula_ramus_attributum(&r, "sanctus_apertus");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "thomas-aquinas"));
    }
    /* Esc: index sanctorum (Franus 2026-10-22 - Esc non est "latus
     * prius"); Ctrl+O bis: sanctus, deinde calendarium (XXVIII) */
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    {
          VicusLatus* l;
         InsulaRamus  r;
              chorda* a;

        l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
        CREDO_NECESSE(l && l->montata);
        CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "sancti");
        r = ((SanctorumMontatio*)l->montatio)->ramus;
        a = insula_ramus_attributum(&r, "visus");
        CREDO_VERUM(!a || chorda_aequalis_literis(*a, "index"));
    }
    CREDO_VERUM(manus_ludus_clavem(m, 'o', (i32)MOD_IMPERIUM));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(manus_ludus_clavem(m, 'o', (i32)MOD_IMPERIUM));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    k = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(dies_est(kalendarium_dies_electus(k), MMXXVI, I,
        XXVIII));
    /* latus sanctorum iam praesens (index): XXI agnes, duplex -
     * argumentum_ponere lateris exstantis */
    CREDO_VERUM(vicus_acervo_aperire(v, "sancti", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", "2026-01-21"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    /* agnes: festum martyrologii eodem die ordinem alterum non facit */
    CREDO_VERUM(manus_ludus_invenire(m, "#n0") != NIHIL);
    CREDO_FALSUM(manus_ludus_invenire(m, "#n1") != NIHIL);
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    CREDO_NECESSE(manus_ludus_premere(m, "#n0"));
    (vacuum)vicus_pulsare(v);
    {
          VicusLatus* l;
         InsulaRamus  r;
              chorda* a;

        l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
        CREDO_NECESSE(l && l->montata);
        CREDO_CHORDA_AEQUALIS_LITERIS(l->genus, "sancti");
        r = ((SanctorumMontatio*)l->montatio)->ramus;
        a = insula_ramus_attributum(&r, "sanctus_apertus");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "agnes-of-rome"));
    }
}

/* focus in notis, deinde visus alius per globulum (Franus 10-15:
 * "res nova in agenda: nihil scribi potest"): notae iam non
 * monstrantur - claves ad calendarium redeunt (prompta in agenda
 * textum accipit; in mense L mensem mutat) */
interior b32
ad_notas_focus (
     Mundus* x,
 ManusLudus* m)
{
     Componens* notae;
       Punctum  locale;
       Punctum  schirmus;

    (vacuum)vicus_acervo_aperire(x->app.vicus, "calendarium",
        "2026-10");
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    /* visus mensis (latus iam in agenda esse potest) */
    (vacuum)manus_ludus_premere(m, "#visus-mensis");
    dispensator_recomponere(x->app.d);
    si (   !manus_ludus_premere(m, "#kalendarium")
        || !manus_ludus_clavem(m, (character)XIII, ZEPHYRUM))
    {
        redde FALSUM;
    }
    dispensator_recomponere(x->app.d);
    notae = manus_ludus_invenire(m, "#notae");
    si (!notae)
    {
        redde FALSUM;
    }
    locale.x = XL;
    locale.y = XXX;
    schirmus = manus_ludus_ad_schirmum(m, notae, locale);
    redde manus_ludus_premere_ad(m, schirmus.x, schirmus.y);
}

interior vacuum
_probare_visus_post_notas (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
             ManusLudus* m;
    KalendariumMontatio* k;
                 chorda* a;
                    i32  i;
                    b32  inventa;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    CREDO_NECESSE(vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v                 = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = X;
    x->tempus.dies    = XIV;
    vicus_horologium_ponere(v, horologium_fictum, &x->tempus);
    m = manus_ludus_creare(c->piscina, x->app.d);

    /* notae -> Agenda -> + New -> "pane" Enter */
    CREDO_NECESSE(ad_notas_focus(x, m));
    CREDO_NECESSE(manus_ludus_premere(m, "#visus-agenda"));
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#nova"));
    k = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(k);
    a = insula_ramus_attributum(&k->ramus, "prompta");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "nova"));
    CREDO_VERUM(manus_ludus_scribere(m, "pane"));
    a = insula_ramus_attributum(&k->ramus, "prompta_textus");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "pane"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
    inventa = FALSUM;
    per (i = ZEPHYRUM; k->documentum
        && i < agenda_numerus_rerum(k->documentum); i++)
    {
        si (chorda_aequalis_literis(agenda_res_index(k->documentum,
            i)->titulus, "pane"))
        {
            inventa = VERUM;
        }
    }
    CREDO_VERUM(inventa);

    /* notae -> Month -> L: mensis sequens */
    CREDO_NECESSE(ad_notas_focus(x, m));
    CREDO_NECESSE(manus_ludus_premere(m, "#visus-mensis"));
    dispensator_recomponere(x->app.d);
    k = kalendarium_frontale(v);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_VERUM(manus_ludus_clavem(m, 'L', (i32)MOD_SHIFT));
    CREDO_VERUM(dies_est(kalendarium_dies_electus(k), MMXXVI, XI,
        XIV));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera",  _probare_latera,  mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "natales", _probare_natales, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "sancti",  _probare_sanctos, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "agenda",  _probare_agendam, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "notae",   _probare_notas,   mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "elenchus", _probare_elenchum, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "visus_post_notas", _probare_visus_post_notas, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_kalendarium", SECTIONES);
}
