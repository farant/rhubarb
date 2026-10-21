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
#include "kalendarium.h"
#include "nomenclator.h"
#include "personae.h"
#include "pictor_documentum.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
         ManusLudus* m;
                i32  vocationes_secutae;   /* semitam_secuta */
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
        "i$via(admonitiones)"));
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
        "admonitiones: semitas nondum accipit",
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

/* semitae R4: calendarium per $via; historia (visus impelle, dies
 * substitue) Ctrl+O restituit; post gradum historiae vicus
 * facies.semitam_secuta vocat (calendarium: liber notarum) */
interior b32
secuta_numerare (
    vacuum* ctx)
{
    ((Mundus*)ctx)->vocationes_secutae++;
    redde VERUM;
}

interior b32
kalendarii_est (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    VicusLatus* l;
        chorda* a;

    l = frons(x);
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "calendarium"))
    {
        redde FALSUM;
    }
    a = insula_ramus_attributum(&((KalendariumMontatio*)l->montatio)
        ->ramus, titulus);
    redde a && chorda_aequalis_literis(*a, valor);
}

interior vacuum
_probare_calendarium (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "calendarium",
        "calendarium/dies/2026-10-19"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(kalendarii_est(x, "visus", "dies"));
    CREDO_VERUM(kalendarii_est(x, "dies_electus", "2026-10-19"));
    CREDO_AEQUALIS_I32(historia_numerus(x), I);
    CREDO_VERUM(frons(x)->facies.semitam_secuta
        == kalendarium_semitam_secuta);
    frons(x)->facies.semitam_secuta      = secuta_numerare;
    frons(x)->facies.semitam_secuta_ctx  = x;
    /* visus mutatus: impelle */
    CREDO_VERUM(vicus_acervo_mutare(x->app.vicus, "calendarium",
        "calendarium/mensis/2026-10-19"));
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_AEQUALIS_I32(historia_numerus(x), II);
    /* dies mutatus: substitue */
    CREDO_VERUM(vicus_acervo_mutare(x->app.vicus, "calendarium",
        "calendarium/mensis/2026-10-25"));
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_AEQUALIS_I32(historia_numerus(x), II);
    CREDO_AEQUALIS_I32(x->vocationes_secutae, ZEPHYRUM);
    /* Ctrl+O: visus diei in XIX; hospes sequitur */
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(kalendarii_est(x, "visus", "dies"));
    CREDO_VERUM(kalendarii_est(x, "dies_electus", "2026-10-19"));
    CREDO_AEQUALIS_I32(x->vocationes_secutae, I);
    /* Ctrl+I: mensis in XXV */
    clavis(x, 'i', (i32)MOD_IMPERIUM);
    CREDO_VERUM(kalendarii_est(x, "visus", "mensis"));
    CREDO_VERUM(kalendarii_est(x, "dies_electus", "2026-10-25"));
    CREDO_AEQUALIS_I32(x->vocationes_secutae, II);
    /* in termino: nihil applicatum, hospes non vocatur */
    clavis(x, 'i', (i32)MOD_IMPERIUM);
    CREDO_AEQUALIS_I32(x->vocationes_secutae, II);
}

/* attributum rami nomenclatoris frontalis = valor (NIHIL absens) */
interior b32
nomenclatoris_est (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    VicusLatus* l;
        chorda* a;

    l = frons(x);
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "nomenclator"))
    {
        redde FALSUM;
    }
    a = insula_ramus_attributum(&((NomenclatorMontatio*)l->montatio)
        ->ramus, titulus);
    si (valor == NIHIL)
    {
        redde !a || a->mensura == ZEPHYRUM;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

/* semitae R4b: nomenclator - persona aperta (impelle), Esc ad indicem
 * (impelle), Ctrl+O personam restituit et iterum indicem; hospes post
 * gradum vocatur */
interior vacuum
_probare_nomenclatorem (
    CredoContextus* c)
{
                 Mundus* x;
     PersonaeDocumentum* doc;
                    i32  numerator;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->volumen);
    doc = personae_aperire(c->piscina, internamentum_creare(c->piscina),
        x->volumen, "personae", ZEPHYRUM);
    numerator = ZEPHYRUM;
    personae_monetam_ponere(doc, moneta_numerator, &numerator);
    CREDO_NECESSE(doc
        && chorda_aequalis_literis(personae_hominem_creare(
        doc, chorda_ex_literis("Anna", c->piscina), chorda_ex_literis(
        "Rossi", c->piscina)), "1"));
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "nomenclator",
        NIHIL));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(nomenclatoris_est(x, "persona_aperta", NIHIL));
    CREDO_AEQUALIS_I32(historia_numerus(x), I);
    CREDO_VERUM(frons(x)->facies.semitam_secuta
        == nomenclator_semitam_secuta);
    frons(x)->facies.semitam_secuta      = secuta_numerare;
    frons(x)->facies.semitam_secuta_ctx  = x;
    CREDO_VERUM(vicus_acervo_mutare(x->app.vicus, "nomenclator",
        "nomenclator/1"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(nomenclatoris_est(x, "persona_aperta", "1"));
    CREDO_AEQUALIS_I32(historia_numerus(x), II);
    clavis(x, (character)XXVII, ZEPHYRUM);
    CREDO_VERUM(nomenclatoris_est(x, "persona_aperta", NIHIL));
    CREDO_AEQUALIS_I32(historia_numerus(x), III);
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(nomenclatoris_est(x, "persona_aperta", "1"));
    CREDO_AEQUALIS_I32(x->vocationes_secutae, I);
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(nomenclatoris_est(x, "persona_aperta", NIHIL));
    CREDO_AEQUALIS_I32(x->vocationes_secutae, II);
}

/* documentum pictoris frontalis (NIHIL si frons non pictor) */
interior PictorDocumentum*
pictura_frontis (
    Mundus* x)
{
    VicusLatus* l;

    l = frons(x);
    si (   !l || !l->montata || !chorda_aequalis_literis(l->genus,
            "pictor"))
    {
        redde NIHIL;
    }
    redde ((PictorMontatio*)l->montatio)->doc;
}

/* semitae R4c: pictor - pictura mutata (impelle), Ctrl+O picturam
 * priorem et documentum eius restituit (hospes verus);
 * $pictor-next a pictura restituta pergit (campus pictura, non
 * argumentum stalum) */
interior vacuum
_probare_pictorem (
    CredoContextus* c)
{
               Mundus* x;
     PictorDocumentum* x1;
     PictorDocumentum* x2;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->volumen);
    x1 = pictor_documentum_creare(c->piscina, internamentum_creare(
        c->piscina), x->volumen, "x1", C, LX, LXIV);
    x2 = pictor_documentum_creare(c->piscina, internamentum_creare(
        c->piscina), x->volumen, "x2", LX, XL, LXIV);
    CREDO_NECESSE(x1 && x2);
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "pictor", "x1"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(pictura_frontis(x)
        && pictura_frontis(x)->latitudo == C);
    CREDO_AEQUALIS_I32(historia_numerus(x), I);
    CREDO_VERUM(frons(x)->facies.semitam_secuta
        == pictor_semitam_secuta);
    CREDO_VERUM(vicus_acervo_mutare(x->app.vicus, "pictor", "x2"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(pictura_frontis(x)
        && pictura_frontis(x)->latitudo == LX);
    CREDO_AEQUALIS_I32(historia_numerus(x), II);
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(pictura_frontis(x)
        && pictura_frontis(x)->latitudo == C);
    /* $pictor-next ex x1: x2 (argumentum stalum "x2" circulum
     * reciperet) */
    ictus(x, VI, ZEPHYRUM);   /* focus: pagina sinistra */
    ictus(x, II, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(x->m, "i$pictor-next"));
    CREDO_VERUM(manus_ludus_clavem(x->m, (character)XXVII, ZEPHYRUM));
    ictus(x, II, III);
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(pictura_frontis(x) && pictura_frontis(x)->latitudo == LX
        && pictura_frontis(x)->altitudo == XL);
}

/* semitae R4c: montatio cum pictura ignota picturam lateris ipsius
 * ostendit, numquam condit (solum argumentum mutatum, $pictor-new) */
interior vacuum
_probare_pictorem_ignotum (
    CredoContextus* c)
{
     Mundus* x;
        Xar* l;
     chorda* pictura;
        i32  i;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "pictor", "nemo"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    CREDO_NECESSE(pictura_frontis(x) != NIHIL);
    pictura = insula_ramus_attributum(&((PictorMontatio*)frons(x)
        ->montatio)->ramus, "pictura");
    CREDO_VERUM(pictura && chorda_aequalis(*pictura, frons(x)->id));
    l = pictor_documenta_enumerare(x->volumen, c->piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        CREDO_FALSUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(l,
            i),
            "nemo"));
    }
}

/* frons dextra applicationis datae (NIHIL si non montata) */
interior VicusLatus*
frons_applicationis (
    VicusApplicatio* app)
{
    VicusLatus* l;

    l = vicus_latus(vicus_activa(app->vicus), VICUS_DEXTRUM);
    redde (l && l->montata) ? l : NIHIL;
}

/* semitae R4d (VI-e): pictor - post Ctrl+O vicus novus (initium) in
 * eodem volumine picturam restitutam ostendit, non argumentum */
interior vacuum
_probare_initium_pictoris (
    CredoContextus* c)
{
              Mundus* x;
     VicusApplicatio  altera;
          VicusLatus* l;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->volumen);
    CREDO_NECESSE(pictor_documentum_creare(c->piscina,
        internamentum_creare(c->piscina), x->volumen, "x1", C, LX, LXIV)
        && pictor_documentum_creare(c->piscina, internamentum_creare(
        c->piscina), x->volumen, "x2", LX, XL, LXIV));
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "pictor", "x1"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(vicus_acervo_mutare(x->app.vicus, "pictor", "x2"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(pictura_frontis(x)
        && pictura_frontis(x)->latitudo == C);
    CREDO_CHORDA_AEQUALIS_LITERIS(frons(x)->argumentum, "x2");
    CREDO_CHORDA_AEQUALIS_LITERIS(frons(x)->semita_servata,
        "pictor/x1");
    memset(&altera, ZEPHYRUM, magnitudo(altera));
    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    (vacuum)vicus_pulsare(altera.vicus);
    l = frons_applicationis(&altera);
    CREDO_NECESSE(l && chorda_aequalis_literis(l->genus, "pictor"));
    CREDO_VERUM(((PictorMontatio*)l->montatio)->doc->latitudo == C);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->argumentum, "x2");
}

/* semitae R4d (VI-e): calendarium - visus servatus, dies (transiens)
 * ad hodie post initium (argumentum "calendarium/dies/2026-10-19"
 * diem non restituit) */
interior vacuum
_probare_initium_calendarii (
    CredoContextus* c)
{
              Mundus* x;
     VicusApplicatio  altera;
          VicusLatus* l;
              chorda* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    CREDO_VERUM(vicus_acervo_aperire(x->app.vicus, "calendarium",
        "calendarium/dies/2026-10-19"));
    (vacuum)vicus_pulsare(x->app.vicus);
    dispensator_recomponere(x->app.d);
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(kalendarii_est(x, "dies_electus", "2026-10-19"));
    CREDO_CHORDA_AEQUALIS_LITERIS(frons(x)->semita_servata,
        "calendarium/dies");
    memset(&altera, ZEPHYRUM, magnitudo(altera));
    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    (vacuum)vicus_pulsare(altera.vicus);
    l = frons_applicationis(&altera);
    CREDO_NECESSE(l
        && chorda_aequalis_literis(l->genus, "calendarium"));
    a = insula_ramus_attributum(&((KalendariumMontatio*)l->montatio)
        ->ramus, "visus");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "dies"));
    a = insula_ramus_attributum(&((KalendariumMontatio*)l->montatio)
        ->ramus, "dies_electus");
    CREDO_VERUM(!a || a->mensura == ZEPHYRUM);
}

/* semitae R4d: argumentum identitas manet - post navigationem in
 * latere agendae Cmd+[ / Cmd+] idem latus invenit (non alterum) */
interior vacuum
_probare_cyclum_post_navigationem (
    CredoContextus* c)
{
    Mundus* x;
       i32  n;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    clavis(x, ']', (i32)MOD_SUPER);
    clavis(x, ']', (i32)MOD_SUPER);
    clavis(x, ']', (i32)MOD_SUPER);
    CREDO_NECESSE(frons(x) && chorda_aequalis_literis(frons(x)->genus,
        "calendarium"));
    CREDO_CHORDA_AEQUALIS_LITERIS(frons(x)->argumentum, "agenda");
    CREDO_VERUM(kalendarium_argumentum_ponere(
        (KalendariumMontatio*)frons(x)->montatio,
        "calendarium?rubrica_filtrum=domus"));
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_VERUM(chorda_continet(frons(x)->semita_servata,
        chorda_ex_literis("rubrica_filtrum=domus", c->piscina)));
    n = (i32)xar_numerus(vicus_activa(x->app.vicus)->acervus);
    clavis(x, '[', (i32)MOD_SUPER);
    clavis(x, ']', (i32)MOD_SUPER);
    CREDO_VERUM(kalendarii_est(x, "rubrica_filtrum", "domus"));
    CREDO_CHORDA_AEQUALIS_LITERIS(frons(x)->argumentum, "agenda");
    CREDO_AEQUALIS_I32((i32)xar_numerus(vicus_activa(x->app.vicus)
        ->acervus), n);
}

/* semitae R4d: initium sine navigatione nihil in volumine scribit -
 * semita prima lateris sine argumento tacite notatur (pictor tabulae
 * 2 in dispositione ordinaria) */
interior vacuum
_probare_initium_tacitum (
    CredoContextus* c)
{
     Mundus* x;
        s64  acta;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(mundum_struere(x, c));
    acta = volumen_summa_actorum(x->volumen);
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_pulsare(x->app.vicus);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(x->volumen), acta);
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
    { "calendarium", _probare_calendarium, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "nomenclator", _probare_nomenclatorem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pictor", _probare_pictorem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "pictor_ignotus", _probare_pictorem_ignotum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "initium_pictoris", _probare_initium_pictoris, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "initium_calendarii", _probare_initium_calendarii, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "cyclus", _probare_cyclum_post_navigationem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "initium_tacitum", _probare_initium_tacitum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_semitae", SECTIONES);
}
