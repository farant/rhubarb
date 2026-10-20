/* probatio_vicus.c - repositorium hospitis cum tabulis
 * (insula-rami-plan T1b)
 *
 * Genera applicationum a principali (hic: probatione) registrantur;
 * vicus nullam applicationem novit. vicus-latera S2a: tabula = par
 * laterum. Volumen novum: dispositio data (acervus absens = sinistrum
 * iteratum), latera montata, dispositio scripta; focus et documentum
 * per reapertionem servantur; registra in spatiis laterum; latus
 * generis ignoti praeteritur cum causa; plures quam X tabulae non
 * leguntur. S7a: radix ex manifesto infixo (genus vicus) - canon et
 * domini (activa: vicus.radix; lectiones: horologium). */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "stml.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior b32
scribam_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)argumentum;
    (vacuum)ctx;
    redde scriba_montare((ScribaMontatio*)sedes, p, in, v, r, id,
                         latitudo, altitudo, NIHIL, NIHIL);
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)argumentum;
    (vacuum)ctx;
    redde pictor_montare((PictorMontatio*)sedes, p, in, v, r, id,
                         latitudo, altitudo, NIHIL);
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

#define DISPOSITIO                                                    \
    "<tabulae activa=\"s1\">"                                         \
    "<tabula id=\"s1\"><latus genus=\"scriba\"/>"                     \
    "<acervus><latus genus=\"pictor\"/></acervus></tabula>"           \
    "<tabula id=\"p1\"><latus genus=\"pictor\"/></tabula>"            \
    "</tabulae>"

interior Vicus*
vicum_aperire_cum (
                Volumen* vol,
    constans character* dispositio,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    Vicus* v;

    v = vicus_creare(piscina, intern, vol, CDLXXX, CDLXXX);
    CREDO_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "scriba",
        magnitudo(ScribaMontatio),
        scribam_montare, scribam_describere, NIHIL));
    CREDO_VERUM(vicus_genus_addere(v, "pictor",
        magnitudo(PictorMontatio),
        pictorem_montare, pictorem_describere, NIHIL));
    CREDO_VERUM(vicus_aperire(v, dispositio));
    redde v;
}

interior Vicus*
vicum_aperire (
                Volumen* vol,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    redde vicum_aperire_cum(vol, DISPOSITIO, piscina, intern);
}

interior b32
latus_est (
    constans VicusLatus* l,
     constans character* id,
     constans character* genus)
{
    redde l && l->montata && chorda_aequalis_literis(l->id, id)
        && chorda_aequalis_literis(l->genus, genus);
}

interior vacuum
ignotum_ponere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(nodus, p, in, "ignotum", "1");
}

/* S7a: campum radicis ponere */
nomen structura {
    constans character* titulus;
    constans character* valor;
} CampusPonendus;

interior vacuum
campum_ponere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans CampusPonendus* c;

    c = (constans CampusPonendus*)ctx;
    insula_attributum_ponere(nodus, p, in, c->titulus, c->valor);
}

interior chorda
lit (
    constans character* s,
               Piscina* piscina)
{
    redde chorda_ex_literis(s, piscina);
}

/* thema semel ante sectiones (ut olim ante probationes) */
interior vacuum
_suitam_parare (
    CredoContextus* contextus)
{
    (vacuum)contextus;
    thema_initiare();
}

/* I-V volumen UNUM per gradus transeunt: IV reaperit quod I et II
 * scripserunt (focus, documentum), V dispositionem eius mutat - ergo
 * sectio una cum CREDO_GRADUS, non sectiones separatae */
interior vacuum
_sectio_prima (
    CredoContextus* contextus)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
                  Vicus* v;
            VicusTabula* t;
         ScribaMontatio* sm;
            InsulaRamus  radix;
                 chorda  index;
                    b32  inventum;
                    s64  acta_ante;

    piscina  = contextus->piscina;
    intern   = internamentum_creare(piscina);
    vol      = volumen_temporarium(piscina, "probatio_vicus");

    CREDO_GRADUS("I: volumen novum - dispositio data");
    v = vicum_aperire(vol, piscina, intern);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), II);
    t = vicus_tabula(v, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(t->id, "s1");
    CREDO_VERUM(latus_est(vicus_latus(t, VICUS_SINISTRUM),
        "s1_sinistrum_scriba", "scriba"));
    CREDO_VERUM(latus_est(vicus_latus(t, VICUS_DEXTRUM),
        "s1_dextrum_pictor", "pictor"));
    /* acervus absens: sinistrum iteratum (decisio VIII) */
    t = vicus_tabula(v, I);
    CREDO_VERUM(latus_est(vicus_latus(t, VICUS_SINISTRUM),
        "p1_sinistrum_pictor", "pictor"));
    CREDO_VERUM(latus_est(vicus_latus(t, VICUS_DEXTRUM),
        "p1_dextrum_pictor", "pictor"));
    CREDO_AEQUALIS_I32(xar_numerus(t->acervus), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_activa(v)->id, "s1");
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_tabula(v, ZEPHYRUM), VICUS_SINISTRUM));
    /* rami: quattuor montationes */
    CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(v->repo)),
        IV);
    /* dispositio scripta, forma nova (S3c: id cuiusque lateris) */
    index = volumen_praesens_legere(vol, lit("vicus/latera", piscina),
        piscina,
        &inventum);
    CREDO_VERUM(inventum);
    CREDO_VERUM(chorda_continet(index, lit(
        "<tabula id=\"p1\" focus=\"sinistrum\">"
        "<latus genus=\"pictor\" id=\"p1_sinistrum_pictor\"/>"
        "<acervus><latus genus=\"pictor\" id=\"p1_dextrum_pictor\"/>"
        "</acervus></tabula>", piscina)));

    CREDO_GRADUS("II: focus et documentum servantur");
    CREDO_FALSUM(vicus_focum_ponere(v, II));
    acta_ante = volumen_summa_actorum(vol);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    /* D1: dispositio in praesentia - diarium non crescit */
    index = volumen_praesens_legere(vol, lit("vicus/latera", piscina),
        piscina, &inventum);
    CREDO_VERUM(chorda_continet(index, lit(
        "<tabula id=\"s1\" focus=\"dextrum\">", piscina)));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), acta_ante);
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_tabula(v, ZEPHYRUM), VICUS_DEXTRUM));
    sm = (ScribaMontatio*)vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_SINISTRUM)->montatio;
    tabula_cellula(&sm->actiones_ctx.laboris, ZEPHYRUM, ZEPHYRUM) = 'q';
    CREDO_VERUM(scriba_documentum_committere(sm->doc,
        &sm->actiones_ctx.laboris) > ZEPHYRUM);

    CREDO_GRADUS("III: canon hospitis");
    radix = insula_ramus_radix(v->repo);
    CREDO_FALSUM(mutare_ramum(&radix, ignotum_ponere,
        NIHIL));
    /* S7a: domini radicis ex manifesto (apps/vicus/organum.stml) */
    {
        CampusPonendus  c;
                chorda* a;

        c.titulus  = "activa";
        c.valor    = "p1";
        CREDO_FALSUM(mutare_ramum(&radix, campum_ponere, &c));
        insula_scriptorem_ponere(v->repo, lit("horologium", piscina));
        CREDO_FALSUM(mutare_ramum(&radix, campum_ponere, &c));
        c.titulus  = "tempus_hora";
        c.valor    = "7";
        insula_scriptorem_ponere(v->repo, lit("vicus.radix", piscina));
        CREDO_FALSUM(mutare_ramum(&radix, campum_ponere, &c));
        insula_scriptorem_ponere(v->repo, lit("horologium", piscina));
        CREDO_VERUM(mutare_ramum(&radix, campum_ponere, &c));
        insula_scriptorem_ponere(v->repo, lit("", piscina));
        /* hospes ipse ut dominus scribit */
        CREDO_VERUM(vicus_activam_ponere(v, "p1"));
        a = insula_attributum(v->repo, "activa");
        CREDO_NON_NIHIL(a);
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "p1");
        a = insula_attributum(v->repo, "prior");
        CREDO_NON_NIHIL(a);
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "s1");
        CREDO_VERUM(vicus_activam_ponere(v, "s1"));
    }

    CREDO_GRADUS("IV: reapertio - dispositio, focus, documenta");
    v = vicum_aperire(vol, piscina, intern);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), II);
    CREDO_AEQUALIS_I32(vicus_tabula(v, ZEPHYRUM)->focus, VICUS_DEXTRUM);
    sm = (ScribaMontatio*)vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_SINISTRUM)->montatio;
    CREDO_VERUM(tabula_cellula(scriba_documentum_tabula(sm->doc),
        ZEPHYRUM, ZEPHYRUM) == 'q');

    CREDO_GRADUS("IVb: registra in spatiis laterum");
    {
               ActioFn  fn;
                vacuum* ctx;
              FiguraFn  ff;
                vacuum* fc;
        PictorMontatio* pm;

        CREDO_VERUM(actio_invenire_in_spatio(vicus_actiones(v),
            lit("scriba:s1_sinistrum_scriba", piscina),
            lit("pagina.clavis", piscina),
            &fn,
            &ctx));
        CREDO_VERUM(ctx == &sm->actiones_ctx);
        CREDO_VERUM(actio_invenire_in_spatio(vicus_actiones(v),
            lit("pictor:s1_dextrum_pictor", piscina),
            lit("penicillus.ictus", piscina),
            &fn,
            &ctx));
        /* nomen nudum (hospes) applicationes non videt */
        CREDO_FALSUM(actio_invenire(vicus_actiones(v),
            lit("pagina.clavis", piscina), &fn, &ctx));
        CREDO_VERUM(figura_invenire_in_spatio(vicus_figurae(v),
            lit("scriba:s1_sinistrum_scriba", piscina), PARTES_CAMPUS,
            ZEPHYRUM,
            &ff,
            &fc));
        CREDO_FALSUM(figura_invenire_in_spatio(vicus_figurae(v),
            lit("scriba:s1_sinistrum_scriba", piscina), PARTES_TABULA,
            ZEPHYRUM,
            &ff,
            &fc));
        /* fons imaginum: latus dextrum (pictor) respondet */
        pm = (PictorMontatio*)vicus_latus(vicus_tabula(v, ZEPHYRUM),
            VICUS_DEXTRUM)->montatio;
        CREDO_VERUM(vicus_imago_fons(
            pictor_documentum_sigillum_hex(pm->doc, piscina), v)
            == pictor_documentum_proiectio(pm->doc));
        /* commutatio: registra sequuntur */
        CREDO_VERUM(vicus_activam_ponere(v, "p1"));
        CREDO_VERUM(actio_invenire_in_spatio(vicus_actiones(v),
            lit("pictor:p1_sinistrum_pictor", piscina),
            lit("penicillus.ictus", piscina),
            &fn,
            &ctx));
        CREDO_FALSUM(actio_invenire_in_spatio(vicus_actiones(v),
            lit("scriba:s1_sinistrum_scriba", piscina),
            lit("pagina.clavis", piscina),
            &fn,
            &ctx));
        CREDO_VERUM(vicus_activam_ponere(v, "s1"));
    }

    CREDO_GRADUS("V: genus ignotum - latus praeteritum");
    CREDO_VERUM(volumen_praesens_ponere(vol, lit("vicus/latera",
        piscina), lit(
        "<tabulae activa=\"s1\">"
        "<tabula id=\"s1\"><latus genus=\"scriba\"/>"
        "<acervus><latus genus=\"lyra\"/></acervus></tabula>"
        "</tabulae>", piscina)));
    v = vicum_aperire(vol, piscina, intern);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), I);
    CREDO_FALSUM(vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_DEXTRUM)->montata);
    CREDO_VERUM(vicus_causa(v).mensura > ZEPHYRUM);
    CREDO_VERUM(vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_SINISTRUM)->montata);
    /* focus in latus non montatum recusatur */
    CREDO_FALSUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    volumen_claudere(vol);

}

interior vacuum
_sectio_secunda (
    CredoContextus* contextus)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol_decem;
                  Vicus* v;
                 chorda  d;
                    i32  k;

    piscina  = contextus->piscina;
    intern   = internamentum_creare(piscina);

    d = lit("<tabulae activa=\"1\">", piscina);
    per (k = I; k <= XI; k++)
    {
        d = chorda_concatenare(d, lit("<tabula id=\"", piscina),
            piscina);
        d = chorda_concatenare(d, chorda_ex_s32((s32)k, piscina),
            piscina);
        d = chorda_concatenare(d, lit("\"><latus genus=\"pictor\"/>"
            "</tabula>", piscina), piscina);
    }
    d = chorda_concatenare(d, lit("</tabulae>", piscina), piscina);
    vol_decem = volumen_temporarium(piscina,
        "probatio_vicus_decem");
    v = vicum_aperire_cum(vol_decem, chorda_ut_cstr(d, piscina),
        piscina, intern);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), VICUS_TABULAE);
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_tabula(v, IX)->id, "10");
    volumen_claudere(vol_decem);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I-V: volumen unum per gradus", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "VI: plures quam decem tabulae: X leguntur", _sectio_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("vicus", SECTIONES, _suitam_parare,
        NIHIL);
}
