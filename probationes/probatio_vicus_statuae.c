/* probatio_vicus_statuae.c - latus statuarum in vico (statuae-plan
 * T9): '$models' in pagina scribae scriptum et ictum latus statuarum
 * in fronte dextra aperit; vicus quadrum eius per fontem imaginum
 * invenit; latus visibile cum cyclo currente pulsatur et quadrum
 * mutat (horologium fenestrae verum), quiescens pulsum rarum petit,
 * tectum (latus aliud supra) non pulsatur. */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "filum.h"
#include "fenestra_tempus.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "statuae.h"
#include "statuae_repositorium.h"
#include "thema.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define VIA_FUNGI   "probationes/fixa/statuae/fungus.glb"

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
    c->datum = x;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_statuae");
    si (x->volumen)
    {
        (vacuum)statuae_addere(x->volumen, chorda_ex_literis("lapis",
            c->piscina), filum_legere_totum(VIA_LAPIDIS, c->piscina));
        (vacuum)statuae_addere(x->volumen, chorda_ex_literis("fungus",
            c->piscina), filum_legere_totum(VIA_FUNGI, c->piscina));
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

/* latus frontale dextrum tabulae activae, si generis dati */
interior VicusLatus*
latus_frontis (
                 Vicus* v,
    constans character* genus)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata
        || !chorda_aequalis_literis(l->genus, genus))
    {
        redde NIHIL;
    }
    redde l;
}

/* verbum in pagina scribae scriptum et ictum (ut probatio_vicus_
 * antiphonale) */
interior vacuum
verbum_ferire (
                Mundus* x,
            ManusLudus* m,
    constans character* verbum)
{
    (vacuum)manus_ludus_premere_ad(m, VI + III, VICUS_ALTITUDO_TABULARUM
        + VIII + II * VIII + IV);
    (vacuum)manus_ludus_scribere(m, "0d$i");
    (vacuum)manus_ludus_scribere(m, verbum);
    (vacuum)manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM);
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV);
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_pulsare(x->app.vicus);
}

interior vacuum
_probare_latus (
    CredoContextus* c)
{
             Mundus* x = (Mundus*)c->datum;
              Vicus* v;
         ManusLudus* m;
         VicusLatus* l;
    StatuaeMontatio* s;
          character  clavis[CXXVIII];

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);

    CREDO_GRADUS("$models: latus statuarum in fronte dextra");
    verbum_ferire(x, m, "$models");
    l = latus_frontis(v, "statuae");
    CREDO_NECESSE_NON_NIHIL(l);
    s = (StatuaeMontatio*)l->montatio;
    CREDO_VERUM(s->volumen == x->volumen);
    CREDO_CHORDA_AEQUALIS_LITERIS(statuae_titulus(s, c->piscina),
        "lapis");

    CREDO_GRADUS("facies: fons, pulsus, mora, ictus primus, titulus");
    CREDO_VERUM(l->facies.fons != NIHIL);
    CREDO_VERUM(l->facies.pulsare != NIHIL);
    CREDO_NECESSE(l->facies.mora != NIHIL);
    CREDO_VERUM(l->facies.ictus_primus != NIHIL);
    CREDO_VERUM(l->facies.locus_titulus != NIHIL);
    CREDO_VERUM(l->facies.effugium_redit);

    CREDO_GRADUS("vicus quadrum per fontem imaginum invenit");
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE_NON_NIHIL(s->quadrum.pixela);
    CREDO_VERUM(vicus_imago_fons(s->quadri_titulus, v) == &s->quadrum);
    CREDO_VERUM(s->quadrum.latitudo > C && s->quadrum.altitudo > C);

    CREDO_GRADUS("cyclus currit: pulsus crebri, quadrum mutatur");
    CREDO_VERUM(l->facies.mora(l->facies.pulsare_ctx)
        == STATUAE_MORA_MOTUS);
    CREDO_VERUM(vicus_mora_proxima(v) >= ZEPHYRUM);
    CREDO_VERUM(vicus_mora_proxima(v) <= STATUAE_MORA_MOTUS);
    strcpy(clavis, s->quadri_clavis);
    fenestra_dormire(60000);
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(strcmp(clavis, s->quadri_clavis) != ZEPHYRUM);

    CREDO_GRADUS("Spatium: quiescit, pulsus rarus, quadrum stat");
    (vacuum)manus_ludus_clavem(m, ' ', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(l->facies.mora(l->facies.pulsare_ctx)
        == STATUAE_MORA_INSPECTIONIS);
    strcpy(clavis, s->quadri_clavis);
    fenestra_dormire(60000);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(strcmp(clavis, s->quadri_clavis) == ZEPHYRUM);
    (vacuum)manus_ludus_clavem(m, ' ', ZEPHYRUM);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(l->facies.mora(l->facies.pulsare_ctx)
        == STATUAE_MORA_MOTUS);

    CREDO_GRADUS("latus aliud supra: statuae tectae non pulsantur");
    CREDO_NECESSE(vicus_acervo_aperire(v, "aetherophonum", NIHIL));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_NECESSE_NON_NIHIL(latus_frontis(v, "aetherophonum"));
    strcpy(clavis, s->quadri_clavis);
    fenestra_dormire(60000);
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(strcmp(clavis, s->quadri_clavis) == ZEPHYRUM);

    CREDO_GRADUS("$models iterum: latus idem in frontem redit");
    CREDO_NECESSE(vicus_acervo_aperire(v, "statuae", NIHIL));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    l = latus_frontis(v, "statuae");
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(l->montatio == (vacuum*)s);
    statuae_dimittere(s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latus $models", _probare_latus, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_statuae", SECTIONES);
}
