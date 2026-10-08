/* probatio_vicus.c - repositorium hospitis cum tabulis
 * (insula-rami-plan T1b)
 *
 * Genera applicationum a principali (hic: probatione) registrantur;
 * vicus nullam applicationem novit. vicus-latera S2a: tabula = par
 * laterum. Volumen novum: dispositio data (acervus absens = sinistrum
 * iteratum), latera montata, dispositio scripta; focus et documentum
 * per reapertionem servantur; registra in spatiis laterum; latus
 * generis ignoti praeteritur cum causa; plures quam X tabulae non
 * leguntur. */
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
                    i32  altitudo)
{
    redde scriba_montare((ScribaMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
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
                    i32  altitudo)
{
    redde pictor_montare((PictorMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
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
    constans character* dispositio)
{
    Vicus* v;

    v = vicus_creare(piscina, intern, vol, NIHIL, CDLXXX, CDLXXX);
    CREDO_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "scriba",
        magnitudo(ScribaMontatio),
        scribam_montare, scribam_describere));
    CREDO_VERUM(vicus_genus_addere(v, "pictor",
        magnitudo(PictorMontatio),
        pictorem_montare, pictorem_describere));
    CREDO_VERUM(vicus_aperire(v, dispositio));
    redde v;
}

interior Vicus*
vicum_aperire (
    Volumen* vol)
{
    redde vicum_aperire_cum(vol, DISPOSITIO);
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

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

s32 principale (vacuum)
{
             Volumen* vol;
               Vicus* v;
         VicusTabula* t;
      ScribaMontatio* sm;
         InsulaRamus  radix;
              chorda  index;
                 b32  inventum;

    piscina = piscina_generare_dynamicum("probatio_vicus", LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_vicus");

    imprimere("\n--- I: volumen novum - dispositio data ---\n");
    v = vicum_aperire(vol);
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
    CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(v->repo,
        INSULA_DURABILIS)), IV);
    /* dispositio scripta, forma nova */
    index = volumen_plagulam_promere(vol, lit("vicus/latera"), piscina,
        &inventum);
    CREDO_VERUM(inventum);
    CREDO_VERUM(chorda_continet(index, lit(
        "<tabula id=\"p1\" focus=\"sinistrum\">"
        "<latus genus=\"pictor\"/>"
        "<acervus><latus genus=\"pictor\"/></acervus></tabula>")));

    imprimere("\n--- II: focus et documentum servantur ---\n");
    CREDO_FALSUM(vicus_focum_ponere(v, II));
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_tabula(v, ZEPHYRUM), VICUS_DEXTRUM));
    sm = (ScribaMontatio*)vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_SINISTRUM)->montatio;
    tabula_cellula(&sm->actiones_ctx.laboris, ZEPHYRUM, ZEPHYRUM) = 'q';
    CREDO_VERUM(scriba_documentum_committere(sm->doc,
        &sm->actiones_ctx.laboris) > ZEPHYRUM);

    imprimere("\n--- III: canon hospitis ---\n");
    radix = insula_ramus_radix(v->repo);
    CREDO_FALSUM(mutare_ramum(&radix, INSULA_EPHEMERA, ignotum_ponere,
        NIHIL));

    imprimere("\n--- IV: reapertio - dispositio, focus, documenta\n");
    v = vicum_aperire(vol);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), II);
    CREDO_AEQUALIS_I32(vicus_tabula(v, ZEPHYRUM)->focus, VICUS_DEXTRUM);
    sm = (ScribaMontatio*)vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_SINISTRUM)->montatio;
    CREDO_VERUM(tabula_cellula(scriba_documentum_tabula(sm->doc),
        ZEPHYRUM, ZEPHYRUM) == 'q');

    imprimere("\n--- IVb: registra in spatiis laterum ---\n");
    {
               ActioFn  fn;
                vacuum* ctx;
              FiguraFn  ff;
                vacuum* fc;
        PictorMontatio* pm;

        CREDO_VERUM(actio_invenire_in_spatio(vicus_actiones(v),
            lit("s1_sinistrum_scriba"), lit("pagina.clavis"), &fn,
            &ctx));
        CREDO_VERUM(ctx == &sm->actiones_ctx);
        CREDO_VERUM(actio_invenire_in_spatio(vicus_actiones(v),
            lit("s1_dextrum_pictor"), lit("penicillus.ictus"), &fn,
            &ctx));
        /* nomen nudum (hospes) applicationes non videt */
        CREDO_FALSUM(actio_invenire(vicus_actiones(v),
            lit("pagina.clavis"), &fn, &ctx));
        CREDO_VERUM(figura_invenire_in_spatio(vicus_figurae(v),
            lit("s1_sinistrum_scriba"), PARTES_CAMPUS, ZEPHYRUM, &ff,
            &fc));
        CREDO_FALSUM(figura_invenire_in_spatio(vicus_figurae(v),
            lit("s1_sinistrum_scriba"), PARTES_TABULA, ZEPHYRUM, &ff,
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
            lit("p1_sinistrum_pictor"), lit("penicillus.ictus"), &fn,
            &ctx));
        CREDO_FALSUM(actio_invenire_in_spatio(vicus_actiones(v),
            lit("s1_sinistrum_scriba"), lit("pagina.clavis"), &fn,
            &ctx));
        CREDO_VERUM(vicus_activam_ponere(v, "s1"));
    }

    imprimere("\n--- V: genus ignotum - latus praeteritum ---\n");
    CREDO_VERUM(volumen_plagulam_condere(vol, lit("vicus/latera"), lit(
        "<tabulae activa=\"s1\">"
        "<tabula id=\"s1\"><latus genus=\"scriba\"/>"
        "<acervus><latus genus=\"lyra\"/></acervus></tabula>"
        "</tabulae>"), "probatio"));
    v = vicum_aperire(vol);
    CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), I);
    CREDO_FALSUM(vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_DEXTRUM)->montata);
    CREDO_VERUM(vicus_causa(v).mensura > ZEPHYRUM);
    CREDO_VERUM(vicus_latus(vicus_tabula(v, ZEPHYRUM),
        VICUS_SINISTRUM)->montata);
    /* focus in latus non montatum recusatur */
    CREDO_FALSUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    volumen_claudere(vol);

    imprimere("\n--- VI: plures quam decem tabulae: X leguntur ---\n");
    {
        Volumen* vol_decem;
         chorda  d;
            i32  k;

        d = lit("<tabulae activa=\"1\">");
        per (k = I; k <= XI; k++)
        {
            d = chorda_concatenare(d, lit("<tabula id=\""), piscina);
            d = chorda_concatenare(d, chorda_ex_s32((s32)k, piscina),
                piscina);
            d = chorda_concatenare(d, lit("\"><latus genus=\"pictor\"/>"
                "</tabula>"), piscina);
        }
        d = chorda_concatenare(d, lit("</tabulae>"), piscina);
        vol_decem = volumen_temporarium(piscina,
            "probatio_vicus_decem");
        v = vicum_aperire_cum(vol_decem, chorda_ut_cstr(d, piscina));
        CREDO_AEQUALIS_I32(vicus_numerus_tabularum(v), VICUS_TABULAE);
        CREDO_CHORDA_AEQUALIS_LITERIS(vicus_tabula(v, IX)->id, "10");
        volumen_claudere(vol_decem);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
