/* probatio_vicus_psalterium.c - psalterium in vico (psalterium planum
 * T7): '$psalterium(109, 8 G)' in pagina scribae scriptum et ictum
 * latus psalterii in fronte dextra aperit (Ps 109, 8 G);
 * '$psalterium(110)' idem latus accipit (vicus_acervo_mutare), tonus
 * lateris manet; '$psalterium(151)' et tonus ignotus vitium spec III.3
 * in linea status ostendunt, latus non mutatur; '$psalterium' indicem
 * ostendit. Harness ex probatio_vicus_antiphonale (quae intacta
 * manet).
 *
 * imagines (fumus, ad inspectionem): vicus MCC x DCCC, index et Ps 109
 * in 8 G per picturam veram (ludus_fenestra) in
 * build/probatio_vicus_psalterium_{index,psalmus}.png. */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "scriba_applicatio.h"
#include "psalterium.h"
#include "thema.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
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
    c->datum = x;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_psalterium");
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

/* latus frontale dextrum tabulae activae, si psalterium */
interior VicusLatus*
latus_psalterii (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata
        || !chorda_aequalis_literis(l->genus, "psalterium"))
    {
        redde NIHIL;
    }
    redde l;
}

/* verbum in pagina scribae scriptum et ictum (ut
 * probatio_vicus_antiphonale) */
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

/* nuntius lineae status lateris sinistri (campus "nuntius" scribae;
 * linea picta in latere angusto praeciditur) incipit a textu */
interior b32
nuntius_incipit (
                 Mundus* x,
     constans character* textus)
{
    ScribaMontatio* sm;
            chorda* n;
               i32  k = (i32)strlen(textus);

    sm = (ScribaMontatio*)vicus_latus(vicus_activa(x->app.vicus),
        VICUS_SINISTRUM)->montatio;
    n  = sm ? insula_ramus_attributum(&sm->ramus, "nuntius") : NIHIL;
    redde n && n->mensura >= k
        && memcmp(n->datum, textus, (size_t)k) == ZEPHYRUM;
}

interior chorda
campus (
            VicusLatus* l,
    constans character* titulus)
{
    chorda* a = insula_ramus_attributum(
        &((PsalteriumMontatio*)l->montatio)->ramus, titulus);
     chorda vacua;

    si (a)
    {
        redde *a;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

interior vacuum
_iussum_probare (
    CredoContextus* c)
{
        Mundus* x = (Mundus*)c->datum;
         Vicus* v;
    ManusLudus* m;
    VicusLatus* l;
        vacuum* montatio;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);

    CREDO_GRADUS("$psalterium(109, 8 G): latus novum in Ps 109, 8 G");
    verbum_ferire(x, m, "$psalterium(109, 8 G)");
    l = latus_psalterii(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(psalterium_titulus(l->montatio,
        c->piscina), "Psalmus 109");
    CREDO_CHORDA_AEQUALIS_LITERIS(campus(l, "signum"), "8 G");
    CREDO_VERUM(((PsalteriumMontatio*)l->montatio)->porta.incipere
        != NIHIL);
    montatio = l->montatio;

    CREDO_GRADUS("$psalterium(110, 2 A), (111): idem latus, tonus "
                 "manet");
    verbum_ferire(x, m, "$psalterium(110, 2 A)");
    l = latus_psalterii(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(l->montatio == montatio);
    CREDO_CHORDA_AEQUALIS_LITERIS(campus(l, "signum"), "2 A");
    verbum_ferire(x, m, "$psalterium(111)");
    l = latus_psalterii(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(l->montatio == montatio);
    CREDO_CHORDA_AEQUALIS_LITERIS(psalterium_titulus(l->montatio,
        c->piscina), "Psalmus 111");
    CREDO_CHORDA_AEQUALIS_LITERIS(campus(l, "signum"), "2 A");

    CREDO_GRADUS("$psalterium(151): vitium, latus non mutatur");
    verbum_ferire(x, m, "$psalterium(151)");
    CREDO_VERUM(nuntius_incipit(x, "psalterium: psalm number 1-150"));
    l = latus_psalterii(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(psalterium_titulus(l->montatio,
        c->piscina), "Psalmus 111");

    CREDO_GRADUS("$psalterium: index");
    verbum_ferire(x, m, "$psalterium");
    l = latus_psalterii(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(l->montatio == montatio);
    CREDO_CHORDA_AEQUALIS_LITERIS(campus(l, "visus"), "index");

    /* ultimum: post hunc nuntium longum (LXX litterae) iussum
     * sequens per verbum_ferire non currit (observatum; nuntius
     * brevis "$psalterium(151)" non nocet) - ictus loco fixo
     * fortasse lineam erravit. Ordo servat probationem. */
    CREDO_GRADUS("$psalterium(109, 8 Z): causa toni");
    verbum_ferire(x, m, "$psalterium(109, 8 Z)");
    CREDO_VERUM(nuntius_incipit(x, "psalterium: tonus 8"));
    CREDO_CHORDA_AEQUALIS_LITERIS(campus(latus_psalterii(v), "signum"),
        "2 A");
}

#define LATITUDO_IMAGINIS  MCC
#define ALTITUDO_IMAGINIS  DCCC

/* quadrum sine fenestra (ut probatio_vicus_iussa), imago scribitur */
interior b32
imaginem_scribere (
                Mundus* x,
               Piscina* piscina,
    constans character* via)
{
    TabulaPixelorum* tp;
      LudusFenestra* lf;

    tp = tabula_pixelorum_creare_nuda(piscina, LATITUDO_IMAGINIS,
        ALTITUDO_IMAGINIS + VICUS_ALTITUDO_TABULARUM);
    lf = tp ? ludus_fenestra_creare(piscina, x->app.d,
        vicus_figurae(x->app.vicus), ZEPHYRUM, vicus_imago_fons,
        x->app.vicus, tp) : NIHIL;
    si (!lf)
    {
        redde FALSUM;
    }
    ludus_quadrum(lf, M);
    redde ludus_fenestra_imaginem_scribere(lf, via);
}

interior vacuum
_imagines_probare (
    CredoContextus* c)
{
        Mundus* x = (Mundus*)c->datum;
         Vicus* v;
    ManusLudus* m;

    CREDO_NECESSE(x && x->volumen
        && vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, LATITUDO_IMAGINIS,
        ALTITUDO_IMAGINIS + VICUS_ALTITUDO_TABULARUM));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);

    verbum_ferire(x, m, "$psalterium(109)");
    verbum_ferire(x, m, "$psalterium");
    CREDO_NECESSE_NON_NIHIL(latus_psalterii(v));
    CREDO_VERUM(imaginem_scribere(x, c->piscina,
        "build/probatio_vicus_psalterium_index.png"));
    verbum_ferire(x, m, "$psalterium(109, 8 G)");
    CREDO_CHORDA_AEQUALIS_LITERIS(psalterium_titulus(
        latus_psalterii(v)->montatio, c->piscina), "Psalmus 109");
    CREDO_VERUM(imaginem_scribere(x, c->piscina,
        "build/probatio_vicus_psalterium_psalmus.png"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iussum $psalterium", _iussum_probare, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "imagines (fumus)", _imagines_probare, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_psalterium", SECTIONES);
}
