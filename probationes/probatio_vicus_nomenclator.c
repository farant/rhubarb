/* probatio_vicus_nomenclator.c - nomenclator in vico (nomenclator-plan
 * P2a): genus 'nomenclator' montatur, documentum personarum UNUM
 * lateribus commune, verbum '$contacts' in pagina scribae latus
 * aperit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "nomenclator.h"
#include "personae.h"
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
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_nomenclator");
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

/* latus frontale tabulae activae, si nomenclator */
interior NomenclatorMontatio*
nomenclator_frontalis (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "nomenclator"))
    {
        redde NIHIL;
    }
    redde (NomenclatorMontatio*)l->montatio;
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

/* latera duo: documentum personarum unum */
interior vacuum
_probare_latera (
    CredoContextus* c)
{
                 Mundus* x;
                  Vicus* v;
    NomenclatorMontatio* prima;
    NomenclatorMontatio* altera;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    prima = nomenclator_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(prima);
    CREDO_NON_NIHIL(prima->documentum);
    /* tabula alia: latus novum (in eadem tabula idem genus sine
     * argumento latus exstans reddit) */
    CREDO_NECESSE(vicus_activam_ponere(v, "2"));
    CREDO_VERUM(vicus_acervo_aperire(v, "nomenclator", NIHIL));
    (vacuum)vicus_pulsare(v);
    altera = nomenclator_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(altera);
    CREDO_VERUM(altera != prima);
    CREDO_VERUM(altera->documentum == prima->documentum);
    /* persona in documento communi: in utroque latere */
    CREDO_VERUM(personae_hominem_creare(prima->documentum,
        chorda_ex_literis("Anna", c->piscina),
        chorda_ex_literis("Rossi",
        c->piscina)) > ZEPHYRUM);
    CREDO_AEQUALIS_S32(nomenclator_persona_electa(altera), I);
}

/* '$contacts' in pagina scribae: ictus in verbo latus nomenclatoris
 * aperit */
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
    CREDO_VERUM(manus_ludus_scribere(m, "i$contacts"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV);
    (vacuum)vicus_pulsare(v);
    CREDO_NON_NIHIL(nomenclator_frontalis(v));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latera", _probare_latera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "verbum", _probare_verbum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_nomenclator", SECTIONES);
}
