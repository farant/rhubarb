/* probatio_vicus_semitae.c - semitae R3 in vico (project-specs/
 * semitae-spec.md): '$via(uri)' latus generis in frontem cum semita
 * fert (latus sine semitis et semita mala nuntium dant); historia
 * lateris (impelle novum, substitue currentem), Ctrl+O / Ctrl+I
 * restituunt EXACTE (semita plena), navigatio nova post reditum
 * futura tollit; latus sine semitis Ctrl+O non capit. Compositio vera
 * (vicus_applicatio), sancti ex fixura. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "sancti_visus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
         ManusLudus* m;
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
    (vacuum)setenv("RHUBARB_SANCTI", "probationes/fixa/sancti", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_semitae");
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
mundum_struere (
            Mundus* x,
    CredoContextus* c)
{
    si (   !x->volumen
        || !vicus_applicatio_aedificare(&x->app, c->piscina,
               internamentum_creare(c->piscina), x->volumen, CDLXXX,
               CDLXXX + VICUS_ALTITUDO_TABULARUM))
    {
        redde FALSUM;
    }
    x->m = manus_ludus_creare(c->piscina, x->app.d);
    redde x->m && vicus_activam_ponere(x->app.vicus, "3");
}

/* ictus in cellulam paginae sinistrae (linea, columna) */
interior vacuum
ictus (
     Mundus* x,
        s32  linea,
        s32  columna)
{
    (vacuum)manus_ludus_premere_ad(x->m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
}

interior vacuum
clavis (
       Mundus* x,
    character  typus,
          i32  modificantes)
{
    (vacuum)manus_ludus_clavem(x->m, typus, modificantes);
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
}

interior VicusLatus*
frons (
    Mundus* x)
{
    redde vicus_latus(vicus_activa(x->app.vicus), VICUS_DEXTRUM);
}

/* attributum rami lateris sanctorum frontalis = valor (NIHIL absens) */
interior b32
sanctorum_est (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    VicusLatus* l;
        chorda* a;

    l = frons(x);
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "sancti"))
    {
        redde FALSUM;
    }
    a = insula_ramus_attributum(&((SanctorumMontatio*)l->montatio)
        ->ramus, titulus);
    si (valor == NIHIL)
    {
        redde !a || a->mensura == ZEPHYRUM;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

/* nuntius paginae sinistrae continet? */
interior b32
nuntius_continet (
                 Mundus* x,
     constans character* pars,
                Piscina* p)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;

    l = vicus_latus(vicus_activa(x->app.vicus), VICUS_SINISTRUM);
    r = insula_ramus(x->app.vicus->repo, "scriba", chorda_ut_cstr(l->id,
        p));
    a = insula_ramus_attributum(&r, "nuntius");
    redde a && chorda_continet(*a, chorda_ex_literis(pars, p));
}

interior i32
historia_numerus (
    Mundus* x)
{
    VicusLatus* l = frons(x);

    redde (l && l->historia) ? xar_numerus(l->historia) : ZEPHYRUM;
}

/* $via: genus in frontem cum semita; latus sine semitis et semita
 * mala nuntium */
interior vacuum
_probare_viam (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    dispensator_recomponere(x->app.d);
    ictus(x, II, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(x->m,
        "i$via(sancti/sanctus/thomas-aquinas)"));
    CREDO_VERUM(manus_ludus_clavem(x->m, (character)XXVII, ZEPHYRUM));
    ictus(x, III, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(x->m,
        "i$via(calendarium/dies/2026-10-19)"));
    CREDO_VERUM(manus_ludus_clavem(x->m, (character)XXVII, ZEPHYRUM));
    ictus(x, IV, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(x->m, "i$via(sancti/nemo)"));
    CREDO_VERUM(manus_ludus_clavem(x->m, (character)XXVII, ZEPHYRUM));
    /* sanctus per semitam */
    ictus(x, II, III);
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(sanctorum_est(x, "visus", "sanctus"));
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus", "thomas-aquinas"));
    /* latus sine semitis */
    ictus(x, III, III);
    CREDO_VERUM(nuntius_continet(x,
        "calendarium: semitas nondum accipit",
        c->piscina));
    /* semita mala */
    ictus(x, IV, III);
    CREDO_VERUM(nuntius_continet(x, "visus", c->piscina));
}

/* historia: impelle (index -> sanctus -> sanctus alius) novum,
 * substitue (pagina textus) currentem; Ctrl+O / Ctrl+I exacte;
 * navigatio post reditum futura tollit */
interior vacuum
_probare_historiam (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "sancti", NIHIL));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(sanctorum_est(x, "visus", "index"));
    CREDO_AEQUALIS_I32(historia_numerus(x), I);
    /* s: ordo nomen - substitue (status, non impelle) */
    clavis(x, 's', ZEPHYRUM);
    CREDO_VERUM(sanctorum_est(x, "ordo", "nomen"));
    CREDO_AEQUALIS_I32(historia_numerus(x), I);
    /* Enter: sanctus electus (agnes, primus nomine) - impelle */
    clavis(x, (character)XIII, ZEPHYRUM);
    CREDO_VERUM(sanctorum_est(x, "visus", "sanctus"));
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus", "agnes-of-rome"));
    CREDO_AEQUALIS_I32(historia_numerus(x), II);
    /* j: sanctus proximus nomine (charles) - impelle */
    clavis(x, 'j', ZEPHYRUM);
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus",
        "charles-lwanga-and-companions"));
    CREDO_AEQUALIS_I32(historia_numerus(x), III);
    CREDO_AEQUALIS_I32(frons(x)->historia_cursor, II);
    /* Ctrl+O: agnes; iterum: index EXACTE (visus index, sanctus
     * apertus sublatus, ordo nomen servatus in introitu substituto) */
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus", "agnes-of-rome"));
    CREDO_AEQUALIS_I32(frons(x)->historia_cursor, I);
    CREDO_AEQUALIS_I32(historia_numerus(x), III);
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(sanctorum_est(x, "visus", "index"));
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus", NIHIL));
    CREDO_VERUM(sanctorum_est(x, "ordo", "nomen"));
    CREDO_AEQUALIS_I32(frons(x)->historia_cursor, ZEPHYRUM);
    /* initium: nihil ultra */
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_AEQUALIS_I32(frons(x)->historia_cursor, ZEPHYRUM);
    /* Ctrl+I: agnes, deinde charles */
    clavis(x, 'i', (i32)MOD_IMPERIUM);
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus", "agnes-of-rome"));
    clavis(x, 'i', (i32)MOD_IMPERIUM);
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus",
        "charles-lwanga-and-companions"));
    CREDO_AEQUALIS_I32(frons(x)->historia_cursor, II);
    /* reditus ad index, deinde navigatio nova: futura tolluntur */
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(sanctorum_est(x, "visus", "index"));
    clavis(x, 'j', ZEPHYRUM);
    clavis(x, 'j', ZEPHYRUM);
    clavis(x, (character)XIII, ZEPHYRUM);
    CREDO_VERUM(sanctorum_est(x, "sanctus_apertus", "thomas-aquinas"));
    CREDO_AEQUALIS_I32(historia_numerus(x), II);
    CREDO_AEQUALIS_I32(frons(x)->historia_cursor, I);
    clavis(x, 'i', (i32)MOD_IMPERIUM);
    CREDO_AEQUALIS_I32(frons(x)->historia_cursor, I);
}

/* latus sine semitis (scriba) Ctrl+O non capit: historia sanctorum
 * intacta */
interior vacuum
_probare_transitum (
    CredoContextus* c)
{
    Mundus* x;
       s32  cursor;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "sancti", NIHIL));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    (vacuum)vicus_pulsare(x->app.vicus);
    clavis(x, (character)XIII, ZEPHYRUM);
    CREDO_AEQUALIS_I32(historia_numerus(x), II);
    cursor = (s32)frons(x)->historia_cursor;
    /* focus in pagina sinistra (scriba) */
    ictus(x, VI, ZEPHYRUM);
    CREDO_VERUM(vicus_latus_focatum(x->app.vicus)
        == vicus_latus(vicus_activa(x->app.vicus), VICUS_SINISTRUM));
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(sanctorum_est(x, "visus", "sanctus"));
    CREDO_AEQUALIS_S32((s32)frons(x)->historia_cursor, cursor);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "via", _probare_viam, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "historia", _probare_historiam, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "transitus", _probare_transitum, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_semitae", SECTIONES);
}
