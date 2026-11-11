/* probatio_vicus_mundus.c - latus mundi in vico (mundus-plan T10):
 * '$world' in pagina scribae scriptum et ictum latus mundi in fronte
 * dextra aperit; vicus quadrum per fontem imaginum invenit; clavis
 * tenta personam movet (horologium verum); focus in latus aliud
 * claves tentas purgat (VicusFacies.focus_amissus); latus tectum non
 * pulsatur.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "volumen.h"
#include "eventus.h"
#include "fenestra.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "mundus_latus.h"
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
#define VIA_PLANI   "probationes/fixa/statuae/planum.glb"

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
} Orbis;

interior vacuum
orbem_parare (
    CredoContextus* c)
{
    Orbis* x;

    x = (Orbis*)piscina_allocare(c->piscina, magnitudo(Orbis));
    memset(x, ZEPHYRUM, magnitudo(Orbis));
    c->datum = x;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_mundus");
    si (x->volumen)
    {
        (vacuum)statuae_addere_generis(x->volumen, chorda_ex_literis(
            "clearing", c->piscina), filum_legere_totum(VIA_PLANI,
            c->piscina), STATUAE_GENUS_LOCUS);
        (vacuum)statuae_addere(x->volumen, chorda_ex_literis("lapis",
            c->piscina), filum_legere_totum(VIA_LAPIDIS, c->piscina));
        (vacuum)statuae_addere(x->volumen, chorda_ex_literis(
            "mushroom man", c->piscina), filum_legere_totum(VIA_FUNGI,
            c->piscina));
    }
}

interior vacuum
orbem_purgare (
    CredoContextus* c)
{
    Orbis* x = (Orbis*)c->datum;

    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

interior b32
aedificare (
             Orbis* x,
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
 * statuae) */
interior vacuum
verbum_ferire (
                 Orbis* x,
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

/* clavis depressa SOLA (sine liberatione): digitus tenet */
interior vacuum
clavem_tenere (
        Orbis* x,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(x->app.d, &e);
}

interior vacuum
_probare_latus (
    CredoContextus* c)
{
                  Orbis* x = (Orbis*)c->datum;
                  Vicus* v;
             ManusLudus* m;
             VicusLatus* l;
    MundusLatusMontatio* s;
                    f32  x0;
                    f32  z0;
              character  clavis[CLX];

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);

    CREDO_GRADUS("$world: latus mundi in fronte dextra");
    verbum_ferire(x, m, "$world");
    l = latus_frontis(v, "mundus");
    CREDO_NECESSE_NON_NIHIL(l);
    s = (MundusLatusMontatio*)l->montatio;
    CREDO_VERUM(s->volumen == x->volumen);
    CREDO_CHORDA_AEQUALIS_LITERIS(mundus_latus_titulus(s, c->piscina),
        "mushroom man");

    CREDO_GRADUS("facies: fons, pulsus, mora, focus amissus, titulus");
    CREDO_VERUM(l->facies.fons != NIHIL);
    CREDO_VERUM(l->facies.pulsare != NIHIL);
    CREDO_NECESSE(l->facies.mora != NIHIL);
    CREDO_VERUM(l->facies.focus_amissus != NIHIL);
    CREDO_VERUM(l->facies.locus_titulus != NIHIL);
    CREDO_VERUM(l->facies.effugium_redit);

    CREDO_GRADUS("vicus quadrum per fontem imaginum invenit");
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE_NON_NIHIL(s->quadrum.pixela);
    CREDO_VERUM(vicus_imago_fons(s->quadri_titulus, v) == &s->quadrum);
    CREDO_VERUM(s->quadrum.latitudo > C && s->quadrum.altitudo > C);
    CREDO_VERUM(s->positum);

    CREDO_GRADUS("persona stat: pulsus rari, quadrum stat");
    CREDO_VERUM(l->facies.mora(l->facies.pulsare_ctx)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    strcpy(clavis, s->quadri_clavis);
    fenestra_dormire(60000);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(strcmp(clavis, s->quadri_clavis) == ZEPHYRUM);

    CREDO_GRADUS("d tenta: pulsus crebri, persona movetur");
    x0 = s->corpus.x;
    z0 = s->corpus.z;
    clavem_tenere(x, 'd');
    CREDO_VERUM(s->tentae[MUNDUS_LATUS_DEXTRUM]);
    CREDO_VERUM(l->facies.mora(l->facies.pulsare_ctx)
        == MUNDUS_LATUS_MORA_MOTUS);
    CREDO_VERUM(vicus_mora_proxima(v) <= MUNDUS_LATUS_MORA_MOTUS);
    fenestra_dormire(60000);
    CREDO_VERUM(vicus_pulsare(v));
    fenestra_dormire(60000);
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(s->corpus.x > x0);
    CREDO_VERUM(s->corpus.z > z0);
    CREDO_VERUM(strcmp(clavis, s->quadri_clavis) != ZEPHYRUM);

    CREDO_GRADUS("focus in latus sinistrum: claves purgatae, sistit");
    CREDO_NECESSE(vicus_focum_ponere(v, VICUS_SINISTRUM));
    CREDO_FALSUM(s->tentae[MUNDUS_LATUS_DEXTRUM]);
    fenestra_dormire(40000);
    (vacuum)vicus_pulsare(v);
    x0 = s->corpus.x;
    z0 = s->corpus.z;
    fenestra_dormire(60000);
    (vacuum)vicus_pulsare(v);
    CREDO_F32_PROXIMUS(s->corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS(s->corpus.z, z0, 0.0001f);
    CREDO_VERUM(l->facies.mora(l->facies.pulsare_ctx)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);

    CREDO_GRADUS("latus aliud supra: mundus tectus non pulsatur");
    CREDO_NECESSE(vicus_focum_ponere(v, VICUS_DEXTRUM));
    clavem_tenere(x, 'd');
    CREDO_NECESSE(vicus_acervo_aperire(v, "aetherophonum", NIHIL));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_NECESSE_NON_NIHIL(latus_frontis(v, "aetherophonum"));
    /* latus tectum focum amisit: clavis tenta purgata */
    CREDO_FALSUM(s->tentae[MUNDUS_LATUS_DEXTRUM]);
    strcpy(clavis, s->quadri_clavis);
    fenestra_dormire(60000);
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(strcmp(clavis, s->quadri_clavis) == ZEPHYRUM);

    CREDO_GRADUS("$world iterum: latus idem in frontem redit");
    CREDO_NECESSE(vicus_acervo_aperire(v, "mundus", NIHIL));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    l = latus_frontis(v, "mundus");
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(l->montatio == (vacuum*)s);
    mundus_latus_dimittere(s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latus $world", _probare_latus, orbem_parare, orbem_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_mundus", SECTIONES);
}
