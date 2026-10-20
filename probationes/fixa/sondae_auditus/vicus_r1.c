/* sonda: latus post mutationem magnitudinis apertum - quae superficies? */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include <stdio.h>
#include <string.h>

hic_manens i32 ultima_lat = -1;
hic_manens i32 ultima_alt = -1;

interior b32
scribam_montare (vacuum* sedes, Piscina* p, InternamentumChorda* in,
    Volumen* v, InsulaRepositorium* r, constans character* id,
    constans character* argumentum, i32 latitudo, i32 altitudo,
    vacuum* ctx)
{
    (vacuum)argumentum;
    (vacuum)ctx;
    ultima_lat = latitudo;
    ultima_alt = altitudo;
    imprimere("montare %s: %d x %d\n", id, (int)latitudo, (int)altitudo);
    redde scriba_montare((ScribaMontatio*)sedes, p, in, v, r, id,
                         latitudo, altitudo, NIHIL, NIHIL);
}

interior b32
pictorem_montare (vacuum* sedes, Piscina* p, InternamentumChorda* in,
    Volumen* v, InsulaRepositorium* r, constans character* id,
    constans character* argumentum, i32 latitudo, i32 altitudo,
    vacuum* ctx)
{
    (vacuum)argumentum;
    (vacuum)ctx;
    imprimere("montare %s: %d x %d\n", id, (int)latitudo, (int)altitudo);
    redde pictor_montare((PictorMontatio*)sedes, p, in, v, r, id,
                         latitudo, altitudo, NIHIL);
}

interior vacuum
scribam_describere (vacuum* montatio, VicusFacies* f)
{
    ScribaMontatio* m;
    m                 = (ScribaMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = scriba_componere;
    f->componere_ctx  = &m->compositio;
}

interior vacuum
pictorem_describere (vacuum* montatio, VicusFacies* f)
{
    PictorMontatio* m;
    m                 = (PictorMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = pictor_componere;
    f->componere_ctx  = &m->compositio;
    f->fons           = pictor_imago_fons;
    f->fons_ctx       = &m->figurae_ctx;
}

interior vacuum
superficiem_monstrare (Vicus* v, constans character* genus,
    constans character* id)
{
    InsulaRamus ramus;
    chorda* a;
    chorda* b;

    ramus = insula_ramus(v->repo, genus, id);
    a = insula_ramus_attributum(&ramus, "superficies_latitudo");
    b = insula_ramus_attributum(&ramus, "superficies_altitudo");
    imprimere("%s: superficies %.*s x %.*s\n", id,
        a ? (int)a->mensura : 1, a ? (constans character*)a->datum : "?",
        b ? (int)b->mensura : 1, b ? (constans character*)b->datum : "?");
}

integer
principale (vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Volumen* vol;
    Vicus* v;
    Dispensator* d;
    Eventus e;
    chorda* rl;
    VicusLatus* l;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    intern  = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "sonda_vicus_r1");
    v = vicus_creare(piscina, intern, vol, 480, 480);
    vicus_genus_addere(v, "scriba", magnitudo(ScribaMontatio),
        scribam_montare, scribam_describere, NIHIL);
    vicus_genus_addere(v, "pictor", magnitudo(PictorMontatio),
        pictorem_montare, pictorem_describere, NIHIL);
    si (!vicus_aperire(v,
        "<tabulae activa=\"s1\">"
        "<tabula id=\"s1\"><latus genus=\"scriba\"/>"
        "<acervus><latus genus=\"pictor\"/></acervus></tabula>"
        "</tabulae>"))
    {
        imprimere("aperire defecit\n");
        redde 1;
    }
    d = dispensator_creare(piscina, intern, v->repo, vicus_actiones(v),
        vicus_componere, v, 300);
    memset(&e, 0, magnitudo(Eventus));
    e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
    e.tempus = 1000;
    e.datum.mutare_magnitudinem.latitudo = 300;
    e.datum.mutare_magnitudinem.altitudo = 320;
    dispensator_tractare(d, &e);
    rl = insula_attributum(v->repo, "superficies_latitudo");
    imprimere("radix post mutationem: superficies_latitudo %.*s\n",
        rl ? (int)rl->mensura : 1, rl ? (constans character*)rl->datum : "?");
    superficiem_monstrare(v, "pictor", "s1_dextrum_pictor");

    imprimere("-- aperire $scriba(notae) post mutationem --\n");
    imprimere("aperire: %d\n", (int)vicus_acervo_aperire(v, "scriba", "notae"));
    (vacuum)vicus_pulsare(v);
    l = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
    imprimere("frons: %.*s\n", (int)l->id.mensura, (constans character*)l->id.datum);
    superficiem_monstrare(v, "scriba", chorda_ut_cstr(l->id, piscina));
    imprimere("EXPECTATUM 150 x 312 (ut s1_dextrum_pictor); montator acceperat %d x %d\n",
        (int)ultima_lat, (int)ultima_alt);
    {
        Componens* arbor;
        dispensator_recomponere(d);
        arbor = dispensator_arbor(d);
        (vacuum)arbor;
    }
    volumen_claudere(vol);
    redde 0;
}
