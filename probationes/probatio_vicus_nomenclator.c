/* probatio_vicus_nomenclator.c - nomenclator in vico (nomenclator-plan
 * P2a): genus 'nomenclator' montatur, documentum personarum UNUM
 * lateribus commune, verbum '$contacts' in pagina scribae latus
 * aperit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "nomenclator.h"
#include "personae.h"
#include "thema.h"
#include "manus_ludus.h"
#include "dispensator.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_nomenclator");
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

/* latus frontale tabulae activae, si nomenclator */
interior NomenclatorMontatio*
nomenclator_frontalis (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "nomenclator"))
    {
        redde NIHIL;
    }
    redde (NomenclatorMontatio*)l->montatio;
}

interior b32
aedificare (
              Mundus* x,
      CredoContextus* c)
{
    redde x && x->volumen && vicus_applicatio_aedificare(&x->app,
        c->piscina, internamentum_creare(c->piscina), x->volumen,
        CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM);
}

/* latera duo: documentum personarum unum */
interior vacuum
_probare_latera (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
    NomenclatorMontatio* prima;
    NomenclatorMontatio* altera;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    prima = nomenclator_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(prima);
    CREDO_NON_NIHIL(prima->documentum);
    /* tabula alia: latus novum (in eadem tabula idem genus sine
     * argumento latus exstans reddit) */
    CREDO_NECESSE(vicus_activam_ponere(v, "2"));
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    altera = nomenclator_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_VERUM(altera != prima);
    CREDO_VERUM(altera->documentum == prima->documentum);
    /* persona in documento communi: in utroque latere */
    CREDO_VERUM(personae_hominem_creare(prima->documentum,
        chorda_ex_literis("Anna", c->piscina),
        chorda_ex_literis("Rossi",
        c->piscina)) > ZEPHYRUM);
    CREDO_AEQUALIS_S32(nomenclator_persona_electa(altera), I);
}

/* '$contacts' in pagina scribae: ictus in verbo latus nomenclatoris
 * aperit */
interior vacuum
_probare_verbum (
    CredoContextus* c)
{
         Mundus* x;
          Vicus* v;
     ManusLudus* m;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);
    (vacuum)manus_ludus_premere_ad(m, VI + III, VICUS_ALTITUDO_TABULARUM
        + VIII + II * VIII + IV);
    CREDO_VERUM(manus_ludus_scribere(m, "i$contacts"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV);
    (vacuum)vicus_pulsare(v);
    CREDO_NON_NIHIL(nomenclator_frontalis(v));
}

/* P2b: in latere focato n, "Anna Rossi", Enter - persona creata,
 * nomina divisa (manus ludus: claves verae vici) */
interior vacuum
_probare_novam (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
             ManusLudus* m;
    NomenclatorMontatio* n;
       constans Persona* q;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    n = nomenclator_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(n);
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#nomenclator"));
    CREDO_VERUM(manus_ludus_scribere(m, "nAnna Rossi"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
    q = personae_persona(n->documentum, I);
    CREDO_NECESSE_NON_NIHIL(q);
    CREDO_CHORDA_AEQUALIS_LITERIS(q->praenomen, "Anna");
    CREDO_CHORDA_AEQUALIS_LITERIS(q->cognomen, "Rossi");
}

/* P5b: notae personae per vicum - Anna creata et aperta (Enter),
 * ictus in charta, "ix" Esc: gestus per faciem lateris
 * (nomenclator_gestum_ponere); 'x' in libro "persona/1" voluminis vici;
 * Ctrl-Shift-Dextra -> pagina II, pulsus vici indicium reficit */
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
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_NECESSE_NON_NIHIL(nomenclator_frontalis(v));
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#nomenclator"));
    CREDO_VERUM(manus_ludus_scribere(m, "nAnna Rossi"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XIII, ZEPHYRUM));
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
        internamentum_creare(c->piscina), x->volumen, "persona/1",
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
    }
}

/* P6b: latus montatum cum argumento "<id>" personam aperit (via
 * montationis - argumentum_ponere non vocatur) */
interior vacuum
_probare_argumentum (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
     PersonaeDocumentum* doc;
    NomenclatorMontatio* n;
                 chorda* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->volumen);
    doc = personae_aperire(c->piscina, internamentum_creare(c->piscina),
        x->volumen, "personae", ZEPHYRUM);
    CREDO_NECESSE(doc && personae_hominem_creare(doc, chorda_ex_literis(
        "Anna", c->piscina), chorda_ex_literis("Rossi", c->piscina))
        == I);
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", "1"));
    (vacuum)vicus_pulsare(v);
    n = nomenclator_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(n);
    a = insula_ramus_attributum(&n->ramus, "persona_aperta");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera", _probare_latera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "verbum", _probare_verbum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "nova", _probare_novam, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "notae", _probare_notas, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "argumentum", _probare_argumentum, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_nomenclator", SECTIONES);
}
