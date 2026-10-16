/* probatio_vicus_admonitiones.c - admonitiones in vico (colloquia-plan
 * T4a): '$notifications' centrum aperit; sine libro (probationes)
 * systema nihil scribit; cum libro (in area) campi radicis
 * admonitiones_non_lectae et admonitio_nova in pulsu; campus
 * admonitiones_lectae_ad in initio applicatur; X in centro campum
 * radicis scribit (dominus admonitiones.agere). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "filum.h"
#include "insula.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "admonitiones_visus.h"
#include "colloquia_visus.h"
#include "thema.h"
#include "manus_ludus.h"
#include "dispensator.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VIA_FIXI "probationes/fixa/colloquia/eventa.jsonl"

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
        "probatio_vicus_admonitiones");
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

/* liber in area: copia fixi (cum '\n' finali) */
interior constans character*
librum_parare (
    CredoContextus* c)
{
     constans character* area;
              character* via;

    area = credo_area(c);
    si (!area)
    {
        redde NIHIL;
    }
    via = (character*)piscina_allocare(c->piscina, (memoriae_index)
        strlen(area) + XXXII);
    sprintf(via, "%s/eventa.jsonl", area);
    si (   !filum_scribere(via, filum_legere_totum(VIA_FIXI,
        c->piscina))
        || !filum_appendere_firmiter(via, chorda_ex_literis("\n",
               c->piscina)))
    {
        redde NIHIL;
    }
    redde via;
}

interior b32
campus_est (
                  Vicus* v,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_attributum(v->repo, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

/* latus frontale tabulae activae, si centrum */
interior AdmonitionumMontatio*
centrum_frontale (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "admonitiones"))
    {
        redde NIHIL;
    }
    redde (AdmonitionumMontatio*)l->montatio;
}

/* '$notifications' in pagina scribae: ictus in verbo centrum aperit */
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
    CREDO_VERUM(manus_ludus_scribere(m, "i$notifications"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV);
    (vacuum)vicus_pulsare(v);
    CREDO_NON_NIHIL(centrum_frontale(v));
}

/* sine libro: systema nihil scribit; centrum vacuum */
interior vacuum
_probare_sine_libro (
    CredoContextus* c)
{
                  Mundus* x;
                   Vicus* v;
    AdmonitionumMontatio* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(v, "admonitiones_non_lectae", NIHIL));
    CREDO_VERUM(vicus_acervo_aperire(v, "admonitiones", NIHIL));
    (vacuum)vicus_pulsare(v);
    a = centrum_frontale(v);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_AEQUALIS_I32(admonitiones_numerus(a->index), ZEPHYRUM);
}

/* cum libro: cauda (IV), nihil novi; nova -> admonitio_nova */
interior vacuum
_probare_systema (
    CredoContextus* c)
{
                  Mundus* x;
                   Vicus* v;
      constans character* via;
    AdmonitionumMontatio* a;

    x    = (Mundus*)c->datum;
    via  = librum_parare(c);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_applicatio_admonitiones_aperire(&x->app, via));
    CREDO_FALSUM(vicus_applicatio_admonitiones_aperire(&x->app, via));
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(campus_est(v, "admonitiones_non_lectae", "4"));
    CREDO_VERUM(campus_est(v, "admonitio_nova", NIHIL));
    CREDO_FALSUM(vicus_pulsare(v));
    CREDO_NECESSE(filum_appendere_firmiter(via, chorda_ex_literis(
        "{\"tempus\":400,\"sessio\":\"s-c\","
        "\"eventus\":\"Notification\","
        "\"panellus\":\"%1\",\"servitor\":\"\",\"via\":\"/w/gamma\","
        "\"transcriptum\":\"\",\"genus\":\"permission_prompt\","
        "\"nuntius\":\"Claude needs your permission to use Edit\"}\n",
        c->piscina)));
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(campus_est(v, "admonitio_nova", "colloquia:s-c:400"));
    CREDO_VERUM(campus_est(v, "admonitiones_non_lectae", "5"));
    /* centrum indicem communem videt */
    CREDO_VERUM(vicus_acervo_aperire(v, "admonitiones", NIHIL));
    (vacuum)vicus_pulsare(v);
    a = centrum_frontale(v);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_AEQUALIS_I32(admonitiones_numerus(a->index), V);
}

/* campus radicis lectae_ad (restitutus) in initio applicatur */
interior vacuum
_probare_lectas_ad (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
     constans character* via;

    x    = (Mundus*)c->datum;
    via  = librum_parare(c);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_radicem_scribere(v, "admonitiones.agere",
        "admonitiones_lectae_ad", "205"));
    CREDO_VERUM(vicus_applicatio_admonitiones_aperire(&x->app, via));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(v, "admonitiones_non_lectae", "1"));
}

/* X in centro: omnes lectae, campus radicis per dominum suum */
interior vacuum
_probare_omnes_legere (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
     constans character* via;
             ManusLudus* m;

    x    = (Mundus*)c->datum;
    via  = librum_parare(c);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_applicatio_admonitiones_aperire(&x->app, via));
    CREDO_VERUM(vicus_acervo_aperire(v, "admonitiones", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(v, "admonitiones_non_lectae", "4"));
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#admonitiones"));
    CREDO_VERUM(manus_ludus_scribere(m, "X"));
    CREDO_VERUM(campus_est(v, "admonitiones_lectae_ad", "220"));
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(campus_est(v, "admonitiones_non_lectae", "0"));
}

/* x textus in linea tabularum, aut -1 */
interior s32
textus_lineae (
                Mundus* x,
    constans character* quaesitus,
               Piscina* p)
{
      Mandata* md;
     Mandatum* m;
          i32  i;

    dispensator_recomponere(x->app.d);
    md = mandata_creare(p, x->app.intern);
    pingere(dispensator_arbor(x->app.d), vicus_figurae(x->app.vicus),
        ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        m = mandata_obtinere(md, i);
        si (   m->genus == MANDATUM_TEXTUS
            && m->fines.y < VICUS_ALTITUDO_TABULARUM
            && chorda_aequalis_literis(m->textus, quaesitus))
        {
            redde m->fines.x;
        }
    }
    redde -I;
}

interior vacuum
ictus_ad (
    Mundus* x,
       s32  px,
       s32  py)
{
    Eventus e;

    dispensator_recomponere(x->app.d);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.tempus            = M;
    e.datum.mus.x       = px;
    e.datum.mus.y       = py;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(x->app.d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(x->app.d, &e);
}

/* T4b insigne: numerus non lectarum ad dextrum lineae (nullum
 * horologium: margo); ictus centrum aperit; 0 nihil */
interior vacuum
_probare_insigne (
    CredoContextus* c)
{
    Mundus* x;
     Vicus* v;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    /* "12": nulla tabula sic inscribitur (tabulae 1-10) */
    CREDO_VERUM(textus_lineae(x, "12", c->piscina) < ZEPHYRUM);
    CREDO_VERUM(vicus_radicem_scribere(v, "admonitiones",
        "admonitiones_non_lectae", "12"));
    /* x1 = CDLXXX - VI; x0 = x1 - IV cellulae; numerus post II */
    CREDO_AEQUALIS_S32(textus_lineae(x, "12", c->piscina),
        CDLXXX - VI - XXIV + XII);
    ictus_ad(x, CDLXXX - VI - XXIV + II, IV);
    (vacuum)vicus_pulsare(v);
    CREDO_NON_NIHIL(centrum_frontale(v));
    CREDO_VERUM(vicus_radicem_scribere(v, "admonitiones",
        "admonitiones_non_lectae", "0"));
    CREDO_VERUM(textus_lineae(x, "0", c->piscina) < ZEPHYRUM);
}

/* T4b volantes: nova (nunc) volat ad dextrum; 'x' dimittit, non
 * legit (corpus non tangitur: porta tmux verum vocaret) */
interior vacuum
_probare_volantes (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
     constans character* via;
              Componens* t;
              character  linea[CCCLXXXIV];

    x    = (Mundus*)c->datum;
    via  = librum_parare(c);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_applicatio_admonitiones_aperire(&x->app, via));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    /* fixa vetera: nulla volat */
    CREDO_VERUM(componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("v1", c->piscina)) == NIHIL);
    sprintf(linea, "{\"tempus\":%ld000,\"sessio\":\"s-c\","
        "\"eventus\":\"Notification\",\"panellus\":\"%%1\","
        "\"servitor\":\"\",\"via\":\"/w/gamma\",\"transcriptum\":"
        "\"\",\"genus\":\"permission_prompt\",\"nuntius\":\"Claude "
        "needs your permission to use Edit\"}\n", (long)time(NIHIL));
    CREDO_NECESSE(filum_appendere_firmiter(via, chorda_ex_literis(linea,
        c->piscina)));
    CREDO_VERUM(vicus_pulsare(v));
    dispensator_recomponere(x->app.d);
    t = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("x1", c->piscina));
    CREDO_NECESSE(t != NIHIL);
    CREDO_AEQUALIS_S32(t->fines.x + t->fines.latitudo, CDLXXX - VI);
    ictus_ad(x, t->fines.x + t->fines.latitudo / II, t->fines.y
        + t->fines.altitudo / II);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("v1", c->piscina)) == NIHIL);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(v, "admonitiones_non_lectae", "5"));
}

/* T5: latus sessionum: sine libro vacuum; cum libro fons communis
 * (IV sessiones fixi) */
interior vacuum
_probare_sessiones (
    CredoContextus* c)
{
                  Mundus* x;
                   Vicus* v;
      constans character* via;
              VicusLatus* l;
    ColloquiorumMontatio* m;

    x    = (Mundus*)c->datum;
    via  = librum_parare(c);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    /* ante fontes (ut latus restitutum, in vicus_aperire montatum):
     * vacuum; fontibus apertis IDEM latus sessiones videt (Franus
     * 10-16: latus restitutum vacuum manebat) */
    CREDO_VERUM(vicus_acervo_aperire(v, "colloquia", NIHIL));
    (vacuum)vicus_pulsare(v);
    l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
    CREDO_NECESSE(l && l->montata && chorda_aequalis_literis(l->genus,
        "colloquia"));
    m = (ColloquiorumMontatio*)l->montatio;
    CREDO_VERUM(   !m->fons
                || xar_numerus(colloquia_visus_fontis(m->fons,
                       c->piscina)) == ZEPHYRUM);
    CREDO_VERUM(vicus_applicatio_admonitiones_aperire(&x->app, via));
    (vacuum)vicus_pulsare(v);
    CREDO_NECESSE(m->fons != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(colloquia_visus_fontis(m->fons,
        c->piscina)), IV);
    /* latus novum in tabula alia idem videt */
    CREDO_NECESSE(vicus_activam_ponere(v, "4"));
    CREDO_VERUM(vicus_acervo_aperire(v, "colloquia", NIHIL));
    (vacuum)vicus_pulsare(v);
    l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
    CREDO_NECESSE(l && l->montata && chorda_aequalis_literis(l->genus,
        "colloquia"));
    m = (ColloquiorumMontatio*)l->montatio;
    CREDO_NECESSE(m->fons != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(colloquia_visus_fontis(m->fons,
        c->piscina)), IV);
}

/* T4d: Esc in centro (et sessionibus) latus tollit, prius redit;
 * latus quod non optat (nomenclator) manet */
interior vacuum
_probare_redire (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
             ManusLudus* m;
            VicusTabula* t;
     constans character* via;
                    i32  n;

    x    = (Mundus*)c->datum;
    via  = librum_parare(c);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    /* fontes aperti: latera claves vere tractant (sine fonte sessiones
     * nihil tractant - Esc sola transiret) */
    CREDO_NECESSE(vicus_applicatio_admonitiones_aperire(&x->app, via));
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    t = vicus_activa(v);
    n = xar_numerus(t->acervus);
    CREDO_VERUM(vicus_acervo_aperire(v, "admonitiones", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_NON_NIHIL(centrum_frontale(v));
    m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#admonitiones"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), n);
    CREDO_VERUM(centrum_frontale(v) == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus(t, VICUS_DEXTRUM)->genus,
        "scriba");
    CREDO_AEQUALIS_I32(t->focus, VICUS_DEXTRUM);
    /* sessiones idem */
    CREDO_VERUM(vicus_acervo_aperire(v, "colloquia", NIHIL));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#colloquia"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), n);
    /* nomenclator non optat: Esc latus non tollit */
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(manus_ludus_premere(m, "#nomenclator"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), n + I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "verbum", _probare_verbum, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "sine_libro", _probare_sine_libro, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "systema", _probare_systema, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "lectae_ad", _probare_lectas_ad, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "omnes_legere", _probare_omnes_legere, mundum_parare,
      mundum_purgare, NIHIL, NIHIL },
    { "insigne", _probare_insigne, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "volantes", _probare_volantes, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "sessiones", _probare_sessiones, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "redire", _probare_redire, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_admonitiones", SECTIONES);
}
