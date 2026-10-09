/* probatio_vicus_commutatio.c - ramus activus in Motu (insula-rami-plan
 * T3a)
 *
 * Vicus Motum dispensatoris ligat: focus et effusio pan/zoom per
 * ramum activum (radix nihil accipit). Commutatio: gestus relinquentis
 * effunditur (insertio scribae pendens committitur), captura et ictus
 * pendens abiciuntur, gestus advenientis ponitur; focus per tabulam
 * servatur. vicus-latera S2a: tabula = par laterum; latus sinistrum
 * focatum, ramus = via lateris. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "motus.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior b32
scribam_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)ctx;
    redde scriba_montare((ScribaMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo, NIHIL);
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)ctx;
    redde pictor_montare((PictorMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
}

interior vacuum
scribae_gestum (
     Motus* motus,
    vacuum* ctx)
{
    scriba_gestum_ponere(motus, (ScribaActiones*)ctx);
}

interior vacuum
scribam_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    ScribaMontatio* m;

    m                 = (ScribaMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = scriba_componere;
    f->componere_ctx  = &m->compositio;
    f->fons           = NIHIL;
    f->fons_ctx       = NIHIL;
    f->gestum_ponere  = scribae_gestum;
    f->gestum_ctx     = &m->actiones_ctx;
}

interior vacuum
pictorem_describere (
         vacuum* montatio,
    VicusFacies* f)
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

interior b32
valor_est (
                chorda* a,
    constans character* v)
{
    redde a ? chorda_aequalis_literis(*a, v) : FALSUM;
}

interior chorda*
rami_attributum (
                 Vicus* v,
    constans character* genus,
    constans character* id,
    constans character* titulus)
{
    InsulaRamus ramus;

    ramus = insula_ramus(v->repo, genus, id);
    redde insula_ramus_attributum(&ramus, INSULA_EPHEMERA, titulus);
}

/* linea l folii (spatiis finalibus omissis) == expectata */
interior b32
linea_est (
    constans TabulaCharacterum* t,
                           i32  l,
           constans character* expectata)
{
    i32 finis;
    i32 n;

    finis = t->latitudo;
    dum (finis > ZEPHYRUM && tabula_cellula(t, l, finis - I) == ' ')
    {
        finis--;
    }
    n = (i32)strlen(expectata);
    redde n == finis
        && memcmp(&tabula_cellula(t, l, ZEPHYRUM), expectata, (size_t)n)
           == ZEPHYRUM;
}

/* eventus muris in puncto (k, k) tabulae pictoris (spatium locale) */
interior vacuum
murem_mittere (
        Dispensator* d,
         ManusLudus* m,
    eventus_genus_t  genus,
                s32  k,
                s64  tempus)
{
     Componens* tabula;
       Punctum  p;
       Eventus  e;

    tabula = manus_ludus_invenire(m, "#tabula");
    CREDO_NON_NIHIL(tabula);
    si (!tabula)
    {
        redde;
    }
    p.x  = k;
    p.y  = k;
    p    = manus_ludus_ad_schirmum(m, tabula, p);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus              = genus;
    e.tempus             = tempus;
    e.datum.mus.x        = p.x;
    e.datum.mus.y        = p.y;
    e.datum.mus.botton   = MUS_SINISTER;
    e.datum.mus.pressio  = EVENTUS_PRESSIO_IGNOTA;
    dispensator_tractare(d, &e);
}

interior b32
focus_est (
           Dispensator* d,
    constans character* id)
{
    redde chorda_aequalis_literis(dispensator_focus(d), id);
}

s32 principale (vacuum)
{
           Volumen* vol;
             Vicus* v;
       Dispensator* d;
        ManusLudus* m;
             Motus* motus;
    ScribaMontatio* sm;
    PictorMontatio* pm;

    piscina = piscina_generare_dynamicum("probatio_vicus_commutatio",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_vicus_commutatio");

    v = vicus_creare(piscina, intern, vol, NIHIL, CDLXXX, CDLXXX);
    CREDO_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "scriba",
        magnitudo(ScribaMontatio),
        scribam_montare, scribam_describere, NIHIL));
    CREDO_VERUM(vicus_genus_addere(v, "pictor",
        magnitudo(PictorMontatio),
        pictorem_montare, pictorem_describere, NIHIL));
    CREDO_VERUM(vicus_aperire(v,
        "<tabulae activa=\"s1\">"
        "<tabula id=\"s1\"><latus genus=\"scriba\"/></tabula>"
        "<tabula id=\"p1\"><latus genus=\"pictor\"/></tabula>"
        "</tabulae>"));
    sm = (ScribaMontatio*)vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_SINISTRUM)->montatio;
    pm = (PictorMontatio*)vicus_latus(vicus_tabula(v, I),
        VICUS_SINISTRUM)->montatio;
    d = dispensator_creare(piscina, intern, v->repo, vicus_actiones(v),
        vicus_componere, v, CCC);
    m      = manus_ludus_creare(piscina, d);
    motus  = dispensator_motus(d);
    vicus_dispensatorem_ligare(v, d);

    imprimere("\n--- I: ligatio - focus ex ramo activo ---\n");
    CREDO_VERUM(focus_est(d, "pagina"));
    CREDO_NIHIL(insula_attributum(v->repo, INSULA_EPHEMERA, "focus"));
    CREDO_VERUM(motus->gestus.status == (vacuum*)&sm->actiones_ctx);

    imprimere("\n--- II: commutatio insertionem effundit ---\n");
    CREDO_VERUM(manus_ludus_scribere(m, "isalve"));
    /* quies scribae (M ms) non elapsa: textus in gestu solum */
    CREDO_VERUM(linea_est(scriba_documentum_tabula(sm->doc), ZEPHYRUM,
        ""));
    CREDO_VERUM(vicus_activam_ponere(v, "p1"));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(sm->doc), ZEPHYRUM,
        "salve"));
    dispensator_recomponere(d);
    CREDO_VERUM(focus_est(d, "tabula"));
    CREDO_NIHIL(motus->gestus.status);

    imprimere("\n--- III: effusio pan/zoom in ramum activum ---\n");
    /* ictus finitus sordidam tollit; quies MEDIO ictu (mus tentus)
     * effusionem facit - in ramum pictoris, non radicem vici */
    dispensator_recomponere(d);
    murem_mittere(d, m, EVENTUS_MUS_DEPRESSUS, V, C * M);
    murem_mittere(d, m, EVENTUS_MUS_MOTUS, XV, C * M + XX);
    CREDO_VERUM(motus->sordida);
    dispensator_pulsare(d, C * M + M);
    CREDO_FALSUM(motus->sordida);
    CREDO_VERUM(valor_est(rami_attributum(v, "pictor",
        "p1_sinistrum_pictor", "pan_x"),
        "0"));
    CREDO_NIHIL(insula_attributum(v->repo, INSULA_EPHEMERA, "pan_x"));
    murem_mittere(d, m, EVENTUS_MUS_LIBERATUS, XV, C * M + M + XX);
    CREDO_VERUM(pictor_documentum_cursor(pm->doc) > ZEPHYRUM);

    imprimere("\n--- IV: reditus - focus et gestus scribae ---\n");
    CREDO_VERUM(vicus_activam_ponere(v, "s1"));
    dispensator_recomponere(d);
    CREDO_VERUM(focus_est(d, "pagina"));
    CREDO_VERUM(motus->gestus.status == (vacuum*)&sm->actiones_ctx);
    CREDO_VERUM(valor_est(rami_attributum(v, "pictor",
        "p1_sinistrum_pictor", "focus"),
        "tabula"));

    imprimere("\n--- V: focus in ramum, non radicem ---\n");
    dispensator_focus_ponere(d, chorda_ex_literis("status", piscina));
    CREDO_VERUM(valor_est(rami_attributum(v, "scriba",
        "s1_sinistrum_scriba", "focus"),
        "status"));
    CREDO_NIHIL(insula_attributum(v->repo, INSULA_EPHEMERA, "focus"));
    dispensator_focus_ponere(d, chorda_ex_literis("pagina", piscina));

    imprimere("\n--- VI: captura et ictus pendens abiciuntur ---\n");
    CREDO_VERUM(vicus_activam_ponere(v, "p1"));
    dispensator_recomponere(d);
    murem_mittere(d, m, EVENTUS_MUS_DEPRESSUS, V, CC * M);
    CREDO_VERUM(motus->captura.mensura > ZEPHYRUM);
    CREDO_VERUM(xar_numerus(motus->ictus_pendens) > ZEPHYRUM);
    CREDO_VERUM(vicus_activam_ponere(v, "s1"));
    CREDO_AEQUALIS_I32(motus->captura.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(motus->ictus_pendens), ZEPHYRUM);
    CREDO_FALSUM(insula_mendacium(v->repo));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
