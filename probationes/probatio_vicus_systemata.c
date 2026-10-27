/* probatio_vicus_systemata.c - systemata hospitis (colloquia-plan T4a):
 * vicus_systema_addere (titulus iteratus et systema NIHIL recusantur),
 * vicus_pulsare systema omni pulsu vocat et VERUM reddit solum si
 * systema mutavit; vicus_radicem_scribere campum radicis scriptore dato
 * scribit (dominus alienus recusatur, valor NIHIL tollit), etiam ab
 * intra systema. Genus fictum 'quietus'; nullum horologium. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "vicus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
     Volumen* volumen;
       Vicus* v;
         i32  montationes;   /* T6: montationes generis quieti */
} Mundus;

interior b32
quietum_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)sedes;
    (vacuum)p;
    (vacuum)in;
    (vacuum)vol;
    (vacuum)r;
    (vacuum)id;
    (vacuum)argumentum;
    (vacuum)latitudo;
    (vacuum)altitudo;
    si (ctx)
    {
        ((Mundus*)ctx)->montationes++;
    }
    redde VERUM;
}

interior vacuum
quietum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    (vacuum)montatio;
    (vacuum)f;
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    thema_initiare();
    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_systemata");
    si (x->volumen)
    {
        x->v = vicus_creare(c->piscina,
            internamentum_creare(c->piscina),
            x->volumen, CDLXXX, CDLXXX);
    }
    si (   x->v
        && (   !vicus_genus_addere(x->v, "quietus", I, quietum_montare,
                   quietum_describere, x)
            || !vicus_aperire(x->v,
                   "<tabulae activa=\"A\"><tabula id=\"A\">"
                   "<latus genus=\"quietus\"/><acervus>"
                   "<latus genus=\"quietus\"/></acervus></tabula>"
                   "</tabulae>")))
    {
        x->v = NIHIL;
    }
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

/* systema fictum: vocationes numerat; mutat = reddit; si valor, campum
 * admonitiones_non_lectae scribit (scriptor "admonitiones") */
nomen structura {
                    i32  vocationes;
                    b32  mutat;
     constans character* valor;
                    b32  scriptum;
} SystemaFictum;

interior b32
systema_fictum (
     Vicus* v,
    vacuum* ctx)
{
    SystemaFictum* s;

    s = (SystemaFictum*)ctx;
    s->vocationes++;
    si (s->valor)
    {
        s->scriptum = vicus_radicem_scribere(v, "admonitiones",
            "admonitiones_non_lectae", s->valor);
        s->valor = NIHIL;
    }
    redde s->mutat;
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

/* additio: titulus iteratus, systema NIHIL, titulus NIHIL recusantur */
interior vacuum
_probare_additionem (
    CredoContextus* c)
{
           Mundus* x;
    SystemaFictum  s;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL);
    memset(&s, ZEPHYRUM, magnitudo(s));
    CREDO_VERUM(vicus_systema_addere(x->v, "admonitiones",
        systema_fictum,
        &s));
    CREDO_FALSUM(vicus_systema_addere(x->v, "admonitiones",
        systema_fictum, &s));
    CREDO_FALSUM(vicus_systema_addere(x->v, "aliud", NIHIL, &s));
    CREDO_FALSUM(vicus_systema_addere(x->v, NIHIL, systema_fictum, &s));
}

/* pulsus: systema semper vocatur; VERUM solum cum mutat */
interior vacuum
_probare_pulsum (
    CredoContextus* c)
{
           Mundus* x;
    SystemaFictum  s;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL);
    memset(&s, ZEPHYRUM, magnitudo(s));
    CREDO_NECESSE(vicus_systema_addere(x->v, "admonitiones",
        systema_fictum, &s));
    (vacuum)vicus_pulsare(x->v);
    s.vocationes = ZEPHYRUM;
    CREDO_FALSUM(vicus_pulsare(x->v));
    CREDO_FALSUM(vicus_pulsare(x->v));
    CREDO_AEQUALIS_I32(s.vocationes, II);
    s.mutat = VERUM;
    CREDO_VERUM(vicus_pulsare(x->v));
    CREDO_AEQUALIS_I32(s.vocationes, III);
}

/* radix: scriptor dominus scribit, alienus recusatur (valor manet),
 * NIHIL tollit; ab intra systema in pulsu */
interior vacuum
_probare_radicem (
    CredoContextus* c)
{
           Mundus* x;
    SystemaFictum  s;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL);
    CREDO_VERUM(campus_est(x->v, "admonitiones_non_lectae", NIHIL));
    CREDO_VERUM(vicus_radicem_scribere(x->v, "admonitiones",
        "admonitiones_non_lectae", "3"));
    CREDO_VERUM(campus_est(x->v, "admonitiones_non_lectae", "3"));
    CREDO_FALSUM(vicus_radicem_scribere(x->v, "horologium",
        "admonitiones_non_lectae", "7"));
    CREDO_VERUM(campus_est(x->v, "admonitiones_non_lectae", "3"));
    CREDO_VERUM(vicus_radicem_scribere(x->v, "admonitiones",
        "admonitio_nova", "colloquia:s-1:1000"));
    CREDO_VERUM(campus_est(x->v, "admonitio_nova",
        "colloquia:s-1:1000"));
    CREDO_VERUM(vicus_radicem_scribere(x->v, "admonitiones",
        "admonitiones_non_lectae", NIHIL));
    CREDO_VERUM(campus_est(x->v, "admonitiones_non_lectae", NIHIL));
    /* ab intra systema */
    memset(&s, ZEPHYRUM, magnitudo(s));
    s.valor = "5";
    s.mutat = VERUM;
    CREDO_NECESSE(vicus_systema_addere(x->v, "admonitiones",
        systema_fictum, &s));
    CREDO_VERUM(vicus_pulsare(x->v));
    CREDO_VERUM(s.scriptum);
    CREDO_VERUM(campus_est(x->v, "admonitiones_non_lectae", "5"));
}

/* T6: argumentum lateris sine remontatione; dispositio servata;
 * vicus novus in eodem volumine eo montat; "" tollit; id ignotum */
interior vacuum
_probare_argumentum (
    CredoContextus* c)
{
         Mundus* x;
          Vicus* altera;
         chorda  servata;
            b32  inventum;
     VicusLatus* l;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL);
    CREDO_AEQUALIS_I32(x->montationes, II);
    CREDO_VERUM(vicus_latus_argumentum_ponere(x->v, "A_dextrum_quietus",
        "tmux - alpha"));
    CREDO_AEQUALIS_I32(x->montationes, II);
    l = vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
    CREDO_NECESSE(l != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, "tmux - alpha");
    servata = volumen_praesens_legere(x->volumen, chorda_ex_literis(
        "vicus/latera", c->piscina), c->piscina, &inventum);
    CREDO_VERUM(inventum);
    CREDO_VERUM(chorda_continet(servata, chorda_ex_literis(
        "argumentum=\"tmux - alpha\"", c->piscina)));
    CREDO_FALSUM(vicus_latus_argumentum_ponere(x->v, "nullum_latus",
        "x"));
    /* restitutio: vicus alter in eodem volumine */
    altera = vicus_creare(c->piscina, internamentum_creare(c->piscina),
        x->volumen, CDLXXX, CDLXXX);
    CREDO_NECESSE(altera != NIHIL);
    CREDO_NECESSE(vicus_genus_addere(altera, "quietus", I,
        quietum_montare, quietum_describere, NIHIL));
    CREDO_NECESSE(vicus_aperire(altera, "<tabulae activa=\"A\"/>"));
    l = vicus_latus(vicus_activa(altera), VICUS_DEXTRUM);
    CREDO_NECESSE(l != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, "tmux - alpha");
    /* "" tollit */
    CREDO_VERUM(vicus_latus_argumentum_ponere(x->v, "A_dextrum_quietus",
        ""));
    servata = volumen_praesens_legere(x->volumen, chorda_ex_literis(
        "vicus/latera", c->piscina), c->piscina, &inventum);
    CREDO_FALSUM(chorda_continet(servata,
        chorda_ex_literis("argumentum",
        c->piscina)));
}

/* T4d: redire - acervus unius recusatur; latus frontale tollitur,
 * prius in fronte, focus dexter, dispositio servata */
interior vacuum
_probare_redire (
    CredoContextus* c)
{
          Mundus* x;
     VicusTabula* t;
          chorda  servata;
             b32  inventum;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL);
    t = vicus_activa(x->v);
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), I);
    CREDO_FALSUM(vicus_acervo_redire(x->v));
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), I);
    CREDO_NECESSE(vicus_acervo_aperire(x->v, "quietus", "secundum"));
    (vacuum)vicus_pulsare(x->v);
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus(t, VICUS_DEXTRUM)
        ->argumentum, "secundum");
    CREDO_NECESSE(vicus_focum_ponere(x->v, VICUS_SINISTRUM));
    CREDO_VERUM(vicus_acervo_redire(x->v));
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), I);
    CREDO_AEQUALIS_I32(vicus_latus(t,
        VICUS_DEXTRUM)->argumentum.mensura,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(t->focus, VICUS_DEXTRUM);
    servata = volumen_praesens_legere(x->volumen, chorda_ex_literis(
        "vicus/latera", c->piscina), c->piscina, &inventum);
    CREDO_VERUM(inventum);
    CREDO_FALSUM(chorda_continet(servata, chorda_ex_literis("secundum",
        c->piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "additio", _probare_additionem, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "pulsus", _probare_pulsum, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "radix", _probare_radicem, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "argumentum", _probare_argumentum, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "redire", _probare_redire, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_systemata", SECTIONES);
}
