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
#include "thema.h"
#include "manus_ludus.h"
#include "dispensator.h"
#include "credo.h"
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera", _probare_latera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "duo", _probare_duo, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "via", _probare_via, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_lector", SECTIONES);
}
