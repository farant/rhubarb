/* probatio_vicus_lector.c - lector in vico (lector L3): genus 'lector'
 * montatur, bibliotheca UNA lateribus communis (RHUBARB_CODICES ad
 * fixa), latus novum bibliothecam monstrat; T5: duo latera, restitutio,
 * $via. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "lector.h"
#include "lector_bibliotheca.h"
#include "scriba_liber.h"
#include "scriba_applicatio.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "thema.h"
#include "manus_ludus.h"
#include "dispensator.h"
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
    (vacuum)setenv("RHUBARB_CODICES",
        "probationes/fixa/lector/bibliotheca",
        I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_lector");
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

interior b32
aedificare (
              Mundus* x,
      CredoContextus* c)
{
    redde x && x->volumen && vicus_applicatio_aedificare(&x->app,
        c->piscina, internamentum_creare(c->piscina), x->volumen,
        CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM);
}

/* latus frontale tabulae activae, si lector */
interior LectorMontatio*
lector_frontalis (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "lector"))
    {
        redde NIHIL;
    }
    redde (LectorMontatio*)l->montatio;
}

/* latera duo, bibliotheca una; latus novum = bibliotheca */
interior vacuum
_probare_latera (
    CredoContextus* c)
{
            Mundus* x;
             Vicus* v;
    LectorMontatio* prima;
    LectorMontatio* altera;
            chorda* visus;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "lector", NIHIL));
    (vacuum)vicus_pulsare(v);
    prima = lector_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(prima);
    CREDO_NECESSE_NON_NIHIL(prima->bibliotheca);
    CREDO_AEQUALIS_I32(lector_bibliotheca_numerus(prima->bibliotheca),
        III);
    CREDO_CHORDA_AEQUALIS_LITERIS(lector_bibliotheca_liber(
        prima->bibliotheca, I)->titulus, "Alpha");
    visus = insula_ramus_attributum(&prima->ramus, "visus");
    CREDO_VERUM(visus
        && chorda_aequalis_literis(*visus, "bibliotheca"));
    CREDO_NECESSE(vicus_activam_ponere(v, "2"));
    CREDO_VERUM(vicus_acervo_aperire(v, "lector", NIHIL));
    (vacuum)vicus_pulsare(v);
    altera = lector_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_VERUM(altera != prima);
    CREDO_VERUM(altera->bibliotheca == prima->bibliotheca);
}

/* duo latera, liber unus: ultima mutatio vincit (latus aperiens
 * positionem LIBRI ostendit) */
interior vacuum
_probare_duo (
    CredoContextus* c)
{
            Mundus* x;
             Vicus* v;
    LectorMontatio* a;
    LectorMontatio* b;
            chorda* caput;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "lector",
        "lector/pagina/urn%3Arhubarb%3Aalpha?caput=3&pagina=0"));
    (vacuum)vicus_pulsare(v);
    a = lector_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(a);
    caput = insula_ramus_attributum(&a->ramus, "caput");
    CREDO_VERUM(caput && chorda_aequalis_literis(*caput, "3"));

    CREDO_NECESSE(vicus_activam_ponere(v, "2"));
    CREDO_VERUM(vicus_acervo_aperire(v, "lector", "urn:rhubarb:alpha"));
    (vacuum)vicus_pulsare(v);
    b = lector_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(b != a);
    caput = insula_ramus_attributum(&b->ramus, "caput");
    CREDO_VERUM(caput && chorda_aequalis_literis(*caput, "3"));

    CREDO_VERUM(lector_argumentum_ponere(b,
        "lector/pagina/urn%3Arhubarb%3Aalpha?caput=1&pagina=0"));

    CREDO_NECESSE(vicus_activam_ponere(v, "1"));
    CREDO_VERUM(vicus_acervo_mutare(v, "lector", "urn:rhubarb:alpha"));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(lector_frontalis(v) == a);
    caput = insula_ramus_attributum(&a->ramus, "caput");
    CREDO_VERUM(caput && chorda_aequalis_literis(*caput, "1"));
}

/* '$via(lector/pagina/<id>)' in pagina scribae: ictus in verbo latus
 * lectoris aperit, liber ad positionem suam (nulla: caput 0) */
interior vacuum
_probare_via (
    CredoContextus* c)
{
            Mundus* x;
             Vicus* v;
        ManusLudus* m;
    LectorMontatio* l;
            chorda* caput;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);
    (vacuum)manus_ludus_premere_ad(m, VI + III, VICUS_ALTITUDO_TABULARUM
        + VIII + II * VIII + IV);
    /* iussum in linea UNA paginae (pagina tabulae III dimidia: ~38
     * columnae; iussum involutum non agnoscitur) */
    CREDO_VERUM(manus_ludus_scribere(m,
        "i$via(lector/pagina/urn:rhubarb:alpha)"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV);
    (vacuum)vicus_pulsare(v);
    l = lector_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(l);
    caput = insula_ramus_attributum(&l->ramus, "caput");
    CREDO_VERUM(caput && chorda_aequalis_literis(*caput, "0"));
    caput = insula_ramus_attributum(&l->ramus, "visus");
    CREDO_VERUM(caput && chorda_aequalis_literis(*caput, "pagina"));
}

/* lector L5: notas_habet hospitis = liber 'comes/<locus>' in volumine
 * vici TEXTUM habet (liber vacuus - comes veteris moris, aut notae
 * deletae - non numeratur); locus alius (sententia proxima) sine
 * notis */
interior vacuum
_probare_notae (
    CredoContextus* c)
{
                  Mundus* x;
                   Vicus* v;
          LectorMontatio* l;
     InternamentumChorda* intern;
             ScribaLiber* liber;
        ScribaDocumentum* d;
       TabulaCharacterum  folium;
                  chorda  locus;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v       = x->app.vicus;
    intern  = internamentum_creare(c->piscina);
    CREDO_VERUM(vicus_acervo_aperire(v, "lector",
        "lector/sententia/urn%3Arhubarb%3Aalpha?caput=3&sententia=19"));
    (vacuum)vicus_pulsare(v);
    l = lector_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE(l->notas_habet != NIHIL);
    locus = chorda_transcribere(lector_locus(l, c->piscina),
        c->piscina);
    CREDO_FALSUM(l->notas_habet(l->notas_habet_ctx, locus));

    CREDO_GRADUS("liber comitis vacuus (mos vetus): notae nullae");
    liber = scriba_liber_aperire_in_spatio(c->piscina, intern,
        x->volumen, chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        "comes/", c->piscina), locus, c->piscina), c->piscina), XL, X);
    CREDO_NECESSE_NON_NIHIL(liber);
    d = scriba_liber_pagina(liber, chorda_ex_literis("1", c->piscina));
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_FALSUM(l->notas_habet(l->notas_habet_ctx, locus));

    CREDO_GRADUS("comes scriptus: notae; deletae: nullae; sententia "
        "proxima sine");
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        d->latitudo, d->altitudo, "ladder");
    CREDO_VERUM(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_VERUM(l->notas_habet(l->notas_habet_ctx, locus));
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        d->latitudo, d->altitudo, "   ");
    CREDO_VERUM(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_FALSUM(l->notas_habet(l->notas_habet_ctx, locus));
    tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
        d->latitudo, d->altitudo, "ladder");
    CREDO_VERUM(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_VERUM(l->notas_habet(l->notas_habet_ctx, locus));
    CREDO_VERUM(lector_argumentum_ponere(l,
        "lector/sententia/urn%3Arhubarb%3Aalpha?caput=3&sententia=31"));
    CREDO_FALSUM(l->notas_habet(l->notas_habet_ctx, lector_locus(l,
        c->piscina)));

    CREDO_GRADUS("Cmd+T (vicus_comitem_commutare) et scriptura: clavis "
        "comitis eadem ac notarum");
    {
         VicusLatus* sinister;
     ScribaMontatio* sm;

        (vacuum)vicus_pulsare(v);
        CREDO_VERUM(vicus_comitem_commutare(v));
        (vacuum)vicus_pulsare(v);
        sinister = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
        sm = sinister ? (ScribaMontatio*)sinister->montatio : NIHIL;
        CREDO_NECESSE(sm && sm->doc);
        CREDO_FALSUM(l->notas_habet(l->notas_habet_ctx, lector_locus(l,
            c->piscina)));
        tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
            sm->doc->latitudo, sm->doc->altitudo, "plums");
        CREDO_VERUM(scriba_documentum_committere(sm->doc, &folium)
            > ZEPHYRUM);
        CREDO_VERUM(l->notas_habet(l->notas_habet_ctx, lector_locus(l,
            c->piscina)));
    }
}

/* vicus latus (MCDXL px): pagina scribae dimidia longa satis ut iussum
 * cum URI in linea una stet */
interior b32
aedificare_lata (
              Mundus* x,
      CredoContextus* c)
{
    redde x && x->volumen && vicus_applicatio_aedificare(&x->app,
        c->piscina, internamentum_creare(c->piscina), x->volumen, MCDXL,
        CDLXXX + VICUS_ALTITUDO_TABULARUM);
}

/* iussum in linea k (vacua) paginae scribae communis scribitur et ictu
 * aperitur - modus probare_via. Pagina communis omnibus tabulis: iussa
 * priora in lineis aliis manent, et ictus ubivis in linea iussi id
 * aperit (etiam post textum) - ergo linea quaeque iussi sua, ictus
 * focalis in linea vacua ipsa */
interior vacuum
iussum_ictu (
                 Mundus* x,
             ManusLudus* m,
     constans character* tabula,
                    s32  linea,
     constans character* iussum)
{
    character textus[CXXVIII];
          s32 y;

    y = VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV + linea * VIII;
    (vacuum)vicus_activam_ponere(x->app.vicus, tabula);
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    (vacuum)manus_ludus_premere_ad(m, VI + III, y);
    sprintf(textus, "i%s", iussum);
    (vacuum)manus_ludus_scribere(m, textus);
    (vacuum)manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM);
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III, y);
    (vacuum)vicus_pulsare(x->app.vicus);
}

/* campus lectoris frontalis aequalis? */
interior b32
lectoris_campus_est (
                  Vicus* v,
     constans character* titulus,
     constans character* valor)
{
    LectorMontatio* l;
            chorda* a;

    l = lector_frontalis(v);
    a = l ? insula_ramus_attributum(&l->ramus, titulus) : NIHIL;
    redde a && chorda_aequalis_literis(*a, valor);
}

/* lector L6: $epubs - latus novum = bibliotheca; $epubs(id) = liber ad
 * positionem suam */
interior vacuum
_probare_iussum_lectoris (
    CredoContextus* c)
{
            Mundus* x;
             Vicus* v;
        ManusLudus* m;
    LectorMontatio* l;
     LectorPositio  q;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare_lata(x, c));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);

    CREDO_GRADUS("$epubs: lector in fronte, bibliotheca");
    iussum_ictu(x, m, "3", ZEPHYRUM, "$epubs");
    l = lector_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(lectoris_campus_est(v, "visus", "bibliotheca"));

    CREDO_GRADUS("$epubs(id): pagina ad positionem libri");
    q.caput      = III;
    q.initium    = ZEPHYRUM;
    q.vestigium  = chorda_ex_literis("line one", c->piscina);
    CREDO_VERUM(lector_positionem_scribere(l->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", c->piscina), &q));
    iussum_ictu(x, m, "2", II, "$epubs(urn:rhubarb:alpha)");
    CREDO_NECESSE_NON_NIHIL(lector_frontalis(v));
    CREDO_VERUM(lector_frontalis(v) != l);
    CREDO_VERUM(lectoris_campus_est(v, "visus", "pagina"));
    CREDO_VERUM(lectoris_campus_est(v, "caput", "3"));
    /* $epubs(URI) currit per eandem viam (lector_argumentum_ponere,
     * probatio_lector semitae); iussum longum in pagina dimidia (LVIII
     * columnae) involvitur nec iussum est */
}

/* lector L6: vicus renascitur super idem volumen (applicatio nova =
 * initium post finem), tabula data activa, pulsus bini; VERUM si
 * lector in fronte */
interior b32
renasci (
                  Mundus* x,
          CredoContextus* c,
      constans character* tabula)
{
    VicusApplicatio altera;

    memset(&altera, ZEPHYRUM, magnitudo(altera));
    si (!vicus_applicatio_aedificare(&altera, c->piscina,
            internamentum_creare(c->piscina), x->volumen, CDLXXX,
            CDLXXX + VICUS_ALTITUDO_TABULARUM))
    {
        redde FALSUM;
    }
    x->app = altera;
    (vacuum)vicus_activam_ponere(x->app.vicus, tabula);
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    redde lector_frontalis(x->app.vicus) != NIHIL;
}

/* lector in tabula data aperitur (argumentum NIHIL = bibliotheca),
 * pulsus, focus dexter */
interior vacuum
lectorem_aperire (
                 Mundus* x,
     constans character* tabula,
     constans character* argumentum)
{
    (vacuum)vicus_activam_ponere(x->app.vicus, tabula);
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_acervo_aperire(x->app.vicus, "lector", argumentum);
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_focum_ponere(x->app.vicus, VICUS_DEXTRUM);
    dispensator_recomponere(x->app.d);
}

/* clavis lectori frontali (focus dexter), pulsus */
interior vacuum
clavis_lectori (
         Mundus* x,
     ManusLudus* m,
      character  k)
{
    (vacuum)vicus_focum_ponere(x->app.vicus, VICUS_DEXTRUM);
    dispensator_recomponere(x->app.d);
    (vacuum)manus_ludus_clavem(m, k, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
}

#define PAGINA_ALPHA  "lector/pagina/urn%3Arhubarb%3Aalpha"
#define SENTENTIA_ALPHA  "lector/sententia/urn%3Arhubarb%3Aalpha"

/* L6: bibliotheca restituta, liber electus servatus */
interior vacuum
_probare_restitutio_bibliothecae (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    lectorem_aperire(x, "3", NIHIL);
    clavis_lectori(x, manus_ludus_creare(c->piscina, x->app.d), 'j');
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "electus_liber",
        "urn:rhubarb:alpha"));
    CREDO_NECESSE(renasci(x, c, "3"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus",
        "bibliotheca"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "electus_liber",
        "urn:rhubarb:alpha"));
}

/* L6: pagina restituta = liber ad positionem suam */
interior vacuum
_probare_restitutio_paginae (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    lectorem_aperire(x, "3", PAGINA_ALPHA "?caput=3&pagina=0");
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "3"));
    CREDO_NECESSE(renasci(x, c, "3"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus", "pagina"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "liber",
        "urn:rhubarb:alpha"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "3"));
}

/* L6: sententia restituta = visus sententiae in sententia positionis */
interior vacuum
_probare_restitutio_sententiae (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    lectorem_aperire(x, "3", SENTENTIA_ALPHA "?caput=3&sententia=19");
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "sententia", "19"));
    CREDO_NECESSE(renasci(x, c, "3"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus",
        "sententia"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "3"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "sententia", "19"));
}

/* L6: favores et index restituti; Esc ex favoribus = liber ad
 * positionem */
interior vacuum
_probare_restitutio_indicum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    lectorem_aperire(x, "3", SENTENTIA_ALPHA "?caput=3&sententia=19");
    clavis_lectori(x, manus_ludus_creare(c->piscina, x->app.d), 'f');
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus", "favores"));
    CREDO_NECESSE(renasci(x, c, "3"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus", "favores"));
    clavis_lectori(x, manus_ludus_creare(c->piscina, x->app.d),
        (character)XXVII);
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus", "pagina"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "3"));

    CREDO_GRADUS("index restitutus");
    clavis_lectori(x, manus_ludus_creare(c->piscina, x->app.d), 't');
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus", "index"));
    CREDO_NECESSE(renasci(x, c, "3"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus", "index"));
}

/* L6: latera duo, liber unus - B ultimus movet: ambo post initium
 * novum positionem B monstrant; tabula nova = bibliotheca */
interior vacuum
_probare_restitutio_duorum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    lectorem_aperire(x, "3", PAGINA_ALPHA "?caput=3&pagina=0");
    lectorem_aperire(x, "2", PAGINA_ALPHA "?caput=1&pagina=0");
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "1"));
    CREDO_NECESSE(renasci(x, c, "3"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "1"));
    (vacuum)vicus_activam_ponere(x->app.vicus, "2");
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "1"));

    CREDO_GRADUS("tabula nova: bibliotheca");
    lectorem_aperire(x, "1", NIHIL);
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus",
        "bibliotheca"));
}

/* L6: lector in tabula sua clausus (vicus_acervo_redire) et iterum
 * apertus statum suum habet (latus reapertum statum regularem tabulae
 * hereditat, semitae-spec VI-f) */
interior vacuum
_probare_iterum_apertum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    lectorem_aperire(x, "3", PAGINA_ALPHA "?caput=3&pagina=0");
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "3"));
    CREDO_VERUM(vicus_acervo_redire(x->app.vicus));
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(lector_frontalis(x->app.vicus) == NIHIL);
    lectorem_aperire(x, "3", NIHIL);
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "visus", "pagina"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "liber",
        "urn:rhubarb:alpha"));
    CREDO_VERUM(lectoris_campus_est(x->app.vicus, "caput", "3"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera", _probare_latera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "duo", _probare_duo, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "via", _probare_via, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "notae", _probare_notae, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "restitutio_bibliothecae", _probare_restitutio_bibliothecae,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
    { "restitutio_paginae", _probare_restitutio_paginae, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "restitutio_sententiae", _probare_restitutio_sententiae,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
    { "restitutio_indicum", _probare_restitutio_indicum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "restitutio_duorum", _probare_restitutio_duorum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "reapertio", _probare_iterum_apertum, mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "epubs", _probare_iussum_lectoris, mundum_parare, mundum_purgare,
        NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_lector", SECTIONES);
}
