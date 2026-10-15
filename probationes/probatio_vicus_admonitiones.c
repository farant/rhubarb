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
#include "thema.h"
#include "manus_ludus.h"
#include "dispensator.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_admonitiones", SECTIONES);
}
