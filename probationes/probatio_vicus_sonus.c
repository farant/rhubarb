/* probatio_vicus_sonus.c - sonus in vico (sonus S4 gradus IV)
 *
 * Compositio VERA (vicus_applicatio, tabula 3 = scriba | scriba), liber
 * praeconii temporarius (ut probatio_vicus_saltus), porta soni FICTA
 * quae vocationes numerat - nihil sonat. tmux: TMUX_TMPDIR in area
 * probationis, ergo systema 'terminalia' nullum servitorem invenit.
 *
 * quies: sine eventu nihil pulsatur; agere omni pulsu. genera: Stop ->
 * "finis", Notification permissionis -> "permissio", StopFailure ->
 * "vitium"; duo eventa uno pulsu -> UNUS sonus, genus recentissimae.
 * mutus / admonitionum_mutus: mutum factum tacet SEMEL, eventa
 * tacent, agere pergit; sine muto iterum sonant. iussum: '$sound' et
 * '$sound(notifications)' campos vertunt, nuntium ponunt, in pagina
 * manent; '$sound(x)' error. persistens: mutus post aedificationem
 * alteram in eodem volumine manet. porta nulla: eventa et iussa sine
 * casu. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "filum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tabula_characterum.h"
#include "scriba_applicatio.h"
#include "componens.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "vicus_sonus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINEA III

nomen structura {
          i32 pulsus;          /* pulsare */
          i32 actus;           /* agere */
          i32 taciturnitates;  /* tacere */
    character genus[XXXII];    /* pulsatum ultimum */
} PortaFicta;

nomen structura {
             Volumen* vol;
     VicusApplicatio  app;
               Vicus* v;
          ManusLudus* m;
      ScribaMontatio* sin;
           character  liber[CCLVI];
                 i32  eventa;
          PortaFicta  ficta;
      VicusPortaSoni  porta;
} Mundus;

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior b32
_pulsare (
     vacuum* ctx,
     chorda  genus)
{
    PortaFicta* f = (PortaFicta*)ctx;

    f->pulsus++;
    si (genus.mensura >= magnitudo(f->genus))
    {
        redde FALSUM;
    }
    memcpy(f->genus, genus.datum, (size_t)genus.mensura);
    f->genus[genus.mensura] = '\0';
    redde VERUM;
}

interior vacuum
_agere (
    vacuum* ctx)
{
    ((PortaFicta*)ctx)->actus++;
}

interior vacuum
_tacere (
    vacuum* ctx)
{
    ((PortaFicta*)ctx)->taciturnitates++;
}

interior vacuum
_ficta_vacare (
    Mundus* s)
{
    memset(&s->ficta, ZEPHYRUM, magnitudo(PortaFicta));
}

/* eventum in librum praeconii (sessio nova per eventum; tempus
 * crescens: recentissima = ultima scripta) */
interior b32
_admonere (
                 Mundus* s,
     constans character* eventus,
     constans character* genus)
{
    character linea[DXII];

    s->eventa++;
    sprintf(linea,
        "{\"tempus\":%ld%03d,\"sessio\":\"s-%d\",\"eventus\":\"%s\","
        "\"panellus\":\"%%%d\",\"servitor\":\"\",\"via\":\"/w/alpha\","
        "\"transcriptum\":\"\",\"genus\":\"%s\",\"nuntius\":\"n\"}\n",
        (long)time(NIHIL), (integer)s->eventa, (integer)s->eventa,
        eventus, (integer)s->eventa, genus);
    redde filum_appendere_firmiter(s->liber, chorda_ex_literis(linea,
        piscina));
}

interior vacuum
_pulsus (
    Mundus* s,
       i32  n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        (vacuum)vicus_pulsare(s->v);
    }
}

interior b32
_veritas (
                 Vicus* v,
    constans character* titulus)
{
    chorda* a;

    a = insula_attributum(v->repo, titulus);
    redde a && chorda_aequalis_literis(*a, "verum");
}

/* punctum schirmi cellulae (linea, columna) paginae sinistrae */
interior vacuum
_ictus (
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

interior b32
_linea_est (
        ScribaMontatio* sm,
                   s32  linea,
    constans character* textus)
{
    constans TabulaCharacterum* t;

    t = &sm->actiones_ctx.laboris;
    redde memcmp(t->cellulae + (i32)linea * t->latitudo, textus,
        strlen(textus)) == ZEPHYRUM;
}

/* nuntius lineae status sinistri: praefixum (non vacuum) textus dati */
interior b32
_nuntius_est (
                 Vicus* v,
        ScribaMontatio* sm,
    constans character* textus)
{
    Componens* r;
    Componens* n;

    r = scriba_componere(v->repo, NIHIL, piscina, intern,
        &sm->compositio);
    n = r ? componens_invenire_per_id(r, chorda_ex_literis("nuntius",
        piscina)) : NIHIL;
    redde n && n->titulus.mensura > ZEPHYRUM
        && (memoriae_index)n->titulus.mensura <= strlen(textus)
        && memcmp(n->titulus.datum, textus,
               (size_t)n->titulus.mensura) == ZEPHYRUM;
}

interior b32
_aedificare (
             Mundus* s,
     CredoContextus* c,
                b32  cum_porta)
{
    si (!vicus_applicatio_aedificare(&s->app, c->piscina, intern,
            s->vol, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM))
    {
        redde FALSUM;
    }
    s->v = s->app.vicus;
    s->m = manus_ludus_creare(c->piscina, s->app.d);
    si (cum_porta)
    {
        vicus_applicatio_soni_portam_ponere(&s->app, &s->porta);
    }
    si (   !s->m || !vicus_activam_ponere(s->v, "3")
        || !vicus_applicatio_admonitiones_aperire(&s->app, s->liber))
    {
        redde FALSUM;
    }
    dispensator_recomponere(s->app.d);
    s->sin = (ScribaMontatio*)vicus_latus(vicus_activa(s->v),
        VICUS_SINISTRUM)->montatio;
    redde s->sin != NIHIL;
}

interior vacuum
_mundum_parare_porta (
    CredoContextus* c,
               b32  cum_porta)
{
                Mundus* s;
    constans character* area;
             character  via_tmux[CCLVI];

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    s = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    CREDO_NECESSE_NON_NIHIL(s);
    memset(s, ZEPHYRUM, magnitudo(Mundus));
    c->datum = s;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    (vacuum)unsetenv("TMUX");
    (vacuum)unsetenv("TMUX_PANE");
    area = credo_area(c);
    CREDO_NECESSE_NON_NIHIL(area);
    sprintf(via_tmux, "%s", area);
    (vacuum)setenv("TMUX_TMPDIR", via_tmux, I);
    sprintf(s->liber, "%s/eventa.jsonl", area);
    CREDO_NECESSE(filum_appendere_firmiter(s->liber,
        chorda_ex_literis("", piscina)));
    s->porta.pulsare = _pulsare;
    s->porta.agere = _agere;
    s->porta.tacere = _tacere;
    s->porta.ctx = &s->ficta;
    s->vol = volumen_temporarium(piscina, "probatio_vicus_sonus");
    CREDO_NECESSE_NON_NIHIL(s->vol);
    CREDO_NECESSE(_aedificare(s, c, cum_porta));
    _pulsus(s, I);
    _ficta_vacare(s);
}

interior vacuum
_mundum_parare (
    CredoContextus* c)
{
    _mundum_parare_porta(c, VERUM);
}

interior vacuum
_mundum_sine_porta (
    CredoContextus* c)
{
    _mundum_parare_porta(c, FALSUM);
}

interior vacuum
_mundum_purgare (
    CredoContextus* c)
{
    Mundus* s = (Mundus*)c->datum;

    si (s != NIHIL && s->vol != NIHIL)
    {
        volumen_claudere(s->vol);
    }
}

interior vacuum
_probare_quietem (
    CredoContextus* c)
{
    Mundus* s = (Mundus*)c->datum;

    _pulsus(s, III);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(s->ficta.actus, III);
    CREDO_AEQUALIS_I32(s->ficta.taciturnitates, ZEPHYRUM);
    CREDO_FALSUM(_veritas(s->v, "sonus_mutus"));
    CREDO_FALSUM(_veritas(s->v, "sonus_admonitionum_mutus"));
}

interior vacuum
_probare_genera (
    CredoContextus* c)
{
    Mundus* s = (Mundus*)c->datum;

    CREDO_GRADUS("Stop -> finis");
    CREDO_NECESSE(_admonere(s, "Stop", ""));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, I);
    CREDO_VERUM(strcmp(s->ficta.genus, "finis") == ZEPHYRUM);

    CREDO_GRADUS("Notification permissionis -> permissio");
    CREDO_NECESSE(_admonere(s, "Notification", "permission_prompt"));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, II);
    CREDO_VERUM(strcmp(s->ficta.genus, "permissio") == ZEPHYRUM);

    CREDO_GRADUS("StopFailure -> vitium");
    CREDO_NECESSE(_admonere(s, "StopFailure", ""));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, III);
    CREDO_VERUM(strcmp(s->ficta.genus, "vitium") == ZEPHYRUM);

    CREDO_GRADUS("pulsus sine eventu: nihil novi");
    _pulsus(s, II);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, III);

    CREDO_GRADUS("duo eventa uno pulsu: unus sonus, recentissima");
    CREDO_NECESSE(_admonere(s, "StopFailure", ""));
    CREDO_NECESSE(_admonere(s, "Notification", "permission_prompt"));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, IV);
    CREDO_VERUM(strcmp(s->ficta.genus, "permissio") == ZEPHYRUM);

    CREDO_GRADUS("eventum sine admonitione (UserPromptSubmit): nihil");
    CREDO_NECESSE(_admonere(s, "UserPromptSubmit", ""));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, IV);
    CREDO_AEQUALIS_I32(s->ficta.actus, VII);
}

/* mutus per campum datum: tacet semel, eventa tacent, agere pergit */
interior vacuum
_mutum_probare (
                Mundus* s,
    constans character* campus)
{
    CREDO_GRADUS("mutus factus: tacere semel");
    CREDO_VERUM(vicus_radicem_scribere(s->v, "sonus.agere", campus,
        "verum"));
    CREDO_VERUM(_veritas(s->v, campus));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.taciturnitates, I);
    _pulsus(s, II);
    CREDO_AEQUALIS_I32(s->ficta.taciturnitates, I);
    CREDO_AEQUALIS_I32(s->ficta.actus, III);

    CREDO_GRADUS("mutus: eventum tacet");
    CREDO_NECESSE(_admonere(s, "Stop", ""));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(s->ficta.actus, IV);

    CREDO_GRADUS("sine muto: iterum sonat, nulla taciturnitas nova");
    CREDO_VERUM(vicus_radicem_scribere(s->v, "sonus.agere", campus,
        "falsum"));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.taciturnitates, I);
    CREDO_NECESSE(_admonere(s, "Stop", ""));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, I);
    CREDO_VERUM(strcmp(s->ficta.genus, "finis") == ZEPHYRUM);
}

interior vacuum
_probare_mutum (
    CredoContextus* c)
{
    _mutum_probare((Mundus*)c->datum, "sonus_mutus");
}

interior vacuum
_probare_admonitionum_mutum (
    CredoContextus* c)
{
    _mutum_probare((Mundus*)c->datum, "sonus_admonitionum_mutus");
}

/* "$sound $sound(notifications) $sound(x)": columnae 0, 7, 29 */
interior vacuum
_probare_iussum (
    CredoContextus* c)
{
    Mundus* s = (Mundus*)c->datum;

    _ictus(s->m, LINEA, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(s->m,
        "i$sound $sound(notifications) $sound(x)"));
    CREDO_VERUM(manus_ludus_clavem(s->m, (character)XXVII, ZEPHYRUM));
    CREDO_NECESSE(_linea_est(s->sin, LINEA,
        "$sound $sound(notifications) $sound(x)"));

    CREDO_GRADUS("$sound: sonus off, in pagina manet");
    _ictus(s->m, LINEA, II);
    CREDO_VERUM(_veritas(s->v, "sonus_mutus"));
    CREDO_VERUM(_nuntius_est(s->v, s->sin, "sound off"));
    CREDO_VERUM(_linea_est(s->sin, LINEA,
        "$sound $sound(notifications) $sound(x)"));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.taciturnitates, I);

    CREDO_GRADUS("$sound iterum: sonus on");
    _ictus(s->m, LINEA, II);
    CREDO_FALSUM(_veritas(s->v, "sonus_mutus"));
    CREDO_VERUM(_nuntius_est(s->v, s->sin, "sound on"));

    CREDO_GRADUS("$sound(notifications): admonitiones solae");
    _ictus(s->m, LINEA, X);
    CREDO_VERUM(_veritas(s->v, "sonus_admonitionum_mutus"));
    CREDO_FALSUM(_veritas(s->v, "sonus_mutus"));
    CREDO_VERUM(_nuntius_est(s->v, s->sin, "notification sounds off"));
    _ictus(s->m, LINEA, X);
    CREDO_FALSUM(_veritas(s->v, "sonus_admonitionum_mutus"));
    CREDO_VERUM(_nuntius_est(s->v, s->sin, "notification sounds on"));

    CREDO_GRADUS("$sound(x): error, campi intacti");
    _ictus(s->m, LINEA, XXXI);
    CREDO_VERUM(_nuntius_est(s->v, s->sin,
        "sound: unknown argument (notifications): x"));
    CREDO_FALSUM(_veritas(s->v, "sonus_mutus"));
    CREDO_FALSUM(_veritas(s->v, "sonus_admonitionum_mutus"));
    CREDO_VERUM(_linea_est(s->sin, LINEA,
        "$sound $sound(notifications) $sound(x)"));
}

interior vacuum
_probare_persistentem (
    CredoContextus* c)
{
    Mundus* s = (Mundus*)c->datum;

    CREDO_VERUM(vicus_radicem_scribere(s->v, "sonus.agere",
        "sonus_mutus", "verum"));
    _pulsus(s, I);
    /* sessio altera in eodem volumine, porta eadem */
    CREDO_NECESSE(_aedificare(s, c, VERUM));
    CREDO_VERUM(_veritas(s->v, "sonus_mutus"));
    _pulsus(s, I);
    _ficta_vacare(s);
    CREDO_NECESSE(_admonere(s, "Stop", ""));
    _pulsus(s, I);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(s->ficta.actus, I);
}

interior vacuum
_probare_portam_nullam (
    CredoContextus* c)
{
    Mundus* s = (Mundus*)c->datum;

    CREDO_NECESSE(_admonere(s, "Stop", ""));
    _pulsus(s, II);
    /* admonitio accepta (systema 'admonitiones' currit sine porta) */
    {
        chorda* a;

        a = insula_attributum(s->v->repo, "admonitiones_non_lectae");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));
    }
    _ictus(s->m, LINEA, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(s->m, "i$sound"));
    CREDO_VERUM(manus_ludus_clavem(s->m, (character)XXVII, ZEPHYRUM));
    _ictus(s->m, LINEA, II);
    CREDO_VERUM(_veritas(s->v, "sonus_mutus"));
    CREDO_VERUM(_nuntius_est(s->v, s->sin, "sound off"));
    _pulsus(s, II);
    CREDO_AEQUALIS_I32(s->ficta.pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(s->ficta.actus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(s->ficta.taciturnitates, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "quies", _probare_quietem, _mundum_parare, _mundum_purgare, NIHIL,
      NIHIL },
    { "genera", _probare_genera, _mundum_parare, _mundum_purgare, NIHIL,
      NIHIL },
    { "mutus", _probare_mutum, _mundum_parare, _mundum_purgare, NIHIL,
      NIHIL },
    { "admonitionum_mutus", _probare_admonitionum_mutum, _mundum_parare,
      _mundum_purgare, NIHIL, NIHIL },
    { "iussum", _probare_iussum, _mundum_parare, _mundum_purgare, NIHIL,
      NIHIL },
    { "persistens", _probare_persistentem, _mundum_parare,
      _mundum_purgare, NIHIL, NIHIL },
    { "porta_nulla", _probare_portam_nullam, _mundum_sine_porta,
      _mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_sonus", SECTIONES);
}
